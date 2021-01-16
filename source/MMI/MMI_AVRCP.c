#include "reside_flash.inc"

#define _MMI_AVRCP_C_

#include "rc.h"
#include "bt_config_profile.h"

#ifdef AVRCP_Profile
#include "MMI.h"
#include "MMI_Protocol.h"
#include "MMI_State.h"
#include "MMI_AVRCP.h"
#include "MMI_HCI.h"
#include "..\HC\HC_Interface.h"
#include "MMI_SDAP.h"
#include "MMI_Driver.h"
#include "MMI_A2DP.h"
#include "MMI_AirModule.h"
#include "MMI_LineIn.h"
#include "MMI_HFP.h"
#ifdef FAST_CONNECT_DONGLE
#include "MMI_FCD.h"
#endif
#include "MMI_Reconnect.h"
#include "..\A2DP\A2DP_Interface.h"
#include "..\AVRCP\AVRCP_Interface.h"

#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];

PRIVATE void MMI_AVRCP_RemotePlayStatus(U8 linkIndex, U8 playStatus);
PRIVATE void MMI_AVRCP_TG_GetKeyVolUpDown(U8 linkIndex);
PRIVATE void MMI_AVRCP_TG_GetVolChanged(U8 linkIndex, U8 absVol);
PRIVATE void MMI_AVRCP_TG_GetSetAbsVol(U8 linkIndex, U8 absVol);
PRIVATE void MMI_AVRCP_SetDrvVolume(U8 linkIndex);

PRIVATE void MMI_AVRCP_KeySwitch(U8 linkIndex);
PRIVATE void MMI_AVRCP_KeyLeftChannel(U8 linkIndex);
PRIVATE void MMI_AVRCP_KeyRightChannel(U8 linkIndex);
PRIVATE void MMI_AVRCP_KeyStereoChannel(U8 linkIndex);

PRIVATE void MMI_AVRCP_ClearAllMissions(U8 linkIndex);
PRIVATE void MMI_AVRCP_SendTrackChangeUartEvt(U8 linkIndex, U8 avrcpEventId);
PRIVATE void MMI_AVRCP_SendPlaybackPosUartEvt(U8 linkIndex, U32 PlaybackPos);
PRIVATE void MMI_AVRCP_SendPlayAppSettingsUartInd(U8 linkIndex, U8 attributeID, U8 attributeValue);

static MMI_KEY_EVT CODE MMI_AVRCP_KeyEvtHandler[AVRCP_KEY_EVENT_NO] =
{
	{MMI_AVRCP_KeyPlay}, 			//AVRCP_KEY_EVENT_KEY_AVRCP_PLAY
	{MMI_AVRCP_KeyStop}, 			//AVRCP_KEY_EVENT_KEY_AVRCP_STOP 
	{MMI_AVRCP_KeyPause}, 			//AVRCP_KEY_EVENT_KEY_AVRCP_PAUSE
	{NULL}, 						//AVRCP_KEY_EVENT_KEY_AVRCP_RECORD
	{MMI_AVRCP_KeyRewindPush}, 		//AVRCP_KEY_EVENT_KEY_AVRCP_REWIND_PUSH, 
	{MMI_AVRCP_KeyFastForwardPush}, //AVRCP_KEY_EVENT_KEY_AVRCP_FAST_FORWARD_PUSH,	 	 
	{NULL}, 						//AVRCP_KEY_EVENT_KEY_AVRCP_EJECT,
	{MMI_AVRCP_KeyForward}, 		//AVRCP_KEY_EVENT_KEY_AVRCP_FORWARD,	 				  
	{MMI_AVRCP_KeyBackward}, 		//AVRCP_KEY_EVENT_KEY_AVRCP_BACKWARD,		
	{MMI_AVRCP_KeyRewindRelease}, 	//AVRCP_KEY_EVENT_KEY_AVRCP_REWIND_RELEASE, 
	{MMI_AVRCP_KeyFastForwardRelease}, //AVRCP_KEY_EVENT_KEY_AVRCP_FAST_FORWARD_RELEASE,  
	{MMI_AVRCP_KeyRepeatModeChange}, //AVRCP_KEY_EVENT_KEY_AVRCP_REPEAT_MODE_CHANGE,	 
	{MMI_AVRCP_KeyShuffleModeChange}, //AVRCP_KEY_EVENT_KEY_AVRCP_SHUFFLE_MODE_CHANGE,
	{MMI_AVRCP_KeySwitch}, 			//AVRCP_KEY_EVENT_KEY_AVRCP_ACTIVE_TOGGLE,
	{MMI_AVRCP_KeyLeftChannel}, //AVRCP_KEY_EVENT_KEY_KEY_LEFT_CHANNEL
	{MMI_AVRCP_KeyRightChannel}, //AVRCP_KEY_EVENT_KEY_KEY_RIGHT_CHANNEL
	{MMI_AVRCP_KeyStereoChannel}, //AVRCP_KEY_EVENT_KEY_KEY_STEREO_CHANNEL,
};

PUBLIC void MMI_AVRCP_ProcessKeyEvent(U8 linkIndex, U8 index)
{
	if(MMI_AVRCP_KeyEvtHandler[index].fKeyEvt && MMI_IsNormalLink(linkIndex))
	{
		MMI_AVRCP_KeyEvtHandler[index].fKeyEvt(linkIndex);
	}
}

