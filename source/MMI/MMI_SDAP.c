#include "reside_flash.inc"

#define _MMI_SDAP_C_

#include "rc.h"
#include "bt_config_profile.h"

#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_PSM.h"
#include "MMI_HCI.h"
#include "MMI_SDAP.h"
#include "MMI_A2DP.h"
#include "MMI_AVRCP.h"
#include "MMI_AirModule.h"
#include "MMI_Reconnect.h"
#include "..\A2DP\A2DP_Interface.h"
#include "..\L2CAP\L2CAP_Interface.h"
#include "..\SDAP\SDAP_Interface.h"	
#ifdef SONGPAL_Profile
#include "..\SongPal\SongPal_Interface.h"	
#endif
#ifdef AIRAPP_Profile
#include "..\AirApp\AirApp_Interface.h"
#endif
#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];

#if defined SONGPAL_Profile || defined AIRAPP_Profile
PRIVATE void MMI_SDAP_SendServiceSearchAttributeReq128(U8 linkIndex, U8 GENERIC_PTR uuid128, U16 attirbuteId);
#endif

PRIVATE void MMI_SDAP_ServiceSearchCallback(U8 XDATA_PTR sdapRspPtr);
PRIVATE void MMI_SDAP_AttributeSearchCallback(U8 XDATA_PTR sdapRspPtr);
PRIVATE void MMI_SDAP_ServiceSearchAttributeCallback(U8 XDATA_PTR sdapRspPtr);
PRIVATE void MMI_SDAP_UseStatusClearStatus(U8 linkIndex);
PRIVATE void MMI_SDAP_SendNextServiceQuery(U8 linkIndex);
PRIVATE BOOL MMI_SDAP_UseStatusIsRegiste(U8 linkIndex ,U8 sdpType);
PRIVATE BOOL MMI_SDAP_UseStatusIsUsed(U8 linkIndex ,U8 sdpType);
PRIVATE void MMI_SDAP_UseStatusUseSDP(U8 linkIndex ,U8 sdpType);

PUBLIC void MMI_SDAP_Init(void)
{
	//SetupServiceRecords
	extern SdpServiceNode gMMI_MpsServiceNode;
	extern SdpServiceNode gMMI_PnpServiceNode;
	U8 GENERIC_PTR attributePtr;

	SDAP_RegisterServiceRecord(&gMMI_MpsServiceNode);
	SDAP_RegisterServiceRecord(&gMMI_PnpServiceNode);
	
	if (attributePtr = (U8 GENERIC_PTR)SDAP_GetServiceAttribute(PROFILE_PNP, ATTRID_ProductID))
	{
		*(U16 GENERIC_PTR)&attributePtr[4] = MMI_PNP_PRODUCT_ID;
	}
	if (attributePtr = (U8 GENERIC_PTR)SDAP_GetServiceAttribute(PROFILE_PNP, ATTRID_ProductVersion))
	{
		*(U16 GENERIC_PTR)&attributePtr[4] = MMI_PNP_PRODUCT_VER;
	}
}

PRIVATE void MMI_SDAP_ConnectionCpl(U8 linkIndex, U8 status)
{
	if(status == L2CAP_STATUS_SUCCESS)
	{
		MMI_SDAP_UseStatusUseSDP(linkIndex, SDP_USE_BY_MMI);
		if(MMI_SDAP_UseStatusIsUsed(linkIndex, SDP_USE_BY_SDK) && api_profile_func[API_GRP_PF_CB_SDP_SERVICE_HANDLER])
			((API_PROFILE_SDP_SERVICE_HANDLER_FUNC)api_profile_func[API_GRP_PF_CB_SDP_SERVICE_HANDLER])(linkIndex);
		else
			MMI_SDAP_SendNextServiceQuery(linkIndex);
	}
	else
	{
		gMMI_ctl.sdapCtl[linkIndex].sdapState = MMI_SDAP_STATE_IDLE;
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT || MMI_IsLinkActiveDisconnecting(linkIndex))
		{
			MMI_ReleaseProfileLinkAndDetach(linkIndex);
		}
	}
}

PRIVATE void MMI_SDAP_DisconnectionCpl(U8 linkIndex, U8 status)
{
	UNUSED(status);
	
	if (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_DISCONNECTION_COMPLETE && !MMI_IsLinkActiveDisconnecting(linkIndex))
	{
		gMMI_ctl.sdapCtl[linkIndex].sdapState = MMI_SDAP_STATE_SERVICE_QUERY_COMPLETE;
				
		if(MMI_CreateActiveProfileConnection(linkIndex))
		{
			switch(GET_PRODUCT_AIR_TYPE(MMI_PNP_PRODUCT_VER))
			{
				case PRODUCT_AIR_SCO_WOOFER:
				case PRODUCT_AIR_SOUNDBAR:
				case PRODUCT_AIR_TWS:
				case PRODUCT_AIR_A2DP_WOOFER:
					if(MMI_GetConnectedProfileCount(linkIndex, TRUE))
					{
						MMI_SetConnected(linkIndex);
					}
					break;
				default:
					break;
			}
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_DEVICE_TYPE, (U8 XDATA_PTR)NULL, linkIndex);
		}
	}
	else
	{
		gMMI_ctl.sdapCtl[linkIndex].sdapState = MMI_SDAP_STATE_IDLE;
	}
	
	MMI_SDAP_UseStatusClearStatus(linkIndex);
}

typedef struct
{
	void (*fEvt)(U8 linkIndex, U8 status);
}MMI_SDAP_EVT;

