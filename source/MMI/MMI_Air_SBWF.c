#define SOUNDBAR_MUTE_WOOFER (FALSE)
#define SOUNDBAR_SEND_SCO_NULL_PKT (FALSE)

PRIVATE void MMI_SBWF_SCOWooferRemoteVolHandler(U8 linkIndex, U8 volume);
PRIVATE void MMI_SBWF_EstSCOToWoofer(U8 linkIndex);
PRIVATE void MMI_SBWF_PrintSoundLevelLog(U8 linkIndex, U8 mode);

PUBLIC void MMI_DRV_WooferMuteOnNotify(void)
{
	if(gDriver_ctl.isAudioHandlingPending)
	{
		gDriver_ctl.sendDSPAppIdle = TRUE;
	}
}

PRIVATE void MMI_SBWF_SendAVRCPAbsoluteVolume(U8 linkIndex, U8 type)
{
	U8 absVol;
	if(type == AUDIO_DEVICE_LINE_IN)
	{
		#ifdef LINEIN_ENABLE
		absVol = MMI_LineIn_GetAbsVolByLineIn();
		#else
		return;
		#endif
	}
	else if(type == AUDIO_DEVICE_MUSIC)
	{
		if(A2DP_STREAMING != A2DP_GetState(gMMI_ctl.audioDevCtl.currentActiveLinkIndex) && !A2DP_IsResume(gMMI_ctl.audioDevCtl.currentActiveLinkIndex))
		{
			return;
		}
		absVol = MMI_AVRCP_GetAbsVolByMusic(gMMI_ctl.audioDevCtl.currentActiveLinkIndex);
	}
	else
		return;
	#ifdef DBG_UART_ENABLE_MMI
	LightDbgPrint("USR - SetAbsVol:%x, %x", (U8)linkIndex, (U8)absVol);
	#endif
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SET_ABSOLUTE_VOLUME, absVol);
}

PRIVATE void MMI_SBWF_SetWooferVolumeLineInMode(U8 soundLevel, U8 smallStep, DriverVolumeSetCmd XDATA_PTR volPtr)
{	
	MMI_DRV_SetSCODriverVolPara(soundLevel, GET_SCO_MIC_GAIN_BY_LEVEL(soundLevel), smallStep, FALSE, volPtr);
}

PRIVATE void MMI_SBWF_SetWooferVolumeA2DPMode(U8 soundLevel, U8 smallStep, DriverVolumeSetCmd XDATA_PTR volPtr)
{
	U8 spkGainSetIndex, micGainSetIndex;
	U8 deltaDigitalGainA, deltaAnalogGainA;
	U8 deltaMicDigitalGainA, deltaMicAnalogGainA_R, deltaMicAnalogGainA_L;
	BOOL isMute;

	volPtr->soundLevel= soundLevel;

	isMute = (soundLevel) ? FALSE : TRUE;
	if(soundLevel)
		soundLevel--;

	spkGainSetIndex = GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel);
	micGainSetIndex = GET_A2DP_MIC_GAIN_BY_LEVEL(soundLevel);
	
	// SPEAKER
	volPtr->spkVols = MMI_SPK_GAINSET[spkGainSetIndex].digitalGainA;
	volPtr->analogDaGain = MMI_SPK_GAINSET[spkGainSetIndex].analogGainA;
	volPtr->micVols = MMI_MIC_GAINSET[micGainSetIndex].digitalGainA;
	volPtr->analogAdGain_R = MMI_MIC_GAINSET[micGainSetIndex].analogGainA_R;
	volPtr->analogAdGain_L = MMI_MIC_GAINSET[micGainSetIndex].analogGainA_L;

	volPtr->smallStepLevel = smallStep;

	if(smallStep && !isMute)
	{
		deltaDigitalGainA = (U16)(MMI_SPK_GAINSET[GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel+1)].digitalGainA - MMI_SPK_GAINSET[spkGainSetIndex].digitalGainA)*smallStep/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
		deltaAnalogGainA = (U16)(MMI_SPK_GAINSET[GET_A2DP_SPK_GAIN_BY_LEVEL(soundLevel+1)].analogGainA - MMI_SPK_GAINSET[spkGainSetIndex].analogGainA)*smallStep/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
		deltaMicDigitalGainA = (U16)(MMI_MIC_GAINSET[GET_A2DP_MIC_GAIN_BY_LEVEL(soundLevel+1)].digitalGainA - MMI_MIC_GAINSET[micGainSetIndex].digitalGainA)*smallStep/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
		deltaMicAnalogGainA_R = (U16)(MMI_MIC_GAINSET[GET_A2DP_MIC_GAIN_BY_LEVEL(soundLevel+1)].analogGainA_R - MMI_MIC_GAINSET[micGainSetIndex].analogGainA_R)*smallStep/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
		deltaMicAnalogGainA_L = (U16)(MMI_MIC_GAINSET[GET_LINEIN_MIC_GAIN_BY_LEVEL(soundLevel+1)].analogGainA_L - MMI_MIC_GAINSET[micGainSetIndex].analogGainA_L)*smallStep/(gMMI_nvram.smallStepVolume.a2dpVolumeSmallStep+1);
		volPtr->spkVols += deltaDigitalGainA;
		volPtr->analogDaGain += deltaAnalogGainA;
		volPtr->micVols += deltaMicDigitalGainA;
		volPtr->analogAdGain_R+= deltaMicAnalogGainA_R;
		volPtr->analogAdGain_L+= deltaMicAnalogGainA_L;
	}
}


