//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"


#define _AVRCP_C_

#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_AVRCP.h"
#include "..\MMI\MMI_SCN.h"
#include "..\MMI\MMI_SDAP.h"
#include "..\SDAP\SDAP_Interface.h"
#ifdef LIGHTING_ENABLE
#include "..\MMI\MMI_AirModule.h"
#endif

#include "AVRCP_interface.h"
#include "AVRCP.h"
#include "..\AVCTP\AVCTP_Interface.h"
#include "..\HC\HC_interface.h"

#ifdef AVRCP_Profile

//PRIVATE void AVRCP(U8 linkIndex);
PRIVATE MMI_PROFILE_INFO XDATA_PTR AVRCP_ConnectRequest(U8 linkIndex, U8 profileId);
PRIVATE U8 AVRCP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
PRIVATE void AVRCP_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
PRIVATE void AVRCP_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr);

static AVRCP_CTL_TYPE XDATA gAVRCP_ctl;

static AVCTP_PROFILE_DESC CODE gAVRCP_ProfileDesc = {
	{ //MMI_PROFILE_DESC
		PROFILE_AVRCP,
		NULL/*AVRCP*/,				//ProfileTaskFunc
		AVRCP_ConnectRequest,		//ProfileConnectRequest
		AVRCP_DisconnectRequest,	//ProfileDisconnectRequest
		AVRCP_LinkEvent, 			//ProfileLinkEvent
		AVRCP_DataEvent, 			//ProfileDataEvent
	},
	AVRCP_PID,
};

static MMI_PROFILE_NODE XDATA gAVRCP_ProfileNode = {
	&gAVRCP_ProfileDesc.mmiProfileDesc,
	NULL,
};


PRIVATE AVRCP_LINK_INFO XDATA_PTR AVRCP_GetLinkInfo(U8 linkIndex)
{
	return &gAVRCP_ctl.linkInfo[linkIndex];
}

PRIVATE void AVRCP_ClearLinkInfo(AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo)
{
	OSMEM_memset_xdata ((U8 XDATA_PTR)avrcpLinkInfo, 0, sizeof(AVRCP_LINK_INFO));
}

PRIVATE void AVRCP_SendEvtToMMI(U8 linkIndex, U8 evtCode)
{
	MMI_AVRCP_EventHandler(linkIndex, evtCode, (AVRCP_TO_MMI_EVT_PARAM XDATA_PTR)NULL);
}

PRIVATE void AVRCP_SendEvtToMMI_GetAppSetting(U8 linkIndex, U8 attributeID, U8 attributeValue)
{
	AVRCP_TO_MMI_EVT_PARAM LDATA param;
	param.GetPlayAppSettings.attributeID = attributeID;
	param.GetPlayAppSettings.attributeValue = attributeValue;
	MMI_AVRCP_EventHandler(linkIndex, AVRCP_EVT_TO_MMI_GET_APPLICATION_SETTINGS, &param);
}

PRIVATE void AVRCP_SendEvtToMMI_PlayStatus(U8 linkIndex, U8 evtCode, U8 status)
{
	AVRCP_TO_MMI_EVT_PARAM LDATA param;
	param.PlayStatus = status;
	MMI_AVRCP_EventHandler(linkIndex, evtCode, &param);
}

PRIVATE void AVRCP_SendEvtToMMI_SetAbsoluteVolume(U8 linkIndex, U8 absoluteVolume)
{
	AVRCP_TO_MMI_EVT_PARAM LDATA param;
	param.AbsoluteVolume = absoluteVolume;
	MMI_AVRCP_EventHandler(linkIndex, AVRCP_EVT_TO_MMI_SET_ABSOLUTE_VOLUME, &param);	
}

PRIVATE void AVRCP_SendEvtToMMI_VendorUnique(U8 linkIndex, U8 operationId, U8 operationParam)
{
	AVRCP_TO_MMI_EVT_PARAM LDATA param;	 
	param.VendorUnique.operationId = operationId;
	param.VendorUnique.operationParam = operationParam;
	
	MMI_AVRCP_EventHandler(linkIndex, AVRCP_EVT_TO_MMI_VENDOR_UNIQUE, &param);	
}
#ifdef LIGHTING_ENABLE
PRIVATE void AVRCP_SendEvtToMMI_VendorUniqueExtern(U8 linkIndex, U8 operationId, 
										U8 operationParam, U32 syncClk, U8 XDATA_PTR ledPara)
{
	AVRCP_TO_MMI_EVT_PARAM LDATA param;	 
	param.VendorUniqueExtendLighting.operationId = operationId;
	param.VendorUniqueExtendLighting.operationParam = operationParam;
	param.VendorUniqueExtendLighting.syncClk = syncClk;
	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&(param.VendorUniqueExtendLighting.ledPara[0]), ledPara, 3);
	MMI_AVRCP_EventHandler(linkIndex, AVRCP_EVT_TO_MMI_VENDOR_UNIQUE, &param);	
}
#endif
PRIVATE U8 XDATA_PTR AVRCP_GetAVCTPMessageBuffer(U8 dataLength)
{
	return AVCTP_GetMessageBuffer(dataLength, AVRCP_PID);
}

PRIVATE U8 AVRCP_GetNextTransactionLabel(U8 linkIndex)
{
	return ((++AVRCP_GetLinkInfo(linkIndex)->nextTransactionLabel) & (AVRCP_MAX_CMD_LABEL_COUNT-1));
}

PRIVATE void AVRCP_SendPassThrough(U8 linkIndex, U8 operationId, U8 buttonState)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_pass_through_operation)))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_pass_through_operation, sizeof(avrcp_pass_through_operation));
		cmdptr[15+3] = (buttonState << 7) | operationId;

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SendGetCapabilities(U8 linkIndex, U8 CapabilityId)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_get_capabilities)))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_get_capabilities, sizeof(avrcp_get_capabilities));
		cmdptr[15+10] = CapabilityId;

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SendSetPlayerApplicationSettingValue(U8 linkIndex, U8 AttributeId, U8 AttributeValue)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_get_player_application_setting_value)+2))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_get_player_application_setting_value, sizeof(avrcp_get_player_application_setting_value));
		cmdptr[15+6] = AVRCP_PDU_ID_SET_PALYER_APPLICATION;
		cmdptr[15+9] = 0x03;
		cmdptr[15+11]= AttributeId;
		cmdptr[15+12]= AttributeValue;

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SendGetPlayerApplicationSettingValue(U8 linkIndex, U8 AttributeId)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_get_player_application_setting_value)+1))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_get_player_application_setting_value, sizeof(avrcp_get_player_application_setting_value));
		cmdptr[15+11] = AttributeId;

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SendGetElementAttributes(U8 linkIndex, U8 AttributeId)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_get_element_attributes)))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_get_element_attributes, sizeof(avrcp_get_element_attributes));
		cmdptr[15+22] = AttributeId;

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SendGetPlayStatus(U8 linkIndex)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_get_play_status)))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_get_play_status, sizeof(avrcp_get_play_status));

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SendVolumeChangedNotify(U8 linkIndex, U8 absoluteVolume, U8 transaction)
{
	U8 XDATA_PTR resPtr;

	if (resPtr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_volume_changed_notify)))
	{
		OSMEM_memcpy_xdata_code(&resPtr[15], avrcp_volume_changed_notify, sizeof(avrcp_volume_changed_notify));
		resPtr[15+11] = absoluteVolume;

		AVCTP_SendMessageRsp(linkIndex, resPtr, transaction);
	}
}

