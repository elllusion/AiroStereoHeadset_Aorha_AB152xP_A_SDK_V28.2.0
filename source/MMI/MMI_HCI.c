//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _MMI_HCI_C_

#include "os.h"
#include "bt_config_profile.h"

#include "align_flash.h"
#include "os_flash.h"
#include "MMI.h"
#include "MMI_Base.h"
#include "MMI_Protocol.h"
#include "MMI_State.h"
#include "MMI_Driver.h"
#include "MMI_SDAP.h"
#include "MMI_Timer.h"
#include "MMI_HCI.h"
#include "MMI_AVRCP.h"
#include "MMI_A2DP.h"
#include "MMI_HFP.h"
#include "MMI_AirModule.h"
#include "MMI_ChgBat.h"
#include "MMI_Reconnect.h"
#include "MMI_LineIn.h"
#include "MMI_SectorManager.h"
#ifdef FAST_CONNECT_DONGLE
#include "MMI_FCD.h"
#endif
#ifdef SPP_Profile
#include "..\SPP\SPP_interface.h"
#endif
#ifdef SUPPORT_LE_XPORT
#include "..\LE_XPORT\le_xport_interface.h"
#endif
#ifdef OLED_ENABLE
#include "..\Driver\OLED.h"
#endif
#include "..\A2DP\A2DP_interface.h"
#include "..\L2CAP\L2CAP_Interface.h"
#include "..\HC\HC_Interface.h"
#include "..\GAP\GAP_interface.h"
#include "drivers_flash.h"
#include "sector_gap_nvram_type.h"
#include "sector_tool_misc.h"
#include "..\LM\LM_Interface.h"
#include "..\RFCOMM\RFCOMM_Interface.h"
#include "..\AVCTP\AVCTP_Interface.h"

#include "I2C.h"

#ifdef TRSPX_App
#include "MMI_TrspXport.h"
#endif

#ifdef IAP2_Profile
#include "..\iAP2\iAP2_interface.h"
#endif

#ifdef AIRAPP_Profile
#include "..\AirApp\AirApp_Interface.h"
#endif

#ifdef PEQ_ENABLE
#include "..\Driver\Peq_HPF.h"
#endif

#ifdef MMI_INQUIRY_CMD
#include "MMI_Inquiry.h"
#endif

#ifdef HID_Profile
#include "MMI_HID.h"
#endif

#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_iosys_evt_func[API_GRP_IOSYS_CB_CNT];
extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];

#define MMI_CMD_PARA_PAGE_SCAN_MODE_R1		0x01

