#include "reside_flash.inc"

#define _MMI_INQUIRY_C_

#include "rc.h"
#include "bt_config_profile.h"

#ifdef MMI_INQUIRY_CMD
#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Protocol.h"
#include "MMI_HCI.h"
#include "MMI_Driver.h"


U8 MMI_INQ_StartInquiry(MMIMsgType XDATA_PTR msgPtr)
{
	U8 inquiry_type = msgPtr->msgBodyPtr.hciVCmd.VPara.inquiryVEvtCmd.inquiryType;
	if(MMI_CheckNestState(MMI_EOF, MMI_INQUIRY))
	{
		return TRUE;//cannot enter inquiry again
	}
	else
	{
		MMI_AddTopState(MMI_EOF, MMI_INQUIRY);
		MMI_HCI_SendInquiry(inquiry_type);		
		return FALSE;
	}	
}

PUBLIC void MMI_INQ_GetInquiryComplete(void)
{
	if(MMI_CheckNestState(MMI_EOF, MMI_INQUIRY))
	{
		MMI_RmvState(MMI_EOF, MMI_INQUIRY);
	}
}

PUBLIC void MMI_INQ_GetInquiryResultWithRSSI(U8 XDATA_PTR dataPtr)
{
	MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_INQUIRY_RESULT_WITH_RSSI, dataPtr, MMI_EOF);
}

PUBLIC void MMI_INQ_GetExtendedInquiryResult(U8 XDATA_PTR dataPtr)
{
	MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_EXTENDED_INQUIRY_RESULT, dataPtr, MMI_EOF);
}

#endif //MMI_INQUIRY


