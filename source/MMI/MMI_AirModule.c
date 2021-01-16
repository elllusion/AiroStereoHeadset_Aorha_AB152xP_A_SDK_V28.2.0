#include "reside_flash.inc"

#define _MMI_AIRMODULE_C_

#include "rc.h"
#include "bt_config_profile.h"

#include "..\L2CAP\L2CAP_Interface.h"

#ifdef AIR_MODULE
#include "MMI.h"
#include "MMI_AirModule.h"
#include "MMI_State.h"
#include "MMI_Protocol.h"
#include "MMI_HCI.h"
#include "MMI_A2DP.h"
#include "MMI_AVRCP.h"
#include "MMI_Driver.h"
#include "MMI_LineIn.h"
#include "MMI_Reconnect.h"
#include "MMI_SDAP.h"
#include "MMI_LED.h"
#include "..\A2DP\A2DP_Interface.h"
#include "..\AVRCP\AVRCP_Interface.h"
#ifdef SAME_SW_DIFF_MODULE
#include "drv_gpio.h"
#endif
#include "..\LM\LM_Interface.h"
#include "..\SECTOR\sector_mmi_nvram_type.h"
#include "..\Driver\Driver_1520.h"


PRIVATE void MMI_AIR_RelayPassThroughComands(U8 linkIndex, U8 keyEvtCode);




extern MmiHandlerSetType CODE_PTR XDATA MMI_StateHandlerSet[MMI_TOTAL_STATE_NO];
extern PUBLIC MMI_KEY_CHECK MMI_AIR_KeyCheckValidHandler[AIR_KEY_EVENT_NO];
extern PUBLIC MMI_KEY_EVT MMI_AIR_KeyEvtHandler[AIR_KEY_EVENT_NO];
extern PUBLIC MmiHandlerSetType MMI_StateTWSPairingHandler;


PUBLIC void MMI_AIR_ProcessKeyEvent(U8 linkIndex, U8 index)
{
	if(MMI_AIR_KeyEvtHandler[index].fKeyEvt)
	{
		MMI_AIR_KeyEvtHandler[index].fKeyEvt(linkIndex);
	}
}

PUBLIC U16 MMI_AIR_CheckValidKey(U8 index, U16 keyCode)
{
	if(MMI_AIR_KeyCheckValidHandler[index].fKeyCheckValid)
	{
		keyCode = MMI_AIR_KeyCheckValidHandler[index].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

PUBLIC BOOL MMI_AIR_CheckHFPKeyValid(void)
{
	#ifdef A2DP_Profile
	if(A2DP_IsLinkFollower(gMMI_ctl.serviceInd) || A2DP_IsLinkRelayer(gMMI_ctl.serviceInd))
		return FALSE;
	#endif

	return TRUE;
}

PUBLIC BOOL MMI_AIR_CheckAVRCPKeyValid(U16 keyCode)
{
	#ifdef A2DP_Profile
	if(A2DP_IsLinkFollower(gMMI_ctl.serviceInd))
	{
		MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, keyCode);
		return FALSE;
	}
	#else
	UNUSED(keyCode);
	#endif
	return TRUE;
}

PUBLIC void MMI_AIR_StartPairing(BOOL isSecurePairing)
{
	gMMI_Air_ctl.isInPairing = TRUE;
	gMMI_Air_ctl.isSecurePairing = isSecurePairing;
	MMI_AddTopState(MMI_EOF, MMI_TWS_PAIRING);
}

PUBLIC void MMI_AIR_StopPairing(void)
{
	gMMI_Air_ctl.isInPairing = FALSE;
	gMMI_Air_ctl.isSecurePairing = FALSE;
	SYS_ReleaseTimer (&gMMI_Air_ctl.stateTimeOutTimer);
	SYS_ReleaseTimer (&gMMI_Air_ctl.stateReminderTimer);
	if(MMI_CheckNestState(MMI_EOF, MMI_TWS_PAIRING))
	{
		MMI_RmvState(MMI_EOF, MMI_TWS_PAIRING);
	}
}

PUBLIC void MMI_AIR_ResumePairing(void)
{
	if((MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER|PRODUCT_AIR_SOUNDBAR|PRODUCT_AIR_TWS) == MMI_EOF) && (MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState()))
	{
		if(gMMI_Air_ctl.isInPairing)
		{
			MMI_AddTopState(MMI_EOF, MMI_TWS_PAIRING);	
		}
	}
	else
	{
		gMMI_Air_ctl.isInPairing = FALSE;
	}
}

PUBLIC void MMI_AIR_SuspendPairing(void)
{
	if(MMI_CheckNestState(MMI_EOF, MMI_TWS_PAIRING))
	{
		MMI_RmvState(MMI_EOF, MMI_TWS_PAIRING);
	}
}

PUBLIC void MMI_AIR_KeyEnterPairing(U8 linkIndex)
{
	UNUSED(linkIndex);
	if((gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_MP_TEST_MODE_ENABLE) || MMI_CheckBootFlag(SYS_SW1_BOOT_FLAG_ENTER_MP_TEST_MODE))
		return;
	else
		MMI_AIR_StartPairing(FALSE);
}

PUBLIC void MMI_AIR_KeyExitPairing(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_AIR_StopPairing();
}

PUBLIC void MMI_AIR_KeyEnterSecurePairing(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_AIR_StartPairing(TRUE);
}

PUBLIC U16 MMI_AIR_CheckKeyEnterPairing(U16 keyCode)
{
	if(MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER|PRODUCT_AIR_SOUNDBAR|PRODUCT_AIR_TWS) != MMI_EOF)
	{
		keyCode = KEY_INVALID;
	}
	return keyCode;
}


