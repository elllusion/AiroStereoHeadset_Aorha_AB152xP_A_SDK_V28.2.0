#include "reside_flash.inc"

#include "rc.h"
#include "bt_config_profile.h"

#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef BLE_ANCS_C
#include "..\GATT\GATT_Interface.h"
#include "..\ATT\ATT_interface.h"
#include "..\MMI_LE\MMI_LE_interface.h"
#include "BLE_ANCS_C.h"
#include "..\MMI_LE\lic_hal.h"

#define DBG_UART_ENABLE_BLE_ANCS_C

PRIVATE void BLE_ANCS_C_LinkEvtHandler(U8 linkIdx, U8 status);
PRIVATE void BLE_ANCS_C_GATTEvtHandler(GATT_EVT_STRU XDATA_PTR evtPtr);
PRIVATE void BLE_ANCS_C_HandleReport(U8 linkIdx, U8 evtId, U8 status);

STATIC MMI_LE_APP_DESC CODE gANCS_Desc = {
	BLE_ANCS_C_LinkEvtHandler,     //APP_LINK_EVENT  AppLinkEvent;
};

STATIC MMI_LE_APP_NODE XDATA gANCS_Node = {
	&gANCS_Desc,
	NULL,
};

STATIC U8 CODE ANCS_NOTIFICATION_SOURCE_CHAR_UUID[] =
{
    0xBD, 0x1D, 0xA2, 0x99, 0xE6, 0x25, 0x58, 0x8C, 
    0xD9, 0x42, 0x01, 0x63, 0x0D, 0x12, 0xBF, 0x9F
};

STATIC U8 CODE ANCS_CONTROL_POINT_CHAR_UUID[] =
{
    0xD9, 0xD9, 0xAA, 0xFD, 0xBD, 0x9B, 0x21, 0x98,
    0xA8, 0x49, 0xE1, 0x45, 0xF3, 0xD8, 0xD1, 0x69
};

STATIC U8 CODE ANCS_DATA_SOURCE_CHAR_UUID[] =
{
    0xFB, 0x7B, 0x7C, 0xCE, 0x6A, 0xB3, 0x44, 0xBE,
    0xB5, 0x4B, 0xD6, 0x24, 0xE9, 0xC6, 0xEA, 0x22
};

STATIC U8 CODE ANCS_PRIMARY_SERVICE_DISCOVER_PARA[] =
{
    0x01, 0x00,                                         //U16 startHandle;
    0xFF, 0xFF,                                         //U16 endHandle;        
};

STATIC GATT_UUID_STRU CODE ANCS_PRIMARY_SERVICE =
{
    ATT_UUID_128_SIZE,
    0xD0, 0x00, 0x2D, 0x12, 0x1E, 0x4B, 0x0F, 0xA4,
    0x99, 0x4E, 0xCE, 0xB5, 0x31, 0xF4, 0x05, 0x79
};

STATIC U16 CODE ATT_UUID_CCCD = 0x2902;

PUBLIC U8 BLE_ANCS_C_Init(U8 linkIdx, BLE_ANCS_C_EVT_CB cb)
{
    if (gBLE_ANCS_C_ctl[linkIdx].cb)
    {
        return BLE_ANCS_C_STATUS_EXIST_CB;
    }
    
    MMI_LE_RegisterApp(&gANCS_Node);    
    gBLE_ANCS_C_ctl[linkIdx].cb = cb;
    return BLE_ANCS_C_STATUS_SUCCESS;
}

PRIVATE U8 BLE_ANCS_C_SetCCCD(U8 linkIdx, U16 handle, U16 value)
{
    U8 err;
    ANCS_WRITE_CCCD_STRU XDATA_PTR gattParaPtr;
    
    if (handle)
    {
        if (gattParaPtr = SYS_MemoryGet(sizeof(ANCS_WRITE_CCCD_STRU)))
        {
            gattParaPtr->linkIdx = linkIdx;
            gattParaPtr->handle = handle;
            gattParaPtr->writeOp = GATT_WRITE_REQ;
            gattParaPtr->len = 2;
            gattParaPtr->CCCD = ENDIAN_TRANSFORM_U16(value);

            err = GATT_CharacteristicWrite((GATT_WRITE_PARA_STRU XDATA_PTR)gattParaPtr);
            OSMEM_Put((U8 XDATA_PTR)gattParaPtr);
            return err; 
        }
    }
    else
    {
        return BLE_ANCS_C_STATUS_HANDLE_NOT_EXIST;
    }
    return BLE_ANCS_C_STATUS_NO_RESOURCE;
}