PRIVATE void AVRCP_SendRegisterNotification(U8 linkIndex, U8 eventId, U8 transaction)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_register_notification_cmd)))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_register_notification_cmd, sizeof(avrcp_register_notification_cmd));
		cmdptr[15+10] = eventId;

		if (eventId == AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED)
		{
			cmdptr[15+14] = gMMI_nvram.mmiAvrcpSettings.playbackPosInterval;
		}

		AVCTP_SendMessageCmd(linkIndex, cmdptr, transaction);
	}
}

PRIVATE void AVRCP_SendContinueResponse(U8 linkIndex, U8 pduId, U8 conti)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_continue_response)))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_continue_response, sizeof(avrcp_continue_response));
		cmdptr[15+6] = conti;
		cmdptr[15+10] = pduId;

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SendSetAbsoluteVolume(U8 linkIndex, U8 absoluteVolume)
{
	U8 XDATA_PTR cmdptr;

	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_set_absolute_volume)))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_set_absolute_volume, sizeof(avrcp_set_absolute_volume));
		cmdptr[15+10] = absoluteVolume;

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SendVendorUnique(U8 linkIndex, U8 operationId, U8 operationParam)
{
	U8 XDATA_PTR cmdptr;

	#ifdef LIGHTING_ENABLE
	if(operationId == AVRCP_VENDOR_UNIQ_LIGHT_MODE_CONTROL)
	{
		if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_vendor_unique_operation_extend)))
		{
			OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_vendor_unique_operation_extend, sizeof(avrcp_vendor_unique_operation_extend));
			cmdptr[15+8] = operationId;
			cmdptr[15+9] = gMMI_Air_ctl.ledMode; //LED mode
			OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&cmdptr[15+10],  (U8 XDATA_PTR)(&gMMI_Air_ctl.ledSyncClkVal), 4);
			//Update Led parameter
			OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&cmdptr[15+14],  (U8 XDATA_PTR)(&gMMI_Air_ctl.ledPara[0]), 3);
			AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
		}
	}
	else
	#endif
	if (cmdptr = AVRCP_GetAVCTPMessageBuffer(sizeof(avrcp_vendor_unique_operation)))
	{
		OSMEM_memcpy_xdata_code(&cmdptr[15], avrcp_vendor_unique_operation, sizeof(avrcp_vendor_unique_operation));
		cmdptr[15+8] = operationId;
		cmdptr[15+9] = operationParam;

		AVCTP_SendMessageCmd(linkIndex, cmdptr, AVRCP_GetNextTransactionLabel(linkIndex));
	}
}

PRIVATE void AVRCP_SetNotifyMask(U8 linkIndex, U8 notifyEventMask)
{
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);
	U8 addEvt = ~avrcpLinkInfo->avrcpNotifyMask & (avrcpLinkInfo->avrcpCapabilities & notifyEventMask);
	
	avrcpLinkInfo->avrcpNotifyMask = notifyEventMask;
	
	if (addEvt & MMI_AVRCP_PLAYBACK_STATUS_CHANGED)
	{
		AVRCP_SendRegisterNotification(linkIndex, AVRCP_EVENT_ID_PLAYBACK_STATUS_CHANGED, AVRCP_LABEL_PLAYBACK_STATUS_CHANGED);
	}

	if (addEvt & MMI_AVRCP_TRACK_CHANGED)
	{
		AVRCP_SendRegisterNotification(linkIndex, AVRCP_EVENT_ID_TRACK_CHANGED, AVRCP_LABEL_TRACK_CHANGED);
	}

	if (addEvt & MMI_AVRCP_TRACK_REACHED_END)
	{
		AVRCP_SendRegisterNotification(linkIndex, AVRCP_EVENT_ID_TRACK_REACHED_END, AVRCP_LABEL_TRACK_REACHED_END);
	}

	if (addEvt & MMI_AVRCP_TRACK_REACHED_START)
	{
		AVRCP_SendRegisterNotification(linkIndex, AVRCP_EVENT_ID_TRACK_REACHED_START, AVRCP_LABEL_TRACK_REACHED_START);
	}

	if (addEvt & MMI_AVRCP_PLAYBACK_POS_CHANGED)
	{
		AVRCP_SendRegisterNotification(linkIndex, AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED, AVRCP_LABEL_PLAYBACK_POS_CHANGED);
	}

	if (addEvt & MMI_AVRCP_VOLUME_CHANGED)
	{
		AVRCP_SendRegisterNotification(linkIndex, AVRCP_EVENT_ID_VOLUME_CHANGED, AVRCP_LABEL_VOLUME_CHANGED);
	}
	
	if (addEvt & MMI_AVRCP_PLAYER_APPLICATION_SETTING_CHANGED)
	{
		AVRCP_SendRegisterNotification(linkIndex, AVRCP_EVENT_ID_PLAYER_APPLICATION_SETTING_CHANGED, AVRCP_LABEL_PLAYER_APPLICATION_SETTING_CHANGED);
	}
}

