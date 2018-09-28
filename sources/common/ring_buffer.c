
#include <string.h>
#include "common/ring_buffer.h"


#define RINGBUFF_SZ(__rb)    ( (uint32_t)((__rb)->sz) / (uint32_t)((__rb)->eSz) )

static INLINE uint32_t _ringbuff_updateIndex( uint32_t pId, uint32_t pUpdateSz, uint32_t pMaxSz )
{
    pId += pUpdateSz;

    if( pId >= pMaxSz )
    {
        pId -= pMaxSz;
    }

    return pId;
}

uint32_t RingBuff_Init(ringbuff_t *pRb, void * const pBuff, uint32_t pSz, uint8_t pElSz )
{
    if( pRb == NULL )
    {
        return 0;
    }
    if( pBuff == NULL )
    {
        return 0;
    }
    if( pSz < pElSz )
    {
        return 0;
    }
    if( pSz == 0 )
    {
        return 0;
    }
    if( pElSz == 0 )
    {
        return 0;
    }

    pRb->data = pBuff;
    pRb->sz = (pSz - (pSz%pElSz));  //unaliged elements are NOT accepted
    pRb->eSz = pElSz;
    pRb->head = pRb->tail = pRb->full = 0;
    pRb->init = true;
    return pRb->sz/pRb->eSz;
}


uint32_t RingBuff_WrOne(ringbuff_t *pRb, void * const pEl, bool_t pOw )
{
    uint8_t *ptr;

    if( pRb == NULL )
    {
        return 0;
    }
    if( !pRb->init )
    {
        return 0;
    }

    if( RingBuff_IsFull(pRb) )
    {
        if( !pOw )
        {
            //não adiciona o item
            return 0;
        }
        else
        {
            //sobrescreve o início do buffer
            pRb->head = _ringbuff_updateIndex( pRb->head, pRb->eSz, pRb->sz );
        }
    }

    ptr = &((uint8_t*)pRb->data)[pRb->tail];

    memcpy(ptr, pEl, pRb->eSz);

    pRb->tail = _ringbuff_updateIndex(pRb->tail, pRb->eSz, pRb->sz);
    pRb->full = pRb->head == pRb->tail;

    return 1;
}


uint32_t RingBuff_WrMany(ringbuff_t *pRb, const void * const pWrBuff, uint32_t pN, bool_t pOw )
{
    uint32_t ii;
    uint8_t *ptr = (uint8_t *)pWrBuff;

    if( pWrBuff == NULL )
    {
        return 0;
    }
    if( pRb == NULL )
    {
        return 0;
    }
    if( !pRb->init )
    {
        return 0;
    }

    for(ii=0; ii < pN; ii++)
    {
        if( 0 == RingBuff_WrOne(pRb, &ptr[(ii*pRb->eSz)], pOw) )
        {
            break;
        }
    }
    return ii;

}


uint32_t RingBuff_WrMany2(ringbuff_t *pRb, const void *pWrBuff, uint32_t pN, bool_t pOw )
{
    uint8_t *ptr = (uint8_t *)pWrBuff;
    uint32_t ii = 0;

    if( !pOw )
    {
        //não adiciona além da conta
        pN = MIN( RingBuff_Free(pRb), pN );
    }

    while(pN--)
    {
        if( RingBuff_WrOne( pRb, ptr, pOw ) )
        {
            ptr = (uint8_t*)ptr + pRb->eSz;
            ii++;
        }
        else
        {
            break;
        }
    }
    return ii;
}


uint32_t RingBuff_RdOnePtr(ringbuff_t *pRb, void **pEl)
{
    uint8_t *ptr;

    if( pRb == NULL )
    {
        return 0;
    }
    if( RingBuff_IsEmpty(pRb) )
    {
        return 0;
    }

    ptr = &((uint8_t*)pRb->data)[pRb->head];

    pRb->head = _ringbuff_updateIndex(pRb->head, pRb->eSz, pRb->sz);
    pRb->full = 0;

    if( pEl == NULL )
    {
    }
    else
    {
        *pEl = (void *)ptr;
    }

    return 1;
}


