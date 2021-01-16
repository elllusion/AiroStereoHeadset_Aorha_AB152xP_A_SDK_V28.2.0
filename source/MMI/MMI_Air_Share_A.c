#ifdef MMI_LE_ENABLE
#include "..\MMI_LE\MMI_LE_interface.h"
#include "..\MMI_LE\link_index_ctl.h"
#endif
#include "..\COMMON\sys_clk_ctl.h"
typedef struct
{
	U8 isChannelMediaPushed:1;
	U8 getRelayVPOverNotify:1;
	U8 getVolKeyFromTWS:1;
	U8 isLightMonoMode:1;
	U8 reserved:4;
	U8 relayerState;
	U8 bgLEDIndex;
	OST XDATA_PTR powerOffTimer;
	OST XDATA_PTR resumeLEDTimer;
}MMI_AIR_SHARE_A_TYPE;

MMI_AIR_SHARE_A_TYPE XDATA gMMI_Share_A_ctl;
#include "..\AirApp\AirApp_Interface.h"
#ifdef LIGHTING_ENABLE
extern AirAppLedCtl XDATA gAirAppLedCtl;
extern U8 MusicYouCanSeeEnable;
#endif
#include "..\Driver\AudioControl.h"
#include "..\Driver\Peq_HPF.h"
#include "MMI_ChgBat.h"
#ifdef SUPPORT_VOICE_COMMAND
#include "..\Driver\VoiceCommand.h"
#endif
#ifdef TWS_ESCO_RELAY
#include "align_flash.h"
#endif
#ifdef AIR_MODULE
#include "..\MMI_FUNCTION\MMI_Air_Function.h"
#endif

#ifdef DEMOSOUND
PRIVATE void MMI_Share_SendDemoSoundAbsVolToFollower(void)
{
	U8 destinationLink;
	if((destinationLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		MMI_A2DP_SetVolByLevel(destinationLink, gMMI_A2DP_ctl.demoSoundCtl.soundLevel, gMMI_A2DP_ctl.demoSoundCtl.smallStep);
		MMI_AVRCP_CmdGenerator(destinationLink,AVRCP_SET_ABSOLUTE_VOLUME,MMI_AVRCP_GetAbsVolByMusic(destinationLink));
		//#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI-tx_absVol[%d]:%d", (U8)destinationLink, (U8)MMI_AVRCP_GetAbsVolByMusic(destinationLink));
		//#endif
	}
}
#endif

PRIVATE void MMI_Share_SendA2DPAbsVolToFollower(U8 sourceLink)
{
	#ifdef A2DP_Profile
	U8 destinationLink;
	if(gMMI_ctl.sdapCtl[sourceLink].remoteApplicationID == PRODUCT_AIR_NONE)
	{
		if((destinationLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
		{
			MMI_A2DP_SetVolByLevel(destinationLink, CURRENT_A2DP_SOUND_LEVEL(sourceLink), CURRENT_A2DP_SMALL_STEP(sourceLink));
			#ifdef AVRCP_Profile
			MMI_AVRCP_CmdGenerator(destinationLink, AVRCP_SET_ABSOLUTE_VOLUME, MMI_AVRCP_GetAbsVolByMusic(destinationLink));

			//#ifdef DBG_UART_ENABLE_MMI
			LightDbgPrint("MMI-tx_absVol[%d]:%d", (U8)destinationLink, (U8)MMI_AVRCP_GetAbsVolByMusic(destinationLink));
			//#endif
			#endif
		}
	}
	#else
	UNUSED(sourceLink);
	#endif
}

PRIVATE void MMI_Share_SendLineInAbsVolToFollower(void)
{
	#if defined(AVRCP_Profile) && defined(A2DP_Profile) && defined LINEIN_ENABLE
	U8 targetLink;
	if((targetLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		MMI_AVRCP_AbsVolToMusicSoundLevel(targetLink, MMI_LineIn_GetAbsVolByLineIn());
		MMI_AVRCP_CmdGenerator(targetLink,AVRCP_SET_ABSOLUTE_VOLUME, MMI_LineIn_GetAbsVolByLineIn());
		//#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI-tx_absVol_L[%d]:%d", (U8)targetLink, (U8)MMI_LineIn_GetAbsVolByLineIn());
		//#endif
	}
	#endif
}

PRIVATE void MMI_Share_SendSyncCmdToRelayer(U8 SyncCmd,U8 CmdPara)
{
	#if defined(AVRCP_Profile) && defined(A2DP_Profile)
	U8 targetLink;

	if((targetLink = A2DP_SearchFollowerLink()) != MAX_MULTI_POINT_NO)
	{
		switch(SyncCmd)
		{
			case AIR_APP_SYNC_SHARE_CMD:
				switch(CmdPara)
				{
					case AIR_APP_CMD_PARA_VP_ON:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_ON);
						break;
					case AIR_APP_CMD_PARA_VP_OFF:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_OFF);
						break;
					case AIR_APP_CMD_PARA_VP_LANG_CHANG:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_LANG_CHANGE);
						break;
					case AIR_APP_CMD_PARA_PEQ_MODE_CHANG:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_PEQ_MODE_CHANGE);
						break;
					default:
						break;
				}
				break;
			case AIR_APP_SYNC_SHARE_VP_LANG_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_VP_LANG_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			default:
				break;
		}
	}
	#else
	UNUSED(SyncCmd);
	UNUSED(CmdPara);
	#endif
}

PRIVATE void MMI_Share_SendSyncCmdToFollower(U8 SyncCmd,U8 CmdPara)
{
	#if defined(AVRCP_Profile) && defined(A2DP_Profile)
	U8 targetLink;

	if((targetLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		switch(SyncCmd)
		{
			case AIR_APP_SYNC_SHARE_CMD:
				switch(CmdPara)
				{
					case AIR_APP_CMD_PARA_VP_ON:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_ON);
						break;
					case AIR_APP_CMD_PARA_VP_OFF:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_OFF);
						break;
					case AIR_APP_CMD_PARA_VP_LANG_CHANG:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_LANG_CHANGE);
						break;
					case AIR_APP_CMD_PARA_PEQ_MODE_CHANG:
						MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD, AVRCP_VENDOR_UNIQ_AIRAPP_PARA_PEQ_MODE_CHANGE);
						break;
					default:
						break;
				}
				break;
			case AIR_APP_SYNC_SHARE_VP_LANG_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_VP_LANG_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_A2DP_PEQ_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			case AIR_APP_SYNC_SHARE_AUX_PEQ_INDEX_AND_CHANG_NEXT:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX_AND_CHANGE_NEXT, CmdPara);
				break;
			default:
				break;
		}
	}
	#else
	UNUSED(SyncCmd);
	UNUSED(CmdPara);
	#endif
}

PUBLIC AIROHA_L2CAP_PKT_STRU XDATA_PTR MMI_Share_GetL2capCmd(U8 pktType, U16 opCode, U8 payloadLength)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr = (AIROHA_L2CAP_PKT_STRU XDATA_PTR)SYS_MemoryGet(payloadLength + 4);
	if(ptr)
	{
		ptr->pktType = pktType;
		ptr->opCode = opCode;
		ptr->payloadLength = payloadLength;
	}
	return ptr;
}

PUBLIC void MMI_Share_SendAirohaL2capData(U8 linkIndex, U8 XDATA_PTR dataPtr, U16 dataLen)
{
	L2CAP_SendAirohaChannelData(linkIndex, dataPtr, dataLen);
}

PRIVATE void MMI_Share_SendStateSync(U8 linkIndex, U8 state)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_STATE_SYNC, sizeof(AIROHA_L2CAP_CMD_STATE_SYNC_PARA_STRU));
	if(ptr)
	{
		ptr->para.stateSync.state = state;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_STATE_SYNC_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
		LightDbgPrint("==SendStateSync:%x,%x",(U8)linkIndex, (U8)state);
	}
}

PRIVATE void MMI_Share_SendKeyAction(U8 linkIndex, U8 keyIndex, U8 keyAction)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_KEY_ACTION, sizeof(AIROHA_L2CAP_CMD_KEY_ACTION_PARA_STRU));
	if(ptr)
	{
		ptr->para.keyAction.keyIndex = keyIndex;
		ptr->para.keyAction.keyAction = keyAction;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_KEY_ACTION_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
		LightDbgPrint("==SendKeyAction:%x,%x, %x",(U8)linkIndex, (U8)keyIndex, (U8)keyAction);
	}
}

