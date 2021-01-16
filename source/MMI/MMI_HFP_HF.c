#ifdef AIRAPP_Profile
#include "..\AirApp\AirApp_Interface.h"
#endif

#ifdef BLE_AIRApp
#include "..\APP\APP_Interface.h"
#include "..\ATT\ATT_interface.h"
#include "..\SDAP\SDAP_Interface.h"
#endif

PUBLIC BOOL MMI_HFP_TransitCallStatus(U8 linkIndex, U8 beforeStatus, U8 afterStatus)
{
	U8 callInd = MMI_HFP_SearchCallStatusInd(linkIndex, beforeStatus);
	
	if(callInd < MMI_MAX_ONGOING_CALL_NO)
	{
		gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[callInd].callStatus = afterStatus;
		if((afterStatus == MMI_CALL_ACTIVE && beforeStatus != MMI_CALL_HELD) || (afterStatus == MMI_CALL_HELD && beforeStatus == MMI_CALL_EMPTY))
		{
			gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo++;
		}
		else if((beforeStatus == MMI_CALL_ACTIVE|| beforeStatus == MMI_CALL_HELD) && afterStatus == MMI_CALL_EMPTY)
		{
			gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo--;
		}
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

PRIVATE void MMI_HFP_GetCallSetupRemoteAlert(U8 linkIndex, BOOL isUnderCallActive)
{
	//For Smasung SGH E848:
	//For IOT Testing Issue 109
	if(!isUnderCallActive)
	{
		if (!SCO_EXISTS(linkIndex))
		{
			if(gMMI_nvram.timerCtl.estSCOAfterOggCallTime)
				SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer,(U32)gMMI_nvram.timerCtl.estSCOAfterOggCallTime*320L);
		}
	}
}

PRIVATE void MMI_HFP_KeyRdial(U16 opcode)
{
	U8 agInd, linkIndex;
	U8 reConnectMode = RECONNECT_TYPE_NULL;
	BOOL isNeedRdial;

	if(MMI_SEPARATE_LNR_BUTTONS_FEAT)
	{
		if (opcode == KEY_RDIAL)
		{
			agInd = 0;
			reConnectMode = RECONNECT_FIRST_ONCE_REDIAL;
		}
		else
		{
			agInd = 1;
			reConnectMode = RECONNECT_SECONDARY_ONCE_REDIAL;
		}
		
		isNeedRdial = ((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&gMMI_driver_variation_nvram.misc_para.agInd[agInd].member[0])) == MAX_MULTI_POINT_NO)?FALSE:TRUE;
	}
	else
	{
		if(CURRENT_ACTIVE_LINK_CNT  && gMMI_HFP_ctl.rdialLinkInd != MMI_EOF)
		{
			linkIndex = gMMI_HFP_ctl.rdialLinkInd;
			isNeedRdial = TRUE;
		}
		else 
		{
			linkIndex = MAX_MULTI_POINT_NO;
			reConnectMode = RECONNECT_LAST_ONCE_REDIAL;
			isNeedRdial = FALSE;
		}
	}

	if(isNeedRdial)
	{
#ifdef SUPPORT_MULTI_POINT
		//2. Pause Music in another link first[should be after link is decided]
		MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
#endif		
		switch(HFP_GetLinkInfo(linkIndex)->mmiProfileInfo.state)
		{
			case PROFILE_DISCONNECTED:
				MMI_ConnectProfile(linkIndex, PROFILE_HANDSFREE);
				/* fallthrough *//*comment to prevent coverity warning*/
			case PROFILE_CONNECTING:
			case PROFILE_CONNECTED:
				gMMI_ctl.mmiInfo[linkIndex].subState = MMI_RDIAL_CONNECTING_HFP;
				break;
			default:
				if(gMMI_ctl.mmiInfo[linkIndex].subState != MMI_RDIAL_STARTING)
				{
					if( gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTED ||gMMI_ctl.mmiInfo[linkIndex].subState==MMI_VDIAL_STARTING )
					{
						if(MMI_CANCEL_VR_WHEN_RDIAL_FEAT)
							MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_CANCEL_VOICE_DIAL);
					}
					MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_LAST_NUMBER_REDIAL);
				}
				break;
		}
	}
	else
	{
		if(MMI_RECONNECT_WHEN_REDIAL_WITHOUT_CONNECTION_FEAT)
		{
			MMI_Reconnect(reConnectMode);
		}
	}
}

PRIVATE void MMI_HFP_KeyVoiceRecognition(U16 opcode)
{
	U8  agInd, linkIndex;
	U8  reConnectMode = RECONNECT_TYPE_NULL;
	BOOL  isNeedVoiceRecognition;

	//1. Decide link and Reconnect or VDial
	if(MMI_SEPARATE_VOICE_DIAL_BUTTONS_FEAT)
	{
		if (opcode == KEY_SET_VOICE_RECOGNITION || opcode == KEY_SET_VOICE_RECOGNITION_CANCEL)
		{
			agInd = 0;
			reConnectMode = RECONNECT_FIRST_ONCE_VOICE_DIAL;
		}
		else
		{
			agInd = 1;
			reConnectMode = RECONNECT_SECONDARY_ONCE_VOICE_DIAL;
		}
		isNeedVoiceRecognition = ((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&gMMI_driver_variation_nvram.misc_para.agInd[agInd].member[0])) == MAX_MULTI_POINT_NO)?FALSE:TRUE;
	}
	else
	{
	   	if(CURRENT_ACTIVE_LINK_CNT && gMMI_HFP_ctl.rdialLinkInd != MMI_EOF)
		{
			linkIndex = gMMI_HFP_ctl.rdialLinkInd;
			isNeedVoiceRecognition = TRUE;
		}
		else
		{
			linkIndex = MAX_MULTI_POINT_NO;
			reConnectMode = RECONNECT_LAST_ONCE_VOICE_DIAL;
			isNeedVoiceRecognition = FALSE;
		}
	}

	if(opcode == KEY_SET_VOICE_RECOGNITION_CANCEL || opcode == KEY_SET_VOICE_RECOGNITION_SECONDARY_CANCEL)
	{
		if(!isNeedVoiceRecognition)
			return;
	}
	
	
	if(isNeedVoiceRecognition)
	{			
		switch(HFP_GetLinkInfo(linkIndex)->mmiProfileInfo.state)
		{
			case PROFILE_DISCONNECTED:
				MMI_ConnectProfile(linkIndex, PROFILE_HANDSFREE);
				/* fallthrough *//*comment to prevent coverity warning*/
			case PROFILE_CONNECTING:
			case PROFILE_CONNECTED:
				gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_CONNECTING_HFP;
				break;
			default:
				if(MMI_HFP_IsSuportAGOptions(linkIndex, MMI_AG_OPTION_VOICE_RECOGNITION))
				{
					//2. Pause Music in another link first[should be done after link is decided]
					if(opcode != KEY_SET_VOICE_RECOGNITION_CANCEL && opcode != KEY_SET_VOICE_RECOGNITION_SECONDARY_CANCEL)
					{
						MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
					}
					if(MMI_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT)
					{
						if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTED || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTING)
						{
							MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_CANCEL_VOICE_DIAL);
						}
						else if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STOP || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STOPPING)
						{
							MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_INIT_VOICE_DIAL);
						}
					}
					else
					{
						if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTED || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTING)
						{
							if(opcode == KEY_SET_VOICE_RECOGNITION_CANCEL || opcode == KEY_SET_VOICE_RECOGNITION_SECONDARY_CANCEL)
								MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_CANCEL_VOICE_DIAL);
						}
						else if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STOP || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STOPPING)
						{
							if(opcode == KEY_SET_VOICE_RECOGNITION || opcode == KEY_SET_VOICE_RECOGNITION_SECONDARY)
								MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_INIT_VOICE_DIAL);
						}
					}
				}
				break;
		}
	}
	else
	{
		if(MMI_RECONNECT_WHEN_VOICE_DIAL_WITHOUT_CONNECTION_FEAT)
		{
			MMI_Reconnect(reConnectMode);
		}
	}
}

PRIVATE void MMI_HFP_KeyAppleSiri(U16 opcode)
{
	U8 agInd, linkIndex;
	
	if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState() || CURRENT_ACTIVE_LINK_CNT == 0)
		return;

	//1. Decide link and Reconnect or VDial
	if(MMI_SEPARATE_VOICE_DIAL_BUTTONS_FEAT)
	{
		agInd = (opcode == KEY_SIRI_START_OR_CONTINUE ||opcode == KEY_SIRI_END)? 0:1;
		if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&gMMI_driver_variation_nvram.misc_para.agInd[agInd].member[0])) == MAX_MULTI_POINT_NO)
			return;
	}
	else
	{
		if(gMMI_HFP_ctl.rdialLinkInd == MMI_EOF)
			return;
	   	linkIndex = gMMI_HFP_ctl.rdialLinkInd;
	}


	if(gMMI_HFP_ctl.HfpCtl[linkIndex].appleSiriStatus != HFP_APLSIRI_AVAILABLE_ENABLE)
	{	
		return;	
	}
	
	if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STOP || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STOPPING)
	{	
		if(opcode == KEY_SIRI_START_OR_CONTINUE ||opcode == KEY_SIRI_START_OR_CONTINUE_SECONDARY)
		{
			MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_INIT_VOICE_DIAL);
			MMI_PushMediaEvent(MEDIA_EVT_SIRI_STARTING);
		}
	}
	else if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTED || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTING)
	{	
		MMI_HFP_SendATCmdByLink(linkIndex, (opcode == KEY_SIRI_START_OR_CONTINUE ||opcode == KEY_SIRI_START_OR_CONTINUE_SECONDARY)? HFP_ATCMD_INIT_VOICE_DIAL: HFP_ATCMD_CANCEL_VOICE_DIAL);
		MMI_PushMediaEvent((opcode == KEY_SIRI_START_OR_CONTINUE ||opcode == KEY_SIRI_START_OR_CONTINUE_SECONDARY)? MEDIA_EVT_SIRI_CONTINUE : MEDIA_EVT_SIRI_END);
	}
}