PUBLIC void MMI_SBWF_Init(void)
{
	#ifdef SAME_SW_DIFF_MODULE
	U8 XDATA_PTR namePtr;
	U8 input;
	namePtr = OSMEM_Get(OSMEM_ptr1);

	input = LC_SFR_READ_BY_FB(FBOFFS_RW_GPIO_O_B0);
	//if(DRV_GPIO_F_GetIOStatus(5))
	if(input & (0x01<<5)) //Soundbar
	{
		MMI_PNP_PRODUCT_ID = PRODUCT_ID_TWS;
		MMI_PNP_PRODUCT_VER = (U16)PRODUCT_AIR_SOUNDBAR;
		MMI_AUDIO_PRIORITY[0] = AUDIO_DEVICE_KEY_AT_CMD;
		MMI_AUDIO_PRIORITY[1] = AUDIO_DEVICE_SCO;
		MMI_AUDIO_PRIORITY[2] = AUDIO_DEVICE_SCO_HSP;
		MMI_AUDIO_PRIORITY[3] = AUDIO_DEVICE_SCO_IDLE;
		MMI_AUDIO_PRIORITY[4] = AUDIO_DEVICE_MUSIC;
		MMI_AUDIO_PRIORITY[5] = AUDIO_DEVICE_MUSIC_CALL_NOTIFY;
		MMI_AUDIO_PRIORITY[6] = AUDIO_DEVICE_FM;
		MMI_AUDIO_PRIORITY[7] = AUDIO_DEVICE_LINE_IN;
		MMI_CONNECTION |= 0x0013;
		MMI_MAX_PAGE_DEVICE = 0x02;
		MMI_DIS_PDL_LESS_THAN = 4;
		gMMI_ctl.currentSupportDeviceNo = 2;

		namePtr[0] = 'S';
		namePtr[1] = 'o';
		namePtr[2] = 'u';
		namePtr[3] = 'n';
		namePtr[4] = 'd';
		namePtr[5] = 'b';
		namePtr[6] = 'a';
		namePtr[7] = 'r';
		gMMI_driver_variation_nvram.misc_para.init.local_name.len = 8;
		MMI_AUX_FS_SEL = PCM_FS_SEL_8KHZ;
	}
	else //Woofer
	{
		MMI_PNP_PRODUCT_ID = PRODUCT_ID_TWS;
		MMI_PNP_PRODUCT_VER = (U16)PRODUCT_AIR_SCO_WOOFER;
		MMI_AUDIO_PRIORITY[0] = AUDIO_DEVICE_KEY_AT_CMD;
		MMI_AUDIO_PRIORITY[1] = AUDIO_DEVICE_MUSIC;
		MMI_AUDIO_PRIORITY[2] = AUDIO_DEVICE_MUSIC_CALL_NOTIFY;
		MMI_AUDIO_PRIORITY[3] = AUDIO_DEVICE_FM;
		MMI_AUDIO_PRIORITY[4] = AUDIO_DEVICE_LINE_IN;
		MMI_AUDIO_PRIORITY[5] = AUDIO_DEVICE_SCO_HSP;
		MMI_AUDIO_PRIORITY[6] = AUDIO_DEVICE_SCO_IDLE;
		MMI_AUDIO_PRIORITY[7] = AUDIO_DEVICE_SCO;
		MMI_MAX_PAGE_DEVICE = 0x01;
		MMI_DIS_PDL_LESS_THAN = 0;
		gMMI_ctl.currentSupportDeviceNo = 1;
		namePtr[0] = 'W';
		namePtr[1] = 'o';
		namePtr[2] = 'o';
		namePtr[3] = 'f';
		namePtr[4] = 'e';
		namePtr[5] = 'r';
		gMMI_driver_variation_nvram.misc_para.init.local_name.len = 6;

		MMI_FEATURE_CTL_AUDIO3 |= 0x0002; //MMI_MUTE_SPEAKER_AS_MICROPHONE_FEAT
		MMI_AUX_FS_SEL= PCM_FS_SEL_8KHZ;
	}
	OSMEM_memcpy_xdata_xdata(gMMI_driver_variation_nvram.misc_para.init.local_name.name, (U8 XDATA_PTR)namePtr, gMMI_driver_variation_nvram.misc_para.init.local_name.len);
	SYS_WriteEIRResponseName(gMMI_driver_variation_nvram.misc_para.init.local_name.name, gMMI_driver_variation_nvram.misc_para.init.local_name.len);
	MMI_SECTOR_SetUpdateFlag(TRUE);
	OSMEM_Put(namePtr);
	#endif
	
	MMI_HCI_SendVCmdAudioEnhanceSCOLatency(MMI_IsLocalDeviceType(PRODUCT_AIR_SOUNDBAR)?TRUE:FALSE);
	
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_SCO_WOOFER))
	{
		gMMI_Air_ctl.SCOAudioFSSel = 0xFF;
	}
}

PUBLIC void MMI_SBWF_CheckTimer(void)
{
	U8 i;
	if(SYS_IsTimerExpired(&gMMI_Air_ctl.closeSCODSPTimer))
	{
		if ((i = MMI_AIR_GetAirLink(PRODUCT_AIR_SOUNDBAR)) != MMI_EOF)
		{
			if(SCO_EXISTS(i))
			{
				#if SOUNDBAR_MUTE_WOOFER
				MMI_DRV_SetSpkMuteByLink(SET_MUTE_ON, i);
				#endif
				MMI_SendDSPCmd(i, DSP_DISABLED);
			}
		}
	}
	
	for (i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(SYS_IsTimerExpired(&gMMI_Air_ctl.linkInfo[i].estSCOTimer))
		{
			if(MMI_IsActiveLink(i) && !SCO_EXISTS(i) && (MMI_CheckInLineInState() || MMI_CheckAllLinksInA2DPState()))
			{
				SYS_SetTimer(&gMMI_Air_ctl.linkInfo[i].estSCOTimer, 2*ONE_SEC);
				MMI_SBWF_EstSCOToWoofer(i);
			}
		}
	}
}

