//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _MMI_C_


#include "os.h"
#include "bt_config_profile.h"

#include "os_flash.h"
#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Timer.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_SDAP.h"
#include "MMI_A2DP.h"
#include "MMI_HFP.h"
#include "MMI_SectorManager.h"
#include "MMI_HCI.h"
#include "MMI_AirModule.h"
#include "MMI_AVRCP.h"
#include "MMI_FM.h"
#include "MMI_LineIn.h"
#include "MMI_ChgBat.h"
#include "MMI_TestMode.h"
#include "MMI_Reconnect.h"
#include "MMI_LED.h"
#include "..\L2CAP\L2CAP_Interface.h"
#include "..\GAP\GAP_Interface.h"
#include "..\HC\HC_Interface.h"
#include "..\LC\LC_Interface.h"
#include "..\A2DP\A2DP_Interface.h"
#ifdef SUPPORT_VOICE_PROMPT
	#include "..\Driver\VoicePrompt.h"
#endif
#ifdef OLED_ENABLE
	#include "..\Driver\OLED.h"
#endif

#ifdef SUPPORT_VOICE_COMMAND
#include "..\Driver\VoiceCommand.h"
#endif

#include "sector_voice_data.h"
#include "sector_merge_runtime.h"

#include "align_flash.h"

#ifdef TRSPX_App
#include "MMI_TrspXport.h"
#endif

#include "..\SECTOR\Config1\sector_config1.h"

#ifdef AIR_MODULE
#include "..\MMI_FUNCTION\MMI_Air_Function.h"
#endif

#ifdef HID_Profile
#include "..\HID\HID_interface.h"
#endif
#include "..\AirApp\AirApp_Interface.h"

extern void DRIVER_Init(void);

#ifdef HFP_Profile
extern void HFP_Init(void);
#endif
#ifdef SPP_Profile
extern void SPP_Init(void);
#endif
#ifdef SONGPAL_Profile
extern void SongPal_Init(void);
#endif
#ifdef LCIF_Profile
extern void LCIF_Init(void);
#endif
#ifdef IAP2_Profile
extern void IAP2_Init(void);
#endif
#ifdef AIRAPP_Profile
extern void AirApp_Init(void);
#endif
#ifdef SAP_Profile
extern void SAP_Init(void);
#endif
#ifdef PBAP_Profile
extern void PBAP_Init(void);
#endif
#ifdef OPP_Profile
extern void OPP_Init(void);
#endif
#ifdef MAP_Profile
extern void MAP_Init(void);
#endif
#ifdef AVRCP_Profile
extern void AVRCP_Init(void);
#endif
#ifdef A2DP_Profile
extern void A2DP_Init(void);
#endif
#ifdef HID_Profile
extern void HID_Init(void);
#endif

extern U32 _gOS_HEAP_START;

#ifdef LIGHTING_ENABLE
extern PUBLIC void MMI_Share_UpdateLedMode(void);
U8 Index_Cnt, Volume_Cnt;
U8 MusicYouCanSeeEnable = 1;
#endif

extern VFUN XDATA sdk_init_func;
extern VFUN XDATA sdk_polling_func;
extern VFUN XDATA sdk_pcb_func;

PUBLIC void MMI_Init (void)
{
	DRIVER_Init();
    GAP_Init();
    L2CAP_Init();

	MMI_SECTOR_MmiNvramInit();
	MMI_SECTOR_MmiDriverNvramInit();

	MMI_DRV_Init();
#ifdef AIR_MODULE
	MMI_AIR_Init();
#endif
	MMI_SDAP_Init();
	MMI_TIMER_Init();
	MMI_STATE_Init();
	MMI_PROTOCOL_Init();
#ifdef OPP_Profile
	OPP_Init();
#endif
#ifdef SAP_Profile
	SAP_Init();
#endif
#ifdef PBAP_Profile
	PBAP_Init();
#endif
#ifdef MAP_Profile
	MAP_Init();
#endif
#ifdef LCIF_Profile
	LCIF_Init();
#endif
#ifdef IAP2_Profile
	IAP2_Init();
#endif
#ifdef SONGPAL_Profile
	SongPal_Init();
#endif
#ifdef AIRAPP_Profile
	AirApp_Init();
#endif
#ifdef SPP_Profile
	SPP_Init();
#endif
#ifdef HFP_Profile
	HFP_Init();
#endif
#ifdef AVRCP_Profile
	AVRCP_Init();
#endif
#ifdef A2DP_Profile
	A2DP_Init();
#endif
#ifdef HFP_Profile
	MMI_HFP_Init();
#endif
#ifdef MMI_BATCHG_ENABLE
	MMI_CHGBAT_Init();
#endif
#ifdef FM_ENABLE
	MMI_FM_Init();
#endif
#ifdef LINEIN_ENABLE
	MMI_LineIn_Init();
#endif
#ifdef HID_Profile
    HID_Init();
#endif
#ifdef TRSPX_App
    MMI_TrspX_Init();
#endif

#ifdef AIR_MODULE
	MMI_Air_Function_Init();
#endif

	MMI_TestMode_Init();

	if(sdk_init_func)
		sdk_init_func();

	MMI_HCI_Init();

	//LightDbgPrint example
	//Normal example
	LightDbgPrint("EX:12345 Number:%d,%D,%x,%X,%h", (U8)0xFE, (U32)0x12345678, (U8)0xFD, (U32)0x9ABCDEF0, (U16)0x0123);

	LightDbgPrint("Heap Start:%X",(U32)_gOS_HEAP_START);
	LightDbgPrint("Momory top flash:%X",(U32)MEMORY_TOP_FLASH);

	if(((U32)_gOS_HEAP_START) < ((U32)MEMORY_TOP_FLASH))
	{
		LightDbgPrint("EDATA is running over!!");
		ASSERT(FALSE);
	}
}

