#include "reside_flash.inc"

#define _MMI_FM_C_

#include "rc.h"
#include "bt_config_profile.h"

#ifdef FM_ENABLE
#include "MMI.h"
#include "MMI_FM.h"
#include "MMI_State.h"
#include "MMI_HCI.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_Reconnect.h"

#ifdef SUPPORT_VOICE_PROMPT
	#include "..\Driver\VoicePrompt.h"
#endif
#include "..\Driver\Driver_FM.h"

#include "..\ROM\DRIVER\drv_gpio.h"
#include "..\SECTOR\Config1\sector_config1.h"


PRIVATE void MMI_FMEntry(U8 linkIndex);
PRIVATE void MMI_FMHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_FMExit(U8 linkIndex);
PRIVATE void MMI_FM_SaveHistoryFreq(void);
PRIVATE S16 MMI_FM_GetCurFreq(void);
PRIVATE void MMI_FM_KeyPowerOn(U8 linkIndex);
PRIVATE void MMI_FM_KeyPowerOff(U8 linkIndex);
PRIVATE void MMI_FM_KeySeekTune(U8 linkIndex);
PRIVATE void MMI_FM_KeyMuteToggle(U8 linkIndex);
PRIVATE U16 MMI_FM_CheckKeyPowerOn(U16 keyCode);
PRIVATE U16 MMI_FM_CheckKeyPowerOff(U16 keyCode);
PRIVATE U16 MMI_FM_CheckKeyFunction(U16 keyCode);

extern MmiHandlerSetType CODE_PTR XDATA MMI_StateHandlerSet[MMI_TOTAL_STATE_NO];

PRIVATE MmiHandlerSetType CODE	MMI_StateFMHandler = 
{
	MMI_FMEntry, MMI_FMHandler, MMI_FMExit, //MMI_FM
};

static MMI_KEY_EVT CODE MMI_FM_KeyEvtHandler[FM_KEY_EVENT_NO] =
{
	{MMI_FM_KeyPowerOn}, //FM_KEY_EVENT_KEY_FM_POWERON
	{MMI_FM_KeyPowerOff}, //FM_KEY_EVENT_KEY_FM_POWEROFF 
	{MMI_FM_KeySeekTune}, //FM_KEY_EVENT_KEY_SEEKUP 
	{MMI_FM_KeySeekTune}, //FM_KEY_EVENT_KEY_SEEKDOWN
	{MMI_FM_KeySeekTune}, //FM_KEY_EVENT_KEY_TUNEUP, 
	{MMI_FM_KeySeekTune}, //FM_KEY_EVENT_KEY_TUNEDN,	 	 
	{MMI_FM_KeyMuteToggle}, //FM_KEY_EVENT_KEY_FM_MUTETOGGLE, 
};

static MMI_KEY_CHECK CODE MMI_FM_KeyCheckValidHandler[FM_KEY_EVENT_NO] =
{
	{MMI_FM_CheckKeyPowerOn}, //FM_KEY_EVENT_KEY_FM_POWERON
	{MMI_FM_CheckKeyPowerOff}, //FM_KEY_EVENT_KEY_FM_POWEROFF 
	{MMI_FM_CheckKeyFunction}, //FM_KEY_EVENT_KEY_SEEKUP 
	{MMI_FM_CheckKeyFunction}, //FM_KEY_EVENT_KEY_SEEKDOWN
	{MMI_FM_CheckKeyFunction}, //FM_KEY_EVENT_KEY_TUNEUP, 
	{MMI_FM_CheckKeyFunction}, //FM_KEY_EVENT_KEY_TUNEDN,	 	 
	{MMI_FM_CheckKeyFunction}, //FM_KEY_EVENT_KEY_FM_MUTETOGGLE,
};

PUBLIC void MMI_FM_Init(void)
{
	MMI_StateHandlerSet[MMI_FM] = &MMI_StateFMHandler;
}