PUBLIC void MMI_HCI_AirohaCmd_SendResponse(U8 cmdOpcode, U8 resultCode);
PUBLIC void MMI_HCI_AirohaCmd_ExtSendResponse(U8 cmdOpcode, U16 extendOCF, U8 resultCode);
PRIVATE void MMI_HCI_ConnectionReq(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_DisconnectionComplete(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_CommandComplete(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_ConnectionComplete(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_SCOConnectionChange(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_PinCodeRequest(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_LinkKeyRequest(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_LinkKeyNotification(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AuthenticationComplete(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_EncryptionChange(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_QosSetupComplete(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_CommandStatus(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_InquiryResult(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_InquiryResultwithRSSI(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_ExtendedInquiryResult(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_InquiryComplete(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_ModeChange(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_IOCapabilityRequest(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_IOCapabilityResponse(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_UserConfirmationRequest(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_RoleChange(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_SCOPacketType(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_NumberOfCompletePktEvt(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_VendorEvt(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_ToolCommand(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_UARTCommand(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_ExtUARTCommand(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_SPPCommand(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_GAP_WriteSecurityModeComplete(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_GAP_WriteConnectabilityModeComplete(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_GAP_WriteDiscoverabilityModeComplete(MMIMsgType XDATA_PTR msgPtr);

PRIVATE void MMI_ScanHandlerWhenSCO(BOOL isStart);
PRIVATE void MMI_HCI_AcceptSCOReqProfileHandle(U8 linkIndex);
PRIVATE void MMI_HCI_ScoDiscHandle(U8 linkIndex);
PRIVATE void MMI_HCI_ScoDiscProfileHandle(U8 linkIndex);
PRIVATE void MMI_HCI_ScoConnOKHandle(U8 linkIndex);
PRIVATE void MMI_HCI_ScoConnOKProfileHandle(U8 linkIndex);
PRIVATE void MMI_HCI_ScoConnFailHandle(U8 linkIndex);
PRIVATE void MMI_HCI_ScoConnFailProfileHandle(U8 linkIndex);
PRIVATE void MMI_HCI_AclDiscHandle(U8 linkIndex);
PRIVATE void MMI_HCI_RemoveSCOAudioLink(U8 linkIndex);
PRIVATE void MMI_HCI_AddSCOAudioLink(U8 linkIndex);

#ifdef TRSPX_App
PRIVATE void MMI_HCI_SendKeypressNtf(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_PasskeyReply(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_TrspXportCommand(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_TrspXUserConfirmationRequest(MMIMsgType XDATA_PTR msgPtr);
#endif
PRIVATE void MMI_HCI_PasskeyRequest(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_SSPCmpEvt(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_ReadRemoteNameEvt(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_PairingFailHandle(U8 XDATA_PTR  bdAddrPtr, U8 reason);
PRIVATE void MMI_HCI_PairingSuccessfulHandle(U8 XDATA_PTR bdAddrPtr);
PRIVATE void MMI_HCI_UpdateRole(U8 linkIndex, U8 role);

typedef struct
{
	HCI_VCMD_OPCODE_STRU VOpCode;
    U8 ServiceType;
    U8 FlowStatus;	
}MMIHCIVCmdQosSetupParaType;

typedef union
{
	U8 role; //HCI_CMD_SWITCH_ROLE
	MMIHCIVCmdQosSetupParaType vQosPara; //HCI_VCMD_OCF_SETUP_QOS
	HCI_VCMD_OPCODE_STRU VOpCode;
}MMIHCIParaType;

typedef struct
{
	U8 cmdCode;
	U8 linkIndex;
	MMIHCIParaType para;
	OSMQ cmdQueue;
}MMIHCICtlType;

MMIHCICtlType XDATA gMMI_HCI_ctl;

PUBLIC void MMI_HCI_SendCmd(MMIMsgType XDATA_PTR msgPtr)
{
	MMIMsgType XDATA_PTR tempPtr;
	
	switch(msgPtr->msgOpcode)
	{
		case HCI_VCMD_AIROHA_TO_UART:
			OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
			break;
		case HCI_VCMD_AIROHA:
			if(msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf == HCI_VENDOR_OGF_AUDIO && msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf == HCI_VCMD_OCF_SETUP_QOS)
			{
				if(gMMI_HCI_ctl.cmdCode)
				{
					U8 i = OSMQ_Entries(&gMMI_HCI_ctl.cmdQueue);
					while(i--)
					{
						tempPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_HCI_ctl.cmdQueue);
						if(	tempPtr->msgOpcode == HCI_VCMD_AIROHA && 
							tempPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf == HCI_VENDOR_OGF_AUDIO && tempPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf == HCI_VCMD_OCF_SETUP_QOS &&
							*((U16 XDATA_PTR)&tempPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ConnHdlB0) == *((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ConnHdlB0))
						{
							OSMEM_Put((U8 XDATA_PTR)tempPtr);
						}
						else
						{
							OSMQ_Put (&gMMI_HCI_ctl.cmdQueue, (U8 XDATA_PTR)tempPtr);
						}
					}
					
					if(	gMMI_HCI_ctl.cmdCode == msgPtr->msgOpcode &&
						gMMI_HCI_ctl.para.vQosPara.VOpCode.VCmdOgf == HCI_VENDOR_OGF_AUDIO && gMMI_HCI_ctl.para.vQosPara.VOpCode.VCmdOcf == HCI_VCMD_OCF_SETUP_QOS &&
						gMMI_HCI_ctl.linkIndex == MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ConnHdlB0))) &&
						gMMI_HCI_ctl.para.vQosPara.ServiceType == msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ServiceType &&
						gMMI_HCI_ctl.para.vQosPara.FlowStatus == msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.FlowStatus)
					{
						OSMEM_Put((U8 XDATA_PTR)msgPtr);
					}
					else
					{
						OSMQ_Put (&gMMI_HCI_ctl.cmdQueue, (U8 XDATA_PTR)msgPtr);
					}
				}
				else
				{
					gMMI_HCI_ctl.cmdCode = msgPtr->msgOpcode;
					gMMI_HCI_ctl.linkIndex = MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ConnHdlB0)));
					gMMI_HCI_ctl.para.vQosPara.VOpCode.VCmdOgf = HCI_VENDOR_OGF_AUDIO;
					gMMI_HCI_ctl.para.vQosPara.VOpCode.VCmdOcf = HCI_VCMD_OCF_SETUP_QOS;
					gMMI_HCI_ctl.para.vQosPara.ServiceType = msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ServiceType;
					gMMI_HCI_ctl.para.vQosPara.FlowStatus = msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.FlowStatus;	
					OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);	
					LightDbgPrint("send airoha qos cmd:%x,%x",(U8)gMMI_HCI_ctl.para.vQosPara.ServiceType, (U8)gMMI_HCI_ctl.para.vQosPara.FlowStatus);
				}			
			}
			else if(msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf == HCI_VENDOR_OGF_GENERAL_V2 && msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf == HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR)
			{
				if(gMMI_HCI_ctl.cmdCode)
				{
					OSMQ_Put (&gMMI_HCI_ctl.cmdQueue, (U8 XDATA_PTR)msgPtr);
				}
				else
				{
					gMMI_HCI_ctl.cmdCode = msgPtr->msgOpcode;
					gMMI_HCI_ctl.linkIndex = MMI_EOF;
					gMMI_HCI_ctl.para.VOpCode.VCmdOgf = HCI_VENDOR_OGF_GENERAL_V2;
					gMMI_HCI_ctl.para.VOpCode.VCmdOcf = HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR;	
					OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);	
					LightDbgPrint("send airoha write bd addr cmd");
				}					
			}
			else
			{
				OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
			}
			break;
	
		case HCI_CMD_CREATE_CONNECTION:
			if(gMMI_HCI_ctl.cmdCode)
			{
				OSMQ_Put (&gMMI_HCI_ctl.cmdQueue, (U8 XDATA_PTR)msgPtr);
			}
			else
			{
				OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);	
			}		
			break;
		
		case HCI_CMD_SWITCH_ROLE:
			if(gMMI_HCI_ctl.cmdCode)
			{
				U8 i = OSMQ_Entries(&gMMI_HCI_ctl.cmdQueue);
				while(i--)
				{
					tempPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_HCI_ctl.cmdQueue);
					if(	HCI_CMD_SWITCH_ROLE == tempPtr->msgOpcode && 
						MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.switchRoleCmd.BdAddr) == MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&tempPtr->msgBodyPtr.hciCmd.switchRoleCmd.BdAddr))
					{
						OSMEM_Put((U8 XDATA_PTR)tempPtr);
					}
					else
					{
						OSMQ_Put (&gMMI_HCI_ctl.cmdQueue, (U8 XDATA_PTR)tempPtr);
					}
				}
				
				if(	gMMI_HCI_ctl.cmdCode == msgPtr->msgOpcode && 
					gMMI_HCI_ctl.linkIndex == MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.switchRoleCmd.BdAddr) && 
					gMMI_HCI_ctl.para.role == msgPtr->msgBodyPtr.hciCmd.switchRoleCmd.Role)
				{
					OSMEM_Put((U8 XDATA_PTR)msgPtr);
				}
				else
				{					
					OSMQ_Put (&gMMI_HCI_ctl.cmdQueue, (U8 XDATA_PTR)msgPtr);
				}
			}
			else
			{
				gMMI_HCI_ctl.cmdCode = msgPtr->msgOpcode;
				gMMI_HCI_ctl.linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.switchRoleCmd.BdAddr);
				gMMI_HCI_ctl.para.role = msgPtr->msgBodyPtr.hciCmd.switchRoleCmd.Role;
				OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
				LightDbgPrint("Send hci role switch to GAP");
			}
			break;	
			
		default:
			OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
			break;	
	}
}


PRIVATE void MMI_HCI_ClearCmdState(U8 cmdCode, U8 linkIndex, U8 VCmdOgf, U8 VCmdOcf)
{
	if(cmdCode == gMMI_HCI_ctl.cmdCode)
	{
		switch(cmdCode)
		{
			case HCI_CMD_SWITCH_ROLE:
				if(linkIndex == MMI_EOF || linkIndex == gMMI_HCI_ctl.linkIndex)
					MMI_SniffEnable(gMMI_HCI_ctl.linkIndex, MMI_SNIFF_ROLE_SWITCH);
				break;
			case HCI_VCMD_AIROHA:
				if(VCmdOgf == HCI_VENDOR_OGF_AUDIO && VCmdOcf == HCI_VCMD_OCF_SETUP_QOS)
				{
					if(gMMI_HCI_ctl.para.vQosPara.VOpCode.VCmdOgf == HCI_VENDOR_OGF_AUDIO && gMMI_HCI_ctl.para.vQosPara.VOpCode.VCmdOcf == HCI_VCMD_OCF_SETUP_QOS)
					{
						break;
					}					
				}
				else if(VCmdOgf == HCI_VENDOR_OGF_GENERAL_V2 && VCmdOcf == HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR)
				{
					if(gMMI_HCI_ctl.para.VOpCode.VCmdOgf == HCI_VENDOR_OGF_GENERAL_V2 && gMMI_HCI_ctl.para.VOpCode.VCmdOcf == HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR)
					{
						break;
					}
				}
				return;
				
			default:
				ASSERT(FALSE);
				break;
		}
		
		gMMI_HCI_ctl.cmdCode = 0;
		gMMI_HCI_ctl.linkIndex = 0;
		OS_memset(&gMMI_HCI_ctl.para, 0, sizeof(MMIHCIParaType));
		LightDbgPrint("Clear HCI_Ctl.cmdCode:%x,%x,%x",(U8)cmdCode, (U8)VCmdOgf, (U8)VCmdOcf);
		
		if(OSMQ_Entries(&gMMI_HCI_ctl.cmdQueue))
		{
			MMI_HCI_SendCmd(OSMQ_Get(&gMMI_HCI_ctl.cmdQueue));
		}
	}
}

PRIVATE BOOL MMI_HCI_IsCmdInBusy(U8 cmdCode, U8 linkIndex, U8 VCmdOgf, U8 VCmdOcf)
{
	U8 i;
	BOOL isBusy = FALSE;
	
	if(gMMI_HCI_ctl.cmdCode)
	{
		if(cmdCode == gMMI_HCI_ctl.cmdCode && linkIndex == gMMI_HCI_ctl.linkIndex)
		{
			switch(cmdCode)
			{
				case HCI_VCMD_AIROHA:
					if(gMMI_HCI_ctl.para.VOpCode.VCmdOgf == VCmdOgf && gMMI_HCI_ctl.para.VOpCode.VCmdOcf == VCmdOcf)
						return TRUE;
					break;
				default:
					ASSERT(FALSE);
					break;
			}
		}
	}
	
	i = OSMQ_Entries(&gMMI_HCI_ctl.cmdQueue);
	while(i--)
	{
		MMIMsgType XDATA_PTR tempPtr;
		tempPtr = (MMIMsgType XDATA_PTR)OSMQ_Get(&gMMI_HCI_ctl.cmdQueue);
		if(cmdCode == tempPtr->msgOpcode)
		{
			if(cmdCode == HCI_VCMD_AIROHA)
			{
				if(VCmdOgf == HCI_VENDOR_OGF_AUDIO && VCmdOcf == HCI_VCMD_OCF_SETUP_QOS)
				{
					if(tempPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf == HCI_VENDOR_OGF_AUDIO && tempPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf == HCI_VCMD_OCF_SETUP_QOS)
					{
						if(linkIndex == MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*((U16 XDATA_PTR)&tempPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ConnHdlB0))))
							isBusy = TRUE;
					}	
				}
			}
		}
		if(tempPtr != (MMIMsgType XDATA_PTR)NULL)
			OSMQ_Put (&gMMI_HCI_ctl.cmdQueue, (U8 XDATA_PTR)tempPtr);
	}
	return isBusy;
}
//------------------------------------------------------------------------------
//NOTE!!/////////////////////
//HCI_EVENT_ROLE_CHANGE
//IVT dongle would do role switch even if we send CreateConnection HCI command with "MMI_CMD_PARA_NOT_ALLOW_ROLE_SWITCH".
//In that case, HCI_EVENT_ROLE_CHANGE would pass to MMI before we get Connection Handle, and that's the problem.
//Try to do Role Switch one more time as long as current role is SLAVE,
//no matter Role Switch last time is sucessful or not.

PUBLIC void MMI_HCI_Handler(void)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = (MMIMsgType XDATA_PTR)OSMQ_Get (OSMQ_GAP_Event_To_MMI_ptr)) != (MMIMsgType XDATA_PTR)NULL)
	{
		switch (msgPtr->eventCode)
		{
		 	case HCI_EVENT_CONNECTION_REQUEST:
				MMI_HCI_ConnectionReq(msgPtr);
				break;

			case HCI_EVENT_DISCONNECTION_COMPLETE:
				MMI_HCI_DisconnectionComplete(msgPtr);
				break;

			case HCI_EVENT_COMMAND_COMPLETE:
				 MMI_HCI_CommandComplete(msgPtr);
				break;

			case HCI_EVENT_CONNECTION_COMPLETE:
			case HCI_EVENT_SYNCHRONOUS_CONNECTION_COMPLETE:
				MMI_HCI_ConnectionComplete(msgPtr);
				break;

			case HCI_EVENT_SYNCHRONOUS_CONNECTION_CHANGED:
				MMI_HCI_SCOConnectionChange(msgPtr);
				break;

			case HCI_EVENT_PIN_CODE_REQUEST:
				MMI_HCI_PinCodeRequest(msgPtr);
				break;

			case HCI_EVENT_LINK_KEY_REQUEST:
				MMI_HCI_LinkKeyRequest(msgPtr);
				break;

			case HCI_EVENT_LINK_KEY_NOTIFICATION:
				MMI_HCI_LinkKeyNotification(msgPtr);
				break;

			case HCI_EVENT_AUTHENTICATION_COMPLETE:
				MMI_HCI_AuthenticationComplete(msgPtr);
				break;

			case HCI_EVENT_ENCRYPTION_CHANGE:
				MMI_HCI_EncryptionChange(msgPtr);
				break;

			case HCI_EVENT_QOS_SETUP_COMPLETE:
				MMI_HCI_QosSetupComplete(msgPtr);
				break;

			case HCI_EVENT_COMMAND_STATUS:
				MMI_HCI_CommandStatus(msgPtr);
				break;
				
			case HCI_EVENT_INQUIRY_RESULT:
				MMI_HCI_InquiryResult(msgPtr);
				break;

			case HCI_EVENT_INQUIRY_RESULT_WITH_RSSI:
				MMI_HCI_InquiryResultwithRSSI(msgPtr);
				break;

			case HCI_EVENT_EXTENDED_INQUIRY_RESULT:
				MMI_HCI_ExtendedInquiryResult(msgPtr);
				break;

			case HCI_EVENT_INQUIRY_COMPLETE:
				MMI_HCI_InquiryComplete(msgPtr);
				break;

			case GAP_EVENT_TO_PROFILE_SNIFF_MODE_COMPLETE:
			case GAP_EVENT_TO_PROFILE_EXIT_SNIFF_MODE_COMPLETE:
			case HCI_EVENT_MODE_CHANGE:
				MMI_HCI_ModeChange(msgPtr);
				break;

			case HCI_EVENT_IO_CAPABILITY_REQUEST:
				MMI_HCI_IOCapabilityRequest(msgPtr);
				break;
			case HCI_EVENT_IO_CAPABILITY_RESPONSE:
				MMI_HCI_IOCapabilityResponse(msgPtr);
				break;

			case HCI_EVENT_USER_CONFIRMATION_REQUEST:
                #ifdef TRSPX_App
                MMI_HCI_TrspXUserConfirmationRequest(msgPtr);
                #else
				MMI_HCI_UserConfirmationRequest(msgPtr);
                #endif
				break;

			case HCI_EVENT_ROLE_CHANGE:
				MMI_HCI_RoleChange(msgPtr);
				break;

			case HCI_EVENT_SCO_PACKET_TYPE:
				MMI_HCI_SCOPacketType(msgPtr);
				break;

			case HCI_EVENT_PASSKEY_REQUEST:
				MMI_HCI_PasskeyRequest(msgPtr);
				break;

			case HCI_EVENT_SIMPLE_PAIRING_COMPLETE:
				MMI_HCI_SSPCmpEvt(msgPtr);
				break;

			case HCI_EVENT_REMOTE_NAME_REQUEST_COMPLETE:
				MMI_HCI_ReadRemoteNameEvt(msgPtr);
				break;

			case HCI_EVENT_NUMBER_OF_COMPLETED_PACKETS:
				MMI_HCI_NumberOfCompletePktEvt(msgPtr);
				break;

			#ifdef AUDIO_BIST
			case HCI_EVENT_BIST_CMD:
				MMI_DRV_AudioBistHCICommand(msgPtr);
				break;
			#endif

			case HCI_EVENT_AIROHA_VENDOR:
				MMI_HCI_VendorEvt(msgPtr);
				break;

			case GAP_EVENT_TO_PROFILE_WRITE_SECURITY_MODE_COMPLETE:
				MMI_GAP_WriteSecurityModeComplete(msgPtr);
				break;

			case GAP_EVENT_TO_PROFILE_WRITE_CONNECTABILITY_MODE_COMPLETE:
				MMI_GAP_WriteConnectabilityModeComplete(msgPtr);
				break;

			case GAP_EVENT_TO_PROFILE_WRITE_DISCOVERABILITY_MODE_COMPLETE:
				MMI_GAP_WriteDiscoverabilityModeComplete(msgPtr);
				break;

			default:
				OSMEM_Put ((U8 XDATA_PTR)msgPtr);
				break;
		}
	}
}

PRIVATE void MMI_HCI_SetU16(U16 XDATA_PTR targetPtr, U16 value)
{
	*targetPtr = ENDIAN_TRANSFORM_U16(value);
}

PRIVATE void MMI_HCI_SetU32(U32 XDATA_PTR targetPtr, U32 value)
{
	*targetPtr = ENDIAN_TRANSFORM_U32(value);
}

PUBLIC void MMI_HCI_WriteLinkPolicySetting(U8 linkIndex, U16 link_policy_settings)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_WRITE_LINK_POLICY_SETTINGS)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_WR_LINK_POLICY_SETTING_STRU); 
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.writeLinkPolicySetting.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.writeLinkPolicySetting.LinkPolicySettingB0, link_policy_settings);
		MMI_HCI_SendCmd(msgPtr);
	}
	else
	{
		ASSERT_PRINT("MEM ALLOC ERR");
	}
}

PRIVATE void MMI_HCI_ConnectionReq(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;

	switch(msgPtr->msgBodyPtr.hciEvt.connRequestEvt.LinkType)
	{
		case LINK_TYPE_SCO_CONNECTION_REQUEST:
		case LINK_TYPE_eSCO_CONNECTION_REQUEST:
			msgPtr->dataOffset = CMD_OFFSET;
			linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.connRequestEvt.BdAddr);
			if(linkIndex < MAX_MULTI_POINT_NO)
			{
				if(!SCO_EXISTS(linkIndex))
				{
					gMMI_ctl.mmiInfo[linkIndex].linkPara.scoIsActiveEstablished = FALSE;
					// verson 2.0
					msgPtr->msgOpcode = HCI_CMD_ACCEPT_SYNCHRONOUS_CONNECTION_REQUEST;
					msgPtr->paraLen = sizeof(HCI_CMD_PARA_ACP_SCO_CONN_REQ_STRU); //21;
					//msgPtr->msgBodyPtr.hciAcpScoCnf.bdAddr already existes, it doesnt need to
					MMI_HCI_SetU32((U32 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.acceptSCOConnReqCmd.TxBandwidth, gMMI_nvram.scoPktPara.TxBandwidth); //(U32)8000);
					MMI_HCI_SetU32((U32 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.acceptSCOConnReqCmd.RxBandwidth, gMMI_nvram.scoPktPara.RxBandwidth); //(U32)8000);
					MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.acceptSCOConnReqCmd.MaxLatencyB0, gMMI_nvram.scoPktPara.MaxLatency); //0XFFFF); //dont care
					MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.acceptSCOConnReqCmd.ContentFormatB0, gMMI_nvram.scoPktPara.VoiceSetting); //(U16)0X0060); // 2's complement, 16-bit sample size, CVSD
					msgPtr->msgBodyPtr.hciCmd.acceptSCOConnReqCmd.ReTxEffort = gMMI_nvram.scoPktPara.ReTxEffort;//dont care
					MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.acceptSCOConnReqCmd.PacketTypeB0, gMMI_nvram.scoPktPara.scoPktType); //(U16)0X0004); //HV3
					
					MMI_HCI_AcceptSCOReqProfileHandle(linkIndex);
				}
				else
				{
					msgPtr->msgOpcode = HCI_CMD_REJECT_SYNCHRONOUS_CONNECTION_REQUEST;
					msgPtr->paraLen = sizeof(HCI_CMD_PARA_REJ_SCO_CONN_REQ_STRU); //7;
					msgPtr->msgBodyPtr.hciCmd.rejectSCOConnReqCmd.Reason = ERR_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES;
				}
			}
			else
			{
				msgPtr->msgOpcode = HCI_CMD_REJECT_SYNCHRONOUS_CONNECTION_REQUEST;
				msgPtr->paraLen = sizeof(HCI_CMD_PARA_REJ_SCO_CONN_REQ_STRU); //7;
				msgPtr->msgBodyPtr.hciCmd.rejectSCOConnReqCmd.Reason = ERR_CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR;
			}
			MMI_HCI_SendCmd(msgPtr);
			break;

		case LINK_TYPE_ACL_CONNECTION_REQUEST:
			msgPtr->dataOffset = CMD_OFFSET;
			if((linkIndex = MMI_GetEmptyLinkIndex((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.connRequestEvt.BdAddr, FALSE)) < MAX_MULTI_POINT_NO )
			{
				ACCEPT_CONN_REQ:
				if(MMI_CONN_STATE_WAITING_ACTIVE == MMI_GetConnectionState(linkIndex))
				{
					MMI_PageFailHandler(linkIndex);
				}
				else if(MMI_CONN_STATE_WAIT_FOR_DETACH == MMI_GetConnectionState(linkIndex))
				{
					goto REJECT_CONN_REQ;
				}
				
				#ifdef AIR_MODULE
				MMI_AIR_ACLConnRequest(linkIndex);
				#endif

				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].bdAddr,
							(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.connRequestEvt.BdAddr, 6);
				OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].linkPara.classOfDev,
							(U8 XDATA_PTR)msgPtr->msgBodyPtr.hciEvt.connRequestEvt.ClassOfDevice, 3);

				MMI_SetConnectionState(linkIndex, MMI_CONN_STATE_WAITING_PASS);
						
				MMI_HCI_UpdateRole(linkIndex, ROLE_SLAVE);
										
				msgPtr->msgOpcode = HCI_CMD_ACCEPT_CONNECTION_REQUEST;
				if (MMI_ROLE_SWITCH_TO_MASTER_DURING_ACCEPT_CONN_FEAT)
				{
					msgPtr->msgBodyPtr.hciCmd.acceptConnReqCmd.Role = HCI_CMD_ACCEPT_CONN_REQ_ROLE_IS_MASTER;
				}
				else
				{
					msgPtr->msgBodyPtr.hciCmd.acceptConnReqCmd.Role = HCI_CMD_ACCEPT_CONN_REQ_ROLE_IS_SLAVE;
				}
				#ifdef AIR_MODULE
				MMI_AIR_AcceptConnReq((U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].bdAddr, &msgPtr->msgBodyPtr.hciCmd.acceptConnReqCmd.Role);
				#endif
			}
			else if((linkIndex = MMI_Reconnect_GetPagingLink()) != MMI_EOF)
			{
				MMI_HCI_SendCreateConnectionCancel((U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].bdAddr);
				MMI_PageFailHandler(linkIndex);
				if((linkIndex = MMI_GetEmptyLinkIndex((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.connRequestEvt.BdAddr, FALSE)) < MAX_MULTI_POINT_NO )
				{
					goto ACCEPT_CONN_REQ;
				}
				else
				{
					goto REJECT_CONN_REQ;
				}
			}
			else
			{
				REJECT_CONN_REQ:
				msgPtr->msgOpcode = HCI_CMD_REJECT_CONNECTION_REQUEST;
				msgPtr->msgBodyPtr.hciCmd.rejectConnReqCmd.Reason = ERR_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES;
			}
			MMI_HCI_SendCmd(msgPtr);
			break;

		default:
			ASSERT_PRINT("link type ERR");
			OSMEM_Put ((U8 XDATA_PTR)msgPtr);
			break;
	}
}

PRIVATE void MMI_HCI_DisconnectionComplete(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;
	U16 connHandle;
	BOOL isActiveDisc;
	BOOL skipReconnect = FALSE;

	connHandle = ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.ConnHdlB0);

	//#ifdef DBG_UART_ENABLE_MMI
	LightDbgPrint("MMI - Disc %D complete:%x",(U32)connHandle,(U8)msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason);
	//#endif

	if((linkIndex = MMI_GetLinkIndexByConnHandle(connHandle)) < MAX_MULTI_POINT_NO)
	{
		if(MMI_IsActiveLink(linkIndex))
		{
			if (connHandle == gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle)
			{
				isActiveDisc = MMI_IsLinkActiveDisconnecting(linkIndex) ? TRUE : FALSE;
				
				// Out-of-Range Case [Link loss]  //supervision timeout
				if((msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason == ERR_LMP_RESPONSE_TIMEOUT || msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason == ERR_CONNECTION_TIMEOUT))
				{
					gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask |= MMI_LINK_LOSS;
					#ifdef AIR_MODULE
					if(linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER))
					{
						skipReconnect = TRUE;
					}
					#endif
					
					#ifdef AIR_UPDATE_DONGLE
						skipReconnect = TRUE;
					#endif
					OSMEM_memcpy_xdata_xdata(&gMMI_ctl.reconnectCtl.linkLossBdAddr, (U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, sizeof(BD_ADDR));
				}
				#if 1 //mantis 9183
				else if(msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason == ERR_REMOTE_USER_TERMINATED_CONNECTION)
				{
					if(!MMI_Reconnect_FindTarget((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr))
						skipReconnect = TRUE;
				}
				#endif

				MMI_HCI_AclDiscHandle(linkIndex);

				#ifdef TRSPX_App
				// after delete ACL
				MMI_TrspX_connectHandle(linkIndex, MMI_TRSPX_CONNECTED_NONE);
				#endif

				//Enable auto switch off timer if there is no acl links.
				MMI_TIMER_ResetAutoSwitchOffTimer();
				
				#ifdef FAST_CONNECT_DONGLE
				MMI_FCD_ReportAction(HCI_VEVT_OCF_FCD_ACL_DISC);
				#endif
				#ifdef OLED_ENABLE
				OLED_DisconnectionShow();
				#endif

				if(MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState()) // Not Power off
				{
					if (CURRENT_ACTIVE_LINK_CNT < gMMI_ctl.currentSupportDeviceNo)
					{
						if( !(MMI_NO_CONNECTABLE_IN_LINE_IN_FEAT && MMI_CheckInLineInState()) && !(MMI_NO_CONNECTABLE_IN_FM_FEAT && MMI_CheckInFMState()))
						{
							MMI_ConnectabilityHandle(CMD_SET_CONNECTABLE);
						}
						if(MMI_DISCOVER_MODE_AFTER_DISCONNECT_FEAT)
						{
							if(CURRENT_ACTIVE_LINK_CNT == 0)
								MMI_SetEnterDiscoverable(TRUE);
							else
								MMI_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
						}
					}

					if(isActiveDisc && msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason != ERR_CONNECTION_TIMEOUT)
					{
						if(!gMMI_ctl.activeLinkRecord)
						{
							if(MMI_IsEnterDiscoverableAfterDisc())
							{
								MMI_SetEnterDiscoverable(TRUE);
								MMI_SetEnterDiscoverableAfterDisc(FALSE);
							}
						}
						if(msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason == ERR_CONNECTION_TERMINATED_BY_LOCAL_HOST)
						{
							if(!skipReconnect)
								MMI_Reconnect_SetDelayTimer(HALF_SEC);
						}
					}
					else
					{
						// Out-of-Range Case [Link loss]
						if(msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason == ERR_LMP_RESPONSE_TIMEOUT || msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason == ERR_CONNECTION_TIMEOUT)
						{
							if(MMI_DO_NOT_DISCO_DURING_LINK_LOSS_FEAT)
							{
								MMI_RmvState(MMI_EOF, MMI_CONDISCABLE);
								MMI_SetEnterDiscoverable(FALSE);
							}
							if(MMI_AUTO_RECONN_AFTER_LINK_LOSS_FEAT && !skipReconnect)
							{
								MMI_Reconnect(RECONNECT_LINK_LOSS);
							}
						}
						#if 1 //mantis 9183
						else if(msgPtr->msgBodyPtr.hciEvt.discCompleteEvt.Reason == ERR_REMOTE_USER_TERMINATED_CONNECTION)
						{
							if(!skipReconnect)
								MMI_Reconnect_SetDelayTimer(HALF_SEC);
						}
						#endif
					}
				}
			}
			else if (connHandle == gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle)
			{
				MMI_HCI_ScoDiscHandle(linkIndex);
				MMI_HCI_ScoDiscProfileHandle(linkIndex);
				MMI_HCI_SendExitSniffMode(linkIndex);
			}
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_CommandComplete(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;

	switch(msgPtr->msgBodyPtr.hciEvt.cmdCompleteEvt.OpCodeB0)
	{
		case HCI_CMD_CREATE_CONNECTION_CANCEL:
			{
				HCI_CMD_RTN_PARA_CREATE_CONN_CANCEL_STRU XDATA_PTR RtnDataPtr = (HCI_CMD_RTN_PARA_CREATE_CONN_CANCEL_STRU XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.cmdCompleteEvt.RtnPara;
				if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&RtnDataPtr->BdAddr)) < MAX_MULTI_POINT_NO)
				{
					MMI_PageFailHandler(linkIndex);		
					MMI_ClearMMILinkSettingType(linkIndex);					
				}
			}
			break;

		case HCI_CMD_INQUIRY_CANCEL:
			MMI_HCI_InquiryComplete(msgPtr);
			return; // msgPtr will be put in MMI_HCI_InquiryComplete(), need return to avoid put twice at the end of this switch case;

		case HCI_CMD_READ_RSSI:
			{
				HCI_CMD_RTN_PARA_RD_RSSI_STRU XDATA_PTR RtnDataPtr = (HCI_CMD_RTN_PARA_RD_RSSI_STRU XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.cmdCompleteEvt.RtnPara;
				if(MMI_DETECT_PHONE_IS_TOO_FAR_FEAT)
				{
					if(RtnDataPtr->RSSI && RtnDataPtr->RSSI < gMMI_nvram.rssiDefaultBoundry)
					{
						if(ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&RtnDataPtr->HandleB0) == gMMI_ctl.mmiInfo[gMMI_ctl.latestConnectedLinkInd].linkPara.aclConnHandle)
							MMI_PushMediaEvent(MEDIA_EVT_PHONE_IS_TOO_FAR_2);
						else
							MMI_PushMediaEvent(MEDIA_EVT_PHONE_IS_TOO_FAR_1);				
					}
				}
				#ifdef FAST_CONNECT_DONGLE
				if((linkIndex = MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&RtnDataPtr->HandleB0))) != MAX_MULTI_POINT_NO)
				{
					MMI_FCD_ReportDevRSSI(RtnDataPtr->RSSI);
					#ifdef OLED_ENABLE
					OLED_ConnectionShow(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], &gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[0], RtnDataPtr->RSSI);
					#endif
				}
				#endif
			}
			break;
		case HCI_VCMD_AIROHA:
			{
				HCI_EVT_PARA_CMD_CPL_RTN_VDATA_STRU XDATA_PTR VCmdCplEvtPtr = (HCI_EVT_PARA_CMD_CPL_RTN_VDATA_STRU XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.cmdCompleteEvt.RtnPara;
				
				if(VCmdCplEvtPtr->vcmdCplEvt.VOpCode.VCmdOgf == HCI_VENDOR_OGF_AUDIO)
				{
					switch(VCmdCplEvtPtr->vcmdCplEvt.VOpCode.VCmdOcf)
					{
						case HCI_VCMD_OCF_ESCO_NULL_PACKET:
							#ifdef AIR_MODULE
							MMI_AIR_HCISCONullPktCmdCpl((VCmdCplEvtPtr->scoNullCmdCplEvt.IsEnable)?TRUE:FALSE);
							#endif							
							break;
						case HCI_VCMD_OCF_AUDIO_SWITCH:
							linkIndex = MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&VCmdCplEvtPtr->audioSwitchCmdCplEvt.ScoHandleB0));
							if(	linkIndex < MAX_MULTI_POINT_NO && VCmdCplEvtPtr->audioSwitchCmdCplEvt.vcmdCpl.Status == AUDIO_SWITCH_OK)
							{
								gMMI_ctl.driverCtl.audioSwitchHandler = ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&VCmdCplEvtPtr->audioSwitchCmdCplEvt.ScoHandleB0);
								gMMI_ctl.driverCtl.audioSwitchState = MMI_AUDIO_SWITCH_ILDE;
							}
							else
							{
								gMMI_ctl.driverCtl.audioSwitchHandler = MMI_EOF_HANDLER;
								if(gMMI_ctl.driverCtl.audioSwitchState == MMI_AUDIO_SWITCH_WAITING_HCI_EVT)
									gMMI_ctl.driverCtl.audioSwitchState = MMI_AUDIO_SWITCH_ERROR;							
							}
							break;
						case HCI_VCMD_OCF_SETUP_QOS:
							//if((linkIndex = MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&VCmdCplEvtPtr->setupQosCmdCplEvt.ConnHdlB0))) != MAX_MULTI_POINT_NO)
							{
								LightDbgPrint("recv airoha qos response");
								MMI_HCI_ClearCmdState(HCI_VCMD_AIROHA, 0 /*link index is don't care */, HCI_VENDOR_OGF_AUDIO, HCI_VCMD_OCF_SETUP_QOS);
							}
							break;
					}
				}
			}
			break;
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_ConnectionComplete(MMIMsgType XDATA_PTR msgPtr)
{
	//After HCI_CMD_ADD_SCO process is done,
	//HCI_EVENT_CONNECTION_COMPLETE would be sent to MMI,
	//not HCI_EVENT_SCO_CONNECTION_COMPLETE.
	//
	//Therefore, we should check parameter "LinkType" in event
	//HCI_EVENT_CONNECTION_COMPLETE first.
	U8 LDATA connectionState;
	U8 linkIndex;
	U16 connHandle = ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.ConnHdlB0);

	if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.BdAddr)) < MAX_MULTI_POINT_NO)
	{
		switch(msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.LinkType)
		{
			case LINK_TYPE_SCO_CONNECTION_REQUEST:
			case LINK_TYPE_eSCO_CONNECTION_REQUEST:
				LightDbgPrint("USR - SCO ConnCpl[%d][%D]:%d",(U8)linkIndex, (U32)connHandle, (U8)msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.Status);

				if(msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.Status) // Fail situation
				{
					MMI_HCI_ScoConnFailHandle(linkIndex);
					MMI_HCI_ScoConnFailProfileHandle(linkIndex);
				}
				else // successful situation
				{
					gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle = connHandle;
					gMMI_ctl.mmiInfo[linkIndex].linkPara.scoType = msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.LinkType;
					gMMI_ctl.mmiInfo[linkIndex].linkPara.txInterval = msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.txInterval;
					gMMI_ctl.mmiInfo[linkIndex].linkPara.rxPacketLen = ENDIAN_TRANSFORM_U16(msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.rxPacketLen);
					MMI_SendDSPCmd(linkIndex, DSP_DISABLED);
					MMI_HCI_ScoConnOKHandle(linkIndex);
					MMI_HCI_ScoConnOKProfileHandle(linkIndex);
				}
				break;

			case LINK_TYPE_ACL_CONNECTION_REQUEST:
				LightDbgPrint("USR - ACL ConnCpl[%d][%D]:%d",(U8)linkIndex,(U32)connHandle, (U8)msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.Status);

				if(msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.Status == STATUS_OK)
				{
					gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle = connHandle;
					MMI_ADD_LINK(linkIndex);
					gMMI_ctl.mmiInfo[linkIndex].linkPara.scoCodecType = SCO_CODEC_CVSD; //set default SCO type to CVSD
					connectionState = MMI_CONN_STATE_ACL_CONNECTED;
					MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_ACL_LINK_STATUS, &connectionState, linkIndex);
					
					L2CAP_SetupLinkInfo(linkIndex, connHandle);
					
					if(MMI_CONN_STATE_WAIT_FOR_DETACH == MMI_GetConnectionState(linkIndex))
					{
						MMI_HCI_SendRelAcl(linkIndex);
						break;
					}
					
					#if !defined(AIR_UPDATE_DONGLE)					
					MMI_LinkKey_SetLinkSoundLevelByHistory(linkIndex);

					
					#ifdef AIR_MODULE
					MMI_AIR_ACLConnectedNotify(linkIndex);				
					#endif
					
					if(MMI_CONN_STATE_WAITING_ACTIVE == MMI_GetConnectionState(linkIndex))
					{
						MMI_PageOKHandler(linkIndex);
					}
					#endif
					
					MMI_SetConnectionState(linkIndex, MMI_CONN_STATE_ACL_CONNECTED);

					MMI_ConnectableStateChangeWhenConnected();

					//To speed up connection & disconnection process, we should avoid
					//sending Sniff_Req to remote AG if ACL is ready but SLC is not.
					//Slow connection problem appears very often when using smart phone
					//(e.g. HTC Desire or Wildfire) to connect with our system right after
					//disconnection of SLC when ACL still exists.
					MMI_SniffDisable(linkIndex, MMI_SNIFF_ACL_READY_SLC_NOT_YET);

					MMI_HCI_SendChangeConnectionPacketType (linkIndex, LINK_TYPE_ACL_CONNECTION_REQUEST);
					if (MMI_ROLE_SWITCH_TO_MASTER_DURING_ACCEPT_CONN_FEAT)
					{
						MMI_HCI_WriteLinkPolicySetting(linkIndex, HCI_LINK_POLICY_ENABLE_HOLD_MODE|HCI_LINK_POLICY_ENABLE_SNIFF_MODE);//disable role switch
					}

					#ifdef TRSPX_App
					MMI_TrspX_connectHandle(linkIndex, MMI_TRSPX_CONNECTED_BR_EDR);
					#endif

					#ifdef FAST_CONNECT_DONGLE
					MMI_FCD_ReportDevAddr(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0]);
					MMI_HCI_SendReadRemoteName(linkIndex);
					#endif

					#ifdef BLE_AIRApp
					MMI_HCI_SendReadRemoteName(linkIndex);
					#endif

					#if AB1520B_UPDATE_LINK_HISTORY_PERIODICALLY
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.writeFlashTimer, 20*ONE_SEC);
					#endif
				}
				else
				{
					if (msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.Status == ERR_LMP_ERROR_TRANSACTION_COLLISION )
					{
						break;//if only transaction collision, ignore this message;
					}
					
					if(MMI_CONN_STATE_WAITING_ACTIVE == MMI_GetConnectionState(linkIndex))
					{
						MMI_PageFailHandler(linkIndex);
						
						if(msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.Status == ERR_ACL_CONNECTION_ALREADY_EXISTS
							|| msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.Status == ERR_LMP_RESPONSE_TIMEOUT)
						{
							MMI_Reconnect_SetDelayTimer(ONE_SEC * 23);
						}
					}
					else if(MMI_CONN_STATE_WAITING_PASS == MMI_GetConnectionState(linkIndex))
					{
						if(msgPtr->msgBodyPtr.hciEvt.connCompleteEvt.Status != ERR_ACL_CONNECTION_ALREADY_EXISTS || gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle != 0)
						{
							#ifdef AIR_MODULE
							MMI_AIR_ACLConnFailPassive(linkIndex);
							#endif
							MMI_ClearMMILinkSettingType(linkIndex);
						}
					}
					else if(MMI_CONN_STATE_WAIT_FOR_DETACH == MMI_GetConnectionState(linkIndex))
					{
						MMI_ClearMMILinkSettingType(linkIndex);
					}
				}
				break;
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_SCOConnectionChange(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;
	
	linkIndex = MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.scoChangeEvt.ConnHdlB0));
	if(linkIndex != MAX_MULTI_POINT_NO)
	{
		if(msgPtr->msgBodyPtr.hciEvt.scoChangeEvt.Status) // Fail situation
		{

		}
		else // successful situation
		{
			gMMI_ctl.mmiInfo[linkIndex].linkPara.txInterval = msgPtr->msgBodyPtr.hciEvt.scoChangeEvt.txInterval;
			gMMI_ctl.mmiInfo[linkIndex].linkPara.rxPacketLen = ENDIAN_TRANSFORM_U16(msgPtr->msgBodyPtr.hciEvt.scoChangeEvt.rxPacketLen);
			#ifdef AIR_MODULE
			MMI_AIR_SCOConnectedNotify(linkIndex, FALSE);
			#endif
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_PinCodeRequest(MMIMsgType XDATA_PTR msgPtr)
{
	msgPtr->dataOffset = CMD_OFFSET;
	msgPtr->msgOpcode = HCI_CMD_PIN_CODE_REQUEST_REPLY;
	msgPtr->paraLen = 23;
	msgPtr->msgBodyPtr.hciCmd.pinCodeReqReplyCmd.PinCodeLen = gMMI_nvram.pinCodeCtl.length;
	MMI_ReadNvramDataChunk ((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.pinCodeReqReplyCmd.PinCode, &gMMI_nvram.pinCodeCtl.value[0], gMMI_nvram.pinCodeCtl.length);
	MMI_HCI_SendCmd(msgPtr);
	MMI_PushMediaEvent(MEDIA_EVT_PINCODE_REQUEST);
}

PRIVATE void MMI_HCI_LinkKeyRequest(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;
	LinkHistoryType XDATA_PTR linkListPtr;

	linkListPtr = MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.linkKeyReqEvt.BdAddr, TRUE, FALSE);
	linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.linkKeyReqEvt.BdAddr);
	if(linkListPtr == (LinkHistoryType XDATA_PTR)NULL || (linkIndex < MAX_MULTI_POINT_NO && gMMI_ctl.mmiInfo[linkIndex].linkPara.isPairFailed))
	{
		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->msgOpcode = HCI_CMD_LINK_KEY_REQUEST_NEGATIVE_REPLY;
		msgPtr->paraLen = 6;
	}
	else
	{
		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->msgOpcode = HCI_CMD_LINK_KEY_REQUEST_REPLY;
		msgPtr->paraLen = 22;
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.linkKeyReqReplyCmd.LinkKey, (U8 XDATA_PTR)&linkListPtr->linkData.linkKey[0], 16);
	}
	if(linkIndex != MAX_MULTI_POINT_NO)
	{
		MMI_TIMER_ReleaseDetachTimer(linkIndex);
	}
	MMI_HCI_SendCmd(msgPtr);
}

PRIVATE void MMI_HCI_LinkKeyNotification(MMIMsgType XDATA_PTR msgPtr)
{
	//HCI_EVENT_LINK_KEY_NOTIFICATION indicates a successful pairing.
	MMI_HCI_PairingSuccessfulHandle((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.linkKeyNotifyEvt);
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_AuthenticationComplete(MMIMsgType XDATA_PTR msgPtr)
{
	U8 index;

	if((index = MMI_GetLinkIndexByConnHandle( ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.authCompleteEvt.ConnHdlB0) )) < MAX_MULTI_POINT_NO)
	{
		if(msgPtr->msgBodyPtr.hciEvt.authCompleteEvt.Status == STATUS_OK)
		{
			gMMI_ctl.mmiInfo[index].linkPara.isAuthenticated = AUTH_OK;
		}
		else
		{
			if(msgPtr->msgBodyPtr.hciEvt.authCompleteEvt.Status != ERR_COMMAND_DISALLOWED)
			{
				gMMI_ctl.mmiInfo[index].linkPara.isAuthenticated = AUTH_ERROR;
			}
			//Issue 569
			MMI_HCI_PairingFailHandle((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[index].bdAddr[0], msgPtr->msgBodyPtr.hciEvt.authCompleteEvt.Status);
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_EncryptionChange(MMIMsgType XDATA_PTR msgPtr)
{
	U8 index;

	if((index = MMI_GetLinkIndexByConnHandle( ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.encryptChangeEvt.ConnHdlB0) )) < MAX_MULTI_POINT_NO)
	{
		if(msgPtr->msgBodyPtr.hciEvt.encryptChangeEvt.Status == STATUS_OK)
		{
			gMMI_ctl.mmiInfo[index].linkPara.isEncrypted = ENCRYPTED_OK;
		}
		else if(msgPtr->msgBodyPtr.hciEvt.encryptChangeEvt.Status == ERR_DIFFERENT_TRANSACTION_COLLISION && !gMMI_ctl.mmiInfo[index].linkPara.isEncryptionFailed)
		{
			//Mantis 10988 & 11005 LM report error code ERR_DIFFERENT_TRANSACTION_COLLISION
			//Need to send Connection Encrtyption again
			MMI_HCI_SendSetConnectionEncryption(index);
			gMMI_ctl.mmiInfo[index].linkPara.isEncryptionFailed = TRUE;
		}
		else if(msgPtr->msgBodyPtr.hciEvt.encryptChangeEvt.Status != ERR_COMMAND_DISALLOWED)
		{
			//Do nothing to maintain this substate,
			//because ERR_COMMAND_DISALLOWED means that remote
			//side will do encryption again.
			gMMI_ctl.mmiInfo[index].linkPara.isEncrypted = ENCRYPTED_ERROR;
		}

		//Headset project should be returned set encryption on.
		if(gMMI_nvram.timerCtl.RefreshEncKeyTimer)
		{
			SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[index].refreshEncKeyTimer, (U32)(gMMI_nvram.timerCtl.RefreshEncKeyTimer) * ONE_MIN);
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_QosSetupComplete(MMIMsgType XDATA_PTR msgPtr)
{
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_CommandStatus(MMIMsgType XDATA_PTR msgPtr)
{
	switch(msgPtr->msgBodyPtr.hciEvt.cmdStatusEvt.OpCodeB0)
	{
		case HCI_CMD_CREATE_CONNECTION:
			if(msgPtr->msgBodyPtr.hciEvt.cmdStatusEvt.Status != STATUS_OK)
			{
				MMI_PageFailHandler(MMI_Reconnect_GetPagingLink());
			}
			break;

		case HCI_CMD_SWITCH_ROLE:
			if(msgPtr->msgBodyPtr.hciEvt.cmdStatusEvt.Status != STATUS_OK)
			{
				MMI_HCI_ClearCmdState(HCI_CMD_SWITCH_ROLE, MMI_EOF, MMI_EOF, MMI_EOF);
			}
			break;
		case HCI_CMD_INQUIRY:
			if(gMMI_ctl.inquiryState == INQUIRY_BUSY_SDK)
			{
				if(api_profile_func[API_GRP_PF_CB_HCI_INQUIRY_CMD_STATUS])
				{
					((API_PROFILE_HCI_INQUIRY_CMD_STATUS_FUNC)api_profile_func[API_GRP_PF_CB_HCI_INQUIRY_CMD_STATUS])(msgPtr->msgBodyPtr.hciEvt.cmdStatusEvt.Status);
				}
			}
			else if(gMMI_ctl.inquiryState == INQUIRY_BUSY_NDK)
			{
				MMI_HCI_SetInquiryOnGoing((msgPtr->msgBodyPtr.hciEvt.cmdStatusEvt.Status != STATUS_OK) ? INQUIRY_IDLE : INQUIRY_OK_NDK);
			}
			break;
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_InquiryResult(MMIMsgType XDATA_PTR msgPtr)
{
	if(gMMI_ctl.inquiryState == INQUIRY_BUSY_SDK || gMMI_ctl.inquiryState == INQUIRY_OK_SDK)
	{
		if(api_profile_func[API_GRP_PF_CB_HCI_INQUIRY_RESULT_EVT])
		{
			((API_PROFILE_HCI_INQ_RESULT_EVT_FUNC)api_profile_func[API_GRP_PF_CB_HCI_INQUIRY_RESULT_EVT])((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.inqResultEvt);
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_InquiryResultwithRSSI(MMIMsgType XDATA_PTR msgPtr)
{
	if(gMMI_ctl.inquiryState == INQUIRY_BUSY_SDK || gMMI_ctl.inquiryState == INQUIRY_OK_SDK)
	{
		if(api_profile_func[API_GRP_PF_CB_HCI_INQUIRY_RESULT_RSSI_EVT])
		{
			((API_PROFILE_HCI_INQ_RESULT_RSSI_EVT_FUNC)api_profile_func[API_GRP_PF_CB_HCI_INQUIRY_RESULT_RSSI_EVT])((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt);
		}
	}
	else
	{
		#ifdef AIR_MODULE
		U8 i, numResponse = msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt.NumRsp;
		for(i = 0; i < numResponse; i++)
		{
			MMI_AIR_GetInquiryResultWithRSSI(*((U8 XDATA_PTR)(&msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt.BdAddr + numResponse * (sizeof(BD_ADDR)+ 7) + i)),
											(U8 XDATA_PTR)(&msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt.BdAddr + (i * sizeof(BD_ADDR))),
											(U8 XDATA_PTR)(&msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt.BdAddr + numResponse * (sizeof(BD_ADDR)+ 2)+ i * 3));
		}
		MMI_AIR_DecideStopInquiry();
		#endif
		#ifdef FAST_CONNECT_DONGLE
		U8 i, numResponse = msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt.NumRsp;
		for(i = 0; i < numResponse; i++)
		{
			MMI_FCD_GetInquiryResultWithRSSI(*((U8 XDATA_PTR)(&msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt.BdAddr + numResponse * (sizeof(BD_ADDR)+ 7) + i)),
											(U8 XDATA_PTR)(&msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt.BdAddr + (i * sizeof(BD_ADDR))),
											(U8 XDATA_PTR)(&msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt.BdAddr + numResponse * (sizeof(BD_ADDR)+ 2)+ i * 3));
		}
		#endif
		#ifdef MMI_INQUIRY_CMD
			MMI_INQ_GetInquiryResultWithRSSI((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.inqResultWithRSSIEvt);
		#endif
		#ifdef DBG_UART_ENABLE_MMI
			LightDbgPrint("MMI-InqResWithRSSI");
		#endif
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_ExtendedInquiryResult(MMIMsgType XDATA_PTR msgPtr)
{
	if(gMMI_ctl.inquiryState == INQUIRY_BUSY_SDK || gMMI_ctl.inquiryState == INQUIRY_OK_SDK)
	{
		if(api_profile_func[API_GRP_PF_CB_HCI_EXT_INQUIRY_RESULT_EVT])
		{
			((API_PROFILE_HCI_EXT_INQ_RESULT_EVT_FUNC)api_profile_func[API_GRP_PF_CB_HCI_EXT_INQUIRY_RESULT_EVT])((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.extInquiryResultEvt);
		}	
	}
	else
	{
		#ifdef AIR_MODULE
		MMI_AIR_GetInquiryResultWithRSSI(msgPtr->msgBodyPtr.hciEvt.extInquiryResultEvt.Rssi,
										(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.extInquiryResultEvt.BdAddr,
										(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.extInquiryResultEvt.ClassOfDevice[0]);
		MMI_AIR_DecideStopInquiry();
		#endif
		#ifdef FAST_CONNECT_DONGLE
		MMI_FCD_GetInquiryResultWithRSSI(msgPtr->msgBodyPtr.hciEvt.extInquiryResultEvt.Rssi,
										(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.extInquiryResultEvt.BdAddr,
										(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.extInquiryResultEvt.ClassOfDevice[0]);
		#endif
		#ifdef MMI_INQUIRY_CMD
			MMI_INQ_GetExtendedInquiryResult((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.extInquiryResultEvt);
		#endif
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_InquiryComplete(MMIMsgType XDATA_PTR msgPtr)
{
	if(gMMI_ctl.inquiryState == INQUIRY_BUSY_SDK ||  gMMI_ctl.inquiryState == INQUIRY_OK_SDK)
	{
		if(msgPtr->eventCode == HCI_EVENT_INQUIRY_COMPLETE)
		{
			if(api_profile_func[API_GRP_PF_CB_HCI_INQUIRY_CPL_EVT])
			{
				((API_PROFILE_HCI_INQUIRY_CPL_EVT_FUNC)api_profile_func[API_GRP_PF_CB_HCI_INQUIRY_CPL_EVT])(msgPtr->msgBodyPtr.hciEvt.inqCompleteEvt.Status);
			}
		}
		else
		{
			if(api_profile_func[API_GRP_PF_CB_HCI_INQ_CANCEL_CMD_CPL_EVT])
			{
				((API_PROFILE_HCI_INQ_CANCEL_CMD_CPL_EVT_FUNC)api_profile_func[API_GRP_PF_CB_HCI_INQ_CANCEL_CMD_CPL_EVT])(msgPtr->msgBodyPtr.hciEvt.cmdCompleteEvt.RtnPara[0]);
			}		
		}
	}
	else
	{
		MMI_HCI_SetInquiryOnGoing(INQUIRY_IDLE);
		#ifdef AIR_MODULE
		MMI_AIR_GetInquiryComplete();
		#endif
		#ifdef FAST_CONNECT_DONGLE
		MMI_FCD_GetInquiryComplete();
		#endif
		#ifdef MMI_INQUIRY_CMD
		MMI_INQ_GetInquiryComplete();
		#endif
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_ModeChange(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex = MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.modeChangeEvt.ConnHdlB0));
	if(linkIndex < MAX_MULTI_POINT_NO)
	{
		gMMI_ctl.mmiInfo[linkIndex].linkPara.mode = msgPtr->msgBodyPtr.hciEvt.modeChangeEvt.CurrentMode;
		if(msgPtr->msgBodyPtr.hciEvt.modeChangeEvt.Status == NO_ERROR)
		{
			switch(gMMI_ctl.mmiInfo[linkIndex].linkPara.mode)
			{
				case MODE_SNIFF:
					gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffInterval = ENDIAN_TRANSFORM_U16(msgPtr->msgBodyPtr.hciEvt.modeChangeEvt.Interval);
					MMI_PowerSaving(TRUE, MMI_PWRSAVING_EXIT_SNIFF, linkIndex);
					SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].sniffTimerPtr);

					if(MMI_IsSniffAllowed(linkIndex))
					{
						MMI_PowerSaving(TRUE, MMI_SNIFF_DISABLE, linkIndex);
						SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].checkRemoteAGAcceptEnterSniffTimer,(U32)gMMI_nvram.timerCtl.CheckRemoteAGAcceptEnterSniffTimer * ONE_SEC);
					}
					else
					{
						MMI_HCI_SendExitSniffMode(linkIndex);
					}
					break;
				case MODE_ACTIVE:
					gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffInterval = 0;
					MMI_PowerSaving(FALSE, MMI_PWRSAVING_EXIT_SNIFF, linkIndex);

					if(MMI_IsSniffAllowed(linkIndex))
					{
						if (gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffTryCnt <= gMMI_nvram.retrySniffCnt)
						{
							if (gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].checkRemoteAGAcceptEnterSniffTimer)
							{
								gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffTryCnt++; //remote hope to keep active
							}
							SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].sniffTimerPtr,(U32)gMMI_nvram.timerCtl.SniffTimer*64L);
						}
					}
					else
					{
						gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffTryCnt = 0;
					}
					break;
				default:
					break;
			}
		}
		else
		{
			if(MMI_IsSniffAllowed(linkIndex))
			{
				SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].sniffTimerPtr,(U32)gMMI_nvram.timerCtl.SniffTimer*64L);
			}
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_IOCapabilityRequest(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef SUPPORT_SIMPLE_PAIRING
	U8 linkIndex;
	if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.ioCapReqEvt.BdAddr)) < MAX_MULTI_POINT_NO)
	{
		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_IO_CAPABILITY_REQ_REPLY_STRU);
		msgPtr->msgOpcode = HCI_CMD_IO_CAPABILITY_REQUEST_REPLY;
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.isIoCapRespSet)
		{
			if(	gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteIoCap == IO_CAP_PARA_DISPLAY_YES_NO 
				&& gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteAuthReq == AUTH_REQUIRE_PARA_GENERAL_BOND_IO_CAP
			  )
			{
				msgPtr->msgBodyPtr.hciCmd.ioCapReqReplyCmd.IoCapability = gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteIoCap;
				msgPtr->msgBodyPtr.hciCmd.ioCapReqReplyCmd.OobDataPresent = gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteOobAuthData;
				msgPtr->msgBodyPtr.hciCmd.ioCapReqReplyCmd.AuthRequire = gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteAuthReq;			
			}
			else
			{
				goto SEND_DEFAULT_IOCAP;
			}
		}
		else
		{
			SEND_DEFAULT_IOCAP:
			msgPtr->msgBodyPtr.hciCmd.ioCapReqReplyCmd.IoCapability = MMI_SIMPLE_PAIRING_PARA.ioCapabilities;
			msgPtr->msgBodyPtr.hciCmd.ioCapReqReplyCmd.OobDataPresent = MMI_SIMPLE_PAIRING_PARA.oobAuthData;
			msgPtr->msgBodyPtr.hciCmd.ioCapReqReplyCmd.AuthRequire = MMI_SIMPLE_PAIRING_PARA.authRequirement;		
		}
		gMMI_ctl.mmiInfo[linkIndex].linkPara.isIoCapRespSet = FALSE;
	}
	else
	{
		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_IO_CAPABILITY_REQ_NEG_REPLY_STRU);
		msgPtr->msgOpcode = HCI_CMD_IO_CAPABILITY_REQ_NEGATIVE_REPLY;
		msgPtr->msgBodyPtr.hciCmd.ioCapReqNegReplyCmd.Reason = ERR_UNKNOWN_CONNECTION_IDENTIFIER;
	}
	MMI_HCI_SendCmd(msgPtr);
#else
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
#endif
}

PRIVATE void MMI_HCI_IOCapabilityResponse(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;
	
	if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.ioCapRespEvt.BdAddr)) < MAX_MULTI_POINT_NO)
	{
		gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteIoCap = msgPtr->msgBodyPtr.hciEvt.ioCapRespEvt.IOCapability;
		gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteOobAuthData = msgPtr->msgBodyPtr.hciEvt.ioCapRespEvt.OOBDataPresent;
		gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteAuthReq = msgPtr->msgBodyPtr.hciEvt.ioCapRespEvt.AuthRequirements;
		gMMI_ctl.mmiInfo[linkIndex].linkPara.isIoCapRespSet = TRUE;
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_UserConfirmationRequest(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef SUPPORT_SIMPLE_PAIRING
	U8 linkIndex;

	MMI_PushMediaEvent(MEDIA_EVT_CONFIRMATION_REQUEST);
	if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.userConfirmNegReplyCmd.BdAddr)) < MAX_MULTI_POINT_NO)
	{
		if(MMI_CONFIRMATION_ACCEPT_REJECT_VIA_KEY)
		{
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_CONFIRMATION_REQUEST, (U8 XDATA_PTR)NULL, linkIndex);
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
		}
		else
		{
			msgPtr->dataOffset = CMD_OFFSET;
			msgPtr->paraLen = 6;
			msgPtr->msgOpcode = HCI_CMD_USR_CONFIRMATION_REQ_REPLY;
			MMI_HCI_SendCmd(msgPtr);
		}
	}
	else
	{
		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->paraLen = 6;
		msgPtr->msgOpcode = HCI_CMD_USR_CONFIRMATION_REQ_NEGATIVE_REPLY;
		MMI_HCI_SendCmd(msgPtr);
	}
#else
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
#endif
}

PRIVATE void MMI_HCI_RoleChange(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef SUPPORT_SIMPLE_PAIRING
	U8 linkIndex;

	if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.BdAddr)) < MAX_MULTI_POINT_NO)
	{
		MMI_HCI_ClearCmdState(HCI_CMD_SWITCH_ROLE, linkIndex, MMI_EOF, MMI_EOF);
		
		if(msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.Status == STATUS_OK)
		{
			MMI_HCI_UpdateRole(linkIndex, (msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.NewRole == HCI_EVT_ROLE_CHANGE_IS_SLAVE)?ROLE_SLAVE:ROLE_MASTER);
			if(MMI_IsActiveLink(linkIndex)) //This should be send after connection complete
			{
				MMI_HCI_SendChangeConnectionPacketType (linkIndex, LINK_TYPE_ACL_CONNECTION_REQUEST);
				if(gMMI_ctl.mmiInfo[linkIndex].linkPara.QosServiceType == SERVICE_TYPE_BEST_EFFORT || gMMI_ctl.mmiInfo[linkIndex].linkPara.QosServiceType == SERVICE_TYPE_GUARANTEED) //Do not send Qos if no need (PS Vita)
				{
					MMI_HCI_SendVCmdAudioQosSetup(linkIndex, gMMI_ctl.mmiInfo[linkIndex].linkPara.QosServiceType, gMMI_ctl.mmiInfo[linkIndex].linkPara.QosFlowStatus, FALSE);
				}
			}
		}
		if(api_profile_func[API_GRP_PF_CB_HCI_ROLE_CHANGED_EVT])
		{
			((API_PROFILE_HCI_ROLE_CHANGED_EVT_FUNC)api_profile_func[API_GRP_PF_CB_HCI_ROLE_CHANGED_EVT])(linkIndex, msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.Status, (msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.NewRole == HCI_EVT_ROLE_CHANGE_IS_SLAVE)?ROLE_SLAVE:ROLE_MASTER);
		}
		#ifdef AIR_MODULE
		LightDbgPrint("MMI - Role: %d,%d", (U8)msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.Status, (U8)msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.NewRole);
		MMI_AIR_HCIRoleChangeEventNotify(linkIndex, msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.Status, (msgPtr->msgBodyPtr.hciEvt.roleChangeEvt.NewRole)? TRUE: FALSE);
		#endif
		if(gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer)
		{
			SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer, 0);
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
#else
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
#endif
}

PRIVATE void MMI_HCI_SCOPacketType(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef SUPPORT_SIMPLE_PAIRING
	U8 linkIndex;

	if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.scoPktTypeEvt.BdAddr)) < MAX_MULTI_POINT_NO)
	{
		gMMI_ctl.mmiInfo[linkIndex].linkPara.scoPktType = msgPtr->msgBodyPtr.hciEvt.scoPktTypeEvt.PacketType;
	}
#endif
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PUBLIC void MMI_HCI_SendToolCommandResponse(U8 ocf, U8 resultCode)
{
	MMIMsgType XDATA_PTR msgPtr;
	if(msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + sizeof(HCI_VCMD_PARA_AIR_CMD_RESP);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = ocf;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_TOOL_CMD;
		msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmd.resultCode = resultCode;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendToolCommandResponseWithData(U8 ocf, U8 resultCode, U16 reportADCvalue)
{
	MMIMsgType XDATA_PTR msgPtr;
	if(msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + sizeof(HCI_VCMD_PARA_AIR_CMD_RESP_DATA);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = ocf;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_TOOL_CMD;
		msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdData.resultCode = resultCode;
		msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdData.ResDataLow=(U8)reportADCvalue;
		msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdData.ResDataHigh=(U8)(reportADCvalue>>8);
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_ToolDSPRefGainCmdResponse(U8 ocf, U8 resultCode)
{
	MMIMsgType XDATA_PTR msgPtr;

	if (resultCode == HCI_VEVT_OCF_RESULT_CODE_SUCCESS)
	{
		if (msgPtr=MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART) )
		{
			msgPtr->paraLen=sizeof(HCI_VCMD_OPCODE_STRU)+sizeof(HCI_VCMD_PARA_AIR_CMD_RESP_DSP_DATA);
			msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = ocf;
			msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_TOOL_CMD;
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.resultCode = resultCode;
			// Respond DSP data
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.CH1_REF_GAIN = 	gAlignDSP.Mailbox.d2m_ctl.evt_word.para.dsp_report_para.ref_gain_para.CH1_REF_GAIN;
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.CH2_REF_GAIN = 	gAlignDSP.Mailbox.d2m_ctl.evt_word.para.dsp_report_para.ref_gain_para.CH2_REF_GAIN;
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.CH3_REF_GAIN = 	gAlignDSP.Mailbox.d2m_ctl.evt_word.para.dsp_report_para.ref_gain_para.CH3_REF_GAIN;
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.CH4_REF_GAIN = 	gAlignDSP.Mailbox.d2m_ctl.evt_word.para.dsp_report_para.ref_gain_para.CH4_REF_GAIN;
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.CH1_REF2_GAIN = gAlignDSP.Mailbox.d2m_ctl.evt_word.para.dsp_report_para.ref_gain_para.CH1_REF2_GAIN;
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.CH2_REF2_GAIN = gAlignDSP.Mailbox.d2m_ctl.evt_word.para.dsp_report_para.ref_gain_para.CH2_REF2_GAIN;
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.CH3_REF2_GAIN = gAlignDSP.Mailbox.d2m_ctl.evt_word.para.dsp_report_para.ref_gain_para.CH3_REF2_GAIN;
			msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmdDSPData.CH4_REF2_GAIN = gAlignDSP.Mailbox.d2m_ctl.evt_word.para.dsp_report_para.ref_gain_para.CH4_REF2_GAIN;
			
			MMI_HCI_SendCmd(msgPtr);    
		}
	}
	/*
	else
	{
		if (msgPtr=MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART) )
		{
			msgPtr->paralen=sizeof(HCI_VCMD_OPCODE_STRU)+sizeof(HCI_VCMD_PARA_AIR_CMD_RESP);
			msgPtr->msgBodyPtr.hciVCmdDSPData.VOpCode.VCmdOcf = ocf;
			msgPtr->msgBodyPtr.hciVCmdDSPData.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_TOOL_CMD;
			msgPtr->msgBodyPtr.hciVCmdDSPData.VParaDSPData.respVEvtCmdDSPData.resultCode = resultCode;

			MMI_HCI_SendCmd(msgPtr);    
		}
	}
	*/
}

PUBLIC void MMI_Tool_DSPSuspended_Event(void)
{
	MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_SUSPEND_DSP, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_ToolCmd_EnableAudioBIST()
{
	U8 misc = gMMI_DRV_ctl.isMiscEnabled;
	
	gMMI_DRV_ctl.isMiscEnabled |= IS_LED_DISABLED;
	if(gMMI_DRV_ctl.isMiscEnabled != misc)
		DRIVER_Cmd_LED_Restart(DRIVER_CMD_LED_RESTART_PARA_FG_AND_BG);
	
	gMMI_DRV_ctl.isMiscEnabled |= IS_VP_DISABLED;
	MMI_DRV_SetButtonLock(TRUE);
	HC_DisableLightDbgPrint();
}

PRIVATE void MMI_HCI_ToolCommand(MMIMsgType XDATA_PTR msgPtr)
{
	switch(msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf)
	{
		case HCI_VCMD_OCF_TOOL_STOP_VP:
			MMI_DRV_SendVoicepromptStopCmd();
			MMI_DRV_SendRingtoneStopCmd();
			gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled |=  (IS_VP_DISABLED|IS_RINGTONE_DISABLED);
			gMMI_DRV_ctl.isMiscEnabled |= IS_VP_DISABLED;
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_STOP_VP, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			break;
		case HCI_VCMD_OCF_TOOL_SUSPEND_DSP:
			MMI_DRV_SuspendDSP(DRV_SUSPEND_DSP_REASON_TOOL, MMI_Tool_DSPSuspended_Event);
			break;
		case HCI_VCMD_OCF_TOOL_RESUME_DSP:
			MMI_DRV_ResumeDSP(DRV_SUSPEND_DSP_REASON_TOOL);
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_RESUME_DSP, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			break;
		case HCI_VCMD_OCF_TOOL_DISABLE_LIGHT_DBG_PRINT:
			HC_DisableLightDbgPrint();
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_DISABLE_LIGHT_DBG_PRINT, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			break;
		case HCI_VCMD_OCF_TOOL_ENABLE_LIGHT_DBG_PRINT:
			HC_EnableLightDbgPrint();
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_ENABLE_LIGHT_DBG_PRINT, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			break;
		case HCI_VCMD_OCF_TOOL_GET_ADC_DATA:
			MMI_DRV_ADCRealTimeReading(msgPtr->msgBodyPtr.hciVCmd.VPara.AdcChannel);
			break;
		case HCI_VCMD_OCF_TOOL_GET_DSP_REF_GAIN:
			MMI_DRV_DSPRefGainCmd();			
			break;
		case HCI_VCMD_OCF_TOOL_ENTER_MP_STATE:
			MMI_DRV_EnterMPStateCmd();
			break;
		case HCI_VCMD_OCF_TOOL_SET_VOL_GAIN:
			MMI_DRV_ToolSetVolGain(&msgPtr->msgBodyPtr.hciVCmd.VPara.getVolCmd);
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_SET_VOL_GAIN, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			break;
		case HCI_VCMD_OCF_TOOL_SET_CURRENT_GAIN:
			MMI_DRV_ToolSetCurrentGain(&msgPtr->msgBodyPtr.hciVCmd.VPara.setCurrGainCmd);
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_SET_CURRENT_GAIN, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			break;
		case HCI_VCMD_OCF_TOOL_AUDIO_BIST:
			MMI_HCI_ToolCmd_EnableAudioBIST();
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_AUDIO_BIST, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			break;
		case HCI_VCMD_OCF_TOOL_NTC_RATIO_VALUE_REPORT:
			MMI_DRV_NTCValueReport();
			break;
		#ifdef PEQ_ENABLE
		case HCI_VCMD_OCF_TOOL_SET_REALTIME_PEQ_HPF:
			MMI_DRV_RealTimeSetPeqHpf(msgPtr);
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_SET_REALTIME_PEQ_HPF, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			return;
		#endif
		default:
			MMI_HCI_SendToolCommandResponse(HCI_VCMD_OCF_TOOL_RESUME_DSP, HCI_VEVT_OCF_RESULT_CODE_FAIL);
			break;
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_GeneralV2CmdCpl(MMIMsgType XDATA_PTR msgPtr)
{
	if(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf == HCI_VEVT_OCF_COMMAND_COMPLETE_V2)
	{
		if(msgPtr->msgBodyPtr.hciVEvt.VPara.vCmdCplPara.vOcf == HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR && msgPtr->msgBodyPtr.hciVEvt.VPara.vCmdCplPara.vOgf == HCI_VENDOR_OGF_GENERAL_V2)
		{
			if(msgPtr->msgBodyPtr.hciVEvt.VPara.vCmdCplPara.status == HCI_ERRCODE_SUCCESS)
			{
				LightDbgPrint("==WriteBdaddr OK!!!");
				MMI_HCI_ClearCmdState(HCI_VCMD_AIROHA, MMI_EOF, HCI_VENDOR_OGF_GENERAL_V2, HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR);
				#ifdef AIR_MODULE
				MMI_AIR_WriteLocalBdAddrCpl(TRUE);
				#endif
			}
			else
			{
				LightDbgPrint("==WriteBdaddr Fail!!!");
				#ifdef AIR_MODULE
				MMI_AIR_WriteLocalBdAddrCpl(FALSE);
				#endif
				MMI_HCI_ClearCmdState(HCI_VCMD_AIROHA, MMI_EOF, HCI_VENDOR_OGF_GENERAL_V2, HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR);				
			}
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}


PRIVATE void MMI_HCI_AirohaCmd_ReconnectToIndex(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_ReadBdAddr(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_WriteDeviceName(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_QueryLinkHistory(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_Connectable(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_NoneConnectable(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_DisconnectLinkByBdAddr(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_CheckChargerStatus(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_StopVoicePrompt(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_PlayVoicePrompt(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_QueryServiceIndex(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_VolumeSet(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_PairingConfirm(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_ChangeAudioSel(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_SetAVRCPVolume(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_HighQualityCodec(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_QueryLinkKeyByBdAddr(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_I2CRead(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_I2CWrite(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_GetRemoteName(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_GetFWVersion(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_DSPFreqSelect(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_ProfileSupport(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_FindMyAccessory(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_Inquiry(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_CreateConnection(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_UnSupported(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_KeyEvent(MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_HCI_AirohaCmd_SecondKeyEvent(MMIMsgType XDATA_PTR msgPtr);

typedef struct
{
	void (*airohaCmdFunc)(MMIMsgType XDATA_PTR msgPtr);
}MmiHciAirohaCmdFuncType;

MmiHciAirohaCmdFuncType CODE uartCmdFunc[UART_CMD_NUM] = 
{
	MMI_HCI_AirohaCmd_ReconnectToIndex, 		//UART_CMD_RECONNECT_TO_INDEX = UART_CMD_START = 0xA0,
	MMI_HCI_AirohaCmd_ReadBdAddr, 				//UART_CMD_READ_BD_ADDR,
	MMI_HCI_AirohaCmd_WriteDeviceName,			//UART_CMD_WRITE_DEVICE_NAME,
	MMI_HCI_AirohaCmd_QueryLinkHistory,			//UART_CMD_QUERY_LINK_HISTORY,
	MMI_HCI_AirohaCmd_Connectable,				//UART_CMD_CONNECTABLE,
	MMI_HCI_AirohaCmd_NoneConnectable,			//UART_CMD_NONE_CONNECTABLE,
	MMI_HCI_AirohaCmd_DisconnectLinkByBdAddr,	//UART_CMD_DISCONNECT_SPECIFIC_LINK,
	MMI_HCI_AirohaCmd_CheckChargerStatus,		//UART_CMD_CHECK_CHARGER_STATUS,
	MMI_HCI_AirohaCmd_StopVoicePrompt,			//UART_CMD_STOP_VOICEPROMPT,
	MMI_HCI_AirohaCmd_PlayVoicePrompt,			//UART_CMD_PLAY_VOICE_PROMPT,
	MMI_HCI_AirohaCmd_QueryServiceIndex,		//UART_CMD_QUERY_SERVICE_INDEX,
	MMI_HCI_AirohaCmd_VolumeSet,				//UART_CMD_VOLUME_SET,
	MMI_HCI_AirohaCmd_PairingConfirm,			//UART_CMD_PAIRING_CONFIRM,
	MMI_HCI_AirohaCmd_ChangeAudioSel,			//UART_CMD_CHANGE_AUDIO_SEL,
	MMI_HCI_AirohaCmd_SetAVRCPVolume,			//UART_CMD_SET_AVRCP_VOL,
	MMI_HCI_AirohaCmd_HighQualityCodec,			//UART_CMD_HIGH_QUALITY_AUDIO_CODEC,

	//0xBX
	MMI_HCI_AirohaCmd_QueryLinkKeyByBdAddr,		//UART_CMD_QUERY_LINKKEY_BY_BDADDR,
	MMI_HCI_AirohaCmd_I2CRead,					//UART_CMD_I2C_READ,
	MMI_HCI_AirohaCmd_I2CWrite,					//UART_CMD_I2C_WRITE,
	MMI_HCI_AirohaCmd_GetRemoteName,			//UART_CMD_GET_REMOTE_NAME,
	MMI_HCI_AirohaCmd_GetFWVersion,				//UART_CMD_GET_FW_VERSION,  	
	MMI_HCI_AirohaCmd_DSPFreqSelect,			//UART_CMD_DSP_FREQUENCT_SELECT,
	MMI_HCI_AirohaCmd_ProfileSupport,			//UART_CMD_SET_PROFILE_SUPPORT_STATUS,
	MMI_HCI_AirohaCmd_FindMyAccessory,			//UART_CMD_FIND_MY_ACCESSORY,
	MMI_HCI_AirohaCmd_Inquiry, 					//UART_CMD_INQUIRY,
	MMI_HCI_AirohaCmd_CreateConnection,			//UART_CMD_CREATE_CONNECTION,
};

PRIVATE void MMI_HCI_UARTCommand(MMIMsgType XDATA_PTR msgPtr)
{
	if(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf < KEY_TOTAL_NO)
	{
		MMI_HCI_AirohaCmd_KeyEvent(msgPtr);
	}
	else if(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf >= UART_CMD_START && msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf < UART_CMD_END)
	{
		uartCmdFunc[msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf-UART_CMD_START].airohaCmdFunc(msgPtr);
	}
	else
	{
		MMI_HCI_AirohaCmd_UnSupported(msgPtr);
	}
	
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_ExtUARTCommand(MMIMsgType XDATA_PTR msgPtr)
{
	if(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf == HCI_VENDOR_OCF_SECOND_KEY_EVENT_CMD)
	{
		if(msgPtr->msgBodyPtr.hciVEvt.VPara.extendedPara.OCF >= KEY_SECOND_TABLE_START_INDEX && msgPtr->msgBodyPtr.hciVEvt.VPara.extendedPara.OCF < KEY_SECOND_TABLE_END)
		{
			MMI_HCI_AirohaCmd_SecondKeyEvent(msgPtr);
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
			return;
		}
	}
	MMI_HCI_AirohaCmd_ExtSendResponse(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf, msgPtr->msgBodyPtr.hciVEvt.VPara.extendedPara.OCF, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PUBLIC void MMI_HCI_SendSPPCmdResponse(U8 cmdOpcode, U8 resultCode)
{
	MMIMsgType XDATA_PTR msgPtr;
	msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART);
	if(msgPtr)
	{
		msgPtr->paraLen = 3;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = cmdOpcode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_SPP;
		msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmd.resultCode = resultCode;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PRIVATE void MMI_HCI_SPPCommand(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef SPP_Profile
	U8 linkIndex;
	if (MMI_SPP_DATA_TO_UART_FEAT)
	{
		if (msgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode == HCI_VEVT_OCF_SPP_TX_DATA_TO_REMOTE_REQ
			#ifdef SONGPAL_Profile
			|| msgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode == HCI_VEVT_OCF_SONGPAL_ANDROID_TX_DATA_TO_REMOTE_REQ
			|| msgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode == HCI_VEVT_OCF_SONGPAL_IAP2_TX_DATA_TO_REMOTE_REQ
			#endif
            #ifdef IAP2_Profile
            || msgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode == HCI_VEVT_OCF_IAP2_TX_DATA_TO_REMOTE_REQ
            #endif
			)
		{
			MMI_SwitchEndianBDAddr((U8 XDATA_PTR)msgPtr->msgBodyPtr.hciUARTSppData.bdAddr.member);

			if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTSppData.bdAddr)) < MAX_MULTI_POINT_NO)
			{
				msgPtr->msgOpcode = msgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode;
				msgPtr->paraLen -= (sizeof(HciUARTSppDataType) - 1);
				MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTSppData.bdAddr.member);

                #ifdef IAP2_Profile
                if(msgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode == HCI_VEVT_OCF_IAP2_TX_DATA_TO_REMOTE_REQ)
                {
					IAP2_SendExtASessionFromUART(linkIndex, (U8 XDATA_PTR)msgPtr);
                }
                else
                #endif
                {
                    OSMQ_Put(OSMQ_SPP_Command_From_MMI_ptr[linkIndex], (U8 XDATA_PTR)msgPtr);
                }

				MMI_HCI_SendSPPCmdResponse((U8)(msgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode + HCI_VEVT_OCF_SPP_TX_DATA_TO_REMOTE_RSP - HCI_VEVT_OCF_SPP_TX_DATA_TO_REMOTE_REQ),
											HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
				MMI_SniffDisable(linkIndex, MMI_SNIFF_SPP);
				return;
			}
			else
			{
				MMI_HCI_SendSPPCmdResponse((U8)(msgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode + HCI_VEVT_OCF_SPP_TX_DATA_TO_REMOTE_RSP - HCI_VEVT_OCF_SPP_TX_DATA_TO_REMOTE_REQ),
											HCI_VEVT_OCF_RESULT_CODE_FAIL);
			}
		}
	}
#endif
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

#ifdef TRSPX_App
PRIVATE void MMI_HCI_SendKeypressNtf(MMIMsgType XDATA_PTR msgPtr)
{
	msgPtr->dataOffset = CMD_OFFSET;
	msgPtr->msgOpcode = HCI_CMD_SEND_KEYPRESS_NOTIFICATION;
	msgPtr->paraLen = 7;
    OSMEM_memcpy_xdata_xdata(msgPtr->msgBodyPtr.hciSendPresskeyNtfCmd.bdAddr.member, msgPtr->msgBodyPtr.hciUARTTrspXData.bdAddr.member, sizeof(BD_ADDR));
    msgPtr->msgBodyPtr.hciSendPresskeyNtfCmd.ntfType = msgPtr->msgBodyPtr.hciUARTTrspXData.para.pkRsp.ntfType;

	MMI_HCI_SendCmd(msgPtr);
}

PRIVATE void MMI_HCI_PasskeyReply(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
    MMIMsgType XDATA_PTR hciPtr;

    if (((hciPtr = (MMIMsgType XDATA_PTR)SYS_MemoryGet((OS_OFFSET_OF(MMIMsgType, msgBodyPtr)) + 10))) != NULL)
    {
    	hciPtr->dataOffset = CMD_OFFSET;
        OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&hciPtr->msgBodyPtr.hciCmd.userPasskeyReqReplyCmd.BdAddr, msgPtr->msgBodyPtr.hciSendPresskeyNtfCmd.bdAddr.member, sizeof(BD_ADDR));
        if (gMMI_ctl.passkey[linkIndex] != MMI_TRSPX_PASSKEY_INVALID)
        {
            hciPtr->msgOpcode = HCI_CMD_USR_PASSKEY_REQ_REPLY;
            hciPtr->paraLen = sizeof(HCI_CMD_PARA_USER_PASSKEY_REQ_REPLY_STRU); //10;
			MMI_HCI_SetU32((U16 XDATA_PTR)&hciPtr->msgBodyPtr.hciCmd.userPasskeyReqReplyCmd.NumericValue, gMMI_ctl.passkey[linkIndex]);
        }
        else
        {
            hciPtr->msgOpcode = HCI_CMD_USR_PASSKEY_REQ_NEGATIVE_REPLY;
            hciPtr->paraLen = sizeof(BD_ADDR);
        }
		MMI_HCI_SendCmd(msgPtr);
    }
}

PRIVATE U8 MMI_EtrKeyMapValue(U8 etrKey)
{
    if (etrKey >= 0x30 && etrKey <= 0x39 )
    {
        return (etrKey - 0x30);
    }
    else
    {
        return MMI_TRSPX_ETRKEY_INVALID_VALUE;
    }
}

PRIVATE void MMI_HCI_UserCnfReply(MMIMsgType XDATA_PTR msgPtr)
{
    msgPtr->dataOffset = CMD_OFFSET;
    msgPtr->paraLen = sizeof(BD_ADDR);
    OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.userConfirmNegReplyCmd.BdAddr, msgPtr->msgBodyPtr.hciUARTTrspXData.bdAddr.member, sizeof(BD_ADDR));
	msgPtr->msgOpcode = (msgPtr->msgBodyPtr.hciUARTTrspXData.para.option == MMI_TRSPX_DISPLAY_ENTERED_INFO_YES)? HCI_CMD_USR_CONFIRMATION_REQ_REPLY : HCI_CMD_USR_CONFIRMATION_REQ_NEGATIVE_REPLY;
	MMI_HCI_SendCmd(msgPtr);
}

#ifdef SUPPORT_LE_XPORT
PRIVATE void MMI_HCI_TrspXport2LeCommand(MMIMsgType XDATA_PTR msgPtr)
{
    switch (msgPtr->msgBodyPtr.hciUARTTrspXData.cmdOpCode)
    {
        case MMI_TRSPX_TX_DATA_FROM_LOCAL_REQ:
	    {
			msgPtr->msgOpcode = SPP_CMD_FROM_MMI_DATA;
			msgPtr->paraLen -= (OS_OFFSET_OF(HciUARTSppDataType, dataBegin));
			OSMQ_Put(OSMQ_SPP_CMD_To_LE_XPORT_ptr, (U8 XDATA_PTR)msgPtr);
            return;
        }
        break;
    }
    OSMEM_Put((U8 XDATA_PTR)msgPtr);
}
#endif

PRIVATE void MMI_HCI_TrspXport2SPPCommand(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;
    switch (msgPtr->msgBodyPtr.hciUARTTrspXData.cmdOpCode)
    {
        case MMI_TRSPX_TX_DATA_FROM_LOCAL_REQ:
	    {
			MMI_SwitchEndianBDAddr((U8 XDATA_PTR)msgPtr->msgBodyPtr.hciUARTTrspXData.bdAddr.member);
			if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTTrspXData.bdAddr)) < MAX_MULTI_POINT_NO)
			{
				msgPtr->msgOpcode = SPP_CMD_FROM_MMI_DATA;
				msgPtr->paraLen -= (OS_OFFSET_OF(HciUARTSppDataType, dataBegin));
				OSMQ_Put(OSMQ_SPP_Command_From_MMI_ptr[linkIndex], (U8 XDATA_PTR)msgPtr);
				MMI_SniffDisable(linkIndex, MMI_SNIFF_SPP);
				return;
			}
        }
        break;

        case MMI_TRSPX_PASSKEY_ENTRY_RSP:
        {
			MMI_SwitchEndianBDAddr((U8 XDATA_PTR)msgPtr->msgBodyPtr.hciUARTTrspXData.bdAddr.member);
			if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTTrspXData.bdAddr)) < MAX_MULTI_POINT_NO)
			{
			    switch (msgPtr->msgBodyPtr.hciUARTTrspXData.para.pkRsp.ntfType)
                {
                    case MMI_TRSPX_PASSKEY_DIGIT_ENTERED:
                        if (gMMI_ctl.passkey[linkIndex] == MMI_TRSPX_PASSKEY_INVALID)
                        {
                            gMMI_ctl.passkey[linkIndex] = 0;
                        }
                        if (MMI_EtrKeyMapValue(msgPtr->msgBodyPtr.hciUARTTrspXData.para.pkRsp.etrPasskey) != MMI_TRSPX_ETRKEY_INVALID_VALUE)
                        {
                            gMMI_ctl.passkey[linkIndex] = gMMI_ctl.passkey[linkIndex] * 10 + MMI_EtrKeyMapValue(msgPtr->msgBodyPtr.hciUARTTrspXData.para.pkRsp.etrPasskey);

                            MMI_HCI_SendKeypressNtf(msgPtr);
                            return;
                        }
                        break;

                    case MMI_TRSPX_PASSKEY_DIGIT_ERASED:
                        MMI_HCI_SendKeypressNtf(msgPtr);
                        if (gMMI_ctl.passkey[linkIndex] != MMI_TRSPX_PASSKEY_INVALID)
                        {
                            gMMI_ctl.passkey[linkIndex] /= 10;
                        }
                        return;

                    case MMI_TRSPX_PASSKEY_CLEARED:
                        MMI_HCI_SendKeypressNtf(msgPtr);
                        gMMI_ctl.passkey[linkIndex] = MMI_TRSPX_PASSKEY_INVALID;
                        return;

                    case MMI_TRSPX_PASSKEY_ENTRY_COMPLETED:
                        MMI_HCI_SendKeypressNtf(msgPtr);
                        MMI_HCI_PasskeyReply(linkIndex, msgPtr);
                        return;

                    default:
                        break;
                }
            }
        }
        break;

        case MMI_TRSPX_DISPLAY_YES_OR_NO_RSP:
        {
			MMI_SwitchEndianBDAddr((U8 XDATA_PTR)msgPtr->msgBodyPtr.hciUARTTrspXData.bdAddr.member);
			if (MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTTrspXData.bdAddr) < MAX_MULTI_POINT_NO)
			{
			    MMI_HCI_UserCnfReply(msgPtr);
                return;
            }
        }
        break;
    }
    OSMEM_Put((U8 XDATA_PTR)msgPtr);
}
#endif

#ifdef TRSPX_App
PRIVATE void MMI_HCI_TrspXportCommand(MMIMsgType XDATA_PTR msgPtr)
{
    if (gMMI_ctl.connectedMode == MMI_TRSPX_CONNECTED_BR_EDR)
    {
        MMI_HCI_TrspXport2SPPCommand(msgPtr);
    }
#ifdef SUPPORT_LE_XPORT
    else
    {
        MMI_HCI_TrspXport2LeCommand(msgPtr);
    }
#endif
}
#endif

#ifdef HID_Profile
PRIVATE void MMI_HCI_HIDCommand(MMIMsgType XDATA_PTR msgPtr)
{
    U8 linkIndex;

    if (msgPtr->paraLen <= OS_OFFSET_OF(HciUARTHIDDataType, dataBegin))
    {
        OSMEM_Put((U8 XDATA_PTR)msgPtr);
        return;
    }
    if (msgPtr->msgBodyPtr.hciUARTHIDData.cmdOpCode == MMI_HID_OCF_UART_CMD)
    {
        if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTHIDData.bdAddr)) < MAX_MULTI_POINT_NO)
	    {
	        MMI_HID_ProcessUartCmd(linkIndex, msgPtr->paraLen - OS_OFFSET_OF(HciUARTHIDDataType, dataBegin), msgPtr->msgBodyPtr.hciUARTHIDData.dataBegin, (U8 XDATA_PTR)msgPtr);
	        return;
	    }
    }
	
    else if (msgPtr->msgBodyPtr.hciUARTHIDData.cmdOpCode == MMI_HID_OCF_PIN_CODE_CMD)
    {
        MMIMsgType XDATA_PTR sendPtr;

        if (sendPtr = SYS_MemoryGet(OS_OFFSET_OF(MMIMsgType, msgBodyPtr) + sizeof(HCI_CMD_PARA_PIN_CODE_REQ_REPLY_STRU)))
        {
            sendPtr->dataOffset = CMD_OFFSET;
        	sendPtr->msgOpcode = HCI_CMD_PIN_CODE_REQUEST_REPLY;
        	sendPtr->paraLen = 23;
        	sendPtr->msgBodyPtr.hciCmd.pinCodeReqReplyCmd.PinCodeLen = msgPtr->msgBodyPtr.hciUARTHIDData.dataBegin[0];
        	OSMEM_memcpy_xdata_xdata(sendPtr->msgBodyPtr.hciCmd.pinCodeReqReplyCmd.BdAddr, (U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTHIDData.bdAddr, 6);
        	MMI_ReadNvramDataChunk ((U8 XDATA_PTR)&sendPtr->msgBodyPtr.hciCmd.pinCodeReqReplyCmd.PinCode, msgPtr->msgBodyPtr.hciUARTHIDData.dataBegin + 1, msgPtr->msgBodyPtr.hciUARTHIDData.dataBegin[0]);
        	MMI_HCI_SendCmd(sendPtr);
        	MMI_PushMediaEvent(MEDIA_EVT_PINCODE_REQUEST);
    	}
    }
	
    OSMEM_Put((U8 XDATA_PTR)msgPtr);
}
#endif

PRIVATE void MMI_HCI_PasskeyRequest(MMIMsgType XDATA_PTR msgPtr)
{
    #ifdef TRSPX_App
	//U8 linkIndex;
	if (MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.userPasskeyReqEvt.BdAddr) < MAX_MULTI_POINT_NO)
    {
        MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.userPasskeyReqEvt.BdAddr);
        msgPtr->paraLen = sizeof(BD_ADDR);
        MMI_HCI_SendTrspXUARTEvent(msgPtr, MMI_TRSPX_PASSKEY_REQ);
    }
    else
    {
        OSMEM_Put((U8 XDATA_PTR)msgPtr);
    }
    #else
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
    #endif
}

PRIVATE void MMI_HCI_SSPCmpEvt(MMIMsgType XDATA_PTR msgPtr)
{
    #ifdef TRSPX_App
    if (MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciSSPEvt.bdAddr) < MAX_MULTI_POINT_NO)
    {
        MMI_SwitchEndianBDAddr((U8 XDATA_PTR)msgPtr->msgBodyPtr.hciSSPEvt.bdAddr.member);
        *((U8 XDATA_PTR)msgPtr->msgBodyPtr.hciSSPEvt.bdAddr.member + sizeof(BD_ADDR)) = msgPtr->msgBodyPtr.hciSSPEvt.status;
        msgPtr->paraLen = sizeof(HciSSPEvent);
        MMI_HCI_SendTrspXUARTEvent(msgPtr, MMI_TRSPX_PAIRING_CMP);
    }
    else
    {
        OSMEM_Put((U8 XDATA_PTR)msgPtr);
    }
    #else
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
    #endif
}

PRIVATE void MMI_HCI_ReadRemoteNameEvt(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;

	if(!msgPtr->msgBodyPtr.hciEvt.remoteNameRequestEvt.Status) // succeeded!! Null-ended
	{
		//Get link index
		if(!((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.remoteNameRequestEvt.BdAddr))==MAX_MULTI_POINT_NO))
		{
			//Update Remote device name to linkinfo
			OSMEM_memcpy_xdata_xdata(&gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[0],
										&msgPtr->msgBodyPtr.hciEvt.remoteNameRequestEvt.RemoteName[0], MAX_REMOTE_NAME_LEN);
			gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[MAX_REMOTE_NAME_LEN-1] = 0;

			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportDevName(&gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[0], MAX_REMOTE_NAME_LEN);
			MMI_HCI_SendReadRSSI(linkIndex);
			#endif
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_NumberOfCompletePktEvt(MMIMsgType XDATA_PTR msgPtr)
{
	#if 0
	U8 linkIndex, i;
	
	HCI_EVT_PARA_NUM_OF_CPL_PKT_STRU XDATA_PTR RtnDataPtr = (HCI_EVT_PARA_NUM_OF_CPL_PKT_STRU XDATA_PTR)&msgPtr->msgBodyPtr;
	for(i = 0; i < RtnDataPtr->NumOfHdl; i++)
	{
		U8 XDATA_PTR connPtr = (U8 XDATA_PTR)(&RtnDataPtr->ConnHdlB0 + (2*i));
		
		if((linkIndex = MMI_GetLinkIndexByConnHandle(ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)connPtr))) < MAX_MULTI_POINT_NO)
		{
			U16 numOfCplPkt = *((U8 XDATA_PTR)(&RtnDataPtr->ConnHdlB0 + 2 * RtnDataPtr->NumOfHdl + i * 2));
			L2CAP_AclTxCountHandler(linkIndex, numOfCplPkt);
		}
	}
	#endif
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_HCI_VendorEvt(MMIMsgType XDATA_PTR msgPtr)
{
	switch(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOgf)
	{
		case HCI_VENDOR_OGF_UART_TOOL_CMD:
			MMI_HCI_ToolCommand(msgPtr);
			break;
		case HCI_VENDOR_OGF_UART_CMD:
			MMI_HCI_UARTCommand(msgPtr);
			break;
		case HCI_VENDOR_OGF_UART_SPP:
			#ifdef TRSPX_App
			MMI_HCI_TrspXportCommand(msgPtr);
			#else
			MMI_HCI_SPPCommand(msgPtr);
			#endif
			break;
		#ifdef HID_Profile	
		case HCI_VENDOR_OGF_UART_HID:
		    MMI_HCI_HIDCommand(msgPtr);
		    break;
		#endif
		
		#ifdef AVRCP_Profile
		case HCI_VENDOR_OGF_UART_AVRCP:
			MMI_AVRCP_HCICommand(msgPtr);
			break;
		#endif
		case HCI_VENDOR_OGF_CUSTOMER_CMD:
			if(api_iosys_evt_func[API_GRP_IOSYS_CB_UART_CUST_CMD])
			{
				((API_IOSYS_UART_CUST_CMD_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_UART_CUST_CMD])((U8 XDATA_PTR)msgPtr);
			}
			else
			{
				OSMEM_Put((U8 XDATA_PTR)msgPtr);
			}
			break;
		case HCI_VENDOR_OGF_EXTENDED_TWO_BYTE_OCF_CMD:
			MMI_HCI_ExtUARTCommand(msgPtr);
			break;
			
		case HCI_VENDOR_OGF_GENERAL_V2:
			MMI_HCI_GeneralV2CmdCpl(msgPtr);
			break;

		#ifdef AIRAPP_Profile
		case HCI_VENDOR_OGF_AIRAPP_PASSTHROUGH_TRANSFER_CMD:
			if(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf == AIRAPP_CMD_PASSTHROUGH_FROM_UART_TO_BT)
			{
				MMI_PowerSaving(TRUE, MMI_PWRSAVING_ENABLE_ADV, MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.bdAddr));
				AirApp_PassThroughTransferData((U8 XDATA_PTR)msgPtr, AIRAPP_RSP_PASSTHROUGH_FROM_BT_TO_APP, HCI_VENDOR_OGF_AIRAPP_PASSTHROUGH_TRANSFER_RSP);
			}
			break;
		#endif
		default:
			OSMEM_Put((U8 XDATA_PTR)msgPtr);
			break;
	}
}

#ifdef TRSPX_App
PRIVATE void MMI_HCI_TrspXUserConfirmationRequest(MMIMsgType XDATA_PTR msgPtr)
{
    if (MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciEvt.userConfirmReqEvt.BdAddr) < MAX_MULTI_POINT_NO)
    {
        #ifdef TRSPX_SUPPORT_AUTO_CONFIRM
        msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->paraLen = 6;
		msgPtr->msgOpcode = HCI_CMD_USR_CONFIRMATION_REQ_REPLY;
		MMI_HCI_SendCmd(msgPtr);
        #else
        MMI_SwitchEndianBDAddr(&msgPtr->msgBodyPtr.hciEvt.userConfirmReqEvt.BdAddr);
        msgPtr->msgBodyPtr.hciEvt.userConfirmReqEvt.NumericValue = ENDIAN_TRANSFORM_U32(msgPtr->msgBodyPtr.hciEvt.userConfirmReqEvt.NumericValue);
        msgPtr->paraLen = sizeof(HCI_EVT_PARA_USER_CONF_REQ_STRU);
        MMI_HCI_SendTrspXUARTEvent(msgPtr, MMI_TRSPX_DISPLAY_YES_OR_NO_REQ);
        #endif
    }
    else
    {
        OSMEM_Put((U8 XDATA_PTR)msgPtr);
    }
}
#endif

PRIVATE void MMI_HCI_PairingFailHandle(U8 XDATA_PTR bdAddrPtr, U8 reason)
{
	U8 linkIndex, airLink;

	linkIndex = MMI_GetLinkIndexByBdA(bdAddrPtr);
	airLink = MMI_LinkKey_SearchAirLinkKeyIndex(bdAddrPtr, FALSE);
	if(linkIndex < MAX_MULTI_POINT_NO)
	{
		if(SYS_IsBDAddrIsTheSame((U8 XDATA_PTR)&gMMI_driver_variation_nvram.fast_dongle_link_history_info.linkData.bdAddr[0], bdAddrPtr))
		{
			goto RETRY_ONCE;
		}
		if(MMI_EOF != airLink && !MMI_AIR_CheckPairingTimeOutTimerExist())
		{
			goto RETRY_ONCE;
		}
		if(MMI_AIR_CheckPairingTimeOutTimerExist() && MMI_IsLinkActiveConnected(airLink))
		{
			MMI_LinkKey_UpdateHistory(bdAddrPtr, FALSE);
			MMI_HCI_SendAuthenticationRequested(linkIndex);
			gMMI_ctl.mmiInfo[linkIndex].linkPara.isPairFailed = TRUE;
			return;
		}
	
		switch((MMI_IsLinkActiveConnected(linkIndex))?gMMI_nvram.pariCtl.pairFailActionInActConn:gMMI_nvram.pariCtl.pairFailActionInPasConn)
		{
			case PAIR_FAIL_RETRY_ONCE:
				RETRY_ONCE:
				if(!gMMI_ctl.mmiInfo[linkIndex].linkPara.isPairFailed && reason != ERR_PIN_OR_KEY_MISSING)
				{
					MMI_HCI_SendAuthenticationRequested(linkIndex);
				}
				else
				{
					MMI_PushMediaEvent(MEDIA_EVT_PAIRING_FAIL);
					if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
					{
						MMI_TIMER_SetDetachTimer(linkIndex, 320L);
						MMI_ClearSequentialActiveProfileList(linkIndex);
						MMI_ClearProfileReconnectListQueue(linkIndex);
					}
					else
						MMI_TIMER_SetDetachTimer(linkIndex, (U32)(gMMI_nvram.timerCtl.waitDetachTime) * HALF_SEC);
					MMI_Reconnect_DeleteInvalidList();
					MMI_LinkKey_UpdateHistory(bdAddrPtr, FALSE);
				}
				break;
				
			case PAIR_FAIL_ALWAYS_RETRY:
				MMI_HCI_SendAuthenticationRequested(linkIndex);
				break;
				
			case PAIR_FAIL_DO_NOTHING:
				MMI_PushMediaEvent(MEDIA_EVT_PAIRING_FAIL);
				MMI_LinkKey_UpdateHistory(bdAddrPtr, FALSE);
				MMI_TIMER_SetDetachTimer(linkIndex, (U32) 30 * ONE_SEC);
				SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].waitDetachTimer, (U32)30 * ONE_SEC);
				MMI_ClearSequentialActiveProfileList(linkIndex);
				break;

			case PAIR_FAIL_DISC_DEFAULT:
			default:
				if(reason == ERR_PIN_OR_KEY_MISSING)
					MMI_Reconnect_DeleteInvalidList();
				if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_ACTIVE_CONNECT)
				{
					MMI_TIMER_SetDetachTimer(linkIndex, 320L);
					MMI_ClearSequentialActiveProfileList(linkIndex);
					MMI_ClearProfileReconnectListQueue(linkIndex);
				}
				else
					MMI_TIMER_SetDetachTimer(linkIndex, (U32)(gMMI_nvram.timerCtl.waitDetachTime) * HALF_SEC);

				MMI_LinkKey_UpdateHistory(bdAddrPtr, FALSE);
				break;
		}
		gMMI_ctl.mmiInfo[linkIndex].linkPara.isPairFailed = TRUE;
	}
}

PRIVATE void MMI_HCI_PairingSuccessfulHandle(U8 XDATA_PTR bdAddrPtr)
{
	MMI_LinkKey_UpdateHistory(bdAddrPtr, TRUE);
	MMI_PushMediaEvent(MEDIA_EVT_PAIRING_SUCCESSFUL);
	#ifdef AIR_MODULE
	MMI_AIR_LinkKeyNotify(bdAddrPtr);
	#endif
}

PRIVATE void _initsubState(void)//Mantis 8599 - avoid Warning C183 : Dead Assignment Eliminated
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)	
	{
		gMMI_ctl.mmiInfo[linkIndex].subState = MMI_INIT_SYS_CONFIGED;
	}	
}

PRIVATE void MMI_GAP_WriteSecurityModeComplete(MMIMsgType XDATA_PTR msgPtr)
{
	//Finish configuration. after power on
	if(MMI_KEY_STATE_POWER_ON == MMI_GetKeyState())
	{
		if(msgPtr->msgBodyPtr.gapWriteSecCpt.status == PROFILE_NO_ERROR)
		{
			_initsubState();

			MMI_ConnectabilityHandle(CMD_SECURITY_MODE_COMPLETE);
			MMI_AddTopState(MMI_EOF, MMI_CONNECTABLE);

			if(MMI_IsEnterDiscoverable())
			{
				MMI_SetEnterDiscoverable(FALSE);
				MMI_AddTopState(MMI_EOF, MMI_CONDISCABLE);
			}
			else
			{
				MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
			}

			if(MMI_LinkKey_IsLinkWatchdog())
			{
				SYS_SetTimer((OST XDATA_PTR XDATA_PTR) &gMMI_ctl.mmiTimerInfo.sysTimer.watchDogReconnectTimer, (U32) gMMI_nvram.timerCtl.mmiWatchDogReconnectTimer * 320L);
			}
			else if(MMI_AUTO_RECONN_AFTER_PWR_ON_FEAT )
			{
				MMI_Reconnect(RECONNECT_PWR_ON);
			}

			DRIVER_ResetLineInKeyTimer(); //MMI_LINE_IN_DETECT_BY_GPIO

			MMI_TIMER_ResetAutoSwitchOffTimer();
			MMI_SetKeyState(MMI_KEY_STATE_NONE);
		}
		else
		{
			ASSERT_PRINT("SECURITY MODE ERR!!");
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_GAP_WriteConnectabilityModeComplete(MMIMsgType XDATA_PTR msgPtr)
{
	//Corresponding to disabling MMI_PWRSAVING_WRITE_CONNECTABILITY flag in PowerOnLoader.
	if(MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState())
	{
		MMI_PowerSaving(TRUE, MMI_PWRSAVING_WRITE_CONNECTABILITY, MAX_MULTI_POINT_NO);
		if (msgPtr->msgBodyPtr.gapWriteConnetabilityModeCpt.connectability_mode != (SAC_NON_CONNECTABLE_MODE|NON_CONNECTABLE_MODE))
		{
			MMI_MMI1Handle(FALSE); //CONNECTABLE_RF_ON
		}
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_GAP_WriteDiscoverabilityModeComplete(MMIMsgType XDATA_PTR msgPtr)
{
	#ifdef AIR_MODULE
	MMI_AIR_GAPGetDiscoverabilityModeCpl(msgPtr->msgBodyPtr.gapWriteDiscoverabilityModeCpt.discoverability_mode);
	#endif

	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PRIVATE void MMI_ScanHandlerWhenSCO(BOOL isStart)
{
	if (MMI_SUPPORT_SCO_CLOSE_SCAN_FEAT)
	{
		if(isStart)
		{
			MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
			MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);
		}
		else
		{
			MMI_DiscoverabilityHandle(CMD_SET_RESUME_DISCOVERABLE);
			MMI_ConnectabilityHandle(CMD_SET_RESUME_CONNECTABLE);
		}
	}
}

PRIVATE void MMI_HCI_AcceptSCOReqProfileHandle(U8 linkIndex)
{
	//MMI_DRV_AudioDeviceRegister(linkIndex, MMI_CheckLinkInCallRelatedState(linkIndex)?AUDIO_DEVICE_SCO:AUDIO_DEVICE_SCO_IDLE);
	#ifdef HFP_Profile
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer);
	#else
	UNUSED(linkIndex);
	#endif
}

PRIVATE void MMI_HCI_ScoDiscHandle(U8 linkIndex)
{
	MMI_DRV_SetSwitchingSCO(FALSE);
	if(gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle == gMMI_ctl.driverCtl.audioSwitchHandler)
		gMMI_ctl.driverCtl.audioSwitchHandler = MMI_EOF_HANDLER;
	gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle = 0x00;	// SCO_ON: 9, 10 , 11
	gMMI_ctl.mmiInfo[linkIndex].linkPara.isSCOAlive = FALSE;
	gMMI_ctl.mmiInfo[linkIndex].linkPara.scoIsActiveEstablished = FALSE;
	gMMI_ctl.mmiInfo[linkIndex].linkPara.isSCODisconnecting = FALSE;

	if (SCO_EXISTS(linkIndex))
	{
		if (--gMMI_ctl.scoCnt == 0)
		{
			if(MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState()) // Not Power off
			{
				//There is no sco link now, MMI needs to change scan window parameters back to the original settings.
				MMI_ScanHandlerWhenSCO(FALSE);
			}
		}
		MMI_PushMediaEvent(MEDIA_EVT_AUDIO_LINK_CLOSE);
		MMI_HCI_SendWritePageScanActivity();
	}
	MMI_HCI_RemoveSCOAudioLink(linkIndex);
}

PRIVATE void MMI_HCI_ScoDiscProfileHandle(U8 linkIndex)
{
	#ifdef HFP_Profile
	MMI_HFP_SCODiscHandle(linkIndex);
	#endif
	#ifdef AIR_MODULE
	MMI_AIR_SCODiscNotify(linkIndex);
	#endif
	if(MMI_IsLinkActiveDisconnecting(linkIndex))
	{
		MMI_ReleaseProfileLinkAndDetach(linkIndex);
	}
}

PRIVATE void MMI_HCI_ScoConnOKHandle(U8 linkIndex)
{
	gMMI_ctl.mmiInfo[linkIndex].linkPara.isSCOAlive = TRUE;
	if(gMMI_ctl.scoCnt++ ==0)
	{
		MMI_HCI_SendWritePageScanActivity();
	}
	MMI_ScanHandlerWhenSCO(TRUE);

	MMI_PushMediaEvent(MEDIA_EVT_AUDIO_LINK_OPEN);

	MMI_HCI_AddSCOAudioLink(linkIndex);
}

PRIVATE void MMI_HCI_ScoConnOKProfileHandle(U8 linkIndex)
{
	#ifdef HFP_Profile
	MMI_HFP_SCOConnectedHandle(linkIndex);
	#endif
	#ifdef AIR_MODULE
	MMI_AIR_SCOConnectedNotify(linkIndex, TRUE);
	#endif
	#if !defined(HFP_Profile) && !defined(AIR_MODULE)
	UNUSED(linkIndex);
	#endif
}

PRIVATE void MMI_HCI_ScoConnFailHandle(U8 linkIndex)
{
	gMMI_ctl.mmiInfo[linkIndex].linkPara.scoIsActiveEstablished = FALSE;
	MMI_HCI_RemoveSCOAudioLink(linkIndex);
}

PRIVATE void MMI_HCI_ScoConnFailProfileHandle(U8 linkIndex)
{
	#ifdef HFP_Profile
	MMI_HFP_SCOConnectFailHandle(linkIndex);
    #else
    UNUSED(linkIndex);
	#endif
}

PRIVATE void MMI_HCI_AclDiscHandle(U8 linkIndex)
{
	BOOL isSCOExist = (SCO_EXISTS(linkIndex))?TRUE:FALSE;
	U8 LDATA connectionState;
	//Save current volumes (voice, mic, ringtone) to nvram, then save to EEPROM
	MMI_LinkKey_SaveHistoryWhenDisconnect(linkIndex);

	MMI_HCI_ScoDiscHandle(linkIndex);
	if(isSCOExist)
		MMI_HCI_ScoDiscProfileHandle(linkIndex);


	#ifdef AIR_MODULE
	MMI_AIR_AclDiscHandle(linkIndex);
	#endif
	//// NOTE!!!!!! this should be the last on coze it will delete the L2capIndex information in HC. After this, HC_ConnectionHandleToL2CAPIndex(connHandle) can NOT be called
	L2CAP_AclDiscHandle(linkIndex);
	OSMQ_FlushAndReleaseMemoryBlock(OSMQ_Protocol_Event_To_MMI_ptr[linkIndex]);
	MMI_ClearProfileReconnectListQueue(linkIndex);
	#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
	MMI_ClearSequentialActiveProfileList(linkIndex);
	#endif
	//DemountAllProfile
	MMI_SDAP_ClearCtlData(linkIndex);

	//ACL is disconnected. rest the whole system memory
	MMI_PowerSaving(TRUE, 0xFF, linkIndex); // all clear
	MMI_DEL_LINK(linkIndex);
	MMI_TIMER_ReleaseConnectionTimer(linkIndex);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].waitDetachTimer);

	gMMI_ctl.mmiInfo[linkIndex].subState = MMI_INIT_SYS_CONFIGED;
	MMI_DRV_RemoveServiceLink(linkIndex);

	if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
	{
		MMI_ReleaseProfileLinkAndDetach(linkIndex);
	}
	else
	{
		MMI_CleanNestState(linkIndex);
	}
	
	connectionState = MMI_CONN_STATE_NONE;
	MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_ACL_LINK_STATUS, &connectionState, linkIndex);
	MMI_ClearMMILinkSettingType(linkIndex);	
}

PRIVATE void MMI_HCI_RemoveSCOAudioLink(U8 linkIndex)
{
	DRIVER_ClearEnableSCODSPCmd(linkIndex);
	#ifdef HFP_Profile
	if(MMI_HFP_IsHSPConnected(linkIndex))
	{
		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_SCO_HSP);
	}
	else
	#endif
	{
		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_SCO);
		MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_SCO_IDLE);
	}
#if defined HFP_Profile && defined MMI_SUPPORT_MULTI_HFP_FEAT
	if(MMI_HFP_SearchCallStatusInd(linkIndex, MMI_CALL_ACTIVE) >= MMI_MAX_ONGOING_CALL_NO || MMI_EOF == MMI_GetNextNormalLink(linkIndex))
	{
		MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO_HSP);
		MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO);
		MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO_IDLE);
	}
#else
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO_HSP);
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO);
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO_IDLE);
#endif
#ifdef SUPPORT_MIC_IN_A2DP_MODE
	if(MMI_GetAudioTransparencyEnable())
	{
		MMI_LineIn_SendDriverVolCmd();
	}
#endif
}
PRIVATE void MMI_HCI_AddSCOAudioLink(U8 linkIndex)
{
	U8 nextLink = MMI_GetNextNormalLink(linkIndex);

#ifdef HFP_Profile
	U8 audioDev = AUDIO_DEVICE_SCO_HSP;
	if(!MMI_HFP_IsHSPConnected(linkIndex))
	{
		if(MMI_CheckLinkInIncomingState(linkIndex) && (nextLink == MMI_EOF || !MMI_CheckLinkInIncomingState(nextLink))
			|| MMI_CheckLinkInOutGoingState(linkIndex) || MMI_CheckLinkInCallActiveState(linkIndex))
		{
			if(MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_SCO))
			{
				if(SCO_EXISTS(linkIndex))
					MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
			}
		}
		audioDev = AUDIO_DEVICE_SCO_IDLE;
	}
	if(MMI_DRV_AudioDeviceRegister(linkIndex, audioDev))
	{
		if(SCO_EXISTS(linkIndex))
			MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
	}
#else
	if(MMI_CheckLinkInIncomingState(linkIndex) && (nextLink == MMI_EOF || !MMI_CheckLinkInIncomingState(nextLink))
		|| MMI_CheckLinkInOutGoingState(linkIndex) || MMI_CheckLinkInCallActiveState(linkIndex))
	{
		if(MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_SCO))
		{
			if(SCO_EXISTS(linkIndex))
				MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
		}
	}
	if(MMI_DRV_AudioDeviceRegister(linkIndex, AUDIO_DEVICE_SCO_IDLE))
	{
		if(SCO_EXISTS(linkIndex))
			MMI_SendDSPCmd(linkIndex, DSP_ENABLE_BY_SCO);
	}
#endif
}

/* Link Control Command */
PUBLIC void MMI_HCI_SendInquiry(U8 inquiryType)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 CODE_PTR LapPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_INQUIRY)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_INQ_STRU); //5;
		if (inquiryType == INQUIRY_GIAC)
		{
			LapPtr = (U8 CODE_PTR)&((GAP_NVRAM_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_GAP_NVRAM_INIT)))->GIAC_Lap[0];
		}
		else
		{
			LapPtr = (U8 CODE_PTR)&((GAP_NVRAM_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_GAP_NVRAM_INIT)))->LIAC_Lap[0];
			#ifdef AIR_MODULE
			if(gMMI_Air_ctl.isSecurePairing)
				LapPtr = (U8 CODE_PTR)&((GAP_NVRAM_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_GAP_NVRAM_INIT)))->LIAC_Lap_Special[0]; //Limited LAP
			#endif
		}
		
		OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.inquiryCmd.Lap[0], LapPtr, sizeof(LAP_ARR));
		msgPtr->msgBodyPtr.hciCmd.inquiryCmd.InquiryLength = 8; //8 * 1.28 = 10.24 Second
		msgPtr->msgBodyPtr.hciCmd.inquiryCmd.NumResponses = 0; // num_responses
		MMI_HCI_SendCmd(msgPtr);

		MMI_HCI_SetInquiryOnGoing(INQUIRY_BUSY_NDK);
	}
}

PUBLIC void MMI_HCI_SendInquiryCancel(void)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if(!MMI_HCI_IsInquiryOnGoing())
		return;
	
	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_INQUIRY_CANCEL)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = 0;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendCreateConnection(U8 linkIndex, U8 XDATA_PTR bd_addr_ptr, U8 type, U8 XDATA_PTR codPtr)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	#ifdef AIR_MODULE
	if(MMI_AIR_SendCreateConnection(linkIndex, bd_addr_ptr, type, codPtr))
		return;
	#endif

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_CREATE_CONNECTION)) != (MMIMsgType XDATA_PTR)NULL)
	{
		LightDbgPrint("==CC :%x,%x,%x",(U8)bd_addr_ptr[0], (U8)bd_addr_ptr[1], (U8)bd_addr_ptr[2]);
		MMI_PageHandler(TRUE);

		msgPtr->paraLen = sizeof(HCI_CMD_PARA_CREATE_CONN_STRU); //13;
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.createConnCmd.BdAddr, bd_addr_ptr, 6);
		msgPtr->msgBodyPtr.hciCmd.createConnCmd.PacketTypeB0 = 8; // set packet type to DM1
		msgPtr->msgBodyPtr.hciCmd.createConnCmd.PacketTypeB1 = 0;// set packet type to DM1
		msgPtr->msgBodyPtr.hciCmd.createConnCmd.PageScanRepetMode = MMI_CMD_PARA_PAGE_SCAN_MODE_R1; //page_scan_mode;
		msgPtr->msgBodyPtr.hciCmd.createConnCmd.Reserved = 0;
		msgPtr->msgBodyPtr.hciCmd.createConnCmd.ClkOffsetB0 = 0x00; //(U8)clock_offset;
		msgPtr->msgBodyPtr.hciCmd.createConnCmd.ClkOffsetB1 = 0x00; //(U8)((clock_offset&0xFF00)>>8);
		if (MMI_ROLE_SWITCH_TO_MASTER_DURING_ACCEPT_CONN_FEAT)
		{
			msgPtr->msgBodyPtr.hciCmd.createConnCmd.AllowRoleSwitch = MMI_CMD_PARA_NOT_ALLOW_ROLE_SWITCH; 
		}
		else
		{
			msgPtr->msgBodyPtr.hciCmd.createConnCmd.AllowRoleSwitch = MMI_CMD_PARA_ALLOW_ROLE_SWITCH; 
		}
		MMI_HCI_SendCmd(msgPtr);
		
		gMMI_ctl.reconnectCtl.type = type;
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].bdAddr, bd_addr_ptr, 6);
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].linkPara.classOfDev, codPtr, 3);
		MMI_SetConnectionState(linkIndex, MMI_CONN_STATE_WAITING_ACTIVE);
		
		MMI_HCI_UpdateRole(linkIndex, ROLE_MASTER);
		
	}
}

