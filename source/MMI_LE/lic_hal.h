#ifndef __LIC_HAL_H__
#define __LIC_HAL_H__

#include "..\COMMON\le_common_hal.h"
#ifdef LE_SUPPORTED_HOST_FLASH

#ifdef AB1600
#define NVRAM_MMI_V_LINK_RECORD(IDX) (gNvram_MMI_link_record[IDX])
#define NVRAM_MMI_V_NEXT_RECORD_IDX
#else

#ifdef _LIC_HAL_C_
	#define LIC_HAL_EXTERN
#else
	#define LIC_HAL_EXTERN extern	
#endif

#include "..\SECTOR\sector_mmi_le_variation_nvram.h"
#include "bt_config_profile.h"

#ifdef SM_SUPPORTED
#include "..\SM\SM_interface.h"
#endif

//miscMask
#define LIC_UPDATE_FLASH                        0x01
#define LIC_UPDATE_FLASH_LATER					0x02

#define LIC_FLASH_UPDATE_ADV_DATA               0
#define LIC_FLASH_UPDATE_ADV_PARAM              1
#define LIC_FLASH_UPDATE_CONN_SETTING           2
#define LIC_FLASH_UPDATE_SCAN_RSP               3
#define LIC_FLASH_UPDATE_VIBRATION_SETTING      4

LIC_HAL_EXTERN NVRAM_MMI_LINK_DATA_TYPE CODE_PTR XDATA gMMI_LE_v_nvram_read_ptr;

#define NVRAM_MMI_V_LINK_RECORD(IDX) 				(gMMI_LE_v_nvram_read_ptr->nvram_MMI_link_record[IDX])
#define NVRAM_MMI_V_NEXT_RECORD_IDX 				(gMMI_LE_v_nvram_read_ptr->nextRecordIdx)
#define NVRAM_MMI_V_ADV_DATA                        (gMMI_LE_v_nvram_read_ptr->advData)
#define NVRAM_MMI_V_CONN_SETTING                    (gMMI_LE_v_nvram_read_ptr->connSetting)
#define NVRAM_MMI_V_VIBRATION_SETTING_PTR           (gMMI_LE_v_nvram_read_ptr->vibrationSetting)
#define NVRAM_MMI_V_LE_FEATURE_CTL		           	(gMMI_LE_v_nvram_read_ptr->leFeatureCtl)

//le feature definition
#define LE_FEATURE_AUTO_PAIRING						0x01

#define LE_FEATURE_AUTO_PAIRING_SUPPORTED			(NVRAM_MMI_V_LE_FEATURE_CTL & 0x01)
//end le feature definition

typedef struct
{
    U8 miscMask;
    U8 XDATA_PTR reservedUpdataPtr;
    U8 nextRecordIdx;
}LIC_HAL_CTL_TYPE;

LIC_HAL_EXTERN LIC_HAL_CTL_TYPE XDATA gLIC_hal_ctl;

PUBLIC void LIC_HalInit(void);
PUBLIC void LIC_SaveToFlash(void);
PUBLIC void LIC_ImmSaveToFlash(U8 CODE_PTR pValidSector, U8 XDATA_PTR pLeNvram);
PUBLIC BOOL LIC_UpdateNvramParam(U8 XDATA_PTR paramPtr, U8 type);
PUBLIC void LIC_FreeNvRamData(void);
PUBLIC NVRAM_MMI_LINK_DATA_TYPE XDATA_PTR LIC_GetNvRamData(void);   // must call LIC_FreeNvRamData() or LIC_SaveToFlash() after use
PUBLIC U8 LIC_GetFlashIdxByAddr(U8 XDATA_PTR addrPtr, U8 addrType);
PUBLIC U8 LIC_GetLatestFlashIdx(void);
#endif

PUBLIC void LIC_UpdateFlashAddr(void);
PUBLIC void LIC_UpdateNvramAddr(U8 idx, HCI_LE_EVT_STRU XDATA_PTR msgPtr);

#endif /* LE_SUPPORTED_HOST_FLASH */
#endif /* __LIC_HAL_H__ */

