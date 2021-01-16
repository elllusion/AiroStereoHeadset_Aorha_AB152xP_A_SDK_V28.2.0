#include "reside_flash.inc"

#define _MMI_LINEIN_C_

#include "rc.h"
#include "bt_config_profile.h"

#ifdef LINEIN_ENABLE
#include "align_flash.h"
#include "MMI.h"
#include "MMI_LineIn.h"
#include "MMI_Driver.h"
#include "MMI_State.h"
#include "MMI_HCI.h"
#include "MMI_Protocol.h"
#include "MMI_AirModule.h"
#include "MMI_Reconnect.h"
#include "MMI_A2DP.h"
#include "..\AVRCP\AVRCP_Interface.h"
#include "..\A2DP\A2DP_Interface.h"
#include "MMI_A2DP.h"
#include "MMI_AVRCP.h"

#ifdef PEQ_ENABLE
#include "..\Driver\Peq_HPF.h"
#endif

PRIVATE void MMI_LineIn_KeyPowerOn(U8 linkIndex);
PRIVATE void MMI_LineIn_KeyPowerOff(U8 linkIndex);
PRIVATE void MMI_LineIn_KeyMuteToggle(U8 linkIndex);
PRIVATE U16 MMI_LineIn_CheckKeyPowerOn(U16 keyCode);
PRIVATE void MMI_LineInEntry(U8 linkIndex);
PRIVATE void MMI_LineInHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_LineInExit(U8 linkIndex);
PRIVATE U8 MMI_LineIn_SmallStepGainCalculator(U8 gainHigh, U8 gainLow);
extern PUBLIC void DRIVER_SetLineInMuteUnMute(BOOL isMute);
extern PUBLIC BOOL DRIVER_IsLineInMute(void);

extern MmiHandlerSetType CODE_PTR XDATA MMI_StateHandlerSet[MMI_TOTAL_STATE_NO];

PRIVATE MmiHandlerSetType CODE	MMI_StateLineInHandler =
{
	MMI_LineInEntry, MMI_LineInHandler, MMI_LineInExit, //MMI_LINE_IN
};


MmiLineInInfoCtrl XDATA gMMI_LineIn_ctl;

PUBLIC MMI_KEY_EVT CODE MMI_LineIn_KeyEvtHandler[LINE_IN_KEY_EVENT_NO] =
{
	{MMI_LineIn_KeyPowerOn}, //LINE_IN_KEY_EVENT_KEY_ENTER_LINEIN_MODE
	{MMI_LineIn_KeyPowerOff}, //LINE_IN_KEY_EVENT_KEY_EXIT_LINEIN_MODE
	{MMI_LineIn_KeyMuteToggle}, //LINE_IN_KEY_EVENT_KEY_LINEIN_MUTE_TOGGLE
};

PUBLIC MMI_KEY_CHECK CODE MMI_LineIn_KeyCheckValidHandler[LINE_IN_KEY_EVENT_NO] =
{
	{MMI_LineIn_CheckKeyPowerOn}, //LINE_IN_KEY_EVENT_KEY_ENTER_LINEIN_MODE
	{NULL}, //LINE_IN_KEY_EVENT_KEY_EXIT_LINEIN_MODE
	{NULL}, //LINE_IN_KEY_EVENT_KEY_LINEIN_MUTE_TOGGLE
};

PUBLIC void MMI_LineIn_Init(void)
{
	MMI_StateHandlerSet[MMI_LINE_IN] = &MMI_StateLineInHandler;

	MMI_LineIn_SetVolByLevel(DEFAULT_LINEIN_SOUND_LEVEL, DEFAULT_LINEIN_SMALL_STEP_LEVEL);

	if(MMI_AUX_FS_SEL == PCM_FS_SEL_2KHZ)
		gMMI_LineIn_ctl.DSPSoundbarDownsampleSel = SW_DOWNSAMPLE;
	else
		gMMI_LineIn_ctl.DSPSoundbarDownsampleSel = HW_DOWNSAMPLE;

	#ifdef PEQ_ENABLE
	LightDbgPrint("Line-in: PEQ set number %d", (U8)PEQ_GetModeNumber(PEQ_LINE_IN));
	#endif
}

