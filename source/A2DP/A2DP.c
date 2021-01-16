//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _A2DP_C_

#include "A2DP.h"

#include "..\MMI\MMI.h"
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_Protocol.h"
#include "..\MMI\MMI_A2DP.h"
#include "..\MMI\MMI_SCN.h"
#include "..\MMI\MMI_SDAP.h"
#include "..\MMI\MMI_Driver.h"
#include "..\Driver\Driver_1520.h"
#ifdef AIR_MODULE
	#include "..\MMI\MMI_AirModule.h"
#endif
#ifdef LINEIN_ENABLE
	#include "..\MMI\MMI_LineIn.h"
#endif
#include "..\MMI\MMI_HCI.h"

#include "..\SDAP\SDAP_Interface.h"

#include "sector_config1.h"

#include "align_flash.h"


#ifdef DSP_ENABLE
#include "..\DSP\dsp_interface.h"
#endif

#ifdef PEQ_ENABLE
#include "..\Driver\Peq_HPF.h"
#endif

#ifdef A2DP_Profile

PRIVATE void A2DP(U8 linkIndex);
PRIVATE MMI_PROFILE_INFO XDATA_PTR A2DP_ConnectRequest(U8 linkIndex, U8 profileId);
PRIVATE U8 A2DP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);

static AVDTP_PROFILE_DESC CODE gA2DP_ProfileDesc = {
	{ //MMI_PROFILE_DESC
		PROFILE_A2DP,
		A2DP,					//ProfileTaskFunc
		A2DP_ConnectRequest,	//ProfileConnectRequest
		A2DP_DisconnectRequest,	//ProfileDisconnectRequest
		A2DP_Signal_LinkEvent,	//ProfileLinkEvent
		A2DP_Signal_DataEvent,	//ProfileDataEvent
	},
	A2DP_Media_LinkEvent,	//MediaLinkEvent
	A2DP_Media_DataEvent,	//MediaDataEvent
};

static MMI_PROFILE_NODE XDATA gA2DP_ProfileNode = {
	&gA2DP_ProfileDesc.mmiProfileDesc,
	NULL,
};

PUBLIC A2DP_LINK_INFO XDATA_PTR A2DP_GetLinkInfo(U8 linkIndex)
{
	return &gA2DP_ctl.linkInfo[linkIndex];
}

PUBLIC void A2DP_SetResume(U8 linkIndex, BOOL isResume)
{
	A2DP_GetLinkInfo(linkIndex)->isResume = isResume;
}

PUBLIC BOOL A2DP_IsResume(U8 linkIndex)
{
	return A2DP_GetLinkInfo(linkIndex)->isResume ? TRUE : FALSE;
}

PUBLIC U8 A2DP_GetCodec(U8 linkIndex)
{
	return A2DP_GetLinkInfo(linkIndex)->localSeid;
}

PUBLIC BOOL A2DP_IsMusicSuspending(U8 linkIndex)
{
	return A2DP_GetLinkInfo(linkIndex)->suspendDelayTimer ? TRUE : FALSE;
}

PUBLIC void A2DP_CommandComplete(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo)
{
	a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;
	a2dpLinkInfo->remoteReqPtr = (U8 XDATA_PTR)NULL;

	SYS_ReleaseTimer(&a2dpLinkInfo->timer);
}

PUBLIC BOOL A2DP_IsLinkFollower(U8 linkIndex)
{
	return A2DP_GetLinkInfo(linkIndex)->airCtl.packetRole == PACKET_ROLE_FOLLOWER ? TRUE : FALSE;
}

PUBLIC BOOL A2DP_IsLinkRelayer(U8 linkIndex)
{
	return A2DP_GetLinkInfo(linkIndex)->airCtl.packetRole == PACKET_ROLE_RELAYER ? TRUE : FALSE;
}

#ifndef A2DP_SRC_Profile
PRIVATE void A2DP_SendAnchorID (void)
{
	U8 XDATA_PTR anchorPtr;

	anchorPtr = OSMEM_Get(OSMEM_ptr1);
	anchorPtr[8] = (U8)A2DP_MEDIA_PKT_ANCHOR_ID;
	anchorPtr[9] = (U8)(A2DP_MEDIA_PKT_ANCHOR_ID>>8);
	OSMQ_MCU_DSP_Put(OSMQ_DSP_L2CAP_Received_PDU_ptr, anchorPtr);
}
#endif

PUBLIC void A2DP_MediaStart(void)
{
	#ifndef A2DP_SRC_Profile
	A2DP_SendAnchorID();
	#endif
	//gA2DP_ctl.relayCtl.airFollowerDropPktCount = OSMEM2_SMALL_RX_NBLKS;
	gA2DP_ctl.relayCtl.airFollowerDropPktCount = 0;
	gAlignDSP.LOCAL_TIMESTAMP_CLK = 0;
	gAlignDSP.TWS_LOCAL_FRAME_SC = 0;
	gAlignDSP.TWS_LOCAL_FRAMECNT = 0;
	gAlignDSP.SYNC_TUNING_VALUE = 0;
	gAlignDSP.SYNC_TUNING_TIME = 0;
}