PUBLIC void MMI_TWS_PairingEntry (U8 linkIndex)
{
	UNUSED(linkIndex);

	MMI_ConnectabilityHandle(CMD_SET_CONNECTABLE);

	
	if (MMI_IsUnderPage())
	{
		MMI_CancelCreateAcl();
	}
	MMI_Reconnect_ClearCtl();
	if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_SCO_WOOFER
	  #ifdef PRODUCT_TYPE_K
		|| gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT
	  #endif
	)
	{
		MMI_HCI_SendInquiry(INQUIRY_LIAC);
	}
	else
	{
		MMI_DiscoverabilityHandle((gMMI_Air_ctl.isSecurePairing)?CMD_SPECIAL_LIMITED_DISCOVERABLE:CMD_LIMITED_DISCOVERABLE);
	}

	MMI_PushMediaEvent(MEDIA_EVT_TWS_ENTER_PAIRING);
	gMMI_Air_ctl.found_rssi = 0;
	SYS_SetTimer (&gMMI_Air_ctl.stateTimeOutTimer, gMMI_nvram.timerCtl.twsPairingTimeout * (ONE_SEC * 1.28));
	SYS_SetTimer (&gMMI_Air_ctl.stateReminderTimer, gMMI_nvram.timerCtl.twsPairingRemindTime * (ONE_SEC/10));
	MMI_PowerSaving(FALSE, MMI_PWRSAVING_WRITE_CONNECTABILITY, MAX_MULTI_POINT_NO);
}

PRIVATE void MMI_AIR_SendCreateConnectionToInquiryDevice(void)
{
	U8 linkIndex;
	if((linkIndex = MMI_GetEmptyLinkIndex((U8 XDATA_PTR)&gMMI_Air_ctl.found_bd_addr, TRUE)) < MAX_MULTI_POINT_NO)
	{
		gMMI_Air_ctl.found_rssi = 0;
		MMI_HCI_SendCreateConnection (	linkIndex,
										(U8 XDATA_PTR)&gMMI_Air_ctl.found_bd_addr, 
										RECONNECT_TWS_PAIRING, 
										(U8 XDATA_PTR)&gMMI_Air_ctl.found_classOfDev);
	}
}

PRIVATE BOOL MMI_AIR_CheckInquiryResults(void)
{
	return (gMMI_Air_ctl.found_rssi)? TRUE : FALSE;
}

PUBLIC void MMI_TWS_PairingExit (U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_HCI_SendInquiryCancel();
	MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);

	if(MMI_AIR_CheckInquiryResults())
	{
		MMI_AIR_SendCreateConnectionToInquiryDevice();
	}
	MMI_PowerSaving(TRUE, MMI_PWRSAVING_WRITE_CONNECTABILITY, MAX_MULTI_POINT_NO);
	MMI_PushMediaEvent(MEDIA_EVT_TWS_EXIT_PAIRING);
}

PUBLIC BOOL MMI_AIR_IsAirLink(U8 linkIndex)
{
	switch(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
	{
		case PRODUCT_AIR_SCO_WOOFER:
		case PRODUCT_AIR_SOUNDBAR:
		case PRODUCT_AIR_A2DP_WOOFER:
		case PRODUCT_AIR_TWS:
			return TRUE;
	}
	return FALSE;
}

PUBLIC U8 MMI_AIR_GetAirLink(U8 airType)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID & airType)
		{
			return linkIndex;
		}
	}
	return MMI_EOF;
}

PUBLIC void MMI_AIR_InvalidLinkHandle(U8 linkIndex)
{
	MMI_LinkKey_DeleteLinkHistory((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
	#ifndef PRODUCT_TYPE_K
	MMI_LinkKey_DeleteAirLinkHistory((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
	#endif
	MMI_ReleaseProfileLinkAndDetach(linkIndex);
}

#if defined SCO_WOOFER || defined SOUND_BAR
	#include "MMI_Air_SBWF.c"
#endif
#ifdef TWS_SETTINGS
	#ifdef PRODUCT_TYPE_A
		#include "MMI_Air_Share_A.c"
	#elif defined PRODUCT_TYPE_K
		#include "MMI_Air_Share_K.c"
	#else
		#include "MMI_Air_Share.c"
	#endif
#elif defined SOUND_BAR || defined SCO_WOOFER
	#include "MMI_Air_Share_A.c"
#endif

PUBLIC void MMI_AIR_Init(void)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_Init();
#elif defined PRODUCT_TYPE_K
	MMI_Share_Init();
#elif defined PRODUCT_TYPE_A
	MMI_Share_Init();
#endif
	MMI_StateHandlerSet[MMI_TWS_PAIRING] = &MMI_StateTWSPairingHandler;
}

PUBLIC void MMI_AIR_CheckTimer(void)
{
	if (SYS_IsTimerExpired (&gMMI_Air_ctl.stateTimeOutTimer))
	{
		MMI_AIR_StopPairing();
	}
	if (SYS_IsTimerExpired (&gMMI_Air_ctl.stateReminderTimer))
	{
		if(MMI_CheckNestState(MMI_EOF, MMI_TWS_PAIRING))
		{
			SYS_SetTimer(&gMMI_Air_ctl.stateReminderTimer,
				gMMI_nvram.timerCtl.twsPairingRemindTime*(ONE_SEC/10));
			MMI_PushMediaEvent(MEDIA_EVT_TWS_DURING_PAIRING);
		}
	}
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_CheckTimer();
#endif
#ifdef TWS_SETTINGS	
	MMI_Share_CheckTimer();
#endif
}

PUBLIC void MMI_AIR_AclDiscHandle(U8 linkIndex)
{
	MMI_AIR_ResumePairing();
	
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_AclDiscHandle(linkIndex);
#endif
#ifdef TWS_SETTINGS
	MMI_Share_AclDiscHandle(linkIndex);
#endif
	OSMEM_memset_xdata(&gMMI_Air_ctl.linkInfo[linkIndex], 0, sizeof(AirCtlLinkInfoType));
}

PUBLIC void MMI_AIR_GetInquiryComplete(void)
{
	if(MMI_CheckNestState(MMI_EOF, MMI_TWS_PAIRING))
	{
		if (MMI_AIR_CheckInquiryResults())
		{
			MMI_AIR_SuspendPairing();
		}
		else
		{
			MMI_HCI_SendInquiry(INQUIRY_LIAC);
		}
	}
}

PUBLIC void MMI_AIR_GetInquiryResultWithRSSI(U8 rssi, U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR codPtr)
{
	if (rssi > gMMI_Air_ctl.found_rssi && rssi > gMMI_nvram.inquiryRSSITH)
	{
		OSMEM_memcpy_xdata_xdata (	(U8 XDATA_PTR)&gMMI_Air_ctl.found_bd_addr,
									bdAddrPtr,
									sizeof(BD_ADDR));
		OSMEM_memcpy_xdata_xdata (	(U8 XDATA_PTR)&gMMI_Air_ctl.found_classOfDev,
									codPtr,
									3);
		gMMI_Air_ctl.found_rssi = rssi;
	}
}

PUBLIC void MMI_AIR_DecideStopInquiry(void)
{
    if(MMI_AIR_CheckInquiryResults())
    {
        MMI_HCI_SendInquiryCancel();
    }
}

PUBLIC BOOL MMI_AIR_ReleaseLink(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_ReleaseLink(linkIndex);
#else
	UNUSED(linkIndex);
	return FALSE;
#endif
}

PRIVATE void MMI_AIR_HandleKeyMute(U8 linkIndex, U8 operationParam)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_HandleKeyMute(linkIndex, operationParam);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
#endif
}

PRIVATE void MMI_AIR_HandleKeyMode(U8 linkIndex, U8 operationParam)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_HandleKeyMode(linkIndex, operationParam);
#elif defined TWS_SETTINGS
	MMI_Share_HandleKeyMode(linkIndex, operationParam);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);	
#endif
}

