#ifndef _SECTOR_CONFIG1_H_
#define _SECTOR_CONFIG1_H_

#include "crc16.h"
#include "flash_descriptor.h"
#include "sectors.h"
#include "sector_sys_local_device_info.h"
#include "sector_sys_local_device_EIR.h"
#include "sector_sys_local_device_control.h"
#include "sector_lc_misc_ctl.h"
#include "sector_lm_param.h"
#include "sector_hc_parameter.h"
#include "sector_uart_parameter.h"
#include "sector_mcu_dsp_param.h"
#include "sector_a2dp_nvram_ctl.h"
#include "sector_driver_led_data.h"
#include "sector_driver_reserved_data.h"
#include "sector_driver_ringtone_data.h"
#include "sector_mmi_nvram_type.h"
#include "sector_mmi_nvram_keymap.h"
#include "sector_mmi_driver_variation_nvram.h"
#include "sector_sys_memory_config.h"
#include "sector_mmi_le_variation_nvram.h"
#include "sector_sm_nvram.h"
#include "sector_gap_nvram_type.h"
#include "sector_driver_ctl_type.h"
#ifdef AB1520B
#include "sector_mmi_le_nvram.h"
#endif
#include "sector_application.h"
#include "sector_iap2_type.h"
#include "sector_le_misc_ctl.h"
#ifndef AB1520B
#include "sector_sys_patch_code_init.h"
#endif
#include "sector_spi_config.h"


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	SECTOR_NAME,
enum
{
	#include "sector_config1_member.h"
	NUM_OF_SECTOR_CFG1
};

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_CFG1];

    CRC16 CRC;

} SECTOR_CFG1_HEADER_STRU;

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_CFG1_HEADER_STRU CODE gSectorConfig1Header;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GET_SECTOR_CFG1_ADDR(A) ((SECTOR_CFG1_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrConfig1)->SectorInfo[A].addr
#define GET_SECTOR_CFG1_LEN(A)  ((SECTOR_CFG1_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrConfig1)->SectorInfo[A].length

#endif /* _SECTOR_CONFIG1_H_ */