//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _MMI_LE_C_

#include "MMI_LE_interface.h"
#ifdef LE_SUPPORTED_HOST_FLASH
#include "..\HC\HC_interface.h"
#include "..\GAP\GAP_InterFace.h"
#include "..\ATT\ATT_interface.h"
#include "..\MMI\MMI.h"
#include "drv_sector_flash.h"
#include "..\MMI\MMI_HCI.h"
#include "MMI_LE.h"
#include "link_index_ctl.h"
//#include "LL_interface.h"
#include "LC_interface.h"
#include "drv_prbs.h"

#ifdef SUPPORT_LE_XPORT
#include "..\MMI\MMI_TrspXport.h"
#include "..\LE_XPORT\Service_LE_xport.h"
#endif

#ifdef BLE_AIRApp
#include "..\AirApp\AirApp_Interface.h"
#endif

#ifdef APP_ANCS_C
#include "..\APP_ANCS\APP_ANCS_interface.h"
#endif

#include "..\COMMON\sys_clk_ctl.h"
#include "sector_config1.h"
#include "..\L2CAP\L2CAP_low_energy.h"
MMI_LE_EXTERN MMI_LE_NVRAM_TYPE CODE_PTR XDATA gMMI_le_nvram;
extern PUBLIC void SaveLeNvramTo4kbFlash(void);
PRIVATE U8 MMI_LE_SetAdvPara(void);
PRIVATE U8 MMI_LE_SetRandomAddr(void);
PRIVATE void MMI_LE_NotifAppLinkStatus(U8 linkIdx, U8 status);
PRIVATE void MMI_LE_HdlConnAction(U8 idx);

#ifdef BLE_AIRApp
extern PUBLIC void ChaCha20Encrypt(U8 XDATA_PTR ptr1, U8 length);
#endif

#ifdef SM_SUPPORTED
PRIVATE SM_MSG XDATA_PTR MMI_GenSMCmd(U8 opCode, U8 idx, U8 len)
{
	SM_MSG XDATA_PTR cmdPtr;

	if((cmdPtr = (SM_MSG XDATA_PTR)SYS_MemoryGet(len + sizeof(SM_MSG_HEADER))) != NEAR_NULL)
	{
    	cmdPtr->hdr.opCode = opCode;
    	cmdPtr->hdr.linkIdx = idx;
    	cmdPtr->hdr.dataLen = len;
    }
    return cmdPtr;
}

PUBLIC void MMI_LE_SendUpdateLinkStatusToSM(U8 idx, U8 status)
{
	SM_MSG XDATA_PTR cmdPtr;
    if((cmdPtr = MMI_GenSMCmd(SM_CMD_MMI_UPDATE_LINK_STATUS, idx, sizeof(SM_MMI_UPDATE_LINK_STATUS))) != NEAR_NULL)
    {
        cmdPtr->msg.updateState.linkStatus = status;
        OSMQ_Put(OSMQ_SM_Command_From_MMI_ptr, (U8 XDATA_PTR)cmdPtr);
    }
}

PUBLIC void MMI_LE_AuthenticationRequest(U8 linkIdx)
{
	SM_MSG XDATA_PTR msgPtr;

	if((msgPtr = (SM_MSG XDATA_PTR)MMI_GenSMCmd(SM_CMD_SEND_SECURITY_REQ, linkIdx, 0)) != NEAR_NULL)
	{
		OSMQ_Put (OSMQ_SM_Command_From_MMI_ptr, (U8 XDATA_PTR)msgPtr);

		#ifdef DBG_UART_ENABLE_MMI_LE
		LightDbgPrint("MMI - START_AUTHENTICATION %d", linkIdx);
		#endif
	}
}

PRIVATE void MMI_LE_CheckEvtFromSM(void)
{
	SM_MSG XDATA_PTR msgPtr;
	if((msgPtr = OSMQ_Get(OSMQ_SM_Event_To_MMI_ptr)) != NEAR_NULL)
	{
  		switch (msgPtr->hdr.opCode)
		{
            case SM_EVT_MMI_UPDATE_LINK_ENC_STATUS:
                //LightDbgPrint("Update SM State,idx:%x,state:%x", (U8)msgPtr->hdr.linkIdx, (U8)(msgPtr->msg.updateState.linkStatus));
				MMI_LE_NotifAppLinkStatus(msgPtr->hdr.linkIdx, msgPtr->msg.status);
            break;

			/*case SM_EVT_PASSKEY_INPUT_REQ:
            {
                HCI_LE_EVT_STRU XDATA_PTR evtPtr;
                gMMI_le_ctl.passkey = MMI_LE_PASSKEY_INVALID;
                if((evtPtr = (HCI_LE_EVT_STRU XDATA_PTR)OSMEM_Get(OSMEM_ptr1)) != NEAR_NULL)
            	{
                    evtPtr->offset = CMD_OFFSET;
                    evtPtr->msgCode = HCI_VCMD_AIROHA_TO_UART;
                    evtPtr->len = (sizeof(BD_ADDR) + 2);
                    evtPtr->payload.uartEvt.vocf = MMI_TRSPX_PASSKEY_REQ;
                    evtPtr->payload.uartEvt.vogf = HCI_VENDOR_OGF_UART_SPP;
                    OSMEM_memcpy_xdata_code((U8 XDATA_PTR)&evtPtr->payload.uartEvt.bdAddr.member, LIC_LE_GetPeerAddr(msgPtr->hdr.linkIdx), sizeof(BD_ADDR));
            		SYS_SwitchEndianBDAddr((U8 XDATA_PTR)&evtPtr->payload.uartEvt.bdAddr.member);
                    OSMQ_Put(OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)evtPtr);
            	}
			}
            break;*/

            case SM_EVT_VERIFY_RAND_ADDR_RSP:
            {
                //LightDbgPrint("Verify RPA,idx:%x,state:%x", (U8)msgPtr->hdr.linkIdx, (U8)(msgPtr->msg.verifyRandPara.status));
                if(msgPtr->msg.verifyRandPara.status == SM_STATUS_FAIL)
                {
                    MMI_LE_DisconnectLink();
                }
                else
                {
                    MMI_LE_HdlConnAction(msgPtr->hdr.linkIdx);
                }
            }
            break;

			default:
            break;
  		}
        OSMEM_Put((U8 XDATA_PTR)msgPtr);
	}

}
#endif

PRIVATE void MMI_LE_NotifAppLinkStatus(U8 linkIdx, U8 status)
{
    MMI_LE_APP_NODE XDATA_PTR appNode = gMMI_LE_AppNodeList;

    while(appNode)
    {
        if (appNode->appDesc->appLinkEvent)
        {
            appNode->appDesc->appLinkEvent(linkIdx, status);
        }
        appNode = appNode->nextNode;
    }
}

PUBLIC void MMI_LE_SetState(U8 state, U8 subState)
{
	gMMI_le_rom_ctl.MMI_LE_State = state;
	gMMI_le_rom_ctl.MMI_LE_SubState = subState;
}

#ifdef MMI_LE_ENABLE
PRIVATE void MMI_LE_SendUartCmdRsp(U8 vocf, U8 vogf, U8 status)
{
    HCI_LE_EVT_STRU XDATA_PTR msgPtr;
    if((msgPtr = (HCI_LE_EVT_STRU XDATA_PTR)OSMEM_Get(OSMEM_ptr1)) != NEAR_NULL)
	{
        msgPtr->offset = CMD_OFFSET;
        msgPtr->msgCode = HCI_VCMD_AIROHA_TO_UART;
        msgPtr->len = 3;
        msgPtr->payload.uartRsp.vocf = vocf;
        msgPtr->payload.uartRsp.vogf = vogf;
        msgPtr->payload.uartRsp.status = status;
        OSMQ_Put(OSMQ_HCIDRV_Command_From_GAP_ptr, (U8 XDATA_PTR)msgPtr);
	}
}
#endif

