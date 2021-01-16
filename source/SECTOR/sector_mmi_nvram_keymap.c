
#include "config.h"
#include "cpu.h"
#include "sector_mmi_nvram_keymap.h"

#pragma userclass (HCONST = CONFIG)


#define	KEY_MAP_MMI_OFF                         (U32)1<<MMI_OFF
#define	KEY_MAP_MMI_CONDISCABLE                 (U32)1<<MMI_CONDISCABLE
#define	KEY_MAP_MMI_CONNECTABLE                 (U32)1<<MMI_CONNECTABLE
#define	KEY_MAP_MMI_CONNECTED                   (U32)1<<MMI_CONNECTED
#define	KEY_MAP_MMI_HFP_INCOMMING               (U32)1<<MMI_HFP_INCOMMING
#define	KEY_MAP_MMI_HFP_OUTGOING                (U32)1<<MMI_HFP_OUTGOING
#define	KEY_MAP_MMI_HFP_CALLACTIVE              (U32)1<<MMI_HFP_CALLACTIVE
#define	KEY_MAP_MMI_HFP_CALLACTIVE_WITHOUT_SCO  (U32)1<<MMI_HFP_CALLACTIVE_WITHOUT_SCO
#define	KEY_MAP_MMI_HFP_CAIMG                   (U32)1<<MMI_HFP_CAIMG
#define	KEY_MAP_MMI_HFP_CAOGG                   (U32)1<<MMI_HFP_CAOGG
#define	KEY_MAP_MMI_HFP_CAMULTY                 (U32)1<<MMI_HFP_CAMULTY
#define	KEY_MAP_MMI_FAKEON                      (U32)1<<MMI_FAKEON	  
#define	KEY_MAP_MMI_FAKEOFF                     (U32)1<<MMI_FAKEOFF
#define	KEY_MAP_MMI_DETACHING_LINK              (U32)1<<MMI_DETACHING_LINK
#define	KEY_MAP_MMI_TEST_MODE                   (U32)1<<MMI_TEST_MODE
#define	KEY_MAP_MMI_FM	                        (U32)1<<MMI_FM
#define	KEY_MAP_MMI_LINE_IN	                    (U32)1<<MMI_LINE_IN
#define	KEY_MAP_MMI_EEPROM_UPDATING	            (U32)1<<MMI_UPDATING
#define	KEY_MAP_MMI_EEPROM_UPDATED_OK	        (U32)1<<MMI_UPDATED_OK
#define	KEY_MAP_MMI_EEPROM_UPDATED_FAIL	        (U32)1<<MMI_UPDATED_FAIL
#define	KEY_MAP_MMI_VOICE_PROMPT_LANG_SELECT	(U32)1<<MMI_VOICE_PROMPT_LANG_SELECT
#define	KEY_MAP_MMI_TWS_PAIRING					(U32)1<<MMI_TWS_PAIRING

#define KEY_MAP_MMI_ALL_STATES 0xFFFFFFFF


/*
declare forward let linker locate key map as the required order
*/
extern U8 CODE gMMI_nvram_keymap_init[KEY_ACTIONTOTALNUMBER]; //mmiKeyMapPairNo locate first
extern MmiKeyMapType CODE gMmiKeyTapMap[];
extern MmiKeyMapType CODE gMmiKeyPressMap[];
extern MmiKeyMapType CODE gMmiKeyPressReleaseMap[];
extern MmiKeyMapType CODE gMmiKeyLongPressMap[];
extern MmiKeyMapType CODE gMmiKeyLongPressReleaseMap[];
extern MmiKeyMapType CODE gMmiKeyDoubleMap[];
extern MmiKeyMapType CODE gMmiKeyTripleMap[];
extern MmiKeyMapType CODE gMmiKeyLongLongPressMap[];
extern MmiKeyMapType CODE gMmiKeyLongLongPressReleaseMap[];
extern MmiKeyMapType CODE gMmiKeyExtremelyLongPressMap[];
extern MmiKeyMapType CODE gMmiKeyExtremelyLongPressReleaseMap[];
extern MmiKeyMapType CODE gMmiKeyRepeatMap[];
extern MmiKeyMapType CODE gMmiKeyDownMap[];
extern MmiKeyMapType CODE gMmiKeyUpMap[];
extern MmiKeyMapType CODE gMmiKeyNoActMap[];