PUBLIC BOOL MMI_SBWF_LoadState(U8 linkIndex, U8 state)
{
#ifdef SOUND_BAR
	U8 airLink = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);
	
	if(state == MMI_CONNECTED && state == MMI_GetTopStateByLink(linkIndex))
	{
		if(CURRENT_ACTIVE_LINK_CNT >= 2)
		{
			MMI_LED_SendBGLedCmd(gMMI_nvram.maxLinkLedSetting.entryLedDisplayIndex);
		}
		else if(airLink == linkIndex)
		{
			MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
		}
		else
		{
			MMI_LED_SendBGLedCmd(12);
		}
		return TRUE;
	}
	else if(state == MMI_CONDISCABLE && MMI_CheckProfileOnAllLinks())
	{
		MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
		return TRUE;
	}
	else if(state == MMI_CONNECTABLE && MMI_CheckProfileOnAllLinks() )
	{
		U8 otherLink = MMI_GetNextActiveLink(linkIndex);
		
		if(otherLink != MMI_EOF)
		{
			if(CURRENT_ACTIVE_LINK_CNT>2)
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.maxLinkLedSetting.entryLedDisplayIndex);
				return TRUE;
			}
			if(airLink == linkIndex)
			{
				MMI_LED_SendBGLedCmd(12);
			}
			else
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_CONNECTED].entryLedDisplayIndex);
			}		
		}
		else
		{
			MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
		}
		return TRUE;
	}
#else
	UNUSED(linkIndex);
	UNUSED(state);
#endif
	return FALSE;
}

#if SOUNDBAR_SEND_SCO_NULL_PKT
PUBLIC void MMI_AIR_MuteOnSCONullPktHandle(U8 linkIndex)
{
	MMI_HCI_SendVCmdAudioSCONullPacket(linkIndex, TRUE);
	gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable = TRUE;
	if(MMI_LineIn_IsDSPOpen())
		MMI_LineIn_MuteControl(TRUE);
	if(MMI_DRV_A2DPDSPEnable())
		MMI_DRV_SetSpkMuteByLink(SET_MUTE_ON, MMI_DRV_GetA2DPEnabledLinkIndex());
}

PUBLIC void MMI_AIR_MuteOffSCONullPktHandle(U8 linkIndex)
{
	U8 i;
	MMI_HCI_SendVCmdAudioSCONullPacket(linkIndex, FALSE);
	gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable = TRUE;
	MMI_LineIn_MuteControl(FALSE);
	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		MMI_DRV_SetSpkMuteByLink(SET_MUTE_OFF, i);
	}
}
#endif

PUBLIC void MMI_SBWF_AclDiscHandle(U8 linkIndex)
{
	if(linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER))
	{
		#if SOUNDBAR_SEND_SCO_NULL_PKT
		MMI_AIR_MuteOffSCONullPktHandle(linkIndex);
		gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable = FALSE;
		#endif
		MMI_DRV_WooferMuteOnNotify();
		#if SOUNDBAR_MUTE_WOOFER
		gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = FALSE;
		#endif
	}
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_SCO_WOOFER))
	{
		gMMI_Air_ctl.SCOAudioFSSel = 0xFF;
	}
	SYS_ReleaseTimer(&gMMI_Air_ctl.linkInfo[linkIndex].estSCOTimer);
}

PRIVATE void MMI_AIR_SendWooferMuteOn(U8 linkIndex)
{
	LightDbgPrint("SendWooferMuteOn:%d,%d, %d",(U8)gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted, (U8)gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable, (U8)gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute);

	if(gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted)
	{
		#if SOUNDBAR_SEND_SCO_NULL_PKT
		if(!gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable && SCO_EXISTS(linkIndex))
		{
			MMI_AIR_MuteOnSCONullPktHandle(linkIndex);
			gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted = FALSE;
		}
		#endif

		#if SOUNDBAR_MUTE_WOOFER
		if(!gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute && MMI_IsProfileConnected(linkIndex, PROFILE_AVRCP))
		{
			MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_MUTE_CONTROL, AVRCP_VENDOR_UNIQ_PARA_MUTE_ON);
			gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = TRUE;
			gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted = FALSE;
		}
		#endif
		
	}
}