PUBLIC void MMI_HCI_SendRelSco(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;
	#ifdef DBG_UART_ENABLE_MMI
	ASSERT(SCO_EXISTS(linkIndex));
	#endif
	
	#ifdef AIR_MODULE
	MMI_AIR_SendRelSco(linkIndex);
	#endif

	if (!SCO_EXISTS(linkIndex) || gMMI_ctl.mmiInfo[linkIndex].linkPara.isSCODisconnecting)
		return;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_DISCONNECT)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_DISCONNECT_STRU); //3;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.disconnectCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle);
		msgPtr->msgBodyPtr.hciCmd.disconnectCmd.Reason = ERR_REMOTE_USER_TERMINATED_CONNECTION;
		
		gMMI_ctl.mmiInfo[linkIndex].linkPara.isSCODisconnecting = TRUE;
		MMI_HCI_SendCmd(msgPtr);
		#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI - [%d]RelSco %D",(U8)linkIndex, (U32)gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle);
		#endif
		SYS_InsertOSTask(HC_TASK);
		SYS_InsertOSTask(LM_TASK);
	}
}

PUBLIC void MMI_HCI_SendRelAcl(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	//Unsniff req shall be sent before sending detach, or remote side
	//might not get detach because sniff intervel is longer than detach
	//timeout timer in LM.
	MMI_HCI_SendExitSniffMode(linkIndex);

	//Save current volumes (voice, mic, ringtone) to nvram, then save to EEPROM
	MMI_LinkKey_SaveHistoryWhenDisconnect(linkIndex);

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_DISCONNECT)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_DISCONNECT_STRU); //3;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.disconnectCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		msgPtr->msgBodyPtr.hciCmd.disconnectCmd.Reason = ERR_REMOTE_USER_TERMINATED_CONNECTION;
		MMI_HCI_SendCmd(msgPtr);
		
		gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask |= MMI_LINK_DETACHING;
		gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask |= MMI_DISC_ACTIVE;
		#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI - [%d]RelAcl%D", (U8)linkIndex, (U32)gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		#endif
	}
}

