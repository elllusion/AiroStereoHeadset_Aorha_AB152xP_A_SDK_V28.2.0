#include "rc.h"
#include "bt_config_profile.h"

#pragma userclass (HCONST = DSP_ROM)

#define ENDIAN_RVRS(A)	(((U16)((A) & 0x00FF) << 8) | ((U16)(A) >> 8))

#ifdef DSP_ENABLE

#ifdef SUPPORT_VOICE_PROMPT
	#if VP_NB_HQ_ENABLE
		#include "VoicePromptTableNBHQ.c"
	#endif
	#if VP_NB_LQ_ENABLE
		#include "VoicePromptTableNBLQ.c"
	#endif
#endif

#endif //DSP_ENABLE