U16 XDATA track_SP;
OST XDATA_PTR XDATA gMMI_CheckTimerPtr1;

#if OS_PRINT_OSMEM1_MSG
U32 gOS_OSMEM1_START;
PRIVATE void MMI_DBG_AlignmentCheck()
{
	if (gOS_OSMEM1_START % 2)
	{
		gOS_OSMEM1_START++;
	}
}

U8 XDATA_PTR XDATA gDBG_GETTER_ADDR_PTR[40];
PRIVATE U8 MMI_DBG_CopyMemGetterAddr(void)
{
	U8 NumBlks = pSysMemoryConfig->OSMEM1ArrayEntries;
	U32 StartOfBlk = gOS_OSMEM1_START;
	U8 blk = 0;

	while (NumBlks-- > 0)
	{
		OSMEM_BLK_PTR pBlk = (OSMEM_BLK_PTR) StartOfBlk;

		if (pBlk->header.dbg_getter_addr[0] | pBlk->header.dbg_getter_addr[1] | pBlk->header.dbg_getter_addr[2])
		{
			gDBG_GETTER_ADDR_PTR[blk++] = (U8 XDATA_PTR) pBlk->header.dbg_getter_addr;
		}

		StartOfBlk += pSysMemoryConfig->OSMEM1ArrayEntrySize + OS_ADDITIONAL_BYTES_FOR_MEMORY_ARRAY;
	}
	return blk;
}

PRIVATE void MMI_DBG_CountMemGetterAddr(U8 validBlks)
{
	U8 i, j, index = 0, count = 0, maxCount = 0;

	for (j = 0 ; j < validBlks ; j++)
	{
		for (i = 0 ; i < validBlks ; i++)
		{
			if (i != j)
			{
				if	(*gDBG_GETTER_ADDR_PTR[j] == *gDBG_GETTER_ADDR_PTR[i] &&
					 *(gDBG_GETTER_ADDR_PTR[j]+1) == *(gDBG_GETTER_ADDR_PTR[i]+1) &&
					 *(gDBG_GETTER_ADDR_PTR[j]+2) == *(gDBG_GETTER_ADDR_PTR[i]+2) )
				{
					count++;
				}
			}
		}
		if(maxCount < count)
		{
			maxCount = count;
			index = j;
		}
		count = 0;
	}

	LightDbgPrint("osmem1:%x,%x,%x, cnt:%d", (U8)*gDBG_GETTER_ADDR_PTR[index],
	  										 (U8)*(gDBG_GETTER_ADDR_PTR[index]+1),
											 (U8)*(gDBG_GETTER_ADDR_PTR[index]+2),
											 (U8) maxCount);
}

PRIVATE void MMI_DBG_Memory1Analysis(void)
{
	MMI_DBG_AlignmentCheck();

	MMI_DBG_CountMemGetterAddr(MMI_DBG_CopyMemGetterAddr());
}
#endif

