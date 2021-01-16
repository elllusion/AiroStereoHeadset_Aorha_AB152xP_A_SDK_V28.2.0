#include "reside_flash.inc"

#define __VENDOR_PROFILE_C__

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#if VENDOR_PROFILE

#include "..\SDK\include\VendorProfile.h"
#include "..\MMI\MMI_Base.h"
#include "..\SDK\include\SDK_SCN.h"
#include "..\SDAP\SDAP_Interface.h"
#include "..\RFCOMM\RFCOMM_Interface.h"

#if 1 // Test Ringtone & Voice Prompt API
#include "..\SDK\include\RingtoneVoicePrompt.h"
#endif

typedef struct
{
	MMI_PROFILE_INFO mmiProfileInfo;
} VPROFILE_LINK_INFO;

typedef struct
{
	VPROFILE_LINK_INFO linkInfo[MAX_MULTI_POINT_NO];
} VPROFILE_CTL;

static VPROFILE_CTL XDATA gVProfile_ctl;

PRIVATE VPROFILE_LINK_INFO XDATA_PTR SDK_VProfile_GetLinkInfo(U8 linkIndex)
{
	return &gVProfile_ctl.linkInfo[linkIndex];
}

PRIVATE void SDK_VProfile(U8 linkIndex);
PRIVATE MMI_PROFILE_INFO XDATA_PTR SDK_VProfile_ConnectRequest(U8 linkIndex, U8 profileId);
PRIVATE U8 SDK_VProfile_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
PRIVATE void SDK_VProfile_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
PRIVATE void SDK_VProfile_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr);

static RFCOMM_PROFILE_DESC CODE gVProfile_ProfileDesc = {
	{ //MMI_PROFILE_DESC
		PROFILE_VENDOR_PROFILE,
		SDK_VProfile,					//ProfileTaskFunc
		SDK_VProfile_ConnectRequest, 	//ProfileConnectRequest
		SDK_VProfile_DisconnectRequest,	//ProfileDisconnectRequest
		SDK_VProfile_LinkEvent,			//ProfileLinkEvent
		SDK_VProfile_DataEvent,			//ProfileDataEvent
	},
	VENDOR_PROFILE_MAX_FRAME_SIZE,
	NULL,			//GivenCreditsEvent
};

static MMI_PROFILE_NODE XDATA gVProfile_ProfileNode = {
	&gVProfile_ProfileDesc.mmiProfileDesc,
	NULL,
};

PUBLIC BOOL SDK_VProfile_TxHandler(U8 linkIndex, U8 XDATA_PTR txPtr, U16 txLen)
{	
	U8 XDATA_PTR dataSentPtr;
	
	dataSentPtr = RFCOMM_AllocProfileData(txLen, SCN_VENDOR_PROFILE);
	if(!dataSentPtr)
		return VENDOR_PROFILE_TX_ERROR;
		
	OSMEM_memcpy_xdata_xdata ( (dataSentPtr + *dataSentPtr), txPtr, txLen);
	RFCOMM_SendProfileData(linkIndex, dataSentPtr);
	return VENDOR_PROFILE_TX_OK;
}

PRIVATE void SDK_VProfile_ClearLinkInfo(U8 linkIndex)
{
	VPROFILE_LINK_INFO XDATA_PTR vprofileLinkInfo = SDK_VProfile_GetLinkInfo(linkIndex);
	
	vprofileLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTED;
}

PRIVATE void SDK_VProfile(U8 linkIndex)
{
	UNUSED(linkIndex);
}

