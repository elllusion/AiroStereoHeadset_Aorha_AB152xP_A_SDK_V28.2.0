//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _MMI_DRIVER_H_
	#define _MMI_DRIVER_H_

	#ifdef _MMI_DRIVER_C_
		#define DRV_EXTERN
	#else
		#define DRV_EXTERN extern
	#endif

	#include "MMI_Base.h"
	
//#define FT_AUDIO_BIST
//#define AB1122
//#define AB1526  /*skip line-in audio test*/

	DRV_EXTERN struct
	{
		U8 isMiscEnabled;
	} XDATA gMMI_DRV_ctl;

	enum soundbar_autx_to_esco_method
	{
		HW_DOWNSAMPLE = 0,
		SW_DOWNSAMPLE,
	};

	enum soundbar_audio_channel_sel
	{
		SOUNDBAR_ESCO_DISABLE       = 0x0000,
		SOUNDBAR_LEFT_CHANNEL       = 0x0001,
		SOUNDBAR_RIGHT_CHANNEL      = 0x0002,
		SOUNDBAR_MIX_TWO_CHANNEL    = 0x0003,
	};

	enum mmi_audio_bandwidth
	{
		MMI_AUDIO_NARROW_BAND,
		MMI_AUDIO_WIDE_BAND
	};

	enum mmi_audio_component
	{
		MMI_AUDIO_COMPONENT_SCO,
		MMI_AUDIO_COMPONENT_LINE_IN,
		MMI_AUDIO_COMPONENT_A2DP
	};

	typedef struct
	{
		U8 keyEventStartIndex;
		U8 keyEventNo;
	}MMI_KEY_EVT_MAP_TYPE;

	typedef struct
	{
		U16 keyEventStartIndex;
		U16 keyEventNo;
	}MMI_KEY_EVT_MAP_TYPE2;

	typedef struct
	{
		void (*fDriverKeyEvent)(KeyIndex keyIndex, U8 keyAction);
		void (*fDriverMuteReminderEvent)(void);
		void (*fDriverReadyPowerOffEvent)(void);
		void (*fDriverFGLedEndEvent)(U16 id);
		void (*fDriverRingtoneEndEvent)(U16 id);
	#ifdef SUPPORT_VOICE_PROMPT
		void (*fDriverVoicePromptEndEvent)(U16 id);
	#endif
	#ifdef SUPPORT_VOICE_COMMAND
		void (*fDriverVoiceCmdEndEvent)(U8 id, U8 matchedNo, U8 stoppedByUser);
	#endif
		void (*fDriverFakeMediaEvent)(MMIMsgType XDATA_PTR msgPtr);
		void (*fDriverA2DPStartAnchorEvent)(void);
		void (*fDriverDSPA2DPEnabledEvent)(U8 linkIndex);
		void (*fDriverDSPA2DPDisabledEvent)(U8 linkIndex);
		void (*fDriverDSPSCOEnabledEvent)(U8 linkIndex, BOOL isLightMono);
		void (*fDriverDSPSCODisabledEvent)(U8 linkIndex);
		void (*fDriverDSPLineInEnabledEvent)(void);
		void (*fDriverDSPLineInDisabledEvent)(void);
		void (*fDriverReqCloseFMEvent)(void);
		void (*fDriverATSwitchEvent)(void);
		void (*fDriverAsyncSCOEnabledEvent)(U8 linkIndex);
		void (*fDriverAsyncSCODisabledEvent)(U8 linkIndex);
		void (*fDriverPEQChangedEvent)(U8 a2dpPeq, U8 auxPeq);
	}MMI_DRV_HANDLE_TYPE;


	PUBLIC void MMI_DRV_Init(void);
	PUBLIC void MMI_DRV_EventHandler(void);
	PUBLIC void MMI_DRV_KeyEventHandler(U8 linkIndex, U16 keyCode);
	PUBLIC U16 MMI_DRV_CheckKeyValid(U16 keyCode);
	PUBLIC void MMI_DRV_LoadVoiceLangCtl(U8 vpLangIndex);
	PUBLIC void MMI_DRV_ChangeVoicePromptLangCandidate(void);
	PUBLIC void MMI_DRV_SendVoicepromptStopCmd(void);
	PUBLIC void MMI_DRV_SpecificEventVPStopCmd(U16 id);
	PUBLIC void MMI_DRV_SendRingtoneCmd(U16 mediaEventIndex, U8 beepPattern);
	PUBLIC void MMI_DRV_SendRingtoneStopCmd(void);
	PUBLIC void MMI_DRV_SpecificEventRingtoneStopCmd(U16 id);
	PUBLIC void MMI_DRV_SendFakeMediaCmd(U16 mediaEventIndex);

	PUBLIC void MMI_DRV_SetMicMuteByLink(U8 muteCmd, U8 linkIndex);
	PUBLIC void MMI_DRV_AutoMICMuteOff(U8 linkIndex);
	PUBLIC void MMI_DRV_MicMuteHandler(U8 linkIndex, U8 muteCmd);
	
	PUBLIC BOOL MMI_DRV_A2DPDSPEnable(void);
	PUBLIC U8 MMI_DRV_GetA2DPEnabledLinkIndex(void);
	PUBLIC void MMI_DRV_SendDriverVolCmd(U8 linkIndex, U8 soundLevel, U8 smallStep, U8 micGainSetIndex);
	PUBLIC void MMI_DRV_SetSCODriverVolPara(U8 soundLevel, U8 micGainSetIndex, U8 smallStep, U8 isWideBand, DriverVolumeSetCmd XDATA_PTR volPtr);
	PUBLIC void MMI_DRV_SetSCOVolValue(U8 linkIndex, U8 soundLevel, U8 smallStep);
	PUBLIC void MMI_DRV_SCOAbsVolToSoundLevel(U8 linkIndex, U8 absVol);
	PUBLIC U8 MMI_DRV_GetAudioInMusicLink(void);
	PUBLIC void MMI_DRV_SendStopVoiceCommandCmd(void);
	PUBLIC void MMI_DRV_FlashStartUpdating(void);
	PUBLIC void MMI_DRV_FlashUpdateFail(void);
	PUBLIC void MMI_DRV_FlashUpdateEnd(void);
	PUBLIC void MMI_DRV_GetFlashUpdatePacket(void);
	PUBLIC U16 MMI_DRV_KeyParser(KeyIndex keyIndex, U8 keyAction, U8 state, BOOL isLocalParser);
	PUBLIC U8 MMI_DRV_GetKeyEventGroup(U16 keyCode);
	PUBLIC void MMI_DRV_KeyHandler(U16 keyEventCode);

	extern MMI_DRV_HANDLE_TYPE CODE gMMI_Drv_Handler;
	
	enum
	{
		NO_FLASH_WRITTEN_NEEDED,
		PENDING_TO_WRITE_FLASH,
		DIRECTLY_WRITE_FLASH,
	};
	
	enum
	{
		AUDIO_SWITCH_RESULT_OK,
		AUDIO_SWITCH_RESULT_PENDING,
		AUDIO_SWITCH_RESULT_FAIL,
	};

	//----------------------------------------------------
	//---------------- Sound Control --------------------
	//----------------------------------------------------

	PUBLIC void MMI_DRV_Beep(U8 soundLevel, U8 device);
	PUBLIC void MMI_DRV_MediaEventSendFakeCmd(U16 mediaEventIndex);
	PUBLIC void MMI_DRV_SendVoicePromptCmd(U16 mediaEventIndex, U8 voicePromptIndex);
	PUBLIC BOOL MMI_CheckValidPowerOffMediaEvt(U16 mediaEventIndex);
	PUBLIC void MMI_RingtoneVoicePromptScheduler(U16 mediaEventIndex, U8 beepPattern, U8 voicePromptIndex, U8 is_voice_prompt_before_ringtone);
	PUBLIC void MMI_SendDSPCmd(U8 linkIndex, U8 dspEnableType);

	PUBLIC MMIMsgType XDATA_PTR MMI_GetMMIMsgTypeCmdMemory(U8 opCode);

	//---------------- Volume Control --------------------

	PUBLIC BOOL MMI_CheckSCOInAudioService(U8 linkIndex);


	PUBLIC U8 MMI_SearchSoundLevelByVgs(U8 vgsValue);
	PUBLIC void MMI_DRV_SetSpkMuteByLink(U8 muteCmd, U8 linkIndex);



	PUBLIC U8 MMI_GetSCOGainByLevel(U8 level, BOOL isSpk);
	PUBLIC U8 MMI_GetA2DPGainByLevel(U8 level, BOOL isSpk);
	PUBLIC U8 MMI_GetLineInGainByLevel(U8 level, BOOL isSpk);
	PUBLIC void MMI_NotifyDriverBatteryStatus( U8 batStatus);
	PUBLIC void MMI_UartVolumeSet(HCI_VEVT_PARA_VOL_SET XDATA_PTR dataPtr);


	PUBLIC void MMI_DRV_AudioSelCmd(DriverAudioSelCmd XDATA_PTR audioSelPtr);
	#ifdef SUPPORT_VOICE_COMMAND
	PUBLIC void MMI_SetVoiceCommandVol(void);
	#endif