///////////////////////////////////////////////////////////////
//	MMI_Connected Related
///////////////////////////////////////////////////////////////
PRIVATE void MMI_ConnectedHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	U8 state;

	gMMI_ctl.msgHandled = PROCD_RELEASE;
	state = MMI_GetTopStateByLink(linkIndex);

	switch(msgPtr->eventCode)
	{
		case HFP_EVENT_TO_MMI_RING_INDICATION:
			//HFP SPEC. 4.13.1 & 4.13.2 & 4.13.3
			if(state == MMI_CONNECTED || ((MMI_FM == state  || MMI_LINE_IN == state)&& MMI_CheckNestState(linkIndex, MMI_CONNECTED)))
			{
				MMI_AddTopState(linkIndex, MMI_HFP_INCOMMING);
				if(MMI_HFP_IsHFPConnected(linkIndex))
				{
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_INCOMMING);
				}
			}
			break;
			
		case HFP_CIEVIND_CALLSETUP:
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
			{
				case CALLSETUP_INCOMMING:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_INCOMMING);
					//HFP SPEC. 4.13.1 & 4.13.2 & 4.13.3
					MMI_AddTopState(linkIndex, MMI_HFP_INCOMMING);
					break;
					
				case CALLSETUP_NO_CALSETUP:
					MMI_HFP_StopIncomingVPRing(linkIndex);
					break;

				case CALLSETUP_REMOTE_ALERTED:
					MMI_HFP_GetCallSetupRemoteAlert(linkIndex, FALSE);
				case CALLSETUP_OUTGOING:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_OUTGOING);
					MMI_AddTopState(linkIndex, MMI_HFP_OUTGOING);
					break;
			}
			break;

		case HFP_CIEVIND_CALL:
			//call indicator changes from 0 to 1
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE)
			{
				MMI_DRV_SendVoicepromptStopCmd();
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_ACTIVE);

				MMI_AddTopState(linkIndex, (SCO_EXISTS(linkIndex))? MMI_HFP_CALLACTIVE:MMI_HFP_CALLACTIVE_WITHOUT_SCO);
				MMI_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
			}
			break;
			
		case DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT:
			if(state == MMI_CONNECTED || ((MMI_FM == state  || MMI_LINE_IN == state)&& MMI_CheckNestState(linkIndex, MMI_CONNECTED)))
			{
				switch(msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id)
				{
					case MEDIA_EVT_KEY_RDIAL:
					case MEDIA_EVT_KEY_RDIAL_SECONDARY:
						MMI_HFP_KeyRdial(msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id);
						break;
					case MEDIA_EVT_KEY_SET_VOICE_RECOGNITION:
					case MEDIA_EVT_KEY_SET_VOICE_RECOGNITION_SECONDARY:
					case MEDIA_EVT_KEY_SET_VOICE_RECOGNITION_CANCEL:
					case MEDIA_EVT_KEY_SET_VOICE_RECOGNITION_SECONDARY_CANCEL:
						MMI_HFP_KeyVoiceRecognition(msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id);
						break;
					case MEDIA_EVT_KEY_SIRI_START_OR_CONTINUE:
					case MEDIA_EVT_KEY_SIRI_END:
					case MEDIA_EVT_KEY_SIRI_START_OR_CONTINUE_SECONDARY:
					case MEDIA_EVT_KEY_SIRI_END_SECONDARY:
						MMI_HFP_KeyAppleSiri(msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id);
						break;
				}			
			}
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI_HFPImg Related
///////////////////////////////////////////////////////////////
PRIVATE void MMI_HFPImgEntryFromCaImg(U8 linkIndex)
{
	U8 nextLink;

	gMMI_HFP_ctl.HfpCtl[linkIndex].isImgVPTriggered = FALSE;

	if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState() || gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry)
		return;
	
	gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry = TRUE;
	
	#ifdef BLE_AIRApp
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length && gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet && SDAP_IsIOS(linkIndex))
		SendPhoneNumberToApp(linkIndex, gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length, (U8)(gMMI_nvram.timerCtl.callerIDRepeatTime/10));	
	#endif	
	
	#ifdef AIRAPP_Profile
	AirApp_EnterIncomingState(linkIndex);
	#endif
	
	#ifdef AIR_MODULE
	MMI_AIR_EnterIncomingState(linkIndex);
	#endif

	MMI_DRV_ScoModeControl(linkIndex, FALSE, TRUE);

	if(!SCO_EXISTS(linkIndex))
	{
		#ifdef A2DP_Profile
		if(MUSIC_EXISTS(linkIndex))
		{
			if(MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY))
			{
				MMI_A2DP_SendDSPEnableCmd(linkIndex);
			}
		}
		#endif
	}
	else
	{
		if(MMI_DRV_AudioDeviceRegister(linkIndex, MMI_HFP_IsHSPConnected(linkIndex)?AUDIO_DEVICE_SCO_HSP:AUDIO_DEVICE_SCO))
		{
			if(SCO_EXISTS(linkIndex))
			{
				MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
			}
		}
	}
	MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);

	nextLink = MMI_HFP_GetNextHFPLink(linkIndex);

	if(nextLink != MMI_EOF && MMI_CheckLinkInCallRelatedState(nextLink) || MMI_PLAY_DEFAULT_RING_INCOMING_FEAT || MMI_HFP_IsHSPConnected(linkIndex))
		MMI_DRV_SetSpkMuteByLink(SET_MUTE_ON, linkIndex);

	MMI_HFP_ImgAudioVoiceHandler(linkIndex, IMG_AUDIO_VOICE_TRIGGER);

	if(gMMI_nvram.timerCtl.autoAnswerTime)
	{
		SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].autoAnswerTimer, (U32)gMMI_nvram.timerCtl.autoAnswerTime* 320L);
	}
	
	gMMI_HFP_ctl.incomingLinkInd = linkIndex;
}

PRIVATE void MMI_HFPImgEntry(U8 linkIndex)
{
	U8 nextLink = MMI_HFP_GetNextHFPLink(linkIndex);

	gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet = FALSE;
	gMMI_HFP_ctl.HfpCtl[linkIndex].isImgVPTriggered = FALSE;
	if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState() || gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry)
		return;
	
	gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry = TRUE;
	
	#ifdef AIRAPP_Profile
	AirApp_EnterIncomingState(linkIndex);
	#endif
	
	#ifdef AIR_MODULE
	MMI_AIR_EnterIncomingState(linkIndex);
	#endif

	MMI_DRV_ScoModeControl(linkIndex, FALSE, TRUE);

	if(nextLink == MMI_EOF || !MMI_CheckLinkInIncomingState(nextLink))
	{
		if(!SCO_EXISTS(linkIndex))
		{
			#ifdef A2DP_Profile
			if(MUSIC_EXISTS(linkIndex))
			{
				if(MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY))
				{
					MMI_A2DP_SendDSPEnableCmd(linkIndex);
				}
			}
			#endif
		}
		else
		{
			if(MMI_DRV_AudioDeviceRegister(linkIndex, MMI_HFP_IsHSPConnected(linkIndex)?AUDIO_DEVICE_SCO_HSP:AUDIO_DEVICE_SCO))
			{
				MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
			}
		}
	}
	MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);

	if(nextLink != MMI_EOF && MMI_CheckLinkInCallRelatedState(nextLink) || MMI_PLAY_DEFAULT_RING_INCOMING_FEAT || MMI_HFP_IsHSPConnected(linkIndex))
		MMI_DRV_SetSpkMuteByLink(SET_MUTE_ON, linkIndex);
	
	MMI_HFP_ImgAudioVoiceHandler(linkIndex, IMG_AUDIO_VOICE_TRIGGER);

	if(gMMI_nvram.timerCtl.autoAnswerTime)
	{
		SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].autoAnswerTimer, (U32)gMMI_nvram.timerCtl.autoAnswerTime* 320L);
	}

	gMMI_HFP_ctl.incomingLinkInd = linkIndex;
}

PRIVATE void MMI_HFPImgHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case HFP_EVENT_TO_MMI_RING_INDICATION:
			MMI_HFP_SendATCmdByLink(linkIndex, HFP_CMD_FROM_MMI_RING_GET);
			break;
		case HFP_ATCMD_CKPD_200:
			MMI_RmvState(linkIndex, MMI_HFP_INCOMMING);
			//Enter CallActive state after CKPD=200 has been sent.
			//There are 2 reasons to explain why it should enter CallActive state but not
			//back to Connected. One is [KEY1, TAP] shall be the key action that could disconnect
			//with the HSP link, and this action might not be HSP related command in Connected state.
			//Another one is Audio Transfer could be activated in that way.
			if (!MMI_NOT_CHG_STATE_ACP_HSP_RING_FEAT)
			{
				if (SCO_EXISTS(linkIndex))
				{
				 	MMI_AddTopState(linkIndex, MMI_HFP_CALLACTIVE);
				}
			}
			break;

		case HFP_CIEVIND_CALL:
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE)
			{
				//ISSUE: Bandit Handset may miss Tx callsetup = 0.-----------
				goto IMG_CALL_ACTIVE;
			}
			break;

		case HFP_CIEVIND_CALLSETUP:
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup == CALLSETUP_NO_CALSETUP)
			{
				if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE)
				{
					IMG_CALL_ACTIVE:
					if(!gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
					{
						//MMI_DRV_SendVoicepromptStopCmd();
						MMI_HFP_StopIncomingVPRing(linkIndex);
					}
					
					//call indicator: 0 to 1, callsetup: 1 to 0
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_INCOMMING, MMI_CALL_ACTIVE);

					//HFP SPEC. 4.13.1 & 4.13.2 & 4.13.3
					// jump to active state if we get both call = 1 & callsetup = 0
					MMI_ReplaceState(linkIndex, (SCO_EXISTS(linkIndex))?MMI_HFP_CALLACTIVE:MMI_HFP_CALLACTIVE_WITHOUT_SCO, MMI_HFP_INCOMMING);
					if(!gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
						MMI_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
				}
				else
				{
					//call indicaotr doesnt change, callsetup from 1 to 0
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_INCOMMING, MMI_CALL_EMPTY);
					MMI_HFP_StopIncomingVPRing(linkIndex);
					//Mantis 9761 related
					if(!gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
					{
						MMI_PushMediaEvent(MEDIA_EVT_INCOMING_CALL_ENDED);
					}
					//HFP SPEC. 4.14.1 & 4.14.2
					MMI_RmvState(linkIndex, MMI_HFP_INCOMMING);
				}
			}
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

PRIVATE void MMI_HFPImgExit(U8 linkIndex)
{
	MMI_HFP_StopIncomingVPRing(linkIndex);

	MMI_PushMediaEvent(MEDIA_EVT_EXIT_INCOMING);
	MMI_HFP_SendATCmdByLink(linkIndex, HFP_CMD_FROM_MMI_EXIT_INCOMING);

	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].autoAnswerTimer);

	gMMI_ctl.mmiInfo[linkIndex].stateEntryExit |= MMI_EXIT_IMGCALL_STATE;

	if(gMMI_nvram.timerCtl.missedCallRemindTimer && gMMI_nvram.missedCallRemindAttempt)
	{
		gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallCnt++;
		gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallRemindAttempt = gMMI_nvram.missedCallRemindAttempt;
		SYS_SetTimer(&gMMI_HFP_ctl.missedCallRemindTimer, (U32)(gMMI_nvram.timerCtl.missedCallRemindTimer) * ONE_SEC);
	}

	MMI_DRV_SetSpkMuteByLink(SET_MUTE_OFF, linkIndex);

	#ifdef A2DP_Profile
	if(MUSIC_EXISTS(linkIndex))
	{	
		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
		MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
	}
	#endif	

	#ifdef AIRAPP_Profile
	AirApp_ExitIncomingState(linkIndex);
	#endif
	gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry = FALSE;
}

///////////////////////////////////////////////////////////////
//	MMI_HFPOgg Related
///////////////////////////////////////////////////////////////
PRIVATE void MMI_HFPOggEntry(U8 linkIndex)
{
	MMI_DRV_ScoModeControl(linkIndex, FALSE, FALSE);
	MMI_DRV_SetSpkMuteByLink(SET_MUTE_OFF, linkIndex);
	
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].audioDevATCmdTimer)
	{
		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
		MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
	}

	if(MMI_DRV_AudioDeviceRegister(linkIndex, MMI_HFP_IsHSPConnected(linkIndex)?AUDIO_DEVICE_SCO_HSP:AUDIO_DEVICE_SCO))
	{
		if(SCO_EXISTS(linkIndex))
		{
			MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
		}
	}

	MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);

	gMMI_HFP_ctl.rdialLinkInd =	linkIndex;
}

PRIVATE void MMI_HFPOggHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case HFP_CIEVIND_CALL:
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE)
			{
				//ISSUE: Bandit Handset may miss Tx callsetup = 0.-----------
				goto OGG_CALL_ACTIVE;
			}
			break;

		//indicator "call" should be sent before callsetup. we might never go into this case
		//however, if "call" is not sent if remote rejects the call. just step into STANDBY state
		case HFP_CIEVIND_CALLSETUP:
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup == CALLSETUP_NO_CALSETUP)
			{
				if(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE)
				{
					OGG_CALL_ACTIVE:
					//call indicator: 1 to 0, callsetup: 2 to 0
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_OUTGOING, MMI_CALL_ACTIVE);

					//HFP SPEC. 4.18 & 4.19 & 4.20
					MMI_ReplaceState(linkIndex, (SCO_EXISTS(linkIndex))?MMI_HFP_CALLACTIVE:MMI_HFP_CALLACTIVE_WITHOUT_SCO, MMI_HFP_OUTGOING);
					if(!gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
						MMI_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
				}
				else
				{
					//call indicator doesnt change, callsetup: 2 to 0
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_OUTGOING, MMI_CALL_EMPTY);

					gMMI_ctl.mmiInfo[linkIndex].stateEntryExit |= MMI_EXIT_OUTGOING_STATE;

					MMI_RmvState(linkIndex, MMI_HFP_OUTGOING);
				}
			}
			else if(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup == CALLSETUP_REMOTE_ALERTED)
			{
				MMI_HFP_GetCallSetupRemoteAlert(linkIndex, FALSE);
			}
			else
			{
				gMMI_ctl.msgHandled = NOTPROCD;
			}
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI_HFPCa Related
///////////////////////////////////////////////////////////////
PRIVATE void MMI_HFPCaEntry(U8 linkIndex)
{
	MMI_HFP_StopIncomingVPRing(linkIndex);
	
	MMI_DRV_ScoModeControl(linkIndex, FALSE, FALSE);

	if(MMI_DRV_AudioDeviceRegister(linkIndex, MMI_HFP_IsHSPConnected(linkIndex)?AUDIO_DEVICE_SCO_HSP:AUDIO_DEVICE_SCO))
	{
		if(SCO_EXISTS(linkIndex))
		{
			MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
		}
	}

	if (MMI_AUTO_UNMUTE_ENTER_OR_EXIT_CALL_FEAT)
	{
		if (linkIndex == gMMI_ctl.audioableSCOLink)
		{
			MMI_DRV_AutoMICMuteOff(linkIndex);
		}
		MMI_DRV_MicMuteHandler(linkIndex, SET_MUTE_OFF);
	}
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallCnt)
		gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallCnt--;
		
	if(HFP_GetLinkInfo(linkIndex)->currentCmd == HFP_ATCMD_INIT_VOICE_DIAL)
	{	//Mantis Issue #6065
		SYS_SetTimer(&HFP_GetLinkInfo(linkIndex)->ost_command_ptr, HALF_SEC);
	}
	MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);
}

