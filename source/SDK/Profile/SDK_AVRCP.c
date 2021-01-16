#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#include "..\MMI\MMI_Base.h"
#include "..\AVRCP\AVRCP_Interface.h"
#include "..\MMI\MMI_AVRCP.h"
#include "..\SDK\include\SDK_AVRCP.h"

#if SDK_AVRCP_PALY_APPLICATION
PUBLIC void SDK_AVRCP_CB_PlayApplication(U8 linkIndex, U8 attributeID, U8 attributeValue)
{
	#if SDK_AVRCP_REPEAT
	if(attributeID == AVRCP_REPEAT_MODE_STATUS)
	{
		LightDbgPrint("SDK AVRCP Repeat mode[%d]:%x",(U8)linkIndex, (U8)attributeValue);
		SDK_AVRCP_SendSetRepeatStatus(linkIndex, (U8)((attributeValue+1)%AVRCP_REPEAT_MODE_NO));
	}
	#endif
	#if SDK_AVRCP_SHUFFLE
	if(attributeID == AVRCP_SHUFFLE_ON_OFF_STATUS)
	{
		LightDbgPrint("SDK AVRCP Shuffle[%d]:%x",(U8)linkIndex, (U8)attributeValue);
		SDK_AVRCP_SendSetShuffleStatus(linkIndex, (U8)((attributeValue+1)%AVRCP_SHUFFLE_MODE_NO));
	}
	#endif
}
#endif

#if SDK_AVRCP_SHUFFLE
PUBLIC void SDK_AVRCP_SendGetShuffleStatus(U8 linkIndex)
{
	if(MMI_AVRCP_IsGetAppSettingsIdle(linkIndex))
	{
		MMI_AVRCP_SDK_SetGetAppSettingsOwner(linkIndex);
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_GET_SHUFFLE_MODE);
	}
}

PUBLIC void SDK_AVRCP_SendSetShuffleStatus(U8 linkIndex, U8 status)
{
	if(status >= AVRCP_SHUFFLE_MODE_NO)
	{
		ASSERT(FALSE);
	}
	MMI_AVRCP_SetPlayAppSettings(linkIndex, AVC_OPERATION_SET_SHUFFLE_MODE, status);
}

PUBLIC void SDK_AVRCP_KeyShuffleChange(U8 linkIndex)
{
	MMI_AVRCP_KeyShuffleModeChange(linkIndex);
}
#endif

#if SDK_AVRCP_REPEAT
PUBLIC void SDK_AVRCP_SendGetRepeatStatus(U8 linkIndex)
{
	if(MMI_AVRCP_IsGetAppSettingsIdle(linkIndex))
	{
		MMI_AVRCP_SDK_SetGetAppSettingsOwner(linkIndex);
		MMI_AVRCP_CmdGenerator(linkIndex, AVRCP_SEND_OPERATION, AVC_OPERATION_GET_REPEAT_MODE);
	}
}

PUBLIC void SDK_AVRCP_SendSetRepeatStatus(U8 linkIndex, U8 status)
{
	if(status >= AVRCP_REPEAT_MODE_NO)
	{
		ASSERT(FALSE);
	}
	MMI_AVRCP_SetPlayAppSettings(linkIndex, AVC_OPERATION_SET_REPEAT_MODE, status);
}

PUBLIC void SDK_AVRCP_KeyRepeatChange(U8 linkIndex)
{
	MMI_AVRCP_KeyRepeatModeChange(linkIndex);
}
#endif

#if SDK_AVRCP_KEYS
PUBLIC void SDK_AVRCP_KeyPlay(U8 linkIndex)
{
	MMI_AVRCP_KeyPlay(linkIndex);
}

PUBLIC void SDK_AVRCP_KeyStop(U8 linkIndex)
{
	MMI_AVRCP_KeyStop(linkIndex);
}

PUBLIC void SDK_AVRCP_KeyPause(U8 linkIndex)
{
	MMI_AVRCP_KeyPause(linkIndex);
}

PUBLIC void SDK_AVRCP_KeyRewindPush(U8 linkIndex)
{
	MMI_AVRCP_KeyRewindPush(linkIndex);
}

PUBLIC void SDK_AVRCP_KeyFastForwardPush(U8 linkIndex)
{
	MMI_AVRCP_KeyFastForwardPush(linkIndex);
}

PUBLIC void SDK_AVRCP_KeyForward(U8 linkIndex)
{
	MMI_AVRCP_KeyForward(linkIndex);
}

PUBLIC void SDK_AVRCP_KeyBackward(U8 linkIndex)
{
	MMI_AVRCP_KeyBackward(linkIndex);
}

PUBLIC void SDK_AVRCP_KeyRewindRelease(U8 linkIndex)
{
	MMI_AVRCP_KeyRewindRelease(linkIndex);
}

PUBLIC void SDK_AVRCP_KeyFastForwardRelease(U8 linkIndex)
{
	MMI_AVRCP_KeyFastForwardRelease(linkIndex);
}
#endif

