/**************************************************************************//**
 * @file     general_defs.h
 * @brief    General definitions, types and macros. Very handy.
 * @date     October/2016
 * @author   Artur Rodrigues (securitoys at outlook.com)
 *
 ******************************************************************************/

#ifndef GENERAL_DEFS_H_
#define GENERAL_DEFS_H_

/**
 * @defgroup GENERAL_DEFS_GROUP General Macros and Defs
 * @{
 */

#if !defined(MAX)
#define MAX(a,b) \
       ({ __typeof__ (a) _a = (a); \
           __typeof__ (b) _b = (b); \
         (_a) > (_b) ? (_a) : (_b); })
#endif

#if !defined(MIN)
#define MIN(a,b) \
       ({ __typeof__ (a) _a = (a); \
           __typeof__ (b) _b = (b); \
         (_a) < (_b) ? (_a) : (_b); })
#endif

/** Number of array elements. */
#define NELEMENTS(array)  (sizeof(array) / sizeof(array[0]))

/** String length of a const string */
#define STRLEN(__str)     (sizeof((__str))-1)

/** Clear object */
#define CLEAR_OBJ(__obj)   (memset(&(__obj), 0, sizeof((__obj))))

/** Clear object by its pointer */
#define CLEAR_PTROBJ(__ptr)   (memset((__ptr), 0, sizeof((*__ptr))))

/** Try for empty string */
#define STR_ISEMPTY( __ptr )         ( ((char_t*)(__ptr))[0] == 0 )
/** Try for equal strings */
#define STR_ISEQUAL( __ptr, __ptr2 ) ( strcmp(__ptr, __ptr2) == 0 )

/** Bitfield Value Mask constructor */
#define BF_MASK(n, __val) ((__val) << (n))
/** Bitfield Bit set mask constructor */
#define BF_BIT(n)                  BF_MASK((n), 1)
/** Bitfield test mask */
#define BF_ISSET( __bf, __mask )   ( ((__bf) & (__mask)) == (__mask) )
/** Biffield set mask*/
#define BF_MASKSET( __bf, __mask ) ( (__bf) |= (__mask) )
/** Bitfield clear mask*/
#define BF_MASKCLR( __bf, __mask ) ( (__bf) &= ~(__mask) )


typedef char char_t;
typedef uint8_t bool_t;


typedef uint16_t res_t; ///< Type used for return codes

#define RES_CODE_BASE_SYS   ( EXIT_SUCCESS )   ///< Default return code
#define RES_SUCCESS         ( EXIT_SUCCESS )   ///< Success return code
#define RES_ERROR               ( EXIT_FAILURE )   ///< Error return code
#define RES_ERR_NO_MORE_RECORDS ( RES_ERROR + 1 )   ///< Error: There's no more records
#define RES_ERR_FORMAT          ( RES_ERROR + 2 )   ///< Error: Data format incorrect
#define RES_ERR_SZ              ( RES_ERROR + 3 )   ///< Error: Data size incorrect
#define RES_ERR_OVERFLOW        ( RES_ERROR + 4 )   ///< Error: overflowed
#define RES_ERR_BUSY            ( RES_ERROR + 5 )   ///< Error: busy
#define RES_ERR_NOT_IMPLEMENTED ( RES_ERROR + 6 )   ///< Error: not implemented


#define RES_CODE_BASE_OTHER    ( 2000 )    ///< Error code base for other modules


/**
 * @}
 */
#endif /* GENERAL_DEFS_H_ */