PRIVATE void MMI_Share_SendBGLed(U8 linkIndex, U8 ledDataIndex)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;
	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_BG_LED, sizeof(AIROHA_L2CAP_CMD_BG_LED_PARA_STRU));
	if(ptr)
	{
		ptr->para.bgLed.ledIndex = ledDataIndex;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(AIROHA_L2CAP_CMD_BG_LED_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
		LightDbgPrint("==SendBGLed:%x,%x",(U8)linkIndex, (U8)ledDataIndex);
	}
}

PUBLIC void MMI_Share_SendBGLedCmd(U8 ledDataIndex)
{
	U8 airlink;
	gMMI_Share_A_ctl.bgLEDIndex = ledDataIndex;
	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkRelayer(airlink))
	{
		MMI_Share_SendBGLed(airlink, ledDataIndex);
	}
}

PUBLIC void MMI_Share_SetPEQParaCmd(U8 linkIndex, U8 peqMode, U8 peqModeIndex, U8 sectorMode)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_SET_PEQ_PARA, sizeof(AIROHA_L2CAP_CMD_PEQ_PARA_STRU));
	if(ptr)
	{
		LightDbgPrint("==SendPEQPara:%d,%d,%d", (U8)peqMode, (U8)peqModeIndex, (U8)sectorMode);
		ptr->para.peqPara.peqMode = peqMode;
		ptr->para.peqPara.peqModeIndex = peqModeIndex;
		ptr->para.peqPara.sectorMode = sectorMode;
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr,  sizeof(AIROHA_L2CAP_CMD_PEQ_PARA_STRU) + 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PUBLIC void MMI_Share_KeyChangePEQCmd(U8 linkIndex)
{
	//UNUSED(linkIndex);
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_PEQ_KEY_CHANGE, 0);
	if(ptr)
	{
		LightDbgPrint("==SendKeyChangePEQ:%d", (U8)linkIndex);
		MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, 4);
		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PUBLIC void MMI_Share_RealtimePEQCmd(U8 linkIndex, U8 XDATA_PTR para, U8 enable_mode)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR ptr;

	switch(enable_mode)
	{
		case PEQ_ENABLED_MODE:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, sizeof(PEQ_CONFIG_SECTOR) + 1);
			break;
		case HPF_ENABLED_MODE:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, sizeof(HPF_CONFIG_SECTOR) + 1);
			break;
		#ifdef PEQ_EXPANSION_ENABLE
		case PEQ_EXPANSION_ENABLED_MODE:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, sizeof(PEQ_CONFIG_SECTOR) + 1);
			break;
		#endif
		#ifdef PEQ_UNDER_100HZ_ENABLE
		case PEQ_UNDER_100HZ_ENABLED_MODE:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, sizeof(PEQ_100HZ_SECTOR) + 1);
			break;
		#endif
		case REALTIME_PEQ_HPF_SET_END:
			ptr = MMI_Share_GetL2capCmd(AIROHA_L2CAP_CMD_TYPE, AIROHA_L2CAP_CMD_REALTIME_PEQ, 1);
			break;
		default:
			ptr = (U8 XDATA_PTR)NULL;
			break;
	}

	if(ptr)
	{
		switch(enable_mode)
		{
			case PEQ_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.realtimePEQPara.para.peq,
										  (U8 XDATA_PTR)para,
										  sizeof(PEQ_CONFIG_SECTOR));
				ptr->para.realtimePEQPara.enable_mode = PEQ_ENABLED_MODE;
				LightDbgPrint("==SendRealtimePEQ:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(PEQ_CONFIG_SECTOR) + 1 + 4);
				break;
			case HPF_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.realtimePEQPara.para.hpf,
										  (U8 XDATA_PTR)para,
										  sizeof(HPF_CONFIG_SECTOR));
				ptr->para.realtimePEQPara.enable_mode = HPF_ENABLED_MODE;
				LightDbgPrint("==SendRealtimeHPF:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(HPF_CONFIG_SECTOR) + 1 + 4);
				break;
			#ifdef PEQ_EXPANSION_ENABLE
			case PEQ_EXPANSION_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.realtimePEQPara.para.peqExpansion,
										  (U8 XDATA_PTR)para,
										  sizeof(PEQ_CONFIG_SECTOR));
				ptr->para.realtimePEQPara.enable_mode = PEQ_EXPANSION_ENABLED_MODE;
				LightDbgPrint("==SendRealtimePEQExpansion:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(PEQ_CONFIG_SECTOR) + 1 + 4);
				break;
			#endif
			#ifdef PEQ_UNDER_100HZ_ENABLE
			case PEQ_UNDER_100HZ_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&ptr->para.realtimePEQPara.para.peqUnder100Hz,
										  (U8 XDATA_PTR)para,
										  sizeof(PEQ_100HZ_SECTOR));
				ptr->para.realtimePEQPara.enable_mode = PEQ_UNDER_100HZ_ENABLED_MODE;
				LightDbgPrint("==SendRealtimePEQUnder100Hz:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, sizeof(PEQ_100HZ_SECTOR) + 1 + 4);
				break;
			#endif
			case REALTIME_PEQ_HPF_SET_END:
				ptr->para.realtimePEQPara.enable_mode = REALTIME_PEQ_HPF_SET_END;
				LightDbgPrint("==SendRealtimeEnd:%d", (U8)linkIndex);
				MMI_Share_SendAirohaL2capData(linkIndex, (U8 XDATA_PTR)ptr, 1 + 4);
				break;
		}

		OSMEM_Put((U8 XDATA_PTR)ptr);
	}
}

PRIVATE void MMI_Share_RealTimePEQParaHandler(AIROHA_L2CAP_PKT_STRU XDATA_PTR dataPtr)
{
	DriverMsg XDATA_PTR paraPtr;
	paraPtr = OSMEM_Get(OSMEM_ptr2_small_TX);

	if(paraPtr)
	{
		switch(dataPtr->para.realtimePEQPara.enable_mode)
		{
			case PEQ_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq,
										  (U8 XDATA_PTR)&dataPtr->para.realtimePEQPara.para.peq,
										  sizeof(PEQ_CONFIG_SECTOR));
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = PEQ_ENABLED_MODE;
				break;
			case HPF_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.hpf,
										  (U8 XDATA_PTR)&dataPtr->para.realtimePEQPara.para.hpf,
										  sizeof(HPF_CONFIG_SECTOR));
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = HPF_ENABLED_MODE;
				break;
			#ifdef PEQ_EXPANSION_ENABLE
			case PEQ_EXPANSION_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq_expansion,
										  (U8 XDATA_PTR)&dataPtr->para.realtimePEQPara.para.peqExpansion,
										  sizeof(PEQ_CONFIG_SECTOR));
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = PEQ_EXPANSION_ENABLED_MODE;
				break;
			#endif
			#ifdef PEQ_UNDER_100HZ_ENABLE
			case PEQ_UNDER_100HZ_ENABLED_MODE:
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq_under100Hz,
										  (U8 XDATA_PTR)&dataPtr->para.realtimePEQPara.para.peqUnder100Hz,
										  sizeof(PEQ_100HZ_SECTOR));
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = PEQ_UNDER_100HZ_ENABLED_MODE;
				break;
			#endif
			case REALTIME_PEQ_HPF_SET_END:
				paraPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode = REALTIME_PEQ_HPF_SET_END;
				break;
		}
	}

	MMI_DRV_RealTimeSetPeqHpf((MMIMsgType XDATA_PTR)paraPtr);
}


