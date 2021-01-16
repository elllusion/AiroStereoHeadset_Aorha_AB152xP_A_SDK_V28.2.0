//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _DRIVER_C_
#define _SOUND_EFFECT_C
#define _DBB_ON_OFF_C

#include <rc.h>

#include "..\MMI\MMI.h"
#include "..\MMI\MMI_SectorManager.h"
#include "..\MMI\MMI_Driver.h"
#include "..\HC\hc_interface.h"
#include "Audio_Mailbox.h"
#include "Driver_1520.h"

#include "sector_config1.h"
#include "sector_voice_data.h"
#include "sector_boundary.h"

#include "Driver.h"
#include "AudioControl.h"
#include "Driver_FM.h"
#include "Driver_Key.h"
#include "drv_sector.h"
#include "Audio_Transparency.h"
#ifdef RINGTONE_ENABLE
	#include "ringtone.h"
#endif
#ifdef SUPPORT_VOICE_PROMPT
	#include "VoicePrompt.h"
#endif
#ifdef SUPPORT_VOICE_COMMAND
	#include "VoiceCommand.h"
	#include "voice_command_adaption_data_ctl.h"
#endif
#ifdef PEQ_ENABLE
	#include "Peq_HPF.h"
#endif

#ifdef DEMOSOUND
#include "DemoSound.h"
#endif

#include "Driver_LED.h"
#include "Driver_Temp.h"
#if (HW_SPI_SUPPORTED)
#include "ExternalFlash_Sector.h"
#endif

#ifdef OPERATION_IN_BB_ASIC_VERSION
#include "audiocontrol_1520HW.h"
#endif

OSMQ XDATA OSMQ_AUDIO_Prompt_Command;
#define OSMQ_AUDIO_Prompt_Command_ptr		&OSMQ_AUDIO_Prompt_Command

DriverCtrlInfo XDATA gDriver_ctl;

#if (HW_SPI_SUPPORTED)
PRIVATE void ExtFlash_Init(void)
{
	U8 LDATA numOfSector;
	U8 XDATA_PTR headerPtr;
	U32 flhPtr;

	//DRV_SPI_BusInitial_Flash();
	if(!DRV_SPIFLH_IsExtFlhExist())
		return;

	if(DRV_SPIFLH_ReadBytes(0, &numOfSector, 1) || numOfSector == 0xFF)
		return;

	flhPtr = 1;

	headerPtr = OSMEM_Get(OSMEM_ptr1);

	while(numOfSector--)
	{
		DRV_SPIFLH_ReadBytes(flhPtr, headerPtr, sizeof(SECTOR_SCRIPT));

		if(((SECTOR_SCRIPT XDATA_PTR)headerPtr)->sectorID == EXTERNAL_FLASH_SECTOR_VOICE_PROMPT_DATA_ID)
		{
			gDriver_ctl.extFlashCtl.vpDataSector = ((SECTOR_SCRIPT XDATA_PTR)headerPtr)->sectorAddress;
			LightDbgPrint("Get ExtVP!!:%X",(U32)gDriver_ctl.extFlashCtl.vpDataSector);
			break;
		}
		flhPtr += sizeof(SECTOR_SCRIPT);
	}
	OSMEM_Put(headerPtr);

	gDriver_ctl.extFlashCtl.status = DRIVER_EXT_FLASH_OK;
}
#endif

extern PUBLIC void LED_Init(void);
extern PUBLIC void DRIVER_LED_Init(void);
#ifdef DEMOSOUND
extern PUBLIC void DemoSound_Init(void);
#endif
extern BOOL IsSectorMemberValid(U8 CODE_PTR pSectorAddr, U32 Size);

PUBLIC void DRIVER_Init(void)
{
	LED_Init();
	DRIVER_LED_Init();

	MailBox_Init();

	#ifdef RINGTONE_ENABLE
	Ring_Init();
	#endif

	AUDIO_Init();
	DRIVER_Key_Init();

	gDriver_ctl.dspLink = 0xFF;

	gSYS_allow_updating = TRUE;
	gDriver_ctl.isReadyToDSPwOff |= DRIVER_READY;
	gDriver_ctl.OTAVoicepromptsExist = FALSE;
	
	#ifdef PEQ_ENABLE
	PEQ_SetRealtimeEnableMode(INIT_ENABLED_MODE);
	PEQ_SetPeqHpfLoadSectorAddress(PEQ_GetSectorMode(PEQ_A2DP));
	#endif

	#if (HW_SPI_SUPPORTED)
	ExtFlash_Init();
	#endif
	#ifdef DEMOSOUND
	DemoSound_Init();
	#endif
}

extern PUBLIC BOOL DRIVER_LED_IsFGLEDOn(void);
PRIVATE void DRIVER_CheckPwrKey()
{
	if (gDriver_ctl.isReadyToDSPwOff == READY_TO_POWEROFF)
	{
		if (!DRIVER_LED_IsFGLEDOn())
		{
			gDriver_ctl.isReadyToDSPwOff = FALSE;
			gMMI_Drv_Handler.fDriverReadyPowerOffEvent();
		}
	}
}

extern PUBLIC BOOL MMI_DRV_IsSpeakerMuteByLink(U8 linkIndex);
extern PUBLIC BOOL MMI_DRV_IsSpeakerMute(void);
PRIVATE void DRIVER_MuteUnMuteAmpOpenCloseHandler(void)
{
	if(gAlignDSP.DSPOUT_MUTE)
	{
		if(!gAUDIO_CtrlInfo.isMuteForVPRingtone)
		{
			if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
			{
				if(!gAUDIO_CtrlInfo.isMuteForPEQ && !gAUDIO_CtrlInfo.isMuteForRedial && !gAUDIO_CtrlInfo.isLineInMute &&  !(gAUDIO_CtrlInfo.isMuteBySoundLevel && MMI_MUTE_LINE_IN_WHEN_VOLUME_IS_ZERO_FEAT))
				{
					gAlignDSP.DSPOUT_MUTE = 0;
					LightDbgPrint("DSPOUT_MUTE = 0");
				}
			}
			else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
			{
				if(!gAUDIO_CtrlInfo.isMuteForPEQ && !gAUDIO_CtrlInfo.isMuteForRedial && !(gAUDIO_CtrlInfo.isMuteBySoundLevel && MMI_MUTE_A2DP_WHEN_VOLUME_IS_ZERO_FEAT) &&
					!MMI_DRV_IsSpeakerMuteByLink(gDriver_ctl.dspLink))
				{
					gAlignDSP.DSPOUT_MUTE = 0;
					LightDbgPrint("1DSPOUT_MUTE = 0");
				}
			}
			else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
			{
				if(	!gAUDIO_CtrlInfo.isMuteForRedial && !(gAUDIO_CtrlInfo.isMuteBySoundLevel && MMI_MUTE_SPEAKER_WHEN_VOLUME_IS_ZERO_FEAT) &&
					!MMI_DRV_IsSpeakerMuteByLink(gDriver_ctl.dspLink) && !gDriver_ctl.rejectCallMuteSCOCmd)
				{
					gAlignDSP.DSPOUT_MUTE = 0;
					LightDbgPrint("2DSPOUT_MUTE = 0");
				}
			}
		}
	}

	//AMP Control
	switch(gAUDIO_CtrlInfo.muteCloseAmpState)
	{
		case AMP_OFF:
			SYS_ReleaseTimer(&gDriver_ctl.spkMuteAmpTimerPtr);
			if(MMI_CLOSE_AMP_WHILE_SPEAKER_MUTED_FEAT)
			{
				if((AUDIO_PURE_ON > AUDIO_CheckAnyAudioEnabled() &&  !gAlignDSP.DSPOUT_MUTE) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
					AUDIO_Open_AMP();
			}
			break;
		case AMP_ON:
			SYS_ReleaseTimer(&gDriver_ctl.spkMuteAmpTimerPtr);
			if(MMI_CLOSE_AMP_WHILE_SPEAKER_MUTED_FEAT && gAlignDSP.DSPOUT_MUTE && !DRIVER_IsRingToneOrVPOrATPlaying() &&  !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM) && !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND) && OSMQ_Entries(OSMQ_AUDIO_Prompt_Command_ptr) == 0)
			{
				#ifdef PEQ_ENABLE
				if(gDriver_ctl.peqMuteTimer == (OST XDATA_PTR)NULL)
				#endif
				{
					SYS_SetTimer(&gDriver_ctl.spkMuteAmpTimerPtr,(U32)gMMI_nvram.timerCtl.CloseAmpAfterMuteSpkTime * 320L);
					gAUDIO_CtrlInfo.muteCloseAmpState= AMP_WAIT_CLOSE;
				}
			}
			break;
		case AMP_WAIT_CLOSE:
			if(!gAlignDSP.DSPOUT_MUTE || DRIVER_IsRingToneOrVPOrATPlaying() || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM) ||AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
			{
				SYS_ReleaseTimer(&gDriver_ctl.spkMuteAmpTimerPtr);
				gAUDIO_CtrlInfo.muteCloseAmpState= AMP_ON;
			}
			if(SYS_IsTimerExpired(&gDriver_ctl.spkMuteAmpTimerPtr))
			{
				AUDIO_Close_AMP();
			}
			break;
	}
}

extern PUBLIC U16 AUDIO_Get3Wire0xA5(U16 HiLo, U8 mask);
PRIVATE void DRIVER_CheckStartAnchor(void)
{
	if(gAlignDSP.DEC_A2DP_START_ANCHOR == 0x0002)
	{
		#ifdef OPERATION_IN_BB_ASIC_VERSION
		if(AUDIO_COMPONENT_NO != AUDIO_CheckAnyAudioEnabled() && gAUDIO_CtrlInfo.isDAEnabled &&
		((gAUDIO_CtrlInfo.AUDIO_OUT_SCENARIO_SEL == AU_OUT_DA) || (gAUDIO_CtrlInfo.AUDIO_OUT_SCENARIO_SEL == AU_OUT_DA_96K)))
		{
			#ifdef AB1520S
			U16 HiLo;
			SYS_3WireRead(0xA5, HiLo);
			HiLo &= 0xFFFC;
			HiLo = AUDIO_Get3Wire0xA5(HiLo, 0x03);
			SYS_3WireWrite(0xA5, ((U8)(HiLo>>8)), ((U8)HiLo)); //A5 D1,D0
			#else
			if ( !(gMMI_nvram.gpioAmpGeneral > 2 && gMMI_nvram.gpioAmpGeneral < 29) )
            {
                AUDIO_SetAudioOut(TRUE);
            }
			#endif
		}
		#endif

		gAlignDSP.DEC_A2DP_START_ANCHOR = 0;
		if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
			gMMI_Drv_Handler.fDriverA2DPStartAnchorEvent();
	}
}



extern PUBLIC void DRIVER_LED_Scheduler(void);
extern PUBLIC void LED_CheckTimer(void);

PUBLIC void DRIVER(void)
{
	DRIVER_ProcMmiCmd(); // ProcMMICmd must be prior than keypolling coz the key para init issue
#ifdef FM_ENABLE
	DRIVER_FM_Handler();
#endif
	DRIVER_Key_Polling();
	DRIVER_RingtoneVPFMATCmdScheduler();
	DRIVER_MailBoxHandler();
	DRIVER_CheckStartAnchor();
	DRIVER_CheckTimer();
	DRIVER_CheckPwrKey();

	DRIVER_MuteUnMuteAmpOpenCloseHandler();
	DRIVER_LED_Scheduler();
	LED_CheckTimer();

	DRIVER_TEMP_Check();

	#ifdef TWO_STEP_CHARGING_FUNCTION
	DRIVER_TEMP_TwoStepsCharging();
	#endif

	#ifdef NTC_RATIO_FUNCTION
	DRIVER_TEMP_NTCRatio();
	#endif

	#ifdef DEMOSOUND
	DemoSound_Polling();
	#endif

    #ifdef DSP_ENABLE
	DSP_DebugPrint();
	#endif
}

