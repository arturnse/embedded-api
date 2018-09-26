/**************************************************************************//**
 * @file     api_config.h
 * @brief    Lib general configurations - configure these defines in your compiler
 * and the API will select the correct definitions.
 * @date     August/2016
 * @author   Artur Rodrigues (securitoys at outlook.com)
 ******************************************************************************/

#ifndef INCLUDES_API_CONFIG_H_
#define INCLUDES_API_CONFIG_H_

#if defined(__LPC11E6X__)

#include "arm_cortex_m0plus.h"

#elif defined(__ARCH_AMD64__)

#include "intel_x86_64.h"

#elif defined(PART_TM4C123GH6PM)

#include "architecture/ti_tm4c123gh6pm.h"

#endif

#include "general_defs.h"


#endif /* INCLUDES_API_CONFIG_H_ */