PRIVATE void MMI_InterruptDebug(void)
{
	//// For system debug to see if the interrupt is working.
	if (gDEBUG_MMI1EnterCount)
	{
		U8 dataCount = 0;

		OS_ENTER_CRITICAL ();
	 	gDEBUG_MMI1EnterCount = 0;
		OS_EXIT_CRITICAL ();

		if (UpperLayer_GetL2capAclInQueue(0))
		{
			dataCount +=  OSMQ_Entries(UpperLayer_GetL2capAclInQueue(0));
		}
		if (UpperLayer_GetL2capAclInQueue(1))
		{
			dataCount += OSMQ_Entries(UpperLayer_GetL2capAclInQueue(1));
		}

		LightDbgPrint("MMI-MMI1Tick RxMem:%d,%d,%d,%d",(U8) dataCount,
											(U8) OSMEM_NFree(OSMEM_ptr2_small_RX),
											(U8) OSMEM_NFree(OSMEM_ptr2_tiny_RX),
											(U8) OSMEM_NFree(OSMEM_ptr2_large_RX));
	}

	if (gDEBUG_ScanEnterCount)
	{
		U8 dataCount = 0;

		OS_ENTER_CRITICAL ();
	 	gDEBUG_ScanEnterCount = 0;
		OS_EXIT_CRITICAL ();

		if (UpperLayer_GetL2capAclInQueue(0))
		{
			dataCount +=  OSMQ_Entries(UpperLayer_GetL2capAclInQueue(0));
		}
		if (UpperLayer_GetL2capAclInQueue(1))
		{
			dataCount += OSMQ_Entries(UpperLayer_GetL2capAclInQueue(1));
		}

		LightDbgPrint("MMI-ScanTick RxMem:%d,%d,%d,%d",(U8) dataCount,
											(U8) OSMEM_NFree(OSMEM_ptr2_small_RX),
											(U8) OSMEM_NFree(OSMEM_ptr2_tiny_RX),
											(U8) OSMEM_NFree(OSMEM_ptr2_large_RX));
	}

	if (gDEBUG_KEYEnterCount)
	{
		OS_ENTER_CRITICAL ();
	 	gDEBUG_KEYEnterCount = 0;
		OS_EXIT_CRITICAL ();
		LightDbgPrint("MMI-Key int occurs:%d,%d", (U8)gMMI_ctl.serviceInd, (U8) MMI_GetTopStateByLink(gMMI_ctl.serviceInd));
	}

	if(MMI_TEST_MODE == MMI_GetTopStateByLink(DEFAULT_LINK_IND))
		return;

	ASSERT(OSMEM_NFree(OSMEM_call_ptr) > 2);
	ASSERT(OSMEM_NFree(OSMEM_timer_ptr) > 5);

    if (!(OSMEM_NFree(OSMEM_ptr1) > 5))
    {
        OSMQ_FlushAndReleaseMemoryBlock(OSMQ_UART_TX_Data_ptr);
        #if OS_PRINT_OSMEM1_MSG
		MMI_DBG_Memory1Analysis();
		#else
		LightDbgPrint("Release OSMQ_UART_TX_Data_ptr");
		#endif
        ASSERT(OSMEM_NFree(OSMEM_ptr1) > 5);
    }

	#ifndef TWS_SETTINGS
	if (OSMEM_NFree(OSMEM_ptr2_small_TX) > 0)
	{
		SYS_SetTimer (&gMMI_CheckTimerPtr1, ONE_SEC * 5);
	}
	else
	{
		ASSERT(!SYS_IsTimerExpired(&gMMI_CheckTimerPtr1));
	}
	#endif

	if (!track_SP)
	{
		track_SP = (U16)((SPH << 8) | SP);
	}
	else
	{
		ASSERT(track_SP == (U16)((SPH << 8) | SP));
	}

}

PRIVATE void MMI_ConnectableToConDiscoveable(void)
{
	if(MMI_IsEnterDiscoverable() && gMMI_ctl.mmiInfo[DEFAULT_LINK_IND].subState >= MMI_INIT_SYS_CONFIGED)
	{
		if(!(MMI_NO_DISCAVERABLE_IN_LINE_IN_FEAT && MMI_CheckInLineInState()) && !(MMI_NO_DISCAVERABLE_IN_FM_FEAT && MMI_CheckInFMState()))
		{
			MMI_InsertState(MMI_EOF, MMI_CONNECTABLE,MMI_FAKEON);
			MMI_InsertState(MMI_EOF, MMI_CONDISCABLE,MMI_CONNECTABLE);
			MMI_SetEnterDiscoverable(FALSE);
		}
	}
}

PRIVATE U16 MMI_PopMediaEvent(void)
{
	U16  eventCode;

	eventCode = gMMI_ctl.mediaTriggerEventCtrl.events[gMMI_ctl.mediaTriggerEventCtrl.nextPopIndex].eventCode;

 	if(eventCode != NO_MEDIA)
	{
		//Clear
		gMMI_ctl.mediaTriggerEventCtrl.events[gMMI_ctl.mediaTriggerEventCtrl.nextPopIndex].eventCode = NO_MEDIA;

		gMMI_ctl.mediaTriggerEventCtrl.nextPopIndex++;
		gMMI_ctl.mediaTriggerEventCtrl.nextPopIndex %= MMI_MEDIA_EVENTS_SIZE;
	}

	return eventCode;
}

#ifdef SUPPORT_VOICE_PROMPT
static U8 XDATA gTestVoicePromptIndex;
PRIVATE U8 MMI_VoicePromptIndexMapping(U16 eventCode, U8 i)
{
	U8 voicePromptIndex = gMMI_nvram.evtParaCtl[i].VoicePromptIndex;

	if(gDriver_ctl.voiceLangCtl.vpScriptOffsetEntries == VOICE_PROMPT_INVALID)
		return VOICE_PROMPT_INVALID;

	if (eventCode == MEDIA_EVT_KEY_TEST_VOICE_PROMPT_PLAY)
	{
		voicePromptIndex = gTestVoicePromptIndex;
		if (voicePromptIndex >= gDriver_ctl.voiceLangCtl.vpScriptOffsetEntries)
		{
			voicePromptIndex -= gDriver_ctl.voiceLangCtl.vpScriptOffsetEntries - VP_EVT_SPECIAL_HANDLE_START;
		}
	}
	else if (eventCode == MEDIA_EVT_KEY_TEST_VOICE_PROMPT_NEXT)
	{
		voicePromptIndex = gTestVoicePromptIndex++;
		if (gTestVoicePromptIndex >= (gDriver_ctl.voiceLangCtl.vpScriptOffsetEntries + VP_EVT_SPECIAL_HANDLE_COUNT))
			gTestVoicePromptIndex = 0;
	}
	return voicePromptIndex;
}
#endif