PUBLIC void MMI_HCI_SendCreateConnectionCancel(U8 XDATA_PTR bd_addr_ptr)
{
	MMIMsgType XDATA_PTR msgPtr;
	if((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_CREATE_CONNECTION_CANCEL)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_CREATE_CONN_CANCEL_STRU); //6;
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.createConnCancelCmd.BdAddr, bd_addr_ptr, 6);
		MMI_HCI_SendCmd(msgPtr);

		SYS_InsertOSTask(HC_TASK);
		SYS_InsertOSTask(LM_TASK);
	}
}

PUBLIC void MMI_HCI_SendChangeConnectionPacketType (U8 linkIndex, U8 pakType)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(pakType == LINK_TYPE_eSCO_CONNECTION_REQUEST)
	{
		MMI_HCI_SendSetupESCO(linkIndex, gMMI_nvram.changePktTypePara.Esco);
	}
	else
	{
		if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_CHANGE_CONNECTION_PACKET_TYPE)) != (MMIMsgType XDATA_PTR)NULL)
		{
			msgPtr->paraLen = sizeof(HCI_CMD_PARA_CHANGE_CONN_PKT_TYPE_STRU); //4;
			if(pakType == LINK_TYPE_ACL_CONNECTION_REQUEST)
			{
				MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.changeConnPktTypeCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
				MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.changeConnPktTypeCmd.PacketTypeB0, gMMI_nvram.changePktTypePara.Acl);
			}
			else
			{
				MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.changeConnPktTypeCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle);
				MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.changeConnPktTypeCmd.PacketTypeB0, gMMI_nvram.changePktTypePara.Sco);
			}
			MMI_HCI_SendCmd(msgPtr);
		}
		else
		{
			ASSERT_PRINT("MEM ALLOC ERR");
		}
	}
}

