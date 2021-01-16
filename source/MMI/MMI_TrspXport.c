#include "reside_flash.inc"

#define _MMI_TRSPX_C_


#include "MMI.h"
#include "MMI_Protocol.h"
#ifdef SPP_Profile
#include "..\RFCOMM\RFCOMM_Interface.h"
#include "..\SPP\SPP_Interface.h"
#endif

#include "..\HC\HC_interface.h"
#include "..\ROM\GAP\GAP_ROM_Interface.h"
#ifdef TRSPX_App
#include "MMI_TrspXport.h"
#include "MMI_HCI.h"
#include "MMI_State.h"


PUBLIC void MMI_TrspX_Init(void)
{
    U8 i;
    
    for (i = 0; i < MAX_MULTI_POINT_NO; i++)
    {
        gMMI_ctl.passkey[i] = MMI_TRSPX_PASSKEY_INVALID;
    }
}

PRIVATE void MMI_SendGAPADVCmd(U8 cmd)
{
    MMIMsgType XDATA_PTR msgPtr;

    if ((msgPtr = (MMIMsgType XDATA_PTR)SYS_MemoryGet(OS_OFFSET_OF(MMIMsgType, msgBodyPtr) + 1)) != NULL)
    {
        msgPtr->dataOffset = CMD_OFFSET;
		msgPtr->msgOpcode =  cmd;
		msgPtr->msgBodyPtr.gapPara = (cmd == GAP_LE_CMD_FROM_PROFILE_ADVERTISING_DISABLE)? FALSE : TRUE;
        OSMQ_Put(OSMQ_GAP_Command_From_PROFILE_ptr, (U8 XDATA_PTR)msgPtr);
    }
}

/* For BLE connection complete use */

PUBLIC void MMI_TrspX_BleConnected(U8 XDATA_PTR addrPtr, U16 connHandle)
{
	if((gTrspx_ctl.mmiBleIdx = MMI_GetEmptyLinkIndex(addrPtr, FALSE)) < MAX_MULTI_POINT_NO)
	{
    	OSMEM_memcpy_xdata_xdata ((U8 XDATA_PTR)gMMI_ctl.mmiInfo[gTrspx_ctl.mmiBleIdx].bdAddr, (U8 XDATA_PTR)addrPtr, 6);
		MMI_SetConnectionState(gTrspx_ctl.mmiBleIdx, MMI_CONN_STATE_ACL_CONNECTED);
		gMMI_ctl.mmiInfo[gTrspx_ctl.mmiBleIdx].linkPara.aclConnHandle = connHandle;
		MMI_ADD_LINK(gTrspx_ctl.mmiBleIdx);
        MMI_ConnectableStateChangeWhenConnected();
		if(!MMI_ALWAYS_ENA_DISCOVER_MODE_FEAT)
		{
			MMI_SetEnterDiscoverable(FALSE);
			if(MMI_CheckNestState(MMI_EOF, MMI_CONDISCABLE))
			{
				MMI_RmvState(MMI_EOF, MMI_CONDISCABLE);
				MMI_SetEnterDiscoverable(FALSE);
			}
		}
		MMI_InsertState(gTrspx_ctl.mmiBleIdx, MMI_CONNECTED, MMI_CheckNestState(gTrspx_ctl.mmiBleIdx, MMI_CONDISCABLE) ? MMI_CONDISCABLE : MMI_CONNECTABLE);

        //MMI_ActivateProfile(gTrspx_ctl.mmiBleIdx, &gMMITrspX_ProfileNode[linkIndex]);
	}
    MMI_TrspX_connectHandle(gTrspx_ctl.mmiBleIdx, MMI_TRSPX_CONNECTED_BLE);
}

PUBLIC void MMI_TrspX_BleDisconnect(void)
{
    if(gTrspx_ctl.mmiBleIdx != MAX_MULTI_POINT_NO)
    {
        //MMI_HCI_AclDiscHandle(gTrspx_ctl.mmiBleIdx);
        //MMI_PROTOCOL_ProfileDisconnected(gTrspx_ctl.mmiBleIdx, PM_CONNECTION_LE);
        MMI_RmvState(gTrspx_ctl.mmiBleIdx, MMI_CONNECTED);
        MMI_DEL_LINK(gTrspx_ctl.mmiBleIdx);
		gMMI_ctl.mmiInfo[gTrspx_ctl.mmiBleIdx].subState = MMI_INIT_SYS_CONFIGED;
		//MMI_SetConnectionState(gTrspx_ctl.mmiBleIdx, MMI_CONN_STATE_NONE);
		MMI_ClearMMILinkSettingType(gTrspx_ctl.mmiBleIdx);
        //MMI_SetEnterDiscoverable(TRUE);
        //MMI_InsertState(MMI_EOF, MMI_CONDISCABLE,MMI_CONNECTABLE);
		MMI_InsertState(MMI_EOF, MMI_CONNECTABLE,MMI_FAKEON);
		MMI_InsertState(MMI_EOF, MMI_CONDISCABLE,MMI_CONNECTABLE);
        gTrspx_ctl.mmiBleIdx = MAX_MULTI_POINT_NO;
    }
}

PUBLIC void MMI_TrspX_connectHandle(U8 linkIndex, U8 connMode)
{
    UNUSED(linkIndex);
    
    switch(connMode)
    {
        case MMI_TRSPX_CONNECTED_BR_EDR:
        {
            MMI_SendGAPADVCmd(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_DISABLE);
        }
        break;

        case MMI_TRSPX_CONNECTED_BLE:
            break;

        case MMI_TRSPX_CONNECTED_NONE:
        {
            if(gMMI_ctl.connectedMode == MMI_TRSPX_CONNECTED_BR_EDR)
            {
                if (CURRENT_ACTIVE_LINK_CNT == 0)
                {
                    MMI_SendGAPADVCmd(GAP_LE_CMD_FROM_PROFILE_ADVERTISING_ENABLE);
                }
                else
                {
                    return; // keep connectedMode = MMI_TRSPX_CONNECTED_BR_EDR
                }
            }
            else
            {
                MMI_TrspX_BleDisconnect();
            }
        }
        break;

        default:
            break;
    }
    gMMI_ctl.connectedMode = connMode;
}

#endif