PRIVATE void AVRCP_SendNextGetElementAttributes(U8 linkIndex)
{
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);
	U8 avrcpMediaEvents = avrcpLinkInfo->avrcpMediaEvents;

	if (avrcpMediaEvents)
	{
		if (avrcpMediaEvents & MMI_AVRCP_MEDIA_TITLE)
		{
			avrcpMediaEvents &= ~MMI_AVRCP_MEDIA_TITLE;
			AVRCP_SendGetElementAttributes(linkIndex, AVRCP_MEDIA_TITLE);
		}
		else if (avrcpMediaEvents & MMI_AVRCP_MEDIA_ARTIST_NAME)
		{
			avrcpMediaEvents &= ~MMI_AVRCP_MEDIA_ARTIST_NAME;
			AVRCP_SendGetElementAttributes(linkIndex, AVRCP_MEDIA_ARTIST_NAME);
		}
		else if (avrcpMediaEvents & MMI_AVRCP_MEDIA_ALBUM_NAME)
		{
			avrcpMediaEvents &= ~MMI_AVRCP_MEDIA_ALBUM_NAME;
			AVRCP_SendGetElementAttributes(linkIndex, AVRCP_MEDIA_ALBUM_NAME);
		}
		else if (avrcpMediaEvents & MMI_AVRCP_MEDIA_TRACK_NUMBER)
		{
			avrcpMediaEvents &= ~MMI_AVRCP_MEDIA_TRACK_NUMBER;
			AVRCP_SendGetElementAttributes(linkIndex, AVRCP_MEDIA_TRACK_NUMBER);
		}
		else if (avrcpMediaEvents & MMI_AVRCP_MEDIA_TOTAL_TRACKS)
		{
			avrcpMediaEvents &= ~MMI_AVRCP_MEDIA_TOTAL_TRACKS;
			AVRCP_SendGetElementAttributes(linkIndex, AVRCP_MEDIA_TOTAL_TRACKS);
		}
		else if (avrcpMediaEvents & MMI_AVRCP_MEDIA_GENRE)
		{
			avrcpMediaEvents &= ~MMI_AVRCP_MEDIA_GENRE;
			AVRCP_SendGetElementAttributes(linkIndex, AVRCP_MEDIA_GENRE);
		}
		else if (avrcpMediaEvents & MMI_AVRCP_MEDIA_PLAYING_TIME)
		{
			avrcpMediaEvents &= ~MMI_AVRCP_MEDIA_PLAYING_TIME;
			AVRCP_SendGetElementAttributes(linkIndex, AVRCP_MEDIA_PLAYING_TIME);
		}
		avrcpLinkInfo->avrcpMediaEvents = avrcpMediaEvents;
	}
}

PRIVATE void AVRCP_RemoteGetCapabilitiesRsp(U8 linkIndex, AVRCP_CMD_TYPE XDATA_PTR remotePduPtr)
{
	U8 XDATA_PTR evtPtr;
	U8 avrcpCapabilities = 0;
	U8 event, count = remotePduPtr->msgBodyPtr.getCapabilitiesRsp.CapabilityCount;

	if(remotePduPtr->msgBodyPtr.getCapabilitiesRsp.CapabilityId == AVRCP_GET_CAPABILITIES_EVENTS_SUPPORTED)
	{
		evtPtr = (U8 XDATA_PTR)&remotePduPtr->msgBodyPtr/*.getCapabilitiesRsp eliminate to prevent coverity warning*/ + sizeof(remotePduPtr->msgBodyPtr.getCapabilitiesRsp);
		while (count--)
		{
			event = *evtPtr++;
			switch (event)
			{
				case AVRCP_EVENT_ID_PLAYBACK_STATUS_CHANGED:
					avrcpCapabilities |= MMI_AVRCP_PLAYBACK_STATUS_CHANGED;
					break;
				case AVRCP_EVENT_ID_TRACK_CHANGED:
					avrcpCapabilities |= MMI_AVRCP_TRACK_CHANGED;
					break;
				case AVRCP_EVENT_ID_TRACK_REACHED_END:
					avrcpCapabilities |= MMI_AVRCP_TRACK_REACHED_END;
					break;
				case AVRCP_EVENT_ID_TRACK_REACHED_START:
					avrcpCapabilities |= MMI_AVRCP_TRACK_REACHED_START;
					break;
				case AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED:
					avrcpCapabilities |= MMI_AVRCP_PLAYBACK_POS_CHANGED;
					break;
				case AVRCP_EVENT_ID_VOLUME_CHANGED:
					avrcpCapabilities |= MMI_AVRCP_VOLUME_CHANGED;
					break;
				case AVRCP_EVENT_ID_PLAYER_APPLICATION_SETTING_CHANGED:
					avrcpCapabilities |= MMI_AVRCP_PLAYER_APPLICATION_SETTING_CHANGED;
					break;
			}
		}									
		AVRCP_GetLinkInfo(linkIndex)->avrcpCapabilities = avrcpCapabilities;
		AVRCP_SetNotifyMask(linkIndex, gMMI_nvram.mmiAvrcpSettings.notifyEventMask);
	}
}

PRIVATE void AVRCP_RemoteGetElementAttributesRsp(U8 linkIndex, AVRCP_CMD_TYPE XDATA_PTR remotePduPtr)
{
	#define MAX_AVRCP_MEDIA_ATTRIBUTE_SIZE	(255 - (sizeof(HciUARTAvrcpMediaAttribute)-1))

	MMIMsgType XDATA_PTR ptr1 = (MMIMsgType XDATA_PTR) remotePduPtr;
	U8 paraLen;

	if (remotePduPtr->msgBodyPtr.getElementAttributesRsp.ParameterLength > MAX_AVRCP_MEDIA_ATTRIBUTE_SIZE)
	{
		paraLen = remotePduPtr->msgBodyPtr.getElementAttributesRsp.ParameterLength = MAX_AVRCP_MEDIA_ATTRIBUTE_SIZE;
		remotePduPtr->msgBodyPtr.getElementAttributesRsp.Attribute[0].AttributeValueLength = MAX_AVRCP_MEDIA_ATTRIBUTE_SIZE - 9;
	}
	else
	{
		paraLen = remotePduPtr->msgBodyPtr.getElementAttributesRsp.ParameterLength;
	}
	
	ptr1->dataOffset = EVT_OFFSET;
	ptr1->eventCode = AVRCP_EVT_TO_MMI_GET_ELEMENT_ATTRIBUTES;
	ptr1->paraLen = paraLen + (sizeof(HciUARTAvrcpMediaAttribute)-1);
	
	OSMEM_memcpy_xdata_xdata(&ptr1->msgBodyPtr.hciUARTAvrcpMediaAttribute.dataBegin,
								&remotePduPtr->msgBodyPtr.getElementAttributesRsp.NumAttributes, paraLen);
	MMI_AVRCP_EventHandler(linkIndex, AVRCP_EVT_TO_MMI_GET_ELEMENT_ATTRIBUTES, (AVRCP_TO_MMI_EVT_PARAM XDATA_PTR)ptr1);	
}

