#define _LIC_HAL_C_

#include "lic_hal.h"

#ifdef LE_SUPPORTED_HOST_FLASH

#include "drv_program_flash_flash.h"

#ifdef AB1600
PUBLIC void LIC_UpdateNvramAddr(U8 idx, HCI_LE_EVT_STRU XDATA_PTR msgPtr)
{
	U8 XDATA_PTR ptr;
	NVRAM_MMI_LINK_RECORD_TYPE XDATA_PTR ptr2;
    U8 startIdx;

    if((ptr = SYS_MemoryGet(MMI_NVRAM_PAGE_SIZE)) == NULL)
    {
        return;
    }
    startIdx = idx & 0xFC;   //4 link data store in a page flash (512)
	OSMEM_memcpy_xdata_xdata(ptr, (U8 XDATA_PTR)&gNvram_MMI_link_record[startIdx], MMI_NVRAM_PAGE_SIZE);
    idx &= 0x03;
    ptr2 = (NVRAM_MMI_LINK_RECORD_TYPE XDATA_PTR)(ptr + (sizeof(NVRAM_MMI_LINK_RECORD_TYPE) * idx));

    OSMEM_memset_xdata((U8 XDATA_PTR)ptr2, 0, sizeof(NVRAM_MMI_LINK_RECORD_TYPE));
	OSMEM_memcpy_xdata_xdata(ptr2->peerAddr, msgPtr->payload.connCmp.peerAddr, BD_ADDR_LEN);
    //to do save ptr to flash
    //to do update gNvram_MMI_variation.nextRecordIdx
    OSMEM_Put(ptr);
}

PUBLIC void LIC_HalInit(void)
{
}

#else

#include "sector_merge_runtime.h"
#include "drv_flash.h"
#include "drv_program_flash.h"
#include "LC.h"
#include "..\MMI\MMI_Interface.h"
#include "..\SDK_Proxy\SDK_Proxy.h"

extern VFUN XDATA api_iosys_evt_func[API_GRP_IOSYS_CB_CNT];

#ifdef DUAL_MIC_ENABLE
extern PUBLIC void AUDIO_SaveDataToFlash(U8 CODE_PTR pValidSector, U8 XDATA_PTR dataPtr);
#endif
extern PUBLIC void MMI_SaveMmiDriverNvramToFlash(U8 CODE_PTR flashAddr);
extern PUBLIC void UpdateValidSector(U32 ValidSectorAddr, U32 InvalidSectorAddr);

PUBLIC void LIC_UpdateFlashAddr(void)
{
    U8 CODE_PTR ptr1 = GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT);
	U8 CODE_PTR ptr2 = GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT2);

	if(*ptr1 == MMI_SECTOR_DATA_VALID)
	{
		gMMI_LE_v_nvram_read_ptr = (NVRAM_MMI_LINK_DATA_TYPE CODE_PTR)GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_LE_V_NVRAM_INIT);
	}
	else if(*ptr2 == MMI_SECTOR_DATA_VALID)
	{
		gMMI_LE_v_nvram_read_ptr = (NVRAM_MMI_LINK_DATA_TYPE CODE_PTR)GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_LE_V_NVRAM_INIT2);
	}
}

PUBLIC U8 LIC_GetLatestFlashIdx(void)
{
    U8 fIdx;
    if(gLIC_hal_ctl.nextRecordIdx != 0)
    {
        fIdx = gLIC_hal_ctl.nextRecordIdx-1;
    }
    else
    {
        fIdx = MAX_NUM_LE_LINK-1;
    }
    //LightDbgPrint("LIC_GetLatestFlashIdx:%x",(U8)fIdx);
    return fIdx;
}