#if defined RINGTONE_ENABLE || defined SUPPORT_VOICE_PROMPT
PRIVATE void DRIVER_CheckMuteByMediaID(U16 ID)
{
	if( ID == MEDIA_EVT_KEY_RDIAL ||ID == MEDIA_EVT_KEY_RDIAL_SECONDARY ||ID== MEDIA_EVT_KEY_SET_VOICE_RECOGNITION ||
		ID == MEDIA_EVT_KEY_SET_VOICE_RECOGNITION_SECONDARY||ID ==MEDIA_EVT_REDIAL_SUCCESSFUL_EVT)
	{
		gAUDIO_CtrlInfo.isMuteForRedial =TRUE;
		gAlignDSP.DSPOUT_MUTE = 1;
		LightDbgPrint("6DSPOUT_MUTE = 1");
		SYS_SetTimer(&gDriver_ctl.redialMuteTimerPtr,2*ONE_SEC);
	}
	else if(MMI_POWER_DROP_SAVE_LINK_HISTORY_FEAT)
	{
		gAUDIO_CtrlInfo.isMuteForVPRingtone = TRUE;
		gAlignDSP.DSPOUT_MUTE = 1;
		LightDbgPrint("5DSPOUT_MUTE = 1");
	}
}
#endif

PUBLIC void DRIVER_RejectCallMuteSCOCmd(BOOL value)
{
	gDriver_ctl.rejectCallMuteSCOCmd = value;
}

PUBLIC void DRIVER_RingtoneVPFMATCmdScheduler(void)
{
	DriverMsg XDATA_PTR msgPtr;
	if(!DRIVER_IsRingToneOrVPOrATPlaying() || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
	{
		if(gDriver_ctl.isAudioHandlingPending || gDriver_ctl.suspendDSPReason)
			return;

		if(!DRIVER_IsMiscCtrlTypeEnabled(DRIVER_VP_RING_LOCKED_FOR_FM) && !(IS_DSP_BUSY)) // FM is in On or Off state
		{
			GET_AUDIO_PROMPT_CMD:
			if((msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get (OSMQ_AUDIO_Prompt_Command_ptr)) != (DriverMsg XDATA_PTR)NULL)
			{
				if(msgPtr->msgOpcode == DRIVER_FAKE_MEDIA_VP_RINGTONE_CMD)
				{
					msgPtr->dataOffset = EVT_OFFSET;
					msgPtr->eventCode = DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT;
					gMMI_Drv_Handler.fDriverFakeMediaEvent((MMIMsgType XDATA_PTR)msgPtr);
					goto GET_AUDIO_PROMPT_CMD;
				}
				#ifdef FM_ENABLE
				if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM) && !DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_RESUME_FM))
				{
					OSMQ_PutFront(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
					DRIVER_FM_InternalPowerOff();
					DRIVER_SetMiscCtrlMap(DRIVER_IS_RESUME_FM, TRUE);
					return;
				}
				#endif
				switch(msgPtr->msgOpcode)
				{
					case DRIVER_RINGTONE_CMD:
						if(DRIVER_RingtoneCmdHandler(msgPtr))
							OSMEM_Put((U8 XDATA_PTR)msgPtr);
						else
							OSMQ_PutFront(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
						break;

					case DRIVER_VOICEPROMPT_CMD:
						#ifdef SUPPORT_VOICE_PROMPT
						if(!DRIVER_VoicePromptCmdHandler(msgPtr))
							OSMQ_PutFront(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
						else
						#endif
							OSMEM_Put((U8 XDATA_PTR)msgPtr);
						break;

					case DRIVER_AUDIO_TRANSPARENCY_CMD:
						if(!DRIVER_AudioTransparencyCmdHandler(msgPtr))
							OSMQ_Put(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
						else
							OSMEM_Put((U8 XDATA_PTR)msgPtr);
						break;
					default:
						OSMEM_Put((U8 XDATA_PTR)msgPtr);
						break;
				}
			}
			else
			{
				gAUDIO_CtrlInfo.isMuteForVPRingtone = FALSE;
				#ifdef FM_ENABLE
				if(gDriver_ctl.resumeFMTimerPtr == (OST XDATA_PTR)NULL  && DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_RESUME_FM) )
				{
					SYS_SetTimer(&gDriver_ctl.resumeFMTimerPtr, ONE_SEC);
				}
				else if(SYS_IsTimerExpired(&gDriver_ctl.resumeFMTimerPtr))
				{
					DRIVER_FM_Resume();
				}
				#endif
			}
		}
	}

	if(SYS_IsTimerExpired(&gDriver_ctl.resumeAppTimer))
	{
		if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT) || gDriver_ctl.isAudioHandlingPending ||AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
		{
			SYS_SetTimer(&gDriver_ctl.resumeAppTimer, ONE_SEC);
		}
		else if(gDriver_ctl.backUpAudioCmdPtr)
		{
			OSMQ_PutFront(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)gDriver_ctl.backUpAudioCmdPtr);
			gDriver_ctl.backUpAudioCmdPtr = NULL;
			DRIVER_ProcMmiCmd();
			#ifdef SUPPORT_MIC_IN_A2DP_MODE
			if(DRIVER_CheckATEnablePlay())
				AudioTrspr_Start();
			#endif
		}
	}
}

PUBLIC void DRIVER_CheckTimer(void)
{
	//Mute Reminder, send event to MMI every n secs if MIC is mute.
	if(SYS_IsTimerExpired(&gDriver_ctl.muteReminderTimerPtr))
	{
		gMMI_Drv_Handler.fDriverMuteReminderEvent();
		SYS_SetTimer(&gDriver_ctl.muteReminderTimerPtr, (U32)gMMI_nvram.timerCtl.MuteReminderTime * ONE_SEC);
	}

	Driver_Key_CheckTimer();

	#ifdef PEQ_ENABLE
	if(SYS_IsTimerExpired(&gDriver_ctl.peqMuteTimer))
	{
		gAUDIO_CtrlInfo.isMuteForPEQ = FALSE;
	}
	#endif

	if(SYS_IsTimerExpired(&gDriver_ctl.redialMuteTimerPtr))
	{
		gAUDIO_CtrlInfo.isMuteForRedial = FALSE;
	}
	AUDIO_CheckTimer();
	
	if(SYS_IsTimerExpired(&gDriver_ctl.checkATSwitchDelayTimer))
	{
        #ifdef SUPPORT_MIC_IN_A2DP_MODE
		if(MMI_GetAudioTransparencyEnable())
		{
			if(DRIVER_CheckATEnablePlay() && !(IS_DSP_BUSY) && OSMQ_Entries(OSMQ_AUDIO_Prompt_Command_ptr) == 0)
			{
				gMMI_Drv_Handler.fDriverATSwitchEvent();
				AudioTrspr_Start();
				SYS_ReleaseTimer(&gDriver_ctl.checkATSwitchDelayTimer);
			}
			else if(gDriver_ctl.checkATSwitchDelayTimer == (OST XDATA_PTR)NULL)
			{
				SYS_SetTimer(&gDriver_ctl.checkATSwitchDelayTimer, (U32) 320L);
			}			
		}
        #endif        
	}
}

PUBLIC void DRIVER_SetA2DPVolPara(DriverVolumeSetCmd XDATA_PTR volPtr)
{
	gDriver_ctl.a2dpSpkCtl.soundLevel		= volPtr->soundLevel;
	gDriver_ctl.a2dpSpkCtl.spkVols			= volPtr->spkVols ;
	gDriver_ctl.a2dpSpkCtl.analogDaGain		= volPtr->analogDaGain;
	gDriver_ctl.a2dpSpkCtl.smallStepLevel	= volPtr->smallStepLevel;
	gDriver_ctl.a2dpSpkCtl.micVols 			= volPtr->micVols;
	gDriver_ctl.a2dpSpkCtl.analogAdGain_L 	= volPtr->analogAdGain_L;
	gDriver_ctl.a2dpSpkCtl.analogAdGain_R 	= volPtr->analogAdGain_R;
}

PUBLIC void DRIVER_SetLineInVolPara(DriverVolumeSetCmd XDATA_PTR volPtr)
{
	gDriver_ctl.lineInSpkMicCtl.soundLevel			= volPtr->soundLevel;
	gDriver_ctl.lineInSpkMicCtl.spkVols 			= volPtr->spkVols;
	gDriver_ctl.lineInSpkMicCtl.analogDaGain		= volPtr->analogDaGain;
	gDriver_ctl.lineInSpkMicCtl.analogAdGain_R		= volPtr->analogAdGain_R;
	gDriver_ctl.lineInSpkMicCtl.analogAdGain_L		= volPtr->analogAdGain_L;
	gDriver_ctl.lineInSpkMicCtl.smallStepLevel		= volPtr->smallStepLevel;
	gDriver_ctl.lineInSpkMicCtl.micVols 			= volPtr->micVols;
}

PUBLIC void DRIVER_SetNormalVolPara(DriverVolumeSetCmd XDATA_PTR volPtr)
{
	if(volPtr->soundLevel != VOLUME_DONT_CARE)
		gDriver_ctl.normalSpkMicCtl.soundLevel 		= volPtr->soundLevel;

	if(volPtr->spkVols != VOLUME_DONT_CARE)
	{
		gDriver_ctl.normalSpkMicCtl.spkVols 		= volPtr->spkVols;
		gDriver_ctl.normalSpkMicCtl.analogDaGain	= volPtr->analogDaGain;
		gDriver_ctl.normalSpkMicCtl.smallStepLevel	= volPtr->smallStepLevel;
		#ifdef AB1500
		gDriver_ctl.normalSpkMicCtl.spkDigitalGain	= 7;
		gDriver_ctl.normalSpkMicCtl.spkDigitalGainM = volPtr->digitalGainM;
		gDriver_ctl.normalSpkMicCtl.spkDigitalGainE = volPtr->digitalGainE;
		#endif
	}

	#ifdef SUPPORT_VOICE_COMMAND
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
	{
		gDriver_ctl.normalSpkMicCtl.micVols = gDriver_ctl.voiceCmdMiscCtl.micVols;
		gDriver_ctl.normalSpkMicCtl.analogAdGain_R = gDriver_ctl.voiceCmdMiscCtl.analogAdGain_R;
		gDriver_ctl.normalSpkMicCtl.analogAdGain_L = gDriver_ctl.voiceCmdMiscCtl.analogAdGain_L;
	}
	else
	#endif
	{
		gDriver_ctl.normalSpkMicCtl.micVols	= volPtr->micVols;
		gDriver_ctl.normalSpkMicCtl.analogAdGain_R = volPtr->analogAdGain_R;
		gDriver_ctl.normalSpkMicCtl.analogAdGain_L = volPtr->analogAdGain_L;
	}
}

PRIVATE U8 DRIVER_MMIReadyCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	gDriver_ctl.isReadyToDSPwOff |= MMI_READY;
	DRIVER_TurnOffPureOn();

	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_UpdateGeneralParaCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	//MMI gets into FakeON state.
	//This maybe move to some where(Dsleep CMD) in the future.
	#ifdef PEQ_ENABLE
	PEQ_HPF_UpdateParameters();
	#endif

	UNUSED(msgPtr);
	return MEMORY_PUT;
}

