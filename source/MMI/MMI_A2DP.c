#include "reside_flash.inc"

#define _MMI_A2DP_C_

#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_A2DP.h"
#include "MMI_HCI.h"
#include "MMI_AirModule.h"
#include "MMI_AVRCP.h"
#include "MMI_SDAP.h"
#include "MMI_HFP.h"
#include "MMI_Reconnect.h"
#include "MMI_LineIn.h"
#include "MMI_Timer.h"
#include "..\A2DP\A2DP_Interface.h"
#include "..\AVRCP\AVRCP_Interface.h"
#include "..\HC\HC_Interface.h"

#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];

#ifdef A2DP_Profile

PUBLIC void MMI_A2DP_SendCmd(U8 linkIndex, U8 cmdCode)
{
	A2DP_MMICommandHandler(linkIndex, cmdCode);
}

PRIVATE void MMI_A2DP_AcceptStart(U8 linkIndex)
{
	MMI_A2DP_SendCmd(linkIndex, A2DP_ACCEPT_START_RESP);
}

PRIVATE void MMI_A2DP_RejectStart(U8 linkIndex)
{
	MMI_A2DP_SendCmd(linkIndex, A2DP_REJECT_START_RESP);
}

PRIVATE void MMI_A2DP_MediaPktMemPut(U8 linkIndex)
{
	MMI_A2DP_SendCmd(linkIndex, A2DP_MEMPUT_START_RESP);
}

PRIVATE void MMI_A2DP_MusicStopClearPara(U8 linkIndex)
{
	#ifdef AVRCP_Profile
	gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus = 0;
	MMI_AVRCP_SetMusicPlay(linkIndex, FALSE);
	MMI_AVRCP_ResetKeyPlayToggle(linkIndex);
	#else
	UNUSED(linkIndex);
	#endif
}

PRIVATE void MMI_A2DP_MusicStopped(U8 linkIndex, BOOL isDisc)
{
#ifdef A2DP_SRC_Profile	
	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_MUSIC);
	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
	MMI_LineIn_DriverCmd(DRIVER_STOP_LINE_IN_CMD);
	UNUSED(isDisc);
#else	
	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
	if(!A2DP_IsResume(linkIndex) || isDisc)
	{
		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_MUSIC);
	}

	MMI_A2DP_MusicStopClearPara(linkIndex);
	#ifdef AVRCP_Profile
	MMI_AVRCP_CancelMission(linkIndex, MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC);
	MMI_AVRCP_CancelMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC);
	MMI_AVRCP_CancelMission(linkIndex, MMI_AVRCP_MISSION_MONITOR_PLAY_STATUS);
	#endif
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_MUSIC);
	#ifdef SUPPORT_MIC_IN_A2DP_MODE
	if(MMI_GetAudioTransparencyEnable())
	{
		MMI_LineIn_SendDriverVolCmd();
	}
	#endif
#endif
}

PRIVATE BOOL MMI_A2DP_GetAudioDevRegister(U8 linkIndex)
{
	BOOL audioDevReg = MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_MUSIC);

	#ifdef AVRCP_Profile
	if(AVRCP_PLAYBACK_STATUS_PLAYING == MMI_AVRCP_GetPlayStatus(linkIndex))
	{
		audioDevReg |= MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
	}
	#endif
	
	return audioDevReg;
}

PUBLIC void MMI_A2DP_EventHandler(U8 linkIndex, U8 eventCode)
{
	BOOL audioDevReg;

	switch(eventCode)
	{			
		case A2DP_CHANNEL_OPEN_COMPLETE_EVENT:
			#ifdef AIR_MODULE
			MMI_AIR_A2DPOpenNotify(linkIndex);
			#endif
			break;
		case A2DP_CHANNEL_START_REQ:
			#ifdef AVRCP_Profile
			MMI_AVRCP_SetMusicPlay(linkIndex, TRUE);
			MMI_AVRCP_ResetKeyPlayToggle(linkIndex);
			#endif

			audioDevReg = MMI_A2DP_GetAudioDevRegister(linkIndex);
			
			if(audioDevReg)
			{
				MMI_A2DP_AcceptStart(linkIndex);
				if(MMI_CheckLinkInCallNotifyState(linkIndex))
					MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
			}
			else if(MMI_CheckLinkInCallNotifyState(linkIndex) && MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY))
			{
				MMI_A2DP_AcceptStart(linkIndex);
			}
			else
			{
				if(MMI_DRV_IsActiveAudioDevInFM())
				{
					if(MMI_A2DP_MEMPUT_WHEN_FM_ON_FEAT)
					{
						MMI_A2DP_MediaPktMemPut(linkIndex);
					}
					else
					{
						MMI_A2DP_RejectStart(linkIndex);
					}
				}
				else if(MMI_DRV_IsActiveAudioDevInAUX())
				{
					if (MMI_A2DP_MEMPUT_WHEN_LINE_IN_FEAT)
					{
						MMI_A2DP_MediaPktMemPut(linkIndex);
					}
					else
					{
						MMI_A2DP_RejectStart(linkIndex);
					}
				}
				else if(MMI_DRV_IsActiveAudioDevInCall())
				{
					if(MMI_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT)
					{
						MMI_A2DP_MediaPktMemPut(linkIndex);
					}
					else
					{
						MMI_A2DP_RejectStart(linkIndex);
					}
				}
				else if(MMI_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT && MMI_DRV_IsActiveAudioDevInMusic())
				{
					MMI_A2DP_MediaPktMemPut(linkIndex);
				}
				else
				{
					MMI_A2DP_RejectStart(linkIndex);
				}
			}
			break;
			
		case A2DP_CHANNEL_START_RESP:
			#ifdef AVRCP_Profile
			MMI_AVRCP_SetMusicPlay(linkIndex, TRUE);
			MMI_AVRCP_ResetKeyPlayToggle(linkIndex);
			#endif
			#ifdef AIR_MODULE
			MMI_AIR_A2DPAcceptStartReqNotify(linkIndex);
			#endif
			MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
			
			audioDevReg = MMI_A2DP_GetAudioDevRegister(linkIndex);
			
			if(audioDevReg)
			{
				MMI_A2DP_SendDSPEnableCmd(linkIndex);
			}
			break;

		case A2DP_CHANNEL_START_RESP_FOR_INT:		
			#ifdef AVRCP_Profile
			MMI_AVRCP_SetMusicPlay(linkIndex, TRUE);
			MMI_AVRCP_ResetKeyPlayToggle(linkIndex);
			#endif

			audioDevReg = MMI_A2DP_GetAudioDevRegister(linkIndex);
			
			if(audioDevReg)
			{
				MMI_A2DP_SendCmd(linkIndex, A2DP_SET_AFTER_START_RESP);
				if(MMI_CheckLinkInCallNotifyState(linkIndex))
					MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
			}
			else if(MMI_CheckLinkInCallNotifyState(linkIndex) && MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY))
			{
				MMI_A2DP_SendCmd(linkIndex, A2DP_SET_AFTER_START_RESP);
			}			
			break;
			
		case A2DP_CHANNEL_SUSPEND_REQ:
			if(eventCode == A2DP_CHANNEL_SUSPEND_REQ)
				MMI_A2DP_SendCmd(linkIndex, A2DP_ACCEPT_SUSPEND_RESP);
			/* fallthrough *//*comment to prevent coverity warning*/
		case A2DP_CHANNEL_SUSPEND_EVT:
			MMI_A2DP_MusicStopped(linkIndex, FALSE);				
			break;
			
		case A2DP_CHANNEL_CLOSE_REQ:
			MMI_A2DP_SendCmd(linkIndex, A2DP_CLOSE_COMPLETE_RESP);
			MMI_ClearProfileInReconnectList(linkIndex, PROFILE_AVRCP);
			/* fallthrough *//*comment to prevent coverity warning*/
		case A2DP_CHANNEL_CLOSED_EVT:
		case A2DP_CHANNEL_MEDIA_CHANNEL_CLOSED_EVT:
			MMI_A2DP_MusicStopped(linkIndex, TRUE);		
			break;
			
		case A2DP_CHANNEL_FOLLOWER_AUX:
			gMMI_A2DP_ctl.auxGainReplaced = TRUE;
			break;
		case A2DP_CHANNEL_FOLLOWER_MUSIC:
			gMMI_A2DP_ctl.auxGainReplaced = FALSE;
			break;
		#ifdef A2DP_SRC_Profile	
		case A2DP_CHANNEL_SOURCE_START_EVT:
			if(MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_MUSIC))
			{
				MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_GUARANTEED, TRUE, FALSE);
				MMI_LineIn_DriverCmd(DRIVER_LINE_IN_CMD);
			}
			break;
		#endif
		
		case A2DP_CHANNEL_RECONNECT_AFTER_ABORT_EVT:
			MMI_A2DP_ConnectMediaChannelSource(linkIndex);
			break;
		
		default:
			break;			
	}
}

