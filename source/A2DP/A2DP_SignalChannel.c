//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _A2DP_SIGNAL_CHANNEL_C_

#include "A2DP.h"
#include "align_flash.h"

#include "..\MMI\MMI.h"
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_Protocol.h"
#include "..\MMI\MMI_A2DP.h"
#include "..\MMI\MMI_Driver.h"
#ifdef AIR_MODULE
	#include "..\MMI\MMI_AirModule.h"
#endif

#ifdef A2DP_Profile

#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];

PRIVATE void A2DP_ClearSubStateAndTimer(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo)
{
	a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;
	SYS_MemoryRelease(&a2dpLinkInfo->discovSeidInfoPtr);
	SYS_ReleaseTimer(&a2dpLinkInfo->timer);
}


/////////////////////////////////////////////////////////////////////
// Delay Report Handler
/////////////////////////////////////////////////////////////////////
PUBLIC void A2DP_SendInitDelayReport(U8 linkIndex)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	A2DP_DelayReportClearPara(linkIndex);
	#if SUPPORT_SBC_SNK
	a2dpLinkInfo->delayReportCtl.reportedValue = ((a2dpLinkInfo->localSeid == AAC_SNK_SEID) ? 200 : 150) * A2DP_DspBufferDepth(linkIndex);
	#endif
	
	AVDTP_SendDelayReportCmd(linkIndex, a2dpLinkInfo->remoteSeid, a2dpLinkInfo->delayReportCtl.reportedValue);
}

PUBLIC void A2DP_SendDelayReport(U8 linkIndex, U16 newDelay, BOOL isBigger)
{
	U8 i, quot;
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);

	if(isBigger)
		quot = newDelay - a2dpLinkInfo->delayReportCtl.reportedValue;
	else
		quot = a2dpLinkInfo->delayReportCtl.reportedValue - newDelay;

	quot = quot / DELAY_REPORT_MAX_DEVIATION;


	for(i = 0; i <= quot; i++)
	{
		if(i == quot)
			AVDTP_SendDelayReportCmd(linkIndex, a2dpLinkInfo->remoteSeid, newDelay);
		else
		{
			U16 delayValue = (isBigger) ? 
							(a2dpLinkInfo->delayReportCtl.reportedValue + (i+1) * DELAY_REPORT_MAX_DEVIATION) :
							(a2dpLinkInfo->delayReportCtl.reportedValue - (i+1)* DELAY_REPORT_MAX_DEVIATION);

			AVDTP_SendDelayReportCmd(linkIndex, a2dpLinkInfo->remoteSeid, delayValue);
		}
	}

	a2dpLinkInfo->delayReportCtl.reportedValue = newDelay;
}

PUBLIC void A2DP_DelayReportClearPara(U8 linkIndex)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	a2dpLinkInfo->delayReportCtl.pktCnt = 0;
	a2dpLinkInfo->delayReportCtl.isDspPlay = FALSE;
}

PUBLIC void A2DP_CalculateDelayReportValue(U8 linkIndex)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	U16 delay = gAlignDSP.DSP_RESERVED_PARA_1;
	
	if(!a2dpLinkInfo->delayReportCtl.isDspPlay)
	{
		a2dpLinkInfo->delayReportCtl.pktCnt++;
		
		#if SUPPORT_SBC_SNK
		if(a2dpLinkInfo->delayReportCtl.pktCnt >= 2 * A2DP_DspBufferDepth(linkIndex))
			a2dpLinkInfo->delayReportCtl.isDspPlay = TRUE;
		#endif
	}
	else if(!(gAlignDSP.debug_var03 >> 8))
	{
		A2DP_DelayReportClearPara(linkIndex);
	}
	
	if(!a2dpLinkInfo->delayReportCtl.isDspPlay)
		return;
	
	if(delay > a2dpLinkInfo->delayReportCtl.reportedValue &&
		delay - a2dpLinkInfo->delayReportCtl.reportedValue >= DELAY_REPORT_RECOMMEND_DEVIATION)
	{
		A2DP_SendDelayReport(linkIndex, delay, TRUE);
	}
	else if(delay < a2dpLinkInfo->delayReportCtl.reportedValue &&
		a2dpLinkInfo->delayReportCtl.reportedValue - delay >= DELAY_REPORT_RECOMMEND_DEVIATION)
	{
		A2DP_SendDelayReport(linkIndex, delay, FALSE);
	}
}


/////////////////////////////////////////////////////////////////////
// Signal Command Handler
/////////////////////////////////////////////////////////////////////

PUBLIC BOOL A2DP_CheckSeidInUse(U8 linkIndex, U8 seid)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	if(linkIndex < MAX_MULTI_POINT_NO)
	{
		if (a2dpLinkInfo->mmiProfileInfo.state == A2DP_CONFIGURED ||
			a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN ||
			a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING)
		{
			if (seid == a2dpLinkInfo->localSeid)
				return TRUE;
		}
	}
	return FALSE;
}

PRIVATE U8 A2DP_SignalCheckIntegrity(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo, AvdtpSignalType XDATA_PTR eventPtr)
{
	U8 serviceLength;
	U16 capabilitiesLength;
	U8 XDATA_PTR categoryPtr;
	U8 acpSeid;

	switch(eventPtr->signalIdentifier & 0x3F)
	{
		case AVDTP_DISCOVER:
			if( eventPtr->l2capLength < 2 )
			{
				return AVDTP_BAD_LENGTH;
			}
			break;

		case AVDTP_SET_CONFIGURATION:
			if( eventPtr->l2capLength < 4 )
			{
				return AVDTP_BAD_LENGTH;
			}
			if ( A2DP_CheckSeidInUse(eventPtr->offset, (eventPtr->para[0]>>2)) )
			{
				return AVDTP_SEP_IN_USE;
			}
			//Check the format of the media transport category
			categoryPtr = &eventPtr->para[2];
			capabilitiesLength = eventPtr->l2capLength - 4;
			while(capabilitiesLength >= 2)
			{
				if (categoryPtr[0] == MEDIA_TRANSPORT && categoryPtr[1] != 0)
				{
					return AVDTP_BAD_MEDIA_TRANSPORT_FORMAT;
				}
				if (categoryPtr[0] == CONTENT_PROTECTION && categoryPtr[1] != 2)
				{
					return AVDTP_BAD_CP_FORMAT;
				}
				serviceLength = 2 + categoryPtr[1];
				categoryPtr += serviceLength;
				capabilitiesLength -= serviceLength;
			}
			break;

		case AVDTP_RECONFIGURE:
			if (eventPtr->para[1] > DELAY_REPORTING)
			{
				return AVDTP_BAD_SERV_CATEGORY;
			}
			if (eventPtr->para[1] != MEDIA_CODEC)	//AVDTP_spec v1.3, pg.63 Note 3.
			{
				return AVDTP_INVALID_CAPABILITIES;
			}
			/* fallthrough */
		case AVDTP_GET_CONFIGURATION:
		case AVDTP_OPEN:
		case AVDTP_START:
		case AVDTP_CLOSE:
		case AVDTP_SUSPEND:
		case AVDTP_SECURITY_CONTROL:
			acpSeid = eventPtr->para[0] >> 2;
			if (a2dpLinkInfo->mmiProfileInfo.state < A2DP_CONFIGURED)
			{
				return AVDTP_BAD_STATE;
			}
			if (acpSeid == 0 || acpSeid > 0x3E)
			{
				return AVDTP_BAD_ACP_SEID;
			}
			if(acpSeid != a2dpLinkInfo->localSeid && a2dpLinkInfo->airCtl.packetRole != PACKET_ROLE_FOLLOWER)
			{
				return AVDTP_BAD_ACP_SEID;
			}
			/* fallthrough */
		case AVDTP_GET_CAPABILITIES:
		case AVDTP_GET_ALL_CAPABILITIES:
		case AVDTP_ABORT:
			if (eventPtr->l2capLength < 3)
			{
				return AVDTP_BAD_LENGTH;
			}
			break;
	}
	return AVDTP_NO_ERROR;
}