PRIVATE void AVRCP_RemoteRegisterNotification(U8 linkIndex, AVRCP_CMD_TYPE XDATA_PTR remotePduPtr)
{
	//AVRCP_TO_MMI_EVT_PARAM LDATA param;
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);
	U8 avrcpNotifyMask = avrcpLinkInfo->avrcpNotifyMask;
	U8 i, notifyLabel = 0;
	
	switch (remotePduPtr->msgBodyPtr.notifyPdu.eventID)
	{
		case AVRCP_EVENT_ID_PLAYBACK_STATUS_CHANGED:
			if (avrcpNotifyMask & MMI_AVRCP_PLAYBACK_STATUS_CHANGED)
			{
				notifyLabel = AVRCP_LABEL_PLAYBACK_STATUS_CHANGED;
			}
			break;
		case AVRCP_EVENT_ID_TRACK_CHANGED:
			if (avrcpNotifyMask & MMI_AVRCP_TRACK_CHANGED)
			{
				notifyLabel = AVRCP_LABEL_TRACK_CHANGED;
			}
			break;
		case AVRCP_EVENT_ID_TRACK_REACHED_END:
			if (avrcpNotifyMask & MMI_AVRCP_TRACK_REACHED_END)
			{
				notifyLabel = AVRCP_LABEL_TRACK_CHANGED;
			}
			break;
		case AVRCP_EVENT_ID_TRACK_REACHED_START:
			if (avrcpNotifyMask & MMI_AVRCP_TRACK_REACHED_START)
			{
				notifyLabel = AVRCP_LABEL_TRACK_REACHED_START;
			}
			break;
		case AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED:
			if (avrcpNotifyMask & MMI_AVRCP_PLAYBACK_POS_CHANGED)
			{
				notifyLabel = AVRCP_LABEL_PLAYBACK_POS_CHANGED;
			}
			break;
		case AVRCP_EVENT_ID_VOLUME_CHANGED:
			if (avrcpNotifyMask & MMI_AVRCP_VOLUME_CHANGED)
			{
				notifyLabel = AVRCP_LABEL_VOLUME_CHANGED;
			}
			break;
		case AVRCP_EVENT_ID_PLAYER_APPLICATION_SETTING_CHANGED:
			if (avrcpNotifyMask & MMI_AVRCP_PLAYER_APPLICATION_SETTING_CHANGED)
			{
				notifyLabel = AVRCP_LABEL_PLAYER_APPLICATION_SETTING_CHANGED;
			}
			break;
	}
	
	if (!notifyLabel)
		return;
		
	if (remotePduPtr->ctypeOrResp == AVC_RES_INTERIM)
	{
		if (notifyLabel == AVRCP_LABEL_TRACK_CHANGED)
		{
			if (!(avrcpLinkInfo->avrcpMiscFlags & AVRCP_MISC_FIRST_INTERIM_TRACK_CHANGED))
			{
				avrcpLinkInfo->avrcpMiscFlags |= AVRCP_MISC_FIRST_INTERIM_TRACK_CHANGED;

				/*for (i = 0; i < 8; i++)
				{
					if (remotePduPtr->msgBodyPtr.notifyPdu.eventParams.UID[i] != 0)
						break;
				}
				if (i == 8)
				{ //current track is selected for browsing not supported player
					goto label_changed_notify;
				}*/

				for (i = 0; i < 8; i++)
				{
					if (remotePduPtr->msgBodyPtr.notifyPdu.eventParams.UID[i] != 0xFF)
						break;
				}
				if (i < 8)
				{ //current track is selected for browsing supported player
					goto label_changed_notify;
				}
			}
		}
		else if (notifyLabel == AVRCP_LABEL_PLAYBACK_STATUS_CHANGED)
		{
			goto label_changed_notify;
		}
		return;
	}

label_changed_notify:
	//OSMEM_memcpy_xdata_xdata(&param.Notify.eventID,
	//						&remotePduPtr->msgBodyPtr.notifyPdu.eventID, remotePduPtr->msgBodyPtr.notifyPdu.ParameterLength);
	//MMI_AVRCP_EventHandler(linkIndex, AVRCP_EVT_TO_MMI_REMOTE_NOTIFICATION_CHANGED, &param);
	MMI_AVRCP_EventHandler(linkIndex, AVRCP_EVT_TO_MMI_REMOTE_NOTIFICATION_CHANGED, (AVRCP_TO_MMI_EVT_PARAM XDATA_PTR)&remotePduPtr->msgBodyPtr.notifyPdu.eventID);

	if (remotePduPtr->ctypeOrResp == AVC_RES_CHANGED)
	{
		AVRCP_SendRegisterNotification(linkIndex, avrcp_mmi_notify_event_id[notifyLabel-AVRCP_FIRST_NOTIFY_LABEL], notifyLabel);
	}
	
	if (notifyLabel == AVRCP_LABEL_TRACK_CHANGED)
	{
		avrcpLinkInfo->avrcpMediaEvents = gMMI_nvram.mmiAvrcpSettings.mediaEventMask;
		AVRCP_SendNextGetElementAttributes(linkIndex);
	}
}

PRIVATE BOOL AVRCP_CheckBluetoothSIG(U8 XDATA_PTR companyIDPtr)
{
	if (companyIDPtr[0] == 0x00 && companyIDPtr[1] == 0x19 && companyIDPtr[2] == 0x58)
		return TRUE;
	else
		return FALSE;
}

PRIVATE BOOL AVRCP_CheckAirohaUnique(U8 XDATA_PTR companyIDPtr)
{
	if (companyIDPtr[0] == 0x0C && companyIDPtr[1] == 0xFC && companyIDPtr[2] == 0x83)
		return TRUE;
	else
		return FALSE;
}