PRIVATE void MMI_HFP_ClearCallStatus(U8 linkIndex)
{
	U8 callInd;

	//no active call, clear the call control table info
	for (callInd = 0; callInd < MMI_MAX_ONGOING_CALL_NO; callInd++)
	{
		gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[callInd].callStatus = MMI_CALL_EMPTY;
		gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 0;
	}
}

PRIVATE void MMI_HFP_CaStateCallHeldOnHeldNoActive(U8 linkIndex)
{
	U8 nextLink;
	U8 activateLink;
	
	if((nextLink = MMI_HFP_GetNextHFPLink(linkIndex)) != MMI_EOF)
	{
		activateLink = linkIndex;
		if(MMI_CheckLinkInCallNotifyState(nextLink))
		{
			if(MMI_SUPPORT_MULTI_HFP_FEAT)
			{
				MMI_HFP_SendATCmdByLink(nextLink, HFP_ATCMD_ACCEPT_CALL);
				MMI_DRV_SendVoicepromptStopCmd();
			}
			activateLink = nextLink;
		}
		else if(MMI_CheckLinkInCallActiveState(nextLink))
		{
			if(MMI_MAX_ONGOING_CALL_NO == MMI_HFP_SearchCallStatusInd(nextLink,MMI_CALL_HELD))
			{
				activateLink = nextLink ;
			}
			else
			{
				if(MMI_SUPPORT_MULTI_HFP_FEAT)
				{
					MMI_HFP_SendATCmdByLink(nextLink, HFP_ATCMD_3WAY_HOLDNACP);
					#ifdef _HFP_CALLHELD_PREDICT_
					MMI_HFP_TransitCallStatus(nextLink, MMI_CALL_HELD, MMI_CALL_ACTIVE);
					#endif
					activateLink = nextLink;
				}
			}
		}

		if(activateLink != linkIndex)
		{
			if(SCO_EXISTS(activateLink))
				MMI_DRV_SetAudioDeviceToAnotherLink(linkIndex, AUDIO_DEVICE_SCO);
			else
				MMI_DRV_SetAudioDeviceToAnotherLink(linkIndex, AUDIO_DEVICE_CALL_STATE);
		}
		else if(gMMI_ctl.audioDevCtl.currentActiveDev == AUDIO_DEVICE_SCO && gMMI_ctl.audioDevCtl.currentActiveLinkIndex != linkIndex)
		{
			MMI_DRV_ResumeOtherAudioDevice(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, AUDIO_DEVICE_SCO);
		}
	}
}

PRIVATE void MMI_HFP_CaStateCallHeldNoCallsHeld(U8 linkIndex)
{
	U8 nextLink;
	U8 activateLink;
	
	if((nextLink = MMI_HFP_GetNextHFPLink(linkIndex)) != MMI_EOF)
	{
		activateLink = linkIndex;
		
		if(MMI_CheckLinkInOutGoingState(nextLink))
		{
			activateLink = nextLink;
		}
		else if(MMI_CheckLinkInIncomingState(nextLink))
		{
			activateLink = linkIndex;
		}
		else if(MMI_CheckLinkInCallActiveState(nextLink))
		{
			if(MMI_MAX_ONGOING_CALL_NO ==MMI_HFP_SearchCallStatusInd(nextLink,MMI_CALL_HELD))
				activateLink = nextLink ;
			else
				activateLink = linkIndex;
		}

		if(activateLink != linkIndex)
		{
			//MMI_DRV_AudioDeviceRegister(activateLink, AUDIO_DEVICE_SCO);
			MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO);
		}
		else if(gMMI_ctl.audioDevCtl.currentActiveDev == AUDIO_DEVICE_SCO && gMMI_ctl.audioDevCtl.currentActiveLinkIndex != linkIndex)
		{
			MMI_DRV_ResumeOtherAudioDevice(gMMI_ctl.audioDevCtl.currentActiveLinkIndex, AUDIO_DEVICE_SCO);
		}
	}
}

PUBLIC void MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls_HF(U8 linkIndex)
{
	if (!MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_HELD))
	{
		MMI_HFP_CaStateCallHeldOnHeldNoActive(linkIndex);
	}
}

PUBLIC void MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld_HF(U8 linkIndex)
{
	if(MMI_HFP_SearchCallStatusInd(linkIndex, MMI_CALL_HELD) < MMI_MAX_ONGOING_CALL_NO)
	{
		if (!MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_ACTIVE))
		{
			MMI_HFP_CaStateCallHeldNoCallsHeld(linkIndex);
		}
	}
}

PRIVATE void MMI_HFPCaHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case HFP_CIEVIND_CALL:
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_NO_ACTIVE)
			{
				MMI_HFP_ClearCallStatus(linkIndex);

				if(!gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
				{
					MMI_PushMediaEvent(MEDIA_EVT_END_OF_CALL);
				}
				//HFP SPEC. 4.15.1 & 4.15.2
				if (MMI_GetTopStateByLink(linkIndex) == MMI_HFP_CALLACTIVE)
				{
					MMI_RmvState(linkIndex, MMI_HFP_CALLACTIVE);
				}
				else if (MMI_GetTopStateByLink(linkIndex) == MMI_HFP_CALLACTIVE_WITHOUT_SCO)
				{
					MMI_RmvState(linkIndex, MMI_HFP_CALLACTIVE_WITHOUT_SCO);
				}
				gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_NO_3WAY;
				MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);
			}
			break;
			
		case HFP_CIEVIND_CALLSETUP:
			if (MMI_HFP_IsSuportAGOptions(linkIndex, MMI_AG_OPTION_THREEWAY_CALLING))
			{
				switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
				{
					case CALLSETUP_INCOMMING:
						MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_INCOMMING);
						//HFP SPEC. 4.22.1
						MMI_AddTopState(linkIndex, MMI_HFP_CAIMG);
						break;

					case CALLSETUP_REMOTE_ALERTED:
						MMI_HFP_GetCallSetupRemoteAlert(linkIndex, TRUE);
					case CALLSETUP_OUTGOING:
						MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_OUTGOING);
						//HFP SPEC. 4.22.2
						MMI_AddTopState(linkIndex, MMI_HFP_CAOGG);
						break;

					case CALLSETUP_NO_CALSETUP:
						//no need to process this case
						gMMI_ctl.msgHandled = NOTPROCD;
						break;
				}
			}
			break;

		case HFP_CIEVIND_CALLHELD:
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld)
			{
				case CALLHELD_ON_HOLD_NOACTIVE:
					MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls(linkIndex);
					break;
				case CALLHELD_CALLS_HELD_ACTIVE:
					/*
					 * added to fix issue #90
					 * in call active state, an outgoing call is accepted by the remote. however callheld = 1 is sent after callsetup = 0
					 * to fix - callheld =0 will be received in call active state, add a new active call
					 */
					if (MMI_HFP_SearchCallStatusInd(linkIndex, MMI_CALL_ACTIVE) == 0)
					{
						MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_HELD);
					}
					 MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_ACTIVE);
					gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_IN_3WAY;
					//HFP SPEC. 4.18 & 4.19 & 4.20
					MMI_AddTopState(linkIndex, MMI_HFP_CAMULTY);
					break;
				case CALLHELD_NO_CALLS_HELD:
					MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld(linkIndex);
					break;
			}
			break;

		case HFP_ATCMD_END_CALL:
			if(gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo)
			{
				if(MMI_HFP_SearchCallStatusInd(linkIndex,MMI_CALL_ACTIVE) < MMI_MAX_ONGOING_CALL_NO)
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_EMPTY);
				else
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_EMPTY);
			}
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

PRIVATE void MMI_HFPCaExit(U8 linkIndex)
{
	if (MMI_AUTO_UNMUTE_ENTER_OR_EXIT_CALL_FEAT)
	{
		if (linkIndex == gMMI_ctl.audioableSCOLink)
		{
			MMI_DRV_AutoMICMuteOff(linkIndex);
		}
		MMI_DRV_MicMuteHandler(linkIndex, SET_MUTE_OFF);
	}

	if(gMMI_ctl.mmiInfo[linkIndex].stateEntryExit & MMI_EXIT_IMGCALL_STATE && !gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
		gMMI_ctl.mmiInfo[linkIndex].stateEntryExit &= ~MMI_EXIT_IMGCALL_STATE;

	if(!gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
		gMMI_ctl.mmiInfo[linkIndex].stateEntryExit |= MMI_EXIT_CALLACTIVE_STATE;
}

///////////////////////////////////////////////////////////////
//	MMI_HFPCaImg Related
///////////////////////////////////////////////////////////////
PRIVATE void MMI_HFPCaImgEntry(U8 linkIndex)
{
	gMMI_HFP_ctl.HfpCtl[linkIndex].isImgVPTriggered = FALSE;

	if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
		return;

	MMI_HFP_ImgAudioVoiceHandler(linkIndex, IMG_AUDIO_VOICE_TRIGGER);

	MMI_HFP_SendATCmdByLink(linkIndex, HFP_CMD_FROM_MMI_RING_GET);

	gMMI_HFP_ctl.incomingLinkInd = linkIndex;
	MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);
}

PRIVATE void MMI_HFPCaImgHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case HFP_CIEVIND_CALL:
			//call = 0 then call = 1 might be sent if call waiting happens and chld = 1 is sent by AG
			//no need to process this message but it is not allowed to pass throught. it will not influence MMI state
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE)
			{
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_INCOMMING, MMI_CALL_ACTIVE);
			}
			else
			{				
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_EMPTY);
				MMI_RmvState(linkIndex, MMI_HFP_CAIMG);
				//mantis 8867: Ca+Img --> Call drop --> no ring in Img: to cover call = 0 and then call = 1 are sent in CaImgState
				MMI_ReplaceStateWithEntry(linkIndex, MMI_HFP_INCOMMING, MMI_GetTopStateByLink(linkIndex), FALSE);
				//MMI_PushMediaEvent(MEDIA_EVT_END_OF_CALL); // mantis 8831
				//mantis 8867: Ca+Img --> Call drop --> no ring in Img: to cover call = 0 and then call = 1 are sent in CaImgState
				SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitCaImgRmvCallTransitTimer, HALF_SEC);
				
				gMMI_ctl.msgHandled = NOTPROCD;
				gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_NO_3WAY;
			}
			break;
			
		case HFP_CIEVIND_CALLHELD:
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld)
			{
				case CALLHELD_CALLS_HELD_ACTIVE:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_HELD);
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_INCOMMING, MMI_CALL_ACTIVE);
					break;

				case CALLHELD_NO_CALLS_HELD:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_ACTIVE);
					break;
				case CALLHELD_ON_HOLD_NOACTIVE:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_HELD);
					break;
			}
			break;
			
		case HFP_CIEVIND_CALLSETUP:
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup == CALLSETUP_NO_CALSETUP)
			{
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_INCOMMING, MMI_CALL_EMPTY);
				if(gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd == MMI_CALL_IN_3WAY)
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_ACTIVE);
				MMI_RmvState(linkIndex, MMI_HFP_CAIMG);
				if(gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo>1)
				{
					MMI_AddTopState(linkIndex, MMI_HFP_CAMULTY);
					MMI_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
				}
				else
				{
					MMI_AddTopState(linkIndex, (SCO_EXISTS(linkIndex))?MMI_HFP_CALLACTIVE:MMI_HFP_CALLACTIVE_WITHOUT_SCO);			
				}
			}
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI_HFPCaOgg Related
///////////////////////////////////////////////////////////////
PRIVATE void MMI_HFPCaOggEntry(U8 linkIndex)
{
	if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
		return;
	MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);
}

