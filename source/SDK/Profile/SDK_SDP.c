#include "reside_flash.inc"

#define __SDK_SDP_C__

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#if SDK_SDP_PROFILE
#include "..\SDK\include\SDK_SDP.h"
#include "..\MMI\MMI_base.h"
#include "..\MMI\MMI_SDAP.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_Protocol.h"
#include "..\SDAP\SDAP_Interface.h"	

typedef struct
{
	U8 profileSCN;
	U8 profileID;
	U32 XDATA_PTR serviceRecordHandle;
}SDK_SDP_CTL;

SDK_SDP_CTL XDATA gSDK_sdpCtl[MAX_MULTI_POINT_NO];

PUBLIC BOOL SDK_SDP_ProfileConnect(U8 linkIndex)
{
	return MMI_SDAP_UseStatusRegister(linkIndex, SDP_USE_BY_SDK);
}

PUBLIC void SDK_SDP_ProfileDisconnect(U8 linkIndex)
{
	MMI_SDAP_UseStatusDeRegister(linkIndex, SDP_USE_BY_SDK);
}

PUBLIC void SDK_SDP_ServiceHandler(U8 linkIndex)
{
	/*==================================Sample Code Start===========================================
	SDK_SDP_SendServiceSearchReq(linkIndex, SERVICE_UUID_HANDSFREE_AG);

	SDK_SDP_SendAttributeSearchReq(linkIndex, *gSDK_sdpCtl[linkIndex].serviceRecordHandle, ATTRID_ServiceRecordHandle, ATTRID_ServiceId);

	SDK_SDP_SendServiceSearchAttributeReq(linkIndex, SERVICE_UUID_HANDSFREE_AG, ATTRID_ProtocolDescriptorList);

	MMI_ConnectProfile(linkIndex, PROFILE_HANDSFREE);

	//SDK_SDP_ProfileDisconnect(linkIndex);
	/*==================================Sample Code End=============================================*/
	UNUSED(linkIndex);
}

PUBLIC void SDK_SDP_SendServiceSearchReq(U8 linkIndex, U16 uuid16)
{
	MMI_SDAP_SendServiceSearchReq(linkIndex, uuid16, SDP_USE_BY_SDK);
}

PUBLIC void SDK_SDP_SendAttributeSearchReq(U8 linkIndex, U32 serviceRecordHandle, U16 attributeId1, U16 attributeId2)
{
	MMI_SDAP_SendAttributeSearchReq(linkIndex, serviceRecordHandle, attributeId1, attributeId2, SDP_USE_BY_SDK);
}

PUBLIC void SDK_SDP_SendServiceSearchAttributeReq(U8 linkIndex, U16 uuid16, U16 attirbuteId)
{
	MMI_SDAP_SendServiceSearchAttributeReq(linkIndex, uuid16, attirbuteId, SDP_USE_BY_SDK);
}

PUBLIC void SDK_SDP_ServiceSearchCallback(U8 XDATA_PTR sdpRspPtr)
{
	U8 linkIndex = ((SdapEvent XDATA_PTR)sdpRspPtr)->eventBody.sssCompleteEvent.linkIndex;
	U8 useType = MMI_SDAP_UseStatusFindInUsed(linkIndex);

	gSDK_sdpCtl[linkIndex].serviceRecordHandle = MMI_SDAP_ParseServiceRecordHandle(sdpRspPtr);

	OSMEM_Put(sdpRspPtr);
	
	if(useType != SDP_USER_EOF)
	{
		MMI_SDAP_UseStatusUseFinish(linkIndex, useType);
		MMI_SDAP_UseStatusSetWaitCmd(linkIndex, MMI_SDAP_UseStatusFindInUsed(linkIndex));
	}
	SDK_SDP_ServiceHandler(linkIndex);
}