PRIVATE void MMI_AVRCP_RemoteNotification(U8 linkIndex, AVRCP_TO_MMI_EVT_PARAM XDATA_PTR paramPtr)
{
	U8 i;
	PlayAppElement XDATA_PTR attriPtr;
	
	switch (paramPtr->Notify.eventID)
	{
		case AVRCP_EVENT_ID_PLAYBACK_STATUS_CHANGED:
			{
				MMI_AVRCP_MISSION_PARAM_TYPE LDATA avrcpParam;
				
				MMI_AVRCP_CancelMission(linkIndex, MMI_AVRCP_MISSION_BACKUP_AND_PUSH_PLAY_STATUS);

				avrcpParam.taskId = MMI_AVRCP_MISSION_BACKUP_AND_PUSH_PLAY_STATUS;
				avrcpParam.timer = 160L;
				avrcpParam.fcallback = NULL;
				avrcpParam.PlayStatus = paramPtr->Notify.eventParams.PlayStatus;
				MMI_AVRCP_PushMission(linkIndex, &avrcpParam);
				gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isDisablePlayStatusMonitor = TRUE;
				LightDbgPrint("AVRCP Push:BACKUP_AND_PUSH:%d",(U8)paramPtr->Notify.eventParams.PlayStatus);
			}
			break;
		case AVRCP_EVENT_ID_TRACK_CHANGED:
		case AVRCP_EVENT_ID_TRACK_REACHED_END:
		case AVRCP_EVENT_ID_TRACK_REACHED_START:
			MMI_AVRCP_SendTrackChangeUartEvt(linkIndex, paramPtr->Notify.eventID);
			break;
		case AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED:
			MMI_AVRCP_SendPlaybackPosUartEvt(linkIndex, paramPtr->Notify.eventParams.PlaybackPos);
			break;
		case AVRCP_EVENT_ID_VOLUME_CHANGED:
			MMI_AVRCP_TG_GetVolChanged(linkIndex, paramPtr->Notify.eventParams.AbsoluteVolume);
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportVolume(HCI_VEVT_OCF_FCD_VOL_A2DP, paramPtr->Notify.eventParams.AbsoluteVolume);
			#else
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_SET_ABSOLUTE_VOLUME, (U8 XDATA_PTR)&paramPtr->Notify.eventParams.AbsoluteVolume, linkIndex);
			#endif
			break;
		case AVRCP_EVENT_ID_PLAYER_APPLICATION_SETTING_CHANGED:
			for(i = 0; i < paramPtr->Notify.eventParams.PlayAppSettings.attributeIDNum ; i++)
			{
				attriPtr = &paramPtr->Notify.eventParams.PlayAppSettings.attribute[i];
				
				if(attriPtr->attributeID == AVRCP_REPEAT_MODE_STATUS || attriPtr->attributeID == AVRCP_SHUFFLE_ON_OFF_STATUS)
				{
					MMI_AVRCP_SendPlayAppSettingsUartInd(linkIndex, attriPtr->attributeID, attriPtr->attributeValue);
					if(api_profile_func[API_GRP_PF_CB_AVRCP_PLAY_APP_CHANGED])
					{
						((API_PROFILE_AVRCP_PLAY_APP_CHANGED_FUNC)api_profile_func[API_GRP_PF_CB_AVRCP_PLAY_APP_CHANGED])(linkIndex, attriPtr->attributeID, attriPtr->attributeValue);
					}
				}
			}
			break;
	}
}

PRIVATE void MMI_AVRCP_GetApplicationSettings(U8 linkIndex, AVRCP_TO_MMI_EVT_PARAM XDATA_PTR paramPtr)
{
	if(paramPtr->GetPlayAppSettings.attributeValue == AVRCP_REPEAT_CMD_REJECT)
	{
		switch(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner)
		{
			case AVRCP_GET_APP_SETTINGS_UART:
				MMI_AVRCP_SendPlayAppSettingsUartInd(linkIndex, AVRCP_REPEAT_CMD_REJECT, 0);
				break;
			case AVRCP_GET_APP_SETTINGS_SDK:
				break;
			default:
	
				break;
		}
		gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_NONE;
		return;
	}
	
	if(paramPtr->GetPlayAppSettings.attributeID == AVRCP_SHUFFLE_ON_OFF_STATUS)
	{
		switch(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner)
		{
			case AVRCP_GET_APP_SETTINGS_KEY:
				MMI_AVRCP_SetPlayAppSettings(linkIndex, AVC_OPERATION_SET_SHUFFLE_MODE, (U8)((paramPtr->GetPlayAppSettings.attributeValue%AVRCP_SHUFFLE_MODE_NO)+1));
				break;
			case AVRCP_GET_APP_SETTINGS_UART:
				MMI_AVRCP_SendPlayAppSettingsUartInd(linkIndex, AVRCP_SHUFFLE_ON_OFF_STATUS, paramPtr->GetPlayAppSettings.attributeValue);
				break;
			case AVRCP_GET_APP_SETTINGS_SDK:
				if(api_profile_func[API_GRP_PF_CB_AVRCP_PLAY_APP_CHANGED])
				{
					((API_PROFILE_AVRCP_PLAY_APP_CHANGED_FUNC)api_profile_func[API_GRP_PF_CB_AVRCP_PLAY_APP_CHANGED])(linkIndex, AVRCP_SHUFFLE_ON_OFF_STATUS, paramPtr->GetPlayAppSettings.attributeValue);
				}
				break;
			default:
				break;
		
		}				
	}
	else if(paramPtr->GetPlayAppSettings.attributeID == AVRCP_REPEAT_MODE_STATUS)
	{
		switch(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner)
		{
			case AVRCP_GET_APP_SETTINGS_KEY:
				MMI_AVRCP_SetPlayAppSettings(linkIndex, AVC_OPERATION_SET_REPEAT_MODE, (U8)((paramPtr->GetPlayAppSettings.attributeValue%AVRCP_REPEAT_MODE_NO)+1));
				break;
			case AVRCP_GET_APP_SETTINGS_UART:
				MMI_AVRCP_SendPlayAppSettingsUartInd(linkIndex, AVRCP_REPEAT_MODE_STATUS, paramPtr->GetPlayAppSettings.attributeValue);
				break;
			case AVRCP_GET_APP_SETTINGS_SDK:
				if(api_profile_func[API_GRP_PF_CB_AVRCP_PLAY_APP_CHANGED])
				{
					((API_PROFILE_AVRCP_PLAY_APP_CHANGED_FUNC)api_profile_func[API_GRP_PF_CB_AVRCP_PLAY_APP_CHANGED])(linkIndex, AVRCP_REPEAT_MODE_STATUS, paramPtr->GetPlayAppSettings.attributeValue);
				}
				break;
			default:
				break;
		
		}
	}
	gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_NONE;
}