MMI_SDAP_EVT CODE MMI_SDAP_EvtHandle[SDAP_EVENT_TO_MMI_NO] = 
{
	{MMI_SDAP_ConnectionCpl}, //SDAP_EVENT_TO_MMI_CLIENT_CONNECTION_COMPLETE
	{MMI_SDAP_DisconnectionCpl}, //SDAP_EVENT_TO_MMI_DISCONNECTION_COMPLETE
};

PUBLIC void MMI_SDAP_EventHandler(U8 linkIndex, U8 opCode, U8 status)
{
	if (MMI_IsActiveLink(linkIndex))
	{
		if(MMI_SDAP_EvtHandle[opCode].fEvt)
			MMI_SDAP_EvtHandle[opCode].fEvt(linkIndex, status);
	}
}

PUBLIC void MMI_SDAP_StartServiceSearchProc(U8 linkIndex)
{
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delaySDPTimer);

	if(!gMMI_ctl.sdapCtl[linkIndex].supportedProfile )
	{
		if (MMI_SDAP_UseStatusRegister(linkIndex, SDP_USE_BY_MMI))
		{
			gMMI_ctl.sdapCtl[linkIndex].sdapState = MMI_SDAP_STATE_WAIT_CONNECTION_COMPLETE;
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SDAP_IN_PROGRESS;
		}
	}
	else if(gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_SERVICE_QUERY_COMPLETE && MMI_SDAP_UseStatusIsRegiste(linkIndex, SDP_USE_BY_MMI))
	{
		MMI_CreateActiveProfileConnection(linkIndex);
	}
}

PUBLIC BOOL MMI_SDAP_HideServiceRecord(U8 profileId)
{
	switch (profileId)
	{
		#ifdef HFP_Profile
		case PROFILE_HEADSET:
			return !MMI_SUPPORT_HSP_FEAT;
		case PROFILE_HANDSFREE:
			return !MMI_SUPPORT_HFP_FEAT;
		#endif

		#ifdef A2DP_Profile
		case PROFILE_A2DP_SINK:
			return !MMI_SUPPORT_A2DP_SINK_FEAT;
		case PROFILE_AVRCP_TG:
		case PROFILE_AVRCP_CT:
			return !MMI_SUPPORT_AVRCP_FEAT;
		#endif
	}
	return FALSE;
}

PUBLIC U8 MMI_SDAP_GetRemoteServiceChannel(U8 linkIndex, U8 profileId)
{
	switch (profileId)
	{
		case PROFILE_HEADSET:
			if (MMI_SUPPORT_HSP_FEAT && (gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_HSP))
			{
				return gMMI_ctl.sdapCtl[linkIndex].hspScn;
			}
			break;

		case PROFILE_HANDSFREE:
			if (MMI_SUPPORT_HFP_FEAT && (gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_HFP))
			{
				return gMMI_ctl.sdapCtl[linkIndex].hfpScn;
			}
			break;
		
		#ifdef SPP_Profile
		case PROFILE_SERIAL_PORT:
			if (gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_SPP)
			{
				return gMMI_ctl.sdapCtl[linkIndex].sppScn;
			}
			break;
		#endif
		
		#ifdef AIRAPP_Profile
		case PROFILE_AIR_APP:
			if (gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_AIRAPP)
			{
				return gMMI_ctl.sdapCtl[linkIndex].airappScn;
			}
			break;
		#endif

		#ifdef SONGPAL_Profile
		case PROFILE_SONGPAL_ANDROID:
		#ifndef IAP2_Profile
		case PROFILE_SONGPAL_IAP2:
		#else
		case PROFILE_IAP2:
		#endif
			if ((gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_SONGPAL) && (gMMI_ctl.sdapCtl[linkIndex].songpalProfileId == profileId))
			{
				return gMMI_ctl.sdapCtl[linkIndex].songpalScn;
			}
			break;
		#endif
			
		case PROFILE_A2DP_SOURCE:
			if (MMI_SUPPORT_A2DP_FEAT && (gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_A2DP))
			{
				return PROFILE_A2DP_SOURCE;
			}
			break;
			
		case PROFILE_AVRCP:
			if (MMI_SUPPORT_AVRCP_FEAT && (gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_AVRCP))
			{
				return PROFILE_AVRCP;
			}
			break;
		default:
			if(api_profile_func[API_GRP_PF_CB_SDP_GET_SERVICE_PROFILESCN])
			{
				return (((API_PROFILE_SDP_GET_SERVICE_PROFILESCN_FUNC)api_profile_func[API_GRP_PF_CB_SDP_GET_SERVICE_PROFILESCN])(linkIndex, profileId));
			}
			break;
	}
	return 0;
}

PUBLIC U32 XDATA_PTR MMI_SDAP_ParseServiceRecordHandle(U8 XDATA_PTR evtPtr)
{
	U16 recordCount;
	
	if (((SdapEvent XDATA_PTR)evtPtr)->eventBody.sssCompleteEvent.payloadLength)
	{
		recordCount = *(U16 XDATA_PTR)&((SdapEvent XDATA_PTR)evtPtr)->eventBody.sssCompleteEvent.payload[0];
		if (recordCount)
		{
			return (U32 XDATA_PTR)&((SdapEvent XDATA_PTR)evtPtr)->eventBody.sssCompleteEvent.payload[2];
		}
	}
	return (U32 XDATA_PTR)NULL;
}

