#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"

#if SDK_HCI
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_HCI.h"
#include "..\ROM\HC\HC_interface.h"
#include "..\SDK\include\SDK_HCI.h"

/*
	Inquiry mode can be decide in sector_hc_paramter_type.c
*/
PUBLIC U8 SDK_HCI_InquiryCmd(U8 XDATA_PTR LapPtr, U8 inquiryLen, U8 numResps)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(MMI_HCI_IsInquiryOnGoing())
		return HCI_CMD_BUSY;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_INQUIRY)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_INQ_STRU); //5;		
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.inquiryCmd.Lap[0], LapPtr, sizeof(LAP_ARR));
		msgPtr->msgBodyPtr.hciCmd.inquiryCmd.InquiryLength = inquiryLen; //inquiryLen * 1.28 Second
		msgPtr->msgBodyPtr.hciCmd.inquiryCmd.NumResponses = numResps; // numResps
		MMI_HCI_SendCmd(msgPtr);

		MMI_HCI_SetInquiryOnGoing(INQUIRY_BUSY_SDK);
		LightDbgPrint("SDK Send Inquiry Cmd");
		return HCI_CMD_OK;
	}
	return HCI_CMD_FAIL;
}

PUBLIC void SDK_HCI_InquiryCmdStatus(U8 status)
{
	MMI_HCI_SetInquiryOnGoing((status != STATUS_OK) ? INQUIRY_IDLE : INQUIRY_OK_SDK);
}

PUBLIC void SDK_HCI_InquiryCplEvt(U8 status)
{
	MMI_HCI_SetInquiryOnGoing(INQUIRY_IDLE);
	UNUSED(status);
	LightDbgPrint("SDK Get InquiryCpl");
}

PUBLIC void SDK_HCI_InquiryResultEvt(U8 XDATA_PTR evtPtr)
{
	//HCI_EVT_PARA_INQ_RST_STRU XDATA_PTR paraPtr = (HCI_EVT_PARA_INQ_RST_STRU XDATA_PTR)evtPtr;
	UNUSED(evtPtr);
	LightDbgPrint("SDK Get InquiryResult");
}

PUBLIC void SDK_HCI_InquiryResultRSSIEvt(U8 XDATA_PTR evtPtr)
{
	//HCI_EVT_PARA_INQ_RST_WITH_RSSI_STRU XDATA_PTR paraPtr = (HCI_EVT_PARA_INQ_RST_WITH_RSSI_STRU XDATA_PTR)evtPtr;
	UNUSED(evtPtr);
	LightDbgPrint("SDK Get InquiryResultRSSI");
}

PUBLIC void SDK_HCI_ExtInquiryResultEvt(U8 XDATA_PTR evtPtr)
{
	//HCI_EVT_PARA_EXT_INQ_RST_STRU XDATA_PTR paraPtr = (HCI_EVT_PARA_EXT_INQ_RST_STRU XDATA_PTR)evtPtr;
	UNUSED(evtPtr);
	LightDbgPrint("SDK Get ExtInquiryResult");
}

PUBLIC void SDK_HCI_InquiryCancelCmd(void)
{
	if(INQUIRY_OK_SDK == MMI_HCI_GetInquiryState())
		MMI_HCI_SendInquiryCancel();
}

PUBLIC void SDK_HCI_InquiryCancelCmdCplEvt(U8 status)
{
	MMI_HCI_SetInquiryOnGoing(INQUIRY_IDLE);
	UNUSED(status);
}


PUBLIC BOOL SDK_HCI_IsRoleMaster(U8 linkIndex)
{
	return MMI_IsRoleOfMaster(linkIndex);
}

PUBLIC BOOL SDK_HCI_IsRoleSlave(U8 linkIndex)
{
	return MMI_IsRoleOfSlave(linkIndex);
}

PUBLIC void SDK_HCI_SwitchRoleCmd(U8 linkIndex, U8 role)
{
	if(role == SDK_ROLE_MASTER && SDK_HCI_IsRoleMaster(linkIndex))
		return;
		
	if(role == SDK_ROLE_SLAVE && SDK_HCI_IsRoleSlave(linkIndex))
		return;

	MMI_HCI_SendSwitchRole(linkIndex, (role == SDK_ROLE_SLAVE)?ROLE_SLAVE:ROLE_MASTER);
}

PUBLIC void SDK_HCI_RoleChangedEvt(U8 linkIndex, U8 status, U8 role)
{
	if(role == ROLE_MASTER)
	{
		LightDbgPrint("SDK RoleChangedEvtM:%d,%d",(U8)linkIndex, (U8)status);
	}
	else
	{
		LightDbgPrint("SDK RoleChangedEvtS:%d,%d",(U8)linkIndex, (U8)status);
	}
	UNUSED(linkIndex);
	UNUSED(status);
	UNUSED(role);
}

PUBLIC void SDK_HCI_WritePageTimeoutEvt(U16 page_timeout)
{
	MMI_HCI_SendWritePageTimeOut(page_timeout);
}
#endif


PUBLIC void SDK_HCI_Init(void)
{
#if SDK_HCI
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HCI_INQUIRY_CMD_STATUS, API_GRP_PROFILE, (VFUN)SDK_HCI_InquiryCmdStatus);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HCI_INQUIRY_CPL_EVT, API_GRP_PROFILE, (VFUN)SDK_HCI_InquiryCplEvt);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HCI_INQUIRY_RESULT_EVT, API_GRP_PROFILE, (VFUN)SDK_HCI_InquiryResultEvt);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HCI_INQUIRY_RESULT_RSSI_EVT, API_GRP_PROFILE, (VFUN)SDK_HCI_InquiryResultRSSIEvt);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HCI_EXT_INQUIRY_RESULT_EVT, API_GRP_PROFILE, (VFUN)SDK_HCI_ExtInquiryResultEvt);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HCI_INQ_CANCEL_CMD_CPL_EVT, API_GRP_PROFILE, (VFUN)SDK_HCI_InquiryCancelCmdCplEvt);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HCI_ROLE_CHANGED_EVT, API_GRP_PROFILE, (VFUN)SDK_HCI_RoleChangedEvt);
	SDK_Proxy_RegisterAPI( API_GRP_PF_CB_HCI_WRITE_PAGE_TIMEOUT_EVT, API_GRP_PROFILE, (VFUN)SDK_HCI_WritePageTimeoutEvt);
#endif
}