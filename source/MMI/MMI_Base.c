/////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _MMI_BASE_C_
#include "os.h"
#include "bt_config_profile.h"

#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_HFP.h"
#include "MMI_HCI.h"
#include "MMI_AVRCP.h"
#include "MMI_A2DP.h"
#include "MMI_ChgBat.h"
#include "MMI_Reconnect.h"
#include "MMI_SDAP.h"
#include "MMI_AirModule.h"
#include "MMI_Timer.h"
#include "..\GAP\GAP_interface.h"
#include "..\HC\HC_interface.h"
#include "..\LC\LC_interface.h"
#ifdef A2DP_Profile
#include "..\A2DP\A2DP_Interface.h"
#include "..\AVRCP\AVRCP_Interface.h"
#endif
#include "..\Driver\Audiocontrol.h"
#include "MMI_SectorManager.h"
#include "..\L2CAP\L2CAP_Interface.h"
#ifdef LE_SUPPORTED_HOST_FLASH
#include "..\MMI_LE\MMI_LE_interface.h"
#include "drv_sector_flash.h"
#endif
#ifndef MMI_LE_ENABLE_ADV
#include "MMI_LE_ROM_interface.h"
#include "..\MMI_LE\link_index_ctl.h"
#endif

#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_iosys_evt_func[API_GRP_IOSYS_CB_CNT];
extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];

PUBLIC U16 MMI_GetConnHandleByLinkIndex(U8 linkIndex)
{
	if(linkIndex < MAX_MULTI_POINT_NO)
	{
		return gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle;
	}
	return 0;
}

PUBLIC U8 XDATA_PTR MMI_GetBdAddrByLinkIndex(U8 linkIndex)
{
	if(linkIndex < MAX_MULTI_POINT_NO)
	{
		return (U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr;
	}
	return (U8 XDATA_PTR)NULL;
}

PUBLIC void MMI_SystemOff(void)
{
	U8 i;

	#if 1
	LightDbgPrint("MMI - PwrOff CheckPoint :%d", (U8) MMI_IsChargerIn());
	OS_delay_ms (100);
	#endif
	DRIVER_TurnOffPureOn();
	MMI_SECTOR_SaveData(SAVE_DATA_TYPE_POWER_OFF);

	MMI_SetKeyState(MMI_KEY_STATE_NONE);

	if (MMI_POWER_KEY_TO_DEEP_SLEEP_FEAT)
	{
        DRV_PMU.fDeepSleep();
		//TBW
	}
	else
	{
		if (MMI_IsChargerIn())
		{
			OS_ENTER_CRITICAL ();
			i = PMU_SYS_GET_BOOT_SW_STATUS_B1 ();
			i &= ~SYS_SW1_BOOT_FLAG_POWER_ON_READY;
			PMU_SYS_SET_BOOT_SW_STATUS_B1 (i);//clear all preview variable in 3 wire register,  for next clean boot-up;
			OS_EXIT_CRITICAL ();

		}
		else
		{
			//There are following reasons causing power off fail.
			//1. User press pio_0: mcu will shut down in 1xx us.
			//2. charger in: mcu will go into shut down. (CHG abnormal state)

			OS_ENTER_CRITICAL ();
				PMU_SYS_SET_BOOT_SW_STATUS_B1 (0);//clear all preview variable in 3 wire register,  for next clean boot-up;
				OS_delay_us(150);//wait for 32K over 4 clocks
			OS_EXIT_CRITICAL ();

			if (MMI_RESET_INSTEAD_OF_POWEROFF_FOR_REGRESSION_TEST)
			{
				MMI_Reset();
			}
			else
			{
				//turn off power
				DRV_PMU.fPowerOff();
			}
		}
		MMI_SendInternalCmdToMMI(MMI_PWROFF_CMD);

	}
	SetPowerSavingForDevice(TRUE, PWRSAV_FAKEON);
}

PUBLIC void MMI_SendInternalCmdToMMI(U8 opcode)
{
	MMIMsgType XDATA_PTR msgPtrToMMI;

	//send Internal cmd to mmi itself to do power on/off routine.
	msgPtrToMMI = MMI_GetEventMessageMem(opcode);
	OSMQ_Put (OSMQ_MMI_Internal_ptr, (U8 XDATA_PTR)msgPtrToMMI);
}

PUBLIC void MMI_PowerOffProce(void)
{
	U8 linkIndex;

	MMI_SetWatchDogReboot(FALSE);
	MMI_CleanNestState(MMI_EOF);
	gSYS_init_charger_exist = FALSE;
	gSYS_init_wakeupkey_exist = FALSE;
	gSYS_init_led_exist = FALSE;

	if (MMI_CHGBAT_GetBatStatus() >= MMI_BAT_CHARGER_IN)
	{//Charging battery Now
		MMI_AddTopState(MMI_EOF, MMI_FAKEOFF);
	}
	else
	{
		SetPowerSavingForDevice(FALSE, PWRSAV_PWRON);
		MMI_AddTopState(MMI_EOF, MMI_OFF);
	}
	MMI_SET_POWER_ON_STATUS(FALSE);
	//Release current running timers.
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.pairModeTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.disPowerOffAfterPowerOnTime);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.batteryMonitoringTimer);
	SYS_ReleaseTimer(&gMMI_ctl.audioDevCtl.resumeTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.rssiReadIntervalTimer);
	MMI_Reconnect_ClearCtl();
	MMI_LinkKey_ClearConnectedFlag();
	gMMI_ctl.audioDevCtl.currentActiveDev = 0;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		gMMI_ctl.mmiInfo[linkIndex].subState = MMI_NONE_SUBSTATE;
		gMMI_ctl.mmiInfo[linkIndex].stateEntryExit = 0;
	}
}


