#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#if SDK_VENDOR_AT_CMD
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_HFP.h"
#include "..\HFP\HFP_Interface.h"
#include "..\HFP\HFP.h"
#include "..\SDK\include\SDK_HFP.h"


#define SDK_HFP_RESULT_CODE_MAX_LENGTH 	10

enum
{
	SDK_HFP_RESULT_XEVENT,
	SDK_HFP_RESULT_NUM,
};

static U8 CODE SDK_HFP_result_code[SDK_HFP_RESULT_NUM][SDK_HFP_RESULT_CODE_MAX_LENGTH] = 
{
	{"+XEVENT"},
};
PRIVATE U8 SDK_AT_MatchResultCode (U8 XDATA_PTR ptr1)
{
	U8 i;
	
	while(*ptr1 == AT_CR || *ptr1 == AT_LF)
	{
		ptr1++;
	}
	
	for (i = 0; i < SDK_HFP_RESULT_NUM; i++)
	{
		if (AT_CompareString (ptr1, &SDK_HFP_result_code[i][0], SDK_HFP_RESULT_CODE_MAX_LENGTH) == 0)
		{
			return i;
		}		
	}
	
	return AT_EOF;
}

PUBLIC void SDK_HFP_AT_ResultCode(U8 linkIndex, U8 XDATA_PTR resultATPtr)
{
	/* U8 XDATA_PTR resultATPtr will be put back to system in NDK. Do not use resultATPtr directly. */
	U8 resultCode;
	
	resultCode = SDK_AT_MatchResultCode(resultATPtr);
	if(resultCode == SDK_HFP_RESULT_XEVENT)
	{
		LightDbgPrint("SDK - [%d] Get +XEVENT",(U8)linkIndex);
	}
}


static U8 CODE SDK_HFP_at_xevent[]=
{
	14/*length of at command string(AT_CR is included)*/, 'A','T','+','X','E','V','E','N','T','=','5',',','1',AT_CR
};


PUBLIC void SDK_HFP_SendATCmd(U8 linkIndex, U8 cmdCode)
{
	MMIMsgType XDATA_PTR msgPtr;

	if((msgPtr = MMI_GetCommandMessageMem(cmdCode)) == (MMIMsgType XDATA_PTR)NULL)
		return;

	if(MMI_HFP_IsHSPConnected(linkIndex))
	{
		if(cmdCode != HFP_ATCMD_ADJUST_MIC_GAIN && cmdCode != HFP_ATCMD_ADJUST_VOLUME)
		{
			msgPtr->msgOpcode = cmdCode = HFP_ATCMD_CKPD_200;
		}
	}
	else
	{
		switch(cmdCode)
		{
			case SDK_HFP_CMD_XEVENT:
				msgPtr->msgBodyPtr.hfpCmd.vendorCmd.atStringLen = SDK_HFP_at_xevent[0];
				OSMEM_memcpy_xdata_code(&msgPtr->msgBodyPtr.hfpCmd.vendorCmd.atString, &SDK_HFP_at_xevent[1], SDK_HFP_at_xevent[0]);
				LightDbgPrint("SDK_HFP_CMD_XEVENT:%d",(U8)linkIndex);
				break;
			default:
				OSMEM_Put((U8 XDATA_PTR)msgPtr);
				return;
		}
	}
	HFP_CommandHandle(linkIndex, cmdCode, msgPtr);
}

PUBLIC void SDK_HFP_AT_ResultCodeOK(U8 linkIndex, U8 cmdCode)
{
	if(cmdCode == SDK_HFP_CMD_XEVENT)
	{
		LightDbgPrint("SDK - Get AT+XEVENT:OK!!");
	}
	UNUSED(linkIndex);
}

PUBLIC void SDK_HFP_AT_ResultCodeError(U8 linkIndex, U8 cmdCode)
{
	if(cmdCode == SDK_HFP_CMD_XEVENT)
	{
		LightDbgPrint("SDK - Get AT+XEVENT:ERROR!!");
	}
	UNUSED(linkIndex);
}
#endif

PUBLIC void SDK_HFP_Init(void)
{
	#if SDK_VENDOR_AT_CMD
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HFP_AT_RESULT_CODE, API_GRP_PROFILE, (VFUN)SDK_HFP_AT_ResultCode);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HFP_AT_RESULT_CODE_OK, API_GRP_PROFILE, (VFUN)SDK_HFP_AT_ResultCodeOK);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HFP_AT_RESULT_CODE_ERROR, API_GRP_PROFILE, (VFUN)SDK_HFP_AT_ResultCodeError);
	#endif
}