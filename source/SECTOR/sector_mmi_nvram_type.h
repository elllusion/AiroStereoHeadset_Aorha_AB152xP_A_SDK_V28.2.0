#ifndef _SECTOR_MMI_NVRAM_H_
#define _SECTOR_MMI_NVRAM_H_

	#include "bt_config_profile.h"
	#include "..\Driver\keypad.h"
	#include "..\MMI\MMI_event_opcode.h"
	#include "..\Driver\Driver_Interface.h"
	#include "..\Driver\Driver_LED.h"
	
	
	//==========================FEATURE DEFINE=======================
#define MMI_REBOOT_ON_RESET_LINK_HISTORY_FEAT				(gMMI_nvram.mmiFeatureCtl.System & 0x01)
#define MMI_DEBUG_PRIVATE_KEY_FEAT							(gMMI_nvram.mmiFeatureCtl.System & 0x02)
#define MMI_DETECT_PHONE_IS_TOO_FAR_FEAT					(gMMI_nvram.mmiFeatureCtl.System & 0x04)
#define MMI_POWER_DROP_SAVE_LINK_HISTORY_FEAT				(gMMI_nvram.mmiFeatureCtl.System & 0x08)
#define MMI_FORCE_USE_ESCO_IN_AUDIO_TRANSFER_FEAT			(gMMI_nvram.mmiFeatureCtl.System & 0x10)
#define MMI_DISABLE_AG_ECNR_IN_SCO							(gMMI_nvram.mmiFeatureCtl.System & 0x20)
#define MMI_RESET_INSTEAD_OF_POWEROFF_FOR_REGRESSION_TEST	(gMMI_nvram.mmiFeatureCtl.System & 0x40)


#define MMI_ENABLE_HANDLING_VGM_FEAT						(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x0001)
#define MMI_REMEMBER_SOUND_LEVEL_CHANGE_FEAT				(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x0002)
#define MMI_CYCLIC_VOLUME_ENABLED_FEAT						(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x0004)
#define MMI_ADJUST_MIC_LEVEL_AUTOMATICALLY_FEAT				(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x0008)
#define MMI_VOICE_PROMPT_FUNC_OFF_FEAT						(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x0010)
#define MMI_READ_OUT_CALLER_ID_BY_VP_FEAT					(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x0100)
#define MMI_IGNORE_CALLER_VOICE_IF_OTHER_LINK_IN_CALL_FEAT	(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x0200)
#define MMI_DO_NOT_RING_WHEN_WATCHDOG_TO_PWON_FEAT			(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x2000)
#define MMI_NO_RING_VP_WHEN_ACTIVE_CONN_SUCCESS_FEAT 		(gMMI_nvram.mmiFeatureCtl.Audio1 & 0x4000)


#define MMI_PLAY_DEFAULT_RING_INCOMING_FEAT								(gMMI_nvram.mmiFeatureCtl.Audio2 & 0x01)
#define MMI_VP_RINGTONE_1OF2_FEAT										(gMMI_nvram.mmiFeatureCtl.Audio2 & 0x02)
#define MMI_QUEUE_RINGTONE_FEAT											(gMMI_nvram.mmiFeatureCtl.Audio2 & 0x04)
#define MMI_DISCARD_VP_WHEN_IN_CALLWITHSCO_FEAT							(gMMI_nvram.mmiFeatureCtl.Audio2 & 0x08)
#define MMI_PLAY_BEEP_ON_REMOTE_INDICATION_VGS_FEAT						(gMMI_nvram.mmiFeatureCtl.Audio2 & 0x20)
#define MMI_DISCARD_VP_WHEN_IN_MUSIC_FEAT								(gMMI_nvram.mmiFeatureCtl.Audio2 & 0x40)


#define MMI_MUTE_SPEAKER_AS_MICROPHONE_FEAT							(MMI_FEATURE_CTL_AUDIO3 & 0x0002)
#define MMI_CLOSE_AMP_WHILE_SPEAKER_MUTED_FEAT						(MMI_FEATURE_CTL_AUDIO3 & 0x0004)
#define MMI_ENABLE_SYNC_MUTE_MIC_TO_ALL_LINK_FEAT					(MMI_FEATURE_CTL_AUDIO3 & 0x0008)
#define MMI_MUTE_CANCEL_WHEN_VOL_CHANGE_FEAT						(MMI_FEATURE_CTL_AUDIO3 & 0x0010)
#define MMI_ALLOW_VOL_ADJUST_WHEN_MUTED_FEAT						(MMI_FEATURE_CTL_AUDIO3 & 0x0020)
#define MMI_AUTO_UNMUTE_ENTER_OR_EXIT_CALL_FEAT						(MMI_FEATURE_CTL_AUDIO3 & 0x0040)
#define MMI_MUTE_SPEAKER_WHEN_VOLUME_IS_ZERO_FEAT					(MMI_FEATURE_CTL_AUDIO3 & 0x0100)	
#define MMI_MUTE_A2DP_WHEN_VOLUME_IS_ZERO_FEAT 						(MMI_FEATURE_CTL_AUDIO3 & 0x0200)
#define MMI_MUTE_LINE_IN_WHEN_VOLUME_IS_ZERO_FEAT 					(MMI_FEATURE_CTL_AUDIO3 & 0x0400)	
#define MMI_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT				(MMI_FEATURE_CTL_AUDIO3 & 0x0800)