PUBLIC void MMI_HCI_SendAuthenticationRequested(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	if (msgPtr = MMI_GetCommandMessageMem(HCI_CMD_AUTHENTICATION_REQUESTED))
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_AUTHENTICATION_REQUESTED_STRU); //2;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.authenRequestedCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		MMI_HCI_SendCmd(msgPtr);
		gMMI_ctl.mmiInfo[linkIndex].linkPara.isAuthenticated = AUTH_ONGOING;
	}
}

PUBLIC void MMI_HCI_SendSetConnectionEncryption(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	if (msgPtr = MMI_GetCommandMessageMem(HCI_CMD_SET_CONNECTION_ENCRYPTION))
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_SET_CONN_ENCRYPTION_STRU); //3;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.setConnEncryptionCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		msgPtr->msgBodyPtr.hciCmd.setConnEncryptionCmd.EncryptionEnable = 0x01;

		MMI_HCI_SendCmd(msgPtr);
		gMMI_ctl.mmiInfo[linkIndex].linkPara.isEncrypted = ENCRYPTED_ONGOING;
	}
}

PUBLIC void MMI_HCI_SendReadRemoteName(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(!MMI_IsActiveLink(linkIndex))
		return;

	if (msgPtr = MMI_GetCommandMessageMem(HCI_CMD_REMOTE_NAME_REQUEST))
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_REMOTE_NAME_REQ_STRU); //10;
		OSMEM_memcpy_xdata_xdata (	(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.remoteNameReqCmd.BdAddr,
									(U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].bdAddr,
									6);
		msgPtr->msgBodyPtr.hciCmd.remoteNameReqCmd.PageScanRepetMode = MMI_CMD_PARA_PAGE_SCAN_MODE_R1; //page_scan_mode; the same as MMI_HCI_SendCreateConnection
		msgPtr->msgBodyPtr.hciCmd.remoteNameReqCmd.Reserved = 0;
		msgPtr->msgBodyPtr.hciCmd.remoteNameReqCmd.ClkOffset[0] = 0;
		msgPtr->msgBodyPtr.hciCmd.remoteNameReqCmd.ClkOffset[1] = 0;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendSetupESCO(U8 linkIndex, U16 scoPktType)
{
	MMIMsgType XDATA_PTR msgPtr;
	U32 TxBandwidth;
	U32 RxBandwidth;
	U16 MaxLatency;
	U16 VoiceSetting;
	U8 ReTxEffort;

	if(msgPtr = MMI_GetCommandMessageMem(HCI_CMD_SETUP_SYNCHRONOUS_CONNECTION))
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_SETUP_SCO_CONN_STRU); //17;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.setupSCOConnCmd.ConnHdlB0, (SCO_EXISTS(linkIndex))?
																							gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle:
																							gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);

		VoiceSetting = 3;
		ReTxEffort = 0x01;
		
		if(scoPktType & HCIPKTYPE_AIROHA_PKT_6SLOTS)
		{
			scoPktType &= ~HCIPKTYPE_AIROHA_PKT_6SLOTS;
			//Tesco = 6 slots, Length = 60 bytes : Tx/Rx Bandwidth = 16k , MaxLatency = 6ms, retransmission effort = 0x01, 8Khz data rate, latency ~15ms (HW SRC)
			TxBandwidth	= 16000L;
			RxBandwidth	= 16000L;
			MaxLatency	= 6;
		}
		else if(scoPktType & HCIPKTYPE_AIROHA_PKT_16SLOTS)
		{
			scoPktType &= ~HCIPKTYPE_AIROHA_PKT_16SLOTS;
			//Tesco = 16 slots, Length = 40 bytes : Tx/Rx Bandwidth = 4.0k , MaxLatency = 12ms, retransmission effort = 0x01, 2Khz data rate, latency ~43ms (SW Down-sampling)
			TxBandwidth	= 4000L;
			RxBandwidth	= 4000L;
			MaxLatency	= 12;
		}
        else if(scoPktType & HCIPKTYPE_AIROHA_PKT_20SLOTS)
		{
			scoPktType &= ~HCIPKTYPE_AIROHA_PKT_20SLOTS;
			//Tesco = 20 slots, Length = 52 bytes : Tx/Rx Bandwidth = 4.16k , MaxLatency = 15ms, retransmission effort = 0x01, 2Khz data rate, latency ~33.25ms (SW Down-sampling)           
            TxBandwidth	= 4160L;
			RxBandwidth	= 4160L;
			MaxLatency	= 15;
		}
		else if(scoPktType & HCIPKTYPE_AIROHA_PKT_24SLOTS)
		{
			scoPktType &= ~HCIPKTYPE_AIROHA_PKT_24SLOTS;
			//Tesco = 24 slots, Length = 60 bytes : Tx/Rx Bandwidth = 4k , MaxLatency = 17ms, retransmission effort = 0x01, 2Khz data rate, latency ~58ms (SW Down-sampling)
			TxBandwidth	= 4000L;
			RxBandwidth	= 4000L;
			MaxLatency	= 17;
		}
		else if(scoPktType & HCIPKTYPE_AIROHA_PKT_2EV5_FOR_CELT)
		{
			//Tesco = 8 slots, Length = 336 bytes : Tx/Rx Bandwidth = 33.6k/12k , MaxLatency = 16ms, retransmission effort = 0x00, Celt encode data ,
			scoPktType = 0x0280;//SOUND_BAR Celt
			TxBandwidth	= 67200L;//67200L; 
			RxBandwidth	= 12000L;//12000L;
			MaxLatency	= 7;//16 12 7 
            ReTxEffort = 0;//0x01:retransmit  0:No reretransmit
		}        
		else
		{
			TxBandwidth	= gMMI_nvram.scoPktPara.TxBandwidth;
			RxBandwidth	= gMMI_nvram.scoPktPara.RxBandwidth;
			MaxLatency	= gMMI_nvram.scoPktPara.MaxLatency;
			ReTxEffort	= gMMI_nvram.scoPktPara.ReTxEffort;
			if (gMMI_ctl.mmiInfo[linkIndex].linkPara.scoCodecType == SCO_CODEC_CVSD)
			{
				VoiceSetting = gMMI_nvram.scoPktPara.VoiceSetting;
			}
		}
		
		MMI_HCI_SetU32((U32 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.setupSCOConnCmd.TxBandwidth, TxBandwidth);
		MMI_HCI_SetU32((U32 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.setupSCOConnCmd.RxBandwidth, RxBandwidth);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.setupSCOConnCmd.MaxLatencyB0, MaxLatency);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.setupSCOConnCmd.VoiceSettingB0, VoiceSetting);
		msgPtr->msgBodyPtr.hciCmd.setupSCOConnCmd.ReTxEffort = ReTxEffort;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.setupSCOConnCmd.PacketTypeB0, scoPktType);
		MMI_HCI_SendCmd(msgPtr);
		
		gMMI_ctl.mmiInfo[linkIndex].linkPara.scoIsActiveEstablished = TRUE;
	}
}