PUBLIC U8 LIC_GetFlashIdxByAddr(U8 XDATA_PTR addrPtr, U8 addrType)
{
    U8 i;
    for(i = 0; i < MAX_NUM_LE_LINK; i++)
    {
        if(gLIC_hal_ctl.reservedUpdataPtr == NULL)
        {
    		if(SYS_ValueCompare(addrPtr, NVRAM_MMI_V_LINK_RECORD(i).peerAddr, BD_ADDR_LEN) && (addrType == NVRAM_MMI_V_LINK_RECORD(i).peerAddrType))
    		{
				#ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
                LightDbgPrint("LIC_GetFlashIdxByAddr_N:%x",(U8)i);
				#endif
    		    return i;
    		}
        }
        else
        {
            NVRAM_MMI_LINK_RECORD_TYPE XDATA_PTR ptr;
            ptr = &(((NVRAM_MMI_LINK_DATA_TYPE XDATA_PTR)gLIC_hal_ctl.reservedUpdataPtr)->nvram_MMI_link_record[i]);
    		if(SYS_ValueCompare(addrPtr, ptr->peerAddr, BD_ADDR_LEN) && (addrType == ptr->peerAddrType))
    		{
				#ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
                LightDbgPrint("LIC_GetFlashIdxByAddr_P:%x",(U8)i);
				#endif
    		    return i;
    		}
        }
    }
    #ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
    LightDbgPrint("No_FlashIdx!!");
	#endif
    return 0xFF;
}

PUBLIC void LIC_SaveToFlash(void)
{
    if(gLIC_hal_ctl.reservedUpdataPtr == NULL)
    {
	    LightDbgPrint("USR - LIC_SaveToFlash err null");
        return;
    }

    gLIC_hal_ctl.miscMask |= LIC_UPDATE_FLASH;

    if(gLIC_hal_ctl.miscMask & LIC_UPDATE_FLASH_LATER)
    {
		//LightDbgPrint("USR - LIC_SaveToFlash need update!");
        gLIC_hal_ctl.miscMask &= ~LIC_UPDATE_FLASH_LATER;
    }
}
#ifdef LE_SUPPORTED_HOST_FLASH
PUBLIC void LIC_ImmSaveToFlash(U8 CODE_PTR pValidSector, U8 XDATA_PTR pLeNvram)
{
	U8 count = 5;

    do 
    {
    	if(!FLASH_PROGRAM_WriteXDataToFlash(pValidSector, pLeNvram, sizeof(NVRAM_MMI_LINK_DATA_TYPE)))
    	{
    		#ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
    	    LightDbgPrint("LIC-FailSaveImmToFlash");
			#endif
    		continue;
    	}
        break;
	} while (--count);

	LightDbgPrint("LIC-SaveToFlashRetry %d", (U8)(5 - count));
}

PUBLIC void SaveLeNvramTo4kbFlash(void)
{
	U8 CODE_PTR ptr1;
	U8 CODE_PTR ptr2;
	U32	Addr_FlashSectorCanWr;
	U32 Addr_FlashSectorOri;
	U8 gMMI_rf_rxon_time;
	U8 XDATA_PTR pLeNvram;
	U8 XDATA_PTR pLeNvram_4bAlign;
	#ifdef DUAL_MIC_ENABLE
	U8 XDATA_PTR pDualMic;
	U8 XDATA_PTR pDualMic_4bAlign;
	#endif

    if ((pLeNvram = OSMEM_Get(&gAlignOSMEM.OSMEM2_small_TX)) == NULL)
	    return;

    #ifdef DUAL_MIC_ENABLE
	if ((pDualMic = OSMEM_Get(&gAlignOSMEM.OSMEM2_small_TX)) == NULL)
	{
		OSMEM_Put(pLeNvram);
	    return;
	}
	pDualMic_4bAlign = OS_AlignToNext4ByteAddr(pDualMic);
	#endif

	if(api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_ALLOC_PROG_MEM])
	{
		if(!((API_IOSYS_NVRAM_ALLOC_PROG_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_ALLOC_PROG_MEM])())
		{
	    	OSMEM_Put(pLeNvram);
			#ifdef DUAL_MIC_ENABLE
			OSMEM_Put(pDualMic);
	    	#endif
			return;
		}
	}

	pLeNvram_4bAlign = OS_AlignToNext4ByteAddr(pLeNvram);

	OS_memcpy(pLeNvram_4bAlign, gLIC_hal_ctl.reservedUpdataPtr,sizeof(NVRAM_MMI_LINK_DATA_TYPE));

	gMMI_rf_rxon_time = MMI_EnterFlashProgramWorkaround();

	ptr1 = GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT);
	ptr2 = GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_DRIVER_V_NVRAM_INIT2);

	if(*ptr1 == MMI_SECTOR_DATA_VALID)
	{
	    Addr_FlashSectorCanWr = FLASH_MEMORY_MAP(ptr2);
	    Addr_FlashSectorOri = FLASH_MEMORY_MAP(ptr1);

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
		LIC_ImmSaveToFlash(
	        GET_SECTOR_MERGE_RUNTIME_2_ADDR(SECTOR_MMI_LE_V_NVRAM_INIT2),
	        pLeNvram_4bAlign);

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
		LIC_ImmSaveToFlash(
	        GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_MMI_LE_V_NVRAM_INIT),
	        pLeNvram_4bAlign);
			
		if(api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_UPDATE])
		{
			((API_IOSYS_NVRAM_UPDATE_CB)api_iosys_evt_func[API_GRP_IOSYS_CB_NVRAM_UPDATE])(GET_SECTOR_MERGE_RUNTIME_1_ADDR(SECTOR_CUSTOMIZE_V_NVRAM_INIT));
		}
		MMI_SaveMmiDriverNvramToFlash(ptr1);
	}

	UpdateValidSector(Addr_FlashSectorCanWr, Addr_FlashSectorOri);

	MMI_ExitFlashProgramWorkaround(gMMI_rf_rxon_time);

	FLASH_PROGRAM_LockAll();

    gLIC_hal_ctl.miscMask &= ~(LIC_UPDATE_FLASH);
	OSMEM_Put(gLIC_hal_ctl.reservedUpdataPtr);
	gLIC_hal_ctl.reservedUpdataPtr = NULL;
    LIC_UpdateFlashAddr();
	OSMEM_Put(pLeNvram);
	#ifdef DUAL_MIC_ENABLE
	OSMEM_Put(pDualMic);
	#endif
}
#endif