PUBLIC void MMI_A2DP_ConnectedHandle(U8 linkIndex)
{
	if(MMI_GetProfileDesc(PROFILE_AVRCP))
	{
		if(!MMI_IsProfileIdLoaded(linkIndex, PROFILE_AVRCP))
		{
			MMI_SetReconnectProfile(linkIndex, PROFILE_AVRCP, 3 * ONE_SEC);
		}
	}
}

PUBLIC BOOL MMI_A2DP_IsConnected(U8 linkIndex)
{
	return (A2DP_GetState(linkIndex) >= PROFILE_CONNECTED) ? TRUE : FALSE;
}

extern MmiCtlType XDATA gMMI_ctl;
PUBLIC void MMI_A2DP_SetDriverVolPara(U8 soundLevel, U8 smallStepLevel, DriverVolumeSetCmd XDATA_PTR volPtr)
{
	U8 spkGainSetIndex, micGainSetIndex;
	U8 deltaDigitalGainL, deltaAnalogGainL;
	BOOL isMute;
	
	volPtr->soundLevel = soundLevel;
	
	isMute = (soundLevel)? FALSE:TRUE;
	
	if(soundLevel)
		soundLevel--;

	if(gMMI_A2DP_ctl.auxGainReplaced)
	{
		spkGainSetIndex = GET_LINEIN_SPK_GAIN_BY_LEVEL(soundLevel);  
		micGainSetIndex = GET_LINEIN_MIC_GAIN_BY_LEVEL(soundLevel); 
		
		volPtr->spkVols = MMI_SPK_GAINSET[spkGainSetIndex].digitalGainL;
		volPtr->analogDaGain = MMI_SPK_GAINSET[spkGainSetIndex].analogGainL;
		volPtr->micVols = MMI_MIC_GAINSET[micGainSetIndex].digitalGainL;
		volPtr->analogAdGain_L = MMI_MIC_GAINSET[micGainSetIndex].analogGainL_L;
		volPtr->analogAdGain_R = MMI_MIC_GAINSET[micGainSetIndex].analogGainL_R;
		volPtr->smallStepLevel = smallStepLevel;
		if(smallStepLevel && !isMute)
		{
			deltaDigitalGainL = (U16)(MMI_SPK_GAINSET[GET_LINEIN_SPK_GAIN_BY_LEVEL(soundLevel+1)].digitalGainL - MMI_SPK_GAINSET[spkGainSetIndex].digitalGainL)*smallStepLevel/(gMMI_nvram.smallStepVolume.lineInVolumeSmallStep+1);
			deltaAnalogGainL = (U16)(MMI_SPK_GAINSET[GET_LINEIN_SPK_GAIN_BY_LEVEL(soundLevel+1)].analogGainL - MMI_SPK_GAINSET[spkGainSetIndex].analogGainL)*smallStepLevel/(gMMI_nvram.smallStepVolume.lineInVolumeSmallStep+1);
			volPtr->spkVols += deltaDigitalGainL;
			volPtr->analogDaGain += deltaAnalogGainL;
		}
		#ifdef LINEIN_ENABLE
		soundLevel++;
		MMI_LineIn_SetVolByLevel(soundLevel, smallStepLevel);
		LightDbgPrint("LineIn SoundLevel:%d - Aux", (U8) soundLevel);
		#endif
	}
	else
	{
		spkGainSetIndex = GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel);
		micGainSetIndex = GET_A2DP_MIC_GAIN_BY_LEVEL(soundLevel);
		
		volPtr->spkVols = MMI_SPK_GAINSET[spkGainSetIndex].digitalGainA;
		volPtr->analogDaGain = MMI_SPK_GAINSET[spkGainSetIndex].analogGainA;
		volPtr->micVols = MMI_MIC_GAINSET[micGainSetIndex].digitalGainA;
		volPtr->analogAdGain_L = MMI_MIC_GAINSET[micGainSetIndex].analogGainA_L;
		volPtr->analogAdGain_R = MMI_MIC_GAINSET[micGainSetIndex].analogGainA_R;
		volPtr->smallStepLevel = smallStepLevel;

		if(smallStepLevel && !isMute)
		{
			deltaDigitalGainL = (U16)(MMI_SPK_GAINSET[GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel+1)].digitalGainA - MMI_SPK_GAINSET[spkGainSetIndex].digitalGainA)*smallStepLevel/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
			deltaAnalogGainL = (U16)(MMI_SPK_GAINSET[GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel+1)].analogGainA - MMI_SPK_GAINSET[spkGainSetIndex].analogGainA)*smallStepLevel/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
			volPtr->spkVols += deltaDigitalGainL;
			volPtr->analogDaGain += deltaAnalogGainL;
		}
	}
}