PRIVATE BOOL A2DP_SetCodecPara(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo, U8 codec)
{
	switch(codec)
	{
		#if SUPPORT_SBC_SNK
		case SBC_SNK_SEID:
			//#ifdef DBG_UART_ENABLE_A2DP
			LightDbgPrint("A2P - SBC");
			//#endif
			a2dpLinkInfo->fA2DPRxMediaCodec = A2DP_SBCDecoderRxMediaPacket;
			return TRUE;
		#endif
			
		#if SUPPORT_AAC_SNK
		case AAC_SNK_SEID:
			//#ifdef DBG_UART_ENABLE_A2DP
			LightDbgPrint("A2P - AAC");
			//#endif
			a2dpLinkInfo->fA2DPRxMediaCodec = A2DP_AACDecoderRxMediaPacket;
			return TRUE;
		#endif
		
		#if SUPPORT_SBC_SRC
		case SBC_SRC_SEID:
			a2dpLinkInfo->fA2DPRxMediaCodec = (A2DP_MEDIA_RX_FUNC)A2DP_UndefinedDecoderRxMediaPacket;
			return TRUE;
		#endif
		
		default:
			if(api_profile_func[API_GRP_PF_CB_AVDTP_SET_MEDIA_PKT_DECODER])
			{
				a2dpLinkInfo->fA2DPRxMediaCodec = (A2DP_MEDIA_RX_FUNC)(((API_PROFILE_AVDTP_SET_MEDIA_PKT_DECODER_FUNC)api_profile_func[API_GRP_PF_CB_AVDTP_SET_MEDIA_PKT_DECODER])(codec));
				if(a2dpLinkInfo->fA2DPRxMediaCodec)
					return TRUE;
			}
						
			a2dpLinkInfo->fA2DPRxMediaCodec = A2DP_UndefinedDecoderRxMediaPacket;
			return FALSE;
	}
}

PRIVATE U8 A2DP_ParseCapabilities(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo, U8 XDATA_PTR categoryPtr, U16 capabilitiesLength, U8 cfgSeid)
{
	U8 value, serviceLength;
	U8 caps = 0;

	while (capabilitiesLength >= 2)
	{
		switch (categoryPtr[0])
		{
			case MEDIA_CODEC:
				if(categoryPtr[2] != AUDIO_TYPE)
					return AVDTP_UNSUPPORTED_CONFIGURATION;
				
				switch (cfgSeid)
				{
					#if SUPPORT_SBC_SNK
					case SBC_SNK_SEID:
					#endif
					#if SUPPORT_SBC_SRC
					case SBC_SRC_SEID:
					#endif
						if (categoryPtr[3] != SBC_CODEC)
							return AVDTP_UNSUPPORTED_CONFIGURATION;

						value = categoryPtr[4] & (A2DP_SBC_SAMPLE_FREQ_48000|A2DP_SBC_SAMPLE_FREQ_44100|A2DP_SBC_SAMPLE_FREQ_32000);
						if (value == 0)
							return AVDTP_UNSUPPORTED_CONFIGURATION;

						OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&a2dpLinkInfo->codecSettings.sbc, (U8 XDATA_PTR)&categoryPtr[4], sizeof(SBCCodecSettings));
						break;

					#if SUPPORT_AAC_SNK
					case AAC_SNK_SEID:
						if (categoryPtr[3] != AAC_CODEC)
							return AVDTP_UNSUPPORTED_CONFIGURATION;
						
						value = categoryPtr[4] & A2DP_AAC_OBJECT_TYPE_MPEG2_AAC_LC;
						if (value == 0)
							return AVDTP_UNSUPPORTED_CONFIGURATION;

						value = categoryPtr[5] & (A2DP_AAC_SAMPLE_FREQ_44100|A2DP_AAC_SAMPLE_FREQ_32000|A2DP_AAC_SAMPLE_FREQ_24000|A2DP_AAC_SAMPLE_FREQ_22050|A2DP_AAC_SAMPLE_FREQ_16000|A2DP_AAC_SAMPLE_FREQ_12000|A2DP_AAC_SAMPLE_FREQ_11025|A2DP_AAC_SAMPLE_FREQ_8000);
						if (value == 0 && (categoryPtr[6] & A2DP_AAC_SAMPLE_FREQ_48000) == 0)
							return AVDTP_UNSUPPORTED_CONFIGURATION;

						if ((categoryPtr[6] & (A2DP_AAC_CHANNELS_1|A2DP_AAC_CHANNELS_2)) == 0)
							return AVDTP_UNSUPPORTED_CONFIGURATION;

						OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&a2dpLinkInfo->codecSettings.aac, (U8 XDATA_PTR)&categoryPtr[4], sizeof(AACCodecSettings));
						break;
					#endif
					
					default:
						if(api_profile_func[API_GRP_PF_CB_AVDTP_SET_CONFIG_CMD])
						{
							U8 cpapLen = ((API_PROFILE_AVDTP_SET_CONFIG_CMD_FUNC)api_profile_func[API_GRP_PF_CB_AVDTP_SET_CONFIG_CMD])(cfgSeid, categoryPtr);
							if(cpapLen)
							{
								OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&a2dpLinkInfo->codecSettings.vendor, (U8 XDATA_PTR)&categoryPtr[4], cpapLen);
							}
							else
							{
								return AVDTP_UNSUPPORTED_CONFIGURATION;
							}
						}
						else
						{
							return AVDTP_UNSUPPORTED_CONFIGURATION;
						}
						break;
				}
				break;

			case CONTENT_PROTECTION:
				if(categoryPtr[2] == A2DP_CP_SCMS_T && categoryPtr[3] == 0)
				{
					caps |= A2DP_CAPS_CONTENT_PROTECTION;
				}
				break;

			case DELAY_REPORTING:
				if (MMI_A2DP_DELAY_REPORT_FEAT)
				{
					caps |= A2DP_CAPS_DELAY_REPORTING;
				}
				break;
		}

		serviceLength = 2 + categoryPtr[1];
		categoryPtr += serviceLength;
		capabilitiesLength -= serviceLength;
	}

	a2dpLinkInfo->enableSCMS = (caps & A2DP_CAPS_CONTENT_PROTECTION) ? TRUE : FALSE;
	a2dpLinkInfo->enableDelayReport = (caps & A2DP_CAPS_DELAY_REPORTING) ? TRUE : FALSE;
	return AVDTP_NO_ERROR;
}

PRIVATE U8 A2DP_SetServiceConfiguration(U8 linkIndex, AvdtpSignalConfigType XDATA_PTR eventPtr, U8 isReConfig)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	U8 cfgSeid = eventPtr->acpSEID >> 2;

	if(isReConfig && a2dpLinkInfo->localSeid != cfgSeid)
		return AVDTP_BAD_ACP_SEID;

	if(!A2DP_SetCodecPara(a2dpLinkInfo, cfgSeid))
	{
		return AVDTP_BAD_ACP_SEID;
	}

	if(isReConfig)
	{
		return A2DP_ParseCapabilities(a2dpLinkInfo, (U8 XDATA_PTR)&eventPtr->intSEID, eventPtr->l2capLength-3, cfgSeid);
	}
	else
	{
		return A2DP_ParseCapabilities(a2dpLinkInfo, (U8 XDATA_PTR)&eventPtr->configData[0], eventPtr->l2capLength-4, cfgSeid);
	}
}