PUBLIC void MMI_Share_AirohaL2capHandler(U8 linkIndex, U8 XDATA_PTR ptr)
{
	AIROHA_L2CAP_PKT_STRU XDATA_PTR dataPtr = (AIROHA_L2CAP_PKT_STRU XDATA_PTR)(ptr + *ptr + L2CAP_HEADER_LEN); //L2CAP data start


	if(dataPtr->pktType == AIROHA_L2CAP_CMD_TYPE)
	{
		LightDbgPrint("Share_Airoha:%X",(U32)dataPtr->opCode);
		switch(dataPtr->opCode)
		{
			#ifdef TWS_ESCO_RELAY
			case AIROHA_L2CAP_CMD_SCO_RELAY:
				MMI_Share_EnableSCORelay(linkIndex, (U8 XDATA_PTR)dataPtr);
				break;
			case AIROHA_L2CAP_CMD_SCO_VOL:
				MMI_Share_SetSCOVol(linkIndex, (U8 XDATA_PTR)dataPtr);
				break;
			#endif
			case AIROHA_L2CAP_CMD_STATE_SYNC:
				gMMI_Share_A_ctl.relayerState = dataPtr->para.stateSync.state;
				LightDbgPrint("==GetStateSync:%x,%x",(U8)linkIndex, (U8)dataPtr->para.stateSync.state);
				break;
			case AIROHA_L2CAP_CMD_KEY_ACTION:
				LightDbgPrint("==GetKeyAction:%x,%x",(U8)dataPtr->para.keyAction.keyIndex, (U8)dataPtr->para.keyAction.keyAction);
				MMI_DRV_KeyHandler(MMI_DRV_KeyParser(dataPtr->para.keyAction.keyIndex, dataPtr->para.keyAction.keyAction, MMI_GetTopStateByLink(gMMI_ctl.serviceInd), TRUE));
				break;
			case AIROHA_L2CAP_CMD_BG_LED:
				LightDbgPrint("==GetBGLed:%x",(U8)dataPtr->para.bgLed.ledIndex);
				MMI_LED_SendBGLedCmd(dataPtr->para.bgLed.ledIndex);
				break;
			case AIROHA_L2CAP_CMD_SET_PEQ_PARA:
				LightDbgPrint("==GetPEQPara:%d,%d,%d",(U8)dataPtr->para.peqPara.peqMode, (U8)dataPtr->para.peqPara.peqModeIndex, (U8)dataPtr->para.peqPara.sectorMode);
				MMI_DRV_SetFollowerPEQParameter(dataPtr->para.peqPara.peqMode, dataPtr->para.peqPara.peqModeIndex, dataPtr->para.peqPara.sectorMode);
				MMI_DRV_FollowerPEQModeReload();
				break;
			case AIROHA_L2CAP_CMD_PEQ_KEY_CHANGE:
				LightDbgPrint("==GetKeyChangePEQ:%x",(U8)linkIndex);
				MMI_DRV_PEQModeChange();
				break;
			case AIROHA_L2CAP_CMD_REALTIME_PEQ:
				LightDbgPrint("==GetRealtimePEQ:%x,%x",(U8)linkIndex, (U8)dataPtr->para.realtimePEQPara.enable_mode);
				MMI_Share_RealTimePEQParaHandler(dataPtr);
				break;
		}
	}
	#ifdef TWS_ESCO_RELAY
	else if(dataPtr->pktType == AIROHA_L2CAP_PKT_TYPE)
	{
		if(MMI_AIR_EscoPacketHandler(ptr))
			return;
		}
	#endif
	OSMEM_Put(ptr);
}
#ifdef TWS_ESCO_RELAY
PUBLIC BOOL MMI_Share_SendVCmdAudioQosSetup(U8 linkIndex, U8 serviceType, U8 flowStatus, U8 rxFull)
{
    UNUSED(flowStatus);
    UNUSED(rxFull);

	if(A2DP_IsLinkRelayer(linkIndex))
	{
		if(serviceType == SERVICE_TYPE_BEST_EFFORT)
		{
			if(gMMI_ctl.audioableSCOLink != MMI_EOF)
				return TRUE;
			if(gMMI_ctl.audioableA2DPLink != MMI_EOF)
				return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void MMI_Share_SendDriverVolCmd(U8 soundLevel, U8 smallStep)
{
	U8 airlink;
	if(soundLevel == VOLUME_DONT_CARE || smallStep == VOLUME_DONT_CARE)
		return;

	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		if(A2DP_IsLinkRelayer(airlink))
			MMI_Share_SendSCOVol(airlink, soundLevel, smallStep);
	}
}

#endif

PUBLIC void MMI_Share_UartSendUnSolicitedIndication(U8 opCode, U8 XDATA_PTR dataPtr, U8 linkIndex)
{
    UNUSED(dataPtr);
	switch(opCode)
	{
		case MMI_UART_IND_STATE_CHANGE:
		case MMI_UART_IND_INSERT_STATE:
		case MMI_UART_IND_REPLACE_STATE:
		case MMI_UART_IND_REMOVE_STATE:
			if( linkIndex != MMI_EOF)
			{
				U8 airlink;
				if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
				{
					if(A2DP_IsLinkRelayer(airlink) && linkIndex != airlink)
						MMI_Share_SendStateSync(airlink, MMI_GetTopStateByLink(linkIndex));
				}
			}
			break;
	}
}

PUBLIC BOOL MMI_Share_Key_Event(KeyIndex keyIndex, U8 keyAction)
{
	U8 airlink;
	U16 keyEventCode;
	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkFollower(airlink))
	{
		if(MMI_KEY_MAP_MAPPING_BY_KEY_DOWN_STATE_FEAT)
		{
			if(keyAction == KEY_DOWN)
			{
				MMI_Share_SendKeyAction(airlink, keyIndex, keyAction);
			}
		}

		keyEventCode = MMI_DRV_KeyParser(keyIndex, keyAction, gMMI_Share_A_ctl.relayerState, FALSE);
		if(keyEventCode == KEY_VOICEUP || keyEventCode == KEY_VOICEDN)
		{
			return FALSE;
		}

		switch(gMMI_Share_A_ctl.relayerState)
		{
			case MMI_LINE_IN:
			case MMI_CONNECTED:
			case MMI_HFP_INCOMMING:
			case MMI_HFP_OUTGOING:
			case MMI_HFP_CALLACTIVE:
			case MMI_HFP_CALLACTIVE_WITHOUT_SCO:
			case MMI_HFP_CAIMG:
			case MMI_HFP_CAOGG:
			case MMI_HFP_CAMULTY:
				if(keyEventCode == KEY_INVALID)
				{
					keyEventCode = MMI_DRV_KeyParser(keyIndex, keyAction, MMI_GetTopStateByLink(gMMI_ctl.serviceInd), FALSE);
					switch(keyEventCode)
					{
						case KEY_VOICE_COMMAND_ENABLE:
						case KEY_VOICE_PROMPT_LANG_CHANGE:
							return TRUE;
					}
					return FALSE;
				}
				LightDbgPrint("MMI_Share_Key_Event:%x,%d, %d",(U8)keyIndex, (U8)keyAction, (U8)gMMI_Share_A_ctl.relayerState);
				switch(MMI_DRV_GetKeyEventGroup(keyEventCode))
				{
					case HID_KEY_EVENT:
					case HFP_KEY_EVENT:
					case AVRCP_KEY_EVENT:
						MMI_Share_SendKeyAction(airlink, keyIndex, keyAction);
						return TRUE;
					case SYSTEM_KEY_EVENT:
						if(keyEventCode == KEY_MIC_MUTE_TOGGLE || keyEventCode == KEY_MIC_MUTE_ON || keyEventCode == KEY_MIC_MUTE_OFF)
						{
							MMI_Share_SendKeyAction(airlink, keyIndex, keyAction);
							return TRUE;
						}
					default:
						return FALSE;
				}
				break;
			default:
				keyEventCode = MMI_DRV_KeyParser(keyIndex, keyAction, MMI_GetTopStateByLink(gMMI_ctl.serviceInd), FALSE);

				LightDbgPrint("MMI_Share_Key2:%d,%d,%X",(U8)keyIndex, (U8)keyAction, (U32)keyEventCode);
				switch(keyEventCode)
				{
					case KEY_RECONNECT_USER_INIT:
					case KEY_DISCOVERABLE:
					case KEY_NONDISCOVERABLE:
						MMI_Share_SendKeyAction(airlink, keyIndex, keyAction);
						return TRUE;
				}
				break;
		}
	}
	return FALSE;
}

PUBLIC void MMI_Share_Init(void)
{
	gMMI_Share_A_ctl.relayerState = 0;
}


PRIVATE void MMI_Share_GetFollowerInfo(U8 cmd,U8 cmdPara)
{
	#if defined(AVRCP_Profile) && defined(A2DP_Profile)
	U8 targetLink;
	if((targetLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		switch(cmd)
		{
			case AIR_APP_GET_INFO_FROM_FOLLOWER:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_GET_INFO_FROM_FOLLOWER, cmdPara);
				break;
			case AIR_APP_GET_CHGBATSTATUS_FROM_FOLLOWER:
				MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_AIRAPP_GET_CHGBAT_STATUS_FROM_FOLLOWER, cmdPara);
				break;
			default:
				break;
		}
	}
	#else
	UNUSED(cmd);
	UNUSED(cmdPara);
	#endif
}