PRIVATE U8 BLE_ANCS_C_SetCCCDByState(U8 linkIdx)
{
    U8 err;
    
    if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_NONE)
    {
        if (gBLE_ANCS_C_ctl[linkIdx].action & BLE_ANCS_C_SET_NOTI_SRC_CCCD_MASK)
        {
            if (err = BLE_ANCS_C_SetCCCD(linkIdx, gBLE_ANCS_C_ctl[linkIdx].notifSourceCCCDHdl, gBLE_ANCS_C_ctl[linkIdx].notiSrcCCCD))
            {
                return err;
            }
            else
            {
                gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_WAIT_FOR_WRITE_NOTI_SRC_CCCD_RSP;
                
                return BLE_ANCS_C_STATUS_SUCCESS;
            }
        }
                
        if (gBLE_ANCS_C_ctl[linkIdx].action & BLE_ANCS_C_SET_DATA_SRC_CCCD_MASK)
        {
            if (err = BLE_ANCS_C_SetCCCD(linkIdx, gBLE_ANCS_C_ctl[linkIdx].dataSourceCCCDHdl, gBLE_ANCS_C_ctl[linkIdx].dataSrcCCCD))
            {
                return err;
            }
            else
            {
                gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_WAIT_FOR_WRITE_DATA_SRC_CCCD_RSP;
                
                return BLE_ANCS_C_STATUS_SUCCESS;
            }
        }
        return BLE_ANCS_C_STATUS_SUCCESS;
    }
    else if ((gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_WRITE_NOTI_SRC_CCCD_RSP) || (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_WRITE_DATA_SRC_CCCD_RSP))
    {
        return BLE_ANCS_C_STATUS_SUCCESS;   // action later
    }
    else
    {
        return BLE_ANCS_C_STATUS_BUSY;
    }
}

PUBLIC U8 BLE_ANCS_C_EnableNotifNotificationSource(U8 linkIdx)
{
    if (gBLE_ANCS_C_ctl[linkIdx].notiSrcCCCD == BLE_ANCS_C_CCCD_NOTI_MASK)
    {
        return BLE_ANCS_C_STATUS_SUCCESS;
    }
    
    gBLE_ANCS_C_ctl[linkIdx].notiSrcCCCD = BLE_ANCS_C_CCCD_NOTI_MASK;
    gBLE_ANCS_C_ctl[linkIdx].action |= BLE_ANCS_C_SET_NOTI_SRC_CCCD_MASK;
    return BLE_ANCS_C_SetCCCDByState(linkIdx);
}

PUBLIC U8 BLE_ANCS_C_DisableNotifNotificationSource(U8 linkIdx)
{
    if (gBLE_ANCS_C_ctl[linkIdx].notiSrcCCCD == 0)
    {
        return BLE_ANCS_C_STATUS_SUCCESS;
    }
    
    gBLE_ANCS_C_ctl[linkIdx].notiSrcCCCD = 0;
    gBLE_ANCS_C_ctl[linkIdx].action |= BLE_ANCS_C_SET_NOTI_SRC_CCCD_MASK;
    return BLE_ANCS_C_SetCCCDByState(linkIdx);
}

PUBLIC U8 BLE_ANCS_C_EnableNotifDataSource(U8 linkIdx)
{
    if (gBLE_ANCS_C_ctl[linkIdx].dataSrcCCCD == BLE_ANCS_C_CCCD_NOTI_MASK)
    {
        return BLE_ANCS_C_STATUS_SUCCESS;
    }
    
    gBLE_ANCS_C_ctl[linkIdx].dataSrcCCCD = BLE_ANCS_C_CCCD_NOTI_MASK; 
    gBLE_ANCS_C_ctl[linkIdx].action |= BLE_ANCS_C_SET_DATA_SRC_CCCD_MASK;
    return BLE_ANCS_C_SetCCCDByState(linkIdx);
}

PUBLIC U8 BLE_ANCS_C_DisableNotifDataSource(U8 linkIdx)
{
    if (gBLE_ANCS_C_ctl[linkIdx].dataSrcCCCD == 0)
    {
        return BLE_ANCS_C_STATUS_SUCCESS;
    }
    
    gBLE_ANCS_C_ctl[linkIdx].dataSrcCCCD = 0;
    gBLE_ANCS_C_ctl[linkIdx].action |= BLE_ANCS_C_SET_DATA_SRC_CCCD_MASK;
    return BLE_ANCS_C_SetCCCDByState(linkIdx);
}

PRIVATE BOOL BLE_ANCS_C_IsNeedLenParaAttrId(U8 attrId)
{
    if ((attrId == NOTI_ATTR_ID_TITILE) || (attrId == NOTI_ATTR_ID_SUBTITLE) || (attrId == NOTI_ATTR_ID_MESSAGE))
    {
        return TRUE;
    }
    return FALSE;
}

PRIVATE U16 BLE_ANCS_C_CalNumByteLen(U8 num, BLE_ANCS_C_ATTR_STRU XDATA_PTR attrPtr)
{
    U16 cnt = 0;
    
    while (num)
    {
        if (BLE_ANCS_C_IsNeedLenParaAttrId(attrPtr->attrId))
        {
            cnt += 2;
        }
        num--;
        attrPtr++;
    }
    return cnt;
}

