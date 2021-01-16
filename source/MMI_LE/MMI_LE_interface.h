//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////

#ifndef _MMI_LE_INTERFACE_H_
#define _MMI_LE_INTERFACE_H_

#include "os.h"
#ifdef LE_SUPPORTED_HOST_FLASH

#ifdef _MMI_LE_C_
	#define MMI_LE_EXTERN
#else
	#define MMI_LE_EXTERN extern	
#endif

#define MMI_LE_LINK_STATUS_DISCONNECTION        0x00
#define MMI_LE_LINK_STATUS_CONNECTION_CMP       0x01
#define MMI_LE_LINK_STATUS_ENCRYPTION_CMP       0x02
#define MMI_LE_LINK_STATUS_ENCRYPTION_FAIL      0x03
#define MMI_LE_LINK_STATUS_AUTHENTICATION_CMP   0x04

#define MMI_LE_ADV_DATA_MAX_DATA_LEN    31
#define MMI_LE_SCAN_RSP_MAX_DATA_LEN    31

enum
{
	MMI_LE_UPDATE_SUCCESS,
	MMI_LE_UPDATE_HCI_CMD_FAIL,
	MMI_LE_UPDATE_WRITE_FLASH_FAIL
};

typedef enum
{
	MMI_LE_ADV_TYPE_ADV_IND,
	MMI_LE_ADV_TYPE_ADV_DIRECT_IND_HIGH_DUTY,
	MMI_LE_ADV_TYPE_ADV_SCAN_IND,
	MMI_LE_ADV_TYPE_ADV_NONCONN_IND,
	MMI_LE_ADV_TYPE_ADV_DIRECT_IND_LOW_DUTY	
}MMI_LE_ADV_TYPE;

enum
{   
    /* HCI_EVENT_UART_LE_CMD */
	MMI_LE_UART_CMD_UPDATE_ADV_DATA = 0x01,
	MMI_LE_UART_CMD_CONNECTION_UPDATE,
	//MMI_LE_UART_CMD_ENABLE_ADV,
};

typedef enum
{
    MMI_LE_ENTER_ADV_STATE,
    MMI_LE_ENTER_POWER_OFF_STATE,
    MMI_LE_ENTER_SUSPEND_STATE,
};

typedef enum
{
    MMI_LE_RESULT_SUCCESS,
    MMI_LE_RESULT_NO_MEMORY,
    MMI_LE_RESULT_UPDATE_NVRAM_FAILED,
    MMI_LE_RESULT_THE_SAME_DATA
}MMI_LE_CONN_UPDATE_RESULT;

typedef struct
{
    U8 addr[6];
}MMI_LE_BD_ADDR;

typedef void (*APP_LINK_EVENT)(U8 linkIndex, U8 status);

struct stMmiLeAppDesc
{
	APP_LINK_EVENT  appLinkEvent;
};

typedef struct stMmiLeAppDesc MMI_LE_APP_DESC;
typedef struct stMmiLeAppNode MMI_LE_APP_NODE;

struct stMmiLeAppNode
{
	MMI_LE_APP_DESC CODE_PTR appDesc;
	MMI_LE_APP_NODE XDATA_PTR nextNode;
};

typedef struct
{
	U8 adv_data_Len;                            //adv data total len
	U8 adv_data[MMI_LE_ADV_DATA_MAX_DATA_LEN];  //adv data
}BLE_ADV_DATA_TYPE;

typedef struct
{
	U8 scan_rsp_data_len;                           //Scan rsp data length
	U8 scan_rsp_data[MMI_LE_SCAN_RSP_MAX_DATA_LEN]; //Scan rsp data
}BLE_SCAN_RSP_TYPE;


MMI_LE_EXTERN OSMQ XDATA OSMQ_MMI_LE_EVT_FROM_UART;
#define OSMQ_MMI_LE_EVT_FROM_UART_ptr         &OSMQ_MMI_LE_EVT_FROM_UART
#define MMI_LE_DELAY_NUM_CONN_INTERVAL        (7)

MMI_LE_EXTERN void MMI_LE_Init(void);
MMI_LE_EXTERN void MMI_LE(void);

PUBLIC void MMI_LE_ConfigStateHandler(void);
PUBLIC void MMI_LE_ReadyStateHandler(void);
PUBLIC void MMI_LE_AdvertisingStateHandler(void);
PUBLIC void MMI_LE_ConnectedStateHandler(void);
PUBLIC void MMI_LE_SuspendStateHandler(void);

PUBLIC U8 MMI_LE_WaitEvent(U8 command_op);
PUBLIC U8 MMI_LE_SetHCIcmdToGAP(U8 command_op,U8 XDATA_PTR param);
PUBLIC U8 XDATA_PTR MMI_LE_GetCmdMsgMem(U8 command_op);
#ifdef SM_SUPPORTED
PUBLIC void MMI_LE_SendUpdateLinkStatusToSM(U8 idx, U8 status);
PUBLIC void MMI_LE_AuthenticationRequest(U8 linkIdx);
#endif
PUBLIC void MMI_LE_EnterAdvertising(void);
PUBLIC U8 MMI_LE_DisableAdv(void);
PUBLIC BOOL MMI_LE_SendConnUpdateReq(U8 XDATA_PTR paraPtr, U8 linkIdx);
PUBLIC void MMI_LE_SectorSaveData(void);
PUBLIC void MMI_LE_DisconnectLink(void);
PUBLIC BOOL MMI_LE_DisconnectPowerOff(void);
PUBLIC BOOL MMI_LE_AllowUpdateFlash(void);
PUBLIC void MMI_LE_SetState(U8 state, U8 subState);
PUBLIC void MMI_LE_SetStateAfterDisc(U8 nextState);
PUBLIC void MMI_LE_DisableBleLink(void);
PUBLIC BOOL MMI_LE_GetLeBdAddr(MMI_LE_BD_ADDR XDATA_PTR bdAddrPtr);
PUBLIC void MMI_LE_RegisterApp(MMI_LE_APP_NODE XDATA_PTR appNode);
PUBLIC void MMI_LE_SetDelayWriteFlashTimer(U8 idx, U32 delayTime);

PUBLIC U8 MMI_LE_UpdateAdvData(BLE_ADV_DATA_TYPE advData);
PUBLIC U8 MMI_LE_UpdateScanRspData(BLE_SCAN_RSP_TYPE scanRsp);

#define RESERVE_BYTE_NO 1
#define CMD_OFFSET (RESERVE_BYTE_NO+1)

/*******************************************************
*	function prototypes of Air Update application 
*	identity verification algorithm
*******************************************************/

#endif /* LE_SUPPORTED_HOST_FLASH */
#endif /* _MMI_LE_INTERFACE_H_ */