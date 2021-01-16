
#include "rc.h"



#define __SERVICE_HEADER_INCLUDED_BY_PROFILE__
/* include service header files here to declare the instance of attribute value */
#include "..\MMI\MMI_Base.h"
#include "..\APP\APP_Interface.h"
#undef __SERVICE_HEADER_INCLUDED_BY_PROFILE__

#include "..\MMI\MMI_ChgBat.h"
#include "..\ATT\ATT_SDK.h"
#include "..\MMI\MMI_interface.h"
#include "..\DRIVER\Peq_HPF.h"
#include "..\ROM\HC\hc.h"
#include "..\ATT\ATT_interface.h"

#include "..\SECTOR\sector_mmi_le_nvram.h"

#include "..\MMI_LE\lic_hal.h"
#include "..\MMI_LE\mmi_le.h"

#include "..\ROM\LL\LL_interface.h"
#include "..\MMI\MMI_Driver.h"
#include "..\MMI\MMI_LED.h"
#include "..\AVRCP\AVRCP_Interface.h"
#include "..\ATT\ATT_External_Struct.h"
#ifdef MMI_LE_APP_ENABLE

#ifdef BLE_AIRApp
#include "..\AirApp\AirApp_Interface.h"
#include "..\MMI_LE\link_index_ctl.h"
#endif
#ifdef LIGHTING_ENABLE
#include "..\MMI\MMI_AirModule.h"
#include "..\AirApp\AirApp_Interface.h"
extern AirAppLedCtl XDATA gAirAppLedCtl;
#endif
#ifdef BLE_APP
#include "..\ATT\ATT.h"
#endif
#ifdef ANCS_CELL_PHONE_APP
#include "..\APP_ANCS\APP_ANCS_interface.h"
#endif	//#ifdef ANCS_CELL_PHONE_APP
//extern	PUBLIC void MMI_HCI_UartSendUnSolicitedIndication(U8 opCode, U8 XDATA_PTR dataPtr, U8 linkIndex);
//MMI_LE_EXTERN MMI_LE_ADV_CTL XDATA gMMI_le_adv_ctl;

extern MmiCtlType XDATA gMMI_ctl;
extern MP_PARAMETER_STRU CODE_PTR XDATA pMpParameter;
extern HC_CTL_STRU XDATA gHC_ctl;
extern MMI_LE_NVRAM_TYPE CODE_PTR XDATA gMMI_le_nvram;
//extern PUBLIC void MMI_AVRCP_PassThroughVolumeHandler(U8 linkIndex);
#include "..\MMI\MMI_AVRCP.h"
extern PUBLIC void MMI_AVRCP_UartSetAbsVol(U8 vol);
extern PUBLIC VOID MMI_LE_SectorSaveData();
#endif

#ifdef LE_SUPPORTED_HOST_FLASH
extern BAS_CTRL XDATA gBAS_Ctrl;

U8 APP_ReadBdAddr_CB (U8 CBtype)
{
    if(CBtype == CB_BEFORE_SENDING)
    {
        OSMEM_memcpy_xdata_xdata(gBAS_Ctrl.BD_ADDR, (U8 XDATA_PTR)&gHC_ctl.BdAddr, sizeof(BD_ADDR_STRU));
    }
    return 0;
}
#endif

#ifdef MMI_LE_APP_ENABLE
U8 Status_CB (U8 CBtype)
{
    //Read
    if(CBtype == CB_BEFORE_SENDING)
    {
        //===Statue(Battery, Vol, PEQ_Mode, VP_Enable/Disable, VP_Lenguage)===

        //Battery
		AppCtl.Status.Battery = MMI_BAT_GetLevelInQuater();

        //Vol
        //AppCtl.Status.Vol = CURRENT_A2DP_SOUND_LEVEL(gMMI_ctl.audioDevCtl.currentActiveLinkIndex);

		//Absolute volume is 0~127, but APP level is 0~255, we should transfer absolute volume for app
		if(CURRENT_A2DP_SOUND_LEVEL(gMMI_ctl.serviceInd) == 0)
			AppCtl.Status.Vol = 0;
		else if(CURRENT_A2DP_SOUND_LEVEL(gMMI_ctl.serviceInd) == MMI_MAX_MUSIC_SOUND_LEVEL())
			AppCtl.Status.Vol = 0xFF;
		else
			AppCtl.Status.Vol = CURRENT_A2DP_SOUND_LEVEL(gMMI_ctl.serviceInd)*MMI_MAX_MUSIC_SOUND_LEVEL()-8;

		//PEQ_Mode
        AppCtl.Status.PEQ_Mode[A2DP] = gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_A2DP];
        AppCtl.Status.PEQ_Mode[LINE_IN] = gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_LINE_IN];
        //VP_Enable/Disable
        AppCtl.Status.VP_OnOff = MMI_DRV_IsVoicePromptEnabled();
        //VP_Lenguage Switch
        AppCtl.Status.VP_Lenguage = gMMI_driver_variation_nvram.misc_para.init.vpLangIndex;


    }
    return 0;
}