PUBLIC void A2DP_SetMediaDataPlayByLink(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo, BOOL isPlay)
{
	if(!a2dpLinkInfo->isMediaDataPlay && isPlay )
	{
		#ifndef A2DP_SRC_Profile
		A2DP_MediaStart();
		#endif
	}
	a2dpLinkInfo->isMediaDataPlay = isPlay;
}

PUBLIC BOOL A2DP_IsMediaDataPlay(U8 linkIndex)
{
	return A2DP_GetLinkInfo(linkIndex)->isMediaDataPlay;
}

PRIVATE void A2DP_ClearLinkInfo(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo)
{
	SYS_MemoryRelease(&a2dpLinkInfo->remoteReqPtr);
	SYS_MemoryRelease(&a2dpLinkInfo->discovSeidInfoPtr);
	
	SYS_ReleaseTimer(&a2dpLinkInfo->timer);
	SYS_ReleaseTimer(&a2dpLinkInfo->suspendDelayTimer);
	SYS_ReleaseTimer(&a2dpLinkInfo->waitForMediaChannelOpenTimer);

	OSMQ_FlushAndReleaseMemoryBlock(&a2dpLinkInfo->OSMQ_AVDTP_Signal_To_A2DP);

	OSMEM_memset_xdata((U8 XDATA_PTR)a2dpLinkInfo ,0 ,sizeof(A2DP_LINK_INFO));
	a2dpLinkInfo->fA2DPRxMediaCodec = A2DP_UndefinedDecoderRxMediaPacket;	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//					AVDTP AnyTime Event Handler							//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

PUBLIC U8 A2DP_GetState(U8 linkIndex)
{
	return A2DP_GetLinkInfo(linkIndex)->mmiProfileInfo.state;
}

PUBLIC void A2DP_AirModeSetDSPSel(void)
{
	U8 audioChannel;
	U8 linkIndex;
	
	switch(gA2DP_ctl.relayCtl.hwAudioChannel)
	{
		case HW_AUDIO_CHANNEL_STEREO:
			audioChannel = AUDIO_CHANNEL_STEREO;
			break;
		case HW_AUDIO_CHANNEL_LEFT:
			audioChannel = AUDIO_CHANNEL_MONO_L;
			break;
		case HW_AUDIO_CHANNEL_RIGHT:
			audioChannel = AUDIO_CHANNEL_MONO_R;
			break;

		case HW_AUDIO_CHANNEL_NOT_DECIDED:
		default:
			if((linkIndex = A2DP_SearchFollowerLink()) != MAX_MULTI_POINT_NO || (linkIndex = A2DP_SearchRelayerLink()) != MAX_MULTI_POINT_NO)
			{
				audioChannel = A2DP_GetLinkInfo(linkIndex)->airCtl.audioChannel;
			}
			else
			{
				audioChannel = AUDIO_CHANNEL_STEREO;
			}
			break;
	}
	
	switch(audioChannel)
	{
		case AUDIO_CHANNEL_MONO_L:
			gAlignDSP.DSP_A2DP_AUDIO_CHANNEL_SEL = A2DP_TWS_AUDIO_SETTING_LEFT_CHANNEL;
			break;
		case AUDIO_CHANNEL_MONO_R:
			gAlignDSP.DSP_A2DP_AUDIO_CHANNEL_SEL = A2DP_TWS_AUDIO_SETTING_RIGHT_CHANNEL;
			break;
		default:
			gAlignDSP.DSP_A2DP_AUDIO_CHANNEL_SEL = A2DP_TWS_AUDIO_SETTING_LEFT_AND_RIGHT_CHANNELS;
			break;
	}
}

PRIVATE void A2DP_SendDiscoverCommand(U8 linkIndex, U8 tsep, U8 preferSeid)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	A2dpDiscovSeidInfo XDATA_PTR discovSeidInfoPtr;
	
	if(a2dpLinkInfo->discovSeidInfoPtr == (U8 XDATA_PTR)NULL)
	{
		a2dpLinkInfo->discovSeidInfoPtr = OSMEM_Get(OSMEM_ptr1);
	}
	if(discovSeidInfoPtr = (A2dpDiscovSeidInfo XDATA_PTR)a2dpLinkInfo->discovSeidInfoPtr)
	{
		discovSeidInfoPtr->tsep = tsep;
		discovSeidInfoPtr->preferSeid = preferSeid;

		MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_CONTROL);
		AVDTP_SendDiscoverCmd(linkIndex);
		a2dpLinkInfo->substates = A2DP_SUBSTATE_IDLE_WAIT_DISCOVER_RESP;
		SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
	}
}

PUBLIC void A2DP_SendAcceptStartResponse(U8 linkIndex)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	AVDTP_SendAcceptRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr);
	A2DP_CommandComplete(a2dpLinkInfo);
	MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);						
	
	#if defined(AVDTP_PTS_TP_SIG_SMG_BV_13_C) || defined(GAVDP_PTS_TP_APP_TRC_BV_01_C) || defined(GAVDP_PTS_TP_APP_TRC_BV_02_C)
	A2DP_MMICommandHandler(linkIndex, A2DP_PAUSE_CMD);
	#endif

	#ifdef AIR_MODULE
	MMI_AIR_A2DPAcceptStartReqNotify(linkIndex);
	#endif
}

