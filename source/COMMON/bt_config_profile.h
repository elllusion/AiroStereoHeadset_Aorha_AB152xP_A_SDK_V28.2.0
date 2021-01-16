#ifndef _BT_CONFIG_PROFILE_H_
#define _BT_CONFIG_PROFILE_H_

#ifndef _CONFIG_H_
#error "This file must include after config.h !!!!"
#endif

#ifdef OPERATION_IN_BB_FPGA_VERSION
	#undef USING_AD_DA
#else
	#define USING_AD_DA
#endif

	//#define ASIC_RELEASE

	#define STEREO_SOC
	#define HEADSET_SOC

	//#define TWS_SETTINGS
	//#define SOUND_BAR
	//#define SCO_WOOFER
	//#define SPEAKER
	//#define FAST_CONNECT_DONGLE
	//#define AIR_UPDATE_DONGLE

	#define SUPPORT_MIC_IN_A2DP_MODE

#ifdef STEREO_SOC
	#define DSP_ENABLE
	#define AUDIO_BIST
	//#define CELT_MODULE
	//#define LIGHTING_ENABLE
	#define FM_ENABLE

	#ifdef TWS_SETTINGS
		/* ======= device type =======  */
			#define AIR_MODULE

		#ifdef AB1520D
			//#define PRODUCT_TYPE_K
            #define PRODUCT_TYPE_A
			#define TWS_ESCO_RELAY
		#else
			#define PRODUCT_TYPE_A
			#define TWS_ESCO_RELAY
		#endif

		/* ======= drivers type =======  */
			#define RINGTONE_ENABLE
			#define SUPPORT_VOICE_PROMPT
			#define LINEIN_ENABLE
			#define PEQ_ENABLE
			#ifndef BLUETOOTH_SPEAKER
				#define SUPPORT_VOICE_COMMAND
				#define DUAL_MIC_ENABLE
				#define SUPOORT_SYS_CLK_CHANGE
			#endif

		/* ======= 3.0 Profiles =======  */
			#define SPP_Profile
			#define A2DP_Profile
			#define AVRCP_Profile
			#define HFP_Profile
			#define AIRAPP_Profile
			//#define LCIF_Profile
			//#define IAP2_Profile
			//#define OPP_Profile
			//#define SAP_Profile
			//#define PBAP_Profile
			//#define MAP_Profile
            //#define KB_App
            //#define SELFIE_App
			#if (defined SELFIE_App || defined KB_App)
				#define HID_Profile
			#endif

		/* ======= 4.0 LE       =======  */
	    #ifdef LE_SUPPORTED_HOST_FLASH
			#define MMI_LE_ENABLE
			#define MMI_LE_APP_ENABLE
    		#define SUPPORT_STATIC_RANDOM_ADDRESS
	        #ifdef AIRAPP_Profile
    		    #define BLE_AIRApp
            #endif            
            //#define APP_ANCS_C
            #ifdef APP_ANCS_C
                #define BLE_ANCS_C
                #define ATT_CLIENT_ENABLE
                #define GATT_ENABLE
                #define ANCS_CELL_PHONE_APP
            #endif

            #if (defined BLE_AIRApp || defined ANCS_CELL_PHONE_APP)
                #define BLE_APP
            #endif

        #endif

	#elif defined SCO_WOOFER
		/* ======= device type =======  */
			#define AIR_MODULE
			//#define SAME_SW_DIFF_MODULE

		/* ======= drivers type =======  */
			#define RINGTONE_ENABLE
			#define SUPPORT_VOICE_PROMPT
			#define LINEIN_ENABLE
			#define PEQ_ENABLE

		/* ======= 3.0 Profiles =======  */
			#define SPP_Profile
			#define A2DP_Profile
			#define AVRCP_Profile

		/* ======= 4.0 LE       =======  */

	#elif defined SOUND_BAR
		/* ======= device type =======  */
			#define AIR_MODULE
			//#define SAME_SW_DIFF_MODULE

		/* ======= drivers type =======  */
			#define RINGTONE_ENABLE
			#define SUPPORT_VOICE_PROMPT
			//#define SUPPORT_VOICE_COMMAND
			#define LINEIN_ENABLE
			//#define PEQ_ENABLE
		/* ======= 3.0 Profiles =======  */
			#define SPP_Profile
			//#define SONGPAL_Profile
			#define A2DP_Profile
			#define AVRCP_Profile
			//#define LCIF_Profile
			//#define IAP2_Profile
			//#define OPP_Profile
			//#define SAP_Profile
			//#define PBAP_Profile
			//#define MAP_Profile
            //#define SELFIE_App
			#if (defined SELFIE_App || defined KB_App)
				#define HID_Profile
			#endif

		/* ======= 4.0 LE       =======  */
        #ifndef CELT_MODULE
            #ifdef LE_SUPPORTED_HOST_FLASH
			#define MMI_LE_ENABLE
			//#define MMI_LE_APP_ENABLE
                #define SUPPORT_STATIC_RANDOM_ADDRESS
            #endif
        #endif

	#elif defined FAST_CONNECT_DONGLE
		/* ======= device type =======  */

		/* ======= drivers type =======  */
			#define OLED_ENABLE
			#define RINGTONE_ENABLE
			#define SUPPORT_VOICE_PROMPT
			#define LINEIN_ENABLE
			#define PEQ_ENABLE

		/* ======= 3.0 Profiles =======  */
			#define HFP_Profile
			#define A2DP_Profile
			#define AVRCP_Profile

			#define HFP_AG_Profile
			#define A2DP_SRC_Profile

			#define AUTO_INCOMING_CALL

		/* ======= 4.0 LE       =======  */
		#ifdef LE_SUPPORTED_HOST_FLASH
			#define MMI_LE_ENABLE
			#define MMI_LE_APP_ENABLE
    		#define SUPPORT_STATIC_RANDOM_ADDRESS
        #endif
	#elif defined AIR_UPDATE_DONGLE
		/* ======= device type =======  */
			//#define AIR_MODULE
			//#define SAME_SW_DIFF_MODULE
			#undef SUPPORT_MIC_IN_A2DP_MODE

		/* ======= drivers type =======  */
			//#define RINGTONE_ENABLE
			//#define SUPPORT_VOICE_PROMPT
			//#define LINEIN_ENABLE
			//#define PEQ_ENABLE
			#undef DSP_ENABLE
			#undef AUDIO_BIST
			#undef FM_ENABLE

		/* ======= 3.0 Profiles =======  */
			/* don't need any profile */
			#define MMI_INQUIRY_CMD
			#define MMI_CREATE_CONNECTION_CMD

		/* ======= 4.0 LE       =======  */
	#elif defined SPEAKER
		/* ======= device type =======  */

		/* ======= drivers type =======  */
			#define RINGTONE_ENABLE
			#define SUPPORT_VOICE_PROMPT
			#define LINEIN_ENABLE
			#define PEQ_ENABLE
			//#define DUAL_MIC_ENABLE

		/* ======= 3.0 Profiles =======  */
			//#define SPP_Profile
			#define HFP_Profile
			#define AIRAPP_Profile
			#define A2DP_Profile
			#define AVRCP_Profile
			//#define LCIF_Profile
			//#define IAP2_Profile
			//#define OPP_Profile
			//#define SAP_Profile
			//#define PBAP_Profile
			//#define MAP_Profile
            //#define SELFIE_App
			#if (defined SELFIE_App || defined KB_App)
				#define HID_Profile
			#endif

		/* ======= 4.0 LE       =======  */
        #ifdef LE_SUPPORTED_HOST_FLASH
			#define MMI_LE_ENABLE
			#define MMI_LE_APP_ENABLE
    		#define SUPPORT_STATIC_RANDOM_ADDRESS
	        #ifdef AIRAPP_Profile
    		    #define BLE_AIRApp
            #endif
            #ifdef BLE_AIRApp
                #define BLE_APP
            #endif
        #endif
	#else
		/* ======= device type =======  */

		/* ======= drivers type =======  */
			#define RINGTONE_ENABLE
			#define SUPPORT_VOICE_PROMPT
			#ifndef AB1520S
			#define SUPPORT_VOICE_COMMAND
			#endif
			//#define VC_MULTI_LAYER
			#define LINEIN_ENABLE
			#define PEQ_ENABLE
			#ifndef AB1520S
				#define DUAL_MIC_ENABLE_CONFIG
				#define DUAL_MIC_ENABLE
			#endif
			#ifndef AB1520S
				#define SUPOORT_SYS_CLK_CHANGE
			#endif
		/* ======= 3.0 Profiles =======  */
			#define SPP_Profile
			#define HFP_Profile
			#define AIRAPP_Profile
			#define A2DP_Profile
			#define AVRCP_Profile
			//#define LCIF_Profile
			//#define IAP2_Profile
			//#define OPP_Profile
			//#define SAP_Profile
			//#define PBAP_Profile
			//#define MAP_Profile
            #define SELFIE_App
			#if (defined SELFIE_App || defined KB_App)
				#define HID_Profile
			#endif

		/* ======= 4.0 LE       =======  */
		#ifdef LE_SUPPORTED_HOST_FLASH
			#define MMI_LE_ENABLE
			#define MMI_LE_APP_ENABLE
    		#define SUPPORT_STATIC_RANDOM_ADDRESS
	        #ifdef AIRAPP_Profile
                #define BLE_AIRApp
            #endif
            #ifdef BLE_AIRApp
                #define BLE_APP
            #endif
        #endif
	#endif
