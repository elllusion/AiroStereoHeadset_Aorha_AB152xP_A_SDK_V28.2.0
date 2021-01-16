#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#define SDK_PROFILE_CONNECTED		TRUE
#define SDK_PROFILE_DISCONNECTED	FALSE

#if SDK_PROFILE

#if SDK_HFP_AT_XEVENT_SAMPLE_CODE
#include "..\SDK\include\SDK_HFP.h"
#include "..\MMI\MMI_SCN.h"
#endif

PUBLIC void SDK_Profile_ConnectStatus(U8 linkIndex, U8 profileID, U8 status)
{
	UNUSED(linkIndex);
	if(status == SDK_PROFILE_CONNECTED)
	{
		LightDbgPrint("SDK Profile[%x] is connected",(U8)profileID);
		#if SDK_HFP_AT_XEVENT_SAMPLE_CODE
		if(profileID == PROFILE_HANDSFREE)
		{
			SDK_HFP_SendATCmd(linkIndex, SDK_HFP_CMD_XEVENT);
		}
		#endif
	}
	else if(status == SDK_PROFILE_DISCONNECTED)
	{
		LightDbgPrint("SDK Profile[%x] is disconnected",(U8)profileID);
	}
}
#endif

PUBLIC void SDK_Profile_Init(void)
{
	#if SDK_PROFILE
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_CONNECTION_STATUS, API_GRP_PROFILE, (VFUN)SDK_Profile_ConnectStatus);
	#endif
}