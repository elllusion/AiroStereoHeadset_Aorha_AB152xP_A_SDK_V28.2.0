//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#ifndef _AVDTP_C_
#define _AVDTP_C_

#include "AVDTP_interface.h"
#include "AVDTP.h"
#include "..\HC\HC_Interface.h"
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_SCN.h"
#include "..\MMI\MMI_A2DP.h"
#include "..\A2DP\A2DP.h"
#include "..\A2DP\A2DP_interface.h"
#include "..\LC\LC_interface.h"

#include "align_flash.h"


#ifdef A2DP_Profile

static AVDTP_CTL_TYPE XDATA gAVDTP_ctl;


PRIVATE AVDTP_LINK_INFO XDATA_PTR AVDTP_GetLinkInfo(U8 linkIndex)
{
	return &gAVDTP_ctl.linkInfo[linkIndex];
}

PRIVATE AvdtpSignalType XDATA_PTR AVDTP_GetSignalCmdBuffer(U8 linkIndex, U8 opCode, U8 size)
{
	AvdtpSignalType XDATA_PTR cmdPtr;

	cmdPtr = (AvdtpSignalType XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	cmdPtr->offset = AVDTP_TX_OFFSET;
	cmdPtr->l2capLength = size;

	cmdPtr->signalHeader.f.messageType = AVDTP_COMMAND_MSG;
	cmdPtr->signalHeader.f.packetType = AVDTP_SINGLE_PACKET;
	cmdPtr->signalHeader.f.transactionLabel = (AVDTP_GetLinkInfo(linkIndex)->misc.bits.transactionLabel++) & 0x0F;
	cmdPtr->signalIdentifier = opCode;
	return cmdPtr;
}

PUBLIC BOOL AVDTP_IsAacAllowedInLink(U8 linkIndex)
{
	L2CAP_CHANNEL_HDL l2capChannelHdl;
	l2capChannelHdl = AVDTP_GetLinkInfo(linkIndex)->l2capChannelHdl[AVDTPSignalChannelType];
	if (MMI_A2DP_BYPASS_IPHONE_CHECK_FOR_AAC_FUNCTION_FEAT || (l2capChannelHdl->DCID & 0xFF00))
	//Apple iOS
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

PRIVATE void AVDTP_SendSignalPacket(U8 linkIndex, AvdtpSignalType XDATA_PTR pktPtr)
{
	L2CAP_CHANNEL_HDL l2capChannelHdl;

	pktPtr->l2capLength = ENDIAN_TRANSFORM_U16(pktPtr->l2capLength);

	LightDbgPrintWithConnectionHandle (
		HCI_VOPCODE_VOGF(HCI_VENDOR_OGF_UART_AVDTP_DBGPRINT,HCI_VEVT_OCF_AVDTP_DBGPRINT_TX_SIGNALLING_PACKET),
	 	MMI_GetConnHandleByLinkIndex(linkIndex),
		(U8 XDATA_PTR)&pktPtr->l2capLength);

	if(pktPtr->signalHeader.f.messageType == AVDTP_COMMAND_MSG)
	{
		MMI_SniffDisable(linkIndex, MMI_SNIFF_AVDTP_SIGNAL);
	}
	else
	{
		MMI_SniffEnable(linkIndex, MMI_SNIFF_AVDTP_SIGNAL);
	}

	l2capChannelHdl = AVDTP_GetLinkInfo(linkIndex)->l2capChannelHdl[AVDTPSignalChannelType];
	UpperLayer_SendL2capChannelData(l2capChannelHdl, (U8 XDATA_PTR)pktPtr);
}

PUBLIC void AVDTP_SendSingleSeidCmd(U8 linkIndex, U8 acpSeid, U8 cmd)
{
	AvdtpSignalType XDATA_PTR cmdPtr = AVDTP_GetSignalCmdBuffer(linkIndex, cmd, 3);
	cmdPtr->para[0] = acpSeid << 2;
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PUBLIC void AVDTP_SendDiscoverCmd(U8 linkIndex)
{
	AVDTP_SendSignalPacket(linkIndex, AVDTP_GetSignalCmdBuffer(linkIndex, AVDTP_DISCOVER, 2));
}

PUBLIC void AVDTP_SendGetCapabilitiesCmd(U8 linkIndex, U8 acpSeid)
{
	AVDTP_SendSingleSeidCmd(linkIndex, acpSeid, AVDTP_GET_CAPABILITIES);
}

PUBLIC void AVDTP_SendGetAllCapabilitiesCmd(U8 linkIndex, U8 acpSeid)
{
	AVDTP_SendSingleSeidCmd(linkIndex, acpSeid, AVDTP_GET_ALL_CAPABILITIES);
}

PUBLIC void AVDTP_SendSetConfigurationCmd(U8 linkIndex, U8 acpSeid, U8 intSeid, U8 configLen, U8 XDATA_PTR configParam)
{
	AvdtpSignalType XDATA_PTR cmdPtr = AVDTP_GetSignalCmdBuffer(linkIndex, AVDTP_SET_CONFIGURATION, (U8)(4 + configLen));
	cmdPtr->para[0] = acpSeid << 2;
	cmdPtr->para[1] = intSeid << 2;
	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&cmdPtr->para[2], configParam, configLen);
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PUBLIC void AVDTP_SendGetConfigurationCmd(U8 linkIndex, U8 acpSeid)
{
	AvdtpSignalType XDATA_PTR cmdPtr = AVDTP_GetSignalCmdBuffer(linkIndex, AVDTP_GET_CONFIGURATION, 3);
	cmdPtr->para[0] = acpSeid << 2;
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PUBLIC void AVDTP_SendReconfigureCmd(U8 linkIndex, U8 acpSeid, U8 configLen, U8 XDATA_PTR configParam)
{
	AvdtpSignalType XDATA_PTR cmdPtr = AVDTP_GetSignalCmdBuffer(linkIndex, AVDTP_RECONFIGURE, (U8)(3 + configLen));
	cmdPtr->para[0] = acpSeid << 2;
	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&cmdPtr->para[1], configParam, configLen);
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PUBLIC void AVDTP_SendOpenCmd(U8 linkIndex, U8 acpSeid)
{
	AVDTP_SendSingleSeidCmd(linkIndex, acpSeid, AVDTP_OPEN);
}

PUBLIC void AVDTP_SendCloseCmd(U8 linkIndex, U8 acpSeid)
{
	AVDTP_SendSingleSeidCmd(linkIndex, acpSeid, AVDTP_CLOSE);
}

PUBLIC void AVDTP_SendStartCmd(U8 linkIndex, U8 acpSeid)
{
	AVDTP_SendSingleSeidCmd(linkIndex, acpSeid, AVDTP_START);
}

PUBLIC void AVDTP_SendSuspendCmd(U8 linkIndex, U8 acpSeid)
{
	AVDTP_SendSingleSeidCmd(linkIndex, acpSeid, AVDTP_SUSPEND);
}

PUBLIC void AVDTP_SendAbortCmd(U8 linkIndex, U8 acpSeid)
{
	AVDTP_SendSingleSeidCmd(linkIndex, acpSeid, AVDTP_ABORT);
}

PUBLIC void AVDTP_SendDelayReportCmd(U8 linkIndex, U8 acpSeid, U16 delay)
{
	AvdtpSignalType XDATA_PTR cmdPtr = AVDTP_GetSignalCmdBuffer(linkIndex, AVDTP_DELAYREPORT, 5);
	cmdPtr->para[0] = acpSeid << 2;
	*(U16 XDATA_PTR)&cmdPtr->para[1] = delay;
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PUBLIC void AVDTP_SendSecurityControlCmd(U8 linkIndex, U8 acpSeid, U8 cpDataLen, U8 XDATA_PTR cpDataPtr)
{
	AvdtpSignalType XDATA_PTR cmdPtr = AVDTP_GetSignalCmdBuffer(linkIndex, AVDTP_SECURITY_CONTROL, (U8)(3 + cpDataLen));
	cmdPtr->para[0] = acpSeid << 2;
	if (cpDataLen)
	{
		OSMEM_memcpy_xdata_xdata(&cmdPtr->para[1], cpDataPtr, cpDataLen);
	}
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PRIVATE void AVDTP_SetAcceptRspHeader(AvdtpSignalType XDATA_PTR cmdPtr, U8 size)
{
	cmdPtr->offset = AVDTP_TX_OFFSET;
	cmdPtr->l2capLength = size;

	cmdPtr->signalHeader.f.messageType = AVDTP_RESPONSE_ACCEPT_MSG;
	cmdPtr->signalHeader.f.packetType = AVDTP_SINGLE_PACKET;
}

#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];

PRIVATE void AVDTP_SendDiscoverRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr)
{
	AcpStreamEndPointIdCtl XDATA_PTR seidPtr;
	U8 i, sep_num;

	seidPtr = (AcpStreamEndPointIdCtl XDATA_PTR)&cmdPtr->para[0];
	for (sep_num = 0, i = 0; i < gA2DP_nvram.seidCtl.a2dp_sep_num; i++)
	{
		if (MMI_A2DP_IsHighQualityCodecDisbaled() || !AVDTP_IsAacAllowedInLink(linkIndex))
		{
			if (gA2DP_nvram.seidCtl.asepidCtl[i].firstAcpSEID == AAC_SNK_SEID)
				continue;	//skip
		}

		A2DP_ReadNvramDataChunk((U8 XDATA_PTR)seidPtr, &gA2DP_nvram.seidCtl.asepidCtl[i], sizeof(AcpStreamEndPointIdCtl));
		if ( A2DP_CheckSeidInUse(linkIndex, seidPtr->firstAcpSEID) )
		{
			seidPtr->inUse = 1;	//inUse bit
		}
		seidPtr++;
		sep_num++;
	}

	if(api_profile_func[API_GRP_PF_CB_AVDTP_DISCOVER_CMD])
	{
		sep_num += ((API_PROFILE_AVDTP_DISCOVER_CMD_FUNC)api_profile_func[API_GRP_PF_CB_AVDTP_DISCOVER_CMD])(linkIndex, (U8 XDATA_PTR)seidPtr);
	}

	AVDTP_SetAcceptRspHeader(cmdPtr, (U8)(sizeof(AcpStreamEndPointIdCtl) * sep_num + 2));
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PRIVATE void AVDTP_SendGetCapabilitiesRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr)
{
	U8 acpSeid = cmdPtr->para[0] >> 2;
	U8 size = 2;

	AVDTP_SetAcceptRspHeader(cmdPtr, 0/*size was calculated later*/);

	switch (acpSeid)
	{
		#if SUPPORT_SBC_SNK
		case SBC_SNK_SEID:
		#endif
		#if SUPPORT_SBC_SRC
		case SBC_SRC_SEID:
		#endif
			A2DP_ReadNvramDataChunk((U8 XDATA_PTR)&cmdPtr->para[0], &gA2DP_nvram.seidCtl.sbcServiceCapabilities, sizeof(SBCServiceCapabilities));
			size += sizeof(SBCServiceCapabilities);
			break;

		#if SUPPORT_AAC_SNK
		case AAC_SNK_SEID:
			A2DP_ReadNvramDataChunk((U8 XDATA_PTR)&cmdPtr->para[0], &gA2DP_nvram.seidCtl.aacServiceCapabilities, sizeof(AACServiceCapabilities));
			size += sizeof(AACServiceCapabilities);
			break;
		#endif

		default:
			if(api_profile_func[API_GRP_PF_CB_AVDTP_GET_CAPABILITY_CMD])
			{
				size += ((API_PROFILE_AVDTP_GET_CAPABILITY_CMD_FUNC)api_profile_func[API_GRP_PF_CB_AVDTP_GET_CAPABILITY_CMD])(acpSeid, (U8 XDATA_PTR)&cmdPtr->para[0]);
			}
			break;
	}

	if (!(MMI_A2DP_DELAY_REPORT_FEAT && (cmdPtr->signalIdentifier & 0x3F) == AVDTP_GET_ALL_CAPABILITIES))
	{
		size -= 2; //eliminating delay report
	}
	cmdPtr->l2capLength = size;

	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PRIVATE void AVDTP_SendSimpleAcceptRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr)
{
	AVDTP_SetAcceptRspHeader(cmdPtr, 2);
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PRIVATE void AVDTP_SendGetConfigurationRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr)
{
	AVDTP_SetAcceptRspHeader(cmdPtr, (U8)(cmdPtr->l2capLength + 2));
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PUBLIC void AVDTP_SendSecurityControlRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr)
{
	U8 content_size = cmdPtr->l2capLength - 3; /*1byte(signal header) + 1byte(signal identifier) + 1bytes(acpSeid)*/

	AVDTP_SetAcceptRspHeader(cmdPtr, (U8)(content_size + 2));
	if (content_size)
	{
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&cmdPtr->para[0], (U8 XDATA_PTR)&cmdPtr->para[1], content_size);
	}
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PUBLIC void AVDTP_SendAcceptRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr)
{
	switch (cmdPtr->signalIdentifier & 0x3F)
	{
		case AVDTP_DISCOVER:
			AVDTP_SendDiscoverRsp(linkIndex, cmdPtr);
			break;
		case AVDTP_GET_CAPABILITIES:
		case AVDTP_GET_ALL_CAPABILITIES:
			AVDTP_SendGetCapabilitiesRsp(linkIndex, cmdPtr);
			break;
		case AVDTP_GET_CONFIGURATION:
			AVDTP_SendGetConfigurationRsp(linkIndex, cmdPtr);
			break;
		case AVDTP_SECURITY_CONTROL:
			AVDTP_SendSecurityControlRsp(linkIndex, cmdPtr);
			break;
		case AVDTP_SET_CONFIGURATION:
		case AVDTP_RECONFIGURE:
		case AVDTP_OPEN:
		case AVDTP_START:
		case AVDTP_CLOSE:
		case AVDTP_SUSPEND:
		case AVDTP_ABORT:
		case AVDTP_DELAYREPORT:
			AVDTP_SendSimpleAcceptRsp(linkIndex, cmdPtr);
			break;
	}
}

PUBLIC void AVDTP_SendRejectRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr, U8 errorCode)
{
	cmdPtr->offset = AVDTP_TX_OFFSET;

	cmdPtr->signalHeader.f.messageType = AVDTP_RESPONSE_REJECT_MSG;
	cmdPtr->signalHeader.f.packetType = AVDTP_SINGLE_PACKET;

	if (errorCode == AVDTP_NOT_SUPPORT_COMMAND)
		goto label_send_general_reject;

	switch (cmdPtr->signalIdentifier & 0x3F)
	{
		case AVDTP_DISCOVER:
		case AVDTP_GET_CAPABILITIES:
		case AVDTP_GET_CONFIGURATION:
		case AVDTP_OPEN:
		case AVDTP_CLOSE:
		case AVDTP_SECURITY_CONTROL:
		case AVDTP_GET_ALL_CAPABILITIES:
			cmdPtr->para[0] = errorCode;
			cmdPtr->l2capLength = 3;
			break;

		case AVDTP_START:
		case AVDTP_SUSPEND:
			cmdPtr->para[1] = errorCode;
			cmdPtr->l2capLength = 4;
			break;

		case AVDTP_SET_CONFIGURATION:
		case AVDTP_RECONFIGURE:
			cmdPtr->para[0] = 0;
			cmdPtr->para[1] = errorCode;
			cmdPtr->l2capLength = 4;
			break;

		default:
			label_send_general_reject:
			cmdPtr->signalHeader.f.messageType = AVDTP_GENERAL_REJECT_MSG;
			cmdPtr->l2capLength = 2;
			break;
	}
	AVDTP_SendSignalPacket(linkIndex, cmdPtr);
}

PRIVATE U8 AVDTP_RemoveChannelHdl(AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo, L2CAP_CHANNEL_HDL l2capChannelHdl)
{
	if(l2capChannelHdl == avdtpLinkInfo->l2capChannelHdl[AVDTPSignalChannelType])
	{
		avdtpLinkInfo->l2capChannelHdl[AVDTPSignalChannelType] = INVALID_L2CAP_CHANNEL;
		avdtpLinkInfo->state = AVDTP_STATE_CONNECTING;
		return AVDTPSignalChannelType;
	}
	else if(l2capChannelHdl == avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType])
	{
		avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType] = INVALID_L2CAP_CHANNEL;
		avdtpLinkInfo->state = AVDTP_STATE_IDLE_SIGNAL_CHANNEL;
		return AVDTPMediaChannelType;
	}
	return MAX_AVDTP_CHANNEL_TYPE_NUM;
}

PRIVATE void AVDTP_ClearLinkInfo(U8 linkIndex)
{
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);

	if(avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType] != INVALID_L2CAP_CHANNEL)
	{
		AVDTP_RemoveChannelHdl(avdtpLinkInfo, avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType]);
	}
	if(avdtpLinkInfo->l2capChannelHdl[AVDTPSignalChannelType] != INVALID_L2CAP_CHANNEL)
	{
		AVDTP_RemoveChannelHdl(avdtpLinkInfo, avdtpLinkInfo->l2capChannelHdl[AVDTPSignalChannelType]);
	}

	avdtpLinkInfo->state = AVDTP_STATE_IDLE;
	avdtpLinkInfo->misc.byte = 0;
}