PUBLIC void MMI_AVRCP_EventHandler(U8 linkIndex, U8 evtCode, AVRCP_TO_MMI_EVT_PARAM XDATA_PTR paramPtr)
{
	#ifdef DBG_UART_ENABLE_MMI
	LightDbgPrint("MMI-AVRCPEvtOPcode[%d]:%x",(U8)linkIndex,(U8)evtCode);
	#endif
	switch(evtCode)
	{	
		case AVRCP_EVT_TO_MMI_GET_STOP_REJECTED:  // the music cant be either paused or stopped.
			#ifdef A2DP_Profile
			MMI_A2DP_ClearResumeState(linkIndex);
			#endif
			break;

		case AVRCP_EVT_TO_MMI_GET_KEY_VOLUME_UP_CMD:
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_AVRCP_UP_DOWN_PASS_THROUGH, &evtCode, linkIndex);
			#ifdef AIR_MODULE
			if(MMI_AIR_TG_GetKeyVolUp(linkIndex))
				break;
			#endif
			if(CURRENT_A2DP_SOUND_LEVEL(linkIndex) < MMI_MAX_MUSIC_SOUND_LEVEL())
			{
				#ifdef A2DP_Profile
				MMI_A2DP_SetVolByLevel(linkIndex, (U8)(CURRENT_A2DP_SOUND_LEVEL(linkIndex) + 1), 0);
				#endif
				MMI_AVRCP_TG_GetKeyVolUpDown(linkIndex);
			}
			break;
		case AVRCP_EVT_TO_MMI_GET_KEY_VOLUME_DOWN_CMD:
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_AVRCP_UP_DOWN_PASS_THROUGH, &evtCode, linkIndex);
			#ifdef AIR_MODULE
			if(MMI_AIR_TG_GetKeyVolDown(linkIndex))
				break;
			#endif
			if(CURRENT_A2DP_SOUND_LEVEL(linkIndex) > 0)
			{
				#ifdef A2DP_Profile
				MMI_A2DP_SetVolByLevel(linkIndex, (U8)(CURRENT_A2DP_SOUND_LEVEL(linkIndex) - 1), 0);
				#endif
				MMI_AVRCP_TG_GetKeyVolUpDown(linkIndex);
			}
			break;

	#ifdef A2DP_SRC_Profile
		case AVRCP_EVT_TO_MMI_GET_KEY_PLAY_CMD:
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportAction(HCI_VEVT_OCF_FCD_PLAY_MUSIC);
			#endif
			MMI_A2DP_SendCmd(linkIndex, A2DP_PLAY_CMD);
			break;
		case AVRCP_EVT_TO_MMI_GET_KEY_STOP_CMD:
		case AVRCP_EVT_TO_MMI_GET_KEY_PAUSE_CMD:
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportAction(HCI_VEVT_OCF_FCD_PAUSE_MUSIC);
			#endif
			MMI_A2DP_SendCmd(linkIndex, A2DP_PAUSE_CMD);
			break;
		case AVRCP_EVT_TO_MMI_GET_KEY_FORWARD_CMD:
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportAction(HCI_VEVT_OCF_FCD_NEXT_TRACK);
			#endif
			break;
		case AVRCP_EVT_TO_MMI_GET_KEY_BACKWARD_CMD:
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportAction(HCI_VEVT_OCF_FCD_PREV_TRACK);
			#endif
			break;
	#endif
	
		case AVRCP_EVT_TO_MMI_GET_KEY_STOP_RESP:
		case AVRCP_EVT_TO_MMI_GET_KEY_PAUSE_RESP:
			MMI_AVRCP_SetMusicPlay(linkIndex, FALSE);
			break;
		case AVRCP_EVT_TO_MMI_GET_KEY_PLAY_RESP:
			MMI_AVRCP_SetMusicPlay(linkIndex, TRUE);
			break;
			
		case AVRCP_EVT_TO_MMI_GET_PLAY_STATUS:
			LightDbgPrint("MMI-Get play status:%x",(U8)paramPtr->PlayStatus);
			
			if(paramPtr->PlayStatus != AVRCP_PLAYBACK_STATUS_REJECT)
				MMI_AVRCP_CancelMission(linkIndex, MMI_AVRCP_MISSION_BACKUP_AND_PUSH_PLAY_STATUS);
			
			gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isGetPlayStatusSupported = (paramPtr->PlayStatus != AVRCP_PLAYBACK_STATUS_REJECT)?TRUE:FALSE;
			gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isSendingGetPlayStatus = AVRCP_PLAY_STATUS_DONE;		
			MMI_AVRCP_RemotePlayStatus(linkIndex, paramPtr->PlayStatus);
			break;
			
		// For Key play and Key pause
		case AVRCP_EVT_TO_MMI_PLAY_STATUS_FOR_KEY_PLAY_PAUSE:
			switch(paramPtr->PlayStatus)
			{
				case AVRCP_PLAYBACK_STATUS_PLAYING:
				case AVRCP_PLAYBACK_STATUS_FWD_SEEK:
				case AVRCP_PLAYBACK_STATUS_REV_SEEK:	
					MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SEND_OPERATION,AVC_OPERATION_PAUSE);
					break;
				case AVRCP_PLAYBACK_STATUS_STOPPED: 
				case AVRCP_PLAYBACK_STATUS_PAUSED:
					MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SEND_OPERATION,AVC_OPERATION_PLAY);
					break;
				default:
					if (MMI_AVRCP_IsMusicPlaying(linkIndex)
						#ifdef A2DP_Profile
						&& MUSIC_EXISTS(linkIndex)
						#endif
						)
						MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SEND_OPERATION,AVC_OPERATION_PAUSE);
					else
						MMI_AVRCP_CmdGenerator(linkIndex,AVRCP_SEND_OPERATION,AVC_OPERATION_PLAY);	
					break;
			}
			break;

		case AVRCP_EVT_TO_MMI_REMOTE_NOTIFICATION_CHANGED:
			MMI_AVRCP_RemoteNotification(linkIndex, paramPtr);
			break;

		case AVRCP_EVT_TO_MMI_SET_ABSOLUTE_VOLUME:
			MMI_AVRCP_TG_GetSetAbsVol(linkIndex, paramPtr->AbsoluteVolume);
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_SET_ABSOLUTE_VOLUME, (U8 XDATA_PTR)&paramPtr->AbsoluteVolume, linkIndex);
			break;

		case AVRCP_EVT_TO_MMI_GET_ELEMENT_ATTRIBUTES:
			if(api_profile_func[API_GRP_PF_CB_AVRCP_MEDIA_ATTRIBUTE])
			{
				((API_PROFILE_AVRCP_MEDIA_ATTRIBUTE_FUNC)api_profile_func[API_GRP_PF_CB_AVRCP_MEDIA_ATTRIBUTE])(linkIndex, (U8 XDATA_PTR)&((MMIMsgType XDATA_PTR)paramPtr)->msgBodyPtr.hciUARTAvrcpMediaAttribute.dataBegin);
			}			
			if (MMI_CC_UART_ENABLE)
			{
				MMI_HCI_SendVCmdAvrcpEventToUART(linkIndex, (MMIMsgType XDATA_PTR)paramPtr, HCI_VEVT_OCF_AVRCP_MEDIA_ATTRIBUTES);
				return; //redirect eventPtr, bypass OSMEM_Put()
			}
			OSMEM_Put((U8 XDATA_PTR)paramPtr);
			break;
			
		case AVRCP_EVT_TO_MMI_VENDOR_UNIQUE:
			#ifdef AIR_MODULE
			#ifdef LIGHTING_ENABLE
			if(paramPtr->VendorUnique.operationId == AVRCP_VENDOR_UNIQ_LIGHT_MODE_CONTROL)
			{
				//operationParam: LED Mode
				gMMI_Air_ctl.ledMode = paramPtr->VendorUnique.operationParam;
				gMMI_Air_ctl.ledSyncClkVal = paramPtr->VendorUniqueExtendLighting.syncClk;
				OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&(gMMI_Air_ctl.ledPara[0]), 
						(U8 XDATA_PTR)&(paramPtr->VendorUniqueExtendLighting.ledPara[0]), 3);
			}
			else
			#endif
			MMI_AIR_HandleVendorAVRCPEvt(linkIndex, paramPtr->VendorUnique.operationId, paramPtr->VendorUnique.operationParam);
			#endif
			break;
			
		case AVRCP_EVT_TO_MMI_GET_APPLICATION_SETTINGS:
			MMI_AVRCP_GetApplicationSettings(linkIndex, paramPtr);
			break;

		default:
			#ifdef AIR_MODULE
			MMI_AIR_HandleAirohaAVRCPEvt(linkIndex, evtCode);
			#endif
			break;
	}
}

