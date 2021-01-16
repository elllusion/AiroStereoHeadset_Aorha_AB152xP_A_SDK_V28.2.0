//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define DRIVER1520_C
#include "os_flash.h"
#include "bt_config_profile.h"
#include "drv_sector.h"
#include "hal.h"
#include "Driver.h"
#include "Driver_1520.h"
#include "Audio_Mailbox.h"
#include "AudioControl.h"
#include "sector_merge_runtime.h"
#include "sector_config1.h"
#include "..\MMI\MMI.h"
#include "..\MMI\MMI_HCI.h"
#include "..\MMI\MMI_Driver.h"
#include "..\MMI\MMI_SectorManager.h"
#include "..\A2DP\A2DP_Interface.h"
#include "..\MMI\MMI_HFP.h"
#include "..\MMI\MMI_interface.h"
#include "..\SECTOR\sector_mmi_nvram_type.h"
#ifdef RINGTONE_ENABLE
	#include "ringtone.h"
#endif
#ifdef SUPPORT_VOICE_PROMPT
	#include "VoicePrompt.h"
#endif
#ifdef SUPPORT_VOICE_COMMAND
	#include "VoiceCommand.h"
#endif
#ifdef PEQ_ENABLE
	#include "Peq_HPF.h"
#endif
	#include "Audio_Transparency.h"
#include "sector_boundary.h"

#include "..\SECTOR\sector_mmi_nvram_type.h"

#ifdef AIR_MODULE
	#include "..\MMI\MMI_AirModule.h"
#endif

#if 1 // Added to make the customer know the DSP playing status.
#include "..\SDK\include\DSP_Status.h"
#endif

#ifdef OPERATION_IN_BB_ASIC_VERSION
#define SET_REG_DA_ANALOG_GAIN(gain) AUDIO_AudioDAGain(gain)
extern PUBLIC void AUDIO_AudioDAGain(U8 value);
#endif

#ifdef TWS_SETTINGS
PUBLIC void DRIVER_TWSClkSetting(BOOL isRelay, U8 linkIndex);
#endif
PRIVATE void DRIVER_LINEModeSetting(U8 payloadEncode, U16 eSCORelay);
PRIVATE U16 DRIVER_CheckDSPClock(BOOL isOn, U8 component);

SOUND_EFFECT_EXTERN SOUND_EFFECT_TYPE XDATA soundEffect_Ctl[SOUND_EFFECT_TOTAL_NO];

#ifdef AIR_MODULE
PUBLIC BOOL DRIVER_CheckA2DPOffReady(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP))
	{
		#if defined SCO_WOOFER || defined SOUND_BAR
		if(MMI_AIR_IsHoldLineInDisable()) //A2DP_DISABLE_HOLD_FOR_MUTE
			return FALSE;
		#endif
		#ifdef TWS_SETTINGS
		if(MAX_MULTI_POINT_NO != A2DP_SearchFollowerLink() || MAX_MULTI_POINT_NO != A2DP_SearchRelayerLink())
		{
			if((U8)(gAlignDSP.debug_var03 >> 8))
			{
				#ifdef AIR_MODULE
				if(gDriver_ctl.checkDspDataStatusTimer == (OST XDATA_PTR)NULL)
				{
					SYS_SetTimer(&gDriver_ctl.checkDspDataStatusTimer, (U32) (gAlignDSP.debug_var03 >> 8)* 96);
				}
				#endif
				return FALSE;
			}
		}
		#endif
	}
	return TRUE;
}

PUBLIC BOOL DRIVER_CheckAUXOffReady(void)
{
	return (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) && MMI_AIR_IsHoldLineInDisable()) ? FALSE : TRUE;
}
#endif

PUBLIC void DRIVER_SendAppIdleToMailBox(void)
{
	DriverMsg XDATA_PTR cmdPtr;
	if(cmdPtr = OSMEM_Get(OSMEM_ptr1))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)cmdPtr, 0, OSMEM1_BLKSIZE);
		Mailbox_TaskOgfAPP(cmdPtr, OCF_APP_IDLE_MODE);
		DRIVER_SendCmdToMailBox(cmdPtr);
	}
}

PUBLIC void DRIVER_BackUpOgfAppCmd(DriverMsg XDATA_PTR msgPtr)
{
	if(!gDriver_ctl.backUpAudioCmdPtr)
	{
		gDriver_ctl.backUpAudioCmdPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	}
	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)gDriver_ctl.backUpAudioCmdPtr, (U8 XDATA_PTR)msgPtr, OSMEM1_BLKSIZE);
}

PUBLIC void DRIVER_ReleaseBackUpAppCmd(void)
{
	SYS_MemoryRelease(&gDriver_ctl.backUpAudioCmdPtr);
}

U32 DualMicSectorAddr(void)
{
    U8 CODE_PTR ptr1 = GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT);
	U8 CODE_PTR ptr2 = GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT2);

	if(*ptr1 == MMI_SECTOR_DATA_VALID)
	{
		return (U32)DSP_GetCODEAddr((U8 CODE_PTR)GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_DUAL_MIC_DATA));
	}
	else if(*ptr2 == MMI_SECTOR_DATA_VALID)
	{
		return (U32)DSP_GetCODEAddr((U8 CODE_PTR)GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_DUAL_MIC_DATA2));
	}
	else
	{
        return (U32)NULL;
	}
}