PRIVATE MMI_PROFILE_INFO XDATA_PTR SDK_VProfile_ConnectRequest(U8 linkIndex, U8 profileId)
{
	VPROFILE_LINK_INFO XDATA_PTR vprofileLinkInfo = SDK_VProfile_GetLinkInfo(linkIndex);
	
	if (vprofileLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
	{
		if (RFCOMM_ConnectProfile(linkIndex, profileId) == NO_ERROR)
		{
			return &vprofileLinkInfo->mmiProfileInfo;
		}
	}
	return (MMI_PROFILE_INFO XDATA_PTR)NULL;
}

PRIVATE U8 SDK_VProfile_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	VPROFILE_LINK_INFO XDATA_PTR vprofileLinkInfo = SDK_VProfile_GetLinkInfo(linkIndex);

	if (&vprofileLinkInfo->mmiProfileInfo == profileInfo)
	{
		if (vprofileLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
		{
			if (CMD_WAITING == RFCOMM_DisconnectProfile(linkIndex, SCN_VENDOR_PROFILE))
			{
				vprofileLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTING;
				return CMD_WAITING;
			}
			MMI_UnloadProfile(linkIndex, &vprofileLinkInfo->mmiProfileInfo);
			SDK_VProfile_ClearLinkInfo(linkIndex);
		}
	}
	return CMD_COMPLETE;
}

PRIVATE void SDK_VProfile_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams)
{
	VPROFILE_LINK_INFO XDATA_PTR vprofileLinkInfo = SDK_VProfile_GetLinkInfo(linkIndex);

	switch (evtCode)
	{
		case PROFILE_LINK_EVENT_CONN_REQUEST:
			evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT;
			if (vprofileLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
			{
				if(MMI_IsLinkActiveDisconnecting(linkIndex))
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT_ACTIVE_DISC;
				}
				else
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_ACCEPT;
					evtParams->connRequest.profileInfo = &vprofileLinkInfo->mmiProfileInfo;
				}
			}
			break;
			
		case PROFILE_LINK_EVENT_CONN_COMPLETE:
			if (vprofileLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTING)
			{
				if (evtParams->connComplete.status == NO_ERROR)
				{
					vprofileLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
					MMI_ActivateProfile(linkIndex, &vprofileLinkInfo->mmiProfileInfo);
				}
				else
				{
					if(evtParams->connComplete.status == SECURITY_BLOCK)
					{
						MMI_SetReconnectProfile(linkIndex, vprofileLinkInfo->mmiProfileInfo.profileDesc->profileId, ONE_SEC);
					}
					evtCode = PROFILE_LINK_EVENT_DISC_COMPLETE;
					goto SPP_DISC_CPL;
				}
			}
			break;
			
		case PROFILE_LINK_EVENT_DISC_COMPLETE:
		case PROFILE_LINK_EVENT_DISC_LINKLOSS:
			if (vprofileLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
			{
				SPP_DISC_CPL:
				MMI_UnloadProfile(linkIndex, &vprofileLinkInfo->mmiProfileInfo);
				MMI_PROTOCOL_L2CAPDisconnected(linkIndex, evtCode, FALSE);
				SDK_VProfile_ClearLinkInfo(linkIndex);
			}
			break;
	}
}