PUBLIC void MMI_HCI_SendConfirmationReply(BOOL isAccept, U8 XDATA_PTR bdAddrPt)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem((isAccept)?HCI_CMD_USR_CONFIRMATION_REQ_REPLY: HCI_CMD_USR_CONFIRMATION_REQ_NEGATIVE_REPLY)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_USER_CONFIRM_REQ_NEG_REPLY_STRU); //6;
		OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.userConfirmNegReplyCmd.BdAddr, (U8 XDATA_PTR)bdAddrPt, sizeof(BD_ADDR));

		MMI_HCI_SendCmd(msgPtr);
	}
}

/* Link Policy Command */
#if 0 //for BQB
PUBLIC void MMI_HCI_SendHoldMode(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_HOLD_MODE)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_HOLD_MODE_STRU); //6;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.holdModeCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.holdModeCmd.HoldModeMaxIntervalB0, 0x0F16/*gMMI_nvram.sniffPara.Sniff_Max_Interval*/); //800 slots equals to 0.5 sec
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.holdModeCmd.HoldModeMinIntervalB0, 0x0F16/*gMMI_nvram.sniffPara.Sniff_Min_Interval*/); //800
		MMI_HCI_SendCmd(msgPtr);
	}
}
#endif

PUBLIC void MMI_HCI_SendSniffMode(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	#ifdef AIR_MODULE //temporary modification
	if (linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_SCO_WOOFER|PRODUCT_AIR_A2DP_WOOFER))
		return;
	#endif

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_SNIFF_MODE)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_SNIFF_MODE_STRU); //10;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffModeCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffModeCmd.SniffMaxIntervalB0, gMMI_nvram.sniffPara.Sniff_Max_Interval); //800 slots equals to 0.5 sec
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffModeCmd.SniffMinIntervalB0, gMMI_nvram.sniffPara.Sniff_Min_Interval); //800
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffModeCmd.SniffAttemptB0, gMMI_nvram.sniffPara.Sniff_Attempt); //4
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffModeCmd.SniffTimeOutB0, gMMI_nvram.sniffPara.Sniff_Timeout); //1
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendExitSniffMode(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(MMI_IsActiveLink(linkIndex))
	{
		#ifdef AIR_MODULE  //temporary modification
		if (!gSYS_update_by_uart_flash_command && linkIndex == MMI_AIR_GetAirLink(PRODUCT_AIR_SOUNDBAR|PRODUCT_AIR_SCO_WOOFER|PRODUCT_AIR_A2DP_WOOFER))
			return;
		#endif

		if (gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_SNIFF || gMMI_ctl.mmiInfo[linkIndex].linkPara.mode == MODE_SNIFFING)
		{
			if (msgPtr = MMI_GetCommandMessageMem(HCI_CMD_EXIT_SNIFF_MODE))
			{
				msgPtr->paraLen = sizeof(HCI_CMD_PARA_EXIT_SNIFF_MODE_STRU); //2;
				MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.exitSniffModeCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
				MMI_HCI_SendCmd(msgPtr);
				
				SYS_InsertOSTask(HC_TASK);
				SYS_InsertOSTask(LM_TASK);

				gMMI_ctl.mmiInfo[linkIndex].linkPara.mode = MODE_ACTIVATING;
				gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffTryCnt = 0;
				LightDbgPrint("USR - [%d]SniffDisable:%X",(U8)linkIndex, (U32)gMMI_ctl.mmiInfo[linkIndex].linkPara.sniffProfileMask);

				//cancel sniff subrating
				SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].checkRemoteAGAcceptEnterSniffTimer);
			}
		}
	}
}

PUBLIC void MMI_HCI_SendSwitchRole(U8 linkIndex, U8 role)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(!MMI_IsActiveLink(linkIndex) || SCO_EXISTS(linkIndex) /* HCI spec*/|| MMI_IsLinkActiveDisconnecting(linkIndex))
		return;
	if((U8)gMMI_ctl.mmiInfo[linkIndex].linkPara.role == role)
		return;
		
	msgPtr = MMI_GetCommandMessageMem( HCI_CMD_SWITCH_ROLE);
	if(msgPtr)
	{
		MMI_SniffDisable(linkIndex, MMI_SNIFF_ROLE_SWITCH);
		
		OSMEM_memcpy_xdata_xdata (	(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.switchRoleCmd.BdAddr,
									(U8 XDATA_PTR)gMMI_ctl.mmiInfo[linkIndex].bdAddr,
									6);
		msgPtr->msgBodyPtr.hciCmd.switchRoleCmd.Role = (role == ROLE_MASTER)?HCI_CMD_SWITCH_ROLE_IS_MASTER:HCI_CMD_SWITCH_ROLE_IS_SLAVE;
		MMI_HCI_SendCmd(msgPtr);

		#ifdef DBG_UART_ENABLE_MMI
		LightDbgPrint("MMI - RoleSwitch:%d",(U8)linkIndex);
		#endif
		
		if( role == ROLE_MASTER)
		{
			gMMI_ctl.mmiInfo[linkIndex].linkPara.roleSwitchCnt++;
		}
	}
}

PUBLIC void MMI_HCI_SendSniffSubrating(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_SNIFF_SUBRATING)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_SNIFF_SUBRATING_STRU); //8;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffSubratingCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffSubratingCmd.MaxLatencyB0, gMMI_nvram.sniffPara.Maximum_Latency);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffSubratingCmd.MinRemoteTimeOutB0, gMMI_nvram.sniffPara.Minimum_Remote_Timeout);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.sniffSubratingCmd.MinLocalTimeOutB0, gMMI_nvram.sniffPara.Minimum_Local_Timeout);
		MMI_HCI_SendCmd(msgPtr);
	}
}

/* Controller & Baseband Command */
PUBLIC void MMI_HCI_SendWritePageTimeOut(U16 timeout)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_WRITE_PAGE_TIMEOUT)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_WR_PAGE_TO_STRU); //2;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.writePageTOCmd.PageToB0, timeout);
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendWritePageScanActivity(void)  //change pageScanWindow
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_WRITE_PAGE_SCAN_ACTIVITY)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_WR_PAGE_SCAN_ACTIVITY_STRU); //4;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.writePageScanActCmd.PageScanIntervalB0, gHC_parameter.page_scan_interval);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.writePageScanActCmd.PageScanWindowB0, (gMMI_ctl.scoCnt)?gMMI_nvram.pageScanWindowSize.largeSize:
																														gMMI_nvram.pageScanWindowSize.smallSize);
		MMI_HCI_SendCmd(msgPtr);
	}
}