PRIVATE void MMI_HFPCaOggHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case HFP_CIEVIND_CALL:
			if(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE)
			{
				//Some AGs send Call = 1 then Callsetup = 0 while remote party answer
				//the phone call. However, all AGs send Callsetup = 0 to indicate the end of
				//this outgoing call, that means adding state 'MultiActiveCall' when we receive
				//Callsetup = 0 is not going to work. To get into state MultiActiveCall, we should
				//regard Call = 1 as a signal for devices such as Samsung C3050c & MTK (Golden).
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_OUTGOING, MMI_CALL_ACTIVE);
			}
			else
			{
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_EMPTY);
				MMI_RmvState(linkIndex, MMI_HFP_CAOGG);
				MMI_ReplaceStateWithEntry(linkIndex, MMI_HFP_OUTGOING, MMI_GetTopStateByLink(linkIndex), FALSE);
				//MMI_PushMediaEvent(MEDIA_EVT_END_OF_CALL); // mantis 8831
				
				gMMI_ctl.msgHandled = NOTPROCD;
				gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_NO_3WAY;
			}
			break;	

		case HFP_CIEVIND_CALLHELD:
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld)
			{
				case CALLHELD_CALLS_HELD_ACTIVE:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_HELD);
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_OUTGOING, MMI_CALL_ACTIVE);
					break;

				case CALLHELD_NO_CALLS_HELD:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_ACTIVE);
					break;
				case CALLHELD_ON_HOLD_NOACTIVE:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_HELD);
					break;
			}
			break;


			
		case HFP_CIEVIND_CALLSETUP:
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup == CALLSETUP_NO_CALSETUP)
			{
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_OUTGOING, MMI_CALL_EMPTY);
				if(gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd == MMI_CALL_IN_3WAY)
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_EMPTY, MMI_CALL_ACTIVE);
				MMI_RmvState(linkIndex, MMI_HFP_CAOGG);
				if(gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo>1)
				{
					MMI_AddTopState(linkIndex, MMI_HFP_CAMULTY);
					MMI_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
				}
				else
				{
					MMI_AddTopState(linkIndex, (SCO_EXISTS(linkIndex))?MMI_HFP_CALLACTIVE:MMI_HFP_CALLACTIVE_WITHOUT_SCO);			
				}
			}
			break;

		case HFP_ATCMD_END_CALL:
			MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_OUTGOING, MMI_CALL_EMPTY);
			MMI_RmvState(linkIndex, MMI_HFP_CAOGG);
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI_HFPCaMulty Related
///////////////////////////////////////////////////////////////
PRIVATE void MMI_HFPCaMultyEntry(U8 linkIndex)
{
	if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
		return;
	MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);
}

PRIVATE void MMI_HFPCaMultyHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	gMMI_ctl.msgHandled = PROCD_RELEASE;
	switch(msgPtr->eventCode)
	{
		case HFP_CIEVIND_CALL:
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_NO_ACTIVE)
			{
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_EMPTY);
				MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_EMPTY);

				//HFP SPEC. 4.15.1 & 4.15.2
				MMI_RmvState(linkIndex, MMI_HFP_CAMULTY);
				MMI_RmvState(linkIndex, MMI_HFP_CALLACTIVE);	//temporarily modified for issue 6380
				MMI_RmvState(linkIndex, MMI_HFP_CALLACTIVE_WITHOUT_SCO);	//modified for issue 9995

				if(!gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey) 
				{
					MMI_PushMediaEvent(MEDIA_EVT_END_OF_CALL);
				}

				MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);
				//pass down to call active
				gMMI_ctl.msgHandled = NOTPROCD;
				gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_NO_3WAY;
			}
			break;

		case HFP_CIEVIND_CALLHELD:
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld)
			{
				case CALLHELD_ON_HOLD_NOACTIVE:
					MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_EMPTY);
					MMI_RmvState(linkIndex, MMI_HFP_CAMULTY);
					break;
				case CALLHELD_NO_CALLS_HELD:
					if(gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd == MMI_CALL_NO_3WAY)
					{
						if( gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus == MMI_CALL_HELD && gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus == MMI_CALL_HELD)
						{
							MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_ACTIVE);
							MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_EMPTY);
						}
						else
						{
							MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_EMPTY);
							MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_EMPTY);
						}
					}
					else
					{
						if( gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus == MMI_CALL_HELD && gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus == MMI_CALL_HELD ||
							(gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus == MMI_CALL_ACTIVE && gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus == MMI_CALL_HELD) ||
							(gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus == MMI_CALL_HELD && gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus == MMI_CALL_ACTIVE))
						{
							MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_EMPTY);
						}
						else
						{
							MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_EMPTY);
							MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_EMPTY);
						}
					}
					//HFP SPEC. 4.15.1 & 4.15.2
					//check the active call no
					//go back to CALLACTIVE if call no is 1
					//if (gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo == 1)
					{
						MMI_RmvState(linkIndex, MMI_HFP_CAMULTY);
					}
					break;

				case CALLHELD_CALLS_HELD_ACTIVE:
					
					if (MMI_HFP_SearchCallStatusInd(linkIndex, MMI_CALL_ACTIVE) == 0)
					{
						MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_ACTIVE);
						MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_HELD);
					}
					else
					{
						MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_HELD);
						MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_HELD, MMI_CALL_ACTIVE);
					}
					break;
			}
			break;

		case HFP_ATCMD_END_CALL:
			MMI_HFP_TransitCallStatus(linkIndex, MMI_CALL_ACTIVE, MMI_CALL_EMPTY);
			break;

		//For special case: Some AGs may not send any result code, if AG answers the second call.
		//Finally, we should receive HFP_CIEVIND_CALLSETUP=CALLSETUP_NO_CALSETUP in ca+incomming state.
		//But this solution will have a side effect(ISSUE:0000192), we need to check
		// incomming & outgoing events in multy-call state.
		case HFP_CIEVIND_CALLSETUP:
			/*
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
			{
				case CALLSETUP_INCOMMING:
					//HFP SPEC. 4.22.1
					MMI_RmvState(linkIndex, MMI_HFP_CAMULTY);
					MMI_AddTopState(linkIndex, MMI_HFP_CAIMG);
					
					

					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[MMI_HFP_SearchCallStatusInd(linkIndex,MMI_CALL_EMPTY)].callStatus = MMI_CALL_INCOMMING;
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo--;

					break;

				case CALLSETUP_OUTGOING:
					//HFP SPEC. 4.22.2
					//!!! maybe we need to check the indicator "callheld"
					MMI_RmvState(linkIndex, MMI_HFP_CAMULTY);
					MMI_AddTopState(linkIndex, MMI_HFP_CAOGG);

					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[MMI_HFP_SearchCallStatusInd(linkIndex,MMI_CALL_EMPTY)].callStatus = MMI_CALL_OUTGOING;
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo--;

					break;

				case CALLSETUP_REMOTE_ALERTED:
					MMI_HFP_GetCallSetupRemoteAlert(linkIndex, TRUE);
				case CALLSETUP_NO_CALSETUP:
					//no need to process this case
					gMMI_ctl.msgHandled = NOTPROCD;
					break;
			} */
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI Handler Tables
///////////////////////////////////////////////////////////////
PRIVATE MmiHandlerSetType CODE	MMI_StateHFPConnectedHandler = 
{
	MMI_ConnectedEntry, MMI_ConnectedHandler, MMI_ConnectedExit, //MMI_CONNECTED
};

PRIVATE MmiHandlerSetType CODE	MMI_StateHFPImgHandler = 
{
	MMI_HFPImgEntry, MMI_HFPImgHandler, MMI_HFPImgExit, //MMI_HFP_INCOMMING
};

PRIVATE MmiHandlerSetType CODE	MMI_StateHFPOggHandler =
{
	MMI_HFPOggEntry, MMI_HFPOggHandler, NULL, //MMI_HFP_OUTGOING
};

PRIVATE MmiHandlerSetType CODE	MMI_StateHFPCaHandler =
{
	MMI_HFPCaEntry, MMI_HFPCaHandler, MMI_HFPCaExit, //MMI_HFP_CALLACTIVE
};

PRIVATE MmiHandlerSetType CODE	MMI_StateHFPCaImgHandler =
{
	MMI_HFPCaImgEntry, MMI_HFPCaImgHandler, MMI_HFPImgExit, //MMI_HFP_CAIMG
};

PRIVATE MmiHandlerSetType CODE	MMI_StateHFPCaOggHandler =
{
	MMI_HFPCaOggEntry, MMI_HFPCaOggHandler, NULL, //MMI_HFP_CAOGG
};

PRIVATE MmiHandlerSetType CODE	MMI_StateHFPCaMultyHandler =
{
	MMI_HFPCaMultyEntry, MMI_HFPCaMultyHandler, NULL, //MMI_HFP_CAMULTY
};

extern MmiHandlerSetType CODE_PTR XDATA MMI_StateHandlerSet[MMI_TOTAL_STATE_NO];

PUBLIC void MMI_HFP_Init_HF(void)
{
	U8 linkIndex;
	
	gMMI_HFP_ctl.incomingLinkInd = MMI_EOF;
	gMMI_HFP_ctl.rdialLinkInd = MMI_EOF;
	
	MMI_StateHandlerSet[MMI_CONNECTED] = &MMI_StateHFPConnectedHandler;
	MMI_StateHandlerSet[MMI_HFP_INCOMMING] = &MMI_StateHFPImgHandler;
	MMI_StateHandlerSet[MMI_HFP_OUTGOING] = &MMI_StateHFPOggHandler;
	MMI_StateHandlerSet[MMI_HFP_CALLACTIVE] = &MMI_StateHFPCaHandler;
	MMI_StateHandlerSet[MMI_HFP_CALLACTIVE_WITHOUT_SCO] = &MMI_StateHFPCaHandler;
	MMI_StateHandlerSet[MMI_HFP_CAIMG] = &MMI_StateHFPCaImgHandler;
	MMI_StateHandlerSet[MMI_HFP_CAOGG] = &MMI_StateHFPCaOggHandler;
	MMI_StateHandlerSet[MMI_HFP_CAMULTY] = &MMI_StateHFPCaMultyHandler;
		
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		#ifdef _EXTENSION_ENTITIES_
		gMMI_HFP_ctl.HfpCtl[linkIndex].DTMFPara = '0'; 
		#endif
		#ifdef _MEMORY_DIALING_
		gMMI_HFP_ctl.HfpCtl[linkIndex].memoryDialIndex = 1; //[0-65535]
		#endif
	}
}

#ifdef SUPPORT_VOICE_PROMPT
PRIVATE void MMI_HFP_SendCallNumVoicePromptCmd(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 tempLength, validDigitCnt;

	if ((msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_VOICEPROMPT_CMD)) == (MMIMsgType XDATA_PTR)NULL)
		return;

	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id = MEDIA_EVT_ENTER_INCOMING;
	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpIndex = VP_EVT_CALLER_ID;
	tempLength = 0;
	validDigitCnt = 0;
	
	while(tempLength < gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length)
	{
		if(gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.numbers[tempLength] >= '0' && gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.numbers[tempLength] <= '9')
		{
			msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.callerIDData.callerID[validDigitCnt] = gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.numbers[tempLength]  - '0';
			validDigitCnt ++;
		}
		tempLength++;
	}
	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.callerIDData.callerIdLength = validDigitCnt;
	OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HFP_SendCallerNameVPCmd(void)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_VOICEPROMPT_CMD)) == (MMIMsgType XDATA_PTR)NULL)
		return;

	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.id = MEDIA_EVT_ENTER_INCOMING;
	msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpIndex = VP_EVT_CALLER_NAME;
	OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
}
#endif


