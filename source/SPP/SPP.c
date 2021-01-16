#include "reside_flash.inc"

#define __SPP_C__

#include "rc.h"
#include "bt_config_profile.h"

#ifdef SPP_Profile

#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_SCN.h"
#include "..\SDAP\SDAP_Interface.h"
#include "..\RFCOMM\RFCOMM_Interface.h"
#include "SPP_interface.h"

typedef struct
{
	MMI_PROFILE_INFO mmiProfileInfo;
} SPP_LINK_INFO;

typedef struct
{
	SPP_LINK_INFO linkInfo[MAX_MULTI_POINT_NO];
} SPP_CTL;

static SPP_CTL XDATA gSPP_ctl;

PRIVATE SPP_LINK_INFO XDATA_PTR SPP_GetLinkInfo(U8 linkIndex)
{
	return &gSPP_ctl.linkInfo[linkIndex];
}

PRIVATE void SPP(U8 linkIndex);
PRIVATE MMI_PROFILE_INFO XDATA_PTR SPP_ConnectRequest(U8 linkIndex, U8 profileId);
PRIVATE U8 SPP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
PRIVATE void SPP_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
PRIVATE void SPP_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr);

static RFCOMM_PROFILE_DESC CODE gSPP_ProfileDesc = {
	{ //MMI_PROFILE_DESC
		PROFILE_SERIAL_PORT,
		SPP,					//ProfileTaskFunc
		SPP_ConnectRequest, 	//ProfileConnectRequest
		SPP_DisconnectRequest,	//ProfileDisconnectRequest
		SPP_LinkEvent,			//ProfileLinkEvent
		SPP_DataEvent,			//ProfileDataEvent
	},
	SPP_MAX_FRAME_SIZE,
	NULL,			//GivenCreditsEvent
};

static MMI_PROFILE_NODE XDATA gSPP_ProfileNode = {
	&gSPP_ProfileDesc.mmiProfileDesc,
	NULL,
};

PUBLIC void SPP_MMICommandHandler(U8 linkIndex)
{	
	MMIMsgType XDATA_PTR msgPtr;
	U8 XDATA_PTR dataSentPtr;
	OSMQ XDATA_PTR mqPtr;

	if(msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get( mqPtr = OSMQ_SPP_Command_From_MMI_ptr[linkIndex] ))
	{
		switch(msgPtr->msgOpcode)
		{
			case SPP_CMD_FROM_MMI_DATA:
				dataSentPtr = RFCOMM_AllocProfileData(msgPtr->paraLen, SCN_SERIAL_PORT);
		#ifdef SONGPAL_Profile
				goto label_check_dataSentPtr;
			case SPP_CMD_FROM_MMI_DATA + SONGPAL_ANDROID_SPP_ENCODING_OFFSET:
				dataSentPtr = RFCOMM_AllocProfileData(msgPtr->paraLen, SCN_SONGPAL_ANDROID);
				goto label_check_dataSentPtr;
			case SPP_CMD_FROM_MMI_DATA + SONGPAL_IAP2_SPP_ENCODING_OFFSET:
				dataSentPtr = RFCOMM_AllocProfileData(msgPtr->paraLen, SCN_SONGPAL_IAP2);
				label_check_dataSentPtr:
		#endif
				if(dataSentPtr == (U8 XDATA_PTR)NULL)
				{
					OSMQ_PutFront(mqPtr, (U8 XDATA_PTR)msgPtr);
					return;
				}
				MMI_SniffDisable(linkIndex, MMI_SNIFF_SPP);
				OSMEM_memcpy_xdata_xdata ( (dataSentPtr + *dataSentPtr), &msgPtr->msgBodyPtr.hciUARTSppData.dataBegin, msgPtr->paraLen);
				RFCOMM_SendProfileData(linkIndex, dataSentPtr);
				MMI_SniffEnable(linkIndex, MMI_SNIFF_SPP);
				break;
		}
		OSMEM_Put((U8 XDATA_PTR)msgPtr);
	}	
}

PUBLIC void SPP_FlushMMICommandQueue(U8 linkIndex, U8 opCode)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 entries = OSMQ_Entries(OSMQ_SPP_Command_From_MMI_ptr[linkIndex]);
	
	while(entries--)
	{
		msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(OSMQ_SPP_Command_From_MMI_ptr[linkIndex]);
		if(msgPtr->msgOpcode == opCode)
		{
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		}
		else
		{
			OSMQ_Put(OSMQ_SPP_Command_From_MMI_ptr[linkIndex], (U8 XDATA_PTR)msgPtr);
		}
	}
}

PRIVATE void SPP_ClearLinkInfo(U8 linkIndex)
{
	SPP_LINK_INFO XDATA_PTR sppLinkInfo = SPP_GetLinkInfo(linkIndex);
	
	SPP_FlushMMICommandQueue(linkIndex, SPP_CMD_FROM_MMI_DATA);
	sppLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTED;
}