PUBLIC void A2DP_MMICommandHandler(U8 linkIndex, U8 cmdCode)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	switch(cmdCode)
	{
		case A2DP_MUSIC_DATA_MEMORY_PUT_CMD:
			A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
			break;
		case A2DP_MUSIC_DATA_PLAY_CMD:
			A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, TRUE);
			break;

		case A2DP_DSP_CLOSED_CLEAN_MEMORY_CMD:
			while(OSMQ_MCU_DSP_Entries(OSMQ_DSP_L2CAP_Received_PDU_ptr))
			{
				OSMEM_Put(OSMQ_MCU_DSP_Get(OSMQ_DSP_L2CAP_Received_PDU_ptr));
			}
			break;
			
		case A2DP_CONNECT_MEDIA_CMD_SOURCE:
			SYS_ReleaseTimer(&a2dpLinkInfo->waitForMediaChannelOpenTimer);
			if(a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
			{
			#ifdef A2DP_SRC_Profile
				A2DP_SendDiscoverCommand(linkIndex, A2DP_SEP_SNK, SBC_SRC_SEID);
			#else
				#if SUPPORT_AAC_SNK
				U8 preferredSeid;
				if (!MMI_A2DP_IsHighQualityCodecDisbaled() && AVDTP_IsAacAllowedInLink(linkIndex))
				{
					preferredSeid = AAC_SNK_SEID;
				}
				else
				{
					preferredSeid = SBC_SNK_SEID;
				}
				A2DP_SendDiscoverCommand(linkIndex, A2DP_SEP_SNK, preferredSeid);
				#else
				A2DP_SendDiscoverCommand(linkIndex, A2DP_SEP_SNK, SBC_SNK_SEID);
				#endif
			#endif
			}
			break;
			
		case A2DP_MEMPUT_START_RESP:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP)
			{
				A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
				AVDTP_SendAcceptRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr);
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
				A2DP_CommandComplete(a2dpLinkInfo);
				MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);			
			}
			break;
			
		case A2DP_REJECT_START_RESP:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP)
			{
				//a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;				
				//AVDTP_SendRejectRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr, AVDTP_BAD_STATE);
				A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
				AVDTP_SendAcceptRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr);
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
				A2DP_CommandComplete(a2dpLinkInfo);
				MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
				MMI_A2DP_SendPauseMusic(linkIndex,FALSE);
			}
			break;
			
		case A2DP_ACCEPT_START_RESP:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP)
			{
				MMI_A2DP_SendDSPEnableCmd(linkIndex);
				
				if(A2DP_AirSideReadyToReceiveMediaStream())
				{
					if(MMI_A2DP_SEND_START_RESP_AFTER_DSP_IS_ENABLED_FEAT && (linkIndex != MMI_DRV_GetA2DPEnabledLinkIndex()))
					{
						a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
						a2dpLinkInfo->substates = A2DP_SUBSTATE_STREAM_WAIT_DSP_ENABLE_FOR_START_RESP;						
					}
					else
					{
						a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
						A2DP_SendAcceptStartResponse(linkIndex);
					}
				}
				else
				{
					a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
					a2dpLinkInfo->substates = A2DP_SUBSTATE_STREAM_GET_MMI_DSP_RESP_WAIT_AIR_READY;					
				}
			}
			break;
			
		case A2DP_SET_AFTER_START_RESP:
			MMI_A2DP_SendDSPEnableCmd(linkIndex);
			
			a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
			A2DP_CommandComplete(a2dpLinkInfo);
			MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);						

			#ifdef AIR_MODULE
			MMI_AIR_A2DPAcceptStartReqNotify(linkIndex);
			#endif
			break;			
			
		case A2DP_MUSIC_PAUSED_BY_AVRCP_PLAY_STATUS:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING)
			{
				if(	a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_GET_MMI_DSP_RESP_WAIT_AIR_READY ||
					a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_DSP_ENABLE_FOR_START_RESP)
				{
					A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
					AVDTP_SendAcceptRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr);
					a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
					A2DP_CommandComplete(a2dpLinkInfo);
					MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);					
				}
			}
			break;
			
		case A2DP_PAUSE_CMD:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING && a2dpLinkInfo->substates == A2DP_SUBSTATE_BEGINNING_STATE)
			{
				A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);			
				a2dpLinkInfo->substates = A2DP_SUBSTATE_STREAM_WAIT_SUSPEND_RESP;				
				AVDTP_SendSuspendCmd(linkIndex, a2dpLinkInfo->remoteSeid);
				SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
			}
			break;
			
		case A2DP_PLAY_CMD:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_BEGINNING_STATE)
			{
				a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE;				
				AVDTP_SendStartCmd(linkIndex, a2dpLinkInfo->remoteSeid);
				SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
			}		
			break;
			
		case A2DP_ACCEPT_SUSPEND_RESP:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING && a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_MMI_DSP_RESP)
			{
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_OPEN;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_SEND_SUSPEND_RESP;
				if(!a2dpLinkInfo->suspendDelayTimer)
				{
					SYS_SetTimer(&a2dpLinkInfo->suspendDelayTimer, 0L);
				}
			}
			break;
			
		case A2DP_CLOSE_COMPLETE_RESP:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_CLOSING && a2dpLinkInfo->substates == A2DP_SUBSTATE_CLOSE_WAIT_MMI_CLOSE_RESP)
			{
				AVDTP_SendAcceptRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr);
				A2DP_CommandComplete(a2dpLinkInfo);
				a2dpLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
				a2dpLinkInfo->isPassive = TRUE;
				MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_CLOSED_EVT);
			}
			break;
			
		case A2DP_AIR_PACKET_ROLE_RELAYER:
			a2dpLinkInfo->airCtl.packetRole = PACKET_ROLE_RELAYER;
			a2dpLinkInfo->enableSCMS = FALSE;
			break;
		case A2DP_AIR_PACKET_ROLE_FOLLOWER:
			if(a2dpLinkInfo->airCtl.packetRole == PACKET_ROLE_RELAYER)
			{
				if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING)
				{
					if(a2dpLinkInfo->substates == A2DP_SUBSTATE_BEGINNING_STATE)
					{
						//#ifdef DBG_UART_ENABLE_A2DP
						LightDbgPrint("A2P-RelayAVDTP_Suspend2");
						//#endif
						a2dpLinkInfo->substates = A2DP_SUBSTATE_STREAM_WAIT_SUSPEND_RESP;
						MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
						AVDTP_SendSuspendCmd(linkIndex, a2dpLinkInfo->remoteSeid);
						SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_INTERNAL);
						A2DP_SetRelayStateReady(FALSE);
						A2DP_AirModeSetDSPSelByRelayStatus(RELAY_STATUS_FOLLOWER);
						MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
					}
				}			
			}
			a2dpLinkInfo->airCtl.packetRole = PACKET_ROLE_FOLLOWER;
			a2dpLinkInfo->enableSCMS = FALSE;
			break;
			
		case A2DP_AIR_AUDIO_CHANNEL_STEREO:
			a2dpLinkInfo->airCtl.audioChannel = AUDIO_CHANNEL_STEREO;
			break;
		case A2DP_AIR_AUDIO_CHANNEL_MONO_L:
			a2dpLinkInfo->airCtl.audioChannel = AUDIO_CHANNEL_MONO_L;
			break;  
		case A2DP_AIR_AUDIO_CHANNEL_MONO_R:
			a2dpLinkInfo->airCtl.audioChannel = AUDIO_CHANNEL_MONO_R;
			break;
			
		case A2DP_AIR_SET_CHANNEL_SEL_CMD:
			if(MMI_A2DP_SEND_START_RESP_AFTER_DSP_IS_ENABLED_FEAT && a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_DSP_ENABLE_FOR_START_RESP)
			{
				A2DP_SendAcceptStartResponse(linkIndex);
			}
			A2DP_AirModeSetDSPSel();
			break;
			
		case A2DP_HW_AUDIO_CHANNEL_STEREO:
			gA2DP_ctl.relayCtl.hwAudioChannel = HW_AUDIO_CHANNEL_STEREO;		
			goto SET_MODE_SEL;
			
		case A2DP_HW_AUDIO_CHANNEL_MONO_L:
			gA2DP_ctl.relayCtl.hwAudioChannel = HW_AUDIO_CHANNEL_LEFT;
			goto SET_MODE_SEL;
			
		case A2DP_HW_AUDIO_CHANNEL_MONO_R:
			gA2DP_ctl.relayCtl.hwAudioChannel = HW_AUDIO_CHANNEL_RIGHT;
			SET_MODE_SEL:
			if(	MMI_DRV_GetA2DPEnabledLinkIndex() != MMI_EOF
				#ifdef LINEIN_ENABLE
				|| MMI_LineIn_IsDSPOpen()
				#endif
			)
			{
				A2DP_AirModeSetDSPSel();
			}	
			break;

		default:
			break;
	}
}