PRIVATE void MMI_AIR_HandleAirappcmd(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_HandleAirappcmd(linkIndex, operationParam);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);	
#endif	
}

PRIVATE void MMI_AIR_AirappSyncVPlangIndex(U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappSyncVPlangIndex(operationParam);
#else
	UNUSED(operationParam);	
#endif	
}

PRIVATE void MMI_AIR_AirappSyncVPlangIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappSyncVPlangIndexAndChangeNext(linkIndex, operationParam);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);	
#endif	
}

PRIVATE void MMI_AIR_AirappSyncA2DPPEQIndex(U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappSyncA2DPPEQIndex(operationParam);
#else
	UNUSED(operationParam);	
#endif	
}

PRIVATE void MMI_AIR_AirappSyncAUXPEQIndex(U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappSyncAUXPEQIndex(operationParam);
#else
	UNUSED(operationParam);	
#endif	
}

PRIVATE void MMI_AIR_AirappSyncA2DPPEQIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappSyncA2DPPEQIndexAndChangeNext(linkIndex, operationParam);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);	
#endif	
}

PRIVATE void MMI_AIR_AirappSyncAUXPEQIndexAndChangeNext(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappSyncAUXPEQIndexAndChangeNext(linkIndex, operationParam);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);	
#endif	
}

PRIVATE void MMI_AIR_AirappGetBatStatusFromFollower(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappGetBatStatusFromFollower(linkIndex, operationParam);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam); 
#endif	
}

PRIVATE void MMI_AIR_AirappReportBatStatusToRelayer(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappReportBatStatusToRelayer(linkIndex, operationParam);
#else
	UNUSED(operationParam); 
	UNUSED(linkIndex); 
#endif	
}

PRIVATE void MMI_AIR_AirappGetChgBatStatusFromFollower(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappGetChgBatStatusFromFollower(linkIndex, operationParam);
#else
	UNUSED(linkIndex);
	UNUSED(operationParam); 
#endif	
}

PRIVATE void MMI_AIR_AirappReportChgBatStatusToRelayer(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_AirappReportChgBatStatusToRelayer(linkIndex, operationParam);
#else
	UNUSED(operationParam); 
	UNUSED(linkIndex); 
#endif	
}

PRIVATE void MMI_AIR_SyncAndSwitchAudioTransparency(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_SyncAndSwitchAudioTransparency(linkIndex, operationParam);
#else
	UNUSED(operationParam); 
	UNUSED(linkIndex); 
#endif	
}

PRIVATE void MMI_AIR_SyncMICVolAndUp(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_SyncMICVolAndUp(linkIndex, operationParam);
#else
	UNUSED(operationParam); 
	UNUSED(linkIndex); 
#endif	
}

PRIVATE void MMI_AIR_SyncMICVolAndDn(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_SyncMICVolAndDn(linkIndex, operationParam);
#else
	UNUSED(operationParam); 
	UNUSED(linkIndex); 
#endif	
}

PRIVATE void MMI_AIR_SyncMICVol(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_SyncMICVol(linkIndex, operationParam);
#else
	UNUSED(operationParam); 
	UNUSED(linkIndex); 
#endif	
}

PRIVATE void MMI_AIR_SyncSCOStatusForAT(U8 linkIndex, U8 operationParam)
{
#ifdef TWS_SETTINGS
	MMI_Share_SyncSCOStatusForAT(linkIndex, operationParam);
#else
	UNUSED(operationParam);
	UNUSED(linkIndex);
#endif
}

#ifdef AIR_MODULE
PUBLIC void MMI_AIR_AirAppGetFollowerInfo(U8 cmd,U8 cmdPara)
{
	MMI_Share_GetFollowerInfo(cmd, cmdPara);
}
#endif

PRIVATE void MMI_AIR_AirAppSetATVolRsp(U8 linkIndex)
{
	#ifdef TWS_SETTINGS
	MMI_Share_AirAppSetATVolRsp(linkIndex);
	#else
	UNUSED(linkIndex);
	#endif
}

PRIVATE void MMI_AIR_AirAppReportATGainLv(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	MMI_Share_AirAppReportATGainLv(linkIndex, operationParam);
	#else
	UNUSED(operationParam);
	UNUSED(linkIndex);
	#endif
}

PRIVATE void MMI_AIR_AirAppReportATStatus(U8 linkIndex, U8 operationParam)
{
	#ifdef TWS_SETTINGS
	MMI_Share_AirAppReportATStatus(linkIndex, operationParam);
	#else
	UNUSED(operationParam);
	UNUSED(linkIndex);
	#endif
}

