#include "reside_flash.inc"

#define __AIRAPP_C__

#include "rc.h"
#include "bt_config_profile.h"

#ifdef AIRAPP_Profile
#include "drv_sector.h"
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI.h"
#include "..\MMI\MMI_SCN.h"
#include "..\MMI\MMI_HCI.h"
#include "..\MMI\MMI_Driver.h"
#include "..\MMI\MMI_ChgBat.h"
#include "..\MMI\MMI_LineIn.h"
#include "..\MMI\MMI_AVRCP.h"
#include "..\MMI\MMI_A2DP.h"
#include "..\MMI\MMI_HFP.h"
#include "..\MMI\MMI_State.h"
#include "..\MMI\MMI_SectorManager.h"
#include "..\MMI\MMI_AirModule.h"
#include "..\MMI\MMI_Interface.h"
#include "..\MMI\MMI_Protocol.h"
#include "..\MMI\MMI_LED.h"
#include "..\SDAP\SDAP_Interface.h"
#include "..\RFCOMM\RFCOMM_Interface.h"
#include "..\Driver\Peq_HPF.h"
#include "..\Driver\AudioControl.h"
#include "..\Driver\Driver.h"
#include "..\Driver\Driver_Interface.h"
#include "..\AVRCP\AVRCP_Interface.h"
#include "..\A2DP\A2DP_Interface.h"
#include "sector_runtime.h"
#include "drv_flash.h"
#include "drv_program_flash.h"
#include "drv_program_flash_flash.h"

#include "AirApp_Interface.h"
#include "..\APP\APP_Interface.h"

#ifdef BLE_AIRApp
#include "..\ATT\ATT_interface.h"
#include "..\MMI_LE\MMI_LE_interface.h"
#include "..\COMMON\le_common_hal.h"
#endif

#include "..\Driver\VoiceCommand.h"
#include "align_flash.h"

#include "..\SDK_Proxy\SDK_Proxy.h"

#include "..\SECTOR\ToolMisc\sector_tool_misc.h"
#include "..\SECTOR\sector_ae_info.h"


U8 CODE gAirApp_uuid128[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};

#define AIRAPP_DATA_STATE_NO_DATA   0
#define AIRAPP_DATA_STATE_CONTINUE  1

typedef struct
{
    U8 status;
    U8 remoteNameLen;
    U8 remoteName[MAX_REMOTE_NAME_LEN];
}AIRAPP_BLE_REPORT_STATUS_TYPE;

typedef struct
{
    U8 targetIdx;
    AIRAPP_BLE_REPORT_STATUS_TYPE report[MAX_MULTI_POINT_NO];
}AIRAPP_BLE_REPORT_STATUS;

typedef struct
{
    U8 state;
    U8 XDATA_PTR dataPtr;
    U16 offset;
    U16 remainLength;
}AIRAPP_DATA_CTL_TYPE;

typedef struct
{
    MMI_PROFILE_INFO mmiProfileInfo;
    U8 batStatus;
    U8 isVPOn:1;
    U8 isCallerNameOn:1;
    U8 reserved:2;
    U8 VPLang:4;
    U8 vol;
    U8 cmdState;
    U8 PEQMode[PEQ_COMPONENT_NUM];
    U8 remoteNameLen;
    U8 remoteName[MAX_REMOTE_NAME_LEN];
    U8 PEQSectorMode[PEQ_COMPONENT_NUM];
    OSMQ OSMQ_AIRAPP_Data_From_Remote;
    AIRAPP_DATA_CTL_TYPE    dataCtl;
} AIRAPP_LINK_INFO;

enum
{
    DSP_UNKNOW_STATE,
    DSP_SUSPENDING_STATE,
    DSP_SUSPENDED_STATE,
};

typedef struct
{
    U8 dspSuspendState;
    U8 callerNameNextId;
    U8 callerNameLinkIndex;
    OST XDATA_PTR suspendDSPTimer;
    U8 dataValid;
}AIRAPP_CALLER_NAME_CTL;

typedef struct
{
    AIRAPP_LINK_INFO linkInfo[MAX_MULTI_POINT_NO];
    U8 flashUpdateLinkIndex;
#ifdef BLE_AIRApp
    U8 isBleConnected:1;
    U8 isCCCDEnable:1;
    U8 isVPOn:1;
    U8 VPLang:4;
    U8 rsd:1;
    U8 batStatus;
    U8 targetIdx;
    U8 normalLinkNum;
    OST XDATA_PTR delaySendDataTimer;
    AIRAPP_DATA_CTL_TYPE dataCtl;
#endif
#if AIRAPP_CALLER_NAME
    AIRAPP_CALLER_NAME_CTL callNameCtl;
#endif
} AIRAPP_CTL;

AIRAPP_CTL XDATA gAirApp_ctl;

extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];
PRIVATE void AirApp(U8 linkIndex);
PRIVATE MMI_PROFILE_INFO XDATA_PTR AirApp_ConnectRequest(U8 linkIndex, U8 profileId);
PRIVATE U8 AirApp_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
PRIVATE void AirApp_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
PRIVATE void AirApp_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr);
#ifdef BLE_AIRApp
PRIVATE void AirApp_BleReportEdrLinkStatus(void);
PRIVATE BOOL AirApp_BleSendAclErrorRsp(U8 ocf);
PRIVATE void AirApp_BleGetEdrLinkStatus(AIRAPP_BLE_REPORT_STATUS XDATA_PTR pData);
PRIVATE void AirApp_BleResetEdrProfileState(AIRAPP_LINK_INFO XDATA_PTR airAppLinkInfo);
PRIVATE void AirApp_BleResetAllEdrProfileState(void);

#endif
PRIVATE void AirApp_MMIKeyEventRsp(U8 linkIndex, U16 mmiKeyCode,U8 result);
#ifdef LIGHTING_ENABLE
AirAppLedCtl XDATA gAirAppLedCtl;
#endif

static RFCOMM_PROFILE_DESC CODE gAirApp_ProfileDesc = {
    { //MMI_PROFILE_DESC
        PROFILE_AIR_APP,
        AirApp,                 //ProfileTaskFunc
        AirApp_ConnectRequest,      //ProfileConnectRequest
        AirApp_DisconnectRequest,   //ProfileDisconnectRequest
        AirApp_LinkEvent,           //ProfileLinkEvent
        AirApp_DataEvent,           //ProfileDataEvent
    },
    AIRAPP_MAX_FRAME_SIZE,
    NULL,           //GivenCreditsEvent
};

static MMI_PROFILE_NODE XDATA gAirApp_ProfileNode = {
    &gAirApp_ProfileDesc.mmiProfileDesc,
    NULL,
};

typedef union
{
    U8 resultCode;
    U8 batStatus;
    U8 isVPOn;
    U8 VPLang;
    U8 PEQMode;
    U8 PEQSectorMode;
    U8 vol;
    U8 isEnterIncoming;
    U8 dataBegin;
    U8 followerData[1];
    #ifdef BLE_AIRApp
    AIRAPP_BLE_REPORT_STATUS  bleReportStatus;
    #endif
    HCI_VCMD_PARA_AIRAPP_CMD_GET_SYNC_DATA      getSyncDataCmd;         //(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_GET_SYNCHRONIZE_DATA)
    HCI_VCMD_PARA_AIRAPP_CMD_GET_DEV_NAME       getDevNameCmd;          //(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_GET_DEVICE_NAME)
    HCI_VCMD_PARA_AIRAPP_CMD_GET_BAT            getBatCmd;              //(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_GET_BATTERY)
    HCI_VCMD_PARA_AIRAPP_CMD_GET_CHGBAT_STATUS  getChgBatStatusCmd;     //(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_GET_CHGBAT_STATUS)
    HCI_VCMD_PARA_AIRAPP_CMD_GET_PEQ            getPEQCmd;              //(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_GET_PEQ)
    HCI_VCMD_PARA_AIRAPP_CMD_GET_VP             getVPCmd;               //(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_GET_VOICE_PROMPT)
    HCI_VCMD_PARA_AIRAPP_CMD_GET_VOL            getVolCmd;              //(HCI_VENDOR_OGF_AIRAPP_CMD, AIRAPP_CMD_GET_VOLUME)
    HCI_VEVT_PARA_AIRAPP_CUSTOMER_RSP           getAirAppCstRsp;        //HCI_VENDOR_OGF_AIRAPP_CUSTOMER_RSP
    HCI_VEVT_PARA_AIRAPP_MMI_KEY_CODE_RSP       airMMIKeyCodeRsp;
    HCI_VCMD_PARA_AIRAPP_CMD_CHANNEL_INFORMATION getChannelInfo;
}AirAppParaType;

typedef struct
{
    U8 hciOpCode;
    U8 hciEvtCode;
    U8 hciLength;
    HCI_VCMD_OPCODE_STRU VOpCode;
    AirAppParaType      VPara;
}AirAppMsgType;

typedef struct
{
    U8 hciOpCode;
    U8 hciEvtCode;
    U8 hciLength;
    HCI_VCMD_OPCODE_STRU VOpCode;
    HCI_VCMD_OPCODE_STRU VFollowerOpCode;
    AirAppParaType      VPara;
}AirAppFollowerMsgType;

typedef union
{
    HCI_ACL_CMD_PARA_CALLER_NAME_WRITE_DATA     writeCallerNameCmd;    //(HCI_ACL_OGF_APP_CALLER_NAME, HCI_ACL_OCF_CALLER_NAME_WRITE_DATA)
    HCI_ACL_EVT_PARA_CALLER_NAME_WRITE_DATA     writeCallerNameEvt;    //(HCI_ACL_OGF_APP_CALLER_NAME, HCI_ACL_OCF_CALLER_NAME_WRITE_RESPONSE)
}AirAppDataParaType;

typedef struct
{
    U8 hciOpCode;
    U8 hciOcf;
    U8 hciOgf;
    U8 hciLength;
    HCI_VCMD_OPCODE_STRU VOpCode;
}AirAppDataHCICmdType;

typedef struct
{
    U8 hciOpCode;
    U8 hciOcf;
    U8 hciOgf;
    U8 hciLength;
    HCI_VCMD_OPCODE_STRU VOpCode;
    U8 para[1];
}AirAppDataHCICmdParaType;

typedef struct
{
    U8 hciOpCode;
    U8 hciHandleL;
    U8 hciHandleH;
    U8 hciLengthL;
    U8 hciLengthH;
    HCI_VCMD_OPCODE_STRU VOpCode;
    AirAppDataParaType para;
}AirAppDataHCIDataType;

typedef struct
{
    U8 hciEventCode;
    U8 hciOpCode;
    U8 hciLength;
    HCI_VCMD_OPCODE_STRU VOpCode;
    U8 paraData[1];
}AirAppDataHCIEventType;


extern MmiLineInInfoCtrl XDATA gMMI_LineIn_ctl;

PUBLIC U8 AirApp_GetFlashUpdateLinkIndex(void)
{
    return gAirApp_ctl.flashUpdateLinkIndex;
}

PUBLIC BOOL AirApp_IsDSPLockedByOTA(void)
{
    return (gAirApp_ctl.flashUpdateLinkIndex == 0xFF) ? FALSE:TRUE;
}

PRIVATE void AirApp_SetDSPLockByOTA(U8 linkIndex)
{
    gAirApp_ctl.flashUpdateLinkIndex = linkIndex;
}

PRIVATE void AirApp_SetDSPUnlockByOTA(void)
{
    gAirApp_ctl.flashUpdateLinkIndex = 0xFF;
}

PRIVATE AIRAPP_LINK_INFO XDATA_PTR AirApp_GetLinkInfo(U8 linkIndex)
{
    return &gAirApp_ctl.linkInfo[linkIndex];
}

PRIVATE U8 XDATA_PTR AirApp_GetMemory(U16 vParaLength)
{
    AirAppMsgType XDATA_PTR airAppPtr;
    U8 XDATA_PTR dataSentPtr;

    // 3 byte: hciOpCode + hciEvtCode + hciLength
    if((dataSentPtr = RFCOMM_AllocProfileData(sizeof(HCI_VCMD_OPCODE_STRU) + vParaLength + 3, SCN_AIR_APP)) == (U8 XDATA_PTR)NULL)
        return (U8 XDATA_PTR)NULL;

    airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppPtr->hciOpCode = AIRAPP_HCI_EVENT_PACKET;
    airAppPtr->hciEvtCode = HCI_EVENT_AIROHA_VENDOR;
    airAppPtr->hciLength = sizeof(HCI_VCMD_OPCODE_STRU) + vParaLength;

    return (U8 XDATA_PTR)dataSentPtr;
}

PRIVATE U8 XDATA_PTR AirApp_GetRelayMemory(U16 vParaLength)
{
    AirAppDataHCICmdParaType XDATA_PTR airAppPtr;
    U8 XDATA_PTR dataSentPtr;

    // 3 byte: hciOpCode + hciEvtCode + hciLength
    if((dataSentPtr = RFCOMM_AllocProfileData(sizeof(HCI_VCMD_OPCODE_STRU) + vParaLength + 4, SCN_AIR_APP)) == (U8 XDATA_PTR)NULL)
        return (U8 XDATA_PTR)NULL;

    airAppPtr = (AirAppDataHCICmdParaType XDATA_PTR)(dataSentPtr + *dataSentPtr);

    airAppPtr->hciOpCode = AIRAPP_HCI_CMD_PACKET;
    airAppPtr->hciOcf = 0x00;
    airAppPtr->hciOgf = 0xFC;
    airAppPtr->hciLength = sizeof(HCI_VCMD_OPCODE_STRU) + vParaLength;

    return (U8 XDATA_PTR)dataSentPtr;
}

PRIVATE void AirApp_SendData(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
    if(linkIndex != MMI_EOF && (linkIndex == A2DP_SearchRelayerLink() || linkIndex == A2DP_SearchFollowerLink()))
    {
        RFCOMM_SendProfileData(linkIndex, dataPtr);
        LightDbgPrint("SendApp:%d", linkIndex);

    }
#ifdef BLE_AIRApp
    else if(AirApp_BleIsActivated())
    {
        AirAppMsgType XDATA_PTR airAppRespPtr;
        U8 len;

        airAppRespPtr = (AirAppMsgType XDATA_PTR)(dataPtr + *dataPtr);
        len = airAppRespPtr->hciLength + 3;
        APP_sendTxData((U8 XDATA_PTR)airAppRespPtr, len);

        #if 1
        {
            //LightDbgPrint("AirApp:ble_rsp len:%d",(U8)len);
            LightDbgPrint("%x %x %x %x %x %x", (U8)dataPtr[15], (U8)dataPtr[16], (U8)dataPtr[17], (U8)dataPtr[18], (U8)dataPtr[19], (U8)dataPtr[20]);
            LightDbgPrint("%x %x %x %x %x %x", (U8)dataPtr[21], (U8)dataPtr[22], (U8)dataPtr[23], (U8)dataPtr[24], (U8)dataPtr[25], (U8)dataPtr[26]);
            if (dataPtr[18] == 0xE6)
            LightDbgPrint("%x %x %x %x %x %x", (U8)dataPtr[53], (U8)dataPtr[54], (U8)dataPtr[55], (U8)dataPtr[56], (U8)dataPtr[57], (U8)dataPtr[58]);
        }
        #endif

        OSMEM_Put(dataPtr);
    }
#endif
    else if(linkIndex != MMI_EOF)
    {
        RFCOMM_SendProfileData(linkIndex, dataPtr);
        LightDbgPrint("SendApp:%d", linkIndex);

    }
}

PRIVATE void AirApp_SendRsp(U8 linkIndex, U8 cmdOpcode, U8 vogf, U8 resultCode)
{
    U8 XDATA_PTR dataSentPtr;
    if((dataSentPtr = AirApp_GetMemory(1)) != (U8 XDATA_PTR)NULL)
    {
        AirAppMsgType XDATA_PTR airAppRespPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
        LightDbgPrint("AirApp: send Response:%x,%d,%d",(U8)cmdOpcode,(U8) vogf,(U8)resultCode);
        airAppRespPtr->VOpCode.VCmdOcf = cmdOpcode;
        airAppRespPtr->VOpCode.VCmdOgf = vogf;
        airAppRespPtr->VPara.resultCode = resultCode;
        AirApp_SendData(linkIndex, dataSentPtr);
    }
}

PRIVATE void AirApp_SendResponse(U8 linkIndex, U8 cmdOpcode, U8 resultCode)
{
    AirApp_SendRsp(linkIndex, cmdOpcode, HCI_VENDOR_OGF_AIRAPP_RESP, resultCode);
}

PRIVATE void AirApp_SendFollowerResponse(U8 linkIndex, U8 followerOcf, U8 followerOgf, U8 resultCode)
{
    AirApp_SendRsp(linkIndex, followerOcf, followerOgf, resultCode);
}

