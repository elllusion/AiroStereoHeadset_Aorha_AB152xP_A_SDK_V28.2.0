//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _HFP_C_


#include "os.h"
#include "bt_config_profile.h"

#ifdef HFP_Profile
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_Protocol.h"
#include "..\MMI\MMI_HFP.h"
#include "..\MMI\MMI_SCN.h"
#include "..\SDAP\SDAP_Interface.h"
#include "..\RFCOMM\RFCOMM_Interface.h"
#include "..\HC\HC_interface.h"

#include "HFP_interface.h"
#include "HFP.h"

typedef struct
{	
	HfpLinkInfo linkInfo[MAX_MULTI_POINT_NO];
} HFP_CTL_TYPE;

PRIVATE void HFP(U8 linkIndex);
PRIVATE MMI_PROFILE_INFO XDATA_PTR HFP_ConnectRequest(U8 linkIndex, U8 profileId);
PUBLIC U8 HFP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
PRIVATE void HFP_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
PRIVATE void HFP_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr);


static HFP_CTL_TYPE XDATA gHFP_ctl;

static RFCOMM_PROFILE_DESC CODE gHFP_ProfileDesc = {
	{ //MMI_PROFILE_DESC
		PROFILE_HANDSFREE,
		HFP,					//ProfileTaskFunc
		HFP_ConnectRequest,		//ProfileConnectRequest
		HFP_DisconnectRequest,	//ProfileDisconnectRequest
		HFP_LinkEvent,			//ProfileLinkEvent
		HFP_DataEvent,			//ProfileDataEvent
	},
	HFP_MAX_FRAME_SIZE,
	NULL,			//GivenCreditsEvent
};

static MMI_PROFILE_NODE XDATA gHFP_ProfileNode = {
	&gHFP_ProfileDesc.mmiProfileDesc,
	NULL,
};

static RFCOMM_PROFILE_DESC CODE gHSP_ProfileDesc = {
	{ //MMI_PROFILE_DESC
		PROFILE_HEADSET,
		HFP,					//ProfileTaskFunc
		HFP_ConnectRequest,		//ProfileConnectRequest
		HFP_DisconnectRequest,	//ProfileDisconnectRequest
		HFP_LinkEvent,			//ProfileLinkEvent
		HFP_DataEvent,			//ProfileDataEvent
	},
	HFP_MAX_FRAME_SIZE,
	NULL,			//GivenCreditsEvent
};

static MMI_PROFILE_NODE XDATA gHSP_ProfileNode = {
	&gHSP_ProfileDesc.mmiProfileDesc,
	NULL,
};

PUBLIC HfpLinkInfo XDATA_PTR HFP_GetLinkInfo(U8 linkIndex)
{
	return &gHFP_ctl.linkInfo[linkIndex];
}

PUBLIC void HFP_CommandHandle(U8 linkIndex, U8 cmdCode, MMIMsgType XDATA_PTR cmdPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);

	if(	cmdCode == hfpLinkInfo->currentCmd && 
		((cmdCode >= HFP_ATCMD_CANCEL_VOICE_DIAL && cmdCode <= HFP_ATCMD_ACCEPT_CALL) || cmdCode == HFP_ATCMD_CKPD_200) )
	{
		OSMEM_Put((U8 XDATA_PTR)cmdPtr);
	}
	else
	{
		if(cmdCode < HFP_TABLE_LOOKUP_ATCMD_COUNT &&
			cmdCode != HFP_ATCMD_IPHONE_BATTERY_SEND && cmdCode != HFP_ATCMD_XEVENT_BATTERY_SEND && cmdCode != HFP_ATCMD_BIEV_BATTERY_SEND)
		{
			MMI_SniffDisable(linkIndex, MMI_SNIFF_HFP);
		}
		OSMQ_Put(&hfpLinkInfo->OSMQ_HFP_Command, (U8 XDATA_PTR)cmdPtr);
	}				
}