#ifdef BLUETOOTH_SPEAKER
	#ifdef SOUND_BAR
		#include "sample_speaker_soundbar_keymap.h"
	#elif defined SCO_WOOFER
		#include "sample_speaker_scowoofer_keymap.h"
	#elif defined TWS_SETTINGS
		#if defined PRODUCT_TYPE_A || defined PRODUCT_TYPE_K
			#include "air_stereo_A_keymap.h"
		#else
			#include "sample_speaker_tws_keymap.h"
		#endif
	#elif defined SAME_SW_DIFF_MODULE
		#include "sample_speaker_scowoofer_keymap.h"
	#else
		#include "sample_speaker_keymap.h"
	#endif
#else
	#ifdef TWS_SETTINGS
		#if defined PRODUCT_TYPE_A || defined PRODUCT_TYPE_K
			#include "air_stereo_A_keymap.h"
		#else
			#include "sample_speaker_tws_keymap.h"
		#endif
	#else
		#include "ForceLinkN95_keymap.h"
	#endif
#endif



#ifdef KEY_TAP_ACTION
	static MmiKeyMapType CODE gMmiKeyTapMap[] = { KEY_TAP_ACTION };
	#define KEY_TAP_ACTION_NUM								(sizeof(gMmiKeyTapMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_TAP_ACTION_NUM								0
#endif

#ifdef KEY_PRESS_ACTION
	static MmiKeyMapType CODE gMmiKeyPressMap[] = { KEY_PRESS_ACTION };
	#define KEY_PRESS_ACTION_NUM							(sizeof(gMmiKeyPressMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_PRESS_ACTION_NUM							0
#endif

#ifdef KEY_PRESS_RELEASE_ACTION
	static MmiKeyMapType CODE gMmiKeyPressReleaseMap[] = { KEY_PRESS_RELEASE_ACTION };
	#define KEY_PRESS_RELEASE_ACTION_NUM					(sizeof(gMmiKeyPressReleaseMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_PRESS_RELEASE_ACTION_NUM					0
#endif

#ifdef KEY_LONG_PRESS_ACTION
	static MmiKeyMapType CODE gMmiKeyLongPressMap[] = { KEY_LONG_PRESS_ACTION };
	#define KEY_LONG_PRESS_ACTION_NUM						(sizeof(gMmiKeyLongPressMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_LONG_PRESS_ACTION_NUM						0
#endif

#ifdef KEY_LONG_PRESS_RELEASE_ACTION
	static MmiKeyMapType CODE gMmiKeyLongPressReleaseMap[] = { KEY_LONG_PRESS_RELEASE_ACTION };
	#define KEY_LONG_PRESS_RELEASE_ACTION_NUM				(sizeof(gMmiKeyLongPressReleaseMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_LONG_PRESS_RELEASE_ACTION_NUM				0
#endif

#ifdef KEY_DOUBLE_ACTION
	static MmiKeyMapType CODE gMmiKeyDoubleMap[] = { KEY_DOUBLE_ACTION };
	#define KEY_DOUBLE_ACTION_NUM							(sizeof(gMmiKeyDoubleMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_DOUBLE_ACTION_NUM							0
#endif

#ifdef KEY_TRIPLE_ACTION
	static MmiKeyMapType CODE gMmiKeyTripleMap[] = { KEY_TRIPLE_ACTION };
	#define KEY_TRIPLE_ACTION_NUM							(sizeof(gMmiKeyTripleMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_TRIPLE_ACTION_NUM							0
#endif

#ifdef KEY_LONG_LONG_PRESS_ACTION
	static MmiKeyMapType CODE gMmiKeyLongLongPressMap[] = { KEY_LONG_LONG_PRESS_ACTION };
	#define KEY_LONG_LONG_PRESS_ACTION_NUM					(sizeof(gMmiKeyLongLongPressMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_LONG_LONG_PRESS_ACTION_NUM					0
#endif

#ifdef KEY_LONG_LONG_PRESS_RELEASE_ACTION
	static MmiKeyMapType CODE gMmiKeyLongLongPressReleaseMap[] = { KEY_LONG_LONG_PRESS_RELEASE_ACTION };
	#define KEY_LONG_LONG_PRESS_RELEASE_ACTION_NUM			(sizeof(gMmiKeyLongLongPressReleaseMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_LONG_LONG_PRESS_RELEASE_ACTION_NUM			0
#endif

#ifdef KEY_EXTREMELY_LONG_PRESS_ACTION
	static MmiKeyMapType CODE gMmiKeyExtremelyLongPressMap[] = { KEY_EXTREMELY_LONG_PRESS_ACTION };
	#define KEY_EXTREMELY_LONG_PRESS_ACTION_NUM				(sizeof(gMmiKeyExtremelyLongPressMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_EXTREMELY_LONG_PRESS_ACTION_NUM				0