PUBLIC void MMI_PowerOffHandler(U8 reason)
{
	LightDbgPrint("==MMI_PowerOffHandler:%d",(U8)reason);
#ifdef AIR_MODULE
	if(MMI_AIR_PowerOffHandler(reason))
		return;
#endif

	if(!MMI_IS_POWER_ON())
	{
		return;
	}
	if(MMI_CheckInFMState())
	{
		MMI_DRV_AudioDeviceDeRegister(MMI_EOF, AUDIO_DEVICE_FM);
		MMI_RmvState(MMI_EOF, MMI_FM);
	}
	if(MMI_CheckInLineInState())
	{
		MMI_DRV_AudioDeviceDeRegister(MMI_EOF, AUDIO_DEVICE_LINE_IN);
		MMI_RmvState(MMI_EOF, MMI_LINE_IN);
	}

	if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
	{
		return;
	}

	MMI_SetKeyState(MMI_KEY_STATE_POWER_OFF);

	MMI_ConnectabilityHandle(CMD_SYSTEM_OFF);
	MMI_DiscoverabilityHandle(CMD_SYSTEM_OFF);
    #if defined(LE_SUPPORTED_HOST_FLASH) && defined(MMI_LE_ENABLE)
    MMI_LE_DisconnectPowerOff();
    #endif

	#ifdef SUPPORT_MIC_IN_A2DP_MODE
	if(MMI_GetAudioTransparencyEnable())
		MMI_DRV_Switch_AudioTransparency(FALSE);
	#endif
	MMI_SendDSPCmd(DEFAULT_LINK_IND, DSP_DISABLE_BY_POWER_OFF);

	MMI_CancelCreateAcl();
	MMI_Reconnect_ClearCtl();

	if (MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.lowBatVolt.shutDownLowBat))
	{
	 	MMI_PushMediaEvent(MEDIA_EVT_POWER_OFF_IN_SHUTDOWN_LOW_BAT);
	}
	else
	{
		switch(MMI_BAT_GetLevelInQuater())
		{
			case IND_BATTERY_LOW:
			case IND_BATTERY_INTERVAL_0:
				MMI_PushMediaEvent(MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT1);
				break;
			case IND_BATTERY_INTERVAL_1:
				MMI_PushMediaEvent(MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT2);
				break;
			case IND_BATTERY_INTERVAL_2:
				MMI_PushMediaEvent(MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT3);
				break;
			case IND_BATTERY_INTERVAL_3:
				MMI_PushMediaEvent(MEDIA_EVT_POWER_OFF);
				break;
			default:
				ASSERT(FALSE);
				break;
		}
	}

	if(api_iosys_evt_func[API_GRP_IOSYS_CB_SYS_POWER_OFF])
	{
		((API_IOSYS_SYS_POWER_OFF_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_SYS_POWER_OFF])();
	}

	if (gMMI_ctl.activeLinkRecord)
	{
		MMI_AddTopState(MMI_EOF, MMI_DETACHING_LINK);
		MMI_ReleaseMultiLink();
	}
	else
	{
		MMI_PowerOffProce();
	}
}

PRIVATE BOOL MMI_SetActionWhenPowerOnInLowBat(U8 actionValue )
{
	if(actionValue == ACTION_POWER_ON_PROHIBITED)
		return TRUE;

	if(actionValue == ACTION_POWER_ON_NO_CONNECTIVITY_UNTIL_BAT_OK || actionValue == ACTION_POWER_ON_NO_CONNECTIVITY_NO_VALID_KEYS_UNTIL_BAT_OK)
		MMI_DRV_SetNoConnectivityPwOnLowBat(TRUE);

	if(actionValue == ACTION_POWER_ON_NO_VALID_KEYS_UNTIL_BAT_OK|| actionValue == ACTION_POWER_ON_NO_CONNECTIVITY_NO_VALID_KEYS_UNTIL_BAT_OK)
		MMI_DRV_SetKeyInvalidPwOnLowBat(TRUE);

	return FALSE;
}

PUBLIC void MMI_ClearActionOfPowerOnInLowBat(void)
{
	if(MMI_DRV_IsNoConnectivityPwOnLowBat())
	{
		MMI_DRV_SetNoConnectivityPwOnLowBat(FALSE);
		MMI_ConnectabilityHandle(CMD_SET_CONNECTABLE);
		MMI_DiscoverabilityHandle(CMD_SET_RESUME_DISCOVERABLE);
	}
	if(MMI_DRV_IsKeyInvalidPwOnLowBat())
	{
		MMI_DRV_SetKeyInvalidPwOnLowBat(FALSE);
		DRIVER_ResetLineInKeyTimer();
	}
}

PUBLIC void MMI_PowerOnLoader(void)
{
	U8 i;

	if(MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.lowBatVolt.lowBatLevel))
	{
		if(MMI_SetActionWhenPowerOnInLowBat((MMI_IsChargerIn())? gMMI_nvram.powerOnLowBatAction.powerOnInLowBatChargerIn:gMMI_nvram.powerOnLowBatAction.powerOnInLowBat))
			return;
	}

	OS_ENTER_CRITICAL ();
	i = PMU_SYS_GET_BOOT_SW_STATUS_B1 ();
	i |= SYS_SW1_BOOT_FLAG_POWER_ON_READY;
	PMU_SYS_SET_BOOT_SW_STATUS_B1 (i);
	OS_EXIT_CRITICAL ();