PUBLIC U8 BLE_ANCS_C_GetNotifAttributes(U8 linkIdx, BLE_ANCS_C_GET_NOTI_ATTR_STRU XDATA_PTR paraPtr)
{
    U8 err;
    U8 numAttr;
    U16 dataLen;
    U8 XDATA_PTR dataPtr;
    GATT_WRITE_PARA_STRU XDATA_PTR gattParaPtr;    
    BLE_ANCS_C_ATTR_STRU XDATA_PTR attrPtr;

    if (gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle == 0)
    {
        return BLE_ANCS_C_STATUS_HANDLE_NOT_EXIST;
    }

    numAttr = paraPtr->numAttr;
    attrPtr = paraPtr->attrList;
    dataLen = OS_OFFSET_OF(ANCS_GET_NOTI_ATTR_CMD_STRU, attrList) + BLE_ANCS_C_CalNumByteLen(numAttr, attrPtr) + numAttr;
    if (gattParaPtr = SYS_MemoryGet(OS_OFFSET_OF(GATT_WRITE_PARA_STRU, value) + dataLen))
    {
        gattParaPtr->linkIdx = linkIdx;
        gattParaPtr->writeOp = GATT_WRITE_REQ;
        gattParaPtr->handle = gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle;
        gattParaPtr->len = dataLen;
        dataPtr = gattParaPtr->value;

        ((ANCS_GET_NOTI_ATTR_CMD_STRU XDATA_PTR)dataPtr)->cmdId = CMD_ID_GET_NOTIFICATION_ATTRIBUTES;        
        OSMEM_memcpy_xdata_xdata(((ANCS_GET_NOTI_ATTR_CMD_STRU XDATA_PTR)dataPtr)->notiUID, paraPtr->notiUID, ANCS_NOTIF_UID_SIZE);
        dataPtr = ((ANCS_GET_NOTI_ATTR_CMD_STRU XDATA_PTR)dataPtr)->attrList;

        while(numAttr)
        {
            *dataPtr = attrPtr->attrId;
            dataPtr++;
            if (BLE_ANCS_C_IsNeedLenParaAttrId(attrPtr->attrId))
            {
                *(U16 XDATA_PTR)dataPtr = ENDIAN_TRANSFORM_U16(attrPtr->len);
                dataPtr += 2;
            }
            attrPtr++;
            numAttr--;
        }

        #ifdef DBG_UART_ENABLE_BLE_ANCS_C
        {
            U8 XDATA_PTR printPtr = gattParaPtr->value;
            LightDbgPrint("getNA %x %x %x %x %x %x", (U8)printPtr[0], (U8)printPtr[1], (U8)printPtr[2], (U8)printPtr[3], (U8)printPtr[4], (U8)printPtr[5]);
        }
        #endif
        err = GATT_CharacteristicWrite(gattParaPtr);
        OSMEM_Put((U8 XDATA_PTR)gattParaPtr);
        return err; 
    }
    return BLE_ANCS_C_STATUS_NO_RESOURCE;
}

PUBLIC U8 BLE_ANCS_C_GetAppAttributes(U8 linkIdx, BLE_ANCS_C_GET_APP_ATTR_STRU XDATA_PTR paraPtr)
{
    U8 err;
    U16 dataLen;
    GATT_WRITE_PARA_STRU XDATA_PTR gattParaPtr;
    ANCS_GET_APP_ATTR_CMD_STRU XDATA_PTR cmdPtr;   

    if (gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle == 0)
    {
        return BLE_ANCS_C_STATUS_HANDLE_NOT_EXIST;
    }
    
    dataLen = OS_OFFSET_OF(ANCS_GET_APP_ATTR_CMD_STRU, appId) + paraPtr->appIdLen + paraPtr->numAttr * (sizeof(BLE_ANCS_C_ATTR_STRU));
    if (gattParaPtr = SYS_MemoryGet(OS_OFFSET_OF(GATT_WRITE_PARA_STRU, value) + dataLen))
    {
        gattParaPtr->linkIdx = linkIdx;
        gattParaPtr->writeOp = GATT_WRITE_REQ;
        gattParaPtr->handle = gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle;
        gattParaPtr->len = dataLen;
        cmdPtr = (ANCS_GET_APP_ATTR_CMD_STRU XDATA_PTR)gattParaPtr->value;

        cmdPtr->cmdId = CMD_ID_GET_APP_ATTRIBUTES;
        OSMEM_memcpy_xdata_xdata(cmdPtr->appId, paraPtr->appId, paraPtr->appIdLen);
        OSMEM_memcpy_xdata_xdata(cmdPtr->appId + paraPtr->appIdLen, paraPtr->appId + paraPtr->appIdLen, paraPtr->numAttr * (sizeof(BLE_ANCS_C_ATTR_STRU)));        

        #ifdef DBG_UART_ENABLE_BLE_ANCS_C
        {
            U8 XDATA_PTR printPtr = gattParaPtr->value;
            LightDbgPrint("getAA %x %x %x %x %x %x", (U8)printPtr[0], (U8)printPtr[1], (U8)printPtr[2], (U8)printPtr[3], (U8)printPtr[4], (U8)printPtr[5]);
        }
        #endif
        err = GATT_CharacteristicWrite(gattParaPtr);
        OSMEM_Put((U8 XDATA_PTR)gattParaPtr);
        return err; 
    }
    return BLE_ANCS_C_STATUS_NO_RESOURCE;
}
/*
PUBLIC U8 BLE_ANCS_C_GetAppAttributes(U8 linkIdx, BLE_ANCS_C_GET_APP_ATTR_STRU XDATA_PTR paraPtr)
{
    U8 err;
    U16 dataLen;
    GATT_WRITE_PARA_STRU XDATA_PTR gattParaPtr;
    ANCS_GET_APP_ATTR_CMD_STRU XDATA_PTR cmdPtr;

    if (gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle == 0)
    {
        return BLE_ANCS_C_STATUS_HANDLE_NOT_EXIST;
    }
    
    dataLen = OS_OFFSET_OF(ANCS_GET_APP_ATTR_CMD_STRU, appId) + paraPtr->appIdLen + paraPtr->numAttr;
    if (gattParaPtr = SYS_MemoryGet(OS_OFFSET_OF(GATT_WRITE_PARA_STRU, value) + dataLen))
    {
        gattParaPtr->linkIdx = linkIdx;
        gattParaPtr->writeOp = GATT_WRITE_REQ;
        gattParaPtr->handle = gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle;
        gattParaPtr->len = dataLen;
        cmdPtr = (ANCS_GET_APP_ATTR_CMD_STRU XDATA_PTR)gattParaPtr->value;

        cmdPtr->cmdId = CMD_ID_GET_APP_ATTRIBUTES;
        OSMEM_memcpy_xdata_xdata(cmdPtr->appId, paraPtr->appId, paraPtr->appIdLen);
        OSMEM_memcpy_xdata_xdata(cmdPtr->appId + paraPtr->appIdLen, paraPtr->appId + paraPtr->appIdLen, paraPtr->numAttr);

        #ifdef DBG_UART_ENABLE_BLE_ANCS_C
        {
        U8 XDATA_PTR printPtr = gattParaPtr->value;
        LightDbgPrint("getAA %x %x %x %x %x %x", (U8)printPtr[0], (U8)printPtr[1], (U8)printPtr[2], (U8)printPtr[3], (U8)printPtr[4], (U8)printPtr[5]);
        }
        #endif
        err = GATT_CharacteristicWrite(gattParaPtr);
        OSMEM_Put((U8 XDATA_PTR)gattParaPtr);
        return err; 
    }
    return BLE_ANCS_C_STATUS_NO_RESOURCE;
}*/