PRIVATE U8 A2DP_GetServiceConfiguration(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo, AvdtpSignalType XDATA_PTR eventPtr)
{
	U8 XDATA_PTR servicePtr;

	if(a2dpLinkInfo->localSeid != (eventPtr->para[0]>>2))
	{
		return AVDTP_BAD_ACP_SEID;
	}

	servicePtr = &eventPtr->para[0];

	servicePtr[0] = MEDIA_TRANSPORT;
	servicePtr[1] = 0;
	servicePtr += 2;

	if(a2dpLinkInfo->enableSCMS)
	{
		servicePtr[0] = CONTENT_PROTECTION;
		servicePtr[1] = 0x02;
		servicePtr[2] = A2DP_CP_SCMS_T;
		servicePtr[3] = 0;
		servicePtr += 4;
	}

	servicePtr[0] = MEDIA_CODEC;
	servicePtr[2] = AUDIO_TYPE;
	switch (a2dpLinkInfo->localSeid)
	{
		#if SUPPORT_SBC_SNK
		case SBC_SNK_SEID:
		#endif
		#if SUPPORT_SBC_SRC
		case SBC_SRC_SEID:
		#endif
			servicePtr[1] = sizeof(SBCCodecSettings) + 2;
			servicePtr[3] = SBC_CODEC;
			OSMEM_memcpy_xdata_xdata(&servicePtr[4], (U8 XDATA_PTR)&a2dpLinkInfo->codecSettings.sbc, sizeof(SBCCodecSettings));
			break;

		#if SUPPORT_AAC_SNK
		case AAC_SNK_SEID:
			servicePtr[1] = sizeof(AACCodecSettings) + 2;
			servicePtr[3] = AAC_CODEC;
			OSMEM_memcpy_xdata_xdata(&servicePtr[4], (U8 XDATA_PTR)&a2dpLinkInfo->codecSettings.aac, sizeof(AACCodecSettings));
			break;
		#endif
		default:
			if(api_profile_func[API_GRP_PF_CB_AVDTP_GET_CONFIG_CMD])
			{
				if(!((API_PROFILE_AVDTP_GET_CONFIG_CMD_FUNC)api_profile_func[API_GRP_PF_CB_AVDTP_GET_CONFIG_CMD])(a2dpLinkInfo->localSeid, servicePtr, (U8 XDATA_PTR)&a2dpLinkInfo->codecSettings.vendor))
				{
					return AVDTP_BAD_ACP_SEID;
				}
			}			
			break;
	}
	servicePtr += servicePtr[1] + 2;

	if (a2dpLinkInfo->enableDelayReport)
	{
		servicePtr[0] = DELAY_REPORTING;
		servicePtr[1] = 0;
		servicePtr += 2;
	}

	eventPtr->l2capLength = (servicePtr - &eventPtr->para[0]);
	return AVDTP_NO_ERROR;
}

PRIVATE void A2DP_SignalCommandHandler(U8 linkIndex, AvdtpSignalType XDATA_PTR eventPtr)
{
	U8 errorCode;
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	if ((errorCode = A2DP_SignalCheckIntegrity(a2dpLinkInfo, eventPtr)) != AVDTP_NO_ERROR)
	{
		AVDTP_SendRejectRsp(linkIndex, eventPtr, errorCode);
		return;
	}

	switch(eventPtr->signalIdentifier & 0x3F)
	{
		case AVDTP_DISCOVER:
			if(a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED && a2dpLinkInfo->substates != A2DP_SUBSTATE_IDLE_WAIT_SET_CONFIG_RESP)
				A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
			break;
		case AVDTP_GET_CAPABILITIES:
		case AVDTP_GET_ALL_CAPABILITIES:
			if(a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED && a2dpLinkInfo->substates != A2DP_SUBSTATE_IDLE_WAIT_SET_CONFIG_RESP)
				A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
			break;
		case AVDTP_SET_CONFIGURATION:
			if(a2dpLinkInfo->mmiProfileInfo.state != PROFILE_CONNECTED)
			{
				errorCode = AVDTP_SEP_IN_USE;
				break;
			}
			if((errorCode = A2DP_SetServiceConfiguration(linkIndex, (AvdtpSignalConfigType XDATA_PTR)eventPtr, FALSE)) == AVDTP_NO_ERROR)
			{
				a2dpLinkInfo->localSeid = ((AvdtpSignalConfigType XDATA_PTR)eventPtr)->acpSEID >> 2;
				a2dpLinkInfo->remoteSeid = ((AvdtpSignalConfigType XDATA_PTR)eventPtr)->intSEID >> 2;
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_CONFIGURED;
				A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
			}
			break;
		case AVDTP_GET_CONFIGURATION:
			if(!(a2dpLinkInfo->mmiProfileInfo.state >= A2DP_CONFIGURED && a2dpLinkInfo->mmiProfileInfo.state <= A2DP_STREAMING))
			{
				errorCode = AVDTP_BAD_STATE;
				break;
			}
			errorCode = A2DP_GetServiceConfiguration(a2dpLinkInfo, eventPtr);
			break;
		case AVDTP_RECONFIGURE:
			if(a2dpLinkInfo->mmiProfileInfo.state != A2DP_OPEN)
			{
				errorCode = AVDTP_BAD_STATE;
				break;
			}
			errorCode = A2DP_SetServiceConfiguration(linkIndex, (AvdtpSignalConfigType XDATA_PTR)eventPtr, TRUE);
			break;
		case AVDTP_OPEN:
			if(a2dpLinkInfo->mmiProfileInfo.state != A2DP_CONFIGURED)
			{
				errorCode = AVDTP_BAD_STATE;
				break;
			}
			a2dpLinkInfo->mmiProfileInfo.state = A2DP_OPEN;
			A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
			break;
		case AVDTP_START:
			if(a2dpLinkInfo->mmiProfileInfo.state != A2DP_OPEN)
			{
				errorCode = AVDTP_BAD_STATE;
				break;
			}
			if(a2dpLinkInfo->airCtl.packetRole == PACKET_ROLE_FOLLOWER)
			{
				U8 acpSEID = (((AvdtpSignalConfigType XDATA_PTR)eventPtr)->acpSEID >> 2);
				if(acpSEID == AUX_ENCODER_SEID)
				{
					a2dpLinkInfo->localSeid = SBC_SNK_SEID;
					MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_FOLLOWER_AUX);
				}
				else
				{
					a2dpLinkInfo->localSeid = acpSEID;
					MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_FOLLOWER_MUSIC);
				}
				A2DP_SetCodecPara(a2dpLinkInfo, a2dpLinkInfo->localSeid);
			}
			else if(a2dpLinkInfo->airCtl.packetRole == PACKET_ROLE_RELAYER)
			{
				a2dpLinkInfo->remoteReqPtr = (U8 XDATA_PTR)eventPtr;
				eventPtr = 0;
				SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
				AVDTP_SendAcceptRsp(linkIndex, (AvdtpSignalType XDATA_PTR)a2dpLinkInfo->remoteReqPtr);
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
				A2DP_CommandComplete(a2dpLinkInfo);
				MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
				break;
			}
			
			if(	a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP ||
				a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_OPEN_MEDIA_CHANNEL||
				a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_MEDIA_CHANNEL_READY)
			{
				OSMQ_PutFront(&a2dpLinkInfo->OSMQ_AVDTP_Signal_To_A2DP, (U8 XDATA_PTR)eventPtr);
				eventPtr = 0;
			}
			else
			{
				a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP;
				a2dpLinkInfo->remoteReqPtr = (U8 XDATA_PTR)eventPtr;
				eventPtr = 0;
				SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
				MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_START_REQ);
			}
			break;
		case AVDTP_CLOSE:
			if(a2dpLinkInfo->mmiProfileInfo.state != A2DP_OPEN && a2dpLinkInfo->mmiProfileInfo.state != A2DP_STREAMING)
			{
				errorCode = AVDTP_BAD_STATE;
				break;
			}
			MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_FOLLOWER_MUSIC);
			if(	a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP||
				a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_MEDIA_CHANNEL_READY||
				a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_MMI_DSP_RESP)
			{
				OSMQ_PutFront(&a2dpLinkInfo->OSMQ_AVDTP_Signal_To_A2DP, (U8 XDATA_PTR)eventPtr);
				eventPtr = 0;
			}
			else
			{
				SYS_MemoryRelease(&a2dpLinkInfo->remoteReqPtr);
				a2dpLinkInfo->remoteReqPtr = (U8 XDATA_PTR)eventPtr;
				eventPtr = 0;

				if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING)
				{
					MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
				}
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_CLOSING;
				a2dpLinkInfo->substates =A2DP_SUBSTATE_CLOSE_WAIT_MMI_CLOSE_RESP;
				A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
				A2DP_DelayReportClearPara(linkIndex);
				SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
				MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_CLOSE_REQ);
			}
			break;
		case AVDTP_SUSPEND:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING)
			{
				if(a2dpLinkInfo->substates == A2DP_SUBSTATE_BEGINNING_STATE)
				{
					A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
					a2dpLinkInfo->substates = A2DP_SUBSTATE_STREAM_WAIT_MMI_DSP_RESP;
					a2dpLinkInfo->remoteReqPtr = (U8 XDATA_PTR)eventPtr;
					eventPtr = 0;
					A2DP_DelayReportClearPara(linkIndex);
					SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
					SYS_SetTimer(&a2dpLinkInfo->suspendDelayTimer, 3 * HALF_SEC);	//for issue 1504
					MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_SUSPEND_REQ);
					break;
				}
				else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_MMI_DSP_RESP)
				{
					OSMQ_PutFront(&a2dpLinkInfo->OSMQ_AVDTP_Signal_To_A2DP, (U8 XDATA_PTR)eventPtr);
					eventPtr = 0;
					break;
				}
			}
			else if (a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN)
			{
				if(a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_SUSPEND)
				{
					A2DP_CommandComplete(a2dpLinkInfo);
					break;
				}
			}
			errorCode = AVDTP_BAD_STATE;
			break;
		case AVDTP_ABORT:
			//by spec. However, AVDTP_ABORT can be sent or received in IDLE state. In the event that an
			//AVDTP_ABORT is received in IDLE state, ACP or INT shall reply with an AVDTP_ABORT_RSP,
			//no state change is required. As there should be no Transport Channels established no actions have
			//to be taken to release the Transport Channels.
			if( a2dpLinkInfo->mmiProfileInfo.state >= A2DP_CONFIGURED && a2dpLinkInfo->mmiProfileInfo.state <= A2DP_STREAMING &&
				a2dpLinkInfo->localSeid == (((AvdtpSignalConfigType XDATA_PTR)eventPtr)->acpSEID >> 2))
			{
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_ABORTING;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_PASSIVE_GET_ABORT_CMD;
				SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
				MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
			}
			else
			{
				/* dicard erroneous abort command, no response should send */
				OSMEM_Put((U8 XDATA_PTR)eventPtr);
				eventPtr = 0;
			}
			break;
		case AVDTP_SECURITY_CONTROL:
			if(!(a2dpLinkInfo->mmiProfileInfo.state >= A2DP_CONFIGURED && a2dpLinkInfo->mmiProfileInfo.state <= A2DP_STREAMING))
			{
				errorCode = AVDTP_BAD_STATE;
				break;
			}
			if(a2dpLinkInfo->localSeid != (((AvdtpSignalConfigType XDATA_PTR)eventPtr)->acpSEID >> 2))
			{
				errorCode = AVDTP_BAD_ACP_SEID;
				break;
			}
			break;
		case AVDTP_DELAYREPORT:
			if (a2dpLinkInfo->enableDelayReport)
			{
				break; //support delay report
			}
			/* fallthrough */
		default:
			errorCode =  AVDTP_NOT_SUPPORT_COMMAND;
			break;
	}
	
	if(eventPtr)
	{
		//eventPtr is transferred to response
		if (errorCode == AVDTP_NO_ERROR)
		{
			AVDTP_SendAcceptRsp(linkIndex, eventPtr);
			if (a2dpLinkInfo->enableDelayReport && ((eventPtr->signalIdentifier & 0x3F) == AVDTP_SET_CONFIGURATION || (eventPtr->signalIdentifier & 0x3F) == AVDTP_RECONFIGURE))
			{
				A2DP_SendInitDelayReport(linkIndex);
			}
			#ifdef AVDTP_PTS_TP_SIG_SMG_BV_11_C
			if ((eventPtr->signalIdentifier & 0x3F) == AVDTP_SET_CONFIGURATION)
			{
				AVDTP_SendGetConfigurationCmd(linkIndex, a2dpLinkInfo->remoteSeid);
			}
			#endif
			#ifdef AVDTP_PTS_TP_SIG_SMG_ESR05_BV_13_C
			if ((eventPtr->signalIdentifier & 0x3F) == AVDTP_OPEN)
			{
				U8 LDATA cfgData[] = {0x07, 0x06, 0x00, 0x00, 0x11, 0x15, 0x08, 0x80};
				AVDTP_SendReconfigureCmd(linkIndex, a2dpLinkInfo->remoteSeid, sizeof(cfgData), (U8 XDATA_PTR)&cfgData[0]);
			}
			#endif
		}
		else
		{
			AVDTP_SendRejectRsp(linkIndex, eventPtr, errorCode);
		}
	}
}