PUBLIC void MMI_AVRCP_ConnectedHandle(U8 linkIndex)
{
	gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus = AVRCP_PLAYBACK_STATUS_NONE;

	#ifdef FAST_CONNECT_DONGLE
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SET_ABSOLUTE_VOLUME, MMI_AVRCP_GetAbsVolByMusic(linkIndex));
	#else
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_GET_PLAY_STATUS);
	#endif
	
	#ifdef AIR_MODULE
	MMI_AIR_AVRCPConnectedNotify(linkIndex);
	#endif
}

PUBLIC BOOL MMI_AVRCP_IsConnected(U8 linkIndex)
{
	return (PROFILE_CONNECTED <= AVRCP_GetState(linkIndex))? TRUE: FALSE;
}

PUBLIC void MMI_AVRCP_CmdGenerator(U8 linkIndex, U8 opCode, U8 opID)
{
	MMI_TO_AVRCP_CMD_PARAM LDATA cmdParam;
		
	if(!MMI_AVRCP_IsConnected(linkIndex))
		return;

	if(opCode == AVRCP_SEND_OPERATION)
	{
		switch(opID)
		{
			case AVC_OPERATION_KEY_PAUSE:
				opID = AVC_OPERATION_PAUSE;
				if(MMI_MUSIC_PLAY_PAUSE_ONEKEY_FEAT)
				{
				
					U8 playStatus = MMI_AVRCP_GetPlayStatus(linkIndex);
					if (gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].lastTxOpIDForPlayPause != 0)
					{
						if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].lastTxOpIDForPlayPause == AVC_OPERATION_PAUSE)
						{
							opID = AVC_OPERATION_PLAY;
						}
						else
						{
							opID = AVC_OPERATION_PAUSE;
						}
					}	
					else if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isPlayStatusWorking)
					{
						if(playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
							opID = AVC_OPERATION_PAUSE;
						else if(playStatus == AVRCP_PLAYBACK_STATUS_STOPPED || playStatus == AVRCP_PLAYBACK_STATUS_PAUSED)
							opID = AVC_OPERATION_PLAY;
						else
							return;
					}
					else
					{
						if(!MMI_KEY_PLAY_PAUSE_DONT_GET_PLAY_STATUS && (MMI_IS_AVRCP_GET_PLAY_STATUS_SUPPORTED(linkIndex)))
							opID = AVC_OPERATION_KEY_PAUSE;
						else
						{
							if (!gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isMusicPlay
								#ifdef A2DP_Profile
								|| !MUSIC_EXISTS(linkIndex)
								#endif
								)
							{
								opID = AVC_OPERATION_PLAY;
							}
						}
					}
					gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].lastTxOpIDForPlayPause = opID;
				}
				break;
			case AVC_OPERATION_KEY_PLAY:
				opID = AVC_OPERATION_PLAY;
				if (MMI_MUSIC_PLAY_PAUSE_ONEKEY_FEAT)
				{
					if (gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].lastTxOpIDForPlayPause != 0)
					{
						if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].lastTxOpIDForPlayPause == AVC_OPERATION_PAUSE)
						{
							opID = AVC_OPERATION_PLAY;
						}
						else
						{
							opID = AVC_OPERATION_PAUSE;
						}
					}						
					else if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isPlayStatusWorking)
					{
						U8 playStatus = MMI_AVRCP_GetPlayStatus(linkIndex);

						if(playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
							opID = AVC_OPERATION_PAUSE;
						else if(playStatus == AVRCP_PLAYBACK_STATUS_STOPPED || playStatus == AVRCP_PLAYBACK_STATUS_PAUSED)
							opID = AVC_OPERATION_PLAY;
						else
							return;
					}
					else
					{
						if(!MMI_KEY_PLAY_PAUSE_DONT_GET_PLAY_STATUS &&(MMI_IS_AVRCP_GET_PLAY_STATUS_SUPPORTED(linkIndex)))
							opID = AVC_OPERATION_KEY_PLAY;
						else
						{
							if (MMI_AVRCP_IsMusicPlaying(linkIndex)
								#ifdef A2DP_Profile
								&& MUSIC_EXISTS(linkIndex)
								#endif
								)
							{
								opID = AVC_OPERATION_PAUSE;
							}
						}
					}
					gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].lastTxOpIDForPlayPause = opID;
				}
				break;

			case AVC_OPERATION_PLAY:
				if (!MMI_IS_AVRCP_GET_PLAY_STATUS_SUPPORTED(linkIndex) &&
					MMI_AVRCP_IsMusicPlaying(linkIndex)
					#ifdef A2DP_Profile
					&& A2DP_STREAMING == A2DP_GetState(linkIndex)
					#endif
					)
				{
					opID = AVC_OPERATION_PAUSE;
				}
				break;
			case AVC_OPERATION_PAUSE:
				if ( !MMI_IS_AVRCP_GET_PLAY_STATUS_SUPPORTED(linkIndex) &&
					!MMI_AVRCP_IsMusicPlaying(linkIndex)
					#ifdef A2DP_Profile
					|| A2DP_STREAMING != A2DP_GetState(linkIndex)
					#endif
					)
				{
					opID = AVC_OPERATION_PLAY;
				}
				break;

			case AVC_OPERATION_REWIND:
			case AVC_OPERATION_FAST_FORWARD:
			case AVC_OPERATION_STOP:
			case AVC_OPERATION_FORWARD:
			case AVC_OPERATION_BACKWARD:
			case AVC_OPERATION_INTERNAL_REWIND_RELEASE:
			case AVC_OPERATION_INTERNAL_FAST_FORWARD_RELEASE:
				#ifdef A2DP_Profile
				if(A2DP_OPEN > A2DP_GetState(linkIndex))
					return;
				#endif
				break;
			case AVC_OPERATION_GET_PLAY_STATUS:
				if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isSendingGetPlayStatus== AVRCP_PLAY_STATUS_SENT)
					return;

				gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isSendingGetPlayStatus = AVRCP_PLAY_STATUS_SENT;
				break;
				
			case AVC_OPERATION_VOL_UP:
			case AVC_OPERATION_VOL_DOWN:			
				break;
		}
	}

	cmdParam.operationId = opID;
	AVRCP_MMICommandHandler(linkIndex, opCode, &cmdParam);
}

PUBLIC void MMI_AVRCP_ClearCtlData(U8 linkIndex)
{
	MMI_AVRCP_ClearAllMissions(linkIndex);
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex], 0x00, sizeof(MmiAvrcpCtlType));
	#ifdef AIR_MODULE
	gMMI_Air_ctl.linkInfo[linkIndex].isHoldForMute = FALSE;
	#endif
}