PRIVATE U8 AVRCP_CheckSupported(U8 linkIndex, AVRCP_CMD_TYPE XDATA_PTR remotePduPtr)
{
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo;
	U8 resultCode = AVC_ERROR_CODE_INVALID_COMMAND;
	U8 operandID;
	
	if(remotePduPtr->subUnitType != SUBUNIT_PANEL || remotePduPtr->subUnitID != 0x00)
		return AVC_ERROR_CODE_INVALID_COMMAND;

	switch(remotePduPtr->opCode)
	{
		case AVC_OPCODE_VENDOR_DEPENDENT:
			if(AVRCP_CheckBluetoothSIG(remotePduPtr->msgBodyPtr.vendorDependentPdu.companyID))
			{
				switch(remotePduPtr->msgBodyPtr.vendorDependentPdu.pduID)
				{
					case AVRCP_PDU_ID_GET_CAPABILITIES:
						if(remotePduPtr->ctypeOrResp == AVC_CMD_STATUS)
						{
							if(remotePduPtr->msgBodyPtr.getCapabilitiesCmd.ParameterLength == 1)
							{ 
								if(remotePduPtr->msgBodyPtr.getCapabilitiesCmd.CapabilityId == AVRCP_GET_CAPABILITIES_COMPANY_ID || 
									remotePduPtr->msgBodyPtr.getCapabilitiesCmd.CapabilityId == AVRCP_GET_CAPABILITIES_EVENTS_SUPPORTED) 
								{
									resultCode = AVC_ERROR_CODE_NO_ERROR;
								}
								else
								{
									resultCode = AVC_ERROR_CODE_INVALID_PARAMETER;
								}
							}
							else
							{
								resultCode = AVC_ERROR_CODE_PARAMETER_ERROR;
							}
						}
						break;
					case AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTES:	/* for AVCTP BQB test case FRA_BV_03_C */
						if(remotePduPtr->ctypeOrResp == AVC_CMD_STATUS)
						{
							resultCode = AVC_ERROR_CODE_NO_ERROR;
						}
						break;
					case AVRCP_PDU_ID_REGISTER_NOTIFICATION:
						if(remotePduPtr->ctypeOrResp == AVC_CMD_NOTIFY)
						{
							if(remotePduPtr->msgBodyPtr.registerPdu.ParameterLength == 5)
							{
								if(remotePduPtr->msgBodyPtr.registerPdu.eventID == AVRCP_EVENT_ID_VOLUME_CHANGED)
								{
									if(!MMI_AVRCP_VOLUME_SYNC_DISABLED_FEAT)
									{
										avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);
										avrcpLinkInfo->avrcpMiscFlags |= AVRCP_REPORT_VOLUME_CHANGE;
										avrcpLinkInfo->volumeNotifyTransactionLabel = remotePduPtr->transactionLabel;
										resultCode = AVC_ERROR_CODE_NO_ERROR;
									}
								}
								else
								{
									resultCode = AVC_ERROR_CODE_INVALID_PARAMETER;
								}
							}
							else
							{
								resultCode = AVC_ERROR_CODE_PARAMETER_ERROR;
							}
						}
						break;
					case AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
						if(remotePduPtr->ctypeOrResp == AVC_CMD_CONTROL && !MMI_AVRCP_VOLUME_SYNC_DISABLED_FEAT)
						{
							if(remotePduPtr->msgBodyPtr.absoluteVolumePdu.ParameterLength == 1)
							{
								AVRCP_SendEvtToMMI_SetAbsoluteVolume(linkIndex, remotePduPtr->msgBodyPtr.absoluteVolumePdu.absoluteVolume);
								resultCode = AVC_ERROR_CODE_NO_ERROR;
							}
							else
							{
								resultCode = AVC_ERROR_CODE_PARAMETER_ERROR;
							}
						}
						break;
				}
			}
			break;
		case AVC_OPCODE_PASS_THROUGH:
			if(remotePduPtr->ctypeOrResp == AVC_CMD_CONTROL)
			{
				operandID = remotePduPtr->msgBodyPtr.passThroughPdu.operandID;
				if(operandID >= AVC_OPERATION_POWER && operandID <= AVC_OPERATION_BACKWARD)
				{
					if(remotePduPtr->msgBodyPtr.passThroughPdu.operandLength == 0)
					{
						if((operandID == AVC_OPERATION_VOL_UP || operandID == AVC_OPERATION_VOL_DOWN) && MMI_AVRCP_VOLUME_SYNC_DISABLED_FEAT)
						{
							break;
						}
						
						if(!remotePduPtr->msgBodyPtr.passThroughPdu.buttonReleased)// Nokia sends only press, no release
						{
							AVRCP_SendEvtToMMI(linkIndex, operandID);	
						}
						resultCode = AVC_ERROR_CODE_NO_ERROR;
					}
					else
					{
						resultCode = AVC_ERROR_CODE_PARAMETER_ERROR;
					}
				}
				else if (operandID == AVC_OPERATION_VENDOR_UNIQUE)
				{
					if(remotePduPtr->msgBodyPtr.vendorUniquePdu.operandLength == 5)
					{
						if (AVRCP_CheckAirohaUnique(remotePduPtr->msgBodyPtr.vendorUniquePdu.companyID))
						{
							if(!remotePduPtr->msgBodyPtr.vendorUniquePdu.buttonReleased)// Nokia sends only press, no release
							{
								AVRCP_SendEvtToMMI_VendorUnique(linkIndex, 
																remotePduPtr->msgBodyPtr.vendorUniquePdu.vendor_unique.airoha.operationId,
																remotePduPtr->msgBodyPtr.vendorUniquePdu.vendor_unique.airoha.operationParam);
							}
							resultCode = AVC_ERROR_CODE_NO_ERROR;
						}
					}
					#ifdef LIGHTING_ENABLE
					else if(remotePduPtr->msgBodyPtr.vendorUniquePdu.operandLength == avrcp_vendor_unique_operation_extend[4])
					{
						if (AVRCP_CheckAirohaUnique(remotePduPtr->msgBodyPtr.vendorUniquePdu.companyID))
						{
							AVRCP_SendEvtToMMI_VendorUniqueExtern(linkIndex, 
																remotePduPtr->msgBodyPtr.vendorUniquePdu.vendor_unique.airoha.operationId,
																remotePduPtr->msgBodyPtr.vendorUniquePdu.vendor_unique.airoha.operationParam,
																remotePduPtr->msgBodyPtr.vendorUniquePdu.vendor_unique.airoha.syncClk,
																&(remotePduPtr->msgBodyPtr.vendorUniquePdu.vendor_unique.airoha.ledPara[0]));
							resultCode = AVC_ERROR_CODE_NO_ERROR;
						}
					}
					#endif
					else
					{
						resultCode = AVC_ERROR_CODE_PARAMETER_ERROR;
					}
				}
			}
			break;
	}
	return resultCode;
}

