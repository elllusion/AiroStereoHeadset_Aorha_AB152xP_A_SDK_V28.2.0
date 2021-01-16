
#ifdef AB1520S
	#define SDK_ENABLE	0
	#define PERIODIC_CALLBACK_FUNC_CNT	1
#else
	#define SDK_ENABLE	0
	#define PERIODIC_CALLBACK_FUNC_CNT	10
#endif

#define API_GRP_IOSYS	1
#define API_GRP_IODRV	2
#define API_GRP_PROFILE	3

///////////////////////////////////////////////////////////////////////
//////////////////////////  IO SYS Event call back register ///////////
///////////////////////////////////////////////////////////////////////
enum
{	API_GRP_IOSYS_CB_KEY,
	API_GRP_IOSYS_CB_IIC_START,
	API_GRP_IOSYS_CB_IIC_STOP,
	API_GRP_IOSYS_CB_FM_FM_MODE,
	API_GRP_IOSYS_CB_FM_BT_MODE,
	API_GRP_IOSYS_CB_FM_GET_FREQ,
	API_GRP_IOSYS_CB_FM_SET_FREQ,
	API_GRP_IOSYS_CB_FM_SEEK,
	API_GRP_IOSYS_CB_FM_MUTE,
	API_GRP_IOSYS_CB_FM_VOL,
	API_GRP_IOSYS_CB_FM_ON,
	API_GRP_IOSYS_CB_FM_OFF,
	API_GRP_IOSYS_CB_CHG_EXT_CHG_STATUS,
	API_GRP_IOSYS_CB_CHG_EXT_CHG_IN,
	API_GRP_IOSYS_CB_CHG_STATUS,
	API_GRP_IOSYS_CB_BAT_LOW,
	API_GRP_IOSYS_CB_SYS_POWER_OFF,
	API_GRP_IOSYS_CB_UART_CUST_CMD,
	API_GRP_IOSYS_CB_NVRAM_UPDATE,
	API_GRP_IOSYS_CB_NVRAM_ALLOC_PROG_MEM,
	API_GRP_IOSYS_CB_CNT,
};

typedef BOOL (*API_IOSYS_KEY_CB_FUNC)(U8 keyIndex, U8 keyAction, U8 linkIndex);

typedef void (*API_IOSYS_IIC_START_CB_FUNC)(U8 device_address);
typedef void (*API_IOSYS_IIC_STOP_CB_FUNC)(U8 device_address);

typedef void (*API_IOSYS_FM_FM_MODE_CB_FUNC)(void);
typedef void (*API_IOSYS_FM_BT_MODE_CB_FUNC)(void);
typedef U16  (*API_IOSYS_FM_GET_FREQ_CB_FUNC)(void);
typedef BOOL (*API_IOSYS_FM_SET_FREQ_CB_FUNC)(U16 freq);
typedef BOOL (*API_IOSYS_FM_SEEK_CB_FUNC)(BOOL isUp);
typedef BOOL (*API_IOSYS_FM_MUTE_CB_FUNC)(U8 isMute);
typedef void (*API_IOSYS_FM_VOL_CB_FUNC)(U8 vol);
typedef void (*API_IOSYS_FM_ON_CB_FUNC)(U8 vol,U8 seekTH);
typedef void (*API_IOSYS_FM_OFF_CB_FUNC)(void);

typedef U8 	 (*API_IOSYS_CHG_EXT_CHG_STATUS_CB_FUNC)(void);
typedef U8 	 (*API_IOSYS_CHG_EXT_CHG_IN_CB_FUNC)(void);
typedef void (*API_IOSYS_CHG_STATUS_CB_FUNC)(U8 status);

typedef void (*API_IOSYS_BAT_LOW_CB_FUNC)(void);

typedef void (*API_IOSYS_SYS_POWER_OFF_CB_FUNC)(void);

typedef void (*API_IOSYS_UART_CUST_CMD_CB_FUNC)(U8 XDATA_PTR msgPtr);

typedef void (*API_IOSYS_NVRAM_UPDATE_CB)(U8 CODE_PTR flashAddr);
typedef BOOL (*API_IOSYS_NVRAM_ALLOC_PROG_CB_FUNC)(void);

///////////////////////////////////////////////////////////////////////
//////////////////////////  IO DRV Event call back register ///////////
///////////////////////////////////////////////////////////////////////
enum
{
	API_GRP_IODRV_CB_FM_GETMUTE,
	API_GRP_IODRV_CB_FM_GETFREQ,
	API_GRP_IODRV_CB_FM_SETMUTE,
	API_GRP_IODRV_CB_FM_SETFREQ,
	API_GRP_IODRV_CB_FM_SETVOL,
	API_GRP_IODRV_CB_FM_AUDIO_OFF,
	API_GRP_IODRV_CB_FM_AUDIO_ON,
	API_GRP_IODRV_CB_FM_PWR_OFF,
	API_GRP_IODRV_CB_FM_PWR_ON,
	API_GRP_IODRV_CB_FM_ABT_PWR_ON,
	API_GRP_IODRV_CB_FM_SEEK,
	API_GRP_IODRV_CB_FM_TUNE,
	API_GRP_IODRV_CB_FM_MUTE,
	API_GRP_IODRV_CB_FM_VOL,
	