PUBLIC void MMI_FM_ProcessKeyEvent(U8 linkIndex, U8 index)
{
	if(MMI_FM_KeyEvtHandler[index].fKeyEvt)
	{
		MMI_FM_KeyEvtHandler[index].fKeyEvt(linkIndex);
	}
}

PUBLIC U16 MMI_FM_CheckValidKey(U8 index, U16 keyCode)
{
	if(MMI_FM_KeyCheckValidHandler[index].fKeyCheckValid)
	{
		keyCode = MMI_FM_KeyCheckValidHandler[index].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

PUBLIC void MMI_FM_DriverCmd(U8 FMCmd)
{
	switch(FMCmd)
	{
		case DRIVER_FM_POWERON_CMD:
			gMMI_FM_ctl.curFMSoundLevel = gMMI_driver_variation_nvram.misc_para.init.FMHistoryVol;
			gMMI_FM_ctl.volumeSmallStepCount = gMMI_driver_variation_nvram.misc_para.init.FMSmallStepVol;
			DRIVER_FM_PowerOnCmd(gMMI_driver_variation_nvram.misc_para.init.FM_history_Freq, gMMI_driver_variation_nvram.misc_para.init.FMHistoryVol);
			break;
		case DRIVER_FM_POWEROFF_CMD:
			gMMI_FM_ctl.isDriverFMBusy = FALSE;
			MMI_FM_SaveHistoryFreq();
			DRIVER_FM_PowerOffCmd();
			break;
		case DRIVER_FM_TUNEUP_CMD:
			DRIVER_FM_TuneCmd(TRUE);
			break;
		case DRIVER_FM_TUNEDN_CMD:
			DRIVER_FM_TuneCmd(FALSE);
			break;
		case DRIVER_FM_HWSEEKUP_CMD:
			DRIVER_FM_SeekCmd(TRUE);
			break;
		case DRIVER_FM_HWSEEKDN_CMD:
			DRIVER_FM_SeekCmd(FALSE);
			break;
		case DRIVER_FM_MUTETOGGLE_CMD:
			DRIVER_FM_MuteCmd(FM_MUTE_MODE_TOGGLE);
			break;
		case DRIVER_FM_MUTE_ON_CMD:
			DRIVER_FM_MuteCmd(FM_MUTE_MODE_ON);
			break;
		case DRIVER_FM_MUTE_OFF_CMD:
			DRIVER_FM_MuteCmd(FM_MUTE_MODE_OFF);
			break;
		case DRIVER_FM_VOLUME_CMD:
			gMMI_FM_ctl.isDriverFMBusy = TRUE;
			DRIVER_FM_VolumeCmd(gMMI_FM_ctl.curFMSoundLevel);
			break ;
		default:
			break;
	}
}

PUBLIC void MMI_FM_VolumeControl(BOOL isVolumeUp)
{
	U8 XDATA_PTR spkSoundLevelPtr;
	spkSoundLevelPtr  = &gMMI_FM_ctl.curFMSoundLevel;
	if(isVolumeUp)
	{
		gMMI_FM_ctl.volumeSmallStepCount = MMI_DRV_IncreaseSmallStepVolume(gMMI_FM_ctl.curFMSoundLevel, SOUND_LEVEL_NO, gMMI_nvram.smallStepVolume.fmVolumeSmallStep, gMMI_FM_ctl.volumeSmallStepCount);

		// Only 16 levels are supported.
		if(gMMI_FM_ctl.volumeSmallStepCount ==0)
		{
			if(*spkSoundLevelPtr < (SOUND_LEVEL_NO-1))
			{
				(*spkSoundLevelPtr)++;
			}
			else if(MMI_FM_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				*spkSoundLevelPtr = 0;
			}
			else
			{
				MMI_PushMediaEvent(MEDIA_EVT_FM_VOLUME_REACH_MAX);
				return;
			}
		}
	}
	else
	{
		if(gMMI_FM_ctl.volumeSmallStepCount ==0)
		{
			if (*spkSoundLevelPtr > 0)
			 {
				(*spkSoundLevelPtr)--;
			 }
			else if(MMI_FM_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				*spkSoundLevelPtr = SOUND_LEVEL_NO-1;
				goto MMI_SEND_FM_VOL;
			}
			else
			{
				MMI_PushMediaEvent(MEDIA_EVT_FM_VOLUME_REACH_MIN);
				return;
			}
		}
		gMMI_FM_ctl.volumeSmallStepCount = MMI_DRV_DecreaseSmallStepVolume(gMMI_nvram.smallStepVolume.fmVolumeSmallStep, gMMI_FM_ctl.volumeSmallStepCount);
	}
	MMI_SEND_FM_VOL:

	if(gMMI_FM_ctl.volumeSmallStepCount)
		return;

	MMI_FM_DriverCmd(DRIVER_FM_VOLUME_CMD);
}

PUBLIC void MMI_FM_SendChannelVoicePromptCmd(S16 curFreq)
{
#ifdef SUPPORT_VOICE_PROMPT
	U8 index;
	U8 digitLen;
	MMIMsgType XDATA_PTR msgPtr;

	if (!MMI_PlayVoicePromptIsAvailable(MEDIA_EVT_FM_NOT_BUSY))
		goto MMI_FM_SEND_FAKE_CMD;

	if ((U16)curFreq >=  MIN_FREQUENCE && (U16)curFreq <= 999)
	{
		digitLen = 3;
	}
	else if((U16)curFreq > 999 && (U16)curFreq <= MAX_FREQUENCE)
	{
		digitLen = 4;
	}
	else
	{
		goto MMI_FM_SEND_FAKE_CMD;
	}

	msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_VOICEPROMPT_CMD);

	if (msgPtr == (MMIMsgType XDATA_PTR)NULL)
		goto MMI_FM_SEND_FAKE_CMD;

	for (index = digitLen; index > 0; index--)
	{
		msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.fmChannel.number[index-1] = curFreq % 10;
		curFreq /= 10;
	}

	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id = MEDIA_EVT_FM_NOT_BUSY;
	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpIndex = VP_EVT_FM_CHANNEL_NUMBER;
	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.fmChannel.digitLength = digitLen;

	OSMQ_PutFront(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	gMMI_FM_ctl.isDriverFMBusy = TRUE;

	MMI_FM_SEND_FAKE_CMD:
	MMI_DRV_SendFakeMediaCmd(MEDIA_EVT_FM_NOT_BUSY);
#else
	UNUSED(curFreq);
#endif
}


PRIVATE void MMI_FMEntry(U8 linkIndex)
{
	UNUSED(linkIndex);
	
	if(MMI_NO_CONNECTABLE_IN_FM_FEAT)
		MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);

	if(MMI_NO_DISCAVERABLE_IN_FM_FEAT)
		MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);

	MMI_PushMediaEvent(MEDIA_EVT_FM_ENABLE);
	SYS_ReleaseTimer(&gMMI_ctl.audioDevCtl.resumeTimer);
}