/////////////////////////////////////////////////////////////////////
// Signal Response Handler
/////////////////////////////////////////////////////////////////////

PRIVATE U8 A2DP_FilterDiscovSeid(A2dpDiscovSeidInfo XDATA_PTR discovSeidInfoPtr, U8 seidDataLen, U8 XDATA_PTR seidDataPtr)
{
	U8 count = 0;

	while(count < A2DP_MAX_SEID_MATCH_NUM && seidDataLen >= sizeof(A2DP_SEID_INFO))
	{
		if( ((A2DP_SEID_INFO XDATA_PTR)seidDataPtr)->inUse == 0 &&
			((A2DP_SEID_INFO XDATA_PTR)seidDataPtr)->mediaType == A2DP_AUDIO &&
			((A2DP_SEID_INFO XDATA_PTR)seidDataPtr)->tsep == discovSeidInfoPtr->tsep )
		{
			discovSeidInfoPtr->seid[count++] = ((A2DP_SEID_INFO XDATA_PTR)seidDataPtr)->seid;
		}
		seidDataPtr += sizeof(A2DP_SEID_INFO);
		seidDataLen -= sizeof(A2DP_SEID_INFO);
	}
	return count;
}

PRIVATE U8 A2DP_MatchDiscovSeid(A2dpDiscovSeidInfo XDATA_PTR discovSeidInfoPtr, AvdtpSignalType XDATA_PTR eventPtr)
{
	U8 XDATA_PTR paraPtr = &eventPtr->para[0];
	U8 caps;
	U8 serviceLength, capabilitiesLength = (U8)(eventPtr->l2capLength - 2);
	U8 LDATA configParam[A2DP_MAX_CAPS_MATCH_SIZE];
	U8 XDATA_PTR configPtr = (U8 XDATA_PTR)&configParam[0];
	U8 LDATA configLen = 0;
	U8 LDATA intSeid = 0;

	caps = 0;

	
	while(capabilitiesLength >= 2)
	{
		switch(paraPtr[0])
		{
			case MEDIA_TRANSPORT:
				caps |= A2DP_CAPS_MEDIA_TRANSPORT;
				configPtr[0] = MEDIA_TRANSPORT;
				configPtr[1] = 0;
				configPtr += 2;
				configLen += 2;
				break;

			case MEDIA_CODEC:
				if(paraPtr[2] == AUDIO_TYPE)
				{
					OSMEM_memcpy_xdata_xdata(configPtr, paraPtr, 4);

					#if SUPPORT_SBC_SNK || SUPPORT_SBC_SRC
					if(paraPtr[3] == SBC_CODEC)
					{
						U8 value, value2;
						
						value2 = 0;
						value = paraPtr[4];

						if(value & (gA2DP_nvram.seidCtl.sbcDefaultSettings.content1 & 0x0F))
							value2 |= (gA2DP_nvram.seidCtl.sbcDefaultSettings.content1 & 0x0F);
						else if(value & A2DP_SBC_CHANNEL_MODE_JOINT_STEREO)
							value2 |= A2DP_SBC_CHANNEL_MODE_JOINT_STEREO;
						else if(value & A2DP_SBC_CHANNEL_MODE_STEREO)
							value2 |= A2DP_SBC_CHANNEL_MODE_STEREO;
						else if(value & A2DP_SBC_CHANNEL_MODE_MONO)
							value2 |= A2DP_SBC_CHANNEL_MODE_MONO;
						else if(value & A2DP_SBC_CHANNEL_MODE_DUAL)
							value2 |= A2DP_SBC_CHANNEL_MODE_DUAL;
						else
							break;

						if(value & (gA2DP_nvram.seidCtl.sbcDefaultSettings.content1 & 0xF0))
							value2 |= (gA2DP_nvram.seidCtl.sbcDefaultSettings.content1 & 0xF0);
						else if(value & A2DP_SBC_SAMPLE_FREQ_48000)
							value2 |= A2DP_SBC_SAMPLE_FREQ_48000;
						else if(value & A2DP_SBC_SAMPLE_FREQ_44100)
							value2 |= A2DP_SBC_SAMPLE_FREQ_44100;
						else if(value & A2DP_SBC_SAMPLE_FREQ_32000)
							value2 |= A2DP_SBC_SAMPLE_FREQ_32000;
						else
							break;

						configPtr[4] = value2;

						value2 = 0;
						value = paraPtr[5];

						if(value & (gA2DP_nvram.seidCtl.sbcDefaultSettings.content2 & 0x03))
							value2 |= (gA2DP_nvram.seidCtl.sbcDefaultSettings.content2 & 0x03);
						else if(value & A2DP_SBC_ALLOCATION_LOUDNESS)
							value2 |= A2DP_SBC_ALLOCATION_LOUDNESS;
						else if(value & A2DP_SBC_ALLOCATION_SNR)
							value2 |= A2DP_SBC_ALLOCATION_SNR;
						else
							break;

						if(value & (gA2DP_nvram.seidCtl.sbcDefaultSettings.content2 & 0x0C))
							value2 |= (gA2DP_nvram.seidCtl.sbcDefaultSettings.content2 & 0x0C);
						else if(value & A2DP_SBC_SUBBAND_NUMBER_8)
							value2 |= A2DP_SBC_SUBBAND_NUMBER_8;
						else if(value & A2DP_SBC_SUBBAND_NUMBER_4)
							value2 |= A2DP_SBC_SUBBAND_NUMBER_4;
						else
							break;

						if(value & (gA2DP_nvram.seidCtl.sbcDefaultSettings.content2 & 0xF0))
							value2 |= (gA2DP_nvram.seidCtl.sbcDefaultSettings.content2 & 0xF0);
						else if(value & A2DP_SBC_BLOCK_LENGTH_16)
							value2 |= A2DP_SBC_BLOCK_LENGTH_16;
						else if(value & A2DP_SBC_BLOCK_LENGTH_12)
							value2 |= A2DP_SBC_BLOCK_LENGTH_12;
						else if(value & A2DP_SBC_BLOCK_LENGTH_8)
							value2 |= A2DP_SBC_BLOCK_LENGTH_8;
						else if(value & A2DP_SBC_BLOCK_LENGTH_4)
							value2 |= A2DP_SBC_BLOCK_LENGTH_4;
						else
							break;

						configPtr[5] = value2;

						if(paraPtr[6] < gA2DP_nvram.seidCtl.sbcDefaultSettings.minBitPool)
							configPtr[6] = gA2DP_nvram.seidCtl.sbcDefaultSettings.minBitPool;
						else
							configPtr[6] = paraPtr[6];

						if(paraPtr[7] > gA2DP_nvram.seidCtl.sbcDefaultSettings.maxBitPool)
							configPtr[7] = gA2DP_nvram.seidCtl.sbcDefaultSettings.maxBitPool;
						else
							configPtr[7] = paraPtr[7];

						configPtr += 8;
						configLen += 8;
						
						#ifdef A2DP_SRC_Profile
						intSeid = SBC_SRC_SEID;
						#else
						intSeid = SBC_SNK_SEID;
						#endif
						break;
					}
					#endif
					#if SUPPORT_AAC_SNK
					if(paraPtr[3] == AAC_CODEC)
					{
						U32 bitrate1, bitrate2;
						
						if(paraPtr[4] & A2DP_AAC_OBJECT_TYPE_MPEG2_AAC_LC)
							configPtr[4] = A2DP_AAC_OBJECT_TYPE_MPEG2_AAC_LC;
						else
							break;

						if(paraPtr[5] & A2DP_AAC_SAMPLE_FREQ_44100)
						{
							configPtr[5] = A2DP_AAC_SAMPLE_FREQ_44100;
							configPtr[6] = 0;
						}
						else if(paraPtr[6] & A2DP_AAC_SAMPLE_FREQ_48000)
						{
							configPtr[5] = 0;
							configPtr[6] = A2DP_AAC_SAMPLE_FREQ_48000;
						}
						else
							break;

						if(paraPtr[6] & A2DP_AAC_CHANNELS_2)
							configPtr[6] |= A2DP_AAC_CHANNELS_2;
						else if(paraPtr[6] & A2DP_AAC_CHANNELS_1)
							configPtr[6] |= A2DP_AAC_CHANNELS_1;
						else
							break;

						bitrate2 = (*(U32 XDATA_PTR)&paraPtr[6]) & 0x007FFFFF;
						bitrate1 = (*(U32 XDATA_PTR)&gA2DP_nvram.seidCtl.aacDefaultSettings.sampleRate2) & 0x007FFFFF;
						if (bitrate2 > bitrate1)
						{
							bitrate2 = bitrate1;
						}
						
						configPtr[7] = (paraPtr[7] & A2DP_AAC_VARIABLE_BIT_RATE) | (U8)(bitrate2 >> 16);
						configPtr[8] = (U8)(bitrate2 >> 8);
						configPtr[9] = (U8)bitrate2;

						configPtr += 10;
						configLen += 10;
						intSeid = AAC_SNK_SEID;
						break;
					}
					#endif
					if(api_profile_func[API_GRP_PF_CB_AVDTP_GET_CAPABILITY_RESP])
					{
						if(((API_PROFILE_AVDTP_GET_CAPABILITY_RESP_FUNC)api_profile_func[API_GRP_PF_CB_AVDTP_GET_CAPABILITY_RESP])(paraPtr, &configPtr, &configLen, &intSeid))
						{
							discovSeidInfoPtr->preferSeid = intSeid;
						}
						if(intSeid)
						{
							break;
						}
					}
				}
				break;

			case CONTENT_PROTECTION:
				if(paraPtr[2] == A2DP_CP_SCMS_T && paraPtr[3] == 0)
				{
					caps |= A2DP_CAPS_CONTENT_PROTECTION;
					OSMEM_memcpy_xdata_xdata(configPtr, paraPtr, 4);
					configPtr += 4;
					configLen += 4;
				}
				break;

			case DELAY_REPORTING:
				if (MMI_A2DP_DELAY_REPORT_FEAT)
				{
					caps |= A2DP_CAPS_DELAY_REPORTING;
					configPtr[0] = DELAY_REPORTING;
					configPtr[1] = 0;
					configPtr += 2;
					configLen += 2;
				}
				break;
		}

		paraPtr += (serviceLength = 2 + paraPtr[1]);
		capabilitiesLength -= serviceLength;
	}
	
	if (intSeid)
	{
		if (discovSeidInfoPtr->preferSeid == intSeid || discovSeidInfoPtr->intSeid == 0)
		{
			discovSeidInfoPtr->acpSeid = discovSeidInfoPtr->seid[discovSeidInfoPtr->index];
			discovSeidInfoPtr->intSeid = intSeid;
			discovSeidInfoPtr->caps = caps;
			discovSeidInfoPtr->configLen = configLen;
			OSMEM_memcpy_xdata_xdata(&discovSeidInfoPtr->configParam[0], (U8 XDATA_PTR)&configParam[0], configLen);
		}
	}
	return intSeid;
}