PUBLIC void MMI_LineIn_ProcessKeyEvent(U8 linkIndex, U8 index)
{
	if(MMI_LineIn_KeyEvtHandler[index].fKeyEvt)
	{
		MMI_LineIn_KeyEvtHandler[index].fKeyEvt(linkIndex);
	}
}

PUBLIC U16 MMI_LineIn_CheckValidKey(U8 index, U16 keyCode)
{
	if(MMI_LineIn_KeyCheckValidHandler[index].fKeyCheckValid)
	{
		keyCode = MMI_LineIn_KeyCheckValidHandler[index].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

PUBLIC void MMI_LineIn_VolumeControl(BOOL isVolumeUp)
{
	U8 XDATA_PTR spkSoundLevelPtr;
	U8 currentActiveDev;
	spkSoundLevelPtr  = &gMMI_LineIn_ctl.currentsoundLevel;
	currentActiveDev = gMMI_ctl.audioDevCtl.currentActiveDev;
	if(isVolumeUp)
	{
		gMMI_LineIn_ctl.smallStepVol = MMI_DRV_IncreaseSmallStepVolume(	gMMI_LineIn_ctl.currentsoundLevel, gMMI_nvram.totalLineInSoundLevel,
																		gMMI_nvram.smallStepVolume.lineInVolumeSmallStep, gMMI_LineIn_ctl.smallStepVol);
		if(gMMI_LineIn_ctl.smallStepVol == 0)
		{
			 if(*spkSoundLevelPtr < MMI_MAX_LINE_IN_SOUND_LEVEL())
			 {
				(*spkSoundLevelPtr)++;
			 }
			 else if(MMI_LINE_IN_CYCLIC_VOLUME_ENABLED_FEAT)
			 {
				*spkSoundLevelPtr = 0;
			 }
			 else
			 {
#if defined BLUETOOTH_SPEAKER || defined SUPPORT_MIC_IN_A2DP_MODE
				LightDbgPrint("Line-in volume reach max");
				if(currentActiveDev != AUDIO_DEVICE_MUSIC_CALL_NOTIFY && currentActiveDev != AUDIO_DEVICE_AVRCP_PLAY)
				{
					MMI_PushMediaEvent(MEDIA_EVT_LINEIN_VOLUME_REACH_MAX);
				}
#endif
				return;
			 }
		}
	}
	else
	{
		if(gMMI_LineIn_ctl.smallStepVol == 0)
		{
#ifdef BLUETOOTH_SPEAKER			
			if (*spkSoundLevelPtr > 0)
#else
			if (*spkSoundLevelPtr > 1)
#endif				
			 {
				(*spkSoundLevelPtr)--;
			 }
			else if(MMI_LINE_IN_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				*spkSoundLevelPtr = MMI_MAX_LINE_IN_SOUND_LEVEL();
				goto MMI_SEND_AUX_VOL;
			}
			else
			{
#if defined BLUETOOTH_SPEAKER || defined SUPPORT_MIC_IN_A2DP_MODE
				LightDbgPrint("Line-in volume reach min");
				if(currentActiveDev != AUDIO_DEVICE_MUSIC_CALL_NOTIFY && currentActiveDev != AUDIO_DEVICE_AVRCP_PLAY)
				{
					MMI_PushMediaEvent(MEDIA_EVT_LINEIN_VOLUME_REACH_MIN);
				}
#endif
				return;
			}
		}
		gMMI_LineIn_ctl.smallStepVol = MMI_DRV_DecreaseSmallStepVolume(gMMI_nvram.smallStepVolume.lineInVolumeSmallStep, gMMI_LineIn_ctl.smallStepVol);

	}
	MMI_SEND_AUX_VOL:
	MMI_LineIn_SetVolByLevel(gMMI_LineIn_ctl.currentsoundLevel, gMMI_LineIn_ctl.smallStepVol);
	MMI_LineIn_SendDriverVolCmd();

	#if defined(AIR_MODULE) && defined(BLUETOOTH_SPEAKER)
	MMI_AIR_LineInKeyVolUpDn();
	#endif

#if defined BLUETOOTH_SPEAKER || defined SUPPORT_MIC_IN_A2DP_MODE
	if(currentActiveDev != AUDIO_DEVICE_MUSIC_CALL_NOTIFY && currentActiveDev != AUDIO_DEVICE_AVRCP_PLAY)
	{
		MMI_DRV_Beep(*spkSoundLevelPtr, AUDIO_DEVICE_LINE_IN);
	}
#endif
	
	if(MMI_REMEMBER_SOUND_LEVEL_CHANGE_FEAT)
	{
		DEFAULT_LINEIN_SOUND_LEVEL = *spkSoundLevelPtr;
		DEFAULT_LINEIN_SMALL_STEP_LEVEL = gMMI_LineIn_ctl.smallStepVol;
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
}

PUBLIC void MMI_LineIn_SetDriverVolPara(DriverVolumeSetCmd XDATA_PTR volPtr)
{
	U8 spkGainSetIndex, micGainSetIndex;
	U8 deltaDigitalGainL, deltaAnalogGainL;
	U8 deltaMicDigitalGainL, deltaMicAnalogGainL_R, deltaMicAnalogGainL_L;
	U8 soundLevel, isMute;
	
	soundLevel = gMMI_LineIn_ctl.currentsoundLevel;
	volPtr->soundLevel = soundLevel;
	isMute = (soundLevel)?FALSE:TRUE;
	if(soundLevel)
		soundLevel--;

	spkGainSetIndex = GET_LINEIN_SPK_GAIN_BY_LEVEL(soundLevel);
	micGainSetIndex = GET_LINEIN_MIC_GAIN_BY_LEVEL(soundLevel);

	volPtr->spkVols = MMI_SPK_GAINSET[spkGainSetIndex].digitalGainL;
	volPtr->analogDaGain = MMI_SPK_GAINSET[spkGainSetIndex].analogGainL;
	volPtr->micVols = MMI_MIC_GAINSET[micGainSetIndex].digitalGainL;
	volPtr->analogAdGain_R = MMI_MIC_GAINSET[micGainSetIndex].analogGainL_R;
	volPtr->analogAdGain_L = MMI_MIC_GAINSET[micGainSetIndex].analogGainL_L;
	volPtr->smallStepLevel = gMMI_LineIn_ctl.smallStepVol;

	if(gMMI_LineIn_ctl.smallStepVol && gMMI_LineIn_ctl.currentsoundLevel && !isMute)
	{
		deltaDigitalGainL = MMI_LineIn_SmallStepGainCalculator(MMI_SPK_GAINSET[GET_LINEIN_SPK_GAIN_BY_LEVEL(soundLevel+1)].digitalGainL, MMI_SPK_GAINSET[spkGainSetIndex].digitalGainL);
		deltaAnalogGainL = MMI_LineIn_SmallStepGainCalculator(MMI_SPK_GAINSET[GET_LINEIN_SPK_GAIN_BY_LEVEL(soundLevel+1)].analogGainL, MMI_SPK_GAINSET[spkGainSetIndex].analogGainL);
		deltaMicDigitalGainL = MMI_LineIn_SmallStepGainCalculator(MMI_MIC_GAINSET[GET_LINEIN_MIC_GAIN_BY_LEVEL(soundLevel+1)].digitalGainL, MMI_MIC_GAINSET[micGainSetIndex].digitalGainL);
		deltaMicAnalogGainL_R = MMI_LineIn_SmallStepGainCalculator(MMI_MIC_GAINSET[GET_LINEIN_MIC_GAIN_BY_LEVEL(soundLevel+1)].analogGainL_R, MMI_MIC_GAINSET[micGainSetIndex].analogGainL_R);
		deltaMicAnalogGainL_L = MMI_LineIn_SmallStepGainCalculator(MMI_MIC_GAINSET[GET_LINEIN_MIC_GAIN_BY_LEVEL(soundLevel+1)].analogGainL_L, MMI_MIC_GAINSET[micGainSetIndex].analogGainL_L);

		volPtr->spkVols += deltaDigitalGainL;
		volPtr->analogDaGain += deltaAnalogGainL;
		volPtr->micVols += deltaMicDigitalGainL;
		volPtr->analogAdGain_R+= deltaMicAnalogGainL_R;
		volPtr->analogAdGain_L+= deltaMicAnalogGainL_L;
	}	
}

PUBLIC void MMI_LineIn_SendDriverVolCmd(void)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_LINE_IN_VOLUME_SET_CMD);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		MMI_LineIn_SetDriverVolPara(&msgPtr->msgBodyPtr.driverCmd.lineInVolSetCmd);
		
		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);

		LightDbgPrint("LineIn SoundLevel:%d", (U8)gMMI_LineIn_ctl.currentsoundLevel);

		#ifdef AIR_MODULE
		MMI_AIR_LineInDriverVolChange();
		#endif
		#ifdef SUPPORT_MIC_IN_A2DP_MODE
		DEFAULT_LINEIN_SOUND_LEVEL = gMMI_LineIn_ctl.currentsoundLevel;
		MMI_SECTOR_SetUpdateFlag(TRUE);
		#endif
	}
}

