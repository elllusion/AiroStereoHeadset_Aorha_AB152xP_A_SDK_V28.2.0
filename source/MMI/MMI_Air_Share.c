#define MMI_SHARE_MASK_CHANNEL_MEDIA_PUSHED	0x01

U8 XDATA gMMI_Share_Mask;

#ifdef LIGHTING_ENABLE
#include "..\AirApp\AirApp_Interface.h"
extern AirAppLedCtl XDATA gAirAppLedCtl;
extern U8 MusicYouCanSeeEnable;
#endif

#ifdef DEMOSOUND
PRIVATE void MMI_Share_SendDemoSoundAbsVolToFollower(void)
{
	U8 destinationLink;
	if((destinationLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		MMI_A2DP_SetVolByLevel(destinationLink, gMMI_A2DP_ctl.demoSoundCtl.soundLevel, gMMI_A2DP_ctl.demoSoundCtl.smallStep);
		MMI_AVRCP_CmdGenerator(destinationLink,AVRCP_SET_ABSOLUTE_VOLUME,MMI_AVRCP_GetAbsVolByMusic(destinationLink));
		//#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI-tx_absVol[%d]:%d", (U8)destinationLink, (U8)MMI_AVRCP_GetAbsVolByMusic(destinationLink));
		//#endif
	}
}
#endif

PRIVATE void MMI_Share_SendA2DPAbsVolToFollower(U8 sourceLink)
{
	U8 destinationLink;
	if(gMMI_ctl.sdapCtl[sourceLink].remoteApplicationID == PRODUCT_AIR_NONE)
	{
		if((destinationLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
		{
			MMI_A2DP_SetVolByLevel(destinationLink, CURRENT_A2DP_SOUND_LEVEL(sourceLink), CURRENT_A2DP_SMALL_STEP(sourceLink));
			MMI_AVRCP_CmdGenerator(destinationLink,AVRCP_SET_ABSOLUTE_VOLUME,MMI_AVRCP_GetAbsVolByMusic(destinationLink));
			//#ifdef DBG_UART_ENABLE_MMI
			LightDbgPrint("MMI-tx_absVol[%d]:%d", (U8)destinationLink, (U8)MMI_AVRCP_GetAbsVolByMusic(destinationLink));
			//#endif
		}	
	}
}

PRIVATE void MMI_Share_SendLineInAbsVolToFollower(void)
{
#ifdef LINEIN_ENABLE
	U8 targetLink;
	if((targetLink = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
	{
		MMI_AVRCP_AbsVolToMusicSoundLevel(targetLink, MMI_LineIn_GetAbsVolByLineIn());
		MMI_AVRCP_CmdGenerator(targetLink,AVRCP_SET_ABSOLUTE_VOLUME,MMI_LineIn_GetAbsVolByLineIn());
		//#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI-tx_absVol_L[%d]:%d", (U8)targetLink, (U8)MMI_LineIn_GetAbsVolByLineIn());
		//#endif
	}
#endif
}

PRIVATE void MMI_Share_SetRelay(U8 linkIndex, U8 sourceLink)
{
	if(MMI_AIR_SetLinkRoleMaster(linkIndex) && !A2DP_IsLinkRelayer(linkIndex))
	{
		MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_PACKET_ROLE_RELAYER);
		MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_AUDIO_CHANNEL_MONO_L);
		MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SET_NOTIFY_MASK, MMI_AVRCP_VOLUME_CHANGED);
		MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_RELAYER);
		if(sourceLink != MMI_EOF)
			MMI_Share_SendA2DPAbsVolToFollower(sourceLink);
	}
	
	if(A2DP_IsLinkRelayer(linkIndex))
	{
		if(MMI_GetConnectedProfileCount(linkIndex, TRUE) && !(gMMI_Share_Mask & MMI_SHARE_MASK_CHANNEL_MEDIA_PUSHED))
		{
			if((MMI_EOF != sourceLink && MMI_GetConnectedProfileCount(sourceLink, TRUE)) || MMI_CheckInLineInState())
			{
				MMI_PushMediaEvent(MEDIA_EVT_TWS_LEFT_CHANNEL);
				gMMI_Share_Mask |= MMI_SHARE_MASK_CHANNEL_MEDIA_PUSHED;
			}
		}
	}
}

PRIVATE void MMI_Share_DecideRole(U8 linkIndex)
{
	BOOL isPhoneExist;
	U8 sourceLink;
	
	switch(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
	{
		case PRODUCT_AIR_TWS:
			isPhoneExist = FALSE;
			for(sourceLink = 0; sourceLink < MAX_MULTI_POINT_NO; sourceLink++)
			{
				if(MMI_IsActiveLink(sourceLink) && PRODUCT_AIR_NONE == gMMI_ctl.sdapCtl[sourceLink].remoteApplicationID)
				{
					isPhoneExist = TRUE;
					break;
				}
			}
			if(isPhoneExist)
			{
				MMI_Share_SetRelay(linkIndex, sourceLink);
			}
			else if(MMI_CheckInLineInState())
			{
				MMI_Share_SetRelay(linkIndex, MMI_EOF);
			}
			else if(!A2DP_IsLinkRelayer(linkIndex))
			{
				MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_PACKET_ROLE_FOLLOWER);
				MMI_A2DP_SendCmd(linkIndex, A2DP_AIR_AUDIO_CHANNEL_MONO_R);
				MMI_AVRCP_SendVendorUnique(linkIndex, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER);
				gMMI_Share_Mask &= ~MMI_SHARE_MASK_CHANNEL_MEDIA_PUSHED;
			}
			
			if(!isPhoneExist)
			{
				if(MMI_IsRoleOfMaster(linkIndex))
				{
					MMI_Reconnect_SetDelayTimer(ONE_SEC);
				}
				else
				{
					MMI_Reconnect_SetDelayTimer(10*ONE_SEC);
				}
			}
			break;

		case PRODUCT_AIR_NONE:
			for(sourceLink = 0; sourceLink < MAX_MULTI_POINT_NO; sourceLink++)
			{
				if(MMI_IsActiveLink(sourceLink) && PRODUCT_AIR_TWS == gMMI_ctl.sdapCtl[sourceLink].remoteApplicationID)
				{
					MMI_Share_SetRelay(sourceLink, linkIndex);
				}
			}
			break;
	}
}

/*
PRIVATE void MMI_Share_DisableReconnectFunc(void)
{
	MMI_CancelCreateAcl();
	MMI_Reconnect_ClearCtl();
	MMI_Reconnect_DisableFunc();
}

PRIVATE void MMI_Share_EnableReconnectFunc(void)
{
	MMI_Reconnect_EnableFunc();
}
*/

#ifdef LIGHTING_ENABLE
PRIVATE U32 MMI_Share_GetCurrentClk(U8 linkIndex)
{
	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.role == ROLE_MASTER)
	{
		return HAL_ReadMasterClock();
	}
	else
	{
		return HAL_ReadSlave0Clock();
	}
}
PRIVATE void MMI_Share_UpdateLedSyncClk(U8 linkIndex, U32 currentClk)
{
	//Check sniff mode or not
	if (gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_SNIFF || gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_SNIFFING)
	{
		gMMI_Air_ctl.ledSyncClkVal = currentClk+ 2000; //500ms
	}
	else
	{
		gMMI_Air_ctl.ledSyncClkVal = currentClk+ 192; //60ms
	}	
}
PUBLIC void MMI_Share_UpdateLedMode(void)
{
	U32 currentClk;
	U8 targetLink;
	if((gMMI_Air_ctl.ledSyncClkVal != 0) &&
		((targetLink= MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF))
	{
		currentClk = MMI_Share_GetCurrentClk(targetLink);
		currentClk = currentClk - gMMI_Air_ctl.ledSyncClkVal;
		if(currentClk<50)
		{
			gMMI_Air_ctl.ledSyncClkVal = 0;
			switch(gMMI_Air_ctl.ledMode)
			{
				case AIRAPP_CMD_ON_OFF_MUSIC_YOU_CAN_SEE:
					MusicYouCanSeeEnable = gMMI_Air_ctl.ledPara[0];
					break;
				case AIRAPP_CMD_GET_COLOR_PICKER_RGB:		
					LED_Set_RGB(gMMI_Air_ctl.ledPara[0], gMMI_Air_ctl.ledPara[1], gMMI_Air_ctl.ledPara[2]);
					break;
				case AIRAPP_CMD_LIGHTING_MODE_SELECT:						
					LED_LightingMode(gMMI_Air_ctl.ledPara[0]);
					break;
				case AIRAPP_CMD_ALARM_CLOCK:			
					LED_AlarmClock(gMMI_Air_ctl.ledPara[0]);
					break;
			}
		}
		else if(currentClk>1000 && currentClk<6000)
		{
			//Sync fail
			gMMI_Air_ctl.ledSyncClkVal = 0;
		}
	}
}
PUBLIC BOOL MMI_Share_ChangeLightModeNotify(U8 ledMode, U8 ledPara1, U8 ledPara2, U8 ledPara3)
{
	U8 targetLink;
	if((targetLink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		gMMI_Air_ctl.ledMode = ledMode;
		gMMI_Air_ctl.ledPara[0] = ledPara1;
		gMMI_Air_ctl.ledPara[1] = ledPara2;
		gMMI_Air_ctl.ledPara[2] = ledPara3;
		MMI_Share_UpdateLedSyncClk(targetLink, MMI_Share_GetCurrentClk(targetLink));
		MMI_AVRCP_SendVendorUnique(targetLink, AVRCP_VENDOR_UNIQ_LIGHT_MODE_CONTROL, ledMode);
		return TRUE;
	}
	return FALSE;
}
#endif

PUBLIC void MMI_Share_AclDiscHandle(U8 linkIndex)
{
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_TWS))
	{
		if(PRODUCT_AIR_NONE == gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
		{
			if(!MMI_CheckNestState(MMI_EOF, MMI_LINE_IN))
			{
				U8 twsLink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);
				if(MMI_EOF != twsLink)
				{
					MMI_A2DP_SendCmd(twsLink, A2DP_AIR_PACKET_ROLE_FOLLOWER);
					MMI_A2DP_SendCmd(twsLink, A2DP_AIR_AUDIO_CHANNEL_MONO_R);
					MMI_AVRCP_SendVendorUnique(twsLink, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER);
				}
			}
		}
		else if(linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_TWS))
		{
			gMMI_ctl.currentSupportDeviceNo = MMI_MAX_PAGE_DEVICE;
			#ifdef LINEIN_ENABLE
			MMI_LineIn_EnableFunction();
			#endif
			#ifdef LIGHTING_ENABLE
			gMMI_Air_ctl.ledSyncClkVal = 0;
			#endif
		}
		gMMI_Share_Mask &= ~MMI_SHARE_MASK_CHANNEL_MEDIA_PUSHED;
	}
}

