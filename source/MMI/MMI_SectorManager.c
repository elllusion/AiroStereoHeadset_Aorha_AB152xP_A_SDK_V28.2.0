//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _MMI_SECTOR_MANAGER_

#include "MMI.h"
#include "MMI_Protocol.h"
#include "MMI_Driver.h"
#include "MMI_SectorManager.h"
#include "MMI_HCI.h"
#include "MMI_AirModule.h"

#include "..\LM\LM_Interface.h"
#include "LC.h"
#include "LC_Interface.h"

#include "..\DRIVER\Peq_HPF.h"

#include "sector_merge_runtime.h"
#include "drv_flash.h"
#include "drv_program_flash.h"

#include "sector_config1.h"
#include "drv_program_flash_flash.h"
#ifdef MMI_LE_ENABLE
#include "..\MMI_LE\MMI_LE_interface.h"
#endif

#include "..\SDK_Proxy\SDK_Proxy.h"

PRIVATE U16 MMI_GetMmiDriverNvramSectorSize(void);
PRIVATE void MMI_SECTOR_ResetAirLinkInfo(void);
PRIVATE void MMI_SECTOR_ResetFastLinkInfo(void);
PRIVATE void MMI_SECTOR_ResetLinkInfo(void);
PRIVATE void MMI_ResetMmiDriverNvramValue(void);
PRIVATE U8 SaveMmiNvramTo4kbFlash(void);

#ifdef DUAL_MIC_ENABLE
extern PUBLIC void AUDIO_SaveDataToFlash(U8 CODE_PTR pValidSector, U8 XDATA_PTR dataPtr);
#endif

#ifdef LE_SUPPORTED_HOST_FLASH
extern PUBLIC void LIC_ImmSaveToFlash(U8 CODE_PTR pValidSector, U8 XDATA_PTR pLeNvram);
extern PUBLIC void LIC_UpdateFlashAddr(void);
#endif

extern VFUN XDATA api_iosys_evt_func[API_GRP_IOSYS_CB_CNT];

PUBLIC void MMI_SECTOR_MmiNvramInit(void)
{
	#if USING_DIRECT_MMI_NVARM
	pMMI_nvram = (MMI_NVRAM_TYPE CODE_PTR) GET_SECTOR_CFG1_ADDR(SECTOR_MMI_NVRAM_INIT);
	#else
	OSMEM_memcpy_xdata_code ((U8 XDATA_PTR) &gMMI_nvram, GET_SECTOR_CFG1_ADDR(SECTOR_MMI_NVRAM_INIT), sizeof (MMI_NVRAM_TYPE));
	#endif
	
	#if USING_DIRECT_MMI_NVARM_KEYMAP
	pMMI_nvram_keymap = (MMI_NVRAM_KEYMAP CODE_PTR) GET_SECTOR_CFG1_ADDR(SECTOR_MMI_NVRAM_KEYMAP_INIT);
	#else
	OSMEM_memcpy_xdata_code ((U8 XDATA_PTR) &gMMI_nvram_keymap, GET_SECTOR_CFG1_ADDR(SECTOR_MMI_NVRAM_KEYMAP_INIT), sizeof (MMI_NVRAM_KEYMAP));
	#endif
	
	#if USING_DIRECT_NVRAM_SPKGAINSET
	#else
	MMI_ReadNvramDataChunk(gMMI_mem_spkGainSet, gMMI_nvram.spkGainSet, sizeof(SpkGainSet) * SOUND_LEVEL_NO);
	MMI_ReadNvramDataChunk(gMMI_mem_spkGainSetSCOWB, gMMI_nvram.spkGainSetSCOWB, sizeof(SpkGainSetSCOWB) * SOUND_LEVEL_NO);
	#endif
	
	#if USING_DIRECT_NVRAM_MICGAINSET
	#else
	MMI_ReadNvramDataChunk(gMMI_mem_micGainSet, gMMI_nvram.micGainSet, sizeof(MicGainSet) * SOUND_LEVEL_NO);
	MMI_ReadNvramDataChunk(gMMI_mem_micGainSetSCOWB, gMMI_nvram.micGainSetSCOWB, sizeof(MicGainSetSCOWB) * SOUND_LEVEL_NO);
	#endif

	#if USING_DIRECT_NVRAM_MICGAINSET_UNDER_A2DP_MODE
	#else
	MMI_ReadNvramDataChunk(gMMI_mem_micGainSet_under_a2dp_mode, gMMI_nvram.micGainSetA2DP, sizeof(MicGainSetA2DP) * SOUND_LEVEL_NO);
	#endif

	#if USING_DIRECT_NVRAM_MISC_TYPE
	#else
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_discoverableIfPdlLessThan, &gMMI_nvram.discoverableIfPdlLessThan, sizeof(U8));
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_maxPageDevice, &gMMI_nvram.maxPageDevice, sizeof(U8));
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_connection, &gMMI_nvram.mmiFeatureCtl.Connection, sizeof(U16));
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_pnp_product_id, &gMMI_nvram.productType.pnpProductID, sizeof(U16));
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_pnp_product_ver, &gMMI_nvram.productType.pnpProductVersion, sizeof(U16));
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_audio_priority, &gMMI_nvram.audioPriority.priority, sizeof(U8)*AUDIO_DEVICE_NUM);
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_feature_ctl_audio3, &gMMI_nvram.mmiFeatureCtl.Audio3, sizeof(U16));
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_aux_fs_sel, &gMMI_nvram.airPara.AUX_FS_SEL, sizeof(U8));
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_feature_ctl_a2dp, &gMMI_nvram.mmiFeatureCtl.A2DP, sizeof(U16));
	MMI_ReadNvramDataChunk((U8 XDATA_PTR)&gMMI_simple_pairing_para, &gMMI_nvram.simplePairingPara, sizeof(SimplePairingPara)*1);
	#endif

}

