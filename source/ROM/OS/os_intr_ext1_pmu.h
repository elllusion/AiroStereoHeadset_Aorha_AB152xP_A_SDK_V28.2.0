#ifndef _OS_INTR_EXT1_PMU_H_
#define _OS_INTR_EXT1_PMU_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * OS_intr_ext1.h is the file defines the interrupt handler interface
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// CONSTANT DEFINITIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- External1 Interrupt ID
typedef U8 OS_EXT1_PMU_INTR_ID;
#define OS_EXT1_PMU_INTR_ID_GPIO1                   ((OS_EXT1_PMU_INTR_ID)(0))
#define OS_EXT1_PMU_INTR_ID_GPIO2                   ((OS_EXT1_PMU_INTR_ID)(1))
#define OS_EXT1_PMU_INTR_ID_GPIO3                   ((OS_EXT1_PMU_INTR_ID)(2))
#define OS_EXT1_PMU_INTR_ID_GPIO4_30                ((OS_EXT1_PMU_INTR_ID)(3))
#define OS_EXT1_PMU_INTR_ID_MMI                     ((OS_EXT1_PMU_INTR_ID)(4))
#define OS_EXT1_PMU_INTR_ID_ADC0                    ((OS_EXT1_PMU_INTR_ID)(5))
#define OS_EXT1_PMU_INTR_ID_CHARGER_CPL             ((OS_EXT1_PMU_INTR_ID)(6))
#define OS_EXT1_PMU_INTR_ID_RECHARGER               ((OS_EXT1_PMU_INTR_ID)(7))
#define OS_EXT1_PMU_INTR_ID_CHARGER_OFF             ((OS_EXT1_PMU_INTR_ID)(8))
#define OS_EXT1_PMU_INTR_ID_CHARGER_ON              ((OS_EXT1_PMU_INTR_ID)(9))
#define OS_EXT1_PMU_INTR_ID_WAKEUP_KEY              ((OS_EXT1_PMU_INTR_ID)(11))
#define OS_EXT1_PMU_INTR_ID_INVALID                 ((OS_EXT1_PMU_INTR_ID)(15))



////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_EXT1_PMU_INTR_RegisterHandler
 *
 * The function is used to register the interrupt handler to the specific
 * interrupt.
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @IntNum : The interrupt number
 * @IntHdlr : The interrupt service routine
 */
EXTERN VOID OS_EXT1_PMU_INTR_RegisterHandler(
    IN      OS_EXT1_PMU_INTR_ID IntrId,
    IN      INTR_HDLR IntrHdlr);

/**
 * OS_EXT1_PMU_INTR_Hdlr
 *
 * This function is interrupt service entry of MCU external interrupt 1.
 *
 * Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID OS_EXT1_PMU_INTR_Hdlr(VOID);

#endif /* _OS_INTR_EXT1_PMU_H_ */