PRIVATE U8 XDATA_PTR AirApp_GetDataMemoryToRelayCmd(U8 cmdOpcode, U8 vogf, U16 vParaLength)
{
    U8 XDATA_PTR dataSentPtr;

    AirAppDataHCICmdParaType XDATA_PTR airAppCmdPtr;
    if((dataSentPtr = AirApp_GetRelayMemory(vParaLength)) == (U8 XDATA_PTR)NULL)
        return (U8 XDATA_PTR)NULL;

    airAppCmdPtr = (AirAppDataHCICmdParaType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppCmdPtr->VOpCode.VCmdOcf = cmdOpcode;
    airAppCmdPtr->VOpCode.VCmdOgf = vogf;

    return dataSentPtr;
}

PRIVATE U8 XDATA_PTR AirApp_GetSltInd(U8 cmdOpcode, U8 vogf, U16 vParaLength)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if((dataSentPtr = AirApp_GetMemory(vParaLength)) == (U8 XDATA_PTR)NULL)
        return (U8 XDATA_PTR)NULL;

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VOpCode.VCmdOcf = cmdOpcode;
    airAppIndPtr->VOpCode.VCmdOgf = vogf;

    return dataSentPtr;
}

PRIVATE U8 XDATA_PTR AirApp_GetFollowerSolicitedInd(U8 cmdOpcode, U8 ogf, U16 vParaLength)
{
    return AirApp_GetSltInd(cmdOpcode, ogf, vParaLength);
}

PRIVATE U8 XDATA_PTR AirApp_GetSolicitedInd(U8 cmdOpcode, U16 vParaLength)
{
    return AirApp_GetSltInd(cmdOpcode, HCI_VENDOR_OGF_AIRAPP_IND_SOLICITED, vParaLength);
}

PRIVATE U8 XDATA_PTR AirApp_GetFollowerUnSolicitedInd(U8 cmdOpcode, U8 ogf, U16 vParaLength)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if((dataSentPtr = AirApp_GetMemory(vParaLength)) == (U8 XDATA_PTR)NULL)
        return (U8 XDATA_PTR)NULL;

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VOpCode.VCmdOcf = cmdOpcode;
    airAppIndPtr->VOpCode.VCmdOgf = ogf;

    return dataSentPtr;
}

PRIVATE U8 XDATA_PTR AirApp_GetUnSolicitedInd(U8 cmdOpcode, U16 vParaLength)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if((dataSentPtr = AirApp_GetMemory(vParaLength)) == (U8 XDATA_PTR)NULL)
        return (U8 XDATA_PTR)NULL;

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VOpCode.VCmdOcf = cmdOpcode;
    airAppIndPtr->VOpCode.VCmdOgf = HCI_VENDOR_OGF_AIRAPP_IND_UNSOLICITED;

    return dataSentPtr;
}

PRIVATE BOOL AirApp_KeyEvent(U8 linkIndex, U8 keyEventCode)
{
    if ((keyEventCode = MMI_DRV_CheckKeyValid(keyEventCode)) == KEY_INVALID)
    {
        return FALSE;
    }

    MMI_PushMediaEvent(keyEventCode);
    MMI_DRV_KeyEventHandler(linkIndex, keyEventCode);
    return TRUE;
}

PRIVATE void AirApp_KeyConnectToConnectable(U8 linkIndex)
{
    AirApp_SendResponse(linkIndex,
                    AIRAPP_CMD_KEY_CONNECTED_TO_CONNECTABLE,
                    (AirApp_KeyEvent(gMMI_ctl.serviceInd, KEY_CONNECTED_TO_CONNECTABLE))? HCI_VEVT_OCF_RESULT_CODE_SUCCESS : HCI_VEVT_OCF_RESULT_CODE_FAIL );
}

PRIVATE void AirApp_KeyResetPairedDevice(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    gMMI_ctl.driverCtl.resetLinkType = airCmdPtr->msgBodyPtr.hciVEvt.VPara.resetLinkType;
    AirApp_SendResponse(linkIndex,
                        AIRAPP_CMD_KEY_RESET_PAIRED_DEVICES,
                        (AirApp_KeyEvent(gMMI_ctl.serviceInd, KEY_RESET_PAIRED_DEVICES))? HCI_VEVT_OCF_RESULT_CODE_SUCCESS : HCI_VEVT_OCF_RESULT_CODE_FAIL );
}

PRIVATE void AirApp_KeyVoicePromptOn(U8 linkIndex)
{
    if(!AirApp_KeyEvent(gMMI_ctl.serviceInd, KEY_VOICE_PROMPT_ENABLE))
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_KEY_VOICE_PROMPT_ENABLE, HCI_VEVT_OCF_RESULT_CODE_FAIL );
    }
    else
    {
#ifdef AIR_MODULE
        MMI_AIR_AiroStereo_Sync(AIR_APP_SYNC_SHARE_CMD,AIR_APP_CMD_PARA_VP_ON);
#endif
        (AirApp_GetLinkInfo(linkIndex))->cmdState = AIRAPP_CMD_KEY_VOICE_PROMPT_ENABLE;
    }
}

PRIVATE void AirApp_KeyVoicePromptOff(U8 linkIndex)
{
    if(!AirApp_KeyEvent(gMMI_ctl.serviceInd, KEY_VOICE_PROMPT_DISABLE))
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_KEY_VOICE_PROMPT_DISABLE, HCI_VEVT_OCF_RESULT_CODE_FAIL );
    }
    else
    {
#ifdef AIR_MODULE
        MMI_AIR_AiroStereo_Sync(AIR_APP_SYNC_SHARE_CMD,AIR_APP_CMD_PARA_VP_OFF);
#endif
        (AirApp_GetLinkInfo(linkIndex))->cmdState = AIRAPP_CMD_KEY_VOICE_PROMPT_DISABLE;
    }
}

PRIVATE void AirApp_KeyVoicePromptLangChange(U8 linkIndex)
{
    if(!AirApp_KeyEvent(gMMI_ctl.serviceInd, KEY_VOICE_PROMPT_LANG_CHANGE))
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_KEY_VOICE_PROMPT_LANG_CHANGE, HCI_VEVT_OCF_RESULT_CODE_FAIL );
    }
    else
    {
        (AirApp_GetLinkInfo(linkIndex))->cmdState = AIRAPP_CMD_KEY_VOICE_PROMPT_LANG_CHANGE;
    }
}

PRIVATE void AirApp_KeyPEQModeChange(U8 linkIndex)
{
    if(!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && !AUDIO_COMPONENT_IS_ACTIVE (AUDIO_LINE_IN))
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_KEY_PEQ_MODE_CHANGE, HCI_VEVT_OCF_RESULT_CODE_FAIL );
        return;
    }
    if(!AirApp_KeyEvent(gMMI_ctl.serviceInd, KEY_PEQ_MODE_CHANGE))
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_KEY_PEQ_MODE_CHANGE, HCI_VEVT_OCF_RESULT_CODE_FAIL );
    }
    else
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_KEY_PEQ_MODE_CHANGE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS );
    }
}


PRIVATE void AirApp_FindMyAccessory(U8 linkIndex)
{
    MMI_PushMediaEvent(MEDIA_EVT_KEY_FIND_MY_ACCESSORY);
    (AirApp_GetLinkInfo(linkIndex))->cmdState = AIRAPP_CMD_FIND_MY_ACCESSORY;
}

PRIVATE void AirApp_WriteDeviceNameCmd(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 len = airCmdPtr->paraLen;

    if(len > sizeof(HCI_VCMD_OPCODE_STRU))
    {
        len -= sizeof(HCI_VCMD_OPCODE_STRU);
        if(MMI_SECTOR_WriteDeviceName(&airCmdPtr->msgBodyPtr.hciVEvt.VPara.writeDevNamePara.nameBegin, len))
        {
            AirApp_SendResponse(linkIndex, AIRAPP_CMD_WRITE_DEVICE_NAME, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
            return;
        }
    }
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_WRITE_DEVICE_NAME, HCI_VEVT_OCF_RESULT_CODE_FAIL);
}

PRIVATE void AirApp_GetVolSetCmd(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 volume = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setVolCmd.vol;
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);

    airappLinkInfo->vol = volume;

    #ifdef LINEIN_ENABLE
    if(MMI_DRV_IsAudioActiveDev(MMI_EOF, AUDIO_DEVICE_LINE_IN))
    {
        MMI_LineIn_AbsVolToSoundLevel(volume);
        MMI_LineIn_SendDriverVolCmd();
        #ifdef AIR_MODULE
        MMI_AIR_LineInUartSetVol();
        #endif
        goto SEND_SET_VOL_RESP;
    }
    #endif

    if(MMI_CheckLinkInCallRelatedState(linkIndex))
    {
        MMI_DRV_SCOAbsVolToSoundLevel(linkIndex, volume);
        MMI_DRV_SendDriverVolCmd(CURRENT_SCO_SOUND_LEVEL(linkIndex), CURRENT_SCO_SMALL_STEP(linkIndex), GET_SCO_MIC_GAIN_BY_LEVEL(CURRENT_SCO_SOUND_LEVEL(linkIndex)),
                                    (gMMI_ctl.mmiInfo[linkIndex].linkPara.scoCodecType == SCO_CODEC_MSBC) ? TRUE : FALSE);
        MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_ADJUST_VOLUME);
        goto SEND_SET_VOL_RESP;
    }
    #ifdef AVRCP_Profile
    MMI_AVRCP_AppSetAbsVol(linkIndex, volume);
    #endif

    SEND_SET_VOL_RESP:
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_VOL, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

#include "sector_tool_misc.h"
PRIVATE void AirApp_GetSyncDataCmd(U8 linkIndex)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;
    SECTOR_AE_INFO_STRU CODE_PTR pAeInfo;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_SYNCHRONIZE_DATA, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_SYNC_DATA))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_SYNCHRONIZE_DATA, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_SYNCHRONIZE_DATA, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    pAeInfo = (SECTOR_AE_INFO_STRU CODE_PTR)GET_SECTOR_TOOLMISC_ADDR(SECTOR_AE_INFO);
    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VPara.getSyncDataCmd.majorVersion = pAeInfo->AeInfo.verInfo.majorVersion;
    airAppIndPtr->VPara.getSyncDataCmd.minorVersion = pAeInfo->AeInfo.verInfo.minorVersion;
    airAppIndPtr->VPara.getSyncDataCmd.buildNumber = pAeInfo->AeInfo.verInfo.buildNumber;
    airAppIndPtr->VPara.getSyncDataCmd.revisionNumber = pAeInfo->AeInfo.verInfo.revisionNumber;

    AirApp_SendData(linkIndex, dataSentPtr);
}

PRIVATE void AirApp_GetDevNameCmd(U8 linkIndex)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_DEVICE_NAME, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_DEV_NAME))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_DEVICE_NAME, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_DEVICE_NAME, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    OSMEM_memcpy_xdata_xdata(   (U8 XDATA_PTR)&airAppIndPtr->VPara.getDevNameCmd.local_name,
                                (U8 XDATA_PTR)&gMMI_driver_variation_nvram.misc_para.init.local_name,
                                sizeof(BD_NAME));
    AirApp_SendData(linkIndex, dataSentPtr);
}

PRIVATE void AirApp_SetVPLang(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    #ifdef AIR_MODULE
    U8 vplangIndex;
    vplangIndex = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setVpLangCmd.VPLangIndex;
    #endif

    if(airCmdPtr->msgBodyPtr.hciVCmd.VPara.setVpLangCmd.VPLangIndex < gMMI_nvram.vpConfig.LangCount && !MMI_VOICE_PROMPT_FUNC_OFF_FEAT)
    {
        gMMI_driver_variation_nvram.misc_para.init.vpLangIndex = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setVpLangCmd.VPLangIndex;
        MMI_DRV_LoadVoiceLangCtl(gMMI_driver_variation_nvram.misc_para.init.vpLangIndex);
        MMI_PushMediaEvent(MEDIA_EVT_VOICE_PROMPT_LANG_CHANGE);
        MMI_SECTOR_SetUpdateFlag(TRUE);

    #ifdef AIR_MODULE
        MMI_AIR_AiroStereo_Sync(AIR_APP_SYNC_SHARE_VP_LANG_INDEX_AND_CHANG_NEXT,vplangIndex);
    #endif

        (AirApp_GetLinkInfo(linkIndex))->cmdState = AIRAPP_CMD_SET_VP_LANG;
        return;
    }
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_VP_LANG, HCI_VEVT_OCF_RESULT_CODE_FAIL);
}

PRIVATE void AirApp_DSPSuspended_Event(void)
{
    AirApp_SendResponse(gMMI_ctl.serviceInd, AIRAPP_CMD_SUSPEND_DSP, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void AirApp_SetDSPSuspend(U8 linkIndex)
{
    MMI_DRV_SuspendDSP(DRV_SUSPEND_DSP_REASON_AIRAPP, AirApp_DSPSuspended_Event);
    UNUSED(linkIndex);
}

PRIVATE void AirApp_SetDSPResume(U8 linkIndex)
{
    MMI_DRV_ResumeDSP(DRV_SUSPEND_DSP_REASON_AIRAPP);
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_RESUME_DSP, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

PRIVATE void AirApp_SetPEQA2DP(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 sectorMode = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQA2DPCmd.sectorMode;

    if(sectorMode != PEQ_GetSectorMode(PEQ_A2DP))
    {
        PEQ_SetSectorMode(PEQ_A2DP, sectorMode);
        PEQ_SetPeqHpfLoadSectorAddress(sectorMode);
        if(airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQA2DPCmd.PEQMode > PEQ_GetModeNumber(PEQ_A2DP))
        {
            sectorMode = (sectorMode + 1) % PEQ_LOAD_SECTOR_NUM;
            PEQ_SetSectorMode(PEQ_A2DP, sectorMode);
            PEQ_SetPeqHpfLoadSectorAddress(sectorMode);
            goto SEND_FAIL_PEQ_A2DP;
        }
    }

    if(airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQA2DPCmd.PEQMode <= PEQ_GetModeNumber(PEQ_A2DP) /*&& AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP)*/)
    {
        if(MMI_MUSIC_FORCE_USE_PEQ_FEAT && airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQA2DPCmd.PEQMode == 0)
            goto SEND_FAIL_PEQ_A2DP;

        PEQ_SetModeIndex(PEQ_A2DP, airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQA2DPCmd.PEQMode);
        if(MMI_REMEMBER_PEQ_CHANGE_FEAT && !MMI_DEFAULT_PEQ_ASSIGNMENT_FEAT)
        {
            gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_A2DP] = PEQ_GetModeIndex(PEQ_A2DP);
            gMMI_driver_variation_nvram.misc_para.PEQSectorMode[PEQ_A2DP] = sectorMode;
            MMI_SECTOR_SetUpdateFlag(TRUE);
        }

        DRIVER_PEQSetMuteTimer();
        DRIVER_PEQReload();

        AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_PEQ_A2DP, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
        return;
    }
    SEND_FAIL_PEQ_A2DP:
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_PEQ_A2DP, HCI_VEVT_OCF_RESULT_CODE_FAIL);
}

PRIVATE void AirApp_SetPEQAUX(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 sectorMode = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQAUXCmd.sectorMode;

    if(sectorMode != PEQ_GetSectorMode(PEQ_LINE_IN))
    {
        PEQ_SetSectorMode(PEQ_LINE_IN, sectorMode);
        PEQ_SetPeqHpfLoadSectorAddress(sectorMode);
        if(airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQAUXCmd.PEQMode > PEQ_GetModeNumber(PEQ_LINE_IN))
        {
            sectorMode = (sectorMode + 1) % PEQ_LOAD_SECTOR_NUM;
            PEQ_SetSectorMode(PEQ_LINE_IN, sectorMode);
            PEQ_SetPeqHpfLoadSectorAddress(sectorMode);
            goto SEND_FAIL_PEQ_AUX;
        }
    }

    if(airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQAUXCmd.PEQMode <= PEQ_GetModeNumber(PEQ_LINE_IN) /*&& AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN)*/)
    {
        if(MMI_MUSIC_FORCE_USE_PEQ_FEAT &&  airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQAUXCmd.PEQMode == 0)
            goto SEND_FAIL_PEQ_AUX;

        PEQ_SetModeIndex(PEQ_LINE_IN, airCmdPtr->msgBodyPtr.hciVCmd.VPara.setPEQAUXCmd.PEQMode);
        if(MMI_REMEMBER_PEQ_CHANGE_FEAT && !MMI_DEFAULT_PEQ_ASSIGNMENT_FEAT)
        {
            gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_LINE_IN] =  PEQ_GetModeIndex(PEQ_LINE_IN);
            gMMI_driver_variation_nvram.misc_para.PEQSectorMode[PEQ_LINE_IN] = sectorMode;
            MMI_SECTOR_SetUpdateFlag(TRUE);
        }

        DRIVER_PEQSetMuteTimer();
        DRIVER_PEQReload();

        AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_PEQ_AUX, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
        return;
    }
    SEND_FAIL_PEQ_AUX:
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_PEQ_AUX, HCI_VEVT_OCF_RESULT_CODE_FAIL);
}

PRIVATE void AirApp_GetChannelInformationCmd(U8 linkIndex)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_CHANNEL_INFORMATION, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_BAT))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_CHANNEL_INFORMATION, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_CHANNEL_INFORMATION, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VPara.getChannelInfo.channelInfo = gMMI_nvram.twsKChannel;

    LightDbgPrint("App ChannelInfo:%d",(U8)airAppIndPtr->VPara.getChannelInfo.channelInfo);
    AirApp_SendData(linkIndex, dataSentPtr);
}