PUBLIC void MMI_AVRCP_AbsVolToMusicSoundLevel(U8 linkIndex, U8 absVol)
{
	U8 i;

	CURRENT_AVRCP_VOLUME(linkIndex) = absVol;
	
	if(absVol == 0)
		i = 0;
	else if(absVol == AVRCP_ABSOLUTE_VOLUME_MAX)
		i = MMI_MAX_MUSIC_SOUND_LEVEL();
	else
		i = ((U16)absVol * MMI_MAX_MUSIC_SOUND_LEVEL() + (AVRCP_ABSOLUTE_VOLUME_FULL_RANGE / 2)) 
			/AVRCP_ABSOLUTE_VOLUME_FULL_RANGE;

	if(i != CURRENT_A2DP_SOUND_LEVEL(linkIndex))
	{
		CURRENT_A2DP_SOUND_LEVEL(linkIndex) = i;
		CURRENT_A2DP_SMALL_STEP(linkIndex) = 0;
	}
}

PUBLIC U8 MMI_AVRCP_SoundLevelToAbsoluteVol(U8 soundLevel, U8 maxLevel, U8 smallSteps, U8 steps)
{
	if(soundLevel || smallSteps)
	{
		if(soundLevel == maxLevel)
		{
			soundLevel = AVRCP_ABSOLUTE_VOLUME_MAX;
		}
		else
		{
			soundLevel = ((U16)soundLevel * AVRCP_ABSOLUTE_VOLUME_FULL_RANGE) / maxLevel
					+ ((U16)smallSteps*AVRCP_ABSOLUTE_VOLUME_FULL_RANGE)/(maxLevel*(steps +1));
		}
	}
	return soundLevel;
}

PUBLIC U8 MMI_AVRCP_GetAbsVolByMusic(U8 linkIndex)
{
	return CURRENT_AVRCP_VOLUME(linkIndex);
	//remember absolute volume coz sound level cannot recover to exactly same absolute volume [mantis 9140]
}

PUBLIC U8 MMI_AVRCP_GetAbsVolBySCO(U8 linkIndex)
{
	return gMMI_ctl.mmiInfo[linkIndex].linkPara.absVolume;
	//return MMI_AVRCP_SoundLevelToAbsoluteVol(CURRENT_SCO_SOUND_LEVEL(linkIndex), (U8)MMI_MAX_SCO_SOUND_LEVEL(), CURRENT_SCO_SMALL_STEP(linkIndex), MMI_SCO_SMALL_STEP_LEVELS());
}

PUBLIC U8 MMI_AVRCP_GetAbsVolByIdle(void)
{	
	return MMI_AVRCP_SoundLevelToAbsoluteVol(gMMI_ctl.idleStateSoundLevel, (U8)gMMI_nvram.totalSoundLevel, gMMI_ctl.idleStateSmallStep, MMI_SCO_SMALL_STEP_LEVELS());
}

PRIVATE void MMI_AVRCP_RemotePlayStatus(U8 linkIndex, U8 playStatus)
{
	//For LG phone send pause-->play-->pause event notifycation after sending AVRCP pause
	#ifdef A2DP_Profile
	if(A2DP_IsMusicSuspending(linkIndex)) 
	{
		if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus != playStatus && playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
		{	
			gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus = playStatus;
			return;
		}
	}
	#endif
	
	if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus != AVRCP_PLAYBACK_STATUS_NONE)		//issue #6396
	{
		if(api_profile_func[API_GRP_PF_CB_AVRCP_PLAY_STATUS])
		{
			((API_PROFILE_AVRCP_PLAY_STATUS_FUNC)api_profile_func[API_GRP_PF_CB_AVRCP_PLAY_STATUS])(linkIndex, playStatus);
		}

		if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus != playStatus)
		{
			if(playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
			{
				MMI_AVRCP_SetMusicPlay(linkIndex, TRUE);
			}
			else if(playStatus == AVRCP_PLAYBACK_STATUS_STOPPED || playStatus == AVRCP_PLAYBACK_STATUS_PAUSED)
			{
				MMI_AVRCP_SetMusicPlay(linkIndex, FALSE);
			}
			#ifdef A2DP_Profile
			MMI_A2DP_PlayStatusChanged(linkIndex, playStatus);
			#endif
			gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isPlayStatusWorking = TRUE;
			MMI_AVRCP_ResetKeyPlayToggle(linkIndex);
		}
		else
		{
			#ifdef A2DP_Profile
			MMI_A2DP_PlayStatusTheSame(linkIndex, playStatus);
			#endif
		}
	}
	else
	{
		#ifdef A2DP_Profile
		MMI_A2DP_CheckPlayStatusConsistency(linkIndex, playStatus, TRUE);
		#endif
	}
	gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus = playStatus;
}

PRIVATE void MMI_AVRCP_TG_GetKeyVolUpDown(U8 linkIndex)
{
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_VOLUME_CHANGED, MMI_AVRCP_GetAbsVolByMusic(linkIndex));
	MMI_AVRCP_SetDrvVolume(linkIndex);
}

PRIVATE void MMI_AVRCP_TG_GetVolChanged(U8 linkIndex, U8 absVol)
{
	LightDbgPrint("AVRCP_GetVolChanged[%d]:%d",(U8)linkIndex, (U8)absVol);
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SET_NOTIFY_MASK, MMI_AVRCP_VOLUME_CHANGED);
#ifdef AIR_MODULE
	MMI_AIR_TG_GetVolChanged(linkIndex, absVol);
#endif
	
}

PRIVATE void MMI_AVRCP_TG_GetSetAbsVol(U8 linkIndex, U8 absVol)
{
	LightDbgPrint("AVRCP_GetSetAbsVol[%d]:%d",(U8)linkIndex, (U8)absVol);

#ifdef AIR_MODULE
	if(MMI_AIR_TG_GetSetAbsVol(linkIndex, absVol))
		return;
#endif
	MMI_AVRCP_AbsVolToMusicSoundLevel(linkIndex, absVol);
	MMI_AVRCP_SetDrvVolume(linkIndex);
}

PRIVATE void MMI_AVRCP_SetDrvVolume(U8 linkIndex)
{
	if (linkIndex == MMI_DRV_GetAudioInMusicLink() /* && gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isMusicPlay mantis 10490*/)
	{
		LightDbgPrint("GetAVRCPVol[%d]:%d,%d",(U8)linkIndex, (U8)CURRENT_A2DP_SOUND_LEVEL(linkIndex), (U8)CURRENT_A2DP_SMALL_STEP(linkIndex));
		#ifdef A2DP_Profile
		if (!MMI_AVRCP_EXT_MCU_VOLUME_CTRL_FEAT)
		{
			MMI_A2DP_SendDriverVolCmd(CURRENT_A2DP_SOUND_LEVEL(linkIndex), CURRENT_A2DP_SMALL_STEP(linkIndex));
					
			if (MMI_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT)
			{
				MMI_LinkKey_SaveHistoryInRunTime(linkIndex);
			}
		}
		#endif
		#ifdef AIR_MODULE
		MMI_AIR_AVRCPSetDriverVol(linkIndex);
		#endif
	}
}