U8 Read_BDAddr_CB (U8 CBtype)
{
    if(CBtype == CB_BEFORE_SENDING)
    {
        OSMEM_memcpy_xdata_xdata(AppCtl.BD_ADDR, (U8 XDATA_PTR)&gHC_ctl.BdAddr, sizeof(BD_ADDR_STRU));
    }
    else//if(CBtype == CB_AFTER_WRITING)
    {
    }
    return 0;
}

U8 Read_FW_Version_CB (U8 CBtype)
{
    if(CBtype == CB_BEFORE_SENDING)
    {
        AppCtl.FW_Version = 0x2000;
    }
    return 0;
}

U8 RW_DeviceName_CB (U8 CBtype)
{
    U8 currentIndex;
    U8 XDATA_PTR msgPtr;
    U8 FlagsLen;
    U8 DeviceNameLen;
    U8 LocalNameShiftOffset;
	U16 AttributCurrentLen;

	if((msgPtr = (U8 XDATA_PTR)OSMEM_Get(OSMEM_ptr1)) != NEAR_NULL)
	{
        OSMEM_memcpy_xdata_code(msgPtr, (U8 CODE_PTR)&gLL_param.adv_data_param.adv_data, sizeof(LE_ADV_DATA_PARAM));
		FlagsLen = *(msgPtr+ADV_FLAGS_LEN_OFFSET);
		DeviceNameLen = *(msgPtr+ADV_LOCALNAME_LEN_OFFSET+FlagsLen);
		LocalNameShiftOffset = FlagsLen+ADV_LOCALNAME_OFFSET;

	    if(CBtype == CB_BEFORE_SENDING)
	    {
	        //OSMEM_memcpy_xdata_xdata(&AppCtl.DeviceName, (U8 CODE_PTR)&(gMMI_le_adv_ctl.adv_data.adv_data + (LocalNameShiftOffset-1)), DeviceNameLen);
	        OSMEM_memcpy_xdata_xdata(&AppCtl.DeviceName, (U8 CODE_PTR)&(gLL_param.adv_data_param.adv_data + (LocalNameShiftOffset-1)), DeviceNameLen);

	        currentIndex = ATT_FindServerIndexByAddr((U8 CODE_PTR)&AppCtl.DeviceName);
	        ATT_ChangeAttributeValueLength(currentIndex, DeviceNameLen);
	    }
	    else//if(CBtype == CB_AFTER_WRITING)
	    {
		    currentIndex = ATT_FindServerIndexByAddr((U8 CODE_PTR)&AppCtl.DeviceName);
			AttributCurrentLen = ATT_GetAttributeValueCurrentLen(currentIndex);
    	    ATT_ChangeAttributeValueLength(currentIndex, AttributCurrentLen);

            //gMMI_le_adv_ctl.adv_data.length = msgPtr[0]-DeviceNameLen-1+AttributCurrentLen;
			//gMMI_le_adv_ctl.adv_data.adv_data[ADV_FLAGS_LEN_OFFSET+FlagsLen] = AttributCurrentLen;
			gLL_param.adv_data_param.adv_data_len = msgPtr[0]-DeviceNameLen-1+AttributCurrentLen;
			gLL_param.adv_data_param.adv_data[ADV_FLAGS_LEN_OFFSET+FlagsLen] = AttributCurrentLen;

	        //OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&(gMMI_le_adv_ctl.adv_data.adv_data+(LocalNameShiftOffset-1)), &AppCtl.DeviceName, AttributCurrentLen);
			OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&(gLL_param.adv_data_param.adv_data+(LocalNameShiftOffset-1)), &AppCtl.DeviceName, AttributCurrentLen);

			//MMI_Save_BLE_DataToFlash();
			MMI_LE_SectorSaveData();
	    }

   	    OSMEM_Put(msgPtr);
    }

    return 0;
}