PUBLIC void MMI_Share_HandleKeyMode(U8 linkIndex, U8 operationParam)
{
	U8 i;
	if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_RELAYER)
	{
		for(i = 0; i < MAX_MULTI_POINT_NO; i++)
		{
			if(i != linkIndex)
			{
				MMI_ReleaseProfileLinkAndDetach(i);
			}
		}
		gMMI_ctl.currentSupportDeviceNo = 1;
		#ifdef LINEIN_ENABLE
		MMI_LineIn_DisableFunction();
		#endif
		MMI_PushMediaEvent(MEDIA_EVT_TWS_RIGHT_CHANNEL);
		
		MMI_CancelCreateAcl();
		MMI_Reconnect_ClearCtl();
	}
	else if(operationParam == AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER)
	{
		gMMI_ctl.currentSupportDeviceNo = MMI_MAX_PAGE_DEVICE;
		#ifdef LINEIN_ENABLE
		MMI_LineIn_EnableFunction();
		#endif
	}
}

PUBLIC void MMI_Share_TG_GetVolChanged(U8 linkIndex, U8 absVol)
{
	if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS)
	{
		U8 i;
		
		MMI_AVRCP_AbsVolToMusicSoundLevel(linkIndex, absVol);
		
		i = MMI_DRV_GetAudioInMusicLink();
		if(MMI_EOF != i)
		{
			MMI_AVRCP_AbsVolToMusicSoundLevel(i, absVol);			
			MMI_AVRCP_CmdGenerator(i, AVRCP_VOLUME_CHANGED, MMI_AVRCP_GetAbsVolByMusic(i));
			
			if(gMMI_ctl.isAudioTransparencyEnable)
				MMI_A2DP_SendDriverVolCmd( CURRENT_MIC_SOUND_LEVEL_IN_A2DP(linkIndex), 0/*CURRENT_A2DP_SMALL_STEP(linkIndex)*/);
			else
				MMI_A2DP_SendDriverVolCmd(CURRENT_A2DP_SOUND_LEVEL(i), CURRENT_A2DP_SMALL_STEP(i));			
		}
		#ifdef LINEIN_ENABLE
		else if(MMI_LineIn_IsDSPOpen())
		{
			MMI_LineIn_AbsVolToSoundLevel(absVol);
			MMI_LineIn_SendDriverVolCmd();
		}
		#endif
	}
}