PUBLIC void MMI_LineIn_SetVolByLevel(U8 soundLevel, U8 smallStep)
{
	gMMI_LineIn_ctl.currentsoundLevel = soundLevel;
	gMMI_LineIn_ctl.smallStepVol = smallStep;
	gMMI_LineIn_ctl.absVolume = MMI_AVRCP_SoundLevelToAbsoluteVol(gMMI_LineIn_ctl.currentsoundLevel, (U8)MMI_MAX_LINE_IN_SOUND_LEVEL(), gMMI_LineIn_ctl.smallStepVol, MMI_LINE_IN_SMALL_STEP_LEVELS());
}

PUBLIC void MMI_LineIn_SetDSPSoundbarDownsampleSel(U8 DSPSoundbarDownsampleSel)
{
	gMMI_LineIn_ctl.DSPSoundbarDownsampleSel = DSPSoundbarDownsampleSel;
}

PUBLIC void MMI_LineIn_NotifyDSPStatus(BOOL isEnable)
{
	gMMI_LineIn_ctl.isDSPOpen = isEnable;
	if(isEnable)
	{
		DriverVolumeSetCmd LDATA volPAra;
		MMI_LineIn_SetDriverVolPara(&volPAra);
		DRIVER_SetLineInVolPara(&volPAra);
		MMI_LineIn_SendDriverVolCmd();
	}
}

