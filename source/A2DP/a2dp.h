//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _A2DP_H_
#define _A2DP_H_

#include "a2dp_interface.h"
#include "..\avdtp\avdtp_interface.h"
	
#define GAVDP_TIMER_100		(3*ONE_SEC)  // by spec GAVDP 1.3 page 30 section 8. Tgavdp is proposed between 0.5~3 seconds
#define GAVDP_TIMER_INTERNAL	(2*ONE_SEC)  // by spec GAVDP 1.3 page 30 section 8. Tgavdp is proposed between 0.5~3 seconds

#define AVDTP_GET_UNACCEPT_RESP			0xFF
#define AVDTP_GET_ACCEPT_RESP			0x01

#define A2DP_CAPS_MEDIA_TRANSPORT		0x01
#define A2DP_CAPS_CONTENT_PROTECTION	0x04
#define A2DP_CAPS_DELAY_REPORTING		0x08

enum
{
	A2DP_SBC,
	A2DP_MPEG_12_AUDIO = 1,
	A2DP_MPEG_24_AAC = 2,
	A2DP_ATRAC_FAMILY = 4,
	NON_A2DP = 0xFF
};

enum
{
	A2DP_DTCP = 0x01,		//Digital Transmission Licensing Administrator
	A2DP_SCMS_T = 0x02,		//SCMS-T uses Cp-bit and L-bit that are defined by IEC60958-3:1999 and IEC61119-6:1992
};

#define AVDTP_MSH_HEADER_SIZE 2

typedef struct
{
	U8 rfa:1;
	U8 inUse:1;
	U8 seid:6;
	U8 rfa2:3;
	U8 tsep:1;
	U8 mediaType:4;
}A2DP_SEID_INFO;

enum 
{
	A2DP_SUBSTATE_BEGINNING_STATE,
	A2DP_SUBSTATE_IDLE_WAIT_DISCOVER_RESP,
	A2DP_SUBSTATE_IDLE_WAIT_GET_CAPABILITY_RESP,
	A2DP_SUBSTATE_IDLE_WAIT_SET_CONFIG_RESP,
	A2DP_SUBSTATE_CONFIG_WAIT_OPEN_RESP,
	A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP,
	//A2DP_SUBSTATE_OPEN_GET_MMI_DSP_RESP,
	A2DP_SUBSTATE_OPEN_MMI_NO_RESP_REPLY_FAIL,
	A2DP_SUBSTATE_OPEN_OPEN_MEDIA_CHANNEL,
	A2DP_SUBSTATE_OPEN_WAIT_MEDIA_CHANNEL_READY,
	A2DP_SUBSTATE_OPEN_WAIT_SEND_MEDIA_CHANNEL,
	A2DP_SUBSTATE_OPEN_WAIT_START,
	A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE_AFTER_ACTIVE_MEDIA_CHANNEL,
	A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE,
	A2DP_SUBSTATE_OPEN_WAIT_SUSPEND,
	A2DP_SUBSTATE_OPEN_SEND_SUSPEND,
	A2DP_SUBSTATE_OPEN_WAIT_SUSPEND_RESPONSE,
	A2DP_SUBSTATE_OPEN_SEND_START_FOR_TWS,
	A2DP_SUBSTATE_OPEN_WAIT_START_RESP_FROM_FOLLOWER,
	A2DP_SUBSTATE_OPEN_SEND_SUSPEND_RESP,
	A2DP_SUBSTATE_STREAM_GET_MMI_DSP_RESP_WAIT_AIR_READY,
	A2DP_SUBSTATE_STREAM_WAIT_DSP_ENABLE_FOR_START_RESP,
	A2DP_SUBSTATE_STREAM_WAIT_MMI_DSP_RESP,
	A2DP_SUBSTATE_STREAM_WAIT_SUSPEND_RESP,
	A2DP_SUBSTATE_STREAM_SEND_SUSPEND_REQ_FOR_TWS, //20
	A2DP_SUBSTATE_CLOSE_NOTIFY_MMI_GET_CLOSE_CMD,
	A2DP_SUBSTATE_CLOSE_WAIT_MMI_CLOSE_RESP,
	A2DP_SUBSTATE_CLOSE_GET_MMI_CLOSE_RESP,
	A2DP_SUBSTATE_CLOSE_SEND_CLOSE_CMD,
	A2DP_SUBSTATE_CLOSE_WAIT_CLOSE_RESP,
	A2DP_SUBSTATE_CLOSE_CLOSE_MEDIA_CHANNEL,
	A2DP_SUBSTATE_CLOSE_WAIT_CLOSE_MEDIA_CHANNEL,
	A2DP_SUBSTATE_CLOSE_CLOSE_CHANNEL,
	A2DP_SUBSTATE_ABORT_PASSIVE_GET_ABORT_CMD,
	A2DP_SUBSTATE_ABORT_WAIT_PASSIVE_RELEASE_MEDIA_CHANNEL,
	A2DP_SUBSTATE_ABORT_SEND_RELEASE_MEDIA_CHANNEL_CMD,
	A2DP_SUBSTATE_ABORT_WAIT_ACTIVE_RELEASE_MEDIA_CHANNEL,
	A2DP_SUBSTATE_ABORT_SEND_ABORT_CMD,
	A2DP_SUBSTATE_ABORT_WAIT_ABORT_RESP,
	A2DP_SUBSTATE_ABORT_DECIDE_RELEASE_MEDIA_CHANNEL,
};