PUBLIC BOOL MMI_Share_TG_GetSetAbsVol(U8 linkIndex, U8 absVol)
{
	UNUSED(linkIndex);
	UNUSED(absVol);
	return FALSE;
}

PUBLIC void MMI_Share_AVRCPSetDriverVol(U8 linkIndex)
{
	MMI_Share_SendA2DPAbsVolToFollower(linkIndex);
}

PUBLIC void MMI_AIR_CreateConnectionToShare(U8 linkIndex)
{
	MMI_AIR_TryToStopAirPairing(linkIndex);
	if(MMI_IsLocalDeviceType(PRODUCT_AIR_TWS))
	{
		MMI_LinkKey_MoveLinkToAirLink((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr);
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
		{
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_AVRCP);
			MMI_CONNECT_ACTIVE_PROFILE(linkIndex, PROFILE_A2DP);
		}
		#if 0
		MMI_TWS_Decide_Role(linkIndex, gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT);
		#endif
	}
	else
	{
		MMI_AIR_InvalidLinkHandle(linkIndex);
	}
}

PUBLIC void MMI_Share_ExitLineInState(void)
{
	U8 twsLink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);
	if(MMI_EOF != twsLink && MMI_EOF == MMI_GetNextNormalLink(twsLink))
	{
		MMI_A2DP_SendCmd(twsLink, A2DP_AIR_PACKET_ROLE_FOLLOWER);
		MMI_A2DP_SendCmd(twsLink, A2DP_AIR_AUDIO_CHANNEL_MONO_R);
		MMI_AVRCP_SendVendorUnique(twsLink, AVRCP_VENDOR_UNIQ_OPID_MODE_NOTIFY, AVRCP_VENDOR_UNIQ_PARA_MODE_FOLLOWER);
		gMMI_Share_Mask &= ~MMI_SHARE_MASK_CHANNEL_MEDIA_PUSHED;
	}
}