PRIVATE U8 AVDTP_SearchChannelTypeByChannelHdl(AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo, L2CAP_CHANNEL_HDL l2capChannelHdl)
{
	U8 channelType;

	for(channelType = 0; channelType < MAX_AVDTP_CHANNEL_TYPE_NUM ;channelType++)
	{
		if(avdtpLinkInfo->l2capChannelHdl[channelType] == l2capChannelHdl)
			break;
	}
	return channelType;
}

PRIVATE void AVDTP_SignalChannelDataHandler(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 XDATA_PTR dataPtr)
{
	U8 linkIndex = L2CAP_GetChannelLinkIndex(l2capChannelHdl);
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);
	U8 XDATA_PTR srcPtr, XDATA_PTR dstPtr;
	U16 copyLength;

	if(avdtpLinkInfo->state && avdtpLinkInfo->l2capChannelHdl[AVDTPSignalChannelType] == l2capChannelHdl)
	{
		copyLength = *(U16 XDATA_PTR)(dataPtr+*dataPtr);
		copyLength = ENDIAN_TRANSFORM_U16(copyLength) + L2CAP_HEADER_LEN;

		srcPtr = dataPtr + copyLength + (L2CAP_RX_PDU_OFFSET - 1);
		dstPtr = dataPtr + copyLength + (L2CAP_TX_PDU_OFFSET - 1);

		for(; copyLength>0; copyLength--)
		{
			*dstPtr-- = *srcPtr--; //Right Shift Two Bytes
		}

		LightDbgPrintWithConnectionHandle (
			HCI_VOPCODE_VOGF(HCI_VENDOR_OGF_UART_AVDTP_DBGPRINT,HCI_VEVT_OCF_AVDTP_DBGPRINT_RX_SIGNALLING_PACKET),
			MMI_GetConnHandleByLinkIndex(linkIndex),
			(U8 XDATA_PTR)&((AvdtpSignalType XDATA_PTR)dataPtr)->l2capLength);

		((AvdtpSignalType XDATA_PTR)dataPtr)->l2capLength = ENDIAN_TRANSFORM_U16(((AvdtpSignalType XDATA_PTR)dataPtr)->l2capLength);

		if(((AvdtpSignalType XDATA_PTR)dataPtr)->signalHeader.f.messageType == AVDTP_COMMAND_MSG)
		{
			MMI_SniffDisable(linkIndex, MMI_SNIFF_AVDTP_SIGNAL);
		}
		else
		{
			MMI_SniffEnable(linkIndex, MMI_SNIFF_AVDTP_SIGNAL);
		}
		avdtpLinkInfo->avdtpProfileDesc->mmiProfileDesc.ProfileDataEvent(linkIndex, (U8 XDATA_PTR)dataPtr);
		return;
	}
	OSMEM_Put(dataPtr);
}