PUBLIC U8 DRIVER_SCOCmdHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	U8 audioSwitchResult;

	if(IS_DSP_BUSY)
		return QUE_PUTFRONT;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendRequestCloseFMEvent();
		return QUE_PUTFRONT;
	}

	#ifdef SUPPORT_VOICE_COMMAND
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND) && !(gDriver_ctl.lightMonoModeMask & (0x01 << msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd)))
	{
		DRIVER_BackUpOgfAppCmd(msgPtr);
		return MEMORY_PUT;
	}
	#endif

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
	{
		if(gDriver_ctl.dspLink != msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd)
		{
			DRIVER_SendAppIdleToMailBox();
			return QUE_PUTFRONT;
		}
		else
		{
			DRIVER_SendSCOEnableEvent((gDriver_ctl.lightMonoModeMask & (0x01 << gDriver_ctl.dspLink))?TRUE:FALSE);
			return MEMORY_PUT;
		}
	}

	if(DRIVER_IsDSPOgfAppOn())
	{
		LightDbgPrint("==AppIdleToMailBox2");
		DRIVER_SendAppIdleToMailBox();
		return QUE_PUTFRONT;
	}

	if(!AUDIO_IsAllowSCOEnabled(msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd))
	{
		return QUE_PUTFRONT;
	}

	if(gAlignDSP.IC_BIST_MODE /*gAlignDSP.IC_BIST_MODE != AUDIO_BIST_DISABLE*/)
	{
		goto DRIVER_ENABLE_SCO;
	}

	if(!MMI_IsActiveLink(msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd))
	{
		return MEMORY_PUT;
	}
	
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT))
		return QUE_PUT;

	if(AUDIO_SWITCH_RESULT_OK != (audioSwitchResult = MMI_DRV_SwitchScoToLink(msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd)))
	{
		//Soming wrong will cause pending forever
		if(audioSwitchResult == AUDIO_SWITCH_RESULT_PENDING)
		{
			return QUE_PUTFRONT;
		}
		else
		{
			return MEMORY_PUT;
		}
	}

	DRIVER_ENABLE_SCO:
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
	{
		DRIVER_ATSwitchVPRTSCO();
		return QUE_PUTFRONT;
	}
	gDriver_ctl.dspLink = msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd;
	if(gDriver_ctl.dspClock != DRIVER_CheckDSPClock(TRUE, AUDIO_SCO))
	{
		if(!AUDIO_SetDSPOff(AUDIO_SCO))
		{
			return QUE_PUTFRONT;
		}
	}
	DRIVER_BackUpOgfAppCmd(msgPtr);
	AUDIO_SetAudioOn(AUDIO_SCO);

	ASSERT(gDriver_ctl.dspLink < 4);

	#ifdef ECNR_OFF_IN_NORMAL_SCO
	if(gDriver_ctl.ecnrMask & (0x01 << gDriver_ctl.dspLink))
	{
		gAlignDSP.DSP_nvram.DSP_Reserved_PARAM.Feature_Sel &= ~ECNR_DISABLE;
	}
	else
	{
		gAlignDSP.DSP_nvram.DSP_Reserved_PARAM.Feature_Sel |= ECNR_DISABLE;
	}
	#endif
	
	#ifdef TWS_SETTINGS
	if(msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.feature_sel.PAYLOAD_ENCODE)
	{
		DRIVER_TWSClkSetting(TRUE, gDriver_ctl.dspLink);
	}
	#endif

	#ifdef SUPPORT_VOICE_COMMAND
		Mailbox_TaskOgfAPP(msgPtr, (gDriver_ctl.lightMonoModeMask & (0x01 << gDriver_ctl.dspLink))?OCF_APP_LIGHT_MONO_MODE:OCF_APP_MONO_MODE);
	#else
		Mailbox_TaskOgfAPP(msgPtr, OCF_APP_MONO_MODE);
	#endif
	msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.DualMic_ptr = DualMicSectorAddr();
	AUDIO_SetAudioInOutToMailBoxCmd(AUDIO_SCO, &msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.audio_scenario);
	DRIVER_SendCmdToMailBox(msgPtr);
	gOS_PowerSavingMCUPauseRequest = TRUE;
	#ifdef PEQ_ENABLE
	DRIVER_PEQReload();
	#endif

	if(gDriver_ctl.redialMuteTimerPtr)
		SYS_SetTimer(&gDriver_ctl.redialMuteTimerPtr,0);

	SYS_ReleaseTimer(&gDriver_ctl.waitQosTimerPtr);
	return RETURN_FUNCTION;
}

PUBLIC U8 DRIVER_A2DPCmdHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	if(IS_DSP_BUSY)
		return QUE_PUTFRONT;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendRequestCloseFMEvent();
		return QUE_PUTFRONT;
	}

	if(DRIVER_IsRingToneOrVPOrATPlaying() && !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
		return QUE_PUTFRONT;

	#ifdef SUPPORT_VOICE_COMMAND
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
	{
		DRIVER_BackUpOgfAppCmd(msgPtr);
		return MEMORY_PUT;
	}
	#endif

	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP))
	{
		if(gDriver_ctl.dspLink == msgPtr->msgBodyPtr.driverCmd.a2dpCmd.linkInd && gDriver_ctl.musicCodecType == msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.codec)
		{
			DRIVER_SendA2DPEnableEvent();
			return MEMORY_PUT;
		}
		else
		{
			#ifdef AIR_MODULE
			if(!DRIVER_CheckA2DPOffReady())
				return QUE_PUTFRONT;
			#endif

			DRIVER_SendAppIdleToMailBox();
			return QUE_PUTFRONT;
		}
	}
	
	if(DRIVER_IsDSPOgfAppOn())
	{
		#ifdef AIR_MODULE
		if(!DRIVER_CheckAUXOffReady())
			return QUE_PUTFRONT;
		#endif

		DRIVER_SendAppIdleToMailBox();
		return QUE_PUTFRONT;
	}

	if(!MMI_IsActiveLink(msgPtr->msgBodyPtr.driverCmd.a2dpCmd.linkInd))
	{
		SYS_ReleaseTimer(&gDriver_ctl.waitQosTimerPtr);
		return MEMORY_PUT;
	}

	if(!MMI_HCI_IsQosReady(msgPtr->msgBodyPtr.driverCmd.a2dpCmd.linkInd) && !SYS_IsTimerExpired(&gDriver_ctl.waitQosTimerPtr))
	{
		if(!gDriver_ctl.waitQosTimerPtr)
		{
			SYS_SetTimer(&gDriver_ctl.waitQosTimerPtr, HALF_SEC);
		}
		return QUE_PUTFRONT;
	}

	gDriver_ctl.dspLink = msgPtr->msgBodyPtr.driverCmd.a2dpCmd.linkInd;
	gDriver_ctl.musicCodecType = msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.codec;
	msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.codec_type = msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.codec;
	msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.tws_mode = msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.twsMode;

	#ifdef TWS_SETTINGS
	gAlignDSP.dsp_param_update_ctl = AIRO_SYNC_MAGIC_TIME;

	msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.start_sync = 1;

	if(msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.twsMode == A2DP_TWS_MODE_RELAYER)
		DRIVER_TWSClkSetting(TRUE, gDriver_ctl.dspLink);
	else if(msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.twsMode == A2DP_TWS_MODE_FOLLOWER)
		DRIVER_TWSClkSetting(FALSE, gDriver_ctl.dspLink);
	else
	    msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.start_sync = 0;
	#endif

	if (gDriver_ctl.isSideToneEnabled)
	{
		LightDbgPrint("open mic2");
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.sidetone_en = 1;
	}
	else
	{
		LightDbgPrint("close mic2");
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.sidetone_en = 0;
	}
	
	DRIVER_BackUpOgfAppCmd(msgPtr);
	AUDIO_SetAudioOn(AUDIO_A2DP);
	Mailbox_TaskOgfAPP(msgPtr, OCF_APP_STEREO_MODE);
	AUDIO_SetAudioInOutToMailBoxCmd(AUDIO_A2DP, &msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.audio_scenario);
	DRIVER_SendCmdToMailBox(msgPtr);
	gOS_PowerSavingMCUPauseRequest = TRUE;
	DRIVER_SetPowerSaving(FALSE, DRIVER_POWER_SAVING_DSP_A2DP);
	#ifdef PEQ_ENABLE
	DRIVER_PEQReload();
	#endif

	SYS_ReleaseTimer(&gDriver_ctl.waitQosTimerPtr);
	return RETURN_FUNCTION;
}

