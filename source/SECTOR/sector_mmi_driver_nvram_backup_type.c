//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#include <config.h>
#include <cpu.h>
#include "sector_mmi_driver_variation_nvram.h"

#pragma userclass (HCONST = CONFIG)


PUBLIC SECTOR_MMI_DRIVER_NVRAM_BACKUP_STRU CODE gMMI_Driver_v_nvram_backup =
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

    //- CRC
    {0,0}
};