U8 ResetPDL_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
        MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, KEY_RESET_PAIRED_DEVICES);
    }
    return 0;
}

U8 Vol_Ctl_CB (U8 CBtype)
{
	#ifdef AVRCP_Profile
	U8 AbsoluteVolume;

    if(CBtype == CB_AFTER_WRITING)
    {
    	//App volume is 0~255, we should transfer it to absolute volume
    	if(AppCtl.Vol_Ctl == 0xFF)
			AbsoluteVolume = AVRCP_ABSOLUTE_VOLUME_MAX;
		else
			AbsoluteVolume = AppCtl.Vol_Ctl/2;

		MMI_AVRCP_UartSetAbsVol(AbsoluteVolume);

    }
	#else
	UNUSED(CBtype);
	#endif
    return 0;
}

U8 PEQ_Mode_Switch_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
        MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, KEY_PEQ_MODE_CHANGE);
    }
    return 0;
}

U8 VoicePromptSwitch_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
        MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, KEY_VOICE_PROMPT_TOGGLE);
    }
    return 0;
}

U8 VP_Language_Switch_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
        MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, KEY_VOICE_PROMPT_LANG_CHANGE);
    }
    return 0;
}

U8 FindMyAccesory_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
        MMI_PushMediaEvent(MEDIA_EVT_KEY_FIND_MY_ACCESSORY);
    }
    return 0;
}

#ifdef BLE_APP
U8 Rx_CB (U8 CBtype)
{
    U8 serverIdx;
    U16 len;
    #ifdef ANCS_CELL_PHONE_APP
    APP_CMD_STRU XDATA_PTR cmdPtr;
    #endif

    if(CBtype == CB_AFTER_WRITING)
    {
        serverIdx = ATT_FindServerIndexByAddr((U8 CODE_PTR)&AppCtl.Rx);
        len = ATT_GetAttributeValueCurrentLen(serverIdx);
        //LightDbgPrint("APP-sIdx:%d,len:%x %x %x", (U8)serverIdx, (U8)(len>>8), (U8)len, (U8)AppCtl.Rx[0]);

		#ifdef ANCS_CELL_PHONE_APP
        cmdPtr = (APP_CMD_STRU XDATA_PTR)AppCtl.Rx;
        if ((cmdPtr->hciXportId == APP_HCI_CMD_PACKET) && (cmdPtr->vogf == HCI_VENDOR_OGF_ANCS_CMD))
        {
            APP_ANCS_CmdRx(cmdPtr, len);
        }
        else
        #endif
        {
            #ifdef BLE_AIRApp
        	AirApp_BleDataEvent(AppCtl.Rx, len);
            #endif
    	}
    }
    return 0;
}

BOOL App_BleIsMemEnough(U16 len)
{
    if (len > BLE_MAX_SEND_TX_DATA_LEN)
    {
        U8 numMem;

        numMem = len / BLE_MAX_SEND_TX_DATA_LEN;
        if (BLE_MAX_SEND_TX_DATA_LEN + ATT_RSP_HEADER_LEN <= OSMEM1_BLKSIZE)
        {
            numMem = (len % BLE_MAX_SEND_TX_DATA_LEN) ? (numMem + 1) : (numMem);
            if (numMem > gAlignOSMEM.OSMEM1.noFB)
            {
                return FALSE;
            }
        }
        else if (BLE_MAX_SEND_TX_DATA_LEN + ATT_RSP_HEADER_LEN <= OSMEM2_BLKSIZE)
        {
            numMem = (len % BLE_MAX_SEND_TX_DATA_LEN + ATT_RSP_HEADER_LEN > OSMEM1_BLKSIZE) ? (numMem + 1) : (numMem);
            //LightDbgPrint("app checkMem %x %x %x %x", (U8)len, (U8)(len>>8), (U8)numMem, (U8)gAlignOSMEM.OSMEM2_small_TX.noFB);
            if (numMem > gAlignOSMEM.OSMEM2_small_TX.noFB)
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }

    }
    return TRUE;
}