PUBLIC void DRIVER_SetInbandSCOMuteUnMute(BOOL isMute)
{
	UNUSED(isMute);
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
		DRIVER_SetNormalVolumeToHW();
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
		DRIVER_SetA2DPVolumeToHW();
}

PUBLIC void DRIVER_SetMicMuteUnMute(BOOL isMute)
{
	UNUSED(isMute);
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
		DRIVER_SetNormalVolumeToHW();
}

PRIVATE U8 DRIVER_SetNormalVolumeCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	BOOL isDiff = FALSE;
	DriverVolumeSetCmd XDATA_PTR volPtr = &msgPtr->msgBodyPtr.driverCmd.volSetCmd;

	if(volPtr->soundLevel != VOLUME_DONT_CARE && gDriver_ctl.normalSpkMicCtl.soundLevel != volPtr->soundLevel)
		isDiff = TRUE;

	if(volPtr->spkVols != VOLUME_DONT_CARE)
	{
		if(	gDriver_ctl.normalSpkMicCtl.spkVols != volPtr->spkVols ||
			gDriver_ctl.normalSpkMicCtl.analogDaGain != volPtr->analogDaGain ||
			gDriver_ctl.normalSpkMicCtl.smallStepLevel 	!= volPtr->smallStepLevel)
			isDiff = TRUE;
	}

	if(volPtr->analogAdGain_R != VOLUME_DONT_CARE)
	{
		if( gDriver_ctl.normalSpkMicCtl.micVols	!= volPtr->micVols ||
			gDriver_ctl.normalSpkMicCtl.analogAdGain_R != volPtr->analogAdGain_R ||
			gDriver_ctl.normalSpkMicCtl.analogAdGain_L != volPtr->analogAdGain_L)
			isDiff = TRUE;
	}

	DRIVER_SetNormalVolPara(volPtr);
	if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP)	&&	!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_LINE_IN) && isDiff)
		DRIVER_SetNormalVolumeToHW();

	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_SetA2DPVolumeCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	DRIVER_SetA2DPVolPara(&msgPtr->msgBodyPtr.driverCmd.a2dpVolSetCmd);

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
	{
		DRIVER_SetA2DPVolumeToHW();
	}
	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_SetLineInVolumeCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	BOOL isDiff = OS_memcmp(&gDriver_ctl.lineInSpkMicCtl, &msgPtr->msgBodyPtr.driverCmd.lineInVolSetCmd, sizeof(DriverVolumeSetCmd));

	DRIVER_SetLineInVolPara(&msgPtr->msgBodyPtr.driverCmd.lineInVolSetCmd);

	if((AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) && isDiff) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
	{
		DRIVER_SetLineInVolumeToHW();
	}
	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_RingToneVPCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	if(MMI_IGNORE_RING_VP_WHEN_FM_ON_FEAT && AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		// Do Nothing to Put Memory
	}
	else if(MMI_IGNORE_RING_VP_WHEN_LINE_IN_FEAT && AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		// Do Nothing to Put Memory
	}
	else if(!DRIVER_IsRingToneOrVPOrATPlaying())
	{
		label_add_queue_and_return:
		if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
		{
			if(IS_DSP_BUSY)
			{
				return QUE_PUTFRONT;
			}
			if(gDriver_ctl.isATSwitch == AT_NOT_IN_SWITCH)
				DRIVER_ATSwitchVPRTSCO();
		}
		OSMQ_Put(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
		return CONTINUE_LOOP;
	}
	else if(MMI_QUEUE_RINGTONE_FEAT)
	{
		 goto label_add_queue_and_return;	//using goto for less code size
	}
	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_AudioTransparencyPushCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	OSMQ_Put(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
	return CONTINUE_LOOP;
}

PRIVATE U8 DRIVER_StopSpecificRingCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	#ifdef RINGTONE_ENABLE
	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_RINGTONE) && gDriver_ctl.playingRingtoneId == msgPtr->msgBodyPtr.driverCmd.stopSpecificRTCmd.id)
	{
		if(!Ring_Pause())
		{
			return QUE_PUTFRONT;
		}
	}
	#else
	UNUSED(msgPtr);
	#endif
	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_StopSpecificVPCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	#ifdef SUPPORT_VOICE_PROMPT
	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_VOICEPROMPT) && gDriver_ctl.playingRingtoneId == msgPtr->msgBodyPtr.driverCmd.stopSpecificVPCmd.id)
	{
		if(!VoicePrompt_Pause())
		{
			return QUE_PUTFRONT;
		}
	}
	#else
	UNUSED(msgPtr);
	#endif
	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_StopRingCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
#ifdef RINGTONE_ENABLE
	if(Ring_Pause())
	{
		DRIVER_ClearAudioCmd(DRIVER_INTERNAL_CLEAN_ALL_AUDIO_CMD, VOICE_PROMPT_ALL_CLEAN);
		return MEMORY_PUT;
	}
	return QUE_PUTFRONT;
#else
	DRIVER_ClearAudioCmd(DRIVER_INTERNAL_CLEAN_ALL_AUDIO_CMD, VOICE_PROMPT_ALL_CLEAN);
	return MEMORY_PUT;
#endif
}

PRIVATE U8 DRIVER_StopVPCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	#ifdef SUPPORT_VOICE_PROMPT
	if(VoicePrompt_Pause())
	{
		DRIVER_ClearAudioCmd(DRIVER_INTERNAL_CLEAN_ALL_AUDIO_CMD, VOICE_PROMPT_ALL_CLEAN);
		return MEMORY_PUT;
	}
	return QUE_PUTFRONT;
	#else
	DRIVER_ClearAudioCmd(DRIVER_INTERNAL_CLEAN_ALL_AUDIO_CMD, VOICE_PROMPT_ALL_CLEAN);
	return MEMORY_PUT;
	#endif
}

PRIVATE U8 DRIVER_FakeMediaVPRingCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	OSMQ_Put(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
	return CONTINUE_LOOP;
}

PRIVATE U8 DRIVER_SCOCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_SCOCmdHandler1520(msgPtr);
}

PRIVATE U8 DRIVER_A2DPCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_A2DPCmdHandler1520(msgPtr);
}

PRIVATE U8 DRIVER_LineInCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	DRIVER_SetMiscCtrlMap(DRIVER_IS_RESUME_FM, FALSE);
	return DRIVER_LineInCmdHandler1520(msgPtr);
}

PRIVATE U8 DRIVER_StopLineInCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_StopLineInCmdHandler1520(msgPtr);
}


PRIVATE U8 DRIVER_StopDSPCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_StopDSPCmdHandler1520(msgPtr);
}

PRIVATE U8 DRIVER_StopDSPPowerOffCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_StopDSPPowerOffCmdHandler1520(msgPtr);
}

#ifdef PEQ_ENABLE
PUBLIC void DRIVER_PEQSetMuteTimer(void)
{
	//Mantis 11468 [Cannt unmute --> if unmute, pop noise]
	//workaround on DSP
	#if 0
	if(gMMI_nvram.timerCtl.peqChangeMutetime)
	{
		SYS_SetTimer(&gDriver_ctl.peqMuteTimer,(U32) gMMI_nvram.timerCtl.peqChangeMutetime*32L);
		gAUDIO_CtrlInfo.isMuteForPEQ = TRUE ;
		gAlignDSP.DSPOUT_MUTE = 1;
		LightDbgPrint("4DSPOUT_MUTE = 1");
	}
	#endif
}

PRIVATE void DRIVER_PEQLDBGPEQIndex(U8 peqMode)
{
	if(peqMode == PEQ_LINE_IN)
	{
		LightDbgPrint("Line-in: Now PEQ index %d", (U8)PEQ_GetModeIndex(PEQ_LINE_IN));
		LightDbgPrint("Line-in: Now sector mode %d", (U8)PEQ_GetSectorMode(PEQ_LINE_IN));
	}
	else if(peqMode == PEQ_A2DP)
	{
		LightDbgPrint("A2DP: Now PEQ index %d", (U8)PEQ_GetModeIndex(PEQ_A2DP));
		LightDbgPrint("A2DP: Now sector mode %d", (U8)PEQ_GetSectorMode(PEQ_A2DP));
	}
}

PUBLIC void DRIVER_PEQReload(void)
{
	U8 peqMode;
	U8 sectorMode;
	
	if(gAlignDSP.DSP_CHANGE_PEQ  == 0)
	{
		PEQ_SetPeqHpfLoadSectorAddress(PEQ_GetSectorMode(PEQ_A2DP));
		if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP))
		{
			if(0 == PEQ_GetModeNumber(PEQ_A2DP))
				return;
			if(PEQ_IsCustomPEQSectorEmpty(PEQ_A2DP) && (PEQ_GetSectorMode(PEQ_A2DP)==PEQ_LOAD_CUSTOM))
			{
				LightDbgPrint("==Cust Wrong %X !!!!!!",(U32)gPeq_ctl.sectorPEQAddress);
				return;
			}
			peqMode = PEQ_A2DP;
			sectorMode = PEQ_GetSectorMode(PEQ_A2DP); 
		}
		else if (AUDIO_COMPONENT_IS_ACTIVE (AUDIO_LINE_IN))
		{
			if(0 == PEQ_GetModeNumber(PEQ_LINE_IN))
				return;
			peqMode = PEQ_LINE_IN;
			sectorMode = PEQ_GetSectorMode(PEQ_LINE_IN); 
		}
		#ifdef AIR_MODULE
		else if(MMI_IsLocalDeviceType(PRODUCT_AIR_SCO_WOOFER) && AUDIO_COMPONENT_IS_ACTIVE (AUDIO_SCO))
		{
			if(0 == PEQ_GetModeNumber(PEQ_A2DP))
				return;
			peqMode = PEQ_A2DP;
			sectorMode = PEQ_GetSectorMode(PEQ_A2DP); 
		}
		#endif
		else
		{
			return;
		}

		DRIVER_PEQLDBGPEQIndex(peqMode);
		
		if(PEQ_GetModeIndex(peqMode))
		{
			PEQ_SetPeqHpfLoadSectorAddress(sectorMode);
			if(!PEQ_SetOn(peqMode))
			{
				goto SEND_RELOAD_CMD;
			}			
		
			#ifdef PEQ_EXPANSION_ENABLE
			if(!PEQ_Expansion_Set(peqMode))
			{
				goto SEND_RELOAD_CMD;
			}
			#endif
			
			#ifdef PEQ_UNDER_100HZ_ENABLE
			if(!PEQ_Under100Hz_Set(peqMode))
			{
				goto SEND_RELOAD_CMD;
			}
			#endif
		}
		else if(!PEQ_SetOff())
		{
			goto SEND_RELOAD_CMD;
		}
		
		#ifdef TWS_SETTINGS
		MMI_DRV_SyncPEQRelayerToFollower(peqMode, PEQ_GetModeIndex(peqMode), sectorMode);
		#endif

		if(PEQ_GetModeIndex(peqMode) && HPF_IsHPFEnabledInSector(peqMode))
		{			
			if(!HPF_SetOn(peqMode))
			{
				goto SEND_RELOAD_CMD;
			}
		}
		else if(!HPF_SetOff())
		{
			goto SEND_RELOAD_CMD;
		}
		return;

		SEND_RELOAD_CMD:
		DRIVER_SendMsgToDriverQue(DRIVER_PEQ_RELOAD_CMD);
	}
}