#define MMI_QUEUE_LED_FEAT										(gMMI_nvram.mmiFeatureCtl.LED & 0x01)
#define MMI_MEDIA_LED_WITH_FILTER_EFFECT_FEAT					(gMMI_nvram.mmiFeatureCtl.LED & 0x02)
#define MMI_LED_PWM_COMPENSATE_IN_DIFFERENT_BATTERY_LEVEL 		(gMMI_nvram.mmiFeatureCtl.LED & 0x04)	
#define MMI_IGNORE_KEYEVENT_JUST_ENABLE_LED_FEAT				(gMMI_nvram.mmiFeatureCtl.LED & 0x08)
#define MMI_SHOW_CONDISCABLE_LED_ONLY_ONE_CONNECTED_DEVICE_FEAT	(gMMI_nvram.mmiFeatureCtl.LED & 0x10)
#define MMI_SDK_DISABLE_LED_FILTER_FEAT							(gMMI_nvram.mmiFeatureCtl.LED & 0x20)
#define MMI_SDK_DISABLE_BACKGROUND_LED_FEAT						(gMMI_nvram.mmiFeatureCtl.LED & 0x40)
#define MMI_SDK_DISABLE_FOREGROUND_LED_FEAT						(gMMI_nvram.mmiFeatureCtl.LED & 0x80)

#define MMI_NO_COMBO_KEY_NODE_FEAT						(gMMI_nvram.mmiFeatureCtl.Mode & 0x01)
#define MMI_ALWAYS_ENA_DISCOVER_MODE_FEAT				(gMMI_nvram.mmiFeatureCtl.Mode & 0x02)
#define MMI_ENTER_DISCOVER_AFTER_PWR_ON_FEAT			(gMMI_nvram.mmiFeatureCtl.Mode & 0x04)
#define MMI_DO_NOT_DISCO_DURING_LINK_LOSS_FEAT			(gMMI_nvram.mmiFeatureCtl.Mode & 0x08)
#define MMI_DISCOVER_MODE_AFTER_DISCONNECT_FEAT			(gMMI_nvram.mmiFeatureCtl.Mode & 0x10)

#define MMI_FORCE_USE_OF_HSP_CONNECTION_FEAT			(gMMI_nvram.mmiFeatureCtl.HSP & 0x01)
#define MMI_NOT_CHG_STATE_ACP_HSP_RING_FEAT				(gMMI_nvram.mmiFeatureCtl.HSP & 0x02)

#define MMI_IPHONE_BATTERY_DISPLAY_FEAT					(gMMI_nvram.mmiFeatureCtl.HSP & 0x10) 	
#define MMI_XEVENT_BATTERY_DISPLAY_FEAT					(gMMI_nvram.mmiFeatureCtl.HSP & 0x20) 	
#define MMI_IPHONE_SIRI_FEAT							(gMMI_nvram.mmiFeatureCtl.HSP & 0x40) 
#define MMI_IPHONE_DEVICE_DOCKED_FEAT					(gMMI_nvram.mmiFeatureCtl.HSP & 0x80) 


#define MMI_ECNR_FEAT							(gMMI_nvram.mmiFeatureCtl.HFP & 0x0001)
#define MMI_CALL_WAITING_3WAYCALLING_FEAT		(gMMI_nvram.mmiFeatureCtl.HFP & 0x0002)
#define MMI_CLI_PRESENTATION_FEAT				(gMMI_nvram.mmiFeatureCtl.HFP & 0x0004)
#define MMI_VOICE_RECOGNITION_FEAT				(gMMI_nvram.mmiFeatureCtl.HFP & 0x0008)
#define MMI_REMOTE_VOLUME_CONTROL_FEAT			(gMMI_nvram.mmiFeatureCtl.HFP & 0x0010)
#define MMI_ENHANCED_CALL_STATUS_FEAT			(gMMI_nvram.mmiFeatureCtl.HFP & 0x0020)
#define MMI_ENHANCED_CALL_CONTROL_FEAT			(gMMI_nvram.mmiFeatureCtl.HFP & 0x0040)
#define MMI_CODEC_NEGOTIATION_FEAT				(gMMI_nvram.mmiFeatureCtl.HFP & 0x0080)
#define MMI_HF_INDICATORS_FEAT					(gMMI_nvram.mmiFeatureCtl.HFP & 0x0100)


#define MMI_ALWAYS_ROLE_SWITCH_TO_MASTER_FEAT					(gMMI_nvram.roleSwitchCnt /*MMI_CONNECTION & 0x0001*/)
#define MMI_ROLE_SWITCH_TO_MASTER_DURING_ACCEPT_CONN_FEAT		(MMI_CONNECTION & 0x0002)
#define MMI_SUPPORT_HFP_FEAT									(MMI_CONNECTION & 0x0004)
#define MMI_SUPPORT_HSP_FEAT									(MMI_CONNECTION & 0x0008)
#define MMI_SUPPORT_A2DP_SINK_FEAT								(MMI_CONNECTION & 0x0010)
#define MMI_SUPPORT_AVRCP_FEAT									(MMI_CONNECTION & 0x0020)
#define MMI_SUPPORT_HFP_VERSION16_FEAT							(MMI_CONNECTION & 0x0040)
#define MMI_SUPPORT_MULTI_HFP_FEAT								(MMI_CONNECTION & 0x0080)
#define MMI_SUPPORT_TWS_SHARE_FEAT								(MMI_CONNECTION & 0x0200)
#define MMI_SUPPORT_RECONN_CLOSE_SCAN_FEAT						(MMI_CONNECTION & 0x0400)
#ifdef HID_Profile
#define MMI_SUPPORT_HID_FEAT									(MMI_CONNECTION & 0x1000)
#endif
//#define MMI_ALWAYS_ROLE_SWITCH_TO_SLAVE_FEAT            		(MMI_CONNECTION & 0x2000)
#define MMI_SUPPORT_SCO_CLOSE_SCAN_FEAT							(MMI_CONNECTION & 0x4000)
#define MMI_SUPPORT_AVRCP_KEY_SYNC								(MMI_CONNECTION & 0x8000)


