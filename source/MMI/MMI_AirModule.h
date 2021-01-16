#ifndef _MMI_AIRMODULE_H_
	#define _MMI_AIRMODULE_H_
	#include "../HFP/HFP_Interface.h"

#ifdef AIR_MODULE

	#ifdef _MMI_AIRMODULE_C_
		#define MMI_AIRMODULE_EXTERN
	#else
		#define MMI_AIRMODULE_EXTERN extern
	#endif

	#define MMI_AIR_POWER_OFF_SYNC_DELAY		1200L //375ms
#define AIROHA_L2CAP_CMD_TYPE	0x01
#define AIROHA_L2CAP_PKT_TYPE	0x02

#define AIROHA_L2CAP_CMD_AUDIO_CHANNEL		0x0001
#define AIROHA_L2CAP_CMD_PHONE_LINKKEY		0x0002
#define AIROHA_L2CAP_CMD_PHONE_STATUS		0x0003
#define AIROHA_L2CAP_CMD_ROLE_CHANGE		0x0004
#define AIROHA_L2CAP_RESP_ROLE_CHANGE		0x0005
#define AIROHA_L2CAP_CMD_TWS_LINKKEY		0x0006
#define AIROHA_L2CAP_CMD_SCO_RELAY			0x0007
#define AIROHA_L2CAP_PKT_SCO_DATA			0x0008
#define AIROHA_L2CAP_CMD_SCO_VOL			0x0009
#define AIROHA_L2CAP_CMD_STATE_SYNC			0x000A
#define AIROHA_L2CAP_CMD_KEY_ACTION			0x000B
#define AIROHA_L2CAP_CMD_BG_LED				0x000C
#define AIROHA_L2CAP_CMD_REPORT_BAT			0x000D
#define AIROHA_L2CAP_CMD_SET_PEQ_PARA		0x000E
#define AIROHA_L2CAP_CMD_PEQ_KEY_CHANGE		0x000F
#define AIROHA_L2CAP_CMD_REALTIME_PEQ		0x0010

typedef struct
{
	U8 channel;
}AIROHA_L2CAP_CMD_AUDIO_CHANNEL_PARA_STRU;

typedef struct
{
	U8 lineInSoundLevel;
	U8 lineInSmallStep;
	U8 storedLinkKeyCnt;
	U8 linkKeyDataBody;
}AIROHA_L2CAP_CMD_PHONE_LINKKEY_PARA_STRU;

typedef struct
{
	U8 status;
	U8 scoSoundLevel;
	U8 scoSmallStep;
	U8 a2dpSoundLevel;
	U8 a2dpSmallStep;
}AIROHA_L2CAP_CMD_PHONE_STATUS_PARA_STRU;

typedef struct
{
	U8 isPhoneConnected;
	U8 isPhoneMusic;
	U8 isPhoneIncoming;
	PhoneNumber	phoneNumber;
	U8 a2dpSoundLevel;
	U8 a2dpSmallStep;
	U8 scoSoundLevel;
	U8 scoSmallStep;
	U8 isPowerOff;
}AIROHA_L2CAP_CMD_ROLE_CHANGE_PARA_STRU;

typedef struct
{
	U8 isPowerOff;
}AIROHA_L2CAP_RESP_ROLE_CHANGE_PARA_STRU;

typedef struct
{
	LinkHistoryType	linkKey;
}AIROHA_L2CAP_CMD_TWS_LINKKEY_PARA_STRU;

typedef struct
{
	U8 isRelay;
	U8 isMSBC;
	U8 scoSoundLevel;
	U8 scoSmallStep;
}AIROHA_L2CAP_CMD_SCO_RELAY_PARA_STRU;

typedef struct
{
	U8 scoSoundLevel;
	U8 scoSmallStep;
}AIROHA_L2CAP_CMD_SCO_VOL_PARA_STRU;