PUBLIC void MMI_SECTOR_MmiDriverNvramInit(void)
{
	U16 sectorSize = MMI_GetMmiDriverNvramSectorSize();
	MMI_DRIVER_VARIATION_HEAD_STRU CODE_PTR ptr1 = (MMI_DRIVER_VARIATION_HEAD_STRU CODE_PTR)GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT);
	MMI_DRIVER_VARIATION_HEAD_STRU CODE_PTR ptr2 = (MMI_DRIVER_VARIATION_HEAD_STRU CODE_PTR)GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT2);

	if(ptr1->CheckValid == MMI_SECTOR_DATA_VALID)
	{
		OSMEM_memcpy_xdata_code ((U8 XDATA_PTR) &gMMI_driver_variation_nvram.misc_para,(U8 CODE_PTR) ptr1->Para,sectorSize);
	}
	else if(ptr2->CheckValid == MMI_SECTOR_DATA_VALID)
	{
		OSMEM_memcpy_xdata_code ((U8 XDATA_PTR) &gMMI_driver_variation_nvram.misc_para,(U8 CODE_PTR) ptr2->Para,sectorSize);
	}
	else
	{
		MMI_ResetMmiDriverNvramValue();
		SaveMmiNvramTo4kbFlash();
	}

	LM_SetLocalNamePtr( &gMMI_driver_variation_nvram.misc_para.init.local_name );
	SYS_WriteEIRResponseName(&gMMI_driver_variation_nvram.misc_para.init.local_name.name[0], gMMI_driver_variation_nvram.misc_para.init.local_name.len);
}

PUBLIC U8 CODE_PTR MMI_SECTOR_GetValidCustormerSectorAddr(void)
{
	U8 CODE_PTR ptr;
	if(((MMI_DRIVER_VARIATION_HEAD_STRU CODE_PTR)GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT))->CheckValid == MMI_SECTOR_DATA_VALID)
	{
		ptr = (U8 CODE_PTR)&((MMI_CUSTOMIZE_DATA_STRU CODE_PTR)GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_CUSTOMIZE_V_NVRAM_INIT))->mmi_customize_data;
	}
	else if(((MMI_DRIVER_VARIATION_HEAD_STRU CODE_PTR)GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT2))->CheckValid == MMI_SECTOR_DATA_VALID)
	{
		ptr = (U8 CODE_PTR)&((MMI_CUSTOMIZE_DATA_STRU CODE_PTR)GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_CUSTOMIZE_V_NVRAM_INIT))->mmi_customize_data;
	}
	else
	{
		ptr = (U8 CODE_PTR)NULL;
	}
	return ptr;
}