#define MMI_SUPPORT_A2DP_FEAT									(MMI_SUPPORT_A2DP_SINK_FEAT) 	


#define MMI_AUTO_RECONN_AFTER_PWR_ON_FEAT						(gMMI_nvram.mmiFeatureCtl.ReConnection & 0x01)
#define MMI_AUTO_RECONN_AFTER_LINK_LOSS_FEAT					(gMMI_nvram.mmiFeatureCtl.ReConnection & 0x02)
#define MMI_RECONNECT_WHEN_REDIAL_WITHOUT_CONNECTION_FEAT		(gMMI_nvram.mmiFeatureCtl.ReConnection & 0x04)
#define MMI_RECONNECT_WHEN_VOICE_DIAL_WITHOUT_CONNECTION_FEAT	(gMMI_nvram.mmiFeatureCtl.ReConnection & 0x08)
#define MMI_STOP_CURRENT_RECONNECT_BY_NEW_RECONNECT_EVT_FEAT	(gMMI_nvram.mmiFeatureCtl.ReConnection & 0x10)
#define MMI_DISCOVER_AFTER_POWERON_RECONNECT_FAIL_FEAT			(gMMI_nvram.mmiFeatureCtl.ReConnection & 0x20)

#define MMI_MUTUAL_CONNECTION_FEAT								(gMMI_nvram.mmiFeatureCtl.ReConnection & 0x80)  //temp


#define MMI_LINE_IN_CYCLIC_VOLUME_ENABLED_FEAT		(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0001)
#define MMI_A2DP_MEMPUT_WHEN_LINE_IN_FEAT			(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0002)
#define MMI_IGNORE_RING_VP_WHEN_LINE_IN_FEAT		(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0008)
#define MMI_NO_CONNECTION_IN_LINE_IN_FEAT			(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0010)
#define MMI_NO_DISCAVERABLE_IN_LINE_IN_FEAT			(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0040)
#define MMI_NO_CONNECTABLE_IN_LINE_IN_FEAT			(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0080)
#define MMI_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT		(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0100)
#define MMI_NO_CONNECTED_MEDIA_EVT_IN_LINE_IN_FEAT	(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0400)
#define MMI_ENTER_LINEIN_SEND_AVRCP_PAUSE_FEAT		(gMMI_nvram.mmiFeatureCtl.LineInFeature & 0x0800)




#define MMI_FM_CYCLIC_VOLUME_ENABLED_FEAT			(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0001)
#define MMI_A2DP_MEMPUT_WHEN_FM_ON_FEAT				(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0002)	   
#define MMI_IGNORE_RING_VP_WHEN_FM_ON_FEAT			(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0008)
#define MMI_FM_CHIP_IS_REPLACED_FEAT				(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0010)
#define MMI_NO_DISCAVERABLE_IN_FM_FEAT				(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0040)
#define MMI_NO_CONNECTABLE_IN_FM_FEAT				(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0080)
#define MMI_NO_CONNECTION_IN_FM_FEAT				(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0100)
#define MMI_UNMUTE_WHEN_SEEK_TUNE_CHANNEL			(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0200)
#define MMI_NO_CONNECTED_MEDIA_EVT_IN_FM_FEAT		(gMMI_nvram.mmiFeatureCtl.FMFeature & 0x0800)

#define MMI_A2DP_CYCLIC_VOLUME_ENABLED_FEAT							(MMI_FEATURE_CTL_A2DP & 0x0001)
#define MMI_MUSIC_LAST_IN_FIRST_PLAYED_AND_PAUSE_OTHERS_FEAT		(MMI_FEATURE_CTL_A2DP & 0x0002)
#define MMI_A2DP_SEND_START_AFTER_ACTIVE_MEDIA_CHANNEL_CONN_FEAT	(MMI_FEATURE_CTL_A2DP & 0x0004)
#define MMI_A2DP_VOLUME_SYNC_HFP_VOLUME_FEAT						(MMI_FEATURE_CTL_A2DP & 0x0008)
#define MMI_ADJUST_A2DP_RATHER_THAN_HFP_IN_IDLE_FEAT				(MMI_FEATURE_CTL_A2DP & 0x0010)
#define MMI_MUSIC_LAST_IN_FIRST_PLAYED_NO_PAUSE_OTHERS_FEAT			(MMI_FEATURE_CTL_A2DP & 0x0020)	
#define MMI_MUSIC_FORCE_USE_PEQ_FEAT								(MMI_FEATURE_CTL_A2DP & 0x0040)
#define MMI_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT						(MMI_FEATURE_CTL_A2DP & 0x0080)
#define MMI_REMEMBER_PEQ_CHANGE_FEAT								(MMI_FEATURE_CTL_A2DP & 0x0100)
#define MMI_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT					(MMI_FEATURE_CTL_A2DP & 0x0200)
#define MMI_A2DP_DELAY_REPORT_FEAT									(MMI_FEATURE_CTL_A2DP & 0x0400)
#define MMI_DEFAULT_PEQ_ASSIGNMENT_FEAT								(MMI_FEATURE_CTL_A2DP & 0x0800)
#define MMI_A2DP_BYPASS_IPHONE_CHECK_FOR_AAC_FUNCTION_FEAT			(MMI_FEATURE_CTL_A2DP & 0x1000)
#define MMI_A2DP_DO_NOT_SEND_SUSPEND_AFTER_ACTIVE_START_FEAT		(MMI_FEATURE_CTL_A2DP & 0x2000)
#define MMI_A2DP_SEND_START_RESP_AFTER_DSP_IS_ENABLED_FEAT			(MMI_FEATURE_CTL_A2DP & 0x4000)