extern U8 gAudio_MappingTable[AUDIO_COMPONENT_NO];
PUBLIC U8 DRIVER_LineInCmdHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	if(IS_DSP_BUSY)
		return QUE_PUTFRONT;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendRequestCloseFMEvent();
		return QUE_PUTFRONT;
	}

	#ifdef SUPPORT_VOICE_COMMAND
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
	{
		DRIVER_BackUpOgfAppCmd(msgPtr);
		return MEMORY_PUT;
	}
	#endif


	if(DRIVER_IsRingToneOrVPOrATPlaying() && !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
		return QUE_PUTFRONT;

	if(!gAUDIO_CtrlInfo.isLineInEnabled && !gAlignDSP.IC_BIST_MODE &&
		(msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.input_fs != 0x10))
	{
		return MEMORY_PUT;
	}

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
#ifndef BLUETOOTH_SPEAKER		
		if(msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.input_fs == 0x10)
			AUDIO_PresetLineInMode(LINE_IN_MIC_ENABLE_ALL);
#endif		
		DRIVER_SendLineInEnableEvent();
		return MEMORY_PUT;
	}

	if(DRIVER_IsDSPOgfAppOn())
	{
		#ifdef AIR_MODULE
		if(!DRIVER_CheckA2DPOffReady())
			return QUE_PUTFRONT;
		#endif

		DRIVER_SendAppIdleToMailBox();
		return QUE_PUTFRONT;
	}


	#ifdef DBG_UART_ENABLE_DRV
	LightDbgPrint("DSP - Start DSP Linein:%d", (U8)msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_RELAY);
	#endif

	DRIVER_LINEModeSetting((U8)msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.PAYLOAD_ENCODE, msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_RELAY);
	DRIVER_BackUpOgfAppCmd(msgPtr);
#ifndef BLUETOOTH_SPEAKER
	if(msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.input_fs == 0x10)
		AUDIO_PresetLineInMode(LINE_IN_MIC_ENABLE_ALL);
#endif	
	AUDIO_SetAudioOn (AUDIO_LINE_IN);
	gDriver_ctl.dspLink = 0xFF;

	#ifdef TWS_SETTINGS
	if(msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.PAYLOAD_ENCODE)
		DRIVER_TWSClkSetting(TRUE, 0xFF);
	#endif

	gAudio_MappingTable[AUDIO_LINE_IN]=gAudio_MappingTable[AUDIO_LINE_IN] |AUDIO_ENABLE_MIC;

	Mailbox_TaskOgfAPP(msgPtr, OCF_APP_LINEIN_MODE);
	AUDIO_SetAudioInOutToMailBoxCmd(AUDIO_LINE_IN, &msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.audio_scenario);
	DRIVER_SendCmdToMailBox(msgPtr);

	#ifdef PEQ_ENABLE
	DRIVER_PEQReload();
	#endif

	return RETURN_FUNCTION;
}

PUBLIC U8 DRIVER_StopDSPCmdHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	LightDbgPrint("==DRIVER_StopDSP:%d,%d",(U8)gDriver_ctl.dspLink, (U8)msgPtr->msgBodyPtr.driverCmd.stopDSPCmd.linkInd);
	if((AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) || AUDIO_COMPONENT_IS_ACTIVE (AUDIO_SCO) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
	 && gDriver_ctl.dspLink == msgPtr->msgBodyPtr.driverCmd.stopDSPCmd.linkInd)
	{
		if(IS_DSP_BUSY)
			return QUE_PUTFRONT;
		#ifdef AIR_MODULE
		if(!DRIVER_CheckA2DPOffReady())
		{
			if(!SYS_IsTimerExpired(&gDriver_ctl.checkDspDataStatusTimer))
			{
				return QUE_PUTFRONT;
			}
		}
		SYS_ReleaseTimer(&gDriver_ctl.checkDspDataStatusTimer);
		#endif

		DRIVER_ReleaseBackUpAppCmd();
		DRIVER_SendAppIdleToMailBox();
	}
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_StopLineInCmdHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		if(IS_DSP_BUSY)
			return QUE_PUTFRONT;

		#ifdef AIR_MODULE
		if(!DRIVER_CheckAUXOffReady())
			return QUE_PUTFRONT;
		#endif

		gAudio_MappingTable[AUDIO_LINE_IN]=AUDIO_ENABLE_DA|AUDIO_ENABLE_AD;			
		
		DRIVER_ReleaseBackUpAppCmd();
		DRIVER_SendAppIdleToMailBox();
	}
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_StopDSPPowerOffCmdHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendRequestCloseFMEvent();
		return QUE_PUTFRONT;
	}
	else if(DRIVER_IsDSPOgfAppOn())
	{
		if(IS_DSP_BUSY)
			return QUE_PUTFRONT;

		#ifdef AIR_MODULE
		if(!DRIVER_CheckAUXOffReady())
			return QUE_PUTFRONT;

		if(!DRIVER_CheckA2DPOffReady())
			return QUE_PUTFRONT;
		#endif

		DRIVER_ReleaseBackUpAppCmd();
		DRIVER_SendAppIdleToMailBox();
		return QUE_PUTFRONT;
	}
	//DRIVER_TurnOffPureOn(); //mantis 8788
	gDriver_ctl.dspLink = 0xFF;
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_CloseAllAudioHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	DRIVER_ClearAudioCmd(DRIVER_INTERNAL_CLEAN_ALL_AUDIO_CMD, NO_MEDIA);

	if(IS_DSP_BUSY)
		return QUE_PUTFRONT;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendRequestCloseFMEvent();
		return QUE_PUTFRONT;
	}
	#ifdef RINGTONE_ENABLE
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) && !Ring_Pause())
	{
		return QUE_PUTFRONT;
	}
	#endif
	#ifdef SUPPORT_VOICE_PROMPT
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT) && !VoicePrompt_Pause())
	{
		return QUE_PUTFRONT;
	}
	#endif
	#ifdef SUPPORT_VOICE_COMMAND
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND) && !VoiceCommand_Pause())
	{
		return QUE_PUTFRONT;
	}
	#endif
	
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY) && !AudioTrspr_Stop())
	{
		return QUE_PUTFRONT;
	}
	
	if(DRIVER_IsDSPOgfAppOn())
	{
		#ifdef AIR_MODULE
		if(!DRIVER_CheckAUXOffReady())
			return QUE_PUTFRONT;

		if(!DRIVER_CheckA2DPOffReady())
			return QUE_PUTFRONT;
		#endif

		DRIVER_ReleaseBackUpAppCmd();
		DRIVER_SendAppIdleToMailBox();
		return QUE_PUTFRONT;
	}
	DRIVER_TurnOffPureOn();
	gDriver_ctl.dspLink = 0xFF;
	return MEMORY_PUT;
}


PUBLIC U8 DRIVER_SuspendDSPCmdHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	if(MEMORY_PUT == DRIVER_CloseAllAudioHandler1520(msgPtr))
	{
		gDriver_ctl.suspendDSPReason |= msgPtr->msgBodyPtr.driverCmd.suspendDspCmd.reason;
		if(msgPtr->msgBodyPtr.driverCmd.suspendDspCmd.callback)
			msgPtr->msgBodyPtr.driverCmd.suspendDspCmd.callback();

		return MEMORY_PUT;
	}
	return QUE_PUTFRONT;
}



PRIVATE void Driver_CheckWriteFlash(void)
{
	if(!MMI_POWER_DROP_SAVE_LINK_HISTORY_FEAT)
		return;


	if(DRIVER_IsDSPOgfAppOn())
	{
		switch(MMI_DRV_WriteFlashAfterRingtoneVPStopped())
		{
			case NO_FLASH_WRITTEN_NEEDED:

				break;
			case PENDING_TO_WRITE_FLASH:
				gDriver_ctl.isAudioHandlingPending = TRUE;
				gDriver_ctl.sendDSPAppIdle = FALSE;
				break;
			case DIRECTLY_WRITE_FLASH:
				gDriver_ctl.isAudioHandlingPending = TRUE;
				gDriver_ctl.sendDSPAppIdle = TRUE;
				break;
			default:
				ASSERT(FALSE);
				break;
		}
	}
}