PUBLIC void MMI_AVRCP_SetMusicPlay(U8 linkIndex, BOOL isPlay)
{
	gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isMusicPlay = isPlay;
}

PUBLIC void MMI_AVRCP_ResetKeyPlayToggle(U8 linkIndex)
{
	gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].lastTxOpIDForPlayPause = 0;//reset for music play updating
}

PUBLIC BOOL MMI_AVRCP_IsMusicPlaying(U8 linkIndex)
{
	return (gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isMusicPlay)?TRUE:FALSE;
}

PUBLIC void MMI_AVRCP_KeyPlay(U8 linkIndex)
{
#ifdef DEMOSOUND
	if(!gMMI_ctl.activeLinkRecord
		#ifdef AIR_MODULE
		|| !MMI_IsNormalLink(linkIndex)
		#endif
	)
	{
		if(gMMI_ctl.audioableA2DPLink == 0x0A)
			MMI_A2DP_KeyDemoSoundPause();
		else
			MMI_A2DP_KeyDemoSoundPlay();
		return;
	}
#endif
	
	if(MMI_AVRCP_IsConnected(linkIndex))
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_KEY_PLAY);
	#if 0	//IOT with LG G2
	else if(MMI_A2DP_IsConnected(linkIndex))
		MMI_A2DP_SendCmd(linkIndex, A2DP_PLAY_CMD);
	#endif
	
	#ifdef A2DP_Profile
	if(!MMI_A2DP_IsConnected(linkIndex))
		MMI_ConnectProfile(linkIndex ,PROFILE_A2DP);
	#endif
}

PUBLIC void MMI_AVRCP_KeyStop(U8 linkIndex)
{
#ifdef DEMOSOUND
	if(!gMMI_ctl.activeLinkRecord
		#ifdef AIR_MODULE
		|| !MMI_IsNormalLink(linkIndex)
		#endif
	)
	{
		MMI_A2DP_KeyDemoSoundStop();
		return;
	}
#endif

	if(MMI_AVRCP_IsConnected(linkIndex))
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_STOP);
	else
		MMI_A2DP_SendCmd(linkIndex, A2DP_PAUSE_CMD);
}

PUBLIC void MMI_AVRCP_KeyPause(U8 linkIndex)
{
#ifdef DEMOSOUND
	if(!gMMI_ctl.activeLinkRecord
		#ifdef AIR_MODULE
		|| !MMI_IsNormalLink(linkIndex)
		#endif
	)
	{
		if(gMMI_ctl.audioableA2DPLink == 0xAA)
			MMI_A2DP_KeyDemoSoundPause();
		else
			MMI_A2DP_KeyDemoSoundPlay();
		return;
	}
#endif
	
	if(MMI_AVRCP_IsConnected(linkIndex))
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_KEY_PAUSE);
	#ifdef A2DP_Profile
	else if(MMI_A2DP_IsConnected(linkIndex))
		MMI_A2DP_SendCmd(linkIndex, A2DP_PAUSE_CMD);
	#endif
	
	#ifdef A2DP_Profile
	if(!MMI_A2DP_IsConnected(linkIndex))
		MMI_ConnectProfile(linkIndex ,PROFILE_A2DP);
	#endif
}

PUBLIC void MMI_AVRCP_KeyRewindPush(U8 linkIndex)
{
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_REWIND);
}

PUBLIC void MMI_AVRCP_KeyFastForwardPush(U8 linkIndex)
{
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_FAST_FORWARD);
}

PUBLIC void MMI_AVRCP_KeyForward(U8 linkIndex)
{
#ifdef DEMOSOUND
	if(!gMMI_ctl.activeLinkRecord
		#ifdef AIR_MODULE
		|| !MMI_IsNormalLink(linkIndex)
		#endif
	)
	{
		MMI_A2DP_KeyDemoSoundForward();
	}
#endif

	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_FORWARD);
}

PUBLIC void MMI_AVRCP_KeyBackward(U8 linkIndex)
{
#ifdef DEMOSOUND
	if(!gMMI_ctl.activeLinkRecord
		#ifdef AIR_MODULE
		|| !MMI_IsNormalLink(linkIndex)
		#endif
	)
	{
		MMI_A2DP_KeyDemoSoundBackward();
	}
#endif
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_BACKWARD);
}

PUBLIC void MMI_AVRCP_KeyRewindRelease(U8 linkIndex)
{
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_INTERNAL_REWIND_RELEASE);
}

PUBLIC void MMI_AVRCP_KeyFastForwardRelease(U8 linkIndex)
{
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_INTERNAL_FAST_FORWARD_RELEASE);
}

PUBLIC void MMI_AVRCP_KeyRepeatModeChange(U8 linkIndex)
{
	if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner == AVRCP_GET_APP_SETTINGS_NONE)
	{
		gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_KEY;
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_GET_REPEAT_MODE);
	}
}

PUBLIC void MMI_AVRCP_KeyShuffleModeChange(U8 linkIndex)
{
	if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner == AVRCP_GET_APP_SETTINGS_NONE)
	{
		gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_KEY;
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_GET_SHUFFLE_MODE);
	}	
}

PRIVATE void MMI_AVRCP_KeySwitch(U8 linkIndex)
{
	UNUSED(linkIndex);
}

PRIVATE void MMI_AVRCP_KeyLeftChannel(U8 linkIndex)
{
	#ifdef A2DP_Profile
	MMI_A2DP_SendCmd(linkIndex, A2DP_HW_AUDIO_CHANNEL_MONO_L);
	#else
	UNUSED(linkIndex);
	#endif
	MMI_PushMediaEvent(MEDIA_EVT_TWS_LEFT_CHANNEL);
}

PRIVATE void MMI_AVRCP_KeyRightChannel(U8 linkIndex)
{
	#ifdef A2DP_Profile
	MMI_A2DP_SendCmd(linkIndex, A2DP_HW_AUDIO_CHANNEL_MONO_R);
	#else
	UNUSED(linkIndex);
	#endif
	MMI_PushMediaEvent(MEDIA_EVT_TWS_RIGHT_CHANNEL);
}

PRIVATE void MMI_AVRCP_KeyStereoChannel(U8 linkIndex)
{
	#ifdef A2DP_Profile
	MMI_A2DP_SendCmd(linkIndex, A2DP_HW_AUDIO_CHANNEL_STEREO);
	#else
	UNUSED(linkIndex);
	#endif
}