PRIVATE void DRIVER_CaculatePEQMode(U8 component)
{
	U8 numOfPEQ;
	U8 modeIndex;

	if (component == PEQ_A2DP)
		numOfPEQ = PEQ_GetModeNumber(PEQ_A2DP);
	else
		numOfPEQ = PEQ_GetModeNumber(PEQ_LINE_IN) ? PEQ_GetModeNumber(PEQ_LINE_IN):PEQ_GetModeNumber(PEQ_A2DP);

	modeIndex = PEQ_GetModeIndex(component);
	
	if(numOfPEQ < modeIndex)
	{
		PEQ_SetSectorMode(component, PEQ_LOAD_DEFAULT);
		if(MMI_MUSIC_FORCE_USE_PEQ_FEAT)
		{
			LightDbgPrint("FORCE to use PEQ feature");
			//1->2->3->1->2->3
			PEQ_SetModeIndex(component, 1);
		}
		else
		{
			LightDbgPrint("NOT FORCE to use PEQ feature");
			//0->1->2->3->0->1->2->3
			PEQ_SetModeIndex(component, 0);
		}
	}	
	else if(numOfPEQ == modeIndex)
	{
		if(PEQ_GetSectorMode(component) == PEQ_LOAD_CUSTOM)
		{
			PEQ_SetSectorMode(component, PEQ_LOAD_DEFAULT);
		}
		else if(PEQ_GetSectorMode(component) == PEQ_LOAD_DEFAULT && !PEQ_IsCustomPEQSectorEmpty(component))
		{
			PEQ_SetSectorMode(component, PEQ_LOAD_CUSTOM);
		}
		
		if(MMI_MUSIC_FORCE_USE_PEQ_FEAT)
		{
			LightDbgPrint("FORCE to use PEQ feature");
			//1->2->3->1->2->3
			PEQ_SetModeIndex(component, 1);
		}
		else
		{
			LightDbgPrint("NOT FORCE to use PEQ feature");
			//0->1->2->3->0->1->2->3
			PEQ_SetModeIndex(component, 0);
		}
	}
	else
	{
		modeIndex += 1;
		PEQ_SetModeIndex(component, modeIndex);
	}
	
	LightDbgPrint("PEQ cal index:%d, mode:%d", (U8)PEQ_GetModeIndex(component), (U8)PEQ_GetSectorMode(component));
}

PUBLIC void DRIVER_ChangePEQMode(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
	{
		if(PEQ_GetModeNumber(PEQ_A2DP) == 0 && PEQ_GetSectorMode(PEQ_A2DP) == PEQ_LOAD_DEFAULT)
		{
			return;
		}
		DRIVER_CaculatePEQMode(PEQ_A2DP);
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		if(PEQ_GetModeNumber(PEQ_LINE_IN) == 0 && PEQ_GetSectorMode(PEQ_LINE_IN) == PEQ_LOAD_DEFAULT)
		{
			return;
		}
		DRIVER_CaculatePEQMode(PEQ_LINE_IN);
	}

	if(MMI_REMEMBER_PEQ_CHANGE_FEAT && !MMI_DEFAULT_PEQ_ASSIGNMENT_FEAT)
	{
		gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_A2DP] =	PEQ_GetModeIndex(PEQ_A2DP);
		gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_LINE_IN] = PEQ_GetModeIndex(PEQ_LINE_IN);
		gMMI_driver_variation_nvram.misc_para.PEQSectorMode[PEQ_A2DP] = PEQ_GetSectorMode(PEQ_A2DP);
		gMMI_driver_variation_nvram.misc_para.PEQSectorMode[PEQ_LINE_IN] = PEQ_GetSectorMode(PEQ_LINE_IN);
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
}

PRIVATE U8 DRIVER_PEQChangeCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !AUDIO_COMPONENT_IS_ACTIVE (AUDIO_LINE_IN))
	{
		return MEMORY_PUT;
	}

	if(gAlignDSP.DSP_CHANGE_PEQ)
	{
		return MEMORY_PUT;
	}
	DRIVER_ChangePEQMode();

	DRIVER_PEQSetMuteTimer();
	DRIVER_PEQReload();
	UNUSED(msgPtr);
	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_ReloadPEQCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	DRIVER_PEQSetMuteTimer();
	DRIVER_PEQReload();
	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_RealTimeSetPEQCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !AUDIO_COMPONENT_IS_ACTIVE (AUDIO_LINE_IN))
	{
		return MEMORY_PUT;
	}	

	switch(msgPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.enable_mode)
	{
		case PEQ_ENABLED_MODE:
			if(PEQ_GetRealtimePEQParameterPtr(PEQ_PARAMETER) == NULL)
			{
				PEQ_AllocateRealtimePEQParameterPtr();
			}
			if(PEQ_GetRealtimePEQParameterPtr(PEQ_PARAMETER) != NULL)
			{
				PEQ_SetRealtimeEnableMode(PEQ_ENABLED_MODE);
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)PEQ_GetRealtimePEQParameterPtr(PEQ_PARAMETER),
					(U8 XDATA_PTR)&msgPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq,
					sizeof(PEQ_CONFIG_SECTOR));
				#ifdef TWS_SETTINGS
				MMI_DRV_SyncRealTimePEQFollowerToRelayer((U8 XDATA_PTR)PEQ_GetRealtimePEQParameterPtr(PEQ_PARAMETER), PEQ_ENABLED_MODE);
				#endif
			}
			else
			{
				return QUE_PUTFRONT;
			}
			break;
		case HPF_ENABLED_MODE:
			if(PEQ_GetRealtimePEQParameterPtr(HPF_PARAMETER) == NULL)
			{
				PEQ_AllocateRealtimePEQParameterPtr();
			}
			if(PEQ_GetRealtimePEQParameterPtr(HPF_PARAMETER) != NULL)
			{
				PEQ_SetRealtimeEnableMode(HPF_ENABLED_MODE);
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)PEQ_GetRealtimePEQParameterPtr(HPF_PARAMETER),
					(U8 XDATA_PTR)&msgPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.hpf,
					sizeof(HPF_CONFIG_SECTOR));
				#ifdef TWS_SETTINGS
				MMI_DRV_SyncRealTimePEQFollowerToRelayer((U8 XDATA_PTR)PEQ_GetRealtimePEQParameterPtr(HPF_PARAMETER), HPF_ENABLED_MODE);
				#endif
			}
			else
			{
				return QUE_PUTFRONT;
			}
			break;
		#ifdef PEQ_EXPANSION_ENABLE
		case PEQ_EXPANSION_ENABLED_MODE:
			if(PEQ_GetRealtimePEQParameterPtr(PEQ_Expansion_PARAMETER) == NULL)
			{
				PEQ_AllocateRealtimePEQParameterPtr();
			}
			if(PEQ_GetRealtimePEQParameterPtr(PEQ_Expansion_PARAMETER) != NULL)
			{
				PEQ_SetRealtimeEnableMode(PEQ_EXPANSION_ENABLED_MODE);
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)PEQ_GetRealtimePEQParameterPtr(PEQ_Expansion_PARAMETER),
					(U8 XDATA_PTR)&msgPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq_expansion,
					sizeof(PEQ_CONFIG_SECTOR));
				#ifdef TWS_SETTINGS
				MMI_DRV_SyncRealTimePEQFollowerToRelayer((U8 XDATA_PTR)PEQ_GetRealtimePEQParameterPtr(PEQ_Expansion_PARAMETER), PEQ_EXPANSION_ENABLED_MODE);
				#endif
			}
			else
			{
				return QUE_PUTFRONT;
			}
			
			break;
		#endif
		#ifdef PEQ_UNDER_100HZ_ENABLE
		case PEQ_UNDER_100HZ_ENABLED_MODE:
			if(PEQ_GetRealtimePEQParameterPtr(PEQ_Under100Hz_PARAMETER) == NULL)
			{
				PEQ_AllocateRealtimePEQParameterPtr();
			}
			if(PEQ_GetRealtimePEQParameterPtr(PEQ_Under100Hz_PARAMETER) != NULL)
			{
				PEQ_SetRealtimeEnableMode(PEQ_UNDER_100HZ_ENABLED_MODE);
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)PEQ_GetRealtimePEQParameterPtr(PEQ_Under100Hz_PARAMETER),
					(U8 XDATA_PTR)&msgPtr->msgBodyPtr.driverCmd.peqRealTimeSetCmd.para.peq_under100Hz, sizeof(PEQ_100HZ_SECTOR));
				#ifdef TWS_SETTINGS
				MMI_DRV_SyncRealTimePEQFollowerToRelayer((U8 XDATA_PTR)PEQ_GetRealtimePEQParameterPtr(PEQ_Under100Hz_PARAMETER), PEQ_UNDER_100HZ_ENABLED_MODE);
				#endif
			}
			else
			{
				return QUE_PUTFRONT;
			}
			break;
		#endif
		case REALTIME_PEQ_HPF_SET_END:
			#ifdef TWS_SETTINGS
			MMI_DRV_SyncRealTimePEQFollowerToRelayer((U8 XDATA_PTR)NULL, REALTIME_PEQ_HPF_SET_END);
			#endif
			if(gAlignDSP.DSP_CHANGE_PEQ != 0)
			{
				return QUE_PUTFRONT;
			}
			PEQ_RealTimeSetPeqHpf();
			break;
	}
	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_SetPEQParameterHandler(DriverMsg XDATA_PTR msgPtr)
{
	PEQ_SetPeqParameter(msgPtr->msgBodyPtr.driverCmd.peqPEQParaSetCmd.peqMode, 
						msgPtr->msgBodyPtr.driverCmd.peqPEQParaSetCmd.peqModeIndex,
						msgPtr->msgBodyPtr.driverCmd.peqPEQParaSetCmd.sectorMode);

	if(MMI_REMEMBER_PEQ_CHANGE_FEAT && !MMI_DEFAULT_PEQ_ASSIGNMENT_FEAT)
	{
		gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_A2DP] =	PEQ_GetModeIndex(PEQ_A2DP);
		gMMI_driver_variation_nvram.misc_para.PEQSectorMode[PEQ_A2DP] = PEQ_GetSectorMode(PEQ_A2DP);
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
					 
	return MEMORY_PUT;
}
#endif

PRIVATE U8 DRIVER_PowerOnCloseADDACmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);

	#ifdef OPERATION_IN_BB_ASIC_VERSION
	AUDIO_ResetADDA();
	#endif

	return MEMORY_PUT;
}

extern PUBLIC void DRIVER_LED_PowerSavingExit(BOOL isResetTimer);
PRIVATE U8 DRIVER_BatteryStatusCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	if( (msgPtr->msgBodyPtr.driverCmd.batStatusCmd.isLowBat && !DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY)) ||
		(!msgPtr->msgBodyPtr.driverCmd.batStatusCmd.isLowBat && DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY)))
	{
		if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
			DRIVER_SetLineInVolumeToHW();
		else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
			DRIVER_SetA2DPVolumeToHW();
		else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO))
			DRIVER_SetNormalVolumeToHW();
	}

	DRIVER_SetMiscCtrlMap(DRIVER_IS_LOW_BATTERY, (msgPtr->msgBodyPtr.driverCmd.batStatusCmd.isLowBat? TRUE:FALSE));
	DRIVER_SetMiscCtrlMap(DRIVER_IS_CHARGER_IN, (msgPtr->msgBodyPtr.driverCmd.batStatusCmd.isCharging? TRUE:FALSE));


	if(msgPtr->msgBodyPtr.driverCmd.batStatusCmd.isCharging)
	{
		DRIVER_LED_PowerSavingExit(FALSE);
	}

	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_VoiceCommandHandler (DriverMsg XDATA_PTR msgPtr)
{
#ifdef SUPPORT_VOICE_COMMAND
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
	{
		AudioTrspr_Stop();
		return QUE_PUTFRONT;
	}

	VoiceCommand_ChooseIdentificationMode();
	if(gVC_ctl.vcIdentificationMode == DEFAULT_VC_IDENTIFY)
	{
		if( VoiceCommand_GetData(msgPtr) && VoiceCommand_Start())
		{
			gDriver_ctl.playingVoiceCmdId = msgPtr->msgBodyPtr.driverCmd.vcCmd.id;
			return MEMORY_PUT;
		}
	}
	else
	{
		if( VoiceCommand_GetData(msgPtr) && VoiceCommand_GetAPDData(msgPtr) && VoiceCommand_Start())
		{
			gDriver_ctl.playingVoiceCmdId = msgPtr->msgBodyPtr.driverCmd.vcCmd.id;
			return MEMORY_PUT;
		}
	}
	return QUE_PUTFRONT;
#else
	DRIVER_SendVoiceCommandEndedEvent((U8)msgPtr->msgBodyPtr.driverCmd.vcCmd.id, MMI_EOF, FALSE);
	return MEMORY_PUT;
#endif

}