PRIVATE void AirApp_GetBatCmd(U8 linkIndex)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;


    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_BATTERY, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_BAT))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_BATTERY, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_BATTERY, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VPara.getBatCmd.BatteryStatus = MMI_BAT_GetLevelInPercent();


    LightDbgPrint("AirAppBat:%d",(U8)airAppIndPtr->VPara.getBatCmd.BatteryStatus);
    AirApp_SendData(linkIndex, dataSentPtr);
}

PRIVATE void AirApp_GetBatCmdRightChannel(U8 linkIndex)
{
    if( MAX_MULTI_POINT_NO == A2DP_SearchRelayerLink())
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_BATTERY_RIGHT_CHANNEL, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_BATTERY_RIGHT_CHANNEL, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
    #ifdef AIR_MODULE
    MMI_AIR_AirAppGetFollowerInfo(AIR_APP_GET_INFO_FROM_FOLLOWER, AIR_APP_CMD_PARA_BAT_STATUS_FROM_FOLLOWER);
    #endif
}

PRIVATE void AirApp_ProcessFollowerCmd(U8 linkIndex, U8 replaceOgf, MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 XDATA_PTR dataSentPtr;
    U8 targetLink = A2DP_SearchRelayerLink();
    U8 oldOgf = (replaceOgf == HCI_VENDOR_OGF_AIRAPP_CMD) ? HCI_VENDOR_OGF_AIRAPP_FOLLOWER_RESP : HCI_VENDOR_OGF_AIRAPP_FOLLOWER_RESP_EX;
    U8 ocf = airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf;
    AirAppDataHCICmdParaType XDATA_PTR airAppCmdPtr;
    U8 paraLength;

    if(MAX_MULTI_POINT_NO == targetLink || !MMI_IsProfileConnected(targetLink,PROFILE_AIR_APP))
    {
        AirApp_SendFollowerResponse(linkIndex, ocf, oldOgf, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    paraLength = airCmdPtr->paraLen - sizeof(HCI_VCMD_OPCODE_STRU);
    dataSentPtr = (U8 XDATA_PTR)AirApp_GetDataMemoryToRelayCmd(ocf, replaceOgf, paraLength);
    if(dataSentPtr == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendFollowerResponse(linkIndex, ocf, oldOgf, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    airAppCmdPtr = (AirAppDataHCICmdParaType XDATA_PTR)(dataSentPtr + *dataSentPtr);

    if(paraLength)
    {
        OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&airAppCmdPtr->para[0],
                                (U8 XDATA_PTR)&airCmdPtr->msgBodyPtr.hciVEvt.VPara.followerInfo.paraData[0],
                                paraLength);
    }

    AirApp_SendData(targetLink, dataSentPtr);

    LightDbgPrint("follower ocf %d, ogf %d", (U8)ocf, (U8)replaceOgf);

}

PUBLIC void Airapp_ReportBatValueRightChannel(U8 linkIndex, U8 batValue)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_BATTERY_RIGHT_CHANNEL, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_BAT))) == (U8 XDATA_PTR)NULL)
        return;

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VPara.getBatCmd.BatteryStatus = batValue;
    LightDbgPrint("AirAppBatRight:%d",(U8)airAppIndPtr->VPara.getBatCmd.BatteryStatus);
    AirApp_SendData(linkIndex, dataSentPtr);
}

PUBLIC void AirApp_ChgBatStatusUpdateEvent(U8 batStatus)
{
    U8 i;

    for(i = 0; i < MAX_MULTI_POINT_NO; i++)
    {
        if(MMI_IsNormalLink(i))
        {
            U8 XDATA_PTR dataSentPtr;
            AirAppMsgType XDATA_PTR airAppIndPtr;

            if(MMI_IsProfileConnected(i, PROFILE_AIR_APP))
            {
                if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_CHGBAT_STATUS, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_CHGBAT_STATUS))) != (U8 XDATA_PTR)NULL)
                {
                    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
                    airAppIndPtr->VPara.getChgBatStatusCmd.BatChargeStatus = batStatus;
                    LightDbgPrint("AirAppChgBatStatusEvt:%d",(U8)batStatus);
                    AirApp_SendData(i, dataSentPtr);
                }
            }
        }
        else if(A2DP_IsLinkFollower(i))
        {
            MMI_AVRCP_SendVendorUnique(i, AVRCP_VENDOR_UNIQ_AIRAPP_REPROT_CHGBAT_STATUS_TO_RELAYER, batStatus);
        }
    }
}

PRIVATE void AirApp_GetChgBatStatusCmd(U8 linkIndex)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;


    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_CHGBAT_STATUS, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_CHGBAT_STATUS))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_CHGBAT_STATUS, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_CHGBAT_STATUS, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VPara.getChgBatStatusCmd.BatChargeStatus = MMI_CHGBAT_GetBatStatus();
    LightDbgPrint("AirAppChgBatStatus:%d",(U8)airAppIndPtr->VPara.getChgBatStatusCmd.BatChargeStatus);
    AirApp_SendData(linkIndex, dataSentPtr);
}

PRIVATE void AirApp_GetChgBatStatusFollower(U8 linkIndex)
{
    if( MAX_MULTI_POINT_NO == A2DP_SearchRelayerLink())
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_CHGBAT_STATUS_FOLLOWER, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_CHGBAT_STATUS_FOLLOWER, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
    #ifdef AIR_MODULE
    MMI_AIR_AirAppGetFollowerInfo(AIR_APP_GET_CHGBATSTATUS_FROM_FOLLOWER, AIR_APP_CMD_PARA_NONE);
    #endif
}

PUBLIC void Airapp_ReportChgBatStatusFollower(U8 linkIndex, U8 chgBatStatus)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if(!MMI_IsProfileConnected(linkIndex, PROFILE_AIR_APP))
        return;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_CHGBAT_STATUS_FOLLOWER, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_CHGBAT_STATUS))) == (U8 XDATA_PTR)NULL)
        return;

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppIndPtr->VPara.getChgBatStatusCmd.BatChargeStatus = chgBatStatus;
    LightDbgPrint("AirAppChgBatFollower:%d",(U8)airAppIndPtr->VPara.getChgBatStatusCmd.BatChargeStatus);
    AirApp_SendData(linkIndex, dataSentPtr);
}

PRIVATE void AirApp_GetPEQCmd(U8 linkIndex)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_PEQ, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_PEQ))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_PEQ, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_PEQ, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);

#ifdef PEQ_ENABLE
    airAppIndPtr->VPara.getPEQCmd.PEQInA2DP = PEQ_GetModeIndex(PEQ_A2DP);
    airAppIndPtr->VPara.getPEQCmd.PEQInAUX = PEQ_GetModeIndex(PEQ_LINE_IN);
    airAppIndPtr->VPara.getPEQCmd.PEQNumInA2DP = PEQ_GetModeNumber(PEQ_A2DP);
    airAppIndPtr->VPara.getPEQCmd.PEQNumInAUX = PEQ_GetModeNumber(PEQ_LINE_IN);
    airAppIndPtr->VPara.getPEQCmd.isUseDefaultPEQ = (MMI_MUSIC_FORCE_USE_PEQ_FEAT)? TRUE : FALSE;
    airAppIndPtr->VPara.getPEQCmd.A2DPSectorMode = PEQ_GetSectorMode(PEQ_A2DP);
    airAppIndPtr->VPara.getPEQCmd.AUXSectorMode = PEQ_GetSectorMode(PEQ_LINE_IN);
#else
    airAppIndPtr->VPara.getPEQCmd.PEQInA2DP = 0;
    airAppIndPtr->VPara.getPEQCmd.PEQInAUX = 0;
    airAppIndPtr->VPara.getPEQCmd.PEQNumInA2DP = 0;
    airAppIndPtr->VPara.getPEQCmd.PEQNumInAUX = 0;
    airAppIndPtr->VPara.getPEQCmd.isUseDefaultPEQ = 0;
    airAppIndPtr->VPara.getPEQCmd.A2DPSectorMode = 0;
    airAppIndPtr->VPara.getPEQCmd.AUXSectorMode = 0;
#endif
    AirApp_SendData(linkIndex, dataSentPtr);
}

PRIVATE void AirApp_GetVPCmd(U8 linkIndex)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_VOICE_PROMPT, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_VP))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_VOICE_PROMPT, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_VOICE_PROMPT, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);

#ifdef SUPPORT_VOICE_PROMPT
    airAppIndPtr->VPara.getVPCmd.isVPEnabled = MMI_DRV_IsVoicePromptEnabled();
    airAppIndPtr->VPara.getVPCmd.VPLangIndex = gMMI_driver_variation_nvram.misc_para.init.vpLangIndex;
    #if VOICE_LANG_CHI
    airAppIndPtr->VPara.getVPCmd.VPLangName[LANG_CHI] = LANG_CHINESE_T;
    #endif
    #if VOICE_LANG_ENG
    airAppIndPtr->VPara.getVPCmd.VPLangName[LANG_ENG] = LANG_INTERNATIONAL_ENGLISH;
    #endif
    airAppIndPtr->VPara.getVPCmd.VPLangCount = LANG_COUNT;

#else
    airAppIndPtr->VPara.getVPCmd.isVPEnabled = FALSE;
    airAppIndPtr->VPara.getVPCmd.VPLangIndex = 0;
    airAppIndPtr->VPara.getVPCmd.VPLangCount = 0;
#endif
    AirApp_SendData(linkIndex, dataSentPtr);
}


PRIVATE void AirApp_GetVolCmd(U8 linkIndex)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_GET_VOLUME, sizeof(HCI_VCMD_PARA_AIRAPP_CMD_GET_VOL))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_VOLUME, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_VOLUME, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);

    #ifdef AVRCP_Profile
    #ifdef LINEIN_ENABLE
    if(MMI_LineIn_IsDSPOpen())
    {
        airAppIndPtr->VPara.getVolCmd.currentAbsVol = MMI_LineIn_GetAbsVolByLineIn();
    }
    else
    #endif
    if(MMI_CheckLinkInCallRelatedState(linkIndex))
    {
        airAppIndPtr->VPara.getVolCmd.currentAbsVol = MMI_AVRCP_GetAbsVolBySCO(linkIndex);
    }
    else
    {
        airAppIndPtr->VPara.getVolCmd.currentAbsVol = MMI_AVRCP_GetAbsVolByMusic(linkIndex);
    }
    #endif
    airappLinkInfo = AirApp_GetLinkInfo(linkIndex);
    airappLinkInfo->vol = airAppIndPtr->VPara.getVolCmd.currentAbsVol;

    AirApp_SendData(linkIndex, dataSentPtr);
}

#ifdef LIGHTING_ENABLE
PRIVATE void AirApp_EnableMusicYouCanSee(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    gAirAppLedCtl.MusicYouCanSeeEnable_ready = airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin;
    if(!(MMI_AIR_ChangeLightModeNotify(AIRAPP_CMD_ON_OFF_MUSIC_YOU_CAN_SEE, gAirAppLedCtl.MusicYouCanSeeEnable_ready ,0 ,0)))
    {
        MusicYouCanSeeEnable = airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin;
    }
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_ON_OFF_MUSIC_YOU_CAN_SEE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}
PRIVATE void AirApp_GetColorPickerRGB(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    gAirAppLedCtl.LedR_ready = airCmdPtr->msgBodyPtr.hciVEvt.VPara.ledRGB.LedR;
    gAirAppLedCtl.LedG_ready = airCmdPtr->msgBodyPtr.hciVEvt.VPara.ledRGB.LedG;
    gAirAppLedCtl.LedB_ready = airCmdPtr->msgBodyPtr.hciVEvt.VPara.ledRGB.LedB;
    if(!(MMI_AIR_ChangeLightModeNotify(AIRAPP_CMD_GET_COLOR_PICKER_RGB,gAirAppLedCtl.LedR_ready,gAirAppLedCtl.LedG_ready,gAirAppLedCtl.LedB_ready)))
    {
        MMI_LED_Set_RGB(
        airCmdPtr->msgBodyPtr.hciVEvt.VPara.ledRGB.LedR,
        airCmdPtr->msgBodyPtr.hciVEvt.VPara.ledRGB.LedG,
        airCmdPtr->msgBodyPtr.hciVEvt.VPara.ledRGB.LedB);
    }
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_COLOR_PICKER_RGB, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}
PRIVATE void AirApp_LightingModeSelect(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    gAirAppLedCtl.LightingMode_ready = airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin;
    if(!(MMI_AIR_ChangeLightModeNotify(AIRAPP_CMD_LIGHTING_MODE_SELECT, gAirAppLedCtl.LightingMode_ready, 0, 0)))
    {
        MMI_LED_LightingMode(airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin);
    }
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_LIGHTING_MODE_SELECT, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}
PRIVATE void AirApp_AlarmClock(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    gAirAppLedCtl.Alarm_clock_OnOff_ready = airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin;
    if(!(MMI_AIR_ChangeLightModeNotify(AIRAPP_CMD_ALARM_CLOCK, gAirAppLedCtl.Alarm_clock_OnOff_ready, 0, 0)))
    {
        MMI_LED_AlarmClock(airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin);
    }
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_ALARM_CLOCK, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}
#endif

PRIVATE void AirApp_SetCallerNameCmd(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    (AirApp_GetLinkInfo(linkIndex))->isCallerNameOn = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setCallerNameCmd.isOn;

    AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_CALLERNAME, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

#ifdef PEQ_ENABLE
PRIVATE void AirApp_SetRealtimePeqHpf(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_REALTIME_PEQ_HPF, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
    MMI_DRV_RealTimeSetPeqHpf(airCmdPtr);
}

PRIVATE void AirApp_SetDefaultPEQMode(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 component = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setDefaultPEQCmd.component;
    U8 PEQMode = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setDefaultPEQCmd.PEQMode;
    U8 PEQSectorMode = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setDefaultPEQCmd.PEQSectorMode;

    if(component == 0xFF)
    {
        MMI_FEATURE_CTL_A2DP &= ~PEQ_DEFAULT_ASSIGNMENT_FEAT;
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_DEFAULT_PEQ_MODE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
        return;
    }

    if(component < PEQ_COMPONENT_NUM && PEQMode <= PEQ_GetModeNumber(component) && PEQSectorMode < PEQ_LOAD_SECTOR_NUM)
    {
        if(MMI_MUSIC_FORCE_USE_PEQ_FEAT && PEQMode == 0)
            goto SEND_FAIL_PEQ_DEFAULT;

        MMI_FEATURE_CTL_A2DP |= PEQ_DEFAULT_ASSIGNMENT_FEAT;
        gMMI_driver_variation_nvram.misc_para.PEQRecord[component] = PEQMode;
        gMMI_driver_variation_nvram.misc_para.PEQSectorMode[component] = PEQSectorMode;
        MMI_SECTOR_SetUpdateFlag(TRUE);

        AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_DEFAULT_PEQ_MODE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
        return;
    }

    SEND_FAIL_PEQ_DEFAULT:
    AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_DEFAULT_PEQ_MODE, HCI_VEVT_OCF_RESULT_CODE_FAIL);
}
#endif