PRIVATE void SDK_VProfile_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	if (SDK_VProfile_GetLinkInfo(linkIndex)->mmiProfileInfo.state == PROFILE_CONNECTED)
	{
        #if 1 // Test Ringtone & Voice Prompt API
		U8 TestRighttoneAndVP; // Demo the usage of the ringtone and voice prompt API.
        #endif
        
        #if 1 // demo by loopback
		U8 XDATA_PTR loopBackPtr;
		LightDbgPrint("SDK_VProfile_DataEvent");
		loopBackPtr = RFCOMM_AllocProfileData(((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength, SCN_VENDOR_PROFILE);
		if(loopBackPtr)
		{
			OSMEM_memcpy_xdata_xdata ( ( loopBackPtr + *loopBackPtr ),
										&((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody[0], ((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength);
			RFCOMM_SendProfileData(linkIndex, loopBackPtr);
		}
        #endif // demo by loopback
		
        #if 1 // Test Ringtone & Voice Prompt API
		TestRighttoneAndVP=((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody[0];
		if(TestRighttoneAndVP=='0'){
			SDK_DRV_SendRingtoneCmd(MEDIA_EVT_KEY_VOICEUP, 0);
		}
		else if(TestRighttoneAndVP=='1'){
			SDK_DRV_SendRingtoneCmd(MEDIA_EVT_KEY_VOICEUP, 1);
		}
		else if(TestRighttoneAndVP=='2'){
			SDK_DRV_SendRingtoneCmd(MEDIA_EVT_KEY_VOICEUP, 2);
		}
		else if(TestRighttoneAndVP=='A'){
			SDK_DRV_SendRingtoneCmd(MEDIA_EVT_KEY_VOICEUP, 17); // Check with Config Tool, we have 18 different ringtones, and we test the last one.
		}
		else if(TestRighttoneAndVP=='B'){
			SDK_DRV_SendRingtoneCmd(MEDIA_EVT_KEY_VOICEUP, 17); // Check with Config Tool, we have 18 different ringtones, and we test the last one.
			SDK_DRV_SendRingtoneStopCmd(); // Just stop.
		}
		else if(TestRighttoneAndVP=='C'){
			SDK_DRV_SendRingtoneCmd(MEDIA_EVT_KEY_VOICEUP, 17);
			SDK_DRV_SpecificEventRingtoneStopCmd(MEDIA_EVT_KEY_VOICEUP); // The event ID is the same, and we can stop it.
		}
		else if(TestRighttoneAndVP=='D'){
			SDK_DRV_SendRingtoneCmd(MEDIA_EVT_KEY_VOICEUP, 17);
			SDK_DRV_SpecificEventRingtoneStopCmd(MEDIA_EVT_KEY_VOICEDN); // The event ID is different, and we cannot stop it.
		}
		else if(TestRighttoneAndVP=='E'){
			SDK_DRV_SendVoicePromptCmd(MEDIA_EVT_KEY_VOICE_PROMPT_ENABLE, 1);
		}
		else if(TestRighttoneAndVP=='I'){
			SDK_DRV_SendVoicePromptCmd(MEDIA_EVT_KEY_TEST_VOICE_PROMPT_PLAY, 1);
		}
		else if(TestRighttoneAndVP=='F'){
			SDK_DRV_SendVoicePromptCmd(MEDIA_EVT_KEY_VOICE_PROMPT_ENABLE, 1);
			SDK_DRV_SendVoicepromptStopCmd();
		}
		else if(TestRighttoneAndVP=='G'){
			SDK_DRV_SendVoicePromptCmd(MEDIA_EVT_KEY_VOICE_PROMPT_ENABLE, 1);
			SDK_DRV_SpecificEventVPStopCmd(MEDIA_EVT_KEY_VOICE_PROMPT_ENABLE);
		}
		else if(TestRighttoneAndVP=='H'){
			SDK_DRV_SendVoicePromptCmd(MEDIA_EVT_KEY_VOICE_PROMPT_ENABLE, 1);
			SDK_DRV_SpecificEventVPStopCmd(MEDIA_EVT_KEY_VOICE_PROMPT_DISABLE); // The event ID is different, and this stop command has no effect.
		}
        #endif

		OSMEM_Put(dataPtr);
	}
	else
	{
		OSMEM_Put(dataPtr);
	}
}

/*
This function is used to initialize the customized vendor profile.
*/
PUBLIC void SDK_VProfile_Init(void)
{
	extern SdpServiceNode gVProfile_ServiceNode;

    /*
	By calling SDAP_RegisterServiceRecord(), we can regiser this customized vendor profile's SDAP records into the SDAP database.
	The definition of "gVProfile_ServiceNode" is in "VendorProfile_ServiceRecord.c", and we can find the customized vendor profile's UUID also in that file. 
	*/
	SDAP_RegisterServiceRecord(&gVProfile_ServiceNode);
	/*
	By calling RFCOMM_RegisterProfile(), we can register callback functions and let RFCOMM call these callback functions when different events occur.
	The definition of "gVProfile_ProfileDesc" is in this file, and it has a function table of these callback functions (eg. SDK_VProfile, SDK_VProfile_LinkEvent...).
	*/
	RFCOMM_RegisterProfile(&gVProfile_ProfileNode);
}
#endif