PRIVATE void SPP(U8 linkIndex)
{
	SPP_MMICommandHandler(linkIndex);
}

PRIVATE MMI_PROFILE_INFO XDATA_PTR SPP_ConnectRequest(U8 linkIndex, U8 profileId)
{
	SPP_LINK_INFO XDATA_PTR sppLinkInfo = SPP_GetLinkInfo(linkIndex);
	
	if (sppLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
	{
		if (RFCOMM_ConnectProfile(linkIndex, profileId) == NO_ERROR)
		{
			return &sppLinkInfo->mmiProfileInfo;
		}
	}
	return (MMI_PROFILE_INFO XDATA_PTR)NULL;
}

PRIVATE U8 SPP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	SPP_LINK_INFO XDATA_PTR sppLinkInfo = SPP_GetLinkInfo(linkIndex);

	if (&sppLinkInfo->mmiProfileInfo == profileInfo)
	{
		if (sppLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
		{
			if (CMD_WAITING == RFCOMM_DisconnectProfile(linkIndex, PROFILE_SERIAL_PORT))
			{
				sppLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTING;
				return CMD_WAITING;
			}
			if (sppLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
			{
				#ifndef TRSPX_App
				MMI_HCI_SendSPPEventToUART(linkIndex, SPP_TO_MMI_OPCODE_DISCONNECTED);
				#endif
			}
			MMI_UnloadProfile(linkIndex, &sppLinkInfo->mmiProfileInfo);
			SPP_ClearLinkInfo(linkIndex);
			MMI_SniffEnable(linkIndex, MMI_SNIFF_SPP);
		}
	}
	return CMD_COMPLETE;
}

PRIVATE void SPP_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams)
{
	SPP_LINK_INFO XDATA_PTR sppLinkInfo = SPP_GetLinkInfo(linkIndex);

	switch (evtCode)
	{
		case PROFILE_LINK_EVENT_CONN_REQUEST:
			evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT;
			if (sppLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
			{
				if(MMI_IsLinkActiveDisconnecting(linkIndex))
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT_ACTIVE_DISC;
				}
				else
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_ACCEPT;
					evtParams->connRequest.profileInfo = &sppLinkInfo->mmiProfileInfo;
				}
			}
			break;
			
		case PROFILE_LINK_EVENT_CONN_COMPLETE:
			if (sppLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTING)
			{
				if (evtParams->connComplete.status == NO_ERROR)
				{
					sppLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
					MMI_ActivateProfile(linkIndex, &sppLinkInfo->mmiProfileInfo);
					#ifndef TRSPX_App
					MMI_HCI_SendSPPEventToUART(linkIndex, SPP_TO_MMI_OPCODE_CONNECTED);
					#endif
				}
				else
				{
					if(evtParams->connComplete.status == SECURITY_BLOCK)
					{
						MMI_SetReconnectProfile(linkIndex, sppLinkInfo->mmiProfileInfo.profileDesc->profileId, ONE_SEC);
					}
					evtCode = PROFILE_LINK_EVENT_DISC_COMPLETE;
					goto SPP_DISC_CPL;
				}
			}
			break;
			
		case PROFILE_LINK_EVENT_DISC_COMPLETE:
		case PROFILE_LINK_EVENT_DISC_LINKLOSS:
			if (sppLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
			{
				SPP_DISC_CPL:
				if (sppLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
				{
					#ifndef TRSPX_App
					MMI_HCI_SendSPPEventToUART(linkIndex, SPP_TO_MMI_OPCODE_DISCONNECTED);
					#endif
				}
				MMI_UnloadProfile(linkIndex, &sppLinkInfo->mmiProfileInfo);
				MMI_PROTOCOL_L2CAPDisconnected(linkIndex, evtCode, FALSE);
				SPP_ClearLinkInfo(linkIndex);
				MMI_SniffEnable(linkIndex, MMI_SNIFF_SPP);
			}
			break;
	}
}

PRIVATE void SPP_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	if (SPP_GetLinkInfo(linkIndex)->mmiProfileInfo.state == PROFILE_CONNECTED)
	{
		/* // loopBack test
		loopBackPtr = RFCOMM_AllocProfileData(((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength, SCN_SERIAL_PORT);
		if(loopBackPtr)
		{
			OSMEM_memcpy_xdata_xdata ( ( loopBackPtr + *loopBackPtr ),
										&((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody[0], ((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength);
			RFCOMM_SendProfileData(linkIndex, loopBackPtr);
		}
		*/
		MMI_HCI_SendSPPDataToUART(linkIndex, dataPtr, SPP_TO_MMI_OPCODE_DATA);
	}
	else
	{
		OSMEM_Put(dataPtr);
	}
}

PUBLIC void SPP_Init(void)
{
	extern SdpServiceNode gSPP_ServiceNode;
	
	SDAP_RegisterServiceRecord(&gSPP_ServiceNode);
	RFCOMM_RegisterProfile(&gSPP_ProfileNode);
}

#endif //SPP_Profile