PRIVATE void MMI_Share_FollowerBehavior(U8 linkIndex)
{
	U8 i;

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(i != linkIndex)
		{
			MMI_ReleaseProfileLinkAndDetach(i);
		}
	}
	gMMI_ctl.currentSupportDeviceNo = 1;
	MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
	MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);

	MMI_SetEnterDiscoverable(FALSE);
	if(MMI_CheckNestState(MMI_EOF, MMI_CONDISCABLE))
		MMI_RmvState(MMI_EOF, MMI_CONDISCABLE);
	#ifdef LINEIN_ENABLE
	MMI_LineIn_DisableFunction();
	#endif
	MMI_CancelCreateAcl();
	MMI_Reconnect_ClearCtl();
	SYS_ReleaseTimer(&gMMI_Share_A_ctl.powerOffTimer);

	#ifdef MMI_LE_ENABLE
	MMI_LE_DisableBleLink();
	#endif

#ifdef DEMOSOUND
	MMI_A2DP_KeyDemoSoundPause();
#endif
}

PRIVATE void MMI_Share_CancelFollowerBehavior(void)
{

	gMMI_ctl.currentSupportDeviceNo = MMI_MAX_PAGE_DEVICE;
	#ifdef LINEIN_ENABLE
	MMI_LineIn_EnableFunction();
	#endif
	#ifdef MMI_LE_ENABLE
	if(!LIC_LE_GetNumOfActiveLink())
	{
		MMI_LE_EnterAdvertising();
	}
	#endif
}

PRIVATE void MMI_Share_ResumeA2DP(U8 linkIndex)
{
	#ifdef A2DP_Profile
	MMIMsgType XDATA_PTR msgPtr;

	if((MMI_DRV_IsAudioActiveDev(linkIndex, AUDIO_DEVICE_MUSIC) || MMI_DRV_IsAudioActiveDev(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY) || MMI_DRV_IsAudioActiveDev(linkIndex, AUDIO_DEVICE_AVRCP_PLAY)) && !gMMI_ctl.audioDevCtl.resumeTimer)
	{
		MMI_A2DP_SendDSPEnableCmd(linkIndex);

		if((msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_STOP_DSP_CMD) ) != (MMIMsgType XDATA_PTR)NULL)
		{
			msgPtr->msgBodyPtr.driverCmd.stopDSPCmd.linkInd = linkIndex;
			OSMQ_PutFront(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		}
	}
	#else
	UNUSED(linkIndex);
	#endif
}

PRIVATE void MMI_Share_SetAudioChannel(U8 linkIndex, U8 channel)
{
	#ifdef A2DP_Profile
	MMI_A2DP_SendCmd(linkIndex, channel);
	if(!gMMI_Share_A_ctl.isChannelMediaPushed)
	{
		MMI_PushMediaEvent((channel == A2DP_AIR_AUDIO_CHANNEL_MONO_R) ? MEDIA_EVT_TWS_RIGHT_CHANNEL : MEDIA_EVT_TWS_LEFT_CHANNEL);
		gMMI_Share_A_ctl.isChannelMediaPushed = TRUE;

		if (channel != A2DP_AIR_AUDIO_CHANNEL_MONO_R )
			MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_SET_CHANNEL_SEL_CMD);


		if(A2DP_IsLinkRelayer(linkIndex))
		{
#ifdef BLUETOOTH_SPEAKER
			MMI_LineIn_EnablePayloadEncoder();
#else
			MMI_LineIn_DisablePayloadEncoder();
#endif
		}

		if(gMMI_ctl.audioableA2DPLink == linkIndex)
		{
			MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_PLAY_CMD);
		}
	}
	#else
	UNUSED(linkIndex);
	UNUSED(channel);
	#endif
}

#ifdef LINEIN_ENABLE
extern MmiLineInInfoCtrl XDATA gMMI_LineIn_ctl;
#endif

PRIVATE void MMI_Share_DecideRole(U8 linkIndex)
{
	U8 sourceLink;

	if(!MMI_SDAP_IsQueryComplete(linkIndex))
		return;

	switch(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
	{
		case PRODUCT_AIR_NONE:
			LightDbgPrint("Decide Role: None");
			#ifdef A2DP_Profile
			for(sourceLink = 0; sourceLink < MAX_MULTI_POINT_NO; sourceLink++)
			{
				if(MMI_IsActiveLink(sourceLink) && A2DP_IsLinkFollower(sourceLink))
				{
					MMI_ReleaseProfileLinkAndDetach (linkIndex);
					break;
				}
			}
			#endif
			SYS_ReleaseTimer(&gMMI_Share_A_ctl.powerOffTimer);
			if(gMMI_ctl.mmiInfo[linkIndex].linkPara.roleSwitchCnt < 2 && MMI_ALWAYS_ROLE_SWITCH_TO_MASTER_FEAT)
			{
				MMI_AIR_SetLinkRoleMaster(linkIndex);
			}
			#ifdef A2DP_Profile
			if(gMMI_ctl.audioableA2DPLink == linkIndex)
				MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_PLAY_CMD);
			#endif
			break;

		case PRODUCT_AIR_TWS:
			{
				U8 key_index, profile, other_link;

				if ((key_index = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE)) == MMI_EOF)
					return;

				#ifdef A2DP_Profile
				other_link = MMI_GetNextActiveLink(linkIndex);
				if(other_link != MMI_EOF && (A2DP_IsLinkRelayer(other_link) || A2DP_IsLinkFollower(other_link)))
				{
					//detach this link because of the other link is tws/shareMe;
					MMI_ReleaseProfileLinkAndDetach (linkIndex);
					MMI_LinkKey_ClearAirLinkHistory(key_index);
					MMI_SECTOR_SetUpdateFlag(TRUE);
					return;
				}
				#else
				UNUSED(other_link);
				#endif

				if(!(profile = gMMI_driver_variation_nvram.air_link_history_info[key_index].profile))
				{
					if (gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
					{
						profile = (NVRAM_PROFILE_LEFT | NVRAM_PROFILE_RELAY);
					}
					else
					{
						profile = (NVRAM_PROFILE_RIGHT | NVRAM_PROFILE_FOLLOW);
					}
					gMMI_driver_variation_nvram.air_link_history_info[key_index].profile = profile;
					MMI_SECTOR_SetUpdateFlag(TRUE);
				}

				if(profile & NVRAM_PROFILE_RELAY)
				{
					MMI_Share_CancelFollowerBehavior();
				}
				if(profile & NVRAM_PROFILE_FOLLOW)
				{
					MMI_Share_FollowerBehavior(linkIndex);
				}

				MMI_LinkKey_ShiftAirLinkKeyToTop(key_index);

				if(!MMI_IsProfileConnected(linkIndex, PROFILE_AVRCP) || !MMI_IsProfileConnected(linkIndex, PROFILE_A2DP) || !MMI_CheckNestState(linkIndex,MMI_CONNECTED))
					return;

				if(profile & NVRAM_PROFILE_RELAY)
				{
					LightDbgPrint("Decide Role:Relay");
					sourceLink = MMI_GetNextNormalLink(linkIndex);

				    if(!MMI_IsProfileConnected(linkIndex, PROFILE_AIR_APP) && (gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT))
					{
						LightDbgPrint("App Profile Connection1");
						MMI_ConnectProfile(linkIndex, PROFILE_AIR_APP);
					}

					#ifdef A2DP_Profile
					if(MMI_AIR_SetLinkRoleMaster(linkIndex) && !A2DP_IsLinkRelayer(linkIndex))
					{
						MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_PACKET_ROLE_RELAYER);
						#ifdef AVRCP_Profile
						MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SET_NOTIFY_MASK, MMI_AVRCP_VOLUME_CHANGED);
						MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_RELAYER);
						#endif
						if(sourceLink != MMI_EOF)
						{
							MMI_Share_SendStateSync(linkIndex, MMI_GetTopStateByLink(sourceLink));
							MMI_Share_SendA2DPAbsVolToFollower(sourceLink);
						}
						#ifdef DEMOSOUND
						else if(gMMI_ctl.audioableA2DPLink == DEMOSOUND_LINKINDEX)
							MMI_Share_SendDemoSoundAbsVolToFollower();
						#endif

						#ifdef TWS_ESCO_RELAY
						if(gMMI_ctl.audioableSCOLink != MMI_EOF && !gMMI_Share_A_ctl.isLightMonoMode)
						{
							MMI_Share_SendSCORelay(linkIndex, TRUE, gMMI_ctl.audioableSCOLink);
						}
						#endif
					}
					#endif
					if(sourceLink == MMI_EOF)
					{
						MMI_Reconnect(RECONNECT_USER_INIT);
						#ifdef LINEIN_ENABLE
						if (!MMI_CheckNestState(MMI_EOF, MMI_LINE_IN))
						#endif
						{
							SYS_SetTimer(&gMMI_Share_A_ctl.powerOffTimer, (U32)gMMI_nvram.timerCtl.twsIdlePowerOffTime * 10 * ONE_SEC);
						}
					}
					SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer);
					SYS_CLK_Set(SYS_CLK_REQ_MMI,SYS_CLK_24M_REQUEST);

					MMI_AIR_AiroStereo_Sync(AIR_APP_SYNC_SHARE_VP_LANG_INDEX_AND_CHANG_NEXT,gMMI_driver_variation_nvram.misc_para.init.vpLangIndex);
					MMI_Share_SendBGLedCmd(gMMI_Share_A_ctl.bgLEDIndex);
				}
				if(profile & NVRAM_PROFILE_FOLLOW)
				{
					#ifdef A2DP_Profile
					LightDbgPrint("Decide Role:Follower");
					MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_PACKET_ROLE_FOLLOWER);
					#endif
					#ifdef AVRCP_Profile
					MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER);
					MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_AIRAPP_REPROT_CHGBAT_STATUS_TO_RELAYER, MMI_CHGBAT_GetBatStatus());
					#endif

					if(!MMI_IsProfileConnected(linkIndex, PROFILE_AIR_APP) && (gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT))
					{
						MMI_ConnectProfile(linkIndex, PROFILE_AIR_APP);
					}

					SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer);
					SYS_CLK_Set(SYS_CLK_REQ_MMI,SYS_CLK_24M_REQUEST);
				}

				#ifdef A2DP_Profile
				if(A2DP_IsLinkRelayer(linkIndex) || (A2DP_IsLinkFollower(linkIndex) && gMMI_Share_A_ctl.getRelayVPOverNotify))
				{
					if(profile & NVRAM_PROFILE_RIGHT)
					{
						LightDbgPrint("Decide Role:Right");
						MMI_Share_SetAudioChannel(linkIndex, A2DP_AIR_AUDIO_CHANNEL_MONO_R);
					}

					if(profile & NVRAM_PROFILE_LEFT)
					{
						LightDbgPrint("Decide Role:Left");
						MMI_Share_SetAudioChannel(linkIndex, A2DP_AIR_AUDIO_CHANNEL_MONO_L);
						#ifdef TWS_SETTINGS
						MMI_DRV_SyncPEQRelayerToFollower(PEQ_A2DP, PEQ_GetModeIndex(PEQ_A2DP), PEQ_GetSectorMode(PEQ_A2DP));
						#endif

					}

					if(A2DP_IsLinkRelayer(linkIndex))
					{
						#ifdef AVRCP_Profile
						MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_SYNC_AND_SWITCH_AUDIO_TRANSPARENCY, DRIVER_CheckAudioComponetEnable(AUDIO_TRANSPARENCY) ? TRUE : FALSE);
						#endif
					}
				}
				#endif
			}
			break;
	}
}

