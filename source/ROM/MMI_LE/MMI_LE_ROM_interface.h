//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////

#ifndef _MMI_LE_ROM_INTERFACE_H_
#define _MMI_LE_ROM_INTERFACE_H_

#include "os.h"
#include "MMI_LE_ROM_LED.h"

#ifdef _MMI_LE_ROM_C_
	#define MMI_LE_ROM_EXTERN
#else
	#define MMI_LE_ROM_EXTERN extern
#endif

typedef enum{
	MMI_LE_ROM_CONFIG,
	MMI_LE_ROM_READY,
	MMI_LE_ROM_ADVERTISING,
	MMI_LE_ROM_CONNECTED,
	MMI_LE_ROM_SUSPEND,
	MMI_LE_ROM_POWER_OFF
}MMI_LE_ROM_MAINSTATE;

typedef enum{
//MMI_LE_CONFIG_SUBSTATE
    MMI_LE_ROM_SET_RANDOM_ADDRESS_SEND_CMD,
    MMI_LE_ROM_SET_RANDOM_ADDRESS_WAIT_EVENT,
    MMI_LE_ROM_ADV_PARAM_SEND_CMD,
    MMI_LE_ROM_ADV_PARAM_WAIT_EVENT,
	MMI_LE_ROM_ADV_DATA_SEND_CMD,
    MMI_LE_ROM_ADV_DATA_WAIT_EVENT,
	MMI_LE_ROM_SCAN_RSP_DATA_SEND_CMD,
    MMI_LE_ROM_SCAN_RSP_DATA_WAIT_EVENT,
//MMI_LE_READY_SUBSTATE
    MMI_LE_ROM_WAITING_TRIGGER,
//MMI_LE_ADVERTISING_SUBSTATE
    MMI_LE_ROM_ADV_NORMAL,
    MMI_LE_ROM_ADV_HIGH_DUTY,
//MMI_LE_CONNECTED_SUBSTATE
    MMI_LE_ROM_CONNECTING,
    MMI_LE_ROM_DISCONNECT,
//MMI_LE_SUSPEND_SUBSTATE
    MMI_LE_ROM_SUSPEND_DISABLE_ADV,
	MMI_LE_ROM_ENTER_SUSPEND,
	MMI_LE_ROM_ENTER_POWER_OFF
}MMI_LE_ROM_SUBSTATE;

PUBLIC void MMI_LE_ROM_LedAllEnable(LED_SETTING_STRU CODE_PTR pPattern);
PUBLIC void MMI_LE_ROM_LedAllDisable(void);
PUBLIC void MMI_EnablePMUBootStatus(void);
PUBLIC void MMI_EngineeringMode(void);
#ifdef AIR_UPDATE_ENABLE
PUBLIC VOID MMI_EngineeringMode_Init(VOID);
PUBLIC void MMI_LE_ROM_CheckVin(void);
PUBLIC void MMI_LE_ROM_KeyPressInEngMode(void);
PUBLIC void MMI_LE_ROM_KeyLongPressInEngMode(void);
PUBLIC void MMI_LE_ROM_CheckKeyStatus(void);
PUBLIC void MMI_LE_ROM_KeyPolling(void);
PUBLIC void MMI_LE_ROM_KeyCheckTimer(void);
PUBLIC void MMI_LE_ROM_CheckAutoShutdownTimer(void);
PUBLIC void MMI_LE_ROM_InitAdcRefValue(void);
PUBLIC void MMI_LE_ROM_ResetAutoShutdownTimer(void);
PUBLIC BOOL MMI_LE_ROM_ScanKeyDebounce(U16 XDATA_PTR keymapPtr);
PUBLIC U16 MMI_LE_ROM_CheckKeyAction(U16 oldKeyBitmap);


PUBLIC void MMI_LE_ROM_Init(void);
PUBLIC void MMI_LE_ROM (void);
PUBLIC void MMI_LE_ROM_ConfigStateHandler(void);
PUBLIC void MMI_LE_ROM_ReadyStateHandler(void);
PUBLIC void MMI_LE_ROM_AdvertisingStateHandler(void);
PUBLIC void MMI_LE_ROM_ConnectedStateHandler(void);
PUBLIC void MMI_LE_ROM_SuspendStateHandler(void);

PUBLIC void MMI_LE_ROM_ReadyToAdv(void);
PUBLIC void MMI_LE_ROM_WaitConnReq(void);
PUBLIC U8 MMI_LE_ROM_WaitEvent(U8 command_op);
PUBLIC U8 MMI_LE_ROM_SetHCIcmdToGAP(U8 command_op);
PUBLIC U8 XDATA_PTR MMI_LE_ROM_GetCmdMsgMem(U8 command_op);



PUBLIC void MMI_LE_ROM_KeyPressInAirMode(void);
PUBLIC void MMI_LE_ROM_KeyLongPressInAirMode(void);
PUBLIC U16 MMI_LE_ROM_GetConnectionHDL(void);
PUBLIC VOID MMI_LE_ROM_SetConnectionHDL(U16 connectionHDL);


#define RESERVE_BYTE_NO 1
#define CMD_OFFSET (RESERVE_BYTE_NO+1)
#define MMI_LE_ROM_GET_CONNHDL_FROM_CONNCPL_EVENT(ptr)         (*((U8 XDATA_PTR)(ptr +(*ptr)+4)) + ((U16)(*((U8 XDATA_PTR)(ptr +(*ptr)+5))) << 8))

/*******************************************************
*	function prototypes of Air Update application
*	identity verification algorithm
*******************************************************/

#endif
#endif