PRIVATE void MMI_MediaEventHandler(void)
{
	U8 i;
	U8 mmiSate;
	U16 eventCode;

	while ((eventCode = MMI_PopMediaEvent()) != NO_MEDIA)
	{
		mmiSate = MMI_GetTopStateByLink(gMMI_ctl.serviceInd);
		//To avoid confusing media (light & ringtone) playing after the system is changing into MMI_OFF state,
		//we ignore all media events except MEDIA_EVT_POWER_OFF if it's in state MMI_OFF.
		if ( (mmiSate != MMI_OFF  && mmiSate != MMI_DETACHING_LINK) || MMI_CheckValidPowerOffMediaEvt(eventCode) ||
			eventCode == MEDIA_EVT_KEY_RESET_PAIRED_DEVICES)
		{
			// Check valid event
			if(eventCode == MEDIA_EVT_KEY_RESET_PAIRED_DEVICES && gMMI_ctl.driverCtl.resetPairedDeviceMediaCount)
			{
				continue;
			}

            if(eventCode == MEDIA_EVT_SLC_DISCONNECTED || eventCode == MEDIA_EVT_TWS_DISCONNECTED
               || eventCode == MEDIA_EVT_SLC_CONNECTED || eventCode == MEDIA_EVT_2_SLC_CONNECTED ||eventCode == MEDIA_EVT_3_SLC_CONNECTED || eventCode == MEDIA_EVT_4_SLC_CONNECTED)
			{
                  MMI_DRV_SetOTAVoicepromptsExist(FALSE);
			}


			if(	(	eventCode == MEDIA_EVT_SLC_CONNECTED ||  eventCode == MEDIA_EVT_2_SLC_CONNECTED ||
					eventCode == MEDIA_EVT_PAIRING_FAIL || eventCode == MEDIA_EVT_PAIRING_SUCCESSFUL ||
					eventCode == MEDIA_EVT_LINK_LOSS || eventCode == MEDIA_EVT_SLC_DISCONNECTED)
				&&((MMI_NO_CONNECTED_MEDIA_EVT_IN_FM_FEAT && MMI_CheckInFMState())  || (MMI_NO_CONNECTED_MEDIA_EVT_IN_LINE_IN_FEAT && MMI_CheckInLineInState())))
			{
				continue;
			}

			// Media handler
			for(i = 0; i < EVENT_TABLE_SIZE && gMMI_nvram.evtParaCtl[i].EvtOpcode != NO_MEDIA; i++)
			{
				if (gMMI_nvram.evtParaCtl[i].EvtOpcode == eventCode)
				{
					// LED
					MMI_LED_SendFGLedCmd(eventCode, i);

					#ifdef SUPPORT_VOICE_COMMAND
					if (MAX_MULTI_POINT_NO != A2DP_SearchFollowerLink() && eventCode == MEDIA_EVT_KEY_VOICE_COMMAND_ENABLE)
						break;
					#endif

					//VoicePrompt  && Ringtone
					MMI_RingtoneVoicePromptScheduler(	eventCode,
														(gMMI_nvram.evtParaCtl[i].BeepPatternIndex),
														#ifdef SUPPORT_VOICE_PROMPT
														MMI_VoicePromptIndexMapping(eventCode, i),
														#else
														0x7F,
														#endif
														gMMI_nvram.evtParaCtl[i].Timeout & VOICE_PROMPT_BEFORE_RINGTONE_BIT);
					#ifdef SUPPORT_VOICE_COMMAND
					MMI_DRV_SendVoiceCommandCmd(eventCode);
					#endif

					MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_MEDIA_EVENT_GENERATED, (U8 XDATA_PTR)i, 0);
					break;
				}
			}

			// LED Filter
			for(i = 0; i< MAX_LED_FILTER_NO; i++)
			{
				if (gMMI_nvram.ledFilterMap[i].evtOpcode == eventCode)
				{
					MMI_LED_SendLedFilterCmd(i);
				}
			}
		}
		MMI_DRV_MediaEventSendFakeCmd(eventCode);
	}

	//To prevent power saving interfere ringtone and LED, we lift the priority of DRIVER TASK
	//to ensure ringtone and LED would be executed immediately.
	if (OSMQ_Entries(OSMQ_DRIVER_Command_ptr))
	{
		SYS_InsertOSTask(DRIVER_TASK);
	}
}