PUBLIC void A2DP_CloseHandler(U8 linkIndex)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);

	A2DP_ClearLinkInfo(a2dpLinkInfo);
	MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
	MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_CONTROL);	

	//TWS related
	A2DP_SetRelayStateReady(FALSE);
}

PRIVATE void A2DP_CheckTimer(U8 linkIndex)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	if(SYS_IsTimerExpired(&a2dpLinkInfo->timer))
	{
		MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_CONTROL);

		switch(a2dpLinkInfo->mmiProfileInfo.state)
		{
			case PROFILE_DISCONNECTED:
			case PROFILE_CONNECTING:
			case PROFILE_DISCONNECTING:
				break;
			case PROFILE_CONNECTED:
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_CLOSING;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_CLOSE_SEND_CLOSE_CMD;
				break;
			case A2DP_CONFIGURED:
				a2dpLinkInfo->mmiProfileInfo.state =  A2DP_ABORTING;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_SEND_ABORT_CMD;
				break;

			case A2DP_OPEN:
				if(a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_SUSPEND)
				{
					#ifdef AVDTP_PTS_TP_SIG_SMG_BV_19_C
						#undef MMI_A2DP_DO_NOT_SEND_SUSPEND_AFTER_ACTIVE_START_FEAT
						#define MMI_A2DP_DO_NOT_SEND_SUSPEND_AFTER_ACTIVE_START_FEAT	1
					#endif
					
					if(MMI_A2DP_DO_NOT_SEND_SUSPEND_AFTER_ACTIVE_START_FEAT)
					{
						A2DP_CommandComplete(a2dpLinkInfo);
						a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
						MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_START_RESP);
						
						#ifdef AVDTP_PTS_TP_SIG_SMG_BV_19_C
						AVDTP_SendCloseCmd(linkIndex, a2dpLinkInfo->remoteSeid);
						#endif
					}
					else
					{
						AVDTP_SendSuspendCmd(linkIndex, a2dpLinkInfo->remoteSeid);
						a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_SUSPEND_RESPONSE;
					}
				}
				else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_START)
				{
					AVDTP_SendStartCmd(linkIndex, a2dpLinkInfo->remoteSeid);
					a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE_AFTER_ACTIVE_MEDIA_CHANNEL;
				}
				else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP)
				{
					AVDTP_SendRejectRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr, AVDTP_BAD_STATE);
					a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;
				}
				else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_SEND_MEDIA_CHANNEL)
				{
					if(AVDTP_ConnectMediaChannel(linkIndex) != INVALID_L2CAP_CHANNEL)
					{
						a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_MEDIA_CHANNEL_READY;
					}
					else
					{
						SYS_SetTimer(&a2dpLinkInfo->timer, HALF_SEC);
					}					
				}
				else
				{
					a2dpLinkInfo->mmiProfileInfo.state = A2DP_ABORTING;
					a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_SEND_ABORT_CMD;
				}
				break;
			case A2DP_STREAMING:
				if(a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_MMI_DSP_RESP)
				{
					AVDTP_SendRejectRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr, AVDTP_BAD_STATE);
					a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;
				}
				else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_SUSPEND_RESP)
				{
					a2dpLinkInfo->mmiProfileInfo.state = A2DP_ABORTING;
					a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_SEND_ABORT_CMD;
				}
				break;
			case A2DP_ABORTING:
				if(a2dpLinkInfo->substates == A2DP_SUBSTATE_ABORT_WAIT_PASSIVE_RELEASE_MEDIA_CHANNEL)
				{
					a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_SEND_RELEASE_MEDIA_CHANNEL_CMD;
				}
				else
				{
					a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_DECIDE_RELEASE_MEDIA_CHANNEL;
				}
				break;
			case A2DP_CLOSING:
				if(a2dpLinkInfo->substates > A2DP_SUBSTATE_CLOSE_CLOSE_CHANNEL)
				{
					A2DP_CloseHandler(linkIndex);
					MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_DISCONNECTION_FAIL_EVENT);
				}
				else
				{
					a2dpLinkInfo->substates = A2DP_SUBSTATE_CLOSE_CLOSE_CHANNEL;
				}
				break;
			default:
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_ABORTING;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;
				break;

		}
	}
	
	if (SYS_IsTimerExpired(&a2dpLinkInfo->suspendDelayTimer))
	{
		if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_SEND_SUSPEND_RESP)
		{
			AVDTP_SendAcceptRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr);
			A2DP_CommandComplete(a2dpLinkInfo);
			a2dpLinkInfo->mmiProfileInfo.state = A2DP_OPEN;
			MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
			MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_SUSPEND_EVT);		
		}
	}
	
	if(SYS_IsTimerExpired(&a2dpLinkInfo->waitForMediaChannelOpenTimer))
	{
		if(MMI_IsLinkTrafficBusy(linkIndex))
		{
			SYS_SetTimer(&a2dpLinkInfo->waitForMediaChannelOpenTimer, (U32)gMMI_nvram.timerCtl.checkA2DPMediaChannelEstTime * 320L);
		}
		else
		{
			if(a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
			{
			#ifdef A2DP_SRC_Profile
				A2DP_SendDiscoverCommand(linkIndex, A2DP_SEP_SNK, SBC_SRC_SEID);
			#else
				#if SUPPORT_AAC_SNK
				U8 preferredSeid;
				if (!MMI_A2DP_IsHighQualityCodecDisbaled() && AVDTP_IsAacAllowedInLink(linkIndex))
				{
					preferredSeid = AAC_SNK_SEID;
				}
				else
				{
					preferredSeid = SBC_SNK_SEID;
				}
				A2DP_SendDiscoverCommand(linkIndex, A2DP_SEP_SRC, preferredSeid);
				#else
				A2DP_SendDiscoverCommand(linkIndex, A2DP_SEP_SRC, SBC_SNK_SEID);
				#endif
			#endif
			}
		}
	}
}

