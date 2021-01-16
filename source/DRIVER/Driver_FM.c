#include "reside_flash.inc"

#define _DRIVER_FM_C_

#include "rc.h"
#include "bt_config_profile.h"

#include "..\MMI\MMI.h"
#include "Driver.h"
#include "..\MMI\MMI_FM.h"

#ifdef RINGTONE_ENABLE
	#include "ringtone.h"
#endif
#ifdef SUPPORT_VOICE_PROMPT
	#include "VoicePrompt.h"
#endif

#include "Audio_Mailbox.h"
#include "Driver_1520.h"

#include "AudioControl.h"
#include "Driver_FM.h"

#include "..\SDK_Proxy\SDK_Proxy.h"

#ifdef FM_ENABLE

extern VFUN XDATA api_iodrv_evt_func[API_GRP_IODRV_CB_CNT];

enum
{
	FM_OFF,
	FM_WAITING_POWER_ON,
	FM_ON,
	FM_SEEK_UP,
	FM_SEEK_DOWN,
	FM_TUNE_UP,
	FM_TUNE_DOWN,
	FM_MUTE_ON,
	FM_MUTE_OFF,
	FM_WAITING_POWER_OFF,
};

enum
{
	FM_SUBSTATE_NONE,
	FM_SUBSTATE_WAITING_ON_CLOSE_OTHER_AUDIOS,
	FM_SUBSTATE_WAITING_ON_CHECK_OTHER_AUDIO_CLOSED,
	FM_SUBSTATE_WAITING_ON_RESET,
	FM_SUBSTATE_WAITING_ON_WAITING_DRV_READY,
	FM_SUBSTATE_WAITING_ON_ABORT,
	FM_SUBSTATE_SEEK_TUNE_WAIT_VP_RING_END,
	FM_SUBSTATE_SEEK_TUNE_WAITING_DRV_READY,
	FM_SUBSTATE_MUTE_SET,
};
typedef struct
{
	U8 fmState;
	U8 fmSubState;
}DriverFMCtlType;

DriverFMCtlType XDATA gDriver_FM_Ctl;

OSMQ XDATA OSMQ_FM_Command_From_MMI;
#define OSMQ_FM_Command_From_MMI_ptr		&OSMQ_FM_Command_From_MMI

PRIVATE void DRIVER_FM_ClearState(U8 fmState)
{
	gDriver_FM_Ctl.fmState = fmState;
	gDriver_FM_Ctl.fmSubState = FM_SUBSTATE_NONE;
	DRIVER_SetPowerSaving(TRUE, DRIVER_POWER_SAVING_FM);
	DRIVER_SetMiscCtrlMap(DRIVER_VP_RING_LOCKED_FOR_FM, FALSE);
}

PRIVATE void DRIVER_FM_SwtchState(U8 fmState, U8 fmSubState)
{
	gDriver_FM_Ctl.fmState = fmState;
	gDriver_FM_Ctl.fmSubState = fmSubState;
	DRIVER_SetMiscCtrlMap(DRIVER_VP_RING_LOCKED_FOR_FM, TRUE);
}

PRIVATE void DRIVER_FM_OffHandler(void)
{
	DriverMsg XDATA_PTR msgPtr;

	if ((msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get (OSMQ_FM_Command_From_MMI_ptr)) != (DriverMsg XDATA_PTR)NULL)
	{
		if(msgPtr->msgOpcode == DRIVER_FM_POWERON_CMD)
		{
			if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_SETVOL])
			{
				((API_IODRV_FM_SETVOL_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_SETVOL])(msgPtr->msgBodyPtr.driverCmd.FMCmd.fmPoweronPara.historyVol);
			}
			if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_SETFREQ])
			{
				((API_IODRV_FM_SETFREQ_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_SETFREQ])(msgPtr->msgBodyPtr.driverCmd.FMCmd.fmPoweronPara.setFreq);
			}
			if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_SETMUTE])
			{
				((API_IODRV_FM_SETMUTE_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_SETMUTE])(msgPtr->msgBodyPtr.driverCmd.FMCmd.fmPoweronPara.isMute);
			}
			DRIVER_FM_SwtchState(FM_WAITING_POWER_ON, FM_SUBSTATE_WAITING_ON_CLOSE_OTHER_AUDIOS);
		}
		else if(msgPtr->msgOpcode == DRIVER_FM_POWEROFF_CMD)
		{
			DRIVER_SetMiscCtrlMap(DRIVER_IS_RESUME_FM, FALSE);

			if(gDriver_ctl.playingRingtoneId == MEDIA_EVT_FM_NOT_BUSY)
			{
				#ifdef RINGTONE_ENABLE
				if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_RINGTONE) && !Ring_Pause())
				{
					OSMQ_PutFront (OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
					return;
				}
				#endif
				#ifdef SUPPORT_VOICE_PROMPT
				if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_VOICEPROMPT) && !VoicePrompt_Pause())
				{
					OSMQ_PutFront (OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
					return;
				}
				#endif
			}
		}
		else
		{
			if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_RESUME_FM))
			{
				OSMQ_PutFront(OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
				return;
			}
		}
		OSMEM_Put((U8 XDATA_PTR)msgPtr);
	}
}


