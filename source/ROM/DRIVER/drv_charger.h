#ifndef _DRV_CHARGER_H_
#define _DRV_CHARGER_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_charger.h is the file defines the interface of internal charger.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 CHARGER_STATUS;
#define CHARGER_IDLE                ((CHARGER_STATUS)0)
#define CHARGER_TRICKLE             ((CHARGER_STATUS)1)
#define CHARGER_CC_MODE             ((CHARGER_STATUS)2)
#define CHARGER_CV_INITIAL          ((CHARGER_STATUS)3)
#define CHARGER_CV_MODE             ((CHARGER_STATUS)4)
#define CHARGER_COMPLETE            ((CHARGER_STATUS)5)



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_charger_func_entry
{
    BOOL (*fIsChargerIn)(VOID);
    CHARGER_STATUS (*fGetStatus)(VOID);

} DRV_CHARGER_ENTRY_STRU;



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN DRV_CHARGER_ENTRY_STRU DRV_CHARGER;



////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_CHARGER_IsChargerIn
 *
 * Check whether the charger is plagged in.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : Boolean
 */
EXTERN BOOL DRV_CHARGER_IsChargerIn(VOID);

/**
 * DRV_CHARGER_GetStatus
 *
 * Report charger status
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Return : The charger status
 */
EXTERN CHARGER_STATUS DRV_CHARGER_GetStatus(VOID);

#endif /* _DRV_CHARGER_H_ */