#if SDK_AVRCP_PLAY_STATUS
PUBLIC void SDK_AVRCP_CB_PlayStatus(U8 linkIndex, U8 playStatus)
{
	switch(playStatus)
	{
		case AVRCP_PLAYBACK_STATUS_STOPPED:
			LightDbgPrint("SDK AVRCP[%d] playStatus: stopped",(U8)linkIndex);
			break;
		case AVRCP_PLAYBACK_STATUS_PLAYING:
			LightDbgPrint("SDK AVRCP[%d] playStatus: palying",(U8)linkIndex);
			break;
		case AVRCP_PLAYBACK_STATUS_PAUSED:
			LightDbgPrint("SDK AVRCP[%d] playStatus: paused",(U8)linkIndex);
			break;
		case AVRCP_PLAYBACK_STATUS_FWD_SEEK:
			LightDbgPrint("SDK AVRCP[%d] playStatus: FWD seek",(U8)linkIndex);
			break;
		case AVRCP_PLAYBACK_STATUS_REV_SEEK:
			LightDbgPrint("SDK AVRCP[%d] playStatus: REV seek",(U8)linkIndex);
			break;
		default:
			LightDbgPrint("SDK AVRCP[%d] playStatus: %x",(U8)linkIndex, (U8)playStatus);
			break;
	}
}

PUBLIC U8 SDK_AVRCP_CurrentPlayStatus(U8 linkIndex)
{
	return MMI_AVRCP_GET_PLAY_STATUS(linkIndex);
}
#endif

#if SDK_AVRCP_MEDIA_ATTRIBUTE
typedef struct
{
	U8 NumAttributes;
	AVRCP_ELEMENT_ATTRIBUTE	Attribute[1];
}SDK_AVRCP_MEDIA_ATTRIBUTE_TYPE;

PUBLIC void SDK_AVRCP_CB_MediaAttribute(U8 linkIndex, U8 XDATA_PTR mediaPtr)
{
	/* U8 XDATA_PTR mediaPtr will be put back to system in NDK. Do not use mediaPtr directly. */
	U8 i;
	SDK_AVRCP_MEDIA_ATTRIBUTE_TYPE XDATA_PTR attributePtr = (SDK_AVRCP_MEDIA_ATTRIBUTE_TYPE XDATA_PTR)mediaPtr;
	
	LightDbgPrint("SDK AVRCP media Attribute Num:%d",(U8)attributePtr->NumAttributes);
	for(i = 0; i < attributePtr->NumAttributes; i++)
	{
		switch((U8)(attributePtr->Attribute[i].AttributeID))
		{
			case AVRCP_MEDIA_TITLE:
				LightDbgPrint("SDK AVRCP_MEDIA_TITLE");
				break;
			case AVRCP_MEDIA_ARTIST_NAME:
				LightDbgPrint("SDK AVRCP_MEDIA_ARTIST_NAME");
				break;
			case AVRCP_MEDIA_ALBUM_NAME:
				LightDbgPrint("SDK AVRCP_MEDIA_ALBUM_NAME");
				break;
			case AVRCP_MEDIA_TRACK_NUMBER:
				LightDbgPrint("SDK AVRCP_MEDIA_TRACK_NUMBER");
				break;
			case AVRCP_MEDIA_TOTAL_TRACKS:
				LightDbgPrint("SDK AVRCP_MEDIA_TOTAL_TRACKS");
				break;
			case AVRCP_MEDIA_GENRE:
				LightDbgPrint("SDK AVRCP_MEDIA_GENRE");
				break;
			case AVRCP_MEDIA_PLAYING_TIME:
				LightDbgPrint("SDK AVRCP_MEDIA_PLAYING_TIME");
				break;		
		}
		mediaPtr += (attributePtr->Attribute[i].AttributeValueLength + 8);
		attributePtr = (SDK_AVRCP_MEDIA_ATTRIBUTE_TYPE XDATA_PTR)mediaPtr;
	}
	UNUSED(linkIndex);
}
#endif


PUBLIC void SDK_AVRCP_Init(void)
{
	#if SDK_AVRCP_PALY_APPLICATION
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_AVRCP_PLAY_APP_CHANGED, API_GRP_PROFILE, (VFUN)SDK_AVRCP_CB_PlayApplication);
	#endif
	#if SDK_AVRCP_PLAY_STATUS
	/* gMMI_nvram.mmiAvrcpSettings.notifyEventMask bit6 should be 1. */
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_AVRCP_PLAY_STATUS, API_GRP_PROFILE, (VFUN)SDK_AVRCP_CB_PlayStatus);
	#endif
	#if SDK_AVRCP_MEDIA_ATTRIBUTE
	/* gMMI_nvram.mmiAvrcpSettings.notifyEventMask bit1 should be 1. */
	/* gMMI_nvram.mmiAvrcpSettings.mediaEventMask should be set for media attributes. 
		#define MMI_AVRCP_MEDIA_TITLE				0x01
		#define MMI_AVRCP_MEDIA_ARTIST_NAME			0x02
		#define MMI_AVRCP_MEDIA_ALBUM_NAME			0x04
		#define MMI_AVRCP_MEDIA_TRACK_NUMBER		0x08
		#define MMI_AVRCP_MEDIA_TOTAL_TRACKS		0x10
		#define MMI_AVRCP_MEDIA_GENRE				0x20
		#define MMI_AVRCP_MEDIA_PLAYING_TIME		0x40
	*/
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_AVRCP_MEDIA_ATTRIBUTE, API_GRP_PROFILE, (VFUN)SDK_AVRCP_CB_MediaAttribute);
	#endif
}