PRIVATE void Driver_MailBoxOgfVPEvtHandle(U8 ocf, U16 cmdType)
{
	UNUSED(cmdType);
	Mailbox_ReleaseTASKVPRTATBusyFlag();
	switch(ocf)
	{
		case OCF_VP_START:
			#ifdef SUPPORT_VOICE_PROMPT
			DRIVER_SetGainAndMute(TARGET_VPRTAT);
			#endif
			break;
		case OCF_VP_STOP:
			#ifdef SUPPORT_VOICE_PROMPT
			VoicePrompt_Stopped();
			#endif
			Driver_CheckWriteFlash();
			break;
	}
}

PRIVATE void Driver_MailBoxOgfRTEvtHandle(U8 ocf, U16 cmdType)
{
	UNUSED(cmdType);
	Mailbox_ReleaseTASKVPRTATBusyFlag();
	switch(ocf)
	{
		case OCF_RT_START:
			#ifdef RINGTONE_ENABLE
			DRIVER_SetGainAndMute(TARGET_VPRTAT);
			#endif
			break;
		case OCF_RT_STOP:
			#ifdef RINGTONE_ENABLE
			Ring_Stopped();
			#endif
			Driver_CheckWriteFlash();
			break;
	}
}

PRIVATE void Driver_MailBoxOgfVCEvtHandle(U8 ocf, U16 cmdType)
{
	UNUSED(cmdType);
	Mailbox_ReleaseTASKVCBusyFlag();
	switch(ocf)
	{
		case OCF_VC_START:
			#ifdef SUPPORT_VOICE_COMMAND
			DRIVER_SetGainAndMute(TARGET_NONE);
			#endif
			break;
		case OCF_VC_STOP:
			#ifdef SUPPORT_VOICE_COMMAND
			VoiceCommand_Stopped();
			#endif
			break;
	}
}

PRIVATE void Driver_MailBoxOgfAPPEvtHandle(U8 ocf, U16 cmdType)
{
	UNUSED(cmdType);
	Mailbox_ReleaseTASKAppBusyFlag();
	switch(ocf)
	{
		case OCF_APP_LIGHT_MONO_MODE:
			DRIVER_SendSCOEnableEvent(TRUE);
			break;
		case OCF_APP_MONO_MODE:
			DRIVER_SendSCOEnableEvent(FALSE);
			break;
		case OCF_APP_STEREO_MODE:
			DRIVER_SendA2DPEnableEvent();
			break;
		case OCF_APP_LINEIN_MODE:
			DRIVER_SendLineInEnableEvent();
			break;
		case OCF_APP_IDLE_MODE:
			if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
			{
				DRIVER_TurnOffSCO();
			}
			else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
			{
				DRIVER_TurnOffA2DP();
			}
			else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
			{
				DRIVER_TurnOffLineIn();
			}
			else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
			{
				DRIVER_TurnOffAsyncSCO();
			}
			break;
		case OCF_APP_ASYNC_MONO_MODE:
			DRIVER_SendAsyncSCOEnabledEvent();
			break;
	}
}

