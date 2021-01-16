#ifndef _SECTOR_MERGE_RUNTIME_H_
#define _SECTOR_MERGE_RUNTIME_H_


#include "config.h"
#include "bt_config_profile.h"
#include "flash_descriptor.h"
#include "sectors.h"
#include "sector_mmi_driver_variation_nvram.h"
#include "sector_mmi_le_variation_nvram.h"
#include "sector_dual_mic_data.h"
#include "sector_mmi_customize_v_nvram.h"


EXTERN MMI_DRIVER_VARIATION_NVRAM_STRU CODE gMMI_Driver_v_nvram_init2;
EXTERN NVRAM_MMI_LINK_RECORD_STRU CODE gMMI_LE_v_nvram_init2;
EXTERN SECTOR_DUAL_MIC_DATA_STRU CODE gSector_DualMicData2;
EXTERN MMI_DRIVER_VARIATION_NVRAM_STRU CODE gMMI_Driver_v_nvram_init;
EXTERN NVRAM_MMI_LINK_RECORD_STRU CODE gMMI_LE_v_nvram_init;
EXTERN SECTOR_DUAL_MIC_DATA_STRU CODE gSector_DualMicData;
EXTERN MMI_CUSTOMIZE_DATA_STRU CODE gMMI_Customize_v_nvram_init;
EXTERN MMI_CUSTOMIZE_DATA_STRU CODE gMMI_Customize_v_nvram_init2;


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	SECTOR_NAME,
enum
{
	#include "sector_merge_runtime_member_1.h"
	NUM_OF_SECTOR_MERGE_RUNTIME_1
};

enum
{
	#include "sector_merge_runtime_member_2.h"
	NUM_OF_SECTOR_MERGE_RUNTIME_2
};

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_MERGE_RUNTIME_1];

    CRC16 CRC;

} SECTOR_MERGE_RUNTIME_HEADER_1_STRU;

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_MERGE_RUNTIME_2];

    CRC16 CRC;

} SECTOR_MERGE_RUNTIME_HEADER_2_STRU;


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_MERGE_RUNTIME_HEADER_1_STRU CODE gSectorRuntimeHeader_1;
EXTERN SECTOR_MERGE_RUNTIME_HEADER_2_STRU CODE gSectorRuntimeHeader_2;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GET_SECTOR_MERGE_RUNTIME_1_ADDR(A) ((SECTOR_MERGE_RUNTIME_HEADER_1_STRU CODE_PTR)gFlashDescriptor->pSectorHdrRuntime_1)->SectorInfo[A].addr
#define GET_SECTOR_MERGE_RUNTIME_1_LEN(A)  ((SECTOR_MERGE_RUNTIME_HEADER_1_STRU CODE_PTR)gFlashDescriptor->pSectorHdrRuntime_1)->SectorInfo[A].length
#define GET_SECTOR_MERGE_RUNTIME_2_ADDR(A) ((SECTOR_MERGE_RUNTIME_HEADER_2_STRU CODE_PTR)gFlashDescriptor->pSectorHdrRuntime_2)->SectorInfo[A].addr
#define GET_SECTOR_MERGE_RUNTIME_2_LEN(A)  ((SECTOR_MERGE_RUNTIME_HEADER_2_STRU CODE_PTR)gFlashDescriptor->pSectorHdrRuntime_2)->SectorInfo[A].length

#endif /* _SECTOR_MERGE_RUNTIME_H_ */