PUBLIC BOOL MMI_LineIn_IsDSPOpen(void)
{
	return gMMI_LineIn_ctl.isDSPOpen;
}

PUBLIC U8 MMI_LineIn_GetSoundLevel(void)
{
	return gMMI_LineIn_ctl.currentsoundLevel;
}

PUBLIC U8 MMI_LineIn_GetSmallStepVol(void)
{
	return gMMI_LineIn_ctl.smallStepVol;
}

PRIVATE void MMI_LineIn_KeyPowerOn(U8 linkIndex)
{
	if(MMI_DRV_AudioDeviceRegister(MMI_EOF, AUDIO_DEVICE_LINE_IN))
	{
		if(MMI_NO_CONNECTION_IN_LINE_IN_FEAT)
		{
			MMI_PROTOCOL_RelAllLinks();
		}
		if (MMI_ENTER_LINEIN_SEND_AVRCP_PAUSE_FEAT)
		{
			#ifdef AVRCP_Profile
			if (MMI_AVRCP_IsMusicPlaying(linkIndex) && MUSIC_EXISTS(linkIndex))
			{
				MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_KEY_PAUSE);
			}
			#else
			UNUSED(linkIndex);
			#endif
		}
		MMI_AddTopState(MMI_EOF, MMI_LINE_IN);
	}
}