PRIVATE void MMI_LE_CheckEvtFromUART(void)
{
    HCI_LE_EVT_STRU XDATA_PTR evtPtr;
#ifdef MMI_LE_ENABLE
    U8 status;
#endif

    while((evtPtr = (HCI_LE_EVT_STRU XDATA_PTR)OSMQ_Get(OSMQ_MMI_LE_EVT_FROM_UART_ptr)) != (HCI_LE_EVT_STRU XDATA_PTR)NEAR_NULL)
    {
#ifdef MMI_LE_ENABLE
        if(evtPtr->evtCode == HCI_EVENT_UART_LE_CMD)
        {
            status = HCI_VEVT_OCF_RESULT_CODE_FAIL;
            switch(evtPtr->payload.uartCmd.vocf)
            {
                case MMI_LE_UART_CMD_UPDATE_ADV_DATA:
                {
                    if(LIC_UpdateNvramParam((U8 XDATA_PTR)&(evtPtr->payload.uartCmd.payload), LIC_FLASH_UPDATE_ADV_DATA))
                    {
						MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_SET_ADV_DATA,(U8 XDATA_PTR)&(evtPtr->payload.uartCmd.payload));
                        status = HCI_VEVT_OCF_RESULT_CODE_SUCCESS;
                    }
                    MMI_LE_SendUartCmdRsp(evtPtr->payload.uartCmd.vocf, HCI_VENDOR_OGF_UART_LE_RESP, status);
                }
                break;

                case MMI_LE_UART_CMD_CONNECTION_UPDATE:
                {
                    U8 i, idx;
					idx = 0xFF;
    			    SYS_SwitchEndianBDAddr((U8 XDATA_PTR)evtPtr->payload.uartCmd.bdAddr.member);
            		for(i = 0; i < MAX_NUM_OF_LINK; i++)
            		{
                		if(SYS_ValueCompare((U8 XDATA_PTR)evtPtr->payload.uartCmd.bdAddr.member, gMMI_LE_v_nvram_read_ptr->nvram_MMI_link_record[i].peerAddr, BD_ADDR_LEN))
                		{
                            idx = i;
                            break;
                		}
            		}
                    if(MMI_LE_SendConnUpdateReq((U8 XDATA_PTR)&(evtPtr->payload.uartCmd.payload), idx) == MMI_LE_RESULT_SUCCESS)
                    {
                        status = HCI_VEVT_OCF_RESULT_CODE_SUCCESS;
                    }
                    else
                    {
                        status = HCI_VEVT_OCF_RESULT_CODE_FAIL;
                    }
                    MMI_LE_SendUartCmdRsp(evtPtr->payload.uartCmd.vocf, HCI_VENDOR_OGF_UART_LE_RESP, status);
                }
                break;

                /*case MMI_LE_UART_CMD_ENABLE_ADV:
                {
                    if((gMMI_le_rom_ctl.MMI_LE_State == MMI_LE_ROM_SUSPEND) && (gMMI_le_rom_ctl.MMI_LE_SubState == MMI_LE_ROM_ENTER_SUSPEND))
                    {
                        MMI_LE_EnterAdvertising();
                        status = HCI_VEVT_OCF_RESULT_CODE_SUCCESS;
                    }
                    MMI_LE_SendUartCmdRsp(evtPtr->payload.uartCmd.vocf, HCI_VENDOR_OGF_UART_LE_RESP, status);
                }
                break;*/

                /*case MMI_TRSPX_PASSKEY_ENTRY_RSP:
                {
                    switch(evtPtr->payload.uartCmd.payload.pkRsp.ntfType)
                    {
                        case MMI_TRSPX_PASSKEY_DIGIT_ENTERED:
                        {
                            if((evtPtr->payload.uartCmd.payload.pkRsp.etrPasskey >= MMI_LE_PASSKEY_ASCII_0)
                                && (evtPtr->payload.uartCmd.payload.pkRsp.etrPasskey <= MMI_LE_PASSKEY_ASCII_9))
                            {
                                evtPtr->payload.uartCmd.payload.pkRsp.etrPasskey &= 0x0F;

                                if(gMMI_le_ctl.passkey == MMI_LE_PASSKEY_INVALID)
                                {
                                    gMMI_le_ctl.passkey = 0;
                                }
                                else
                                {
                                    gMMI_le_ctl.passkey *= 10;
                                }
                                gMMI_le_ctl.passkey += evtPtr->payload.uartCmd.payload.pkRsp.etrPasskey;
                                status = HCI_VEVT_OCF_RESULT_CODE_SUCCESS;
                            }
                        }
                        break;

                        case MMI_TRSPX_PASSKEY_DIGIT_ERASED:
                        {
                            if(gMMI_le_ctl.passkey != MMI_LE_PASSKEY_INVALID)
                            {
                                gMMI_le_ctl.passkey /= 10;
                                status = HCI_VEVT_OCF_RESULT_CODE_SUCCESS;
                            }
                        }
                        break;

                        case MMI_TRSPX_PASSKEY_CLEARED:
                        {
                            gMMI_le_ctl.passkey = MMI_TRSPX_PASSKEY_INVALID;
                            status = HCI_VEVT_OCF_RESULT_CODE_SUCCESS;
                        }
                        break;

                        #ifdef SM_SUPPORTED
                        case MMI_TRSPX_PASSKEY_ENTRY_COMPLETED:
                        {
                            U8 i, idx;
                        	SM_MSG XDATA_PTR cmdPtr;

            			    SYS_SwitchEndianBDAddr((U8 XDATA_PTR)evtPtr->payload.uartEvt.bdAddr.member);
							idx = LIC_INVALID_LINK_INDEX;
                    		for (i = 0; i < LIC_LE_GetNumOfActiveLink(); i++)
                    		{
                        		if(SYS_ValueCompare((U8 XDATA_PTR)evtPtr->payload.uartEvt.bdAddr.member, LIC_LE_GetPeerAddr(LIC_LE_GetActiveLinkIdx(i)), BD_ADDR_LEN))
                        		{
                                    idx = LIC_LE_GetActiveLinkIdx(i);
                                    break;
                        		}
                    		}
                            if((idx != 0xFF) && ((cmdPtr = MMI_GenSMCmd(SM_CMD_PASSKEY_INPUT_RSP, idx, sizeof(SM_PASSKEY_INPUT))) != NEAR_NULL))
                            {
                                cmdPtr->msg.keyPara.passkey[0] = BYTE2(gMMI_le_ctl.passkey);
                                cmdPtr->msg.keyPara.passkey[1] = BYTE1(gMMI_le_ctl.passkey);
                                cmdPtr->msg.keyPara.passkey[2] = BYTE0(gMMI_le_ctl.passkey);
                                cmdPtr->msg.keyPara.status = SM_STATUS_OK;

                                OSMQ_Put(OSMQ_SM_Command_From_MMI_ptr, (U8 XDATA_PTR)cmdPtr);
                                gMMI_le_ctl.passkey = MMI_LE_PASSKEY_INVALID;
                                status = HCI_VEVT_OCF_RESULT_CODE_SUCCESS;
                            }
                        }
                        break;
                        #endif

                        default:
                            break;
                    }
                    MMI_LE_SendUartCmdRsp(evtPtr->payload.uartCmd.vocf, HCI_VENDOR_OGF_UART_LE_RESP, status);
                }
                break;*/
            }
        }
#endif
        OSMEM_Put(evtPtr);
    }
}

PUBLIC void MMI_LE_Init(void)
{
    LIC_LE_Init();

#ifdef MMI_LE_ENABLE_ADV    //enable LE adv when init. if no, it will enable after power on
    if(pMpParameterF->leBDAddr.type == LE_PUBLIC_DEVICE_ADDR)
    {
        MMI_LE_SetState(MMI_LE_ROM_CONFIG, MMI_LE_ROM_ADV_PARAM_SEND_CMD);
    }
    else
    {
        MMI_LE_SetState(MMI_LE_ROM_CONFIG, MMI_LE_ROM_SET_RANDOM_ADDRESS_SEND_CMD);
    }
    MMI_PowerSaving(FALSE, MMI_PWRSAVING_ENABLE_ADV, MAX_MULTI_POINT_NO);
#else
    MMI_LE_SetState(MMI_LE_ROM_SUSPEND, MMI_LE_ROM_ENTER_SUSPEND);
#endif
    gMMI_le_ctl.isInited = 0;
#ifdef APP_ANCS_C
    APP_ANCS_Init();
#endif
}

