#ifndef _SECTOR_MMI_CUSTOMIZE_V_NVRAM_H_
#define _SECTOR_MMI_CUSTOMIZE_V_NVRAM_H_

#include "config.h"
#include "cpu.h"
#include "crc16.h"
#include "bt_config_profile.h"

typedef struct
{
	U8 para[4][256];
} CUSTOMIZE_DATA_TYPE;

typedef struct
{
	CUSTOMIZE_DATA_TYPE mmi_customize_data;
	CRC16 CRC;
	
} MMI_CUSTOMIZE_DATA_STRU;

#endif /* _SECTOR_MMI_CUSTOMIZE_V_NVRAM_H_ */