PRIVATE void Driver_MailBoxOgfDSPEvtHandle(U8 ocf, U16 cmdType)
{
	Mailbox_ReleaseTASKDSPBusyFlag();
	switch(ocf)
	{

		case OCF_DSP_AUDIO_ON:
			#ifdef OPERATION_IN_BB_ASIC_VERSION
			AUDIO_SetGainCtlOwner(GAIN_OWNER_DSP);
			#endif
			break;
		case OCF_DSP_AUDIO_OFF:
			#ifdef OPERATION_IN_BB_ASIC_VERSION
			AUDIO_SetGainCtlOwner(GAIN_OWNER_MCU);
			#endif
			AUDIO_CloseAudios(AUDIO_DSP_RELATED_ENUM);
			DRIVER_CheckATEnablePlay();
			gMailBox_ctl.setVolumeAgain = FALSE;

			if(gDriver_ctl.isAudioHandlingPending)
			{
				MMI_SECTOR_SaveData(SAVE_DATA_TYPE_DSP_OFF);
				gDriver_ctl.isAudioHandlingPending = FALSE;
				if(gDriver_ctl.backUpAudioCmdPtr)
				{
					OSMQ_PutFront(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)gDriver_ctl.backUpAudioCmdPtr);
					gDriver_ctl.backUpAudioCmdPtr = NULL;
					DRIVER_ProcMmiCmd();
				}
			}
			break;
		case OCF_DSP_SET_VOL:
			#ifdef AUDIO_CTL_VERIFY
			{
				U16 HiLo;

				SYS_3WireRead(0xA0, HiLo);
				LightDbgPrint("DRV - 3wire 0xA0, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA1, HiLo);
				LightDbgPrint("DRV - 3wire 0xA1, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA2, HiLo);
				LightDbgPrint("DRV - 3wire 0xA2, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA3, HiLo);
				LightDbgPrint("DRV - 3wire 0xA3, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA4, HiLo);
				LightDbgPrint("DRV - 3wire 0xA4, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA5, HiLo);
				LightDbgPrint("DRV - 3wire 0xA5, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA6, HiLo);
				LightDbgPrint("DRV - 3wire 0xA6, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA7, HiLo);
				LightDbgPrint("DRV - 3wire 0xA7, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA8, HiLo);
				LightDbgPrint("DRV - 3wire 0xA8, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xA9, HiLo);
				LightDbgPrint("DRV - 3wire 0xA9, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
				SYS_3WireRead(0xAA, HiLo);
				LightDbgPrint("DRV - 3wire 0xAA, %x, %x", ((U8)(HiLo>>8)), ((U8)HiLo));
			}
			#endif
			break;
		case OCF_DSP_UPDATE_PARA:

			if (cmdType == DSP_FEEDBACK_REF_GAIN)
			{
				MMI_HCI_ToolDSPRefGainCmdResponse(HCI_VCMD_OCF_TOOL_GET_DSP_REF_GAIN,HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			}
			else if (cmdType == DSP_UPDATE_WID)
			{
				if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
				{
					LightDbgPrint("A2DP mode-Sound Effect %d ", (U8)soundEffect_Ctl[SOUND_EFFECT_A2DP].modeSoundEffect);
				}
				else if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
				{
					LightDbgPrint("Line-In mode-Sound Effect %d ", (U8)soundEffect_Ctl[SOUND_EFFECT_LINE_IN].modeSoundEffect);
				}
				LightDbgPrint("SoundEffect done");
			}
			else if (cmdType == DSP_UPDATE_DBB)
			{
				LightDbgPrint("DBB Switch %d", (U8)dbbOnOff_Ctl);
				LightDbgPrint("DBB OnOff done");
			}
			break;
	}
}

PRIVATE void Driver_MailBoxOgfATEvtHandle(U8 ocf, U16 cmdType)
{
	UNUSED(cmdType);
	Mailbox_ReleaseTASKVPRTATBusyFlag();
	switch(ocf)
	{
		case OCF_AT_START:
			DRIVER_SetGainAndMute(TARGET_VPRTAT);
			gDriver_ctl.isATSwitch = AT_NOT_IN_SWITCH;
			break;
		case OCF_AT_STOP:
			AUDIO_SetAudioOff(AUDIO_TRANSPARENCY);
			break;
	}
}

typedef struct
{
	void (*driverMailBoxEvent)(U8 ocf,  U16 cmdType);
}DriverMailBoxEvtFuncType;

DriverMailBoxEvtFuncType CODE DriverMailBoxEvtHandler[OGF_MAX_NUM] =
{
	Driver_MailBoxOgfVPEvtHandle, //OGF_VP = 0,
	Driver_MailBoxOgfRTEvtHandle, //OGF_RT,
	Driver_MailBoxOgfVCEvtHandle, //OGF_VC,
	Driver_MailBoxOgfAPPEvtHandle, //OGF_APP,
	Driver_MailBoxOgfDSPEvtHandle, //OGF_DSP,
	Driver_MailBoxOgfATEvtHandle, //OGF_AT,
};

OSMQ XDATA Driver_MailBox_Evt;

PUBLIC void DRIVER_ProcMailboxEvent(U8 XDATA_PTR msgPtr)
{
	OSMQ_Put(&Driver_MailBox_Evt, (U8 XDATA_PTR)msgPtr);
}

PUBLIC void DRIVER_MailBoxHandler(void)
{
	DriverMsg XDATA_PTR msgPtr;
	while(OSMQ_Entries(&Driver_MailBox_Evt))
	{
		msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(&Driver_MailBox_Evt);
		DriverMailBoxEvtHandler[msgPtr->msgBodyPtr.driverEvt.mailBoxEvt.ogf].driverMailBoxEvent(msgPtr->msgBodyPtr.driverEvt.mailBoxEvt.ocf, msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_update_para.cmd_type);
		OSMEM_Put((U8 XDATA_PTR)msgPtr);
	}
	if(!IS_DSP_BUSY && gDriver_ctl.isAudioHandlingPending && gDriver_ctl.sendDSPAppIdle)
	{
		DRIVER_SendAppIdleToMailBox();
		gDriver_ctl.sendDSPAppIdle = FALSE;
	}

	if(!IS_DSP_BUSY && gAUDIO_CtrlInfo.isGainOwnByDSP && gMailBox_ctl.setVolumeAgain)
	{
		if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
		{
			DRIVER_SetLineInVolumeToHW();
		}
		else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
		{
			LightDbgPrint("Driver:setVolumeAgainA2DP");
			DRIVER_SetA2DPVolumeToHW();
		}
		else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO)|| AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
		{
			DRIVER_SetNormalVolumeToHW();
		}
		else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
		{
			DRIVER_SetGainAndMute(TARGET_VPRTAT);
		}
		else if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
		{
			DRIVER_SetGainAndMute(TARGET_NONE);
		}
	}
}

PUBLIC void DRIVER_SetGainAndMute(U8 target)
{
	DriverMsg XDATA_PTR msgPtr;
	U8 analogAdGain_R, analogAdGain_L, analogDaGain, digitalAdGain;
	U16 spkVolsApp, spkVolsVPRT;
	U8 vprtSoundLevel;

	gMailBox_ctl.setVolumeAgain = TRUE;

	if(IS_DSP_BUSY)
		return;

	///////// AD Gain ////////////////////////
	analogAdGain_R = VOLUME_DONT_CARE;
	analogAdGain_L = VOLUME_DONT_CARE;
	digitalAdGain = VOLUME_DONT_CARE;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
	{
		analogAdGain_R = gDriver_ctl.voiceCmdMiscCtl.analogAdGain_R;
		analogAdGain_L = gDriver_ctl.voiceCmdMiscCtl.analogAdGain_L;
		digitalAdGain = gDriver_ctl.voiceCmdMiscCtl.micVols;
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
	{
		analogAdGain_R = gDriver_ctl.lineInSpkMicCtl.analogAdGain_R;
		analogAdGain_L = gDriver_ctl.lineInSpkMicCtl.analogAdGain_L;
		digitalAdGain = gDriver_ctl.lineInSpkMicCtl.micVols;
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO)|| AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
	{
		analogAdGain_R = gDriver_ctl.normalSpkMicCtl.analogAdGain_R;
		analogAdGain_L = gDriver_ctl.normalSpkMicCtl.analogAdGain_L;
		digitalAdGain = gDriver_ctl.normalSpkMicCtl.micVols;
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP)) // Soundbar case
	{
		analogAdGain_R = gDriver_ctl.a2dpSpkCtl.analogAdGain_R;
		analogAdGain_L = gDriver_ctl.a2dpSpkCtl.analogAdGain_L;
		digitalAdGain = gDriver_ctl.a2dpSpkCtl.micVols;
	}


	/////////  DA  Gain  ///////////////////////
	analogDaGain = VOLUME_DONT_CARE;
	vprtSoundLevel = VOLUME_DONT_CARE;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
	{
		analogDaGain = gDriver_ctl.a2dpSpkCtl.analogDaGain;
		
		if(vprtSoundLevel = gDriver_ctl.a2dpSpkCtl.soundLevel)
			vprtSoundLevel--;
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
	{
		analogDaGain = gDriver_ctl.lineInSpkMicCtl.analogDaGain;
		if(vprtSoundLevel = gDriver_ctl.lineInSpkMicCtl.soundLevel)
			vprtSoundLevel--;
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO)|| AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
	{
		analogDaGain = gDriver_ctl.normalSpkMicCtl.analogDaGain;
		vprtSoundLevel = gDriver_ctl.normalSpkMicCtl.soundLevel;
	}


	///////////  APP SpkVol  ////////////////////
	spkVolsApp = gDriver_ctl.DSPGainLv;


	//////////    VPRT Vol ///////////////////
	spkVolsVPRT = VOLUME_DONT_CARE;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE))
	{
		DRIVER_CTL_TYPE CODE_PTR vprtGainTablePtr = (DRIVER_CTL_TYPE CODE_PTR)GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT);

		if(vprtSoundLevel == VOLUME_DONT_CARE)
		{
			vprtSoundLevel = gDriver_ctl.vprtSoundLevel;
			
			analogDaGain = (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE)) ?
				vprtGainTablePtr->vprtAnalogGain[vprtSoundLevel].rtAnalogGain :
				vprtGainTablePtr->vprtAnalogGain[vprtSoundLevel].vpAnalogGain;
			
			spkVolsVPRT = (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE)) ?
				vprtGainTablePtr->vprtSpkGainSet[vprtSoundLevel].digitalGainRT :
				vprtGainTablePtr->vprtSpkGainSet[vprtSoundLevel].digitalGainVP;
		}
		else
		{
			if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO)|| AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
			{
				spkVolsVPRT = (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE)) ?
					vprtGainTablePtr->vprtSpkGainSetSco[vprtSoundLevel].digitalGainRT :
					vprtGainTablePtr->vprtSpkGainSetSco[vprtSoundLevel].digitalGainVP;
			}
			else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
			{
				spkVolsVPRT = (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE)) ?
					vprtGainTablePtr->vprtSpkGainSetA2dp[vprtSoundLevel].digitalGainRT :
					vprtGainTablePtr->vprtSpkGainSetA2dp[vprtSoundLevel].digitalGainVP;
			}
			else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
			{
				spkVolsVPRT = (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE)) ?
					vprtGainTablePtr->vprtSpkGainSetLineIn[vprtSoundLevel].digitalGainRT :
					vprtGainTablePtr->vprtSpkGainSetLineIn[vprtSoundLevel].digitalGainVP;
			}
		}
		target |= TARGET_VPRTAT;
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
	{
		spkVolsVPRT = gDriver_ctl.lineInSpkMicCtl.spkVols;
		target |= TARGET_VPRTAT;
	}

	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	if(msgPtr)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		Mailbox_TaskOgfDSP(msgPtr, OCF_DSP_SET_VOL);
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.GainTarget = target;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.APP_GainLv = VOLUME_DONT_CARE;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.VPRT_GainLv = VOLUME_DONT_CARE;
		if(target&TARGET_APP)
		{
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.APP_GainLv = spkVolsApp;
			LightDbgPrint("DRV-Vol[APP]:%d,%d,%d,%d",(U8)spkVolsApp, (U8)analogDaGain, (U8)analogAdGain_R, (U8)analogAdGain_L);
		}
		if(target&TARGET_VPRTAT)
		{
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.VPRT_GainLv = spkVolsVPRT;
			LightDbgPrint("DRV-Vol[VPRT]:%d,%d,%d,%d",(U8)spkVolsVPRT, (U8)analogDaGain, (U8)analogAdGain_R, (U8)analogAdGain_L);
		}
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.au_ad_ana_gain.Field.AuInGainCtrl_L = analogAdGain_L;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.au_ad_ana_gain.Field.AuInGainCtrl_R = analogAdGain_R;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.ear_sdmod_gain.Field.AuDAC_DSMGain = 0x03;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.ear_sdmod_gain.Field.SpkVolCtrl = analogDaGain;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.MicDigitalGain = digitalAdGain;

		#ifdef OPERATION_IN_BB_ASIC_VERSION
		if(analogDaGain == 0)
		{
			analogDaGain = 0x10;
		}
		else
		{
			analogDaGain --;
		}
		SET_REG_DA_ANALOG_GAIN(analogDaGain);
		#endif
		DRIVER_SendCmdToMailBox(msgPtr);
	}
}

extern PUBLIC BOOL MMI_DRV_IsScoInCVSD(U8 linkIndex);
#if 1 // Added to make the customer know the DSP playing status.
static DSP_PLAYING_STATUS g_DSPPlayingStatus=SDK_DSP_PLAYING_STOPPED;
#endif

PRIVATE U16 DRIVER_CheckDSPClock(BOOL isOn, U8 component)
{
	U8 productType;
	U16 dspClock;
	
	productType = GET_PRODUCT_AIR_TYPE(pMMI_nvram->productType.pnpProductVersion);
	
	if(isOn)
	{
		switch(component)
		{
			case AUDIO_SCO:
				#if defined(AB1520SC) || defined(AB1520S) || defined(AB1520U)
				dspClock = DSP_CLOCK_96M;
				UNUSED(productType);
				#else
					#ifdef BLUETOOTH_SPEAKER
					dspClock = DSP_CLOCK_96M;
					UNUSED(productType);
					#else
					if(	!(((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->isDualMic) && gDriver_ctl.dspLink < MAX_MULTI_POINT_NO &&
					MMI_DRV_IsScoInCVSD(gDriver_ctl.dspLink) && (productType !=PRODUCT_AIR_SCO_WOOFER))
					{
						dspClock = DSP_CLOCK_48M;
					}
					else
						dspClock = DSP_CLOCK_96M;
					#endif
				#endif
				break;
			#ifdef A2DP_Profile
			case AUDIO_A2DP:
				#if defined(PEQ_ENABLE) || defined(BLUETOOTH_SPEAKER)
				dspClock = DSP_CLOCK_96M;
				UNUSED(productType);
				#else
				if( (gDriver_ctl.musicCodecType == SBC_SRC_SEID || gDriver_ctl.musicCodecType == SBC_SNK_SEID)&&
				 (soundEffect_Ctl[SOUND_EFFECT_A2DP].modeSoundEffect == DSP_SOUNDEFFECT_NO_UPDATE) && (productType !=PRODUCT_AIR_SOUNDBAR))
				{
					dspClock = DSP_CLOCK_48M;
				}
				else
					dspClock = DSP_CLOCK_96M;
				#endif

				break;
			#endif

			case AUDIO_LINE_IN:
				#ifdef PEQ_ENABLE
				dspClock = DSP_CLOCK_96M;
				UNUSED(productType);
				#else
				if(gAUDIO_CtrlInfo.isNormalLineMode && (soundEffect_Ctl[SOUND_EFFECT_A2DP].modeSoundEffect == DSP_SOUNDEFFECT_NO_UPDATE))
				{
					dspClock = DSP_CLOCK_48M;
				}
				else
					dspClock = DSP_CLOCK_96M;
				break;
				#endif
			default:
				dspClock = DSP_CLOCK_96M;
				UNUSED(productType);
				break;
		}
	}
	else
	{
		dspClock = DSP_CLOCK_96M;
	}
	return dspClock;
}

PUBLIC void DRIVER_AudioOnOffCmdToDSP(BOOL isOn, U8 component)
{
	DriverMsg XDATA_PTR msgPtr;
	MMI_NVRAM_TYPE CODE_PTR pMMI_nvram;

	pMMI_nvram=(MMI_NVRAM_TYPE CODE_PTR) GET_SECTOR_CFG1_ADDR(SECTOR_MMI_NVRAM_INIT);

	if ((DSP_IsDSPEnabled() && isOn) || (!DSP_IsDSPEnabled() && !isOn))
		return;

#if 1 // Added to make the customer know the DSP playing status.
	if(isOn){
		switch(component){
			case AUDIO_RINGTONE:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_RINGTONE;
				break;
			case AUDIO_SCO:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_SCO;
				break;
			case AUDIO_VOICEPROMPT:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_VOICEPROMPT;
				break;
			case AUDIO_A2DP:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_A2DP;
				break;
			case AUDIO_LINE_IN:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_AUDIO_LINE_IN;
				break;
			case AUDIO_VOICECOMMAND:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_VOICECOMMAND;
				break;
			case AUDIO_ASYNC_SCO:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_ASYNC_SCO;
				break;
			case AUDIO_FM:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_AUDIO_FM;
				break;
			default:
				g_DSPPlayingStatus=SDK_DSP_PLAYING_STOPPED;
				break;
			}
	}
	else{
		g_DSPPlayingStatus=SDK_DSP_PLAYING_STOPPED;
	}
#endif
	//Mantis 10350 dsp clock should be check when opening mode.
	gDriver_ctl.dspClock = DRIVER_CheckDSPClock(isOn, component);

	if(msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
	{
		U16 HiLo;
		SYS_3WireRead(0xA0, HiLo);
		HiLo &= 0x001F;  // 5 bits valid

		if(HiLo == 0x10)
		{
			HiLo = 0x00;
		}
		else
		{
			HiLo ++;
		}

		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		Mailbox_TaskOgfDSP(msgPtr, isOn? OCF_DSP_AUDIO_ON:OCF_DSP_AUDIO_OFF);
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.GainTarget = 0x00;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.APP_GainLv = 0x00;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.VPRT_GainLv = 0x00;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.au_ad_ana_gain.Field.AuInGainCtrl_L = 0x18;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.au_ad_ana_gain.Field.AuInGainCtrl_R = 0x18;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.ear_sdmod_gain.Field.AuDAC_DSMGain = 0x00;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.ear_sdmod_gain.Field.SpkVolCtrl = (U8)HiLo;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.MicDigitalGain = 0x03;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.DspClock = gDriver_ctl.dspClock;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_gain_para.DacEnable = (isOn & gAUDIO_CtrlInfo.isNeedPilotTone)? 0 : 1;
		DRIVER_SendCmdToMailBox(msgPtr);

		gAUDIO_CtrlInfo.isNeedPilotTone = FALSE;
	}
}

PUBLIC void DRIVER_TurnOffSCO(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
	{
		AUDIO_SetAudioOff (AUDIO_SCO);
		DRIVER_SendSCODisableEvent();
		DRIVER_CheckATEnablePlay();
	}
}

PUBLIC void DRIVER_TurnOffA2DP(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
	{
		AUDIO_SetAudioOff (AUDIO_A2DP);
		DRIVER_SendA2DPDisableEvent();
	}
}

PUBLIC void DRIVER_TurnOffLineIn(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		AUDIO_SetAudioOff (AUDIO_LINE_IN);
		DRIVER_SendLineInDisableEvent();
	}
}

PUBLIC void DRIVER_TurnOffAsyncSCO(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
	{
		AUDIO_SetAudioOff (AUDIO_ASYNC_SCO);
		DRIVER_SendAsyncSCODisabledEvent();
	}
}

PUBLIC U8 DRIVER_DSPSoundEffect1520(DriverMsg XDATA_PTR msgPtr)
{
	if (msgPtr)
	{
		if(IS_DSP_BUSY)
			return QUE_PUTFRONT;

		if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
		{
			OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
			Mailbox_TaskOgfDSP(msgPtr, OCF_DSP_UPDATE_PARA);
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_update_para.cmd_type = DSP_UPDATE_WID;
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_update_para.cmd_para = (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN)) ? soundEffect_Ctl[SOUND_EFFECT_LINE_IN].modeSoundEffect : soundEffect_Ctl[SOUND_EFFECT_A2DP].modeSoundEffect;

			DRIVER_SendCmdToMailBox(msgPtr);
			return CONTINUE_LOOP;
		}
	}
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_DSPDBBOnOff1520(DriverMsg XDATA_PTR msgPtr)
{
	if (msgPtr)
	{
		if(IS_DSP_BUSY)
			return QUE_PUTFRONT;

		if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
		{
			OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
			Mailbox_TaskOgfDSP(msgPtr, OCF_DSP_UPDATE_PARA);
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_update_para.cmd_type = DSP_UPDATE_DBB;

			if (dbbOnOff_Ctl == DSP_DBB_OFF || dbbOnOff_Ctl ==DSP_DBB_NO_UPDATE)
			{
				msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_update_para.cmd_para = DSP_DBB_ON;
				dbbOnOff_Ctl = DSP_DBB_ON;
			}
			else if (dbbOnOff_Ctl == DSP_DBB_ON)
			{
				msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_update_para.cmd_para = DSP_DBB_OFF;
				dbbOnOff_Ctl = DSP_DBB_OFF;
			}

			DRIVER_SendCmdToMailBox(msgPtr);
			return CONTINUE_LOOP;
		}
	}
	return MEMORY_PUT;
}


PUBLIC U8 DRIVER_DSPRefGain1520(DriverMsg XDATA_PTR msgPtr)
{
	if (msgPtr)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		Mailbox_TaskOgfDSP(msgPtr, OCF_DSP_UPDATE_PARA);

		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.dsp_update_para.cmd_type = DSP_FEEDBACK_REF_GAIN;
		DRIVER_SendCmdToMailBox(msgPtr);
	}

	return CONTINUE_LOOP;
}

#ifdef SUPPORT_MIC_IN_A2DP_MODE
PUBLIC U8 DRIVER_A2DPAddMicHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	if(IS_DSP_BUSY)
		return QUE_PUTFRONT;
			
	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP))
	{
		DRIVER_SendAppIdleToMailBox();
			return QUE_PUTFRONT;
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		return MEMORY_PUT;
	}
	else if(DRIVER_IsRingToneOrVPOrATPlaying())
	{
		return QUE_PUTFRONT;
	}
	#ifdef SUPPORT_VOICE_COMMAND
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
	{
		DRIVER_BackUpOgfAppCmd(msgPtr);
		return MEMORY_PUT;
	}
	#endif
	
	if(msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.sidetone_en)
	{
		LightDbgPrint("open mic");
		gDriver_ctl.isSideToneEnabled = 1;
		gAudio_MappingTable[AUDIO_A2DP] = gAudio_MappingTable[AUDIO_A2DP] |AUDIO_ENABLE_MIC |AUDIO_ENABLE_AD;		
	}
	else
	{
		LightDbgPrint("close mic");
		gDriver_ctl.isSideToneEnabled = 0;
		gAudio_MappingTable[AUDIO_A2DP] = AUDIO_ENABLE_DA;		
	}
	
	#ifndef BLUETOOTH_SPEAKER //line-in for audio transparency(Non-A2DP MIC)
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		if(msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.sidetone_en)
		{
			DRIVER_SendAppIdleToMailBox();
			return QUE_PUTFRONT;
		}
	}
	#endif
	return MEMORY_PUT;
}
#endif