PRIVATE void MMI_SDAP_ServiceSearchCallback(U8 XDATA_PTR sdapRspPtr)
{
	U8 linkIndex = ((SdapEvent XDATA_PTR)sdapRspPtr)->eventBody.sssCompleteEvent.linkIndex;
	U8 useType = MMI_SDAP_UseStatusFindInUsed(linkIndex);
	U32 XDATA_PTR pServiceRecordHandle;
	BOOL pnpFound = FALSE;

	if (pServiceRecordHandle = MMI_SDAP_ParseServiceRecordHandle(sdapRspPtr))
	{
		if (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_PNP_SERVICE_RESULT)
		{
			pnpFound = TRUE;
		}
	}
	OSMEM_Put(sdapRspPtr);
	
	if(useType != SDP_USER_EOF)
	{
		MMI_SDAP_UseStatusUseFinish(linkIndex, useType);
		MMI_SDAP_UseStatusSetWaitCmd(linkIndex, MMI_SDAP_UseStatusFindInUsed(linkIndex));
	}
	
	if (pnpFound)
	{
		MMI_SDAP_SendAttributeSearchReq(linkIndex, *pServiceRecordHandle, ATTRID_SpecifcationID, ATTRID_VendorIDSource, SDP_USE_BY_MMI);
	}
	else
	{
		MMI_SDAP_SendNextServiceQuery(linkIndex);
	}
}

PRIVATE void MMI_SDAP_ParsePNPInformation(SdapEvent XDATA_PTR evtPtr, PNPInformation XDATA_PTR pnpInfo)
{
	U8 size;
	U16 totalBytes = evtPtr->eventBody.sssCompleteEvent.payloadLength;
	U8 XDATA_PTR payloadPtr = (U8 XDATA_PTR)evtPtr->eventBody.sssCompleteEvent.payload;

	OSMEM_memset_xdata((U8 XDATA_PTR)pnpInfo, 0, sizeof(PNPInformation));

	//skip data element sequence
	if (totalBytes == 0 || DELEM_TYPE(payloadPtr[0]) != DELEM_SEQUENCE)
		return;

	//skip data element sequence
	size = SDAP_GetDataElementSize(payloadPtr[0]);
	payloadPtr += size;
	totalBytes -= size;
	
	while (totalBytes)
	{
		//check attribute id type
		if (payloadPtr[0] != DELEM_U16)
			break;

		switch (*(U16 XDATA_PTR)&payloadPtr[1])
		{
			case ATTRID_SpecifcationID:
				pnpInfo->specId = *(U16 XDATA_PTR)&payloadPtr[4];
				break;
			case ATTRID_VendorID:
				pnpInfo->vendorId = *(U16 XDATA_PTR)&payloadPtr[4];
				break;
			case ATTRID_ProductID:
				pnpInfo->productId = *(U16 XDATA_PTR)&payloadPtr[4];
				break;
			case ATTRID_ProductVersion:
				pnpInfo->productVersion = *(U16 XDATA_PTR)&payloadPtr[4];
				break;
			case ATTRID_PrimaryRecord:
				pnpInfo->primaryRecord = *(U8 XDATA_PTR)&payloadPtr[4];
				break;
			case ATTRID_VendorIDSource:
				pnpInfo->vendorIdSource = *(U16 XDATA_PTR)&payloadPtr[4];
				break;
		}
		
		//next data element
		size = SDAP_GetDataElementSize(payloadPtr[3]) + 3/*size of attribute element*/;
		payloadPtr += size;
		totalBytes -= size;
	}
}

PRIVATE BOOL MMI_SDAP_IsRemoteAirModeSuppored(PNPInformation XDATA_PTR pnpInfo)
{
	if (pnpInfo->vendorId != AIROHA_COMPANY_ID ||
		pnpInfo->vendorIdSource != VENDOR_ID_SOURCE_BLUETOOTH ||
		pnpInfo->productId != MMI_PNP_PRODUCT_ID )
	{
		return FALSE;
	}
	return TRUE;
}

PRIVATE void MMI_SDAP_AttributeSearchCallback(U8 XDATA_PTR sdapRspPtr)
{
	U8 linkIndex = ((SdapEvent XDATA_PTR)sdapRspPtr)->eventBody.sssCompleteEvent.linkIndex;
	U8 useType = MMI_SDAP_UseStatusFindInUsed(linkIndex);
	PNPInformation LDATA pnpInfo;

	if (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_PNP_SERVICE_RESULT)
	{
		MMI_SDAP_ParsePNPInformation((SdapEvent XDATA_PTR)sdapRspPtr, (PNPInformation XDATA_PTR)&pnpInfo);

		if(MMI_SDAP_IsRemoteAirModeSuppored((PNPInformation XDATA_PTR)&pnpInfo))
		{
			gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID = GET_PRODUCT_AIR_TYPE(((PNPInformation XDATA_PTR)&pnpInfo)->productVersion);
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_PNP;
		}
		else
		{
			gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID = 0;
		}
	}
	OSMEM_Put(sdapRspPtr);
	
	if(useType != SDP_USER_EOF)
	{
		MMI_SDAP_UseStatusUseFinish(linkIndex, useType);
		MMI_SDAP_UseStatusSetWaitCmd(linkIndex, MMI_SDAP_UseStatusFindInUsed(linkIndex));
	}

	MMI_SDAP_SendNextServiceQuery(linkIndex);
}