#ifdef REMOTE_EEPROM_UPDATE
PRIVATE void MMI_EEPROMUpdateHandler(void)
{
	U8 XDATA_PTR ptrEEPROMUpdate;

	if ((ptrEEPROMUpdate = (U8 XDATA_PTR)OSMQ_Get (OSMQ_Remote_EEPROM_Update_Event_ptr)) != (U8 XDATA_PTR)NULL)
	{
		switch(ptrEEPROMUpdate[0])
		{
			case EEPROM_REM_UPDATE_OPCODE_START_UPDATING:
				MMI_DRV_SendVoicepromptStopCmd();
				gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled |=  IS_VP_DISABLED;
				gMMI_DRV_ctl.isMiscEnabled |= IS_VP_DISABLED;
				MMI_AddTopState(MMI_EOF, MMI_UPDATING);
				break;
			case EEPROM_REM_UPDATE_OPCODE_TERMINATE_OK:
				MMI_AddTopState(MMI_EOF, MMI_UPDATED_OK);
				break;
			case EEPROM_REM_UPDATE_OPCODE_TERMINATE_NG:
				MMI_AddTopState(MMI_EOF, MMI_UPDATED_FAIL);
				break;
			case EEPROM_REM_UPDATE_OPCODE_RESET:
				MMI_Reset();
				break;
		}
		OSMEM_Put (ptrEEPROMUpdate);
	}
}
#endif


//==========================================================================
//==============================MMI Function====================================
//==========================================================================
PRIVATE void MMI_PairModeTimeoutHandler(void)
{
	MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
	if(MMI_CheckNestState(MMI_EOF, MMI_CONDISCABLE))
	{
		MMI_SetEnterDiscoverable(FALSE);
		MMI_RmvState(MMI_EOF, MMI_CONDISCABLE);

		switch(gMMI_nvram.actionAfterPairingModeTimeout)
		{
			case POWER_OFF:
				CHECK_POWER_OFF_IN_PAIR_MODE_TO:
				if (gSYS_update_by_uart_flash_command)
				{
					return;
				}
				if(MMI_CONNECTABLE == MMI_GetTopStateByLink(gMMI_ctl.serviceInd))
				{
					MMI_PowerOffHandler(POWEROFF_REASON_PAIRING_MODE_TO);
				}
				break;
			case RETURN_TO_CONNECTABLE:
				CHECK_CONNECTABLE_IN_PAIR_MODE_TO:
				MMI_TIMER_ResetAutoSwitchOffTimer();
				MMI_ConnectabilityHandle(CMD_SET_CONNECTABLE);
				break;
			case POWER_OFF_IF_PDL_IS_EMPTY:
				if( MMI_LinkKey_GetPDLNumber())
				{
					goto CHECK_CONNECTABLE_IN_PAIR_MODE_TO;
				}
				else
				{
					goto CHECK_POWER_OFF_IN_PAIR_MODE_TO;
				}
				break;
		}
	}
}

PRIVATE void MMI_CheckTimerByLink(void)
{
	U8 linkIndex;

	//check sniff timer to decide shall we go into sniff
	for (linkIndex = 0;linkIndex < MAX_MULTI_POINT_NO;linkIndex++)
	{
		#ifdef HFP_Profile
		MMI_HFP_CheckTimer(linkIndex);
		#endif

		#ifdef AIR_MODULE
		MMI_AIR_CheckTimer();
		#endif

		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].connectionPowerSavingTimer))
		{
			MMI_PowerSaving(TRUE, MMI_PWRSAVING_PROFILE_CONNECTION, linkIndex);
		}

		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delaySDPTimer))
		{
			#if 1
			MMI_SDAP_StartServiceSearchProc(linkIndex);
			#else //for L2CAP_PTS_TC_CLS_CLR_BV_01_C, don't start SDP search instead of sending a connection-less pattern
			extern void L2CAP_SendCLSData_PTS(U8 linkIndex);
			L2CAP_SendCLSData_PTS(linkIndex);
			#endif
		}

		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].checkRemoteAGAcceptEnterSniffTimer))
		{
			gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffTryCnt = 0;
			#if 1
			MMI_HCI_SendSniffSubrating(linkIndex);
			#endif
		}

		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].refreshEncKeyTimer))
		{
			if (!SCO_EXISTS(linkIndex))
			{
				MMI_HCI_SendRefreshEncryptionKey(linkIndex);
			}
			//SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].refreshEncKeyTimer,
			//			(U32)(gMMI_nvram.timerCtl.RefreshEncKeyTimer) * ONE_MIN);

			MMI_PushMediaEvent(MEDIA_EVT_REFRESH_ENCRYPTION);
		}

		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].sniffTimerPtr))
		{
			if(MMI_IsActiveLink(linkIndex))
			{
				if(gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_ACTIVE)
				{
					MMI_HCI_SendSniffMode(linkIndex);
				}
			}
		}

		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].waitDetachTimer))
		{
			if (MMI_IsActiveLink(linkIndex))
			{
				MMI_HCI_SendRelAcl(linkIndex);
				if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
					MMI_AddTopState(MMI_EOF, MMI_DETACHING_LINK);
			}
		}

		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delayRemoveConnectedTimer))
		{
			if(MMI_IsActiveLink(linkIndex) && MMI_IsAllProfileDisconnected(linkIndex))
			{
				if(!MMI_GetConnectedProfileCount(linkIndex, TRUE))
				{
					MMI_RmvState(linkIndex, MMI_CONNECTED);
					MMI_PowerSaving(FALSE, MMI_PWRSAVING_DISC, linkIndex);
				}
			}
		}

		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].nfcDetachIfNoProfileTimer))
		{
			/*empty block, to prevent coverity warning*/
		}

		//Mantis 9926 change role switch timing into profile active
		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delayRoleSwitchTimer))
		{
			if(MMI_ALWAYS_ROLE_SWITCH_TO_MASTER_FEAT && MMI_MasterRoleAvailable(linkIndex) && MMI_IsRoleOfSlave(linkIndex))
			{
				MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
				SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer, 3*ONE_SEC);
			}
		}
	}
}