#if defined(LE_SUPPORTED_HOST_FLASH) && defined(MMI_LE_ENABLE)
#ifndef MMI_LE_ENABLE_ADV   //if MMI_LE_ENABLE_ADV not define, enable LE adv after power on
    if(pMpParameterF->leBDAddr.type == LE_PUBLIC_DEVICE_ADDR)
    {
        MMI_LE_SetState(MMI_LE_ROM_CONFIG, MMI_LE_ROM_ADV_PARAM_SEND_CMD);
    }
    else
    {
        MMI_LE_SetState(MMI_LE_ROM_CONFIG, MMI_LE_ROM_SET_RANDOM_ADDRESS_SEND_CMD);
    }
    MMI_PowerSaving(FALSE, MMI_PWRSAVING_ENABLE_ADV, MAX_MULTI_POINT_NO);
#endif
#endif

	SetPowerSavingForDevice(TRUE, PWRSAV_PWRON);
	MMI_PowerSaving(FALSE, MMI_PWRSAVING_WRITE_CONNECTABILITY, MAX_MULTI_POINT_NO);
	SetPowerSavingForDevice(TRUE, PWRSAV_FAKEON);


	MMI_SET_POWER_ON_STATUS(TRUE);
	MMI_SetKeyState(MMI_KEY_STATE_POWER_ON);

	if (gMMI_nvram.timerCtl.DisPowerOffAfterPowerOnTime)
	{
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.disPowerOffAfterPowerOnTime, (U32)(gMMI_nvram.timerCtl.DisPowerOffAfterPowerOnTime) * ONE_SEC);
	}

	if (MMI_CheckNestState(MMI_EOF, MMI_FAKEOFF))
	{
		MMI_RmvState(MMI_EOF, MMI_FAKEOFF);
	}

	MMI_GAP_WritePairingMode();
	MMI_GAP_WriteSecurityMode();
	MMI_HCI_SendWriteSimplePairingDebugMode(MMI_DEBUG_PRIVATE_KEY_FEAT);

	//Not watch dog reset
	if(!gMMI_ctl.mmiTimerInfo.sysTimer.watchDogRebootNoMediaTimer)
	{
		//Play startup media
		switch(MMI_BAT_GetLevelInQuater())
		{
			case IND_BATTERY_LOW:
			case IND_BATTERY_INTERVAL_0:
				MMI_PushMediaEvent(MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT1);
				break;
			case IND_BATTERY_INTERVAL_1:
				MMI_PushMediaEvent(MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT2);
				break;
			case IND_BATTERY_INTERVAL_2:
				MMI_PushMediaEvent(MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT3);
				break;
			case IND_BATTERY_INTERVAL_3:
				MMI_PushMediaEvent(MEDIA_EVT_POWER_ON);
				break;
			default:
				ASSERT(FALSE);
				break;
		}
		MMI_LinkKey_ClearConnectedFlag();
	}
	//Added by Eric Chou, 2011.1.13
	//New Feature - "MMI_INIT_BAT_CHECK_ON_POWER_UP_FEAT"
	//Send media event of current battery level indication on power up.
	if (MMI_INIT_BAT_CHECK_ON_POWER_UP_FEAT)
	{
		#ifdef MMI_BATCHG_ENABLE
		MMI_CheckCurrentBatLevel();
		#endif
	}
	//0 means diabling batteryMonitoringTimer.
	if (gMMI_nvram.timerCtl.BatteryMonitoringPeriod)
	{
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.batteryMonitoringTimer, (U32)(gMMI_nvram.timerCtl.BatteryMonitoringPeriod) * ONE_SEC);
	}

	MMI_DRV_SetSCOVolValue(DEFAULT_LINK_IND, DEFAULT_SCO_SOUND_LEVEL, DEFAULT_IDLE_SMALL_STEP);

	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoPowerOnTimer);

	DRIVER_ResetLineInKeyTimer();

	if(MMI_ENTER_DISCOVER_AFTER_PWR_ON_FEAT)
		MMI_SetEnterDiscoverable(TRUE);
}

PUBLIC void MMI_ReleaseMultiLink(void)
{
	U8 i;

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		#ifdef HFP_Profile
		MMI_HFP_StopIncomingVPRing(i);
		#endif
		MMI_ReleaseProfileLinkAndDetach(i);
	}
}

// Sys_Init --> register
static MMI_PROFILE_NODE XDATA_PTR gMMI_ProfileNodeList;

PUBLIC void MMI_RegisterProfile(MMI_PROFILE_NODE XDATA_PTR profileNode)
{
	profileNode->nextNode = gMMI_ProfileNodeList;
	gMMI_ProfileNodeList = profileNode;
	#ifdef DBG_UART_ENABLE_MMI
	LightDbgPrint("MMI BuiltIn:%x",(U8)profileNode->profileDesc->profileId);
	#endif
}

PUBLIC MMI_PROFILE_DESC CODE_PTR MMI_GetProfileDesc(U8 profileId)
{
	MMI_PROFILE_DESC CODE_PTR profileDesc = (MMI_PROFILE_DESC CODE_PTR)NULL;
	MMI_PROFILE_NODE XDATA_PTR profileNode = gMMI_ProfileNodeList;
	while (profileNode)
	{
		if(!profileNode->profileDesc)
		{
			ASSERT(FALSE);
		}
		if (profileNode->profileDesc->profileId == profileId)
		{
			profileDesc = profileNode->profileDesc;
			break;
		}
		profileNode = profileNode->nextNode;
	}
	return profileDesc;
}


static MMI_PROFILE_INFO XDATA_PTR gMMI_LoadedProfileInfoList[MAX_MULTI_POINT_NO];