PRIVATE void MMI_FMHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(linkIndex);
	
	if (msgPtr->eventCode== DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT)
	{
		switch(msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id)
		{
			case MEDIA_EVT_FM_ENABLE:
				MMI_FM_DriverCmd(DRIVER_FM_POWERON_CMD);
				break;
			case MEDIA_EVT_KEY_SEEKUP:
				MMI_FM_DriverCmd(DRIVER_FM_HWSEEKUP_CMD);
				break;
			case MEDIA_EVT_KEY_SEEKDOWN:
				MMI_FM_DriverCmd(DRIVER_FM_HWSEEKDN_CMD);
				break;
			case MEDIA_EVT_KEY_TUNEUP:
				MMI_FM_DriverCmd(DRIVER_FM_TUNEUP_CMD);
				break;
			case MEDIA_EVT_KEY_TUNEDN:
				MMI_FM_DriverCmd(DRIVER_FM_TUNEDN_CMD);
				break;
			case MEDIA_EVT_FM_MUTE_OFF:
				MMI_FM_DriverCmd(DRIVER_FM_MUTE_OFF_CMD);
				break;
			case MEDIA_EVT_FM_MUTE_ON:
				MMI_FM_DriverCmd(DRIVER_FM_MUTE_ON_CMD);
				break;
			case MEDIA_EVT_FM_NOT_BUSY:
				gMMI_FM_ctl.isDriverFMBusy = FALSE;
				break;
		}
	}
	else if(msgPtr->eventCode== DRIVER_FM_BUSY_EVENT)
	{
		gMMI_FM_ctl.isDriverFMBusy = TRUE;
	}
	else if(msgPtr->eventCode== DRIVER_FM_NOT_BUSY_EVENT)
	{
		MMI_FM_SendChannelVoicePromptCmd(MMI_FM_GetCurFreq());
	}
	else if(msgPtr->eventCode== DRIVER_FM_MUTE_ON_EVENT )
	{
		MMI_PushMediaEvent(MEDIA_EVT_FM_MUTE_OFF); //toggle
	}
	else if(msgPtr->eventCode== DRIVER_FM_MUTE_OFF_EVENT)
	{
		MMI_PushMediaEvent(MEDIA_EVT_FM_MUTE_ON); //toggle
	}
	gMMI_ctl.msgHandled = NOTPROCD;
}