PUBLIC void MMI_A2DP_SetDriverVolParaforMicCtlInA2DP(U8 soundLevel, U8 smallStepLevel, DriverVolumeSetCmd XDATA_PTR volPtr)
{
	U8 spkGainSetIndex, micGainSetIndex;
	U8 deltaDigitalGainL, deltaAnalogGainL;
	BOOL isMute;

	isMute = (soundLevel)?FALSE:TRUE;
	volPtr->soundLevel = soundLevel;
	if(soundLevel)
		soundLevel--;

	spkGainSetIndex = GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel);

	if (gMMI_ctl.isMicAdjsutInA2DP && !gMMI_ctl.isVolumeSetInitInA2DP)
	{
		U8 micCtlInA2DPSoundLevel = CURRENT_A2DP_SOUND_LEVEL(gMMI_ctl.linkIndexMicCtlInA2DP);
		
		volPtr->soundLevel = micCtlInA2DPSoundLevel;
		if(micCtlInA2DPSoundLevel)
			micCtlInA2DPSoundLevel--;
		
		spkGainSetIndex = GET_A2DP_SPK_GAIN_BY_LEVEL(micCtlInA2DPSoundLevel);

		volPtr->spkVols =	 MMI_SPK_GAINSET[spkGainSetIndex].digitalGainA;//VOLUME_DONT_CARE;
		volPtr->analogDaGain = 	MMI_SPK_GAINSET[spkGainSetIndex].analogGainA;//VOLUME_DONT_CARE;
		volPtr->micVols = 		gMMI_nvram.micGainSetA2DP[soundLevel].digitalGainA2DP;
		volPtr->analogAdGain_L = 	gMMI_nvram.micGainSetA2DP[soundLevel].analogGainA2DP_L;
		volPtr->analogAdGain_R =	gMMI_nvram.micGainSetA2DP[soundLevel].analogGainA2DP_R;
		gMMI_ctl.isMicAdjsutInA2DP = FALSE;	
	}
	else if (!gMMI_ctl.isMicAdjsutInA2DP && !gMMI_ctl.isVolumeSetInitInA2DP)
	{
		spkGainSetIndex = GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel);
		micGainSetIndex = CURRENT_MIC_SOUND_LEVEL_IN_A2DP(gMMI_ctl.linkIndexMicCtlInA2DP);

		volPtr->spkVols =   MMI_SPK_GAINSET[spkGainSetIndex].digitalGainA;
		volPtr->analogDaGain = MMI_SPK_GAINSET[spkGainSetIndex].analogGainA;
		volPtr->micVols = 		gMMI_nvram.micGainSetA2DP[micGainSetIndex].digitalGainA2DP;
		volPtr->analogAdGain_L = 	gMMI_nvram.micGainSetA2DP[micGainSetIndex].analogGainA2DP_L;
		volPtr->analogAdGain_R =	gMMI_nvram.micGainSetA2DP[micGainSetIndex].analogGainA2DP_R;
	}
	else if (gMMI_ctl.isVolumeSetInitInA2DP)
	{
		spkGainSetIndex = GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel);
		micGainSetIndex = GET_A2DP_MIC_GAIN_BY_LEVEL(soundLevel);
		volPtr->spkVols =   MMI_SPK_GAINSET[spkGainSetIndex].digitalGainA;
		volPtr->analogDaGain = MMI_SPK_GAINSET[spkGainSetIndex].analogGainA;
		volPtr->micVols =  MMI_MIC_GAINSET[micGainSetIndex].digitalGainA;
		volPtr->analogAdGain_L = MMI_MIC_GAINSET[micGainSetIndex].analogGainA_L;
		volPtr->analogAdGain_R = MMI_MIC_GAINSET[micGainSetIndex].analogGainA_R;
		gMMI_ctl.isVolumeSetInitInA2DP = FALSE;
	}
	
	volPtr->smallStepLevel = smallStepLevel;

	if(smallStepLevel && !isMute)
	{
		deltaDigitalGainL = (U16)(MMI_SPK_GAINSET[GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel+1)].digitalGainA - MMI_SPK_GAINSET[spkGainSetIndex].digitalGainA)*smallStepLevel/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
		deltaAnalogGainL = (U16)(MMI_SPK_GAINSET[GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel+1)].analogGainA - MMI_SPK_GAINSET[spkGainSetIndex].analogGainA)*smallStepLevel/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
		volPtr->spkVols += deltaDigitalGainL;
		volPtr->analogDaGain += deltaAnalogGainL;
	}
}


PUBLIC void MMI_A2DP_SendDriverVolCmd(U8 soundLevel, U8 smallStepLevel)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_A2DP_VOLUME_SET_CMD);
	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		if (gMMI_ctl.isMicEnableInA2DP)
			MMI_A2DP_SetDriverVolParaforMicCtlInA2DP(soundLevel, smallStepLevel, &msgPtr->msgBodyPtr.driverCmd.a2dpVolSetCmd);
		else
			MMI_A2DP_SetDriverVolPara(soundLevel, smallStepLevel, &msgPtr->msgBodyPtr.driverCmd.a2dpVolSetCmd);

		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);

		LightDbgPrint("A2DP SoundLevel:%d, %d", (U8)soundLevel, (U8)smallStepLevel);
	}
}