PUBLIC void MMI_SECTOR_SaveData(U8 writeType)
{
	if(!gSYS_allow_updating || gSYS_update_by_uart_flash_command)
		return;

	if(!MMI_NEEDS_TO_UPDATE_NVRAM())
		goto MMI_CHECK_RESET;

	#ifdef MMI_LE_ENABLE
	if (!MMI_LE_AllowUpdateFlash())
        return;
    #endif

	switch(writeType)
	{
		case SAVE_DATA_TYPE_RUNTIME:
			if(rs_is_ongoing || MMI_HCI_IsInquiryOnGoing() || gMMI_ctl.page_ongoing)
				return;
			break;
		case SAVE_DATA_TYPE_DSP_OFF:
			if (MMI_HCI_IsInquiryOnGoing() || gMMI_ctl.page_ongoing)
				return;
			while(rs_is_ongoing);//Please check this flag before programming flash. about 10ms at most
				goto MMI_WRITE_FLASH;
			break;
		case SAVE_DATA_TYPE_POWER_OFF:
			goto MMI_WRITE_FLASH;

		case SAVE_DATA_TYPE_SDK:
			if(rs_is_ongoing || MMI_HCI_IsInquiryOnGoing() || gMMI_ctl.page_ongoing)
				return;
			else
				goto MMI_WRITE_FLASH;
			break;

		default:
			ASSERT(FALSE);
			break;
	}

	if(gMMI_ctl.mmiTimerInfo.sysTimer.writeFlashTimer == (OST XDATA_PTR)NULL)
	{
		#if AB1520B_UPDATE_LINK_HISTORY_PERIODICALLY
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.writeFlashTimer, 20*ONE_SEC);
		#else
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.writeFlashTimer, /*HALF_SEC*/ ONE_SEC);
		#endif
	}
	else if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.writeFlashTimer) || MMI_DRV_IsReset())
	{
		MMI_WRITE_FLASH:
		if (!SaveMmiNvramTo4kbFlash())
		    return;
		#ifdef AIR_MODULE
		MMI_AIR_SaveData();
		#endif
		MMI_PushMediaEvent(MEDIA_EVT_RESET_COMPLETE);

		MMI_SECTOR_SetUpdateFlag(FALSE);

		MMI_CHECK_RESET:
		if(MMI_DRV_IsReset())
		{
			MMI_DRV_SetReset(FALSE);
			if(MMI_UKEY_SWITCH_ONOFF_TYPE_FEAT  && MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
			{
				return;
			}

			MMI_Reset();
		}
	}
}

PRIVATE U8 MMI_SECTOR_LinkDataSum(U8 XDATA_PTR linkDataPtr)
{
	U8 i;
	U8 Sum = 0;

	if(linkDataPtr == NULL)
		return 0xFF;

	for(i = 0; i < sizeof(HciLinkKeyNtf); i++)
		Sum += *(linkDataPtr + i);

	return Sum;
}