PRIVATE void AVDTP_MediaChannelDataHandler(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 XDATA_PTR dataPtr)
{
	U8 linkIndex = L2CAP_GetChannelLinkIndex(l2capChannelHdl);
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);

	if(avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType] == l2capChannelHdl)
	{
		avdtpLinkInfo->avdtpProfileDesc->MediaDataEvent(linkIndex, dataPtr);
	}
	else
	{
		OSMEM_Put(dataPtr);
	}
}

/*
PRIVATE void AVDTP(L2CAP_CHANNEL_HDL l2capChannelHdl)
{
	U8 linkIndex = L2CAP_GetChannelLinkIndex(l2capChannelHdl);
}*/

PRIVATE void AVDTP_SetL2capConnReqParamsForSignalChannel(L2CAPConnReqParams XDATA_PTR paramPtr)
{
	paramPtr->needAuth = TRUE;
	paramPtr->mtuSize = A2DP_SIGNAL_MTU_SIZE;
	paramPtr->ChannelDataHandler = AVDTP_SignalChannelDataHandler;
}

PRIVATE void AVDTP_SetL2capConnReqParamsForMediaChannel(L2CAPConnReqParams XDATA_PTR paramPtr, U8 linkIndex)
{
    A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo = A2DP_GetLinkInfo(linkIndex);

	paramPtr->needAuth = FALSE;
    paramPtr->mtuSize = A2DP_MEDIA_MTU_SIZE;
    #if SUPPORT_AAC_SNK
    if(a2dpLinkInfo->fA2DPRxMediaCodec == A2DP_AACDecoderRxMediaPacket)
    {
        paramPtr->mtuSize = A2DP_MEDIA_AAC_MTU_SIZE;
    }
	#endif
    LightDbgPrint("MTU size:%h",(U16)(paramPtr->mtuSize));
	paramPtr->ChannelDataHandler = AVDTP_MediaChannelDataHandler;
}