PUBLIC void MMI_LE_RegisterApp(MMI_LE_APP_NODE XDATA_PTR appNode)
{
	appNode->nextNode = gMMI_LE_AppNodeList;
	gMMI_LE_AppNodeList = appNode;
}

#ifdef MMI_LE_TEST_NOTIFICATION
PUBLIC void MMI_LE_NotificationTest(void)
{
	U8 i;
	U16 connHdl;

	if(gMMI_le_ctl.notiDurationTimerPtr == (OST XDATA_PTR)NULL)
	{
		SYS_SetTimer (&gMMI_le_ctl.notiDurationTimerPtr, MMI_LE_NOTI_INTERVAL);
	}
	else if (SYS_IsTimerExpired(&gMMI_le_ctl.notiDurationTimerPtr))
	{
		for (i = 0; i < MAX_NUM_OF_LINK; i++)
		{
			connHdl = LIC_LE_GetConnHdl(i);
			if (connHdl != LIC_INVALID_CONNECTION_HANDLE)
			{
                #ifdef DBG_UART_ENABLE_MMI_LE
                LightDbgPrint("USR - MMI_LE noti connHdl: %x %x ",(U8)connHdl, (U8)(connHdl>>8));
                #endif
				ATT_HandleValueReportMux(0x1B, 3, connHdl);
			}
		}
		SYS_SetTimer (&gMMI_le_ctl.notiDurationTimerPtr, MMI_LE_NOTI_INTERVAL);
	}
}
#endif

PRIVATE void MMI_LE_UpdateAdvInfo(U8 subState)
{
    gMMI_le_ctl.enableAdvAfterCMD = 1;
    MMI_LE_SetState(MMI_LE_ROM_CONFIG, subState);
    MMI_LE_ConfigStateHandler();
}

PRIVATE void MMI_LE_EnableAdv(void)
{
    if(gMMI_le_rom_ctl.MMI_LE_State != MMI_LE_ROM_ADVERTISING)
    {
        MMI_LE_SetState(MMI_LE_ROM_READY, MMI_LE_ROM_WAITING_TRIGGER);
        MMI_LE_ReadyStateHandler();
    }
}

PUBLIC void MMI_LE_SetStateAfterDisc(U8 nextState)
{
    gMMI_le_ctl.nextActionAfterDisc = nextState;
}

PRIVATE void MMI_LE_CheckStateAfterDisc(void)
{
    switch (gMMI_le_ctl.nextActionAfterDisc)
    {
        case MMI_LE_ENTER_POWER_OFF_STATE:
        {
            MMI_LE_SetState(MMI_LE_ROM_POWER_OFF, MMI_LE_ROM_ENTER_POWER_OFF);
        }
        break;

        case MMI_LE_ENTER_SUSPEND_STATE:
        {
            MMI_LE_SetState(MMI_LE_ROM_SUSPEND, MMI_LE_ROM_ENTER_SUSPEND);
        }
        break;

        case MMI_LE_ENTER_ADV_STATE:
        default:
        {
            MMI_LE_EnterAdvertising();
        }
        break;
    }
    gMMI_le_ctl.nextActionAfterDisc = MMI_LE_ENTER_ADV_STATE;
}

PUBLIC void MMI_LE_EnterAdvertising(void)
{
    if(gMMI_le_ctl.isInited == 0)
    {
		LightDbgPrint("USR - LE enter ADV init");

        if(pMpParameterF->leBDAddr.type == LE_PUBLIC_DEVICE_ADDR)
        {
            MMI_LE_SetState(MMI_LE_ROM_CONFIG, MMI_LE_ROM_ADV_PARAM_SEND_CMD);
        }
        else
        {
            MMI_LE_SetState(MMI_LE_ROM_CONFIG, MMI_LE_ROM_SET_RANDOM_ADDRESS_SEND_CMD);
        }
        MMI_PowerSaving(FALSE, MMI_PWRSAVING_ENABLE_ADV, MAX_MULTI_POINT_NO);
    }
    else if(gMMI_le_ctl.rPowerAdv)
    {
        gMMI_le_ctl.rPowerAdv = 0;
        MMI_LE_UpdateAdvInfo(MMI_LE_ROM_ADV_PARAM_SEND_CMD);
    }
    else
    {
        MMI_LE_EnableAdv();
    }
}

PUBLIC void MMI_LE_DisableBleLink(void)
{
    switch (gMMI_le_rom_ctl.MMI_LE_State)
    {
        case MMI_LE_ROM_CONFIG:
        {
            MMI_LE_SetState(MMI_LE_ROM_SUSPEND, MMI_LE_ROM_ENTER_SUSPEND);
        }
        break;

        case MMI_LE_ROM_READY:
        {
        	if (gMMI_le_rom_ctl.MMI_LE_SubState == MMI_LE_READY_ENABLE_ADV_WAIT_EVT)
    		{
    			gMMI_le_ctl.disableBle = 1;
    		}
    		else
    		{
            	MMI_LE_SetState(MMI_LE_ROM_SUSPEND, MMI_LE_ROM_ENTER_SUSPEND);
        	}
        }
        break;

        case MMI_LE_ROM_ADVERTISING:
        {
            MMI_LE_DisableAdv();
        }
        break;

        case MMI_LE_ROM_CONNECTED:
        {
            MMI_LE_DisconnectLink();
	        MMI_LE_SetStateAfterDisc(MMI_LE_ENTER_SUSPEND_STATE);
        }
        break;

        default:
        break;
    }
}

PUBLIC U8 MMI_LE_DisableAdv(void)
{
    if(gMMI_le_rom_ctl.MMI_LE_State != MMI_LE_ROM_ADVERTISING)
    {
        return LE_SUCCESS;
    }

    SYS_ReleaseTimer(&gMMI_le_rom_ctl.AdvDurationTimerPtr);
	if(MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_DISABLE,NULL))
	{
        MMI_LE_SetState(MMI_LE_ROM_SUSPEND, MMI_LE_ROM_SUSPEND_DISABLE_ADV);
        return LE_SUCCESS;
	}
    return LE_FAIL;
}

PUBLIC U8 MMI_LE_SendConnUpdateReq(U8 XDATA_PTR paraPtr, U8 linkIdx)
{
    /* send connection parameter update request */
    L2CAP_CONN_PARA_REQ_TYPE XDATA_PTR ptr;

    if(paraPtr != NULL)
    {
        if(SYS_ValueCompare(paraPtr, (U8 CODE_PTR)&NVRAM_MMI_V_CONN_SETTING, sizeof(LE_CONN_SETTING_TYPE)))
        {
            return MMI_LE_RESULT_THE_SAME_DATA;
        }
        if(!LIC_UpdateNvramParam(paraPtr, LIC_FLASH_UPDATE_CONN_SETTING))
        {
            return MMI_LE_RESULT_UPDATE_NVRAM_FAILED;
        }
        if(gMMI_le_rom_ctl.MMI_LE_State != MMI_LE_ROM_CONNECTED)
        {
            return MMI_LE_RESULT_SUCCESS;
        }
    }

    if((linkIdx == 0xFF) ||
        ((ptr = (L2CAP_CONN_PARA_REQ_TYPE XDATA_PTR)OSMEM_Get(OSMEM_ptr1)) == NULL))
    {
        return MMI_LE_RESULT_NO_MEMORY;
    }
    if (paraPtr == NULL)
    {
        OS_cmemcpy((U8 XDATA_PTR)ptr, (U8 CODE_PTR)&NVRAM_MMI_V_CONN_SETTING, sizeof(L2CAP_CONN_PARA_REQ_TYPE));
        UpperLayer_SendL2capConnParaUpdateRequest(linkIdx, (U8 XDATA_PTR)ptr);
    }
    else
    {
        UpperLayer_SendL2capConnParaUpdateRequest(linkIdx, (U8 XDATA_PTR)paraPtr);
    }

    OSMEM_Put(ptr);
    return MMI_LE_RESULT_SUCCESS;
}

