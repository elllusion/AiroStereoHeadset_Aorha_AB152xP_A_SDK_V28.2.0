#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"
#include "..\Driver\Driver_Interface.h"
#include "..\Driver\keypad.h"
#include "sector_mmi_nvram_type.h"
#include "..\MMI\MMI_State.h"
#include "..\SDK\include\Key_Event.h"

#include "..\MMI\MMI_Driver.h"

#if SDK_HCI_ROLE_SWITCH_SAMPLE_CODE || SDK_HCI_INQUIRY_SAMPLE_CODE
#include "..\SDK\include\SDK_HCI.h"
#endif

#if SDK_AVRCP_REPEAT || SDK_AVRCP_SHUFFLE || SDK_AVRCP_KEYS
#include "..\SDK\include\SDK_AVRCP.h"
#endif

#define SDK_KEY_PARSER	0


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
#define	KEY_MAP_MMI_INQUIRY						(U32)1<<MMI_INQUIRY
#define	KEY_MAP_MMMI_MP_STATE					(U32)1<<MMI_MP_STATE
#define	KEY_MAP_MMI_STATE_RESERVED1				(U32)1<<MMI_STATE_RESERVED1
#define	KEY_MAP_MMI_STATE_RESERVED2				(U32)1<<MMI_STATE_RESERVED2
#define	KEY_MAP_MMI_STATE_RESERVED3				(U32)1<<MMI_STATE_RESERVED3
#define	KEY_MAP_MMI_STATE_RESERVED4				(U32)1<<MMI_STATE_RESERVED4
#define	KEY_MAP_MMI_STATE_RESERVED5				(U32)1<<MMI_STATE_RESERVED5
#define	KEY_MAP_MMI_STATE_RESERVED6				(U32)1<<MMI_STATE_RESERVED6
#define	KEY_MAP_MMI_STATE_RESERVED7				(U32)1<<MMI_STATE_RESERVED7

#define KEY_MAP_MMI_ALL_STATES 0xFFFFFFFF


#if SDK_KEY_PARSER
typedef struct
{
	U8 keyIndex;
	U8 keyAction;
	U32 mmiState;
	U16 keyEvent;
}SDK_KEY_MAP_STRU;

#define SDK_KEY_EVT_NUM  10

SDK_KEY_MAP_STRU CODE sdk_key_map[SDK_KEY_EVT_NUM]=
{
	{KEY5, KEY_TAP, KEY_MAP_MMI_CONDISCABLE|KEY_MAP_MMI_CONNECTABLE, SDK_KEY_EVENT_1},
	{KEY5, KEY_TAP, KEY_MAP_MMI_FM, SDK_KEY_EVENT_2},
	#if SDK_HCI_ROLE_SWITCH_SAMPLE_CODE
	{KEY6, KEY_DOUBLE, KEY_MAP_MMI_CONNECTED, SDK_KEY_EVENT_3_ROLE_SWITCH_MASTER},
	{KEY6, KEY_TAP, KEY_MAP_MMI_CONNECTED, SDK_KEY_EVENT_4_ROLE_SWITCH_SLAVE},
	#endif
	#if SDK_HCI_INQUIRY_SAMPLE_CODE
	{KEY6, KEY_DOUBLE, KEY_MAP_MMI_CONDISCABLE|KEY_MAP_MMI_CONNECTABLE, SDK_KEY_EVENT_5_INQUIRY},
	#endif
	#if SDK_AVRCP_REPEAT
	{KEY6, KEY_TAP, KEY_MAP_MMI_CONNECTED, SDK_KEY_EVENT_6_GET_REPEAT},
	#endif
	#if SDK_AVRCP_SHUFFLE
	{KEY6, KEY_DOUBLE, KEY_MAP_MMI_CONNECTED, SDK_KEY_EVENT_7_GET_SHUFFLE},
	#endif
	#if SDK_AVRCP_KEYS
	{KEY6, KEY_TAP, KEY_MAP_MMI_CONNECTED, SDK_KEY_EVENT_8_KEY_PLAY},
	{KEY6, KEY_DOUBLE, KEY_MAP_MMI_CONNECTED, SDK_KEY_EVENT_9_KEY_FORWARD},
	#endif
};

typedef struct
{
	U32 keyDownState;
}SDK_KEY_CTL_TYPE;