PRIVATE void MMI_LineIn_KeyPowerOff(U8 linkIndex)
{
	UNUSED(linkIndex);
	if (MMI_CheckNestState(MMI_EOF, MMI_LINE_IN))
	{		
		MMI_RmvState(MMI_EOF, MMI_LINE_IN);
		if(!MMI_DRV_IsAudioDeviceRegistered(MMI_EOF, AUDIO_DEVICE_FM))
			MMI_PushMediaEvent(MEDIA_EVT_EXIT_LINE_IN_MODE);
		
label_clear_line_in:	
		MMI_DRV_AudioDeviceDeRegister(MMI_EOF, AUDIO_DEVICE_LINE_IN);
		MMI_DRV_ResumeOtherAudioDevice(MMI_EOF, AUDIO_DEVICE_LINE_IN);		
		if(MMI_NO_CONNECTABLE_IN_LINE_IN_FEAT)
		{
			MMI_ConnectabilityHandle(CMD_SET_RESUME_CONNECTABLE);
		}

		if(MMI_NO_DISCAVERABLE_IN_LINE_IN_FEAT)
		{
			MMI_DiscoverabilityHandle(CMD_SET_RESUME_DISCOVERABLE);
		}

		if(MMI_NO_CONNECTION_IN_LINE_IN_FEAT)
		{
			if(gMMI_ctl.miscMask4 & MMI_MASK4_RECONNECT_LINK0)
				MMI_Reconnect(RECONNECT_FIRST_ONCE);
			if((gMMI_ctl.miscMask4 & MMI_MASK4_RECONNECT_LINK1))
				MMI_Reconnect(RECONNECT_SECONDARY_ONCE);
			gMMI_ctl.miscMask4 &= ~(MMI_MASK4_RECONNECT_LINK0 |MMI_MASK4_RECONNECT_LINK1);
		}
	}
	else if (!MMI_CheckNestState(MMI_EOF, MMI_LINE_IN)
		&& MMI_DRV_IsAudioDeviceRegistered(DEFAULT_LINK_IND, AUDIO_DEVICE_LINE_IN))
	{
		LightDbgPrint("Clear Line-in audiodev only");
		goto label_clear_line_in;
	}
}

PRIVATE void MMI_LineIn_KeyMuteToggle(U8 linkIndex)
{
	UNUSED(linkIndex);
	if(!DRIVER_IsLineInMute())
	{
		MMI_LineIn_MuteControl(TRUE);
		MMI_PushMediaEvent(MEDIA_EVT_MUTE_ON);
	}
	else
	{
		MMI_PushMediaEvent(MEDIA_EVT_MUTE_OFF);
	}
}

PRIVATE U16 MMI_LineIn_CheckKeyPowerOn(U16 keyCode)
{
	if(!MMI_NO_CONNECTION_IN_LINE_IN_FEAT)
	{
		if(MMI_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && MMI_CheckProfileOnAllLinks())
		{
			keyCode = KEY_INVALID;
		}
		else if(MMI_DRV_IsHighPriAudioDeviceRegistered(AUDIO_DEVICE_LINE_IN))
		{
			keyCode = KEY_INVALID;
			DRIVER_ResetLineInKeyTimer();
		}
	}

	if(MMI_CheckNestState(MMI_EOF, MMI_LINE_IN) 
		|| gMMI_LineIn_ctl.isLineInProhibited
		|| MMI_DRV_IsAudioDeviceRegistered(DEFAULT_LINK_IND, AUDIO_DEVICE_LINE_IN))
	{
		keyCode = KEY_INVALID;
	}
	#ifdef AIR_MODULE
	MMI_AIR_CheckKeyLineInPowerOn(keyCode);
	#endif
	return keyCode;
}