#define IMG_AUDIO_VOICE_LOAD_OK				0x00
#define IMG_AUDIO_VOICE_LOAD_STATE_ERROR	0x01
#define IMG_AUDIO_VOICE_LOAD_PEDNING		0x02
#define IMG_AUDIO_VOICE_LOAD_INVALID		0xFF

#ifdef BLE_AIRApp
PRIVATE BOOL MMI_HFP_IsBleAppCallerNameOn(U8 linkIndex)
{
	if(AirApp_BleIsConnected() && (ATT_FindServerIndexByAddr((U8 CODE_PTR)&AppCtl.phoneNumber) != 0) && ((((ATT_GetCCCDByLink(ATT_FindServerIndexByAddr((U8 CODE_PTR)AppCtl.phnoeNumber_CCCD), 0)) >> 8) & 0x01) == 0x01) && SCO_EXISTS(linkIndex))
	{
		return TRUE;
	}
	return FALSE;
}
#endif

PRIVATE U8 MMI_HFP_IncomingVPHandler(U8 linkIndex)
{
	if(!gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry)
		return IMG_AUDIO_VOICE_LOAD_STATE_ERROR;
		
	#ifdef BLE_AIRApp
	if(MMI_HFP_IsBleAppCallerNameOn(linkIndex))
	{
		if(gMMI_HFP_ctl.HfpCtl[linkIndex].isImgVPTriggered)
			return IMG_AUDIO_VOICE_LOAD_INVALID;	
	}
	#endif
		
	MMI_PushMediaEvent(MEDIA_EVT_ENTER_INCOMING);
	gMMI_HFP_ctl.HfpCtl[linkIndex].isImgVPTriggered = TRUE;
	
	return IMG_AUDIO_VOICE_LOAD_OK;
}

PRIVATE U8 MMI_HFP_CallerNameHandler(U8 linkIndex)
{
	if (MMI_HFP_INCOMMING != MMI_GetTopStateByLink(linkIndex))
		return IMG_AUDIO_VOICE_LOAD_STATE_ERROR;
		
	if(MMI_HFP_IsHSPConnected(linkIndex))
		return IMG_AUDIO_VOICE_LOAD_INVALID;

	if(DRIVER_Query_IsDSPSuspending())
		return IMG_AUDIO_VOICE_LOAD_PEDNING;

	if(linkIndex != gMMI_ctl.serviceInd && MMI_IGNORE_CALLER_VOICE_IF_OTHER_LINK_IN_CALL_FEAT) // for 1 to 2 incomming both --> caller name is valid in only service link
		return IMG_AUDIO_VOICE_LOAD_INVALID;

	if(MMI_IsProfileConnected(linkIndex, PROFILE_AIR_APP))
	{
		if(gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerNameGet)
		{
			#ifdef SUPPORT_VOICE_PROMPT
			LightDbgPrint("Call Name!!!!");
			MMI_HFP_SendCallerNameVPCmd();
			return IMG_AUDIO_VOICE_LOAD_OK;
			#endif
		}
	}
	#ifdef BLE_AIRApp
	else if(MMI_HFP_IsBleAppCallerNameOn(linkIndex))
	{
		LightDbgPrint("Call Name_BLE:%d!!!!", (U8)(gMMI_nvram.timerCtl.callerIDRepeatTime/10));
		return IMG_AUDIO_VOICE_LOAD_OK;
	}
	#endif
	
	return IMG_AUDIO_VOICE_LOAD_INVALID;
}

PRIVATE U8 MMI_HFP_CallerIDHandler(U8 linkIndex)
{
	if(MMI_HFP_INCOMMING != MMI_GetTopStateByLink(linkIndex))
		return IMG_AUDIO_VOICE_LOAD_STATE_ERROR;

	if(MMI_HFP_IsHSPConnected(linkIndex))
		return IMG_AUDIO_VOICE_LOAD_INVALID;

	if(DRIVER_Query_IsDSPSuspending())
		return IMG_AUDIO_VOICE_LOAD_PEDNING;
	
	if(linkIndex != gMMI_ctl.serviceInd && MMI_IGNORE_CALLER_VOICE_IF_OTHER_LINK_IN_CALL_FEAT) // for 1 to 2 incomming both --> caller name is valid in only service link
		return IMG_AUDIO_VOICE_LOAD_INVALID;
	
	#ifdef BLE_AIRApp
	if(MMI_HFP_IsBleAppCallerNameOn(linkIndex))
		return IMG_AUDIO_VOICE_LOAD_INVALID;
	#endif
	
	if(MMI_IsProfileConnected(linkIndex, PROFILE_AIR_APP) && gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerNameGet)
		return IMG_AUDIO_VOICE_LOAD_INVALID;
	
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet)
	{
		U8 tempLength, validDigitCnt;
		if(gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length == 0)
			return IMG_AUDIO_VOICE_LOAD_INVALID;

		validDigitCnt = 0;
		tempLength = gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length;		

		while(tempLength--)
		{
			if(gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.numbers[tempLength-1] >= '0' && gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.numbers[tempLength-1] <= '9')
				validDigitCnt ++;
		}

		if(validDigitCnt == 0)
			return IMG_AUDIO_VOICE_LOAD_INVALID;
			
		if (MMI_READ_OUT_CALLER_ID_BY_VP_FEAT)
		{
			#ifdef SUPPORT_VOICE_PROMPT
			LightDbgPrint("Call ID!!!!");
			MMI_HFP_SendCallNumVoicePromptCmd(linkIndex);
			return IMG_AUDIO_VOICE_LOAD_OK;
			#endif
		}
	}
	return IMG_AUDIO_VOICE_LOAD_INVALID;
}

PRIVATE U8 MMI_HFP_RingHandler(U8 linkIndex)
{
	U8 nextLink;
	BOOL isInCall;
	if(!gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry)
		return IMG_AUDIO_VOICE_LOAD_STATE_ERROR;
		
	if(MMI_PLAY_DEFAULT_RING_INCOMING_FEAT || MMI_HFP_IsHSPConnected(linkIndex))
		goto DECIDE_RING_MEDIA;
		
	#ifdef BLE_AIRApp
	if(MMI_HFP_IsBleAppCallerNameOn(linkIndex) && linkIndex == gMMI_ctl.serviceInd)
		return IMG_AUDIO_VOICE_LOAD_INVALID;
	#endif

	nextLink = MMI_HFP_GetNextHFPLink(linkIndex);
	if(nextLink != MMI_EOF && MMI_CheckLinkInCallRelatedState(nextLink))
	{
		isInCall = TRUE;
		goto RING_START;
	}

	//BRSF sent by Nokia 2630 indicates that it supports in-band ringing.
	//However, in its MUTE mode, 2630 won't establish SCO conection with us.
	//So we can't hear in-band ringtone as well as local alerting ringtone.
	if (!MMI_HFP_IsSuportAGOptions(linkIndex, MMI_AG_OPTION_INBAND_RINGTONE) ||
		(MMI_HFP_IsSuportAGOptions(linkIndex, MMI_AG_OPTION_INBAND_RINGTONE) && !SCO_EXISTS(linkIndex)))
	{
		DECIDE_RING_MEDIA:
		nextLink = MMI_HFP_GetNextHFPLink(linkIndex);

		if(MMI_CheckLinkInCallActiveState(linkIndex) ||(nextLink != MMI_EOF && (MMI_CheckLinkInCallActiveState(nextLink) ||MMI_CheckLinkInOutGoingState(nextLink))) )
			isInCall = TRUE;
		else
			isInCall = FALSE;

		RING_START:
		LightDbgPrint("MMI_HFP_RingHandler!!!,%d",(U8)isInCall);
		if(SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], &gMMI_driver_variation_nvram.misc_para.agInd[0].member[0]))
		{
			MMI_PushMediaEvent((isInCall) ? MEDIA_EVT_RINGTONE_IN_CALL : MEDIA_EVT_RINGTONE);
		}
		else
		{
			MMI_PushMediaEvent((isInCall) ? MEDIA_EVT_RINGTONE2_IN_CALL : MEDIA_EVT_RINGTONE_2);	
		}
		return IMG_AUDIO_VOICE_LOAD_OK;
	}
	
	return IMG_AUDIO_VOICE_LOAD_INVALID;
}

PRIVATE void MMI_HFP_ReleaseConnectionTimer(U8 linkIndex)
{
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].sendBatteryAtCmdTimer);
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].networkServiceIndicatorRepeatTimer);
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].autoAnswerTimer);
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer);
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransferDetachTimer);
	SYS_ReleaseTimer(&gMMI_HFP_ctl.missedCallRemindTimer);
}

PRIVATE U8 MMI_HFP_SwitchLinkToNextHFPLink(U8 linkIndex)
{
	U8 nextActiveLinkIndex;
	if((nextActiveLinkIndex = MMI_GetNextActiveLink(linkIndex)) != MMI_EOF)
	{
		if(MMI_HFP_IsConnected(nextActiveLinkIndex))
			return nextActiveLinkIndex;
	}
	return MMI_EOF;
}

PRIVATE void MMI_HFP_ClearCtlData(U8 linkIndex)
{
	MMI_HFP_ReleaseConnectionTimer(linkIndex);
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_HFP_ctl.HfpCtl[linkIndex], 0, sizeof(MmiHfpCtlType));
	#ifdef _EXTENSION_ENTITIES_
	gMMI_HFP_ctl.HfpCtl[linkIndex].DTMFPara = '0'; 
	#endif
	#ifdef _MEMORY_DIALING_
	gMMI_HFP_ctl.HfpCtl[linkIndex].memoryDialIndex = 1; //[0-65535]
	#endif
	gMMI_HFP_ctl.rdialLinkInd = MMI_HFP_SwitchLinkToNextHFPLink(linkIndex);
	gMMI_HFP_ctl.incomingLinkInd = MMI_HFP_SwitchLinkToNextHFPLink(linkIndex);
	if(gMMI_HFP_ctl.incomingLinkInd != MMI_EOF)
	{
		if(!gMMI_HFP_ctl.HfpCtl[gMMI_HFP_ctl.incomingLinkInd].incomingCall.length)
			gMMI_HFP_ctl.incomingLinkInd = MMI_EOF;
	}
}

PRIVATE void MMI_HFP_VGSHandler(U8 linkIndex, U8 vgsLevel)
{
	U8 newLevel = MMI_SearchSoundLevelByVgs(vgsLevel);

	if(!MMI_CheckSCOInAudioService(linkIndex))
		goto VGS_SET_LEVEL;


	if(MMI_MUTE_CANCEL_WHEN_VOL_CHANGE_FEAT)
	{
		MMI_DRV_AutoMICMuteOff(linkIndex);
	}

	if (!gMMI_ctl.mmiInfo[linkIndex].linkPara.isMicMute || MMI_MUTE_CANCEL_WHEN_VOL_CHANGE_FEAT || MMI_ALLOW_VOL_ADJUST_WHEN_MUTED_FEAT)
	{
		//Notify driver
		MMI_DRV_SendDriverVolCmd(linkIndex, newLevel, CURRENT_SCO_SMALL_STEP(linkIndex), VOLUME_MIC_FOLLOW_SPK);

		if (MMI_PLAY_BEEP_ON_REMOTE_INDICATION_VGS_FEAT)
		{
			MMI_DRV_Beep(newLevel, AUDIO_DEVICE_SCO);
		}
	}

	VGS_SET_LEVEL:
	//Refresh
	MMI_DRV_SetSCOVolValue(linkIndex, newLevel, 0);
	
	if (MMI_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT)
	{
		MMI_LinkKey_SaveHistoryInRunTime(linkIndex);
	}
}

PRIVATE void MMI_HFP_VGMHandler(U8 linkIndex, U8 vgm)
{
	if(!MMI_ENABLE_HANDLING_VGM_FEAT)
		return;

	CURRENT_SCO_VGM(linkIndex) = vgm;

	if(MMI_CheckSCOInAudioService(linkIndex))
	{
		MMI_DRV_SendDriverVolCmd(linkIndex, VOLUME_DONT_CARE, VOLUME_DONT_CARE, GET_SCO_MIC_GAIN_BY_VGM(vgm)); //Notify Driver	
	}
}