#define MMI_MUSIC_PLAY_PAUSE_ONEKEY_FEAT							(gMMI_nvram.mmiFeatureCtl.AVRCP & 0x01)
#define MMI_KEY_PLAY_PAUSE_DONT_GET_PLAY_STATUS						(gMMI_nvram.mmiFeatureCtl.AVRCP & 0x02)
#define MMI_AVRCP_VOLUME_SYNC_DISABLED_FEAT							(gMMI_nvram.mmiFeatureCtl.AVRCP & 0x04)
#define MMI_AVRCP_EXT_MCU_VOLUME_CTRL_FEAT							(gMMI_nvram.mmiFeatureCtl.AVRCP & 0x08)


#define MMI_CHARGER_OUT_THEN_POWER_ON_FEAT							(gMMI_nvram.mmiFeatureCtl.Battery & 0x02)
#define MMI_INIT_BAT_CHECK_ON_POWER_UP_FEAT							(gMMI_nvram.mmiFeatureCtl.Battery & 0x04)
#define MMI_POWER_OFF_WHEN_CHARGING_TO_FEAT							(gMMI_nvram.mmiFeatureCtl.Battery & 0x10)

#define MMI_CC_UART_ENABLE											(gMMI_nvram.mmiFeatureCtl.customerUART & 0x01)
#define MMI_CONFIRMATION_ACCEPT_REJECT_VIA_KEY						(gMMI_nvram.mmiFeatureCtl.customerUART & 0x02)
#define MMI_SPP_DATA_TO_UART_FEAT									(gMMI_nvram.mmiFeatureCtl.customerUART & 0x04)

#define MMI_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT	(gMMI_nvram.mmiFeatureCtl.Keys & 0x01)	
#define MMI_FAST_DETECT_FOR_ROTARY_ENCODER_FEAT			(gMMI_nvram.mmiFeatureCtl.Keys & 0x02)	
#define MMI_KEY_MAP_MAPPING_BY_KEY_DOWN_STATE_FEAT		(gMMI_nvram.mmiFeatureCtl.Keys & 0x04)
#define MMI_SEPARATE_LNR_BUTTONS_FEAT					(gMMI_nvram.mmiFeatureCtl.Keys & 0x08) 		
#define MMI_SEPARATE_VOICE_DIAL_BUTTONS_FEAT			(gMMI_nvram.mmiFeatureCtl.Keys & 0x10) 		
#define MMI_CANCEL_VR_WHEN_RDIAL_FEAT					(gMMI_nvram.mmiFeatureCtl.Keys & 0x20)
#define MMI_UKEY_SWITCH_ONOFF_TYPE_FEAT 				(gMMI_nvram.mmiFeatureCtl.Keys & 0x40)
#define MMI_POWER_KEY_TO_DEEP_SLEEP_FEAT				(gMMI_nvram.mmiFeatureCtl.Keys & 0x80)


#define MMI_AIR_SYNC_POWER_KEY_FEAT						(gMMI_nvram.mmiFeatureCtl.AirFeature & 0x01)
#define MMI_AIR_SCO_RELAY_WITHOUT_CALL					(gMMI_nvram.mmiFeatureCtl.AirFeature & 0x02)