PRIVATE void DRIVER_FM_WaitingPowerOnHandler(void)
{
	U8 cmdStatus;
	
	if(!api_iodrv_evt_func[API_GRP_IODRV_CB_FM_PWR_ON])
	{
		DRIVER_FM_ClearState(FM_OFF);
		gMMI_FM_Drv_Handler.fDriverFMDisabledEvent();
		return;		
	}

	switch(gDriver_FM_Ctl.fmSubState)
	{
		case FM_SUBSTATE_WAITING_ON_CLOSE_OTHER_AUDIOS:
			if(IS_DSP_BUSY)
				break;

			gDriver_FM_Ctl.fmSubState =FM_SUBSTATE_WAITING_ON_RESET;
			
			if(DRIVER_IsDSPOgfAppOn())
			{
				DRIVER_SendAppIdleToMailBox();
				gDriver_FM_Ctl.fmSubState = FM_SUBSTATE_WAITING_ON_CHECK_OTHER_AUDIO_CLOSED;
			}
			break;

		case FM_SUBSTATE_WAITING_ON_CHECK_OTHER_AUDIO_CLOSED:
			if(IS_DSP_BUSY)
				break;
			if(!DRIVER_IsDSPOgfAppOn())
			{
				gDriver_FM_Ctl.fmSubState =FM_SUBSTATE_WAITING_ON_RESET;
			}
			break;
		
		case FM_SUBSTATE_WAITING_ON_RESET:
			if(DRIVER_IsRingToneOrVPOrATPlaying() || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
			   break;
			   
			AUDIO_Close_AMP();
			
			if(MEMORY_PUT != DRIVER_CloseAllAudioHandler1520((DriverMsg XDATA_PTR)NULL))
				break;

			DRIVER_SetPowerSaving(FALSE, DRIVER_POWER_SAVING_FM);
			gDriver_FM_Ctl.fmSubState = FM_SUBSTATE_WAITING_ON_WAITING_DRV_READY;
			break;
		case FM_SUBSTATE_WAITING_ON_WAITING_DRV_READY:
			cmdStatus = ((API_IODRV_FM_PWR_ON_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_PWR_ON])();
			if(cmdStatus == FM_CMD_STATUS_OK)
			{
				//Aurio Related 
				AUDIO_SetAudioOn(AUDIO_FM);
				if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_AUDIO_ON])
				{
					((API_IODRV_FM_AUDIO_ON_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_AUDIO_ON])();
				}
				AUDIO_Open_AMP();				
				DRIVER_FM_ClearState(FM_ON);			
			}
			else if(cmdStatus == FM_CMD_STATUS_FAIL)
			{
				gDriver_FM_Ctl.fmSubState = FM_SUBSTATE_WAITING_ON_ABORT;
			}
			break;
			
		case FM_SUBSTATE_WAITING_ON_ABORT:
			if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_ABT_PWR_ON])
			{
				((API_IODRV_FM_ABT_PWR_ON_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_ABT_PWR_ON])();
			}
			DRIVER_FM_ClearState(FM_OFF);
			gMMI_FM_Drv_Handler.fDriverFMDisabledEvent();		
			break;
	}
}

