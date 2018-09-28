
#include "common/entropy_pool.h"

static uint32_t _epool_updateIndex(uint32_t pId, uint32_t pSzUpdate, uint32_t pSzMax)
{
    pId += pSzUpdate;

    if( pId >= pSzMax )
    {
        pId -= pSzMax;
    }

    return pId;
}

uint32_t EntropyPool_Init(entropy_pool_t *pPool, void * const pBuff, uint32_t pSz)
{
    if( pPool == NULL )
    {
        return 0;
    }
    if( pBuff == NULL )
    {
        return 0;
    }
    if( pSz == 0 )
    {
        return 0;
    }
    pPool->buff.d = pBuff;
    pPool->buff.sz = pSz;
    pPool->buff.head = pPool->buff.tail = pPool->full = pPool->entropy_cur = 0;
    pPool->f = ePoolInitialized;
    return pPool->buff.sz * 8;
}

uint32_t EntropyPool_Store32(entropy_pool_t *pPool, uint32_t pWord, uint8_t pBits)
{
    uint8_t * pool;
    uint8_t entropy;
    uint32_t posBit;

    if( pPool == NULL )
    {
        return 0;
    }
    if( (pPool->f & ePoolInitialized) == 0 )
    {
        return 0;
    }

    pBits %= (sizeof(pWord)*8);   //máximo é 32 bits
    entropy = (uint8_t)pBits;

    while( pBits-- )
    {
        pool = pPool->buff.d;
        posBit = pPool->entropy_cur % 8;

        pool[pPool->buff.tail] ^= (pWord & 1) << posBit;

        pWord >>= 1;
        pPool->entropy_cur += 1;

        if( (pPool->entropy_cur % 8) == 0 )
        {
            if( pPool->full )
            {
                //sobrescreve o início do buffer
                pPool->buff.head = _epool_updateIndex(pPool->buff.head, 1, pPool->buff.sz);
            }
            //incrementa o ponteiro
            pPool->buff.tail = _epool_updateIndex(pPool->buff.tail, 1, pPool->buff.sz);
            //verifica se encheu
            pPool->full = pPool->buff.tail == pPool->buff.head;
        }
    }

    return entropy;
}

uint32_t EntropyPool_Bits(entropy_pool_t *pPool)
{
    return EntropyPool_Used(pPool) * 8;
}

uint32_t EntropyPool_BitsMax(entropy_pool_t *pPool)
{
    return EntropyPool_Sz(pPool) * 8;
}

uint32_t EntropyPool_RdOnePtr(entropy_pool_t *pPool, void **pEl)
{
    uint8_t *ptr;

    if( pPool == NULL )
    {
        return 0;
    }
    if( EntropyPool_IsEmpty(pPool) )
    {
        return 0;
    }

    ptr = &((uint8_t*)pPool->buff.d)[pPool->buff.head];

    pPool->buff.head = _epool_updateIndex(pPool->buff.head, 1, pPool->buff.sz);
    pPool->full = 0;

    if( pEl == NULL )
    {
    }
    else
    {
        *pEl = (void *)ptr;
    }

    return 1;
}

uint32_t EntropyPool_RdOne(entropy_pool_t *pPool, void *pEl)
{
    uint8_t *ptr;

    if( pPool == NULL )
    {
        return 0;
    }
    if( EntropyPool_IsEmpty(pPool) )
    {
        return 0;
    }

    ptr = &((uint8_t*)pPool->buff.d)[pPool->buff.head];

    pPool->buff.head = _epool_updateIndex(pPool->buff.head, 1, pPool->buff.sz);
    pPool->full = 0;

    if( pEl == NULL )
    {
    }
    else
    {
        memcpy(pEl, ptr, 1);
    }

    return 1;
}

uint32_t EntropyPool_RdMany(entropy_pool_t *pPool, void *pBuff, uint32_t pSz)
{
    uint32_t result;
    uint8_t *ptr = pBuff;
    uint32_t cpSz;

    if( pBuff == NULL )
    {
        return EntropyPool_Discard( pPool, pSz );
    }

    if( pPool == NULL )
    {
        return 0;
    }

    if( ! (pPool->f & ePoolInitialized) )
    {
        return 0;
    }

    result = pSz = MIN( pSz, EntropyPool_Used(pPool) );

    //segmenta em duas partes: até o final do buffer e então do início até o meio.
    // assim: h = head t = tail
    //
    //  |=====t---------------------h=======| pega o tail até o final
    //  |h====t-----------------------------| pega o início até o head
    //  |----ht-----------------------------|

    if( pPool->buff.head + (1 * pSz) > pPool->buff.sz )
    {
        ptr = &((uint8_t*)pPool->buff.d)[pPool->buff.head];

        cpSz = pPool->buff.sz - pPool->buff.head;

        memcpy(pBuff, ptr, cpSz);

        //por isso o buffer precisa estar alinhado!
        pPool->buff.head = _epool_updateIndex(pPool->buff.head, cpSz, pPool->buff.sz );
        pPool->full = 0;

        pBuff = &(((uint8_t*)pBuff)[cpSz]);

        //número de elementos retirados:
        pSz -= (cpSz/1);
    }

    ptr = &((uint8_t*)pPool->buff.d)[pPool->buff.head];

    cpSz = (pSz*1);

    memcpy(pBuff, ptr, cpSz);

    pPool->buff.head = _epool_updateIndex(pPool->buff.head, cpSz, pPool->buff.sz );
    if( pSz > 0 )
    {
        pPool->full = 0;
    }

    return result;
}

uint32_t EntropyPool_Discard(entropy_pool_t *pPool, uint32_t pSz)
{

    if( pPool == NULL )
    {
        return 0;
    }

    pSz = MIN( pSz, EntropyPool_Used(pPool) );

    pPool->buff.head = _epool_updateIndex(pPool->buff.head, pSz, pPool->buff.sz );
    if( pSz > 0 )
    {
        pPool->full = 0;
    }

    return pSz;

}

void EntropyPool_Flush(entropy_pool_t *pPool)
{
    if( pPool == NULL )
    {
        return;
    }

    pPool->buff.head = pPool->buff.tail = pPool->full = 0;

    return;
}

uint32_t EntropyPool_Used(entropy_pool_t *pPool)
{
    if( pPool == NULL )
    {
        return 0;
    }
    if( ! (pPool->f & ePoolInitialized) )
    {
        return 0;
    }

    if( (pPool->buff.head == pPool->buff.tail) && !(pPool->full) )
    {
        return 0;
    }
    else if(pPool->buff.tail > pPool->buff.head)
    {
        return (pPool->buff.tail - pPool->buff.head);
    }
    else
    {
        return (pPool->buff.sz - (pPool->buff.head - pPool->buff.tail));
    }
}

bool_t EntropyPool_IsEmpty(entropy_pool_t *pPool)
{
    return (bool_t)(EntropyPool_Used(pPool) == 0);
}

bool_t EntropyPool_IsFull(entropy_pool_t *pPool)
{
    return (bool_t)(EntropyPool_Sz(pPool) == EntropyPool_Used(pPool));
}

uint32_t EntropyPool_Sz(entropy_pool_t *pPool)
{
    if( pPool == NULL )
    {
        return 0;
    }
    if( ! (pPool->f & ePoolInitialized) )
    {
        return 0;
    }
    else
    {
        return pPool->buff.sz;
    }

}