PRIVATE void AVRCP_SendResp(U8 linkIndex, AVRCP_CMD_TYPE XDATA_PTR remotePduPtr, U8 resultCode)
{
	U8 length;
	U8 XDATA_PTR msgPtr;
	U8 XDATA_PTR resPtr;

	if(msgPtr = AVRCP_GetAVCTPMessageBuffer(0/*fake length, fixed below*/))
	{
		resPtr = &msgPtr[15];
		switch (remotePduPtr->opCode)
		{
			case AVC_OPCODE_UNIT_INFO:
			case AVC_OPCODE_SUBUNIT_INFO:
				OSMEM_memcpy_xdata_code(resPtr, AVRCP_unit_info_resp, (length = sizeof(AVRCP_unit_info_resp)));
				resPtr[1] = (remotePduPtr->subUnitType<<3) | remotePduPtr->subUnitID;
				resPtr[2] = remotePduPtr->opCode;
				break;
			case AVC_OPCODE_VENDOR_DEPENDENT:
				OSMEM_memcpy_xdata_xdata(resPtr, ((U8 XDATA_PTR)remotePduPtr)+AVRCP_PAYLOAD_OFFSET, sizeof(AVRCP_CMD_TYPE)-AVRCP_PAYLOAD_OFFSET);
				if(resultCode == AVC_ERROR_CODE_NO_ERROR)
				{
					if(remotePduPtr->msgBodyPtr.vendorDependentPdu.pduID == AVRCP_PDU_ID_GET_CAPABILITIES)
					{
						resPtr[0] = AVC_RES_STABLE;
						if(remotePduPtr->msgBodyPtr.getCapabilitiesCmd.CapabilityId == AVRCP_GET_CAPABILITIES_COMPANY_ID)
						{
							*(U16 XDATA_PTR)&resPtr[8] = 5; //ParameterLength
							resPtr[11] = 1;
							resPtr[12] = resPtr[3];
							resPtr[13] = resPtr[4];
							resPtr[14] = resPtr[5];
							length = 15;
							break;
						}
						else if(remotePduPtr->msgBodyPtr.getCapabilitiesCmd.CapabilityId == AVRCP_GET_CAPABILITIES_EVENTS_SUPPORTED)
						{
							if(MMI_AVRCP_VOLUME_SYNC_DISABLED_FEAT)
							{
								*(U16 XDATA_PTR)&resPtr[8] = 2; //ParameterLength
								resPtr[11] = 0;
								length = 12;
							}
							else
							{
								*(U16 XDATA_PTR)&resPtr[8] = 3; //ParameterLength
								resPtr[11] = 1;
								resPtr[12] = AVRCP_EVENT_ID_VOLUME_CHANGED;
								length = 13;
							}
							break;
						}
					}
					else if(remotePduPtr->msgBodyPtr.vendorDependentPdu.pduID == AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTES)	/* for AVCTP BQB test case FRA_BV_03_C */
					{
						resPtr[0] = AVC_RES_STABLE;
						*(U16 XDATA_PTR)&resPtr[8] = 1; //ParameterLength
						resPtr[10] = 0;
						length = 11;
						break;
					}
					else if(remotePduPtr->msgBodyPtr.vendorDependentPdu.pduID == AVRCP_PDU_ID_REGISTER_NOTIFICATION && remotePduPtr->msgBodyPtr.registerPdu.eventID == AVRCP_EVENT_ID_VOLUME_CHANGED)
					{
						resPtr[0] = AVC_RES_INTERIM;
						*(U16 XDATA_PTR)&resPtr[8] = 2; //ParameterLength
						resPtr[11] = MMI_AVRCP_GetAbsVolByMusic(linkIndex);
						length = 12;
						break;
					}
					else if(remotePduPtr->msgBodyPtr.vendorDependentPdu.pduID == AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME)
					{
						resPtr[0] = AVC_RES_ACCEPTED;
						/* ParameterLength not changed */
						length = 11;
						break;
					}
				}
				
				//Implemented according to AVRCP - 6.15.1 AVRCP Specific AV/C Commands
				resPtr[0] = (resultCode == AVC_ERROR_CODE_INVALID_COMMAND) ? AVC_RES_NOT_IMPLEMENTED : AVC_RES_REJECTED;
				*(U16 XDATA_PTR)&resPtr[8] = 1; //ParameterLength
				resPtr[10] = resultCode;
				length = 11;
				break;
			case AVC_OPCODE_PASS_THROUGH:
				OSMEM_memcpy_xdata_xdata(resPtr, ((U8 XDATA_PTR)remotePduPtr)+AVRCP_PAYLOAD_OFFSET,  (length = 5+remotePduPtr->msgBodyPtr.passThroughPdu.operandLength));
				resPtr[0] = (resultCode == AVC_ERROR_CODE_NO_ERROR) ? AVC_RES_ACCEPTED : AVC_RES_NOT_IMPLEMENTED;
				break;
			default:
				OSMEM_Put(msgPtr);
				return;
		}
		msgPtr[8] += length; //set correct l2cap packet length
		AVCTP_SendMessageRsp(linkIndex, msgPtr, remotePduPtr->transactionLabel);
	}
}

PUBLIC U8 AVRCP_GetState(U8 linkIndex)
{
	return AVRCP_GetLinkInfo(linkIndex)->mmiProfileInfo.state;
}

PUBLIC void AVRCP_MMICommandHandler(U8 linkIndex, U8 cmdCode, MMI_TO_AVRCP_CMD_PARAM XDATA_PTR paramPtr)
{
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);

	MMI_SniffDisable(linkIndex, MMI_SNIFF_AVRCP);
	
	switch (cmdCode)
	{
		case AVRCP_SEND_OPERATION:	
			if(avrcpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
			{
				// If Pause is not supported, send Stop
				if((avrcpLinkInfo->avrcpMiscFlags & AVRCP_NOT_SUPPORT_PAUSE_CMD_IN_REMOTE) && paramPtr->operationId == AVC_OPERATION_PAUSE)
					paramPtr->operationId = AVC_OPERATION_STOP;

				switch(paramPtr->operationId)
				{
					#ifdef PEQ_ENABLE
					case AVC_OPERATION_PEQOFF:
					case AVC_OPERATION_PEQON:
						AVRCP_SendSetPlayerApplicationSettingValue(linkIndex, AVRCP_PEQ_ON_OFF_STATUS, paramPtr->operationId);
						break;
					#endif
					case AVC_OPERATION_GET_REPEAT_MODE:
						AVRCP_SendGetPlayerApplicationSettingValue(linkIndex, AVRCP_REPEAT_MODE_STATUS);
						break;
					case AVC_OPERATION_SET_REPEAT_MODE:
						AVRCP_SendSetPlayerApplicationSettingValue(linkIndex, AVRCP_REPEAT_MODE_STATUS, paramPtr->PlayAppSettings.attributeValue);
						break;
					case AVC_OPERATION_GET_SHUFFLE_MODE:
						AVRCP_SendGetPlayerApplicationSettingValue(linkIndex, AVRCP_SHUFFLE_ON_OFF_STATUS);
						break;
					case AVC_OPERATION_SET_SHUFFLE_MODE:
						AVRCP_SendSetPlayerApplicationSettingValue(linkIndex, AVRCP_SHUFFLE_ON_OFF_STATUS, paramPtr->PlayAppSettings.attributeValue);
						break;
					case AVC_OPERATION_KEY_PLAY:
					case AVC_OPERATION_KEY_PAUSE:
						avrcpLinkInfo->avrcpMiscFlags |= AVRCP_SEND_PLAY_PAUSE_AFTER_GET_PLAY_STATUS;
						/* fallthrough *//*comment to prevent coverity warning*/
					case AVC_OPERATION_GET_PLAY_STATUS:
						AVRCP_SendGetPlayStatus(linkIndex);
						break;
					case AVC_OPERATION_INTERNAL_REWIND_RELEASE:
					case AVC_OPERATION_INTERNAL_FAST_FORWARD_RELEASE:
						if(avrcpLinkInfo->avrcpMiscFlags & AVRCP_MISC_REWIND_OR_FAST_FORWARD_SENT)
						{
							avrcpLinkInfo->avrcpMiscFlags &= (~AVRCP_MISC_REWIND_OR_FAST_FORWARD_SENT);
							AVRCP_SendPassThrough(linkIndex, (paramPtr->operationId == AVC_OPERATION_INTERNAL_REWIND_RELEASE ? AVC_OPERATION_REWIND : AVC_OPERATION_FAST_FORWARD), RELEASED);
						}
						break;
					default:
						AVRCP_SendPassThrough(linkIndex, paramPtr->operationId, PUSHED);
						if(paramPtr->operationId == AVC_OPERATION_REWIND || paramPtr->operationId == AVC_OPERATION_FAST_FORWARD)
						{
							avrcpLinkInfo->avrcpMiscFlags |=AVRCP_MISC_REWIND_OR_FAST_FORWARD_SENT;
						}
						else
						{
							AVRCP_SendPassThrough(linkIndex, paramPtr->operationId, RELEASED);
						}
						break;
				}
			}
			break;
		case AVRCP_VOLUME_CHANGED:
			if(avrcpLinkInfo->avrcpMiscFlags & AVRCP_REPORT_VOLUME_CHANGE)
			{
				avrcpLinkInfo->avrcpMiscFlags &= ~AVRCP_REPORT_VOLUME_CHANGE;
				AVRCP_SendVolumeChangedNotify(linkIndex, paramPtr->operationId, avrcpLinkInfo->volumeNotifyTransactionLabel);
			}
			break;
		case AVRCP_SET_NOTIFY_MASK:
			AVRCP_SetNotifyMask(linkIndex, paramPtr->operationId);
			break;
		case AVRCP_SET_ABSOLUTE_VOLUME:
			AVRCP_SendSetAbsoluteVolume(linkIndex, paramPtr->operationId);
			break;
		case AVRCP_GET_ELEMENT_ATTRIBUTES:
			AVRCP_SendGetElementAttributes(linkIndex, paramPtr->operationId);
			break;
		case AVRCP_SEND_VENDOR_UNIQUE:
			AVRCP_SendVendorUnique(	linkIndex, 
									paramPtr->VendorUnique.operationId,
									paramPtr->VendorUnique.operationParam);
			break;
		default:
			ASSERT_PRINT("INVALID CMD CODE(AVRCP)");
			break;
	}
	MMI_SniffEnable(linkIndex, MMI_SNIFF_AVRCP);
}