PUBLIC U8 BLE_ANCS_C_PerformNotiAction(U8 linkIdx, BLE_ANCS_C_PERFORM_NOTI_ACTION_STRU XDATA_PTR paraPtr)
{
    U8 err;
    GATT_WRITE_PARA_STRU XDATA_PTR gattParaPtr;
    ANCS_PERFORM_NOTIFICATION_ACTION_STRU XDATA_PTR actionPtr;

    if (gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle == 0)
    {
        return BLE_ANCS_C_STATUS_HANDLE_NOT_EXIST;
    }

    if (gattParaPtr = SYS_MemoryGet(OS_OFFSET_OF(GATT_WRITE_PARA_STRU, value) + sizeof(ANCS_PERFORM_NOTIFICATION_ACTION_STRU)))
    {
        gattParaPtr->linkIdx = linkIdx;
        gattParaPtr->writeOp = GATT_WRITE_REQ;
        gattParaPtr->handle = gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle;
        gattParaPtr->len = sizeof(ANCS_PERFORM_NOTIFICATION_ACTION_STRU);

        actionPtr = gattParaPtr->value;
        actionPtr->cmdId = CMD_ID_PERFORM_NOTIFICATION_ACTION;
        OSMEM_memcpy_xdata_xdata(actionPtr->notiUID, (U8 XDATA_PTR)paraPtr, sizeof(BLE_ANCS_C_PERFORM_NOTI_ACTION_STRU));

        #ifdef DBG_UART_ENABLE_BLE_ANCS_C
        {
            U8 XDATA_PTR printPtr = gattParaPtr->value;
            LightDbgPrint("getAA %x %x %x %x %x %x", (U8)printPtr[0], (U8)printPtr[1], (U8)printPtr[2], (U8)printPtr[3], (U8)printPtr[4], (U8)printPtr[5]);
        }
        #endif
        err = GATT_CharacteristicWrite(gattParaPtr);
        OSMEM_Put((U8 XDATA_PTR)gattParaPtr);
        return err; 
    }
    return BLE_ANCS_C_STATUS_NO_RESOURCE;
}

PUBLIC U8 BLE_ANCS_C_WriteData(U8 linkIdx, U8 XDATA_PTR dataPtr, U16 len)
{
    U8 err;
    GATT_WRITE_PARA_STRU XDATA_PTR gattParaPtr;

    if (gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle == 0)
    {
        return BLE_ANCS_C_STATUS_HANDLE_NOT_EXIST;
    }

    if (gattParaPtr = SYS_MemoryGet(OS_OFFSET_OF(GATT_WRITE_PARA_STRU, value) + len))
    {
        gattParaPtr->linkIdx = linkIdx;
        gattParaPtr->writeOp = GATT_WRITE_REQ;
        gattParaPtr->handle = gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charValHandle;
        gattParaPtr->len = len;
        
        OSMEM_memcpy_xdata_xdata(gattParaPtr->value, (U8 XDATA_PTR)dataPtr, len);

        #ifdef DBG_UART_ENABLE_BLE_ANCS_C
        {
            U8 XDATA_PTR printPtr = gattParaPtr->value;
            LightDbgPrint("getAA %x %x %x %x %x %x", (U8)printPtr[0], (U8)printPtr[1], (U8)printPtr[2], (U8)printPtr[3], (U8)printPtr[4], (U8)printPtr[5]);
        }
        #endif
        err = GATT_CharacteristicWrite(gattParaPtr);
        OSMEM_Put((U8 XDATA_PTR)gattParaPtr);
        return err; 
    }
    return BLE_ANCS_C_STATUS_NO_RESOURCE;
}

