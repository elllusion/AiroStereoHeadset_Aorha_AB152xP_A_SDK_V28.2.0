//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
// Init: N95
// Target: T9
//////////////////////////////////////////////////////////

#include "config.h"
#include "cpu.h"
#include "sector_gap_nvram_type.h"

#pragma userclass (HCONST = CONFIG)


PUBLIC SECTOR_GAP_NVRAM_STRU CODE gGAP_nvram_init =
{
	{
		{0x33, 0x8B, 0x9E}, //GIAC_Lap
	#ifdef TWS_SETTINGS
		{0x11, 0x8B, 0x9E}, //LIAC_Lap
	#else
		{0x11, 0x8B, 0x9E}, //LIAC_Lap
	#endif
		{0x05, 0x8B, 0x9E}, //LIAC_Lap_Special
	},
	{0,0}, //CRC
};