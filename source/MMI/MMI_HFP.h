#ifndef _MMI_HFP_H_
	#define _MMI_HFP_H_
	
#ifdef HFP_Profile
	#include "..\HFP\HFP_Interface.h"

	#ifdef _MMI_HFP_C_
		#define MMI_HFP_EXTERN
	#else
		#define MMI_HFP_EXTERN extern
	#endif

	PUBLIC void HFP_CommandHandle(U8 linkIndex, U8 cmdCode, MMIMsgType XDATA_PTR cmdPtr);
	
	//Following defines the optional functions of AG
	#define MMI_AG_OPTION_THREEWAY_CALLING 		0X0001
	#define MMI_AG_OPTION_EC_NR 				0X0002
	#define MMI_AG_OPTION_VOICE_RECOGNITION 	0X0004
	#define MMI_AG_OPTION_INBAND_RINGTONE 		0X0008 //in-band ringtone capability
	#define MMI_AG_OPTION_VOICE_TAG				0X0010 //attach a no to a voice tag
	#define MMI_AG_OPTION_REJECT_CALL			0X0020 //ability to reject a call
	#define MMI_AG_OPTION_ENHCALL_STATUS		0X0040 //enhanced call status
	#define MMI_AG_OPTION_ENHCALL_CTRL			0X0080 //enhanced call control
	#define MMI_AG_OPTION_ENHERR_RESCODE		0X0100 //enhanced error result codes
	#define MMI_AG_OPTION_CODEC_NEGOTIATION		0X0200 //codec negotiation 
	#define MMI_AG_OPTION_HF_INDICATORS			0X0400 //HF indicators 
	
	//Battery Report Settings
	#define MMI_HF_IPHONE_BAT_EN	0x01
	#define MMI_HF_XEVENT_BAT_EN	0x02
	#define MMI_HF_BIEV_BAT_EN		0x04
	#define MMI_HF_IPHONE_BAT_TM	0x10
	#define MMI_HF_XEVENT_BAT_TM	0x20
	#define MMI_HF_BIEV_BAT_TM		0x40
	
	#define MMI_HF_BAT_EN_MASK		(MMI_HF_IPHONE_BAT_EN | MMI_HF_XEVENT_BAT_EN | MMI_HF_BIEV_BAT_EN)
	#define MMI_HF_BAT_TM_MASK		(MMI_HF_IPHONE_BAT_TM | MMI_HF_XEVENT_BAT_TM | MMI_HF_BIEV_BAT_TM)
	
	enum
	{
		CALLHELD_NO_CALLS_HELD = 0,
		CALLHELD_CALLS_HELD_ACTIVE, //both active & held call
		CALLHELD_ON_HOLD_NOACTIVE, //only held call
	};
	
	enum
	{
		CALL_NO_ACTIVE = 0,
		CALL_ACTIVE,
	};

	enum
	{
		CALLSETUP_NO_CALSETUP = 0,
		CALLSETUP_INCOMMING,
		CALLSETUP_OUTGOING,
		CALLSETUP_REMOTE_ALERTED,
	};

	enum
	{
		MMI_CALL_EMPTY,
		MMI_CALL_ACTIVE, //ON CALL
		MMI_CALL_INCOMMING,
		MMI_CALL_OUTGOING,
		MMI_CALL_HELD,
		MMI_CALL_ALL_ONGOING,
	};

	enum
	{
		MMI_CALL_NO_3WAY,
		MMI_CALL_IN_3WAY,
	};
	
	typedef struct
	{
		U8 service:1;
		U8 call:1;
		U8 callsetup:3;
		U8 callheld:3;
		U8 signal:3;	//signaling channel
		U8 roam:1;	//media channel
		U8 chgbat:3;
		U8 reserved:1;
	}HFPInitIndicatorType;

	typedef struct
	{
		U8 callStatus;
	}MmiCallCtrlTblIType;
	#define MMI_MAX_ONGOING_CALL_NO 2
	

	
	typedef struct
	{
		U16 agOption;
		HFPInitIndicatorType indicator;
		U8 hfpIndicatorMappingTbl[HFP_CIEVIND_NO];
		U8 ongoingCallNo;
		MmiCallCtrlTblIType callCtrlTbl[MMI_MAX_ONGOING_CALL_NO];		
		U8 missedCallCnt;
		U8 missedCallRemindAttempt;
		U8 appleSiriStatus;
		PhoneNumber incomingCall;
		
		U8 audioTransfer:1;
		U8 sendingVGSVGM:1; // flag for avoiding repeated sending AT+VGS
		U8 relSCOCozExitOggState:1;
		U8 isCallerNameGet:1;
		U8 isCallerIdGet:1;
		U8 isHFPImgEntry:1;
		U8 isImgVPTriggered:1;
		U8 is3WayAdd:1;
		
		U8 batReportState;
		
		U8 reconnectReason;
		#ifdef _MEMORY_DIALING_
		U16 memoryDialIndex; //[0-65535]
		#endif
		#ifdef _EXTENSION_ENTITIES_	
		U8 DTMFPara;
		#endif
		OST XDATA_PTR sendBatteryAtCmdTimer;
		OST XDATA_PTR networkServiceIndicatorRepeatTimer;
		OST XDATA_PTR autoAnswerTimer;
		OST XDATA_PTR waitToEstSCOInOggCallTimer;
		OST XDATA_PTR audioTransferDetachTimer;
		OST XDATA_PTR audioDevATCmdTimer;
		OST XDATA_PTR waitCaImgRmvCallTransitTimer; //Mantis 8867
		OST XDATA_PTR rejectCallMuteSCOTimer;
	}MmiHfpCtlType;
	
	typedef struct
	{
		MmiHfpCtlType HfpCtl[MAX_MULTI_POINT_NO];
		U8 incomingLinkInd;
		U8 rdialLinkInd;
		OST XDATA_PTR missedCallRemindTimer;
		OST XDATA_PTR callerRepeatTimer;
		U8 imgAudioVoiceIndex;
		U8 imgAudioVoiceState;
	}GMmiHfpCtlType;
	
	MMI_HFP_EXTERN GMmiHfpCtlType XDATA gMMI_HFP_ctl;

	PUBLIC BOOL MMI_SendATCmdUnderConnection(U8 linkIndex, U8 opcode);
	
	PUBLIC void MMI_HFP_ProcessKeyEvent(U8 linkIndex, U8 index);
	PUBLIC U16 MMI_HFP_CheckValidKey(U8 index, U16 keyCode);
	PUBLIC BOOL MMI_HFP_SetMemoryDialIndex(U8 linkIndex, U16 index);
	PUBLIC BOOL MMI_HFP_SetDialNumber(U8 linkIndex, U8 XDATA_PTR numberPtr, U8 length);
	PUBLIC BOOL MMI_HFP_SetDTMF(U8 linkIndex, U8 index);
	PUBLIC void MMI_HFP_SCOConnectedHandle(U8 linkIndex);
	PUBLIC void MMI_HFP_SCOConnectFailHandle(U8 linkIndex);
	PUBLIC void MMI_HFP_SCODiscHandle(U8 linkIndex);
	PUBLIC void MMI_HFP_VoiceControl(U8 linkIndex, BOOL isVolumeUp, BOOL isFollowerCmd);
	PUBLIC BOOL MMI_HFP_IsHSPConnected(U8 linkIndex);
	PUBLIC BOOL MMI_HFP_IsHFPConnected(U8 linkIndex);
	PUBLIC BOOL MMI_HFP_IsConnected(U8 linkIndex);
	PUBLIC void MMI_HFP_SetReconnectReason(U8 linkIndex, U8 reason);
	PUBLIC U8 MMI_HFP_SearchCallStatusInd(U8 linkInd, U8 callStatus);
	
	PUBLIC BOOL MMI_HFP_SendATCmdByLink(U8 linkIndex, U8 opcode);
	PUBLIC U8 MMI_HFP_GetNextHFPLink(U8 linkIndex);

	PUBLIC U16 MMI_HFP_GET_AGOPTION(U8 linkIndex);
	PUBLIC void MMI_HFP_SET_AGOPTION(U8 linkIndex, U16 agOption);
	PUBLIC void MMI_HFP_CLEAR_AGOPTION(U8 linkIndex, U16 agOption);
	PUBLIC U8 MMI_HFP_GET_VGS(U8 linkIndex);
	
	PUBLIC void MMI_HFP_StopIncomingVPRing(U8 linkIndex);

