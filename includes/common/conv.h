/**************************************************************************//**
 * @file     conv.h
 * @brief    Functions that converts presentation data
 * @date     April/2016
 * @author   Artur Rodrigues (securitoys at outlook.com)
 *
 ******************************************************************************/

#ifndef INCLUDES_COMMON_CONV_H_
#define INCLUDES_COMMON_CONV_H_

#include "api_config.h"

/**
 * Converts an integer (with signal) to a string.
 *
 * @param[in]   pVal                Int value
 * @param[out]  pStrTo              Converted string
 * @param[in]   pStrMaxLen          Max number of chars in the string (without the trailing 0)
 *
 * @return #RES_SUCCESS when conversion is properly done.
 */
res_t Conv_IntToStr( int32_t pVal, char_t *pStrTo, uint8_t pStrMaxLen );

#endif /* INCLUDES_COMMON_CONV_H_ */