PUBLIC U8 MMI_SDAP_ParseRemoteScn(U8 XDATA_PTR evtPtr, U16 psm) 
{
	U8 size, result = 0;
	U16 totalBytes = ((SdapEvent XDATA_PTR)evtPtr)->eventBody.sssCompleteEvent.payloadLength;
	U8 XDATA_PTR payloadPtr = (U8 XDATA_PTR)((SdapEvent XDATA_PTR)evtPtr)->eventBody.sssCompleteEvent.payload;

	while (totalBytes)
	{
		//Check if this data element is "RFCOMM/SCN" (HFP Spec 1.5 P.82)
		if (payloadPtr[0] == DELEM_UUID16)
		{
			if (*((U16 XDATA_PTR)&payloadPtr[1]) == psm)
			{
				if (psm == PSM_RFCOMM && payloadPtr[3] == DELEM_U8)
				{
					result = payloadPtr[4];
					break;
				}
				#ifdef A2DP_Profile
				if(psm == PSM_AVDTP || psm == PSM_AVCTP)
				{
					result = 1;
					break;
				}
				#endif
			}
		}

		//next data element
		size = SDAP_GetDataElementSize(payloadPtr[0]);
		payloadPtr += size;
		totalBytes -= size;
	}

	return result;
}

PRIVATE void MMI_SDAP_ServiceSearchAttributeCallback(U8 XDATA_PTR sdapRspPtr)
{
	U8 linkIndex = ((SdapEvent XDATA_PTR)sdapRspPtr)->eventBody.sssCompleteEvent.linkIndex;
	U8 useType = MMI_SDAP_UseStatusFindInUsed(linkIndex);

	switch(gMMI_ctl.sdapCtl[linkIndex].sdapState)
	{
		case MMI_SDAP_STATE_WAIT_HFP_SERVICE_RESULT:
			if (gMMI_ctl.sdapCtl[linkIndex].hfpScn = MMI_SDAP_ParseRemoteScn(sdapRspPtr, PSM_RFCOMM))
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_HFP;
			}
			break;
		case MMI_SDAP_STATE_WAIT_HSP_SERVICE_RESULT:
			if (gMMI_ctl.sdapCtl[linkIndex].hspScn = MMI_SDAP_ParseRemoteScn(sdapRspPtr, PSM_RFCOMM))
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_HSP;
			}
			break;
		case MMI_SDAP_STATE_WAIT_A2DP_SERVICE_RESULT:
			if (MMI_SDAP_ParseRemoteScn(sdapRspPtr, PSM_AVDTP))
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_A2DP;
			}
			else if (gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID == PRODUCT_AIR_TWS && MMI_SUPPORT_A2DP_SINK_FEAT)
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_A2DP;
			}
			break;
		case MMI_SDAP_STATE_WAIT_AVRCP_TG_SERVICE_RESULT:
		case MMI_SDAP_STATE_WAIT_AVRCP_CT_SERVICE_RESULT:
			if (MMI_SDAP_ParseRemoteScn(sdapRspPtr, PSM_AVCTP))
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_AVRCP;
			}
			else if (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_AVRCP_TG_SERVICE_RESULT)
			{
				gMMI_ctl.sdapCtl[linkIndex].searchedProfile &= ~SDAP_SEARCH_AVRCP;
			}
			break;
		#ifdef SPP_Profile
		case MMI_SDAP_STATE_WAIT_SPP_SERVICE_RESULT:
			if (gMMI_ctl.sdapCtl[linkIndex].sppScn = MMI_SDAP_ParseRemoteScn(sdapRspPtr, PSM_RFCOMM))
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_SPP;
			}
			break;
		#endif
		#ifdef SONGPAL_Profile
		case MMI_SDAP_STATE_WAIT_SONGPAL_ANDROID_RESULT:
		case MMI_SDAP_STATE_WAIT_SONGPAL_IAP2_RESULT:
			if (gMMI_ctl.sdapCtl[linkIndex].songpalScn = MMI_SDAP_ParseRemoteScn(sdapRspPtr, PSM_RFCOMM))
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_SONGPAL;
				gMMI_ctl.sdapCtl[linkIndex].songpalProfileId = (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_SONGPAL_ANDROID_RESULT) ?
																	PROFILE_SONGPAL_ANDROID :
																	#ifndef IAP2_Profile
																	PROFILE_SONGPAL_IAP2;
																	#else
																	PROFILE_IAP2;
																	#endif
			}
			else if (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_SONGPAL_ANDROID_RESULT)
			{
				gMMI_ctl.sdapCtl[linkIndex].searchedProfile &= ~SDAP_SEARCH_SONGPAL;
			}
			break;
		#endif
		#ifdef AIRAPP_Profile
		case MMI_SDAP_STATE_WAIT_AIRAPP_SERVICE_RESULT:
			if (gMMI_ctl.sdapCtl[linkIndex].airappScn = MMI_SDAP_ParseRemoteScn(sdapRspPtr, PSM_RFCOMM))
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_AIRAPP;
			}
			break;		
		#endif
		#ifdef PBAP_Profile
		case MMI_SDAP_STATE_WAIT_PBAP_SERVICE_RESULT:
			if (gMMI_ctl.sdapCtl[linkIndex].pbapScn = MMI_SDAP_ParseRemoteScn(sdapRspPtr, PSM_RFCOMM))
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_PBAP;
			}
			break;
		#endif
	}
	OSMEM_Put(sdapRspPtr);

	if(useType != SDP_USER_EOF)
	{
		MMI_SDAP_UseStatusUseFinish(linkIndex, useType);
		MMI_SDAP_UseStatusSetWaitCmd(linkIndex, MMI_SDAP_UseStatusFindInUsed(linkIndex));
	}
	
	MMI_SDAP_SendNextServiceQuery(linkIndex);
}