PUBLIC void HFP_CommandComplete (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	
	SYS_ReleaseTimer(&hfpLinkInfo->ost_command_ptr);

	hfpLinkInfo->currentCmd = HFP_NO_COMMAND;
	hfpLinkInfo->subState = HFP_SUBSTATE_NO_SUBSTATE;
	
	MMI_SniffEnable(linkIndex, MMI_SNIFF_HFP);
}

PRIVATE void HFP_ClearLinkInfo(HfpLinkInfo XDATA_PTR hfpLinkInfo)
{
	SYS_ReleaseTimer(&hfpLinkInfo->ost_command_ptr);
	OSMQ_FlushAndReleaseMemoryBlock(&hfpLinkInfo->OSMQ_HFP_Command);
	SYS_MemoryRelease(&hfpLinkInfo->pushedCmdCtlPtr);
	OSMEM_memset_xdata((U8 XDATA_PTR)hfpLinkInfo, 0, sizeof(HfpLinkInfo));
}

PRIVATE MMI_PROFILE_INFO XDATA_PTR HFP_ConnectRequest(U8 linkIndex, U8 profileId)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	
	if (hfpLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
	{
		if (RFCOMM_ConnectProfile(linkIndex, profileId) == NO_ERROR)
		{
			MMI_SniffDisable(linkIndex, MMI_SNIFF_HFP);
			return &hfpLinkInfo->mmiProfileInfo;
		}
	}
	return (MMI_PROFILE_INFO XDATA_PTR)NULL;
}


PUBLIC U8 HFP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	OSMQ_FlushAndReleaseMemoryBlock(&hfpLinkInfo->OSMQ_HFP_Command);
	HFP_CommandComplete(linkIndex);
	
	if (hfpLinkInfo->mmiProfileInfo.state >= PROFILE_CONNECTING && &hfpLinkInfo->mmiProfileInfo == profileInfo)
	{
		MMI_SniffDisable(linkIndex, MMI_SNIFF_HFP);
		
		if(CMD_WAITING == RFCOMM_DisconnectProfile(linkIndex, hfpLinkInfo->mmiProfileInfo.profileDesc->profileId))
		{
			hfpLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTING;
			return CMD_WAITING;
		}
		MMI_HFP_ProfileDiscHandle(linkIndex);
		MMI_UnloadProfile(linkIndex, &hfpLinkInfo->mmiProfileInfo);
		HFP_ClearLinkInfo(HFP_GetLinkInfo(linkIndex));
		MMI_SniffEnable(linkIndex, MMI_SNIFF_HFP);
	}
	return CMD_COMPLETE;
}

///////////////////////////////////////////////////////////

PRIVATE void HFP (U8 linkIndex)
{	
	//Handle no-connection cmd/rsp
	HFP_TimeoutMonitor(linkIndex);

	if (HFP_GetLinkInfo(linkIndex)->mmiProfileInfo.state >= PROFILE_CONNECTED)
	{
		//Main state machine, the most important process in the function is SLC ESTABLISHMENT.
		HFP_MainStateEntry(linkIndex);

		//Handle command from MMI
		HFP_ProcessCmdFromMMI(linkIndex);			
	}
}