/*
PRIVATE void AVRCP(U8 linkIndex)
{
}*/

PRIVATE MMI_PROFILE_INFO XDATA_PTR AVRCP_ConnectRequest(U8 linkIndex, U8 profileId)
{
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);
	UNUSED(profileId);
	
	if (avrcpLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
	{
		if (MMI_SDAP_GetRemoteServiceChannel(linkIndex, PROFILE_AVRCP) != 0)
		{
			if (AVCTP_ConnectChannel(linkIndex) != INVALID_L2CAP_CHANNEL)
			{
				return &avrcpLinkInfo->mmiProfileInfo;
			}
		}
	}
	return (MMI_PROFILE_INFO XDATA_PTR)NULL;
}

PRIVATE U8 AVRCP_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);

	if (avrcpLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED && &avrcpLinkInfo->mmiProfileInfo == profileInfo)
	{
		if(CMD_WAITING == AVCTP_DisconnectChannel(linkIndex))
		{
			avrcpLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTING;
			return CMD_WAITING;
		}
		MMI_AVRCP_ClearCtlData(linkIndex);
		MMI_UnloadProfile(linkIndex, &avrcpLinkInfo->mmiProfileInfo);
		AVRCP_ClearLinkInfo(avrcpLinkInfo);
		MMI_SniffEnable(linkIndex, MMI_SNIFF_AVRCP);
	}
	return CMD_COMPLETE;
}

PRIVATE void AVRCP_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams)
{
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);

	switch(evtCode)
	{
		case PROFILE_LINK_EVENT_CONN_REQUEST:
			evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT;
			if (avrcpLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
			{
				if(MMI_IsLinkActiveDisconnecting(linkIndex))
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT_ACTIVE_DISC;
				}
				else
				{
					evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_ACCEPT;
					evtParams->connRequest.profileInfo = &avrcpLinkInfo->mmiProfileInfo;
				}
			}
			break;
			
		case PROFILE_LINK_EVENT_CONN_COMPLETE:
			if (avrcpLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTING)
			{
				if (evtParams->connComplete.status == NO_ERROR)
				{
					avrcpLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
					AVRCP_SendGetCapabilities(linkIndex, AVRCP_GET_CAPABILITIES_EVENTS_SUPPORTED);
					MMI_ActivateProfile(linkIndex, &avrcpLinkInfo->mmiProfileInfo);
					MMI_AVRCP_ConnectedHandle(linkIndex);
				}
				else
				{
					if(evtParams->connComplete.status == SECURITY_BLOCK)
					{
						MMI_SetReconnectProfile(linkIndex, avrcpLinkInfo->mmiProfileInfo.profileDesc->profileId, ONE_SEC);
					}
					evtCode = PROFILE_LINK_EVENT_DISC_COMPLETE;
					goto AVRCP_DISC_CPL;
				}
			}
			break;
			
		case PROFILE_LINK_EVENT_DISC_COMPLETE:
		case PROFILE_LINK_EVENT_DISC_LINKLOSS:
			if (avrcpLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
			{
				AVRCP_DISC_CPL:
				MMI_AVRCP_ClearCtlData(linkIndex);
				MMI_UnloadProfile(linkIndex, &avrcpLinkInfo->mmiProfileInfo);
				MMI_PROTOCOL_L2CAPDisconnected(linkIndex, evtCode, FALSE);
				AVRCP_ClearLinkInfo(avrcpLinkInfo);
				MMI_SniffEnable(linkIndex, MMI_SNIFF_AVRCP);
			}
			break;
	}
}

