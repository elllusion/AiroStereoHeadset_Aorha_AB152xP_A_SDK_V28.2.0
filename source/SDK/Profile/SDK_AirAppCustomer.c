#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"
#include "..\MMI\MMI_Base.h"

#include "..\SDK\include\SDK_AirAppCustomer.h"
#include "..\\AirApp\AirApp_Interface.h"

/*									SAMPLE CODE START											*/
#define SAMPLE_LEN		1
PRIVATE void  SDK_AIRAPP_CST_Sample1(U8 linkIndex, U8 vOcf,U8 dataLen, U8 XDATA_PTR dataPtr)
{
	U8 XDATA_PTR rspPtr;

	UNUSED(dataPtr);
	UNUSED(dataLen);
	rspPtr = (U8 XDATA_PTR) SYS_MemoryGet(SAMPLE_LEN);

	if(rspPtr != (U8 XDATA_PTR)NULL)
	{
		#ifdef AIRAPP_Profile
		AirApp_CustomerDataRsp(linkIndex, vOcf, RSP_SUCCESS, SAMPLE_LEN, rspPtr);
		#else
		UNUSED(linkIndex);
		UNUSED(vOcf);
		#endif
		OSMEM_Put(rspPtr);
	}
}
/*									SAMPLE CODE END												*/

PUBLIC void SDK_AirAppCustomerCMDHandler(U8 linkIndex, U8 XDATA_PTR msgPtr)
{
	U8 vOcf;
	U8 dataLen;
	MMIMsgType XDATA_PTR airCmdPtr = (MMIMsgType XDATA_PTR)msgPtr;

	if(airCmdPtr == (MMIMsgType XDATA_PTR) NULL)
		return;
	
	vOcf = airCmdPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf;
	dataLen = airCmdPtr->paraLen - sizeof(HCI_VCMD_OPCODE_STRU);
	
	switch(vOcf)
	{
		/*											SAMPLE CODE START														*/
		case SDK_AIRAPP_CST_SAMPLE1:
			SDK_AIRAPP_CST_Sample1(linkIndex, vOcf, dataLen, (U8 XDATA_PTR)airCmdPtr->msgBodyPtr.hciVCmd.VPara.dataBegin);
			break;
		/*											SAMPLE CODE END															*/
		default:
			#ifdef AIRAPP_Profile
			AirApp_CustomerDataRsp(linkIndex, vOcf, RSP_FAIL, 0, (U8 XDATA_PTR)NULL);
			#endif
			break;
	}
}

PUBLIC void SDK_AirAppCustomerInit(void)
{
	#if SDK_AIRAPP_CUSTOMER
	SDK_Proxy_RegisterAPI(API_GRP_PF_CB_AIRAPP_CUSTOMER_CTL, API_GRP_PROFILE, (VFUN)SDK_AirAppCustomerCMDHandler);
	#endif
}