//===========================FEATURE DEFINE [END]============================
#define MMI_LINE_IN_DETECT_BY_GPIO					(gMMI_nvram.lineInAutoDetectKeyIndex < TOTAL_KEY_NO)

	#define LED_OVERRIDE_BIT 0x80
	#define LED_RESET_FG_BIT 0x40
	
	#define LED_INVALID 0xFF
	
	
	enum
	{
		MMI_OFF,			//0
		MMI_CONDISCABLE,
		MMI_CONNECTABLE,
		MMI_CONNECTED,		
		MMI_HFP_INCOMMING,	
		MMI_HFP_OUTGOING,	//5
		MMI_HFP_CALLACTIVE,
		MMI_HFP_CALLACTIVE_WITHOUT_SCO,
		MMI_HFP_CAIMG,		
		MMI_HFP_CAOGG,		
		MMI_HFP_CAMULTY,	//10 
		MMI_FAKEON, 	  
		MMI_FAKEOFF,		//12
		MMI_DETACHING_LINK,
		MMI_TEST_MODE,
		MMI_FM,
		MMI_LINE_IN,
		MMI_UPDATING,
		MMI_UPDATED_OK,
		MMI_UPDATED_FAIL,
		MMI_VOICE_PROMPT_LANG_SELECT, //20
		MMI_TWS_PAIRING,
		MMI_INQUIRY,
		MMI_MP_STATE,
		MMI_STATE_RESERVED1,
		MMI_STATE_RESERVED2,
		MMI_STATE_RESERVED3,
		MMI_STATE_RESERVED4,
		MMI_STATE_RESERVED5,
		MMI_STATE_RESERVED6,
		MMI_STATE_RESERVED7, //30
		MMI_TOTAL_STATE_NO
	};

	enum
	{
		MCU_NO_TEMP_DETECT,
		MCU_SOURCE_INTERNAL,
		MCU_SOURCE_EXTERNAL,
	};
	
	// PinCodeCtl
	typedef	struct 
	{
		U8 length;
		U8 value[16];
	} PinCodeCtl;

	//PageScanWindowSize
	typedef struct
	{
		U16 smallSize;
		U16 largeSize;
	}PageScanWindowSize;
	
	//ScoPktPara
	typedef struct
	{
		U32 TxBandwidth;
		U32 RxBandwidth;
		U16 MaxLatency;
		U16 VoiceSetting;
		U8 ReTxEffort;
		U16 scoPktType;
	}ScoPktPara;
	
	typedef struct
	{
		U16 Acl;
		U16 Sco;
		U16 Esco;
	}ChangePktTypePara;
	
	typedef struct
	{
		U8 oobAuthData:1;
		U8 authRequirement:3;
		U8 ioCapabilities:4;	
	}SimplePairingPara;
	
	
	typedef struct
	{
		U8 interval_1To1;
		U8 interval_1To2;
	}MMI1IntInterval;
	
	
	#define MAX_KEY_MATCH_SEQ_NO 	6
	#define MAX_BTN_SEQ_NO 			2
	typedef struct
	{
		KeyIndex keyIndex;
		U8 keyAction;
	}KeyEvent;
	
	typedef struct
	{
		U8 mmiKeyCode; 
		KeyEvent keyEvent[MAX_KEY_MATCH_SEQ_NO];
	}MMIBtnSeqInfo;
	
	//SniffPara
	typedef struct
	{
		//Sniff
		U16 Sniff_Max_Interval;
		U16 Sniff_Min_Interval;
		U16 Sniff_Attempt;
		U16 Sniff_Timeout;

		//Sniff Subrating
		U16 Maximum_Latency;
		U16 Minimum_Remote_Timeout;
		U16 Minimum_Local_Timeout;
	}SniffPara;
	
	
	// LED 
	typedef struct
	{
		//The significant bit represents user defined or not.
		U8 entryLedDisplayIndex;
	}LedSettingType;
	
	typedef struct
	{
		DriverKeyTimeType	keyTime; 
		U8 keyGpioIndex[TOTAL_KEY_NO-1];
		U32 keyMask;
	}KeypadGpioCtl;
	
	
	//ProductType
	#define PRODUCT_TYPE_HEADSET	0x01
	#define PRODUCT_TYPE_SPEAKER	0x02

	#define PRODUCT_ID_TWS			0x0123
	#define PRODUCT_VER_TWS			0x0100
	
	#define PRODUCT_AIR_NONE		0x00
	#define PRODUCT_AIR_TWS			0x01
	#define PRODUCT_AIR_SOUNDBAR	0x02
	#define PRODUCT_AIR_SCO_WOOFER	0x04
	#define PRODUCT_AIR_A2DP_WOOFER	0x08
	
	#define GET_PRODUCT_AIR_TYPE(A)	(U8)((A)& 0x000F)

	typedef struct
	{
		U8	reserved;
		U8	sinkSupportedFeatures;
		U16	pnpProductID; // 8 bits: customer ID, 8 bits: product ID 
		U16	pnpProductVersion;// 12 bits: reserved, 4 bits: AirProductType 
	} ProductType;
	
	enum lpf_cutoff_freq_enum
	{
		LPF_CUTOFF_100HZ = 0,
		LPF_CUTOFF_150HZ,
		LPF_CUTOFF_200HZ,
		LPF_CUTOFF_250HZ,
		LPF_CUTOFF_300HZ,
		LPF_CUTOFF_350HZ,
		LPF_CUTOFF_400HZ,
		LPF_CUTOFF_450HZ,
		LPF_CUTOFF_800HZ,
		LPF_CUTOFF_880HZ,
		LPF_CUSTOMIZED1,
		LPF_CUSTOMIZED2,
		LPF_CUSTOMIZED3,
		LPF_CUSTOMIZED4,
		LPF_CUSTOMIZED5,
	}; //LPFCutFreq
	
	enum pcm_fs_sel_enum
	{
		PCM_FS_SEL_2KHZ = 0,
		PCM_FS_SEL_4KHZ,
		PCM_FS_SEL_8KHZ,
		PCM_FS_SEL_1KHZ,
	}; //PCM_FS_SEL //AUX_FS_SEL //A2DP_FS_SEL

	enum soundbar_external_mcu
	{
		WITH_EXTERNAL_MCU = 0,
		WITHOUT_EXTERNAL_MCU,
	}; //relayScoSource
	
	typedef struct
	{
		U8 LPFCutFreq;
		U8 relayScoSource;
		U8 A2DP_FS_SEL;
		U8 AUX_FS_SEL;
	}AirParaType;
	
	//actionAfterPairingModeTimeout
	enum
	{
		RETURN_TO_CONNECTABLE,
		POWER_OFF,
		POWER_OFF_IF_PDL_IS_EMPTY
	};
	
	//ReConnectInfo
	enum
	{
		RECONNECT_UNNECESSARY,
		RECONNECT_LAST_ONE,
		RECONNECT_LAST_TWO,
		RECONNECT_LIST, 
		RECONNECT_LAST_DISC,
		RECONNECT_AG_IND_0,
		RECONNECT_AG_IND_1,
		RECONNECT_AG_IND_0_1,
		RECONNECT_TWS,
		RECONNECT_FAST_DONGLE,
		RECONNECT_ALL_DROPPED_LINKS,
		RECONNECT_CURRENT_LINK,
		RECONNECT_TWS_0011,
		RECONNECT_TWS_0101,
		RECONNECT_MODE_TOTAL_NO,
	};
	typedef struct
	{
		U8 afterPowerOn;
		U8 afterLinkLoss;
		U8 userInitiated;
		U8 watchDogReTryCnt;
		U8 linkLossReTryCnt;
		U8 autoRecTryCnt;
		U8 profileRetryCnt;
	}ReConnectInfo;
	
	//MMIFeatureCtl
	typedef struct
	{
		U8 System;
		U16 Audio1;
		U8 Audio2;
		U16 Audio3;
		U8 LED;
		U8 Mode;
		U8 HSP;
		U16 HFP;
		U16	Connection;
		U8 	ReConnection;
		U16	LineInFeature;
		U16	FMFeature;
		U16 A2DP;
		U8 AVRCP;
		U8 Battery;
		U8 customerUART;
		U8 Keys;
		U8 AirFeature;
	}MMIFeatureCtl;
	
	
	//SpkGainSet
	#define SOUND_LEVEL_NO			16
	typedef struct
	{
	////    SCO    ////////
		U8 digitalGain;
		U8 analogGain;
	//////Line In////////
		U8 digitalGainL;
		U8 analogGainL;
	////A2DP/////////
		U8 digitalGainA;
		U8 analogGainA;
	}SpkGainSet;

	typedef struct
	{
		U8 digitalGainWB;
		U8 analogGainWB;
	}SpkGainSetSCOWB;

	typedef struct
	{
		U8 digitalGain;
		U8 digitalGainL;
		U8 digitalGainVC;
		U8 digitalGainA;
		U8 analogGain_R;
		U8 analogGainL_R;
		U8 analogGainVC_R;
		U8 analogGainA_R;
		U8 analogGain_L;
		U8 analogGainL_L;
		U8 analogGainVC_L;
		U8 analogGainA_L;
	}MicGainSet;

	typedef struct
	{
		U8 digitalGainWB;
		U8 analogGainWB_R;
		U8 analogGainWB_L;
	}MicGainSetSCOWB;

	typedef struct
	{
		U8 digitalGainA2DP;
		U8 analogGainA2DP_R;
		U8 analogGainA2DP_L;
	}MicGainSetA2DP; 

	typedef struct
	{
		U8 spkGainSetIndex;
		U8 micGainSetIndex;
	}VgsTable;
	
	
	typedef struct
	{
		U8 vgs;
		U8 beepTone;
	}SoundLevel;
	
	typedef struct
	{
		U8 baseSoundLevel;
		U8 decreaseGain;
	}MmiDecreaseVolWhenLowBatType;
	
	typedef struct
	{
		U8 fmVolumeSmallStep;
		U8 lineInVolumeSmallStep;
		U8 a2dpVolumeSmallStep;
		U8 scoVolumeSmallStep;
	}MMIVolumeSmallStepsType;
	
	typedef struct
	{
		struct {
			U8 level0;
			U8 level1;
			U8 level2;
			U8 level3;
		} ledDuration;
		U8 lowBatteryLEDTimer;
		U8 lowBatteryRingTimer;
		struct {
			U8 tempDetectorSource;
			U8 aioEnableGPIO;
			U8 tempDetectorOnWaitStableTime; // unit:10ms
			U8 tempDetectorDetectingTime;  // unit:10ms
			U16 tempDetectorSleepTime; //unit:0.5 sec	
		} tempInfo;
	}BatteryPara;
	
	#define MMI_NO_TEMP_DETECT	0xFF
	
	enum
	{
		ACTION_NORMAL,
		ACTION_POWER_ON_PROHIBITED,
		ACTION_POWER_ON_NO_CONNECTIVITY_UNTIL_BAT_OK,
		ACTION_POWER_ON_NO_VALID_KEYS_UNTIL_BAT_OK,
		ACTION_POWER_ON_NO_CONNECTIVITY_NO_VALID_KEYS_UNTIL_BAT_OK,
	};

	typedef struct
	{
		U8 powerOnInLowBat;
		U8 powerOnInLowBatChargerIn;
	}MMIPowerOnInLowBatCtlType;
	
	
	//TimerCtl
	typedef struct
	{
		U16 AutoSwitchOffTimer;	  				//unit: 1s
		U16 AutoPowerOnTimer;	  				//unit: 0.1s
		U16 NetworkServiceIndicatorRepeatTime;	//unit: 1s
		U16 DisPowerOffAfterPowerOnTime; 		//unit: 1s
		U16 PairModeTimeout;		  	//unit: 1s
		U16	MuteReminderTime;		  	//unit: 1s
		U16 ConnectableTimeout;		  	//unit: 1s
		U16 PairModeTimeoutIfPDLIsEmpty;//unit: 1s
		U8 ReConnectDelayTimer;			//unit: 1s
		U8 CheckChargingTimer;	      	//unit: 0.5 hour (30 mins)
		U16 SniffTimer; 		      	//unit: 20 ms
		U16 reserved1;
		U8 HFPSLCTimer;					//unit: 1s
		U8 PowerSavingLightTimer;     	//unit: 0.5 min (30 secs)
		U8 AutoSwitchOffInLowBatTimer;	//unit: 1min
		U16 RefreshEncKeyTimer;			//unit: 1min
		U8 IncomingCallRingTime;		//unit: 0.1s
		U8 RingtonePlayingTime; 		//unit: 0.5s
		U8 BatteryMonitoringPeriod;		//unit: 1sec
		U8 DelayChgMediaEvtTimer;		//unit: 10ms
		U8 AudioTransferDetachTime;		//unit: 0.5s
		U8 CheckRemoteAGAcceptEnterSniffTimer;//unit:1s
		U8 reserved2;
		U8 estSCOAfterOggCallTime;		//unit: 0.1s	   
		U8 checkA2DPMediaChannelEstTime;//unit: 0.1s
		U8 RSSIReadIntervalPeriod;		//unit: 1s
		U8 missedCallRemindTimer;		//unit: 1s
		U8 waitForResumeTimer;			//unit: 0.1s
		U8 delayforCloseAudioTimer; 	//uint: 0.1s
		U16 openAudioAmpDelayTime;		//unit: 1ms
		U8 peqChangeMutetime; 			//unit: 10ms	
		U8 FMPowerOnOffDelayTimer;		//unit: 0.1s
		U8 FMSeekTuneDelayTimer;		//unit: 0.1s
		U8 pmSdapWaitTimer;				//unit: 1s
		U8 pmWatchDogTimer;				//unit: 1s
		U8 mmiWatchDogReconnectTimer;	//unit: 0.1s
		U8 autoAnswerTime;				//unit: 0.1s
		U8 CloseAmpAfterMuteSpkTime; 	//unit: 0.1s
		U8 reserved3;
		U8 reconnectdurationLinkLossTimer; //unit:0.5s
		U8 twsPairingRemindTime; 		//unit: 0.1s	
		U8 twsPairingTimeout;			//unit: 1.28s
		U8 twsRelayDropPacketTimeout;	//unit: 10ms
		U8 twsRelayNeedSwitchTimeout;	//unit: 0.1s	
		U8 reserved4;
		U8 twsIdlePowerOffTime;			//unit: 10s
		U8 callerIDRepeatTime; 			//unit: 0.1sec
		U8 threeWireMuteTimer; 			//unit: 10ms
		U8 waitDetachTime;				//unit: 0.5 sec
		U8 soundEffectMutetimer; 		//unit: 10ms	
	}TimerCtl;
	
	typedef struct
	{
		U8	LangCount;
		U8	LangSelectTime;	//unit: 0.1s
		U8	LangSelectRound;
	} VoicePromptConfig;
	
	//EvtPara
	#define EVENT_TABLE_SIZE 		75
	#define INVALID_BEEP_PATTERN	0xFF
	
	#define RINGTONE_OVERRIDE_BIT					0x80 //in VoicePromptIndex
	#define VOICE_PROMPT_BEFORE_RINGTONE_BIT 	0x80 //in Timeout
	#define EVENT_LED_FG_TIMEOUT_MASK				0x7F//for Timeout
	typedef struct
	{
		U16 EvtOpcode;
		U8 ledDataIndex;
		U8 BeepPatternIndex;
		U8 VoicePromptIndex;
		U8 Timeout;
	}EvtPara;
	
	//LedFilterMap
	#define MAX_LED_FILTER_NO 		16
	typedef struct
	{
		U16 evtOpcode;
		U8 type;
		U8 para;
	}LedFilterMap;
	
	//MMIFCCInfo
	typedef struct
	{
		U8 Reg32B1; 
		U8 Reg32B2;
		U8 Reg35B1;
		U8 Reg35B2;
	}MMIFCCInfo;
		
	
	//MmiAvrcpSettings
	//Following defines AVRCP notify event mask
	#define MMI_AVRCP_PLAYBACK_STATUS_CHANGED	0x01
	#define MMI_AVRCP_TRACK_CHANGED				0x02
	#define MMI_AVRCP_TRACK_REACHED_END			0x04
	#define MMI_AVRCP_TRACK_REACHED_START		0x08
	#define MMI_AVRCP_PLAYBACK_POS_CHANGED		0x10
	#define MMI_AVRCP_VOLUME_CHANGED			0x20
	#define MMI_AVRCP_PLAYER_APPLICATION_SETTING_CHANGED	0x40

	//Following defines AVRCP media event mask
	#define MMI_AVRCP_MEDIA_TITLE				0x01
	#define MMI_AVRCP_MEDIA_ARTIST_NAME			0x02
	#define MMI_AVRCP_MEDIA_ALBUM_NAME			0x04
	#define MMI_AVRCP_MEDIA_TRACK_NUMBER		0x08
	#define MMI_AVRCP_MEDIA_TOTAL_TRACKS		0x10
	#define MMI_AVRCP_MEDIA_GENRE				0x20
	#define MMI_AVRCP_MEDIA_PLAYING_TIME		0x40
	
	typedef struct
	{
		U8 notifyEventMask;
		U8 playbackPosInterval;	//unit in seconds
		U8 mediaEventMask;
		U8 continueResponseCount;
	}MmiAvrcpSettings;
	
	
	//mmiHfpCINDMask
	//Following defines HFP CIND mask
	#define MMI_HFP_CIND_SERVICE				0x01
	#define MMI_HFP_CIND_SHOWALL				0x02	//accept all indicators
	#define MMI_HFP_CIND_SHOWEXT				0x04	//accept unknown indicators
	#define MMI_HFP_CIND_CUSTOM_CODE			0x08	//make copy to custom code
	#define MMI_HFP_CIND_SIGNAL					0x10
	#define MMI_HFP_CIND_ROAM					0x20
	#define MMI_HFP_CIND_BATTCHG				0x40

	
	typedef struct
	{
		U8 tws_sbc_target_blocks_offset;
		U8 tws_nblks_of_large_RX_memory;
	} TWS_PARA;

	enum
	{
		AUDIO_DEVICE_NONE,
		AUDIO_DEVICE_SCO,
		AUDIO_DEVICE_SCO_HSP,
		AUDIO_DEVICE_SCO_IDLE,
		AUDIO_DEVICE_MUSIC,
		AUDIO_DEVICE_MUSIC_CALL_NOTIFY,
		AUDIO_DEVICE_KEY_AT_CMD,
		AUDIO_DEVICE_SYS, 
		AUDIO_DEVICE_LINE_IN = AUDIO_DEVICE_SYS,
		AUDIO_DEVICE_FM,
		AUDIO_DEVICE_NUM = AUDIO_DEVICE_FM,
		AUDIO_DEVICE_DEMO_SOUND,
		AUDIO_DEVICE_CALL_STATE,
		AUDIO_DEVICE_AVRCP_PLAY,
		AUDIO_DEVICE_MIC_TO_SPK,		
	};
	
	/*
	#if (AUDIO_DEVICE_SYS >7)
		#error AUDIO_DEVICE_SYS is not allowed.
	#endif */
	
	typedef struct
	{
		U8 priority[AUDIO_DEVICE_NUM];
	}AudioPriorityType;
	
	enum
	{
		PAIR_FAIL_DISC_DEFAULT,
		PAIR_FAIL_RETRY_ONCE,
		PAIR_FAIL_ALWAYS_RETRY,
		PAIR_FAIL_DO_NOTHING,
	};

	enum
	{
		SDAP_DO_NOTHING,
		SDAP_FIRST_CONN_HFP,
		SDAP_FIRST_CONN_AVRCP,
	};

	enum
	{
		MMI_HFP_NONE,
		MMI_HFP_INCOMING_VP,
		MMI_HFP_CALLER_NAME,
		MMI_HFP_CALLER_ID,
		MMI_HFP_RINGTONE,
		MMI_HFP_VOICE_NUM = MMI_HFP_RINGTONE,
	};
	
	typedef struct
	{
		U8 pairFailActionInActConn:4;
		U8 pairFailActionInPasConn:4;
	}PairCtl;
	
	///// NV RAM
	typedef struct
	{
		PinCodeCtl pinCodeCtl;
		U8 inquiryRSSITH;
		U8 twsKChannel;
		U8 twsLinkLossRetryCnt;
		U32 twsKIdleChangeBdaddrTime; //unit:100ms
		U32 twsKWaitChangeBdaddrTime; //unit:100ms
		U8 linkLossLoopCnt;
		U8 Reserved3[24];
		PageScanWindowSize pageScanWindowSize;
		ScoPktPara scoPktPara;
		ChangePktTypePara changePktTypePara;
		SimplePairingPara simplePairingPara;
		MMI1IntInterval mmi1IntInterval;
		MMIBtnSeqInfo mmiBtnSeqInfo[MAX_BTN_SEQ_NO];
		SniffPara sniffPara;
		U8 retrySniffCnt;
		U8 rssiDefaultBoundry;
		LedSettingType defaultLedSetting[MMI_TOTAL_STATE_NO];
		LedSettingType maxLinkLedSetting;
		U8 lineInAutoDetectKeyIndex;
		KeypadGpioCtl keypadGpioCtl;
		ProductType productType;
		AirParaType airPara;
		U8 maxPageDevice;
		U8 missedCallRemindAttempt;
		U8 discoverableIfPdlLessThan;
		U8 actionAfterPairingModeTimeout;
		ReConnectInfo reConnectInfo;
		MMIFeatureCtl mmiFeatureCtl;
		SpkGainSet spkGainSet[SOUND_LEVEL_NO];
		MicGainSet micGainSet[SOUND_LEVEL_NO];
		VgsTable vgsTable[SOUND_LEVEL_NO];
		SoundLevel soundLevels[SOUND_LEVEL_NO];
		SoundLevel musicSoundLevels[SOUND_LEVEL_NO];
		SoundLevel lineInSoundLevels[SOUND_LEVEL_NO];
		MmiDecreaseVolWhenLowBatType scoDecreaseGainCtl;
		MmiDecreaseVolWhenLowBatType a2dpDecreaseGainCtl;
		MmiDecreaseVolWhenLowBatType lineinDecreaseGainCtl;
		U8 totalSoundLevel;
		U8 totalMusicSoundLevel;
		U8 totalLineInSoundLevel;
		U8 defaultA2DPSoundLevel;
		U8 reserved5;
		U8 defaultVCLevel;
		MMIVolumeSmallStepsType smallStepVolume;
		MMIPowerOnInLowBatCtlType powerOnLowBatAction;
		BatteryPara batteryPara;
		TimerCtl timerCtl;
		VoicePromptConfig vpConfig;
		EvtPara evtParaCtl[EVENT_TABLE_SIZE];
		U8 key1GpioIndex;
		LedFilterMap ledFilterMap[MAX_LED_FILTER_NO]; 
		MMIFCCInfo mmiFCCInfo;	
		U8 storedLinkKeyCnt;
		U8 gpioAmpGeneral;
		U8 gpioAmpBtFm;
		MmiAvrcpSettings mmiAvrcpSettings;
		U8 mmiHfpCINDMask;
		U8 fmSeekTH;
		U32 reserved4[3];
		U8	audioPriorityExtra[4];
		TWS_PARA tws_para;
		AudioPriorityType audioPriority;
		// *************************** IMPORTANT************************************
		// flash code version: 001.62971.3204.0
		// NOTICE - structure definition above matches specified flash code version.
		//          DO NOT modify it. ONLY APPENDing fields is allowed.
		// *************************** IMPORTANT************************************
		PairCtl	pariCtl;
		U8 roleSwitchCnt;
		U8 mcuADCSource;
		U8 enlargedWaitDetachTime;	//unit: 0.5 sec
		U8 checkA2DPMediaChannelEstTimeActiveConn;//unit:0.1s
		U32 GENERIC_PTR sbcFlashAddress;
		U8 roleChangeBatteryDiff; //in percent
		U8 roleChangeBatMonitorTime; //unit: half minute
		U8 roleChangeBatteryThreshold; //in percent
		U8 firstConnectProfile;
		U8 reserved10[64];
		U8 defaultMicSoundLevelInA2DP;
		MicGainSetA2DP micGainSetA2DP[SOUND_LEVEL_NO];
		U8 incomingCallVoicePriority[4];
		SpkGainSetSCOWB spkGainSetSCOWB[SOUND_LEVEL_NO];
		MicGainSetSCOWB micGainSetSCOWB[SOUND_LEVEL_NO];
	} MMI_NVRAM_TYPE;

	typedef struct
	{
		MMI_NVRAM_TYPE	mmi_nvram;

		CRC16 CRC;
		
	} SECTOR_MMI_NVRAM_STRU;

#endif