typedef struct
{
	U8 state;
}AIROHA_L2CAP_CMD_STATE_SYNC_PARA_STRU;

typedef struct
{
	U8 keyIndex;
	U8 keyAction;
}AIROHA_L2CAP_CMD_KEY_ACTION_PARA_STRU;

typedef struct
{
	U8 ledIndex;
}AIROHA_L2CAP_CMD_BG_LED_PARA_STRU;

typedef struct
{
	U8 batInPercent;
}AIROHA_L2CAP_CMD_REPORT_BAT_STRU;

typedef struct
{
	U8 peqMode;
	U8 peqModeIndex;
	U8 sectorMode;
}AIROHA_L2CAP_CMD_PEQ_PARA_STRU;

typedef union
{
	PEQ_CONFIG_SECTOR peq;
	HPF_CONFIG_SECTOR hpf;
	#ifdef PEQ_EXPANSION_ENABLE
	PEQ_CONFIG_SECTOR peqExpansion;
	#endif
	#ifdef PEQ_UNDER_100HZ_ENABLE
	PEQ_100HZ_SECTOR peqUnder100Hz;
	#endif
}REALTIME_PEQ_PARA_STRU;

typedef struct
{
	U8 enable_mode;
	REALTIME_PEQ_PARA_STRU para;
}AIROHA_L2CAP_CMD_REALTIME_PEQ_STRU;

typedef union
{
	AIROHA_L2CAP_CMD_AUDIO_CHANNEL_PARA_STRU 		audioChannel;
	AIROHA_L2CAP_CMD_PHONE_LINKKEY_PARA_STRU 		phoneLinkKey;
	AIROHA_L2CAP_CMD_PHONE_STATUS_PARA_STRU 		phoneStatus;
	AIROHA_L2CAP_CMD_ROLE_CHANGE_PARA_STRU	 		roleChange;
	AIROHA_L2CAP_RESP_ROLE_CHANGE_PARA_STRU	 		respRoleChange;
	AIROHA_L2CAP_CMD_TWS_LINKKEY_PARA_STRU	 		twsLinkKey;
	AIROHA_L2CAP_CMD_SCO_RELAY_PARA_STRU	 		scoRelay;
	AIROHA_L2CAP_CMD_SCO_VOL_PARA_STRU		 		scoVol;
	AIROHA_L2CAP_CMD_STATE_SYNC_PARA_STRU	 		stateSync;
	AIROHA_L2CAP_CMD_KEY_ACTION_PARA_STRU	 		keyAction;
	AIROHA_L2CAP_CMD_BG_LED_PARA_STRU		 		bgLed;
	AIROHA_L2CAP_CMD_REPORT_BAT_STRU				batReport;
	AIROHA_L2CAP_CMD_PEQ_PARA_STRU					peqPara;
	AIROHA_L2CAP_CMD_REALTIME_PEQ_STRU				realtimePEQPara;
}AIROHA_L2CAP_PKT_PARA_STRU;