PRIVATE U8 DRIVER_SetVoiceCommandVolHandler(DriverMsg XDATA_PTR msgPtr)
{
	gDriver_ctl.voiceCmdMiscCtl.micVols 	= 	msgPtr->msgBodyPtr.driverCmd.vcVolCmd.micVols;
	gDriver_ctl.voiceCmdMiscCtl.analogAdGain_R =  	msgPtr->msgBodyPtr.driverCmd.vcVolCmd.analogAdGain_R;
	gDriver_ctl.voiceCmdMiscCtl.analogAdGain_L =  	msgPtr->msgBodyPtr.driverCmd.vcVolCmd.analogAdGain_L;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
		DRIVER_SetGainAndMute(TARGET_NONE);

	return MEMORY_PUT;
}

PRIVATE U8 DRIVER_StopVoiceCommandHandler(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	#ifdef SUPPORT_VOICE_COMMAND
	return (VoiceCommand_Pause())?MEMORY_PUT:QUE_PUTFRONT;
	#else
	return MEMORY_PUT;
	#endif
}

PRIVATE U8 DRIVER_SetAudioSelHandler(DriverMsg XDATA_PTR msgPtr)
{
	AUDIO_Close_AMP();
	if(MEMORY_PUT == DRIVER_CloseAllAudioHandler1520(msgPtr))
	{
		AUDIO_DisbaleCurrentDigitalInOut();
		if(msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.isResetToDefault)
		{
			AUDIO_SetAudioInOutAsDefault();
		}
		else
		{
			AUDIO_SetAudioInOut(msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.audioInSel, msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.audioOutSel);

			gAUDIO_CtrlInfo.I2SIN_PIO = msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.I2SInPIO;
			gAUDIO_CtrlInfo.AUDIO_I2S_MASTER_GROUP_SEL = msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.audioI2SMasterGroupSel;
			gAUDIO_CtrlInfo.AUDIO_I2S_SLAVE_GROUP_SEL = msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.audioI2SSlaveGroupSel;
			gAUDIO_CtrlInfo.SPDIF_PIO = msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.SPDIFPIO;
			gAUDIO_CtrlInfo.MCLK_INTERNAL = msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.MCLKInternal;
			gAUDIO_CtrlInfo.MCLKDivideNum = msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.MCLKDivideNum;
			gAUDIO_CtrlInfo.MCLK_PHASE_INVERT = msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.MCLKPhaseInvert;
			gAUDIO_CtrlInfo.I2SMCLKPioDisable = (msgPtr->msgBodyPtr.driverCmd.audioSelectCmd.I2SMCLKPioDisable)?TRUE:FALSE;
		}
		AUDIO_EnableDigitalInOut();
		return MEMORY_PUT;
	}
	return QUE_PUTFRONT;
}

PUBLIC U8 DRIVER_SuspendDSPHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_SuspendDSPCmdHandler1520(msgPtr);
}

PUBLIC U8 DRIVER_ChangeSoundEffectHandler(DriverMsg XDATA_PTR msgPtr)
{
	if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !AUDIO_COMPONENT_IS_ACTIVE (AUDIO_LINE_IN))
	{
		return MEMORY_PUT;
	}

	if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP))
	{
		soundEffect_Ctl[SOUND_EFFECT_A2DP].modeSoundEffect =(soundEffect_Ctl[SOUND_EFFECT_A2DP].modeSoundEffect % DSP_SOUNDEFFECT_TOTAL_NO)+1;
		if (soundEffect_Ctl[SOUND_EFFECT_A2DP].modeSoundEffect== DSP_SOUNDEFFECT_TOTAL_NO)
		{
			soundEffect_Ctl[SOUND_EFFECT_A2DP].modeSoundEffect=DSP_SOUNDEFFECT_NONE;
		}
	}
	else if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		soundEffect_Ctl[SOUND_EFFECT_LINE_IN].modeSoundEffect =(soundEffect_Ctl[SOUND_EFFECT_LINE_IN].modeSoundEffect % DSP_SOUNDEFFECT_TOTAL_NO)+1;
		if (soundEffect_Ctl[SOUND_EFFECT_LINE_IN].modeSoundEffect== DSP_SOUNDEFFECT_TOTAL_NO)
		{
			soundEffect_Ctl[SOUND_EFFECT_LINE_IN].modeSoundEffect = DSP_SOUNDEFFECT_NONE;
		}
	}
	return DRIVER_DSPSoundEffect1520(msgPtr);
}

PUBLIC U8 DRIVER_ControlDBBOnOffHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_DSPDBBOnOff1520(msgPtr);
}

PUBLIC U8 DRIVER_GetDSPParaRefGainHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_DSPRefGain1520(msgPtr);
}

#ifdef DEMOSOUND
PUBLIC U8 DRIVER_DemoSoundPlayHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DemoSound_Play(msgPtr);
}

PUBLIC U8 DRIVER_DemoSoundPauseHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DemoSound_Pause(msgPtr);
}

PUBLIC U8 DRIVER_DemoSoundStopHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DemoSound_Stop(msgPtr);
}

PUBLIC U8 DRIVER_DemoSoundForwardHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DemoSound_ChangeSongForward(msgPtr);
}

PUBLIC U8 DRIVER_DemoSoundBackwardHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DemoSound_ChangeSongBackward(msgPtr);
}
#endif

#ifdef SUPPORT_MIC_IN_A2DP_MODE
PUBLIC U8 DRIVER_A2DPAddMicHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_A2DPAddMicHandler1520(msgPtr);
}
#endif

PUBLIC U8 DRIVER_MicTestHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_MicTestHandler1520(msgPtr);
}

PRIVATE U8 DRIVER_SwitchStereoMonoHandler(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	return DRIVER_SwitchStereoMonoHandler1520();
}

PUBLIC U8 DRIVER_SCOModeControlHandler(DriverMsg XDATA_PTR msgPtr)
{
	return DRIVER_SCOModeControlHandler1520(msgPtr);
}

PRIVATE U8 DRIVER_SetVprtVolumeCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	BOOL isDiff = (gDriver_ctl.vprtSoundLevel != msgPtr->msgBodyPtr.driverCmd.vprtVolSetCmd.soundLevel);
	
	if(isDiff)
	{
		gDriver_ctl.vprtSoundLevel = msgPtr->msgBodyPtr.driverCmd.vprtVolSetCmd.soundLevel;
		
		if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT))
			DRIVER_SetVprtVolumeToHW();
	}
	return MEMORY_PUT;
}

PUBLIC BOOL DRIVER_AudioTransparencyCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
		return FALSE;

	if(msgPtr->msgBodyPtr.driverCmd.audioTransparencyCmd.isEnable)
	{
		if(AudioTrspr_Start())
		{
			return TRUE;
		}
	}
	else
	{
		if(AudioTrspr_Stop())
		{
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void DRIVER_SetMiscCtrlMap(U8 mistCtrlType, BOOL enable)
{
	if (enable)
	{
		gDriver_ctl.miscCtrlMap	|= mistCtrlType;
	}
	else
	{
	 	gDriver_ctl.miscCtrlMap	&= (~mistCtrlType);
	}
}

PUBLIC void DRIVER_TurnOffPureOn(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_PURE_ON))
	{
		AUDIO_SetAudioOff (AUDIO_PURE_ON);
	}
}

PUBLIC BOOL DRIVER_IsMiscCtrlTypeEnabled(U8 mistCtrlType)
{
	return (gDriver_ctl.miscCtrlMap	& mistCtrlType) == 0 ? FALSE : TRUE;
}

PUBLIC DriverMsg XDATA_PTR DRIVER_GetGeneralEventMemory(U8 driverMsgType)
{
	DriverMsg XDATA_PTR msgPtr;

	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);

	if(msgPtr)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);

		msgPtr->dataOffset = EVT_OFFSET;
		msgPtr->eventCode = driverMsgType;
	}
	return msgPtr;
}



PUBLIC void DRIVER_SendA2DPEnableEvent(void)
{
	gMMI_Drv_Handler.fDriverDSPA2DPEnabledEvent(gDriver_ctl.dspLink);
	DRIVER_SetA2DPVolumeToHW();
}

PUBLIC void DRIVER_SendA2DPDisableEvent(void)
{
	gMMI_Drv_Handler.fDriverDSPA2DPDisabledEvent(gDriver_ctl.dspLink);
	gDriver_ctl.dspLink = 0xFF;
}

PUBLIC void DRIVER_SendSCOEnableEvent(BOOL isLightMono)
{
	gMMI_Drv_Handler.fDriverDSPSCOEnabledEvent(gDriver_ctl.dspLink, isLightMono);
	DRIVER_SetNormalVolumeToHW();
}

PUBLIC void DRIVER_SendSCODisableEvent(void)
{
	gMMI_Drv_Handler.fDriverDSPSCODisabledEvent(gDriver_ctl.dspLink);
	gDriver_ctl.dspLink = 0xFF;
}

PUBLIC void DRIVER_SendLineInEnableEvent(void)
{
	gMMI_Drv_Handler.fDriverDSPLineInEnabledEvent();
	DRIVER_SetLineInVolumeToHW();
}

PUBLIC void DRIVER_SendLineInDisableEvent(void)
{
	gMMI_Drv_Handler.fDriverDSPLineInDisabledEvent();
}

PUBLIC void DRIVER_SendRequestCloseFMEvent(void)
{
	gMMI_Drv_Handler.fDriverReqCloseFMEvent();
}

PUBLIC void DRIVER_SendAsyncSCOEnabledEvent(void)
{
	gMMI_Drv_Handler.fDriverAsyncSCOEnabledEvent(gDriver_ctl.dspLink);
	DRIVER_SetNormalVolumeToHW();
}

PUBLIC void DRIVER_SendAsyncSCODisabledEvent(void)
{
	gMMI_Drv_Handler.fDriverAsyncSCODisabledEvent(gDriver_ctl.dspLink);
}


PUBLIC BOOL DRIVER_RingtoneCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
#ifdef RINGTONE_ENABLE
	if(Ring_GetData(msgPtr->msgBodyPtr.driverCmd.ringtoneCmd.beepIndex) && Ring_Start((msgPtr->msgBodyPtr.driverCmd.ringtoneCmd.suspendApp)? TRUE : FALSE))
	{
		DRIVER_CheckMuteByMediaID(gDriver_ctl.playingRingtoneId = msgPtr->msgBodyPtr.driverCmd.ringtoneCmd.id);
		LightDbgPrint("Play RT Idx:%X", (U32)msgPtr->msgBodyPtr.driverCmd.ringtoneCmd.id);
		return TRUE;
	}
	return FALSE;
