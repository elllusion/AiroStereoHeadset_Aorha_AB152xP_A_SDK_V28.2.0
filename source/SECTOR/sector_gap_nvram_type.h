#ifndef _SECTOR_GAP_NVRAM_H_
#define _SECTOR_GAP_NVRAM_H_

	#include "bt_config_profile.h"
	#include "bt_types.h"
	#include <crc16.h>
	
	typedef struct
	{
		LAP_ARR GIAC_Lap;
		LAP_ARR LIAC_Lap;
		LAP_ARR LIAC_Lap_Special;
	}GAP_NVRAM_TYPE;
	
	typedef struct
	{
		GAP_NVRAM_TYPE gap_nvram;

		CRC16 CRC;
		
	} SECTOR_GAP_NVRAM_STRU;
	
#endif