PUBLIC void MMI_AIR_SendWooferMuteOff(U8 linkIndex)
{
	U8 i;
	MMIMsgType XDATA_PTR msgPtr;
	BOOL keepMute = FALSE;

	if(MMI_LineIn_IsDSPOpen())
	{
		i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
		while(i--)
		{
			msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
			if(msgPtr->msgOpcode == DRIVER_STOP_LINE_IN_CMD )
			{
				keepMute = TRUE;
			}
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		}
	}
	else if(MMI_DRV_A2DPDSPEnable())
	{
		i = OSMQ_Entries(OSMQ_DRIVER_Command_ptr);
		while(i--)
		{
			msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(OSMQ_DRIVER_Command_ptr);
			if(msgPtr->msgOpcode == DRIVER_STOP_DSP_CMD && msgPtr->msgBodyPtr.driverCmd.stopDSPCmd.linkInd == MMI_DRV_GetA2DPEnabledLinkIndex())
			{
				keepMute = TRUE;
			}
			else if(msgPtr->msgOpcode == DRIVER_A2DP_CMD && msgPtr->msgBodyPtr.driverCmd.a2dpCmd.linkInd != MMI_DRV_GetA2DPEnabledLinkIndex())
			{
				keepMute = TRUE;
			}
			OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		}
	}

	LightDbgPrint("SendWooferMuteOff:%d,%d,%d, %d",(U8)gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted, (U8)gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable, (U8)gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute, (U8)keepMute);

	if(!gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted && !keepMute)
	{
		#if SOUNDBAR_SEND_SCO_NULL_PKT
		if(!gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable && SCO_EXISTS(linkIndex))
		{
			MMI_AIR_MuteOffSCONullPktHandle(linkIndex);
			gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted = TRUE;
		}
		#endif

		#if SOUNDBAR_MUTE_WOOFER
		if(!gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute && MMI_IsProfileConnected(linkIndex, PROFILE_AVRCP) &&
			!MMI_IsLinkActiveDisconnecting(linkIndex))
		{
			MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_MUTE_CONTROL, AVRCP_VENDOR_UNIQ_PARA_MUTE_OFF);
			gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = TRUE;
			gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted = TRUE;
		}
		#endif
		
	}
}

PUBLIC BOOL MMI_SBWF_ReleaseLink(U8 linkIndex)
{
	if(linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER))
	{
		MMI_AIR_SendWooferMuteOn(linkIndex);
		#if SOUNDBAR_SEND_SCO_NULL_PKT && SOUNDBAR_MUTE_WOOFER
		if(
			#if SOUNDBAR_SEND_SCO_NULL_PKT
			gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable
			#endif
			#if SOUNDBAR_SEND_SCO_NULL_PKT && SOUNDBAR_MUTE_WOOFER
			||
			#endif
			#if SOUNDBAR_MUTE_WOOFER
			gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute
			#endif
		)
		{
			return TRUE;
		}
		#endif
	}
	return FALSE;
}

PUBLIC void MMI_SBWF_HandleKeyMute(U8 linkIndex, U8 operationParam)
{
#if SOUNDBAR_MUTE_WOOFER
	if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_SCO_WOOFER)
	{
		if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MUTE_OFF)
		{
			MMI_DRV_SetSpkMuteByLink(SET_MUTE_OFF, linkIndex);
			MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_MUTE_CONTROL, AVRCP_VENDOR_UNIQ_PARA_MUTE_OFF_IND);
			gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted = TRUE;
		}
		else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MUTE_ON)
		{
			MMI_DRV_SetSpkMuteByLink(SET_MUTE_ON, linkIndex);
			MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_MUTE_CONTROL, AVRCP_VENDOR_UNIQ_PARA_MUTE_ON_IND);
			gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted = FALSE;
		}
	}
	else if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_SOUNDBAR)
	{
		if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MUTE_OFF_IND)
		{
			gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = FALSE;
			if(!gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted)
			{
				MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_MUTE_CONTROL, AVRCP_VENDOR_UNIQ_PARA_MUTE_ON);
				gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = TRUE;
			}
		}
		else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MUTE_ON_IND)
		{
			gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = FALSE;
			if(gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted)
			{
				if(!MMI_IsLinkActiveDisconnecting(linkIndex))
				{
					MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_MUTE_CONTROL, AVRCP_VENDOR_UNIQ_PARA_MUTE_OFF);
					gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = TRUE;
				}
			}
			if(MMI_IsLinkActiveDisconnecting(linkIndex))
			{
				MMI_ReleaseProfileLinkAndDetach(linkIndex);
			}
			MMI_DRV_WooferMuteOnNotify();
		}
	}
#else
	UNUSED(linkIndex);
	UNUSED(operationParam);
#endif
}

#ifdef PEQ_ENABLE
	#include "..\Driver\Peq_HPF.h"
#endif

PUBLIC void MMI_SBWF_HandleKeyMode(U8 linkIndex, U8 operationParam)
{
	gMMI_Air_ctl.modeInSoundbar = operationParam;
	if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_IDLE)
	{
		gMMI_Air_ctl.SCOAudioFSSel = 0xFF;
		SYS_SetTimer(&gMMI_Air_ctl.closeSCODSPTimer, 5 * ONE_SEC);
	}
	else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_LINEIN)
	{
		gMMI_Air_ctl.SCOAudioFSSel = MMI_AUX_FS_SEL;
		SYS_ReleaseTimer(&gMMI_Air_ctl.closeSCODSPTimer);
		#ifdef PEQ_ENABLE
		PEQ_SetModeIndex(PEQ_A2DP, 2);
		#endif
		if(SCO_EXISTS(linkIndex))
			MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
	}
	else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_A2DP)
	{
		gMMI_Air_ctl.SCOAudioFSSel = gMMI_nvram.airPara.A2DP_FS_SEL;
		SYS_ReleaseTimer(&gMMI_Air_ctl.closeSCODSPTimer);
		#ifdef PEQ_ENABLE
		PEQ_SetModeIndex(PEQ_A2DP, 1);
		#endif
		if(SCO_EXISTS(linkIndex))
			MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
	}
}

PUBLIC void MMI_SBWF_AVRCPSetDriverVol(U8 linkIndex)
{
	if((linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER)) != MMI_EOF)
	{
		MMI_SBWF_SendAVRCPAbsoluteVolume(linkIndex, AUDIO_DEVICE_MUSIC);
	}	
}