PUBLIC U8 MMI_LE_SendDisconnectCMD(U8 linkIndex)
{
    U8 XDATA_PTR msgPtr;
    HCI_CMD_PARA_DISCONNECT_STRU XDATA_PTR ptr;
    U16 connHdl;
    if((msgPtr = MMI_LE_GetCmdMsgMem(GAP_CMD_FROM_PROFILE_DISCONNECT)) == (U8 XDATA_PTR)NULL)
    {
        return LE_FAIL;
    }
    ptr = (HCI_CMD_PARA_DISCONNECT_STRU XDATA_PTR)(msgPtr + GAP_COMMAND_PARA0_OFFSET);
    connHdl = LIC_LE_GetConnHdl(linkIndex);
    ptr->ConnHdlB0 = U16_LSB(connHdl);
    ptr->ConnHdlB1 = U16_MSB(connHdl);
    ptr->Reason = ERR_REMOTE_USER_TERMINATED_CONNECTION;
	OSMQ_Put(OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);
    return LE_SUCCESS;
}

PUBLIC void MMI_LE_DisconnectLink(void)
{
    U8 i;

    LightDbgPrint("MMI - LE DIS %x", (U8)gMMI_le_ctl.discActive);
    if (gMMI_le_ctl.discActive)
    {
        return;
    }
    gMMI_le_ctl.discActive = TRUE;

	for (i = 0; i < LIC_LE_GetNumOfActiveLink(); i++)
	{
        MMI_LE_SendDisconnectCMD(LIC_LE_GetActiveLinkIdx(i));
        i++;
	}
}

PUBLIC BOOL MMI_LE_DisconnectPowerOff(void)
{
    if(gMMI_le_rom_ctl.MMI_LE_State != MMI_LE_ROM_CONNECTED)
    {
        return TRUE;
    }
    //gMMI_le_ctl.powerOff = 1;
    MMI_LE_SetStateAfterDisc(MMI_LE_ENTER_POWER_OFF_STATE);

    MMI_LE_DisconnectLink();
    return FALSE;
}


PUBLIC void MMI_LE(void)
{
    U8 XDATA_PTR ptr1;

	L2CAP_LE_ResourceManager();

    MMI_LE_CheckEvtFromUART();
#ifdef MMI_LE_ENABLE
    switch(gMMI_le_rom_ctl.MMI_LE_State)
    {
		case MMI_LE_ROM_CONFIG:

			MMI_LE_ConfigStateHandler();
			break;

		case MMI_LE_ROM_READY:

			MMI_LE_ReadyStateHandler();
			break;

		case MMI_LE_ROM_ADVERTISING:

			MMI_LE_AdvertisingStateHandler();
			break;

		case MMI_LE_ROM_CONNECTED:

			MMI_LE_ConnectedStateHandler();
			break;

		case MMI_LE_ROM_SUSPEND:
		case MMI_LE_ROM_POWER_OFF:
			MMI_LE_SuspendStateHandler();
			break;

		default:
			#ifdef DBG_UART_ENABLE_MMI_LE
				//LightDbgPrint("USR - MMI_LE Err State");
			#endif
			ASSERT(0);
			break;
	}

    #ifdef MMI_LE_TEST_NOTIFICATION
	MMI_LE_NotificationTest();
    #endif

    MMI_LE_SectorSaveData();

    if(gMMI_le_rom_ctl.MMI_LE_State == MMI_LE_ROM_CONNECTED)
    {
        return;
    }
#endif
    while((ptr1 = OSMQ_Get (OSMQ_GAP_Event_To_MMILE_ptr)) != (U8 XDATA_PTR)0)
    {
        OSMEM_Put(ptr1);
    }
}

PUBLIC void MMI_LE_ConfigStateHandler(void)
{
	switch(gMMI_le_rom_ctl.MMI_LE_SubState)
	{
		case MMI_LE_ROM_SET_RANDOM_ADDRESS_SEND_CMD:
			if(MMI_LE_SetRandomAddr())
			{
				gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_SET_RANDOM_ADDRESS_WAIT_EVENT;

				#ifdef DBG_UART_ENABLE_MMI_LE
				//LightDbgPrint("USR - Set random address!");
				#endif
			}
			break;

		case MMI_LE_ROM_SET_RANDOM_ADDRESS_WAIT_EVENT:
			if(MMI_LE_WaitEvent(GAP_EVENT_TO_PROFILE_SET_RANDOM_ADDRESS_COMPLETE))
			{
    			gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_ADV_PARAM_SEND_CMD;
			}
			break;

		case MMI_LE_ROM_ADV_PARAM_SEND_CMD:
			if(MMI_LE_SetAdvPara())
			{
                gMMI_le_ctl.isInited = 1;
				gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_ADV_PARAM_WAIT_EVENT;

				#ifdef DBG_UART_ENABLE_MMI_LE
				//LightDbgPrint("USR - LE ADV Para!");
				#endif
			}
			break;

		case MMI_LE_ROM_ADV_PARAM_WAIT_EVENT:
			if(MMI_LE_WaitEvent(GAP_LE_CMD_FROM_PROFILE_SET_ADV_PARAMETER))
			{
                if(gMMI_le_ctl.enableAdvAfterCMD)
                {
                    gMMI_le_ctl.enableAdvAfterCMD = 0;
                    MMI_LE_EnableAdv();
                }
                else
                {
                    gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_ADV_DATA_SEND_CMD;
                }
			}
			break;

		case MMI_LE_ROM_ADV_DATA_SEND_CMD:
			if(MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_SET_ADV_DATA,NULL))
			{
				gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_ADV_DATA_WAIT_EVENT;

				#ifdef DBG_UART_ENABLE_MMI_LE
				//LightDbgPrint("USR - LE ADV Data!");
				#endif
			}
			break;

		case MMI_LE_ROM_ADV_DATA_WAIT_EVENT:
			if(MMI_LE_WaitEvent(GAP_LE_CMD_FROM_PROFILE_SET_ADV_DATA))
			{
                    gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_SCAN_RSP_DATA_SEND_CMD;
			}
			break;

		case MMI_LE_ROM_SCAN_RSP_DATA_SEND_CMD:
			if(MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_SET_SCAN_RESP_DATA,NULL))
			{
				gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_SCAN_RSP_DATA_WAIT_EVENT;
				#ifdef DBG_UART_ENABLE_MMI_LE
				//LightDbgPrint("USR - LE Scan Data!");
				#endif
			}
			break;

		case MMI_LE_ROM_SCAN_RSP_DATA_WAIT_EVENT:
			if(MMI_LE_WaitEvent(GAP_LE_CMD_FROM_PROFILE_SET_SCAN_RESP_DATA))
			{
             	MMI_LE_SetState(MMI_LE_ROM_READY, MMI_LE_ROM_WAITING_TRIGGER);
			}
			break;

		default:
			#ifdef DBG_UART_ENABLE_MMI_LE
				//LightDbgPrint("USR - MMI_LE Err Substate");
			#endif
			ASSERT(0);
			break;
	}
}

PUBLIC void MMI_LE_ReadyStateHandler(void)
{
	switch(gMMI_le_rom_ctl.MMI_LE_SubState)
	{
        case MMI_LE_ROM_WAITING_TRIGGER:
        {
        	if(MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_ENABLE,NULL))
        	{
        		gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_READY_ENABLE_ADV_WAIT_EVT;

        		#ifdef DBG_UART_ENABLE_MMI_LE
        		LightDbgPrint("USR - LE Enable Adv");
        		#endif
        	}
        }
        break;

        case MMI_LE_READY_ENABLE_ADV_WAIT_EVT:
        {
            if(MMI_LE_WaitEvent(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_ENABLE))
            {
                MMI_LE_SetState(MMI_LE_ROM_ADVERTISING, MMI_LE_ROM_ADV_NORMAL);
                MMI_LE_AdvertisingStateHandler();
                MMI_PowerSaving(TRUE, MMI_PWRSAVING_ENABLE_ADV, MAX_MULTI_POINT_NO);
                if (gMMI_le_ctl.disableBle)
            	{
            		gMMI_le_ctl.disableBle = 0;
            		MMI_LE_DisableBleLink();
            	}
            }
        }
        break;
	}

    /*
  	U8 gpioValue;
	HC_SFR_READ_BY_FB(FBOFFS_RO_DEBOUNCE_DO_B0, gpioValue);

	if ((gpioValue & 0x80))//button Gpio7 pressed
	{
		if (gMMI_le_rom_ctl.EnableAdvTimerPtr == (OST XDATA_PTR)NULL)
		{
			SYS_SetTimer (&gMMI_le_rom_ctl.EnableAdvTimerPtr, 2*(3200L));
		}
		else if (SYS_IsTimerExpired(&gMMI_le_rom_ctl.EnableAdvTimerPtr))
		{
			#ifdef DBG_UART_ENABLE_MMI_LE
				LightDbgPrint("USR - LE Enable Adv!");
			#endif
			if(MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_ENABLE))
			{
				gMMI_le_rom_ctl.MMI_LE_State = MMI_LE_ROM_ADVERTISING;
				gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_ADV_NORMAL;
			}
		}
	}
	else if (gMMI_le_rom_ctl.EnableAdvTimerPtr != (OST XDATA_PTR)NULL)
	{
		SYS_ReleaseTimer (&gMMI_le_rom_ctl.EnableAdvTimerPtr);
	}
	*/
}