#else
	DRIVER_SendRingtoneEndedEvent(msgPtr->msgBodyPtr.driverCmd.ringtoneCmd.id);
	return TRUE;
#endif
}


#ifdef SUPPORT_VOICE_PROMPT
PRIVATE void DRIVER_SendVoicePromptEndedEvent (U16 id)
{
	if(id != NO_MEDIA)
		gMMI_Drv_Handler.fDriverVoicePromptEndEvent(id);
}

PUBLIC void DRIVER_VoicePromptOffCallback(void)
{
	DRIVER_SendVoicePromptEndedEvent(gDriver_ctl.playingRingtoneId);
	gDriver_ctl.playingRingtoneId = NO_MEDIA;
}

PUBLIC BOOL DRIVER_VoicePromptCmdHandler(DriverMsg XDATA_PTR msgPtr)
{
	if(!AUDIO_IsAllowVoicePrompt())
		return TRUE;
	if(msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.isOverSCO && !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
		return TRUE;

	switch(VoicePrompt_GetData(msgPtr))
	{
		case VP_GET_DATA_OK:
			if(VoicePrompt_Start(msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.suspendApp ? TRUE : FALSE, msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.isOverSCO ? TRUE : FALSE))
			{
				DRIVER_CheckMuteByMediaID(gDriver_ctl.playingRingtoneId = msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id);
				LightDbgPrint("Play VP Idx:%X", (U32)msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id);
				return TRUE;
			}
			break;

		case VP_GET_DATA_BUSY:
			return FALSE;

		case VP_GET_DATA_FAIL:
			gDriver_ctl.playingRingtoneId = msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id;
			DRIVER_VoicePromptOffCallback();
		default:
			return TRUE;
	}
	return FALSE;
}
#endif

PUBLIC void DRIVER_ATSwitchVPRTSCO(void)
{
	gDriver_ctl.isATSwitch = AT_IN_SWITCH;
	AudioTrspr_Stop();
}

extern PUBLIC BOOL MMI_CheckLinkInCallRelatedState(U8 link);
PUBLIC BOOL DRIVER_CheckATEnablePlay(void)
{
    #ifdef SUPPORT_MIC_IN_A2DP_MODE
	if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) && !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT) && !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND) &&
		!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO) && MMI_GetAudioTransparencyEnable() && !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_ASYNC_SCO) &&
		gDriver_ctl.isATSwitch == AT_IN_SWITCH && !MMI_CheckLinkInCallRelatedState(0) && !MMI_CheckLinkInCallRelatedState(1))
	{
		if(gDriver_ctl.checkATSwitchDelayTimer == (OST XDATA_PTR)NULL)
		{
			SYS_SetTimer(&gDriver_ctl.checkATSwitchDelayTimer, (U32) 320L);
		}
		return TRUE;
	}
    #endif
	return FALSE;
}

PUBLIC void DRIVER_ClearAudioCmd(U8 opCode, U16 id)
{
	U8 j, deleteSame;
	DriverMsg XDATA_PTR msgPtr;
	j = OSMQ_Entries(OSMQ_AUDIO_Prompt_Command_ptr);
	deleteSame = 0;
	while(j--)
	{
		msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_AUDIO_Prompt_Command_ptr);
		switch(opCode)
		{
			case DRIVER_INTERNAL_CLEAN_ALL_AUDIO_CMD:
				if(msgPtr->msgOpcode == DRIVER_FAKE_MEDIA_VP_RINGTONE_CMD)
					OSMQ_Put(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
				else
				{
					OSMEM_Put((U8 XDATA_PTR)msgPtr);
				}
				break;
			case DRIVER_VOICEPROMPT_CMD:
				if(msgPtr->msgOpcode == DRIVER_VOICEPROMPT_CMD)
				{
					if(	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id == id || VOICE_PROMPT_ALL_CLEAN == id ||
						(MEDIA_EVT_ENTER_LINE_IN_MODE == id   && msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id == MEDIA_EVT_EXIT_LINE_IN_MODE && deleteSame) ||
						(MEDIA_EVT_ENTER_DISCOVERABLE == id  && msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id == MEDIA_EVT_EXIT_DISCOVERABLE&& deleteSame))
					{
						deleteSame = 1;
						OSMEM_Put((U8 XDATA_PTR)msgPtr);
						continue;
					}
				}
				OSMQ_Put(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
				break;
			case DRIVER_RINGTONE_CMD:
				if(msgPtr->msgOpcode == DRIVER_RINGTONE_CMD)
				{
					if(	msgPtr->msgBodyPtr.driverCmd.ringtoneCmd.id == id || VOICE_PROMPT_ALL_CLEAN == id ||
						(id == MEDIA_EVT_ENTER_LINE_IN_MODE && msgPtr->msgBodyPtr.driverCmd.ringtoneCmd.id == MEDIA_EVT_EXIT_LINE_IN_MODE && deleteSame )||
						(id == MEDIA_EVT_ENTER_DISCOVERABLE && msgPtr->msgBodyPtr.driverCmd.ringtoneCmd.id == MEDIA_EVT_EXIT_DISCOVERABLE && deleteSame))
					{
						deleteSame = 1;
						OSMEM_Put((U8 XDATA_PTR)msgPtr);
						continue;
					}
				}
				OSMQ_Put(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
				break;
			default:
				OSMQ_Put(OSMQ_AUDIO_Prompt_Command_ptr, (U8 XDATA_PTR)msgPtr);
				break;
		}
	}
}

PRIVATE void DRIVER_SoundLevelMuteUnmute(U8 soundLevel, U8 smallStep)
{
	if(soundLevel || smallStep)
	{
		gAUDIO_CtrlInfo.isMuteBySoundLevel = FALSE;
	}
	else
	{
		gAUDIO_CtrlInfo.isMuteBySoundLevel = TRUE;
	}
}

PRIVATE void DRIVER_MicUnmute(void)
{
	LightDbgPrint("MIC Unmute");
	AUDIO_SwitchMute (ADC, FALSE);

	//Disable Mute Reminder
	SYS_ReleaseTimer(&gDriver_ctl.muteReminderTimerPtr);
}

PRIVATE void DRIVER_MicMute(void)
{
	LightDbgPrint("MIC Mute");
	AUDIO_SwitchMute (ADC, TRUE);

	//Enable Mute Reminder
	if(gMMI_nvram.timerCtl.MuteReminderTime)
		SYS_SetTimer(&gDriver_ctl.muteReminderTimerPtr, (U32)gMMI_nvram.timerCtl.MuteReminderTime * ONE_SEC);
}

PUBLIC void DRIVER_SetVprtVolumeToHW(void)
{
	DRIVER_SetGainAndMute(TARGET_VPRTAT);
}

PUBLIC void DRIVER_SetLineInVolumeToHW(void)
{

	DRIVER_SoundLevelMuteUnmute(gDriver_ctl.lineInSpkMicCtl.soundLevel, gDriver_ctl.lineInSpkMicCtl.smallStepLevel);

	if(  gAUDIO_CtrlInfo.isLineInMute || gAUDIO_CtrlInfo.isMuteForPEQ ||
		(gAUDIO_CtrlInfo.isMuteBySoundLevel && MMI_MUTE_LINE_IN_WHEN_VOLUME_IS_ZERO_FEAT) )
	{
		gAlignDSP.DSPOUT_MUTE = 1;
		LightDbgPrint("3DSPOUT_MUTE = 1");
	}

	DRIVER_MicUnmute();

	gDriver_ctl.DSPGainLv = (U16)gDriver_ctl.lineInSpkMicCtl.spkVols;

	if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY) && (gDriver_ctl.lineInSpkMicCtl.soundLevel > gMMI_nvram.lineinDecreaseGainCtl.baseSoundLevel))
		gDriver_ctl.DSPGainLv -= (U16)gMMI_nvram.lineinDecreaseGainCtl.decreaseGain;

	DRIVER_SetGainAndMute(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY) ? TARGET_VPRTAT : TARGET_APP);
}


PUBLIC void DRIVER_SetA2DPVolumeToHW(void)
{

	DRIVER_SoundLevelMuteUnmute(gDriver_ctl.a2dpSpkCtl.soundLevel, gDriver_ctl.a2dpSpkCtl.smallStepLevel);

	if(	 gAUDIO_CtrlInfo.isMuteForPEQ ||
		(gAUDIO_CtrlInfo.isMuteBySoundLevel && MMI_MUTE_A2DP_WHEN_VOLUME_IS_ZERO_FEAT) ||
		MMI_DRV_IsSpeakerMuteByLink(gDriver_ctl.dspLink))
	{
		gAlignDSP.DSPOUT_MUTE = 1;
		LightDbgPrint("2DSPOUT_MUTE = 1");
	}

	DRIVER_MicUnmute();

	gDriver_ctl.DSPGainLv = (U16)gDriver_ctl.a2dpSpkCtl.spkVols;

	if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY) && (gDriver_ctl.a2dpSpkCtl.soundLevel > gMMI_nvram.a2dpDecreaseGainCtl.baseSoundLevel))
		gDriver_ctl.DSPGainLv -= (U16)gMMI_nvram.a2dpDecreaseGainCtl.decreaseGain;

	DRIVER_SetGainAndMute(TARGET_APP);
}

extern PUBLIC BOOL MMI_DRV_IsMicMute(U8 linkIndex);
PUBLIC void DRIVER_SetNormalVolumeToHW(void)
{
	if(!DSP_IsDSPEnabled())
		return;

	DRIVER_SoundLevelMuteUnmute(gDriver_ctl.normalSpkMicCtl.soundLevel, gDriver_ctl.normalSpkMicCtl.smallStepLevel);


	if(	MMI_DRV_IsSpeakerMuteByLink(gDriver_ctl.dspLink)|| (gAUDIO_CtrlInfo.isMuteBySoundLevel && MMI_MUTE_SPEAKER_WHEN_VOLUME_IS_ZERO_FEAT) )
	{
		gAlignDSP.DSPOUT_MUTE = 1;
		LightDbgPrint("DSPOUT_MUTE = 1");
	}

	if(MMI_DRV_IsMicMute(gDriver_ctl.dspLink) && !DRIVER_IsMicMuted())
	{
		DRIVER_MicMute();
	}

	if(!MMI_DRV_IsMicMute(gDriver_ctl.dspLink) && DRIVER_IsMicMuted())
	{
		DRIVER_MicUnmute();
	}


	gDriver_ctl.DSPGainLv = (U16)gDriver_ctl.normalSpkMicCtl.spkVols;

	if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY) && (gDriver_ctl.normalSpkMicCtl.soundLevel > gMMI_nvram.scoDecreaseGainCtl.baseSoundLevel))
		gDriver_ctl.DSPGainLv  -=  (U16)gMMI_nvram.scoDecreaseGainCtl.decreaseGain;

	DRIVER_SetGainAndMute(TARGET_APP);
}

PUBLIC void DRIVER_SetLineInMuteUnMute(BOOL isMute)
{
	gAUDIO_CtrlInfo.isLineInMute = isMute;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		DRIVER_SetLineInVolumeToHW();
	}
}

PUBLIC BOOL DRIVER_IsLineInMute(void)
{
	return gAUDIO_CtrlInfo.isLineInMute;
}


PUBLIC BOOL DRIVER_IsMicMuted(void)
{
	return gAUDIO_CtrlInfo.isADMute;
}