	API_GRP_IODRV_CB_AMP_ON,
	API_GRP_IODRV_CB_AMP_OFF,
	API_GRP_IODRV_CB_CNT,
};

typedef BOOL (*API_IODRV_FM_GETMUTE_FUNC)(void);
typedef S16	 (*API_IODRV_FM_GETFREQ_FUNC)(void);
typedef void (*API_IODRV_FM_SETMUTE_FUNC)(U8 isMute);
typedef void (*API_IODRV_FM_SETFREQ_FUNC)(S16 freq);
typedef void (*API_IODRV_FM_SETVOL_FUNC)(U8 vol);
typedef void (*API_IODRV_FM_AUDIO_OFF_FUNC)(void);
typedef void (*API_IODRV_FM_AUDIO_ON_FUNC)(void);
typedef U8 (*API_IODRV_FM_PWR_OFF_FUNC)(void);
typedef U8 (*API_IODRV_FM_PWR_ON_FUNC)(void);
typedef void (*API_IODRV_FM_ABT_PWR_ON_FUNC)(void);
typedef U8 (*API_IODRV_FM_SEEK_FUNC)(BOOL isUp);
typedef U8 (*API_IODRV_FM_TUNE_FUNC)(BOOL isUp);
typedef U8 (*API_IODRV_FM_MUTE_FUNC)(BOOL isMute);
typedef void (*API_IODRV_FM_VOL_FUNC)(U8 vol);

typedef void (*API_IODRV_AMP_ON_FUNC)(void);
typedef void (*API_IODRV_AMP_OFF_FUNC)(void);

void SDK_Proxy_RegisterAPI(U8 apiId, U8 apiGrp, VFUN func);


///////////////////////////////////////////////////////////////////////
//////////////////////////  Profile call back register ///////////
///////////////////////////////////////////////////////////////////////
enum
{
	API_GRP_PF_CB_AVDTP_DISCOVER_CMD,
	API_GRP_PF_CB_AVDTP_GET_CAPABILITY_CMD,
	API_GRP_PF_CB_AVDTP_SET_MEDIA_PKT_DECODER,
	API_GRP_PF_CB_AVDTP_SET_CONFIG_CMD,
	API_GRP_PF_CB_AVDTP_GET_CONFIG_CMD,
	API_GRP_PF_CB_AVDTP_GET_CAPABILITY_RESP,
	API_GRP_PF_CB_HCI_INQUIRY_CMD_STATUS,
	API_GRP_PF_CB_HCI_INQUIRY_CPL_EVT,
	API_GRP_PF_CB_HCI_INQUIRY_RESULT_EVT,
	API_GRP_PF_CB_HCI_INQUIRY_RESULT_RSSI_EVT,
	API_GRP_PF_CB_HCI_EXT_INQUIRY_RESULT_EVT,
	API_GRP_PF_CB_HCI_INQ_CANCEL_CMD_CPL_EVT,
	API_GRP_PF_CB_HCI_ROLE_CHANGED_EVT,
	API_GRP_PF_CB_HCI_WRITE_PAGE_TIMEOUT_EVT,
	API_GRP_PF_CB_A2DP_AUDIO_STATUS,
	API_GRP_PF_CB_HFP_AT_RESULT_CODE,
	API_GRP_PF_CB_HFP_AT_RESULT_CODE_OK,
	API_GRP_PF_CB_HFP_AT_RESULT_CODE_ERROR,
	API_GRP_PF_CB_CONNECTION_STATUS,
	API_GRP_PF_CB_AVRCP_PLAY_APP_CHANGED,
	API_GRP_PF_CB_AVRCP_PLAY_STATUS,
	API_GRP_PF_CB_AVRCP_MEDIA_ATTRIBUTE,
	API_GRP_PF_CB_AIRAPP_CUSTOMER_CTL,
	API_GRP_PF_CB_SDP_SERVICE_HANDLER,
	API_GRP_PF_CB_SDP_SERVICE_SEARCH,
	API_GRP_PF_CB_SDP_ATTRIBUTE_SEARCH,
	API_GRP_PF_CB_SDP_SERVICE_SEARCH_ATTRIBUTE,
	API_GRP_PF_CB_SDP_CLEAR_CTL_DATA,
	API_GRP_PF_CB_SDP_GET_SERVICE_PROFILESCN,
	API_GRP_PF_CB_CNT,
};