#endif //#ifdef STEREO_SOC


	#define I2C_SPEED_ENABLE
	//#define I2C_SPEED_100K_ENABLE
	//#define I2C_SPEED_800K_ENABLE
	//#define I2C_NONBLOCKING_ENABLE

	#define VOICE_LANG_CHI	1
	#define VOICE_LANG_ENG	1

	#ifdef SUPPORT_VOICE_PROMPT
		#define VP_WB_ENABLE	0
		#define VP_NB_HQ_ENABLE	0
		#define VP_NB_LQ_ENABLE	1

		#if !VP_WB_ENABLE && !VP_NB_HQ_ENABLE && !VP_NB_LQ_ENABLE
			#error "At lease one Voice Prompt type should be defined!!!"
		#endif
	#else
		#define VP_WB_ENABLE	0
		#define VP_NB_HQ_ENABLE	0
		#define VP_NB_LQ_ENABLE	0
	#endif
	#ifdef SUPPORT_VOICE_COMMAND
		#ifndef SUPPORT_VOICE_PROMPT
			#error "Voice Command must work under Voice Prompt Supported!!!"
		#else
			#define VC_PLAN_MODEL	1
		#endif
	#endif


	#ifdef A2DP_SRC_Profile
		#define SUPPORT_SBC_SNK		0
		#define SUPPORT_AAC_SNK		0
		#define SUPPORT_SBC_SRC		1
	#else
		#define SUPPORT_SBC_SNK		1
		#define SUPPORT_AAC_SNK		1
		#define SUPPORT_SBC_SRC		0
	#endif

	//#define DEMOSOUND


	#if defined(PBAP_Profile) || defined(OPP_Profile) || defined(MAP_Profile)
		#define SUPPORT_OBEX
	#endif

    #ifdef HFP_Profile
        #define _THREE_WAY_HANDLING_
        #define _PLACE_CALL_WITH_PHONE_NUMBER_
        #define _MEMORY_DIALING_
        #define _EXTENSION_ENTITIES_
        #define _HFP_CODEC_NEGO_
		#define _HFP_CALLHELD_PREDICT_
		//#define _HFP_HF_INDICATORS_
    #endif


	#define AB1520_FLASH_REMOTE_UPDATE

	#if defined SOUND_BAR || defined SCO_WOOFER
		#define AB1520B_UPDATE_LINK_HISTORY_PERIODICALLY 0
	#else
		#define AB1520B_UPDATE_LINK_HISTORY_PERIODICALLY 0
	#endif

	#ifdef AIRAPP_Profile
		#define AIRAPP_CALLER_NAME		1
	#else
		#define AIRAPP_CALLER_NAME		0
	#endif

    #define SUPPORTED_PEDOMETER (FALSE)

    //#define TRSPX_App
    #ifdef TRSPX_App
        #define SPP_Profile
        #ifdef LE_SUPPORTED_HOST_FLASH
			#define MMI_LE_ENABLE
			#define SUPPORT_LE_XPORT
    		#define SUPPORT_STATIC_RANDOM_ADDRESS
        #endif
		#define TRSPX_SUPPORT_AUTO_CONFIRM
    #endif

    #define RC_LOCAL_NAME_LENGTH        32        // For SOC - to reduce the used eeprom space


    //#define PASSKEY_STRATEGY
    //#define OOB_STRATEGY
    #define NUMERIC_COMPARISON_STRATEGY
    //#define USING_SIMPLE_PAIRING_DEBUG_PRIVATE_KEY