PRIVATE void MMI_CheckLowBatteryTimers(void)
{
	if (gMMI_ctl.mmiBatInfo.batStatus ==MMI_BAT_LOW)
	{
		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryLEDtimer))
		{
			if(gMMI_nvram.batteryPara.lowBatteryLEDTimer == gMMI_nvram.batteryPara.lowBatteryRingTimer)
			{
				MMI_PushMediaEvent(MEDIA_EVT_BAT_LOW_RING);
			}
			MMI_PushMediaEvent(MEDIA_EVT_BAT_LOW_LED);
			SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryLEDtimer, (U32)gMMI_nvram.batteryPara.lowBatteryLEDTimer * ONE_SEC);
		}
		if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryRingtimer))
		{
			if(gMMI_nvram.batteryPara.lowBatteryLEDTimer != gMMI_nvram.batteryPara.lowBatteryRingTimer)
			{
				MMI_PushMediaEvent(MEDIA_EVT_BAT_LOW_RING);
			}
			SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryRingtimer, (U32)gMMI_nvram.batteryPara.lowBatteryRingTimer * ONE_SEC);
		}

	}
	else
	{
		SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryRingtimer);
		SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryLEDtimer);
	}
}

PRIVATE void _SendReadRSSI(void)//Mantis 8599 - avoid Warning C183 : Dead Assignment Eliminated
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_IsActiveLink(linkIndex))
		{
			MMI_HCI_SendReadRSSI(linkIndex);
		}
	}
}

#ifdef SUPPORT_VOICE_COMMAND
PRIVATE void MMI_CheckVoiceCommandStopTimer(void)
{
	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.voiceCommandTriggerTimer))
	{
		MMI_DRV_SendStopVoiceCommandCmd();
	}
}
#endif