PUBLIC void MMI_HCI_SendWriteLinkSuperVisionTimeout(U8 linkIndex,U16 timeout)
{
	MMIMsgType XDATA_PTR msgPtr;
	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_WRITE_LINK_SUPERVISION_TIMEOUT)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_WR_LINK_SUPERVISION_TO_STRU);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.writeLinkSpervisionTOCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.writeLinkSpervisionTOCmd.LinkSupervisionToB0, timeout);
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendWriteInquiryMode(U8 inquiryMode)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_WRITE_INQUIRY_MODE)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_WR_INQ_MODE_STRU); //4;
		msgPtr->msgBodyPtr.hciCmd.writeInquiryModeCmd.InqMode = inquiryMode;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendRefreshEncryptionKey(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_REFRESH_ENCRYPTION_KEY)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_REFRESH_ENC_KEY_STRU); //2;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.refreshEncrypKeyCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendReadRSSI(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr = (MMIMsgType XDATA_PTR)MMI_GetCommandMessageMem(HCI_CMD_READ_RSSI);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_RD_RSSI_STRU); //2;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciCmd.readRSSICmd.HandleB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendWriteSimplePairingDebugMode(U8 isON)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(msgPtr = MMI_GetCommandMessageMem(HCI_CMD_WRITE_SIMPLE_PAIRING_DEBUG_MODE))
	{
		msgPtr->paraLen = sizeof(HCI_CMD_PARA_WR_SIMPLE_PAIRING_DBG_MODE_STRU); //1;
		msgPtr->msgBodyPtr.hciCmd.writeSSPDebugModeCmd.SimplePairingDbgMode = (isON)?0x01:0x00;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendEnableDeviceUnderTestMode(void)
{
	MMIMsgType XDATA_PTR msgPtr;
	if ((msgPtr = MMI_GetCommandMessageMem(HCI_CMD_ENABLE_DEVICE_UNDER_TEST_MODE)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->paraLen = 0;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC BOOL MMI_HCI_SendVCmdAudioSwitch(U8 linkIndex)
{
	MMIMsgType XDATA_PTR msgPtr;

	if (SCO_EXISTS(linkIndex))
	{
		if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
		{
			msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_AUDIO_SWITCH;
			msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_AUDIO;
			MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.audioSwitchCmd.ScoHandleB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle);
			msgPtr->msgBodyPtr.hciVCmd.VPara.audioSwitchCmd.IsMute = 0;//must set zero

			MMI_HCI_SendCmd(msgPtr);
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void MMI_HCI_SendVCmdAudioEnhanceSCOLatency(BOOL isEnable)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_ENHANCE_ESCO_LATENCY;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_AUDIO;
		msgPtr->msgBodyPtr.hciVCmd.VPara.enhanceESCOLatencyCmd.ScoHandleB0 = 0;
		msgPtr->msgBodyPtr.hciVCmd.VPara.enhanceESCOLatencyCmd.ScoHandleB1 = 0;
		msgPtr->msgBodyPtr.hciVCmd.VPara.enhanceESCOLatencyCmd.IsEnable = isEnable;//must set zero

		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdAudioSCONullPacket(U8 linkIndex, BOOL isPktNull)
{
	MMIMsgType XDATA_PTR msgPtr;

	if (SCO_EXISTS(linkIndex))
	{
		if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
		{
			msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_ESCO_NULL_PACKET;
			msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_AUDIO;
			MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.scoNullPktCmd.ScoHandleB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.scoConnHandle);
			msgPtr->msgBodyPtr.hciVCmd.VPara.scoNullPktCmd.IsNullPkt = isPktNull;//must set zero

			MMI_HCI_SendCmd(msgPtr);
		}
	}
}

PUBLIC void MMI_HCI_SendVCmdAudioQosSetup(U8 linkIndex, U8 serviceType, U8 flowStatus, U8 rxFull)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(!MMI_IsActiveLink(linkIndex) || MMI_IsLinkActiveDisconnecting(linkIndex))
		return;

	#ifdef AIR_MODULE
	if(MMI_AIR_SendVCmdAudioQosSetup(linkIndex, serviceType, flowStatus, rxFull))
		return;
	#endif

	//if(gMMI_ctl.mmiInfo[linkIndex].linkPara.QosServiceType == serviceType && !flowStatus)
	//	return;

	if (msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_PARA_AUDIO_SETUP_QOS) + sizeof(HCI_VCMD_OPCODE_STRU);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_SETUP_QOS;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_AUDIO;
		MMI_HCI_SetU16((U16 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ConnHdlB0, gMMI_ctl.mmiInfo[linkIndex].linkPara.aclConnHandle);
		msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.Flag = 0;
		msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.ServiceType = serviceType;
		MMI_HCI_SetU32((U32 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.TokenRate, 0xFFFFFFFFL);
		MMI_HCI_SetU32((U32 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.PeakBandwidth, 0xFFFFFFFFL);
		MMI_HCI_SetU32((U32 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.Latency, 10000L);//10ms
		MMI_HCI_SetU32((U32 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.DelayVariation, 0L);
		msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.FlowStatus = flowStatus;
		msgPtr->msgBodyPtr.hciVCmd.VPara.setupQosCmd.RxFull = rxFull;
		
		gMMI_ctl.mmiInfo[linkIndex].linkPara.QosServiceType = serviceType;
		gMMI_ctl.mmiInfo[linkIndex].linkPara.QosFlowStatus = flowStatus;
		gMMI_ctl.mmiInfo[linkIndex].linkPara.RxFull = rxFull;
		LightDbgPrint("compose airoha qos cmd:%d",(U8)rxFull);
		LightDbgPrint("acl %d active %d,%d,%d", (U8)linkIndex, (U8)MMI_IsActiveLink(linkIndex),(U8)serviceType,(U8)flowStatus);
		
		MMI_HCI_SendCmd(msgPtr);
	}
}

PRIVATE void MMI_HCI_SendVCmdWriteSFR(U8 Bank, U8 Offset, U8 Value)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_CONFIG_WRITE_SFR;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VENDOR_OGF_LABTEST;
		msgPtr->msgBodyPtr.hciVCmd.VPara.writeSFRCmd.Value = Value;
		msgPtr->msgBodyPtr.hciVCmd.VPara.writeSFRCmd.Offset = Offset;
		msgPtr->msgBodyPtr.hciVCmd.VPara.writeSFRCmd.Bank = Bank;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdWriteLocalBdAddr(U8 XDATA_PTR bdPtr)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		LightDbgPrint("==Local :%x,%x,%x",(U8)bdPtr[0], (U8)bdPtr[1], (U8)bdPtr[2]);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_GENERAL_V2;
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.writeLocalBdAddrCmd, bdPtr, sizeof(HCI_VCMD_PARA_GNL_WRITE_BD_ADDR));
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdLabTestStart(void)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_START;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_LABTEST;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdLabTestUpdateUapLap(U8 XDATA_PTR sourcePtr)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_CONFIG_UAP_AND_LAP;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_LABTEST;
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.configUAPLAPCmd, sourcePtr, sizeof(HCI_VCMD_PARA_LABTEST_CFG_UAP_LAP));
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdLabTestPause(void)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_PAUSE;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_LABTEST;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdLabTestSwitchMCUTo12M(void)
{
	MMI_HCI_SendVCmdWriteSFR(0xC2, 0xC9, 0x00);
}

PUBLIC void MMI_HCI_SendVCmdLabTestLTAddrInTxPkt(void)
{
	MMI_HCI_SendVCmdWriteSFR(0xC2, 0x2C, 0x07);
}

PUBLIC void MMI_HCI_SendVCmdLabTestReportRateSetup(void)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_REPORT_RATE_SETUP;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_LABTEST;
		msgPtr->msgBodyPtr.hciVCmd.VPara.reportRateSetupCmd.RptRateB0 = 0xE8;
		msgPtr->msgBodyPtr.hciVCmd.VPara.reportRateSetupCmd.RptRateB1 = 0x03;
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdLabTestSingleToneSetup(U8 channel, U16 gc, U16 bias)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_SINGLE_TONE_SETUP;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_LABTEST;
		msgPtr->msgBodyPtr.hciVCmd.VPara.singleToneSetupCmd.Channel = channel;
		msgPtr->msgBodyPtr.hciVCmd.VPara.singleToneSetupCmd.Gc2 = (U8)gc;
		msgPtr->msgBodyPtr.hciVCmd.VPara.singleToneSetupCmd.Gc1 = (U8)(gc >> 8);
		msgPtr->msgBodyPtr.hciVCmd.VPara.singleToneSetupCmd.Bias2_2 = (U8)bias;
		msgPtr->msgBodyPtr.hciVCmd.VPara.singleToneSetupCmd.Bias2_1 = (U8)(bias >>8);
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdLabTestTxModemManualOff(void)
{
	MMI_HCI_SendVCmdWriteSFR(0xE4, 0xE7, 0x00);
}

PUBLIC void MMI_HCI_SendVCmdLabTestContinueTxData(HCI_VCMD_PARA_LABTEST_CTX_DATA XDATA_PTR dataPtr)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_CTX_DATA;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_LABTEST;
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.cTxDataCmd, dataPtr, sizeof(HCI_VCMD_PARA_LABTEST_CTX_DATA));
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdLabTestBurstTxPacket(HCI_VCMD_PARA_LABTEST_BTX_PACKET XDATA_PTR dataPtr)
{
	MMIMsgType XDATA_PTR msgPtr;

	msgPtr = (MMIMsgType XDATA_PTR)OSMEM_Get(OSMEM_ptr2);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM2_BLKSIZE);

		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->msgOpcode =  HCI_VCMD_AIROHA;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_BTX_PACKET;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_LABTEST;
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.bTxPktCmd, dataPtr, sizeof(HCI_VCMD_PARA_LABTEST_BTX_PACKET));
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdLabTestContinueRxStart(U8 channel, U8 HighSide, U8 ReptRssi)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = HCI_VCMD_OCF_LABTEST_CRX_START;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_LABTEST;
		msgPtr->msgBodyPtr.hciVCmd.VPara.cRxStartCmd.Channel = channel;
		msgPtr->msgBodyPtr.hciVCmd.VPara.cRxStartCmd.HighSide = HighSide;
		msgPtr->msgBodyPtr.hciVCmd.VPara.cRxStartCmd.ReptRssi = ReptRssi;
		MMI_HCI_SendCmd(msgPtr);
	}
}

#ifdef TRSPX_App
PUBLIC void MMI_HCI_SendTrspXUARTEvent(MMIMsgType XDATA_PTR msgPtr, U8 cmdOpcode)
{
	msgPtr->dataOffset = CMD_OFFSET;
	msgPtr->msgOpcode =  HCI_VCMD_AIROHA_TO_UART;
    msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = cmdOpcode;
    msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_SPP;
	
	MMI_HCI_SendCmd(msgPtr);
}
#endif

#ifdef SPP_Profile
PUBLIC void MMI_HCI_SendSPPEventToUART(U8 linkIndex, U8 cmdOpcode)
{
	MMIMsgType XDATA_PTR uartMsgPtr;

	if(MMI_SPP_DATA_TO_UART_FEAT)
	{
		//redirect msgPtr, bypass OSMEM_Put()
		if(uartMsgPtr = (MMIMsgType XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
		{
			uartMsgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode = cmdOpcode + (HCI_VEVT_OCF_SPP_CONNECTED - SPP_TO_MMI_OPCODE_CONNECTED);
			uartMsgPtr->msgBodyPtr.hciUARTSppData.ogf = HCI_VENDOR_OGF_UART_SPP;

			OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&uartMsgPtr->msgBodyPtr.hciUARTSppData.bdAddr, (U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, sizeof(BD_ADDR));
			MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&uartMsgPtr->msgBodyPtr.hciUARTSppData.bdAddr);

			uartMsgPtr->dataOffset = CMD_OFFSET;
			uartMsgPtr->msgOpcode = HCI_VCMD_AIROHA_TO_UART;
			uartMsgPtr->paraLen = (sizeof(HciUARTSppDataType)-1);
			MMI_HCI_SendCmd(uartMsgPtr);
		}
	}
}

PUBLIC void MMI_HCI_SendSPPDataToUART(U8 linkIndex, U8 XDATA_PTR msgPtr, U8 cmdOpcode)
{
	U16 dataLen;
	MMIMsgType XDATA_PTR uartMsgPtr;

	#define SPP_UART_DATA_OFFSET	(OS_OFFSET_OF(MMIMsgType, msgBodyPtr) + (sizeof(HciUARTSppDataType)-1))
	#define SPP_RFCOMM_DATA_OFFSET	(OS_OFFSET_OF(ProfileDataEvent, dataBody))

	if(MMI_SPP_DATA_TO_UART_FEAT)
	{
		dataLen = ((ProfileDataEvent XDATA_PTR)msgPtr)->dataLength;
		uartMsgPtr = (MMIMsgType XDATA_PTR)msgPtr;

		ASSERT(SPP_UART_DATA_OFFSET == SPP_RFCOMM_DATA_OFFSET);

		uartMsgPtr->msgBodyPtr.hciUARTSppData.cmdOpCode = cmdOpcode + (HCI_VEVT_OCF_SPP_RX_DATA_FROM_REMOTE - SPP_TO_MMI_OPCODE_DATA);
		uartMsgPtr->msgBodyPtr.hciUARTSppData.ogf = HCI_VENDOR_OGF_UART_SPP;

		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&uartMsgPtr->msgBodyPtr.hciUARTSppData.bdAddr, (U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, sizeof(BD_ADDR));
		MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&uartMsgPtr->msgBodyPtr.hciUARTSppData.bdAddr);

		uartMsgPtr->dataOffset = CMD_OFFSET;
		uartMsgPtr->msgOpcode = HCI_VCMD_AIROHA_TO_UART;
		uartMsgPtr->paraLen = dataLen + (sizeof(HciUARTSppDataType)-1);

		MMI_HCI_SendCmd(uartMsgPtr);
		
		return; //redirect msgPtr, bypass OSMEM_Put()			
	}
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}
#endif


PUBLIC void MMI_HCI_AirohaCmd_SendToRemote(MMIMsgType XDATA_PTR evtMsgPtr)
{
	OSMQ_Put (OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)evtMsgPtr);
}

PUBLIC void MMI_HCI_AirohaCmd_SendResponse(U8 cmdOpcode, U8 resultCode)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + sizeof(HCI_VCMD_PARA_AIR_CMD_RESP);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = cmdOpcode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_RESP;
		msgPtr->msgBodyPtr.hciVCmd.VPara.respVEvtCmd.resultCode = resultCode;
		MMI_HCI_AirohaCmd_SendToRemote(msgPtr);
	}
}

PUBLIC void MMI_HCI_AirohaCmd_ExtSendResponse(U8 cmdOpcode, U16 extendOCF, U8 resultCode)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(msgPtr = MMI_GetCommandMessageMem(HCI_VCMD_AIROHA_TO_UART))
	{
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) +sizeof(HCI_VCMD_PARA_EXT_UART_PARA);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = cmdOpcode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_EXTENDED_TWO_BYTE_OCF_RESP;
		msgPtr->msgBodyPtr.hciVCmd.VPara.extendedPara.OCF = extendOCF;
		msgPtr->msgBodyPtr.hciVCmd.VPara.extendedPara.result = resultCode;

		MMI_HCI_AirohaCmd_SendToRemote(msgPtr);
	}
}

PUBLIC MMIMsgType XDATA_PTR MMI_HCI_AirohaCmd_IssueSolicitedInd(U8 opCode, U16 vParaLength)
{
	MMIMsgType XDATA_PTR msgPtr;
	
	if(msgPtr = (MMIMsgType XDATA_PTR)SYS_MemoryGet(vParaLength + sizeof(HCI_VCMD_OPCODE_STRU) + 5))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, vParaLength + sizeof(HCI_VCMD_OPCODE_STRU) + 5);

		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->msgOpcode =  HCI_VCMD_AIROHA_TO_UART;
		msgPtr->paraLen = sizeof(HCI_VCMD_OPCODE_STRU) + vParaLength;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_IND_SOLICITED;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = opCode;
	}
	return msgPtr;
}



PUBLIC void MMI_HCI_UartSendUnSolicitedIndication(U8 opCode, U8 XDATA_PTR dataPtr, U8 linkIndex)
{
	U8 i;
	BOOL withBDAddr;
	U8 XDATA_PTR tempPtr;
	MMIMsgType XDATA_PTR msgPtr;
	#ifdef MMI_INQUIRY_CMD
	U16 length;
	#endif
	
	#ifdef AIR_MODULE
	MMI_AIR_UartSendUnSolicitedIndication(opCode, dataPtr, linkIndex);
	#endif

	if (!MMI_CC_UART_ENABLE)
		return;

	if(msgPtr = MMI_GetCommandMessageMem( HCI_VCMD_AIROHA_TO_UART ))
	{
		withBDAddr = FALSE;

		msgPtr->paraLen = 2; // sizeof(opCode + ogf)
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = opCode;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = HCI_VENDOR_OGF_UART_IND_UNSOLICITED;
		tempPtr = (U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara;

		switch(opCode)
		{
			case MMI_UART_IND_STATE_CHANGE:
				if( linkIndex != MMI_EOF)
				{
					tempPtr[sizeof(BD_ADDR)] = MMI_GetTopStateByLink(linkIndex);
					msgPtr->paraLen += 1;
					withBDAddr = TRUE;
				}
				else
				{
					*tempPtr = MMI_GetTopStateByLink(DEFAULT_LINK_IND);
					msgPtr->paraLen += 1;
				}
				break;

			case MMI_UART_IND_CONFIRMATION_REQUEST:
				withBDAddr = TRUE;
				break;

			case MMI_UART_IND_INSERT_STATE:
			case MMI_UART_IND_REPLACE_STATE:
				OSMEM_memcpy_xdata_xdata (tempPtr+sizeof(BD_ADDR), dataPtr, 2);
				msgPtr->paraLen += 2;
				withBDAddr = TRUE;
				break;

			case MMI_UART_IND_REMOVE_STATE:
				if(dataPtr[1] == MMI_EOF)
				{
					tempPtr[0] = dataPtr[0];
					msgPtr->paraLen += 1;
				}
				else
				{
					tempPtr[sizeof(BD_ADDR)] = dataPtr[0];
					msgPtr->paraLen += 1;
					withBDAddr = TRUE;
				}
				break;

			case MMI_UART_IND_MEDIA_EVENT_GENERATED:
				#if 1
				i = (U8)dataPtr;
				MMI_ReadNvramDataChunk(tempPtr, &gMMI_nvram.evtParaCtl[i], sizeof(EvtPara));
				#else
				OSMEM_memcpy_xdata_xdata (tempPtr, dataPtr, sizeof(EvtPara));
				#endif
				msgPtr->paraLen += sizeof(EvtPara);
				break;

			case MMI_UART_IND_PLUS_VGS:
			case MMI_UART_IND_PLUS_VGM:
			case MMI_UART_IND_SET_ABSOLUTE_VOLUME:
				tempPtr[sizeof(BD_ADDR)] = dataPtr[0];
				msgPtr->paraLen += 1;
				withBDAddr = TRUE;
				break;

			case MMI_UART_IND_DEVICE_TYPE:
				switch(gMMI_ctl.sdapCtl[linkIndex].remoteApplicationID)
				{
					case PRODUCT_AIR_TWS:
						tempPtr[sizeof(BD_ADDR)] = MMI_UART_DEVICE_TYPE_AIROSHARE;
						break;

					case PRODUCT_AIR_SOUNDBAR:
						tempPtr[sizeof(BD_ADDR)] = MMI_UART_DEVICE_TYPE_AIROSOUNDBAR;
						break;
					case PRODUCT_AIR_A2DP_WOOFER:
					case PRODUCT_AIR_SCO_WOOFER:
						tempPtr[sizeof(BD_ADDR)] = MMI_UART_DEVICE_TYPE_AIROWOOFER;
						break;
					case PRODUCT_AIR_NONE:
						tempPtr[sizeof(BD_ADDR)] = MMI_UART_DEVICE_TYPE_PHONE;
						break;
				}
				msgPtr->paraLen += 1;
				withBDAddr = TRUE;
				break;
				
			case MMI_UART_IND_PROFILE_CONNECTED:
			case MMI_UART_IND_PROFILE_DISCONNECTED:
			case MMI_UART_IND_ACL_LINK_STATUS:
			case MMI_UART_IND_AVRCP_UP_DOWN_PASS_THROUGH:
				tempPtr[sizeof(BD_ADDR)] = *dataPtr;
				msgPtr->paraLen += 1;
				withBDAddr = TRUE;
				break;
				
			#ifdef MMI_INQUIRY_CMD
			case MMI_UART_IND_INQUIRY_RESULT_WITH_RSSI:
			case MMI_UART_IND_EXTENDED_INQUIRY_RESULT:
				length = OSMEM1_BLKSIZE - (msgPtr->dataOffset + 3 + msgPtr->paraLen);
				OSMEM_memcpy_xdata_xdata(tempPtr, dataPtr, length);
				msgPtr->paraLen += length;
				break;
			#endif

			default:
				OSMEM_Put((U8 XDATA_PTR)msgPtr);
				return;
		}

		if(withBDAddr)
		{
			//put current BDAddr ahead of the data
			OSMEM_memcpy_xdata_xdata(tempPtr, &gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], sizeof(BD_ADDR));
			MMI_SwitchEndianBDAddr(tempPtr);
			msgPtr->paraLen += sizeof(BD_ADDR);
		}
		MMI_HCI_SendCmd(msgPtr);
	}
}

PUBLIC void MMI_HCI_SendVCmdAvrcpEventToUART(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr, U8 evtCode)
{
	msgPtr->dataOffset = CMD_OFFSET;
	msgPtr->msgOpcode =  HCI_VCMD_AIROHA_TO_UART;

	msgPtr->msgBodyPtr.hciUARTAvrcpEvent.cmdOpCode = evtCode;
	msgPtr->msgBodyPtr.hciUARTAvrcpEvent.ogf = HCI_VENDOR_OGF_UART_AVRCP;

	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTAvrcpEvent.bdAddr, (U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr, sizeof(BD_ADDR));
	MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciUARTAvrcpEvent.bdAddr);

	MMI_HCI_SendCmd(msgPtr);
}


