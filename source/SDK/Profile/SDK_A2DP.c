#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#if SDK_A2DP
#include "..\A2DP\A2DP_Interface.h"
#include "..\MMI\MMI_A2DP.h"


PUBLIC BOOL SDK_A2DP_IsInStreaming(U8 linkIndex)
{
	return MMI_A2DP_IsInStreaming(linkIndex);
}

PRIVATE void SDK_A2DP_StatusChanged(U8 linkIndex, U8 newStatus)
{
	switch(newStatus)
	{
		case PROFILE_DISCONNECTED:
			LightDbgPrint("SDK A2DP[%d]: PROFILE_DISCONNECTED", (U8)linkIndex);
			break;
		case PROFILE_CONNECTING:
			LightDbgPrint("SDK A2DP[%d]: PROFILE_CONNECTING", (U8)linkIndex);
			break;
		case PROFILE_CONNECTED:
			LightDbgPrint("SDK A2DP[%d]: PROFILE_CONNECTED", (U8)linkIndex);
			break;
		case PROFILE_DISCONNECTING:
			LightDbgPrint("SDK A2DP[%d]: PROFILE_DISCONNECTING", (U8)linkIndex);
			break;
		case A2DP_CONFIGURED:
			LightDbgPrint("SDK A2DP[%d]: A2DP_CONFIGURED", (U8)linkIndex);
			break;
		case A2DP_OPEN:
			LightDbgPrint("SDK A2DP[%d]: A2DP_OPEN", (U8)linkIndex);
			break;
		case A2DP_STREAMING:
			LightDbgPrint("SDK A2DP[%d]: A2DP_STREAMING", (U8)linkIndex);
			break;
		case A2DP_CLOSING:
			LightDbgPrint("SDK A2DP[%d]: A2DP_CLOSING", (U8)linkIndex);
			break;
		case A2DP_ABORTING:
			LightDbgPrint("SDK A2DP[%d]: A2DP_ABORTING", (U8)linkIndex);
			break;	
	}
}

U8 XDATA gSDK_A2DP_status[MAX_MULTI_POINT_NO];
PRIVATE void SDK_A2DP_CheckStatus(void)
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		if(gSDK_A2DP_status[linkIndex] != A2DP_GetState(linkIndex))
		{
			gSDK_A2DP_status[linkIndex] = A2DP_GetState(linkIndex);
			SDK_A2DP_StatusChanged(linkIndex, gSDK_A2DP_status[linkIndex]);
		}
	}
}

PUBLIC void SDK_A2DP_CB_AudioStatus(U8 linkIndex, U8 status)
{
	if(status)
	{
		LightDbgPrint("A2DP Audio is On",(U8)linkIndex);
	}
	else
	{
		LightDbgPrint("A2DP Audio is Off",(U8)linkIndex);
	}
}

PUBLIC BOOL SDK_A2DP_IsAudioOn(U8 linkIndex)
{
	return MMI_A2DP_IsAudioOn(linkIndex);
}

PUBLIC void SDK_A2DP_Polling(void)
{
	SDK_A2DP_CheckStatus();
}
#endif


PUBLIC void SDK_A2DP_Init(void)
{
	#if SDK_A2DP
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_A2DP_AUDIO_STATUS, API_GRP_PROFILE, (VFUN)SDK_A2DP_CB_AudioStatus);
	#endif
}