PRIVATE U8 MMI_SECTOR_LinkInfoSum()
{
	U8 i;
	U8 airInfo, fcdInfo, agInfo;

	airInfo = fcdInfo = agInfo = 0;

	for(i = 0; i < NVRAM_AIR_LINK_KEY_NO; i++)
		airInfo += MMI_SECTOR_LinkDataSum((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info[i].linkData);

	fcdInfo += MMI_SECTOR_LinkDataSum((U8 XDATA_PTR)&gMMI_driver_variation_nvram.fast_dongle_link_history_info.linkData);

	for(i = 0; i < NVRAM_LINK_KEY_ITEMS; i++)
		agInfo += MMI_SECTOR_LinkDataSum((U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info[i].linkData);

	return (airInfo + fcdInfo + agInfo);
}

PRIVATE U8 MMI_SECTOR_LinkA2dpSoundLevelSum()
{
	U8 i;
	U8 airInfo, fcdInfo, agInfo;

	airInfo = fcdInfo = agInfo = 0;

	for(i = 0; i < NVRAM_AIR_LINK_KEY_NO; i++)
		airInfo += gMMI_driver_variation_nvram.air_link_history_info[i].a2dpSoundLevel;

	fcdInfo += gMMI_driver_variation_nvram.fast_dongle_link_history_info.a2dpSoundLevel;

	for(i = 0; i < NVRAM_LINK_KEY_ITEMS; i++)
		agInfo += gMMI_driver_variation_nvram.link_history_info[i].a2dpSoundLevel;

	return (airInfo + fcdInfo + agInfo);
}

PUBLIC void MMI_SECTOR_ResetPairedDevices(void)
{
	if(gMMI_ctl.driverCtl.resetLinkType == 0xFF)
	{
		MMI_ResetMmiDriverNvramValue();
	}
	else
	{
		if(gMMI_ctl.driverCtl.resetLinkType & MMI_RESET_FAST_DONGLE_LINK_INFO)
		{
			MMI_SECTOR_ResetFastLinkInfo();
		}
		if(gMMI_ctl.driverCtl.resetLinkType & MMI_RESET_AIR_LINK_INFO)
		{
			MMI_SECTOR_ResetAirLinkInfo();
		}
		if(gMMI_ctl.driverCtl.resetLinkType & MMI_RESET_LINK_INFO)
		{
			MMI_SECTOR_ResetLinkInfo();
		}
	}

	gMMI_ctl.driverCtl.resetLinkType = 0;

	LightDbgPrint("LinkInfo:%x Vol:%d Lang:%d", (U8)MMI_SECTOR_LinkInfoSum(),
												(U8)MMI_SECTOR_LinkA2dpSoundLevelSum(),
												(U8)gMMI_driver_variation_nvram.misc_para.init.vpLangIndex);
	LightDbgPrint("PEQ Mode: A2DP[%d] LineIn[%d]", (U8)gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_A2DP],
													(U8)gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_LINE_IN]);

	MMI_SECTOR_SetUpdateFlag(TRUE);
	if (MMI_REBOOT_ON_RESET_LINK_HISTORY_FEAT)
		MMI_DRV_SetReset(TRUE);
}

PUBLIC void MMI_SECTOR_SetUpdateFlag(BOOL isUpdate)
{
	if(isUpdate)
	{
		MMI_UPDATE_NVRAM(TRUE);
	}
	else
	{
		MMI_UPDATE_NVRAM(FALSE);
		SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.writeFlashTimer);
	}
}

PUBLIC BOOL MMI_SECTOR_WriteDeviceName(U8 XDATA_PTR namePtr, U8 len)
{
	if(len > RC_LOCAL_NAME_LENGTH)
		return FALSE;
	else
	{
		MMI_SECTOR_SetUpdateFlag(TRUE);
		OSMEM_memset_xdata((U8 XDATA_PTR)gMMI_driver_variation_nvram.misc_para.init.local_name.name, 0, gMMI_driver_variation_nvram.misc_para.init.local_name.len);
		OSMEM_memcpy_xdata_xdata(gMMI_driver_variation_nvram.misc_para.init.local_name.name, namePtr,
									(gMMI_driver_variation_nvram.misc_para.init.local_name.len = len));
		SYS_WriteEIRResponseName(namePtr, len);
		return TRUE;
	}
}


PUBLIC void MMI_DisableLcHwInt(void)
{
    U8 i;

	LC_ENABLE_SAC_SCAN(0);

    for (i=0;i<MAX_NUM_ACL_LINK;i++)
    {
        U8 sac_idx;
        U16 temp_interr;

        sac_idx = gLC_ctl.conn_idx_2_sac_idx[i];
        LC_SAC_GET_INTERRUPT_ENABLE(sac_idx,gLC_ctl.LinkIntrRecover[i]);
        temp_interr = gLC_ctl.LinkIntrRecover[i] &
            (~(LC_SAC_INTERRUPT_ENABLE_ESCO|LC_SAC_INTERRUPT_ENABLE_SNIFF|LC_SAC_INTERRUPT_ENABLE_SERVICE
            |LC_SAC_INTERRUPT_ENABLE_LARGE_RX_FIFO_EMPTY
            |LC_SAC_INTERRUPT_ENABLE_AFH_MEASURMENT|LC_SAC_INTERRUPT_ENABLE_HOLD));
        LC_SAC_SET_INTERRUPT_ENABLE(sac_idx,temp_interr);
    }
	gLC_ctl.LinkIntrRecover3 = LC_SFR_READ_BY_E7 (E7OFFS_RW_SAC_INTERRUPT_ENABLE3);
    LC_SFR_WRITE_BY_E7(E7OFFS_RW_SAC_INTERRUPT_ENABLE3, 0);
}