PUBLIC void LIC_FreeNvRamData(void)
{
    if(gLIC_hal_ctl.reservedUpdataPtr == NULL)
    {
	    LightDbgPrint("USR - LE_FreeNvRamData err null");
        return;
    }

    if(gLIC_hal_ctl.miscMask & LIC_UPDATE_FLASH)
    {
	    //LightDbgPrint("USR - LE_FreeNvRamData err mask");
        return;
    }

    if(gLIC_hal_ctl.miscMask & LIC_UPDATE_FLASH_LATER)
    {
		//LightDbgPrint("USR - LE_FreeNvRamData need update!");
        gLIC_hal_ctl.miscMask &= ~LIC_UPDATE_FLASH_LATER;
        gLIC_hal_ctl.miscMask |= LIC_UPDATE_FLASH;
        return;
    }

	//LightDbgPrint("USR - LE_FreeNvRamData!");
    OSMEM_Put(gLIC_hal_ctl.reservedUpdataPtr);
	gLIC_hal_ctl.reservedUpdataPtr = NULL;
}

/* must call LIC_FreeNvRamData() or LIC_SaveToFlash after use*/
PUBLIC NVRAM_MMI_LINK_DATA_TYPE XDATA_PTR LIC_GetNvRamData(void)
{
    if(gLIC_hal_ctl.reservedUpdataPtr == NULL)
    {
    	if((gLIC_hal_ctl.reservedUpdataPtr = (NVRAM_MMI_LINK_DATA_TYPE XDATA_PTR)SYS_MemoryGet(sizeof(NVRAM_MMI_LINK_DATA_TYPE))) != NULL)
    	{
    		OSMEM_memcpy_xdata_code((U8 XDATA_PTR)gLIC_hal_ctl.reservedUpdataPtr, &NVRAM_MMI_V_LINK_RECORD(0), sizeof(NVRAM_MMI_LINK_DATA_TYPE));
    	}
    	else
    	{
    		#ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
    		LightDbgPrint("USR - no mem");
    		#endif
    	}
    }
    else if(gLIC_hal_ctl.miscMask & LIC_UPDATE_FLASH)
    {
		//LightDbgPrint("USR - LE_GetNvRamData update later");
        gLIC_hal_ctl.miscMask &= ~(LIC_UPDATE_FLASH);
        gLIC_hal_ctl.miscMask |= LIC_UPDATE_FLASH_LATER;
    }
	return gLIC_hal_ctl.reservedUpdataPtr;
}