PUBLIC U8 MMI_GetConnectedProfileCount(U8 linkIndex, BOOL connectedEvtOnly)
{
	MMI_PROFILE_INFO XDATA_PTR profileCurr;
	U8 i = 0;

	for (profileCurr = gMMI_LoadedProfileInfoList[linkIndex]; profileCurr ; profileCurr = profileCurr->nextProfile)
	{
		if (!connectedEvtOnly || profileCurr->profileDesc->profileId != PROFILE_SDAP)
		{
			if(profileCurr->isAlive)
				i++;
		}
	}
	return i;
}

PUBLIC BOOL MMI_IsProfileConnected(U8 linkIndex, U8 profileId)
{
	MMI_PROFILE_INFO XDATA_PTR profileCurr = gMMI_LoadedProfileInfoList[linkIndex];
	while (profileCurr)
	{
		if (profileCurr->profileDesc->profileId == profileId)
		{
			return (profileCurr->isAlive)?TRUE:FALSE;
		}
		profileCurr = profileCurr->nextProfile;
	}
	return FALSE;
}

PUBLIC BOOL MMI_IsProfileIdLoaded(U8 linkIndex, U8 profileId)
{
	MMI_PROFILE_INFO XDATA_PTR profileCurr = gMMI_LoadedProfileInfoList[linkIndex];
	while (profileCurr)
	{
		if(!profileCurr->profileDesc)
		{
			ASSERT(FALSE);
		}

		if (profileCurr->profileDesc->profileId == profileId)
		{
			return TRUE;
		}
		profileCurr = profileCurr->nextProfile;
	}
	return FALSE;
}

PUBLIC BOOL MMI_IsProfileInfoLoaded(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	MMI_PROFILE_INFO XDATA_PTR profileCurr = gMMI_LoadedProfileInfoList[linkIndex];
	while (profileCurr)
	{
		if (profileCurr == profileInfo)
		{
			return TRUE;
		}
		profileCurr = profileCurr->nextProfile;
	}
	return FALSE;
}

PRIVATE void MMI_CheckProfileReconnectTimer(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 i = OSMQ_Entries(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);
	while(i--)
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);

		if(OST_TimerExpired(&msgPtr->msgBodyPtr.profileReconnectEvt.reconnectTimer))
		{
			if(!MMI_IsProfileIdLoaded(linkIndex, msgPtr->msgBodyPtr.profileReconnectEvt.profileId) &&
				!MMI_IsProfileConnected(linkIndex, msgPtr->msgBodyPtr.profileReconnectEvt.profileId))
			{
				MMI_ConnectProfile(linkIndex, msgPtr->msgBodyPtr.profileReconnectEvt.profileId);
			}
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		}
		else
		{
			OSMQ_Put(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect, (U8 XDATA_PTR)msgPtr);
		}
	}
}

PUBLIC void MMI_ClearProfileInReconnectList(U8 linkIndex, U8 profileId)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 i = OSMQ_Entries(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);
	while(i--)
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);
		if(msgPtr->msgBodyPtr.profileReconnectEvt.profileId == profileId)
		{
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
			break;
		}
		OSMQ_Put(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void MMI_ProfileHandle(void)
{
	U8 linkIndex;
	MMI_PROFILE_INFO XDATA_PTR profileCurr;
	MMI_PROFILE_INFO XDATA_PTR profileNext;

	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		profileCurr = gMMI_LoadedProfileInfoList[linkIndex];
		while (profileCurr)
		{
			profileNext = profileCurr->nextProfile;

			if(!profileCurr->profileDesc)
			{
				ASSERT(FALSE);
			}

			if(profileCurr->profileDesc->ProfileTaskFunc)
			{
				profileCurr->profileDesc->ProfileTaskFunc(linkIndex);
			}

			profileCurr = profileNext;
		}
		MMI_CheckProfileReconnectTimer(linkIndex);
	}
}

//connecting --> Load
PUBLIC void MMI_LoadProfile(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo, MMI_PROFILE_DESC CODE_PTR profileDesc)
{
	if(MMI_IsProfileInfoLoaded(linkIndex, profileInfo))
	{
		ASSERT(FALSE);
	}

	profileInfo->profileDesc = profileDesc;
	profileInfo->state = PROFILE_CONNECTING;

	profileInfo->nextProfile = gMMI_LoadedProfileInfoList[linkIndex];
	gMMI_LoadedProfileInfoList[linkIndex] = profileInfo;
	LightDbgPrint("Load[%d]:%x", (U8)linkIndex, (U8)profileInfo->profileDesc->profileId);
}

PUBLIC MMI_PROFILE_DESC CODE_PTR MMI_UnloadProfile(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	U8 LDATA connProfileId;
	MMI_PROFILE_INFO XDATA_PTR profileCurr, XDATA_PTR profilePrev;

	#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
	if (profileInfo->profileDesc->profileId != PROFILE_SDAP)
	{
		MMI_SequentialActiveProfileConnection(linkIndex);
	}
	#endif

	profilePrev = profileCurr = gMMI_LoadedProfileInfoList[linkIndex];

	while (profileCurr)
	{
		if (profileCurr == profileInfo)
		{
			LightDbgPrint("UnLoad[%d]:%x", (U8)linkIndex, (U8)profileInfo->profileDesc->profileId);
			if(!profileCurr->profileDesc)
			{
				ASSERT(FALSE);
			}

			if(profileCurr->isAlive)
			{
				connProfileId = profileCurr->profileDesc->profileId;
				MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_PROFILE_DISCONNECTED, (U8 XDATA_PTR)&connProfileId, linkIndex);
				if(api_profile_func[API_GRP_PF_CB_CONNECTION_STATUS])
				{
					((API_PROFILE_CONNECTION_STATUS_FUNC)api_profile_func[API_GRP_PF_CB_CONNECTION_STATUS])(linkIndex, connProfileId, FALSE);
				}
			}

			if (profilePrev == profileCurr)
			{
				gMMI_LoadedProfileInfoList[linkIndex] = profileCurr->nextProfile;
			}
			else
			{
				profilePrev->nextProfile = profileCurr->nextProfile;
			}

			profileCurr->nextProfile = NULL;
			profileCurr->isAlive = FALSE;
			return profileCurr->profileDesc;
		}
		profilePrev = profileCurr;
		profileCurr = profileCurr->nextProfile;
	}
	return (MMI_PROFILE_DESC CODE_PTR)NULL;
}


