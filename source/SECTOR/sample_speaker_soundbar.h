#include "..\ROM\COMMON\config.h"

PUBLIC SECTOR_MMI_NVRAM_STRU CODE gMMI_nvram_init =
{
	{
			{   //PinCodeCtl
				//Length
				4,
				//Value
				{'0','0','0','0',0,0,0,0,0,0,0,0,0,0,0,0}
			},
			0xE0, //U8 inquiryRSSITH;
			0,	//U8 twsKChannel;
			0, //U8 twsLinkLossRetryCnt;
			0, //U32 twsKIdleChangeBdaddrTime; //unit:100ms
			0, //U32 twsKWaitChangeBdaddrTime; //unit:100ms
			0, //linkLossLoopCnt
			{//U8 Reserved3[24]
				0,
			},
			{//ScanWindowSize
				0x0012, 0x001E
			},
			//{//Sco Packet Para
			//	0x00001f40, 0x00001f40, 0xFFFF, 0x0060, 0x00, 0x038F
			//},
			{//Sco Packet Para
				0x00001f40, 0x00001f40, 0xFFFF, 0x0063, 0x00, 0x038F
			},
			{//ChangePktType Para
				//0x3318, 0x00E0, 0x038F
				0xCC18, 0x00E0, 0x038F//to enable 2M/3M 5 slot packet
			},
			{ //simplePairingPara
			  	0, 4, 3
			},
			{//mmi1IntInterval
				15, 15,
			},
			{
				KEY_ENTER_TESTMODE, {KEY1,KEY_DOUBLE, KEY3,KEY_TAP, KEY1,KEY_DOUBLE, KEY4,KEY_TAP, KEY1,KEY_DOUBLE, KEY1,KEY_TAP},
				0, {KEYNONE,0, 0,0, 0,0, 0,0, 0,0, 0,0},
			},
			{//Sniff Para
				//Modified by EricChou,Ken Chang 2008.12.31
				//First 2 parameters have been modified from 200 to 800.
				872, 728, 4, 1,
				2000, 2000, 2000
			},
			3, //retrySniffCnt
			25, // rssiDefaultBoundry;
			{
				//LED setting for each main state
				{0x08},	 //MMI_OFF
				{0x0a},		 //MMI_CONDISCABLE
				{0x0a}, //MMI_CONNECTABLE
				{0x02},	 //MMI_HFP_CONNECTED
				{0x05},  //INCOMING CALL
				{0x06},	 //OUTGOING
				{0x06},//CALLACTIVE
				{0x06},//CALLACTIVE_WITHOUT_SCO
				{0x06},	 //MMI_HFP_CAIMG
				{0x06},	 //MMI_HFP_CAOGG
				{0x06},	 //MMI_HFP_CAMULTY
				{0x08},	 //FAKE_ON
				{0x08},	 //FAKE_OFF
				{0x08},	 //DETACH_LINK
				{0x0b}, //TEST MODE
				{0x02}, //MMI_FM,
				{0x02}, //MMI_LINE_IN,
				{0x08}, //MMI_UPDATING
				{0x09}, //MMI_UPDATED_OK
				{0x0a}, //MMI_UPDATED_FAIL
				{0x0a},	//MMI_VOICE_PROMPT_LANG_SELECT,
				{0x05}, //MMI_TWS_PAIRING,
				{0x08},  //MMI_INQUIRY
				{0x08},  //MMI_MP_STATE
				{0x08},  //MMI_STATE_RESERVED1
				{0x08},  //MMI_STATE_RESERVED2
				{0x08}, //MMI_STATE_RESERVED3
				{0x08}, //MMI_STATE_RESERVED4,
				{0x08}, //MMI_STATE_RESERVED5,
				{0x08}, //MMI_STATE_RESERVED6
				{0x08}, //MMI_STATE_RESERVED7
			},
			{  // maxLinkLedSetting
				0x09
			},
			{  // lineInAutoDetectKeyIndex
				0xFF,
			},
			{//KeypadGpioCtl
				{
					{
					  // keyMultyTime[TOTAL_KEY_NO]; // in 0.1 s
					  #ifdef ASIC_RELEASE
					  1, 1, 1, 1,  1, 1, 1, 1,1, 1,
					  #else
					  5, 5, 5, 5,  5, 5, 1, 1,1, 1,
					  #endif
					},
					{
					  // keyPressTime[TOTAL_KEY_NO]; // in 0.1 s
					  #ifdef ASIC_RELEASE
					  1, 1, 1, 1,  1, 1, 1, 1,1, 1,
					  #else
					  15, 15, 15, 15, 15, 15, 19, 19, 19, 19,
					  #endif
					},
					{
					  // keyLongPressTime[TOTAL_KEY_NO]; // in 0.1 s
					  #ifdef ASIC_RELEASE
					  1, 1, 1, 1,  1, 1, 1, 1,1, 1,
					  #else
					  10, 10, 10, 10, 10, 10,  10, 10, 10, 10,
					  #endif
					},
					{
					  // keyLongLongPressTime[TOTAL_KEY_NO]; // in 0.1 s
					  #ifdef ASIC_RELEASE
					  1, 1, 1, 1,  1, 1, 1, 1,1, 1,
					  #else
					  30, 10, 10, 10, 10, 10, 10, 10, 10, 10,
					  #endif
					},
					{
					  // keyExtremelyLongPressTime[TOTAL_KEY_NO]; // in 0.1 s
					  #ifdef ASIC_RELEASE
					  1, 1, 1, 1,  1, 1, 1, 1,1, 1,
					  #else
					  30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
					  #endif
					},
					{
					  // keyContPressTime[TOTAL_KEY_NO]; // in 0.1 s
					  #ifdef ASIC_RELEASE
					  1, 1, 1, 1,  1, 1, 1, 1,1, 1,
					  #else
					  5, 5, 20, 20, 5, 5, 5, 5, 5, 5,
					  #endif
					},
				},
				//0x01, 0x02, 0x10, 0x20, 0x33
				#ifdef OPERATION_IN_BB_FPGA_VERSION
				0x00, 0x02, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  0x0000000E
				#elif defined (FLASH_BOOT)
				0x00, 0x02, 0x03, 0x01, 0x06, 0x07, 0x00, 0x00, 0x00,  0x00000006 //-ice
				#elif defined (ASIC_DBG_PORT)
				0x00, 0x02, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  0x0000000E //-ice
				#else
				    #ifdef SELFIE_App
				        0x04, 0x02, 0x03, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00,  0x0000003E //-ice
				    #else
				        0x00, 0x02, 0x03, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00,  0x0000002E //-ice
				    #endif				
				#endif
			},
			{ //productType
				0,	//reserved
				PRODUCT_TYPE_SPEAKER,
				PRODUCT_ID_TWS,  //pnpProductID
				PRODUCT_VER_TWS << 4 | PRODUCT_AIR_SOUNDBAR, //pnpProductVersion
			},
			{ //airPara
				LPF_CUTOFF_800HZ, //U8 LPFCutFreq;
				WITHOUT_EXTERNAL_MCU, //U8 relayScoSource;
				PCM_FS_SEL_2KHZ, //U8 A2DP_FS_SEL;
				PCM_FS_SEL_2KHZ, //U8 AUX_FS_SEL;
			},
			#ifdef CELT_MODULE 
			1, //maxPageDevice
			#elif defined SUPPORT_1TO2AG_SOUNDBAR
			3, //maxPageDevice
			#else
			2, //maxPageDevice
			#endif
			8, //missedCallRemindAttempt
			0, //discoverableIfPdlLessThan
			POWER_OFF_IF_PDL_IS_EMPTY,//actionAfterPairingModeTimeout - 0:Return to Connectable / 1:Power Off / 2:Power off if PDL is empty
			{//reConnectInfo
				RECONNECT_LAST_ONE,	//afterPowerOn
				RECONNECT_LAST_DISC,	//afterLinkLoss only two options: RECONNECT_LAST_DISC and RECONNECT_UNNECESSARY
				RECONNECT_LAST_ONE,		//userInitiated
				0x05, //watchDogReTryCnt
				0xFF,
				0x03,
				0x05,
			},
			{//MMI Feature
				0x21, //System
				0x2BE9, 0x84, 0x07F6,  // Audio
				0x01, //LED
				0x00, //Mode
				0x70, //HSP
				0x001F
				#ifdef _HFP_CODEC_NEGO_
					| 0x0080
				#endif
				#ifdef _HFP_HF_INDICATORS_
					| 0x0100
				#endif
				, //HFP
				0x0030, //Connection
				0x1E ,  //ReConnection
				0x0006, //LineInFeature
				0x0606, //FMFeature
				#ifdef OPERATION_IN_BB_FPGA_VERSION
				0x0142, //A2DP
				#else
				0x01C2,// 0x0182 | 0x0020, //A2DP //MMI_MUSIC_LAST_IN_FIRST_PLAYED_NO_PAUSE_OTHERS_FEAT //party mode
				//0x0182 | 0x0200, //A2DP //MMI_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT //private mode
				#endif
				0x03, //AVRCP
				0x04, //Battery
				0x00 | 0x01, //customerUART
				0x21, //Keys
				0x00, //AirFeature
			},
			{
				//SpkGainSet spkGainSet[16]  (digitalGain, analogGain, digitalGainL, analogGainL, digitalGainA, analogGainA)
				{ 3,  14,  3,  14,  3,  14},
				{ 7,  14,  7,  14,  7,  14},
				{13,  14, 13,  14, 13,  14},
				{19,  14, 19,  14, 19,  14},
				{25,  14, 25,  14, 25,  14},
				{31,  14, 31,  14, 31,  14},
				{37,  14, 37,  14, 37,  14},
				{43,  14, 43,  14, 43,  14},
				{49,  14, 49,  14, 49,  14},
				{55,  14, 55,  14, 55,  14},
				{61,  14, 61,  14, 61,  14},
				{67,  14, 67,  14, 67,  14},
				{73,  14, 73,  14, 73,  14},
				{79,  14, 79,  14, 79,  14},
				{85,  14, 85,  14, 85,  14},
				{91,  14, 91,  14, 91,  14}
			},
			{
				//MicGainSet micGainSet[16]
				//(digitalGain, digitalGainL, digitalGainVC, digitalGainA, analogGain_R, analogGainL_R, analogGainVC_R, analogGainA_R, analogGain_L, analogGainL_L, analogGainVC_L, analogGainA_L)
			 	{0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0},
				{0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0},
				{0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0},
				{0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0},
				{0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0}, {0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0},
				{0, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 0},
			},
			{
				//VgsTable vgsTable[16] (spkGainSetIndex, micGainSetIndex)
			 	{0, 15}, {1, 14}, {2, 13}, {3, 12}, {4, 11}, {5, 10}, {6, 9}, {7, 8},
				{8, 7}, {9, 6}, {10, 5}, {11, 4}, {12, 3}, {13, 2}, {14, 1}, {15, 0},
			},
			{
			  	//SoundLevel newSoundLevels[16];
				//VGS, Beep tone
			  	{0, SHORT_1_NOTE_1},
				{1, SHORT_1_NOTE_1},
				{2, SHORT_1_NOTE_1},
				{3, SHORT_1_NOTE_1},
				{4, SHORT_1_NOTE_1},
				{5, SHORT_1_NOTE_1},
				{6, SHORT_1_NOTE_1},
			  	{7, SHORT_1_NOTE_1},
				{8, SHORT_1_NOTE_1},
			  	{9, SHORT_1_NOTE_1},
				{10, SHORT_1_NOTE_1},
				{11, SHORT_1_NOTE_1},
				{12, SHORT_1_NOTE_1},
				{13, SHORT_1_NOTE_1},
				{14, SHORT_1_NOTE_1},
				{15, SHORT_1_NOTE_1},
			},
			{
				//musicSoundLevels[16]
				{0, SHORT_1_NOTE_1},
				{1, SHORT_1_NOTE_1},
				{2, SHORT_1_NOTE_1},
				{3, SHORT_1_NOTE_1},
				{4, SHORT_1_NOTE_1},
				{5, SHORT_1_NOTE_1},
				{6, SHORT_1_NOTE_1},
			  	{7, SHORT_1_NOTE_1},
				{8, SHORT_1_NOTE_1},
			  	{9, SHORT_1_NOTE_1},
				{10, SHORT_1_NOTE_1},
				{11, SHORT_1_NOTE_1},
				{12, SHORT_1_NOTE_1},
				{13, SHORT_1_NOTE_1},
				{14, SHORT_1_NOTE_1},
				{15, SHORT_1_NOTE_1},
			},
			{
			//lineInSoundLevels[16]
				{0, SHORT_1_NOTE_1},
				{1, SHORT_1_NOTE_1},
				{2, SHORT_1_NOTE_1},
				{3, SHORT_1_NOTE_1},
				{4, SHORT_1_NOTE_1},
				{5, SHORT_1_NOTE_1},
				{6, SHORT_1_NOTE_1},
				{7, SHORT_1_NOTE_1},
				{8, SHORT_1_NOTE_1},
				{9, SHORT_1_NOTE_1},
				{10, SHORT_1_NOTE_1},
				{11, SHORT_1_NOTE_1},
				{12, SHORT_1_NOTE_1},
				{13, SHORT_1_NOTE_1},
				{14, SHORT_1_NOTE_1},
				{15, SHORT_1_NOTE_1},
			},
			{ 15, 0}, //scoDecreaseGainCtl
			{15, 0}, // a2dpDecreaseGainCtl
			{15, 0}, //lineinDecreaseGainCtl
			16,	//totalSoundLevel
			17, //totalMusicSoundLevel
			17, // totalLineInSoundLevel
			13,  //defaultA2DPSoundLevel
			0, //reserved4
			7, //defaultVCLevel
			{ //smallStepVolume
				0x00, //fmVolumeSmallStep
				0x00, //lineInVolumeSmallStep
				0x00, //a2dpVolumeSmallStep
				0x00, //normalVolumeSmallStep
			},
			{ //powerOnLowBatAction
				ACTION_NORMAL,  //powerOnInLowBat
				ACTION_NORMAL, //powerOnInLowBatChargerIn
			},
			{ //batteryPara
				{
					8, 7, 6, 5,		//voltage low to high
				},
				20, //Low Battery LED Timer
				20, //Low Battery Ring Timer
				{
					16, //U8 tempDetectorSource; //select build in TEMP;
					15, //U8 aioEnableGPIO;
					10, //U8 tempDetectorOnWaitStableTime; // unit:10 ms
					10, //U8 tempDetectorDetectingTime;  // unit: 10 ms
					180, //U16 tempDetectorSleepTime; //unit:0.5 sec
				},
			},
			{//Timers
				1800,	//AutoSwitchOffTimer: 1s
				20, 	//AutoPowerOnTimer: 0.1s
				10,		//NetworkServiceIndicatorRepeatTime: 1s
				0,	 	//DisPowerOffAfterPowerOnTime: 1s
				300,	//PairModeTimeout: 1s
				10,		//MuteReminderTime, unit : sec
				0,		//ConnectableTimeout, unit : sec; 0--> IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL
				300,	//PairModeTimeoutIfPDLIsEmpty : 1s
				5, 	//ReConnectDelayTimer: 1s
				0,		//CheckChargingTimer: 0.5 hour
				1000,	//SniffTimer; //in 20ms
				0,		//reserved1
				0x64,	//HFPSLCTimer
				0,		//PowerSavingLightTimer //0.5 min (30 secs)		  // 0: disable, and > 0  enable
				10,		//U8 AutoSwitchOffInLowBatTimer unit: 1min
				0,		//U16 RefreshEncKeyTimer unit: 1min
			#if AIRAPP_CALLER_NAME
				50,		//U8 IncomingCallRingTimer, unit: 0.1 sec
			#else
				30,		//U8 IncomingCallRingTimer, unit: 0.1 sec
			#endif
				20,		//U8 ringtonePlayingTime, unit : 0.5 sec
				30,		//U8 batteryMonitoringPeriod : 1 sec
				0,		//U8 DelayChgMediaEvtTimer: unit 10ms
				20,		//U8 AudioTransferDetachTime: unit: 0.5s
				3,		//U8 CheckRemoteAGAcceptEnterSniffTimer:1s
				0,		//U8 reserved2;
				80,		//U8 estSCOAfterOggCallTime;   unit:0.1s //iot isseu 8s is the best value
				26,		//U8 checkA2DPMediaChannelEstTime;	//unit:0.1S
				10,		//U8 RSSIReadIntervalPeriod
				15,		//U8 missedCallRemindTimer
				20,		//U8 waitForResumeTimer;		// unit: 0.1s
				100,	// U8 delayforCloseAudioTimer; //uint:0.1S
				120,	// U16 openAudioAmpDelayTime;	//unit:1ms	   //issue #2907
				100,	// U8 peqChangeMutetime; //unit:10ms
				30,		//U8 FMPowerOnOffDelayTimer;	//unit:0.1s
				60,		//U8 FMSeekTuneDelayTimer;		//unit:0.1s
				3,		//pmSdapWaitTimer, (unit in second)
				30,		//pmWatchDogTimer, (unit in second)
				60,		// mmiWatchDogReconnectTimer;	//unit: 0.1s
				0,		//autoAnswerTime;	// unit: 0.1s
				50,		//U8 CloseAmpAfterMuteSpkTime; //// unit: 0.1s
				0,		//U8 reserved3;
				0,		//reconnectdurationLinkLossTimer unit:0.5 sec
				40,		//U8 twsPairingRemindTime; //unit: 0.1S
				60,		//U8 twsPairingTimeout;////unit: 1.28S
				10,		//U8 twsRelayDropPacketTimeout;//unit: 10ms
				10,		//U8 twsRelayNeedSwitchTimeout;//unit: 0.1S
				0,		//U8 reserved4;
				0,		//U8 twsIdlePowerOffTime;	//unit: 10s
			#if AIRAPP_CALLER_NAME
				50,	//U8 callerIDRepeatTime; // unit: 0.1 sec
			#else
				100,	//U8 callerIDRepeatTime; // unit: 0.1 sec
			#endif
				7,		//U8 threeWireMuteTimer; 	//unit: 10ms
				10,		//U8 waitDetachTime;	//unit: 0.5 sec
				20,		// U8 soundEffectMutetimer; //unit:10ms
			},
			{//VoicePromptConfig
				LANG_COUNT,	//LangCount
				50,				//LangSelectTime	//unit: 0.1s
				5,				//LangSelectRound
			},
			{//Evt Table
				{
					MEDIA_EVT_SLC_CONNECTED, LED_INVALID, RISING_4_NOTE, VP_EVT_CONNECTED, 0
				},
				{
					MEDIA_EVT_2_SLC_CONNECTED, LED_INVALID, RISING_4_NOTE, VP_EVT_SECOND_DEVICE_CONNECTED, 0
				},
				{
					MEDIA_EVT_3_SLC_CONNECTED, LED_INVALID, RISING_4_NOTE, VP_EVT_SECOND_DEVICE_CONNECTED, 0
				},
				{
					MEDIA_EVT_4_SLC_CONNECTED, LED_INVALID, RISING_4_NOTE, VP_EVT_SECOND_DEVICE_CONNECTED, 0
				},
				{
					MEDIA_EVT_SLC_DISCONNECTED, LED_INVALID, FALLING_4_NOTE, VP_EVT_DEVICE_DISCONNECTED, 0
				},
				{
					MEDIA_EVT_LINK_LOSS, LED_INVALID, SHORT_2_NOTE, VP_EVT_DEVICE_DISCONNECTED, 0
				},
				{
					MEDIA_EVT_PAIRING_SUCCESSFUL, 0x03, SHORT_1_NOTE_3, VP_EVT_PAIRING_SUCCESSFUL, 20
				},
				{
					MEDIA_EVT_PAIRING_FAIL, LED_INVALID, SHORT_2_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_BATTERY_INTERVAL_0, LED_INVALID, INVALID_BEEP_PATTERN, VP_EVT_BATTERY_IS_LOW,0
				},
				{
					MEDIA_EVT_BATTERY_INTERVAL_1, LED_INVALID, INVALID_BEEP_PATTERN, VP_EVT_BATTERY_IS_MEDIAN,0
				},
				{
					MEDIA_EVT_BATTERY_INTERVAL_2, LED_INVALID, INVALID_BEEP_PATTERN, VP_EVT_BATTERY_IS_MEDIAN,0
				},
				{
					MEDIA_EVT_BATTERY_INTERVAL_3, LED_INVALID, INVALID_BEEP_PATTERN, VP_EVT_BATTERY_IS_HIGH, 0
				},
				{
					MEDIA_EVT_BAT_LOW_LED, 0x03, INVALID_BEEP_PATTERN, VOICE_PROMPT_INVALID, 10
				},
				{
					MEDIA_EVT_BAT_LOW_RING, LED_INVALID, SHORT_2_NOTE, VP_EVT_LOW_BATTERY, 0
				},
				#ifdef ASIC_RELEASE
					{
						MEDIA_EVT_POWER_ON, 0x00|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VOICE_PROMPT_INVALID, 30
					},
				#else
					{
						MEDIA_EVT_POWER_ON, 0x00|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_ON, 30
					},
				#endif
				{
					MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT1, 0x00|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_ON, 30
				},
				{
					MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT2, 0x00|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_ON, 30
				},
				{
					MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT3, 0x00|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_ON, 30
				},
				{
					MEDIA_EVT_POWER_OFF, 0x01|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_OFF, 30
				},
				{
					MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT1, 0x01|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_OFF, 30
				},
				{
					MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT2, 0x01|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_OFF, 30
				},
				{
					MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT3, 0x01|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_OFF, 30
				},
				{
					MEDIA_EVT_POWER_OFF_IN_SHUTDOWN_LOW_BAT, 0x01|LED_OVERRIDE_BIT, LONG_1_NOTE_HIGH, VP_EVT_POWER_OFF, 30
				},
				{
					MEDIA_EVT_ENTER_INCOMING, LED_INVALID, MEDIAN_1_NOTE, VP_EVT_INCOMING_CALL, 0
				},
				{
					MEDIA_EVT_RINGTONE, LED_INVALID, SHORT_2_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_RINGTONE_2, LED_INVALID, SHORT_2_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_RINGTONE_IN_CALL, LED_INVALID, SHORT_2_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_RINGTONE2_IN_CALL, LED_INVALID, SHORT_2_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_RECONNECT_FAILED, LED_INVALID, FALLING_2_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_MUTE_REMINDER, LED_INVALID, SHORT_2_NOTE, VP_EVT_MUTE_ON, 0
				},
				{
					MEDIA_EVT_KEY_MIC_MUTE_ON, LED_INVALID, SHORT_2_NOTE, VP_EVT_MUTE_ON, 0
				},
				{
					MEDIA_EVT_KEY_MIC_MUTE_OFF, LED_INVALID, SHORT_2_NOTE, VP_EVT_MUTE_OFF, 0
				},
				{
					MEDIA_EVT_KEY_VOICE_PROMPT_ENABLE, LED_INVALID, SHORT_2_NOTE, VP_EVT_VOICE_PROMPT_ON, 0
				},
				{
					MEDIA_EVT_KEY_VOICE_PROMPT_DISABLE, LED_INVALID, SHORT_2_NOTE, VP_EVT_VOICE_PROMPT_OFF, 0
				},
				{
					MEDIA_EVT_VOLUME_REACH_MAX, LED_INVALID, SHORT_2_NOTE, VP_EVT_VOLUME_MAXIMUM, 0
				},
				{
					MEDIA_EVT_A2DP_VOLUME_REACH_MAX, LED_INVALID, SHORT_2_NOTE, VP_EVT_VOLUME_MAXIMUM, 0
				},
				{
					MEDIA_EVT_LINEIN_VOLUME_REACH_MAX, LED_INVALID, SHORT_2_NOTE, VP_EVT_VOLUME_MAXIMUM, 0
				},
				{
					MEDIA_EVT_KEY_DISCOVERABLE, LED_INVALID, MEDIAN_1_NOTE, VP_EVT_PAIRING, 0
				},
				{
					MEDIA_EVT_KEY_CNLOUTGOING, LED_INVALID, SHORT_1_NOTE_3, VP_EVT_CALL_CANCELLED, 0
				},
				{
					MEDIA_EVT_KEY_RDIAL, LED_INVALID, VERYLONG_1_NOTE_HIGH, VP_EVT_RE_DIALING, 0
				},
				{
					MEDIA_EVT_KEY_SET_VOICE_RECOGNITION, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_REJCALL, LED_INVALID, SHORT_1_NOTE_3, VP_EVT_CALL_REJECTED, 0
				},
				{
					MEDIA_EVT_KEY_RECONNECT_USER_INIT, LED_INVALID, SHORT_1_NOTE_2, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_3WAYRELNACP, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_3WAYHOLDNACP, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_3WAYRELNUDUB, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_3WAYADD, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_ACPCALL, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_END_OF_CALL, 0x06, FALLING_4_NOTE, VP_EVT_CALL_ENDED, 10
				},
				{
					MEDIA_EVT_KEY_MEDIA_TRIGGER_1, LED_INVALID, MEDIAN_1_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_ENDCALL, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_AUDIO_TRANSFER, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_ENTER_LINE_IN_MODE, LED_INVALID, LONG_1_NOTE_HIGH, VP_EVT_LINE_IN_MODE, 0
				},
				{
					MEDIA_EVT_EXIT_LINE_IN_MODE, LED_INVALID, LONG_1_NOTE_HIGH, VP_EVT_BLUETOOTH_MODE, 0
				},
				{
					MEDIA_EVT_VOICE_PROMPT_LANG_CHANGE, LED_INVALID, INVALID_BEEP_PATTERN, VP_EVT_VOICE_PROMPT_LANG_CHANGE, 0
				},
				{
					MEDIA_EVT_KEY_PEQ_MODE_CHANGE, 	LED_INVALID, MEDIAN_1_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_RESET_PAIRED_DEVICES, LED_INVALID, MEDIAN_1_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_A2DP_FORWARD, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_A2DP_BACKWARD, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				//for tws
				{
					MEDIA_EVT_TWS_ENTER_PAIRING, 	LED_INVALID, MEDIAN_1_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_TWS_DURING_PAIRING, 	LED_INVALID, MEDIAN_1_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_TWS_EXIT_PAIRING, 	LED_INVALID, MEDIAN_1_NOTE, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_KEY_MULTI_A2DP_PLAY_MODE_TOGGLE, LED_INVALID, LONG_1_NOTE_HIGH, VOICE_PROMPT_INVALID, 0
				},
				#ifdef MMI_LE_APP_ENABLE
				//For APP FindMyAccessory
				{
					MEDIA_EVT_KEY_FIND_MY_ACCESSORY, LED_INVALID, INVALID_BEEP_PATTERN, VP_EVT_BATTERY_IS_HIGH, 0
				},
				#endif
				{
					MEDIA_EVT_CALL_CONNECTED, LED_INVALID, SHORT_1_NOTE_3, VOICE_PROMPT_INVALID, 0
				},
				{
					MEDIA_EVT_INCOMING_CALL_ENDED, LED_INVALID, INVALID_BEEP_PATTERN, VP_EVT_INCOMING_CALL_ENDED, 0
				},
				{
					MEDIA_EVT_AG_CNLOUTGOING, LED_INVALID, INVALID_BEEP_PATTERN, VP_EVT_CALL_CANCELLED, 0 
				},
				{
					MEDIA_EVT_AG_REJCALL, LED_INVALID, SHORT_1_NOTE_3, VP_EVT_CALL_REJECTED, 0
				},
			},
			{
				0, // U8 key1GpioIndex;
			},
			{
			//LedFilter
				{
					MEDIA_EVT_BAT_CHGRIN, LED_FILTER_1, 0,
					//MEDIA_EVT_KEY_RECONNECT_USER_INIT, LED_FILTER_CANCEL, LED_FILTER_USER_DEFINED,
				},
				{
					MEDIA_EVT_BAT_RECHG,LED_FILTER_1, 0,
				},
				{
					MEDIA_EVT_BAT_CHGROUT, LED_FILTER_CANCEL, 0,
				},
				{
					MEDIA_EVT_BAT_CHGROUT, LED_FILTER_CANCEL, 1,
				},
				{
					MEDIA_EVT_BAT_CHGCPL, LED_FILTER_CANCEL, 0,
				},
				{
					MEDIA_EVT_BAT_CHGTO, LED_FILTER_2, 1,
				},

			},
			{
				0xC1, 0x4C, 0x00, 0xF9
			},
			{
				NVRAM_LINK_KEY_ITEMS, //storedLinkKeyCnt
			},
			0xFF,	//gpioAmpGeneral
			0xFF,	//gpioAmpBtFm
			{	//mmiAvrcpSettings
				0x01,	//notifyEventMask  
				1,	//playbackPosInterval, (unit in seconds)
				0,	//mediaEventMask
				0	//continueResponseCount
			},
			{	//mmiHfpCINDMask
				0
			},
			{
				0x06	//FM SEEK_threshold 7-bit <6:0>
			},
			{ //U32 reserved4[3];
				0,
				0,
				0,
			},
			{
				AUDIO_DEVICE_FM,
				AUDIO_DEVICE_LINE_IN,
				AUDIO_DEVICE_DEMO_SOUND,
				0,
			}, //audioPriorityExtra[4]
			{	//tws_para
				2,//tws_sbc_target_blocks_offset
				6,//tws_nblks_of_large_RX_memory
			},
			{
				AUDIO_DEVICE_KEY_AT_CMD,
				AUDIO_DEVICE_SCO,
				AUDIO_DEVICE_SCO_HSP,
				AUDIO_DEVICE_SCO_IDLE,
				AUDIO_DEVICE_MUSIC,
				AUDIO_DEVICE_AVRCP_PLAY,
				AUDIO_DEVICE_CALL_STATE,
				AUDIO_DEVICE_MUSIC_CALL_NOTIFY,
			}, //audioPriority
			{
				PAIR_FAIL_DISC_DEFAULT, //pairFailActionInActConn
				PAIR_FAIL_DO_NOTHING, //pairFailActionInPasConn
			}, //PairCtl
			2, //roleSwitchCnt
			MCU_NO_TEMP_DETECT, // U8 mcuADCSource
			70,		//U8 enlargedWaitDetachTime;	//unit: 0.5 sec
			7, //checkA2DPMediaChannelEstTimeActiveConn :0.1s
			NULL, //sbcFlashAddress
			0, //U8 roleChangeBatteryDiff; //in percent
			0, //U8 roleChangeBatMonitorTime; //unit: half minute
			0, //U8 roleChangeBatteryThreshold; //in percent
			SDAP_FIRST_CONN_AVRCP, //firstConnectProfile
			{0},
			7, //defaultMicSoundLevelInA2DP
			{     //digitalGainA2DP,  analogGainA2DP_R,  analogGainA2DP_L;
				{0, 0, 0}, {0, 1, 1}, {0, 1, 1}, {0, 1, 1}, {0, 1, 1}, 
				{1, 4, 4}, {1, 4, 4}, {1, 4, 4}, {1, 4, 4}, {1, 4, 4}, 
				{2, 7, 7}, {2, 7, 7}, {2, 7, 7}, {2, 7, 7}, {2, 7, 7}, 
				{3, 8, 8},
			},
			{	//incomingCallVoicePriority
				MMI_HFP_INCOMING_VP,
				MMI_HFP_CALLER_ID,
				MMI_HFP_CALLER_NAME,
				MMI_HFP_RINGTONE
			},
			{	//SpkGainSetSCOWB spkGainSetSCOWB[16]	(digitalGainWB, analogGainWB)
				{ 3,  14},
				{ 7,  14},
				{13,  14},
				{19,  14},
				{25,  14},
				{31,  14},
				{37,  14},
				{43,  14},
				{49,  14},
				{55,  14},
				{61,  14},
				{67,  14},
				{73,  14},
				{79,  14},
				{85,  14},
				{91,  14},
			},
			{	//MicGainSetSCOWB micGainSetSCOWB[16]	(digitalGainWB, analogGainWB_R, analogGainWB_L)
				{3, 9, 9}, {3, 9, 9}, {3, 9, 9},
				{3, 9, 9}, {3, 9, 9}, {3, 9, 9},
				{3, 9, 9}, {3, 9, 9}, {3, 9, 9},
				{3, 9, 9}, {3, 9, 9}, {3, 9, 9},
				{3, 9, 9}, {3, 9, 9}, {3, 9, 9},
				{3, 9, 9},
			},
	},

    //- CRC
    {0,0}
};

