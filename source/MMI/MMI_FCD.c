#include "reside_flash.inc"

#define __MMI_FCD_C__

#include "rc.h"
#include "bt_config_profile.h"
#include "..\MMI_LE\MMI_LE_interface.h"
#include "..\L2CAP\L2CAP_interface.h"
#include "MMI_FCD.h"
#include "MMI_Base.h"
#include "MMI_Driver.h"
#include "..\COMMON\align_flash.h"

#ifdef FAST_CONNECT_DONGLE
#include "MMI_HCI.h"
#include "MMI_Protocol.h"
#include "MMI_Reconnect.h"
#include "..\HC\HC_Interface.h"

PUBLIC void MMI_FCD_ReportDevAddr(U8 XDATA_PTR bdAddr)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if (msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + sizeof(BD_ADDR);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VEVT_OCF_FCD_DEV_ADDR;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_FCD;
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.dataBegin, bdAddr, sizeof(BD_ADDR));
		
		OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
	}		
}

PUBLIC void MMI_FCD_ReportDevName(U8 XDATA_PTR name, U8 maxLen)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if (msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + maxLen;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VEVT_OCF_FCD_DEV_NAME;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_FCD;
		OSMEM_memcpy_xdata_xdata(&msgPtr->msgBodyPtr.hciVCmd.VPara.dataBegin, name, maxLen);

		OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void MMI_FCD_ReportDevRSSI(U8 rssi)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if (msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + 1;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VEVT_OCF_FCD_DEV_RSSI;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_FCD;
		msgPtr->msgBodyPtr.hciVCmd.VPara.dataBegin = rssi;

		OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
	}		
}

PUBLIC void MMI_FCD_ReportAction(U8 opCode)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if (msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = opCode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_FCD;
		
		OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
	}	
}

PUBLIC void MMI_FCD_ReportVolume(U8 opCode, U8 vol)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if (msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + 1;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = opCode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_FCD;
		msgPtr->msgBodyPtr.hciVCmd.VPara.dataBegin = vol;
		
		OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
	}	
}

PUBLIC void MMI_FCD_SendInquiry(void)
{
	gMMI_FCD_ctl.found_rssi = 0;
	MMI_HCI_SendInquiry(INQUIRY_GIAC);
}

PUBLIC void MMI_FCD_GetInquiryComplete(void)
{
	if (gMMI_FCD_ctl.found_rssi)
	{
		U8 linkIndex;
		if((linkIndex = MMI_GetEmptyLinkIndex((U8 XDATA_PTR)&gMMI_FCD_ctl.found_bd_addr, TRUE)) < MAX_MULTI_POINT_NO)
		{
			gMMI_FCD_ctl.found_rssi = 0;
			MMI_HCI_SendCreateConnection (	linkIndex,
											(U8 XDATA_PTR)&gMMI_FCD_ctl.found_bd_addr, 
											RECONNECT_TYPE_NULL, 
											(U8 XDATA_PTR)&gMMI_FCD_ctl.found_classOfDev);
		}
	}
	else
	{
		MMI_HCI_SendInquiry(INQUIRY_GIAC);
	}
}

PUBLIC void MMI_FCD_GetInquiryResultWithRSSI(U8 rssi, U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR codPtr)
{
	if (rssi > gMMI_FCD_ctl.found_rssi)
	{
		OSMEM_memcpy_xdata_xdata (	(U8 XDATA_PTR)&gMMI_FCD_ctl.found_bd_addr,
									bdAddrPtr,
									sizeof(BD_ADDR));
		OSMEM_memcpy_xdata_xdata (	(U8 XDATA_PTR)&gMMI_FCD_ctl.found_classOfDev,
									codPtr,
									3);
		gMMI_FCD_ctl.found_rssi = rssi;
	}
}

#endif //FAST_CONNECT_DONGLE

#ifdef AUDIO_BIST
PRIVATE void MMI_FCD_SendAudioBistResult(U8 SignalPower, U8 SNDR)
{
	MMI_FCD_REMOTE_AUDIOBIST_STRU XDATA_PTR distPtr;
	
	if((distPtr = (MMI_FCD_REMOTE_AUDIOBIST_STRU XDATA_PTR)SYS_MemoryGet(sizeof(MMI_FCD_REMOTE_AUDIOBIST_STRU))) != NULL)
	{
		distPtr->fcdHeader.packetType = FCD_EVT;
		distPtr->fcdHeader.cmdEvt = FCD_CMD_AUDIOBIST;
		distPtr->fcdHeader.errorCode = FCD_NO_ERROR;
		
		distPtr->signalPower = SignalPower;
		distPtr->SNDR = SNDR;
		
		L2CAP_SendFCDChannelData(gMMI_FCD_ctl.audioBistLinkIndex, (U8 XDATA_PTR)distPtr, (U16)sizeof(MMI_FCD_REMOTE_AUDIOBIST_STRU));
		OSMEM_Put(distPtr);
	}
}

PUBLIC void MMI_FCD_AudioBistResult()
{
	MMI_FCD_SendAudioBistResult(gMMI_FCD_ctl.audioBistSNP, gMMI_FCD_ctl.audioBistSNDR);
}