//////////////////////////////////////////////////////////////////////////
///
/// Profile-Dependended Functions
///
//////////////////////////////////////////////////////////////////////////
#ifdef HFP_AG_Profile
  #define MMI_HFP_Init												MMI_HFP_Init_AG
  #define MMI_HFP_CheckTimer										MMI_HFP_CheckTimer_AG
  #define MMI_HFP_ConnectedHandle									MMI_HFP_ConnectedHandle_AG
  #define MMI_HSP_ConnectedHandle									MMI_HSP_ConnectedHandle_AG
  #define MMI_HFP_ProfileDiscHandle									MMI_HFP_ProfileDiscHandle_AG
  #define MMI_HFP_EventHandler										MMI_HFP_EventHandler_AG
  #define MMI_HFP_FakeMediaEvt										MMI_HFP_FakeMediaEvt_AG
  #define MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls		MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls_AG
  #define MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld			MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld_AG
#else
  #define MMI_HFP_Init												MMI_HFP_Init_HF
  #define MMI_HFP_CheckTimer										MMI_HFP_CheckTimer_HF
  #define MMI_HFP_ConnectedHandle									MMI_HFP_ConnectedHandle_HF
  #define MMI_HSP_ConnectedHandle									MMI_HSP_ConnectedHandle_HF
  #define MMI_HFP_ProfileDiscHandle									MMI_HFP_ProfileDiscHandle_HF
  #define MMI_HFP_EventHandler										MMI_HFP_EventHandler_HF
  #define MMI_HFP_FakeMediaEvt										MMI_HFP_FakeMediaEvt_HF
  #define MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls		MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls_HF
  #define MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld			MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld_HF
#endif
	
	PUBLIC void MMI_HFP_Init(void);
	PUBLIC void MMI_HFP_CheckTimer(U8 linkIndex);
	PUBLIC void MMI_HFP_ConnectedHandle(U8 linkIndex);
	PUBLIC void MMI_HSP_ConnectedHandle(U8 linkIndex);
	PUBLIC void MMI_HFP_ProfileDiscHandle(U8 linkIndex);
	PUBLIC void MMI_HFP_EventHandler(U8 linkIndex, U8 evtCode, HfpToMMIEvtParamType XDATA_PTR hfpParamPtr);
	
	PUBLIC void MMI_HFP_AirAppNotifyCallerNameReady(U8 linkIndex);
	PUBLIC void MMI_HFP_FakeMediaEvt(U16 id);
	
	PUBLIC void MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls(U8 linkIndex);
	PUBLIC void MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld(U8 linkIndex);
	
	#define IMG_AUDIO_VOICE_TRIGGER		0x01
	#define IMG_AUDIO_VOICE_FAKE_EVT	0x02
	#define IMG_AUDIO_VOICE_TIMER_TO	0x03
	PUBLIC void MMI_HFP_ImgAudioVoiceHandler(U8 linkIndex, U8 action);

#endif //HFP_Profile

#endif //_MMI_HFP_H_