#ifdef BLE_AIRApp
PRIVATE void AirApp_BleSetEdrTargetIdx(MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 targetLink = airCmdPtr->msgBodyPtr.hciVCmd.VPara.dataBegin;

    if ((targetLink < MAX_MULTI_POINT_NO) && MMI_IsNormalLink(targetLink))
    {
        AirApp_SendResponse(gAirApp_ctl.targetIdx, AIRAPP_CMD_SET_TARGET_LINK_INDEX, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

        if (gAirApp_ctl.targetIdx != targetLink)
        {
            gAirApp_ctl.targetIdx = targetLink;
            AirApp_BleReportEdrLinkStatus();
        }
    }
    else
    {
        AirApp_SendResponse(gAirApp_ctl.targetIdx, AIRAPP_CMD_SET_TARGET_LINK_INDEX, HCI_VEVT_OCF_RESULT_CODE_FAIL);
    }
}
PRIVATE void AirApp_BleSetLeConnInterval(MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 result = MMI_LE_SendConnUpdateReq(&airCmdPtr->msgBodyPtr.hciVCmd.VPara.dataBegin, 0);

    if ((result == MMI_LE_RESULT_SUCCESS) || (result == MMI_LE_RESULT_THE_SAME_DATA))
    {
        AirApp_SendResponse(gAirApp_ctl.targetIdx, AIRAPP_CMD_SET_LE_CONN_INTERVAL, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
    }
    else
    {
        AirApp_SendResponse(gAirApp_ctl.targetIdx, AIRAPP_CMD_SET_LE_CONN_INTERVAL, HCI_VEVT_OCF_RESULT_CODE_FAIL);
    }
}

PRIVATE void AirApp_BleGetLinkStatus(void)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppPtr;

    LightDbgPrint("AirApp_BleGetLinkStatus");

    if((dataSentPtr = AirApp_GetSltInd(AIRAPP_CMD_GET_LINK_STATUS, HCI_VENDOR_OGF_AIRAPP_IND_SOLICITED_EX, sizeof(AIRAPP_BLE_REPORT_STATUS))) == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendRsp(gAirApp_ctl.targetIdx, AIRAPP_CMD_GET_LINK_STATUS, HCI_VENDOR_OGF_AIRAPP_RESP_EX, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    AirApp_SendRsp(gAirApp_ctl.targetIdx, AIRAPP_CMD_GET_LINK_STATUS, HCI_VENDOR_OGF_AIRAPP_RESP_EX, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);

    airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);

    AirApp_BleGetEdrLinkStatus(&airAppPtr->VPara.bleReportStatus);

    AirApp_SendData(gAirApp_ctl.targetIdx, dataSentPtr);
}
#endif

PRIVATE void AirApp_MuteSpeaker(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    gAlignDSP.DSPOUT_MUTE = (airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin)?TRUE:FALSE;
    MMI_DRV_SetSpkMuteByLink((airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin)?SET_MUTE_ON : SET_MUTE_OFF, linkIndex);
    LightDbgPrint("AirApp_MuteSpeaker:%d,%d",(U8)gAlignDSP.DSPOUT_MUTE, (U8)airCmdPtr->msgBodyPtr.hciVEvt.VPara.dataBegin);
    AirApp_SendRsp(linkIndex, AIRAPP_CMD_MUTE_SPK, HCI_VENDOR_OGF_AIRAPP_RESP_EX, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
}

#if AIRAPP_CALLER_NAME
PUBLIC void AirApp_DSPSuspended(void)
{
    gAirApp_ctl.callNameCtl.dspSuspendState = DSP_SUSPENDED_STATE;
}

PRIVATE BOOL AirApp_DisableAndSuspendDSP(void)
{
    if(gAirApp_ctl.callNameCtl.dspSuspendState == DSP_UNKNOW_STATE)
    {
        MMI_DRV_SuspendDSP(DRV_SUSPEND_DSP_REASON_AIRAPP, AirApp_DSPSuspended);
        gAirApp_ctl.callNameCtl.dspSuspendState = DSP_SUSPENDING_STATE;
    }
    SYS_SetTimer(&gAirApp_ctl.callNameCtl.suspendDSPTimer, 8*ONE_SEC);

    return (gAirApp_ctl.callNameCtl.dspSuspendState == DSP_SUSPENDED_STATE)? TRUE : FALSE;
}

PRIVATE void AirApp_ResumeDSP(void)
{
    if(gAirApp_ctl.callNameCtl.dspSuspendState != DSP_UNKNOW_STATE)
    {
        SYS_ReleaseTimer(&gAirApp_ctl.callNameCtl.suspendDSPTimer);
        gAirApp_ctl.callNameCtl.dspSuspendState = DSP_UNKNOW_STATE;
        MMI_DRV_ResumeDSP(DRV_SUSPEND_DSP_REASON_AIRAPP);
    }
}

PRIVATE BOOL AirApp_IsCallerNameSectorValid(void)
{
    SECTOR_APP_CALLERNAME_DATA_STRU CODE_PTR ptr1 = (SECTOR_APP_CALLERNAME_DATA_STRU CODE_PTR)GET_SECTOR_RUNTIME_ADDR(SECTOR_APP_CALLERNAME_DATA);
    if(ptr1 && gAirApp_ctl.callNameCtl.dataValid == 0xFF)
        return TRUE;

    return FALSE;
}

PRIVATE BOOL AirApp_IsMMIAllowWrite(U8 linkIndex)
{
    U8 nextLink = MMI_GetNextNormalLink(linkIndex);
    if(nextLink != MMI_EOF)
    {
        if(MMI_CheckLinkInCallNotifyState(nextLink))
        {
            return FALSE;
        }
        if(MMI_CheckLinkInCallActiveState(nextLink) && SCO_EXISTS(nextLink))
        {
            return FALSE;
        }
    }
    else
    {
        if(MMI_CheckLinkInCallActiveState(linkIndex) && SCO_EXISTS(linkIndex))
        {
            return FALSE;
        }
        if(MMI_CheckLinkInOutGoingState(linkIndex))
        {
            return FALSE;
        }
    }
    return TRUE;
}

PRIVATE void AirApp_EraseCallerNameSector(void)
{
    U8 CODE_PTR ptr1;
    U8 gMMI_rf_rxon_time;

    if(gAirApp_ctl.callNameCtl.callerNameLinkIndex == MMI_EOF || !gSYS_allow_updating)
        return;

    gAirApp_ctl.callNameCtl.callerNameLinkIndex = MMI_EOF;
    gAirApp_ctl.callNameCtl.callerNameNextId = 0;
    gAirApp_ctl.callNameCtl.dataValid = 0xFF;

    ptr1 = GET_SECTOR_RUNTIME_ADDR(SECTOR_APP_CALLERNAME_DATA);
    //- HW workaround: Let RF can't receive packet
    gMMI_rf_rxon_time = MMI_EnterFlashProgramWorkaround();

    FLASH_PROGRAM_UnlockSector(FLASH_MEMORY_MAP(ptr1));
    fFlashDrv->SectorErase(FLASH_MEMORY_MAP(ptr1));
    HC_SFR_WRITE_BY_FB(FBOFFS_WO_MCU_CACHE_FLUSH, 1);

    FLASH_PROGRAM_LockAll();
    MMI_ExitFlashProgramWorkaround(gMMI_rf_rxon_time);

    AirApp_ResumeDSP();

    LightDbgPrint("Erase Caller Name Sector");
}

PRIVATE void AirApp_WriteCallerNameData(U8 XDATA_PTR dataPtr, U16 length, U16 offset)
{
    U8 gMMI_rf_rxon_time;
    U8 count = 5;
    U8 CODE_PTR ptr1 = (U8 CODE_PTR)GET_SECTOR_RUNTIME_ADDR(SECTOR_APP_CALLERNAME_DATA);

    if(!ptr1)
        return;

    //- HW workaround: Let RF can't receive packet
    gMMI_rf_rxon_time = MMI_EnterFlashProgramWorkaround();
    FLASH_PROGRAM_UnlockSector(FLASH_MEMORY_MAP(ptr1));

    do
    {
        if(!FLASH_PROGRAM_WriteXDataToFlash(&ptr1[offset], dataPtr, length))
        {
            continue;
        }
        break;
    } while (--count);

    FLASH_PROGRAM_LockAll();
    MMI_ExitFlashProgramWorkaround(gMMI_rf_rxon_time);
}

PRIVATE void AirApp_SetWriteCallerNameDataEnd(void)
{
    gAirApp_ctl.callNameCtl.dataValid = 0;
}

PRIVATE void AirApp_SendCallerNameWriteEvt(U8 linkIndex, U8 status, U8 id)
{
    AirAppDataHCIDataType XDATA_PTR airAppPtr;
    U8 XDATA_PTR dataSentPtr;

    if((dataSentPtr = RFCOMM_AllocProfileData(sizeof(HCI_VCMD_OPCODE_STRU) + sizeof(HCI_ACL_EVT_PARA_CALLER_NAME_WRITE_DATA) + 5, SCN_AIR_APP)) == (U8 XDATA_PTR)NULL)
        return;

    airAppPtr = (AirAppDataHCIDataType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppPtr->hciOpCode = AIRAPP_HCI_DATA_PACKET;
    airAppPtr->hciLengthL = (U8)(sizeof(HCI_VCMD_OPCODE_STRU) + sizeof(HCI_ACL_EVT_PARA_CALLER_NAME_WRITE_DATA));
    airAppPtr->hciLengthH = (U8)(((U16)(sizeof(HCI_VCMD_OPCODE_STRU) + sizeof(HCI_ACL_EVT_PARA_CALLER_NAME_WRITE_DATA))) >> 8);
    airAppPtr->VOpCode.VCmdOcf = HCI_ACL_OCF_CALLER_NAME_WRITE_RESPONSE;
    airAppPtr->VOpCode.VCmdOgf = HCI_ACL_OGF_APP_CALLER_NAME;
    airAppPtr->para.writeCallerNameEvt.status = status;
    airAppPtr->para.writeCallerNameEvt.id = id;
    AirApp_SendData(linkIndex, dataSentPtr);

    LightDbgPrint("AirApp:Evt id:%d,S:%d",(U8)id, (U8)status);
}

PRIVATE BOOL AirApp_CallerNameDataHandler(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
    AirAppDataHCIDataType XDATA_PTR appDataPtr = (AirAppDataHCIDataType XDATA_PTR)&((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody;
    U16 length;
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);
    U8 XDATA_PTR dataAlignPtr;
    U8 XDATA_PTR dataAlignPtr2;

    if(appDataPtr->VOpCode.VCmdOcf == HCI_ACL_OCF_CALLER_NAME_WRITE_DATA)
    {
        if(!airappLinkInfo->isCallerNameOn || !AirApp_IsCallerNameSectorValid() || !AirApp_IsMMIAllowWrite(linkIndex))
        {
            LightDbgPrint("AirApp: AirApp_IsMMIAllowWrite");
            CALLER_NAME_SEND_STOP:
            AirApp_SendCallerNameWriteEvt(linkIndex, CALLER_NAME_WRITE_STOP, appDataPtr->para.writeCallerNameCmd.id);
            return FALSE;
        }
        if(gAirApp_ctl.callNameCtl.callerNameNextId != appDataPtr->para.writeCallerNameCmd.id)
        {
            LightDbgPrint("AirApp:NotExpectID:%d, %d",(U8)appDataPtr->para.writeCallerNameCmd.id, (U8)gAirApp_ctl.callNameCtl.callerNameNextId);
        }
        if(appDataPtr->para.writeCallerNameCmd.id == 0)
        {
            if(gAirApp_ctl.callNameCtl.callerNameNextId != 0)
            {
                LightDbgPrint("AirApp: callerNameNextId!=0");
                goto CALLER_NAME_SEND_STOP;
            }
            gAirApp_ctl.callNameCtl.callerNameLinkIndex = linkIndex;
        }
        else if(gAirApp_ctl.callNameCtl.callerNameLinkIndex != linkIndex)
        {
            LightDbgPrint("AirApp:callerNameLinkIndex");
            goto CALLER_NAME_SEND_STOP;
        }

        if(!AirApp_DisableAndSuspendDSP())
        {
            LightDbgPrint("AirApp:AirApp_DisableAndSuspendDSP");
            OSMQ_PutFront(&airappLinkInfo->OSMQ_AIRAPP_Data_From_Remote, dataPtr);
            return TRUE;
        }

        length = *(U16 XDATA_PTR)&appDataPtr->hciLengthL;
        length = ENDIAN_TRANSFORM_U16(length) - 3;

        if(length % 256)
        {
            goto CALLER_NAME_SEND_STOP;
        }

        if((dataAlignPtr = SYS_MemoryGet(length + 4)) == (U8 XDATA_PTR)NULL)
        {
            if((dataAlignPtr = OSMEM_Get(&gAlignOSMEM.OSMEM2_small_RX))== (U8 XDATA_PTR)NULL)
            {
                LightDbgPrint("AirApp:4 bytes Align Fail");
                OSMQ_PutFront(&airappLinkInfo->OSMQ_AIRAPP_Data_From_Remote, dataPtr);
                return TRUE;
            }
        }

        dataAlignPtr2 = OS_AlignToNext4ByteAddr(dataAlignPtr);
        OSMEM_memcpy_xdata_xdata (dataAlignPtr2, (U8 XDATA_PTR)&appDataPtr->para.writeCallerNameCmd.callerNameDataBegin, length);
        AirApp_WriteCallerNameData(dataAlignPtr2, length, 256 * 2 * appDataPtr->para.writeCallerNameCmd.id);
        OSMEM_Put(dataAlignPtr);
        gAirApp_ctl.callNameCtl.callerNameNextId++;
        AirApp_SendCallerNameWriteEvt(linkIndex, CALLER_NAME_WRITE_OK, appDataPtr->para.writeCallerNameCmd.id);
    }
    else if(appDataPtr->VOpCode.VCmdOcf == HCI_ACL_OCF_CALLER_NAME_ALL_DATA_READY)
    {
        LightDbgPrint("AirApp:CallerNameEnd");
        AirApp_SetWriteCallerNameDataEnd();
        #ifdef HFP_Profile
        MMI_HFP_AirAppNotifyCallerNameReady(linkIndex);
        #endif
        AirApp_ResumeDSP();
    }
    return FALSE;
}
#endif

#include "..\SECTOR\toolmisc\sector_tool_misc.h"
PRIVATE void AirApp_GetSectorInfo(U8 linkIndex, MMIMsgType XDATA_PTR airCmdPtr)
{
    U8 CODE_PTR sectorHead;
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppRespPtr;
    SECTOR_AE_INFO_STRU CODE_PTR pAeInfo;

    pAeInfo = (SECTOR_AE_INFO_STRU CODE_PTR)GET_SECTOR_TOOLMISC_ADDR(SECTOR_AE_INFO);
    sectorHead = (U8 CODE_PTR)pAeInfo;

    if((dataSentPtr = AirApp_GetSolicitedInd(AIRAPP_CMD_PARSING_AE_SECTOR,
        airCmdPtr->msgBodyPtr.hciVEvt.VPara.sectorAEInfo.dataSize))
        == (U8 XDATA_PTR)NULL)
    {
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_PARSING_AE_SECTOR, HCI_VEVT_OCF_RESULT_CODE_FAIL);
        return;
    }

    LightDbgPrint("app Parsing offset %d", (U8)airCmdPtr->msgBodyPtr.hciVEvt.VPara.sectorAEInfo.offset);
    LightDbgPrint("app Parsing size %d", (U8)airCmdPtr->msgBodyPtr.hciVEvt.VPara.sectorAEInfo.dataSize);
    LightDbgPrint("sector length %d ", (U8)GET_SECTOR_TOOLMISC_LEN(SECTOR_AE_INFO));
    if(airCmdPtr->msgBodyPtr.hciVEvt.VPara.sectorAEInfo.offset +
        airCmdPtr->msgBodyPtr.hciVEvt.VPara.sectorAEInfo.dataSize > GET_SECTOR_TOOLMISC_LEN(SECTOR_AE_INFO))
    {
        LightDbgPrint("Airapp:required size is too large");
        AirApp_SendResponse(linkIndex, AIRAPP_CMD_PARSING_AE_SECTOR, HCI_VEVT_OCF_RESULT_CODE_NOT_ENOUGH_DATA);
        return;
    }

    airAppRespPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppRespPtr->VOpCode.VCmdOcf = airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf;
    airAppRespPtr->VOpCode.VCmdOgf = HCI_VENDOR_OGF_AIRAPP_PARSING_SECTOR_INFO;

    OS_cmemcpy((U8 XDATA_PTR)&airAppRespPtr->VPara.dataBegin,
        sectorHead + airCmdPtr->msgBodyPtr.hciVEvt.VPara.sectorAEInfo.offset,
        airCmdPtr->msgBodyPtr.hciVEvt.VPara.sectorAEInfo.dataSize);

    AirApp_SendData(linkIndex, dataSentPtr);

}

PRIVATE U8 AirApp_DecideRespOGF(U8 ogf)
{
    if(ogf == HCI_VENDOR_OGF_AIRAPP_RESP)
    {
        return (U8)HCI_VENDOR_OGF_AIRAPP_FOLLOWER_RESP;
    }
    else if(ogf == HCI_VENDOR_OGF_AIRAPP_RESP_EX)
    {
        return (U8)HCI_VENDOR_OGF_AIRAPP_FOLLOWER_RESP_EX;
    }
    else
        return 0;
}

PRIVATE U8 AirApp_DecideUnsolicitedOGF(U8 ogf)
{
    if(ogf == HCI_VENDOR_OGF_AIRAPP_IND_UNSOLICITED)
    {
        return (U8)HCI_VENDOR_OGF_AIRAPP_FOLLOWER_IND_UNSOLICITED;
    }
    else if(ogf == HCI_VENDOR_OGF_AIRAPP_IND_UNSOLICITED_EX)
    {
        return (U8)HCI_VENDOR_OGF_AIRAPP_FOLLOWER_IND_UNSOLICITED_EX;
    }
    else
        return 0;
}

PRIVATE U8 AirApp_DecideSolicitedOGF(U8 ogf)
{
    if(ogf == HCI_VENDOR_OGF_AIRAPP_IND_SOLICITED)
    {
        return (U8)HCI_VENDOR_OGF_AIRAPP_FOLLOWER_IND_SOLICITED;
    }
    else if(ogf == HCI_VENDOR_OGF_AIRAPP_IND_SOLICITED_EX)
    {
        return (U8)HCI_VENDOR_OGF_AIRAPP_FOLLOWER_IND_SOLICITED_EX;
    }
    else
        return 0;
}

PRIVATE BOOL AirApp_EventHandler(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
    U8 targetLink, followerOcf, indLen;
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppIndPtr;
    AirAppDataHCIEventType XDATA_PTR appEventPtr = (AirAppDataHCIEventType XDATA_PTR)&((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody;


    #ifdef BLE_AIRApp
    if(AirApp_BleIsActivated())
    {
        LightDbgPrint("SendBLE:%d", linkIndex);
        targetLink = MMI_EOF;
        goto SEND_EVT_DATA;
    }
    #endif

    LightDbgPrint("SendEvent:%d", linkIndex);

    if((targetLink = MMI_GetNextNormalLink(linkIndex)) == MMI_EOF || (AirApp_GetLinkInfo(targetLink))->mmiProfileInfo.state != PROFILE_CONNECTED)
        return FALSE;

    SEND_EVT_DATA:

     followerOcf = appEventPtr->VOpCode.VCmdOcf;
     indLen = appEventPtr->hciLength - sizeof(HCI_VCMD_OPCODE_STRU);

    if(appEventPtr->VOpCode.VCmdOgf == HCI_VENDOR_OGF_AIRAPP_RESP)
    {
        LightDbgPrint("AirApp Evt Response");
        AirApp_SendFollowerResponse(targetLink, followerOcf, AirApp_DecideRespOGF(appEventPtr->VOpCode.VCmdOgf), appEventPtr->paraData[0]);
    }
    else if(appEventPtr->VOpCode.VCmdOgf == HCI_VENDOR_OGF_AIRAPP_IND_UNSOLICITED)
    {
        LightDbgPrint("AirApp Evt Unsolicited");

        dataSentPtr = AirApp_GetFollowerUnSolicitedInd(followerOcf, AirApp_DecideUnsolicitedOGF(appEventPtr->VOpCode.VCmdOgf), indLen);
        if(dataSentPtr)
        {
            airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);

            if(indLen)
            {
                OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&airAppIndPtr->VPara.followerData[0], (U8 XDATA_PTR)&appEventPtr->paraData[0], indLen);
            }

            AirApp_SendData(targetLink, dataSentPtr);
        }
    }
    else //Solicitied Case
    {
        LightDbgPrint("AirApp Evt Solicited");

        dataSentPtr = AirApp_GetFollowerSolicitedInd(followerOcf, AirApp_DecideSolicitedOGF(appEventPtr->VOpCode.VCmdOgf), indLen);
        if(dataSentPtr)
        {
            airAppIndPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            if(indLen)
            {
                OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&airAppIndPtr->VPara.followerData[0], (U8 XDATA_PTR)&appEventPtr->paraData[0], indLen);
            }
            AirApp_SendData(targetLink, dataSentPtr);
        }
    }
    return FALSE;
}

PRIVATE BOOL AirApp_CmdHandler(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
    #ifdef AIR_MODULE
    U8 airlink;
    #endif
    MMIMsgType XDATA_PTR airCmdPtr;
    AirAppDataHCICmdType XDATA_PTR appDataPtr = (AirAppDataHCICmdType XDATA_PTR)&((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody;
    U8 length = appDataPtr->hciLength;

    airCmdPtr = (MMIMsgType XDATA_PTR)dataPtr;
    airCmdPtr->dataOffset = CMD_OFFSET;
    airCmdPtr->msgOpcode = HCI_EVENT_AIROHA_VENDOR;
    airCmdPtr->paraLen = length;

    LightDbgPrint("App:%x,%x,%x,%x",(U8)((U8 XDATA_PTR)appDataPtr)[0],(U8)((U8 XDATA_PTR)appDataPtr)[1],(U8)((U8 XDATA_PTR)appDataPtr)[2],(U8)((U8 XDATA_PTR)appDataPtr)[3]);
    LightDbgPrint("App:%x,%x,%x,%x",(U8)((U8 XDATA_PTR)appDataPtr)[4],(U8)((U8 XDATA_PTR)appDataPtr)[5],(U8)((U8 XDATA_PTR)appDataPtr)[6],(U8)((U8 XDATA_PTR)appDataPtr)[7]);
    OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&airCmdPtr->msgBodyPtr.hciVEvt, (U8 XDATA_PTR)&appDataPtr->VOpCode, length);
    LightDbgPrint("AirApp cmd:%x,%x",(U8)airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf,(U8) airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOgf);
    gSYS_update_by_uart_flash_command = FALSE;

	if(linkIndex == AirApp_GetFlashUpdateLinkIndex() || MMI_HCI_GetFlashUpdateState()!=FLASH_UPDATE_OFF)
    {
        AirApp_SetDSPUnlockByOTA();
        MMI_DRV_FlashUpdateEnd();
        MMI_HCI_SetFlashUpdateState(FLASH_UPDATE_OFF);
    }

    switch(airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOgf)
    {
        case HCI_VENDOR_OGF_AIRAPP_CMD_EX:
            switch (airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf)
            {
                #ifdef BLE_AIRApp
                case AIRAPP_CMD_GET_LINK_STATUS:
                    AirApp_BleGetLinkStatus();
                    break;
                #endif
                case AIRAPP_CMD_MUTE_SPK:
                    AirApp_MuteSpeaker(linkIndex, airCmdPtr);
                    break;
            }
            break;

        case HCI_VENDOR_OGF_AIRAPP_FOLLOWER_CMD_EX:
            LightDbgPrint("App Follower Ex Command");
            AirApp_ProcessFollowerCmd(linkIndex, HCI_VENDOR_OGF_AIRAPP_CMD_EX, airCmdPtr);
            break;

        case HCI_VENDOR_OGF_AIRAPP_FOLLOWER_CMD:
            LightDbgPrint("App Follower Command");
            AirApp_ProcessFollowerCmd(linkIndex, HCI_VENDOR_OGF_AIRAPP_CMD, airCmdPtr);
            break;

        case HCI_VENDOR_OGF_AIRAPP_CMD:
            switch(airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf)
            {
                case AIRAPP_CMD_KEY_CONNECTED_TO_CONNECTABLE:
                    AirApp_KeyConnectToConnectable(linkIndex);
                    break;
                case AIRAPP_CMD_KEY_RESET_PAIRED_DEVICES:
                    AirApp_KeyResetPairedDevice(linkIndex, airCmdPtr);
                    break;
                case AIRAPP_CMD_KEY_VOICE_PROMPT_ENABLE:
                    AirApp_KeyVoicePromptOn(linkIndex);
                    break;
                case AIRAPP_CMD_SUSPEND_DSP:
                    AirApp_SetDSPSuspend(linkIndex);
                    break;
                case AIRAPP_CMD_RESUME_DSP:
                    AirApp_SetDSPResume(linkIndex);
                    break;
                case AIRAPP_CMD_KEY_VOICE_PROMPT_DISABLE:
                    AirApp_KeyVoicePromptOff(linkIndex);
                    break;
                case AIRAPP_CMD_KEY_VOICE_PROMPT_LANG_CHANGE:
                    AirApp_KeyVoicePromptLangChange(linkIndex);
                    break;
                case AIRAPP_CMD_KEY_PEQ_MODE_CHANGE:
                    AirApp_KeyPEQModeChange(linkIndex);
                    break;
                case AIRAPP_CMD_FIND_MY_ACCESSORY:
                    AirApp_FindMyAccessory(linkIndex);
                    break;
                case AIRAPP_CMD_WRITE_DEVICE_NAME:
                    AirApp_WriteDeviceNameCmd(linkIndex, airCmdPtr);
                    break;
                case AIRAPP_CMD_SET_VOL:
                    AirApp_GetVolSetCmd(linkIndex, airCmdPtr);
                    break;
                case AIRAPP_CMD_GET_SYNCHRONIZE_DATA:
                    AirApp_GetSyncDataCmd(linkIndex);
                    break;
                case AIRAPP_CMD_GET_DEVICE_NAME:
                    AirApp_GetDevNameCmd(linkIndex);
                    break;
                case AIRAPP_CMD_SET_VP_LANG:
                    AirApp_SetVPLang(linkIndex, airCmdPtr);
                    break;
                case AIRAPP_CMD_CHANNEL_INFORMATION:
                    AirApp_GetChannelInformationCmd(linkIndex);
                    break;
                case AIRAPP_CMD_SET_PEQ_A2DP:
                    AirApp_SetPEQA2DP(linkIndex, airCmdPtr);
                    break;
                case AIRAPP_CMD_SET_PEQ_AUX:
                    AirApp_SetPEQAUX(linkIndex, airCmdPtr);
                    break;
                #ifdef PEQ_ENABLE
                case AIRAPP_CMD_SET_REALTIME_PEQ_HPF:
                    AirApp_SetRealtimePeqHpf(linkIndex, airCmdPtr);
                    return TRUE;
                case AIRAPP_CMD_SET_DEFAULT_PEQ_MODE:
                    AirApp_SetDefaultPEQMode(linkIndex, airCmdPtr);
                    break;
                #endif
                case AIRAPP_CMD_GET_BATTERY:
                    AirApp_GetBatCmd(linkIndex);
                    break;
                case AIRAPP_CMD_GET_CHGBAT_STATUS:
                    AirApp_GetChgBatStatusCmd(linkIndex);
                    break;
                case AIRAPP_CMD_GET_CHGBAT_STATUS_FOLLOWER:
                    AirApp_GetChgBatStatusFollower(linkIndex);
                    break;
                case AIRAPP_CMD_GET_PEQ:
                    AirApp_GetPEQCmd(linkIndex);
                    break;
                case AIRAPP_CMD_GET_VOICE_PROMPT:
                    AirApp_GetVPCmd(linkIndex);
                    break;
                case AIRAPP_CMD_GET_VOLUME:
                    AirApp_GetVolCmd(linkIndex);
                    break;
                case AIRAPP_CMD_GET_BATTERY_RIGHT_CHANNEL:
                    AirApp_GetBatCmdRightChannel(linkIndex);
                    break;
                #ifdef LIGHTING_ENABLE
                case AIRAPP_CMD_ON_OFF_MUSIC_YOU_CAN_SEE:
                    AirApp_EnableMusicYouCanSee(linkIndex, airCmdPtr);
                    break;
                case AIRAPP_CMD_GET_COLOR_PICKER_RGB:
                    AirApp_GetColorPickerRGB(linkIndex, airCmdPtr);
                    break;
                case AIRAPP_CMD_LIGHTING_MODE_SELECT:
                    AirApp_LightingModeSelect(linkIndex, airCmdPtr);
                    break;
                case AIRAPP_CMD_ALARM_CLOCK:
                    AirApp_AlarmClock(linkIndex, airCmdPtr);
                    break;
                #endif
                case AIRAPP_CMD_SET_CALLERNAME:
                    AirApp_SetCallerNameCmd(linkIndex, airCmdPtr);
                    break;
                #ifdef BLE_AIRApp
                case AIRAPP_CMD_SET_TARGET_LINK_INDEX:
                    AirApp_BleSetEdrTargetIdx(airCmdPtr);
                    break;
                case AIRAPP_CMD_SET_LE_CONN_INTERVAL:
                    AirApp_BleSetLeConnInterval(airCmdPtr);
                    break;
                #endif
                case AIRAPP_CMD_KEY_MMI_KEY_EVENT:
                    switch(airCmdPtr->msgBodyPtr.hciVCmd.VPara.airMMIKeyEventCmd.mmiKeyCode)
                    {
                        case KEY_AUDIO_TRANSPARENCY:
                            #if defined AIR_MODULE && defined AVRCP_Profile
                            if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
                            {
                                MMI_AVRCP_SendVendorUnique(airlink, AVRCP_VENDOR_UNIQ_SYNC_AND_SWITCH_AUDIO_TRANSPARENCY, (gMMI_ctl.isAudioTransparencyEnable? FALSE : TRUE));
                            }
                            #endif
                            MMI_DRV_Switch_AudioTransparency(gMMI_ctl.isAudioTransparencyEnable ? FALSE : TRUE);
                            AirApp_MMIKeyEventRsp(linkIndex, airCmdPtr->msgBodyPtr.hciVCmd.VPara.airMMIKeyEventCmd.mmiKeyCode, gMMI_ctl.isAudioTransparencyEnable);
                            break;
                        case KEY_ROLE_CHANGE:
                            #if defined AIR_MODULE
                            if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && A2DP_IsLinkRelayer(airlink))
                            {
                                MMI_DRV_KeyEventHandler(airlink, KEY_ROLE_CHANGE);
                                AirApp_MMIKeyEventRsp(linkIndex, KEY_ROLE_CHANGE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
                                break;
                            }
                            #endif
                            AirApp_MMIKeyEventRsp(linkIndex, airCmdPtr->msgBodyPtr.hciVCmd.VPara.airMMIKeyEventCmd.mmiKeyCode, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                            break;
                        default:
                            AirApp_MMIKeyEventRsp(linkIndex, airCmdPtr->msgBodyPtr.hciVCmd.VPara.airMMIKeyEventCmd.mmiKeyCode, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                            break;
                    }
                    break;
                case AIRAPP_CMD_SET_MASTER_AT_GAIN_CHANGE:
                    if(airCmdPtr->msgBodyPtr.hciVCmd.VPara.setATGainCmd.audioTsprcySoundLv < gMMI_nvram.totalLineInSoundLevel)
                    {
                        gMMI_LineIn_ctl.currentsoundLevel = airCmdPtr->msgBodyPtr.hciVCmd.VPara.setATGainCmd.audioTsprcySoundLv;
                        MMI_LineIn_SendDriverVolCmd();
                        AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_MASTER_AT_GAIN_CHANGE, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
                    }
                    else
                    {
                        AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_MASTER_AT_GAIN_CHANGE, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                    }
                    break;
                case AIRAPP_CMD_SET_SLAVE_AT_GAIN_CHANGE:
                    #if defined AIR_MODULE && defined AVRCP_Profile
                    if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF && airCmdPtr->msgBodyPtr.hciVCmd.VPara.setATGainCmd.audioTsprcySoundLv < gMMI_nvram.totalLineInSoundLevel)
                    {
                        MMI_AVRCP_SendVendorUnique(airlink, AVRCP_VENDOR_UNIQ_SET_AT_VOL_CMD, airCmdPtr->msgBodyPtr.hciVCmd.VPara.setATGainCmd.audioTsprcySoundLv);
                    }
                    else
                    #endif
                    {
                        AirApp_SendResponse(linkIndex, AIRAPP_CMD_SET_SLAVE_AT_GAIN_CHANGE, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                    }
                    break;
                case AIRAPP_CMD_GET_MASTER_AT_GAIN:
                    if(MMI_GetAudioTransparencyEnable() && gMMI_LineIn_ctl.currentsoundLevel < gMMI_nvram.totalLineInSoundLevel)
                    {
                        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_MASTER_AT_GAIN, gMMI_LineIn_ctl.currentsoundLevel);
                    }
                    else
                    {
                        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_MASTER_AT_GAIN, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                    }
                    break;
                case AIRAPP_CMD_GET_SLAVE_AT_GAIN:
                    #if defined AIR_MODULE && defined AVRCP_Profile
                    if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
                    {
                        MMI_AVRCP_SendVendorUnique(airlink, AVRCP_VENDOR_UNIQ_GET_AT_GAIN_LV, 0);
                    }
                    else
                    #endif
                    {
                        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_SLAVE_AT_GAIN, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                    }
                    break;
                case AIRAPP_CMD_GET_MASTER_AT_STATUS:
                    AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_MASTER_AT_STATUS, gMMI_ctl.isAudioTransparencyEnable ? HCI_VEVT_OCF_RESULT_CODE_SUCCESS : HCI_VEVT_OCF_RESULT_CODE_FAIL);
                    break;
                case AIRAPP_CMD_GET_SLAVE_AT_STATUS:
                    #if defined AIR_MODULE && defined AVRCP_Profile
                    if((airlink = MMI_AIR_GetAirLink(PRODUCT_AIR_TWS)) != MMI_EOF)
                    {
                        MMI_AVRCP_SendVendorUnique(airlink, AVRCP_VENDOR_UNIQ_GET_AT_STATUS, 0);
                    }
                    else
                    #endif
                    {
                        AirApp_SendResponse(linkIndex, AIRAPP_CMD_GET_SLAVE_AT_STATUS, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                    }
                    break;
                default:
                    LightDbgPrint("AirApp cmd Fail:%x",(U8)airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf);
                    AirApp_SendResponse(linkIndex, airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                    break;
            }
            break;
        case HCI_VENDOR_OGF_AIRAPP_CUSTOMER_CMD:
            if(api_profile_func[API_GRP_PF_CB_AIRAPP_CUSTOMER_CTL])
            {
                ((API_PROFILE_AIRAPP_CUSTOMER_CTL_FUNC)api_profile_func[API_GRP_PF_CB_AIRAPP_CUSTOMER_CTL])(linkIndex,(U8 XDATA_PTR) airCmdPtr);
            }
            break;
        case HCI_VENDOR_OGF_AIRAPP_PASSTHROUGH_TRANSFER_CMD:
            if(airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf == AIRAPP_CMD_PASSTHROUGH_FROM_APP_TO_BT)
            {
                AirApp_SendRsp(linkIndex, AIRAPP_RSP_PASSTHROUGH_FROM_APP_TO_BT, HCI_VENDOR_OGF_AIRAPP_PASSTHROUGH_TRANSFER_RSP, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
                MMI_HCI_AirohaCmd_PassThroughData(linkIndex, AIRAPP_RSP_PASSTHROUGH_FROM_BT_TO_UART, HCI_VENDOR_OGF_AIRAPP_PASSTHROUGH_TRANSFER_RSP, airCmdPtr);
            }
            break;
        case HCI_VENDOR_OGF_AIRAPP_PARSING_SECTOR_INFO:
            switch(airCmdPtr->msgBodyPtr.hciVEvt.VOpCode.VCmdOcf)
            {
                case AIRAPP_CMD_PARSING_AE_SECTOR:
                    AirApp_GetSectorInfo(linkIndex, airCmdPtr);
                    break;
            }
            break;
    }
    return FALSE;
}

PUBLIC void AirApp_OTAFail(void)
{
    if(AirApp_IsDSPLockedByOTA())
    {
        LightDbgPrint("Clear OTA Flag");
        MMI_SniffEnable(AirApp_GetFlashUpdateLinkIndex(), MMI_SNIFF_OTA);
        AirApp_SetDSPUnlockByOTA();
    }
    MMI_DRV_SetOTAVoicepromptsExist(FALSE);
}

PRIVATE BOOL AirApp_DataHandler(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
    AirAppDataHCIDataType XDATA_PTR appDataPtr = (AirAppDataHCIDataType XDATA_PTR)&((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody;

    switch (appDataPtr->VOpCode.VCmdOgf)
    {

#ifdef BLE_AIRApp
        case HCI_ACL_OGF_APP_REMOTE_FLASH_BLE:
            LightDbgPrint("@SamAPPin: %x", (U8)appDataPtr->VOpCode.VCmdOgf);
            if(A2DP_SearchRelayerLink() == MAX_MULTI_POINT_NO)
            {
                AirApp_BleSendAclErrorRsp(appDataPtr->VOpCode.VCmdOcf);
                LightDbgPrint("Follower does not exist.");
                return FALSE;
            }
            /* fall through */
        case HCI_ACL_OGF_APP_FLASH_BLE:
#endif

#ifdef AB1520_FLASH_REMOTE_UPDATE
        case HCI_ACL_OGF_APP_REMOTE_FLASH:
#endif

        case HCI_ACL_OGF_APP_FLASH:
            if(AirApp_IsDSPLockedByOTA())
            {
                if(linkIndex != AirApp_GetFlashUpdateLinkIndex())
                {
                    LightDbgPrint("DataHandler fail: %x %x", (U8)gAirApp_ctl.flashUpdateLinkIndex, (U8)linkIndex);
                    return FALSE;
                }
            }
            else
            {
                LightDbgPrint("To Lock DSP");

                AirApp_SetDSPLockByOTA(linkIndex);

                //if(MMI_GetNextNormalLink(linkIndex) != MMI_EOF)
                //  MMI_ReleaseProfileLinkAndDetach(MMI_GetNextNormalLink(linkIndex));

                if(linkIndex != 0xFF) /* SniffDisable when 3.0 normal link exist */
                    MMI_SniffDisable(linkIndex, MMI_SNIFF_OTA);
            }
            *(U16 XDATA_PTR)&appDataPtr->hciHandleL = ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&appDataPtr->hciHandleL);
            *(U16 XDATA_PTR)&appDataPtr->hciLengthL = ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)&appDataPtr->hciLengthL);

            OSMEM_memcpy_xdata_xdata(&((HC_ACL_CMD_STRU XDATA_PTR)dataPtr)->ConnHdl, &appDataPtr->hciHandleL, *(U16 XDATA_PTR)&appDataPtr->hciLengthL + 4/*ConnHdl+Len*/);
            OSMQ_Put(OSMQ_HC_FlashData_From_UART_ptr, dataPtr);

            return TRUE;

#if AIRAPP_CALLER_NAME
        case HCI_ACL_OGF_APP_CALLER_NAME:
            return AirApp_CallerNameDataHandler(linkIndex, dataPtr);
#endif

    }
    return FALSE;
}

#define OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(PTR_A) (OS_CONTAINER_OF(PTR_A,OSMEM_BLK,data_space)->header.mem_ptr)
PRIVATE void AirApp_RemoteDataHandler(U8 linkIndex)
{
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);
    U8 XDATA_PTR dataPtr = OSMQ_Get(&airappLinkInfo->OSMQ_AIRAPP_Data_From_Remote);

    if(!dataPtr)
        return;

    if(airappLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
    {
        switch(airappLinkInfo->dataCtl.state)
        {
            case AIRAPP_DATA_STATE_NO_DATA:
                if(((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody[0] == AIRAPP_HCI_CMD_PACKET)
                {
                    if(AirApp_CmdHandler(linkIndex, dataPtr))
                        return; //bypass OSMEM_Put()
                }
                else if(((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody[0] == AIRAPP_HCI_EVENT_PACKET)
                {
                    if(AirApp_EventHandler(linkIndex, dataPtr))
                        return; //bypass OSMEM_Put();
                }
                else if(((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody[0] == AIRAPP_HCI_DATA_PACKET)
                {
                    AirAppDataHCIDataType XDATA_PTR airDataPtr = (AirAppDataHCIDataType XDATA_PTR)&((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody[0];
                    U16 totalLength = *(U16 XDATA_PTR)&airDataPtr->hciLengthL;
                    totalLength = ENDIAN_TRANSFORM_U16(totalLength) + 5;

                    if(((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength == totalLength)
                    {
                        if (AirApp_DataHandler(linkIndex, dataPtr))
                            return; //bypass OSMEM_Put()
                    }
                    else
                    {
                        totalLength += (sizeof(ProfileDataEvent) -1);
                        if(totalLength > OSMEM2_LARGE_RX_BLKSIZE)
                        {
                            //Do nothing -->Drop packet
                        }
                        else if(totalLength > OSMEM2_SMALL_RX_BLKSIZE)
                        {
                            if(OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(dataPtr) == OSMEM_ptr2_large_RX)
                            {
                                goto SET_DATA_PTR;
                            }
                            else
                            {
                                if(airappLinkInfo->dataCtl.dataPtr = OSMEM_Get(OSMEM_ptr2_large_RX))
                                {
                                    goto NEW_DATA_PKT;
                                }
                            }
                        }
                        else if(totalLength > OSMEM2_TINY_RX_BLKSIZE)
                        {
                            if(OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(dataPtr) == OSMEM_ptr2_large_RX || OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(dataPtr) == OSMEM_ptr2_small_RX)
                            {
                                goto SET_DATA_PTR;
                            }
                            else
                            {
                                if(airappLinkInfo->dataCtl.dataPtr = OSMEM_Get(OSMEM_ptr2_small_RX))
                                {
                                    goto NEW_DATA_PKT;
                                }
                                else if(airappLinkInfo->dataCtl.dataPtr = OSMEM_Get(OSMEM_ptr2_large_RX))
                                {
                                    goto NEW_DATA_PKT;
                                }
                            }
                        }
                        else
                        {
                            if(OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(dataPtr) == OSMEM_ptr2_large_RX || OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(dataPtr) == OSMEM_ptr2_small_RX ||OS_GET_MEM_PTR_FROM_MEMORY_BLOCK(dataPtr) == OSMEM_ptr2_tiny_RX)
                            {
                                SET_DATA_PTR:
                                totalLength -= (sizeof(ProfileDataEvent) -1);
                                airappLinkInfo->dataCtl.dataPtr = dataPtr;
                                airappLinkInfo->dataCtl.state = AIRAPP_DATA_STATE_CONTINUE;
                                airappLinkInfo->dataCtl.offset = ((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength;
                                airappLinkInfo->dataCtl.remainLength = totalLength - ((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength;
                                return;
                            }
                            else
                            {
                                if(airappLinkInfo->dataCtl.dataPtr = OSMEM_Get(OSMEM_ptr2_tiny_RX))
                                {
                                    goto NEW_DATA_PKT;
                                }
                                else if(airappLinkInfo->dataCtl.dataPtr = OSMEM_Get(OSMEM_ptr2_small_RX))
                                {
                                    goto NEW_DATA_PKT;
                                }
                                else if(airappLinkInfo->dataCtl.dataPtr = OSMEM_Get(OSMEM_ptr2_large_RX))
                                {
                                    NEW_DATA_PKT:
                                    OSMEM_memcpy_xdata_xdata(airappLinkInfo->dataCtl.dataPtr, dataPtr, totalLength);
                                    totalLength -= (sizeof(ProfileDataEvent) -1);
                                    airappLinkInfo->dataCtl.state = AIRAPP_DATA_STATE_CONTINUE;
                                    airappLinkInfo->dataCtl.offset = ((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength;
                                    airappLinkInfo->dataCtl.remainLength = totalLength - ((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength;
                                }
                            }
                        }
                    }
                }
                break;
            case AIRAPP_DATA_STATE_CONTINUE:
                {
                    U16 dataLength = ((ProfileDataEvent XDATA_PTR)dataPtr)->dataLength;

                    if(airappLinkInfo->dataCtl.remainLength >= dataLength)
                    {
                        OSMEM_memcpy_xdata_xdata(&(((ProfileDataEvent XDATA_PTR)airappLinkInfo->dataCtl.dataPtr)->dataBody[airappLinkInfo->dataCtl.offset]), &(((ProfileDataEvent XDATA_PTR)dataPtr)->dataBody[0]), dataLength);
                        airappLinkInfo->dataCtl.remainLength -= dataLength;
                        airappLinkInfo->dataCtl.offset += dataLength;
                        ((ProfileDataEvent XDATA_PTR)airappLinkInfo->dataCtl.dataPtr)->dataLength += dataLength;
                    }
                    else
                    {
                        SYS_MemoryRelease(&airappLinkInfo->dataCtl.dataPtr);
                        airappLinkInfo->dataCtl.state = AIRAPP_DATA_STATE_NO_DATA;
                    }

                    if(airappLinkInfo->dataCtl.remainLength == 0)
                    {
                        if (AirApp_DataHandler(linkIndex, airappLinkInfo->dataCtl.dataPtr))
                        {
                            airappLinkInfo->dataCtl.dataPtr = NULL;
                        }
                        else
                        {
                            SYS_MemoryRelease(&airappLinkInfo->dataCtl.dataPtr);
                        }
                        airappLinkInfo->dataCtl.state = AIRAPP_DATA_STATE_NO_DATA;
                    }
                }
                break;
        }
    }
    OSMEM_Put(dataPtr);

}

PRIVATE void AirApp_PollingIndications(U8 linkIndex)
{
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);

    if(airappLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
    {
        AirAppMsgType XDATA_PTR airAppPtr;
        U8 XDATA_PTR dataSentPtr;

        U8 para = MMI_BAT_GetLevelInPercent();
        if(airappLinkInfo->batStatus != para)
        {
            //LightDbgPrint("AirAppBat:%d",(U8)para, (U8)airappLinkInfo->batStatus);
            if(airappLinkInfo->batStatus > para)
            {
                if(airappLinkInfo->batStatus - para < 10)
                    goto SEND_VP_POLLING;
            }
            else
            {
                if(para - airappLinkInfo->batStatus < 10)
                    goto SEND_VP_POLLING;
            }

            if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_BAT_STATUS, 1)) == (U8 XDATA_PTR)NULL)
                return;

            airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            airAppPtr->VPara.batStatus = para;
            AirApp_SendData(linkIndex, dataSentPtr);
            airappLinkInfo->batStatus = para;
            LightDbgPrint("AirApp - report bat:%d",(U8)para);
        }

        SEND_VP_POLLING:
        para = MMI_DRV_IsVoicePromptEnabled();
        if(airappLinkInfo->isVPOn != para)
        {
            if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_VP_STATUS, 1)) == (U8 XDATA_PTR)NULL)
                return;

            airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            airAppPtr->VPara.isVPOn = para;
            AirApp_SendData(linkIndex, dataSentPtr);
            airappLinkInfo->isVPOn = para;
            LightDbgPrint("AirApp - VP:%d",(U8)para);
        }

        para = gMMI_driver_variation_nvram.misc_para.init.vpLangIndex;
        if(airappLinkInfo->VPLang != para)
        {
            if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_VP_LANG, 1)) == (U8 XDATA_PTR)NULL)
                return;

            airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            airAppPtr->VPara.VPLang = para;
            AirApp_SendData(linkIndex, dataSentPtr);
            airappLinkInfo->VPLang = para;
            LightDbgPrint("AirApp - VPLang:%d",(U8)para);
        }

        para = PEQ_GetModeIndex(PEQ_A2DP);
        if(airappLinkInfo->PEQMode[PEQ_A2DP] != para)
        {
            if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_PEQ_A2DP_CHANGE, 1)) == (U8 XDATA_PTR)NULL)
                return;

            airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            airAppPtr->VPara.PEQMode = para;
            AirApp_SendData(linkIndex, dataSentPtr);
            airappLinkInfo->PEQMode[PEQ_A2DP] = para;
            LightDbgPrint("AirApp - PEQ_A2DP:%d",(U8)para);
        }

        para = PEQ_GetModeIndex(PEQ_LINE_IN);
        if(airappLinkInfo->PEQMode[PEQ_LINE_IN] != para)
        {
            if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_PEQ_AUX_CHANGE, 1)) == (U8 XDATA_PTR)NULL)
                return;

            airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            airAppPtr->VPara.PEQMode = para;
            AirApp_SendData(linkIndex, dataSentPtr);
            airappLinkInfo->PEQMode[PEQ_LINE_IN] = para;
            LightDbgPrint("AirApp - PEQ_LINE_IN:%d",(U8)para);
        }

        #ifdef AVRCP_Profile
        #ifdef LINEIN_ENABLE
        if(MMI_LineIn_IsDSPOpen())
        {
            para = MMI_LineIn_GetAbsVolByLineIn();
        }
        else
        #endif
        if(MMI_CheckLinkInCallRelatedState(linkIndex))
        {
            para = MMI_AVRCP_GetAbsVolBySCO(linkIndex);
        }
        else
        {
            para = MMI_AVRCP_GetAbsVolByMusic(linkIndex);
        }
        #endif

        if(airappLinkInfo->vol != para)
        {
            if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_VOL_CHANGE, 1)) == (U8 XDATA_PTR)NULL)
                return;

            airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            airAppPtr->VPara.vol = para;
            AirApp_SendData(linkIndex, dataSentPtr);
            airappLinkInfo->vol = para;
            LightDbgPrint("AirApp - vol:%d",(U8)para);
        }

        para = PEQ_GetSectorMode(PEQ_A2DP);
        if(airappLinkInfo->PEQSectorMode[PEQ_A2DP] != para)
        {
            if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_PEQ_SECTOR_CHANGE, 1)) == (U8 XDATA_PTR)NULL)
                return;

            airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            airAppPtr->VPara.PEQSectorMode = para;
            AirApp_SendData(linkIndex, dataSentPtr);
            airappLinkInfo->PEQSectorMode[PEQ_A2DP] = para;
            LightDbgPrint("AirApp - A2DP Sector Mode:%d",(U8)para);
        }

        para = PEQ_GetSectorMode(PEQ_LINE_IN);
        if(airappLinkInfo->PEQSectorMode[PEQ_LINE_IN] != para)
        {
            if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_PEQ_SECTOR_CHANGE, 1)) == (U8 XDATA_PTR)NULL)
                return;

            airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
            airAppPtr->VPara.PEQSectorMode = para;
            AirApp_SendData(linkIndex, dataSentPtr);
            airappLinkInfo->PEQSectorMode[PEQ_LINE_IN] = para;
            LightDbgPrint("AirApp - LINE IN Sector Mode:%d",(U8)para);
        }
    }
}