PUBLIC void MMI_SetConnected(U8 linkIndex)
{
	BOOL isInConnected;

	#ifdef AIR_MODULE
	MMI_AIR_SetConnected(linkIndex);
	#endif

	MMI_DRV_AddServiceLink(linkIndex);

	////Load link history info which was set in EEPROM
	MMI_LinkKey_LoadHistory(linkIndex);
	MMI_LinkKey_UpdateAGInd(linkIndex);

	if(MMI_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && MMI_CheckInLineInState())
	{
		MMI_RmvState(MMI_EOF, MMI_LINE_IN);
	}

#ifdef DEMOSOUND
	if(MMI_IsNormalLink(linkIndex))
	{
		MMI_A2DP_KeyDemoSoundPause();
	}
#endif

	isInConnected = (MMI_CheckNestState(linkIndex, MMI_CONNECTED)) ? TRUE : FALSE;

	MMI_InsertState(linkIndex, MMI_CONNECTED, MMI_CheckNestState(linkIndex, MMI_CONDISCABLE) ? MMI_CONDISCABLE : MMI_CONNECTABLE);

	if(MMI_CountConnectedDev() >= gMMI_ctl.currentSupportDeviceNo)
	{
		MMI_SetEnterDiscoverable(FALSE);
		if(MMI_CheckNestState(MMI_EOF, MMI_CONDISCABLE))
			MMI_RmvState(MMI_EOF, MMI_CONDISCABLE);
	}

	MMI_SniffEnable(linkIndex, MMI_SNIFF_ACL_READY_SLC_NOT_YET);

	if(!MMI_NO_RING_VP_WHEN_ACTIVE_CONN_SUCCESS_FEAT || !MMI_IsLinkActiveConnected(linkIndex))
	{
		if(!isInConnected)
		{
		    MMI_DRV_SetOTAVoicepromptsExist(TRUE);
			MMI_PushMediaEvent((U8)(MEDIA_EVT_SLC_CONNECTED+MMI_CountConnectedDev()-1));
		}
	}

	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.QosServiceType != SERVICE_TYPE_GUARANTEED)
	{
		MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
	}

	MMI_PowerSaving(TRUE, MMI_PWRSAVING_DISC, linkIndex);

	#ifdef AIR_MODULE
	MMI_AIR_SetConnected(linkIndex);
	MMI_AIR_ResumePairing();
	#endif
}

//connected --> Load and Activate
PUBLIC void MMI_ActivateProfile(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	U8 LDATA connProfileId;
	U8 profileCount;

	if(!MMI_IsProfileInfoLoaded(linkIndex, profileInfo))
	{
		ASSERT(FALSE);
	}
	if(!profileInfo->profileDesc)
	{
		ASSERT(FALSE);
	}
	profileInfo->isAlive = TRUE;
	MMI_ClearProfileInReconnectList(linkIndex, profileInfo->profileDesc->profileId);

	connProfileId = profileInfo->profileDesc->profileId;
	MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_PROFILE_CONNECTED, &connProfileId, linkIndex);

	if(api_profile_func[API_GRP_PF_CB_CONNECTION_STATUS])
	{
		((API_PROFILE_CONNECTION_STATUS_FUNC)api_profile_func[API_GRP_PF_CB_CONNECTION_STATUS])(linkIndex, connProfileId, TRUE);
	}

	if(profileInfo->profileDesc->profileId == PROFILE_SDAP)
	{
		#if 1 //mantis 9183: delay the reconnect success condition until profile connection success
		if (gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT || (gMMI_ctl.reconnectCtl.reconnectListPtr->currentTargetIndex != 0xFF ||
			gMMI_ctl.reconnectCtl.reconnectListPtr->currentHighPriorityIndex != 0xFF))
		{
			MMI_Reconnect_DeleteInvalidList();
			MMI_Reconnect_SetDelayTimer((U32)gMMI_nvram.timerCtl.ReConnectDelayTimer* ONE_SEC);
		}
		#endif
		return;
	}

	#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
	MMI_SequentialActiveProfileConnection(linkIndex);
	#endif

	gMMI_ctl.latestConnectedLinkInd = linkIndex;

	profileCount = MMI_GetConnectedProfileCount(linkIndex, TRUE);
	// Any profile is connected we call this is in connected state
	if(profileCount == 1)
	{
		//Mantis 9926 change role switch timing into profile active
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delayRoleSwitchTimer, 5 * HALF_SEC);
		switch(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER))
		{
			case PRODUCT_AIR_SCO_WOOFER:
			case PRODUCT_AIR_SOUNDBAR:
			case PRODUCT_AIR_TWS:
			case PRODUCT_AIR_A2DP_WOOFER:
				if(MMI_SDAP_IsQueryComplete(linkIndex))
				{
					MMI_SetConnected(linkIndex);
				}
				break;
			default:
				if(gMMI_nvram.firstConnectProfile)
				{
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delaySDPTimer, 320L);
				}
				MMI_SetConnected(linkIndex);
				break;
		}

		if (gMMI_nvram.timerCtl.RSSIReadIntervalPeriod && (gMMI_ctl.mmiTimerInfo.sysTimer.rssiReadIntervalTimer == (OST XDATA_PTR)NULL))
		{
			SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.rssiReadIntervalTimer, (U32)(gMMI_nvram.timerCtl.RSSIReadIntervalPeriod) * ONE_SEC);
		}
	}
	MMI_TIMER_ReleaseDetachTimer(linkIndex);
}