PRIVATE void A2DP_SetMatchConfiguration(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo, A2dpDiscovSeidInfo XDATA_PTR discovSeidInfoPtr)
{
	U8 XDATA_PTR configPtr = &discovSeidInfoPtr->configParam[0];
	U8 serviceLength, configLen = discovSeidInfoPtr->configLen;
	
	while (configLen >= 2)
	{
		switch (configPtr[0])
		{
			case MEDIA_CODEC:
				switch (discovSeidInfoPtr->intSeid)
				{
					#if SUPPORT_SBC_SNK
					case SBC_SNK_SEID:
					#endif
					#if SUPPORT_SBC_SRC
					case SBC_SRC_SEID:
					#endif
						OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&a2dpLinkInfo->codecSettings.sbc, &configPtr[4], sizeof(SBCCodecSettings));
						break;
					
					#if SUPPORT_AAC_SNK
					case AAC_SNK_SEID:
						OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&a2dpLinkInfo->codecSettings.aac, &configPtr[4], sizeof(AACCodecSettings));
						break;
					#endif
				}
				break;
		}

		configPtr += (serviceLength = 2 + configPtr[1]);
		configLen -= serviceLength;
	}
	
	A2DP_SetCodecPara(a2dpLinkInfo, discovSeidInfoPtr->intSeid);

	a2dpLinkInfo->enableSCMS = (discovSeidInfoPtr->caps & A2DP_CAPS_CONTENT_PROTECTION) ? TRUE : FALSE;
	a2dpLinkInfo->enableDelayReport = (discovSeidInfoPtr->caps & A2DP_CAPS_DELAY_REPORTING) ? TRUE : FALSE;
}