PUBLIC BOOL MMI_SBWF_TG_GetSetAbsVol(U8 linkIndex, U8 absVol)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_SOUNDBAR)
	{
		MMI_SBWF_SCOWooferRemoteVolHandler(linkIndex, absVol);
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL MMI_AIR_CreateConnectionToWoofer(U8 linkIndex)
{
	MMI_AIR_TryToStopAirPairing(linkIndex);
	if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_SOUNDBAR)
	{
		if (MMI_AIR_GetNextAirLink(linkIndex) != linkIndex)
		// the other woofer exists
		{			
			MMI_ReleaseProfileLinkAndDetach(linkIndex);
			return TRUE;
		}
		MMI_LinkKey_MoveLinkToAirLink((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
		{			
			if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_A2DP_WOOFER)
			{
				MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_A2DP);
			}
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_AVRCP);
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_SERIAL_PORT);
		}
		else
		{	
			MMI_AIR_SetLinkRoleMaster(linkIndex);

			if(MMI_GetConnectedProfileCount(linkIndex, TRUE))
			{
				MMI_SBWF_EstSCOToWoofer(linkIndex);
			}
		}
	}
	else
	{
		MMI_AIR_InvalidLinkHandle(linkIndex);
	}
	return FALSE;
}

PUBLIC void MMI_AIR_CreateConnectionToSoundbar(U8 linkIndex)
{
	MMI_AIR_TryToStopAirPairing(linkIndex);
	if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_SCO_WOOFER || GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_A2DP_WOOFER)
	{
		MMI_LinkKey_MoveLinkToAirLink((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
		{
			if(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER) == PRODUCT_AIR_A2DP_WOOFER)
			{
				MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_A2DP);
			}
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_AVRCP);
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_SERIAL_PORT);
		}
	}
	else
	{
		MMI_AIR_InvalidLinkHandle(linkIndex);
	}
}

PUBLIC BOOL MMI_SBWF_IsHoldLineInDisable(void)
{
#if SOUNDBAR_SEND_SCO_NULL_PKT && SOUNDBAR_MUTE_WOOFER
	U8 linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);
	if(linkIndex != MMI_EOF)
	{
		if(
			#if SOUNDBAR_SEND_SCO_NULL_PKT
			gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable
			#endif
			#if SOUNDBAR_SEND_SCO_NULL_PKT && SOUNDBAR_MUTE_WOOFER
			||
			#endif
			#if SOUNDBAR_MUTE_WOOFER
			gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute
			#endif
		)
			return TRUE;
	}
#endif
	return FALSE;
}

PUBLIC void MMI_SBWF_ExitLineInState(void)
{
	U8 i = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);

	if(i != MMI_EOF)
	{
		MMI_AIR_SendWooferMuteOn(i);
		MMI_SBWF_EstSCOToWoofer(i);
	}
}

PUBLIC void MMI_SBWF_EnterLineInState(void)
{
	U8 i =  MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);

	if(i != MMI_EOF)
	{
		MMI_SBWF_EstSCOToWoofer(i);
	}
}

PUBLIC void MMI_SBWF_LineInEnableNotify(void)
{
	U8 i =  MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);

	if(i != MMI_EOF)
	{
		if(!gMMI_Air_ctl.linkInfo[i].isSettingSCO)
		{
			MMI_AIR_SendWooferMuteOff(i);
		}
	}
}

PUBLIC void MMI_SBWF_LineInDriverVolChange(void)
{
	U8 linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);
	
	if(linkIndex != MMI_EOF)
	{
		if(MMI_CheckInLineInState())
			MMI_SBWF_SendAVRCPAbsoluteVolume(linkIndex, AUDIO_DEVICE_LINE_IN);	
	}
}

PUBLIC void MMI_SBWF_ACLConnectedNotify(U8 linkIndex)
{
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_SOUNDBAR))
	{
		MMI_HCI_SendReadRemoteName(linkIndex);
	}
}

PUBLIC void MMI_SBWF_SCOConnectedNotify(U8 linkIndex, BOOL isConnCpl)
{
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_SCO_WOOFER))
	{
		#if SOUNDBAR_MUTE_WOOFER
		if(!gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted)
		{
			MMI_DRV_SetSpkMuteByLink(SET_MUTE_ON, linkIndex);
		}
		#endif

		MMI_SendDSPCmd(linkIndex, DSP_DISABLED);
		MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
		return;
	}
	
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_SCO_WOOFER)
	{
		gMMI_Air_ctl.linkInfo[linkIndex].isSettingSCO = FALSE;
		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_SCO_IDLE);
		SYS_ReleaseTimer(&gMMI_Air_ctl.linkInfo[linkIndex].estSCOTimer);
		if(gMMI_Air_ctl.linkInfo[linkIndex].isRefreshSCOAgain)
		{
			MMI_SBWF_EstSCOToWoofer(linkIndex);
		}
		else
		{
			MMI_SniffEnable(linkIndex, MMI_SNIFF_SOUNDBAR_EST_SCO);
			if(MMI_LineIn_IsDSPOpen() || MMI_DRV_A2DPDSPEnable())
			{
				MMI_AIR_SendWooferMuteOff(linkIndex);
			}
			else
			{
				if(isConnCpl)
				{
					#if SOUNDBAR_SEND_SCO_NULL_PKT
					MMI_AIR_MuteOnSCONullPktHandle(linkIndex);
					#endif
				}
			}
		}
		DRIVER_ClearEnableSCODSPCmd(linkIndex);
		MMI_DRV_RemoveServiceLink(linkIndex);
	}
}