PRIVATE void MMI_HFP_PlusClipHandler(U8 linkIndex, HfpToMMIEvtParamType XDATA_PTR hfpParamPtr)
{
	if(hfpParamPtr) //CCWA
	{
		if(OS_memcmp(&gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.numbers[0], (U8 XDATA_PTR)&hfpParamPtr->clipEvt.callNumId[0], gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length))
			gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet = FALSE;
	}

	if(hfpParamPtr && !gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet)
	{
		if(hfpParamPtr->clipEvt.length == 0 || hfpParamPtr->clipEvt.length > CALLER_ID_DIGIT_NO)
		{
			gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length = 0;
		}
		else
		{
			gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length = hfpParamPtr->clipEvt.length;
			OSMEM_memcpy_xdata_xdata(&gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.numbers[0], (U8 XDATA_PTR)&hfpParamPtr->clipEvt.callNumId[0], hfpParamPtr->clipEvt.length);
		}
		#ifdef BLE_AIRApp
		if(gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length && gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry && SDAP_IsIOS(linkIndex))
			SendPhoneNumberToApp(linkIndex, gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length, (U8)(gMMI_nvram.timerCtl.callerIDRepeatTime/10));	
		#endif
		gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet = TRUE;
	}
}

PRIVATE BOOL _SendBatteryATCmd(U8 linkIndex, U8 atCmdCode, U8 tmMask)
{
	if (gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState & tmMask)
	{
		if (gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState & (tmMask >> 4))
		{
			if (MMI_SendATCmdUnderConnection(linkIndex, atCmdCode))
			{
				gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState &= ~tmMask;
				return TRUE;
			}
		}
		else
		{
			gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState &= ~tmMask;
		}
	}
	return FALSE;
}

PRIVATE void MMI_HFP_SendBatteryATCmd(U8 linkIndex)
{
	if (_SendBatteryATCmd(linkIndex, HFP_ATCMD_IPHONE_BATTERY_SEND, MMI_HF_IPHONE_BAT_TM))
		goto label_adjust_battery_timer;

	if (_SendBatteryATCmd(linkIndex, HFP_ATCMD_XEVENT_BATTERY_SEND, MMI_HF_XEVENT_BAT_TM))
		goto label_adjust_battery_timer;

	if (_SendBatteryATCmd(linkIndex, HFP_ATCMD_BIEV_BATTERY_SEND, MMI_HF_BIEV_BAT_TM))
		goto label_adjust_battery_timer;

label_adjust_battery_timer:
	if (gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState & MMI_HF_BAT_TM_MASK)
	{
		SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].sendBatteryAtCmdTimer, (U32)2 * ONE_SEC);
	}
	else
	{
		if (gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState & MMI_HF_BAT_EN_MASK)
		{
			gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState |= (gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState & MMI_HF_BAT_EN_MASK) << 4;
			SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].sendBatteryAtCmdTimer, (U32)75 * ONE_SEC);
		}
	}
}

PRIVATE U8 MMI_HFP_IndicatorEvent(U8 linkIndex, U8 indicator)
{
	U8 i;

	if (indicator == 0)
		return 0;
	
	for (i = 0; i < HFP_CIEVIND_NO; i++)
	{
		if (gMMI_HFP_ctl.HfpCtl[linkIndex].hfpIndicatorMappingTbl[i] == indicator)
			break;
	}
	return (i == HFP_CIEVIND_NO) ? 0 : (i + HFP_CIEVIND_SERVICE);
}

PRIVATE void MMI_HFP_PlusCIEVHandle(U8 linkIndex, U8 index, U8 val)
{
	switch(index)
	{
		//Network signal strength indicator
		case HFP_CIEVIND_SERVICE:
			gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.service = val;
			MMI_PushMediaEvent((val)?MEDIA_EVT_NETWORK_PRESENT:MEDIA_EVT_NETWORK_NOT_PRESENT);
			if(gMMI_nvram.timerCtl.NetworkServiceIndicatorRepeatTime)
			{
				SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].networkServiceIndicatorRepeatTimer, 
							(U32)(gMMI_nvram.timerCtl.NetworkServiceIndicatorRepeatTime) * ONE_SEC);
			}						
			break;
			
		case HFP_CIEVIND_CALL:
			LightDbgPrint("call[%d]:%d",(U8)linkIndex, (U8)val);
			gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call = val;
			break;

		case HFP_CIEVIND_CALLSETUP:
			LightDbgPrint("callsetup[%d]:%d",(U8)linkIndex, (U8)val);
			gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup = val;

			if(MMI_PLAY_DEFAULT_RING_INCOMING_FEAT && gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup == 0 
				&& gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
				SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].rejectCallMuteSCOTimer, 20*320);
			break;

		case HFP_CIEVIND_CALLHELD:
			LightDbgPrint("callheld[%d]:%d",(U8)linkIndex, (U8)val);
			gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld = val;
			break;

		case HFP_CIEVIND_SIGNAL:
			gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.signal = val;
			break;

		case HFP_CIEVIND_ROAM:
			gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.roam = val;
			break;
		
		case HFP_CIEVIND_BATTCHG:
			gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.chgbat = val;
			break;
		
	}
}

PRIVATE void MMI_HFP_SendEvtToStateHandle(U8 linkIndex, U8 evtCode)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if(msgPtr = (MMIMsgType XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
	{
		msgPtr->eventCode = evtCode;
		gMMI_ctl.msgHandled = NOTPROCD;
		MMI_STATE_PassThruMsg(linkIndex, msgPtr);
		if (gMMI_ctl.msgHandled != PROCD_TRANSIT)
		{
			OSMEM_Put ((U8 XDATA_PTR)msgPtr);
		}
	}
}

PUBLIC void MMI_HFP_EventHandler_HF(U8 linkIndex, U8 evtCode, HfpToMMIEvtParamType XDATA_PTR hfpParamPtr)
{
	switch(evtCode)
	{
		//HFP indicator (via +CIEV) inteprator
		case HFP_EVENT_TO_MMI_INDICATOR_UPDATE_INDICATION:
			hfpParamPtr->cievEvt.index = MMI_HFP_IndicatorEvent(linkIndex, hfpParamPtr->cievEvt.index);
			//!!! REMARK !!!
			//This event should be valid if and only if AT+CIND=? has been processed
			//event message is only translated into indicator type, should left to mmi handlers to process.
			MMI_HFP_PlusCIEVHandle(linkIndex, hfpParamPtr->cievEvt.index, hfpParamPtr->cievEvt.val);
			if(hfpParamPtr->cievEvt.index != 0)
			{
				MMI_HFP_SendEvtToStateHandle(linkIndex, hfpParamPtr->cievEvt.index);
			}
			break;
			
		case HFP_EVENT_TO_MMI_INDICATION_VALUES:
			{
				U8 indicatorInd;
				for (indicatorInd = 0; indicatorInd < hfpParamPtr->cindEvt.indicatorNo; indicatorInd++)
				{
					MMI_HFP_PlusCIEVHandle(linkIndex, MMI_HFP_IndicatorEvent(linkIndex, (U8)(indicatorInd + 1)), hfpParamPtr->cindEvt.indicatorVal[indicatorInd]);
				}
			}
			break;
			
		case HFP_EVENT_TO_MMI_SET_INBAND_RING_TONE:
			if (hfpParamPtr->bsirEvt.agSuportIR)
			{
				gMMI_HFP_ctl.HfpCtl[linkIndex].agOption |= MMI_AG_OPTION_INBAND_RINGTONE;
			}
			else
			{		
				gMMI_HFP_ctl.HfpCtl[linkIndex].agOption &= ~MMI_AG_OPTION_INBAND_RINGTONE;
			}
			break;
			
		case HFP_EVENT_TO_MMI_CALL_WAITING_NUMBER:
		case HFP_EVENT_TO_MMI_CALLING_LINE_NUMBER:
			MMI_HFP_PlusClipHandler(linkIndex, hfpParamPtr);
			break;
			
		case HFP_EVENT_TO_MMI_VGS_UPDATE_INDICATION:
			MMI_HFP_VGSHandler(linkIndex, hfpParamPtr->vgsEvt.level);
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_PLUS_VGS, (U8 XDATA_PTR)&hfpParamPtr->vgsEvt.level, linkIndex);
			break;
			
		case HFP_EVENT_TO_MMI_VGM_UPDATE_INDICATION:
			MMI_HFP_VGMHandler(linkIndex, hfpParamPtr->vgmEvt.level);
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_PLUS_VGM, (U8 XDATA_PTR)&hfpParamPtr->vgmEvt.level, linkIndex); 
			break;
			
		case HFP_EVENT_TO_MMI_INDICATOR_ORDER:
			OSMEM_memcpy_xdata_xdata(gMMI_HFP_ctl.HfpCtl[linkIndex].hfpIndicatorMappingTbl, &hfpParamPtr->cievOrderEvt.ind[0], HFP_CIEVIND_NO);
			break;
			
		case HFP_EVENT_TO_MMI_ERROR_RESULT_CODE_RESPONSE:
			MMI_PushMediaEvent(MEDIA_EVT_HFP_ERROR_RECEIVED);
			break;
			
		case HFP_EVENT_TO_MMI_BVRA_INDICATION:
			if (hfpParamPtr->bvraEvt.voiceRecog == 0)
			{
				gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STOP;
				if(gMMI_HFP_ctl.HfpCtl[linkIndex].appleSiriStatus == HFP_APLSIRI_AVAILABLE_ENABLE)
				{
					MMI_PushMediaEvent(MEDIA_EVT_SIRI_END);
				}
			}
			else if (hfpParamPtr->bvraEvt.voiceRecog == 1)
			{
				gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STARTED;
				if(gMMI_HFP_ctl.HfpCtl[linkIndex].appleSiriStatus == HFP_APLSIRI_AVAILABLE_ENABLE)
				{
					MMI_PushMediaEvent(MEDIA_EVT_SIRI_STARTING);
				}
			}
			break;
			
		case HFP_EVENT_TO_MMI_APPLE_SIRI_STATUS_INDICATION:
			gMMI_HFP_ctl.HfpCtl[linkIndex].appleSiriStatus = hfpParamPtr->siriEvt.siriStatus;
			break;

		case HFP_EVENT_TO_MMI_BCS_UPDATE_INDICATION:
			if(hfpParamPtr->bcsEvt.codecType == MMI_EOF)
			{
				MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_AVAILABLE_CODEC);
			}
			else
			{
				gMMI_ctl.mmiInfo[linkIndex].linkPara.scoCodecType = hfpParamPtr->bcsEvt.codecType;
				MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_SELECT_CODEC);
				if(gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer)
				{
					SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer,(U32)gMMI_nvram.timerCtl.estSCOAfterOggCallTime*320L);
				}
			}
			break;
		
		//Cmd response		
		case HFP_ATCMD_ADJUST_VOLUME:
			gMMI_HFP_ctl.HfpCtl[linkIndex].sendingVGSVGM = FALSE;
			break;
			
		case HFP_ATCMD_ADJUST_MIC_GAIN:
			gMMI_HFP_ctl.HfpCtl[linkIndex].sendingVGSVGM = FALSE;
			if(hfpParamPtr->cmdCplEvt.param != VGS_VGM_ERROR)
			{
				CURRENT_SCO_VGM(linkIndex) = hfpParamPtr->cmdCplEvt.param;
				//Notify driver
				MMI_DRV_SendDriverVolCmd(linkIndex, VOLUME_DONT_CARE, VOLUME_DONT_CARE, GET_SCO_MIC_GAIN_BY_VGM(hfpParamPtr->cmdCplEvt.param));
			}
			break;

		case HFP_ATCMD_CODEC_CONNECTION:
			if(hfpParamPtr->cmdCplEvt.param != PROFILE_NO_ERROR)
			{
				MMI_HFP_AudioTransferKeyHandler(linkIndex, FALSE);
			}
			break;

		case HFP_ATCMD_END_CALL:
		case HFP_ATCMD_CANCEL_OUTGOING_CALL:
			if(hfpParamPtr->cmdCplEvt.param == PROFILE_NO_ERROR)
			{
				SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer);
				MMI_HFP_SendEvtToStateHandle(linkIndex, evtCode);
			}
			break;

		case HFP_ATCMD_LAST_NUMBER_REDIAL:
			if(hfpParamPtr->cmdCplEvt.param == PROFILE_NO_ERROR)
			{
				MMI_PushMediaEvent(MEDIA_EVT_REDIAL_SUCCESSFUL_EVT);
				SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].audioDevATCmdTimer, 20 * ONE_SEC);
			}
			else
			{
				MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
				MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
			}
			gMMI_ctl.mmiInfo[linkIndex].subState = MMI_SBY_NONE_SUBSTATE;
			break;

		case HFP_ATCMD_IPHONE_BATTERY_INQUIRY:
			LightDbgPrint("Battery Inquiry Event %d", (U8)hfpParamPtr->cmdCplEvt.param);
			switch (hfpParamPtr->cmdCplEvt.param)
			{
				case HFP_BATTERY_REPORT_NONE:
					break;
				case HFP_BATTERY_REPORT_IPHONE:
					gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState |= MMI_HF_IPHONE_BAT_EN;
					if(MMI_IPHONE_SIRI_FEAT)
					{
						MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_APPLE_SIRI_STATUS);
					}
					break;
				case HFP_BATTERY_REPORT_XEVENT:
					gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState |= MMI_HF_XEVENT_BAT_EN;
					break;
				case HFP_BATTERY_REPORT_BIEV_SLC_0:
				case HFP_BATTERY_REPORT_BIEV_UNS_0:
					gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState &= ~MMI_HF_BIEV_BAT_EN;
					if (hfpParamPtr->cmdCplEvt.param == HFP_BATTERY_REPORT_BIEV_SLC_0)
						return;	//don't start timer until SLC establish complete
					break;
				case HFP_BATTERY_REPORT_BIEV_SLC_1:
				case HFP_BATTERY_REPORT_BIEV_UNS_1:
					gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState |= MMI_HF_BIEV_BAT_EN;
					if (hfpParamPtr->cmdCplEvt.param == HFP_BATTERY_REPORT_BIEV_SLC_1)
						return;	//don't start timer until SLC establish complete
					break;
				default: //ignore invalid event
					return;
			}
			
			if (gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState & MMI_HF_BAT_EN_MASK)
			{
				gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState |= (gMMI_HFP_ctl.HfpCtl[linkIndex].batReportState & MMI_HF_BAT_EN_MASK) << 4;
				SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].sendBatteryAtCmdTimer, (U32)5);
			}
			else
			{
				SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].sendBatteryAtCmdTimer);
			}
			break;
			
		case HFP_ATCMD_CANCEL_VOICE_DIAL:
			if (MMI_HFP_IsSuportAGOptions(linkIndex, MMI_AG_OPTION_VOICE_RECOGNITION))
			{
				gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STOP;
			}
			MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
			MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
			break;

		case HFP_ATCMD_INIT_VOICE_DIAL:
			if (MMI_HFP_IsSuportAGOptions(linkIndex, MMI_AG_OPTION_VOICE_RECOGNITION))
			{
				if(hfpParamPtr->cmdCplEvt.param == PROFILE_NO_ERROR || hfpParamPtr->cmdCplEvt.param == HFP_EVENT_TO_MMI_BVRA_TIMEOUT)
					gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STARTED;
				else
					gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STOP;
			}
			MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
			MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
			break;
	
		default:
			MMI_HFP_SendEvtToStateHandle(linkIndex, evtCode);
			break;
	}
}