PRIVATE void AVDTP_RemoteConnReqEvt(L2CAP_CHANNEL_HDL l2capChannelHdl, L2CAPRemoteConnReqType XDATA_PTR reqPtr)
{
	U8 linkIndex = L2CAP_GetChannelLinkIndex(l2capChannelHdl);
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);
	LINK_EVENT_PARAMS LDATA evtParams;
	U8 channelType;

	if (avdtpLinkInfo->state == AVDTP_STATE_IDLE)
	{
		if (avdtpLinkInfo->avdtpProfileDesc = (AVDTP_PROFILE_DESC CODE_PTR)MMI_GetProfileDesc(PROFILE_A2DP))
		{
			evtParams.connRequest.profileId = avdtpLinkInfo->avdtpProfileDesc->mmiProfileDesc.profileId;
			avdtpLinkInfo->avdtpProfileDesc->mmiProfileDesc.ProfileLinkEvent(linkIndex, PROFILE_LINK_EVENT_CONN_REQUEST, &evtParams);
			if (evtParams.connRequest.result == PROFILE_LINK_RESULT_CONN_ACCEPT)
			{
				MMI_LoadProfile(linkIndex, evtParams.connRequest.profileInfo, &avdtpLinkInfo->avdtpProfileDesc->mmiProfileDesc);

				reqPtr->status = L2CAP_STATUS_SUCCESS;
				AVDTP_SetL2capConnReqParamsForSignalChannel(&reqPtr->params);

				avdtpLinkInfo->state = AVDTP_STATE_CONNECTING;
				avdtpLinkInfo->l2capChannelHdl[AVDTPSignalChannelType] = l2capChannelHdl;
				avdtpLinkInfo->misc.bits.isPassive = TRUE;
				return;
			}
		}
	}
	else if (avdtpLinkInfo->state == AVDTP_STATE_IDLE_SIGNAL_CHANNEL)
	{
		if((channelType = AVDTP_SearchChannelTypeByChannelHdl(avdtpLinkInfo, INVALID_L2CAP_CHANNEL)) < MAX_AVDTP_CHANNEL_TYPE_NUM &&
			avdtpLinkInfo->avdtpProfileDesc != (AVDTP_PROFILE_DESC CODE_PTR)NULL)
		{
			evtParams.connRequest.profileId = avdtpLinkInfo->avdtpProfileDesc->mmiProfileDesc.profileId;
			avdtpLinkInfo->avdtpProfileDesc->MediaLinkEvent(linkIndex, PROFILE_LINK_EVENT_CONN_REQUEST, &evtParams);
			if (evtParams.connRequest.result == PROFILE_LINK_RESULT_CONN_ACCEPT)
			{
				reqPtr->status = L2CAP_STATUS_SUCCESS;
				AVDTP_SetL2capConnReqParamsForMediaChannel(&reqPtr->params,linkIndex);

				avdtpLinkInfo->l2capChannelHdl[channelType] = l2capChannelHdl;
				return;
			}
		}
	}
	reqPtr->status = L2CAP_STATUS_CONNECTION_REFUSED_NO_RESOURCE;
}