PRIVATE void MMI_FMExit(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_FM_DriverCmd(DRIVER_FM_POWEROFF_CMD);
}

PRIVATE void MMI_FM_SaveHistoryFreq(void)
{
	S16 curFreq;
	curFreq = MMI_FM_GetCurFreq();
	if ((U16)curFreq> MAX_FREQUENCE || (U16)curFreq< MIN_FREQUENCE)
	{
		gMMI_driver_variation_nvram.misc_para.init.FM_history_Freq = 875;
		gMMI_driver_variation_nvram.misc_para.init.FMHistoryVol = gMMI_FM_ctl.curFMSoundLevel;
		gMMI_driver_variation_nvram.misc_para.init.FMSmallStepVol = gMMI_FM_ctl.volumeSmallStepCount;
	}
	else
	{
		gMMI_driver_variation_nvram.misc_para.init.FM_history_Freq = curFreq;
		gMMI_driver_variation_nvram.misc_para.init.FMHistoryVol = gMMI_FM_ctl.curFMSoundLevel;
		gMMI_driver_variation_nvram.misc_para.init.FMSmallStepVol = gMMI_FM_ctl.volumeSmallStepCount;
	}
	MMI_SECTOR_SetUpdateFlag(TRUE);
}

PRIVATE S16 MMI_FM_GetCurFreq(void)
{
	return DRIVER_FM_GetFreq();
}

PRIVATE void MMI_FM_KeyPowerOn(U8 linkIndex)
{
	UNUSED(linkIndex);
	if(MMI_DRV_AudioDeviceRegister(MMI_EOF, AUDIO_DEVICE_FM))
	{
		if(MMI_NO_CONNECTION_IN_FM_FEAT)
		{
			MMI_PROTOCOL_RelAllLinks();
		}
		MMI_AddTopState(MMI_EOF, MMI_FM);	
	}
}