PUBLIC U8 BLE_ANCS_C_WriteVibrationSetting(U8 XDATA_PTR dataPtr)
{
    return LIC_UpdateNvramParam(dataPtr, LIC_FLASH_UPDATE_VIBRATION_SETTING);
}

PUBLIC U8 CODE_PTR BLE_ANCS_C_GetVibrationSetting(void)
{
    return NVRAM_MMI_V_VIBRATION_SETTING_PTR;
}

PRIVATE void BLE_ANCS_C_PrimServDiscRsp(U8 linkIdx, GATT_PRIM_SERV_DISC_RSP XDATA_PTR rspPtr)
{
    GATT_CHAR_DISC_PARA_STRU para;

    #ifdef DBG_UART_ENABLE_BLE_ANCS_C
    LightDbgPrint("primSerDisc cnt %x %x %x", (U8)rspPtr->count, (U8)rspPtr->service[0].startHandle, (U8)(rspPtr->service[0].startHandle>>8));
    #endif
    
    if (rspPtr->count)
    {        
        para.linkIdx = linkIdx;
        para.startHandle = rspPtr->service[0].startHandle;
        para.endHandle = rspPtr->service[0].endHandle;        

        GATT_CharacteristicDiscover(&para);

        gBLE_ANCS_C_ctl[linkIdx].serviceRange.startHandle = para.startHandle;
        gBLE_ANCS_C_ctl[linkIdx].serviceRange.endHandle = para.endHandle;
    }
    else
    {
        BLE_ANCS_C_HandleReport(linkIdx, ANCS_C_EVT_DISCOVER_FAILED, BLE_ANCS_C_STATUS_NOT_SERVICE);
    }
}

PRIVATE void BLE_ANCS_C_UpdataCharHandle(U8 linkIdx, GATT_CHAR_DECL_STRU XDATA_PTR charDeclPtr)
{
    U8 XDATA_PTR uuid;
    U16 endHandle;
    
    UNUSED(linkIdx);
    if (charDeclPtr->charUUID.uuidLen == ATT_UUID_128_SIZE)
    {
        if ((uuid = SYS_MemoryGet(ATT_UUID_128_SIZE)) == NULL)
        {
            return;
        }
        if (gBLE_ANCS_C_ctl[linkIdx].notiSourceChar.charDeclHandle == 0)
        {
            OSMEM_memcpy_xdata_code(uuid, ANCS_NOTIFICATION_SOURCE_CHAR_UUID, ATT_UUID_128_SIZE);      
            if (!OS_memcmp(charDeclPtr->charUUID.uuid, uuid, ATT_UUID_128_SIZE))
            {
                #ifdef DBG_UART_ENABLE_BLE_ANCS_C
                LightDbgPrint("found noti src char %x %x", (U8)charDeclPtr->charValHandle, (U8)(charDeclPtr->charValHandle>>8));
                #endif
                OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gBLE_ANCS_C_ctl[linkIdx].notiSourceChar, (U8 XDATA_PTR)charDeclPtr, sizeof(GATT_CHAR_DECL_STRU));

                if (gBLE_ANCS_C_ctl[linkIdx].notiSourceChar.charValHandle)
                {
                    gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.startHandle = ENDIAN_TRANSFORM_U16(gBLE_ANCS_C_ctl[linkIdx].notiSourceChar.charValHandle) + 1;
                    gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.endHandle = ENDIAN_TRANSFORM_U16(gBLE_ANCS_C_ctl[linkIdx].serviceRange.endHandle);
                }
                OSMEM_Put(uuid);
                goto FIND_END_HANDLE;
            }
        }

        if (gBLE_ANCS_C_ctl[linkIdx].controlPointChar.charDeclHandle == 0)
        {
            OSMEM_memcpy_xdata_code(uuid, ANCS_CONTROL_POINT_CHAR_UUID, ATT_UUID_128_SIZE);      
            if (!OS_memcmp(charDeclPtr->charUUID.uuid, uuid, ATT_UUID_128_SIZE))
            {
                #ifdef DBG_UART_ENABLE_BLE_ANCS_C
                LightDbgPrint("found control point char %x %x", (U8)charDeclPtr->charValHandle, (U8)(charDeclPtr->charValHandle>>8));
                #endif
                OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gBLE_ANCS_C_ctl[linkIdx].controlPointChar, (U8 XDATA_PTR)charDeclPtr, sizeof(GATT_CHAR_DECL_STRU)); 

                OSMEM_Put(uuid);
                goto FIND_END_HANDLE;
            }
        }

        if (gBLE_ANCS_C_ctl[linkIdx].dataSourceChar.charDeclHandle == 0)
        {
            OSMEM_memcpy_xdata_code(uuid, ANCS_DATA_SOURCE_CHAR_UUID, ATT_UUID_128_SIZE);      
            if (!OS_memcmp(charDeclPtr->charUUID.uuid, uuid, ATT_UUID_128_SIZE))
            {
                #ifdef DBG_UART_ENABLE_BLE_ANCS_C
                LightDbgPrint("found data src char %x %x", (U8)charDeclPtr->charValHandle, (U8)(charDeclPtr->charValHandle>>8));
                #endif
                OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gBLE_ANCS_C_ctl[linkIdx].dataSourceChar, (U8 XDATA_PTR)charDeclPtr, sizeof(GATT_CHAR_DECL_STRU));
                
                if (gBLE_ANCS_C_ctl[linkIdx].dataSourceChar.charValHandle)
                {
                    gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.startHandle = ENDIAN_TRANSFORM_U16(gBLE_ANCS_C_ctl[linkIdx].dataSourceChar.charValHandle) + 1;
                    gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.endHandle = ENDIAN_TRANSFORM_U16(gBLE_ANCS_C_ctl[linkIdx].serviceRange.endHandle);
                }
                OSMEM_Put(uuid);
                goto FIND_END_HANDLE;
            }
        }
        OSMEM_Put(uuid);
    }
    
    FIND_END_HANDLE:
    if (charDeclPtr->charDeclHandle)
    {
        endHandle = ENDIAN_TRANSFORM_U16(charDeclPtr->charDeclHandle) - 1;

        if (gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.startHandle && (endHandle >= gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.startHandle) && (endHandle < gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.endHandle))
        {
            gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.endHandle = endHandle;
            
            #ifdef DBG_UART_ENABLE_BLE_ANCS_C
            LightDbgPrint("noSrc findHdl %x %x %x %x", (U8)gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.startHandle, (U8)(gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.startHandle>>8), 
                                                        (U8)gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.endHandle, (U8)(gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.endHandle>>8));
            #endif
        }
        if (gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.startHandle && (endHandle >= gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.startHandle) && (endHandle < gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.endHandle))
        {
            gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.endHandle = endHandle;
            
            #ifdef DBG_UART_ENABLE_BLE_ANCS_C
            LightDbgPrint("dataSrc findHdl %x %x %x %x", (U8)gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.startHandle, (U8)(gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.startHandle>>8), 
                                                        (U8)gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.endHandle, (U8)(gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.endHandle>>8));
            #endif
        }
    }
}