PRIVATE void AVDTP_ConnCplEvt(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 status)
{
	U8 linkIndex = L2CAP_GetChannelLinkIndex(l2capChannelHdl);
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);
	LINK_EVENT_PARAMS LDATA evtParams;
	U8 channelType;

	if((channelType = AVDTP_SearchChannelTypeByChannelHdl(avdtpLinkInfo, l2capChannelHdl)) < MAX_AVDTP_CHANNEL_TYPE_NUM)
	{
		if (status == L2CAP_STATUS_SUCCESS)
		{
			evtParams.connComplete.status = NO_ERROR;
		}
		else
		{
			if (status == L2CAP_STATUS_CONNECTION_REFUSED_SECURITY_BLOCK)
			{
				evtParams.connComplete.status = SECURITY_BLOCK;
			}
			else
			{
				evtParams.connComplete.status = PROTOCOL_CONNECTION_ERROR;
			}
		}
		evtParams.connComplete.isPassive = avdtpLinkInfo->misc.bits.isPassive;

		if (channelType == AVDTPSignalChannelType)
		{
			avdtpLinkInfo->avdtpProfileDesc->mmiProfileDesc.ProfileLinkEvent(linkIndex, PROFILE_LINK_EVENT_CONN_COMPLETE, &evtParams);
		}
		else
		{
			avdtpLinkInfo->avdtpProfileDesc->MediaLinkEvent(linkIndex, PROFILE_LINK_EVENT_CONN_COMPLETE, &evtParams);
		}

		if (status == L2CAP_STATUS_SUCCESS)
		{
			avdtpLinkInfo->state = AVDTP_STATE_IDLE_SIGNAL_CHANNEL + 2*channelType;
		}
		else
		{
			if(AVDTP_RemoveChannelHdl(avdtpLinkInfo, l2capChannelHdl) == AVDTPSignalChannelType)
			{
				AVDTP_ClearLinkInfo(linkIndex);
			}
		}
	}
}