PRIVATE void _HFP_CheckTimer_HF(void)//Mantis 8599 - avoid Warning C183 : Dead Assignment Eliminated
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)			
	{
		if(!(gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallCnt && gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallRemindAttempt))
		{
			gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallCnt = 0;
			gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallRemindAttempt = 0;
		}
		else if(gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallCnt && gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallRemindAttempt)
		{
			if(SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], &gMMI_driver_variation_nvram.misc_para.agInd[0].member[0]))
				MMI_PushMediaEvent(MEDIA_EVT_MISSED_CALL_REMINDER1);
			else
				MMI_PushMediaEvent(MEDIA_EVT_MISSED_CALL_REMINDER2);
			gMMI_HFP_ctl.HfpCtl[linkIndex].missedCallRemindAttempt--;
			SYS_SetTimer(&gMMI_HFP_ctl.missedCallRemindTimer, (U32)(gMMI_nvram.timerCtl.missedCallRemindTimer) * ONE_SEC);
		}
	}	
}

PUBLIC void MMI_HFP_CheckTimer_HF(U8 linkIndex)
{
	U8 nextActiveLink;
	
	if(SYS_IsTimerExpired(&gMMI_HFP_ctl.HfpCtl[linkIndex].sendBatteryAtCmdTimer))
	{
		MMI_HFP_SendBatteryATCmd(linkIndex);
	}
	
	if (SYS_IsTimerExpired(&gMMI_HFP_ctl.HfpCtl[linkIndex].networkServiceIndicatorRepeatTimer))
	{
		if(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.service)
		{
			MMI_PushMediaEvent(MEDIA_EVT_NETWORK_PRESENT);
		}
		else
		{
			MMI_PushMediaEvent(MEDIA_EVT_NETWORK_NOT_PRESENT);
		}
		SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].networkServiceIndicatorRepeatTimer, (U32)(gMMI_nvram.timerCtl.NetworkServiceIndicatorRepeatTime) * ONE_SEC);
	}
	
	if(SYS_IsTimerExpired(&gMMI_HFP_ctl.callerRepeatTimer))
	{
		LightDbgPrint("callerRepeatTimer!!!!");
		MMI_HFP_ImgAudioVoiceHandler(gMMI_HFP_ctl.imgAudioVoiceIndex, IMG_AUDIO_VOICE_TIMER_TO);
	}
	
	if(SYS_IsTimerExpired(&gMMI_HFP_ctl.HfpCtl[linkIndex].autoAnswerTimer))
	{
		nextActiveLink = MMI_GetNextActiveLink(linkIndex);
		if(nextActiveLink == MMI_EOF || (!MMI_CheckLinkInCallActiveState(nextActiveLink) && gMMI_ctl.mmiInfo[nextActiveLink].nestStatePtr->nestState != MMI_HFP_OUTGOING))
		{
			MMI_DRV_KeyEventHandler(linkIndex, KEY_ACPCALL);
			MMI_HFP_SendATCmdByLink(linkIndex, HFP_CMD_FROM_MMI_EXIT_INCOMING);
		}
	}
	
	if(SYS_IsTimerExpired(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer))
	{
		if(MMI_CheckLinkInCallRelatedState(linkIndex))
		{
			MMI_HFP_CreateAudioConnection(linkIndex);
		}
	}
	
	if (SYS_IsTimerExpired(&gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransferDetachTimer))
	{
		//if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
		//{
		//	MMI_ReleaseProfileLinkAndDetach(linkIndex);
		//}
	}
	
	if(SYS_IsTimerExpired(&gMMI_HFP_ctl.missedCallRemindTimer))
	{
		if((gMMI_HFP_ctl.HfpCtl[0].missedCallCnt && gMMI_HFP_ctl.HfpCtl[0].missedCallRemindAttempt) &&
		   (gMMI_HFP_ctl.HfpCtl[1].missedCallCnt && gMMI_HFP_ctl.HfpCtl[1].missedCallRemindAttempt))
		{
			MMI_PushMediaEvent(MEDIA_EVT_MISSED_CALL_REMINDER1);

			MMI_PushMediaEvent(MEDIA_EVT_MISSED_CALL_REMINDER2);

			gMMI_HFP_ctl.HfpCtl[0].missedCallRemindAttempt--;
			gMMI_HFP_ctl.HfpCtl[1].missedCallRemindAttempt--;
			SYS_SetTimer(&gMMI_HFP_ctl.missedCallRemindTimer, (U32)(gMMI_nvram.timerCtl.missedCallRemindTimer) * ONE_SEC);
		}
		else
		{
			_HFP_CheckTimer_HF();
		}
	}
	
	if(SYS_IsTimerExpired(&gMMI_HFP_ctl.HfpCtl[linkIndex].audioDevATCmdTimer))
	{
		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
		MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_KEY_AT_CMD);
	}
	
	if(SYS_IsTimerExpired(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitCaImgRmvCallTransitTimer))
	{
		if(MMI_HFP_INCOMMING == MMI_GetTopStateByLink(linkIndex))
			MMI_HFPImgEntryFromCaImg(linkIndex);
	}

	if(SYS_IsTimerExpired(&gMMI_HFP_ctl.HfpCtl[linkIndex].rejectCallMuteSCOTimer) && MMI_PLAY_DEFAULT_RING_INCOMING_FEAT)
	{
			MMI_DRV_RejectCallMuteSCOCmd(FALSE);
			SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].rejectCallMuteSCOTimer);
	}
}

PRIVATE void MMI_HFP_AudioTransferDetachHandle(U8 linkIndex)
{
	if(MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState())
	{
		// NOTE!!!!This shloud be prior than MMI_HFP_ReleaseConnectionTimer
		if (gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransferDetachTimer)
		{
			gMMI_ctl.miscMask2 |= MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER;
		}
		else
		{
			gMMI_ctl.miscMask2 &= ~MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER;
		}	
	}
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransferDetachTimer);
}

PUBLIC void MMI_HFP_ProfileDiscHandle_HF(U8 linkIndex)
{
	U8 state, next_link;
	
	MMI_HFP_AudioTransferDetachHandle(linkIndex);
	
	next_link = MMI_GetNextActiveLink(linkIndex);
	if(MMI_CheckLinkInIncomingState(linkIndex) && (next_link == MMI_EOF || !MMI_CheckLinkInIncomingState(next_link)))
	{
		MMI_HFP_StopIncomingVPRing(linkIndex);
	}
	
	//!!!!!!Here should be given more strict condion.
	//remove all states besides connectable and fakeon.
	//Please consider the 2 to 1
	state = MMI_GetTopStateByLink(linkIndex);
	if(state >=  MMI_HFP_INCOMMING	&& state <= MMI_HFP_CAMULTY)
	{
		//if(state >= MMI_HFP_CALLACTIVE )
			//MMI_PushMediaEvent(MEDIA_EVT_END_OF_CALL);

		MMI_RmvState(linkIndex, state);
	}
	MMI_HFP_ClearCtlData(linkIndex);
}

PUBLIC void MMI_HSP_ConnectedHandle_HF(U8 linkIndex)
{	
	LinkHistoryType XDATA_PTR linkListPtr= MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], TRUE, FALSE);
	if(linkListPtr != (LinkHistoryType XDATA_PTR)NULL)
	{
		linkListPtr->miscMask |= NVRAM_LINK_KEY_MISC_MASK_HSP;
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}	
	
	//!!! should check the call callsetup status to make state jump decision
	// coze in HSP, only incoming and call active, no difference in logic, so we put this here
	if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_NO_ACTIVE)
	{
		switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
		{
			case CALLSETUP_INCOMMING:
				MMI_AddTopState(linkIndex, MMI_HFP_INCOMMING);
				break;

			case CALLSETUP_REMOTE_ALERTED:
				MMI_HFP_GetCallSetupRemoteAlert(linkIndex, FALSE);
			case CALLSETUP_OUTGOING:
				MMI_AddTopState(linkIndex, MMI_HFP_OUTGOING);
				break;
		}
	}
	else // gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE
	{
		if (SCO_EXISTS(linkIndex))
		{
			MMI_AddTopState(linkIndex, MMI_HFP_CALLACTIVE);
		}
		else
		{
			MMI_AddTopState(linkIndex, MMI_HFP_CALLACTIVE_WITHOUT_SCO);
			// Add this coze maybe AG doesnt init esting sco right after connection.
			MMI_HFP_GetCallSetupRemoteAlert(linkIndex, FALSE);
		}
		MMI_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);

		if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld == CALLHELD_NO_CALLS_HELD)
		{
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
			{
				case CALLSETUP_INCOMMING:
					MMI_AddTopState(linkIndex, MMI_HFP_CAIMG);
					break;
				case CALLSETUP_OUTGOING:
					MMI_AddTopState(linkIndex, MMI_HFP_CAOGG);
					break;
				default:
					break;
			}
		}
		else
		{
			MMI_AddTopState(linkIndex, MMI_HFP_CAMULTY);
			gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 2;
		}
	}
	//Send VGS according to user setting to VG
	MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_ADJUST_VOLUME);
}