#ifdef AIR_MODULE
PRIVATE void A2DP_AirHandler(U8 linkIndex)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	A2DP_LINK_INFO XDATA_PTR a2dpNextLinkInfo;
	
	switch(a2dpLinkInfo->airCtl.packetRole)
	{
		case PACKET_ROLE_NONE:

			break;
		case PACKET_ROLE_RELAYER:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_BEGINNING_STATE)
			{
				if(MMI_DRV_GetA2DPEnabledLinkIndex() != MMI_EOF)
				{
					#ifdef DEMOSOUND
					if(DEMOSOUND_LINKINDEX == MMI_DRV_GetA2DPEnabledLinkIndex())
					{
						if(!MMI_AIR_SetLinkRoleMaster(linkIndex))
							return;
							
						//#ifdef DBG_UART_ENABLE_A2DP
						LightDbgPrint("A2P-RelayAVDTP_Start:DemoSound");
						//#endif
						a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_START_RESP_FROM_FOLLOWER;
						SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_INTERNAL);
						
						MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_GUARANTEED, FALSE, FALSE);
						
						a2dpLinkInfo->remoteSeid = gDriver_ctl.musicCodecType;
						AVDTP_SendStartCmd(linkIndex, a2dpLinkInfo->remoteSeid);
						MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
						gA2DP_ctl.relayCtl.audioSource = RELAY_SOURCE_DEMOSOUND;
						gA2DP_ctl.relayCtl.audioSourceLinkIndex = MMI_DRV_GetA2DPEnabledLinkIndex();			
					}
					else
					#endif
					{
						a2dpNextLinkInfo = A2DP_GetLinkInfo(MMI_DRV_GetA2DPEnabledLinkIndex());
						if(a2dpNextLinkInfo->mmiProfileInfo.state == A2DP_STREAMING)
						{
							if(!MMI_AIR_SetLinkRoleMaster(linkIndex))
								return;
								
							//#ifdef DBG_UART_ENABLE_A2DP
							LightDbgPrint("A2P-RelayAVDTP_Start:Music");
							//#endif
							MMI_AIR_A2DPStartRelay(MMI_DRV_GetA2DPEnabledLinkIndex());
							a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_START_RESP_FROM_FOLLOWER;
							SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_INTERNAL);
							
							MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_GUARANTEED, FALSE, FALSE);
							
							a2dpLinkInfo->remoteSeid = a2dpNextLinkInfo->localSeid;
							AVDTP_SendStartCmd(linkIndex, a2dpLinkInfo->remoteSeid);
							MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
							gA2DP_ctl.relayCtl.audioSource = RELAY_SOURCE_MUSIC;
							gA2DP_ctl.relayCtl.audioSourceLinkIndex = MMI_DRV_GetA2DPEnabledLinkIndex();
							
						}
					}
				}
				#if (defined LINEIN_ENABLE) &&  (defined BLUETOOTH_SPEAKER)
				else if(MMI_LineIn_IsDSPOpen())
				{
					if(!MMI_AIR_SetLinkRoleMaster(linkIndex))
						return;
						
					//#ifdef DBG_UART_ENABLE_A2DP
					LightDbgPrint("A2P-RelayAVDTP_Start:AUX");
					//#endif
					
					MMI_AIR_LineInStartRelay();
					a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_START_RESP_FROM_FOLLOWER;
					MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_GUARANTEED, FALSE, FALSE);
					
					a2dpLinkInfo->remoteSeid = AUX_ENCODER_SEID;
					a2dpLinkInfo->fA2DPRxMediaCodec = A2DP_UndefinedDecoderRxMediaPacket;
					AVDTP_SendStartCmd(linkIndex, a2dpLinkInfo->remoteSeid);
					SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_INTERNAL);
					MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
					gA2DP_ctl.relayCtl.audioSource = RELAY_SOURCE_AUX;
					gA2DP_ctl.relayCtl.audioSourceLinkIndex = MMI_EOF;
				}
				#endif
			}
			else if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING && a2dpLinkInfo->substates == A2DP_SUBSTATE_BEGINNING_STATE)
			{
				#ifdef DEMOSOUND
				if(gA2DP_ctl.relayCtl.audioSource == RELAY_SOURCE_DEMOSOUND)
				{
					U8 a2dpDSPLink = MMI_DRV_GetA2DPEnabledLinkIndex();
					if(a2dpDSPLink == MMI_EOF)
					{
						#if (defined LINEIN_ENABLE) && (defined BLUETOOTH_SPEAKER)
						if(MMI_LineIn_IsDSPOpen())
						{
							//#ifdef DBG_UART_ENABLE_A2DP
							LightDbgPrint("A2P-Relay_Suspend:AUX Open");
							//#endif
							goto RELAY_SEND_SUSPEND;								
						}
						#endif
						
						if(!gA2DP_ctl.relayCtl.delaySendSuspendTimer)
						{
							LightDbgPrint("A2P-Relay_Suspend:Timer");
							if(DRIVER_Query_IsDSPSuspendingReason(DRV_SUSPEND_DSP_REASON_AIRAPP) || 
								DRIVER_Query_IsWaitingDSPSuspend(DRV_SUSPEND_DSP_REASON_AIRAPP))
							{
								LightDbgPrint("A2P-Relay_Suspend-APP:Timer");
								SYS_SetTimer(&gA2DP_ctl.relayCtl.delaySendSuspendTimer, 16);
							}
							else
							SYS_SetTimer(&gA2DP_ctl.relayCtl.delaySendSuspendTimer, 2 * ONE_SEC);
						}
						else if(SYS_IsTimerExpired(&gA2DP_ctl.relayCtl.delaySendSuspendTimer))
						{
							//#ifdef DBG_UART_ENABLE_A2DP
							LightDbgPrint("A2P-Relay_Suspend:MusicOff");
							//#endif
							goto RELAY_SEND_SUSPEND;							
						}
					}
					else if(a2dpDSPLink != gA2DP_ctl.relayCtl.audioSourceLinkIndex)
					{
						//#ifdef DBG_UART_ENABLE_A2DP
						LightDbgPrint("A2P-Relay_Suspend:changeLink");
						//#endif
						goto RELAY_SEND_SUSPEND;
					}
					else
					{
						if(a2dpLinkInfo->remoteSeid != gDriver_ctl.musicCodecType)
						{
							//#ifdef DBG_UART_ENABLE_A2DP
							LightDbgPrint("A2P-Relay_Suspend:MusicSeid");
							//#endif
							goto RELAY_SEND_SUSPEND;
						}
						SYS_ReleaseTimer(&gA2DP_ctl.relayCtl.delaySendSuspendTimer);
					}
				}
				else 
				#endif
				if(gA2DP_ctl.relayCtl.audioSource == RELAY_SOURCE_MUSIC)
				{
					U8 a2dpDSPLink = MMI_DRV_GetA2DPEnabledLinkIndex();
					if(a2dpDSPLink == MMI_EOF)
					{
						#if (defined LINEIN_ENABLE) && (defined BLUETOOTH_SPEAKER)
						if(MMI_LineIn_IsDSPOpen())
						{
							//#ifdef DBG_UART_ENABLE_A2DP
							LightDbgPrint("A2P-Relay_Suspend:AUX Open");
							//#endif
							goto RELAY_SEND_SUSPEND;								
						}
						#endif
						
						if(!gA2DP_ctl.relayCtl.delaySendSuspendTimer)
						{
							LightDbgPrint("A2P-Relay_Suspend:Timer");
							if(DRIVER_Query_IsDSPSuspendingReason(DRV_SUSPEND_DSP_REASON_AIRAPP) || 
								DRIVER_Query_IsWaitingDSPSuspend(DRV_SUSPEND_DSP_REASON_AIRAPP))
							{
								LightDbgPrint("A2P-Relay_Suspend-APP:Timer");
								SYS_SetTimer(&gA2DP_ctl.relayCtl.delaySendSuspendTimer, 16);
							}
							else
							SYS_SetTimer(&gA2DP_ctl.relayCtl.delaySendSuspendTimer, 2 * ONE_SEC);
						}
						else if(SYS_IsTimerExpired(&gA2DP_ctl.relayCtl.delaySendSuspendTimer))
						{
							//#ifdef DBG_UART_ENABLE_A2DP
							LightDbgPrint("A2P-Relay_Suspend:MusicOff");
							//#endif
							goto RELAY_SEND_SUSPEND;							
						}
					}
					else if(a2dpDSPLink != gA2DP_ctl.relayCtl.audioSourceLinkIndex)
					{
						//#ifdef DBG_UART_ENABLE_A2DP
						LightDbgPrint("A2P-Relay_Suspend:changeLink");
						//#endif
						goto RELAY_SEND_SUSPEND;
					}
					else
					{
						a2dpNextLinkInfo = A2DP_GetLinkInfo(a2dpDSPLink);
						if(a2dpLinkInfo->remoteSeid != a2dpNextLinkInfo->localSeid)
						{
							//#ifdef DBG_UART_ENABLE_A2DP
							LightDbgPrint("A2P-Relay_Suspend:MusicSeid");
							//#endif
							goto RELAY_SEND_SUSPEND;
						}
						SYS_ReleaseTimer(&gA2DP_ctl.relayCtl.delaySendSuspendTimer);
					}
				}
				#if (defined LINEIN_ENABLE) &&  (defined BLUETOOTH_SPEAKER)
				else if(gA2DP_ctl.relayCtl.audioSource == RELAY_SOURCE_AUX)
				{
					if(!MMI_LineIn_IsDSPOpen())
					{
						//#ifdef DBG_UART_ENABLE_A2DP
						LightDbgPrint("A2P-Relay_Suspend:AUX");
						//#endif
						goto RELAY_SEND_SUSPEND;						
					}
					break;
				}
				#endif
			}
			break;
		case PACKET_ROLE_FOLLOWER:

			break;
	}
	
	return;
	
	RELAY_SEND_SUSPEND:
	a2dpLinkInfo->substates = A2DP_SUBSTATE_STREAM_WAIT_SUSPEND_RESP;
	MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
	AVDTP_SendSuspendCmd(linkIndex, a2dpLinkInfo->remoteSeid);
	SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_INTERNAL);
	A2DP_SetRelayStateReady(FALSE);
	A2DP_AirModeSetDSPSelByRelayStatus(RELAY_STATUS_FOLLOWER);
	MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
	SYS_ReleaseTimer(&gA2DP_ctl.relayCtl.delaySendSuspendTimer);
	gA2DP_ctl.relayCtl.audioSource = 0;
	gA2DP_ctl.relayCtl.audioSourceLinkIndex = MMI_EOF;	
}
#endif