PRIVATE void AVDTP_DisconnectCplEvt(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 reason)
{
	U8 linkIndex = L2CAP_GetChannelLinkIndex(l2capChannelHdl);
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);
	U8 channelType;

	channelType = AVDTP_RemoveChannelHdl(avdtpLinkInfo, l2capChannelHdl);
	if (channelType < MAX_AVDTP_CHANNEL_TYPE_NUM)
	{
		if (channelType == AVDTPSignalChannelType)
		{
			avdtpLinkInfo->avdtpProfileDesc->mmiProfileDesc.ProfileLinkEvent(linkIndex, reason, (LINK_EVENT_PARAMS XDATA_PTR)NULL);
			AVDTP_ClearLinkInfo(linkIndex);
		}
		else
		{
			avdtpLinkInfo->avdtpProfileDesc->MediaLinkEvent(linkIndex, reason, (LINK_EVENT_PARAMS XDATA_PTR)NULL);
		}
	}
}

PUBLIC L2CAP_CHANNEL_HDL AVDTP_ConnectSignalChannel(U8 linkIndex)
{
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);
	L2CAP_CHANNEL_HDL l2capChannelHdl;
	L2CAPLocalConnReqType LDATA reqMsg;

	if(	avdtpLinkInfo->state == AVDTP_STATE_IDLE &&
		(avdtpLinkInfo->avdtpProfileDesc = (AVDTP_PROFILE_DESC CODE_PTR)MMI_GetProfileDesc(PROFILE_A2DP)))
	{
		reqMsg.psm = PSM_AVDTP;
		AVDTP_SetL2capConnReqParamsForSignalChannel(&reqMsg.params);

		l2capChannelHdl = UpperLayer_SendL2capConnectionRequest(linkIndex, &reqMsg);
		if (l2capChannelHdl != INVALID_L2CAP_CHANNEL)
		{
			avdtpLinkInfo->state = AVDTP_STATE_CONNECTING;
			avdtpLinkInfo->l2capChannelHdl[AVDTPSignalChannelType] = l2capChannelHdl;
			return l2capChannelHdl;
		}
	}
	return INVALID_L2CAP_CHANNEL;
}