#endif

#ifdef KEY_EXTREMELY_LONG_PRESS_RELEASE_ACTION
	static MmiKeyMapType CODE gMmiKeyExtremelyLongPressReleaseMap[] = { KEY_EXTREMELY_LONG_PRESS_RELEASE_ACTION };
	#define KEY_EXTREMELY_LONG_PRESS_RELEASE_ACTION_NUM		(sizeof(gMmiKeyExtremelyLongPressReleaseMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_EXTREMELY_LONG_PRESS_RELEASE_ACTION_NUM		0
#endif

#ifdef KEY_REPEAT_ACTION
	static MmiKeyMapType CODE gMmiKeyRepeatMap[] = { KEY_REPEAT_ACTION };
	#define KEY_REPEAT_ACTION_NUM							(sizeof(gMmiKeyRepeatMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_REPEAT_ACTION_NUM							0
#endif

#ifdef KEY_DOWN_ACTION
	static MmiKeyMapType CODE gMmiKeyDownMap[] = { KEY_DOWN_ACTION };
	#define KEY_DOWN_ACTION_NUM								(sizeof(gMmiKeyDownMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_DOWN_ACTION_NUM								0
#endif

#ifdef KEY_UP_ACTION
	static MmiKeyMapType CODE gMmiKeyUpMap[] = { KEY_UP_ACTION };
	#define KEY_UP_ACTION_NUM								(sizeof(gMmiKeyUpMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_UP_ACTION_NUM								0
#endif

#ifdef KEY_NOACT_ACTION
	static MmiKeyMapType CODE gMmiKeyNoActMap[] = { KEY_NOACT_ACTION };
	#define KEY_NOACT_ACTION_NUM							(sizeof(gMmiKeyNoActMap)/sizeof(MmiKeyMapType))
#else
	#define KEY_NOACT_ACTION_NUM							0
#endif

#define TOTAL_KEY_ACTION_NUM	( KEY_TAP_ACTION_NUM							\
								+ KEY_PRESS_ACTION_NUM							\
								+ KEY_PRESS_RELEASE_ACTION_NUM					\
								+ KEY_LONG_PRESS_ACTION_NUM						\
								+ KEY_LONG_PRESS_RELEASE_ACTION_NUM				\
								+ KEY_DOUBLE_ACTION_NUM							\
								+ KEY_TRIPLE_ACTION_NUM							\
								+ KEY_LONG_LONG_PRESS_ACTION_NUM				\
								+ KEY_LONG_LONG_PRESS_RELEASE_ACTION_NUM		\
								+ KEY_EXTREMELY_LONG_PRESS_ACTION_NUM			\
								+ KEY_EXTREMELY_LONG_PRESS_RELEASE_ACTION_NUM	\
								+ KEY_REPEAT_ACTION_NUM							\
								+ KEY_DOWN_ACTION_NUM							\
								+ KEY_UP_ACTION_NUM								\
								+ KEY_NOACT_ACTION_NUM )

PUBLIC U8 CODE gMMI_nvram_keymap_init[KEY_ACTIONTOTALNUMBER] = 
{
	KEY_TAP_ACTION_NUM,
	KEY_PRESS_ACTION_NUM,
	KEY_PRESS_RELEASE_ACTION_NUM,
	KEY_LONG_PRESS_ACTION_NUM,
	KEY_LONG_PRESS_RELEASE_ACTION_NUM,
	KEY_DOUBLE_ACTION_NUM,
	KEY_TRIPLE_ACTION_NUM,
	KEY_LONG_LONG_PRESS_ACTION_NUM,
	KEY_LONG_LONG_PRESS_RELEASE_ACTION_NUM,
	KEY_EXTREMELY_LONG_PRESS_ACTION_NUM,
	KEY_EXTREMELY_LONG_PRESS_RELEASE_ACTION_NUM,
	KEY_REPEAT_ACTION_NUM,
	KEY_DOWN_ACTION_NUM,
	KEY_UP_ACTION_NUM,
	KEY_NOACT_ACTION_NUM
};

static MmiKeyMapType CODE _mmi_nvram_keymap_reserved[MAX_KEY_ACTION_NUM - TOTAL_KEY_ACTION_NUM];

/*
Key Map Sector CRC
*/
static CRC16 CODE _mmi_nvram_keymap_init_crc =
{
    //- CRC
    {0,0}
};