PRIVATE void AirApp_CheckTimer(U8 linkindex)
{
#if AIRAPP_CALLER_NAME
    if(SYS_IsTimerExpired(&gAirApp_ctl.callNameCtl.suspendDSPTimer))
    {
        AirApp_ResumeDSP();
    }
#endif
    UNUSED(linkindex);
}

PRIVATE void AirApp(U8 linkIndex)
{
    AirApp_RemoteDataHandler(linkIndex);
    AirApp_PollingIndications(linkIndex);
    AirApp_CheckTimer(linkIndex);
    #if AIRAPP_CALLER_NAME
    if(!MMI_AllLinksInCallRelatedState())
        AirApp_EraseCallerNameSector();
    #endif
}

PRIVATE void AirApp_ClearLinkInfo(U8 linkIndex)
{
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);
    airappLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTED;
    OSMQ_FlushAndReleaseMemoryBlock(&airappLinkInfo->OSMQ_AIRAPP_Data_From_Remote);
    if(linkIndex == gAirApp_ctl.callNameCtl.callerNameLinkIndex)
    {
        AirApp_ResumeDSP();
    }
    if(airappLinkInfo->dataCtl.dataPtr)
        SYS_MemoryRelease(&airappLinkInfo->dataCtl.dataPtr);

    airappLinkInfo->dataCtl.state = 0;
    airappLinkInfo->dataCtl.offset = 0;
}