PRIVATE void MMI_LineInEntry(U8 linkIndex)
{
	U8 i;
	UNUSED(linkIndex);

	if(MMI_NO_CONNECTABLE_IN_LINE_IN_FEAT)
		MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);

	if(MMI_NO_DISCAVERABLE_IN_LINE_IN_FEAT)
		MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);

	MMI_PushMediaEvent(MEDIA_EVT_ENTER_LINE_IN_MODE);

	SYS_ReleaseTimer(&gMMI_ctl.audioDevCtl.resumeTimer);

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(gMMI_ctl.sdapCtl[i].remoteApplicationID == PRODUCT_AIR_NONE)
			MMI_HCI_SendRelSco(i);
	}
	#ifdef AIR_MODULE
	MMI_AIR_EnterLineInState();
	#endif
}

PRIVATE void MMI_LineInHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(linkIndex);

	if (msgPtr->eventCode== DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT)
	{
		if(msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id == MEDIA_EVT_ENTER_LINE_IN_MODE)
		{
			MMI_LineIn_DriverCmd(DRIVER_LINE_IN_CMD);
		}
		else if(msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id == MEDIA_EVT_MUTE_OFF)
		{
			MMI_LineIn_MuteControl(FALSE);
		}
	}
	gMMI_ctl.msgHandled = NOTPROCD;
}

PRIVATE void MMI_LineInExit(U8 linkIndex)
{
	UNUSED(linkIndex);
	#ifdef AIR_MODULE
	MMI_AIR_ExitLineInState();
	#endif

	MMI_LineIn_DriverCmd(DRIVER_STOP_LINE_IN_CMD);
}

PUBLIC void MMI_LineIn_DriverCmd(U8 LineInCmd)
{
	MMIMsgType XDATA_PTR msgPtr;

	msgPtr = MMI_GetMMIMsgTypeCmdMemory(LineInCmd);

	if(LineInCmd == DRIVER_LINE_IN_CMD )
	{
		#ifdef AIR_MODULE
		if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER)== PRODUCT_AIR_SOUNDBAR)
		{
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_RELAY = SOUNDBAR_MIX_TWO_CHANNEL;
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_DOWNSAMPLE_METHOD = gMMI_LineIn_ctl.DSPSoundbarDownsampleSel;
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_EXTERNAL_MCU = gMMI_nvram.airPara.relayScoSource;
			#ifdef CELT_MODULE
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 336;  //HCIPKTYPE_AIROHA_PKT_2EV5_FOR_CELT
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 16000L;    //HCIPKTYPE_AIROHA_PKT_2EV5_FOR_CELT
			#else
			switch(MMI_AUX_FS_SEL)
			{
				case PCM_FS_SEL_2KHZ:
					msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_24SLOTS
					msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 4000L;   //HCIPKTYPE_AIROHA_PKT_24SLOTS
					break;
				case PCM_FS_SEL_8KHZ:
					msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_6SLOTS
					msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 16000L; //HCIPKTYPE_AIROHA_PKT_6SLOTS
					break;
				case PCM_FS_SEL_4KHZ:
					msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_PACKET_LENGTH = 60; //HCIPKTYPE_AIROHA_PKT_12SLOTS
					msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_BANDWITH = 8000L; //HCIPKTYPE_AIROHA_PKT_12SLOTS
				default:
					ASSERT(FALSE);
					break;
			}
			#endif
		}
		else if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER)== PRODUCT_AIR_SCO_WOOFER)
		{
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_RELAY = SOUNDBAR_MIX_TWO_CHANNEL;
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.WOOFER_LPF_ENABLE = TRUE;
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.LPF_CUTOFF_FREQ_SEL = (U16)gMMI_nvram.airPara.LPFCutFreq;
		}
		#endif
		if(gMMI_LineIn_ctl.isPayloadEncode)
		{
			msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.PAYLOAD_ENCODE = TRUE;
		}
		#ifdef A2DP_SRC_Profile
		msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.PAYLOAD_ENCODE = TRUE;
		#endif
	}	
	OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
}