PUBLIC U8 DRIVER_MicTestHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	if (IS_DSP_BUSY)
		return QUE_PUTFRONT;
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
		AUDIO_SetMicSwitch(msgPtr->msgBodyPtr.driverCmd.micTestCmd.micEnableSet);
	else if(!DSP_IsDSPEnabled())
		AUDIO_SetECNROnOff(msgPtr->msgBodyPtr.driverCmd.micTestCmd.controlECNR);

	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_SwitchStereoMonoHandler1520(void)
{
	AUDIO_SwitchStereoMono();
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_SCOModeControlHandler1520(DriverMsg XDATA_PTR msgPtr)
{
#if defined SUPPORT_VOICE_COMMAND || defined ECNR_OFF_IN_NORMAL_SCO
	BOOL isSuspend;
	#ifdef SUPPORT_VOICE_COMMAND
	U8 lightMonoModeMaskOld;
	#endif

	if(IS_DSP_BUSY)
		return QUE_PUTFRONT;

	ASSERT(msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd < 4);

	#ifdef ECNR_OFF_IN_NORMAL_SCO
	if(msgPtr->msgBodyPtr.driverCmd.scoCmd.enablePara.scoPara.isECNROn)
	{
		gDriver_ctl.ecnrMask |= (0x01 << msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd);
	}
	else
	{
		gDriver_ctl.ecnrMask &= ~(0x01 << msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd);
	}
	#endif

	#ifdef SUPPORT_VOICE_COMMAND
	lightMonoModeMaskOld = gDriver_ctl.lightMonoModeMask;
	if(msgPtr->msgBodyPtr.driverCmd.scoCmd.enablePara.scoPara.isLightMonoMode)
	{
		gDriver_ctl.lightMonoModeMask |= (0x01 << msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd);
	}
	else
	{
		gDriver_ctl.lightMonoModeMask &= ~(0x01 << msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd);
	}
	#endif

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO) && gDriver_ctl.dspLink == msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd)
	{
		isSuspend = FALSE;

		if(msgPtr->msgBodyPtr.driverCmd.scoCmd.enablePara.scoPara.isECNROn)
		{
			if(gAlignDSP.DSP_nvram.DSP_Reserved_PARAM.Feature_Sel & ECNR_DISABLE)
			{
				isSuspend = TRUE;
			}
		}
		else
		{
			if(!(gAlignDSP.DSP_nvram.DSP_Reserved_PARAM.Feature_Sel & ECNR_DISABLE))
			{
				isSuspend = TRUE;
			}
		}

		#if !(defined AB1520S) && !(defined AB1520U)
		if(msgPtr->msgBodyPtr.driverCmd.scoCmd.enablePara.scoPara.isLightMonoMode)
		{
			if(!(lightMonoModeMaskOld & (0x01 << gDriver_ctl.dspLink)))
			{
				isSuspend = TRUE;
			}
		}
		else
		{
			if(lightMonoModeMaskOld & (0x01 << gDriver_ctl.dspLink))
			{
				isSuspend = TRUE;
			}
		}
		#endif

		if(isSuspend)
		{
			DRIVER_SendAppIdleToMailBox();
			gDriver_ctl.isResumeSCO = TRUE;
			return QUE_PUTFRONT;
		}
		else
		{
			gDriver_ctl.isResumeSCO = FALSE;
			return MEMORY_PUT;
		}
	}

	if(gDriver_ctl.isResumeSCO)
	{
		msgPtr->msgOpcode = DRIVER_SCO_CMD;
		gDriver_ctl.isResumeSCO = FALSE;
		return QUE_PUTFRONT;
	}