PUBLIC void MMI_A2DP_VolumeControl(U8 linkIndex, BOOL isVolumeUp, BOOL isFollowerCmd)
{
	U8 XDATA_PTR spkSoundLevelPtr;

	spkSoundLevelPtr =  &CURRENT_A2DP_SOUND_LEVEL(linkIndex);
	gMMI_ctl.linkIndexMicCtlInA2DP = linkIndex;
	if (isVolumeUp)
	{
		CURRENT_A2DP_SMALL_STEP(linkIndex) = MMI_DRV_IncreaseSmallStepVolume(	CURRENT_A2DP_SOUND_LEVEL(linkIndex), 
																						gMMI_nvram.totalMusicSoundLevel,
																						gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep,
																						CURRENT_A2DP_SMALL_STEP(linkIndex));

		if(CURRENT_A2DP_SMALL_STEP(linkIndex) == 0)
		{
			if (*spkSoundLevelPtr < MMI_MAX_MUSIC_SOUND_LEVEL())
			{
				(*spkSoundLevelPtr)++;
			}
			else if(MMI_A2DP_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				*spkSoundLevelPtr = 0;
			}
			else
			{
				LightDbgPrint("A2DP volume reach max volume");
				MMI_PushMediaEvent(MEDIA_EVT_A2DP_VOLUME_REACH_MAX);
				return;
			}
		}
	}
	else
	{
		if(CURRENT_A2DP_SMALL_STEP(linkIndex) == 0)
		{
			if (*spkSoundLevelPtr > 0)
			{
				(*spkSoundLevelPtr)--;
			}
			else if(MMI_A2DP_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				*spkSoundLevelPtr = MMI_MAX_MUSIC_SOUND_LEVEL();
				goto MMI_SEND_A2DP_VOL;
			}
			else
			{
				LightDbgPrint("A2DP volume reach min volume");
				MMI_PushMediaEvent(MEDIA_EVT_A2DP_VOLUME_REACH_MIN);
				return;
			}
		}
		CURRENT_A2DP_SMALL_STEP(linkIndex) = MMI_DRV_DecreaseSmallStepVolume(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep, CURRENT_A2DP_SMALL_STEP(linkIndex));
	}
	MMI_SEND_A2DP_VOL:
	
	MMI_A2DP_SetVolByLevel(linkIndex, CURRENT_A2DP_SOUND_LEVEL(linkIndex), CURRENT_A2DP_SMALL_STEP(linkIndex));
	#ifdef AVRCP_Profile
	LightDbgPrint("MMI-SendAVRCPVolChanged[%d]:%d",(U8)linkIndex, (U8)MMI_AVRCP_GetAbsVolByMusic(linkIndex));
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_VOLUME_CHANGED, MMI_AVRCP_GetAbsVolByMusic(linkIndex));
	#endif
	
	if(linkIndex == MMI_DRV_GetAudioInMusicLink() 
	#ifdef AVRCP_Profile
		&& MMI_AVRCP_IsMusicPlaying(linkIndex)
	#endif
	)
	{
		#ifdef AIR_MODULE
		MMI_AIR_A2DPKeyVolUpDn(linkIndex, isVolumeUp);
		#endif
		MMI_A2DP_SendDriverVolCmd(*spkSoundLevelPtr, CURRENT_A2DP_SMALL_STEP(linkIndex));
		if(!isFollowerCmd)
			MMI_DRV_Beep(*spkSoundLevelPtr, AUDIO_DEVICE_MUSIC);
	}
			
	if (MMI_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT)
	{
		MMI_LinkKey_SaveHistoryInRunTime(linkIndex);
	}
}

PRIVATE void MMI_A2DP_RemoveAudioLink(U8 linkIndex)
{
	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_MUSIC);
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_MUSIC);
}

PUBLIC void MMI_A2DP_ClearCtlData(U8 linkIndex)
{
	MMI_ClearProfileInReconnectList(linkIndex, PROFILE_AVRCP);
	MMI_A2DP_RemoveAudioLink(linkIndex);
	MMI_A2DP_MusicStopClearPara(linkIndex);
	MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_FOLLOWER_MUSIC);
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_A2DP_ctl.A2dpCtl[linkIndex], 0x00, sizeof(MmiA2dpCtlType));
}

PUBLIC void MMI_A2DP_ClearResumeState(U8 linkInd)
{
	A2DP_SetResume(linkInd, FALSE);
	MMI_DRV_AudioDeviceDeRegister(linkInd, AUDIO_DEVICE_MUSIC);
	MMI_DRV_AudioDeviceDeRegister(linkInd, AUDIO_DEVICE_AVRCP_PLAY);
}

#ifdef AVRCP_Profile
PRIVATE void MMI_A2DP_MonitorPlayStatus(U8 linkIndex, U8 isSuccessful)
{
	if(isSuccessful)
	{
		MMI_AVRCP_MISSION_PARAM_TYPE LDATA avrcpParam;
		avrcpParam.taskId = MMI_AVRCP_MISSION_MONITOR_PLAY_STATUS;
		avrcpParam.timer = HALF_SEC;
		avrcpParam.fcallback = MMI_A2DP_MonitorPlayStatus;
		MMI_AVRCP_PushMission(linkIndex, &avrcpParam);
	}
}
#endif

PRIVATE void MMI_A2DP_SendPause(U8 linkIndex, U8 playStatus)
{
	MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
	
	#ifdef AVRCP_Profile
	if(MMI_AVRCP_IsConnected(linkIndex))
	{
		MMI_AVRCP_MISSION_PARAM_TYPE LDATA avrcpParam;
		switch(playStatus)
		{
			case AVRCP_PLAYBACK_STATUS_PLAYING:
				MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SEND_OPERATION,AVC_OPERATION_PAUSE);
				
				//A2DP_SetResume(linkIndex, TRUE);
				if(!gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isDisablePlayStatusMonitor)
				{
					avrcpParam.taskId = MMI_AVRCP_MISSION_MONITOR_PLAY_STATUS;
					avrcpParam.timer = 2*320L;
					avrcpParam.fcallback = MMI_A2DP_MonitorPlayStatus;
					MMI_AVRCP_PushMission(linkIndex, &avrcpParam);
					gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus = AVRCP_PLAYBACK_STATUS_PAUSED;
				}
				break;
			case AVRCP_PLAYBACK_STATUS_STOPPED:
				MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SEND_OPERATION,AVC_OPERATION_STOP);
				/* fallthrough *//*comment to prevent coverity warning*/
			case AVRCP_PLAYBACK_STATUS_PAUSED:
				break;
			case AVRCP_PLAYBACK_STATUS_FWD_SEEK:
			case AVRCP_PLAYBACK_STATUS_REV_SEEK:
				break;

			default:
				MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SEND_OPERATION,AVC_OPERATION_STOP);
				break;

		}
	}
	else
	#else
	UNUSED(playStatus);
	#endif
	{
		MMI_A2DP_SendCmd(linkIndex, A2DP_PAUSE_CMD);
	}

	// Reset the resume record in other link in case of confusion.
#ifdef SUPPORT_MULTI_POINT	
	if(A2DP_IsResume(linkIndex) && (A2DP_STREAMING == A2DP_GetState(linkIndex)))
	{
		U8 nextActiveLink;
		for(nextActiveLink = 0; nextActiveLink < MAX_MULTI_POINT_NO; nextActiveLink++)
		{
			if(nextActiveLink == linkIndex || !MMI_IsActiveLink(nextActiveLink))
				continue;

			MMI_A2DP_ClearResumeState(nextActiveLink);
		}
	}