extern PUBLIC void MMI_DRV_AudioBistVendEvtHandler(VOID);
PUBLIC void MMI_FCD_AudioBistControl()
{
	#define AVERAGE_COUNT 10
	U8 cnt;
	U32 signalPower, SNDR;
	
	if(!gMMI_FCD_ctl.remoteAudioBist)
		return;
	
	signalPower = SNDR = 0;
	for(cnt = 0; cnt < AVERAGE_COUNT; cnt++)
	{
		SYS_SetTimer(&gMMI_FCD_ctl.remote_audioBist_timer, 2*32L); // 20 ms, DSP updates value every 16 ms
		while(!SYS_IsTimerExpired(&gMMI_FCD_ctl.remote_audioBist_timer));
		
		OS_ENTER_MUTEX_CRITICAL();
		signalPower += ENDIAN_TRANSFORM_U16(gAlignDSP.IC_BIST_SIGNSE_POWER);
		SNDR += ENDIAN_TRANSFORM_U16(gAlignDSP.IC_BIST_SNDR_VAL);
		OS_EXIT_MUTEX_CRITICAL();
	}

	gMMI_FCD_ctl.audioBistSNP = (U8)(signalPower/(U32)AVERAGE_COUNT);
	gMMI_FCD_ctl.audioBistSNDR = (U8)(SNDR/(U32)AVERAGE_COUNT);
	//MMI_FCD_SendAudioBistResult((U8)(signalPower/(U32)AVERAGE_COUNT), (U8)(SNDR/(U32)AVERAGE_COUNT));
}

PRIVATE void MMI_FCD_SendAudioBISTCmd(U8 linkIndex, MMI_FCD_REMOTE_AUDIOBIST_STRU XDATA_PTR dataPtr)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	gMMI_FCD_ctl.audioBistLinkIndex = linkIndex;
	gMMI_FCD_ctl.remoteAudioBist = TRUE;
	if((msgPtr = MMI_GetEventMessageMem(HCI_EVENT_BIST_CMD)) != NULL)
	{
		msgPtr->paraLen = sizeof(HciBISTCmd);
		msgPtr->msgBodyPtr.hciBISTCmd.cmdOpCode = HCI_VCMD_OCF_RF_BIST_AUDIO_BIST;
		msgPtr->msgBodyPtr.hciBISTCmd.ogf = HCI_VENDOR_OGF_BIST;
		
		msgPtr->msgBodyPtr.hciBISTCmd.AudioIn = dataPtr->audioIn;
		msgPtr->msgBodyPtr.hciBISTCmd.AudioOut = dataPtr->audioOut;
		msgPtr->msgBodyPtr.hciBISTCmd.BISTScenario = dataPtr->audioBistMode;
		
		MMI_DRV_AudioBistHCICommand(msgPtr);
	}
}
#endif //AUDIO_BIST

PRIVATE void MMI_FCD_SendBleBdAddr(U8 linkIndex)
{
    #ifdef LE_SUPPORTED_HOST_FLASH
	MMI_FCD_BDADDR_STRU XDATA_PTR distPtr;
	
	if((distPtr = (MMI_FCD_BDADDR_STRU XDATA_PTR)SYS_MemoryGet(sizeof(MMI_FCD_BDADDR_STRU))) != NULL)
	{
		distPtr->fcdHeader.packetType = FCD_EVT;
		distPtr->fcdHeader.cmdEvt = FCD_CMD_BLEBDADDR_REQUEST;
		distPtr->fcdHeader.errorCode = (MMI_LE_GetLeBdAddr((MMI_LE_BD_ADDR XDATA_PTR)distPtr->bdAddr.member)) ? FCD_NO_ERROR : FCD_NOT_SUPPORT_BLE;
		
		L2CAP_SendFCDChannelData(linkIndex, (U8 XDATA_PTR)distPtr, (U16)sizeof(MMI_FCD_BDADDR_STRU));
		OSMEM_Put(distPtr);
	}
    #else
    UNUSED(linkIndex);
    #endif
}

PRIVATE void MMI_FCD_CmdHandler(U8 linkIndex,  U8 XDATA_PTR dataPtr)
{
	switch(((MMI_FCD_STRU XDATA_PTR)dataPtr)->cmdEvt)
	{
		case FCD_CMD_BLEBDADDR_REQUEST:
			if(((MMI_FCD_STRU XDATA_PTR)dataPtr)->errorCode == FCD_NO_ERROR)
				MMI_FCD_SendBleBdAddr(linkIndex);
			break;
		
		#ifdef AUDIO_BIST
		case FCD_CMD_AUDIOBIST:
			if(((MMI_FCD_STRU XDATA_PTR)dataPtr)->errorCode == FCD_NO_ERROR)
				MMI_FCD_SendAudioBISTCmd(linkIndex, (MMI_FCD_REMOTE_AUDIOBIST_STRU XDATA_PTR)dataPtr);
			break;
		#endif
	}
}

PRIVATE void MMI_FCD_EvtHandler(U8 linkIndex,  U8 XDATA_PTR dataPtr)
{
	UNUSED(linkIndex);
	UNUSED(dataPtr);
}

PUBLIC void MMI_FCD_CmdEvtHandler(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	switch(((MMI_FCD_STRU XDATA_PTR)dataPtr)->packetType)
	{
		case FCD_CMD:
			MMI_FCD_CmdHandler(linkIndex, dataPtr);
			break;
		
		case FCD_EVT:
			MMI_FCD_EvtHandler(linkIndex, dataPtr);
			break;
		
		default:
			break;
	}
}