PUBLIC BOOL DRIVER_IsRingToneOrVPOrATPlaying(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT)
		|| AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY) /*|| AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND)*/
	)
		return TRUE;

	return FALSE;
}

PUBLIC DriverMsg XDATA_PTR DRIVER_GetCmdMsg(U8 opCode)
{
	DriverMsg XDATA_PTR msgPtr;

	if((msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1)) != (DriverMsg XDATA_PTR)NULL)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		msgPtr->msgOpcode = opCode;
		return msgPtr;
	}
	return (DriverMsg XDATA_PTR)NULL;
}

PUBLIC void DRIVER_SendMsgToDriverQue(U8 opCode)
{
	DriverMsg XDATA_PTR msgPtr;

	if((msgPtr = DRIVER_GetCmdMsg(opCode)) != (DriverMsg XDATA_PTR)NULL)
	{
		if(opCode == DRIVER_STOP_VOICE_COMMAND_CMD)
			OSMQ_PutFront(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		else
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void DRIVER_SendRingtoneEndedEvent(U16 id)
{
	if(id != NO_MEDIA)
		gMMI_Drv_Handler.fDriverRingtoneEndEvent(id);
}

PUBLIC void DRIVER_SendVoiceCommandEndedEvent(U8 id, U8 matchedNo, U8 stoppedByUser)
{
	#ifdef SUPPORT_VOICE_COMMAND
	if(id != NO_MEDIA )
		gMMI_Drv_Handler.fDriverVoiceCmdEndEvent(id, matchedNo, stoppedByUser);
	#else
	UNUSED(id);
	UNUSED(matchedNo);
	UNUSED(stoppedByUser);
	#endif
}

#ifdef RINGTONE_ENABLE
PUBLIC void DRIVER_RingtoneOffCallback(void)
{
	DRIVER_SendRingtoneEndedEvent(gDriver_ctl.playingRingtoneId);
	gDriver_ctl.playingRingtoneId = NO_MEDIA;
}
#endif

#ifdef SUPPORT_VOICE_COMMAND
PUBLIC void DRIVER_VoiceCommandOffCallback(void)
{
	U8 matchedNo = VC_INVALID_EVT;

	if(gVC_ctl.result != VC_STOP_BY_USER)
	{
		if(gVC_ctl.vcIdentificationMode == APD_VC_IDENTIFY)
		{
			//matchedNo = VoiceCommand_GetEvtIndex(gVC_ctl.result);
			matchedNo = gVC_ctl.result;
		}
		else
		{
			if(gVC_ctl.result < TOTAL_VOICE_COMMAND_PATTERNS)
			{
				/*
				matchedNo = VoiceCommand_GetEvtIndex(gVC_ctl.result);
				gVC_ctl.vcnextLayer = VoiceCommand_VCLayer(matchedNo);
				*/
				matchedNo = gVC_ctl.result;
				gVC_ctl.vcnextLayer = VoiceCommand_VCLayer(matchedNo);
			}
		}
	}

	if(gVC_ctl.vcIdentificationMode != APD_VC_IDENTIFY)
	{
		if(matchedNo == VC_INVALID_EVT || gVC_ctl.result == VC_STOP_BY_USER)
		{
			gVC_ctl.vcnextLayer = VC_MULTI_LAYER_SET_0;
		}
	}

	DRIVER_SendVoiceCommandEndedEvent(gDriver_ctl.playingVoiceCmdId, (matchedNo == VC_INVALID_EVT)?MMI_EOF:matchedNo, (gVC_ctl.result == VC_STOP_BY_USER)?TRUE:FALSE);
	gDriver_ctl.playingVoiceCmdId = NO_MEDIA;
	DRIVER_CheckATEnablePlay();
}
#endif


PUBLIC DRIVER_MMI_CMD_HANDLER_TYPE CODE DriverMMICmdHandler[MMI_TO_DRIVER_TOTAL_CMD] =
{
	DRIVER_MMIReadyCmdHandler, //DRIVER_MMI_READY_CMD
	DRIVER_UpdateGeneralParaCmdHandler, //DRIVER_UPDATE_GENERAL_PARA_CMD
	DRIVER_SetNormalVolumeCmdHandler, //DRIVER_VOLUME_SET_CMD
	DRIVER_SetA2DPVolumeCmdHandler, //DRIVER_A2DP_VOLUME_SET_CMD
	DRIVER_SetLineInVolumeCmdHandler, //DRIVER_LINE_IN_VOLUME_SET_CMD
	DRIVER_RingToneVPCmdHandler, //DRIVER_RINGTONE_CMD
	DRIVER_RingToneVPCmdHandler, //DRIVER_VOICEPROMPT_CMD
	DRIVER_StopSpecificRingCmdHandler, //DRIVER_STOP_SPECIFIC_RINGTONE_CMD
	DRIVER_StopSpecificVPCmdHandler, //DRIVER_STOP_SPECIFIC_VOICEPROMPT_CMD
	DRIVER_StopRingCmdHandler, //DRIVER_STOP_RINGTONE_CMD
	DRIVER_StopVPCmdHandler, //DRIVER_STOP_VP_CMD
	DRIVER_FakeMediaVPRingCmdHandler, //DRIVER_FAKE_MEDIA_VP_RINGTONE_CMD
	DRIVER_SCOCmdHandler, //DRIVER_SCO_CMD
	DRIVER_A2DPCmdHandler, //DRIVER_A2DP_CMD
	DRIVER_LineInCmdHandler, //DRIVER_LINE_IN_CMD
	DRIVER_StopLineInCmdHandler, //DRIVER_STOP_LINE_IN_CMD
	DRIVER_StopDSPCmdHandler, //DRIVER_STOP_DSP_CMD
	DRIVER_StopDSPPowerOffCmdHandler, //DRIVER_STOP_DSP_POWER_OFF_CMD
	#ifdef PEQ_ENABLE
	DRIVER_PEQChangeCmdHandler, //DRIVER_CHANGE_PEQ_CMD
	DRIVER_ReloadPEQCmdHandler, //DRIVER_PEQ_RELOAD_CMD
	DRIVER_RealTimeSetPEQCmdHandler,  //DRIVER_REALTIME_SET_PEQ_HPF_CMD
	DRIVER_SetPEQParameterHandler,	//DRIVER_SET_PEQ_PARA_CMD
	#endif
	DRIVER_PowerOnCloseADDACmdHandler, //DRIVER_POWER_ON_CLOSE_ADDA_CMD
	DRIVER_BatteryStatusCmdHandler, //DRIVER_BATTERY_STATUS_CMD
	DRIVER_VoiceCommandHandler, //DRIVER_VOICE_COMMAND_CMD
	DRIVER_SetVoiceCommandVolHandler, //DRIVER_SET_VOICE_COMMAND_VOL_CMD
	DRIVER_StopVoiceCommandHandler, //DRIVER_STOP_VOICE_COMMAND_CMD
	DRIVER_SetAudioSelHandler, //DRIVER_SET_AUDIO_SEL_CMD
	DRIVER_SuspendDSPHandler, //DRIVER_SUSPEND_DSP_CMD
	DRIVER_ChangeSoundEffectHandler,   //DRIVER_CHANGE_SOUND_EFFECT_CMD
	DRIVER_ControlDBBOnOffHandler,       //DRIVER_CONTROL_DBB_ON_OFF
	DRIVER_GetDSPParaRefGainHandler, //DRIVER_GET_DSP_REF_GAIN_CMD
	#ifdef DEMOSOUND
	DRIVER_DemoSoundPlayHandler,	//DRIVER_DEMOSOUND_PLAY_CMD,
	DRIVER_DemoSoundPauseHandler,	//DRIVER_DEMOSOUND_PAUSE_CMD,
	DRIVER_DemoSoundStopHandler,	//DRIVER_DEMOSOUND_STOP_CMD,
	DRIVER_DemoSoundForwardHandler,	//DRIVER_DEMOSOUND_FORWARD_CMD,
	DRIVER_DemoSoundBackwardHandler,//DRIVER_DEMOSOUND_BACKWARD_CMD,
	#endif
	#ifdef SUPPORT_MIC_IN_A2DP_MODE
	DRIVER_A2DPAddMicHandler,//DRIVER_A2DP_ADD_MIC_CMD,
	#endif
	DRIVER_MicTestHandler, //DRIVER_MIC_TEST_CMD,
	DRIVER_SwitchStereoMonoHandler, //DRIVER_DSP_SWITCH_STEREO_MONO_CMD,
	DRIVER_SCOModeControlHandler, //DRIVER_SCO_MODE_CONTROL_CMD,
	DRIVER_SetVprtVolumeCmdHandler,//DRIVER_VPRT_VOLUME_SET_CMD
	DRIVER_AudioTransparencyPushCmdHandler, //DRIVER_AUDIO_TRANSPARENCY_CMD,
	DRIVER_AsyncSCOCmdHandler1520, //DRIVER_ASYNC_SCO_CMD
};


PUBLIC void DRIVER_ProcMmiCmd(void)
{
	U8 cmdHandler;
	DriverMsg XDATA_PTR msgPtr;

	if(gDriver_ctl.isAudioHandlingPending || gDriver_ctl.suspendDSPReason)
		return;

	while ((msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get (OSMQ_DRIVER_Command_ptr)) != (DriverMsg XDATA_PTR)NULL)
	{

		cmdHandler = MEMORY_PUT;
		if(msgPtr->msgOpcode < MMI_TO_DRIVER_TOTAL_CMD)
		{
			cmdHandler = DriverMMICmdHandler[msgPtr->msgOpcode].MMICmdHandler(msgPtr);
		}

		if(cmdHandler == QUE_PUTFRONT)
		{
			OSMQ_PutFront(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
			return;
		}
		else if(cmdHandler == CONTINUE_LOOP)
			continue;
		else if(cmdHandler == RETURN_FUNCTION)
			return;
		else if(cmdHandler == QUE_PUT)
		{
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
			return;
		}
		else
			OSMEM_Put ((U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void DRIVER_SetPowerSaving(BOOL isPowerSaving, U8 device)
{
	if(!isPowerSaving)
		gDriver_ctl.powerSavingMask |= device;
	else
		gDriver_ctl.powerSavingMask &= (~device);

	SetPowerSavingForDevice((gDriver_ctl.powerSavingMask)?FALSE:TRUE, PWRSAV_DRIVER);
}

PUBLIC void DRIVER_SetBusyMask(BOOL isBusy, U8 mask)
{
	if(isBusy)
		gDriver_ctl.busyMask |= mask;
	else
		gDriver_ctl.busyMask &= ~mask;

	if(gDriver_ctl.busyMask)
		gDriver_ctl.isReadyToDSPwOff &= ~DRIVER_READY;
	else
		gDriver_ctl.isReadyToDSPwOff |= DRIVER_READY;
}

PUBLIC void DRIVER_SendStopVoiceCommandCmd(void)
{
	U8 i, cnt;
	DriverMsg XDATA_PTR msgPtr;

	cnt = 0;
	i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
	while(i--)
	{
		msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
		if(msgPtr->msgOpcode == DRIVER_VOICE_COMMAND_CMD)
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		else if(msgPtr->msgOpcode == DRIVER_STOP_VOICE_COMMAND_CMD)
		{
			if(!cnt)
				OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
			else
				OSMEM_Put((U8 XDATA_PTR)msgPtr);
			cnt++;
		}
		else
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND) && !cnt)
	{
		DRIVER_SendMsgToDriverQue(DRIVER_STOP_VOICE_COMMAND_CMD);
	}
}

PUBLIC void DRIVER_SendStopSpecificRTCmd(U16 id)
{
	#ifdef RINGTONE_ENABLE
	U8 i, cnt;
	#endif
	DRIVER_STOPSPECIFIC_RING:
	DRIVER_ClearAudioCmd(DRIVER_RINGTONE_CMD, id);
	#ifdef RINGTONE_ENABLE
	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_RINGTONE) && gDriver_ctl.playingRingtoneId == id)
	{
		DriverMsg XDATA_PTR msgPtr;

		cnt = 0;
		i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
		while(i--)
		{
			msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
			if(msgPtr->msgOpcode == DRIVER_STOP_SPECIFIC_RINGTONE_CMD && msgPtr->msgBodyPtr.driverCmd.stopSpecificRTCmd.id == id)
			{
				if(!cnt)
					OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
				else
					OSMEM_Put((U8 XDATA_PTR)msgPtr);
				cnt++;
			}
			else
				OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		}

		if(!cnt && (msgPtr = DRIVER_GetCmdMsg(DRIVER_STOP_SPECIFIC_RINGTONE_CMD)) != (DriverMsg XDATA_PTR)NULL)
		{
			msgPtr->msgBodyPtr.driverCmd.stopSpecificRTCmd.id = id;
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		}
	}
	#endif
	if(id == MEDIA_EVT_RINGTONE)
	{
		id = MEDIA_EVT_RINGTONE_2;
		goto DRIVER_STOPSPECIFIC_RING;
	}
	else if(id == MEDIA_EVT_RINGTONE_2)
	{
		id = MEDIA_EVT_RINGTONE_IN_CALL;
		goto DRIVER_STOPSPECIFIC_RING;
	}
	else if(id == MEDIA_EVT_RINGTONE_IN_CALL)
	{
		id = MEDIA_EVT_RINGTONE2_IN_CALL;
		goto DRIVER_STOPSPECIFIC_RING;
	}
}

PUBLIC void DRIVER_SendStopSpecificVPCmd(U16 id)
{
	#ifdef SUPPORT_VOICE_PROMPT
	U8 i, cnt;
	#endif

	DRIVER_STOP_SPECIFIC_VP:
	DRIVER_ClearAudioCmd(DRIVER_VOICEPROMPT_CMD, id);
	#ifdef SUPPORT_VOICE_PROMPT
	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_VOICEPROMPT) && gDriver_ctl.playingRingtoneId == id)
	{
		DriverMsg XDATA_PTR msgPtr;

		cnt = 0;
		i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
		while(i--)
		{
			msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
			if(msgPtr->msgOpcode == DRIVER_STOP_SPECIFIC_VOICEPROMPT_CMD && msgPtr->msgBodyPtr.driverCmd.stopSpecificVPCmd.id == id)
			{
				if(!cnt)
					OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
				else
					OSMEM_Put((U8 XDATA_PTR)msgPtr);
				cnt++;
			}
			else
				OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		}

		if(!cnt && (msgPtr = DRIVER_GetCmdMsg(DRIVER_STOP_SPECIFIC_VOICEPROMPT_CMD)) != (DriverMsg XDATA_PTR)NULL)
		{
			msgPtr->msgBodyPtr.driverCmd.stopSpecificVPCmd.id = id;
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		}
	}
	#endif
	if(id == MEDIA_EVT_RINGTONE)
	{
		id = MEDIA_EVT_RINGTONE_2;
		goto DRIVER_STOP_SPECIFIC_VP;
	}
	else if(id == MEDIA_EVT_RINGTONE_2)
	{
		id = MEDIA_EVT_RINGTONE_IN_CALL;
		goto DRIVER_STOP_SPECIFIC_VP;
	}
	else if(id == MEDIA_EVT_RINGTONE_IN_CALL)
	{
		id = MEDIA_EVT_RINGTONE2_IN_CALL;
		goto DRIVER_STOP_SPECIFIC_VP;
	}
}

PUBLIC void DRIVER_ClearStopDSPCmd(U8 linkIndex)
{
	U8 i;
	DriverMsg XDATA_PTR msgPtr;

	i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
	while(i--)
	{
		msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
		if(msgPtr->msgOpcode == DRIVER_STOP_DSP_CMD && msgPtr->msgBodyPtr.driverCmd.stopDSPCmd.linkInd == linkIndex)
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		else
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void DRIVER_ClearEnableSCODSPCmd(U8 linkIndex)
{
	U8 i;
	DriverMsg XDATA_PTR msgPtr;

	i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
	while(i--)
	{
		msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
		if(msgPtr->msgOpcode == DRIVER_SCO_CMD && msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd == linkIndex)
		{
			if(MMI_DRV_GetAudioSwitchState() == MMI_AUDIO_SWITCH_WAITING_HCI_EVT)
				MMI_DRV_SetAudioSwitchState(MMI_AUDIO_SWITCH_ILDE);
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		}
		else
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}
	if(gDriver_ctl.backUpAudioCmdPtr)
	{
		if(gDriver_ctl.backUpAudioCmdPtr->msgOpcode == DRIVER_SCO_CMD && gDriver_ctl.backUpAudioCmdPtr->msgBodyPtr.driverCmd.scoCmd.linkInd == linkIndex)
		{
			DRIVER_ReleaseBackUpAppCmd();
		}
	}
}

PUBLIC void DRIVER_ClearEnableA2DPDSPCmd(U8 linkIndex)
{
	U8 i;
	DriverMsg XDATA_PTR msgPtr;

	i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
	while(i--)
	{
		msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
		if(msgPtr->msgOpcode == DRIVER_A2DP_CMD && msgPtr->msgBodyPtr.driverCmd.a2dpCmd.linkInd == linkIndex)
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		else
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}

	if(gDriver_ctl.backUpAudioCmdPtr)
	{
		if(gDriver_ctl.backUpAudioCmdPtr->msgOpcode == DRIVER_A2DP_CMD && gDriver_ctl.backUpAudioCmdPtr->msgBodyPtr.driverCmd.a2dpCmd.linkInd == linkIndex)
		{
			DRIVER_ReleaseBackUpAppCmd();
		}
	}
}

PUBLIC void DRIVER_LoadLanguageCtlFromROM(U8 LangIndex)
{
	#ifdef SUPPORT_VOICE_PROMPT
		U8 CODE_PTR vp_sector_base_addr = GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_LANG_CTL);
		OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&gDriver_ctl.voiceLangCtl, vp_sector_base_addr + (sizeof(VoiceLangCtl) * LangIndex), sizeof(VoiceLangCtl));

		if(gDriver_ctl.voiceLangCtl.vpScriptOffsetTbl.vp_ptr && gDriver_ctl.voiceLangCtl.vpScriptDataTbl.vp_ptr && gDriver_ctl.voiceLangCtl.vpDataOffsetTbl.vp_ptr)
		{
			gDriver_ctl.voiceLangCtl.vpScriptOffsetTbl.vp_ptr += (U32)vp_sector_base_addr;
			gDriver_ctl.voiceLangCtl.vpScriptDataTbl.vp_ptr += (U32)vp_sector_base_addr;
			gDriver_ctl.voiceLangCtl.vpDataOffsetTbl.vp_ptr += (U32)vp_sector_base_addr;
			gDriver_ctl.voiceLangCtl.vpScriptOffsetEntries--;	//decrease to get actual voice prompt event count
			#if (defined VOICE_LANG_CHI && defined VOICE_LANG_ENG)
			LightDbgPrint("Language Change into:%d",(U8) LangIndex);
			#else
				#if (defined VOICE_LANG_CHI)
				LightDbgPrint("Language Change into:0");
				#endif
				#if (defined VOICE_LANG_ENG)
				LightDbgPrint("Language Change into:1");
				#endif
				#if (!defined VOICE_LANG_CHI || !defined VOICE_LANG_ENG)
				LightDbgPrint("Not Language can be changed");
				#endif
			#endif
		}
		else
		{
			gDriver_ctl.voiceLangCtl.vpScriptOffsetTbl.vp_ptr = (U32)0;
			gDriver_ctl.voiceLangCtl.vpScriptDataTbl.vp_ptr = (U32)0;
			gDriver_ctl.voiceLangCtl.vpDataOffsetTbl.vp_ptr = (U32)0;
			gDriver_ctl.voiceLangCtl.vpScriptOffsetEntries = VOICE_PROMPT_INVALID;
			LightDbgPrint("Language Change fail");
		}
	#else
		LightDbgPrint("Not support voice prompt");
		UNUSED(LangIndex);
	#endif

	#ifdef SUPPORT_VOICE_COMMAND
		gVC_ctl.dataBasePtr = (U32) GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA);//base
		gVC_ctl.dataBasePtr += (U32) ((DRIVER_VOICE_COMMAND_DATA_TYPE CODE_PTR)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA))->vcAddrTable[LangIndex].vcDataBasePtr;//+offset
		gVC_ctl.dataMapPtr = (U32) GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA);//base
		gVC_ctl.dataMapPtr += (U32) ((DRIVER_VOICE_COMMAND_DATA_TYPE CODE_PTR)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA))->vcAddrTable[LangIndex].vcEvtDataMapPtr;//+offset
	#else
		UNUSED(LangIndex);
	#endif
}