PRIVATE void MMI_LE_HdlConnAction(U8 idx)
{
    #ifdef SUPPORT_LE_XPORT
    {
        U16 connHdl;

    	connHdl = two_byte_swap(LIC_LE_GetConnHdl(idx));
        MMI_TrspX_BleConnected(LIC_LE_GetPeerAddr(idx), connHdl);
    }
    #endif

    #if defined(SM_SUPPORTED) && defined(MMI_LE_CONNCMP_SEND_SECURITY_REQ)
    MMI_LE_AuthenticationRequest(idx);
    #endif

    #ifdef MMI_LE_CONNCMP_SEND_CONNUPDATE
    {
        //send L2CAP connection parameter update request
        MMI_LE_SendConnUpdateReq(NULL, idx);
    }
    #endif

    MMI_LE_NotifAppLinkStatus(idx, MMI_LE_LINK_STATUS_CONNECTION_CMP);
}

PRIVATE void MMI_LE_CheckRPA(U8 idx, U8 pAddrType, U8 XDATA_PTR pAddr)
{
    #ifdef SM_SUPPORTED
    if((pAddrType == 0x01) && (pAddr[5] & 0x40))
    {
    	SM_MSG XDATA_PTR cmdPtr;
        if((cmdPtr = MMI_GenSMCmd(SM_CMD_VERIFY_RAND_ADDR_REQ, idx, sizeof(SM_VERIFY_RAND_ADDR))) != NEAR_NULL)
        {
            OS_memcpy(cmdPtr->msg.verifyRandPara.addr, pAddr, 6);
            OSMQ_Put(OSMQ_SM_Command_From_MMI_ptr, (U8 XDATA_PTR)cmdPtr);
        }
        else
        {
            MMI_LE_DisconnectLink();
        }
        return;
    }
    #else
        UNUSED(pAddrType);
        UNUSED(pAddr);
    #endif

    MMI_LE_HdlConnAction(idx);
}

PRIVATE void MMI_LE_HdlConnCmpEvt(HCI_LE_EVT_STRU XDATA_PTR ptr1, U8 isDirectedAdv)
{
	U16 connHdl;
	U8 idx;

	if(ptr1->payload.connCmp.status == STATUS_OK)
	{
		SYS_ReleaseTimer(&gMMI_le_rom_ctl.AdvDurationTimerPtr);
        connHdl = two_byte_swap(ptr1->payload.connCmp.connHandle);
		#ifdef DBG_UART_ENABLE_MMI_LE
			LightDbgPrint("MMI_LE cc isDirADv:%x,ch:%x,%x, ", (U8)isDirectedAdv, (U8)BYTE0(connHdl), (U8)BYTE1(connHdl));
		#endif

        #ifdef BLE_AIRApp
        AirApp_BleUpdateConnStatus(TRUE, (U8 XDATA_PTR)ptr1);
	    SYS_CLK_Set(SYS_CLK_REQ_LE, SYS_CLK_24M_REQUEST);
        #endif

		if (gMMI_le_ctl.crtLinkCnt == (MAX_NUM_OF_LINK - 1))
		{
			LIC_LE_AddLinkInfo(ptr1);
			gMMI_le_ctl.crtLinkCnt++;
            MMI_LE_SetState(MMI_LE_ROM_CONNECTED, MMI_LE_ROM_CONNECTING);
			OSMEM_Put(ptr1);
		}
		else
		{
			#ifdef MMI_LE_SINGLE_LINK
			LIC_LE_AddLinkInfo(ptr1);
			gMMI_le_ctl.crtLinkCnt++;
            MMI_LE_SetState(MMI_LE_ROM_CONNECTED, MMI_LE_ROM_CONNECTING);
			OSMEM_Put(ptr1);
			#else
			if(MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_ENABLE,NULL))
			{
				#ifdef DBG_UART_ENABLE_MMI_LE
				LightDbgPrint("USR - MMI_ENABLE ADV!");
				#endif

				LIC_LE_AddLinkInfo(ptr1);
				gMMI_le_ctl.crtLinkCnt++;
				OSMEM_Put(ptr1);
			}
			else
			{
				OSMQ_PutFront(OSMQ_GAP_Event_To_MMILE_ptr, (U8 XDATA_PTR)ptr1);
			}
			#endif
		}
		idx = LIC_LE_GetIdxByConnHdl(connHdl);

		LightDbgPrint("MMI_LE cc:%x %x %x %x", (U8)idx, (U8)isDirectedAdv, (U8)BYTE0(connHdl), (U8)BYTE1(connHdl));
        MMI_LE_SetDelayWriteFlashTimer(idx, MMI_LE_DELAY_NUM_CONN_INTERVAL * ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)ptr1->payload.connCmp.connInterval) * 4);

		L2CAP_LE_SetupSession(idx, connHdl);

        if(isDirectedAdv)
        {
            MMI_LE_CheckRPA(idx, ptr1->payload.connCmp.peerAddrType, ptr1->payload.connCmp.peerAddr);
        }
        else
        {
            MMI_LE_HdlConnAction(idx);
        }
	}
	else
	{
		//TODO: handle the failure sub event code
		#ifdef DBG_UART_ENABLE_MMI_LE
			//LightDbgPrint("USR - LE connection fail");
		#endif

		OSMEM_Put(ptr1);
		//fail to establish connection; should power off or sleep
	}
}

PRIVATE void MMI_LE_HdlDisconnCmpEvt(HCI_LE_EVT_STRU XDATA_PTR ptr1)
{
	U16 connHdl;
	U8 idx;

	if(ptr1->payload.disconnCmp.status == STATUS_OK)
	{
        connHdl = two_byte_swap(ptr1->payload.disconnCmp.connHandle);

		#ifdef DBG_UART_ENABLE_MMI_LE
		LightDbgPrint("USR - MMI_LE dcc:%d", connHdl);
		#endif

		if((idx = LIC_LE_DelLinkInfo(connHdl)) == LIC_INVALID_LINK_INDEX)
		{
			OSMEM_Put(ptr1);
			return;
		}

		#ifdef MMI_LE_SINGLE_LINK
		if(MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_ENABLE,NULL))
		{
			#ifdef DBG_UART_ENABLE_MMI_LE
			LightDbgPrint("USR - MMI_ENABLE ADV!");
			#endif

			gMMI_le_ctl.crtLinkCnt--;
			OSMEM_Put(ptr1);
		}
		else
		{
			#ifdef DBG_UART_ENABLE_MMI_LE
			//LightDbgPrint("USR - no MEM to MMI_ENABLE ADV!");
			#endif

			// need to modify
			LIC_LE_AddLinkInfo(ptr1);
			OSMQ_PutFront(OSMQ_GAP_Event_To_MMILE_ptr,ptr1);
		}
		#endif
        if (LIC_LE_GetNumOfActiveLink() == 0)
        {
            gMMI_le_ctl.discActive = FALSE;
        }
		L2CAP_LE_ClearSession(idx);
		ATT_ClearCCCD(idx);
	}
	else
	{
		#ifdef DBG_UART_ENABLE_MMI_LE
			//LightDbgPrint("USR - LE disconnection fail");
		#endif
		OSMEM_Put(ptr1);
	}
}