BOOL APP_sendTxData(U8 XDATA_PTR dataPtr, U16 len)
{
    U8 linkIdx;
    BOOL rtValue;
    ATT_SEND_DATA_STRU XDATA_PTR paraPtr;

    if ((((ATT_GetCCCDByLink(AppCtl.TxCccdIdx, 0)) >> 8) & 0x01) == 0)
    {
        //LightDbgPrint("CCCD %x %x", (U8)ATT_GetCCCDByLink(AppCtl.TxCccdIdx, 0), (U8)(ATT_GetCCCDByLink(AppCtl.TxCccdIdx, 0)>>8));
        return FALSE;
    }

    if(LIC_LE_GetNumOfActiveLink() > 0)
    {
		linkIdx = LIC_LE_GetActiveLinkIdx(0);

    	if (paraPtr = SYS_MemoryGet(sizeof(ATT_SEND_DATA_STRU)))
        {
        	paraPtr->dataPtr = dataPtr;
        	paraPtr->serverIdx = AppCtl.TxIdx;
        	paraPtr->len = len;
        	paraPtr->maxLen = BLE_MAX_SEND_TX_DATA_LEN;
        	paraPtr->connHdl = LIC_LE_GetConnHdl(linkIdx);
        	rtValue = ATT_SendTxData(paraPtr);

        	OSMEM_Put((U8 XDATA_PTR)paraPtr);
        	return rtValue;
        }
    }
    return FALSE;
}

#endif//#ifdef BLE_APP

#ifdef LIGHTING_ENABLE
U8 OnOffMusicYouCanSee_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
		gAirAppLedCtl.MusicYouCanSeeEnable_ready = AppCtl.OnOff_MusicYouCanSee;
		if(!(MMI_AIR_ChangeLightModeNotify(AIRAPP_CMD_ON_OFF_MUSIC_YOU_CAN_SEE, gAirAppLedCtl.MusicYouCanSeeEnable_ready ,0 ,0)))
		{
			MusicYouCanSeeEnable = AppCtl.OnOff_MusicYouCanSee;
		}
    }
    return 0;
}
U8 RGB_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
    	gAirAppLedCtl.LedR_ready = AppCtl.RGB[0];
		gAirAppLedCtl.LedG_ready = AppCtl.RGB[1];
		gAirAppLedCtl.LedB_ready = AppCtl.RGB[2];
		if(!(MMI_AIR_ChangeLightModeNotify(AIRAPP_CMD_GET_COLOR_PICKER_RGB,gAirAppLedCtl.LedR_ready,gAirAppLedCtl.LedG_ready,gAirAppLedCtl.LedB_ready)))
		{
			MMI_LED_Set_RGB(AppCtl.RGB[0], AppCtl.RGB[1], AppCtl.RGB[2]);
		}
    }
    return 0;
}
U8 LightingMode_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
		gAirAppLedCtl.LightingMode_ready = AppCtl.LightingMode;
		if(!(MMI_AIR_ChangeLightModeNotify(AIRAPP_CMD_LIGHTING_MODE_SELECT, gAirAppLedCtl.LightingMode_ready, 0, 0)))
		{
			MMI_LED_LightingMode(AppCtl.LightingMode);
		}
    }
    return 0;
}
U8 AlarmClock_CB (U8 CBtype)
{
    if(CBtype == CB_AFTER_WRITING)
    {
    	gAirAppLedCtl.Alarm_clock_OnOff_ready = AppCtl.Alarm_Clock;
		if(!(MMI_AIR_ChangeLightModeNotify(AIRAPP_CMD_ALARM_CLOCK, gAirAppLedCtl.Alarm_clock_OnOff_ready, 0, 0)))
		{
			MMI_LED_AlarmClock(AppCtl.Alarm_Clock);
		}
    }
    return 0;
}
#endif

#endif //#ifdef MMI_LE_APP_ENABLE