PRIVATE void MMI_SDAP_SendNextServiceQuery(U8 linkIndex)
{
	U8 searchedProfile = gMMI_ctl.sdapCtl[linkIndex].searchedProfile;
	U8 sdapState = MMI_SDAP_STATE_SEND_DISCONNECTION_REQ;
	
	if (!(searchedProfile & SDAP_SEARCH_PNP))
	{
		searchedProfile |= SDAP_SEARCH_PNP;
		MMI_SDAP_SendServiceSearchReq(linkIndex, SERVICE_UUID_PNP_INFORMATION, SDP_USE_BY_MMI);
		sdapState = MMI_SDAP_STATE_WAIT_PNP_SERVICE_RESULT;
		goto label_search_sent;
	}

	if(gMMI_nvram.firstConnectProfile == SDAP_FIRST_CONN_AVRCP && !(searchedProfile & SDAP_SEARCH_AVRCP))
	{
		goto CONNECT_AVRCP_SDP;
	}

	if (!(searchedProfile & SDAP_SEARCH_HFP))
	{
		searchedProfile |= SDAP_SEARCH_HFP;
		if (MMI_SUPPORT_HFP_FEAT)
		{
			#ifdef HFP_AG_Profile
			MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_HANDSFREE, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
			#else
			MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_HANDSFREE_AG, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
			#endif
			sdapState = MMI_SDAP_STATE_WAIT_HFP_SERVICE_RESULT;
			goto label_search_sent;
		}
	}

	if (!(searchedProfile & SDAP_SEARCH_HSP))
	{
		searchedProfile |= SDAP_SEARCH_HSP;
		if (MMI_SUPPORT_HSP_FEAT)
		{
			#ifdef HFP_AG_Profile
			MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_HEADSET, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
			#else
			MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_HEADSET_AG, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
			#endif
			sdapState = MMI_SDAP_STATE_WAIT_HSP_SERVICE_RESULT;
			goto label_search_sent;
		}
	}

	if (!(searchedProfile & SDAP_SEARCH_A2DP))
	{
		searchedProfile |= SDAP_SEARCH_A2DP;
		if (MMI_SUPPORT_A2DP_FEAT)
		{
			#ifdef A2DP_SRC_Profile
			MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_AUDIO_SINK, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
			#else
			MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_AUDIO_SOURCE, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
			#endif
			sdapState = MMI_SDAP_STATE_WAIT_A2DP_SERVICE_RESULT;
			goto label_search_sent;
		}
	}

	if (!(searchedProfile & SDAP_SEARCH_AVRCP))
	{
		CONNECT_AVRCP_SDP:
		searchedProfile |= SDAP_SEARCH_AVRCP;
		if (MMI_SUPPORT_AVRCP_FEAT)
		{
			if (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_AVRCP_TG_SERVICE_RESULT)
			{
				MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_REMOTE_CONTROL_CTL, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
				sdapState = MMI_SDAP_STATE_WAIT_AVRCP_CT_SERVICE_RESULT;
			}
			else
			{
				MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_REMOTE_CONTROL_TRG, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
				sdapState = MMI_SDAP_STATE_WAIT_AVRCP_TG_SERVICE_RESULT;
			}
			goto label_search_sent;
		}
	}

	#ifdef SPP_Profile
	if (!(searchedProfile & SDAP_SEARCH_SPP))
	{
		searchedProfile |= SDAP_SEARCH_SPP;
		MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_SERIAL_PORT, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
		sdapState = MMI_SDAP_STATE_WAIT_SPP_SERVICE_RESULT;
		goto label_search_sent;
	}
	#endif

	#ifdef SONGPAL_Profile
	if (!(searchedProfile & SDAP_SEARCH_SONGPAL))
	{
		searchedProfile |= SDAP_SEARCH_SONGPAL;
		if (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_SONGPAL_ANDROID_RESULT)
		{
			MMI_SDAP_SendServiceSearchAttributeReq128(linkIndex, (U8 GENERIC_PTR)gSongPal_iap2_uuid128, ATTRID_ProtocolDescriptorList);
			sdapState = MMI_SDAP_STATE_WAIT_SONGPAL_IAP2_RESULT;
		}
		else
		{
			MMI_SDAP_SendServiceSearchAttributeReq128(linkIndex, (U8 GENERIC_PTR)gSongPal_android_uuid128, ATTRID_ProtocolDescriptorList);
			sdapState = MMI_SDAP_STATE_WAIT_SONGPAL_ANDROID_RESULT;
		}
		goto label_search_sent;
	}
	#endif
	
	#ifdef AIRAPP_Profile
	if (!(searchedProfile & SDAP_SEARCH_AIRAPP))
	{
		searchedProfile |= SDAP_SEARCH_AIRAPP;
		MMI_SDAP_SendServiceSearchAttributeReq128(linkIndex, (U8 GENERIC_PTR)gAirApp_uuid128, ATTRID_ProtocolDescriptorList);
		sdapState = MMI_SDAP_STATE_WAIT_AIRAPP_SERVICE_RESULT;
		goto label_search_sent;
	}	
	#endif

	#ifdef PBAP_Profile
	if (!(searchedProfile & SDAP_SEARCH_PBAP))
	{
		searchedProfile |= SDAP_SEARCH_PBAP;
		MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_PB_ACCESS_SERVER, ATTRID_ProtocolDescriptorList, SDP_USE_BY_MMI);
		sdapState = MMI_SDAP_STATE_WAIT_PBAP_SERVICE_RESULT;
		goto label_search_sent;
	}
	#endif

label_search_sent:
	gMMI_ctl.sdapCtl[linkIndex].searchedProfile = searchedProfile;
	gMMI_ctl.sdapCtl[linkIndex].sdapState = sdapState;
	if(sdapState == MMI_SDAP_STATE_SEND_DISCONNECTION_REQ)
	{
		MMI_SDAP_UseStatusDeRegister(linkIndex, SDP_USE_BY_MMI);
	}
}