SDK_KEY_CTL_TYPE XDATA gSDK_Key_ctl;

#if SDK_HCI_INQUIRY_SAMPLE_CODE
U8 XDATA Liac[3] = {0x11, 0x8B, 0x9E};
#endif

BOOL SDK_KeyEvtHandler(U8 linkIndex, U16 keyEvt)
{
	switch(keyEvt)
	{
		case SDK_KEY_EVENT_2:
			MMI_DRV_KeyEventHandler(0, KEY_FM_POWEROFF);
			return TRUE;
			break;
		case SDK_KEY_EVENT_1:
			MMI_DRV_KeyEventHandler(0, KEY_FM_POWERON);
			return TRUE;
			break;
		#if SDK_HCI_ROLE_SWITCH_SAMPLE_CODE
		case SDK_KEY_EVENT_3_ROLE_SWITCH_MASTER:
			SDK_HCI_SwitchRoleCmd(linkIndex, SDK_ROLE_MASTER);
			break;
		case SDK_KEY_EVENT_4_ROLE_SWITCH_SLAVE:
			SDK_HCI_SwitchRoleCmd(linkIndex, SDK_ROLE_SLAVE);
			break;
		#endif
		#if SDK_HCI_INQUIRY_SAMPLE_CODE
		case SDK_KEY_EVENT_5_INQUIRY:
			SDK_HCI_InquiryCmd(Liac, 8, 0);
			break;
		#endif
		#if SDK_AVRCP_REPEAT
		case SDK_KEY_EVENT_6_GET_REPEAT:
			SDK_AVRCP_SendGetRepeatStatus(linkIndex);
			break;
		#endif
		#if SDK_AVRCP_SHUFFLE
		case SDK_KEY_EVENT_7_GET_SHUFFLE:
			SDK_AVRCP_SendGetShuffleStatus(linkIndex);
			break;
		#endif
		#if SDK_AVRCP_KEYS
		case SDK_KEY_EVENT_8_KEY_PLAY:
			SDK_AVRCP_KeyPlay(linkIndex);
			break;
		case SDK_KEY_EVENT_9_KEY_FORWARD:
			SDK_AVRCP_KeyForward(linkIndex);
			break;
		#endif
	}
	UNUSED(linkIndex);
	return FALSE;
}

/*  SDK_CB_KeyParser: callback function which is called in NDK when key action is detected
 *	<input 1>
 *		U8 keyIndex		: key index which is defined in "..\Driver\keypad.h"
 *  	value range		: KEY1 - KEY10
 *	<input 2>
 *		U8 keyAction	: key action which is defined in "..\Driver\Driver_Interface.h"
 *  	value range		: KEY_TAP - KEY_UP
 *  <input 3>
 *		U8 linkIndex	: current service link index
 *  	value range		: 0 - (max supproted device number -1)
 */
void SDK_CB_KeyParser(U8 keyIndex, U8 keyAction, U8 linkIndex)
{
	U8 i;
	U32 stateBitMask;
	U16 keyEvt = SDK_KEY_EVENT_NULL;
	
	if(MMI_KEY_MAP_MAPPING_BY_KEY_DOWN_STATE_FEAT)
	{
		if(keyAction == KEY_DOWN)
			gSDK_Key_ctl.keyDownState = MMI_GetTopStateByLink(linkIndex);
	}
	else
	{
		gSDK_Key_ctl.keyDownState = MMI_GetTopStateByLink(linkIndex);
	}

	stateBitMask = 1L << (gSDK_Key_ctl.keyDownState);
			
	for(i = 0 ; i < SDK_KEY_EVT_NUM; i++)
	{
		SDK_KEY_MAP_STRU CODE_PTR keyMapPtr = &sdk_key_map[i];
		if(keyIndex == keyMapPtr->keyIndex && keyAction == keyMapPtr->keyAction && (stateBitMask & keyMapPtr->mmiState))
		{
			keyEvt = keyMapPtr->keyEvent;
			break;
		}
	}

	SDK_KeyEvtHandler(linkIndex, keyEvt);
}

#endif //SDK_KEY_PARSER

void SDK_Key_Init(void)
{
#if SDK_KEY_PARSER
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_KEY, API_GRP_IOSYS, (VFUN)SDK_CB_KeyParser);
#endif
}