PUBLIC void MMI_LE_AdvertisingStateHandler(void)
{
	HCI_LE_EVT_STRU XDATA_PTR ptr1;

	switch(gMMI_le_rom_ctl.MMI_LE_SubState)
	{
		case MMI_LE_ROM_ADV_NORMAL:
		{
			if((gMMI_le_rom_ctl.AdvDurationTimerPtr == (OST XDATA_PTR)NULL) && (gMMI_LE_v_nvram_read_ptr->advTimout != 0) && (!gMMI_le_ctl.rPowerAdv))
			{
				SYS_SetTimer(&gMMI_le_rom_ctl.AdvDurationTimerPtr, (gMMI_LE_v_nvram_read_ptr->advTimout * ONE_SEC));
				#ifdef DBG_UART_ENABLE_MMI_LE
					//LightDbgPrint("USR - LE_ADV_Timer");
				#endif
			}
			else if((ptr1 = (HCI_LE_EVT_STRU XDATA_PTR)OSMQ_Get(OSMQ_GAP_Event_To_MMILE_ptr)) != (HCI_LE_EVT_STRU XDATA_PTR)NEAR_NULL)
			{
                switch(ptr1->evtCode)
                {
                    case HCI_EVENT_LE_META:
                    {
                        if(ptr1->payload.connCmp.subEvtCode == HCI_EVENT_LE_CONNECTION_COMPLETE)
                        {
                            MMI_LE_HdlConnCmpEvt(ptr1, FALSE);
                        }
                    }
                    break;

                    case HCI_EVENT_DISCONNECTION_COMPLETE:
                    {
                        MMI_LE_HdlDisconnCmpEvt(ptr1);
                    }
                    break;

                    default:
					{
						//unexpected event, should have a event handler

						//OSMQ_PutFront(OSMQ_HC_Event_To_Bus_ptr, ptr1);
						#ifdef DBG_UART_ENABLE_MMI_LE
							//LightDbgPrint("USR - MMI_LE_ Put ptr");
							LightDbgPrint("USR - LE unex evt %d %d", (U8)ptr1->evtCode, (U8)ptr1->payload.subEvtCode);
						#endif
						OSMEM_Put(ptr1);
					}
                    break;
                }
			}
            else if(SYS_IsTimerExpired(&gMMI_le_rom_ctl.AdvDurationTimerPtr))
			{
				if(MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_DISABLE,NULL))
				{
                    gMMI_le_ctl.rPowerAdv = 1;
					gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ADV_DISABLE_WAIT_EVT;
				}
				else
				{
					SYS_SetTimer(&gMMI_le_rom_ctl.AdvDurationTimerPtr, 1* ONE_SEC);    //get message memory fail, wait 1 second
				}
			}

			break;
		}//end case MMI_LE_ROM_ADV_NORMAL:

		case MMI_LE_ROM_ADV_HIGH_DUTY:
		//wait either connection complete or high duty cycle timeout
		    if((ptr1 = (HCI_LE_EVT_STRU XDATA_PTR)OSMQ_Get(OSMQ_GAP_Event_To_MMILE_ptr)) != (HCI_LE_EVT_STRU XDATA_PTR)NEAR_NULL)
			{
				if((ptr1->evtCode == HCI_EVENT_LE_META) && (ptr1->payload.connCmp.subEvtCode == HCI_EVENT_LE_CONNECTION_COMPLETE))
				{
					#ifdef DBG_UART_ENABLE_MMI_LE
					//LightDbgPrint("USR - LE META -conn complete");
					#endif

					switch(ptr1->payload.connCmp.status)     	//subevent code
					{
						case STATUS_OK:
                            MMI_LE_HdlConnCmpEvt(ptr1, TRUE);
/*
                            LIC_LE_AddLinkInfo(ptr1);
                            MMI_LE_SetState(MMI_LE_ROM_CONNECTED, MMI_LE_ROM_CONNECTING);
                            OSMEM_Put(ptr1);
*/
                            break;

                        case ERR_DIRECTED_ADVERTISING_TIMEOUT:	//TODO
                            MMI_LE_EnableAdv();
                            OSMEM_Put(ptr1);
                            break;

						default:
							#ifdef DBG_UART_ENABLE_MMI_LE
							//LightDbgPrint("USR - unknown Sub Evt");
							#endif
							ASSERT(0);
							break;
					}
				}
				else
				{
					//OSMQ_PutFront(OSMQ_HC_Event_To_Bus_ptr, ptr1);
					//TODO : should have a nother handler to handle such event
					#ifdef DBG_UART_ENABLE_MMI_LE
						//LightDbgPrint("USR - LE unexpected evt %d %d", HC_EVENT_CODE(ptr1), *HC_EVENT_PARAM_PTR(ptr1));
					#endif
					ASSERT(0);
					//OSMEM_Put(ptr1);
				}
			}
			break;

		case MMI_LE_ADV_DISABLE_WAIT_EVT:

			if(MMI_LE_WaitEvent(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_DISABLE)) //disable adv (HCI cmd is the same with enable)
			{
                 if(gMMI_le_ctl.rPowerAdv)
                {
                    MMI_LE_UpdateAdvInfo(MMI_LE_ROM_ADV_PARAM_SEND_CMD);
                }
			}

        	break;

		default:
			#ifdef DBG_UART_ENABLE_MMI_LE
				//LightDbgPrint("USR - MMI_LE Err Substate");
			#endif
			ASSERT(0);
			break;
	}
}

PUBLIC void MMI_LE_ConnectedStateHandler(void)
{
    HCI_LE_EVT_STRU XDATA_PTR evtPtr;
    U8 idx;

	switch(gMMI_le_rom_ctl.MMI_LE_SubState)
	{
		case MMI_LE_ROM_CONNECTING:
		{
		    #ifdef BLE_AIRApp
            AirApp_BlePolling();
            #endif

			if((evtPtr = (HCI_LE_EVT_STRU XDATA_PTR)OSMQ_Get(OSMQ_GAP_Event_To_MMILE_ptr)) != (HCI_LE_EVT_STRU XDATA_PTR)NEAR_NULL)
			{
                switch(evtPtr->evtCode)
                {
                    case HCI_EVENT_DISCONNECTION_COMPLETE:
                    {
                        if(evtPtr->payload.disconnCmp.status == STATUS_OK)
                        {
                            #ifdef DBG_UART_ENABLE_MMI_LE
                                U16 connHdl;
                                connHdl = two_byte_swap(evtPtr->payload.disconnCmp.connHandle);
        						LightDbgPrint("USR - MMI_LE dcc:%d", connHdl);
        					#endif

        					if((idx = LIC_LE_DelLinkInfo(two_byte_swap(evtPtr->payload.disconnCmp.connHandle))) == LIC_INVALID_LINK_INDEX)
        					{
                                break;
        					}
        					{
                                U16 connHdl;
                                connHdl = two_byte_swap(evtPtr->payload.disconnCmp.connHandle);
                                LightDbgPrint("MMI_LE dcc: %x %d", (U8)idx, connHdl);
        					}
    						gMMI_le_ctl.crtLinkCnt--;
    						MMI_LE_CheckStateAfterDisc();
        					L2CAP_LE_ClearSession(idx);
        					ATT_ClearCCCD(idx);
        					ATT_ResetMTUSize();
                            if (LIC_LE_GetNumOfActiveLink() == 0)
                            {
                                gMMI_le_ctl.discActive = FALSE;
                            }

                            #ifdef SUPPORT_LE_XPORT
                            MMI_TrspX_BleDisconnect();
                            gleXport.connPara[0]= 0;
                            #endif

                            #ifdef BLE_AIRApp
                            AirApp_BleUpdateConnStatus(FALSE, NULL);
                            SYS_CLK_Set(SYS_CLK_REQ_LE, SYS_CLK_24M_LEAVE);
                            #endif

                            MMI_LE_NotifAppLinkStatus(idx, MMI_LE_LINK_STATUS_DISCONNECTION);
                        }
                        else
                        {
        					#ifdef DBG_UART_ENABLE_MMI_LE
        						LightDbgPrint("USR - LE dcc evt %x %x", (U8)evtPtr->evtCode, (U8)evtPtr->payload.paraPtr[0]);
        					#endif
                        }
                    }
                    break;

                    #ifdef SUPPORT_LE_XPORT
                    case HCI_EVENT_LE_META:
                    {
                        if((evtPtr->payload.updateCmp.subEvtCode == HCI_EVENT_LE_CONNECTION_UPDATE_COMPLETE) && (evtPtr->payload.updateCmp.status == STATUS_OK))
                        {
                            gleXport.connPara[0]= 0;
                        }
                    }
                    break;
                    #endif

                    default:
                    {
    					#ifdef DBG_UART_ENABLE_MMI_LE
    						LightDbgPrint("USR - LE unex evt %x %x", (U8)evtPtr->evtCode, (U8)evtPtr->payload.subEvtCode);
    					#endif
                    }
                    break;
                }
				OSMEM_Put(evtPtr);
			}
#ifdef SM_SUPPORTED
            MMI_LE_CheckEvtFromSM();
#endif
		}
		break;

		case MMI_LE_ROM_DISCONNECT:
			break;

		default:
			#ifdef DBG_UART_ENABLE_MMI_LE
				//LightDbgPrint("USR - MMI_LE Err Substate");
			#endif
			ASSERT(0);
			break;
	}
}