PUBLIC void MMI_SDAP_SendServiceSearchReq(U8 linkIndex, U16 uuid16, U8 useType)
{
	MMIMsgType XDATA_PTR cmdPtr;
	MMI_SDAP_UseStatusUseSDP(linkIndex, useType);
	if(!MMI_SDAP_UseStatusIsUsed(linkIndex, useType))
	{
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitUser = useType;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitCmd = SDP_SERVICE_SEARCH;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.serviceSearchCmd.uuid16 = uuid16;
		return;
	}
	
	cmdPtr = MMI_PROTOCOL_CommandFactory(MMI_CMD_TO_SDAP_SERVICE_SEARCH);

	if(cmdPtr)
	{
		if(useType == SDP_USE_BY_SDK && MMI_SDAP_UseStatusIsUsed(linkIndex, SDP_USE_BY_SDK) && api_profile_func[API_GRP_PF_CB_SDP_SERVICE_SEARCH])
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = ((API_PROFILE_SDP_SERVICE_SEARCH_FUNC)api_profile_func[API_GRP_PF_CB_SDP_SERVICE_SEARCH]);
		else if(MMI_SDAP_UseStatusIsUsed(linkIndex, SDP_USE_BY_MMI))
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = MMI_SDAP_ServiceSearchCallback;
		else
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = NULL;

		cmdPtr->msgBodyPtr.sdapSearchCmd.param.serviceSearch.serviceId.uuidLen = 2;
		*(U16 XDATA_PTR)&cmdPtr->msgBodyPtr.sdapSearchCmd.param.serviceSearch.serviceId.uuid[0] = uuid16;
		SDAP_SendSearchRequest(gMMI_ctl.sdapCtl[linkIndex].sdapClient, (U8 XDATA_PTR)cmdPtr);
	}	
}

PUBLIC void MMI_SDAP_SendAttributeSearchReq(U8 linkIndex, U32 serviceRecordHandle, U16 attributeId1, U16 attributeId2, U8 useType)
{
	MMIMsgType XDATA_PTR cmdPtr;
	MMI_SDAP_UseStatusUseSDP(linkIndex, useType);
	if(!MMI_SDAP_UseStatusIsUsed(linkIndex, useType))
	{
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitUser = useType;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitCmd = SDP_ATTRIBUTE_SEARCH;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.attributeSearchCmd.serviceRecordHandle = serviceRecordHandle;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.attributeSearchCmd.attributeId1 = attributeId1;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.attributeSearchCmd.attributeId2 = attributeId2;
		return;
	}
	
	cmdPtr = MMI_PROTOCOL_CommandFactory(MMI_CMD_TO_SDAP_ATTRIBUTE_SEARCH);

	if(cmdPtr)
	{
		if(useType == SDP_USE_BY_SDK && MMI_SDAP_UseStatusIsUsed(linkIndex, SDP_USE_BY_SDK) && api_profile_func[API_GRP_PF_CB_SDP_ATTRIBUTE_SEARCH])
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = ((API_PROFILE_SDP_ATTRIBUTE_SEARCH_FUNC)api_profile_func[API_GRP_PF_CB_SDP_ATTRIBUTE_SEARCH]);
		else if(MMI_SDAP_UseStatusIsUsed(linkIndex, SDP_USE_BY_MMI))
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = MMI_SDAP_AttributeSearchCallback;
		else
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = NULL;

		cmdPtr->msgBodyPtr.sdapSearchCmd.param.attributeSearch.serviceRecordHandle = serviceRecordHandle;
		cmdPtr->msgBodyPtr.sdapSearchCmd.param.attributeSearch.attributeId1 = attributeId1;
		cmdPtr->msgBodyPtr.sdapSearchCmd.param.attributeSearch.attributeId2 = attributeId2;
		
		SDAP_SendSearchRequest(gMMI_ctl.sdapCtl[linkIndex].sdapClient, (U8 XDATA_PTR)cmdPtr);
	}	
}

PUBLIC void MMI_SDAP_SendServiceSearchAttributeReq(U8 linkIndex, U16 uuid16, U16 attirbuteId, U8 useType)
{
	MMIMsgType XDATA_PTR cmdPtr;
	MMI_SDAP_UseStatusUseSDP(linkIndex, useType);
	if(!MMI_SDAP_UseStatusIsUsed(linkIndex, useType))
	{
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitUser = useType;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitCmd = SDP_SERVICE_ATTRIBUTE_SEARCH;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.serviceAttributeSearchCmd.uuid16 = uuid16;
		gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.serviceAttributeSearchCmd.attirbuteId = attirbuteId;
		return;
	}
	cmdPtr = MMI_PROTOCOL_CommandFactory(MMI_CMD_TO_SDAP_SERVICE_SEARCH_ATTRIBUTE);

	if(cmdPtr)
	{
		if(useType == SDP_USE_BY_SDK && MMI_SDAP_UseStatusIsUsed(linkIndex, SDP_USE_BY_SDK) && api_profile_func[API_GRP_PF_CB_SDP_SERVICE_SEARCH_ATTRIBUTE])
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = ((API_PROFILE_SDP_SERVICE_SEARCH_ATTRIBUTE_FUNC)api_profile_func[API_GRP_PF_CB_SDP_SERVICE_SEARCH_ATTRIBUTE]);
		else if(MMI_SDAP_UseStatusIsUsed(linkIndex, SDP_USE_BY_MMI))
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = MMI_SDAP_ServiceSearchAttributeCallback;
		else
			cmdPtr->msgBodyPtr.sdapSearchCmd.callback = NULL;

		cmdPtr->msgBodyPtr.sdapSearchCmd.param.serviceSearchAttribute.serviceId.uuidLen = 2;
		*(U16 XDATA_PTR)&cmdPtr->msgBodyPtr.sdapSearchCmd.param.serviceSearchAttribute.serviceId.uuid[0] = uuid16;
		cmdPtr->msgBodyPtr.sdapSearchCmd.param.serviceSearchAttribute.attributeId = attirbuteId;		

		SDAP_SendSearchRequest(gMMI_ctl.sdapCtl[linkIndex].sdapClient, (U8 XDATA_PTR)cmdPtr);
	}	
}