PRIVATE void HFP_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);

	switch (evtCode)
	{
		case PROFILE_LINK_EVENT_CONN_REQUEST:
			evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT;
			if (hfpLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
			{
				if(MMI_IsLinkActiveDisconnecting(linkIndex))
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT_ACTIVE_DISC;
				}
				else
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_ACCEPT;
					evtParams->connRequest.profileInfo = &hfpLinkInfo->mmiProfileInfo;
				}
			}
			break;
		case PROFILE_LINK_EVENT_CONN_COMPLETE:
			if (hfpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTING)
			{
				if(evtParams->connComplete.status == NO_ERROR)
				{
					//Disable PowerSaving until SLC Routine is done.
					MMI_SniffDisable(linkIndex, MMI_SNIFF_HFP);
					hfpLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
					if (hfpLinkInfo->mmiProfileInfo.profileDesc->profileId == PROFILE_HANDSFREE)
					{
						//Start SLC level connection
						hfpLinkInfo->currentCmd = HFP_CMD_INTERNAL_SLC_INIT_ROUTINE;
						#ifdef HFP_AG_Profile
						hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_BRSF_CMD;
						#else
						hfpLinkInfo->subState = HFP_SUBSTATE_SEND_BRSF_CMD;
						#endif
					}
					else
					{
						hfpLinkInfo->currentCmd = HFP_CMD_INTERNAL_SLC_INIT_ROUTINE_HEADSET;
						#ifdef HFP_AG_Profile
						hfpLinkInfo->subState = (evtParams->connComplete.isPassive) ? HSP_SUBSTATE_WAIT_AT_CKPD200 : HSP_SUBSTATE_SEND_RING;
						#else
						hfpLinkInfo->subState = (evtParams->connComplete.isPassive) ? HSP_SUBSTATE_SLC_PROCESS_COMPLETE : HSP_SUBSTATE_SEND_CKPD200;
						#endif
					}
				}
				else
				{
					if(evtParams->connComplete.status == SECURITY_BLOCK)
					{
						MMI_SetReconnectProfile(linkIndex, hfpLinkInfo->mmiProfileInfo.profileDesc->profileId, ONE_SEC);						
					}
					evtCode = PROFILE_LINK_EVENT_DISC_COMPLETE;
					goto HFP_DISC_CPL;
				}
			}
			break;

		case PROFILE_LINK_EVENT_DISC_COMPLETE:
		case PROFILE_LINK_EVENT_DISC_LINKLOSS:
			if (hfpLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
			{
				HFP_DISC_CPL:
				MMI_HFP_ProfileDiscHandle(linkIndex);
				MMI_UnloadProfile(linkIndex, &hfpLinkInfo->mmiProfileInfo);
				MMI_PROTOCOL_L2CAPDisconnected(linkIndex, evtCode, FALSE);
				HFP_ClearLinkInfo(HFP_GetLinkInfo(linkIndex));
				MMI_SniffEnable(linkIndex, MMI_SNIFF_HFP);
			}
			break;
	}
}

PRIVATE void HFP_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	AT_SegmentAndEliminateSpace(linkIndex, dataPtr);

	OSMEM_Put(dataPtr);
}

PUBLIC U8 HFP_GetState(U8 linkIndex)
{
	return (HFP_GetLinkInfo(linkIndex))->mmiProfileInfo.state;
}

PUBLIC U8 HFP_GetProfileId(U8 linkIndex)
{
	return (HFP_GetLinkInfo(linkIndex))->mmiProfileInfo.profileDesc->profileId;
}

PUBLIC void HFP_Init (void)
{
	extern SdpServiceNode gHSP_ServiceNode;
	extern SdpServiceNode gHFP_ServiceNode;
	U8 GENERIC_PTR attributePtr;
	
	SDAP_RegisterServiceRecord(&gHSP_ServiceNode);
	SDAP_RegisterServiceRecord(&gHFP_ServiceNode);

	if (attributePtr = (U8 GENERIC_PTR)SDAP_GetServiceAttribute(PROFILE_HANDSFREE, ATTRID_SupportedFeatures))
	{
		U16 supportedFeatures = gMMI_nvram.mmiFeatureCtl.HFP & 0x001F;
		if (MMI_CODEC_NEGOTIATION_FEAT)
		{
			supportedFeatures |= 0x0020;
		}
		*(U16 GENERIC_PTR)&attributePtr[4] = supportedFeatures;
	}

	RFCOMM_RegisterProfile(&gHSP_ProfileNode);
	RFCOMM_RegisterProfile(&gHFP_ProfileNode);
}

#endif // HFP_Profile
