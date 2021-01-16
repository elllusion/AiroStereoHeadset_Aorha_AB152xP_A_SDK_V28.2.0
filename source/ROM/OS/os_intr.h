#ifndef _OS_INTR_H_
#define _OS_INTR_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * OS_intr.h is the file defines the interrupt handler interface
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// CONSTANT DEFINITIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- Interrupt ID
typedef U8 OS_INTR_ID;
#define OS_INTR_ID_EXT0         ((OS_INTR_ID)(0))
#define OS_INTR_ID_TIMER0       ((OS_INTR_ID)(1))
#define OS_INTR_ID_EXT1         ((OS_INTR_ID)(2))
#define OS_INTR_ID_TIMER1       ((OS_INTR_ID)(3))
#define OS_INTR_ID_TIMER2       ((OS_INTR_ID)(4))



////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef VOID (*INTR_HDLR)(VOID);



////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_INTR_Hdlr
 *
 * Interrupt Handler
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @IntrId : Interrupt ID
 */
EXTERN VOID OS_INTR_Hdlr(
    IN      OS_INTR_ID IntrId);

/**
 * OS_INTR_RegisterHdlr
 *
 * Register the interrupt handler by specified interrupt ID
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @IntrId : Interrupt ID
 * @IntrHdlr : Interrupt handler function
 */
EXTERN VOID OS_INTR_RegisterHdlr(
    IN      OS_INTR_ID IntrId,
    IN      INTR_HDLR IntrHdlr);

/**
 * CtxRestore
 *
 * Do the context restore
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID CtxRestore(VOID);

#endif /* _OS_INTR_H_ */