PRIVATE void BLE_ANCS_C_HandleReport(U8 linkIdx, U8 evtId, U8 status)
{
    if (gBLE_ANCS_C_ctl[linkIdx].cb)
    {
        BLE_ANCS_C_EVT_STRU evt;

        evt.evtId = evtId;
        evt.linkIdx = linkIdx;
        evt.rsp.errCode = status;
        gBLE_ANCS_C_ctl[linkIdx].cb(&evt);
    } 
}

PRIVATE void BLE_ANCS_C_CharDescDiscover(U8 linkIdx)
{
    GATT_CHAR_DESC_PARA_STRU para;

    if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_NONE)
    {
        if (gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.startHandle)
        {
            para.linkIdx = linkIdx;
            para.startHandle = ENDIAN_TRANSFORM_U16(gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.startHandle);
            para.endHandle = ENDIAN_TRANSFORM_U16(gBLE_ANCS_C_ctl[linkIdx].findNotiSrcDesc.endHandle);
            
            GATT_CharacteristicDescriptorsDiscover(&para);

            gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_WAIT_FOR_FIND_INFO_NOTIF_SOURCE;
        }
        else
        {
            BLE_ANCS_C_HandleReport(linkIdx, ANCS_C_EVT_DISCOVER_FAILED, BLE_ANCS_C_STATUS_NO_NOTI_SRC_CCCD_HANDLE);
        }
    }
    else if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_FIND_INFO_DATA_SOURCE)
    {
        if (gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.startHandle)
        {
            para.linkIdx = linkIdx;
            para.startHandle = ENDIAN_TRANSFORM_U16(gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.startHandle);
            para.endHandle = ENDIAN_TRANSFORM_U16(gBLE_ANCS_C_ctl[linkIdx].findDataSrcDesc.endHandle);

            GATT_CharacteristicDescriptorsDiscover(&para);

            gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_WAIT_FOR_FIND_INFO_DATA_SOURCE;
        }
        else
        {
            gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_NONE;
            BLE_ANCS_C_HandleReport(linkIdx, ANCS_C_EVT_DISCOVER_COMPLETE, BLE_ANCS_C_STATUS_SUCCESS);  // since data source charateristic is optional
        }
    }
    else if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_FIND_INFO_COMPLETE)
    {
        gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_NONE;
        BLE_ANCS_C_HandleReport(linkIdx, ANCS_C_EVT_DISCOVER_COMPLETE, BLE_ANCS_C_STATUS_SUCCESS);               
    }
}

PRIVATE void BLE_ANCS_C_CharDeclDiscRsp(U8 linkIdx, GATT_CHAR_DECL_DISC_RSP XDATA_PTR rspPtr)
{
    GATT_CHAR_DECL_STRU XDATA_PTR charDeclPtr;
    U16 count = rspPtr->count;

    charDeclPtr = rspPtr->chars;    
    while (count)
    {
        BLE_ANCS_C_UpdataCharHandle(linkIdx, charDeclPtr);
        charDeclPtr++;
        count--;
    }
    BLE_ANCS_C_CharDescDiscover(linkIdx);
}