#define A2DP_MAX_SEID_MATCH_NUM		6
#define A2DP_MAX_CAPS_MATCH_SIZE	26

typedef struct
{
	U8 tsep;
	U8 preferSeid;
	U8 count;
	U8 index;
	U8 seid[A2DP_MAX_SEID_MATCH_NUM];
	U8 acpSeid;
	U8 intSeid;
	U8 caps;
	U8 configLen;
	U8 configParam[A2DP_MAX_CAPS_MATCH_SIZE];
}A2dpDiscovSeidInfo;

enum 
{
	A2DP_RELAY_OK,
	A2DP_RELAY_NG,
	A2DP_NO_RELAY	
};

enum
{
	RELAY_STATE_IDLE,
	RELAY_STATE_CHANNEL_READY,
	RELAY_STATE_CHANNEL_BAD,
	RELAY_STATE_CHANNEL_GOOD,
	RELAY_STATE_WAIT_TO_RESUME_RELAY,
	RELAY_STATE_NO,
};

#define A2DP_MEDIA_PKT_ANCHOR_ID		0x0022
#define A2DP_CONTENT_PROTECTION_ID		0x0023

typedef	union
{
	U8 sbc_byte2;
	struct
	{
		U8 SUBBANDS:1;
		U8 ALLOCATION_METHOD:1;
		U8 CHANNEL_MODE:2;
		U8 BLOCKS:2;
		U8 SAMPLING_FREQ:2;
	}bit_alloc;	
}SBC_BYTE2;

typedef struct stru_audio_frame_header
{
	U8 SyncWord;
	SBC_BYTE2 Byte1;
	U8 Bitpool;
	U8 RSVD[10];
}SBCFrameHeaderStru;

typedef struct
{
	U16 timeStampClk;
	U16 tuningValue;
	U16 tuningTime;
	U16 frameCount;
	U16 remoterameSC;
	U16 remoteFrameCount;
}A2dpAirohaRxMediaPktType;

typedef struct
{
	U16 timeStampClk;
	U16 tuningValue;
	U16 tuningTime;
	U16 frmeCount;
	U16 localFrameSC;
	U16 localFrameCount;
}A2dpAirohaTxMediaPktType;

typedef struct
{
	U8 version:2;
	U8 padding:1;
	U8 extension:1;
	U8 CSRCCount:4;
	U8 marker:1;
	U8 payloadType:7;
	U16 sequenceNumber;
	U32 timeStamp;
	U32 SSRC;
}StandardRTPHeaderType;
//https://www.ietf.org/rfc/rfc3016.txt

#define GET_NUMBER_OF_FRAMES(A)	((A)& 0x0F)

typedef struct
{
	U8 numOfFrames:4;
	U8 reserved:1;
	U8 starting:1;
	U8 fragmented:1;
}SBCPayloadInfoType;

typedef struct
{
	SBCFrameHeaderStru frameHeader;
	U8 framedata[1];
}SBCFrameType;

typedef struct
{
	SBCPayloadInfoType payloadInfo;
	SBCFrameHeaderStru frameHeader;
	U8 framedata[1];
}SBCPayloadType;

typedef struct
{
	U8 payload;
}AACPayloadType;


typedef struct
{
    U8 contentProtHeader;
    SBCPayloadType payload;
}CPHeader;

typedef union
{
    CPHeader cpHeader;
	SBCPayloadType payload;
	AACPayloadType aacPayload;
}PayloadHeaderType;

typedef union
{
	A2dpAirohaRxMediaPktType airohaRxShareHeader;
	StandardRTPHeaderType rtpHeader;
}A2DPSBCRxRTPHeaderType;

typedef struct
{
	L2CAP_DATA_HDR_STRU	L2capHeader;
	A2DPSBCRxRTPHeaderType a2dpRTPHeader;
	PayloadHeaderType PLHeader;
}A2dpSBCRxMediaPktType;

typedef union
{
	A2dpAirohaTxMediaPktType airohaTxShareHeader;
	StandardRTPHeaderType rtpHeader;
}A2DPSBCTxRTPHeaderType;

typedef struct
{
	L2CAP_DATA_HDR_STRU	L2capHeader;
	A2DPSBCTxRTPHeaderType a2dpRTPHeader;
	PayloadHeaderType PLHeader;
}A2dpSBCTxMediaPktType;


typedef struct
{
	L2CAP_DATA_HDR_STRU	L2capHeader;
	StandardRTPHeaderType a2dpRTPHeader;
	PayloadHeaderType PLHeader;
}A2dpAACRxMediaPktType;

typedef struct
{
	void (* A2DPRelayHandler)(U8 action);
}A2DP_RELAY_HANDLER_TYPE;