PUBLIC void MMI_ClearProfileReconnectListQueue(U8 linkIndex)
{
	OSMQ_FlushAndReleaseMemoryBlock(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);
}

PUBLIC BOOL MMI_IsProfileInReconnectList(U8 linkIndex, U8 profileId)
{
	MMIMsgType XDATA_PTR msgPtr;
	BOOL found = FALSE;
	U8 i = OSMQ_Entries(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);

	while(i--)
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);
		if( msgPtr->msgBodyPtr.profileReconnectEvt.profileId == profileId)
		{
			found = TRUE;
		}
		OSMQ_Put(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect, (U8 XDATA_PTR)msgPtr);
	}
	return found;
}

PUBLIC void MMI_SetReconnectProfile(U8 linkIndex, U8 profileId, U32 waitingTime)
{
	MMIMsgType XDATA_PTR msgPtr;
	BOOL found = FALSE;
	U8 i = OSMQ_Entries(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);

	while(i--)
	{
		msgPtr = OSMQ_Get(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect);
		if(msgPtr->msgBodyPtr.profileReconnectEvt.profileId == profileId)
		{
			found = TRUE;

			if(msgPtr->msgBodyPtr.profileReconnectEvt.retryCnt < gMMI_nvram.reConnectInfo.profileRetryCnt)
			{
				msgPtr->msgBodyPtr.profileReconnectEvt.retryCnt++;
				OST_SetTimer(&msgPtr->msgBodyPtr.profileReconnectEvt.reconnectTimer, waitingTime);
				OSMQ_Put(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect, (U8 XDATA_PTR)msgPtr);
			}
			else
			{
				OSMEM_Put((U8 XDATA_PTR)msgPtr);
			}
			break;
		}
		OSMQ_Put(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect, (U8 XDATA_PTR)msgPtr);
	}

	if(!found)
	{
		msgPtr = MMI_GetMMIMsgTypeCmdMemory(MMI_EOF);
		if(msgPtr)
		{
			msgPtr->msgBodyPtr.profileReconnectEvt.profileId = profileId;
			msgPtr->msgBodyPtr.profileReconnectEvt.retryCnt = 1;
			OST_SetTimer(&msgPtr->msgBodyPtr.profileReconnectEvt.reconnectTimer, waitingTime);
			OSMQ_Put(&gMMI_ctl.mmiInfo[linkIndex].linkPara.OSMQ_Profile_Reconnect, (U8 XDATA_PTR)msgPtr);
		}
	}
}

PUBLIC U8 MMI_DisconnectLoadedProfile(U8 linkIndex)
{
	MMI_PROFILE_INFO XDATA_PTR profileCurr;
	MMI_PROFILE_INFO XDATA_PTR profileNext = gMMI_LoadedProfileInfoList[linkIndex];

	while(profileCurr = profileNext)
	{
		LightDbgPrint("ActiveDisc[%d]:%x", (U8)linkIndex, (U8)profileCurr->profileDesc->profileId);
		profileNext = profileCurr->nextProfile; /*save next first, because profile object might be clear */
		if(CMD_WAITING == MMI_DisconnectProfile(linkIndex, profileCurr))
			return CMD_WAITING;
	}
	return CMD_COMPLETE;
}

PUBLIC U8 MMI_DisconnectSpecificProfile(U8 linkIndex, U8 profileId)
{

	MMI_PROFILE_INFO XDATA_PTR profileCurr = gMMI_LoadedProfileInfoList[linkIndex];

	while (profileCurr)
	{
		if ((profileCurr->profileDesc->profileId == profileId) && (profileCurr->isAlive))
		{
			if(CMD_WAITING == MMI_DisconnectProfile(linkIndex, profileCurr))
				return CMD_WAITING;
		}
		profileCurr = profileCurr->nextProfile;
	}
	return CMD_COMPLETE;
}

PUBLIC BOOL MMI_IsLinkActiveDisconnecting(U8 linkIndex)
{
	if(linkIndex >= MAX_MULTI_POINT_NO || !MMI_IsActiveLink(linkIndex))
		return FALSE;

	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_DISC_ACTIVE)
		return TRUE;

	return FALSE;
}

PUBLIC void MMI_AllL2CAPChannelsClosedNotify(U8 linkIndex)
{
	if(MMI_IsLinkActiveDisconnecting(linkIndex))
	{
		MMI_ReleaseProfileLinkAndDetach(linkIndex);
	}
}