#endif	
}

#ifdef AVRCP_Profile
PRIVATE void MMI_A2DP_GetPauseMusicCallBack(U8 linkIndex, U8 isSuccessful)
{
	UNUSED(isSuccessful);
	if(!MMI_CheckLinkInCallActiveState(linkIndex))
	{
		MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_STOPPED);
	}
}

PRIVATE void MMI_A2DP_GetPlayStatusToPauseMusicCallBack(U8 linkIndex, U8 isSuccessful)
{
	#ifdef AVRCP_Profile
	if(isSuccessful)
	{
		MMI_AVRCP_MISSION_PARAM_TYPE LDATA avrcpParam;
		avrcpParam.taskId = MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC;
		avrcpParam.timer = ONE_SEC;
		avrcpParam.fcallback = MMI_A2DP_GetPauseMusicCallBack;
		MMI_AVRCP_PushMission(linkIndex, &avrcpParam);
	}
	else
	#else
	UNUSED(isSuccessful);
	#endif
	{
		if(!MMI_CheckLinkInCallActiveState(linkIndex))
		{
			MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_STOPPED);
		}
	}
}

PUBLIC void MMI_A2DP_CheckPlayStatusConsistency(U8 linkIndex, U8 playStatus, U8 needDelay)
{
	if (playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
	{
		if (A2DP_OPEN == A2DP_GetState(linkIndex))
		// play status is playing but no AVDTP_START received
		{
			LightDbgPrint("check streaming state later");
			MMI_AVRCP_CheckStreamingStateLater(linkIndex, playStatus, needDelay);
		}
	}
}

PUBLIC void MMI_A2DP_PlayStatusChanged(U8 linkIndex, U8 playStatus)
{
	#ifdef AVRCP_Profile
	switch(playStatus)
	{
		case AVRCP_PLAYBACK_STATUS_PLAYING:
			if(MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC) || MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC))
			{
				if(!MMI_CheckLinkInCallActiveState(linkIndex))
				{
					MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_PLAYING);
				}
			}
			else if(A2DP_STREAMING == A2DP_GetState(linkIndex))
			{
				BOOL audioDevReg = MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
				if(MMI_AllLinksInCallRelatedState())
				{
					if(!MMI_CheckLinkInCallActiveState(linkIndex) && !MMI_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT)
					{
						A2DP_SetResume(linkIndex, TRUE);
						MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_PLAYING);
					}
				}
				else if(audioDevReg)
				{
					MMI_A2DP_SendDSPEnableCmd(linkIndex);
				}			
			}
			else
			{
				MMI_A2DP_CheckPlayStatusConsistency(linkIndex, playStatus, FALSE);
			}
			break;
			
		case AVRCP_PLAYBACK_STATUS_STOPPED:
		case AVRCP_PLAYBACK_STATUS_PAUSED:
			MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
			if(MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC) || MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC))
			{
				MMI_A2DP_StopDSP(linkIndex);
			}
			else
			{
				MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_AVRCP_PLAY);
			}
			A2DP_MMICommandHandler(linkIndex, A2DP_MUSIC_PAUSED_BY_AVRCP_PLAY_STATUS);
			break;
		case AVRCP_PLAYBACK_STATUS_FWD_SEEK:
		case AVRCP_PLAYBACK_STATUS_REV_SEEK:	
			if(MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC))
			{
				MMI_AVRCP_MISSION_PARAM_TYPE LDATA avrcpParam;
				avrcpParam.taskId = MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC;
				avrcpParam.timer = HALF_SEC;
				avrcpParam.fcallback = MMI_A2DP_GetPlayStatusToPauseMusicCallBack;
				MMI_AVRCP_PushMission(linkIndex, &avrcpParam);			
			}
			break;
		case AVRCP_PLAYBACK_STATUS_REJECT:
			if(MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC))
			{
				if(!MMI_CheckLinkInCallActiveState(linkIndex))
				{
					MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_PLAYING);
				}
			}		
			break;
	}
	#else
	UNUSED(linkIndex);
	UNUSED(playStatus);
	#endif
}

PUBLIC void MMI_A2DP_PlayStatusTheSame(U8 linkIndex, U8 playStatus)
{
	#ifdef AVRCP_Profile
	switch(playStatus)
	{
		case AVRCP_PLAYBACK_STATUS_PLAYING:
			if(MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC) || MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC))
			{
				if(!MMI_CheckLinkInCallActiveState(linkIndex))
				{
					MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_PLAYING);
				}
			}
			else if(A2DP_STREAMING == A2DP_GetState(linkIndex))
			{
				if(MMI_AllLinksInCallRelatedState())
				{
					if(!MMI_CheckLinkInCallActiveState(linkIndex) && !MMI_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT)
					{
						A2DP_SetResume(linkIndex, TRUE);
						MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_PLAYING);
					}
				}			
			}
			break;
			
		case AVRCP_PLAYBACK_STATUS_STOPPED:
			if(MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC))
			{
				if(!MMI_CheckLinkInCallActiveState(linkIndex))
				{
					MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_STOPPED);
				}			
			}
			/* fallthrough */
		case AVRCP_PLAYBACK_STATUS_PAUSED:
			MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC);
			//MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC);
			break;
		case AVRCP_PLAYBACK_STATUS_FWD_SEEK:
		case AVRCP_PLAYBACK_STATUS_REV_SEEK:	
			if(MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC))
			{
				MMI_AVRCP_MISSION_PARAM_TYPE LDATA avrcpParam;
				avrcpParam.taskId = MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC;
				avrcpParam.timer = HALF_SEC;
				avrcpParam.fcallback = MMI_A2DP_GetPlayStatusToPauseMusicCallBack;
				MMI_AVRCP_PushMission(linkIndex, &avrcpParam);			
			}
			break;
		case AVRCP_PLAYBACK_STATUS_REJECT:
			if(MMI_AVRCP_CheckMission(linkIndex, MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC))
			{
				if(!MMI_CheckLinkInCallActiveState(linkIndex))
				{
					MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_PLAYING);
				}
			}		
			break;
	}
	#else
	UNUSED(linkIndex);
	UNUSED(playStatus);
	#endif
}
#endif