typedef struct
{
	U8 	pktType;
	U16	opCode;
	U8 payloadLength;
	AIROHA_L2CAP_PKT_PARA_STRU	para;
}AIROHA_L2CAP_PKT_STRU;

	typedef struct
	{
		U8 scoTypeLineIn:1;
		U8 isUnMuted:1;
		U8 isHoldForDisable:1;
		U8 isSettingSCO:1;
		U8 isHoldForMute:1;
		U8 isRefreshSCOAgain:1;
		U8 reserved:2;
		OST XDATA_PTR estSCOTimer;
	}AirCtlLinkInfoType;

	typedef struct
	{
		U8 SCOAudioFSSel;
		U8 found_rssi;
		BD_ADDR found_bd_addr;
		U8 found_classOfDev[3];
		U8 isSecurePairing:1;
		U8 isInPairing:1;
		U8 miscMask:6;
		U8 modeInSoundbar;
		OST XDATA_PTR stateTimeOutTimer;
		OST XDATA_PTR stateReminderTimer;
		OST XDATA_PTR closeSCODSPTimer;
		#ifdef LIGHTING_ENABLE
		U32 ledSyncClkVal;
		U8 ledMode;
		U8 ledPara[3];
		#endif
		AirCtlLinkInfoType linkInfo[MAX_MULTI_POINT_NO];
	}MmiAirCtlType;

	MMI_AIRMODULE_EXTERN MmiAirCtlType XDATA gMMI_Air_ctl;

	PUBLIC void MMI_AIR_Init(void);
	PUBLIC void MMI_AIR_CheckTimer(void);
	PUBLIC void MMI_AIR_AclDiscHandle(U8 linkIndex);
	PUBLIC void MMI_AIR_GetInquiryComplete(void);
	PUBLIC void MMI_AIR_GetInquiryResultWithRSSI(U8 rssi, U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR codPtr);
	PUBLIC void MMI_AIR_DecideStopInquiry(void);
	PUBLIC void MMI_AIR_HCISCONullPktCmdCpl(BOOL isMute);
	PUBLIC BOOL MMI_AIR_ReleaseLink(U8 linkIndex);
	PUBLIC void MMI_AIR_HandleVendorAVRCPEvt(U8 linkIndex, U8 operationId, U8 operationParam);
	PUBLIC void MMI_AIR_HandleAirohaAVRCPEvt(U8 linkIndex, U8 evetCode);
	PUBLIC BOOL MMI_AIR_SendKeyPowerOffToAirDevice(void);
	PUBLIC void MMI_AIR_AVRCPSetDriverVol(U8 linkIndex);
	PUBLIC void MMI_AIR_TG_GetVolChanged(U8 linkIndex, U8 absVol);
	PUBLIC BOOL MMI_AIR_TG_GetSetAbsVol(U8 linkIndex, U8 absVol);
	PUBLIC BOOL MMI_AIR_CreateConnectionToWoofer(U8 linkIndex);
	PUBLIC void MMI_AIR_CreateConnectionToSoundbar(U8 linkIndex);
	PUBLIC void MMI_AIR_CreateConnectionToShare(U8 linkIndex);
	PUBLIC BOOL MMI_AIR_IsEnableLPFUnderSCO(U8 linkIndex);
	PUBLIC U8 MMI_AIR_GetAirLink(U8 airType);
	PUBLIC BOOL MMI_AIR_IsAirLink(U8 linkIndex);
	PUBLIC BOOL MMI_AIR_IsHoldLineInDisable(void);
	PUBLIC void MMI_AIR_ExitLineInState(void);
	PUBLIC void MMI_AIR_EnterLineInState(void);
	PUBLIC void MMI_AIR_LineInEnableNotify(void);
	PUBLIC void MMI_AIR_LineInDisableNotify(void);
	PUBLIC void MMI_AIR_LineInDriverVolChange(void);
	PUBLIC void MMI_AIR_LineInStartRelay(void);
	PUBLIC void MMI_AIR_LineInKeyVolUpDn(void);
	PUBLIC void MMI_AIR_LineInUartSetVol(void);
	PUBLIC void MMI_AIR_AiroStereo_Sync(U8 syncCmd,U8 cmdPara);
	PUBLIC void MMI_AIR_PageFailHandler(U8 linkIndex);
	PUBLIC void MMI_AIR_ACLConnRequest(U8 linkIndex);
	PUBLIC void MMI_AIR_ACLConnectedNotify(U8 linkIndex);
	PUBLIC void MMI_AIR_ACLConnFailPassive(U8 linkIndex);
	PUBLIC void MMI_AIR_SCOConnectedNotify(U8 linkIndex, BOOL isConnCpl);
	PUBLIC void MMI_AIR_SCODiscNotify(U8 linkIndex);
	PUBLIC BOOL MMI_AIR_IdleKeyVolUpDnFirst(BOOL isVolumeUp);
	PUBLIC void MMI_AIR_IdleKeyVolUpDnLast(BOOL isVolumeUp);
	PUBLIC void MMI_AIR_AVRCPConnectedNotify(U8 linkIndex);
	PUBLIC BOOL MMI_AIR_TG_GetKeyVolUp(U8 linkIndex);
	PUBLIC BOOL MMI_AIR_TG_GetKeyVolDown(U8 linkIndex);
	PUBLIC void MMI_AIR_A2DPSignalChannelReadyNotify(U8 linkIndex);
	PUBLIC void MMI_AIR_A2DPOpenNotify(U8 linkIndex);
	PUBLIC void MMI_AIR_A2DPAcceptStartReqNotify(U8 linkIndex);
	PUBLIC void MMI_AIR_DriverSendA2DPDSPEnableCmd(U8 linkIndex);
	PUBLIC void MMI_AIR_A2DPDSPEnabledNotify(U8 linkIndex);
	PUBLIC void MMI_AIR_A2DPDSPDisabledNotify(U8 linkIndex);
	PUBLIC void MMI_AIR_DriverSendA2DPDSPDisableCmd(U8 linkIndex);
	PUBLIC void MMI_AIR_A2DPStartRelay(U8 linkIndex);
	PUBLIC void MMI_AIR_A2DPKeyVolUpDn(U8 linkIndex, BOOL isVolumeUp);
	PUBLIC void MMI_AIR_DemoSoundKeyVolUpDn(void);
	PUBLIC void MMI_AIR_TryToStopAirPairing(U8 linkIndex);
	PUBLIC BOOL MMI_AIR_SDAPDisconnectedNotify(U8 linkIndex);
	PUBLIC void MMI_AIR_LinkKeyNotify(U8 XDATA_PTR bdAddrPtr);
	PUBLIC U8 MMI_AIR_GetNextAirLink(U8 linkIndex);

	PUBLIC void MMI_AIR_HCIRoleChangeEventNotify(U8 linkIndex, U8 status, U8 roleOfSlave);
	PUBLIC void MMI_AIR_GAPGetDiscoverabilityModeCpl(U8 mode);

	PUBLIC void MMI_TWS_Decide_Role(U8 linkIndex, U8 isActive);

	PUBLIC void MMI_AIR_ProcessKeyEvent(U8 linkIndex, U8 index);
	PUBLIC U16 MMI_AIR_CheckValidKey(U8 index, U16 keyCode);
	PUBLIC BOOL MMI_AIR_CheckHFPKeyValid(void);
	PUBLIC BOOL MMI_AIR_CheckAVRCPKeyValid(U16 keyCode);

	PUBLIC void MMI_AIR_StartPairing(BOOL isSecurePairing);
	PUBLIC void MMI_AIR_StopPairing(void);
	PUBLIC void MMI_AIR_ResumePairing(void);
	PUBLIC void MMI_AIR_SuspendPairing(void);

	PUBLIC BOOL MMI_AIR_PreProcessForWriteFlashAfterRingVP(void);
	PUBLIC BOOL MMI_AIR_SetLinkRoleMaster(U8 linkIndex);
	#ifdef LIGHTING_ENABLE
	PUBLIC BOOL MMI_AIR_ChangeLightModeNotify(U8 ledMode, U8 ledPara1, U8 ledPara2, U8 ledPara3);
	#endif
	PUBLIC void MMI_AIR_SetConnected(U8 linkIndex);
	PUBLIC void MMI_AIR_FakeMediaEvt(U16 mediaId);
	PUBLIC void MMI_AIR_CheckKeyLineInPowerOn(U16 keyCode);
	PUBLIC void MMI_AIR_ReconnectUserInit(U8 type, U8 settings);
	PUBLIC BOOL MMI_AIR_LoadState(U8 linkIndex, U8 state);
	PUBLIC void MMI_AIR_ConnectedExit(U8 linkIndex);
	PUBLIC BOOL MMI_AIR_DSPSCOEnabled_Event(U8 linkIndex, BOOL isLightMono);
	PUBLIC void MMI_AIR_DSPSCODisabled_Event(U8 linkIndex);


	PUBLIC void MMI_AIR_KeyEnterPairing(U8 linkIndex);
	PUBLIC void MMI_AIR_KeyExitPairing(U8 linkIndex);
	PUBLIC void MMI_AIR_KeyEnterSecurePairing(U8 linkIndex);
	PUBLIC void MMI_TWS_PairingEntry (U8 linkIndex);
	PUBLIC void MMI_TWS_PairingExit (U8 linkIndex);
	PUBLIC U16 MMI_AIR_CheckKeyEnterPairing(U16 keyCode);
	PUBLIC void MMI_AIR_AirAppGetFollowerInfo(U8 cmd,U8 cmdPara);
	PUBLIC BOOL MMI_AIR_PowerOffHandler(U8 reason);
	PUBLIC U8 XDATA_PTR MMI_AIR_Reconnect_GetTarget(U8 type);
	PUBLIC void MMI_AIR_Reconnect_DeleteInvalidList(U8 linkIndex);
	PUBLIC void MMI_AIR_AcceptConnReq(U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR rolePtr);
	PUBLIC void MMI_AIR_SendDriverVolCmd(U8 soundLevel, U8 smallStep);
	PUBLIC BOOL MMI_AIR_ReconnectLinkLoss(U8 XDATA_PTR bdAddrPtr);
	PUBLIC BOOL MMI_AIR_SendCreateConnection(U8 linkIndex, U8 XDATA_PTR bd_addr_ptr, U8 type, U8 XDATA_PTR codPtr);
	PUBLIC void MMI_AIR_SendRelSco(U8 linkIndex);
	PUBLIC BOOL MMI_AIR_SendVCmdAudioQosSetup(U8 linkIndex, U8 serviceType, U8 flowStatus, U8 rxFull);
	PUBLIC void MMI_AIR_SaveData(void);
	PUBLIC void MMI_AIR_UartSendUnSolicitedIndication(U8 opCode, U8 XDATA_PTR dataPtr, U8 linkIndex);
	PUBLIC BOOL MMI_AIR_Key_Event(KeyIndex keyIndex, U8 keyAction);
	PUBLIC void MMI_AIR_SendBGLedCmd(U8 ledDataIndex);
	PUBLIC U16 MMI_AIR_CheckKeyDiscoverable(U16 keyCode);
	PUBLIC void MMI_AIR_WriteSecurityModeComplete(void);
	PUBLIC void MMI_AIR_WriteDiscoverabilityMode(U8 discoverabilityMode);
	PUBLIC void MMI_AIR_EnterIncomingState(U8 linkIndex);
	PUBLIC U16 MMI_AIR_CheckKeyPowerOff(U16 keyCode);
	PUBLIC void MMI_AIR_KeyRoleChange(U8 linkIndex);

	#ifdef PEQ_ENABLE
	PUBLIC void MMI_AIR_SetPEQParaCmd(U8 linkIndex, U8 peqMode, U8 peqModeIndex, U8 sectorMode);
	PUBLIC void MMI_AIR_RealtimePEQCmd(U8 linkIndex, U8 XDATA_PTR para, U8 enable_mode);
	#endif
	
	PUBLIC void MMI_AIR_SaveHistory(U8 linkIndex);

#endif //AIR_MODULE

	PUBLIC void MMI_AIR_AirohaL2capHandler(U8 linkIndex, U8 XDATA_PTR ptr1);
	PUBLIC void MMI_AIR_WriteLocalBdAddrCpl(BOOL isSuccess);
	PUBLIC BOOL MMI_AIR_CheckPairingTimeOutTimerExist(void);

#endif //_MMI_AIRMODULE_H_