PRIVATE void A2DP_SignalResponseAcceptHandler(U8 linkIndex, AvdtpSignalType XDATA_PTR eventPtr)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	A2dpDiscovSeidInfo XDATA_PTR discovSeidInfoPtr;
	
	switch(eventPtr->signalIdentifier & 0x3F)
	{
		case AVDTP_DISCOVER:
			if(a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED && a2dpLinkInfo->substates == A2DP_SUBSTATE_IDLE_WAIT_DISCOVER_RESP)
			{
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				if(discovSeidInfoPtr = (A2dpDiscovSeidInfo XDATA_PTR)a2dpLinkInfo->discovSeidInfoPtr)
				{
					discovSeidInfoPtr->count = A2DP_FilterDiscovSeid(discovSeidInfoPtr, (U8)(eventPtr->l2capLength - AVDTP_MSH_HEADER_SIZE), &eventPtr->para[0]);
					if(discovSeidInfoPtr->count)
					{
						discovSeidInfoPtr->intSeid = 0;
						discovSeidInfoPtr->index = 0;
						#if defined(AVDTP_PTS_TP_SIG_SMG_BV_25_28_C_BI_35_36_C) || defined(AVDTP_PTS_TP_SIG_SYN_BV_02_C)
						AVDTP_SendGetAllCapabilitiesCmd(linkIndex, discovSeidInfoPtr->seid[discovSeidInfoPtr->index]);
						#else
						AVDTP_SendGetCapabilitiesCmd(linkIndex, discovSeidInfoPtr->seid[discovSeidInfoPtr->index]);
						#endif
						a2dpLinkInfo->substates = A2DP_SUBSTATE_IDLE_WAIT_GET_CAPABILITY_RESP;
						SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
						return;
					}
				}
				A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
			}
			break;
			
		case AVDTP_GET_CAPABILITIES:
		case AVDTP_GET_ALL_CAPABILITIES:
			if(a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED && a2dpLinkInfo->substates == A2DP_SUBSTATE_IDLE_WAIT_GET_CAPABILITY_RESP)
			{
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				if(discovSeidInfoPtr = (A2dpDiscovSeidInfo XDATA_PTR)a2dpLinkInfo->discovSeidInfoPtr)
				{
					if(discovSeidInfoPtr->preferSeid == A2DP_MatchDiscovSeid(discovSeidInfoPtr, eventPtr))
					{
						label_send_set_config_cmd:
						A2DP_SetMatchConfiguration(a2dpLinkInfo, discovSeidInfoPtr);
						AVDTP_SendSetConfigurationCmd(linkIndex,
														a2dpLinkInfo->remoteSeid = discovSeidInfoPtr->acpSeid,
														a2dpLinkInfo->localSeid = discovSeidInfoPtr->intSeid,
														discovSeidInfoPtr->configLen, &discovSeidInfoPtr->configParam[0]);
						SYS_MemoryRelease(&a2dpLinkInfo->discovSeidInfoPtr);
						a2dpLinkInfo->substates = A2DP_SUBSTATE_IDLE_WAIT_SET_CONFIG_RESP;
						SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
						return;
					}
					else
					{
						discovSeidInfoPtr->index++;
						if(discovSeidInfoPtr->index < discovSeidInfoPtr->count)
						{
							AVDTP_SendGetCapabilitiesCmd(linkIndex, discovSeidInfoPtr->seid[discovSeidInfoPtr->index]);
							SYS_SetTimer(&a2dpLinkInfo->timer, GAVDP_TIMER_100);
							return;
						}
						else if(discovSeidInfoPtr->intSeid)
						{
							goto label_send_set_config_cmd;
						}
					}
				}
				A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
			}
			break;
			
		case AVDTP_SET_CONFIGURATION:
			if(a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED && a2dpLinkInfo->substates == A2DP_SUBSTATE_IDLE_WAIT_SET_CONFIG_RESP)
			{
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_CONFIGURED;
				
				if(a2dpLinkInfo->enableDelayReport)
				{
					A2DP_SendInitDelayReport(linkIndex);
				}

				#ifdef AVDTP_PTS_TP_SIG_SEC_BV_01_C
				{
					U8 LDATA cpData[] = {0x12, 0x34};
					AVDTP_SendSecurityControlCmd(linkIndex, a2dpLinkInfo->remoteSeid, sizeof(cpData), (U8 XDATA_PTR)&cpData[0]);
				}
				#endif
				#ifdef AVDTP_PTS_TP_SIG_SMG_BV_23_C
				{
					AVDTP_SendAbortCmd(linkIndex, a2dpLinkInfo->remoteSeid);
				}
				#endif
				
				a2dpLinkInfo->substates = A2DP_SUBSTATE_CONFIG_WAIT_OPEN_RESP;
				AVDTP_SendOpenCmd(linkIndex, a2dpLinkInfo->remoteSeid);
				return;
			}		
			break;
			
		case AVDTP_OPEN:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_CONFIGURED && a2dpLinkInfo->substates == A2DP_SUBSTATE_CONFIG_WAIT_OPEN_RESP)
			{
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_OPEN;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_OPEN_MEDIA_CHANNEL;
				if(AVDTP_ConnectMediaChannel(linkIndex) != INVALID_L2CAP_CHANNEL)
				{
					a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_MEDIA_CHANNEL_READY;
				}
				MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_CONTROL);				
			}
			break;
			
		case AVDTP_START:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE_AFTER_ACTIVE_MEDIA_CHANNEL)
			{
				a2dpLinkInfo->substates = A2DP_SUBSTATE_OPEN_WAIT_SUSPEND;
				SYS_SetTimer(&a2dpLinkInfo->timer, HALF_SEC);
			}
			else if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE)
			{
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;
				#ifdef A2DP_SRC_Profile
					MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_SOURCE_START_EVT);
				#else
					MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_START_RESP_FOR_INT);
				#endif
				MMI_SniffDisable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
			}
			else if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_START_RESP_FROM_FOLLOWER)
			{
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_STREAMING;
				a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;
				A2DP_SetRelayStateReady(TRUE);
			}			
			break;
		
		case AVDTP_SUSPEND:
			if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN && a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_SUSPEND_RESPONSE)
			{
				A2DP_CommandComplete(a2dpLinkInfo);
			}
			else if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING && a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_SUSPEND_RESP)
			{
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				a2dpLinkInfo->mmiProfileInfo.state = A2DP_OPEN;
				A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
				A2DP_DelayReportClearPara(linkIndex);
				A2DP_CommandComplete(a2dpLinkInfo);
				MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_SUSPEND_EVT);
				MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_MUSIC);
				#if defined(AVDTP_PTS_TP_SIG_SMG_BV_13_C) || defined(GAVDP_PTS_TP_APP_TRC_BV_01_C)
				{
					U8 LDATA cfgData[] = {0x07, 0x06, 0x00, 0x00, 0x11, 0x15, 0x08, 0x80};
					AVDTP_SendReconfigureCmd(linkIndex, a2dpLinkInfo->remoteSeid, sizeof(cfgData), (U8 XDATA_PTR)&cfgData[0]);
				}
				#endif
			}
			break;
	}
}