PUBLIC void MMI_A2DP_SendPauseMusic(U8 linkIndex, U8 isCall)
{
	if(A2DP_STREAMING == A2DP_GetState(linkIndex) && !A2DP_IsResume(linkIndex))
	{
		MMI_A2DP_StopDSP(linkIndex);

		if (isCall && MMI_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT)
		{

		}
		#ifdef AVRCP_Profile
		else if(MMI_AVRCP_IsConnected(linkIndex) || MMI_IsProfileInReconnectList(linkIndex, PROFILE_AVRCP) || MMI_IsProfileIdLoaded(linkIndex, PROFILE_AVRCP)) 
		{
			MMI_AVRCP_MISSION_PARAM_TYPE LDATA avrcpParam;
			
			if(gMMI_ctl.audioDevCtl.currentActiveLinkIndex == linkIndex)
			{
				if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isDisablePlayStatusMonitor)
				{
					avrcpParam.taskId = MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC;
					avrcpParam.timer = 2 * ONE_SEC;
					avrcpParam.fcallback = MMI_A2DP_GetPauseMusicCallBack;
					MMI_AVRCP_PushMission(linkIndex, &avrcpParam);
					
					avrcpParam.taskId = MMI_AVRCP_MISSION_QUERY_CURRENT_PLAY_STATUS;
					avrcpParam.timer = 0;
					avrcpParam.fcallback = NULL;
					MMI_AVRCP_PushMission(linkIndex, &avrcpParam);
					
				}
				else
				{
					avrcpParam.taskId = MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC;
					avrcpParam.timer = 0;
					avrcpParam.fcallback = MMI_A2DP_GetPlayStatusToPauseMusicCallBack;
					MMI_AVRCP_PushMission(linkIndex, &avrcpParam);				
				}
			}
			else
			{
				// Wait for AVRCP state transition
				// Just got A2DP connected. waiting for AVRCP
				avrcpParam.taskId = MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC;
				avrcpParam.timer = HALF_SEC;
				avrcpParam.fcallback = MMI_A2DP_GetPlayStatusToPauseMusicCallBack;
				MMI_AVRCP_PushMission(linkIndex, &avrcpParam);
			}
		}
		#endif
		else
		{
			MMI_A2DP_SendPause(linkIndex, AVRCP_PLAYBACK_STATUS_NONE);
		}
	}
}

PUBLIC BOOL MMI_A2DP_EncodedDataHandler(U8 XDATA_PTR ptr1)
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		#ifdef SUPPORT_SBC_SRC
		if(MMI_A2DP_IsInStreaming(linkIndex) && A2DP_IsMediaDataPlay(linkIndex))
		{
			return A2DP_SendEncodedMediaPacket(linkIndex, ptr1);
		}
		#endif
		#ifdef AIR_MODULE
		if(MMI_A2DP_IsInStreaming(linkIndex) && A2DP_IsLinkRelayer(linkIndex))
		{
			return A2DP_SendEncodedMediaPacket(linkIndex, ptr1);
		}
		#endif
		UNUSED(ptr1);
	}
	return FALSE;
}

PUBLIC void MMI_A2DP_ConnectMediaChannelSource(U8 linkIndex)
{
	MMI_A2DP_SendCmd(linkIndex, A2DP_CONNECT_MEDIA_CMD_SOURCE);
}

PUBLIC void MMI_A2DP_HighQualityCodecHandle(U8 settings)
{
	gMMI_A2DP_ctl.isHQCodecDisabled = (settings & UART_CMD_HQ_CODEC_ENABLED) ? FALSE : TRUE;
}

PUBLIC BOOL MMI_A2DP_IsHighQualityCodecDisbaled(void)
{
	return gMMI_A2DP_ctl.isHQCodecDisabled;
}

PUBLIC BOOL MMI_A2DP_IsInStreaming(U8 linkIndex)
{
	return (A2DP_STREAMING == A2DP_GetState(linkIndex) && !A2DP_IsResume(linkIndex)) ? TRUE : FALSE;
}

PUBLIC void MMI_A2DP_StopDSP(U8 linkIndex)
{
	MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
	MMI_SendDSPCmd(linkIndex, DSP_DISABLED);
}

PUBLIC void MMI_A2DP_SendResume(U8 linkIndex)
{
	#ifdef AVRCP_Profile
	if(MMI_AVRCP_IsConnected(linkIndex))
	{
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION,AVC_OPERATION_PLAY);
		MMI_A2DP_ClearResumeState(linkIndex);
	}
	else 
	#endif
	{
		MMI_A2DP_SendCmd(linkIndex, A2DP_PLAY_CMD);
	}
}

PUBLIC BOOL MMI_A2DP_IsAudioOn(U8 linkIndex)
{
	return (gMMI_ctl.audioableA2DPLink == linkIndex)? TRUE : FALSE;
}

PUBLIC void MMI_A2DP_NotifyDSPStatus(BOOL isEnable, U8 linkIndex)
{
	if(isEnable)
	{
		//A2DP DSP Enabled
		DriverVolumeSetCmd LDATA volPara;
		if(linkIndex < MAX_MULTI_POINT_NO)
		{
			MMI_A2DP_SetDriverVolPara(CURRENT_A2DP_SOUND_LEVEL(linkIndex), CURRENT_A2DP_SMALL_STEP(linkIndex), &volPara);
			LightDbgPrint("SendA2DPVol[%d]:%d,%d",(U8)linkIndex, (U8)CURRENT_A2DP_SOUND_LEVEL(linkIndex), (U8)CURRENT_A2DP_SMALL_STEP(linkIndex));
			DRIVER_SetA2DPVolPara(&volPara);
			MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_SET_CHANNEL_SEL_CMD);
			MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_PLAY_CMD);
			MMI_PushMediaEvent(MEDIA_EVT_A2DP_ENABLE);
		}
		#ifdef DEMOSOUND
		else
		{
			MMI_A2DP_SetDriverVolPara(gMMI_A2DP_ctl.demoSoundCtl.soundLevel, gMMI_A2DP_ctl.demoSoundCtl.smallStep, &volPara);
			LightDbgPrint("SendA2DPVol:%d,%d",(U8)gMMI_A2DP_ctl.demoSoundCtl.soundLevel, (U8)gMMI_A2DP_ctl.demoSoundCtl.smallStep);
			DRIVER_SetA2DPVolPara(&volPara);
		}
		#endif
		#ifdef AIR_MODULE
		MMI_AIR_A2DPDSPEnabledNotify(linkIndex);
		#endif

		gMMI_ctl.audioableA2DPLink = linkIndex;
	}
	else
	{
		//A2DP DSP Disabled
		gMMI_ctl.audioableA2DPLink = MMI_EOF;
		MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
		MMI_PushMediaEvent(MEDIA_EVT_A2DP_DISABLE);
		MMI_A2DP_SendCmd(linkIndex, A2DP_DSP_CLOSED_CLEAN_MEMORY_CMD);
		#ifdef AIR_MODULE
		MMI_AIR_A2DPDSPDisabledNotify(linkIndex);
		#endif
	}
	
	if(api_profile_func[API_GRP_PF_CB_A2DP_AUDIO_STATUS] && linkIndex < MAX_MULTI_POINT_NO)
	{
		((API_PROFILE_A2DP_AUDIO_STATUS_FUNC)api_profile_func[API_GRP_PF_CB_A2DP_AUDIO_STATUS])(linkIndex, isEnable);
	}
}