#if defined SONGPAL_Profile || defined AIRAPP_Profile
PRIVATE void MMI_SDAP_SendServiceSearchAttributeReq128(U8 linkIndex, U8 GENERIC_PTR uuid128, U16 attirbuteId)
{
	MMIMsgType XDATA_PTR cmdPtr = MMI_PROTOCOL_CommandFactory(MMI_CMD_TO_SDAP_SERVICE_SEARCH_ATTRIBUTE);

	if(cmdPtr)
	{
		cmdPtr->msgBodyPtr.sdapSearchCmd.callback = MMI_SDAP_ServiceSearchAttributeCallback;
		cmdPtr->msgBodyPtr.sdapSearchCmd.param.serviceSearchAttribute.serviceId.uuidLen = 16;
		OSMEM_memcpy_xdata_generic(&cmdPtr->msgBodyPtr.sdapSearchCmd.param.serviceSearchAttribute.serviceId.uuid[0], uuid128, 16);
		cmdPtr->msgBodyPtr.sdapSearchCmd.param.serviceSearchAttribute.attributeId = attirbuteId;		

		SDAP_SendSearchRequest(gMMI_ctl.sdapCtl[linkIndex].sdapClient, (U8 XDATA_PTR)cmdPtr);
	}	
}
#endif

PUBLIC void MMI_SDAP_ClearCtlData(U8 linkIndex)
{
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_ctl.sdapCtl[linkIndex], 0, sizeof(SDAPCtlType));
	#if SDK_SDP_PROFILE
	if(api_profile_func[API_GRP_PF_CB_SDP_CLEAR_CTL_DATA])
		((API_PROFILE_SDP_CLEAR_CTL_DATA_FUNC)api_profile_func[API_GRP_PF_CB_SDP_CLEAR_CTL_DATA])(linkIndex);
	#endif
}

PUBLIC BOOL MMI_SDAP_IsQueryComplete(U8 linkIndex)
{
	return (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_SERVICE_QUERY_COMPLETE) ? TRUE : FALSE;
}

#ifdef HID_Profile
PUBLIC BOOL MMI_SDAP_IsSupportHID(U8 linkIndex)
{
	if(gMMI_ctl.sdapCtl[linkIndex].supportedProfile & SUPPORT_HID)
		return TRUE;

	return FALSE;
}
#endif

PUBLIC void MMI_SDAP_SetRemoteApplicationID(U8 linkIndex, U8 productType)
{
	gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID = productType;
}

PRIVATE BOOL MMI_SDAP_UseStatusIsRegiste(U8 linkIndex ,U8 sdpType)
{
	return (gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[sdpType] > SDP_USER_STATUS_UNREGISTER) ? TRUE : FALSE;
}

PRIVATE BOOL MMI_SDAP_UseStatusIsUsed(U8 linkIndex ,U8 sdpType)
{
	return (gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[sdpType] == SDP_USER_STATUS_IN_USED) ? TRUE : FALSE;
}

PRIVATE void MMI_SDAP_UseStatusUseSDP(U8 linkIndex ,U8 sdpType)
{
	if(gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[((sdpType == SDP_USE_BY_MMI) ? SDP_USE_BY_SDK: SDP_USE_BY_MMI)] == SDP_USER_STATUS_IN_USED)
		gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[sdpType] = SDP_USER_STATUS_WAIT_TO_USE;
	else
		gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[sdpType] = SDP_USER_STATUS_IN_USED;
}

PUBLIC void MMI_SDAP_UseStatusUseFinish(U8 linkIndex ,U8 sdpType)
{
	gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[sdpType] = SDP_USER_STATUS_REGISTER;
	if(gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[((sdpType == SDP_USE_BY_MMI) ? SDP_USE_BY_SDK: SDP_USE_BY_MMI)] == SDP_USER_STATUS_WAIT_TO_USE)
		gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[((sdpType == SDP_USE_BY_MMI) ? SDP_USE_BY_SDK: SDP_USE_BY_MMI)] = SDP_USER_STATUS_IN_USED;
}

PUBLIC BOOL MMI_SDAP_UseStatusRegister(U8 linkIndex ,U8 sdpType)
{
	if(MMI_IsProfileConnected(linkIndex, PROFILE_SDAP) || 
		(gMMI_ctl.sdapCtl[linkIndex].sdapClient = MMI_ConnectProfile(linkIndex, PROFILE_SDAP)))
	{
		gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[sdpType] = SDP_USER_STATUS_REGISTER;
		return TRUE;
	}
	else
		return FALSE;
}

PRIVATE void MMI_SDAP_ProfileDisconnect(U8 linkIndex)
{
	MMI_DisconnectProfile(linkIndex, gMMI_ctl.sdapCtl[linkIndex].sdapClient);
	gMMI_ctl.sdapCtl[linkIndex].sdapState = MMI_SDAP_STATE_WAIT_DISCONNECTION_COMPLETE;
}

