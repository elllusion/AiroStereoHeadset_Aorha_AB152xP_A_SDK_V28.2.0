//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
// Init: N95
// Target: T9
//////////////////////////////////////////////////////////

#include "config.h"
#include "cpu.h"
#include "sector_mmi_nvram_type.h"
#include "sector_driver_ringtone_data.h"
#include "sector_mmi_driver_variation_nvram.h"
#include "..\Driver\VoicePrompt.h"

#pragma userclass (HCONST = CONFIG)


#ifdef BLUETOOTH_SPEAKER
	#ifdef SOUND_BAR
		#include "sample_speaker_soundbar.h"
	#elif defined SCO_WOOFER
		#include "sample_speaker_scowoofer.h"
	#elif defined TWS_SETTINGS
		#if defined PRODUCT_TYPE_A || defined PRODUCT_TYPE_K
			#include "air_stereo_A.h"
		#else
			#include "sample_speaker_tws.h"
		#endif
	#elif defined SAME_SW_DIFF_MODULE
		#include "sample_speaker_scowoofer.h"
	#else
		#include "sample_speaker.h"
	#endif
#else
	#ifdef TWS_SETTINGS
		#if defined PRODUCT_TYPE_A || defined PRODUCT_TYPE_K
			#include "air_stereo_A.h"
		#else
			#include "sample_speaker_tws.h"
		#endif
	#else
		#include "ForceLinkN95.h"
	#endif
#endif