PUBLIC void MMI_A2DP_SendDSPA2DPEnableRelatedCmd(U8 linkIndex, U8 opCode)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(opCode);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		DRIVER_ClearStopDSPCmd(linkIndex);

		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.codec = A2DP_GetCodec(linkIndex);
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.linkInd = linkIndex;
		
		#ifdef AIR_MODULE
		if(MMI_AIR_IsTWSApplication())
		{
			if(A2DP_IsLinkFollower(linkIndex))
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.twsMode = A2DP_TWS_MODE_FOLLOWER;
			else
			{
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.twsMode = A2DP_TWS_MODE_RELAYER;
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.sbc_target_blocks_offset = gMMI_nvram.tws_para.tws_sbc_target_blocks_offset;
			}
		}
		else
		{
			msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.twsMode = A2DP_TWS_MODE_NONE;
		}
		MMI_AIR_DriverSendA2DPDSPEnableCmd(linkIndex);
		MMI_AIR_AVRCPSetDriverVol(linkIndex);

		if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER)== PRODUCT_AIR_SOUNDBAR)
		{
			msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_RELAY = SOUNDBAR_MIX_TWO_CHANNEL;
			msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_EXTERNAL_MCU = gMMI_nvram.airPara.relayScoSource;
			switch(gMMI_nvram.airPara.A2DP_FS_SEL)
			{
				case PCM_FS_SEL_2KHZ:
					msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_24SLOTS
					msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 4000L; //HCIPKTYPE_AIROHA_PKT_24SLOTS
					break;
				case PCM_FS_SEL_8KHZ:
					msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_6SLOTS
					msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 16000L; //HCIPKTYPE_AIROHA_PKT_6SLOTS
					break;
				case PCM_FS_SEL_4KHZ:
					msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_6SLOTS
					msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 16000L; //HCIPKTYPE_AIROHA_PKT_6SLOTS
				default:
					ASSERT(FALSE);
					break;
			}
		}
		#endif
		
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.sidetone_en = (gMMI_ctl.isMicEnableInA2DP)? 1: 0;

		LightDbgPrint("sidetone:[%d]",msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.sidetone_en);

		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);

		// To accelerate the speed of open DSP
		SYS_InsertOSTask(DRIVER_TASK);
		SYS_InsertOSTask(MMI_TASK);
		
		if(gMMI_ctl.audioableA2DPLink != MMI_EOF && gMMI_ctl.audioableA2DPLink != linkIndex)
		{
			MMI_HCI_SendVCmdAudioQosSetup(gMMI_ctl.audioableA2DPLink, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
		}
		if(gMMI_ctl.audioableSCOLink != MMI_EOF && gMMI_ctl.audioableSCOLink != linkIndex)
		{
			MMI_HCI_SendVCmdAudioQosSetup(gMMI_ctl.audioableSCOLink, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
		}
		if (A2DP_IsLinkFollower(linkIndex))
		{
			MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_GUARANTEED, TRUE, TRUE);
		}
		else
		{
			MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_GUARANTEED, TRUE, FALSE);
		}

		#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("USR - [%d]Send A2DPMusicEnable:%x",(U8)linkIndex, (U8)opCode);
		#endif
	}

}

PUBLIC void MMI_A2DP_SendDSPEnableCmd(U8 linkIndex)
{
	MMI_A2DP_SendDSPA2DPEnableRelatedCmd(linkIndex, DRIVER_A2DP_CMD);
}

PUBLIC void MMI_A2DP_SetVolByLevel(U8 linkIndex, U8 soundLevel, U8 smallStep)
{
	CURRENT_A2DP_SOUND_LEVEL(linkIndex) = soundLevel;
	CURRENT_A2DP_SMALL_STEP(linkIndex) = smallStep;
	#ifdef AVRCP_Profile
	CURRENT_AVRCP_VOLUME(linkIndex) = MMI_AVRCP_SoundLevelToAbsoluteVol(CURRENT_A2DP_SOUND_LEVEL(linkIndex), (U8)MMI_MAX_MUSIC_SOUND_LEVEL(), CURRENT_A2DP_SMALL_STEP(linkIndex), MMI_A2DP_SMALL_STEP_LEVELS());
	#endif
}

#ifdef A2DP_SRC_Profile
PUBLIC void MMI_A2DP_LineInEnableNotify(void)
{
	if(MMI_A2DP_IsInStreaming(gMMI_ctl.audioDevCtl.currentActiveLinkIndex))
		MMI_A2DP_SendCmd(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, A2DP_MUSIC_DATA_PLAY_CMD);
}
#endif

#ifdef DEMOSOUND
PUBLIC void MMI_A2DP_DemoSoundSetCmd(MMIMsgType XDATA_PTR msgPtr)
{
	#ifdef AIR_MODULE
	if(MMI_AIR_IsTWSApplication())
	{
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.twsMode = A2DP_TWS_MODE_RELAYER;
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.sbc_target_blocks_offset = gMMI_nvram.tws_para.tws_sbc_target_blocks_offset;
	}
	else
	{
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.enablePara.a2dpPara.twsMode = A2DP_TWS_MODE_NONE;
	}

	if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER)== PRODUCT_AIR_SOUNDBAR)
	{
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_RELAY = SOUNDBAR_MIX_TWO_CHANNEL;
		msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_EXTERNAL_MCU = gMMI_nvram.airPara.relayScoSource;
		switch(gMMI_nvram.airPara.A2DP_FS_SEL)
		{
			case PCM_FS_SEL_2KHZ:
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_24SLOTS
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 4000L; //HCIPKTYPE_AIROHA_PKT_24SLOTS
				break;
			case PCM_FS_SEL_8KHZ:
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_6SLOTS
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 16000L; //HCIPKTYPE_AIROHA_PKT_6SLOTS
				break;
			case PCM_FS_SEL_4KHZ:
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_6SLOTS
				msgPtr->msgBodyPtr.driverCmd.a2dpCmd.m2d_ctl.para.stereo_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 16000L; //HCIPKTYPE_AIROHA_PKT_6SLOTS
			default:
				ASSERT(FALSE);
				break;
		}
	}
	#else
	UNUSED(msgPtr);
	#endif
}