PRIVATE void DRIVER_FM_PowerOnHandler(void)
{
	DriverMsg XDATA_PTR msgPtr;

	if ((msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get (OSMQ_FM_Command_From_MMI_ptr)) != (DriverMsg XDATA_PTR)NULL)
	{
		switch(msgPtr->msgOpcode)
		{
			case DRIVER_FM_HWSEEKUP_CMD:
				DRIVER_FM_SwtchState(FM_SEEK_UP, FM_SUBSTATE_SEEK_TUNE_WAIT_VP_RING_END);
				break;
			case DRIVER_FM_HWSEEKDN_CMD:
				DRIVER_FM_SwtchState(FM_SEEK_DOWN, FM_SUBSTATE_SEEK_TUNE_WAIT_VP_RING_END);		
				break;
			case DRIVER_FM_TUNEUP_CMD:
				DRIVER_FM_SwtchState(FM_TUNE_UP, FM_SUBSTATE_SEEK_TUNE_WAIT_VP_RING_END);
				break;
			case DRIVER_FM_TUNEDN_CMD:
				DRIVER_FM_SwtchState(FM_TUNE_DOWN, FM_SUBSTATE_SEEK_TUNE_WAIT_VP_RING_END);		
				break;
			case DRIVER_FM_MUTETOGGLE_CMD:
				if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_GETMUTE])
				{
					if(((API_IODRV_FM_GETMUTE_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_GETMUTE])())
						gMMI_FM_Drv_Handler.fDriverFMMuteOnEvent((MMIMsgType XDATA_PTR)DRIVER_GetGeneralEventMemory(DRIVER_FM_MUTE_ON_EVENT));
					else
						gMMI_FM_Drv_Handler.fDriverFMMuteOffEvent((MMIMsgType XDATA_PTR)DRIVER_GetGeneralEventMemory(DRIVER_FM_MUTE_OFF_EVENT));	
				}		
				break;
			case DRIVER_FM_MUTE_ON_CMD:
				DRIVER_FM_SwtchState(FM_MUTE_ON, FM_SUBSTATE_MUTE_SET);
				break;
			case DRIVER_FM_MUTE_OFF_CMD:
				DRIVER_FM_SwtchState(FM_MUTE_OFF, FM_SUBSTATE_MUTE_SET);
				break;
			case DRIVER_FM_VOLUME_CMD:
				if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_VOL])
				{
					((API_IODRV_FM_VOL_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_VOL])(msgPtr->msgBodyPtr.driverCmd.FMCmd.FMVolume);
				}
				gMMI_FM_Drv_Handler.fDriverFMCmdSetEvent();			
				break;
			case DRIVER_FM_POWEROFF_CMD:
				DRIVER_SetMiscCtrlMap(DRIVER_IS_RESUME_FM, FALSE);
			case DRIVER_FM_INTERNAL_POWEROFF_CMD:
				DRIVER_FM_SwtchState(FM_WAITING_POWER_OFF, FM_SUBSTATE_NONE);
				break;
		}
		OSMEM_Put((U8 XDATA_PTR)msgPtr);
	}
}