PUBLIC U16 DRIVER_CurrentPlayingMediaID(void)
{
	return gDriver_ctl.playingRingtoneId;
}


PUBLIC void DRIVER_Cmd_SuspendDSP(U8 reason, VFUN callback)
{
	DriverMsg XDATA_PTR msgPtr;

	if((msgPtr = DRIVER_GetCmdMsg(DRIVER_SUSPEND_DSP_CMD)) != (DriverMsg XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.driverCmd.suspendDspCmd.reason = reason;
		msgPtr->msgBodyPtr.driverCmd.suspendDspCmd.callback = callback;
		OSMQ_PutFront(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void DRIVER_Cmd_ClearSuspendDSPReason(U8 reason)
{
	gDriver_ctl.suspendDSPReason &= (~reason);
}

PUBLIC BOOL DRIVER_Query_IsDSPSuspending(void)
{
	return (gDriver_ctl.suspendDSPReason)? TRUE:FALSE;
}

PUBLIC BOOL DRIVER_Query_IsDSPSuspendingReason(U8 reason)
{
	LightDbgPrint("Query_IsDSP: %d, %d", (U8)gDriver_ctl.suspendDSPReason, (U8)reason);
	return (gDriver_ctl.suspendDSPReason&reason)? TRUE:FALSE;
}
PUBLIC BOOL DRIVER_Query_IsWaitingDSPSuspend(U8 reason)
{
	DriverMsg XDATA_PTR msgPtr;
	BOOL isWaiting = FALSE;
	U8 i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
	
	while(i--)
	{
		msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
		
		if(msgPtr->msgOpcode == DRIVER_SUSPEND_DSP_CMD && msgPtr->msgBodyPtr.driverCmd.suspendDspCmd.reason == reason)
			isWaiting = TRUE;
		
		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}
	return isWaiting;
}

PUBLIC BOOL DRIVER_IsDemoSoundReady(void)
{
#ifdef DEMOSOUND
	return DemoSound_IsAvailable();
#else
	return FALSE;
#endif
}

PUBLIC BOOL DRIVER_CheckAudioTransparencyReady(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
		return FALSE;
	return TRUE;
}

PUBLIC U8 DRIVER_CheckAudioComponetEnable(U8 componet)
{
	return AUDIO_COMPONENT_IS_ACTIVE(componet);
}

PUBLIC U8 DRIVER_GetVoicepromptsNumberInQueue(void)
{
	return OSMQ_Entries(OSMQ_AUDIO_Prompt_Command_ptr);
}