/**************************************************************************//**
 * @file      pool.h
 * @brief    Functions prototypes for an entropy pool.
 * @date     January/2017
 * @author   Artur Rodrigues (securitoys at outlook.com)
 *
 ******************************************************************************/

#ifndef INCLUDES_COMMON_POOL_H_
#define INCLUDES_COMMON_POOL_H_

#include "api_config.h"

typedef struct
{
    struct
    {
        void * d;
        uint32_t head;
        uint32_t tail;
        uint32_t sz;
    }buff;
    uint32_t entropy_cur;

    uint32_t full;
    enum
    {
        ePoolInitialized = BF_BIT(5),
    }f;
}entropy_pool_t;


/**
 * Initializes the structure
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 * @param[in]       pBuff        : Pointer to a RAM location
 * @param[in]       pSz         : Size of RAM location
 *
 * @return Number of bits the pool accepts.
 */
uint32_t EntropyPool_Init(entropy_pool_t *pPool, void * const pBuff, uint32_t pSz);

/**
 * Stores some bits on pool.
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 * @param[in]       pWord       : little-endian word containing entropy
 * @param[in]       pBits       : entropy bits. Max is 32
 *
 * @return Number of bits stored.
 */
uint32_t EntropyPool_Store32(entropy_pool_t *pPool, uint32_t pWord, uint8_t pBits);

/**
 * Pool size, in bytes.
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 *
 * @return Number of bytes the pool accepts.
 */
uint32_t EntropyPool_Sz(entropy_pool_t *pPool);

/**
 * Check if full
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 *
 * @return #true if full
 */
bool_t EntropyPool_IsFull(entropy_pool_t *pPool);

/**
 * Check if empty
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 *
 * @return #true if empty
 */
bool_t EntropyPool_IsEmpty(entropy_pool_t *pPool);

/**
 * Return entropy stored, in bytes
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 *
 * @return #true if empty
 */
uint32_t EntropyPool_Used(entropy_pool_t *pPool);

/**
 * Discards all entropy
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 *
 */
void EntropyPool_Flush(entropy_pool_t *pPool);

/**
 * Discard number of entropy, in bytes
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 * @param[in]       pSz         : Number of bytes to discard from the pool
 *
 * @return  Number of bytes discarded
 */
uint32_t EntropyPool_Discard(entropy_pool_t *pPool, uint32_t pSz);

/**
 * Reads buffer of entropy
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 * @param[in]       pBuff        : Pointer to a RAM location
 * @param[in]       pSz         : Number of bytes to discard from the pool
 *
 * @return  Number of bytes discarded
 */
uint32_t EntropyPool_RdMany(entropy_pool_t *pPool, void *pBuff, uint32_t pSz);

/**
 * Reads 1 byte of entropy
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 * @param[in]       pEl         : Pointer to a RAM location
 *
 * @return  Number of bytes read
 */
uint32_t EntropyPool_RdOne(entropy_pool_t *pPool, void *pEl);

/**
 * Reads 1 byte of entropy
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 * @param[in,out]   pEl         : Pointer to the pointer to the byte containing the entropy
 *
 * @return  Number of bytes read
 */
uint32_t EntropyPool_RdOnePtr(entropy_pool_t *pPool, void **pEl);

/**
 * Checks maximum bits allowed on this pool
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 *
 * @return  Number of bits the pool can hold
 */
uint32_t EntropyPool_BitsMax(entropy_pool_t *pPool);

/**
 * Checks number of bits stored
 *
 * @param[in,out]   pPool       : Pointer to the structure #entropy_pool_t
 *
 * @return  Number of bits stored on the pool
 */
uint32_t EntropyPool_Bits(entropy_pool_t *pPool);

#endif /* INCLUDES_COMMON_POOL_H_ */