PRIVATE void DRIVER_FM_SeekTuneHandler(BOOL isSeek, BOOL isUp)
{
	U8 cmdStatus;
	
	if(isSeek)
	{
		if(!api_iodrv_evt_func[API_GRP_IODRV_CB_FM_SEEK])
		{
			DRIVER_FM_ClearState(FM_ON);
			return;		
		}	
	}
	else
	{
		if(!api_iodrv_evt_func[API_GRP_IODRV_CB_FM_TUNE])
		{
			DRIVER_FM_ClearState(FM_ON);
			return;		
		}	
	}


	switch(gDriver_FM_Ctl.fmSubState)
	{
		case FM_SUBSTATE_SEEK_TUNE_WAIT_VP_RING_END:
			if(DRIVER_IsRingToneOrVPOrATPlaying() || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
				break;

			gMMI_FM_Drv_Handler.fDriverFMBusyEvent((MMIMsgType XDATA_PTR)DRIVER_GetGeneralEventMemory(DRIVER_FM_BUSY_EVENT));
			gDriver_FM_Ctl.fmSubState = FM_SUBSTATE_SEEK_TUNE_WAITING_DRV_READY;
			break;

		case FM_SUBSTATE_SEEK_TUNE_WAITING_DRV_READY:
			if(isSeek)
				cmdStatus = ((API_IODRV_FM_SEEK_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_SEEK])(isUp);
			else
				cmdStatus = ((API_IODRV_FM_TUNE_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_TUNE])(isUp);
				
			if(cmdStatus == FM_CMD_STATUS_OK || cmdStatus == FM_CMD_STATUS_FAIL)
			{
				DRIVER_FM_ClearState(FM_ON);
				gMMI_FM_Drv_Handler.fDriverFMNotBusyEvent((MMIMsgType XDATA_PTR)DRIVER_GetGeneralEventMemory(DRIVER_FM_NOT_BUSY_EVENT));				
			}
			break;
	}
}

PRIVATE void DRIVER_FM_MuteOnOffHandler(BOOL isOn)
{
	U8 cmdStatus;

	if(DRIVER_IsRingToneOrVPOrATPlaying() || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
		return;
		
	if(!api_iodrv_evt_func[API_GRP_IODRV_CB_FM_MUTE])
	{
		DRIVER_FM_ClearState(FM_ON);
		return;		
	}	
		
	cmdStatus = ((API_IODRV_FM_MUTE_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_MUTE])(isOn);
	if(cmdStatus == FM_CMD_STATUS_OK || cmdStatus == FM_CMD_STATUS_FAIL)
	{	
		DRIVER_FM_ClearState(FM_ON);
		gMMI_FM_Drv_Handler.fDriverFMCmdSetEvent();	
	}
}


PRIVATE void DRIVER_FM_WaitingPowerOffHandler(void)
{
	if(DRIVER_IsRingToneOrVPOrATPlaying() || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
		return;

	AUDIO_Close_AMP();
	if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_AUDIO_OFF])
	{
		((API_IODRV_FM_AUDIO_OFF_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_AUDIO_OFF])();
	}
	AUDIO_SetAudioOff(AUDIO_FM);
	
	if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_PWR_OFF])
	{
		((API_IODRV_FM_PWR_OFF_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_PWR_OFF])();
	}
	
	DRIVER_FM_ClearState(FM_OFF);
	if(!DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_RESUME_FM))
	{	
		gMMI_FM_Drv_Handler.fDriverFMDisabledEvent();
	}
}

PUBLIC void DRIVER_FM_Handler(void)
{	
	DriverMsg XDATA_PTR msgPtr;
	
	if ((msgPtr = (DriverMsg XDATA_PTR)OSMQ_Get (OSMQ_FM_Command_From_MMI_ptr)) != (DriverMsg XDATA_PTR)NULL)
	{
		if(msgPtr->msgOpcode == DRIVER_FM_POWEROFF_CMD)
		{
			if(gDriver_FM_Ctl.fmState == FM_WAITING_POWER_ON)
			{
				gDriver_FM_Ctl.fmSubState = FM_SUBSTATE_WAITING_ON_ABORT;
			}
			else if(gDriver_FM_Ctl.fmState == FM_SEEK_UP || gDriver_FM_Ctl.fmState == FM_SEEK_DOWN || gDriver_FM_Ctl.fmState == FM_TUNE_UP || gDriver_FM_Ctl.fmState == FM_TUNE_DOWN)
			{
				DRIVER_FM_ClearState(FM_ON);
				gMMI_FM_Drv_Handler.fDriverFMNotBusyEvent((MMIMsgType XDATA_PTR)DRIVER_GetGeneralEventMemory(DRIVER_FM_NOT_BUSY_EVENT));
			}
			else if(gDriver_FM_Ctl.fmState == FM_MUTE_ON || gDriver_FM_Ctl.fmState == FM_MUTE_OFF)
			{
				DRIVER_FM_ClearState(FM_ON);
				gMMI_FM_Drv_Handler.fDriverFMCmdSetEvent();
			}
		}
		OSMQ_PutFront(OSMQ_FM_Command_From_MMI_ptr,(U8 XDATA_PTR)msgPtr);
	}

	switch(gDriver_FM_Ctl.fmState)
	{
		case FM_OFF:
			DRIVER_FM_OffHandler();
			break;
		case FM_WAITING_POWER_ON:
			DRIVER_FM_WaitingPowerOnHandler();
			break;
		case FM_ON:
			DRIVER_FM_PowerOnHandler();
			break;
		case FM_SEEK_UP:
			DRIVER_FM_SeekTuneHandler(TRUE, TRUE);
			break;
		case FM_SEEK_DOWN:
			DRIVER_FM_SeekTuneHandler(TRUE, FALSE);
			break;
		case FM_TUNE_UP:
			DRIVER_FM_SeekTuneHandler(FALSE, TRUE);
			break;
		case FM_TUNE_DOWN:
			DRIVER_FM_SeekTuneHandler(FALSE, FALSE);
			break;
		case FM_MUTE_ON:
			DRIVER_FM_MuteOnOffHandler(TRUE);
			break;
		case FM_MUTE_OFF:
			DRIVER_FM_MuteOnOffHandler(FALSE);
			break;
		case FM_WAITING_POWER_OFF:
			DRIVER_FM_WaitingPowerOffHandler();
			break;
	}
}