PUBLIC U8 AVDTP_DisconnectSignalChannel(U8 linkIndex)
{
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);
	L2CAP_CHANNEL_HDL l2capChannelHdl;
	U8 channelType;
	U8 cmdResult = CMD_COMPLETE;

	for(channelType = 0; channelType < MAX_AVDTP_CHANNEL_TYPE_NUM; channelType++)
	{
		l2capChannelHdl = avdtpLinkInfo->l2capChannelHdl[MAX_AVDTP_CHANNEL_TYPE_NUM-1-channelType];
		if(l2capChannelHdl != INVALID_L2CAP_CHANNEL)
		{
			if(CMD_WAITING == UpperLayer_SendL2capDisconnectionRequest(l2capChannelHdl))
				cmdResult = CMD_WAITING;
			else
				AVDTP_RemoveChannelHdl(avdtpLinkInfo, l2capChannelHdl);
		}
	}
	if(cmdResult == CMD_COMPLETE)
	{
		AVDTP_ClearLinkInfo(linkIndex);
	}
	return cmdResult;
}

PUBLIC L2CAP_CHANNEL_HDL AVDTP_ConnectMediaChannel(U8 linkIndex)
{
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);
	L2CAP_CHANNEL_HDL l2capChannelHdl;
	L2CAPLocalConnReqType LDATA reqMsg;

	if(avdtpLinkInfo->state == AVDTP_STATE_IDLE_SIGNAL_CHANNEL)
	{
		reqMsg.psm = PSM_AVDTP;
		AVDTP_SetL2capConnReqParamsForMediaChannel(&reqMsg.params,linkIndex);

		l2capChannelHdl = UpperLayer_SendL2capConnectionRequest(linkIndex, &reqMsg);
		if (l2capChannelHdl != INVALID_L2CAP_CHANNEL)
		{
			avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType] = l2capChannelHdl;
			return l2capChannelHdl;
		}
	}
	return INVALID_L2CAP_CHANNEL;
}