PUBLIC void MMI_HFP_ConnectedHandle_HF(U8 linkIndex)
{	
	LinkHistoryType XDATA_PTR linkListPtr = MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], TRUE, FALSE);
	if(linkListPtr != (LinkHistoryType XDATA_PTR)NULL)
	{
		linkListPtr->miscMask |= NVRAM_LINK_KEY_MISC_MASK_HFP;
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
	
	/* redial link: first connected, first assigned */
	if(gMMI_HFP_ctl.rdialLinkInd == MMI_EOF)
	{
		gMMI_HFP_ctl.rdialLinkInd = linkIndex;
	}

	//!!! should check the call callsetup status to make state jump decision
	// coze in HSP, only incoming and call active, no difference in logic, so we put this here
	if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_NO_ACTIVE)
	{
		switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
		{
			case CALLSETUP_INCOMMING:
				gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus = MMI_CALL_INCOMMING;
				gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 1;
				MMI_AddTopState(linkIndex, MMI_HFP_INCOMMING);
				break;

			case CALLSETUP_REMOTE_ALERTED:
				MMI_HFP_GetCallSetupRemoteAlert(linkIndex, FALSE);
			case CALLSETUP_OUTGOING:
				gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus = MMI_CALL_OUTGOING;
				gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 1;
				MMI_AddTopState(linkIndex, MMI_HFP_OUTGOING);
				break;
		}
	}
	else // gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.call == CALL_ACTIVE
	{
		if (SCO_EXISTS(linkIndex))
		{
			MMI_AddTopState(linkIndex, MMI_HFP_CALLACTIVE);
		}
		else
		{
			MMI_AddTopState(linkIndex, MMI_HFP_CALLACTIVE_WITHOUT_SCO);
			// Add this coze maybe AG doesnt init esting sco right after connection.
			MMI_HFP_GetCallSetupRemoteAlert(linkIndex, FALSE);
		}
		MMI_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);

		if (gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld == CALLHELD_NO_CALLS_HELD)
		{
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
			{
				case CALLSETUP_INCOMMING:
					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus = MMI_CALL_ACTIVE;
					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus = MMI_CALL_INCOMMING;
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 2;
					MMI_AddTopState(linkIndex, MMI_HFP_CAIMG);
					break;
				case CALLSETUP_OUTGOING:
					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus = MMI_CALL_ACTIVE;
					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus = MMI_CALL_OUTGOING;
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 2;
					MMI_AddTopState(linkIndex, MMI_HFP_CAOGG);
					break;
				case CALLSETUP_NO_CALSETUP:
					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus = MMI_CALL_ACTIVE;
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 1;
					break;
				default:
					break;
			}
		}
		else if(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld == CALLHELD_CALLS_HELD_ACTIVE)
		{
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
			{
				case CALLSETUP_NO_CALSETUP:
					gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_IN_3WAY;
					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus = MMI_CALL_ACTIVE;
					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus = MMI_CALL_HELD;
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 2;
					MMI_AddTopState(linkIndex, MMI_HFP_CAMULTY);
					break;
				case CALLSETUP_INCOMMING:
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 1;
					MMI_AddTopState(linkIndex, MMI_HFP_CAIMG);
					break;
				case CALLSETUP_OUTGOING:
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo = 1;
					MMI_AddTopState(linkIndex, MMI_HFP_CAOGG);
					break;
			}
		}
	}
	
	if(MMI_CheckLinkInCallRelatedState(linkIndex))
		return;

	if(MMI_IsLinkActiveConnected(linkIndex))
	{
		switch(gMMI_HFP_ctl.HfpCtl[linkIndex].reconnectReason)
		{
			case RECONNECT_LAST_ONCE_REDIAL:
			case RECONNECT_FIRST_ONCE_REDIAL:
				gMMI_HFP_ctl.rdialLinkInd = linkIndex;
				MMI_DRV_KeyEventHandler(linkIndex, KEY_RDIAL);
				break;
			case RECONNECT_SECONDARY_ONCE_REDIAL:
				gMMI_HFP_ctl.rdialLinkInd = linkIndex;
				MMI_DRV_KeyEventHandler(linkIndex, KEY_RDIAL_SECONDARY);
				break;
			case RECONNECT_LAST_ONCE_VOICE_DIAL:
			case RECONNECT_FIRST_ONCE_VOICE_DIAL:
				gMMI_HFP_ctl.rdialLinkInd = linkIndex;
				MMI_DRV_KeyEventHandler(linkIndex, KEY_SET_VOICE_RECOGNITION);
				break;
			case RECONNECT_SECONDARY_ONCE_VOICE_DIAL:
				gMMI_HFP_ctl.rdialLinkInd = linkIndex;
				MMI_DRV_KeyEventHandler(linkIndex, KEY_SET_VOICE_RECOGNITION_SECONDARY);
				break;
			case RECONNECT_LAST_ONCE_AUDIOTRANSFER:
				SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer);
				break;
		}
	}
	else if(MMI_CheckLinkInCallRelatedState(linkIndex))
	{
		gMMI_ctl.mmiInfo[linkIndex].subState = MMI_SBY_NONE_SUBSTATE;
	}
	else
	{
		if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_CONNECTING_HFP)
		{
			MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_INIT_VOICE_DIAL);
		}
		else if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_RDIAL_CONNECTING_HFP)
		{
			MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_LAST_NUMBER_REDIAL);
		}
	}
}

PUBLIC void MMI_HFP_AirAppNotifyCallerNameReady(U8 linkIndex)
{
	gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerNameGet = TRUE;
}

PRIVATE void MMI_HFP_ImgLoadAudioVoice(void)
{
	U8 result = IMG_AUDIO_VOICE_LOAD_STATE_ERROR;
	switch(gMMI_HFP_ctl.imgAudioVoiceState)
	{
		case MMI_HFP_INCOMING_VP:
			result = MMI_HFP_IncomingVPHandler(gMMI_HFP_ctl.imgAudioVoiceIndex);
			break;
		case MMI_HFP_CALLER_NAME:
			result = MMI_HFP_CallerNameHandler(gMMI_HFP_ctl.imgAudioVoiceIndex);
			break;
		case MMI_HFP_CALLER_ID:
			result = MMI_HFP_CallerIDHandler(gMMI_HFP_ctl.imgAudioVoiceIndex);
			break;
		case MMI_HFP_RINGTONE:
			result = MMI_HFP_RingHandler(gMMI_HFP_ctl.imgAudioVoiceIndex);
			break;	
	}
	if(result == IMG_AUDIO_VOICE_LOAD_STATE_ERROR)
	{
		// cancel this link
		gMMI_HFP_ctl.imgAudioVoiceState = MMI_HFP_NONE;
		MMI_DRV_SendFakeMediaCmd(MEDIA_EVT_ENTER_INCOMING);
	}
	else if(result == IMG_AUDIO_VOICE_LOAD_PEDNING)
	{
		SYS_SetTimer(&gMMI_HFP_ctl.callerRepeatTimer, 320L);			
	}
	else if(result == IMG_AUDIO_VOICE_LOAD_OK)
	{
		if(gMMI_HFP_ctl.imgAudioVoiceState != MMI_HFP_INCOMING_VP && gMMI_HFP_ctl.imgAudioVoiceState != MMI_HFP_RINGTONE)
			MMI_DRV_SendFakeMediaCmd(MEDIA_EVT_ENTER_INCOMING);
	}
	else
	{
		//decide audio voice
		MMI_DRV_SendFakeMediaCmd(MEDIA_EVT_ENTER_INCOMING);
	}
}

PUBLIC void MMI_HFP_ImgAudioVoiceHandler(U8 linkIndex, U8 action)
{	
	if(action == IMG_AUDIO_VOICE_TRIGGER)
	{
		if(gMMI_HFP_ctl.imgAudioVoiceState == MMI_HFP_NONE)
		{	
			gMMI_HFP_ctl.imgAudioVoiceIndex = linkIndex;
			gMMI_HFP_ctl.imgAudioVoiceState = gMMI_nvram.incomingCallVoicePriority[0];
			SYS_SetTimer(&gMMI_HFP_ctl.callerRepeatTimer, 3 * ONE_SEC); // wait BSIR and CLIP and SCO --> 2 seconds
		}
	}
	else if(action == IMG_AUDIO_VOICE_FAKE_EVT)
	{
		U8 i, nextLink;
		if(gMMI_HFP_ctl.imgAudioVoiceState == MMI_HFP_NONE)
		{
			nextLink = MMI_GetNextNormalLink(gMMI_HFP_ctl.imgAudioVoiceIndex);
			if(MMI_EOF != nextLink && MMI_CheckLinkInIncomingState(nextLink))
			{
				gMMI_HFP_ctl.imgAudioVoiceIndex = nextLink;
				gMMI_HFP_ctl.imgAudioVoiceState = gMMI_nvram.incomingCallVoicePriority[0];
				SYS_SetTimer(&gMMI_HFP_ctl.callerRepeatTimer, gMMI_nvram.timerCtl.callerIDRepeatTime* 320L / 2);
			}
		}
		else
		{	
			i = 0;
			while(i < MMI_HFP_VOICE_NUM)
			{
				if(gMMI_nvram.incomingCallVoicePriority[i] == gMMI_HFP_ctl.imgAudioVoiceState)
				{
					if(i == (MMI_HFP_VOICE_NUM -1) || gMMI_nvram.incomingCallVoicePriority[i+1] == MMI_HFP_NONE)
					{
						nextLink = MMI_GetNextNormalLink(gMMI_HFP_ctl.imgAudioVoiceIndex);
						if(MMI_EOF != nextLink && MMI_CheckLinkInIncomingState(nextLink))
						{
							gMMI_HFP_ctl.imgAudioVoiceIndex = nextLink;
							gMMI_HFP_ctl.imgAudioVoiceState = gMMI_nvram.incomingCallVoicePriority[0];
							SYS_SetTimer(&gMMI_HFP_ctl.callerRepeatTimer, gMMI_nvram.timerCtl.callerIDRepeatTime* 320L / 2);
						}
						#ifdef BLE_AIRApp
						else if(MMI_HFP_IsBleAppCallerNameOn(linkIndex))
						{
							gMMI_HFP_ctl.imgAudioVoiceState = MMI_HFP_NONE;
							SYS_ReleaseTimer(&gMMI_HFP_ctl.callerRepeatTimer);
							gMMI_HFP_ctl.HfpCtl[linkIndex].isImgVPTriggered	= FALSE;					
						}
						#endif
						else if(gMMI_HFP_ctl.HfpCtl[linkIndex].isHFPImgEntry)
						{
							gMMI_HFP_ctl.imgAudioVoiceState = gMMI_nvram.incomingCallVoicePriority[0];
							SYS_SetTimer(&gMMI_HFP_ctl.callerRepeatTimer, gMMI_nvram.timerCtl.callerIDRepeatTime* 320L);
						}
						else
						{
							gMMI_HFP_ctl.imgAudioVoiceState = MMI_HFP_NONE;
							SYS_ReleaseTimer(&gMMI_HFP_ctl.callerRepeatTimer);
							gMMI_HFP_ctl.HfpCtl[linkIndex].isImgVPTriggered	= FALSE;	
						}
					}
					else
					{
						gMMI_HFP_ctl.imgAudioVoiceState = gMMI_nvram.incomingCallVoicePriority[i+1];
						SYS_SetTimer(&gMMI_HFP_ctl.callerRepeatTimer, 320L);
					}
					break;
				}
				i++;
			}
		}
	}
	else if(action == IMG_AUDIO_VOICE_TIMER_TO)
	{
		MMI_HFP_ImgLoadAudioVoice();
	}
}


PUBLIC void MMI_HFP_FakeMediaEvt_HF(U16 id)
{
	if(id == MEDIA_EVT_ENTER_INCOMING || id == MEDIA_EVT_RINGTONE_IN_CALL || id == MEDIA_EVT_RINGTONE || id == MEDIA_EVT_RINGTONE2_IN_CALL || id == MEDIA_EVT_RINGTONE_2)
	{
		MMI_HFP_ImgAudioVoiceHandler(gMMI_HFP_ctl.imgAudioVoiceIndex, IMG_AUDIO_VOICE_FAKE_EVT);
	}
}