typedef U8 (*API_PROFILE_AVDTP_DISCOVER_CMD_FUNC)(U8 linkIndex, U8 XDATA_PTR seidPtr);
typedef U8 (*API_PROFILE_AVDTP_GET_CAPABILITY_CMD_FUNC)(U8 seid, U8 XDATA_PTR capaPtr);
typedef VFUN (*API_PROFILE_AVDTP_SET_MEDIA_PKT_DECODER_FUNC)(U8 seid);
typedef U8 (*API_PROFILE_AVDTP_SET_CONFIG_CMD_FUNC)(U8 cfgSeid, U8 XDATA_PTR categoryPtr);
typedef U8 (*API_PROFILE_AVDTP_GET_CONFIG_CMD_FUNC)(U8 seid, U8 XDATA_PTR servicePtr, U8 XDATA_PTR localConfigPtr);
typedef BOOL (*API_PROFILE_AVDTP_GET_CAPABILITY_RESP_FUNC)(U8 XDATA_PTR paraPtr, U8 XDATA_PTR XDATA_PTR pConfigPtr, U8 XDATA_PTR pConfigLen, U8 XDATA_PTR pIntSeid);
typedef void (*API_PROFILE_HCI_INQUIRY_CMD_STATUS_FUNC)(U8 status);
typedef void (*API_PROFILE_HCI_INQUIRY_CPL_EVT_FUNC)(U8 status);
typedef void (*API_PROFILE_HCI_INQ_RESULT_EVT_FUNC)(U8 XDATA_PTR evtPtr);
typedef void (*API_PROFILE_HCI_INQ_RESULT_RSSI_EVT_FUNC)(U8 XDATA_PTR evtPtr);
typedef void (*API_PROFILE_HCI_EXT_INQ_RESULT_EVT_FUNC)(U8 XDATA_PTR evtPtr);
typedef void (*API_PROFILE_HCI_INQ_CANCEL_CMD_CPL_EVT_FUNC)(U8 status);
typedef void (*API_PROFILE_HCI_ROLE_CHANGED_EVT_FUNC)(U8 linkIndex, U8 status, U8 role);
typedef void (*API_PROFILE_A2DP_AUDIO_STATUS_FUNC)(U8 linkIndex, U8 status);
typedef void (*API_PROFILE_HFP_RESULT_CODE_FUNC)(U8 linkIndex, U8 XDATA_PTR resultATPtr);
typedef void (*API_PROFILE_HFP_RESULT_CODE_OK_FUNC)(U8 linkIndex, U8 cmdCode);
typedef void (*API_PROFILE_HFP_RESULT_CODE_ERROR_FUNC)(U8 linkIndex, U8 cmdCode);
typedef void (*API_PROFILE_CONNECTION_STATUS_FUNC)(U8 linkIndex, U8 profileID, U8 status);
typedef void (*API_PROFILE_AVRCP_PLAY_APP_CHANGED_FUNC)(U8 linkIndex, U8 attributeID, U8 attributeValue);
typedef void (*API_PROFILE_AVRCP_PLAY_STATUS_FUNC)(U8 linkIndex, U8 playStatus);
typedef void (*API_PROFILE_AVRCP_MEDIA_ATTRIBUTE_FUNC)(U8 linkIndex, U8 XDATA_PTR mediaPtr);
typedef void (*API_PROFILE_AIRAPP_CUSTOMER_CTL_FUNC)(U8 linkIndex, U8 XDATA_PTR msgPtr);
typedef void (*API_PROFILE_SDP_SERVICE_HANDLER_FUNC)(U8 linkIndex);
typedef void (*API_PROFILE_SDP_SERVICE_SEARCH_FUNC)(U8 linkIndex, U8 XDATA_PTR sdpRspPtr);
typedef void (*API_PROFILE_SDP_ATTRIBUTE_SEARCH_FUNC)(U8 linkIndex, U8 XDATA_PTR sdpRspPtr);
typedef void (*API_PROFILE_SDP_SERVICE_SEARCH_ATTRIBUTE_FUNC)(U8 linkIndex, U8 XDATA_PTR sdpRspPtr);
typedef void (*API_PROFILE_SDP_CLEAR_CTL_DATA_FUNC)(U8 linkIndex);
typedef U8   (*API_PROFILE_SDP_GET_SERVICE_PROFILESCN_FUNC)(U8 linkIndex, U8 profileID);

///////////////////////////////////////////////////////////////////////
//////////////////////////  Periodic call back register ///////////////
///////////////////////////////////////////////////////////////////////


typedef struct
{
	U32 	duration;
	VFUN	cbFunc;
	U8 		isTimerSet:1;
	U8 		oneTimeOnly:1;
	U8 		reserved:6;
	OST 	timer;
}API_PERIODIC_FUNC;


void SDK_Proxy_RegisterPeriodicAPI(U8 apiId, VFUN func, U32 time, BOOL oneTime);