PUBLIC U8 AVDTP_DisconnectMediaChannel(U8 linkIndex)
{
	AVDTP_LINK_INFO XDATA_PTR avdtpLinkInfo = AVDTP_GetLinkInfo(linkIndex);

	if(avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType] != INVALID_L2CAP_CHANNEL)
	{
		if(CMD_WAITING == UpperLayer_SendL2capDisconnectionRequest(avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType]))
		{
			return CMD_WAITING;
		}
		AVDTP_RemoveChannelHdl(avdtpLinkInfo, avdtpLinkInfo->l2capChannelHdl[AVDTPMediaChannelType]);
	}
	return CMD_COMPLETE;
}

PUBLIC void AVDTP_SendMediaChannelData(U8 linkIndex, AvdtpMediaType XDATA_PTR dataPtr)
{
	L2CAP_CHANNEL_HDL l2capChannelHdl = AVDTP_GetLinkInfo(linkIndex)->l2capChannelHdl[AVDTPMediaChannelType];

	if (l2capChannelHdl != INVALID_L2CAP_CHANNEL)
	{
		dataPtr->l2capLength = ENDIAN_TRANSFORM_U16(dataPtr->l2capLength);
		UpperLayer_SendL2capChannelData(l2capChannelHdl, (U8 XDATA_PTR)dataPtr);
	}
	else
	{
		OSMEM_Put(dataPtr);
	}
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/*                   	   AVDTP Utility							*/
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
static L2CAP_PROTOCOL_DESC CODE gAVDTP_ProtocolDesc = {
	PSM_AVDTP,
	NULL, /*AVDTP,	*/				//TaskEntry
	AVDTP_RemoteConnReqEvt,	//RemoteConnReqEvt
	AVDTP_ConnCplEvt,		//ConnCplEvt
	AVDTP_DisconnectCplEvt, //DisconnectCplEvt
};

static L2CAP_PROTOCOL_NODE XDATA gAVDTP_ProtocolNode = {
	&gAVDTP_ProtocolDesc,
	NULL
};

PUBLIC void AVDTP_Init(void)
{
	L2CAP_RegisterProtocol(&gAVDTP_ProtocolNode);
}
#endif


#endif
