#ifndef _DRV_PMU_H_
#define _DRV_PMU_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_pmu.h is the file defines the interface of PMU register access
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U16 PMU_SIGNAL;
#define SIGNAL_FORCE_ENTER_ENG_MODE             ((PMU_SIGNAL)1 << 0)
#define SIGNAL_FORCE_ENTER_AIR_UPDATE_MODE      ((PMU_SIGNAL)1 << 1)
#define SIGNAL_FORCE_ENTER_LOADER_MODE          ((PMU_SIGNAL)1 << 2)
#define SIGNAL_FORCE_ENTER_CTLR_MODE            ((PMU_SIGNAL)1 << 3)
#define SIGNAL_DISABLE_SLIP_PROTOCOL            ((PMU_SIGNAL)1 << 4)
#define SIGNAL_GEN_HCI_RST_CMD_CPL_EVT          ((PMU_SIGNAL)1 << 8)
#define SIGNAL_GEN_HCI_SW_RST_FLAG              ((PMU_SIGNAL)1 << 9)



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_pmu_func_entry
{
    VOID (*fReset)(VOID);
    VOID (*fDeepSleep)(VOID);
    VOID (*fPowerOff)(VOID);

} DRV_PMU_ENTRY_STRU;



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN DRV_PMU_ENTRY_STRU DRV_PMU;



////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_PMU_Init
 *
 * This function is to read PMU signal from PMU registers, and store the PMU
 * signals into static memory.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_PMU_Init(VOID);

/**
 * DRV_PMU_CheckSignal
 *
 * Check PMU signals.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Signal : The specific signal which wants to check.
 */
EXTERN BOOL DRV_PMU_CheckSignal(
    IN      PMU_SIGNAL Signal);

/**
 * DRV_PMU_StoreSignal
 *
 * Store PMU signal into PMU registers
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Signal : The specific signal which wants to save.
 */
EXTERN VOID DRV_PMU_StoreSignal(
    IN      PMU_SIGNAL Signal);

/**
 * DRV_PMU_IsIceActive
 *
 * Check whether ICE is active
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : Boolean
 */
EXTERN BOOL DRV_PMU_IsIceActive(VOID);

/**
 * DRV_PMU_IsWdRstHappened
 *
 * Check whether last reboot is due to Watchdog timer reset.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : Boolean
 */
EXTERN BOOL DRV_PMU_IsWdRstHappened(VOID);

/**
 * DRV_PMU_IsDeepSleepHappened
 *
 * Check whether boot from deep sleep mode.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : Boolean
 */
EXTERN BOOL DRV_PMU_IsDeepSleepHappened(VOID);

/**
 * DRV_PMU_Frozen
 *
 * Freeze the CPU
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_PMU_Frozen(VOID);

/**
 * DRV_PMU_Sleep
 *
 * Force system entering the sleep mode
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_PMU_Sleep(VOID);

#endif /* _DRV_PMU_H_ */

