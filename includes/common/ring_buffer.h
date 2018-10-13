/**************************************************************************//**
 * @file      ring_buffer.h
 * @brief    Functions prototypes for a decent FIFO ring buffer.
 * @date     December/2016
 * @author   Artur Rodrigues (securitoys at outlook.com)
 *
 ******************************************************************************/


#ifndef __RINGBUFF_BUFFER_H_
#define __RINGBUFF_BUFFER_H_

#include "api_config.h"


/**
 * @addtogroup COMMON_GROUP
 * @{
 */

/**
 * Ring buffer structure
 */
typedef struct
{
    void *data;     ///< Data in buffer
    uint32_t head;  ///< First element ID
    uint32_t tail;  ///< Last element ID
    uint32_t sz;    ///< Buffer size (bytes)
    uint8_t full    :1; ///< flag indicating full
    uint8_t init    :1; ///< flag indicating initialized
    uint8_t eSz     :6; ///< size of a single element(bytes)
}ringbuff_t;

/**
 * Initializes the structure
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 * @param[in]       pBuff        : Pointer to a RAM location
 * @param[in]       pSz         : Size of RAM location
 * @param[in]       pElementSz  : Size element of a single element
 *
 * @return The number of elements this ring buffer holds
 */
uint32_t RingBuff_Init(ringbuff_t *pRb, void * const pBuff, uint32_t pSz, uint8_t pElSz );

/**
 * Write an element in the ring  buffer
 *
 * @note In case FIFO is full, you can overwrite the older element by using a parameter.
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 * @param[in]       pEl         : Pointer to the input element
 * @param[in]       pOw         : If the buffer is full, #true overwrites the oldest element.
 * #false does not overwrite the oldest element.
 *
 * @warning the input pointer must be capable of informing a full-sized element, or a buffer
 * overflow may happen.
 *
 * @return number of elements inserted
 */
uint32_t RingBuff_WrOne(ringbuff_t *pRb, void * const pEl, bool_t pOw );

/**
 * Write many elements in the ring buffer
 *
 * @note In case FIFO is full, you can overwrite the older element by using a parameter.
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 * @param[in,out]   pWrBuff      : Pointer to an element vector
 * @param[in]       pN          : Number of elements to insert
 * @param[in]       pOw         : If the buffer is full, #true overwrites the oldest element.
 * #false does not overwrite the oldest element.
 *
 * @warning the input pointer must be capable of informing a full-sized element, or a buffer
 * overflow may happen.
 *
 * @return number of elements inserted
 */
//FIXME esta chamando varias vezes a função de escrever um só! corrigir!
uint32_t RingBuff_WrMany(ringbuff_t *pRb, const void * const pWrBuff, uint32_t pN, bool_t pOw );

/**
 * Gets the next element from the ring buffer by its pointer.
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 * @param[in,out]   pEl         : Pointer to point to the RAM where the element is.
 * It can be #NULL, so the element will be simply discarded.
 *
 * @note THERE WILL BE NO MEMORY COPY
 *
 * @return number of elements read
 *
 */
uint32_t RingBuff_RdOnePtr(ringbuff_t *pRb, void **pEl);

/**
 * Gets the next element from the ring buffer.
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 * @param[in,out]   pEl         : Pointer to the RAM where the element is.
 * It can be #NULL, so the element will be simply discarded.
 *
 * @note The element will be properly copied.
 *
 * @return number of elements read
 *
 */
uint32_t RingBuff_RdOne(ringbuff_t *pRb, void *pEl);

/**
 * Gets N elements from the ring buffer, leaving them in the buffer.
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 * @param[in,out]   pRdBuff      : Pointer to an element vector. Can be #NULL
 * @param[in]       pN          : Number of elements to read
 *
 * @return number of elements read
 */
uint32_t RingBuff_RdAdvance(ringbuff_t *pRb, void *pRdBuff, uint32_t pN);

/**
 * Gets N elements from the ring buffer
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 * @param[in,out]   pRdBuff      : Pointer to an element vector. If #NULL,
 * no memory copy is made and the elements will be discarded.
 * @param[in]       pN          : Number of elements to read
 *
 * @return number of elements read
 */
uint32_t RingBuff_RdMany(ringbuff_t *pRb, void *pRdBuff, uint32_t pN);

/**
 * Discards N elements from the ring buffer
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 * @param[in]       pN          : Number of elements to read
 *
 * @return number of elements discarded
 */
uint32_t RingBuff_Discard(ringbuff_t *pRb, uint32_t pN);

/**
 * Discards all elements from the ring buffer
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 */
void RingBuff_Flush(ringbuff_t *pRb);

/**
 * Gets number of elements in the ring buffer
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 *
 * @return number of elements stored
 */
uint32_t RingBuff_Used(ringbuff_t *pRb);

/**
 * Gets number of free slots
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 *
 * @return number of free slots
 */
uint32_t RingBuff_Free(ringbuff_t *pRb);

/**
 * Checks if empty
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 *
 * @return #true if there's no element
 */
bool_t RingBuff_IsEmpty(ringbuff_t *pRb);

/**
 * Checks if full
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 *
 * @return #true if already full
 */
bool_t RingBuff_IsFull(ringbuff_t *pRb);

/**
 * Gets elements max size
 *
 * @param[in,out]   pRb         : Pointer to the structure #ringbuff_t
 *
 * @return maximum number of elements
 */
uint32_t RingBuff_Sz(ringbuff_t *pRb);


/**
 * @}
 */

#endif /* __RINGBUFF_BUFFER_H_ */