#else
	UNUSED(msgPtr);
#endif

	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_AsyncSCOCmdHandler1520(DriverMsg XDATA_PTR msgPtr)
{
	if(IS_DSP_BUSY)
		return QUE_PUTFRONT;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendRequestCloseFMEvent();
		return QUE_PUTFRONT;
	}

	#ifdef SUPPORT_VOICE_COMMAND
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
	{
		DRIVER_BackUpOgfAppCmd(msgPtr);
		return MEMORY_PUT;
	}
	#endif
	
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
		return MEMORY_PUT;
	
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
	{
		DRIVER_ATSwitchVPRTSCO();
		return QUE_PUTFRONT;
	}

	if(DRIVER_IsDSPOgfAppOn())
	{
		DRIVER_SendAppIdleToMailBox();
		return QUE_PUTFRONT;
	}

	if(!MMI_IsActiveLink(msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd))
	{
		return MEMORY_PUT;
	}

	if(gDriver_ctl.dspClock != DRIVER_CheckDSPClock(TRUE, AUDIO_ASYNC_SCO))
	{
		return QUE_PUTFRONT;
	}
	LightDbgPrint("AUDIO_ASYNC_SCO1");

	gDriver_ctl.dspLink = msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd;
	DRIVER_BackUpOgfAppCmd(msgPtr);
	AUDIO_SetAudioOn(AUDIO_ASYNC_SCO);

	ASSERT(gDriver_ctl.dspLink < 4);

	//gAlignDSP.DSP_nvram.DSP_Reserved_PARAM.Feature_Sel &= ~ECNR_DISABLE;
	#ifdef TWS_SETTINGS
	DRIVER_TWSClkSetting(FALSE, gDriver_ctl.dspLink);
	#endif
	gAlignDSP.dsp_param_update_ctl = AIRO_SYNC_MAGIC_TIME;
	
	Mailbox_TaskOgfAPP(msgPtr, OCF_APP_ASYNC_MONO_MODE);
	AUDIO_SetAudioInOutToMailBoxCmd(AUDIO_ASYNC_SCO, &msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.async_mono_para.audio_scenario);
	DRIVER_SendCmdToMailBox(msgPtr);
	gOS_PowerSavingMCUPauseRequest = TRUE;
	#ifdef PEQ_ENABLE
	//DRIVER_PEQReload();
	#endif

	SYS_ReleaseTimer(&gDriver_ctl.waitQosTimerPtr);
	return RETURN_FUNCTION;
}
PRIVATE void DRIVER_LINEModeSetting(U8 payloadEncode, U16 eSCORelay)
{
	if ((payloadEncode == FALSE) && (eSCORelay == SOUNDBAR_ESCO_DISABLE))
		gAUDIO_CtrlInfo.isNormalLineMode=1;
	else
		gAUDIO_CtrlInfo.isNormalLineMode=0;
}