PUBLIC void MMI_LE_SuspendStateHandler(void)
{
	if((gMMI_le_rom_ctl.MMI_LE_SubState == MMI_LE_ROM_SUSPEND_DISABLE_ADV) && (MMI_LE_WaitEvent(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_DISABLE)))
	{
        gMMI_le_rom_ctl.MMI_LE_SubState = MMI_LE_ROM_ENTER_SUSPEND;
	}
}

PRIVATE U8 MMI_LE_SetRandomAddr(void)
{
    U8 XDATA_PTR msgPtr;
    if((msgPtr = MMI_LE_GetCmdMsgMem(GAP_LE_CMD_FROM_PROFILE_SET_RANDOM_ADDRESS)) == (U8 XDATA_PTR)NULL)
    {
        return LE_FAIL;
    }
    /*
    msgPtr[GAP_COMMAND_PARA0_OFFSET + 0] = 0xFF;
    msgPtr[GAP_COMMAND_PARA0_OFFSET + 1] = 0x78;
    msgPtr[GAP_COMMAND_PARA0_OFFSET + 2] = 0x34;
    msgPtr[GAP_COMMAND_PARA0_OFFSET + 3] = 0x56;
    msgPtr[GAP_COMMAND_PARA0_OFFSET + 4] = 0x89;
    msgPtr[GAP_COMMAND_PARA0_OFFSET + 5] = 0xFF;
    */
    if(pMpParameterF->leBDAddr.type == LE_RANDOMBDADDR_FIXED)
    {
        OS_cmemcpy(msgPtr + GAP_COMMAND_PARA0_OFFSET, (U8 CODE_PTR)&pMpParameterF->leBDAddr.staticRandomAddress.member[0], sizeof(BD_ADDR));
    }
    else if(pMpParameterF->leBDAddr.type == LE_RANDOMBDADDR_RANDOM)
    {
        msgPtr[GAP_COMMAND_PARA0_OFFSET + 0] = DRV_PRBS_Read();
        msgPtr[GAP_COMMAND_PARA0_OFFSET + 1] = DRV_PRBS_Read();
        msgPtr[GAP_COMMAND_PARA0_OFFSET + 2] = DRV_PRBS_Read();
        msgPtr[GAP_COMMAND_PARA0_OFFSET + 3] = DRV_PRBS_Read();
        msgPtr[GAP_COMMAND_PARA0_OFFSET + 4] = DRV_PRBS_Read();
        msgPtr[GAP_COMMAND_PARA0_OFFSET + 5] = DRV_PRBS_Read() | 0xC0;
        OSMEM_memcpy_xdata_xdata(gMMI_le_ctl.randomBdAddr, msgPtr + GAP_COMMAND_PARA0_OFFSET, 6);
    }

    OSMQ_Put(OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);
    return LE_SUCCESS;
}

PRIVATE U8 MMI_LE_SetAdvPara(void)
{
    LE_ADV_PARA_TYPE XDATA_PTR advPtr;
    U8 XDATA_PTR msgPtr;

    if((msgPtr = MMI_LE_GetCmdMsgMem(GAP_LE_CMD_FROM_PROFILE_SET_ADV_PARAMETER)) == (U8 XDATA_PTR)NULL)
    {
        return LE_FAIL;
    }

    OS_cmemcpy(msgPtr + GAP_COMMAND_PARA0_OFFSET, (U8 CODE_PTR)&gMMI_LE_v_nvram_read_ptr->advPara, sizeof(LE_ADV_PARA_TYPE));

    advPtr = (LE_ADV_PARA_TYPE XDATA_PTR)(msgPtr + GAP_COMMAND_PARA0_OFFSET);

    if(pMpParameterF->leBDAddr.type == LE_PUBLIC_DEVICE_ADDR)
    {
        advPtr->ownAddrType = 0;
    }
    else
    {
        advPtr->ownAddrType = 1;
    }
	//Test print
	LightDbgPrint("USR - LE SetAdvPara %x %x", (U8)pMpParameterF->leBDAddr.type, (U8)advPtr->ownAddrType);

    if(gMMI_le_ctl.rPowerAdv)
    {
        advPtr->advIntMin = gMMI_LE_v_nvram_read_ptr->rPowerAdvIntMin;
        advPtr->advIntMax = gMMI_LE_v_nvram_read_ptr->rPowerAdvIntMax;
    }
    OSMQ_Put(OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);
    return LE_SUCCESS;
}

////////////////////////////////////////////////////

PUBLIC U8 MMI_LE_SetHCIcmdToGAP(U8 command_op,U8 XDATA_PTR param)
{
    U8 XDATA_PTR msgPtr;
    #ifdef BLE_AIRApp
	//U8 tmp[6];
    #endif

    if((msgPtr = MMI_LE_GetCmdMsgMem(command_op)) == (U8 XDATA_PTR)NULL)
    {
        return LE_FAIL;
    }
    switch(command_op)
    {
        case GAP_LE_CMD_FROM_PROFILE_SET_ADV_DATA:
			if(param){
				LE_ADV_DATA_TYPE XDATA_PTR advData_ptr= (LE_ADV_DATA_TYPE XDATA_PTR) param;
				OS_cmemcpy(msgPtr + GAP_COMMAND_PARA0_OFFSET, advData_ptr, (advData_ptr->adv_data_Len + 1));
			}else{
            	OS_cmemcpy(msgPtr + GAP_COMMAND_PARA0_OFFSET, (U8 CODE_PTR)&NVRAM_MMI_V_ADV_DATA, (NVRAM_MMI_V_ADV_DATA.adv_data_Len + 1));
			}

			#ifdef BLE_AIRApp
			if(LE_FEATURE_AUTO_PAIRING_SUPPORTED && ((MMI_LE_ADV_DATA_MAX_DATA_LEN - NVRAM_MMI_V_ADV_DATA.adv_data_Len - 1) > 9))
			{
				msgPtr[GAP_COMMAND_PARA0_OFFSET + NVRAM_MMI_V_ADV_DATA.adv_data_Len + 1] = 0x09;
				msgPtr[GAP_COMMAND_PARA0_OFFSET + NVRAM_MMI_V_ADV_DATA.adv_data_Len + 2] = 0xFF;
				msgPtr[GAP_COMMAND_PARA0_OFFSET + NVRAM_MMI_V_ADV_DATA.adv_data_Len + 3] = 0x94;
				msgPtr[GAP_COMMAND_PARA0_OFFSET + NVRAM_MMI_V_ADV_DATA.adv_data_Len + 4] = 0x00;
				//OS_cmemcpy((U8 XDATA_PTR)&tmp[0], (U8 XDATA_PTR)&gHC_ctl.BdAddr, sizeof(BD_ADDR_STRU));
				//ChaCha20Encrypt((U8 XDATA_PTR)&tmp[0], sizeof(BD_ADDR_STRU));
				//ChaCha20Encrypt((U8 XDATA_PTR)&tmp[0], sizeof(BD_ADDR_STRU));
				//CRYPTO_chacha_20((U8 XDATA_PTR)&tmp[0], (U8 XDATA_PTR)&tmp[0], sizeof(BD_ADDR_STRU), key, nonce, 0);

				//OS_cmemcpy(msgPtr + (GAP_COMMAND_PARA0_OFFSET + NVRAM_MMI_V_ADV_DATA.adv_data_Len + 5), (U8 XDATA_PTR)&tmp[0], sizeof(BD_ADDR_STRU));
				OS_cmemcpy(msgPtr + (GAP_COMMAND_PARA0_OFFSET + NVRAM_MMI_V_ADV_DATA.adv_data_Len + 5), (U8 XDATA_PTR)&gHC_ctl.BdAddr, sizeof(BD_ADDR_STRU));
				msgPtr[GAP_COMMAND_PARA0_OFFSET] = NVRAM_MMI_V_ADV_DATA.adv_data_Len + 0x0A;
			}
			#endif
        	break;

        case GAP_LE_CMD_FROM_PROFILE_SET_SCAN_RESP_DATA:
			if(param){
				LE_SCAN_RSP_TYPE XDATA_PTR scanRsp_ptr = (LE_SCAN_RSP_TYPE XDATA_PTR)param;
				OS_cmemcpy(msgPtr + GAP_COMMAND_PARA0_OFFSET, scanRsp_ptr,(scanRsp_ptr->scan_rsp_data_len+1));
			}else{
            	OS_cmemcpy(msgPtr + GAP_COMMAND_PARA0_OFFSET, (U8 CODE_PTR)&gMMI_LE_v_nvram_read_ptr->scanRsp, (gMMI_LE_v_nvram_read_ptr->scanRsp.scan_rsp_data_len+1));
				}
        	break;

        case GAP_LE_CMD_FROM_PROFILE_ADVERTISING_ENABLE:
            *(msgPtr + GAP_COMMAND_PARA0_OFFSET) = LE_ADV_ENABLE;
            break;
        case GAP_LE_CMD_FROM_PROFILE_ADVERTISING_DISABLE:
            *(msgPtr + GAP_COMMAND_PARA0_OFFSET) = LE_ADV_DISABLE;
            break;

    }
    OSMQ_Put(OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);
    return LE_SUCCESS;
}