PRIVATE void AVRCP_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	AVRCP_LINK_INFO XDATA_PTR avrcpLinkInfo = AVRCP_GetLinkInfo(linkIndex);
	AVRCP_CMD_TYPE XDATA_PTR remotePduPtr = (AVRCP_CMD_TYPE XDATA_PTR)dataPtr;
	U8 operandID;

	if(avrcpLinkInfo->mmiProfileInfo.state != PROFILE_CONNECTED)
	{
		goto AVRCP_DATA_END;
	}

	LightDbgPrintWithConnectionHandle (
		HCI_VOPCODE_VOGF(HCI_VENDOR_OGF_UART_AVRCP_DBGPRINT,HCI_VEVT_OCF_AVRCP_DBGPRINT_RX),
		MMI_GetConnHandleByLinkIndex(linkIndex),
		(U8 XDATA_PTR)&remotePduPtr->l2capLength);
	
	if(remotePduPtr->ctypeOrResp <= AVC_CMD_GENERAL_INQUIRY) // Command
	{
		AVRCP_SendResp(linkIndex, remotePduPtr, AVRCP_CheckSupported(linkIndex, remotePduPtr));
		goto AVRCP_DATA_END;
	}

	if(remotePduPtr->opCode == AVC_OPCODE_VENDOR_DEPENDENT)
	{
		switch(remotePduPtr->msgBodyPtr.vendorDependentPdu.pduID)
		{
			case AVRCP_PDU_ID_GET_CAPABILITIES:
				if(remotePduPtr->ctypeOrResp == AVC_RES_STABLE)
				{
					AVRCP_RemoteGetCapabilitiesRsp(linkIndex, remotePduPtr);
				}
				break;
			case AVRCP_PDU_ID_GET_PALYER_APPLICATION:
				if(remotePduPtr->ctypeOrResp == AVC_RES_STABLE)
				{
					if(	remotePduPtr->msgBodyPtr.playerSettingsValueResp.settingAttributeID == AVRCP_REPEAT_MODE_STATUS || 
						remotePduPtr->msgBodyPtr.playerSettingsValueResp.settingAttributeID == AVRCP_SHUFFLE_ON_OFF_STATUS)
					{
						AVRCP_SendEvtToMMI_GetAppSetting(linkIndex, remotePduPtr->msgBodyPtr.playerSettingsValueResp.settingAttributeID, remotePduPtr->msgBodyPtr.playerSettingsValueResp.settingValue);
					}
				}
				else if(remotePduPtr->ctypeOrResp == AVC_RES_REJECTED)
				{
					AVRCP_SendEvtToMMI_GetAppSetting(linkIndex, remotePduPtr->msgBodyPtr.playerSettingsValueResp.settingAttributeID, AVRCP_REPEAT_CMD_REJECT);					
				}
				break;
			case AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTES:
				if(remotePduPtr->ctypeOrResp == AVC_RES_REJECTED || remotePduPtr->ctypeOrResp == AVC_RES_STABLE)
				{
					AVRCP_SendNextGetElementAttributes(linkIndex);
					if(remotePduPtr->ctypeOrResp == AVC_RES_STABLE)
					{
						U8 packetType = remotePduPtr->msgBodyPtr.vendorDependentPdu.packetType;
						if (packetType == AVRCP_PKT_TYPE_START)
						{
							avrcpLinkInfo->continueResponseCount = gMMI_nvram.mmiAvrcpSettings.continueResponseCount;
						}

						if (packetType == AVRCP_PKT_TYPE_START || packetType == AVRCP_PKT_TYPE_CONTINUE)
						{
							if (avrcpLinkInfo->continueResponseCount)
							{
								avrcpLinkInfo->continueResponseCount--;
								AVRCP_SendContinueResponse(linkIndex, AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTES, AVRCP_PDU_ID_REQUEST_CONTINUE_RESPONSE);
							}
							else
							{
								AVRCP_SendContinueResponse(linkIndex, AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTES, AVRCP_PDU_ID_ABORT_CONTINUE_RESPONSE);
							}
						}

						if (packetType == AVRCP_PKT_TYPE_SINGLE || packetType == AVRCP_PKT_TYPE_START)
						{
							AVRCP_RemoteGetElementAttributesRsp(linkIndex, remotePduPtr);
							return; //redirect remotePduPtr, bypass OSMEM_Put()
						}
					}
				}
				break;
			case AVRCP_PDU_ID_GET_PLAY_STATUS:
				if(remotePduPtr->ctypeOrResp == AVC_RES_NOT_IMPLEMENTED || remotePduPtr->ctypeOrResp == AVC_RES_REJECTED)
				{
					AVRCP_SendEvtToMMI_PlayStatus(linkIndex, 
												(avrcpLinkInfo->avrcpMiscFlags & AVRCP_SEND_PLAY_PAUSE_AFTER_GET_PLAY_STATUS)?AVRCP_EVT_TO_MMI_PLAY_STATUS_FOR_KEY_PLAY_PAUSE:AVRCP_EVT_TO_MMI_GET_PLAY_STATUS, 
												AVRCP_PLAYBACK_STATUS_REJECT);
					avrcpLinkInfo->avrcpMiscFlags &= ~AVRCP_SEND_PLAY_PAUSE_AFTER_GET_PLAY_STATUS;
				}
				else if(remotePduPtr->ctypeOrResp == AVC_RES_STABLE)
				{
					AVRCP_SendEvtToMMI_PlayStatus(linkIndex, 
												(avrcpLinkInfo->avrcpMiscFlags & AVRCP_SEND_PLAY_PAUSE_AFTER_GET_PLAY_STATUS)?AVRCP_EVT_TO_MMI_PLAY_STATUS_FOR_KEY_PLAY_PAUSE:AVRCP_EVT_TO_MMI_GET_PLAY_STATUS, 
												remotePduPtr->msgBodyPtr.getPlayStatusRsp.PlayStatus);
					avrcpLinkInfo->avrcpMiscFlags &= ~AVRCP_SEND_PLAY_PAUSE_AFTER_GET_PLAY_STATUS;
				}
				break;
			case AVRCP_PDU_ID_REGISTER_NOTIFICATION:
				if(remotePduPtr->ctypeOrResp == AVC_RES_CHANGED || remotePduPtr->ctypeOrResp == AVC_RES_INTERIM)
				{
					AVRCP_RemoteRegisterNotification(linkIndex, remotePduPtr);
				}
				break;
		}	
	}
	else if(remotePduPtr->opCode == AVC_OPCODE_PASS_THROUGH)
	{
		operandID = remotePduPtr->msgBodyPtr.passThroughPdu.operandID;
		switch(remotePduPtr->ctypeOrResp)
		{
			case AVC_RES_NOT_IMPLEMENTED:
			case AVC_RES_REJECTED:
				if(operandID == AVC_OPERATION_PAUSE)
				{
					if(remotePduPtr->ctypeOrResp == AVC_RES_NOT_IMPLEMENTED)
					{
						avrcpLinkInfo->avrcpMiscFlags |= AVRCP_NOT_SUPPORT_PAUSE_CMD_IN_REMOTE;			
					}
					AVRCP_SendPassThrough(linkIndex, AVC_OPERATION_STOP, PUSHED);
					AVRCP_SendPassThrough(linkIndex, AVC_OPERATION_STOP, RELEASED);
				}
				else if(operandID == AVC_OPERATION_STOP)
				{
					AVRCP_SendEvtToMMI(linkIndex, AVRCP_EVT_TO_MMI_GET_STOP_REJECTED);
				}
				break;
			case AVC_RES_ACCEPTED:
				if(remotePduPtr->msgBodyPtr.passThroughPdu.buttonReleased)
				{
					if(operandID >= AVC_OPERATION_POWER && operandID <= AVC_OPERATION_BACKWARD)
					{
						AVRCP_SendEvtToMMI(linkIndex, (U8)(operandID + AVRCP_EVT_PASS_THROUGH_OFFSET));
					}
				}
				break;
		}	
	
	}
	
	AVRCP_DATA_END:
	OSMEM_Put((U8 XDATA_PTR)remotePduPtr);
}

PUBLIC void AVRCP_Init(void)
{
	extern SdpServiceNode gAVRCP_TgServiceNode;
	extern SdpServiceNode gAVRCP_CtServiceNode;
	
	SDAP_RegisterServiceRecord(&gAVRCP_TgServiceNode);
	SDAP_RegisterServiceRecord(&gAVRCP_CtServiceNode);

	AVCTP_RegisterProfile(&gAVRCP_ProfileNode);
}

#endif //AVRCP_Profile