PUBLIC void SDK_SDP_AttributeSearchCallback(U8 XDATA_PTR sdpRspPtr)
{
	U8 linkIndex = ((SdapEvent XDATA_PTR)sdpRspPtr)->eventBody.sssCompleteEvent.linkIndex;
	U8 useType = MMI_SDAP_UseStatusFindInUsed(linkIndex);

	SDK_SDP_ServiceHandler(linkIndex);
	
	OSMEM_Put(sdpRspPtr);
	
	if(useType != SDP_USER_EOF)
	{
		MMI_SDAP_UseStatusUseFinish(linkIndex, useType);
		MMI_SDAP_UseStatusSetWaitCmd(linkIndex, MMI_SDAP_UseStatusFindInUsed(linkIndex));
	}
	SDK_SDP_ServiceHandler(linkIndex);
}

PUBLIC void SDK_SDP_ServiceSearchAttributeCallback(U8 XDATA_PTR sdpRspPtr)
{
	U8 linkIndex = ((SdapEvent XDATA_PTR)sdpRspPtr)->eventBody.sssCompleteEvent.linkIndex;
	U8 useType = MMI_SDAP_UseStatusFindInUsed(linkIndex);
	
	/*=============================Sample Code Start=============================
	U8 profileID = PROFILE_HANDSFREE;
	U8 scn = MMI_SDAP_ParseRemoteScn(sdpRspPtr, PSM_RFCOMM);
	
	if(!MMI_SDAP_SetProfileSCN(linkIndex, profileID, scn))
	{
		gSDK_sdpCtl[linkIndex].profileSCN = scn;
		gSDK_sdpCtl[linkIndex].profileID = profileID;
	}
	/*===============================Sample Code End=============================*/

	SDK_SDP_ServiceHandler(linkIndex);
	
	OSMEM_Put(sdpRspPtr);
	
	if(useType != SDP_USER_EOF)
	{
		MMI_SDAP_UseStatusUseFinish(linkIndex, useType);
		MMI_SDAP_UseStatusSetWaitCmd(linkIndex, MMI_SDAP_UseStatusFindInUsed(linkIndex));
	}
	SDK_SDP_ServiceHandler(linkIndex);
}

PUBLIC void SDK_SDP_ClearCtlData(U8 linkIndex)
{
	OSMEM_memset_xdata((U8 XDATA_PTR)&gSDK_sdpCtl[linkIndex], 0 , sizeof(SDK_SDP_CTL));
}

PUBLIC U8 SDK_SDP_GetServiceProfileSCN(U8 linkIndex, U8 profileID)
{
	if(gSDK_sdpCtl[linkIndex].profileID == profileID)
		return gSDK_sdpCtl[linkIndex].profileSCN;
	else
		return 0;
}
#endif

PUBLIC void SDK_SDP_Init(void)
{
	#if SDK_SDP_PROFILE
	SDK_Proxy_RegisterAPI(API_GRP_PF_CB_SDP_SERVICE_HANDLER, API_GRP_PROFILE, (VFUN)SDK_SDP_ServiceHandler);
	SDK_Proxy_RegisterAPI(API_GRP_PF_CB_SDP_SERVICE_SEARCH, API_GRP_PROFILE, (VFUN)SDK_SDP_ServiceSearchCallback);
	SDK_Proxy_RegisterAPI(API_GRP_PF_CB_SDP_ATTRIBUTE_SEARCH, API_GRP_PROFILE, (VFUN)SDK_SDP_AttributeSearchCallback);
	SDK_Proxy_RegisterAPI(API_GRP_PF_CB_SDP_SERVICE_SEARCH_ATTRIBUTE, API_GRP_PROFILE, (VFUN)SDK_SDP_ServiceSearchAttributeCallback);
	SDK_Proxy_RegisterAPI(API_GRP_PF_CB_SDP_CLEAR_CTL_DATA, API_GRP_PROFILE, (VFUN)SDK_SDP_ClearCtlData);
	SDK_Proxy_RegisterAPI(API_GRP_PF_CB_SDP_GET_SERVICE_PROFILESCN, API_GRP_PROFILE, (VFUN)SDK_SDP_GetServiceProfileSCN);
	#endif
}