PUBLIC void MMI_ReleaseProfileLinkAndDetach(U8 linkIndex)
{
	if(linkIndex >= MAX_MULTI_POINT_NO)
		return;

	if(!MMI_IsActiveLink(linkIndex))
	{
		switch(MMI_GetConnectionState(linkIndex))
		{
			case MMI_CONN_STATE_WAITING_PASS:
				MMI_SetConnectionState(linkIndex, MMI_CONN_STATE_WAIT_FOR_DETACH);
				break;
			case MMI_CONN_STATE_WAITING_ACTIVE:
				MMI_HCI_SendCreateConnectionCancel((U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].bdAddr);
				MMI_SetConnectionState(linkIndex, MMI_CONN_STATE_WAIT_FOR_DETACH);
				break;
			default:
				if(!gMMI_ctl.activeLinkRecord && MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
				{
					MMI_PowerOffProce();
				}
				break;
		}
		return;
	}
	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_LINK_DETACHING)
		return;

	gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask |= MMI_DISC_ACTIVE;
	MMI_HCI_WriteLinkPolicySetting(linkIndex, HCI_LINK_POLICY_ENABLE_ROLE_SWITCH|HCI_LINK_POLICY_ENABLE_HOLD_MODE);
	MMI_SniffDisable(linkIndex, MMI_SNIFF_DISC);
	MMI_ClearProfileReconnectListQueue(linkIndex);
	MMI_TIMER_ReleaseConnectionTimer(linkIndex);

#ifdef AIR_MODULE
	if(MMI_AIR_ReleaseLink(linkIndex))
		return;
#endif

	MMI_HCI_SendRelSco(linkIndex);
	//release one profile at a time
	//force detach and power off if timeout
	if(CMD_WAITING == MMI_DisconnectLoadedProfile(linkIndex))
	{
		MMI_TIMER_SetDetachTimer(linkIndex, (U32)(gMMI_nvram.timerCtl.waitDetachTime) * HALF_SEC);
	}
	else if(SCO_EXISTS(linkIndex))
	{
	}
	else if(!L2CAP_IsResourceReleased(linkIndex))
	{

	}
	else if(MMI_IsActiveLink(linkIndex))
	{
		MMI_TIMER_SetDetachTimer(linkIndex, 32L);
		if(MMI_IsAllProfileDisconnected(MMI_EOF) && MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
			MMI_AddTopState(MMI_EOF, MMI_DETACHING_LINK);
	}
}

PUBLIC BOOL MMI_IsAllProfileDisconnected(U8 linkIndex)
{
	if(linkIndex == MMI_EOF)
	{
		for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
		{
			if(gMMI_LoadedProfileInfoList[linkIndex])
				return FALSE;
		}
	}
	else
	{
		if(gMMI_LoadedProfileInfoList[linkIndex])
			return FALSE;
	}
	return TRUE;
}

PRIVATE void MMI_RemoveConnectedState(U8 linkIndex)
{
	if(!MMI_GetConnectedProfileCount(linkIndex, TRUE))
	{
		MMI_RmvState(linkIndex, MMI_CONNECTED);
		MMI_PowerSaving(FALSE, MMI_PWRSAVING_DISC, linkIndex);
	}
}

PUBLIC void MMI_PROTOCOL_L2CAPDisconnected(U8 linkIndex, U8 reason, U8 use_enlarged_wait_detach_time)
{
	if(reason != PROFILE_LINK_EVENT_DISC_COMPLETE)
		return;

	// active release link
	if((MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState()) || MMI_IsLinkActiveDisconnecting(linkIndex))
	{
		MMI_RemoveConnectedState(linkIndex);
		MMI_ReleaseProfileLinkAndDetach(linkIndex);
	}
	else
	{
		if (!MMI_IsActiveLink(linkIndex))
			goto REMOVE_CONNECTED_IMMEDIATLY;

		if(!MMI_IsAllProfileDisconnected(linkIndex))
			return;

		if(gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].nfcDetachIfNoProfileTimer)
		{
			MMI_HCI_SendRelAcl(linkIndex);
			SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].nfcDetachIfNoProfileTimer);
			goto REMOVE_CONNECTED_IMMEDIATLY;
		}
		else
		{
			if (use_enlarged_wait_detach_time)
			{
				MMI_TIMER_SetDetachTimer(linkIndex, (U32)(gMMI_nvram.enlargedWaitDetachTime) * HALF_SEC);
			}
			else
			{
				MMI_TIMER_SetDetachTimer(linkIndex, (U32)(gMMI_nvram.timerCtl.waitDetachTime) * HALF_SEC);
			}

			if(gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delayRemoveConnectedTimer == (OST XDATA_PTR)NULL)
			{
				SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delayRemoveConnectedTimer, 640L);
			}
			return;
		}
	}
	return;

	REMOVE_CONNECTED_IMMEDIATLY:
	MMI_RemoveConnectedState(linkIndex);
}

PUBLIC void MMI_ClearMMILinkSettingType(U8 linkIndex)
{
	OSMEM_memset_xdata((U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].bdAddr, 0, 6);
	MMI_ClearProfileReconnectListQueue(linkIndex);
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].linkPara, 0, sizeof(MMILinkSettingType));
}

PUBLIC void MMI_PushMediaEvent(U16 event)
{
	U8 index;

	if(event != NO_MEDIA)
	{
		index = gMMI_ctl.mediaTriggerEventCtrl.nextPushIndex;
		gMMI_ctl.mediaTriggerEventCtrl.events[index].eventCode = event;

		gMMI_ctl.mediaTriggerEventCtrl.nextPushIndex++;
		gMMI_ctl.mediaTriggerEventCtrl.nextPushIndex %= MMI_MEDIA_EVENTS_SIZE;
	}
}

PUBLIC BOOL MMI_IsSniffAllowed(U8 linkIndex)
{
	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffProfileMask ||(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState()))
		return FALSE;

	return TRUE;
}

PUBLIC void MMI_SniffDisable(U8 linkIndex, U8 sniffProfile)
{
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].sniffTimerPtr);
	if (!MMI_IsActiveLink(linkIndex))
		return;
	MMI_HCI_SendExitSniffMode(linkIndex);
	gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffProfileMask  |= (U16_BIT_MASK(sniffProfile));
	MMI_PowerSaving(FALSE, MMI_SNIFF_DISABLE, linkIndex);
}

PUBLIC void MMI_SniffEnable(U8 linkIndex, U8 sniffProfile)
{
	if (!MMI_IsActiveLink(linkIndex))
	{
		return;
	}

	gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffProfileMask  &= (~(U16_BIT_MASK(sniffProfile)));

	if(MMI_IsSniffAllowed(linkIndex))
	{
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].sniffTimerPtr,(U32)gMMI_nvram.timerCtl.SniffTimer* 64L);
	}
}