PRIVATE void MMI_CheckTimer(void)
{
	//check sniff timer to decide shall we go into sniff
	MMI_CheckTimerByLink();

	#ifdef SUPPORT_VOICE_COMMAND
	MMI_CheckVoiceCommandStopTimer();
	#endif
	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffTimer))
	{
		if (gSYS_update_by_uart_flash_command)
		{
			return;
		}

		if (!MMI_CheckNestState(MMI_EOF, MMI_OFF) &&
			!MMI_CheckNestState(MMI_EOF, MMI_TEST_MODE) &&
			!MMI_CheckNestState(MMI_EOF, MMI_FAKEOFF)  &&
			!MMI_CheckInLineInState() &&
			!MMI_CheckInFMState())
		{
			//Power Off Headset immediately.
			MMI_PowerOffHandler(POWEROFF_REASON_AUTO_SWITCH_OFF_TIMER);
		}
	}

	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.pairModeTimer))
	{
		MMI_PairModeTimeoutHandler();
	}

	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.disPowerOffAfterPowerOnTime))
	{

	}

	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.checkChargingTimer))
	{
		MMI_PushMediaEvent(MEDIA_EVT_BAT_CHGTO);

		//turn off power-Let Hw gets into abnormal state
		//MCU will be shut down.
		if (MMI_POWER_OFF_WHEN_CHARGING_TO_FEAT)
		{
			OS_ENTER_CRITICAL ();
			LC_SET_PMU_POWER_OFF();
			_nop_ ();
			_nop_ ();
			_nop_ ();
			_nop_ ();
			_nop_ ();
			_nop_ ();
			OS_EXIT_CRITICAL ();
		}
	}

	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer))
	{
		MMI_PowerOffHandler(POWEROFF_REASON_AUTO_SWITCH_OFF_TIMER);
	}

	MMI_CheckLowBatteryTimers();

	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer))
	{
		if (CURRENT_ACTIVE_LINK_CNT > 0)
		{
			MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE);
			MMI_PushMediaEvent(MEDIA_EVT_CONNECTABLE_TIMEOUT);
		}
	}

	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.rssiReadIntervalTimer))
	{
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.rssiReadIntervalTimer, (U32)(gMMI_nvram.timerCtl.RSSIReadIntervalPeriod) * ONE_SEC);
		if(MMI_DETECT_PHONE_IS_TOO_FAR_FEAT)
		{
			_SendReadRSSI();
		}
	}

	#ifdef MMI_BATCHG_ENABLE
	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.batteryMonitoringTimer))
	{
		MMI_CheckCurrentBatLevel();
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.batteryMonitoringTimer, (U32)(gMMI_nvram.timerCtl.BatteryMonitoringPeriod) * ONE_SEC);
	}

	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.delayChgMediaEvtTimer))
	{
		switch(gMMI_ctl.mmiBatInfo.batStatus)
		{
			case MMI_BAT_LOW:
				MMI_PushMediaEvent(MEDIA_EVT_BAT_LOW);
				break;

			case MMI_BAT_WELL:
				MMI_PushMediaEvent(MEDIA_EVT_BAT_CHGROUT);
				break;

			case MMI_BAT_CHARGING_FULL:
				MMI_PushMediaEvent(MEDIA_EVT_BAT_CHGCPL);
				break;

			case MMI_BAT_CHARGER_RECHARGE:
				MMI_PushMediaEvent(MEDIA_EVT_BAT_RECHG);
				break;

			case MMI_BAT_CHARGER_IN:
				MMI_PushMediaEvent(MEDIA_EVT_BAT_CHGRIN);
				break;
		}
	}

	if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.delayDisableMMI1Timer))
	{
		OS_MMI1Disable();
	}
	#endif

	if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.btnSequenceTimer))
	{
		OSMEM_memset_xdata ((U8 XDATA_PTR)&gMMI_ctl.driverCtl.btnSeqRecord[0], 0, MAX_BTN_SEQ_NO);
	}

	#ifdef SUPPORT_VOICE_PROMPT
	if(SYS_IsTimerExpired(&gMMI_ctl.vpRuntime.vpSelectTimerPtr))
	{
		gMMI_ctl.vpRuntime.vpSelectIndex++;
		if (gMMI_ctl.vpRuntime.vpSelectIndex >= gMMI_nvram.vpConfig.LangCount)
		{
			gMMI_ctl.vpRuntime.vpSelectIndex = 0;
			gMMI_ctl.vpRuntime.vpSelectRound++;
		}

		if (gMMI_ctl.vpRuntime.vpSelectRound < gMMI_nvram.vpConfig.LangSelectRound)
		{
			MMI_DRV_ChangeVoicePromptLangCandidate();
		}
		else
		{
			MMI_RmvState(MMI_EOF, MMI_VOICE_PROMPT_LANG_SELECT);
		}
	}
	#endif

	if(SYS_IsTimerExpired(&gMMI_ctl.audioDevCtl.resumeTimer))
	{
		switch(gMMI_ctl.audioDevCtl.currentActiveDev)
		{
			case AUDIO_DEVICE_FM:
				if(!MMI_CheckInFMState())
					MMI_AddTopState(MMI_EOF, MMI_FM);
				break;
			case AUDIO_DEVICE_LINE_IN:
				if(!MMI_CheckInLineInState())
				{
					if(MMI_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && MMI_CheckProfileOnAllLinks())
					{
						return;
					}

					MMI_PushMediaEvent(MEDIA_EVT_KEY_ENTER_LINEIN_MODE);
					MMI_AddTopState(MMI_EOF, MMI_LINE_IN);
				}
				break;
			#ifdef DEMOSOUND
			case AUDIO_DEVICE_DEMO_SOUND:
				MMI_A2DP_DemoSoundPlayCmd();
				break;
			#endif
			case AUDIO_DEVICE_MUSIC:
			case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
			case AUDIO_DEVICE_AVRCP_PLAY:
				#ifdef A2DP_Profile
				if(A2DP_IsResume(gMMI_ctl.audioDevCtl.currentActiveLinkIndex))
				{
					MMI_A2DP_SendResume(gMMI_ctl.audioDevCtl.currentActiveLinkIndex);
				}
				if(A2DP_STREAMING == A2DP_GetState(gMMI_ctl.audioDevCtl.currentActiveLinkIndex))
				{
					MMI_A2DP_SendDSPEnableCmd(gMMI_ctl.audioDevCtl.currentActiveLinkIndex);
				}
				else if(!A2DP_IsResume(gMMI_ctl.audioDevCtl.currentActiveLinkIndex))
				{
					MMI_DRV_AudioDeviceDeRegister(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, gMMI_ctl.audioDevCtl.currentActiveDev);
					MMI_DRV_ResumeOtherAudioDevice(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, gMMI_ctl.audioDevCtl.currentActiveDev);
				}
				#endif
				break;
			case AUDIO_DEVICE_SCO:
			case AUDIO_DEVICE_SCO_HSP:
			case AUDIO_DEVICE_SCO_IDLE:
				if(SCO_EXISTS(gMMI_ctl.audioDevCtl.currentActiveLinkIndex))
				{
					MMI_SendDSPCmd(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, DSP_ENABLE_BY_SCO);
				}
				break;
			case AUDIO_DEVICE_CALL_STATE:
				break;
			case AUDIO_DEVICE_MIC_TO_SPK:
				#ifdef LINEIN_ENABLE
				if(gMMI_ctl.isAudioTransparencyEnable == TRUE)
				{
					MMI_DRV_KeyEventHandler(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, KEY_MIC_TO_SPK_ENABLE);
					MMI_DRV_KeyEventHandler(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, KEY_MIC_L_ENABLE);
				}
				#endif
				break;
		}
	}

	if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.watchDogReconnectTimer))
	{
		MMI_Reconnect(RECONNECT_WATCHDOG);
	}

	if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.watchDogRebootNoMediaTimer))
	{
		/*empty block, to prevent coverity warning*/
	}

	if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.userReconnectKeyEventTimer))
	{
		if(MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState()  && !gMMI_ctl.activeLinkRecord)
		{
			MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, KEY_RECONNECT_USER_INIT);
		}
	}

	#ifdef AIRAPP_Profile
	if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.waitForOTAPacketTimer))
	{
		LightDbgPrint("OTA Packet Timer expired");
		MMI_DRV_FlashUpdateFail();
		MMI_HCI_SetFlashUpdateState(FLASH_UPDATE_OFF);
	}
	#endif

	MMI_Reconnect_CheckTimer();
	#ifdef SUPPORT_MIC_IN_A2DP_MODE
	if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.waitATTriggerMediaTimer))
	{
		MMI_DRV_Switch_AudioTransparency((gMMI_ctl.isAudioTransparencyEnable)? FALSE:TRUE);
	}
	#endif
}