#ifdef PEQ_ENABLE
	PUBLIC BOOL MMI_DRV_PEQModeChange(void);
	PUBLIC void MMI_DRV_RealTimeSetPeqHpf(MMIMsgType XDATA_PTR msgPtr);
	#ifdef AIR_MODULE
	PUBLIC void MMI_DRV_SyncPEQRelayerToFollower(U8 peqMode, U8 peqModeIndex, U8 sectorMode);
	PUBLIC void MMI_DRV_SetFollowerPEQParameter(U8 peqMode, U8 peqModeIndex, U8 sectorMode);
	PUBLIC void MMI_DRV_FollowerPEQModeReload(void);
	PUBLIC void MMI_DRV_SyncRealTimePEQFollowerToRelayer(U8 XDATA_PTR para, U8 enable_mode);
	#endif
#endif

	PUBLIC BOOL MMI_PlayVoicePromptIsAvailable(U16 mediaEventIndex);

	PUBLIC void MMI_DRV_VoiceAdjustment(U8 linkIndex, U16 keyCode, BOOL isFollowerCmd);
	PUBLIC U8 MMI_DRV_IncreaseSmallStepVolume(U8 spkSoundLevel, U8 spkSoundLevelTotalNo, U8 smallStepAnchorCnt, U8 smallStep);
	PUBLIC U8 MMI_DRV_DecreaseSmallStepVolume(U8 smallStepAnchorCnt, U8 smallStep);


	PUBLIC void MMI_DRV_RemoveServiceLink(U8 linkIndex);
	PUBLIC void MMI_DRV_AddServiceLink(U8 linkIndex);
	PUBLIC BOOL MMI_DRV_SetAudioDeviceToAnotherLink(U8 linkIndex, U8 audioDev);
	PUBLIC BOOL MMI_DRV_IsHighPriAudioDeviceRegistered(U8 audioDev);
	PUBLIC BOOL MMI_DRV_IsOtherAudioDeviceEnabled(U8 linkIndex, U8 audioDev);
	PUBLIC BOOL MMI_DRV_IsAudioDeviceRegistered(U8 linkIndex, U8 audioDev);
	PUBLIC BOOL MMI_DRV_AudioDeviceRegister(U8 linkIndex, U8 audioDev);
	PUBLIC void MMI_DRV_AudioDeviceDeRegister(U8 linkIndex, U8 audioDev); //This must be used before state changing
	PUBLIC void MMI_DRV_ResumeOtherAudioDevice(U8 linkIndex, U8 audioDev);
	PUBLIC BOOL MMI_DRV_IsAudioActiveDev(U8 linkIndex, U8 audioDev);
	PUBLIC BOOL MMI_DRV_IsActiveAudioDevInCall(void);
	PUBLIC BOOL MMI_DRV_IsActiveAudioDevInMusic(void);
	PUBLIC BOOL MMI_DRV_IsActiveAudioDevInFM(void);
	PUBLIC BOOL MMI_DRV_IsActiveAudioDevInAUX(void);
	
	PUBLIC U8 MMI_DRV_WriteFlashAfterRingtoneVPStopped(void);
	PUBLIC void MMI_DRV_AudioBistHCICommand(MMIMsgType XDATA_PTR msgPtr);
	
	PUBLIC U8 MMI_DRV_SwitchScoToLink(U8 linkIndex);
	PUBLIC void MMI_DRV_SetAudioSwitchState(U8 state);
	PUBLIC U8 MMI_DRV_GetAudioSwitchState(void);
	#ifdef SUPPORT_VOICE_COMMAND
	PUBLIC void MMI_DRV_SendVoiceCommandCmd(U16 mediaEvent);
	#endif
	
	PUBLIC void MMI_DRV_ResumeDSP(U8 reason);
	PUBLIC void MMI_DRV_SuspendDSP(U8 reason, VFUN callback);

	PUBLIC void MMI_DRV_DSPRefGainCmd(void);
	PUBLIC void MMI_DRV_SendAsyncSCOEnableCmd(U8 linkIndex);

	PUBLIC void MMI_DRV_Switch_AudioTransparency(BOOL isEnable);
	PUBLIC void MMI_DRV_SetMicSoundLevelInA2DP(U8 soundLevel);
	PUBLIC void MMI_DRV_EnableVoicePrompt(void);
	PUBLIC void MMI_DRV_DisableVoicePrompt(void);
	PUBLIC BOOL MMI_DRV_IsVoicePromptEnabled(void);
	PUBLIC void MMI_DRV_EnterMPStateCmd(void);
	PUBLIC void MMI_DRV_ADCRealTimeReading(U8 channel);
	PUBLIC void MMI_DRV_ToolSetVolGain(HCI_VCMD_PARA_AIR_CMD_GET_VOL_GET XDATA_PTR msgPtr);
	PUBLIC void MMI_DRV_ToolSetCurrentGain(HCI_VCMD_PARA_AIR_CMD_SET_CURR_GAIN XDATA_PTR msgPtr);
	PUBLIC void MMI_DRV_ScoModeControl(U8 linkIndex, BOOL isECNRDisable, BOOL isLightMono);
	PUBLIC void MMI_DRV_NTCValueReport(void);


	PUBLIC void MMI_DRV_RejectCallMuteSCOCmd(BOOL value);
	PUBLIC void MMI_DRV_MicVolHandler(U8 linkIndex, U16 keyCode);
	PUBLIC void MMI_DRV_MaxMinVolumeLog(U16 mediaID);
	
	PUBLIC BOOL MMI_DRV_OTAVoicepromptsExist(void);
	PUBLIC void MMI_DRV_SetOTAVoicepromptsExist(BOOL exist);
	PUBLIC BOOL MMI_DRV_IsOTAVoicepromptsExisting(void);
	#ifdef AIRAPP_Profile
	PUBLIC BOOL MMI_DRV_IsDSPLockedByOTA(void);
	#endif
	
#endif

enum mmi_dgbprint_enum
{
	OCF_DBGPRINT_U8 = 0,
	OCF_DBFPRINT_U16,
};