PUBLIC BOOL DRIVER_IsDSPOgfAppOn(void)
{
	return (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))? TRUE : FALSE;
}

#ifdef TWS_SETTINGS
enum host_link_idx_enum
{
	HOST_SLAVE_0_LINK   = 0,
	HOST_MASTER_0_LINK  = 1,
	HOST_MASTER_1_LINK  = 2,
	HOST_SLAVE_1_LINK   = 3,
};

PUBLIC void DRIVER_TWSClkSetting(BOOL isRelay, U8 linkIndex)
{
	FOUR_BYTE_UNION_TYPE current_time;
	FOUR_BYTE_UNION_TYPE sc_start_count_time;
	U8 index;

	if (!isRelay)
	{
        LightDbgPrint("DSP - Slave Device Clock");
		index = HC_ConnectionHandleToIndex((MMI_GetConnHandleByLinkIndex(linkIndex)));
	}
    else
    {
        index = HOST_MASTER_0_LINK;
    }

    LightDbgPrint("DSP - Airostereo LinkIndex: %d", (U8)index);

	switch (index)
	{
		case HOST_MASTER_0_LINK:
		case HOST_MASTER_1_LINK:
            OS_ENTER_CRITICAL();
            LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_CLK_SOURCE_SEL,0x01);
            LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_SC_CNT_FREQ_DIVIDER,0x02);
            OS_EXIT_CRITICAL();
            current_time.value = HAL_ReadMasterClock();
			break;

		case HOST_SLAVE_0_LINK:
            OS_ENTER_CRITICAL();
            LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_CLK_SOURCE_SEL,0x02);
            LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_SC_CNT_FREQ_DIVIDER,0x02);
            OS_EXIT_CRITICAL();
            current_time.value = HAL_ReadSlave0Clock();
			break;

		case HOST_SLAVE_1_LINK:
			OS_ENTER_CRITICAL();
			LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_CLK_SOURCE_SEL,0x03);
			LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_SC_CNT_FREQ_DIVIDER,0x02);
			OS_EXIT_CRITICAL();
			current_time.value = HAL_ReadSlave1Clock();
			break;
	}

	sc_start_count_time.value = (  current_time.value + (0x40 - ( current_time.value % 0x40 )) );

	OS_ENTER_CRITICAL();
	LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_SC_NEXT_INSTANT_B0,sc_start_count_time.in_byte.B0);
	LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_SC_NEXT_INSTANT_B1,sc_start_count_time.in_byte.B1);
	LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_SC_NEXT_INSTANT_B2,sc_start_count_time.in_byte.B2);
	LC_SFR_WRITE_BY_E7(E7OFFS_WR_TWS_SC_NEXT_INSTANT_B3,sc_start_count_time.in_byte.B3);
	OS_EXIT_CRITICAL();
}
#endif //TWS_SETTINGS
#if 1 // Added to make the customer know the DSP playing status.
DSP_PLAYING_STATUS DSP_Playing_Status()
{
	return g_DSPPlayingStatus;
}
#endif