#ifdef LIGHTING_ENABLE
PUBLIC void MMI_MusicYouCanSeeHandler(void)
{
	U8 linkIndex, isMusicPlaying;

	MMI_Share_UpdateLedMode();
	isMusicPlaying = 0;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_AVRCP_IsMusicPlaying(linkIndex))
			isMusicPlaying = 1;
	}
	if(MusicYouCanSeeEnable && isMusicPlaying)
	{
		if((Volume_Cnt != (U8)gAlignDSP.LED_vol_idx || Index_Cnt != (U8)gAlignDSP.LED_band_idx))
		{
			Volume_Cnt = (U8)gAlignDSP.LED_vol_idx;
			Index_Cnt = (U8)gAlignDSP.LED_band_idx;
            MMI_LED_Set_MusicYouCanSee(Index_Cnt, Volume_Cnt);
		}
	}
	else if(Volume_Cnt || Index_Cnt)
	{
		Volume_Cnt = 0;
		Index_Cnt = 0;
		MMI_LED_Set_MusicYouCanSee(Index_Cnt, Volume_Cnt);
	}
}
#endif


#ifdef AIRAPP_Profile
extern PUBLIC void AirApp_DumpMicData(U8 linkIndex, U8 XDATA_PTR ptr, U16 length);
#endif

PRIVATE void MMI_ScoMicDumpHandler(void)
{
	U16 XDATA_PTR dataPtr;

	while((dataPtr = OSMQ_MCU_DSP_Get(OSMQ_DSP_AirDataOut_ptr)) != NULL)
	{
		#ifdef AIRAPP_Profile
		U16 length;
		if(gMMI_ctl.audioDevCtl.currentActiveDev == AUDIO_DEVICE_SCO || gMMI_ctl.audioDevCtl.currentActiveDev == AUDIO_DEVICE_SCO_HSP
			#ifdef SUPPORT_VOICE_COMMAND
			|| VoiceCommand_IsEnabled()
			#endif
			)
		{
			U16 i;
			length = dataPtr[1];
			for(i = 0 ; i < length; i++)
			{
				dataPtr[2+i] = ENDIAN_TRANSFORM_U16(dataPtr[2+i]);
			}

			AirApp_DumpMicData(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, (U8 XDATA_PTR)dataPtr, (U16)(dataPtr[1] * 2 + 4));
		}
		#endif
		OSMEM_Put(dataPtr);
	}
}

PUBLIC void MMI (void)
{
	GAP();
	L2CAP();

	//// For system debug to see if the interrupt is working.
	MMI_InterruptDebug();

	GAP_TestCaseGeneration();

	//GAP/HCI Event----------------------------------
	MMI_HCI_Handler();

	// Profile -----------------------------------------------------------
	MMI_PROTOCOL_EventHandler();

	MMI_ProfileHandle();

	#ifdef AVRCP_Profile
	MMI_AVRCP_Handle();
	#endif

	#ifdef LIGHTING_ENABLE
	MMI_MusicYouCanSeeHandler();
	#endif

	MMI_DRV_EventHandler();
	MMI_MediaEventHandler();
	#ifdef REMOTE_EEPROM_UPDATE
	MMI_EEPROMUpdateHandler();
	#endif

	MMI_SECTOR_SaveData(SAVE_DATA_TYPE_RUNTIME);

	MMI_PollingBatStatus();


	MMI_ConnectableToConDiscoveable();

	MMI_CheckTimer();
	#ifdef OLED_ENABLE
	OLED_CheckTimer();
	#endif

	#ifdef LINEIN_ENABLE
	MMI_LineIn_EncodedDataHandler();
	#endif

	MMI_ScoMicDumpHandler();

	GAP_TestCaseGeneration();

	if(sdk_polling_func)
		sdk_polling_func();

	if(sdk_pcb_func)
		sdk_pcb_func();
}