enum
{
	HW_AUDIO_CHANNEL_NOT_DECIDED,
	HW_AUDIO_CHANNEL_STEREO,
	HW_AUDIO_CHANNEL_LEFT,
	HW_AUDIO_CHANNEL_RIGHT,
};

#define RELAY_SOURCE_MUSIC		0x01
#define RELAY_SOURCE_AUX		0x02
#define RELAY_SOURCE_DEMOSOUND	0x03

typedef struct
{
	U8	txTrafficState;
	U8	relayCnt;
	U8	hwAudioChannel;
	OST XDATA_PTR pktDroppedTimer;
	OST XDATA_PTR badChannelSwithToGoodTimer;
	U8 	airFollowerDropPktCount;
	U8 	audioSource;
	U8 	audioSourceLinkIndex;
	OST XDATA_PTR delaySendSuspendTimer;
}A2DP_RELAY_CTL;

typedef struct
{
	U8 role;
	U8 packetRole;
	U8 audioChannel;
}A2DP_AIR_CTL;

typedef BOOL (*A2DP_MEDIA_RX_FUNC)(U8 linkIndex, U8 XDATA_PTR ptr1);

#define DELAY_REPORT_MAX_DEVIATION			300 //unit: 0.1 ms
#define DELAY_REPORT_RECOMMEND_DEVIATION	150 //unit: 0.1 ms
typedef struct
{
	U8 isDspPlay:1;
	U8 reserved:7;
	U8 pktCnt;
	U16 reportedValue;
}DELAY_REPROT_CTL;

typedef struct
{
	MMI_PROFILE_INFO mmiProfileInfo;
	
	U8 substates;
	U8 XDATA_PTR remoteReqPtr;
	U8 XDATA_PTR discovSeidInfoPtr;
	U8 isPassive:1;
	U8 isResume:1;
	U8 mediaChannelConnected:1;
	U8 enableDelayReport:1;
	U8 enableSCMS:1;
	U8 isMediaDataPlay:1;
	U8 reserved:2;
	U8 localSeid;
	U8 remoteSeid;

	union {
		#if SUPPORT_SBC_SNK || SUPPORT_SBC_SRC
		SBCCodecSettings sbc;
		#endif
		#if SUPPORT_AAC_SNK
		AACCodecSettings aac;
		#endif
		U8 vendor[12];
	} codecSettings;
	OST XDATA_PTR timer;
	OST XDATA_PTR suspendDelayTimer;
	OST XDATA_PTR waitForMediaChannelOpenTimer;
	OSMQ OSMQ_AVDTP_Signal_To_A2DP;
	A2DP_AIR_CTL airCtl;
	A2DP_MEDIA_RX_FUNC	fA2DPRxMediaCodec;
	DELAY_REPROT_CTL delayReportCtl;
}A2DP_LINK_INFO;

typedef	struct
{
	A2DP_LINK_INFO linkInfo[MAX_MULTI_POINT_NO];
	A2DP_RELAY_CTL relayCtl;
	U8 gDebugPringCnt;
	U8 gNoRxMemCnt;
}A2DP_CTL;

A2DP_EXTERN A2DP_CTL XDATA gA2DP_ctl;


PUBLIC A2DP_LINK_INFO XDATA_PTR A2DP_GetLinkInfo(U8 linkIndex);

PUBLIC void A2DP_Signal_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
PUBLIC void A2DP_Signal_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr);
PUBLIC void A2DP_SignalHandler(U8 linkIndex);

PUBLIC BOOL A2DP_UndefinedDecoderRxMediaPacket(U8 linkIndex, U8 XDATA_PTR ptr1);
PUBLIC BOOL A2DP_SBCDecoderRxMediaPacket(U8 linkIndex, U8 XDATA_PTR ptr1);
PUBLIC BOOL A2DP_AACDecoderRxMediaPacket(U8 linkIndex, U8 XDATA_PTR ptr1);

PUBLIC void A2DP_Media_LinkEvent(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
PUBLIC void A2DP_Media_DataEvent(U8 linkIndex, U8 XDATA_PTR dataPtr);

PUBLIC void A2DP_SetRelayStateReady(BOOL isReady);
PUBLIC void A2DP_SetMediaDataPlayByLink(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo, BOOL isPlay);

PUBLIC void A2DP_AirModeSetDSPSel(void);

PUBLIC BOOL A2DP_AirSideReadyToReceiveMediaStream (void);
PUBLIC void A2DP_AirModeSetDSPSelByRelayStatus(U8 relayStatus);

PUBLIC void A2DP_CloseHandler(U8 linkIndex);
PUBLIC void A2DP_CommandComplete(A2DP_LINK_INFO XDATA_PTR a2dpLinkInfo);
PUBLIC void A2DP_SendAcceptStartResponse(U8 linkIndex);

PUBLIC void A2DP_DelayReportClearPara(U8 linkIndex);
PUBLIC void A2DP_SendInitDelayReport(U8 linkIndex);
PUBLIC void A2DP_SendDelayReport(U8 linkIndex, U16 newDelay, BOOL isBigger);
PUBLIC void A2DP_CalculateDelayReportValue(U8 linkIndex);
PUBLIC U8 A2DP_DspBufferDepth(U8 linkIndex);

#endif