PRIVATE MMI_PROFILE_INFO XDATA_PTR AirApp_ConnectRequest(U8 linkIndex, U8 profileId)
{
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);

#ifdef BLE_AIRApp
    AirApp_BleResetEdrProfileState(airappLinkInfo);
#endif

    if (airappLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
    {
        if (RFCOMM_ConnectProfile(linkIndex, profileId) == NO_ERROR)
        {
            return &airappLinkInfo->mmiProfileInfo;
        }
    }
    return (MMI_PROFILE_INFO XDATA_PTR)NULL;
}

PRIVATE U8 AirApp_DisconnectRequest(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo)
{
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);

    if (&airappLinkInfo->mmiProfileInfo == profileInfo)
    {
        if (airappLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
        {
            if (CMD_WAITING == RFCOMM_DisconnectProfile(linkIndex, profileInfo->profileDesc->profileId))
            {
                airappLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTING;
                return CMD_WAITING;
            }

            MMI_UnloadProfile(linkIndex, &airappLinkInfo->mmiProfileInfo);
            AirApp_ClearLinkInfo(linkIndex);
        }
    }
    return CMD_COMPLETE;
}

PRIVATE void AirApp_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams)
{
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);

    switch (evtCode)
    {
        case PROFILE_LINK_EVENT_CONN_REQUEST:
            evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT;
            if (airappLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
            {
                if(MMI_IsLinkActiveDisconnecting(linkIndex))
                {
                    evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT_ACTIVE_DISC;
                }
                else
                {
                    #ifdef AIR_MODULE
                    if (!MMI_AIR_IsAirLink(linkIndex))
                    #endif
                    {
                        #ifdef BLE_AIRApp
                        if(AirApp_BleIsActivated())
                        {
                            evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_REJECT_ACTIVE_DISC;
                            break;
                        }
                        else
                        {
                            AirApp_BleSetEnable(FALSE);
                        }
                        #endif
                    }

                    evtParams->connRequest.result = PROFILE_LINK_RESULT_CONN_ACCEPT;
                    evtParams->connRequest.profileInfo = &airappLinkInfo->mmiProfileInfo;

                }
            }
            break;

        case PROFILE_LINK_EVENT_CONN_COMPLETE:
            if (airappLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTING)
            {
                if (evtParams->connComplete.status == NO_ERROR)
                {
                    airappLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;
                    MMI_ActivateProfile(linkIndex, &airappLinkInfo->mmiProfileInfo);
                }
                else
                {
                    if(evtParams->connComplete.status == SECURITY_BLOCK)
                    {
                        MMI_SetReconnectProfile(linkIndex, airappLinkInfo->mmiProfileInfo.profileDesc->profileId, ONE_SEC);
                    }
                    evtCode = PROFILE_LINK_EVENT_DISC_COMPLETE;
                    goto AIRAPP_DISC_CPL;
                }
            }
            break;

        case PROFILE_LINK_EVENT_DISC_COMPLETE:
        case PROFILE_LINK_EVENT_DISC_LINKLOSS:
            if (airappLinkInfo->mmiProfileInfo.state != PROFILE_DISCONNECTED)
            {
                AIRAPP_DISC_CPL:

                MMI_UnloadProfile(linkIndex, &airappLinkInfo->mmiProfileInfo);
                MMI_PROTOCOL_L2CAPDisconnected(linkIndex, evtCode, FALSE);
                AirApp_ClearLinkInfo(linkIndex);
                #ifdef BLE_AIRApp
                AirApp_BleSetEnable(TRUE);
                #endif
            }
            break;
    }
}