PUBLIC void MMI_AIR_HandleVendorAVRCPEvt(U8 linkIndex, U8 operationId, U8 operationParam)
{
	switch(operationId)
	{
		case AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY:
			MMI_AIR_HandleKeyMode(linkIndex, operationParam);
			break;

		case AVRCP_VENDOR_UNIQ_MUTE_CONTROL:
			MMI_AIR_HandleKeyMute(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_SHARE_CMD:
			MMI_AIR_HandleAirappcmd(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX:
			MMI_AIR_AirappSyncVPlangIndex(operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_VP_LANG_INDEX_AND_CHANGE_NEXT:
			MMI_AIR_AirappSyncVPlangIndexAndChangeNext(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX:
			MMI_AIR_AirappSyncA2DPPEQIndex(operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX:
			MMI_AIR_AirappSyncAUXPEQIndex(operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_A2DP_PEQ_INDEX_AND_CHANGE_NEXT:
			MMI_AIR_AirappSyncA2DPPEQIndexAndChangeNext(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_SYNC_AUX_PEQ_INDEX_AND_CHANGE_NEXT:
			MMI_AIR_AirappSyncAUXPEQIndexAndChangeNext(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_GET_INFO_FROM_FOLLOWER:
			MMI_AIR_AirappGetBatStatusFromFollower(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_REPORT_INFO_TO_RELAYER:
			MMI_AIR_AirappReportBatStatusToRelayer(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_GET_CHGBAT_STATUS_FROM_FOLLOWER:
			MMI_AIR_AirappGetChgBatStatusFromFollower(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_AIRAPP_REPROT_CHGBAT_STATUS_TO_RELAYER:
			MMI_AIR_AirappReportChgBatStatusToRelayer(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_SYNC_AND_SWITCH_AUDIO_TRANSPARENCY:
			MMI_AIR_SyncAndSwitchAudioTransparency(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_SYNC_MIC_VOL_AND_UP:
			MMI_AIR_SyncMICVolAndUp(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_SYNC_MIC_VOL_AND_DN:
			MMI_AIR_SyncMICVolAndDn(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_SET_AT_VOL_CMD:
			MMI_AIR_SyncMICVol(linkIndex, operationParam);
			MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_SET_AT_VOL_RSP, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_SET_AT_VOL_RSP:
			MMI_AIR_AirAppSetATVolRsp(linkIndex);
			break;
		case AVRCP_VENDOR_UNIQ_SYNC_SCO_STATUS_FOR_AT:
			MMI_AIR_SyncSCOStatusForAT(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_GET_AT_GAIN_LV:
			MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_REPORT_AT_GAIN_LV, MMI_GetAudioTransparencyEnable() ? DEFAULT_LINEIN_SOUND_LEVEL : HCI_VEVT_OCF_RESULT_CODE_FAIL);
			break;
		case AVRCP_VENDOR_UNIQ_REPORT_AT_GAIN_LV:
			MMI_AIR_AirAppReportATGainLv(linkIndex, operationParam);
			break;
		case AVRCP_VENDOR_UNIQ_GET_AT_STATUS:
			MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_REPORT_AT_STATUS, MMI_GetAudioTransparencyEnable() ? HCI_VEVT_OCF_RESULT_CODE_SUCCESS : HCI_VEVT_OCF_RESULT_CODE_FAIL);
			break;
		case AVRCP_VENDOR_UNIQ_REPORT_AT_STATUS:
			MMI_AIR_AirAppReportATStatus(linkIndex, operationParam);
			break;
		default:
			break;
	}
}

PUBLIC void MMI_AIR_HandleAirohaAVRCPEvt(U8 linkIndex, U8 evetCode)
{
	switch(evetCode)
	{
		case AVRCP_EVT_TO_MMI_GET_KEY_POWER_RESP: //Sent Power to peer already
			if(gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer)
			{
				SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer);
				MMI_DRV_KeyEventHandler(linkIndex, KEY_POWER_OFF);
			}
			break;
		case AVRCP_EVT_TO_MMI_GET_KEY_POWER_CMD:  //Get power off from peer
			switch(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
			{
				case PRODUCT_AIR_SCO_WOOFER:
				case PRODUCT_AIR_SOUNDBAR:
				case PRODUCT_AIR_A2DP_WOOFER:
				case PRODUCT_AIR_TWS:
					#if 1 //using sync delay to ensure remote get POWEROFF key response
					MMI_SetKeyState(MMI_KEY_STATE_POWER_OFF_SYNC);
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer, MMI_AIR_POWER_OFF_SYNC_DELAY);
					#else
					MMI_DRV_KeyEventHandler(linkIndex, KEY_POWER_OFF);
					#endif
					break;

				case PRODUCT_AIR_NONE:
					break;
			}
			break;

		case AVRCP_EVT_TO_MMI_GET_KEY_PLAY_CMD:
		case AVRCP_EVT_TO_MMI_GET_KEY_PAUSE_CMD:
			MMI_AIR_RelayPassThroughComands(linkIndex, KEY_AVRCP_PLAY);
			break;

		case AVRCP_EVT_TO_MMI_GET_KEY_FORWARD_CMD:
			MMI_AIR_RelayPassThroughComands(linkIndex, KEY_AVRCP_FORWARD);
			break;

		case AVRCP_EVT_TO_MMI_GET_KEY_BACKWARD_CMD:
			MMI_AIR_RelayPassThroughComands(linkIndex, KEY_AVRCP_BACKWARD);
			break;
	}
}

#ifdef AVRCP_Profile
PUBLIC U8 MMI_AIR_SendKeyPowerOffToAirDevice(void)
{
	U8 linkIndex;
	
	#ifdef PRODUCT_TYPE_K
	linkIndex = MMI_Share_SendKeyPowerOffToAirDevice();
	if(linkIndex < MAX_MULTI_POINT_NO)
	{
		LightDbgPrint("==Share_SendKeyPowerOff TRUE:");
		MMI_SetKeyState(MMI_KEY_STATE_POWER_OFF_SYNC);
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer,
						gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffInterval ?
							(gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffInterval * 4L) : MMI_AIR_POWER_OFF_SYNC_DELAY);
		return linkIndex;
	}
	#endif

	if(MMI_AIR_SYNC_POWER_KEY_FEAT)
	{
		for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
		{
			if(MMI_IsActiveLink(linkIndex) && MMI_AIR_IsAirLink(linkIndex))
			{
				MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_POWER);
				SYS_SetTimer(	&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer,
								gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffInterval ?
								(gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffInterval * 4L) : MMI_AIR_POWER_OFF_SYNC_DELAY);
				return linkIndex;
			}
		}
	}
	return MAX_MULTI_POINT_NO;
}

PUBLIC void MMI_AIR_AVRCPSetDriverVol(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_AVRCPSetDriverVol(linkIndex);
#elif defined TWS_SETTINGS
	MMI_Share_AVRCPSetDriverVol(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_TG_GetVolChanged(U8 linkIndex, U8 absVol)
{
#if defined TWS_SETTINGS
	MMI_Share_TG_GetVolChanged(linkIndex, absVol);
#else
	UNUSED(linkIndex);
	UNUSED(absVol);
#endif
}

PUBLIC BOOL MMI_AIR_TG_GetSetAbsVol(U8 linkIndex, U8 absVol)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_TG_GetSetAbsVol(linkIndex, absVol);
#elif defined TWS_SETTINGS
	return MMI_Share_TG_GetSetAbsVol(linkIndex, absVol);
#else
	UNUSED(linkIndex);
	UNUSED(absVol);
	return FALSE;
#endif
}
#endif //of AVRCP_Profile

PUBLIC BOOL MMI_AIR_IsEnableLPFUnderSCO(U8 linkIndex)
{
	UNUSED(linkIndex);
	if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_SCO_WOOFER)
	{
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL MMI_AIR_IsHoldLineInDisable(void)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_IsHoldLineInDisable();
#else
	return FALSE;
#endif
}

PUBLIC void MMI_AIR_ExitLineInState(void)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_ExitLineInState();
#elif defined TWS_SETTINGS
	MMI_Share_ExitLineInState();
#endif
}

PUBLIC void MMI_AIR_EnterLineInState(void)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_EnterLineInState();
#elif defined TWS_SETTINGS
	MMI_Share_EnterLineInState();
#endif
}

PUBLIC void MMI_AIR_LineInEnableNotify(void)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_LineInEnableNotify();
#endif
}

PUBLIC void MMI_AIR_LineInDisableNotify(void)
{

}

PUBLIC void MMI_AIR_LineInDriverVolChange(void)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_LineInDriverVolChange();
#endif
}

PUBLIC void MMI_AIR_LineInStartRelay(void)
{
#if defined TWS_SETTINGS
	MMI_Share_LineInStartRelay();
#endif
}

PUBLIC void MMI_AIR_LineInKeyVolUpDn(void)
{
#if defined TWS_SETTINGS
	MMI_Share_LineInKeyVolUpDn();
#endif
}

PUBLIC void MMI_AIR_LineInUartSetVol(void)
{
#if defined TWS_SETTINGS
	MMI_Share_LineInUartSetVol();
#endif
}

PUBLIC void MMI_AIR_AiroStereo_Sync(U8 syncCmd,U8 cmdPara)
{
#ifdef TWS_SETTINGS
	MMI_Share_AiroStereo_Sync(syncCmd,cmdPara);
#else
	UNUSED(syncCmd);
	UNUSED(cmdPara);
#endif
}

PUBLIC void MMI_AIR_PageFailHandler(U8 linkIndex)
{
	if(MMI_GetBdAddrByLinkIndex(linkIndex))
	{
		if(SYS_IsBDAddrIsTheSame(&gMMI_Air_ctl.found_bd_addr, MMI_GetBdAddrByLinkIndex(linkIndex)))
		{
			MMI_AIR_ResumePairing();
		}
	}
	
	#ifdef PRODUCT_TYPE_K
	MMI_Share_PageFailHandler(linkIndex);
	#endif
}

PUBLIC void MMI_AIR_ACLConnRequest(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_AIR_SuspendPairing();  //exit TWS_PAIRING state when remote acl requested;
}

PUBLIC void MMI_AIR_ACLConnectedNotify(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_ACLConnectedNotify(linkIndex);
#elif defined PRODUCT_TYPE_K
	MMI_Share_ACLConnectedNotify(linkIndex);
#else
	UNUSED(linkIndex);
#endif	
}

PUBLIC void MMI_AIR_ACLConnFailPassive(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_AIR_ResumePairing();
}

PUBLIC void MMI_AIR_SCOConnectedNotify(U8 linkIndex, BOOL isConnCpl)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_SCOConnectedNotify(linkIndex, isConnCpl);
#else
	UNUSED(linkIndex);
	UNUSED(isConnCpl);
#endif
}

PUBLIC void MMI_AIR_SCODiscNotify(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_SCODiscNotify(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC BOOL MMI_AIR_IdleKeyVolUpDnFirst(BOOL isVolumeUp)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_IdleKeyVolUpDnFirst(isVolumeUp);
#elif defined PRODUCT_TYPE_A || defined PRODUCT_TYPE_K
	return MMI_Share_IdleKeyVolUpDnFirst(isVolumeUp);
#else
	UNUSED(isVolumeUp);
	return FALSE;
#endif	
}

PUBLIC void MMI_AIR_IdleKeyVolUpDnLast(BOOL isVolumeUp)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_IdleKeyVolUpDnLast(isVolumeUp);
#elif defined PRODUCT_TYPE_A
	MMI_Share_IdleKeyVolUpDnLast(isVolumeUp);
#else
	UNUSED(isVolumeUp);
#endif	
}

PUBLIC void MMI_AIR_AVRCPConnectedNotify(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_AVRCPConnectedNotify(linkIndex);
#elif defined TWS_SETTINGS
	MMI_Share_AVRCPConnectedNotify(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC BOOL MMI_AIR_TG_GetKeyVolUp(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_TG_GetKeyVolUp(linkIndex);
#elif defined TWS_SETTINGS
	return MMI_Share_TG_GetKeyVolUp(linkIndex);
#else
	UNUSED(linkIndex);
	return FALSE;
#endif
}

PUBLIC BOOL MMI_AIR_TG_GetKeyVolDown(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_TG_GetKeyVolDown(linkIndex);
#elif defined TWS_SETTINGS
	return MMI_Share_TG_GetKeyVolDown(linkIndex);
#else
	UNUSED(linkIndex);
	return FALSE;
#endif
}

PUBLIC void MMI_AIR_A2DPSignalChannelReadyNotify(U8 linkIndex)
{
#if defined TWS_SETTINGS
	MMI_Share_A2DPSignalChannelReadyNotify(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_A2DPOpenNotify(U8 linkIndex)
{
#if defined TWS_SETTINGS
	MMI_Share_A2DPOpenNotify(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_A2DPAcceptStartReqNotify(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_A2DPAcceptStartReqNotify(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_DriverSendA2DPDSPEnableCmd(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_DriverSendA2DPDSPEnableCmd(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_A2DPDSPEnabledNotify(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_A2DPDSPEnabledNotify(linkIndex);
#elif defined TWS_SETTINGS
	MMI_Share_A2DPDSPEnabledNotify(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_A2DPDSPDisabledNotify(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_A2DPDSPDisabledNotify(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_DriverSendA2DPDSPDisableCmd(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_DriverSendA2DPDSPDisableCmd(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_A2DPStartRelay(U8 linkIndex)
{
#if defined TWS_SETTINGS
	MMI_Share_A2DPStartRelay(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_A2DPKeyVolUpDn(U8 linkIndex, BOOL isVolumeUp)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	UNUSED(isVolumeUp);
	MMI_SBWF_A2DPKeyVolUpDn(linkIndex);
#elif defined TWS_SETTINGS
	MMI_Share_A2DPKeyVolUpDn(linkIndex, isVolumeUp);
#else
	UNUSED(linkIndex);
	UNUSED(isVolumeUp);
#endif
}

PUBLIC void MMI_AIR_DemoSoundKeyVolUpDn(void)
{
#if defined TWS_SETTINGS
	#ifdef DEMOSOUND
	MMI_Share_SendDemoSoundAbsVolToFollower();
	#endif
#endif
}

PUBLIC void MMI_AIR_TryToStopAirPairing(U8 linkIndex)
{
	if(PRODUCT_AIR_NONE != gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
	{
		MMI_AIR_StopPairing();
	}
}

PUBLIC BOOL MMI_AIR_SDAPDisconnectedNotify(U8 linkIndex)
{
	MMI_AIR_TryToStopAirPairing(linkIndex);

#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_SDAPDisconnectedNotify(linkIndex);
#elif defined TWS_SETTINGS
	return MMI_Share_SDAPDisconnectedNotify(linkIndex);
#else
	UNUSED(linkIndex);
	return FALSE;
#endif
}

PUBLIC void MMI_AIR_LinkKeyNotify(U8 XDATA_PTR bdAddrPtr)
{
	U8 linkIndex = MMI_GetLinkIndexByBdA(bdAddrPtr);
	if(linkIndex != MAX_MULTI_POINT_NO && PRODUCT_AIR_NONE != gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
		MMI_LinkKey_MoveLinkToAirLink((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
	
	#ifdef PRODUCT_TYPE_K
	MMI_Share_LinkKeyNotify(linkIndex);
	#endif
}


extern MMI_KEY_EVT_MAP_TYPE XDATA MMI_KeyEvtMap[KEY_ACTIONTOTALNUMBER];
PRIVATE void MMI_AIR_RelayPassThroughComands(U8 linkIndex, U8 keyEvtCode)
{
	U8 i, j, keyEvtNo;

	if(MMI_SUPPORT_AVRCP_KEY_SYNC && PRODUCT_AIR_TWS == gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
	{		
		keyEvtNo = 0;
		for(j = 0; j < KEY_ACTIONTOTALNUMBER; j++)
		{
			keyEvtNo += MMI_KeyEvtMap[j].keyEventNo;
		}
		
		for(i = 0; i < MAX_MULTI_POINT_NO; i++)
		{
			U32 stateBitMask;
			
			if(!MMI_IsActiveLink(i) || i == linkIndex)
				continue;
				
			if(PRODUCT_AIR_NONE != gMMI_ctl.sdapCtl[i].remoteApplicationID)
				continue;

			stateBitMask = 1L << (MMI_GetTopStateByLink(i));

			for(j = 0; j < keyEvtNo; j++)
			{
				if(keyEvtCode == gMMI_nvram_keymap.mmiKeyMapTbl[j].mmiKeyEvent && (stateBitMask & gMMI_nvram_keymap.mmiKeyMapTbl[j].keySuppotedState))
				{
					MMI_PushMediaEvent(keyEvtCode);
					MMI_DRV_KeyEventHandler(i, keyEvtCode);
					break;
				}
			}
		}
	}
}

PUBLIC void MMI_AIR_HCIRoleChangeEventNotify(U8 linkIndex, U8 status, U8 roleOfSlave)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_HCIRoleChangeEventNotify(linkIndex, status, roleOfSlave);
#elif defined TWS_SETTINGS
	MMI_Share_HCIRoleChangeEventNotify(linkIndex, status, roleOfSlave);
#else
	UNUSED(linkIndex);
	UNUSED(status);
	UNUSED(roleOfSlave);
#endif
}

PUBLIC void MMI_AIR_HCISCONullPktCmdCpl(BOOL isMute)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_HCISCONullPktCmdCpl(isMute);
#else
	UNUSED(isMute);
#endif
}


PUBLIC BOOL MMI_AIR_IsTWSApplication(void)
{
	return (MMI_IsLocalDeviceType(PRODUCT_AIR_TWS))? TRUE:FALSE;
}

PUBLIC U8 MMI_AIR_GetNextAirLink(U8 linkIndex)
{
	U8 i, j;
	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		j = (i+linkIndex +1)%MAX_MULTI_POINT_NO;
		if(MMI_IsActiveLink(j) && PRODUCT_AIR_NONE != gMMI_ctl.sdapCtl[j].remoteApplicationID)
			return j;
	}

	return MMI_EOF;
}

PUBLIC void MMI_AIR_GAPGetDiscoverabilityModeCpl(U8 mode)
{
	if(MMI_CheckNestState(MMI_EOF, MMI_TWS_PAIRING)
		&& mode == (LIMITED_DISCOVERABLE_MODE | SAC_LIMITED_DISCOVERABLE_MODE)
		&& GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) != PRODUCT_AIR_SOUNDBAR)
	{
		MMI_HCI_SendInquiry(INQUIRY_LIAC);
	}
}

PUBLIC BOOL MMI_AIR_PreProcessForWriteFlashAfterRingVP(void)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_PreProcessForWriteFlashAfterRingVP();
#elif defined TWS_SETTINGS
	return MMI_Share_PreProcessForWriteFlashAfterRingVP();
#else
	return FALSE;
#endif
}

PUBLIC BOOL MMI_AIR_SetLinkRoleMaster(U8 linkIndex)
{
	if(!MMI_IsRoleOfMaster(linkIndex))
	{
		MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
		return FALSE;
	}
	return TRUE;
}

PUBLIC void MMI_AIR_SetConnected(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_SetConnected(linkIndex);
#elif defined TWS_SETTINGS
	MMI_Share_SetConnected(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_FakeMediaEvt(U16 id)
{
#ifdef TWS_SETTINGS
	MMI_Share_FakeMediaEvt(id);
#else
	UNUSED(id);
#endif
}

PUBLIC void MMI_AIR_CheckKeyLineInPowerOn(U16 keyCode)
{
#ifdef TWS_SETTINGS
	MMI_Share_CheckKeyLineInPowerOn(keyCode);
#else
	UNUSED(keyCode);
#endif
}

PUBLIC void MMI_AIR_ReconnectUserInit(U8 type, U8 settings)
{
#if defined PRODUCT_TYPE_A || defined PRODUCT_TYPE_K
	UNUSED(type);
	UNUSED(settings);
#else
	if(settings != RECONNECT_TWS)
	{
		if(MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER|PRODUCT_AIR_TWS) == MMI_EOF)
			MMI_Reconnect_AddListByMode(type, RECONNECT_TWS);
	}
#endif
}

PUBLIC BOOL MMI_AIR_LoadState(U8 linkIndex, U8 state)
{
#ifdef TWS_SETTINGS
	return MMI_Share_LoadState(linkIndex, state);
#elif defined SOUND_BAR || defined SCO_WOOFER
	return MMI_SBWF_LoadState(linkIndex, state);
#else
	UNUSED(linkIndex);
	UNUSED(state);
	return FALSE;
#endif
}

PUBLIC void MMI_AIR_ConnectedExit(U8 linkIndex)
{
#ifdef TWS_SETTINGS
	MMI_Share_ConnectedExit(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

#ifdef LIGHTING_ENABLE
PUBLIC BOOL MMI_AIR_ChangeLightModeNotify(U8 ledMode, U8 ledPara1, U8 ledPara2, U8 ledPara3)
{
	return MMI_Share_ChangeLightModeNotify(ledMode, ledPara1, ledPara2, ledPara3);
}
#endif

PUBLIC BOOL MMI_AIR_DSPSCOEnabled_Event(U8 linkIndex, BOOL isLightMono)
{
	UNUSED(isLightMono);
#if defined SCO_WOOFER || defined SOUND_BAR
	return MMI_SBWF_DSPSCOEnabledNotify(linkIndex);
#elif defined TWS_SETTINGS
	return MMI_Share_DSPSCOEnabled_Event(linkIndex, isLightMono);	
#else
	UNUSED(linkIndex);
	return FALSE;
#endif
}

PUBLIC void MMI_AIR_DSPSCODisabled_Event(U8 linkIndex)
{
#ifdef TWS_SETTINGS
	MMI_Share_DSPSCODisabled_Event(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC BOOL MMI_AIR_PowerOffHandler(U8 reason)
{
	if(reason == POWEROFF_REASON_SHUT_DOWN_LOW_BAT || reason == POWEROFF_REASOM_TEMP_PROTECT)
	{
		U8 linkIndex;
		for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
		{
			if(MMI_IsActiveLink(linkIndex) && A2DP_IsLinkRelayer(linkIndex))
			{
				LightDbgPrint("MMI_AIR_PowerOffHandler:LowBat");
				SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.batteryMonitoringTimer);
				//If ready to battery low power off, do not play VP		
				SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryLEDtimer);
				SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryRingtimer);

				MMI_AIR_SendKeyPowerOffToAirDevice();
				return TRUE;
			}
		}
	}
	return FALSE;
}

PUBLIC U8 XDATA_PTR MMI_AIR_Reconnect_GetTarget(U8 type)
{
#ifdef PRODUCT_TYPE_K
	return MMI_Share_Reconnect_GetTarget(type);
#else
	UNUSED(type);
	return (MMIReonnectDataType XDATA_PTR)NULL;
#endif
}

PUBLIC void MMI_AIR_Reconnect_DeleteInvalidList(U8 linkIndex)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_Reconnect_DeleteInvalidList(linkIndex);
#else
	UNUSED(linkIndex);
#endif
}

PUBLIC void MMI_AIR_AcceptConnReq(U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR rolePtr)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_AcceptConnReq(bdAddrPtr, rolePtr);
#else
	UNUSED(bdAddrPtr);
	UNUSED(rolePtr);
#endif
}

PUBLIC void MMI_AIR_SendDriverVolCmd(U8 soundLevel, U8 smallStep)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_SendDriverVolCmd(soundLevel, smallStep);
#elif defined PRODUCT_TYPE_A && defined TWS_ESCO_RELAY
	MMI_Share_SendDriverVolCmd(soundLevel, smallStep);
#else
	UNUSED(soundLevel);
	UNUSED(smallStep);
#endif
}

PUBLIC BOOL MMI_AIR_ReconnectLinkLoss(U8 XDATA_PTR bdAddrPtr)
{
#ifdef PRODUCT_TYPE_K
	return MMI_Share_ReconnectLinkLoss(bdAddrPtr);
#else
	UNUSED(bdAddrPtr);
	return FALSE;
#endif
}

PUBLIC BOOL MMI_AIR_SendCreateConnection(U8 linkIndex, U8 XDATA_PTR bd_addr_ptr, U8 type, U8 XDATA_PTR codPtr)
{
#ifdef PRODUCT_TYPE_K
	return MMI_Share_SendCreateConnection(linkIndex, bd_addr_ptr, type, codPtr);
#else
	UNUSED(bd_addr_ptr);
	UNUSED(linkIndex);
	UNUSED(type);
	UNUSED(codPtr);
	return FALSE;
#endif	
}

PUBLIC void MMI_AIR_SendRelSco(U8 linkIndex)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	MMI_SBWF_SendRelSco(linkIndex);
#else
	UNUSED(linkIndex);
#endif	
}

PUBLIC BOOL MMI_AIR_SendVCmdAudioQosSetup(U8 linkIndex, U8 serviceType, U8 flowStatus, U8 rxFull)
{
#if defined SCO_WOOFER || defined SOUND_BAR
	UNUSED(flowStatus);
	UNUSED(rxFull);
	if(linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_SOUNDBAR))
		return TRUE;
#endif

#ifdef PRODUCT_TYPE_K
	return MMI_Share_SendVCmdAudioQosSetup(linkIndex, serviceType, flowStatus, rxFull);
#elif defined PRODUCT_TYPE_A && defined TWS_ESCO_RELAY
	return MMI_Share_SendVCmdAudioQosSetup(linkIndex, serviceType, flowStatus, rxFull);
#else
	UNUSED(linkIndex);
	UNUSED(serviceType);
	UNUSED(flowStatus);
	UNUSED(rxFull);
	return FALSE;
#endif

}

PUBLIC void MMI_AIR_SaveData(void)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_SaveData();
#endif
}

PUBLIC void MMI_AIR_UartSendUnSolicitedIndication(U8 opCode, U8 XDATA_PTR dataPtr, U8 linkIndex)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_UartSendUnSolicitedIndication(opCode, dataPtr, linkIndex);
#elif defined PRODUCT_TYPE_A
	MMI_Share_UartSendUnSolicitedIndication(opCode, dataPtr, linkIndex);
#else
	UNUSED(opCode);
	UNUSED(dataPtr);
	UNUSED(linkIndex);
#endif	
}

PUBLIC BOOL MMI_AIR_Key_Event(KeyIndex keyIndex, U8 keyAction)
{
#ifdef PRODUCT_TYPE_K
	return MMI_Share_Key_Event(keyIndex, keyAction);
#elif defined PRODUCT_TYPE_A
	return MMI_Share_Key_Event(keyIndex, keyAction);
#else
	UNUSED(keyIndex);
	UNUSED(keyAction);
	return FALSE;
#endif		
}

PUBLIC void MMI_AIR_SendBGLedCmd(U8 ledDataIndex)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_SendBGLedCmd(ledDataIndex);
#elif defined PRODUCT_TYPE_A
	MMI_Share_SendBGLedCmd(ledDataIndex);
#else
	UNUSED(ledDataIndex);
#endif	
}

PUBLIC U16 MMI_AIR_CheckKeyDiscoverable(U16 keyCode)
{
#ifdef TWS_SETTINGS
	U8 linkIndex;

	if((linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		if(A2DP_IsLinkFollower(linkIndex))
		{
			#ifdef AVRCP_Profile
			MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_DISCOVERABLE);
			#endif
			keyCode = KEY_INVALID;
		}
	}
	#ifdef PRODUCT_TYPE_K
	else if(0 == MMI_LinkKey_GetAirLinkPDLNumber())
	{
		keyCode = KEY_INVALID;
	}
	else if(gMMI_nvram.twsKChannel == NVRAM_PROFILE_RIGHT)
	{
		keyCode = KEY_INVALID;
	}
	#endif
#endif
	return keyCode;	
}

PUBLIC void MMI_AIR_WriteSecurityModeComplete(void)
{
#ifdef PRODUCT_TYPE_K	
	MMI_Share_WriteSecurityModeComplete();
#endif
}

PUBLIC void MMI_AIR_WriteDiscoverabilityMode(U8 discoverabilityMode)
{
#ifdef PRODUCT_TYPE_K	
	MMI_Share_WriteDiscoverabilityMode(discoverabilityMode);
#else
	UNUSED(discoverabilityMode);
#endif	
}
PUBLIC void MMI_AIR_EnterIncomingState(U8 linkIndex)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_EnterIncomingState(linkIndex);
#else
	UNUSED(linkIndex);
#endif	
}

PUBLIC U16 MMI_AIR_CheckKeyPowerOff(U16 keyCode)
{
	#ifdef AVRCP_Profile
	if(MAX_MULTI_POINT_NO != MMI_AIR_SendKeyPowerOffToAirDevice())
	{
		keyCode = KEY_INVALID;
	}
	#endif
	
	return keyCode;
}

PUBLIC void MMI_AIR_KeyRoleChange(U8 linkIndex)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_KeyRoleChange(linkIndex);
#else
	UNUSED(linkIndex);
#endif	
}

PUBLIC void MMI_AIR_SaveHistory(U8 linkIndex)
{
    UNUSED(linkIndex);	
#ifdef PRODUCT_TYPE_K 
	MMI_Share_SaveData();
#endif
}

#endif //AIR_MODULE

#ifdef PEQ_ENABLE
PUBLIC void MMI_AIR_SetPEQParaCmd(U8 linkIndex, U8 peqMode, U8 peqModeIndex, U8 sectorMode)
{
	#ifdef PRODUCT_TYPE_K
	MMI_Share_SetPEQParaCmd(linkIndex, peqMode, peqModeIndex, sectorMode);
	#elif defined PRODUCT_TYPE_A 
	MMI_Share_SetPEQParaCmd(linkIndex, peqMode, peqModeIndex, sectorMode);
	#else
	UNUSED(linkIndex);
	UNUSED(peqMode);
	UNUSED(peqModeIndex);
	UNUSED(sectorMode);
	#endif
}

PUBLIC void MMI_AIR_RealtimePEQCmd(U8 linkIndex, U8 XDATA_PTR para, U8 enable_mode)
{
	#ifdef PRODUCT_TYPE_K
	MMI_Share_RealtimePEQCmd(linkIndex, para, enable_mode);
	#elif defined PRODUCT_TYPE_A 
	MMI_Share_RealtimePEQCmd(linkIndex, para, enable_mode);
	#else
	UNUSED(linkIndex);
	UNUSED(para);
	UNUSED(enable_mode);
	#endif
}
#endif

PUBLIC void MMI_AIR_AirohaL2capHandler(U8 linkIndex, U8 XDATA_PTR ptr1)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_AirohaL2capHandler(linkIndex, ptr1);
#elif defined PRODUCT_TYPE_A 
	MMI_Share_AirohaL2capHandler(linkIndex, ptr1);
#else
	UNUSED(linkIndex);
	OSMEM_Put(ptr1);
#endif
}

PUBLIC void MMI_AIR_WriteLocalBdAddrCpl(BOOL isSuccess)
{
#ifdef PRODUCT_TYPE_K
	MMI_Share_WriteLocalBdAddrCpl(isSuccess);
#else
	UNUSED(isSuccess);
#endif
}

PUBLIC BOOL MMI_AIR_CheckPairingTimeOutTimerExist(void)
{
	#ifdef TWS_SETTINGS
	return (gMMI_Air_ctl.stateTimeOutTimer == (OST XDATA_PTR) NULL) ? FALSE : TRUE;
	#else
	return FALSE; 
	#endif
}