#define RFCOMM_L2CAP_CHANNEL_NUM	MAX_MULTI_POINT_NO
#define SDP_L2CAP_CHANNEL_NUM		(2*MAX_MULTI_POINT_NO)
#ifdef A2DP_Profile
	#define AVDTP_L2CAP_CHANNEL_NUM		(2*MAX_MULTI_POINT_NO)	//AVDTP*2(Signaling and Media)
#else
	#define AVDTP_L2CAP_CHANNEL_NUM		0
#endif
#ifdef AVRCP_Profile
	#define AVCTP_L2CAP_CHANNEL_NUM		MAX_MULTI_POINT_NO
#else
	#define AVCTP_L2CAP_CHANNEL_NUM		0
#endif

#ifdef HID_Profile
	#define HID_L2CAP_CHANNEL_NUM	(2*MAX_MULTI_POINT_NO)
#else
	#define HID_L2CAP_CHANNEL_NUM		0
#endif

#define MAX_NUM_L2CAP_CHANNEL		(RFCOMM_L2CAP_CHANNEL_NUM + SDP_L2CAP_CHANNEL_NUM + AVDTP_L2CAP_CHANNEL_NUM + AVCTP_L2CAP_CHANNEL_NUM + HID_L2CAP_CHANNEL_NUM)

//#define SUPPORT_EXTENDED_INQUIRY_RESPONSE

//#define REMOTE_EEPROM_UPDATE

#define MMI_BATCHG_ENABLE

#define LM_BR_EDR_VERSION           (TRUE)
#ifdef MMI_LE_ENABLE
	#define LM_LE_VERSION               (TRUE)
#else
	#define LM_LE_VERSION               (FALSE)
#endif
#define LM_DUAL_VERSION             (LM_BR_EDR_VERSION && LM_LE_VERSION)

#ifdef AB1520D
	#define PEQ_EXPANSION_ENABLE
	#define PEQ_UNDER_100HZ_ENABLE
#endif

#endif //_BT_CONFIG_PROFILE_H_
