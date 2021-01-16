
#ifndef __AIRAPP_INTERFACE_H__
#define __AIRAPP_INTERFACE_H__

#ifndef _BT_CONFIG_PROFILE_H_
#error "To include bt_config_profile.h before this file!!!"
#endif

#ifdef __AIRAPP_C__
#define AIRAPP_EXTERN
#else
#define AIRAPP_EXTERN	extern
#endif

#ifdef LIGHTING_ENABLE
typedef struct
{
	U8 MusicYouCanSeeEnable_ready;
	U8 LedR_ready;
	U8 LedG_ready;
	U8 LedB_ready;
	U8 LightingMode_ready;
	U8 Alarm_clock_OnOff_ready;
}AirAppLedCtl;

extern PUBLIC void MMI_HCI_AirohaCmd_SendResponse(U8 cmdOpcode, U8 resultCode);
extern PUBLIC void LED_Set_RGB(U8 LedR, U8 LedG, U8 LedB);
extern PUBLIC void LED_LightingMode(U8 LightMode);
extern PUBLIC void LED_AlarmClock(U8 OnOff);
extern U8 MusicYouCanSeeEnable;
#endif

#define AIRAPP_MAX_FRAME_SIZE		270	/* to fit in page program size */ /* to fit in uart output header structure */

#define AIRAPP_HCI_CMD_PACKET		0x01
#define AIRAPP_HCI_DATA_PACKET		0x02
#define AIRAPP_HCI_EVENT_PACKET		0x04

enum
{
	AIR_APP_SYNC_SHARE_CMD,
	AIR_APP_SYNC_SHARE_VP_LANG_INDEX,
	AIR_APP_SYNC_SHARE_VP_LANG_INDEX_AND_CHANG_NEXT,
	AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX,
	AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX,
	AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX_AND_CHANG_NEXT,
	AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX_AND_CHANG_NEXT,
};

enum
{
	AIR_APP_GET_INFO_FROM_FOLLOWER,
	AIR_APP_GET_CHGBATSTATUS_FROM_FOLLOWER,
	AIR_APP_REPORT_INFO_TO_RELAYER,
	AIR_APP_REPORT_CHGBATSTATUS_TO_RELAYER,
};
enum
{
	AIR_APP_CMD_PARA_VP_ON,
	AIR_APP_CMD_PARA_VP_OFF,
	AIR_APP_CMD_PARA_VP_LANG_CHANG,
	AIR_APP_CMD_PARA_PEQ_MODE_CHANG,
	AIR_APP_CMD_PARA_BAT_STATUS_FROM_FOLLOWER,
	AIR_APP_CMD_PARA_BAT_STATUS_TO_RELAYER,
	AIR_APP_CMD_PARA_NONE,
};

EXTERN U8 CODE gAirApp_uuid128[16];

EXTERN PUBLIC void AirApp_FakeMedia_Event(U16 id);
EXTERN PUBLIC void AirApp_Init(void);
EXTERN PUBLIC U8 AirApp_GetFlashUpdateLinkIndex(void);

#ifdef BLE_AIRApp
EXTERN PUBLIC U8 AirApp_BleDataEvent(U8 XDATA_PTR ptr, U16 length);
EXTERN PUBLIC void AirApp_BleUpdateConnStatus(BOOL isConnected, U8 XDATA_PTR ptr1);
EXTERN PUBLIC void AirApp_BlePolling(void);
EXTERN PUBLIC U8 AirApp_BleCheckEdrLinkStatus(void);
EXTERN PUBLIC void SendPhoneNumberToApp(U8 linkIndex, U8 length, U8 interval);
EXTERN PUBLIC BOOL AirApp_BleIsConnected(void);
EXTERN PUBLIC void AirApp_BleSetEnable(BOOL enabled);
PUBLIC BOOL AirApp_BleIsActivated(void);
#endif

EXTERN PUBLIC void AirApp_EnterIncomingState(U8 linkIndex);
EXTERN PUBLIC void AirApp_ExitIncomingState(U8 linkIndex);

EXTERN PUBLIC BOOL AirApp_IsCallerNameDataValid(void);
EXTERN PUBLIC void Airapp_ReportBatValueRightChannel(U8 linkIndex, U8 batValue);
PUBLIC void AirApp_ChgBatStatusUpdateEvent(U8 batStatus);
PUBLIC void Airapp_ReportChgBatStatusFollower(U8 linkIndex, U8 chgBatStatus);
PUBLIC void AirApp_OTAFail(void);
PUBLIC void AirApp_CustomerDataRsp(U8 linkIndex, U8 vOcf, U8 result, U8 dataLen, U8 XDATA_PTR dataPtr);
PUBLIC void AirApp_PassThroughTransferData(U8 XDATA_PTR airCmdPtr, U8 cmdOpcode, U8 vogf);
PUBLIC void AirApp_SlaveATGainRsp(U8 linkIndex, U8 cmdOpcode, U8 resultCode);

PUBLIC BOOL AirApp_IsDSPLockedByOTA(void);

#endif //__AIRAPP_INTERFACE_H__