/////////////////////////////////////////////////////////////////////
// Signal State Handler
/////////////////////////////////////////////////////////////////////

PRIVATE U8 A2DP_WaitRemoteMsg(U8 linkIndex, AvdtpSignalType XDATA_PTR eventPtr, U8 opCode)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	U8 rerturnValue = 0;

	if(eventPtr && ((eventPtr->signalIdentifier & 0x3F) == opCode))
	{
		switch(eventPtr->signalHeader.f.messageType)
		{
			case AVDTP_RESPONSE_ACCEPT_MSG:
				rerturnValue = AVDTP_GET_UNACCEPT_RESP;
				switch(opCode)
				{
					case AVDTP_GET_CONFIGURATION:
						if(eventPtr->l2capLength == 3)
						{
							rerturnValue = AVDTP_GET_UNACCEPT_RESP;
						}
						break;

					default:
						if(eventPtr->l2capLength > 2)
						{
							rerturnValue = AVDTP_GET_UNACCEPT_RESP;
						}
						else
						{
							rerturnValue = AVDTP_GET_ACCEPT_RESP;
						}
						break;
				}
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				break;

			case AVDTP_RESPONSE_REJECT_MSG:
			case AVDTP_GENERAL_REJECT_MSG:
				rerturnValue = AVDTP_GET_UNACCEPT_RESP;
				SYS_ReleaseTimer(&a2dpLinkInfo->timer);
				break;
		}
	}
	return rerturnValue;
}

PRIVATE void A2DP_StateStreamingHandler(U8 linkIndex, AvdtpSignalType XDATA_PTR eventPtr)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	UNUSED(eventPtr);
	
	switch(a2dpLinkInfo->substates)
	{
		case A2DP_SUBSTATE_STREAM_GET_MMI_DSP_RESP_WAIT_AIR_READY:
			if(A2DP_AirSideReadyToReceiveMediaStream())
			{
				if(MMI_A2DP_SEND_START_RESP_AFTER_DSP_IS_ENABLED_FEAT && (linkIndex != MMI_DRV_GetA2DPEnabledLinkIndex()))
				{
					a2dpLinkInfo->substates = A2DP_SUBSTATE_STREAM_WAIT_DSP_ENABLE_FOR_START_RESP;
				}
				else
				{
					A2DP_SendAcceptStartResponse(linkIndex);
				}
			}
			break;
		case A2DP_SUBSTATE_STREAM_WAIT_DSP_ENABLE_FOR_START_RESP:
			if(linkIndex == MMI_DRV_GetA2DPEnabledLinkIndex())
			{
				A2DP_SendAcceptStartResponse(linkIndex);
			}
			break;
	}
}

PRIVATE void A2DP_StateClosingHandler(U8 linkIndex, AvdtpSignalType XDATA_PTR eventPtr)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	U8 waitMsgValue;

	switch(a2dpLinkInfo->substates)
	{
		case A2DP_SUBSTATE_CLOSE_SEND_CLOSE_CMD: /// Active close the a2dp
			a2dpLinkInfo->substates = A2DP_SUBSTATE_CLOSE_WAIT_CLOSE_RESP;
			A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
			AVDTP_SendCloseCmd(linkIndex, a2dpLinkInfo->remoteSeid);
			break;

		case A2DP_SUBSTATE_CLOSE_WAIT_CLOSE_RESP:
			waitMsgValue =A2DP_WaitRemoteMsg(linkIndex,eventPtr, AVDTP_CLOSE);
			if(waitMsgValue)
			{
				a2dpLinkInfo->substates = A2DP_SUBSTATE_CLOSE_CLOSE_MEDIA_CHANNEL;
				MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_CLOSED_EVT);
			}
			break;

		case A2DP_SUBSTATE_CLOSE_CLOSE_MEDIA_CHANNEL:
			if(a2dpLinkInfo->mediaChannelConnected && CMD_WAITING == AVDTP_DisconnectMediaChannel(linkIndex))
			{
				a2dpLinkInfo->substates = A2DP_SUBSTATE_CLOSE_WAIT_CLOSE_MEDIA_CHANNEL;
			}
			else
			{
				a2dpLinkInfo->mediaChannelConnected = FALSE;
				goto CLOSE_SIGNAL_CHANNEL;
			}
			break;

		case A2DP_SUBSTATE_CLOSE_WAIT_CLOSE_MEDIA_CHANNEL:
			if(!a2dpLinkInfo->mediaChannelConnected)
				goto CLOSE_SIGNAL_CHANNEL;
			break;

		case A2DP_SUBSTATE_CLOSE_CLOSE_CHANNEL: // Active close the L2capChannel
			CLOSE_SIGNAL_CHANNEL:
			if(CMD_WAITING == AVDTP_DisconnectSignalChannel(linkIndex))
			{
				a2dpLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTING; 
			}
			else
			{
				MMI_A2DP_ClearCtlData(linkIndex);
				MMI_UnloadProfile(linkIndex, &a2dpLinkInfo->mmiProfileInfo);
				A2DP_CloseHandler(linkIndex);			
			}
			break;
	}
}

PRIVATE void A2DP_StateAbortingHandler(U8 linkIndex, AvdtpSignalType XDATA_PTR eventPtr)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	//When the INT and ACP have received appropriate responses to the
	//Abort Command they transition to the <ABORTING> state until Transport
	//Channel release is complete

	switch(a2dpLinkInfo->substates)
	{
		case A2DP_SUBSTATE_ABORT_PASSIVE_GET_ABORT_CMD: // Passive get Abort Cmd
			a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_WAIT_PASSIVE_RELEASE_MEDIA_CHANNEL;
			SYS_MemoryRelease(&a2dpLinkInfo->remoteReqPtr);
			A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
			break;
		case A2DP_SUBSTATE_ABORT_WAIT_PASSIVE_RELEASE_MEDIA_CHANNEL: //Passive
		case A2DP_SUBSTATE_ABORT_WAIT_ACTIVE_RELEASE_MEDIA_CHANNEL: //Active
			if(!a2dpLinkInfo->mediaChannelConnected)
			{
				a2dpLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
				A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
				if (a2dpLinkInfo->airCtl.packetRole == PACKET_ROLE_RELAYER)
				//Abort is not Close, and we try to recover Media channel for relayer/follower pair 
				{
					MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_RECONNECT_AFTER_ABORT_EVT);
				}
			}
			break;
		case A2DP_SUBSTATE_ABORT_SEND_RELEASE_MEDIA_CHANNEL_CMD:// Active
			if(CMD_WAITING == AVDTP_DisconnectMediaChannel(linkIndex))
			{
				a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_WAIT_ACTIVE_RELEASE_MEDIA_CHANNEL;
			}
			else
			{
				a2dpLinkInfo->mediaChannelConnected = FALSE;
				a2dpLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
				A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
			}
			break;

		case A2DP_SUBSTATE_ABORT_SEND_ABORT_CMD:  // Active send abort cmd
			a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_WAIT_ABORT_RESP;
			SYS_MemoryRelease(&a2dpLinkInfo->remoteReqPtr);
			AVDTP_SendAbortCmd(linkIndex, a2dpLinkInfo->remoteSeid);
			break;
		case A2DP_SUBSTATE_ABORT_WAIT_ABORT_RESP:// Active send abort cmd
			if(A2DP_WaitRemoteMsg(linkIndex,eventPtr, AVDTP_ABORT))
			{
				a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_DECIDE_RELEASE_MEDIA_CHANNEL;
			}
			break;
		case A2DP_SUBSTATE_ABORT_DECIDE_RELEASE_MEDIA_CHANNEL:
			if(a2dpLinkInfo->mediaChannelConnected)
				a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_SEND_RELEASE_MEDIA_CHANNEL_CMD;
			else
				a2dpLinkInfo->substates = A2DP_SUBSTATE_ABORT_WAIT_ACTIVE_RELEASE_MEDIA_CHANNEL;
			break;
	}
}