PUBLIC void MMI_EnableLcHwInt(void)
{
    U8 i;
    for (i=0;i<MAX_NUM_ACL_LINK;i++)
    {
        U8 sac_idx;

        sac_idx = gLC_ctl.conn_idx_2_sac_idx[i];
        LC_SAC_SET_INTERRUPT_ENABLE(sac_idx,gLC_ctl.LinkIntrRecover[i]);
    }

	LC_SFR_WRITE_BY_E7(E7OFFS_RW_SAC_INTERRUPT_ENABLE3, gLC_ctl.LinkIntrRecover3);
	LC_ENABLE_SAC_SCAN(4);
}

#if (defined AB1520S) || (defined AB1520U)
TWO_BYTE_UNION_TYPE Wire3_0x1E;
U8 Wire3_0x1E_18 = 0x03;
#endif

PUBLIC U8 MMI_EnterFlashProgramWorkaround(void)
{
    U8 rf_rxon_time;

    while(1)
    {
        U8 rx_status;

	    OS_ENTER_CRITICAL();
        rx_status = LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & 0x40;
        if(gLC_ctl.SW_Mode_Going && rx_status)
        {
            if(gLCS_swc_current_state==(VFUN CODE_PTR)gLCS_Standby)
            {
                break;
            }
            else
            {
	            OS_EXIT_CRITICAL();
	            continue;
            }
        }
        else
        {
            break;
        }
    }
	BANK_SFR_READ(C2,C2OFFS_RW_RF_RXON_TIME,rf_rxon_time);
	BANK_SFR_WRITE(C2,C2OFFS_RW_RF_RXON_TIME,250);
    MMI_DisableLcHwInt();

#if (defined AB1520S) || (defined AB1520U)
	//Block vlotage = 1.8V
	SYS_3WireRead(0x1E, Wire3_0x1E.value);
	SYS_3WireWrite(0x1E,Wire3_0x1E.in_byte.B1,(Wire3_0x1E.in_byte.B0 & 0xF8 | Wire3_0x1E_18));
	OS_delay_us(66);	
#endif	
    return rf_rxon_time;
}

PUBLIC void MMI_ExitFlashProgramWorkaround(U8 rf_rxon_time)
{
    MMI_EnableLcHwInt();
	OS_EXIT_CRITICAL();
	while(LC_CURR_USED_RX_BUFF_NUM());
	BANK_SFR_WRITE(C2,C2OFFS_RW_RF_RXON_TIME,rf_rxon_time);

#if (defined AB1520S) || (defined AB1520U)	
	//Block vlotage = 1.5V
	SYS_3WireWrite(0x1E,Wire3_0x1E.in_byte.B1,Wire3_0x1E.in_byte.B0);
	OS_delay_us(66);
#endif	
}

PUBLIC void MMI_SaveMmiDriverNvramToFlash(U8 CODE_PTR flashAddr)
{
	U8 count = 5;
	U16 length = MMI_GetMmiDriverNvramSectorSize();

	gMMI_driver_variation_nvram.CheckValid = 0xFF;

	do
	{
		if(!FLASH_PROGRAM_WriteXDataToFlash(flashAddr, (U8 XDATA_PTR)&gMMI_driver_variation_nvram, length))
		{
			continue;
		}
		break;
	}while (--count);

	LightDbgPrint("MMI-SaveToFlashRetry %d", (U8)(5 - count));
}

VOID UpdateValidSector(U32 ValidSectorAddr, U32 InvalidSectorAddr)
{
    U8 LDATA check_valid;
	U8 count = 5;

	do
    {
    	//verify ok, then update check_byte to valid
    	check_valid = MMI_SECTOR_DATA_VALID;
    	if(fFlashDrv->ByteProgram(ValidSectorAddr, &check_valid, 1 ))
    	{
    		continue;
    	}

    	//verify ok, then update another check_byte to invalid
    	check_valid = MMI_SECTOR_DATA_INVALID;
    	FLASH_PROGRAM_UnlockSector(InvalidSectorAddr);

    	if(fFlashDrv->ByteProgram(InvalidSectorAddr, &check_valid, 1))
    	{
    		continue;
    	}
		break;
	} while (--count);

	LightDbgPrint("MMI-UpdateValidSectorRetry %d", (U8)(5 - count));

	HC_SFR_WRITE_BY_FB(FBOFFS_WO_MCU_CACHE_FLUSH, 1);
}