PUBLIC void MMI_A2DP_DemoSoundPlayCmd(void)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_DEMOSOUND_PLAY_CMD);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{		
		MMI_A2DP_DemoSoundSetCmd(msgPtr);

		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);

		// To accelerate the speed of open DSP
		SYS_InsertOSTask(DRIVER_TASK);
		SYS_InsertOSTask(MMI_TASK);
		/*
		if(gMMI_ctl.audioableA2DPLink != MMI_EOF && gMMI_ctl.audioableA2DPLink != linkIndex)
		{
			MMI_HCI_SendVCmdAudioQosSetup(gMMI_ctl.audioableA2DPLink, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
		}
		if(gMMI_ctl.audioableSCOLink != MMI_EOF && gMMI_ctl.audioableSCOLink != linkIndex)
		{
			MMI_HCI_SendVCmdAudioQosSetup(gMMI_ctl.audioableSCOLink, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
		}
		if (A2DP_IsLinkFollower(linkIndex))
		{
			MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_GUARANTEED, TRUE, TRUE);
		}
		else
		{
			MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_GUARANTEED, TRUE, FALSE);
		}
		*/
		LightDbgPrint("MMI Send DemoSound Play");
	}
}

PRIVATE void MMI_A2DP_DemoSoundPauseCmd(void)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_DEMOSOUND_PAUSE_CMD);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{		
		MMI_A2DP_DemoSoundSetCmd(msgPtr);

		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		LightDbgPrint("MMI Send DemoSound Pause");
	}
}

PUBLIC void MMI_A2DP_DemoSoundStopCmd(void)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_DEMOSOUND_STOP_CMD);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{		
		MMI_A2DP_DemoSoundSetCmd(msgPtr);

		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		LightDbgPrint("MMI Send DemoSound Stop");
	}
}

PRIVATE void MMI_A2DP_DemoSoundForwardCmd(void)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_DEMOSOUND_FORWARD_CMD);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{		
		MMI_A2DP_DemoSoundSetCmd(msgPtr);

		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		LightDbgPrint("MMI Send DemoSound Forward");
	}
}

PRIVATE void MMI_A2DP_DemoSoundBackwardCmd(void)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_DEMOSOUND_BACKWARD_CMD);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{		
		MMI_A2DP_DemoSoundSetCmd(msgPtr);

		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		LightDbgPrint("MMI Send DemoSound Backward");
	}
}

PUBLIC void MMI_A2DP_KeyDemoSoundPlay(void)
{
	if(!DRIVER_IsDemoSoundReady())
		return;
	
	if(!MMI_DRV_AudioDeviceRegister(MMI_EOF, AUDIO_DEVICE_DEMO_SOUND))
		return;
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffTimer);
	MMI_A2DP_DemoSoundPlayCmd();
}

PUBLIC void MMI_A2DP_KeyDemoSoundPause(void)
{
	if(!gMMI_ctl.activeLinkRecord)
		MMI_TIMER_ResetAutoSwitchOffTimer();
	MMI_A2DP_DemoSoundPauseCmd();
	MMI_DRV_AudioDeviceDeRegister(MMI_EOF, AUDIO_DEVICE_DEMO_SOUND);
	MMI_DRV_ResumeOtherAudioDevice(MMI_EOF, AUDIO_DEVICE_DEMO_SOUND);
}

PUBLIC void MMI_A2DP_KeyDemoSoundStop(void)
{
	if(!gMMI_ctl.activeLinkRecord)
		MMI_TIMER_ResetAutoSwitchOffTimer();
	MMI_A2DP_DemoSoundStopCmd();
	MMI_DRV_AudioDeviceDeRegister(MMI_EOF, AUDIO_DEVICE_DEMO_SOUND);
	MMI_DRV_ResumeOtherAudioDevice(MMI_EOF, AUDIO_DEVICE_DEMO_SOUND);
}

PUBLIC void MMI_A2DP_KeyDemoSoundForward(void)
{
	MMI_A2DP_DemoSoundForwardCmd();
}

PUBLIC void MMI_A2DP_KeyDemoSoundBackward(void)
{
	MMI_A2DP_DemoSoundBackwardCmd();
}

PUBLIC void MMI_A2DP_DemoSoundVolumeControl(U8 isVolumeUp)
{
	U8 XDATA_PTR spkSoundLevelPtr;

	spkSoundLevelPtr =  &gMMI_A2DP_ctl.demoSoundCtl.soundLevel;

	if (isVolumeUp)
	{
		gMMI_A2DP_ctl.demoSoundCtl.smallStep = MMI_DRV_IncreaseSmallStepVolume(	gMMI_A2DP_ctl.demoSoundCtl.soundLevel, 
																				gMMI_nvram.totalMusicSoundLevel,
																				gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep,
																				gMMI_A2DP_ctl.demoSoundCtl.smallStep);

		if(gMMI_A2DP_ctl.demoSoundCtl.smallStep == 0)
		{
			if (*spkSoundLevelPtr < MMI_MAX_MUSIC_SOUND_LEVEL())
			{
				(*spkSoundLevelPtr)++;
			}
			else if(MMI_A2DP_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				*spkSoundLevelPtr = 0;
			}
			else
			{
				LightDbgPrint("A2DP demo sound volume reach max volume");
				MMI_PushMediaEvent(MEDIA_EVT_A2DP_VOLUME_REACH_MAX);
				return;
			}
		}
	}
	else
	{
		if(gMMI_A2DP_ctl.demoSoundCtl.smallStep == 0)
		{
			if (*spkSoundLevelPtr > 0)
			{
				(*spkSoundLevelPtr)--;
			}
			else if(MMI_A2DP_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				*spkSoundLevelPtr = MMI_MAX_MUSIC_SOUND_LEVEL();
				goto MMI_SEND_A2DP_VOL;
			}
			else
			{
				LightDbgPrint("A2DP demo sound volume reach min volume");
				MMI_PushMediaEvent(MEDIA_EVT_A2DP_VOLUME_REACH_MIN);
				return;
			}
		}
		gMMI_A2DP_ctl.demoSoundCtl.smallStep = MMI_DRV_DecreaseSmallStepVolume(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep, gMMI_A2DP_ctl.demoSoundCtl.smallStep);
	}
	MMI_SEND_A2DP_VOL:
	#ifdef AIR_MODULE
	MMI_AIR_DemoSoundKeyVolUpDn();
	#endif
	MMI_A2DP_SendDriverVolCmd(*spkSoundLevelPtr, gMMI_A2DP_ctl.demoSoundCtl.smallStep,FALSE);
}
#endif

#endif //A2DP_Profile