PUBLIC void MMI_GAP_WriteSecurityMode(void)
{
	MMIMsgType XDATA_PTR msgPtr;
	if ((msgPtr = MMI_GetCommandMessageMem(GAP_CMD_FROM_PROFILE_WRITE_SECURITY_MODE)) != (MMIMsgType XDATA_PTR)NULL)
	{
		//Support Security Mode 4
		msgPtr->msgBodyPtr.gapPara = SECURITY_MODE_4;
		OSMQ_Put (OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void MMI_GAP_WritePairingMode(void)
{
	MMIMsgType XDATA_PTR msgPtr;

	if ((msgPtr = MMI_GetCommandMessageMem(GAP_CMD_FROM_PROFILE_WRITE_PAIRING_MODE)) != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.gapPairingModeCmd.pairingMode= PAIRING_MODE;
		OSMQ_Put (OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void MMI_GAP_WriteConnectabilityMode(U8 connectabilityMode)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(gMMI_ctl.connectableState == connectabilityMode)
		return;

	msgPtr = MMI_GetCommandMessageMem(GAP_CMD_FROM_PROFILE_WRITE_CONNECTABILITY_MODE);
	msgPtr->msgBodyPtr.gapPara = connectabilityMode;

	if(MMI_DRV_IsNoConnectivityPwOnLowBat())
	{
		msgPtr->msgBodyPtr.gapPara = (SAC_NON_CONNECTABLE_MODE | NON_CONNECTABLE_MODE);
	}

	OSMQ_Put (OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);

	gMMI_ctl.connectableState = msgPtr->msgBodyPtr.gapPara;

	if(gMMI_ctl.connectableState == (SAC_NON_CONNECTABLE_MODE | NON_CONNECTABLE_MODE))
	{
		MMI_MMI1Handle(TRUE); //CONNECTABLE_SET_RF_OFF
	}
}

PUBLIC void MMI_GAP_WriteDiscoverabilityMode(U8 discoverabilityMode)
{
	MMIMsgType XDATA_PTR msgPtr;

	if(gMMI_ctl.discoverableState == discoverabilityMode)
	{
		return;
	}

	msgPtr = MMI_GetCommandMessageMem(GAP_CMD_FROM_PROFILE_WRITE_DISCOVERABILITY_MODE);
	msgPtr->msgBodyPtr.gapPara = discoverabilityMode;

	if(MMI_DRV_IsNoConnectivityPwOnLowBat())
	{
		msgPtr->msgBodyPtr.gapPara = (SAC_NON_DISCOVERABLE_MODE| NON_DISCOVERABLE_MODE);
	}
	
	gMMI_ctl.discoverableState = msgPtr->msgBodyPtr.gapPara;
	
	#ifdef AIR_MODULE
	MMI_AIR_WriteDiscoverabilityMode(msgPtr->msgBodyPtr.gapPara);
	#endif
	
	OSMQ_Put (OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);
	
}

PUBLIC BOOL MMI_HCI_IsQosReady(U8 linkIndex)
{
	if(!MMI_HCI_IsCmdInBusy(HCI_VCMD_AIROHA, linkIndex, HCI_VENDOR_OGF_AUDIO, HCI_VCMD_OCF_SETUP_QOS) && gMMI_ctl.mmiInfo[linkIndex].linkPara.QosServiceType == SERVICE_TYPE_GUARANTEED)
		return TRUE;

	return FALSE;
}

PUBLIC BOOL MMI_HCI_IsWriteLocalBdAddrBusy(void)
{
	if(MMI_HCI_IsCmdInBusy(HCI_VCMD_AIROHA, MMI_EOF, HCI_VENDOR_OGF_GENERAL_V2, HCI_VCMD_OCF_GENERAL_WRITE_BD_ADDR))
		return TRUE;

	return FALSE;
}

PRIVATE void MMI_HCI_AirohaCmd_ReconnectToIndex(MMIMsgType XDATA_PTR msgPtr)
{
	if(MMI_Reconnect_ToUARTIndex(msgPtr->msgBodyPtr.hciVEvt.VPara.reconnectPara.reconnectIndex))
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_RECONNECT_TO_INDEX, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
	}
	else
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_RECONNECT_TO_INDEX, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_ReadBdAddr(MMIMsgType XDATA_PTR msgPtr)
{
	MMIMsgType XDATA_PTR evtMsgPtr;

	UNUSED(msgPtr);

	if(evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(UART_CMD_READ_BD_ADDR, sizeof(HCI_VCMD_PARA_READ_BDADDR)))
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_READ_BD_ADDR, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
		
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&evtMsgPtr->msgBodyPtr.hciVCmd.VPara.readBdAddrVEvtCmd.BdAddr, (U8 XDATA_PTR)&gHC_ctl.BdAddr, sizeof(BD_ADDR_STRU));
		MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&evtMsgPtr->msgBodyPtr.hciVCmd.VPara.readBdAddrVEvtCmd.BdAddr);		
		
		MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);
	}
	else
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_READ_BD_ADDR, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_WriteDeviceName(MMIMsgType XDATA_PTR msgPtr)
{
	if(msgPtr->paraLen > sizeof(HCI_VCMD_OPCODE_STRU))
	{
		msgPtr->paraLen -= sizeof(HCI_VCMD_OPCODE_STRU); //substract ofc and cmdCode
		
		if(MMI_SECTOR_WriteDeviceName(&msgPtr->msgBodyPtr.hciVEvt.VPara.writeDevNamePara.nameBegin, msgPtr->paraLen))
		{
			MMI_HCI_AirohaCmd_SendResponse(UART_CMD_WRITE_DEVICE_NAME, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			return;
		}
	}
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_WRITE_DEVICE_NAME, HCI_VEVT_OCF_RESULT_CODE_FAIL);
}

PRIVATE void MMI_HCI_AirohaCmd_QueryLinkHistory(MMIMsgType XDATA_PTR msgPtr)
{
	MMIMsgType XDATA_PTR evtMsgPtr;
	U8 validLinkKeyCnt = 0;
	U8 i;
	U8 XDATA_PTR dataPtr;
	
	switch(msgPtr->msgBodyPtr.hciVEvt.VPara.queryLinkKeyPara.type)
	{
		case MMI_RESET_LINK_INFO:
			for(i = 0; i < gMMI_nvram.storedLinkKeyCnt; i++)
			{
				if(gMMI_driver_variation_nvram.link_history_info[i].key_status == NVRAM_KEY_OK)
				{
					validLinkKeyCnt++;
				}
			}		
			break;
		case MMI_RESET_AIR_LINK_INFO:
			for(i = 0; i < NVRAM_AIR_LINK_KEY_NO; i++)
			{
				if(gMMI_driver_variation_nvram.air_link_history_info[i].key_status == NVRAM_KEY_OK)
				{
					validLinkKeyCnt++;
				}
			}		
			break;
		case MMI_RESET_FAST_DONGLE_LINK_INFO:
			if(gMMI_driver_variation_nvram.fast_dongle_link_history_info.key_status == NVRAM_KEY_OK)
			{
				validLinkKeyCnt = 1;
			}
			break;
	}
	
	if( evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(UART_CMD_QUERY_LINK_HISTORY, (U8)(validLinkKeyCnt * (1 + sizeof(BD_ADDR)))))
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_QUERY_LINK_HISTORY, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
		
		dataPtr = (U8 XDATA_PTR)&evtMsgPtr->msgBodyPtr.hciVCmd.VPara;
		switch(msgPtr->msgBodyPtr.hciVEvt.VPara.queryLinkKeyPara.type)
		{
			case MMI_RESET_LINK_INFO:
				for(i = 0; i < gMMI_nvram.storedLinkKeyCnt; i++)
				{
					if(gMMI_driver_variation_nvram.link_history_info[i].key_status == NVRAM_KEY_OK)
					{
						dataPtr[0] = i;
						OSMEM_memcpy_xdata_xdata(&dataPtr[1], (U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[i].linkData.bdAddr[0], sizeof(BD_ADDR));
						MMI_SwitchEndianBDAddr(&dataPtr[1]);
						dataPtr += (1+sizeof(BD_ADDR));
					}
				}
				break;
			case MMI_RESET_AIR_LINK_INFO:
				for(i = 0; i < NVRAM_AIR_LINK_KEY_NO; i++)
				{
					if(gMMI_driver_variation_nvram.air_link_history_info[i].key_status == NVRAM_KEY_OK)
					{
						dataPtr[0] = i;
						OSMEM_memcpy_xdata_xdata(&dataPtr[1], (U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[i].linkData.bdAddr[0], sizeof(BD_ADDR));
						MMI_SwitchEndianBDAddr(&dataPtr[1]);
						dataPtr += (1+sizeof(BD_ADDR));
					}
				}
				break;
			case MMI_RESET_FAST_DONGLE_LINK_INFO:
				if(gMMI_driver_variation_nvram.fast_dongle_link_history_info.key_status == NVRAM_KEY_OK)
				{
					dataPtr[0] = 0;
					OSMEM_memcpy_xdata_xdata(&dataPtr[1], (U8 XDATA_PTR)&gMMI_driver_variation_nvram.fast_dongle_link_history_info.linkData.bdAddr[0], sizeof(BD_ADDR));
					MMI_SwitchEndianBDAddr(&dataPtr[1]);
					dataPtr += (1+sizeof(BD_ADDR));
				}
				break;
		}
		MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);
	}
	else
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_QUERY_LINK_HISTORY, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_Connectable(MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	MMI_ConnectabilityHandle(CMD_SET_CONNECTABLE);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_CONNECTABLE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_AirohaCmd_NoneConnectable(MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_NONE_CONNECTABLE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_AirohaCmd_DisconnectLinkByBdAddr(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;

	MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.BdAddr);

	if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.BdAddr)) == MAX_MULTI_POINT_NO)
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_DISCONNECT_SPECIFIC_LINK, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
	else
	{
		MMI_ReleaseProfileLinkAndDetach(linkIndex);
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_DISCONNECT_SPECIFIC_LINK, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_CheckChargerStatus(MMIMsgType XDATA_PTR msgPtr)
{
	MMIMsgType XDATA_PTR evtMsgPtr;

	UNUSED(msgPtr);
	if( evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(UART_CMD_CHECK_CHARGER_STATUS, sizeof(HCI_VCMD_PARA_CHECK_CHARGER)))
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_CHECK_CHARGER_STATUS, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
		evtMsgPtr->msgBodyPtr.hciVCmd.VPara.checkChargerVEvtCmd.chargerStatus = gMMI_ctl.mmiBatInfo.batStatus;
		MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);
	}
	else
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_CHECK_CHARGER_STATUS, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_StopVoicePrompt(MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	MMI_DRV_SendVoicepromptStopCmd();
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_STOP_VOICEPROMPT, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_AirohaCmd_PlayVoicePrompt(MMIMsgType XDATA_PTR msgPtr)
{
	MMI_DRV_SendVoicePromptCmd(MEDIA_EVT_UART_CMD_RESERVE, msgPtr->msgBodyPtr.hciVEvt.VPara.playVPPara.vpIndex);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_PLAY_VOICE_PROMPT, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_AirohaCmd_QueryServiceIndex(MMIMsgType XDATA_PTR msgPtr)
{
	MMIMsgType XDATA_PTR evtMsgPtr;

	UNUSED(msgPtr);
	if( evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(UART_CMD_QUERY_SERVICE_INDEX, sizeof(HCI_VCMD_PARA_QUERY_SERVICE_IND)))
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_QUERY_SERVICE_INDEX, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
		evtMsgPtr->msgBodyPtr.hciVCmd.VPara.queryServiceIndVEvtCmd.serviceIndex = gMMI_ctl.serviceInd;
		MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);
	}
	else
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_QUERY_SERVICE_INDEX, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_VolumeSet(MMIMsgType XDATA_PTR msgPtr)
{
	MMI_UartVolumeSet(&msgPtr->msgBodyPtr.hciVEvt.VPara.volSetPara);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_VOLUME_SET, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_AirohaCmd_PairingConfirm(MMIMsgType XDATA_PTR msgPtr)
{
	MMI_HCI_SendConfirmationReply(msgPtr->msgBodyPtr.hciVEvt.VPara.pairingConfPara.isAccept, (U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.pairingConfPara.BdAddr);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_PAIRING_CONFIRM, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_AirohaCmd_ChangeAudioSel(MMIMsgType XDATA_PTR msgPtr)
{
	MMI_DRV_AudioSelCmd((DriverAudioSelCmd XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.dataBegin);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_CHANGE_AUDIO_SEL, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_AirohaCmd_SetAVRCPVolume(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef AVRCP_Profile
	MMI_AVRCP_UartSetAbsVol(msgPtr->msgBodyPtr.hciVEvt.VPara.avrcpVolPara.vol);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_SET_AVRCP_VOL, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
#else
	UNUSED(msgPtr);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_SET_AVRCP_VOL, HCI_VEVT_OCF_RESULT_CODE_FAIL);
#endif
}

PRIVATE void MMI_HCI_AirohaCmd_HighQualityCodec(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef A2DP_Profile
	MMI_A2DP_HighQualityCodecHandle(msgPtr->msgBodyPtr.hciVEvt.VPara.hqCodecPara.settings);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_HIGH_QUALITY_AUDIO_CODEC, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
#else
	UNUSED(msgPtr);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_HIGH_QUALITY_AUDIO_CODEC, HCI_VEVT_OCF_RESULT_CODE_FAIL);
#endif
}

PRIVATE void MMI_HCI_AirohaCmd_QueryLinkKeyByBdAddr(MMIMsgType XDATA_PTR msgPtr)
{
	MMIMsgType XDATA_PTR evtMsgPtr;
	LinkHistoryType XDATA_PTR linkListPtr;
	
	MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.BdAddr);
	if(linkListPtr = MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.BdAddr, TRUE, FALSE))
	{
		if(evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(UART_CMD_QUERY_LINKKEY_BY_BDADDR, 16))
		{
			MMI_HCI_AirohaCmd_SendResponse(UART_CMD_QUERY_LINKKEY_BY_BDADDR, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
			
			OSMEM_memcpy_xdata_xdata(&evtMsgPtr->msgBodyPtr.hciVCmd.VPara.queryLinkKeyVEvtCmd.linkKey, (U8 XDATA_PTR)&linkListPtr->linkData.linkKey[0], 16);
			MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);
			return;
		}
	}
	
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_QUERY_LINKKEY_BY_BDADDR, HCI_VEVT_OCF_RESULT_CODE_FAIL);
}

PRIVATE void MMI_HCI_AirohaCmd_I2CRead(MMIMsgType XDATA_PTR msgPtr)
{
	MMIMsgType XDATA_PTR evtMsgPtr;
	
	if(evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(UART_CMD_I2C_READ, msgPtr->msgBodyPtr.hciVEvt.VPara.i2cReadPara.dataLength))
	{		
		if(I2C_StdReadChunk(msgPtr->msgBodyPtr.hciVEvt.VPara.i2cReadPara.deviceAddr, 
							msgPtr->msgBodyPtr.hciVEvt.VPara.i2cReadPara.regAddr,
							msgPtr->msgBodyPtr.hciVEvt.VPara.i2cReadPara.dataLength, 
							&evtMsgPtr->msgBodyPtr.hciVCmd.VPara.i2cReadVEvtCmd.i2cDataBegin))
		{
			OSMEM_Put(evtMsgPtr);
			goto I2C_READ_FAIL;
		}
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_I2C_READ, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
		MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);
	}
	else
	{
		I2C_READ_FAIL:
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_I2C_READ, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_I2CWrite(MMIMsgType XDATA_PTR msgPtr)
{
	if(I2C_StdWriteChunk(msgPtr->msgBodyPtr.hciVEvt.VPara.i2cWritePara.deviceAddr, 
						msgPtr->msgBodyPtr.hciVEvt.VPara.i2cWritePara.regAddr, 
						msgPtr->msgBodyPtr.hciVEvt.VPara.i2cWritePara.dataLength, 
						&msgPtr->msgBodyPtr.hciVEvt.VPara.i2cWritePara.dataBegin))
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_I2C_WRITE, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
	else
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_I2C_WRITE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_GetRemoteName(MMIMsgType XDATA_PTR msgPtr)
{
    MMIMsgType XDATA_PTR evtMsgPtr;
	U8 linkIndex;

	MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.BdAddr);
	if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.BdAddr)) == MAX_MULTI_POINT_NO)
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_GET_REMOTE_NAME, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
	else
	{
		if(!gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[0])
		{
			MMI_HCI_AirohaCmd_SendResponse(UART_CMD_GET_REMOTE_NAME, HCI_VEVT_OCF_RESULT_CODE_FAIL);
		}
		else
		{
			U8 nameLength = 0;
			while(gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[nameLength] && nameLength < MAX_REMOTE_NAME_LEN)
			{
				nameLength++;
			}
			
			if(evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(UART_CMD_GET_REMOTE_NAME, sizeof(BD_ADDR) + nameLength + 1))
			{
				MMI_HCI_AirohaCmd_SendResponse(UART_CMD_GET_REMOTE_NAME, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
				OSMEM_memcpy_xdata_xdata(&evtMsgPtr->msgBodyPtr.hciVCmd.VPara.getRemoteNemeVEvtCmd.BdAddr, &gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], sizeof(BD_ADDR));
				OSMEM_memcpy_xdata_xdata(&evtMsgPtr->msgBodyPtr.hciVCmd.VPara.getRemoteNemeVEvtCmd.nameBegin, &gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[0], nameLength);
				MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);
			}
            else
            {
                MMI_HCI_AirohaCmd_SendResponse(UART_CMD_GET_REMOTE_NAME, HCI_VEVT_OCF_RESULT_CODE_FAIL);
            }
		}
	}
}

PRIVATE void MMI_HCI_AirohaCmd_GetFWVersion(MMIMsgType XDATA_PTR msgPtr)
{
    MMIMsgType XDATA_PTR evtMsgPtr;
	SECTOR_AE_INFO_STRU CODE_PTR pAeInfo;

	pAeInfo = (SECTOR_AE_INFO_STRU CODE_PTR)GET_SECTOR_TOOLMISC_ADDR(SECTOR_AE_INFO);


    UNUSED(msgPtr);

    if(evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(UART_CMD_GET_FW_VERSION, sizeof(HCI_VCMD_PARA_GET_FW_VERSION)))
    {
        MMI_HCI_AirohaCmd_SendResponse(UART_CMD_GET_FW_VERSION, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
		
		evtMsgPtr->msgBodyPtr.hciVCmd.VPara.fwVersionVEvtCmd.majorVersion = pAeInfo->AeInfo.verInfo.majorVersion;
		evtMsgPtr->msgBodyPtr.hciVCmd.VPara.fwVersionVEvtCmd.minorVersion = pAeInfo->AeInfo.verInfo.minorVersion;
		evtMsgPtr->msgBodyPtr.hciVCmd.VPara.fwVersionVEvtCmd.buildNumber = (U8)(pAeInfo->AeInfo.verInfo.buildNumber);
        MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);
    }
    else
    {
        MMI_HCI_AirohaCmd_SendResponse(UART_CMD_GET_FW_VERSION, HCI_VEVT_OCF_RESULT_CODE_FAIL);
    }
}


PRIVATE void MMI_HCI_AirohaCmd_DSPFreqSelect(MMIMsgType XDATA_PTR msgPtr)
{
	#ifdef LINEIN_ENABLE
	MMI_LineIn_SetDSPSoundbarDownsampleSel(msgPtr->msgBodyPtr.hciVEvt.VPara.DSPSoundbarDownsampleSel);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_DSP_FREQUENCT_SELECT, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
	#else
	UNUSED(msgPtr);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_DSP_FREQUENCT_SELECT, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	#endif
}

PRIVATE void MMI_HCI_AirohaCmd_ProfileSupport(MMIMsgType XDATA_PTR msgPtr)
{
	U8 linkIndex;
	U8 profileId = msgPtr->msgBodyPtr.hciVEvt.VPara.profileSupportPara.profileId;

	MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.profileSupportPara.BdAddr);
	if((linkIndex=MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.profileSupportPara.BdAddr))==MAX_MULTI_POINT_NO || profileId != PROFILE_OBJECT_PUSH)
	{
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_SET_PROFILE_SUPPORT_STATUS, HCI_VEVT_OCF_RESULT_CODE_FAIL);
	}
	else
	{
		gMMI_ctl.mmiInfo[linkIndex].linkPara.isOppProhibited = (msgPtr->msgBodyPtr.hciVEvt.VPara.profileSupportPara.isSupported)?FALSE:TRUE;
		MMI_HCI_AirohaCmd_SendResponse(UART_CMD_SET_PROFILE_SUPPORT_STATUS, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_FindMyAccessory(MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	MMI_PushMediaEvent(MEDIA_EVT_KEY_FIND_MY_ACCESSORY);
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_FIND_MY_ACCESSORY, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void MMI_HCI_AirohaCmd_Inquiry(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef MMI_INQUIRY_CMD
	U8 status;
	if (MMI_INQ_StartInquiry(msgPtr))
	{
		status = HCI_VEVT_OCF_RESULT_CODE_FAIL;
	}
	else
	{
		status = HCI_VEVT_OCF_RESULT_CODE_SUCCESS;
	}
	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_INQUIRY, status);
#else
	UNUSED(msgPtr);
#endif
}

PRIVATE void MMI_HCI_AirohaCmd_CreateConnection(MMIMsgType XDATA_PTR msgPtr)
{
#ifdef MMI_CREATE_CONNECTION_CMD
	U8 linkIndex;
	U8 XDATA_PTR ptr1 = (U8 XDATA_PTR)msgPtr->msgBodyPtr.hciVCmd.VPara.createConnCmd.bd_addr.member;
	
	MMI_SwitchEndianBDAddr(ptr1);
	if((linkIndex = MMI_GetEmptyLinkIndex(ptr1, TRUE)) < MAX_MULTI_POINT_NO)
	{
		MMI_HCI_SendCreateConnection (	linkIndex,
										ptr1, 
										RECONNECT_USER_INIT, 
										ptr1);//cod is dont care
	}

	MMI_HCI_AirohaCmd_SendResponse(UART_CMD_CREATE_CONNECTION, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
#else
	UNUSED(msgPtr);
#endif
}

PRIVATE void MMI_HCI_AirohaCmd_UnSupported(MMIMsgType XDATA_PTR msgPtr)
{
	MMI_HCI_AirohaCmd_SendResponse(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf, HCI_VEVT_OCF_RESULT_CODE_FAIL);
}

PRIVATE void MMI_HCI_AirohaCmd_KeyEvent(MMIMsgType XDATA_PTR msgPtr)
{
	U16 keyEventCode; 
	U8 linkIndex = 0;

	if(msgPtr->paraLen == 8  || msgPtr->paraLen == 9)
	{
		MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.BdAddr);
		if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.BdAddr)) == MAX_MULTI_POINT_NO)
		{
			UART_CMD_SEND_FAIL:
			MMI_HCI_AirohaCmd_SendResponse(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf, HCI_VEVT_OCF_RESULT_CODE_FAIL);
			return;
		}
	}
	else if(msgPtr->paraLen == 2)
	{
		linkIndex = gMMI_ctl.serviceInd;
	}

	switch(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf)
	{
		case KEY_DIALMEM:
			#if defined HFP_Profile
			if(MMI_HFP_SetMemoryDialIndex(linkIndex, msgPtr->msgBodyPtr.hciVEvt.VPara.dialMemPara.index))
				break;
			#endif
			goto UART_CMD_SEND_FAIL;

		case KEY_DIALNUM:
			#if defined HFP_Profile
			if(MMI_HFP_SetDialNumber(linkIndex, &msgPtr->msgBodyPtr.hciVEvt.VPara.dialNumPara.length, (U8)(msgPtr->msgBodyPtr.hciVEvt.VPara.dialNumPara.length+1)))
				break;
			#endif
			goto UART_CMD_SEND_FAIL;
		
		case KEY_SEND_DTMF:
			#if defined HFP_Profile
			if(MMI_HFP_SetDTMF(linkIndex, msgPtr->msgBodyPtr.hciVEvt.VPara.hfpDTMFIndex))
				break;
			#endif
			goto UART_CMD_SEND_FAIL;

		case KEY_RESET_PAIRED_DEVICES:
			gMMI_ctl.driverCtl.resetLinkType = msgPtr->msgBodyPtr.hciVEvt.VPara.resetLinkType;
			break;
	}

	keyEventCode = (U16)msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf;

	if(keyEventCode >= KEY_TOTAL_NO)
	{
		goto UART_CMD_SEND_FAIL;
	}

	if (((keyEventCode != KEY_BUTTONLOCKTOGGLE) && MMI_DRV_IsButtonLocked())
		 || !MMI_DRV_IsButtonLocked() && MMI_IGNORE_KEYEVENT_JUST_ENABLE_LED_FEAT)
	{
		goto UART_CMD_SEND_FAIL;
	}

	if ((keyEventCode = MMI_DRV_CheckKeyValid(keyEventCode)) == KEY_INVALID)
	{
		goto UART_CMD_SEND_FAIL;
	}
	
	if(	keyEventCode == KEY_CHECK_BATTERY_STATUS)
	{
		MMIMsgType XDATA_PTR evtMsgPtr;
		
		if(evtMsgPtr = MMI_HCI_AirohaCmd_IssueSolicitedInd(KEY_CHECK_BATTERY_STATUS, sizeof(HCI_VCMD_PARA_CHECK_BAT)))
		{
			MMI_HCI_AirohaCmd_SendResponse(KEY_CHECK_BATTERY_STATUS, HCI_VEVT_OCF_RESULT_CODE_SUCCESS );
			MMI_PushMediaEvent(KEY_CHECK_BATTERY_STATUS);
			evtMsgPtr->msgBodyPtr.hciVCmd.VPara.checkBatVEvtCmd.batStatus = MMI_BAT_GetLevelInQuater();
			MMI_HCI_AirohaCmd_SendToRemote(evtMsgPtr);			
		}
		else
		{
			goto UART_CMD_SEND_FAIL;
		}
	}
	else
	{
		MMI_HCI_AirohaCmd_SendResponse(msgPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf, HCI_VEVT_OCF_RESULT_CODE_SUCCESS );
		MMI_PushMediaEvent(keyEventCode);
		MMI_DRV_KeyEventHandler(linkIndex, keyEventCode);
	}
}

PRIVATE void MMI_HCI_AirohaCmd_SecondKeyEvent(MMIMsgType XDATA_PTR msgPtr)
{
	U16 keyEventCode; 
	U8 linkIndex = 0;

	if(msgPtr->paraLen == 10  || msgPtr->paraLen == 11)
	{
		MMI_SwitchEndianBDAddr((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.extendedPara.BdAddr);
		if((linkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVEvt.VPara.extendedPara.BdAddr)) == MAX_MULTI_POINT_NO)
		{
			UART_CMD_SEND_FAIL:
			MMI_HCI_AirohaCmd_ExtSendResponse(HCI_VENDOR_OCF_SECOND_KEY_EVENT_CMD, msgPtr->msgBodyPtr.hciVEvt.VPara.extendedPara.OCF, HCI_VEVT_OCF_RESULT_CODE_FAIL);
			return;
		}
	}
	else if(msgPtr->paraLen == 4)
	{
		linkIndex = gMMI_ctl.serviceInd;
	}

	keyEventCode = msgPtr->msgBodyPtr.hciVEvt.VPara.extendedPara.OCF;

	if(keyEventCode >= KEY_SECOND_TABLE_END)
	{
		goto UART_CMD_SEND_FAIL;
	}

	if (((keyEventCode != KEY_BUTTONLOCKTOGGLE) && MMI_DRV_IsButtonLocked())
		 || !MMI_DRV_IsButtonLocked() && MMI_IGNORE_KEYEVENT_JUST_ENABLE_LED_FEAT)
	{
		goto UART_CMD_SEND_FAIL;
	}

	if ((keyEventCode = MMI_DRV_CheckKeyValid(keyEventCode)) == KEY_INVALID)
	{
		goto UART_CMD_SEND_FAIL;
	}

	MMI_HCI_AirohaCmd_ExtSendResponse(HCI_VENDOR_OCF_SECOND_KEY_EVENT_CMD, msgPtr->msgBodyPtr.hciVEvt.VPara.extendedPara.OCF, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
	MMI_PushMediaEvent(keyEventCode);
	MMI_DRV_KeyEventHandler(linkIndex, keyEventCode);
}

PRIVATE void MMI_HCI_UpdateRole(U8 linkIndex, U8 role)
{
	gMMI_ctl.mmiInfo[linkIndex].linkPara.role = role;
}

PUBLIC void MMI_HCI_SetInquiryOnGoing(U8 inquiryState)
{
	gMMI_ctl.inquiryState = inquiryState;
}

PUBLIC BOOL MMI_HCI_IsInquiryOnGoing(void)
{
	return (gMMI_ctl.inquiryState != INQUIRY_IDLE)?TRUE:FALSE;
}

PUBLIC U8 MMI_HCI_GetInquiryState(void)
{
	return gMMI_ctl.inquiryState;
}

PRIVATE MMIMsgType XDATA_PTR MMI_GetPassThroughDataMem(U8 command_op, U8 memLen)
{
	MMIMsgType XDATA_PTR msgPtr;

	msgPtr = (MMIMsgType XDATA_PTR)SYS_MemoryGet(memLen);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, (memLen > OSMEM1_BLKSIZE) ? OSMEM2_SMALL_TX_BLKSIZE : OSMEM1_BLKSIZE);

		msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->msgOpcode =  command_op;
	}

	return msgPtr;
}

PUBLIC void MMI_HCI_AirohaCmd_PassThroughData(U8 linkIndex, U8 cmdOcf, U8 cmdOgf, MMIMsgType XDATA_PTR airCmdPtr)
{
	MMIMsgType XDATA_PTR msgPtr;
	U8 memLen = (airCmdPtr->paraLen + sizeof(msgPtr->reserveByte) + sizeof(msgPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.bdAddr) + 4);

	if(msgPtr = MMI_GetPassThroughDataMem(HCI_VCMD_AIROHA_TO_UART, memLen))
	{
		MMI_PowerSaving(FALSE, MMI_PWRSAVING_ENABLE_ADV, linkIndex);
		msgPtr->paraLen = airCmdPtr->paraLen + sizeof(msgPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.bdAddr);
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOcf = cmdOcf;
		msgPtr->msgBodyPtr.hciVCmd.VOpCode.VCmdOgf = cmdOgf;
		//Set Bd Address
		OSMEM_memcpy_xdata_xdata(	(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.bdAddr,
									MMI_GetBdAddrByLinkIndex(linkIndex),
									sizeof(msgPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.bdAddr));

		//Set AirApp Data
		OSMEM_memcpy_xdata_xdata(	(U8 XDATA_PTR)&msgPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.dataStart,
									(U8 XDATA_PTR)&airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin,
									(airCmdPtr->paraLen - sizeof(HCI_VCMD_OPCODE_STRU)));
		MMI_HCI_AirohaCmd_SendToRemote(msgPtr);
	}
}

PUBLIC void MMI_HCI_Init(void)
{
	gMMI_ctl.FlashUpdateState = FLASH_UPDATE_OFF;
}

PUBLIC U8 MMI_HCI_GetFlashUpdateState(void)
{
	return gMMI_ctl.FlashUpdateState;
}

PUBLIC void MMI_HCI_SetFlashUpdateState(U8 state)
{
	gMMI_ctl.FlashUpdateState = state;
}