PRIVATE U8 SaveMmiNvramTo4kbFlash(void)
{
	U8 CODE_PTR ptr1;
	U8 CODE_PTR ptr2;
	U32	Addr_FlashSectorCanWr;
	U32 Addr_FlashSectorOri;
	U8 gMMI_rf_rxon_time;
	#ifdef LE_SUPPORTED_HOST_FLASH
    U8 XDATA_PTR pLeNvram;
	U8 XDATA_PTR pLeNvram_4bAlign;
	#endif
    #ifdef DUAL_MIC_ENABLE
	U8 XDATA_PTR pDualMic;
	U8 XDATA_PTR pDualMic_4bAlign;
	#endif

    //- If there are enough memory
	#ifdef LE_SUPPORTED_HOST_FLASH
    if ((pLeNvram = OSMEM_Get(&gAlignOSMEM.OSMEM2_small_TX)) == NULL)
    {
	    return FALSE;
	}
	pLeNvram_4bAlign = OS_AlignToNext4ByteAddr(pLeNvram);
	#endif

    #ifdef DUAL_MIC_ENABLE
	if ((pDualMic = OSMEM_Get(&gAlignOSMEM.OSMEM2_small_TX)) == NULL)
	{
	    #ifdef LE_SUPPORTED_HOST_FLASH
	    OSMEM_Put(pLeNvram);
	    #endif
	    return FALSE;
	}
	pDualMic_4bAlign = OS_AlignToNext4ByteAddr(pDualMic);
	#endif

	if(api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_ALLOC_PROG_MEM])
	{
		if(!((API_IOSYS_NVRAM_ALLOC_PROG_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_ALLOC_PROG_MEM])())
		{
			#ifdef LE_SUPPORTED_HOST_FLASH
	    	OSMEM_Put(pLeNvram);
	    	#endif
			#ifdef DUAL_MIC_ENABLE
			OSMEM_Put(pDualMic);
			#endif
			return FALSE;
		}
	}

    //- HW workaround: Let RF can't receive packet
    gMMI_rf_rxon_time = MMI_EnterFlashProgramWorkaround();

	ptr1 = GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT);
	ptr2 = GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT2);

    //- Check valid
	if(*ptr1 == MMI_SECTOR_DATA_VALID)
	{
	    Addr_FlashSectorCanWr = FLASH_MEMORY_MAP(ptr2);
	    Addr_FlashSectorOri = FLASH_MEMORY_MAP(ptr1);

		#ifdef LE_SUPPORTED_HOST_FLASH
	    FLASH_PROGRAM_PageReadConti(
            FLASH_MEMORY_MAP(GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_LE_V_NVRAM_INIT)),
            pLeNvram_4bAlign,
            (U16)GET_SECTOR_MERGE_RUNTIME_1_LEN(SECTOR_MMI_LE_V_NVRAM_INIT));
		#endif

	    #ifdef DUAL_MIC_ENABLE
	    FLASH_PROGRAM_PageReadConti(
            FLASH_MEMORY_MAP(GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_DUAL_MIC_DATA)),
            pDualMic_4bAlign,
            (U16)GET_SECTOR_MERGE_RUNTIME_1_LEN(SECTOR_DUAL_MIC_DATA));
        #endif

	    FLASH_PROGRAM_UnlockSector(Addr_FlashSectorCanWr);

	    fFlashDrv->SectorErase(Addr_FlashSectorCanWr);

		#ifdef DUAL_MIC_ENABLE
	    AUDIO_SaveDataToFlash(
	        GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_DUAL_MIC_DATA2),
	        pDualMic_4bAlign);
		#endif

        #ifdef LE_SUPPORTED_HOST_FLASH
		LIC_ImmSaveToFlash(
	        GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_LE_V_NVRAM_INIT2),
	        pLeNvram_4bAlign);
        #endif

		if(api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_UPDATE])
		{
			((API_IOSYS_NVRAM_UPDATE_CB)api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_UPDATE])(GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_CUSTOMIZE_V_NVRAM_INIT2));
		}
		MMI_SaveMmiDriverNvramToFlash(ptr2);
	}
	else
	{
	    Addr_FlashSectorCanWr = FLASH_MEMORY_MAP(ptr1);
	    Addr_FlashSectorOri = FLASH_MEMORY_MAP(ptr2);

		#ifdef LE_SUPPORTED_HOST_FLASH
	    FLASH_PROGRAM_PageReadConti(
            FLASH_MEMORY_MAP(GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_LE_V_NVRAM_INIT2)),
            pLeNvram_4bAlign,
            (U16)GET_SECTOR_MERGE_RUNTIME_2_LEN(SECTOR_MMI_LE_V_NVRAM_INIT2));
		#endif

	    #ifdef DUAL_MIC_ENABLE
	    FLASH_PROGRAM_PageReadConti(
            FLASH_MEMORY_MAP(GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_DUAL_MIC_DATA2)),
            pDualMic_4bAlign,
            (U16)GET_SECTOR_MERGE_RUNTIME_2_LEN(SECTOR_DUAL_MIC_DATA2));
    	#endif

    	FLASH_PROGRAM_UnlockSector(Addr_FlashSectorCanWr);

	    fFlashDrv->SectorErase(Addr_FlashSectorCanWr);

		#ifdef DUAL_MIC_ENABLE
	    AUDIO_SaveDataToFlash(
	        GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_DUAL_MIC_DATA),
	        pDualMic_4bAlign);
		#endif

        #ifdef LE_SUPPORTED_HOST_FLASH
		LIC_ImmSaveToFlash(
	        GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_LE_V_NVRAM_INIT),
	        pLeNvram_4bAlign);
        #endif

		if(api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_UPDATE])
		{
			((API_IOSYS_NVRAM_UPDATE_CB)api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_UPDATE])(GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_CUSTOMIZE_V_NVRAM_INIT));
		}
		MMI_SaveMmiDriverNvramToFlash(ptr1);
	}

	UpdateValidSector(Addr_FlashSectorCanWr, Addr_FlashSectorOri);

	MMI_ExitFlashProgramWorkaround(gMMI_rf_rxon_time);

	FLASH_PROGRAM_LockAll();

	#ifdef LE_SUPPORTED_HOST_FLASH
    LIC_UpdateFlashAddr();
    OSMEM_Put(pLeNvram);
	#endif
	
	#ifdef DUAL_MIC_ENABLE
	OSMEM_Put(pDualMic);
	#endif

	return TRUE;
}

