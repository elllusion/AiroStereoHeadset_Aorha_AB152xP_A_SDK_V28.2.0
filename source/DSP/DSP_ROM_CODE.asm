;*
;*
;*
;*
;*
#include "config.h"
#include "bt_config_profile.h"

NAME DSP_ROM_CODE


#ifdef DSP_ENABLE

        ?FC?DSP_CODE_ARRAY?DSP SEGMENT 'HCONST_DSP_ROM'
RSEG    ?FC?DSP_CODE_ARRAY?DSP



$if AB1520S
    #include "AB1520S_DSP\DSP_ROM_AB1520S_TEXT.h"
    #include "AB1520S_DSP\DSP_ROM_AB1520S_TEXT2.h"
    #include "AB1520S_DSP\DSP_ROM_AB1520S_TEXT3.h"
    #include "AB1520S_DSP\DSP_ROM_AB1520S_TEXT4.h"
$else
    #ifdef LIGHTING_ENABLE
    #include "AB1520_DSP_Lighting\DSP_ROM_AB1520_TEXT.h"
    #include "AB1520_DSP_Lighting\DSP_ROM_AB1520_TEXT2.h"
    #include "AB1520_DSP_Lighting\DSP_ROM_AB1520_TEXT3.h"
    #include "AB1520_DSP_Lighting\DSP_ROM_AB1520_TEXT4.h"
    
    #elif defined CELT_MODULE
        $if SOUND_BAR
            #include "AB1520_DSP_CELT_Encode\DSP_ROM_AB1520_TEXT.h"
            #include "AB1520_DSP_CELT_Encode\DSP_ROM_AB1520_TEXT2.h"
            #include "AB1520_DSP_CELT_Encode\DSP_ROM_AB1520_TEXT3.h"
            #include "AB1520_DSP_CELT_Encode\DSP_ROM_AB1520_TEXT4.h"
            #include "AB1520_DSP_CELT_Encode\DSP_ROM_AB1520_TEXT5.h"
        $else
            #include "AB1520_DSP_CELT_Decode\DSP_ROM_AB1520_TEXT.h"
            #include "AB1520_DSP_CELT_Decode\DSP_ROM_AB1520_TEXT2.h"
            #include "AB1520_DSP_CELT_Decode\DSP_ROM_AB1520_TEXT3.h"
            #include "AB1520_DSP_CELT_Decode\DSP_ROM_AB1520_TEXT4.h"
            #include "AB1520_DSP_CELT_Decode\DSP_ROM_AB1520_TEXT5.h"
        $endif
    
    #else
        /* Standard Soundbar/Woofer DSP */
        $if (SCO_WOOFER || SOUND_BAR)
        #include "AB1520_DSP_SoundbarWoofer\DSP_ROM_AB1520_TEXT.h"
        #include "AB1520_DSP_SoundbarWoofer\DSP_ROM_AB1520_TEXT2.h"
        #include "AB1520_DSP_SoundbarWoofer\DSP_ROM_AB1520_TEXT3.h"
        #include "AB1520_DSP_SoundbarWoofer\DSP_ROM_AB1520_TEXT4.h"
        $else
        /* Standard Speaker/Headset DSP */
		#include "AB1520_DSP\DSP_ROM_AB1520_TEXT.h"
		#include "AB1520_DSP\DSP_ROM_AB1520_TEXT2.h"
		#include "AB1520_DSP\DSP_ROM_AB1520_TEXT3.h"
		#include "AB1520_DSP\DSP_ROM_AB1520_TEXT4.h"
        $endif
    #endif
$endif

#endif //DSP_ENABLE

END