PUBLIC void AirApp_FakeMedia_Event(U16 id)
{
    U8 linkIndex, cmdCode;

    cmdCode = 0;
    switch(id)
    {
        case MEDIA_EVT_KEY_VOICE_PROMPT_ENABLE:
            cmdCode = AIRAPP_CMD_KEY_VOICE_PROMPT_ENABLE;
            break;
        case MEDIA_EVT_KEY_VOICE_PROMPT_DISABLE:
            cmdCode = AIRAPP_CMD_KEY_VOICE_PROMPT_DISABLE;
            break;
        case MEDIA_EVT_VOICE_PROMPT_LANG_CHANGE:
            cmdCode = AIRAPP_CMD_KEY_VOICE_PROMPT_LANG_CHANGE;
            break;
        case MEDIA_EVT_KEY_FIND_MY_ACCESSORY:
            cmdCode = AIRAPP_CMD_FIND_MY_ACCESSORY;
            break;
        default:
            return;
    }
    START_SEARCH:
    for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
    {
        if(cmdCode && (AirApp_GetLinkInfo(linkIndex))->cmdState == cmdCode)
        {
            AirApp_SendResponse(linkIndex, cmdCode, HCI_VEVT_OCF_RESULT_CODE_SUCCESS);
            (AirApp_GetLinkInfo(linkIndex))->cmdState = 0;
        }
    }

    if(cmdCode == AIRAPP_CMD_KEY_VOICE_PROMPT_LANG_CHANGE)
    {
        cmdCode = AIRAPP_CMD_SET_VP_LANG;
        goto START_SEARCH;
    }
}

PRIVATE void AirApp_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
    AIRAPP_LINK_INFO XDATA_PTR airappLinkInfo = AirApp_GetLinkInfo(linkIndex);

    OSMQ_Put(&airappLinkInfo->OSMQ_AIRAPP_Data_From_Remote,dataPtr);
}

#ifdef BLE_AIRApp
PRIVATE void AirApp_BleInit(void)
{
    gAirApp_ctl.targetIdx = 0xFF;

    AppCtl.TxIdx = ATT_FindServerIndexByAddr((U8 CODE_PTR)&AppCtl.TxIdx);
    AppCtl.TxCccdIdx = ATT_FindServerIndexByAddr((U8 CODE_PTR)AppCtl.TX_CCCD);

    AirApp_SetDSPUnlockByOTA();
}

PUBLIC void AirApp_BleSetEnable(BOOL enabled)
{
    if (enabled)
    {
        ATT_ModifyAttrPermisson(AppCtl.TxCccdIdx, ATT_R_PERMIT | ATT_W_PERMIT);
    }
    else
    {
        ATT_ModifyAttrPermisson(AppCtl.TxCccdIdx, ATT_R_PERMIT | ATT_W_BANNED);
    }
}

PUBLIC BOOL AirApp_BleIsConnected(void)
{
    return gAirApp_ctl.isBleConnected;
}

PUBLIC BOOL AirApp_BleIsActivated(void)
{
    U16 cccd;

    cccd = ATT_GetCCCDByLink(AppCtl.TxCccdIdx, 0);

    //LightDbgPrint("isBleAct %x %x %x %x", (U8)gAirApp_ctl.isBleConnected, (U8)cccd, (U8)(cccd >> 8), (U8)((cccd >> 8) & 0x01));

    return (gAirApp_ctl.isBleConnected && (cccd & 0x0100));
}

PRIVATE void AirApp_BlePollingIndications(void)
{
    U8 XDATA_PTR dataSentPtr = NULL;
    AirAppMsgType XDATA_PTR airAppPtr;
    U8 para;

    /* Check battery */
    para = MMI_BAT_GetLevelInPercent();

    if (gAirApp_ctl.batStatus != para)
    {
        if (gAirApp_ctl.batStatus > para)
        {
            if ((gAirApp_ctl.batStatus - para) < 10)
                goto BLE_VP_POLLING;
        }
        else
        {
            if ((para - gAirApp_ctl.batStatus) < 10)
                goto BLE_VP_POLLING;
        }

        if ((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_BAT_STATUS, 1)) == (U8 XDATA_PTR)NULL)
            return;

        airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
        airAppPtr->VPara.batStatus = para;
        AirApp_SendData(gAirApp_ctl.targetIdx, dataSentPtr);
        gAirApp_ctl.batStatus = para;
        //LightDbgPrint("AirApp - ble report bat:%d",(U8)para);
    }


    /* Check VP */
    BLE_VP_POLLING:
    para = MMI_DRV_IsVoicePromptEnabled();

    if (gAirApp_ctl.isVPOn != para)
    {
        if ((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_VP_STATUS, 1)) == (U8 XDATA_PTR)NULL)
            return;

        airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
        airAppPtr->VPara.isVPOn = para;
        AirApp_SendData(gAirApp_ctl.targetIdx, dataSentPtr);
        gAirApp_ctl.isVPOn = para;
        //LightDbgPrint("AirApp - ble report VP:%d",(U8)para);
    }


    /* Check VP_Lang */
    para = gMMI_driver_variation_nvram.misc_para.init.vpLangIndex;

    if (gAirApp_ctl.VPLang != para)
    {
        if ((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_VP_LANG, 1)) == (U8 XDATA_PTR)NULL)
            return;

        airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
        airAppPtr->VPara.VPLang = para;
        AirApp_SendData(gAirApp_ctl.targetIdx, dataSentPtr);
        gAirApp_ctl.VPLang = para;
        //LightDbgPrint("AirApp - ble report VPLang:%d",(U8)para);
    }
}

PUBLIC void AirApp_BlePolling(void)
{
    if (gAirApp_ctl.delaySendDataTimer != NULL && !SYS_IsTimerExpired(&gAirApp_ctl.delaySendDataTimer))
    {
        return;
    }

    if ((((ATT_GetCCCDByLink(AppCtl.TxCccdIdx, 0)) >> 8) & 0x01) == 0)
    {
        return;
    }

    if (!gAirApp_ctl.isCCCDEnable)
    {
        gAirApp_ctl.isCCCDEnable = TRUE;
    }

    if (AirApp_BleCheckEdrLinkStatus())
    {
        AirApp_PollingIndications(gAirApp_ctl.targetIdx);
    }
    else
    {
        /* 3.0 Normal link not exist */
        AirApp_BlePollingIndications();
    }
}

PUBLIC void AirApp_BleUpdateConnStatus(BOOL isConnected, U8 XDATA_PTR ptr1)
{
    HCI_LE_EVT_STRU XDATA_PTR dataPtr = (HCI_LE_EVT_STRU XDATA_PTR)ptr1;

    gAirApp_ctl.isBleConnected = isConnected;
    gAirApp_ctl.targetIdx = 0xFF;

    if (isConnected)
    {
        /* BLE connected */
        SYS_SetTimer(&gAirApp_ctl.delaySendDataTimer, MMI_LE_DELAY_NUM_CONN_INTERVAL * ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)dataPtr->payload.connCmp.connInterval) * 4);
    }
    else
    {
        /* BLE disconnected */
        SYS_ReleaseTimer(&gAirApp_ctl.delaySendDataTimer);

        if (gAirApp_ctl.isCCCDEnable)
        {
            gAirApp_ctl.isCCCDEnable = FALSE;
            gAirApp_ctl.isVPOn = 0;
            gAirApp_ctl.VPLang = 0;
            if(gAirApp_ctl.dataCtl.remainLength)
            {
                OSMEM_Put(gAirApp_ctl.dataCtl.dataPtr);
                gAirApp_ctl.dataCtl.dataPtr = NULL;
                gAirApp_ctl.dataCtl.remainLength = 0;
            }
            AirApp_BleResetAllEdrProfileState();
        }
    }
}

PRIVATE BOOL AirApp_BleUpdateEdrTargetIdx(U8 connectedIdx)
{
    if (((gAirApp_ctl.targetIdx == 0xFF) && (gAirApp_ctl.targetIdx != connectedIdx)) ||
        ((gAirApp_ctl.targetIdx < MAX_MULTI_POINT_NO) && (!MMI_IsNormalLink(gAirApp_ctl.targetIdx))))
    {
        gAirApp_ctl.targetIdx = connectedIdx;

        return TRUE;
    }

    return FALSE;
}

PRIVATE U8 AirApp_BleNum2String(U8 num)
{
    if (num > 9)
    {
        return (num + 0x37);
    }
    else
    {
        return (num + 0x30);
    }
}

PRIVATE void AirApp_BleBdAddr2ASCII(U8 XDATA_PTR bdAddrPtr, U8 XDATA_PTR dstBdAddrPtr)
{
    U8 i;
    U8 dstIdx = 11;

    for(i = 0; i < 6; i++)
    {
        dstBdAddrPtr[dstIdx--] = AirApp_BleNum2String(bdAddrPtr[i] & 0x0F);
        dstBdAddrPtr[dstIdx--] = AirApp_BleNum2String((bdAddrPtr[i] >> 4) & 0x0F);
    }
}

PRIVATE BOOL AirApp_BleUpdateEdrRemoteName(U8 linkIndex)
{
    AIRAPP_LINK_INFO XDATA_PTR airAppLinkInfo = AirApp_GetLinkInfo(linkIndex);

    if (gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[0] && !SYS_ValueCompare(airAppLinkInfo->remoteName, gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName, MAX_REMOTE_NAME_LEN))
    {
        OSMEM_memcpy_xdata_xdata(airAppLinkInfo->remoteName, gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName, MAX_REMOTE_NAME_LEN);

        airAppLinkInfo->remoteNameLen = 0;
        while (gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[airAppLinkInfo->remoteNameLen])
        {
            airAppLinkInfo->remoteNameLen++;
        }

        return TRUE;
    }
    else if (!gMMI_ctl.mmiInfo[linkIndex].linkPara.remoteName[0] && !airAppLinkInfo->remoteName[0])
    {
        AirApp_BleBdAddr2ASCII(gMMI_ctl.mmiInfo[linkIndex].bdAddr, airAppLinkInfo->remoteName);

        airAppLinkInfo->remoteNameLen = 12;

        return TRUE;
    }

    return FALSE;
}

PRIVATE void AirApp_BleGetEdrLinkStatus(AIRAPP_BLE_REPORT_STATUS XDATA_PTR pData)
{
    AIRAPP_LINK_INFO XDATA_PTR airAppLinkInfo;
    U8 i;

    OSMEM_memset_xdata(pData, 0, sizeof(AIRAPP_BLE_REPORT_STATUS));

    for (i = 0; i < MAX_MULTI_POINT_NO; i++)
    {
        airAppLinkInfo = AirApp_GetLinkInfo(i);

        if (airAppLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED)
        {
            pData->report[i].status = TRUE;
            OSMEM_memcpy_xdata_xdata(&pData->report[i].remoteNameLen, &airAppLinkInfo->remoteNameLen, airAppLinkInfo->remoteNameLen + 1);
        }
    }

    pData->targetIdx = gAirApp_ctl.targetIdx;
}

PRIVATE void AirApp_BleReportEdrLinkStatus(void)
{
    U8 XDATA_PTR dataSentPtr;
    AirAppMsgType XDATA_PTR airAppPtr;

    if ((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_REPORT_LINK_STATUS, sizeof(AIRAPP_BLE_REPORT_STATUS))) == (U8 XDATA_PTR)NULL)
        return;

    airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);

    AirApp_BleGetEdrLinkStatus(&airAppPtr->VPara.bleReportStatus);

    AirApp_SendData(gAirApp_ctl.targetIdx, dataSentPtr);
}

PRIVATE BOOL AirApp_BleSetEdrProfileState(AIRAPP_LINK_INFO XDATA_PTR airAppLinkInfo, U8 state)
{
    if (state == PROFILE_CONNECTED)
    {
        if (airAppLinkInfo->mmiProfileInfo.state == PROFILE_DISCONNECTED)
        {
            /* BLE set AirApp profile as connected  */
            /* (only when 3.0 link is Normal Link) */
            airAppLinkInfo->mmiProfileInfo.state = PROFILE_CONNECTED;

            return TRUE;
        }
    }
    else if (state == PROFILE_DISCONNECTED)
    {
        if ((airAppLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED) && (!airAppLinkInfo->mmiProfileInfo.isAlive))
        {
            /* BLE set AirApp profile as disconnected */
            /* (only when 3.0 link is Normal Link) */
            airAppLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTED;

            return TRUE;
        }
    }

    return FALSE;
}

PUBLIC U8 AirApp_BleCheckEdrLinkStatus(void)
{
    BOOL bUpdate = FALSE;
    U8 i;
    AIRAPP_LINK_INFO XDATA_PTR airAppLinkInfo;
    U8 connectedIdx = 0xFF;
    U8 normalLinkNum = 0;

    /* Check 3.0 connection status */
    for (i = 0; i < MAX_MULTI_POINT_NO; i++)
    {
        airAppLinkInfo = AirApp_GetLinkInfo(i);

        if (MMI_IsNormalLink(i))
        {
            if (AirApp_BleSetEdrProfileState(airAppLinkInfo, PROFILE_CONNECTED))
            {
                LightDbgPrint("update 1");
                bUpdate = TRUE;
            }

            if (AirApp_BleUpdateEdrRemoteName(i))
            {
                LightDbgPrint("update 2");
                bUpdate = TRUE;
            }

            normalLinkNum++;
            connectedIdx = i;

        }
#ifdef AIR_MODULE
        else if (!MMI_AIR_IsAirLink(i))
#else
        else
#endif
        {
            if (AirApp_BleSetEdrProfileState(airAppLinkInfo, PROFILE_DISCONNECTED))
            {
                LightDbgPrint("update 3");
                OSMEM_memset_xdata(airAppLinkInfo->remoteName, 0, MAX_REMOTE_NAME_LEN);
                airAppLinkInfo->remoteNameLen = 0;
				if(AirApp_IsDSPLockedByOTA())
                    AirApp_SetDSPUnlockByOTA();
                bUpdate = TRUE;
            }
        }
    }

    /* Check 3.0 normal link number */
    if (gAirApp_ctl.normalLinkNum != normalLinkNum)
    {
        LightDbgPrint("update 4");
        gAirApp_ctl.normalLinkNum = normalLinkNum;
        bUpdate = TRUE;
    }

    /* Check target index */
    if (AirApp_BleUpdateEdrTargetIdx(connectedIdx))
    {
        LightDbgPrint("update 5");
        bUpdate = TRUE;
    }

    if (bUpdate)
    {
        AirApp_BleReportEdrLinkStatus();
    }

    if (gAirApp_ctl.targetIdx == 0xFF)
    {
        return FALSE;
    }

    return TRUE;
}