PUBLIC void MMI_PowerSaving(BOOL isEnable, U8 pwrSavingMask, U8 linkIndex)
{
	U8 i;
	BOOL isPwrSaving = TRUE;

	if(linkIndex >= MAX_MULTI_POINT_NO)
	{
		if(isEnable)
			gMMI_ctl.pwrSavingMask &= ~pwrSavingMask;
		else
			gMMI_ctl.pwrSavingMask |= pwrSavingMask;
	}
	else
	{
		if(isEnable)
			gMMI_ctl.linkPwrSavingMask[linkIndex] &= ~pwrSavingMask;
		else
			gMMI_ctl.linkPwrSavingMask[linkIndex] |= pwrSavingMask;

	}

	if(gMMI_ctl.pwrSavingMask)
		isPwrSaving = FALSE;

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(gMMI_ctl.linkPwrSavingMask[i])
			isPwrSaving = FALSE;
	}

	SetPowerSavingForDevice(isPwrSaving, PWRSAV_MMI);
}


PUBLIC	MMIMsgType XDATA_PTR MMI_GetCommandMessageMem (U8 command_op)
{
	MMIMsgType XDATA_PTR msgPtr;

	msgPtr = (MMIMsgType XDATA_PTR)OSMEM_Get(OSMEM_ptr1);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);

		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->msgOpcode =  command_op;
	}

	return msgPtr;
}

PUBLIC	MMIMsgType XDATA_PTR MMI_GetEventMessageMem (U8 event_op)
{
	MMIMsgType XDATA_PTR msgPtr;

	msgPtr = (MMIMsgType XDATA_PTR)OSMEM_Get(OSMEM_ptr1);

	OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);

	msgPtr->dataOffset = EVT_OFFSET;
	msgPtr->eventCode = event_op;

	return msgPtr;
}

PUBLIC void MMI_Reset(void)
{
	AUDIO_ResetADDA();

	OS_delay_us(100);

	DRV_PMU.fReset();

	while(TRUE);//cannot replace with ASSERT(FALSE) because it wait reset here;
}

PUBLIC BOOL MMI_IsActiveLink(U8 linkIndex)
{
	return (gMMI_ctl.activeLinkRecord & U8_BIT_MASK(linkIndex));
}

PUBLIC void MMI_SwitchEndianBDAddr(U8 XDATA_PTR bdAddr)
{
	U8 i, temp;

	for (i = 0; i < 3; i++)
	{
		temp= bdAddr[i];
		bdAddr[i] = bdAddr[5-i];
		bdAddr[5-i] = temp;
	}
}

PUBLIC BOOL MMI_CheckBootFlag(U8 flag)
{
	U8 i;

	OS_ENTER_CRITICAL ();
	i = PMU_SYS_GET_BOOT_SW_STATUS_B1();
	OS_EXIT_CRITICAL ();
	if (i&flag)
		return TRUE;
	else
		return FALSE;
}

PUBLIC BOOL MMI_IsLinkTrafficBusy(U8 linkIndex)
{
#ifdef HFP_Profile
	if(MMI_CheckLinkInCallRelatedState(linkIndex) ||SCO_EXISTS(linkIndex))
		return TRUE;
#else
	UNUSED(linkIndex);
#endif
	return FALSE;
}

PUBLIC U8 MMI_GetNextActiveLink(U8 linkIndex)
{
#ifdef SUPPORT_MULTI_POINT
	U8 i, j;

	for(i = linkIndex+1; i < linkIndex+MAX_MULTI_POINT_NO; i++)
	{
		j = i % MAX_MULTI_POINT_NO;
		if(MMI_IsActiveLink(j))
			return j;
	}
#else
	UNUSED(linkIndex);
#endif
	return MMI_EOF;
}

PUBLIC U8 MMI_GetNextNormalLink(U8 linkIndex)
{
#ifdef SUPPORT_MULTI_POINT
	U8 i, j;

	for(i = linkIndex+1; i < linkIndex+MAX_MULTI_POINT_NO; i++)
	{
		j = i % MAX_MULTI_POINT_NO;
		if(MMI_IsActiveLink(j)&& PRODUCT_AIR_NONE == gMMI_ctl.sdapCtl[j].remoteApplicationID)
			return j;
	}
#else
	UNUSED(linkIndex);
#endif
	return MMI_EOF;
}

PUBLIC BOOL MMI_IsNormalLink(U8 linkIndex)
{
    if(MMI_IsActiveLink(linkIndex)&& PRODUCT_AIR_NONE == gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
    {
        return TRUE;
    }
    return FALSE;
}

PUBLIC BOOL MMI_IsLocalDeviceType(U8 deviceType)
{
	return(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == deviceType)?TRUE:FALSE;
}

PUBLIC BOOL MMI_IsSpeakerProduct(void)
{
	return (gMMI_nvram.productType.sinkSupportedFeatures & PRODUCT_TYPE_SPEAKER) ? TRUE : FALSE;
}

PUBLIC U8 MMI_IsPowerOn(void)
{
	return gMMI_ctl.driverCtl.isPowerOn;
}

PUBLIC U8 MMI_GetKeyState(void)
{
	return gMMI_ctl.keyState;
}

PUBLIC void MMI_SetConnectionState(U8 linkIndex, U8 connectionState)
{
	gMMI_ctl.mmiInfo[linkIndex].linkPara.connectionState = connectionState;
}

PUBLIC U8 MMI_GetConnectionState(U8 linkIndex)
{
	return gMMI_ctl.mmiInfo[linkIndex].linkPara.connectionState;
}