PRIVATE MMI_PROFILE_INFO XDATA_PTR A2DP_ConnectRequest(U8 linkIndex, U8 profileId)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	UNUSED(profileId);

	if (a2dpLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
	{
		if (MMI_SDAP_GetRemoteServiceChannel(linkIndex, PROFILE_A2DP_SOURCE) != 0)
		{
			if (AVDTP_ConnectSignalChannel(linkIndex) != INVALID_L2CAP_CHANNEL)
			{
				return &a2dpLinkInfo->mmiProfileInfo;
			}
		}
	}
	return (MMI_PROFILE_INFO XDATA_PTR)NULL;
}

PRIVATE U8 A2DP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);

	if (&a2dpLinkInfo->mmiProfileInfo == profileInfo)
	{
		switch(a2dpLinkInfo->mmiProfileInfo.state)
		{
			case A2DP_STREAMING:
			case A2DP_OPEN:
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_CLOSING;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_CLOSE_SEND_CLOSE_CMD;		
				/* fall through */
			case A2DP_CLOSING:
				return CMD_WAITING;
			case PROFILE_CONNECTED:
			case A2DP_CONFIGURED:
			case PROFILE_CONNECTING:
			case PROFILE_DISCONNECTING:
				if(CMD_WAITING == AVDTP_DisconnectSignalChannel(linkIndex))
				{
					a2dpLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTING; 
					return CMD_WAITING;
				}
				break;
		}
		MMI_A2DP_ClearCtlData(linkIndex);
		MMI_UnloadProfile(linkIndex, &a2dpLinkInfo->mmiProfileInfo);
		A2DP_CloseHandler(linkIndex);
	}
	return CMD_COMPLETE;
}