PUBLIC void MMI_SDAP_UseStatusDeRegister(U8 linkIndex ,U8 sdpType)
{
	gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[sdpType] = SDP_USER_STATUS_UNREGISTER;
	if(gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[((sdpType == SDP_USE_BY_MMI) ? SDP_USE_BY_SDK: SDP_USE_BY_MMI)] == SDP_USER_STATUS_WAIT_TO_USE)
		gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[((sdpType == SDP_USE_BY_MMI) ? SDP_USE_BY_SDK: SDP_USE_BY_MMI)] = SDP_USER_STATUS_IN_USED;
	
	if(MMI_SDAP_UseStatusIsRegiste(linkIndex, ((sdpType == SDP_USE_BY_MMI) ? SDP_USE_BY_SDK: SDP_USE_BY_MMI)))
	{
		if(sdpType == SDP_USE_BY_MMI)
		{
			if(api_profile_func[API_GRP_PF_CB_SDP_SERVICE_HANDLER])
				((API_PROFILE_SDP_SERVICE_HANDLER_FUNC)api_profile_func[API_GRP_PF_CB_SDP_SERVICE_HANDLER])(linkIndex);
		}
		else
			MMI_SDAP_SendNextServiceQuery(linkIndex);
	}
	else
	{
		MMI_SDAP_ProfileDisconnect(linkIndex);
	}
}

PRIVATE void MMI_SDAP_UseStatusClearStatus(U8 linkIndex)
{
	gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[SDP_USE_BY_MMI] = SDP_USER_STATUS_NONE;
	gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[SDP_USE_BY_SDK] = SDP_USER_STATUS_NONE;
}

PUBLIC U8 MMI_SDAP_UseStatusFindInUsed(U8 linkIndex)
{
	U8 useType = SDP_USER_EOF;
	
	for(useType = 0; useType < SDP_USER_NUM; useType++)
	{
		if(gMMI_ctl.sdapCtl[linkIndex].sdpUserStatus[useType] == SDP_USER_STATUS_IN_USED)
			return useType;
	}
	return useType;
}

PUBLIC void MMI_SDAP_UseStatusSetWaitCmd(U8 linkIndex, U8 sdpType)
{
	if(MMI_SDAP_UseStatusFindInUsed(linkIndex) == SDP_USER_EOF)
		return;
	
	if(gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitUser == sdpType)
	{
		if(gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitCmd == SDP_SERVICE_SEARCH)
		{
			MMI_SDAP_SendServiceSearchReq(linkIndex, gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.serviceSearchCmd.uuid16, MMI_SDAP_UseStatusFindInUsed(linkIndex));
		}
		else if(gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitCmd == SDP_ATTRIBUTE_SEARCH)
		{
			MMI_SDAP_SendAttributeSearchReq(linkIndex, gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.attributeSearchCmd.serviceRecordHandle,
											gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.attributeSearchCmd.attributeId1,
											gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.attributeSearchCmd.attributeId2,
											MMI_SDAP_UseStatusFindInUsed(linkIndex));
		}
		else if(gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.waitCmd == SDP_SERVICE_ATTRIBUTE_SEARCH)
		{
			MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.serviceAttributeSearchCmd.uuid16,
													gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd.cmdRecord.serviceAttributeSearchCmd.attirbuteId,
													MMI_SDAP_UseStatusFindInUsed(linkIndex));
		}
	}
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_ctl.sdapCtl[linkIndex].SdpTempCmd, 0 ,sizeof(SdpTempCmdType));
}

PUBLIC BOOL MMI_SDAP_SetProfileSCN(U8 linkIndex, U8 profileID, U8 scn)
{
	switch(profileID)
	{
		case PROFILE_HANDSFREE:
			gMMI_ctl.sdapCtl[linkIndex].hfpScn = scn;
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_HFP;
			break;
		case PROFILE_HEADSET:
			gMMI_ctl.sdapCtl[linkIndex].hspScn = scn;
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_HSP;
			break;
		case PROFILE_A2DP_SOURCE:
			if(scn)
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_A2DP;
			}
			else
				return FALSE;
			break;
		case PROFILE_AVRCP:
			if(scn)
			{
				gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_AVRCP;
			}
			else
				return FALSE;
			break;
		#ifdef SPP_Profile
		case PROFILE_SERIAL_PORT:
			gMMI_ctl.sdapCtl[linkIndex].sppScn = scn;
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_SPP;
			break;
		#endif
		#ifdef SONGPAL_Profile
		case PROFILE_SONGPAL_ANDROID:
		#ifndef IAP2_Profile
		case PROFILE_SONGPAL_IAP2:
		#else
		case PROFILE_IAP2:
		#endif
			gMMI_ctl.sdapCtl[linkIndex].songpalScn = scn;
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_SONGPAL;
			gMMI_ctl.sdapCtl[linkIndex].songpalProfileId = (gMMI_ctl.sdapCtl[linkIndex].sdapState == MMI_SDAP_STATE_WAIT_SONGPAL_ANDROID_RESULT) ?
																	PROFILE_SONGPAL_ANDROID :
																	#ifndef IAP2_Profile
																	PROFILE_SONGPAL_IAP2;
																	#else
																	PROFILE_IAP2;
																	#endif
			break;
		#endif
		#ifdef AIRAPP_Profile
		case PROFILE_AIR_APP:
			gMMI_ctl.sdapCtl[linkIndex].airappScn = scn;
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_AIRAPP;
			break;		
		#endif
		#ifdef PBAP_Profile
		case PROFILE_PHONEBOOK_ACCESS:
			gMMI_ctl.sdapCtl[linkIndex].pbapScn = scn;
			gMMI_ctl.sdapCtl[linkIndex].supportedProfile |= SUPPORT_PBAP;
			break;
		#endif
		default:
			return FALSE;
			break;
	}
	return TRUE;
}