PUBLIC void MMI_LineIn_MuteControl(BOOL isMuteOn)
{
	DRIVER_SetLineInMuteUnMute((isMuteOn)?TRUE:FALSE);
}

PRIVATE U8 MMI_LineIn_SmallStepGainCalculator(U8 gainHigh, U8 gainLow)
{
	return (((gainHigh-gainLow)/(gMMI_nvram.smallStepVolume.lineInVolumeSmallStep+1))*gMMI_LineIn_ctl.smallStepVol);
}

PUBLIC void MMI_LineIn_EncodedDataHandler(void)
{
	U8 XDATA_PTR ptr1;
	
	while(ptr1 = (U8 XDATA_PTR)OSMQ_MCU_DSP_Get(OSMQ_DSP_LineIn_Encoded_Payload_ptr))
	{
		#ifdef A2DP_Profile
		if(MMI_LineIn_IsDSPOpen())
		{
			if(!MMI_A2DP_EncodedDataHandler(ptr1))
				OSMEM_Put(ptr1);
		}
		else
		#endif
		{
			OSMEM_Put(ptr1);
		}
	}
}

PUBLIC void MMI_LineIn_DisableFunction(void)
{
	gMMI_LineIn_ctl.isLineInProhibited = TRUE;
	if(MMI_CheckNestState(MMI_EOF, MMI_LINE_IN))
	{
		MMI_PushMediaEvent(KEY_EXIT_LINEIN_MODE);
		MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, KEY_EXIT_LINEIN_MODE);
	}
}

PUBLIC void MMI_LineIn_EnableFunction(void)
{
	gMMI_LineIn_ctl.isLineInProhibited = FALSE;
	DRIVER_ResetLineInKeyTimer();
}

PUBLIC void MMI_LineIn_AbsVolToSoundLevel(U8 absVol)
{
	U8 i;

	gMMI_LineIn_ctl.absVolume = absVol;
	
	if(absVol == 0)
		i = 0;
	else if(absVol == AVRCP_ABSOLUTE_VOLUME_MAX)
		i = MMI_MAX_LINE_IN_SOUND_LEVEL();
	else
		i = ((U16)absVol * MMI_MAX_LINE_IN_SOUND_LEVEL() + (AVRCP_ABSOLUTE_VOLUME_FULL_RANGE / 2)) 
			/AVRCP_ABSOLUTE_VOLUME_FULL_RANGE;

	if(i != MMI_LineIn_GetSoundLevel())
	{
		gMMI_LineIn_ctl.currentsoundLevel = i;
		gMMI_LineIn_ctl.smallStepVol = 0;
	}
}

PUBLIC U8 MMI_LineIn_GetAbsVolByLineIn(void)
{
	return gMMI_LineIn_ctl.absVolume;
}

PRIVATE void MMI_LineIn_ResumeEncoder(U8 currState, U8 oldState)
{
	if(oldState != currState && MMI_DRV_IsAudioActiveDev(MMI_EOF, AUDIO_DEVICE_LINE_IN) && !gMMI_ctl.audioDevCtl.resumeTimer)
	{
		MMI_LineIn_DriverCmd(DRIVER_STOP_LINE_IN_CMD);
		MMI_LineIn_DriverCmd(DRIVER_LINE_IN_CMD);	
	}
}

PUBLIC void MMI_LineIn_EnablePayloadEncoder(void)
{
	U8 oldIsPayloadEncode = gMMI_LineIn_ctl.isPayloadEncode;
	
	gMMI_LineIn_ctl.isPayloadEncode = TRUE;
	
	MMI_LineIn_ResumeEncoder(TRUE, oldIsPayloadEncode);
}

PUBLIC void MMI_LineIn_DisablePayloadEncoder(void)
{
	U8 oldIsPayloadEncode = gMMI_LineIn_ctl.isPayloadEncode;
	
	gMMI_LineIn_ctl.isPayloadEncode = FALSE;
	
	MMI_LineIn_ResumeEncoder(FALSE, oldIsPayloadEncode);
}


#endif