PUBLIC void MMI_Share_EnterLineInState(void)
{
	U8 i =  MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);
	if(i != MMI_EOF)
	{
		MMI_Share_SetRelay(i, MMI_EOF);
	}
}

PUBLIC void MMI_Share_LineInStartRelay(void)
{
	U8 i =  MMI_AIR_GetAirLink(PRODUCT_AIR_TWS);
	if(i != MMI_EOF)
	{
		MMI_Share_SetRelay(i, MMI_EOF);
	}
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_LineInKeyVolUpDn(void)
{
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_LineInUartSetVol(void)
{
	MMI_Share_SendLineInAbsVolToFollower();
}

PUBLIC void MMI_Share_AVRCPConnectedNotify(U8 linkIndex)
{
	UNUSED(linkIndex);
}

PUBLIC void MMI_Share_A2DPSignalChannelReadyNotify(U8 linkIndex)
{
	if(	gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS &&
		gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT )
	{
		MMI_A2DP_ConnectMediaChannelSource(linkIndex);
	}
}

PUBLIC void MMI_Share_A2DPOpenNotify(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
}

PUBLIC void MMI_Share_A2DPDSPEnabledNotify(U8 linkIndex)
{
	if(!MMI_SDAP_IsQueryComplete(linkIndex))
	{
		MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
	}
}


PUBLIC void MMI_Share_A2DPStartRelay(U8 linkIndex)
{
	MMI_Share_SendA2DPAbsVolToFollower(linkIndex);
}

PUBLIC void MMI_Share_A2DPKeyVolUpDn(U8 linkIndex, BOOL isVolumeUp)
{
	UNUSED(isVolumeUp);
	MMI_Share_SendA2DPAbsVolToFollower(linkIndex);
}

PUBLIC BOOL MMI_Share_SDAPDisconnectedNotify(U8 linkIndex)
{
	MMI_Share_DecideRole(linkIndex);
	return FALSE;
}

PUBLIC void MMI_Share_HCIRoleChangeEventNotify(U8 linkIndex, U8 status, U8 roleOfSlave)
{
	if(status == NO_ERROR && gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS && !roleOfSlave)
	{
		MMI_Share_DecideRole(linkIndex);
	}
}

PUBLIC BOOL MMI_Share_PreProcessForWriteFlashAfterRingVP(void)
{
	U8 linkIndex;
	
	if((linkIndex = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
	{
		MMI_A2DP_SendCmd(linkIndex, A2DP_MUSIC_DATA_MEMORY_PUT_CMD);
	}
	return FALSE;
}

PUBLIC void MMI_Share_SetConnected(U8 linkIndex)
{
	UNUSED(linkIndex);
}

PUBLIC void MMI_Share_FakeMediaEvt(U16 id)
{
	UNUSED(id);
}

PUBLIC void MMI_Share_CheckKeyLineInPowerOn(U16 keyCode)
{
	UNUSED(keyCode);
}

PUBLIC void MMI_Share_CheckTimer(void)
{

}

PUBLIC BOOL MMI_Share_LoadState(U8 linkIndex, U8 state)
{
	UNUSED(linkIndex);
	UNUSED(state);
	return FALSE;
}

PUBLIC void MMI_Share_ConnectedExit(U8 linkIndex)
{
	UNUSED(linkIndex);
}


#if 0
PUBLIC void MMI_TWS_Decide_Role (U8 linkIndex, U8 isActive)
{
	U8 key_index, profile;
	U8 other_link = MMI_GetNextActiveLink(linkIndex);

	if ((key_index = MMI_LinkKey_SearchAirLinkKeyIndex ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, TRUE)) == MMI_EOF)
		return;

	if(other_link != MMI_EOF && A2DP_GetAirRole(other_link) != AIR_ROLE_NONE)
	{
		//detach this link because of the other link is tws/shareMe;
		MMI_ReleaseProfileLinkAndDetach (linkIndex);
		MMI_LinkKey_ClearAirLinkHistory(key_index);
		MMI_SECTOR_SetUpdateFlag(TRUE);
		return;
	}

	profile = gMMI_driver_variation_nvram.air_link_history_info[key_index].profile;
	if (!profile)
	{
		if (MMI_SUPPORT_TWS_SHARE_FEAT)
		{
			if (isActive)
			{
				profile = NVRAM_PROFILE_RELAY;
			}
			else
			{
				profile = NVRAM_PROFILE_FOLLOW;
			}
		}
		else if (MMI_SUPPORT_TWS_FEAT)
		{
			if (isActive)
			{
				profile = NVRAM_PROFILE_LEFT;
			}
			else
			{
				profile = NVRAM_PROFILE_RIGHT;
			}
		}
	}

	key_index = MMI_LinkKey_ShiftAirLinkKeyToTop(key_index);
	gMMI_driver_variation_nvram.air_link_history_info[key_index].profile = profile;
	MMI_SECTOR_SetUpdateFlag(TRUE);

	if(gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting > A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS)
		return;

	switch (profile)
	{
		case NVRAM_PROFILE_LEFT:
			gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_CHANNEL;
			MMI_Share_ReportChannel(A2DP_TWS_AUDIO_SETTING_LEFT_CHANNEL);
			MMI_SniffDisable (linkIndex, MMI_SNIFF_TWS);
			MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
			break;

		case NVRAM_PROFILE_RIGHT:
			gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_RIGHT_CHANNEL;
			MMI_SniffDisable (linkIndex, MMI_SNIFF_TWS);
			MMI_Share_ReportChannel(A2DP_TWS_AUDIO_SETTING_RIGHT_CHANNEL);

			gMMI_ctl.currentSupportDeviceNo = 1;
			MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
			MMI_ReleaseProfileLinkAndDetach (other_link);
			break;

		case NVRAM_PROFILE_RELAY:
			if (MMI_SUPPORT_AVRCP_KEY_SYNC)
			{
				MMI_HCI_SendSwitchRole(linkIndex, ROLE_MASTER);
				gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_RELAY_ONLY;
			}
			else
			{
				gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_FOLLOWER;
			}
			//by default set to follower, and change to relay only when playing
			MMI_Share_ReportChannel(A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_RELAY_ONLY);
			MMI_SniffDisable (linkIndex, MMI_SNIFF_TWS);
			break;

		case NVRAM_PROFILE_FOLLOW:
			gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_FOLLOWER;
			//by default set to follower, and change to relay only when playing
			MMI_SniffDisable (linkIndex, MMI_SNIFF_TWS);
			MMI_Share_ReportChannel(A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS_FOLLOWER);
			break;

		default:
			gMMI_A2DP_ctl.A2dpCtl[linkIndex].tws_audio_setting = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS;
			break;
	}
}
#endif