PUBLIC void MMI_AVRCP_SendVendorUnique(U8 linkIndex, U8 operationId, U8 operationParam)
{
	MMI_TO_AVRCP_CMD_PARAM LDATA cmdParam;
	
	if(!MMI_AVRCP_IsConnected(linkIndex))
	{
		LightDbgPrint("ASSERT - Send Vendor Cmd Fail");
		return;
	}
	
	cmdParam.VendorUnique.operationId = operationId;
	cmdParam.VendorUnique.operationParam = operationParam;

	AVRCP_MMICommandHandler(linkIndex, AVRCP_SEND_VENDOR_UNIQUE, &cmdParam);
}

PUBLIC void MMI_AVRCP_SetPlayAppSettings(U8 linkIndex, U8 attributeID, U8 attributeValue)
{
	MMI_TO_AVRCP_CMD_PARAM LDATA cmdParam;
	
	if(!MMI_AVRCP_IsConnected(linkIndex))
	{
		LightDbgPrint("ASSERT - Send Vendor Cmd Fail");
		return;
	}
	cmdParam.PlayAppSettings.attributeID = attributeID;
	cmdParam.PlayAppSettings.attributeValue = attributeValue;
	AVRCP_MMICommandHandler(linkIndex, AVRCP_SEND_OPERATION, &cmdParam);
}

PUBLIC void MMI_AVRCP_UartSetAbsVol(U8 vol)
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_NONE)
		{
			MMI_AVRCP_AbsVolToMusicSoundLevel(linkIndex, vol);
			MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_VOLUME_CHANGED, vol);
			MMI_AVRCP_SetDrvVolume(linkIndex);
		}
	}
}

#ifdef AIRAPP_Profile
PUBLIC void MMI_AVRCP_AppSetAbsVol(U8 linkIndex, U8 vol)
{
	MMI_AVRCP_AbsVolToMusicSoundLevel(linkIndex, vol);
	MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_VOLUME_CHANGED, vol);
	MMI_AVRCP_SetDrvVolume(linkIndex);
}
#endif

PRIVATE void MMI_AVRCP_ClearAllMissions(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;
	while(OSMQ_Entries(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que))
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);
		if(msgPtr->msgBodyPtr.avrcpTaskCmd.fcallback)
		{
			msgPtr->msgBodyPtr.avrcpTaskCmd.fcallback(linkIndex, FALSE);
		}
		OSMEM_Put((U8 XDATA_PTR)msgPtr);
	}
}

PRIVATE BOOL MMI_AVRCP_CheckRepeatMission(U8 linkIndex, U8 taskId)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 i = OSMQ_Entries(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);
	BOOL isRepeat = FALSE;
	
	while(i--)
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);
		OSMQ_Put(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que, (U8 XDATA_PTR)msgPtr);
		if(msgPtr->msgOpcode == taskId)
			isRepeat = TRUE;
	}
	return isRepeat;
}

PUBLIC void MMI_AVRCP_PushMission(U8 linkIndex, MMI_AVRCP_MISSION_PARAM_TYPE XDATA_PTR paramPtr)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(MMI_AVRCP_CheckRepeatMission(linkIndex, paramPtr->taskId))
		return;
	
	if((msgPtr = MMI_GetMMIMsgTypeCmdMemory(paramPtr->taskId)) != (MMIMsgType XDATA_PTR)NULL)
	{
		OST_SetTimer(&msgPtr->msgBodyPtr.avrcpTaskCmd.waitingTimer, paramPtr->timer);
		msgPtr->msgBodyPtr.avrcpTaskCmd.fcallback = paramPtr->fcallback;
		
		if(paramPtr->taskId == MMI_AVRCP_MISSION_BACKUP_AND_PUSH_PLAY_STATUS 
			|| paramPtr->taskId == MMI_AVRCP_MISSION_CHECK_STREAMING_STATE_LATER)
		{
			msgPtr->msgBodyPtr.avrcpTaskCmd.PlayStatus = paramPtr->PlayStatus;
		}
		
		OSMQ_Put(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que, (U8 XDATA_PTR)msgPtr);
	}
}

PRIVATE void MMI_AVRCP_PopMissions(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 i = OSMQ_Entries(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);

	while(i--)
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);
		
		if(!msgPtr)
		{
			break; // clear mission might occur, memory in queue might be clear in this while loop. mantis 8796
		}
		
		if(OST_TimerExpired(&msgPtr->msgBodyPtr.avrcpTaskCmd.waitingTimer))
		{
			switch(msgPtr->msgOpcode)
			{
				case MMI_AVRCP_MISSION_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC:
				case MMI_AVRCP_MISSION_MONITOR_PLAY_STATUS:
					MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_GET_PLAY_STATUS);
					if(msgPtr->msgBodyPtr.avrcpTaskCmd.fcallback)
					{
						msgPtr->msgBodyPtr.avrcpTaskCmd.fcallback(linkIndex, TRUE);
					}
					break;
					
				case MMI_AVRCP_MISSION_SEND_PAUSE_MUSIC:
					if(msgPtr->msgBodyPtr.avrcpTaskCmd.fcallback)
					{
						msgPtr->msgBodyPtr.avrcpTaskCmd.fcallback(linkIndex, FALSE);
					}				
					break;
					
				case MMI_AVRCP_MISSION_QUERY_CURRENT_PLAY_STATUS:
					#ifdef A2DP_Profile
					MMI_A2DP_PlayStatusTheSame(linkIndex, gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus);
					#endif
					break;
					
				case MMI_AVRCP_MISSION_BACKUP_AND_PUSH_PLAY_STATUS:
					LightDbgPrint("AVRCP Pop:PLAY_STATUS:%d,%d",(U8)linkIndex, (U8)msgPtr->msgBodyPtr.avrcpTaskCmd.PlayStatus);
					MMI_AVRCP_RemotePlayStatus(linkIndex, msgPtr->msgBodyPtr.avrcpTaskCmd.PlayStatus);
					break;
					
				case MMI_AVRCP_MISSION_CHECK_STREAMING_STATE_LATER:
					LightDbgPrint("AVRCP chk streaming later TO");
					if (msgPtr->msgBodyPtr.avrcpTaskCmd.PlayStatus == gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus)
					{
						if (msgPtr->msgBodyPtr.avrcpTaskCmd.PlayStatus == AVRCP_PLAYBACK_STATUS_PLAYING 
							#ifdef A2DP_Profile
							&& A2DP_OPEN == A2DP_GetState(linkIndex)
							#endif
							)
						{
							if(!MMI_DRV_IsActiveAudioDevInFM() && !MMI_DRV_IsActiveAudioDevInAUX() && !MMI_DRV_IsActiveAudioDevInCall()
								#ifdef DEMOSOUND
								&& gMMI_ctl.audioDevCtl.currentActiveDev != AUDIO_DEVICE_DEMO_SOUND
								#endif
								&& !(MMI_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT && MMI_DRV_IsActiveAudioDevInMusic()))
							{								
								MMI_A2DP_SendCmd(linkIndex, A2DP_PLAY_CMD);
							}
						}
					}
					break;
					
				default:
					
					break;
			}
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		}
		else
		{
			OSMQ_Put(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que, (U8 XDATA_PTR)msgPtr);
		}
	}
}

