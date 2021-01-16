#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "drv_uart_transparent.h"
#include "..\MMI\MMI_Base.h"
#include "hci.h"
#include "hci_airoha.h"
#include "HC_Interface.h"

#define SDK_UART_CUST_CMD	0


#define HCI_VENDOR_OGF_CUSTOMER_CMD							(HCI_VOGF)0x58
#define HCI_VENDOR_OGF_CUSTOMER_RESP						(HCI_VOGF)0x59
#define HCI_VENDOR_OGF_CUSTOMER_IND_UNSOLICITED				(HCI_VOGF)0x5A
#define HCI_VENDOR_OGF_CUSTOMER_IND_SOLICITED				(HCI_VOGF)0x5B

PUBLIC void SDK_CB_UART_Cust_SendUARTResponse(U8 cmdOpcode, U8 resultCode)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + sizeof(HCI_VCMD_PARA_AIR_CMD_RESP);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = cmdOpcode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_CUSTOMER_RESP;
		msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmd.resultCode = resultCode;
		OSMQ_Put(OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC MMIMsgType XDATA_PTR SDK_CB_UART_Cust_SolicitedInd(U8 opCode, U16 vParaLength)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if(msgPtr = (MMIMsgType XDATA_PTR)SYS_MemoryGet(vParaLength + sizeof(HCI_VCMD_OPCODE_STRU) + 5))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, vParaLength + sizeof(HCI_VCMD_OPCODE_STRU) + 5);

		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->msgOpcode =  HCI_VCMD_AIROHA_TO_UART;
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + vParaLength;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_CUSTOMER_IND_SOLICITED;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = opCode;
	}
	return msgPtr;
}

PUBLIC void SDK_CB_UART_Cust_UnSolicitedInd(U8 opCode, U8 XDATA_PTR dataPtr, U8 linkIndex)
{
	U8 XDATA_PTR tempPtr;
	MMIMsgType XDATA_PTR msgPtr;

	if (!MMI_CC_UART_ENABLE)
		return;

	if(msgPtr = MMI_GetCommandMessageMem( HCI_VCMD_AIROHA_TO_UART ))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU); // sizeof(opCode + ogf)
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = opCode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_CUSTOMER_IND_UNSOLICITED;
		tempPtr = (U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara;
		UNUSED(dataPtr);
		UNUSED(linkIndex);
		OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

#if SDK_UART_CUST_CMD
PRIVATE void SDK_CB_UART_Cust_Command(MMIMsgType XDATA_PTR msgPtr)
{
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}
#endif

PUBLIC void SDK_UART_Init(void)
{
#if SDK_UART_CUST_CMD
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_UART_CUST_CMD, 	API_GRP_IOSYS, (VFUN)SDK_CB_UART_Cust_Command);
#endif
}