PRIVATE void MMI_ResetMmiDriverNvramValue(void)
{
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_driver_variation_nvram, 0, sizeof(MMI_DRIVER_VARIATION_NVRAM_TYPE));
	OSMEM_memcpy_xdata_code ((U8 XDATA_PTR) &gMMI_driver_variation_nvram.misc_para.init, (U8 CODE_PTR)&((MMI_DRIVER_NVRAM_BACKUP_TYPE CODE_PTR)GET_SECTOR_CFG1_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_BACKUP))->init, sizeof (DRIVER_BACKUP_NVRAM_TYPE));
}

PRIVATE void MMI_SECTOR_ResetAirLinkInfo(void)
{
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_driver_variation_nvram.air_link_history_info, 0, NVRAM_AIR_LINK_KEY_NO * sizeof(LinkHistoryType));
}

PRIVATE void MMI_SECTOR_ResetFastLinkInfo(void)
{
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_driver_variation_nvram.fast_dongle_link_history_info, 0, sizeof(LinkHistoryType));
}

PRIVATE void MMI_SECTOR_ResetLinkInfo(void)
{
	OSMEM_memset_xdata((U8 XDATA_PTR)&gMMI_driver_variation_nvram.link_history_info, 0, gMMI_nvram.storedLinkKeyCnt * sizeof(LinkHistoryType));
}

PRIVATE U16 MMI_GetMmiDriverNvramSectorSize(void)
{
	return (U16)(sizeof(MISC_PARA_TYPE) + (gMMI_nvram.storedLinkKeyCnt + NVRAM_AIR_LINK_KEY_NO + 1) *sizeof(LinkHistoryType));
}