uint32_t RingBuff_RdOne(ringbuff_t *pRb, void *pEl)
{
    uint8_t *ptr;

    if( pRb == NULL )
    {
        return 0;
    }
    if( RingBuff_IsEmpty(pRb) )
    {
        return 0;
    }

    ptr = &((uint8_t*)pRb->data)[pRb->head];

    pRb->head = _ringbuff_updateIndex(pRb->head, pRb->eSz, pRb->sz);
    pRb->full = 0;

    if( pEl == NULL )
    {
    }
    else
    {
        memcpy(pEl, ptr, pRb->eSz);
    }

    return 1;
}


uint32_t RingBuff_RdAdvance(ringbuff_t *pRb, void *pRdBuff, uint32_t pN)
{
    ringbuff_t rb;
    memcpy(&rb, pRb, sizeof(rb));
    return RingBuff_RdMany( &rb, pRdBuff, pN );
}


uint32_t RingBuff_RdMany(ringbuff_t *pRb, void *pRdBuff, uint32_t pN)
{
    uint32_t result;
    uint8_t *ptr = pRdBuff;
    uint32_t cpSz;

    if( pRdBuff == NULL )
    {
        return RingBuff_Discard( pRb, pN );
    }

    if( pRb == NULL )
    {
        return 0;
    }

    if( !pRb->init )
    {
        return 0;
    }

    result = pN = MIN( pN, RingBuff_Used(pRb) );

    //segmenta em duas partes: até o final do buffer e então do início até o meio.
    // assim: h = head t = tail
    //
    //  |=====t---------------------h=======| pega o tail até o final
    //  |h====t-----------------------------| pega o início até o head
    //  |----ht-----------------------------|

    if( pRb->head + (pRb->eSz * pN) > pRb->sz )
    {
        ptr = &((uint8_t*)pRb->data)[pRb->head];

        cpSz = pRb->sz - pRb->head;

        memcpy(pRdBuff, ptr, cpSz);

        //por isso o buffer precisa estar alinhado!
        pRb->head = _ringbuff_updateIndex(pRb->head, cpSz, pRb->sz );
        pRb->full = 0;

        pRdBuff = &(((uint8_t*)pRdBuff)[cpSz]);

        //número de elementos retirados:
        pN -= (cpSz/pRb->eSz);
    }

    ptr = &((uint8_t*)pRb->data)[pRb->head];

    cpSz = (pN*pRb->eSz);

    memcpy(pRdBuff, ptr, cpSz);

    pRb->head = _ringbuff_updateIndex(pRb->head, cpSz, pRb->sz );
    if( pN > 0 )
    {
        pRb->full = 0;
    }

    return result;
}


uint32_t RingBuff_Discard(ringbuff_t *pRb, uint32_t pN)
{

    if( pRb == NULL )
    {
        return 0;
    }

    pN = MIN( pN, RingBuff_Used(pRb) );

    pRb->head = _ringbuff_updateIndex(pRb->head, pN*pRb->eSz, pRb->sz );
    if( pN > 0 )
    {
        pRb->full = 0;
    }

    return pN;

}


void RingBuff_Flush(ringbuff_t *pRb)
{
    if( pRb == NULL )
    {
        return;
    }

    pRb->head = pRb->tail = pRb->full = 0;

    return;
}

uint32_t RingBuff_Used(ringbuff_t *pRb)
{
    if( pRb == NULL )
    {
        return 0;
    }
    if( !pRb->init )
    {
        return 0;
    }

    if( (pRb->head == pRb->tail) && !(pRb->full) )
    {
        return 0;
    }
    else if(pRb->tail > pRb->head)
    {
        return ((pRb->tail - pRb->head) / pRb->eSz);
    }
    else
    {
        return ((pRb->sz - (pRb->head - pRb->tail)) / pRb->eSz);
    }
}

uint32_t RingBuff_Free(ringbuff_t *pRb)
{
    if( pRb == NULL )
    {
        return 0;
    }
    if( !pRb->init )
    {
        return 0;
    }

    return RINGBUFF_SZ(pRb) - RingBuff_Used(pRb);
}

bool_t RingBuff_IsEmpty(ringbuff_t *pRb)
{
    return (bool_t)(RingBuff_Used(pRb) == 0);
}

bool_t RingBuff_IsFull(ringbuff_t *pRb)
{
    return (bool_t)(RingBuff_Sz(pRb) == RingBuff_Used(pRb));
}

uint32_t RingBuff_Sz(ringbuff_t *pRb)
{
    if( pRb == NULL )
    {
        return 0;
    }
    else if( !pRb->init )
    {
        return 0;
    }
    else
    {
        return RINGBUFF_SZ(pRb);
    }

}