PUBLIC void MMI_SBWF_SCODiscNotify(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_SCO_WOOFER)
	{
		#if SOUNDBAR_SEND_SCO_NULL_PKT
		MMI_AIR_MuteOffSCONullPktHandle(linkIndex);
		gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable = FALSE;
		#endif
		#if SOUNDBAR_MUTE_WOOFER
		gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = FALSE;
		#endif
		MMI_DRV_WooferMuteOnNotify();
		gMMI_Air_ctl.linkInfo[linkIndex].isSettingSCO = FALSE;

		if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
			return;

		SYS_SetTimer(&gMMI_Air_ctl.linkInfo[linkIndex].estSCOTimer, 10 * ONE_SEC);
	}
}

PUBLIC BOOL MMI_SBWF_IdleKeyVolUpDnFirst(BOOL isVolumeUp)
{
	U8 linkIndex;
	if((linkIndex =  MMI_AIR_GetAirLink(PRODUCT_AIR_SOUNDBAR)) != MMI_EOF)
	{
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, (isVolumeUp) ? AVC_OPERATION_VOL_UP : AVC_OPERATION_VOL_DOWN);
		return TRUE;
	}
	return FALSE;
}

PUBLIC void MMI_SBWF_AVRCPConnectedNotify(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_SCO_WOOFER)
	{
		MMI_SBWF_EstSCOToWoofer(linkIndex);
	}
}

PUBLIC BOOL MMI_SBWF_TG_GetKeyVolUp(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_SCO_WOOFER)
	{
		MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, KEY_VOICEUP);
	}
	return FALSE;
}

PUBLIC BOOL MMI_SBWF_TG_GetKeyVolDown(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_SCO_WOOFER)
	{
		MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, KEY_VOICEDN);
	}
	return FALSE;
}

PUBLIC void MMI_SBWF_A2DPAcceptStartReqNotify(U8 linkIndex)
{
	U8 i;
	U8 a2dpLink;

	if((i =  MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER)) != MMI_EOF)
	{
		a2dpLink = MMI_DRV_GetA2DPEnabledLinkIndex();
		if(a2dpLink != MMI_EOF && a2dpLink != linkIndex)
		{
			MMI_AIR_SendWooferMuteOn(i);
		}
		MMI_SBWF_EstSCOToWoofer(i);
	}
}

PUBLIC void MMI_SBWF_DriverSendA2DPDSPEnableCmd(U8 linkIndex)
{
	UNUSED(linkIndex);
}

PUBLIC void MMI_SBWF_A2DPDSPEnabledNotify(U8 linkIndex)
{
	U8 i =  MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);

	if(i != MMI_EOF)
	{
		if(!gMMI_Air_ctl.linkInfo[i].isSettingSCO)
		{
			MMI_AIR_SendWooferMuteOff(i);
		}
	}
	UNUSED(linkIndex);
}

PUBLIC void MMI_SBWF_A2DPDSPDisabledNotify(U8 linkIndex)
{
	UNUSED(linkIndex);
}

PUBLIC void MMI_SBWF_DriverSendA2DPDSPDisableCmd(U8 linkIndex)
{
	U8 i = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);

	if(i != MMI_EOF)
	{
		MMI_AIR_SendWooferMuteOn(i);
	}
	UNUSED(linkIndex);
}

PUBLIC void MMI_SBWF_A2DPKeyVolUpDn(U8 linkIndex)
{
	if((linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER)) != MMI_EOF)
	{
		MMI_SBWF_SendAVRCPAbsoluteVolume(linkIndex, AUDIO_DEVICE_MUSIC);
	}	
}

PUBLIC BOOL MMI_SBWF_SDAPDisconnectedNotify(U8 linkIndex)
{
	BOOL error = FALSE;
	
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_SOUNDBAR))
	{
		#ifdef SONGPAL_Profile
		if ((gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_SONGPAL))
		{
			if(gMMI_ctl.sdapCtl[linkIndex].songpalProfileId == PROFILE_SONGPAL_ANDROID ||
				gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
			{	
				MMI_CONNECT_ACTIVE_PROFILE(linkIndex, gMMI_ctl.sdapCtl[linkIndex].songpalProfileId);
			}
		}
		#endif	
	}
	else if(MMI_IsLocalDeviceType(PRODUCT_AIR_SCO_WOOFER))
	{
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
		{
			MMI_AIR_InvalidLinkHandle(linkIndex);
			error = TRUE;
		}
		else
		{
			MMI_LinkKey_DeleteLinkHistory((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
		}	
	}
	return error;
}

PUBLIC void MMI_SBWF_HCIRoleChangeEventNotify(U8 linkIndex, U8 status, U8 roleOfSlave)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_SCO_WOOFER)
	{
		if(status == STATUS_OK && !roleOfSlave)
		{	
			MMI_SBWF_EstSCOToWoofer(linkIndex);
		}
		else
		{
			MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
		}
	}
}

PUBLIC void MMI_SBWF_HCISCONullPktCmdCpl(BOOL isMute)
{
	#if SOUNDBAR_SEND_SCO_NULL_PKT
	U8 linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);

	if(linkIndex != MMI_EOF)
	{
		LightDbgPrint("VCMD_AIROHA[%d]:%d",(U8)linkIndex, (U8)isMute);

		gMMI_Air_ctl.linkInfo[linkIndex].isHoldForDisable = FALSE;
		if(gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted == isMute)
		{
			if(gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted)
			{
				if(!MMI_IsLinkActiveDisconnecting(linkIndex))
				{
					MMI_AIR_MuteOffSCONullPktHandle(linkIndex);
				}
			}
			else
			{
				MMI_AIR_MuteOnSCONullPktHandle(linkIndex);
			}
		}

		if(isMute)
		{
			if(MMI_IsLinkActiveDisconnecting(linkIndex))
			{
				MMI_ReleaseProfileLinkAndDetach(linkIndex);
			}
			MMI_DRV_WooferMuteOnNotify();
		}
	}
	#else
	UNUSED(isMute);
	#endif
}