PUBLIC U8 MMI_LE_WaitEvent(U8 command_op)
{
    U8 XDATA_PTR mq_ptr;
	while(OSMQ_Entries(OSMQ_GAP_Event_To_MMILE_ptr))
    {
        mq_ptr = OSMQ_Get (OSMQ_GAP_Event_To_MMILE_ptr);
        #ifdef DBG_UART_ENABLE_MMI_LE
            //LightDbgPrint("USR - Get event: %d %d ", (U8)mq_ptr[3], (U8)mq_ptr[6]);
        #endif
        if( (mq_ptr[GAP_EVENT_CODE_OFFSET] == command_op))
        {
            OSMEM_Put (mq_ptr);
            return TRUE;
        }
        else
        {
            //OSMQ_PutFront(OSMQ_GAP_Event_To_PM_ptr, mq_ptr);
            //return FALSE;
			OSMEM_Put (mq_ptr); //todo: since 7/15 version PM doesn't handle GAP event to PM, put ptr at here
        }
    }
    return FALSE;
}

PUBLIC U8 XDATA_PTR MMI_LE_GetCmdMsgMem(U8 command_op)
{
    MMI_LE_ROM_CMDTYPE XDATA_PTR ptr1;
    ptr1 = (MMI_LE_ROM_CMDTYPE XDATA_PTR)OSMEM_Get (OSMEM_ptr1);

    if(ptr1 != (MMI_LE_ROM_CMDTYPE XDATA_PTR)NULL)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)ptr1, 0, OSMEM1_BLKSIZE);
    	ptr1->dataOffset = CMD_OFFSET;
		ptr1->cmdOpcode =  command_op;
	    return ptr1;
    }
    else
    {
        return NULL;
    }
}

PUBLIC void MMI_LE_SetDelayWriteFlashTimer(U8 idx, U32 delayTime)
{
	SYS_SetTimer(&gMMI_le_ctl.delayWriteFlashTimerPtr[idx], delayTime);
}

PUBLIC BOOL MMI_LE_AllowUpdateFlash(void)
{
    U8 i;
    #if (LE_SUPPORTED_CONTROLLER)
    if (gLC_ctl.BLE_device_ctl[0].chM_update_ctl.chM_update_bit_ctl.chM_updating || gLC_ctl.BLE_device_ctl[1].chM_update_ctl.chM_update_bit_ctl.chM_updating &&
        gLC_ctl.BLE_device_ctl[0].conn_update_ctl.conn_update_bit_ctl.conn_updating || gLC_ctl.BLE_device_ctl[1].conn_update_ctl.conn_update_bit_ctl.conn_updating)
    {
        return FALSE;
    }
    #endif
    for (i = 0; i < MAX_NUM_OF_LINK; i++)
    {
        if (gMMI_le_ctl.delayWriteFlashTimerPtr[i] != NULL && !SYS_IsTimerExpired(&gMMI_le_ctl.delayWriteFlashTimerPtr[i]))
        {
            return FALSE;
        }
    }

    return TRUE;
}
PUBLIC void MMI_LE_SectorSaveData(void)
{
	if(!gSYS_allow_updating || !(gLIC_hal_ctl.miscMask & LIC_UPDATE_FLASH) || gSYS_update_by_uart_flash_command)
		return;
	if (MMI_HCI_IsInquiryOnGoing() || gMMI_ctl.page_ongoing)
		return;
	if(rs_is_ongoing)
		return;

    if (!MMI_LE_AllowUpdateFlash())
        return;

    //LightDbgPrint("LIC-WillSaveImmToFlash");

	SaveLeNvramTo4kbFlash();
}

PUBLIC BOOL MMI_LE_GetLeBdAddr(MMI_LE_BD_ADDR XDATA_PTR bdAddrPtr)
{
#ifdef MMI_LE_ENABLE
    if (bdAddrPtr == NULL)
    {
        return FALSE;
    }

    switch(pMpParameterF->leBDAddr.type)
    {
        case LE_RANDOMBDADDR_FIXED:
            OSMEM_memcpy_xdata_code(bdAddrPtr, pMpParameterF->leBDAddr.staticRandomAddress.member, 6);
            break;

        case LE_RANDOMBDADDR_RANDOM:
            OSMEM_memcpy_xdata_xdata(bdAddrPtr, gMMI_le_ctl.randomBdAddr, 6);
            break;

        default:
            OSMEM_memcpy_xdata_xdata(bdAddrPtr, (U8 XDATA_PTR)&pMpParameter->MP_SysLocalDeviceInfo.BdAddr, 6);
            break;
    }
    return TRUE;
#else
	UNUSED(bdAddrPtr);
    return FALSE;
#endif
}

PUBLIC U8 MMI_LE_UpdateAdvData(BLE_ADV_DATA_TYPE advData){

	if(!MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_SET_ADV_DATA,(U8 XDATA_PTR)&advData))
    	return MMI_LE_UPDATE_HCI_CMD_FAIL;

	if (!LIC_UpdateNvramParam((U8 XDATA_PTR)&advData, LIC_FLASH_UPDATE_ADV_DATA))
		return MMI_LE_UPDATE_WRITE_FLASH_FAIL;

	return MMI_LE_UPDATE_SUCCESS;

}

PUBLIC U8 MMI_LE_UpdateScanRspData(BLE_SCAN_RSP_TYPE scanRsp){

	if(!MMI_LE_SetHCIcmdToGAP(GAP_LE_CMD_FROM_PROFILE_SET_SCAN_RESP_DATA,(U8 XDATA_PTR)&scanRsp))
		return MMI_LE_UPDATE_HCI_CMD_FAIL;

	if(!LIC_UpdateNvramParam((U8 XDATA_PTR)&scanRsp, LIC_FLASH_UPDATE_SCAN_RSP))
		return MMI_LE_UPDATE_WRITE_FLASH_FAIL;

	return MMI_LE_UPDATE_SUCCESS;
}

#endif