PUBLIC void A2DP_SignalHandler(U8 linkIndex)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	AvdtpSignalType XDATA_PTR eventPtr = (AvdtpSignalType XDATA_PTR)OSMQ_Get(&a2dpLinkInfo->OSMQ_AVDTP_Signal_To_A2DP);

	if(eventPtr)
	{
		switch(eventPtr->signalHeader.f.messageType)
		{
			case AVDTP_COMMAND_MSG:
				A2DP_SignalCommandHandler(linkIndex, eventPtr);
				eventPtr = 0;			
				break;
		
			case AVDTP_RESPONSE_ACCEPT_MSG:
				A2DP_SignalResponseAcceptHandler(linkIndex, eventPtr);
				break;
				
			case AVDTP_RESPONSE_REJECT_MSG:
			case AVDTP_GENERAL_REJECT_MSG:
				if(a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
				{
					if(a2dpLinkInfo->substates == A2DP_SUBSTATE_IDLE_WAIT_DISCOVER_RESP)
					{
						if((eventPtr->signalIdentifier & 0x3F) == AVDTP_DISCOVER)
						{
							A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
						}
					}
					else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_IDLE_WAIT_GET_CAPABILITY_RESP)
					{
						if((eventPtr->signalIdentifier & 0x3F) == AVDTP_GET_CAPABILITIES || (eventPtr->signalIdentifier & 0x3F) == AVDTP_GET_ALL_CAPABILITIES)
						{
							A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
						}					
					}
					else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_IDLE_WAIT_SET_CONFIG_RESP)
					{
						if((eventPtr->signalIdentifier & 0x3F) == AVDTP_SET_CONFIGURATION)
						{
							A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
						}
					}
				}
				else if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_CONFIGURED)
				{
					if(a2dpLinkInfo->substates == A2DP_SUBSTATE_CONFIG_WAIT_OPEN_RESP)
					{
						if((eventPtr->signalIdentifier & 0x3F) == AVDTP_OPEN)
						{
							A2DP_ClearSubStateAndTimer(a2dpLinkInfo);
							MMI_SniffEnable(linkIndex, MMI_SNIFF_A2DP_CONTROL);
						}
					}
				}
				else if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_OPEN)
				{
					if(a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE_AFTER_ACTIVE_MEDIA_CHANNEL || a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE)
					{
						if((eventPtr->signalIdentifier & 0x3F) == AVDTP_START)
						{
							A2DP_CommandComplete(a2dpLinkInfo);
						}
					}
					else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_SUSPEND_RESPONSE)
					{
						if((eventPtr->signalIdentifier & 0x3F) == AVDTP_SUSPEND)
						{
							A2DP_CommandComplete(a2dpLinkInfo);
						}					
					}
					else if(a2dpLinkInfo->substates == A2DP_SUBSTATE_OPEN_WAIT_START_RESP_FROM_FOLLOWER)
					{
						if((eventPtr->signalIdentifier & 0x3F) == AVDTP_START)
						{
							A2DP_CommandComplete(a2dpLinkInfo);
							A2DP_SetRelayStateReady(FALSE);
						}
					}
				}
				else if(a2dpLinkInfo->mmiProfileInfo.state == A2DP_STREAMING && a2dpLinkInfo->substates == A2DP_SUBSTATE_STREAM_WAIT_SUSPEND_RESP)
				{
					if((eventPtr->signalIdentifier & 0x3F) == AVDTP_SUSPEND)
					{
						A2DP_SetMediaDataPlayByLink(a2dpLinkInfo, FALSE);
						A2DP_CommandComplete(a2dpLinkInfo);
						MMI_A2DP_EventHandler(linkIndex, A2DP_CHANNEL_SUSPEND_FAIL_EVT);
					}
				}
				break;
		}
	}

	switch(a2dpLinkInfo->mmiProfileInfo.state)
	{
		case PROFILE_DISCONNECTED:
		case PROFILE_CONNECTING:
		case PROFILE_DISCONNECTING:
		case PROFILE_CONNECTED:
		case A2DP_CONFIGURED:
		case A2DP_OPEN:
			break;
		case A2DP_STREAMING:
			A2DP_StateStreamingHandler(linkIndex, eventPtr);
			break;
		case A2DP_CLOSING:
			A2DP_StateClosingHandler(linkIndex, eventPtr);
			break;
		case A2DP_ABORTING:
			A2DP_StateAbortingHandler(linkIndex, eventPtr);
			break;
	}

	if(eventPtr)
	{
		OSMEM_Put((U8 XDATA_PTR)eventPtr);
	}
}

PUBLIC void A2DP_Signal_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);
	
	switch(evtCode)
	{
		case PROFILE_LINK_EVENT_CONN_REQUEST:
			evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT;
			if (a2dpLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
			{
				if(MMI_IsLinkActiveDisconnecting(linkIndex))
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT_ACTIVE_DISC;
				}
				else
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_ACCEPT;
					evtParams->connRequest.profileInfo = &a2dpLinkInfo->mmiProfileInfo;
				}
			}
			break;
			
		case PROFILE_LINK_EVENT_CONN_COMPLETE:
			if (a2dpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTING)
			{
				if (evtParams->connComplete.status == NO_ERROR)
				{
					a2dpLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
					a2dpLinkInfo->substates = A2DP_SUBSTATE_BEGINNING_STATE;
					a2dpLinkInfo->isPassive = evtParams->connComplete.isPassive;
					#ifdef A2DP_SRC_Profile
					MMI_A2DP_ConnectMediaChannelSource(linkIndex);
					#else
					if (a2dpLinkInfo->isPassive)
					{
						if(gMMI_nvram.timerCtl.checkA2DPMediaChannelEstTime)
						{
							SYS_SetTimer(&a2dpLinkInfo->waitForMediaChannelOpenTimer,(U32) gMMI_nvram.timerCtl.checkA2DPMediaChannelEstTime * 320L);
						}
					}
					else
					{
						if(gMMI_nvram.checkA2DPMediaChannelEstTimeActiveConn)
						{
							SYS_SetTimer(&a2dpLinkInfo->waitForMediaChannelOpenTimer,(U32) gMMI_nvram.checkA2DPMediaChannelEstTimeActiveConn * 320L);
							#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
							MMI_SequentialActiveProfileConnection(linkIndex);
							#endif
						}
					}
					#endif
					#ifdef AIR_MODULE
					MMI_AIR_A2DPSignalChannelReadyNotify(linkIndex);
					#endif					
				}
				else
				{
					if(evtParams->connComplete.status == SECURITY_BLOCK)
					{
						MMI_SetReconnectProfile(linkIndex, a2dpLinkInfo->mmiProfileInfo.profileDesc->profileId, ONE_SEC);
					}
					evtCode = PROFILE_LINK_EVENT_DISC_COMPLETE;
					goto A2DP_DISC_CPL;
				}
			}		
			break;
			
		case PROFILE_LINK_EVENT_DISC_COMPLETE:
		case PROFILE_LINK_EVENT_DISC_LINKLOSS:
			if (a2dpLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
			{
				A2DP_DISC_CPL:
				MMI_A2DP_ClearCtlData(linkIndex);
				MMI_UnloadProfile(linkIndex, &a2dpLinkInfo->mmiProfileInfo);
				MMI_PROTOCOL_L2CAPDisconnected(linkIndex, evtCode, FALSE);
				A2DP_CloseHandler(linkIndex);
			}	
			break;
	}
}

PUBLIC void A2DP_Signal_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);

	OSMQ_Put(&a2dpLinkInfo->OSMQ_AVDTP_Signal_To_A2DP, (U8 XDATA_PTR)dataPtr);
}

#endif //A2DP_Profile