PRIVATE void MMI_FM_KeyPowerOff(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_DRV_AudioDeviceDeRegister(MMI_EOF, AUDIO_DEVICE_FM);
	MMI_RmvState(MMI_EOF, MMI_FM);

	if(MMI_NO_CONNECTABLE_IN_FM_FEAT)
	{
		MMI_ConnectabilityHandle(CMD_SET_RESUME_CONNECTABLE);
	}

	if(MMI_NO_DISCAVERABLE_IN_FM_FEAT)
	{
		MMI_DiscoverabilityHandle(CMD_SET_RESUME_DISCOVERABLE);
	}

	if(MMI_NO_CONNECTION_IN_FM_FEAT)
	{
		if(gMMI_ctl.miscMask4& MMI_MASK4_RECONNECT_LINK0)
			MMI_Reconnect(RECONNECT_FIRST_ONCE);
		if((gMMI_ctl.miscMask4& MMI_MASK4_RECONNECT_LINK1))
			MMI_Reconnect(RECONNECT_SECONDARY_ONCE);
		gMMI_ctl.miscMask4 &= ~(MMI_MASK4_RECONNECT_LINK0 | MMI_MASK4_RECONNECT_LINK1);
	}
	
	MMI_DRV_ResumeOtherAudioDevice(MMI_EOF, AUDIO_DEVICE_FM);
}

PRIVATE void MMI_FM_KeySeekTune(U8 linkIndex)
{
	UNUSED(linkIndex);
	gMMI_FM_ctl.isDriverFMBusy = TRUE;
}

PRIVATE void MMI_FM_KeyMuteToggle(U8 linkIndex)
{
	UNUSED(linkIndex);
	gMMI_FM_ctl.isDriverFMBusy = TRUE;
	MMI_FM_DriverCmd(DRIVER_FM_MUTETOGGLE_CMD);
}

PRIVATE U16 MMI_FM_CheckKeyPowerOn(U16 keyCode)
{
	if(MMI_DRV_IsHighPriAudioDeviceRegistered(AUDIO_DEVICE_LINE_IN))
	{
		keyCode = KEY_INVALID;
	}
	return keyCode;
}

PRIVATE U16 MMI_FM_CheckKeyPowerOff(U16 keyCode)
{
	if(!MMI_CheckInFMState())
	{
		keyCode = KEY_INVALID;
	}
	return keyCode;
}

PRIVATE U16 MMI_FM_CheckKeyFunction(U16 keyCode)
{
	if(gMMI_FM_ctl.isDriverFMBusy)
		keyCode = KEY_INVALID;
		
	return keyCode;
}

PRIVATE void MMI_FM_DRV_Event_PassThruMsg(MMIMsgType XDATA_PTR msgPtr)
{
	gMMI_ctl.msgHandled = NOTPROCD;
	MMI_STATE_PassThruMsg(gMMI_ctl.serviceInd, msgPtr);
	if (gMMI_ctl.msgHandled != PROCD_TRANSIT)
	{
		OSMEM_Put ((U8 XDATA_PTR)msgPtr);
	}
}

PRIVATE void MMI_FM_DRV_NotBusy_Event(MMIMsgType XDATA_PTR msgPtr)
{
	gMMI_FM_ctl.isDriverFMBusy = FALSE;
	MMI_FM_DRV_Event_PassThruMsg(msgPtr);
}

PRIVATE void MMI_FM_DRV_Disabled_Event(void)
{
	gMMI_FM_ctl.isDriverFMBusy = FALSE;
}

PRIVATE void MMI_FM_DRV_CmdSet_Event(void)
{
	gMMI_FM_ctl.isDriverFMBusy = FALSE;
}

MMI_FM_DRV_HANDLE_TYPE CODE gMMI_FM_Drv_Handler =
{
	MMI_FM_DRV_Event_PassThruMsg,	//fDriverFMBusyEvent,
	MMI_FM_DRV_NotBusy_Event,		//fDriverFMNotBusyEvent,
	MMI_FM_DRV_Disabled_Event,		//fDriverFMDisabledEvent
	MMI_FM_DRV_CmdSet_Event,		//fDriverFMCmdSetEvent
	MMI_FM_DRV_Event_PassThruMsg,	//fDriverFMMuteOnEvent,
	MMI_FM_DRV_Event_PassThruMsg,	//fDriverFMMuteOffEvent,
};

#endif //FM_ENABLE