PRIVATE BOOL AirApp_BleConsiderCmd(U8 XDATA_PTR ptr)
{
    LightDbgPrint("cmd %x %x %x", (U8)ptr[0], (U8)ptr[4], (U8)ptr[5]);

    if (ptr[0] != AIRAPP_HCI_CMD_PACKET)
    {
        return FALSE;
    }

    switch(ptr[4])
    {
        case AIRAPP_CMD_GET_LINK_STATUS:
        {
            if(ptr[5] == HCI_VENDOR_OGF_AIRAPP_CMD_EX)
                return TRUE;
        }
        break;

        case AIRAPP_CMD_KEY_RESET_PAIRED_DEVICES:
        case AIRAPP_CMD_FIND_MY_ACCESSORY:
        case AIRAPP_CMD_GET_BATTERY:
        case AIRAPP_CMD_SET_LE_CONN_INTERVAL:
        case AIRAPP_CMD_GET_SYNCHRONIZE_DATA:
        case AIRAPP_CMD_CHANNEL_INFORMATION:
        case AIRAPP_CMD_WRITE_DEVICE_NAME:
        case AIRAPP_CMD_GET_DEVICE_NAME:
            return TRUE;
    }

    return FALSE;
}

PRIVATE U8 XDATA_PTR AirApp_BleGetProfileDataEvtPtr(U8 XDATA_PTR ptr, U16 length)
{
    ProfileDataEvent XDATA_PTR dataPtr;

    if ((dataPtr = (ProfileDataEvent XDATA_PTR)SYS_MemoryGet(sizeof(ProfileDataEvent)+length)) != NEAR_NULL)
    {
        OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)dataPtr->dataBody, ptr, length);
    }
    return dataPtr;
}

PUBLIC U8 AirApp_BleDataEvent(U8 XDATA_PTR ptr, U16 length)
{
    AIRAPP_DATA_CTL_TYPE XDATA_PTR airAppDataCtl;
    U8 XDATA_PTR dataPtr;
    U16 dataLen;
    BOOL isOTA = FALSE;

    if (!AirApp_BleIsActivated())
    {
        return FALSE;
    }

    if (gAirApp_ctl.targetIdx < MAX_MULTI_POINT_NO)
    {
        LightDbgPrint("LE_DEa:%x %x %x %x %x", (U8)ptr[0], (U8)ptr[1], (U8)ptr[2], (U8)ptr[3], (U8)ptr[4]);
        airAppDataCtl = &(AirApp_GetLinkInfo(gAirApp_ctl.targetIdx)->dataCtl);
    }
    else
    {
        LightDbgPrint("LE_DEb:%x %x %x %x %x", (U8)ptr[0], (U8)ptr[1], (U8)ptr[2], (U8)ptr[3], (U8)ptr[4]);
        /* 3.0 Normal link not exist */
        airAppDataCtl = &gAirApp_ctl.dataCtl;
    }

    LightDbgPrint("LE_DE 0:%x %x %x", (U8)gAirApp_ctl.targetIdx, (U8)ptr[5], (U8)ptr[6]);
    if (airAppDataCtl->remainLength)
    {
        ProfileDataEvent XDATA_PTR profileDataPtr;
        profileDataPtr = (ProfileDataEvent XDATA_PTR)airAppDataCtl->dataPtr;

        LightDbgPrint("LE_DE 1:%h %h", (U16)airAppDataCtl->remainLength, (U16)length);
        if (airAppDataCtl->remainLength < length)
        {
            airAppDataCtl->remainLength = 0;

            if (profileDataPtr->dataBody[0] == AIRAPP_HCI_CMD_PACKET)
            {
                AirApp_SendResponse(gAirApp_ctl.targetIdx, profileDataPtr->dataBody[4], HCI_VEVT_OCF_RESULT_CODE_FAIL);
            }
            else
            {
                AirApp_SendResponse(gAirApp_ctl.targetIdx, profileDataPtr->dataBody[5], HCI_VEVT_OCF_RESULT_CODE_FAIL);
            }

            SYS_MemoryRelease(&airAppDataCtl->dataPtr);
            return FALSE;
        }

        airAppDataCtl->remainLength -= length;
        OSMEM_memcpy_xdata_xdata(profileDataPtr->dataBody+airAppDataCtl->offset, ptr, length);
        airAppDataCtl->offset += length;

        if (airAppDataCtl->remainLength == 0)
        {
            LightDbgPrint("LE_DE 2:%x %x", (U8)profileDataPtr->dataBody[5], (U8)profileDataPtr->dataBody[6]);
            if (AirApp_DataHandler(gAirApp_ctl.targetIdx, (U8 XDATA_PTR)airAppDataCtl->dataPtr))
            {
                airAppDataCtl->dataPtr = NULL;
            }
            else
            {
                LightDbgPrint("LE_DE 3:DataHandler FALSE");
                SYS_MemoryRelease(&airAppDataCtl->dataPtr);
            }
        }

        return TRUE;
    }
    else if ((ptr[0] == AIRAPP_HCI_DATA_PACKET) && (ptr[6] == HCI_ACL_OGF_APP_FLASH_BLE || ptr[6] == HCI_ACL_OGF_APP_REMOTE_FLASH_BLE))
    {
        dataLen = ENDIAN_TRANSFORM_U16(*(U16 XDATA_PTR)(ptr+3)) + 5;

        LightDbgPrint("LE_DE 4:%h %h %x %x", (U16)dataLen, (U16)length, (U8)ptr[7], (U8)ptr[8]);

        if (dataLen > length)
        {
            airAppDataCtl->dataPtr = SYS_MemoryGet(sizeof(ProfileDataEvent) + dataLen);
            if (airAppDataCtl->dataPtr)
            {
                airAppDataCtl->remainLength = dataLen - length;
                airAppDataCtl->offset = length;
                OSMEM_memcpy_xdata_xdata(((ProfileDataEvent XDATA_PTR)airAppDataCtl->dataPtr)->dataBody, ptr, length);

                return TRUE;
            }

            LightDbgPrint("LE_DE 6");
            AirApp_SendResponse(gAirApp_ctl.targetIdx, ptr[5], HCI_VEVT_OCF_RESULT_CODE_FAIL);
            return FALSE;
        }
        else if (dataLen < length)
        {

            LightDbgPrint("LE_DE 7");
            AirApp_SendResponse(gAirApp_ctl.targetIdx, ptr[5], HCI_VEVT_OCF_RESULT_CODE_FAIL);
            return FALSE;
        }

        /* Don't care 3.0 */
        isOTA = TRUE;
    }
    else
    {
        AirAppDataHCICmdType XDATA_PTR appDataPtr =(AirAppDataHCICmdType XDATA_PTR) ptr;

        if ((appDataPtr->hciOpCode == AIRAPP_HCI_CMD_PACKET) && ((appDataPtr->hciLength+4) != length))
        {
            LightDbgPrint("Return packet");
            return FALSE;
        }

        if (!AirApp_BleCheckEdrLinkStatus())
        {
            /* 3.0 Normal link not exist */
            if (AirApp_BleConsiderCmd(ptr))
            {
                if ((dataPtr = AirApp_BleGetProfileDataEvtPtr(ptr, length)) != NEAR_NULL)
                {
                    AirApp_CmdHandler(gAirApp_ctl.targetIdx, dataPtr);
                    OSMEM_Put(dataPtr);
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                if (ptr[5] == HCI_VENDOR_OGF_AIRAPP_CMD)
                {
                    AirApp_SendRsp(gAirApp_ctl.targetIdx, ptr[4], HCI_VENDOR_OGF_AIRAPP_RESP, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                }
                else if (ptr[5] == HCI_VENDOR_OGF_AIRAPP_CMD_EX)
                {
                    AirApp_SendRsp(gAirApp_ctl.targetIdx, ptr[4], HCI_VENDOR_OGF_AIRAPP_RESP_EX, HCI_VEVT_OCF_RESULT_CODE_FAIL);
                }

                return FALSE;
            }
        }
    }

    if((dataPtr = AirApp_BleGetProfileDataEvtPtr(ptr, length)) == NEAR_NULL)
    {
        LightDbgPrint("LE_DE 12");
        return FALSE;
    }

    if (isOTA)
    {
        if(!AirApp_DataHandler(gAirApp_ctl.targetIdx, dataPtr))
        {
            LightDbgPrint("LeDEvt13");
            OSMEM_Put(dataPtr);
        }
    }
    else
    {
        AirApp_DataEvent(gAirApp_ctl.targetIdx, dataPtr);
        AirApp_RemoteDataHandler(gAirApp_ctl.targetIdx);
    }
    return TRUE;
}

PRIVATE void AirApp_BleResetEdrProfileState(AIRAPP_LINK_INFO XDATA_PTR airAppLinkInfo)
{
    if ((AirApp_BleIsActivated()) &&
        (airAppLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED) && (!airAppLinkInfo->mmiProfileInfo.isAlive))
    {
        /* BLE set airapp profile as disconnected */
        /* (only when 3.0 link is Normal Link) */
        airAppLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTED;
    }

}

PRIVATE void AirApp_BleResetAllEdrProfileState(void)
{
    U8 i;
    AIRAPP_LINK_INFO XDATA_PTR airAppLinkInfo;

    for (i = 0; i < MAX_MULTI_POINT_NO; i++)
    {
        airAppLinkInfo = AirApp_GetLinkInfo(i);

        if ((airAppLinkInfo->mmiProfileInfo.state == PROFILE_CONNECTED) && (!airAppLinkInfo->mmiProfileInfo.isAlive))
        {
            /* BLE set airapp profile as disconnected */
            /* (only when 3.0 link is Normal Link) */
            airAppLinkInfo->mmiProfileInfo.state = PROFILE_DISCONNECTED;

        }
    }

}
PRIVATE BOOL AirApp_BleSendAclErrorRsp(U8 ocf)
{
    U8 errrsp[8];

    errrsp[0] = HCI_UART_XPORT_PKT_ID_ACL_DATA;
    errrsp[1] = 0x00;
    errrsp[2] = 0x0F;
    errrsp[3] = 0x03;
    errrsp[4] = 0x00;
    errrsp[5] = ocf;
    errrsp[6] = 0x09;
    errrsp[7] = 0xFF;


    if (APP_sendTxData((U8 XDATA_PTR)&errrsp[0], 5 + 3))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


#endif

PUBLIC void AirApp_Init(void)
{
    extern SdpServiceNode gAirApp_ServiceNode;
    RFCOMM_RegisterProfile(&gAirApp_ProfileNode);
    SDAP_RegisterServiceRecord(&gAirApp_ServiceNode);

    #ifdef BLE_AIRApp
    AirApp_BleInit();
    #endif
    #if AIRAPP_CALLER_NAME
    AirApp_EraseCallerNameSector();
    #endif
}

PUBLIC void AirApp_EnterIncomingState(U8 linkIndex)
{
#if AIRAPP_CALLER_NAME
    AirAppMsgType XDATA_PTR airAppPtr;
    U8 XDATA_PTR dataSentPtr;
    if((AirApp_GetLinkInfo(linkIndex))->mmiProfileInfo.state != PROFILE_CONNECTED)
        return;

    if(!(AirApp_GetLinkInfo(linkIndex))->isCallerNameOn || !AirApp_IsCallerNameSectorValid() || !AirApp_IsMMIAllowWrite(linkIndex))
        return;
    if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_INCOMING_CALL_STATUS, 1)) == (U8 XDATA_PTR)NULL)
        return;
    airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppPtr->VPara.isEnterIncoming = TRUE;
    AirApp_SendData(linkIndex, dataSentPtr);

    AirApp_DisableAndSuspendDSP();
#else
    UNUSED(linkIndex);
#endif
}

PUBLIC void AirApp_ExitIncomingState(U8 linkIndex)
{
#if AIRAPP_CALLER_NAME

    AirAppMsgType XDATA_PTR airAppPtr;
    U8 XDATA_PTR dataSentPtr;

    if(gAirApp_ctl.callNameCtl.callerNameLinkIndex == linkIndex || gAirApp_ctl.callNameCtl.callerNameLinkIndex == MMI_EOF)
    {
        AirApp_ResumeDSP();
    }

    if((AirApp_GetLinkInfo(linkIndex))->mmiProfileInfo.state != PROFILE_CONNECTED)
        return;

    if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_INCOMING_CALL_STATUS, 1)) == (U8 XDATA_PTR)NULL)
        return;

    airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    airAppPtr->VPara.isEnterIncoming = FALSE;
    AirApp_SendData(linkIndex, dataSentPtr);
#endif
    UNUSED(linkIndex);
}

PUBLIC BOOL AirApp_IsCallerNameDataValid(void)
{
    return (gAirApp_ctl.callNameCtl.dataValid == 0)? TRUE: FALSE;
}

PUBLIC void AirApp_DumpMicData(U8 linkIndex, U8 XDATA_PTR ptr, U16 length)
{
    AirAppMsgType XDATA_PTR airAppPtr;
    U8 XDATA_PTR dataSentPtr;

    if((AirApp_GetLinkInfo(linkIndex))->mmiProfileInfo.state != PROFILE_CONNECTED)
        return;

    if((dataSentPtr = AirApp_GetUnSolicitedInd(AIRAPP_IND_DUMP_MIC_DATA, length)) == (U8 XDATA_PTR)NULL)
        return;

    airAppPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
    OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&airAppPtr->VPara, ptr, length);
    AirApp_SendData(linkIndex, dataSentPtr);
}

PUBLIC void AirApp_CustomerDataRsp(U8 linkIndex, U8 vOcf, U8 result, U8 dataLen, U8 XDATA_PTR dataPtr)
{
    U8 XDATA_PTR dataSentPtr;

    if(linkIndex >= MAX_MULTI_POINT_NO)
        return;

    if((dataSentPtr = AirApp_GetMemory(dataLen + 1)) != (U8 XDATA_PTR)NULL)
    {
        AirAppMsgType XDATA_PTR airAppRespPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
        airAppRespPtr->VOpCode.VCmdOcf = vOcf;
        airAppRespPtr->VOpCode.VCmdOgf = HCI_VENDOR_OGF_AIRAPP_CUSTOMER_RSP;
        airAppRespPtr->VPara.getAirAppCstRsp.result = result;
        if(dataPtr != (U8 XDATA_PTR)NULL)
        {
            OSMEM_memcpy_xdata_xdata(   (U8 XDATA_PTR)&airAppRespPtr->VPara.getAirAppCstRsp.dataStart,
                                        (U8 XDATA_PTR)&dataPtr,
                                        dataLen);
        }
        AirApp_SendData(linkIndex, dataSentPtr);
    }
}

PUBLIC void AirApp_PassThroughTransferData(U8 XDATA_PTR ptr, U8 cmdOpcode, U8 vogf)
{
    MMIMsgType XDATA_PTR airCmdPtr = (MMIMsgType XDATA_PTR)ptr;
    U8 XDATA_PTR dataSentPtr;
    U8 airLinkIndex = MMI_GetLinkIndexByBdA((U8 XDATA_PTR)&airCmdPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.bdAddr);

    if(airLinkIndex >= MAX_MULTI_POINT_NO)
        return;

    if((dataSentPtr = AirApp_GetMemory(airCmdPtr->paraLen - sizeof(HCI_VCMD_OPCODE_STRU) - sizeof(airCmdPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.bdAddr))) != (U8 XDATA_PTR)NULL)
    {
        AirAppMsgType XDATA_PTR airAppRespPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
        LightDbgPrint("AirApp: PassThrogh[%d]:%x,%d",(U8) airLinkIndex,(U8)cmdOpcode,(U8)airCmdPtr->paraLen);
        airAppRespPtr->VOpCode.VCmdOcf = cmdOpcode;
        airAppRespPtr->VOpCode.VCmdOgf = vogf;
        OSMEM_memcpy_xdata_xdata(   (U8 XDATA_PTR)&airAppRespPtr->VPara.dataBegin,
                                    (U8 XDATA_PTR)&airCmdPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.dataStart,
                                    (airCmdPtr->paraLen - sizeof(HCI_VCMD_OPCODE_STRU) - sizeof(airCmdPtr->msgBodyPtr.hciVCmd.VPara.passThroughCmd.bdAddr)));
        AirApp_SendData(airLinkIndex, dataSentPtr);
    }
    OSMEM_Put((U8 XDATA_PTR)ptr);
}

PRIVATE void AirApp_MMIKeyEventRsp(U8 linkIndex, U16 mmiKeyCode,U8 result)
{
    U8 XDATA_PTR dataSentPtr;
    if((dataSentPtr = AirApp_GetMemory(sizeof(HCI_VEVT_PARA_AIRAPP_MMI_KEY_CODE_RSP))) != (U8 XDATA_PTR)NULL)
    {
        AirAppMsgType XDATA_PTR airAppRespPtr = (AirAppMsgType XDATA_PTR)(dataSentPtr + *dataSentPtr);
        LightDbgPrint("AirApp: Key Rsp:%X,%d",(U32)mmiKeyCode,(U8) result);
        airAppRespPtr->VOpCode.VCmdOcf = AIRAPP_CMD_KEY_MMI_KEY_EVENT;
        airAppRespPtr->VOpCode.VCmdOgf = HCI_VENDOR_OGF_AIRAPP_RESP;
        airAppRespPtr->VPara.airMMIKeyCodeRsp.mmiKeyCode = mmiKeyCode;
        airAppRespPtr->VPara.airMMIKeyCodeRsp.result = result;
        AirApp_SendData(linkIndex, dataSentPtr);
    }
}

PUBLIC void AirApp_SlaveATGainRsp(U8 linkIndex, U8 cmdOpcode, U8 resultCode)
{
    AirApp_SendRsp(linkIndex, cmdOpcode, HCI_VENDOR_OGF_AIRAPP_RESP, resultCode);
}
#endif //AIRAPP_Profile