#ifdef LIGHTING_ENABLE
PRIVATE U32 MMI_Share_GetCurrentClk(U8 linkIndex)
{
	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.role == ROLE_MASTER)
	{
		return HAL_ReadMasterClock();
	}
	else
	{
		return HAL_ReadSlave0Clock();
	}
}
PRIVATE void MMI_Share_UpdateLedSyncClk(U8 linkIndex, U32 currentClk)
{
	//Check sniff mode or not
	if (gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_SNIFF || gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_SNIFFING)
	{
		gMMI_Air_ctl.ledSyncClkVal = currentClk + 2000; //500ms
	}
	else
	{
		gMMI_Air_ctl.ledSyncClkVal = currentClk + 192; //60ms
	}
}
PUBLIC void MMI_Share_UpdateLedMode(void)
{
	U8 targetLink;
	U32 currentClk;

	if((gMMI_Air_ctl.ledSyncClkVal != 0) &&
		((targetLink= MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF))
	{
		currentClk = MMI_Share_GetCurrentClk(targetLink);
		currentClk = currentClk - gMMI_Air_ctl.ledSyncClkVal;
		if(currentClk<50)
		{
			gMMI_Air_ctl.ledSyncClkVal = 0;
			switch(gMMI_Air_ctl.ledMode)
			{
				case AIRAPP_CMD_ON_OFF_MUSIC_YOU_CAN_SEE:
					MusicYouCanSeeEnable = gMMI_Air_ctl.ledPara[0];
					break;
				case AIRAPP_CMD_GET_COLOR_PICKER_RGB:
					MMI_LED_Set_RGB(gMMI_Air_ctl.ledPara[0], gMMI_Air_ctl.ledPara[1], gMMI_Air_ctl.ledPara[2]);
					break;
				case AIRAPP_CMD_LIGHTING_MODE_SELECT:
					MMI_LED_LightingMode(gMMI_Air_ctl.ledPara[0]);
					break;
				case AIRAPP_CMD_ALARM_CLOCK:
					MMI_LED_AlarmClock(gMMI_Air_ctl.ledPara[0]);
					break;
			}
		}
		else if(currentClk>1000 && currentClk<6000)
		{
			//Sync fail
			gMMI_Air_ctl.ledSyncClkVal =0;
		}
	}
}
PUBLIC BOOL MMI_Share_ChangeLightModeNotify(U8 ledMode, U8 ledPara1, U8 ledPara2, U8 ledPara3)
{
	U8 targetLink;

	if((targetLink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		gMMI_Air_ctl.ledMode = ledMode;
		gMMI_Air_ctl.ledPara[0] = ledPara1;
		gMMI_Air_ctl.ledPara[1] = ledPara2;
		gMMI_Air_ctl.ledPara[2] = ledPara3;
		MMI_Share_UpdateLedSyncClk(targetLink, MMI_Share_GetCurrentClk(targetLink));
		#ifdef AVRCP_Profile
		MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_LIGHT_MODE_CONTROL, ledMode);
		#endif
		return TRUE;
	}
	return FALSE;
}
#endif

PUBLIC void MMI_Share_AclDiscHandle(U8 linkIndex)
{
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_TWS))
	{
		if(linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
		{
			MMI_Share_CancelFollowerBehavior();

			gMMI_Share_A_ctl.isChannelMediaPushed = FALSE;
        	MMI_DRV_SetOTAVoicepromptsExist(TRUE);
			MMI_PushMediaEvent(MEDIA_EVT_TWS_DISCONNECTED);
			SYS_ReleaseTimer(&gMMI_Share_A_ctl.resumeLEDTimer);
			MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_AUDIO_CHANNEL_STEREO);
			MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_SET_CHANNEL_SEL_CMD);
			#ifdef LIGHTING_ENABLE
			gMMI_Air_ctl.ledSyncClkVal = 0;
			#endif
			SYS_CLK_Set(SYS_CLK_REQ_MMI,SYS_CLK_24M_LEAVE);
			gMMI_Share_A_ctl.relayerState = 0;
		}
		else
		{
			if(MMI_GetNextNormalLink(linkIndex) == MMI_EOF && !MMI_CheckNestState(MMI_EOF, MMI_LINE_IN) && MMI_EOF != MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
			{
				SYS_SetTimer(&gMMI_Share_A_ctl.powerOffTimer, (U32)gMMI_nvram.timerCtl.twsIdlePowerOffTime * 10 * ONE_SEC);
			}
		}
	}
}

PUBLIC void MMI_Share_HandleKeyMode(U8 linkIndex, U8 operationParam)
{
	if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_RELAYER)
	{
		MMI_Share_FollowerBehavior(linkIndex);
	}
	else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER)
	{
		MMI_Share_CancelFollowerBehavior();
	}
	else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_VP_OVER)
	{
		gMMI_Share_A_ctl.getRelayVPOverNotify = TRUE;
		MMI_Share_DecideRole(linkIndex);
	}
}