PUBLIC void LIC_UpdateNvramAddr(U8 idx, HCI_LE_EVT_STRU XDATA_PTR msgPtr)
{
	NVRAM_MMI_LINK_DATA_TYPE XDATA_PTR dataPtr;

	#ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
	LightDbgPrint("USR - addr :%x %x %x %x %x %x", (U8)msgPtr->payload.connCmp.peerAddr[0], (U8)msgPtr->payload.connCmp.peerAddr[1], (U8)msgPtr->payload.connCmp.peerAddr[2], (U8)msgPtr->payload.connCmp.peerAddr[3], (U8)msgPtr->payload.connCmp.peerAddr[4], (U8)msgPtr->payload.connCmp.peerAddr[5]);
	#endif

	if((dataPtr = LIC_GetNvRamData()) == NULL)
	{
		return;
	}

	// update addr and addrType
	OSMEM_memcpy_xdata_xdata(dataPtr->nvram_MMI_link_record[idx].peerAddr, msgPtr->payload.connCmp.peerAddr, BD_ADDR_LEN);
	dataPtr->nvram_MMI_link_record[idx].peerAddrType = msgPtr->payload.connCmp.peerAddrType;
	// update next index
	dataPtr->nextRecordIdx = ((idx + 1) % MAX_NUM_OF_LINK);
    gLIC_hal_ctl.nextRecordIdx = dataPtr->nextRecordIdx;
    LIC_SaveToFlash();
	//OSMEM_Put((U8 XDATA_PTR)dataPtr);

	#ifdef DBG_UART_ENABLE_LINK_INDEX_CTL
	LightDbgPrint("USR - updateNvram cmp!");
	LightDbgPrint("USR - addr :%x %x %x %x %x %x", (U8)NVRAM_MMI_V_LINK_RECORD(idx).peerAddr[0], (U8)NVRAM_MMI_V_LINK_RECORD(idx).peerAddr[1],(U8)NVRAM_MMI_V_LINK_RECORD(idx).peerAddr[2], (U8)NVRAM_MMI_V_LINK_RECORD(idx).peerAddr[3],(U8)NVRAM_MMI_V_LINK_RECORD(idx).peerAddr[4], (U8)NVRAM_MMI_V_LINK_RECORD(idx).peerAddr[5]);
	#endif
}

PUBLIC BOOL LIC_UpdateNvramParam(U8 XDATA_PTR paramPtr, U8 type)
{
	NVRAM_MMI_LINK_DATA_TYPE XDATA_PTR dataPtr;
	if((dataPtr = LIC_GetNvRamData()) == NULL)
	{
		return FALSE;
	}

	switch(type)
	{
        case LIC_FLASH_UPDATE_ADV_DATA:
        {
            U8 len;
            len = *paramPtr;
        	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&dataPtr->advData, paramPtr, len+1);
            if(len < MMI_LE_ADV_DATA_MAX_DATA_LEN)
            {
                OSMEM_memset_xdata((U8 XDATA_PTR)&dataPtr->advData.adv_data[len], 0, (MMI_LE_ADV_DATA_MAX_DATA_LEN - len));
            }
        }
        break;

        case LIC_FLASH_UPDATE_ADV_PARAM:
        {
            OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&dataPtr->advPara, paramPtr, sizeof(LE_ADV_PARA_TYPE));
        }
        break;

        case LIC_FLASH_UPDATE_CONN_SETTING:
        {
            OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&dataPtr->connSetting, paramPtr, sizeof(LE_CONN_SETTING_TYPE));
        }
        break;

        case LIC_FLASH_UPDATE_SCAN_RSP:
        {
            U8 len;
            len = *paramPtr;
        	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&dataPtr->scanRsp, paramPtr, len+1);
            if(len != MMI_LE_SCAN_RSP_MAX_DATA_LEN)
            {
                OSMEM_memset_xdata((U8 XDATA_PTR)&dataPtr->scanRsp.scan_rsp_data[len], 0, (MMI_LE_SCAN_RSP_MAX_DATA_LEN - len));
            }
        }
        break;

        case LIC_FLASH_UPDATE_VIBRATION_SETTING:
        {
            OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&dataPtr->vibrationSetting, paramPtr, VIBRATION_SETTING_LEN);
        }
        break;
	}

	LIC_SaveToFlash();
	//OSMEM_Put((U8 XDATA_PTR)dataPtr);
    return TRUE;
}

PUBLIC void LIC_HalInit(void)
{
    LIC_UpdateFlashAddr();
    gLIC_hal_ctl.nextRecordIdx = gMMI_LE_v_nvram_read_ptr->nextRecordIdx;
}

#endif /* LE_SUPPORTED */
#endif /* LE_SUPPORTED_HOST_FLASH */