PUBLIC BOOL MMI_SBWF_PreProcessForWriteFlashAfterRingVP(void)
{
#if SOUNDBAR_MUTE_WOOFER || SOUNDBAR_SEND_SCO_NULL_PKT
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_SOUNDBAR))
	{
		U8 linkIndex;
		linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER);
		if(linkIndex != MMI_EOF 
		#if SOUNDBAR_MUTE_WOOFER || SOUNDBAR_SEND_SCO_NULL_PKT
		&& gMMI_Air_ctl.linkInfo[linkIndex].isUnMuted
		#endif
		)
		{
			#if SOUNDBAR_SEND_SCO_NULL_PKT
			if(SCO_EXISTS(linkIndex))
			{
				MMI_AIR_SendWooferMuteOn(linkIndex);
				return TRUE;
			}
			#endif

			#if SOUNDBAR_MUTE_WOOFER
			if(MMI_IsProfileConnected(linkIndex, PROFILE_AVRCP))
			{
				MMI_AIR_SendWooferMuteOn(linkIndex);
				return TRUE;
			}
			#endif
		}
	}
#endif
	return FALSE;
}

PUBLIC void MMI_SBWF_SetConnected(U8 linkIndex)
{
	if(!MMI_IsLocalDeviceType(PRODUCT_AIR_SOUNDBAR))
		return;
		
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID != PRODUCT_AIR_NONE)
		return;
		
	if(MMI_CheckNestState(linkIndex, MMI_CONNECTED))
	{
		MMI_SetEnterDiscoverable(FALSE);
		if(MMI_CheckNestState(MMI_EOF, MMI_CONDISCABLE))
		{
			MMI_RmvState(MMI_EOF, MMI_CONDISCABLE);	
		}
	}
}

PUBLIC BOOL MMI_SBWF_DSPSCOEnabledNotify(U8 linkIndex)
{
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_SCO_WOOFER))
	{
		DriverVolumeSetCmd XDATA_PTR volParaPtr;
		
		switch(gMMI_Air_ctl.modeInSoundbar)
		{
			case AVRCP_VENDOR_UNIQ_PARA_MODE_IDLE:

				break;
			case AVRCP_VENDOR_UNIQ_PARA_MODE_LINEIN:
				if(volParaPtr = (DriverVolumeSetCmd XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
				{
					LightDbgPrint("1WooferVolLineIn:%d,%d",(U8)MMI_LineIn_GetSoundLevel(),(U8)MMI_LineIn_GetSmallStepVol());
					MMI_SBWF_SetWooferVolumeLineInMode(MMI_LineIn_GetSoundLevel(), MMI_LineIn_GetSmallStepVol(), volParaPtr);
					DRIVER_SetNormalVolPara(volParaPtr);
					OSMEM_Put((U8 XDATA_PTR)volParaPtr);
				}
				return TRUE;

			case AVRCP_VENDOR_UNIQ_PARA_MODE_A2DP:
				if(volParaPtr = (DriverVolumeSetCmd XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
				{
					MMI_SBWF_SetWooferVolumeA2DPMode(CURRENT_A2DP_SOUND_LEVEL(linkIndex), CURRENT_A2DP_SMALL_STEP(linkIndex), volParaPtr);
					DRIVER_SetNormalVolPara(volParaPtr);
					LightDbgPrint("WooferVolMusic2:%d,%d",(U8)CURRENT_A2DP_SOUND_LEVEL(linkIndex), (U8)CURRENT_A2DP_SMALL_STEP(linkIndex));
					OSMEM_Put((U8 XDATA_PTR)volParaPtr);
				}
				return TRUE;
		}		
	}
	return FALSE;
}

extern MmiLineInInfoCtrl XDATA gMMI_LineIn_ctl;
PRIVATE void MMI_SBWF_PrintSoundLevelLog(U8 linkIndex, U8 mode)
{
	switch(mode)
	{
		case AVRCP_VENDOR_UNIQ_PARA_MODE_LINEIN:
			LightDbgPrint("Woofer line-in mode soundLevel: %d", (U8)gMMI_LineIn_ctl.currentsoundLevel);
			break;
		case AVRCP_VENDOR_UNIQ_PARA_MODE_A2DP:
			LightDbgPrint("Woofer A2DP mode soundLevel: %d", (U8)CURRENT_A2DP_SOUND_LEVEL(linkIndex));
			break;
		default:
			break;
	}
}

PRIVATE void MMI_SBWF_SCOWooferRemoteVolHandler(U8 linkIndex, U8 volume)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	switch(gMMI_Air_ctl.modeInSoundbar)
	{
		case AVRCP_VENDOR_UNIQ_PARA_MODE_IDLE:

			break;
		case AVRCP_VENDOR_UNIQ_PARA_MODE_LINEIN:
			MMI_LineIn_AbsVolToSoundLevel(volume);
			MMI_SBWF_PrintSoundLevelLog(linkIndex, AVRCP_VENDOR_UNIQ_PARA_MODE_LINEIN);
			if((msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_VOLUME_SET_CMD)) != (MMIMsgType XDATA_PTR)NULL)
			{
				MMI_SBWF_SetWooferVolumeLineInMode(MMI_LineIn_GetSoundLevel(), MMI_LineIn_GetSmallStepVol(), &msgPtr->msgBodyPtr.driverCmd.volSetCmd);
				OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
			}
			break;

		case AVRCP_VENDOR_UNIQ_PARA_MODE_A2DP:
			MMI_AVRCP_AbsVolToMusicSoundLevel(linkIndex, volume);
			MMI_SBWF_PrintSoundLevelLog(linkIndex, AVRCP_VENDOR_UNIQ_PARA_MODE_A2DP);
			if((msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_VOLUME_SET_CMD)) != (MMIMsgType XDATA_PTR)NULL)
			{
				MMI_SBWF_SetWooferVolumeA2DPMode(CURRENT_A2DP_SOUND_LEVEL(linkIndex), CURRENT_A2DP_SMALL_STEP(linkIndex), &msgPtr->msgBodyPtr.driverCmd.volSetCmd);
				OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
			}
			break;
	}
}

