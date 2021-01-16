

/////////////////////////// FM //////////////////////////////////////
#define SDK_FM_ENABLE	0
#define FM_AR1000_CHIP

	#if SDK_FM_ENABLE
		#define SDK_FM_WAIT_ON_SIGNAL_READY_TIME	(2 * 320L)
		#define SDK_FM_CMD_TIMEOUT					((U32)gMMI_nvram.timerCtl.FMPowerOnOffDelayTimer * 320L)
	#endif
	
	
/////////////////////////// Amplifier ///////////////////////////////
#define AMP_ENABLE	0

	#if AMP_ENABLE
		#define AMP_PIO	 							(4)
	#endif
	
	
////////////////////////// NvRam	/////////////////////////////////
#define SDK_NV_RAM	0


///////////////////////// HCI  //////////////////////////////////////
#define SDK_HCI		0

#define SDK_HCI_ROLE_SWITCH_SAMPLE_CODE	0
#define SDK_HCI_INQUIRY_SAMPLE_CODE		0

////////////////////////// Profile     /////////////////////////////////
#define SDK_PROFILE	0

////////////////////////// A2DP     /////////////////////////////////
#define SDK_VENDOR_CODEC	0
	#if SDK_VENDOR_CODEC
		#define CODEC_HIGH_QUALITY		0
		#define CODEC_AAC_DEMO			0
	#endif
	
#define SDK_A2DP	0

////////////////////////// AVRCP    /////////////////////////////////
#define	SDK_AVRCP_SHUFFLE			0
#define SDK_AVRCP_REPEAT			0
#define SDK_AVRCP_KEYS				0
#define SDK_AVRCP_PLAY_STATUS		0
#define SDK_AVRCP_MEDIA_ATTRIBUTE	0

		#if SDK_AVRCP_SHUFFLE || SDK_AVRCP_REPEAT
			#define SDK_AVRCP_PALY_APPLICATION	1
		#else
			#define SDK_AVRCP_PALY_APPLICATION	0
		#endif

////////////////////////// HFP      /////////////////////////////////
#define SDK_VENDOR_AT_CMD		0
#define SDK_HFP_AT_XEVENT_SAMPLE_CODE	0
	
////////////////////////// Vendor Profile  //////////////////////////
#define VENDOR_PROFILE	0
	#define VENDOR_PROFILE_MAX_FRAME_SIZE	80

////////////////////////// Memory Allocation ////////////////////////
#define SDK_MEMORY_ALLOCATION 0

////////////////////////// AirApp Customer //////////////////////////
#define SDK_AIRAPP_CUSTOMER	0

////////////////////////// SDP		/////////////////////////////////
#define SDK_SDP_PROFILE		0