PUBLIC void MMI_AVRCP_CancelMission(U8 linkIndex, U8 taskId)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 i = OSMQ_Entries(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);

	while(i--)
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);
		if(msgPtr->msgOpcode == taskId)
		{
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		}
		else
		{
			OSMQ_Put(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que, (U8 XDATA_PTR)msgPtr);
		}
	}
}

PUBLIC BOOL MMI_AVRCP_CheckMission(U8 linkIndex, U8 taskId)
{
	BOOL isTaskMatched = FALSE;
	MMIMsgType XDATA_PTR msgPtr;
	U8 i = OSMQ_Entries(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);

	while(i--)
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que);
		if(msgPtr->msgOpcode == taskId)
		{
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
			isTaskMatched = TRUE;
		}
		else
		{
			OSMQ_Put(&gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].Task_Que, (U8 XDATA_PTR)msgPtr);
		}
	}
	return isTaskMatched;
}


PUBLIC void MMI_AVRCP_Handle(void)
{
	U8 linkIndex;
	for (linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(MMI_IsProfileConnected(linkIndex, PROFILE_AVRCP))
		{
			MMI_AVRCP_PopMissions(linkIndex);
		}
	}
}

PUBLIC U8 MMI_AVRCP_GetPlayStatus(U8 linkIndex)
{
	if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].isDisablePlayStatusMonitor)
	{
		return gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].playStatus;
	}
	else
	{
		return AVRCP_PLAYBACK_STATUS_NONE;
	}
}

PUBLIC void MMI_AVRCP_CheckStreamingStateLater(U8 linkIndex, U8 playStatus, U8 needDelay)
{
	MMI_AVRCP_MISSION_PARAM_TYPE LDATA avrcpParam;
		
	MMI_AVRCP_CancelMission(linkIndex, MMI_AVRCP_MISSION_CHECK_STREAMING_STATE_LATER);

	avrcpParam.taskId = MMI_AVRCP_MISSION_CHECK_STREAMING_STATE_LATER;
	if (needDelay)
	{
		avrcpParam.timer = ONE_SEC * 2;		
	}
	else
	{
		avrcpParam.timer = ONE_SEC/2;		
	}
		
	avrcpParam.fcallback = NULL;
	avrcpParam.PlayStatus = playStatus;
	MMI_AVRCP_PushMission(linkIndex, &avrcpParam);
}

PUBLIC void MMI_HCI_SendAVRCPCmdResponse(U8 cmdOpcode, U8 resultCode)
{
	MMIMsgType XDATA_PTR msgPtr;
	msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART);
	if(msgPtr)
	{
		msgPtr->paraLen = 3;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = cmdOpcode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_AVRCP;
		msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmd.resultCode = resultCode;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_AVRCP_HCICommand(MMIMsgType XDATA_PTR msgPtr)
{
	if(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf == HCI_VCMD_OCF_AVRCP_APPLICATION_SETTINGS)
	{
		U8 linkIndex;

		MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTAvrcpCmd.bdAddr.member);
		if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTAvrcpCmd.bdAddr)) < MAX_MULTI_POINT_NO)
		{
			U8 attributeID = msgPtr->msgBodyPtr.hciUARTAvrcpCmd.dataBegin;

			if(gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner == AVRCP_GET_APP_SETTINGS_NONE)
			{
				if(attributeID == AVRCP_REPEAT_MODE_STATUS)
				{
					gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_UART;
					MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_GET_REPEAT_MODE);
					MMI_HCI_SendAVRCPCmdResponse(HCI_VCMD_OCF_AVRCP_APPLICATION_SETTINGS, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
					OSMEM_Put((U8 XDATA_PTR)msgPtr);
					return;
				}
				else if(attributeID == AVRCP_SHUFFLE_ON_OFF_STATUS)
				{
					gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_UART;
					MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_GET_SHUFFLE_MODE);
					MMI_HCI_SendAVRCPCmdResponse(HCI_VCMD_OCF_AVRCP_APPLICATION_SETTINGS, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
					OSMEM_Put((U8 XDATA_PTR)msgPtr);
					return;
				}
			}
		}
	}
	MMI_HCI_SendAVRCPCmdResponse(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_AVRCP_SendTrackChangeUartEvt(U8 linkIndex, U8 avrcpEventId)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if (!MMI_CC_UART_ENABLE)
		return;
		
	if(msgPtr = MMI_GetCommandMessageMem( HCI_VCMD_AIROHA_TO_UART ))
	{
		msgPtr->paraLen = sizeof(HciUARTAvrcpChangedNotify);
		msgPtr->msgBodyPtr.hciUARTAvrcpChangedNotify.avrcpEventId = avrcpEventId;

		MMI_HCI_SendVCmdAvrcpEventToUART(linkIndex, msgPtr, HCI_VEVT_OCF_AVRCP_CHANGED_NOTIFY);
	}
}

PRIVATE void MMI_AVRCP_SendPlaybackPosUartEvt(U8 linkIndex, U32 PlaybackPos)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if (!MMI_CC_UART_ENABLE)
		return;
		
	if(msgPtr = MMI_GetCommandMessageMem( HCI_VCMD_AIROHA_TO_UART ))
	{
		msgPtr->paraLen = sizeof(HciUARTAvrcpPlaybackPos);
		msgPtr->msgBodyPtr.hciUARTAvrcpPlaybackPos.playbackPos = PlaybackPos;

		MMI_HCI_SendVCmdAvrcpEventToUART(linkIndex, msgPtr, HCI_VEVT_OCF_AVRCP_PLAYBACK_POS);
	}
}

PRIVATE void MMI_AVRCP_SendPlayAppSettingsUartInd(U8 linkIndex, U8 attributeID, U8 attributeValue)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if(!MMI_CC_UART_ENABLE)
		return;
		
	if(msgPtr = MMI_GetCommandMessageMem( HCI_VCMD_AIROHA_TO_UART ))
	{
		msgPtr->paraLen = sizeof(HciUARTAvrcpPlayAppSetting);
		msgPtr->msgBodyPtr.hciUARTAvrcpPlayApp.attributeID = attributeID;
		msgPtr->msgBodyPtr.hciUARTAvrcpPlayApp.attributeValue = attributeValue;
		MMI_HCI_SendVCmdAvrcpEventToUART(linkIndex, msgPtr, HCI_VEVT_OCF_AVRCP_APPLICATION_SETTINGS_IND);
	}
}

PUBLIC BOOL MMI_AVRCP_IsGetAppSettingsIdle(U8 linkIndex)
{
	return (gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner == AVRCP_GET_APP_SETTINGS_NONE)?TRUE:FALSE;
}

PUBLIC void MMI_AVRCP_SDK_SetGetAppSettingsOwner(U8 linkIndex)
{
	gMMI_AVRCP_ctl.AvrcpCtl[linkIndex].getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_SDK;
}
#endif