PUBLIC void MMI_Share_HandleAirappcmd(U8 linkIndex, U8 operationParam)
{
	switch(operationParam)
	{
		case AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_ON:
			MMI_PushMediaEvent(KEY_VOICE_PROMPT_ENABLE);
			MMI_DRV_KeyEventHandler(linkIndex, KEY_VOICE_PROMPT_ENABLE);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_OFF:
			MMI_PushMediaEvent(KEY_VOICE_PROMPT_DISABLE);
			MMI_DRV_KeyEventHandler(linkIndex, KEY_VOICE_PROMPT_DISABLE);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_PARA_VP_LANG_CHANGE:
			MMI_PushMediaEvent(KEY_VOICE_PROMPT_LANG_CHANGE);
			MMI_DRV_KeyEventHandler(linkIndex, KEY_VOICE_PROMPT_LANG_CHANGE);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_PARA_PEQ_MODE_CHANGE:
			if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !MMI_LineIn_IsDSPOpen())
			{
				LightDbgPrint("AirApp : [follower] KEY_PEQ_MODE_CHANGE fail");
			}
			else{
				MMI_PushMediaEvent(KEY_PEQ_MODE_CHANGE);
				MMI_DRV_KeyEventHandler(linkIndex, KEY_PEQ_MODE_CHANGE);
			}
			break;
		default:
			break;
	}
}

PUBLIC void MMI_Share_AirappSyncVPlangIndex(U8 operationParam)
{
	gMMI_driver_variation_nvram.misc_para.init.vpLangIndex = operationParam;
}

PUBLIC void MMI_Share_AirappSyncVPlangIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
	if(gMMI_driver_variation_nvram.misc_para.init.vpLangIndex == operationParam)
	{
		return;
	}
	else
	{
		MMI_PushMediaEvent(KEY_VOICE_PROMPT_LANG_CHANGE);
		MMI_DRV_KeyEventHandler(linkIndex, KEY_VOICE_PROMPT_LANG_CHANGE);
	}
}

#ifdef PEQ_ENABLE
PUBLIC void MMI_Share_AirappSyncA2DPPEQIndex(U8 operationParam)
{
	PEQ_SetModeIndex(PEQ_A2DP, operationParam);
}

PUBLIC void MMI_Share_AirappSyncAUXPEQIndex(U8 operationParam)
{
	PEQ_SetModeIndex(PEQ_LINE_IN, operationParam);
}

PUBLIC void MMI_Share_AirappSyncA2DPPEQIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
	PEQ_SetModeIndex(PEQ_A2DP, operationParam);
	if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !MMI_LineIn_IsDSPOpen())
	{
		LightDbgPrint("AirApp : [follower] KEY_PEQ_MODE_CHANGE fail");
	}
	else
	{
			MMI_PushMediaEvent(KEY_PEQ_MODE_CHANGE);
			MMI_DRV_KeyEventHandler(linkIndex, KEY_PEQ_MODE_CHANGE);
	}
}

PUBLIC void MMI_Share_AirappSyncAUXPEQIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
	PEQ_SetModeIndex(PEQ_LINE_IN, operationParam);
	if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !MMI_LineIn_IsDSPOpen())
	{
		LightDbgPrint("AirApp : [follower] KEY_PEQ_MODE_CHANGE fail");
	}
	else
	{
		MMI_PushMediaEvent(KEY_PEQ_MODE_CHANGE);
		MMI_DRV_KeyEventHandler(linkIndex, KEY_PEQ_MODE_CHANGE);
	}
}
#endif

PUBLIC void MMI_Share_AirappGetBatStatusFromFollower(U8 linkIndex, U8 operationParam)
{
	MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_AIRAPP_REPORT_INFO_TO_RELAYER, MMI_BAT_GetLevelInPercent());
	UNUSED(operationParam);
}

PUBLIC void MMI_Share_AirappGetChgBatStatusFromFollower(U8 linkIndex, U8 operationParam)
{
	MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_AIRAPP_REPROT_CHGBAT_STATUS_TO_RELAYER, MMI_CHGBAT_GetBatStatus());
	UNUSED(operationParam);
}

