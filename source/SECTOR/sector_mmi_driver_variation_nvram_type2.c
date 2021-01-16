//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include <config.h>
#include <cpu.h>
#include "sector_mmi_driver_variation_nvram.h"

#pragma userclass (HCONST = MMI_V_NVRAM2)


PUBLIC MMI_DRIVER_VARIATION_NVRAM_STRU CODE gMMI_Driver_v_nvram_init2 = 
{
	{	
		MMI_SECTOR_DATA_INVALID,
		{
			//non-zero init variables;
			{
				MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_ISMISCENABLED,	// isMiscEnabled
				MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FM_HISTORY_FREQ,//Fm history
				MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FMHISTORYVOL, //FMHistoryVol
				MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FMSMALLSTEPVOL, //FMSmallStepVol
				MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_LINEINHISTORYVOL, // LineInHistoryVol
				MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_LINEINSMALLSTEPVOL, //LineInSmallStepVol
				MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_DEFAULTSCOSOUNDLEVEL,	//defaultSCOSoundLevel
				0, //smallStepSoundLevelIdle
				MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_VPLANGINDEX,	//vpLangIndex
				{	//local_name
					#ifdef SOUND_BAR
					8, //len
					{'S', 'O', 'U', 'N', 'D', 'B', 'A', 'R'},
					#elif defined SCO_WOOFER
					6, //len
					{'W', 'O', 'O', 'F', 'E', 'R'},
					#elif defined TWS_SETTINGS
						#ifdef PRODUCT_TYPE_A
							7, //len
							{'A', 'i', 'r', 'o', 'S', '-', 'A'},
						#elif defined PRODUCT_TYPE_K
							7, //len
							{'A', 'i', 'r', 'o', 'S', '-', 'K'},
						#else
							6, //len
							{'T', 'W', 'S', 'Y', 'O', 'Y'},
						#endif
					#elif defined FAST_CONNECT_DONGLE
					8, //len
					{'F', 'C', 'D', 'O', 'N', 'G', 'L', 'E'},
					#else
					6, //len
					{'A', 'I', 'R', 'O', 'H', 'A'},
					#endif
				},
			},

			{{0,0,0,0,0,0}, {0,0,0,0,0,0}}, //agInd[MAX_MULTI_POINT_NO];
			{0,0},		//PEQRecord
			{0,0},      //sectorMode 0->default PEQ, 1->custom PEQ
		},
		{
			{0, {{0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0},{0,0,0},0,0,0,0,0,0},
			{0, {{0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0},{0,0,0},0,0,0,0,0,0},
		}, //air_link_history_info
		{0, {{0x88, 0x99, 0xCC, 0x66, 0x00, 0x00}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0},{0,0,0},0,0,0,0,0,0},//fast_dongle_link_history_info
		{
			{0, {{0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0},{0,0,0},0,0,0,0,0,0},
			{0, {{0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0},{0,0,0},0,0,0,0,0,0},
			//{0, {{0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0},{0,0,0},0,0,0,0,0,0},
			//{0, {{0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0},{0,0,0},0,0,0,0,0,0},
		}, //link_history_info
	},
	{0,0} //CRC
};