PRIVATE void BLE_ANCS_C_UpdateCCCDHdl(U8 linkIdx, U16 handle)
{
    if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_FIND_INFO_NOTIF_SOURCE)
    {
        #ifdef DBG_UART_ENABLE_BLE_ANCS_C
        LightDbgPrint("notiSrcCCCD %x %x", (U8)handle, (U8)(handle>>8));
        #endif
        
        gBLE_ANCS_C_ctl[linkIdx].notifSourceCCCDHdl = handle;
        gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_FIND_INFO_DATA_SOURCE;
    }
    else if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_FIND_INFO_DATA_SOURCE)
    {
        #ifdef DBG_UART_ENABLE_BLE_ANCS_C
        LightDbgPrint("dataSrcCCCD %x %x", (U8)handle, (U8)(handle>>8));
        #endif
        
        gBLE_ANCS_C_ctl[linkIdx].dataSourceCCCDHdl = handle;
        gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_FIND_INFO_COMPLETE;
    }
}

PRIVATE void BLE_ANCS_C_CharDescDiscRsp(U8 linkIdx, GATT_CHAR_DESC_DISC_RSP XDATA_PTR rspPtr)
{    
    GATT_CHAR_DESC_STRU XDATA_PTR descPtr;
    U16 count = rspPtr->count;

    descPtr = rspPtr->desc;
    while (count)
    {        
        if ((descPtr->descUUID.uuidLen == sizeof(ATT_UUID_CCCD)) && ((*(U16 XDATA_PTR)descPtr->descUUID.uuid) == ENDIAN_TRANSFORM_U16(ATT_UUID_CCCD)))
        {            
            BLE_ANCS_C_UpdateCCCDHdl(linkIdx, descPtr->handle);
            break;
        }
        count--;
        descPtr++;
    }    
    BLE_ANCS_C_CharDescDiscover(linkIdx);
}

PRIVATE void BLE_ANCS_C_WriteRsp(U8 linkIdx, U8 status)
{
    if (status == GATT_STATUS_SUCCESS)
    {
        if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_WRITE_NOTI_SRC_CCCD_RSP)
        {
            gBLE_ANCS_C_ctl[linkIdx].action &= (~BLE_ANCS_C_SET_NOTI_SRC_CCCD_MASK);
            gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_NONE;
        }
        else if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_WRITE_DATA_SRC_CCCD_RSP)
        {
            gBLE_ANCS_C_ctl[linkIdx].action &= (~BLE_ANCS_C_SET_DATA_SRC_CCCD_MASK);
            gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_NONE;
        }        
    }
    else if (status == GATT_STATUS_INSUFFICIENT_AUTHEN)
    {        
        MMI_LE_AuthenticationRequest(linkIdx);
        gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_WAIT_FOR_AUTHENTICATION;
    }
    else if (status == GATT_STATUS_INSUFFICIENT_ENCRYPT)
    {
        if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_WRITE_NOTI_SRC_CCCD_RSP || gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_WRITE_DATA_SRC_CCCD_RSP)
        {
            // PIN Or Key Missing
            //MMI_LE_AuthenticationRequest(linkIdx);    //re-send security request is not working
            gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_NONE;
            return;
        }
        else
        {
            gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_WAIT_FOR_ENCRYPT;
        }
    }
    BLE_ANCS_C_SetCCCDByState(linkIdx);
}

PRIVATE void BLE_ANCS_C_HVX(U8 linkIdx, GATT_HVX_STRU XDATA_PTR rspPtr)
{
    BLE_ANCS_C_EVT_STRU XDATA_PTR evtPtr;

    evtPtr = (BLE_ANCS_C_EVT_STRU XDATA_PTR)(((U8 XDATA_PTR)rspPtr) + 1);
    if (gBLE_ANCS_C_ctl[linkIdx].cb)
    {
        if (rspPtr->handle == gBLE_ANCS_C_ctl[linkIdx].notiSourceChar.charValHandle)
        {
            if (rspPtr->len != sizeof(ANCS_NOTI_SRC_STRU))
            {
                return;
            }
            evtPtr->linkIdx = linkIdx;
            evtPtr->evtId = ANCS_C_EVT_SOURCE_NOTIFICATION;
            gBLE_ANCS_C_ctl[linkIdx].cb(evtPtr);
        }
        else if (rspPtr->handle == gBLE_ANCS_C_ctl[linkIdx].dataSourceChar.charValHandle)
        {
            evtPtr->linkIdx = linkIdx;
            evtPtr->evtId = ANCS_C_EVT_DATA_NOTIFICATION;
            gBLE_ANCS_C_ctl[linkIdx].cb(evtPtr);
        }
    }    
}