PRIVATE void A2DP(U8 linkIndex)
{
	//State Machine.
	A2DP_SignalHandler(linkIndex);
	// Check if time out
	A2DP_CheckTimer(linkIndex);
	#ifdef AIR_MODULE
	A2DP_AirHandler(linkIndex);
	#endif
}

PRIVATE void _InitMediaCodec(void)//Mantis 8599 - avoid Warning C183 : Dead Assignment Eliminated
{
	U8 linkIndex;
	for (linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		A2DP_GetLinkInfo(linkIndex)->fA2DPRxMediaCodec = A2DP_UndefinedDecoderRxMediaPacket;
	}
}

PUBLIC void A2DP_Init(void)
{
	U8 GENERIC_PTR attributePtr;
	
#if USING_DIRECT_A2DP_NVARM
	pA2DP_nvram = (A2DP_NVRAM_CTL_TYPE CODE_PTR) GET_SECTOR_CFG1_ADDR(SECTOR_A2DP_NVRAM_CTL_INIT);
#else
	OSMEM_memcpy_xdata_code ((U8 XDATA_PTR) &gA2DP_nvram, GET_SECTOR_CFG1_ADDR(SECTOR_A2DP_NVRAM_CTL_INIT), sizeof (A2DP_NVRAM_CTL_TYPE));
#endif

	_InitMediaCodec();
	
	{
		#ifdef A2DP_SRC_Profile
		extern SdpServiceNode gA2DP_SourceServiceNode;
		SDAP_RegisterServiceRecord(&gA2DP_SourceServiceNode);
		#else
		extern SdpServiceNode gA2DP_SinkServiceNode;
		SDAP_RegisterServiceRecord(&gA2DP_SinkServiceNode);
		#endif
	}
	
	if (attributePtr = (U8 GENERIC_PTR)SDAP_GetServiceAttribute(PROFILE_A2DP_SINK, ATTRID_SupportedFeatures))
	{
		*(U16 GENERIC_PTR)&attributePtr[4] = gMMI_nvram.productType.sinkSupportedFeatures;
	}

	MMI_RegisterProfile(&gA2DP_ProfileNode);
	
	#ifdef PEQ_ENABLE
	LightDbgPrint("A2DP: PEQ set number %d", (U8)PEQ_GetModeNumber(PEQ_A2DP));
	#endif
}

PUBLIC void A2DP_SetChannelOpenTimer(U8 linkIndex, U32 setTime)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	SYS_SetTimer(&a2dpLinkInfo->waitForMediaChannelOpenTimer,setTime);
}
#endif //A2DP_Profile