PUBLIC void MMI_Share_AirappReportChgBatStatusToRelayer(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	if(MMI_EOF != MMI_GetNextNormalLink(linkIndex))
		Airapp_ReportChgBatStatusFollower(MMI_GetNextNormalLink(linkIndex), operationParam);
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_SyncAndSwitchAudioTransparency(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_DRV_Switch_AudioTransparency(operationParam);
	UNUSED(linkIndex);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
#endif
}
PUBLIC void MMI_Share_SyncMICVolAndUp(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	if(MMI_DRV_IsActiveAudioDevInMusic())
	{
		gMMI_ctl.mmiInfo[linkIndex].linkPara.micSoundLevelA2DP = operationParam;
	}
	else
	{
		gMMI_LineIn_ctl.currentsoundLevel = operationParam;
	}
	MMI_DRV_MicVolHandler(linkIndex, KEY_MICUP);
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_SyncMICVolAndDn(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	if(MMI_DRV_IsActiveAudioDevInMusic())
	{
		gMMI_ctl.mmiInfo[linkIndex].linkPara.micSoundLevelA2DP = operationParam;
	}
	else
	{
		gMMI_LineIn_ctl.currentsoundLevel = operationParam;
	}
	MMI_DRV_MicVolHandler(linkIndex, KEY_MICDN);
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_SyncMICVol(U8 linkIndex, U8 operationParam)
{
	UNUSED(linkIndex);
	#ifdef TWS_SETTINGS
	LightDbgPrint("sync AT vol form left[%d]",operationParam);
	gMMI_LineIn_ctl.currentsoundLevel = operationParam;
	MMI_LineIn_SendDriverVolCmd();
	#else
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_SyncSCOStatusForAT(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	U8 airlink;
	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		if(operationParam)
		{
			gMMI_ctl.audioableSCOLink = linkIndex;
		}
		else
		{
			gMMI_ctl.audioableSCOLink = MMI_EOF;
		}
		if((A2DP_IsLinkFollower(airlink) && gMMI_ctl.isAudioTransparencyEnable))
		{
			MMI_DRV_Switch_AudioTransparency(operationParam ? FALSE : TRUE);
		}
	}
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_AirAppSetATVolRsp(U8 linkIndex)
{
	#ifdef AIRAPP_Profile
	U8 anotherLink;
	anotherLink = MMI_GetNextNormalLink(linkIndex);
	if(anotherLink < MAX_MULTI_POINT_NO)
	{
		AirApp_SlaveATGainRsp(anotherLink, AIRAPP_CMD_SET_SLAVE_AT_GAIN_CHANGE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
	}
	#else
	UNUSED(linkIndex);
	#endif
}

PUBLIC void MMI_Share_AirAppReportATGainLv(U8 linkIndex, U8 operationParam)
{
	#ifdef AIRAPP_Profile
	U8 anotherLink;
	anotherLink = MMI_GetNextNormalLink(linkIndex);
	if(anotherLink < MAX_MULTI_POINT_NO)
	{
		AirApp_SlaveATGainRsp(anotherLink, AIRAPP_CMD_GET_SLAVE_AT_GAIN, operationParam);
	}
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_AirAppReportATStatus(U8 linkIndex, U8 operationParam)
{
	#ifdef AIRAPP_Profile
	U8 anotherLink;
	anotherLink = MMI_GetNextNormalLink(linkIndex);
	if(anotherLink < MAX_MULTI_POINT_NO)
	{
		AirApp_SlaveATGainRsp(anotherLink, AIRAPP_CMD_GET_SLAVE_AT_STATUS, operationParam);
	}
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC U8 MMI_Share_ChgLinkIndexNumber(U8 linkIndex)
{
	if (linkIndex == 1)
		linkIndex=0;
	else if (linkIndex ==0)
		linkIndex=1;

	return linkIndex;
}

PUBLIC void MMI_Share_AirappReportBatStatusToRelayer(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	linkIndex=MMI_Share_ChgLinkIndexNumber(linkIndex);
	Airapp_ReportBatValueRightChannel(linkIndex, operationParam);
	#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
	#endif
}

PUBLIC void MMI_Share_TG_GetVolChanged(U8 linkIndex, U8 absVol)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
	{
		U8 i;

		#ifdef AVRCP_Profile
		MMI_AVRCP_AbsVolToMusicSoundLevel(linkIndex, absVol);
		#else
		UNUSED(absVol);
		#endif

		i = MMI_DRV_GetAudioInMusicLink();
		if(MMI_EOF != i)
		{
			MMI_AVRCP_AbsVolToMusicSoundLevel(i, absVol);
			#ifdef AVRCP_Profile
			MMI_AVRCP_CmdGenerator(i, AVRCP_VOLUME_CHANGED, MMI_AVRCP_GetAbsVolByMusic(i));
			#endif
			#ifdef A2DP_Profile
			MMI_A2DP_SendDriverVolCmd(CURRENT_A2DP_SOUND_LEVEL(i), CURRENT_A2DP_SMALL_STEP(i));
			#endif
		}
		#ifdef LINEIN_ENABLE
		else if(MMI_LineIn_IsDSPOpen())
		{
			MMI_LineIn_AbsVolToSoundLevel(absVol);
			MMI_LineIn_SendDriverVolCmd();
		}
		#endif
	}
}

PUBLIC BOOL MMI_Share_TG_GetSetAbsVol(U8 linkIndex, U8 absVol)
{
	UNUSED(absVol);
	UNUSED(linkIndex);
	return FALSE;
}

PUBLIC void MMI_Share_AVRCPSetDriverVol(U8 linkIndex)
{
	MMI_Share_SendA2DPAbsVolToFollower(linkIndex);
}

PUBLIC void MMI_AIR_CreateConnectionToShare(U8 linkIndex)
{
	#ifdef A2DP_Profile
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_TWS))
	{
		U8 other_link = MMI_GetNextActiveLink(linkIndex);

		if(other_link != MMI_EOF && (A2DP_IsLinkRelayer(other_link) || A2DP_IsLinkFollower(other_link)))
		{
			//detach this link because of the other link is tws/shareMe;
			MMI_ReleaseProfileLinkAndDetach (linkIndex);
			MMI_LinkKey_DeleteAirLinkHistory((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
			return;
		}

		MMI_LinkKey_MoveLinkToAirLink((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
		{
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_AVRCP);
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_A2DP);
		}
		#if 0
		MMI_TWS_Decide_Role(linkIndex, gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT);
		#endif
	}
	else
	#else
	UNUSED(linkIndex);
	#endif
	{
		MMI_AIR_InvalidLinkHandle(linkIndex);
	}
}

PUBLIC void MMI_Share_ExitLineInState(void)
{
	U8 i = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);

	if(MMI_EOF != i)
	{
		SYS_SetTimer(&gMMI_Share_A_ctl.resumeLEDTimer, HALF_SEC);
		if(MMI_EOF == MMI_GetNextNormalLink(i))
		{
			SYS_SetTimer(&gMMI_Share_A_ctl.powerOffTimer, (U32)gMMI_nvram.timerCtl.twsIdlePowerOffTime * 10 * ONE_SEC);
		}
	}
}

PUBLIC void MMI_Share_EnterLineInState(void)
{
	SYS_ReleaseTimer(&gMMI_Share_A_ctl.powerOffTimer);
	//MMI_PushMediaEvent(MEDIA_EVT_TWS_DISCONNECTED);
	SYS_ReleaseTimer(&gMMI_Share_A_ctl.resumeLEDTimer);
}

PUBLIC void MMI_Share_LineInStartRelay(void)
{
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_LineInKeyVolUpDn(void)
{
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_LineInUartSetVol(void)
{
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_AiroStereo_Sync(U8 SyncCmd,U8 CmdPara)
{
	if(A2DP_SearchRelayerLink() != MAX_MULTI_POINT_NO)
	{
		LightDbgPrint("Sync cmd:Relayer to Follower");
		MMI_Share_SendSyncCmdToFollower(SyncCmd,CmdPara);
	}
	else if(A2DP_SearchFollowerLink() != MAX_MULTI_POINT_NO)
	{
		LightDbgPrint("Sync cmd:Follower to Relayer");
		MMI_Share_SendSyncCmdToRelayer(SyncCmd,CmdPara);
	}
}

PUBLIC BOOL MMI_Share_IdleKeyVolUpDnFirst(BOOL isVolumeUp)
{
	UNUSED(isVolumeUp);
	if(MMI_EOF == MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
		{
		if(!CURRENT_ACTIVE_LINK_CNT)
			return TRUE;
		}
	return FALSE;
}


PUBLIC void MMI_Share_IdleKeyVolUpDnLast(BOOL isVolumeUp)
{
	U8 linkIndex;
	if((linkIndex =  MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkFollower(linkIndex))
	{
		if(!gMMI_Share_A_ctl.getVolKeyFromTWS)
			MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, (isVolumeUp) ? AVC_OPERATION_VOL_UP : AVC_OPERATION_VOL_DOWN);
	}
}

PUBLIC void MMI_Share_AVRCPConnectedNotify(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC BOOL MMI_Share_TG_GetKeyVolUp(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
	{
		gMMI_Share_A_ctl.getVolKeyFromTWS = TRUE;
	        if(A2DP_IsLinkFollower(linkIndex))
		MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !KEY_VOICEDN : KEY_VOICEUP);
else if(A2DP_IsLinkRelayer(linkIndex))
			MMI_DRV_VoiceAdjustment(linkIndex, (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !KEY_VOICEDN : KEY_VOICEUP, TRUE);
		gMMI_Share_A_ctl.getVolKeyFromTWS = FALSE;
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL MMI_Share_TG_GetKeyVolDown(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
	{
		gMMI_Share_A_ctl.getVolKeyFromTWS = TRUE;
	if(A2DP_IsLinkFollower(linkIndex))
		MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !KEY_VOICEUP : KEY_VOICEDN);
else if(A2DP_IsLinkRelayer(linkIndex))
			MMI_DRV_VoiceAdjustment(linkIndex, (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !KEY_VOICEUP : KEY_VOICEDN, TRUE);
		gMMI_Share_A_ctl.getVolKeyFromTWS = FALSE;
		return TRUE;
	}
	return FALSE;
}

PUBLIC void MMI_Share_A2DPSignalChannelReadyNotify(U8 linkIndex)
{
	#ifdef A2DP_Profile
	if(	gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS &&
		gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT )
	{
		MMI_A2DP_ConnectMediaChannelSource(linkIndex);
	}
	#else
	UNUSED(linkIndex);
	#endif
}

PUBLIC void MMI_Share_A2DPOpenNotify(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC void MMI_Share_A2DPDSPEnabledNotify(U8 linkIndex)
{
	if(linkIndex < MAX_MULTI_POINT_NO)
	{
		#ifdef A2DP_Profile
		if(!MMI_SDAP_IsQueryComplete(linkIndex))
		{
			MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
		}
		else
		{
			if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
			{
				if(!gMMI_Share_A_ctl.isChannelMediaPushed)
					MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
			}
		}
		#endif
	}
	#ifdef DEMOSOUND
	else
	{
		MMI_Share_SendDemoSoundAbsVolToFollower();
	}
	#endif
}

PUBLIC void MMI_Share_A2DPStartRelay(U8 linkIndex)
{
	MMI_Share_SendA2DPAbsVolToFollower(linkIndex);
}

PUBLIC void MMI_Share_A2DPKeyVolUpDn(U8 linkIndex, BOOL isVolumeUp)
{
	UNUSED(isVolumeUp);
	if(MAX_MULTI_POINT_NO == A2DP_SearchFollowerLink())
	MMI_Share_SendA2DPAbsVolToFollower(linkIndex);
}

PUBLIC BOOL MMI_Share_SDAPDisconnectedNotify(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
	return FALSE;
}

PUBLIC void MMI_Share_HCIRoleChangeEventNotify(U8 linkIndex, U8 status, U8 roleOfSlave)
{
	UNUSED(status);
	UNUSED(roleOfSlave);
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC BOOL MMI_Share_PreProcessForWriteFlashAfterRingVP(void)
{
	#ifdef A2DP_Profile
	U8 linkIndex;

	if((linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
	}
	#endif
	return FALSE;
}

PUBLIC void MMI_Share_SetConnected(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC void MMI_Share_FakeMediaEvt(U16 id)
{
	if(id == MEDIA_EVT_TWS_LEFT_CHANNEL || id == MEDIA_EVT_TWS_RIGHT_CHANNEL)
	{
		U8 linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);
		if(linkIndex != MMI_EOF)
		{
			#ifdef A2DP_Profile
			if(A2DP_IsLinkRelayer(linkIndex))
			{
				#ifdef AVRCP_Profile
				MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_VP_OVER);
				#endif
			}
			else
			#endif
			{
				MMI_Share_ResumeA2DP(linkIndex);
			}
		}
	}
}

PUBLIC void MMI_Share_CheckKeyLineInPowerOn(U16 keyCode)
{
	if(keyCode == KEY_INVALID)
	{
		U8 profile, key_index;
		U8 linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);

		if(linkIndex == MMI_EOF || (key_index = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE)) == MMI_EOF)
            return;

		profile = gMMI_driver_variation_nvram.air_link_history_info[key_index].profile;

		if(profile & NVRAM_PROFILE_FOLLOW)
		{
			if(profile & NVRAM_PROFILE_RIGHT)
			{
				MMI_PushMediaEvent(MEDIA_EVT_TWS_RIGHT_CHANNEL);
			}

			if(profile & NVRAM_PROFILE_LEFT)
			{
				MMI_PushMediaEvent(MEDIA_EVT_TWS_LEFT_CHANNEL);
			}
		}
	}
}

PUBLIC void MMI_Share_CheckTimer(void)
{
	U8 linkIndex;

	if(SYS_IsTimerExpired(&gMMI_Share_A_ctl.powerOffTimer))
	{
		#ifdef AVRCP_Profile
		MMI_AIR_SendKeyPowerOffToAirDevice();
		#endif
	}

	if(SYS_IsTimerExpired(&gMMI_Share_A_ctl.resumeLEDTimer))
	{
		if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
			return;

		if((linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
		{
			U8 profile, index;

			if ((index = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE)) == MMI_EOF)
				return;

			profile = gMMI_driver_variation_nvram.air_link_history_info[index].profile;

			if(profile & NVRAM_PROFILE_RIGHT)
			{
				for(index = 0; index< MAX_LED_FILTER_NO; index++)
				{
					if (gMMI_nvram.ledFilterMap[index].evtOpcode == MEDIA_EVT_TWS_RIGHT_CHANNEL)
					{
						MMI_LED_SendLedFilterCmd(index);
					}
				}
			}

			if(profile & NVRAM_PROFILE_LEFT)
			{
				for(index = 0; index< MAX_LED_FILTER_NO; index++)
				{
					if (gMMI_nvram.ledFilterMap[index].evtOpcode == MEDIA_EVT_TWS_LEFT_CHANNEL)
					{
						MMI_LED_SendLedFilterCmd(index);
					}
				}
			}
		}
	}

	#ifdef TWS_ESCO_RELAY
	MMI_AIR_EscoDataRelay();
	#endif
}

PUBLIC BOOL MMI_Share_LoadState(U8 linkIndex, U8 state)
{
	U8 airLink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);

	if(state == MMI_GetTopStateByLink(linkIndex) && state == MMI_CONNECTED)
	{
		if(linkIndex == airLink && CURRENT_ACTIVE_LINK_CNT != gMMI_ctl.currentSupportDeviceNo)
		{
			if(CURRENT_ACTIVE_LINK_CNT == 1)
			{
				if(MMI_CheckNestState(linkIndex, MMI_CONDISCABLE))
				{
					MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_CONDISCABLE].entryLedDisplayIndex);
				}
				else
				{
					MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_CONNECTABLE].entryLedDisplayIndex);
				}
			}
			else if( gMMI_ctl.currentSupportDeviceNo==1 )
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
			}
			return TRUE;
		}
	}

	if((state == MMI_CONDISCABLE || state == MMI_CONNECTABLE))
	{
		if(MMI_EOF != airLink )
		{
			U8 index;
			if ((index = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[airLink].bdAddr, TRUE)) == MMI_EOF)
				return FALSE;

			if((gMMI_driver_variation_nvram.air_link_history_info[index].profile & NVRAM_PROFILE_FOLLOW) && (CURRENT_ACTIVE_LINK_CNT>1))
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_CONNECTED].entryLedDisplayIndex);
			}
			else
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
			}
			return TRUE;
		}
		else if(MMI_CheckProfileOnAllLinks())
		{
			MMI_LED_SendBGLedCmd(5);
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void MMI_Share_ConnectedExit(U8 linkIndex)
{
	U8 keyState = MMI_GetKeyState();
	// Not detach after audio transfer
	if (MMI_KEY_STATE_POWER_OFF != keyState && MMI_KEY_STATE_POWER_OFF_SYNC != keyState)
	{
		if(MMI_EOF == MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
		{
			MMI_LineIn_DisablePayloadEncoder();
		}
	}

	UNUSED(linkIndex);
}

PUBLIC BOOL MMI_Share_DSPSCOEnabled_Event(U8 linkIndex, BOOL isLightMono)
{
	U8 airlink;

	#ifdef TWS_ESCO_RELAY
	gMMI_Share_A_ctl.isLightMonoMode = isLightMono;
	#else
	UNUSED(isLightMono);
	#endif

	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		LightDbgPrint("SCO ENABLE:%d",(U8)isLightMono);
		#ifdef TWS_ESCO_RELAY
		if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_NONE && !isLightMono)
		{
			MMI_Share_SendSCORelay(airlink, TRUE, linkIndex);
		}
		#endif
	}
	UNUSED(linkIndex);
	return FALSE;
}

PUBLIC void MMI_Share_DSPSCODisabled_Event(U8 linkIndex)
{
	U8 airlink;
	if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		LightDbgPrint("SCO DISABLE");
		#ifdef TWS_ESCO_RELAY
		if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_NONE)
		{
			MMI_Share_SendSCORelay(airlink, FALSE, linkIndex);
		}
		#endif
	}
	UNUSED(linkIndex);
}

#if 0
PUBLIC void MMI_TWS_Decide_Role (U8 linkIndex, U8 isActive)
{
	U8 key_index, profile;
	U8 other_link = MMI_GetNextActiveLink(linkIndex);

	if ((key_index = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE)) == MMI_EOF)
		return;

	if(A2DP_GetAirRole(other_link) != AIR_ROLE_NONE)
	{
		//detach this link because of the other link is tws/shareMe;
		MMI_ReleaseProfileLinkAndDetach (linkIndex);
		MMI_LinkKey_ClearAirLinkHistory(key_index);
		MMI_SECTOR_SetUpdateFlag(TRUE);
		return;
	}

	profile = gMMI_driver_variation_nvram.air_link_history_info[key_index].profile;
	if (!profile)
	{
		if (MMI_SUPPORT_TWS_SHARE_FEAT)
		{
			if (isActive)
			{
				profile = NVRAM_PROFILE_RELAY;
			}
			else
			{
				profile = NVRAM_PROFILE_FOLLOW;
			}
		}
		else if (MMI_SUPPORT_TWS_FEAT)
		{
			if (isActive)
			{
				profile = NVRAM_PROFILE_LEFT;
			}
			else
			{
				profile = NVRAM_PROFILE_RIGHT;
			}
		}
	}


	#ifdef A2DP_Profile
	if(gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting > A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS)
		return;
	#endif

	switch (profile)
	{
		case NVRAM_PROFILE_LEFT:
			gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_CHANNEL;
			MMI_Share_ReportChannel(A2DP_TWS_AUDIO_SETTING_LEFT_CHANNEL);
			MMI_SniffDisable (linkIndex, MMI_SNIFF_TWS);
			MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
			break;

		case NVRAM_PROFILE_RIGHT:
			gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_RIGHT_CHANNEL;
			MMI_SniffDisable (linkIndex, MMI_SNIFF_TWS);
			MMI_Share_ReportChannel(A2DP_TWS_AUDIO_SETTING_RIGHT_CHANNEL);

			gMMI_ctl.currentSupportDeviceNo = 1;
			MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
			MMI_ReleaseProfileLinkAndDetach (other_link);
			break;

		case NVRAM_PROFILE_RELAY:
			if (MMI_SUPPORT_AVRCP_KEY_SYNC)
			{
				MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
				gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_RELAY_ONLY;
			}
			else
			{
				gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_FOLLOWER;
			}
			//by default set to follower, and change to relay only when playing
			MMI_Share_ReportChannel(A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_RELAY_ONLY);
			MMI_SniffDisable (linkIndex, MMI_SNIFF_TWS);
			break;

		case NVRAM_PROFILE_FOLLOW:
			gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_FOLLOWER;
			//by default set to follower, and change to relay only when playing
			MMI_SniffDisable (linkIndex, MMI_SNIFF_TWS);
			MMI_Share_ReportChannel(A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_FOLLOWER);
			break;

		default:
			gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS;
			break;
	}
}
#endif
