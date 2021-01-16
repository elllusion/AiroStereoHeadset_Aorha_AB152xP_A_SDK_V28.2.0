#ifndef _OS_TYPES_H_
#define _OS_TYPES_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * os_types.h defines the types of operations system
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if (COMPILER_IS_KEIL_C51)
#define OS_PTR              XDATA_PTR
#elif (COMPILER_IS_KEIL_C251)
#define OS_PTR              NEAR *
#else
#define OS_PTR              *
#endif

#if (COMPILER_IS_KEIL_C51)
#define OS_NULL_DATA_PTR    ((VOID XDATA_PTR)0)
#elif (COMPILER_IS_KEIL_C251)
#define OS_NULL_DATA_PTR    ((VOID NEAR *)0)
#else
#define OS_NULL_DATA_PTR    NULL
#endif



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if (COMPILER_IS_KEIL_C51)
typedef VOID XDATA_PTR      OS_DATA_PTR;
typedef CONST FAR *         OS_CODE_PTR;
typedef VOID GENERIC_PTR    OS_GENERIC_PTR;
#elif (COMPILER_IS_KEIL_C251)
typedef VOID NEAR *         OS_DATA_PTR;
typedef VOID CONST FAR *    OS_CODE_PTR;
typedef VOID GENERIC_PTR    OS_GENERIC_PTR;
#else
typedef VOID *              OS_DATA_PTR;
typedef VOID *              OS_CODE_PTR;
typedef VOID *              OS_GENERIC_PTR;
#endif



////////////////////////////////////////////////////////////////////////////////
// Macro Prototypes ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_OFFSET_OF - get the offset of a member from the containing structure
 * @type:         the type of the container struct this is embedded in.
 * @member:       the name of the member within the struct.
 */
#define OS_OFFSET_OF(type,member) ((SIZE)(VOID OS_PTR)&((type OS_PTR)0)->member)

/**
 * OS_CONTAINER_OF - cast a member of a structure out to the containing structure
 * @ptr:             the pointer to the member.
 * @type:            the type of the container struct this is embedded in.
 * @member:          the name of the member within the struct.
 */
#define OS_CONTAINER_OF(ptr,type,member) ((type OS_PTR)((U8 OS_PTR)(ptr) - OS_OFFSET_OF(type,member)))

#endif /* _OS_TYPES_H_ */