PRIVATE void MMI_SBWF_EstSCOToWoofer(U8 linkIndex)
{
	U16 pakType = 0;

	if (!MMI_IsRoleOfMaster(linkIndex))
	{
		if(!gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer)
		{
			MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
		}
		return;
	}

	if(!MMI_AVRCP_IsConnected(linkIndex))
		return;

	if(MMI_CheckInLineInState())
	{
		MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_LINEIN);
		SCO_LINEIN:
		if(gMMI_Air_ctl.linkInfo[linkIndex].isRefreshSCOAgain || !gMMI_Air_ctl.linkInfo[linkIndex].scoTypeLineIn || !SCO_EXISTS(linkIndex))
		{
			if(!gMMI_Air_ctl.linkInfo[linkIndex].isSettingSCO)
			{
                #ifndef CELT_MODULE
				switch(MMI_AUX_FS_SEL)
				{
					case PCM_FS_SEL_2KHZ:
						pakType = HCIPKTYPE_AIROHA_PKT_24SLOTS;
						break;
					case PCM_FS_SEL_8KHZ:
						pakType = HCIPKTYPE_AIROHA_PKT_6SLOTS;
						break;
					case PCM_FS_SEL_4KHZ:
						pakType = HCIPKTYPE_AIROHA_PKT_6SLOTS;
					default:
						ASSERT(FALSE);
						break;
				}
                #endif

				MMI_SniffDisable(linkIndex, MMI_SNIFF_SOUNDBAR_EST_SCO);
                #ifdef CELT_MODULE
                MMI_HCI_SendSetupESCO(linkIndex, gMMI_nvram.scoPktPara.scoPktType|HCIPKTYPE_AIROHA_PKT_2EV5_FOR_CELT);
                #else
				MMI_HCI_SendSetupESCO(linkIndex, gMMI_nvram.scoPktPara.scoPktType|pakType);
                #endif
				gMMI_Air_ctl.linkInfo[linkIndex].isSettingSCO = TRUE;
				gMMI_Air_ctl.linkInfo[linkIndex].isRefreshSCOAgain = FALSE;
			}
			else
			{
				gMMI_Air_ctl.linkInfo[linkIndex].isRefreshSCOAgain = TRUE;
			}
		}
		else
		{
			MMI_AIR_SCOConnectedNotify(linkIndex, FALSE);
		}
		gMMI_Air_ctl.linkInfo[linkIndex].scoTypeLineIn = TRUE;
		MMI_SBWF_SendAVRCPAbsoluteVolume(linkIndex, AUDIO_DEVICE_LINE_IN);
	}
	else if(MMI_CheckAllLinksInA2DPState())
	{
		MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_A2DP);
		if(gMMI_Air_ctl.linkInfo[linkIndex].isRefreshSCOAgain || gMMI_Air_ctl.linkInfo[linkIndex].scoTypeLineIn || !SCO_EXISTS(linkIndex))
		{
			if(!gMMI_Air_ctl.linkInfo[linkIndex].isSettingSCO)
			{
				switch(gMMI_nvram.airPara.A2DP_FS_SEL)
				{
					case PCM_FS_SEL_2KHZ:
						pakType = HCIPKTYPE_AIROHA_PKT_24SLOTS;
						break;
					case PCM_FS_SEL_8KHZ:
						pakType = HCIPKTYPE_AIROHA_PKT_6SLOTS;
						break;
					case PCM_FS_SEL_4KHZ:
						pakType = HCIPKTYPE_AIROHA_PKT_6SLOTS;
					default:
						ASSERT(FALSE);
						break;
				}
				MMI_SniffDisable(linkIndex, MMI_SNIFF_SOUNDBAR_EST_SCO);
				MMI_HCI_SendSetupESCO(linkIndex, gMMI_nvram.scoPktPara.scoPktType|pakType);
				gMMI_Air_ctl.linkInfo[linkIndex].isSettingSCO = TRUE;
				gMMI_Air_ctl.linkInfo[linkIndex].isRefreshSCOAgain = FALSE;
			}
			else
			{
				gMMI_Air_ctl.linkInfo[linkIndex].isRefreshSCOAgain = TRUE;
			}
		}
		else
		{
			MMI_AIR_SCOConnectedNotify(linkIndex, FALSE);
		}
		gMMI_Air_ctl.linkInfo[linkIndex].scoTypeLineIn = FALSE;
		MMI_SBWF_SendAVRCPAbsoluteVolume(linkIndex, AUDIO_DEVICE_MUSIC);
	}
	else // IDLE state
	{
		MMI_AIR_SendWooferMuteOn(linkIndex);
		MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_IDLE);
		if(!SCO_EXISTS(linkIndex))
			goto SCO_LINEIN;
	}
}

PUBLIC void MMI_SBWF_SendRelSco(U8 linkIndex)
{
	SYS_ReleaseTimer(&gMMI_Air_ctl.linkInfo[linkIndex].estSCOTimer);
}