PRIVATE void BLE_ANCS_C_GATTEvtHandler(GATT_EVT_STRU XDATA_PTR evtPtr)
{
    #ifdef DBG_UART_ENABLE_BLE_ANCS_C
    LightDbgPrint("ANCS_GATTEvtHandle %x %x", (U8)evtPtr->evtId, (U8)evtPtr->status);
    #endif
    
    switch (evtPtr->evtId)
    {
        case GATT_EVT_PRIM_SERVC_DISC_RSP:
        {
            #ifdef DBG_UART_ENABLE_BLE_ANCS_C
            LightDbgPrint("GATT_EVT_PRIM_SERVC_DISC_RSP %x");
            #endif
            BLE_ANCS_C_PrimServDiscRsp(evtPtr->linkIdx, (GATT_PRIM_SERV_DISC_RSP XDATA_PTR)&evtPtr->rsp.primServDiscRsp);
        }
        break;

        case GATT_EVT_CHAR_DISC_RSP:
        {
            #ifdef DBG_UART_ENABLE_BLE_ANCS_C
            LightDbgPrint("GATT_EVT_CHAR_DISC_RSP %x");
            #endif
            BLE_ANCS_C_CharDeclDiscRsp(evtPtr->linkIdx, (GATT_CHAR_DECL_DISC_RSP XDATA_PTR)&evtPtr->rsp.charDeclDiscRsp);
        }
        break;

        case GATT_EVT_DESC_DISC_RSP:
        {
            #ifdef DBG_UART_ENABLE_BLE_ANCS_C
            LightDbgPrint("GATT_EVT_DESC_DISC_RSP %x", (U8)gBLE_ANCS_C_ctl[evtPtr->linkIdx].state);
            #endif
            BLE_ANCS_C_CharDescDiscRsp(evtPtr->linkIdx, (GATT_CHAR_DESC_DISC_RSP XDATA_PTR)&evtPtr->rsp.charDescDiscRsp);
        }
        break;

        case GATT_EVT_WRITE_RSP:
        {
            #ifdef DBG_UART_ENABLE_BLE_ANCS_C
            LightDbgPrint("GATT_EVT_WRITE_RSP %x %x", (U8)evtPtr->status, (U8)gBLE_ANCS_C_ctl[evtPtr->linkIdx].state);
            #endif
            BLE_ANCS_C_WriteRsp(evtPtr->linkIdx, evtPtr->status);
            
        }
        break;
            
        case GATT_EVT_HVX:
        {
            #ifdef DBG_UART_ENABLE_BLE_ANCS_C
            LightDbgPrint("GATT_EVT_HVX %x");
            #endif
            BLE_ANCS_C_HVX(evtPtr->linkIdx, (GATT_HVX_STRU XDATA_PTR)&evtPtr->rsp.hvxEvt);
        }
        break;
    }
}

PRIVATE void BLE_ANCS_C_StartPrimSerDiscover(U8 linkIdx)
{
    GATT_PRIM_SERV_DISC_PARA_STRU XDATA_PTR gattParaPtr;

    if (gattParaPtr = (GATT_PRIM_SERV_DISC_PARA_STRU XDATA_PTR)SYS_MemoryGet(sizeof(GATT_PRIM_SERV_DISC_PARA_STRU)))
    {
        gattParaPtr->linkIdx = linkIdx;
        OSMEM_memcpy_xdata_code(&gattParaPtr->startHandle, ANCS_PRIMARY_SERVICE_DISCOVER_PARA, sizeof(ANCS_PRIMARY_SERVICE_DISCOVER_PARA));
        gattParaPtr->primServUuidPtr = (GATT_UUID_STRU*)&ANCS_PRIMARY_SERVICE;            
        GATT_PrimaryServiceDiscover(gattParaPtr);
        OSMEM_Put((U8 XDATA_PTR)gattParaPtr);
        GATT_RegisterCB(linkIdx, BLE_ANCS_C_GATTEvtHandler);
    }
}

PRIVATE void BLE_ANCS_C_LinkEvtHandler(U8 linkIdx, U8 status)
{
    #ifdef DBG_UART_ENABLE_BLE_ANCS_C
    LightDbgPrint("ANCS link status %x %x", (U8)linkIdx, (U8)status);
    #endif
    
    switch (status)
    {
        case MMI_LE_LINK_STATUS_CONNECTION_CMP:
        {            
            BLE_ANCS_C_StartPrimSerDiscover(linkIdx);            
        }
        break;
            
        case MMI_LE_LINK_STATUS_AUTHENTICATION_CMP:
        {                        
            if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_AUTHENTICATION)
            {
                gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_NONE;
                BLE_ANCS_C_SetCCCDByState(linkIdx);
            }            
        }
        break;

        case MMI_LE_LINK_STATUS_ENCRYPTION_CMP:
        {
            if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_ENCRYPT)
            {
                gBLE_ANCS_C_ctl[linkIdx].state = ANCS_STATE_NONE;
                BLE_ANCS_C_SetCCCDByState(linkIdx);
            }
        }
        break;

        case MMI_LE_LINK_STATUS_ENCRYPTION_FAIL:
        {
            if (gBLE_ANCS_C_ctl[linkIdx].state == ANCS_STATE_WAIT_FOR_ENCRYPT)
            {
                // todo:
            }
        }
        break;

        case MMI_LE_LINK_STATUS_DISCONNECTION:
        {            
            OSMEM_memset_xdata((U8 XDATA_PTR)&gBLE_ANCS_C_ctl[linkIdx], 0, OS_OFFSET_OF(BLE_ANCS_C_CTL, cb));
            GATT_UnregisterCB(linkIdx);
            GATT_Reset(linkIdx);
        }
        break;
    }
}
#endif
#endif