PUBLIC S16 DRIVER_FM_GetFreq(void)
{
	if(api_iodrv_evt_func[API_GRP_IODRV_CB_FM_GETFREQ])
	{
		return (((API_IODRV_FM_GETFREQ_FUNC)api_iodrv_evt_func[API_GRP_IODRV_CB_FM_GETFREQ])());
	}
	return 0;
}

PUBLIC void DRIVER_FM_Resume(void)
{
	if(!DRIVER_IsDSPOgfAppOn())
	{
		if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_RESUME_FM) && !AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM) )
		{
			DRIVER_FM_SwtchState(FM_WAITING_POWER_ON, FM_SUBSTATE_WAITING_ON_CLOSE_OTHER_AUDIOS);
			DRIVER_SetMiscCtrlMap(DRIVER_IS_RESUME_FM, FALSE);
		}
	}
	else
	{
		DRIVER_SetMiscCtrlMap(DRIVER_IS_RESUME_FM, FALSE);
	}
}

PUBLIC void DRIVER_FM_InternalPowerOff(void)
{
	DriverMsg XDATA_PTR msgPtr;
	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	msgPtr->msgOpcode = DRIVER_FM_INTERNAL_POWEROFF_CMD;
	OSMQ_PutFront(OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
}

PUBLIC void DRIVER_FM_PowerOnCmd(S16 setFreq, U8 historyVol)
{
	DriverMsg XDATA_PTR msgPtr;
	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	msgPtr->msgOpcode = DRIVER_FM_POWERON_CMD;
	msgPtr->msgBodyPtr.driverCmd.FMCmd.fmPoweronPara.setFreq = setFreq;
	msgPtr->msgBodyPtr.driverCmd.FMCmd.fmPoweronPara.historyVol = historyVol;
	msgPtr->msgBodyPtr.driverCmd.FMCmd.fmPoweronPara.isMute = 0;
	OSMQ_Put(OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
}

PUBLIC void DRIVER_FM_PowerOffCmd(void)
{
	DriverMsg XDATA_PTR msgPtr;
	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	msgPtr->msgOpcode = DRIVER_FM_POWEROFF_CMD;
	OSMQ_PutFront(OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
}

PUBLIC void DRIVER_FM_TuneCmd(BOOL isUp)
{
	DriverMsg XDATA_PTR msgPtr;
	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	msgPtr->msgOpcode = (isUp)?DRIVER_FM_TUNEUP_CMD:DRIVER_FM_TUNEDN_CMD;
	OSMQ_Put(OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
}

PUBLIC void DRIVER_FM_SeekCmd(BOOL isUp)
{
	DriverMsg XDATA_PTR msgPtr;
	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	msgPtr->msgOpcode = (isUp)?DRIVER_FM_HWSEEKUP_CMD:DRIVER_FM_HWSEEKDN_CMD;
	OSMQ_Put(OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
}

PUBLIC void DRIVER_FM_MuteCmd(U8 muteMode)
{
	DriverMsg XDATA_PTR msgPtr;
	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	if(muteMode == FM_MUTE_MODE_OFF)
		msgPtr->msgOpcode = DRIVER_FM_MUTE_OFF_CMD;
	else if(muteMode == FM_MUTE_MODE_ON)
		msgPtr->msgOpcode = DRIVER_FM_MUTE_ON_CMD;
	else
		msgPtr->msgOpcode = DRIVER_FM_MUTETOGGLE_CMD;

	OSMQ_Put(OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
}

PUBLIC void DRIVER_FM_VolumeCmd(U8 FMVolume)
{
	DriverMsg XDATA_PTR msgPtr;
	msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	msgPtr->msgOpcode = DRIVER_FM_VOLUME_CMD;
	msgPtr->msgBodyPtr.driverCmd.FMCmd.FMVolume = FMVolume;
	OSMQ_Put(OSMQ_FM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);
	LightDbgPrint("FM SoundLevel:%d", (U8)gMMI_FM_ctl.curFMSoundLevel);
}
#endif //FM_ENABLE
