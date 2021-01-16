//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "os.h"
#include "bt_config_profile.h"
#include "keypad.h"
#ifdef RINGTONE_ENABLE
#include "ringtone.h"
#endif
#include "voice_prompt_ctl.h"
#include "voice_command_ctl.h"
#include "Audio_MailBox_structure.h"
#include "sector_peq_hpf_param.h"

#define VOLUME_MIC_FOLLOW_SPK	0xEE
#define VOLUME_DONT_CARE 0xFF

enum
{
	KEY_TAP,
	KEY_PRESS,
	KEY_PRESS_RELEASE,
	KEY_LONGPRESS,
	KEY_LONGPRESS_RELEASE,
	KEY_DOUBLE,
	KEY_TRIPLE,
	KEY_LONGLONGPRESS,
	KEY_LONGLONGPRESS_RELEASE,
	KEY_EXTREMELY_LONGPRESS,
	KEY_EXTREMELY_LONGPRESS_RELEASE,
	KEY_REPEAT,
	KEY_DOWN,
	KEY_UP,
	KEY_NOACT,
	KEY_ACTIONTOTALNUMBER, 
};

typedef struct
{
	U8 keyMultiTime[TOTAL_KEY_NO];//in 0.1 s
	U8 keyPressTime[TOTAL_KEY_NO];//in 0.1 s
	U8 keyLongPressTime[TOTAL_KEY_NO]; // in 0.1 s
	U8 keyLongLongPressTime[TOTAL_KEY_NO]; // in 0.1 s
	U8 keyExtremelyLongPressTime[TOTAL_KEY_NO]; // in 0.1 s
	U8 keyContPressTime[TOTAL_KEY_NO]; // in 0.1 s
}DriverKeyTimeType;

enum
{
	DRIVER_MMI_READY_CMD,
	DRIVER_UPDATE_GENERAL_PARA_CMD,
	DRIVER_VOLUME_SET_CMD, 
	DRIVER_A2DP_VOLUME_SET_CMD,
	DRIVER_LINE_IN_VOLUME_SET_CMD,

	DRIVER_RINGTONE_CMD,  
	DRIVER_VOICEPROMPT_CMD,
	DRIVER_STOP_SPECIFIC_RINGTONE_CMD,  
	DRIVER_STOP_SPECIFIC_VOICEPROMPT_CMD,	
	DRIVER_STOP_RINGTONE_CMD,
	DRIVER_STOP_VP_CMD,
	DRIVER_FAKE_MEDIA_VP_RINGTONE_CMD,
	DRIVER_SCO_CMD,
	DRIVER_A2DP_CMD,	
	DRIVER_LINE_IN_CMD,
	DRIVER_STOP_LINE_IN_CMD,
	DRIVER_STOP_DSP_CMD,
	DRIVER_STOP_DSP_POWER_OFF_CMD,

#ifdef PEQ_ENABLE
	DRIVER_CHANGE_PEQ_CMD,
	DRIVER_PEQ_RELOAD_CMD,
	DRIVER_REALTIME_SET_PEQ_HPF_CMD,
	DRIVER_SET_PEQ_PARA_CMD,
#endif

	DRIVER_POWER_ON_CLOSE_ADDA_CMD,
	DRIVER_BATTERY_STATUS_CMD,
	DRIVER_VOICE_COMMAND_CMD,
	DRIVER_SET_VOICE_COMMAND_VOL_CMD,
	DRIVER_STOP_VOICE_COMMAND_CMD,
	DRIVER_SET_AUDIO_SEL_CMD,
	DRIVER_SUSPEND_DSP_CMD,
	DRIVER_CHANGE_SOUND_EFFECT_CMD,
	DRIVER_CONTROL_DBB_ON_OFF,
	DRIVER_GET_DSP_REF_GAIN_CMD,
	#ifdef DEMOSOUND
	DRIVER_DEMOSOUND_PLAY_CMD,
	DRIVER_DEMOSOUND_PAUSE_CMD,
	DRIVER_DEMOSOUND_STOP_CMD,
	DRIVER_DEMOSOUND_FORWARD_CMD,
	DRIVER_DEMOSOUND_BACKWARD_CMD,
	#endif
	#ifdef SUPPORT_MIC_IN_A2DP_MODE
	DRIVER_A2DP_ADD_MIC_CMD,
	#endif
	DRIVER_MIC_TEST_CMD,
	DRIVER_DSP_SWITCH_STEREO_MONO_CMD,
	DRIVER_SCO_MODE_CONTROL_CMD,
	DRIVER_VPRT_VOLUME_SET_CMD,
	DRIVER_AUDIO_TRANSPARENCY_CMD,
	DRIVER_ASYNC_SCO_CMD,
	MMI_TO_DRIVER_TOTAL_CMD,
	DRIVER_FM_POWERON_CMD,
	DRIVER_FM_POWEROFF_CMD,
	DRIVER_FM_INTERNAL_POWEROFF_CMD,
	DRIVER_FM_HWSEEKDN_CMD,
	DRIVER_FM_HWSEEKUP_CMD,
	DRIVER_FM_TUNEUP_CMD,
	DRIVER_FM_TUNEDN_CMD,
	DRIVER_FM_MUTETOGGLE_CMD,
	DRIVER_FM_VOLUME_CMD,
	DRIVER_FM_MUTE_ON_CMD,
	DRIVER_FM_MUTE_OFF_CMD,
	
	DRIVER_FG_LED_CMD,

	
	DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT= 0X60,
#ifdef FM_ENABLE
	DRIVER_FM_BUSY_EVENT,
	DRIVER_FM_NOT_BUSY_EVENT,
	DRIVER_FM_MUTE_ON_EVENT,
	DRIVER_FM_MUTE_OFF_EVENT,
#endif
	DRIVER_INTERNAL_CLEAN_ALL_AUDIO_CMD = 0xFF,
};

////////////////////////////////////////////////////////////////////
/////////////////////DRIVER COMMAND ////////////////////////////////
////////////////////////////////////////////////////////////////////
/*
 0x07 DRIVER_VOLUME_SET_CMD
 0x08 DRIVER_A2DP_VOLUME_SET_CMD
 0x09 DRIVER_LINE_IN_VOLUME_SET_CMD
*/
typedef struct
{
	U8 soundLevel;
	U8 spkVols; // 1. vol level of each type of vol ctrl, i.e., voice, tone or mic. 2. levels of voice, tone & mic sequentially for vol_set cmd
	U8 micVols;
	U8 analogDaGain;
	U8 analogAdGain_R;
	U8 analogAdGain_L;
	U8 smallStepLevel;
}DriverVolumeSetCmd;

/*
 0x0A DRIVER_RINGTONE_CMD
*/
typedef struct
{
	U8 beepIndex;
	U16 id;
	U8 suspendApp;
}DriverRingtoneCmd;

/*
 0x0B DRIVER_VOICEPROMPT_CMD
*/
enum
{
	PLAY_NONE,
	PLAY_RINGTONE,
	PLAY_VOICEPROMPT,
};

#ifdef PEQ_ENABLE
/*
  DRIVER_REALTIME_SET_PEQ_HPF_CMD
*/
typedef union
{
	HPF_CONFIG_SECTOR	hpf;													// hpf parameter	
	PEQ_CONFIG_SECTOR	peq;													// peq parameter
	#ifdef PEQ_EXPANSION_ENABLE
	PEQ_CONFIG_SECTOR	peq_expansion;											// peq_expansion parameter
	#endif
	#ifdef PEQ_UNDER_100HZ_ENABLE
	PEQ_100HZ_SECTOR peq_under100Hz;		// peq under 100Hz parameter
	#endif
}DriverPEQRealTimeParaType;

typedef struct
{
	U8 reserveByte[2];					// U8 VCmdOcf + U8 VCmdOgf
	U8 enable_mode; 					// 1<<0: enable peq, 1<<1: enable hpf, 1<<2: enable peq_expansion, 1<<3: enable peq_under100hz
	DriverPEQRealTimeParaType	para;	// peq/hpf/peq_expansion/peq_under100Hz parameter
}DriverRealTimePEQCmd;

typedef struct
{
	U8 reserveByte[2];					// U8 VCmdOcf + U8 VCmdOgf
	U8 peqMode;					
	U8 peqModeIndex;
	U8 sectorMode;
}DriverSetPEQParaCmd;
#endif

#define CALLER_ID_DIGIT_NO	16
typedef struct
{
	U8 callerIdLength;
	U8 callerID[CALLER_ID_DIGIT_NO];
}DriverCallerIDType;

typedef struct
{
	U8 digitLength;
	U8 number[6];
}DriverFMChannelNumberType;

typedef union
{
	DriverCallerIDType callerIDData;
	DriverFMChannelNumberType fmChannel;
}DriverVPDataType;

typedef struct
{
	U16 id;
	U8 vpIndex;
	U8 suspendApp;
	DriverVPDataType vpData;
	U8 isOverSCO;
}DriverVoicePromptCmd;

/*
 0x0C DRIVER_STOP_SPECIFIC_RINGTONE_CMD
*/
typedef struct
{
	U16 id;
}DriverStopSpecificRTCmd;

/*
 0x0D DRIVER_STOP_SPECIFIC_VOICEPROMPT_CMD
*/
typedef struct
{
	U16 id;
}DriverStopSpecificVPCmd;

/*
 0x10 DRIVER_FAKE_MEDIA_VP_RINGTONE_CMD
*/
typedef struct
{
	U16 id;
}DriverFakeMediaCmd;


/*
 0x11 DRIVER_SCO_CMD
 0x12 DRIVER_A2DP_CMD
 0x13 DRIVER_LINE_IN_CMD
 0x15 DRIVER_STOP_DSP_CMD
*/
typedef struct
{
	U8 isECNROn;
	U8 isLightMonoMode;
}DriverSCOEnableParaType;

typedef struct
{
	U8 codec;
	U8 sbc_target_blocks_offset;
	U8 twsMode;
	U8 sidetone_en;
}DriverA2dpEnableParaType;

typedef union
{
	DriverSCOEnableParaType scoPara;
	DriverA2dpEnableParaType a2dpPara;
}DriverAudioEnableParaType;

typedef struct
{
	M2D_CTL_STRU m2d_ctl;	
	CMD_RESPONSE_STRU cmd_resp;
	U8 linkInd;
	DriverAudioEnableParaType enablePara;
}DriverMailBoxCmdType;

/*
 0x1A DRIVER_BATTERY_STATUS_CMD
*/
typedef struct
{
	U8 isLowBat;
	U8 isCharging;
}DriverBatStatusCmd;

/*
 0x1B DRIVER_VOICE_COMMAND_CMD
*/
typedef struct
{
	U16 id;
	U8 actionIndex;
}DriverVoiceCommandCmd;

/*
 0x1C DRIVER_SET_VOICE_COMMAND_VOL_CMD
*/
typedef struct
{
	U8 micVols;
	U8 analogAdGain_R;
	U8 analogAdGain_L;
}DriverVoiceCommandVolCmd;

/*
 0x1E DRIVER_SET_AUDIO_SEL_CMD
*/
typedef struct
{
	U8 audioInSel;
	U8 audioOutSel;
	U8 I2SInPIO;
	U8 audioI2SMasterGroupSel;
	U8 audioI2SSlaveGroupSel;
	U8 SPDIFPIO;
	U8 MCLKInternal;
	U8 MCLKDivideNum;
	U8 MCLKPhaseInvert;
	U8 isResetToDefault;
	U8 I2SMCLKPioDisable;
}DriverAudioSelCmd;

/*
 0x1F DRIVER_SUSPEND_DSP_CMD
*/
#define DRV_SUSPEND_DSP_REASON_TOOL		0x01
#define DRV_SUSPEND_DSP_REASON_AIRAPP	0x02
#define DRV_SUSPEND_DSP_REASON_FOTA		0x04  

typedef struct
{
	U8 reason;
	VFUN callback;
}DriverSuspendDSPCmd;

/*
 0x20 DRIVER_FM_POWERON_CMD
 0x21 DRIVER_FM_POWEROFF_CMD
 0x22 DRIVER_FM_INTERNAL_POWEROFF_CMD
 0x23 DRIVER_FM_HWSEEKDN_CMD
 0x24 DRIVER_FM_HWSEEKUP_CMD
 0x25 DRIVER_FM_TUNEUP_CMD
 0x26 DRIVER_FM_TUNEDN_CMD
 0x27 DRIVER_FM_MUTETOGGLE_CMD
 0x28 DRIVER_FM_VOLUME_CMD
 0x29 DRIVER_FM_MUTE_ON_CMD
 0x2A DRIVER_FM_MUTE_OFF_CMD
*/
typedef  struct
{
	S16 setFreq;
	U8 historyVol;
	U8 isMute;
}FMPowerOnPara;
typedef union
{
	FMPowerOnPara fmPoweronPara;
	U8  FMVolume;
}DriverFMCmd;

/*
 0x01 DRIVER_FG_LED_CMD
*/
typedef struct
{
	U8 ledDisplayIndex;
	U8 timeout;
	U8 eventCode;
	U8 isOverrideFilter;
}DriverLedCmd;

typedef struct
{
	U8 micEnableSet;
	U8 controlECNR;
}DriverMicTestCmd;

//DRIVER_DSP_SWITCH_STEREP_MONO_CMD
typedef struct
{
	U8 soundType;
}DriverSwitchStereoMonoCmd;

//DRIVER_AUDIO_TRANSPARENCY_CMD
typedef struct
{
	U8 isEnable;
}DriverAudioTransparencyCmd;

typedef union
{
														//DRIVER_MMI_READY_CMD,
														//DRIVER_UPDATE_GENERAL_PARA_CMD,
	DriverVolumeSetCmd			volSetCmd;				//DRIVER_VOLUME_SET_CMD, 
	DriverVolumeSetCmd			a2dpVolSetCmd;			//DRIVER_A2DP_VOLUME_SET_CMD,
	DriverVolumeSetCmd			lineInVolSetCmd;		//DRIVER_LINE_IN_VOLUME_SET_CMD,

	DriverRingtoneCmd			ringtoneCmd;			//DRIVER_RINGTONE_CMD,  
	DriverVoicePromptCmd		voicePromptCmd;			//DRIVER_VOICEPROMPT_CMD,
	DriverStopSpecificRTCmd		stopSpecificRTCmd;		//DRIVER_STOP_SPECIFIC_RINGTONE_CMD,  
	DriverStopSpecificVPCmd		stopSpecificVPCmd;		//DRIVER_STOP_SPECIFIC_VOICEPROMPT_CMD,	
														//DRIVER_STOP_RINGTONE_CMD,
														//DRIVER_STOP_VP_CMD,
	DriverFakeMediaCmd			fakeMediaCmd;			//DRIVER_FAKE_MEDIA_VP_RINGTONE_CMD,
	DriverMailBoxCmdType		scoCmd;					//DRIVER_SCO_CMD,
	DriverMailBoxCmdType		a2dpCmd;				//DRIVER_A2DP_CMD,	
	DriverMailBoxCmdType		auxCmd;					//DRIVER_LINE_IN_CMD,
														//DRIVER_STOP_LINE_IN_CMD,
	DriverMailBoxCmdType		stopDSPCmd;				//DRIVER_STOP_DSP_CMD,
														//DRIVER_STOP_DSP_POWER_OFF_CMD,
#ifdef PEQ_ENABLE
														//DRIVER_CHANGE_PEQ_CMD,
														//DRIVER_PEQ_RELOAD_CMD,
	DriverRealTimePEQCmd		peqRealTimeSetCmd;		//DRIVER_REALTIME_SET_PEQ_HPF_CMD,
	DriverSetPEQParaCmd			peqPEQParaSetCmd;		//DRIVER_SET_PEQ_PARA_CMD,
#endif
														//DRIVER_POWER_ON_CLOSE_ADDA_CMD,
	DriverBatStatusCmd			batStatusCmd;			//DRIVER_BATTERY_STATUS_CMD,
	DriverVoiceCommandCmd		vcCmd;					//DRIVER_VOICE_COMMAND_CMD,
	DriverVoiceCommandVolCmd	vcVolCmd;				//DRIVER_SET_VOICE_COMMAND_VOL_CMD,
														//DRIVER_STOP_VOICE_COMMAND_CMD,
	DriverAudioSelCmd			audioSelectCmd;			//DRIVER_SET_AUDIO_SEL_CMD,
	DriverSuspendDSPCmd			suspendDspCmd;			//DRIVER_SUSPEND_DSP_CMD,
	DriverVolumeSetCmd			vprtVolSetCmd;			//DRIVER_VPRT_VOLUME_SET_CMD,
														//MMI_TO_DRIVER_TOTAL_CMD,

	DriverFMCmd 				FMCmd; 					//DRIVER_FM_POWERON_CMD,
														//DRIVER_FM_POWEROFF_CMD,
														//DRIVER_FM_INTERNAL_POWEROFF_CMD,
														//DRIVER_FM_HWSEEKDN_CMD,
														//DRIVER_FM_HWSEEKUP_CMD,
														//DRIVER_FM_TUNEUP_CMD,
														//DRIVER_FM_TUNEDN_CMD,
														//DRIVER_FM_MUTETOGGLE_CMD,
														//DRIVER_FM_VOLUME_CMD,
														//DRIVER_FM_MUTE_ON_CMD,
														//DRIVER_FM_MUTE_OFF_CMD,
	DriverLedCmd				fgLEDCmd;				//DRIVER_FG_LED_CMD,
	DriverMicTestCmd			micTestCmd;				//DRIVER_MIC_TEST_CMD,
	DriverAudioTransparencyCmd	audioTransparencyCmd;	//DRIVER_AUDIO_TRANSPARENCY_CMD

	DriverSwitchStereoMonoCmd	switchStereoMonoCmd;	//DRIVER_DSP_SWITCH_STEREO_MONO_CMD,

	DriverMailBoxCmdType 		mailBoxCmd;				//mailBoxCmd

}DriverCmdType; //union size should not be over (size of memory 1 - 5)

////////////////////////////////////////////////////////////////////
/////////////////////DRIVER EVENT //////////////////////////////////
////////////////////////////////////////////////////////////////////
/*
 0x6E DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT
*/
typedef struct
{
	U16 id;
}DriverFakeMediaEvent;

/*
 0xXX MailBoxEvt
*/
typedef struct
{
	U8 ogf;
	U8 ocf;
}DriverMailBoxEvent;

typedef union
{
	DriverFakeMediaEvent	fakeMediaEvt;		//DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT = 0X60,
	#ifdef FM_ENABLE
												//DRIVER_FM_BUSY_EVENT,
												//DRIVER_FM_NOT_BUSY_EVENT,
												//DRIVER_FM_MUTE_ON_EVENT,
												//DRIVER_FM_MUTE_OFF_EVENT,
	#endif
	
	DriverMailBoxEvent		mailBoxEvt;			//MailBoxEvt
}DriverEvtType;	

typedef union
{
	DriverCmdType driverCmd;
	DriverEvtType driverEvt;
}DriverMsgBody;

#define RESERVE_BYTE_NO 1
#define CMD_OFFSET (RESERVE_BYTE_NO+1)
#define EVT_OFFSET (RESERVE_BYTE_NO+2)

typedef struct DriverMsg_st
{
	U8 dataOffset; //to the start of meaningful message
	U8 reserveByte[RESERVE_BYTE_NO];
	U8 msgOpcode;
	U8 eventCode;
	U8 paraLen;
	DriverMsgBody msgBodyPtr;
}DriverMsg;

enum
{	
	MMI_READY = 0x01,			//MMI turn off LED and detach Ready
	DRIVER_READY = 0x02, 		//PWR_KEY Release Ready
	READY_TO_POWEROFF = (MMI_READY | DRIVER_READY)	//0x03
};

enum
{
	BAT_WELL,
	LOW_BAT_LEVEL_1,
	LOW_BAT_LEVEL_2,
	BAT_CHARGING
};

enum
{
	DRIVER_IS_RESUME_FM						= 0x01,
	DRIVER_VP_RING_LOCKED_FOR_FM			= 0x04,
	DRIVER_IS_LOW_BATTERY					= 0x08,
	DRIVER_IS_CHARGER_IN					= 0x20,
};

enum
{
	DRIVER_MUTE_NONE,
	DRIVER_MUTE_LINE_IN,
	DRIVER_MUTE_A2DP,
	DRIVER_MUTE_SCO
};

enum
{
	DRIVER_MIC_DEFAULT,
	DRIVER_MIC_ENABLE_L,
	DRIVER_MIC_ENABLE_R,
	DRIVER_MIC_ENABLE_ALL,
};

enum
{
	DRIVER_CONTROL_ECNR_DEFAULT,
	DRIVER_CONTROL_ECNR_ENABLE,
	DRIVER_CONTROL_ECNR_DISABLE,
};

enum
{
	DRIVER_AUDIO_TRANSPARENCY_STOP,
	DRIVER_AUDIO_TRANSPARENCY_START,
};

#define DRIVER_BUSY_POWER_KEY	0x01
#define DRIVER_BUSY_DSP_ON		0x02
PUBLIC void DRIVER_SetBusyMask(BOOL isBusy, U8 mask);


#define DRIVER_EXT_FLASH_OK		0x01
typedef struct
{
	U8 status;
	U32	vpDataSector;
}ExtFlashCtl;

typedef struct
{
	U8 busyMask;

	OST XDATA_PTR muteReminderTimerPtr;
	OST XDATA_PTR resumeFMTimerPtr;
	OST XDATA_PTR spkMuteAmpTimerPtr;
	OST XDATA_PTR redialMuteTimerPtr;
	OST XDATA_PTR waitQosTimerPtr;

	////////////////////Vol////////////////
	DriverVolumeSetCmd lineInSpkMicCtl;
	DriverVolumeSetCmd normalSpkMicCtl;
	DriverVolumeSetCmd a2dpSpkCtl;
	DriverVoiceCommandVolCmd voiceCmdMiscCtl;
	U8 vprtSoundLevel;
	/////////////////AUDIO /////////////////////////////
	U16 playingRingtoneId;
	#ifdef SUPPORT_VOICE_COMMAND
	U8 playingVoiceCmdId;
	#endif
	U16 DSPGainLv;

	DriverMsg XDATA_PTR backUpAudioCmdPtr;
	U8 isAudioHandlingPending:1;
	U8 sendDSPAppIdle:1;
	U8 resumeAppAfterVPRT:1;
	U8 isResumeSCO:1;
	U8 isAIOFuncDisabled:1;
	U8 isSideToneEnabled:1;
	U8 rejectCallMuteSCOCmd:1;
	U8 isATSwitch:1;
	
	U8 lightMonoModeMask:4;
	U8 ecnrMask:4;
	U8 suspendDSPReason;
	
	OST XDATA_PTR resumeAppTimer;


	/////////////// MISC//////////////////////////////
	U8 miscCtrlMap;
	U8 isReadyToDSPwOff;
	U8 dspLink;
	U8 musicCodecType;
	//////////////  PEQ//////////////
#ifdef PEQ_ENABLE
	OST XDATA_PTR peqMuteTimer;
#endif
	OST XDATA_PTR soundEffectMuteTimer;
	U8 powerSavingMask;
	
	
	///////////// Language ////////////////////////
	VoiceLangCtl voiceLangCtl;
	#if (HW_SPI_SUPPORTED)
	ExtFlashCtl	extFlashCtl;
	#endif
	
#ifdef AIR_MODULE
	OST XDATA_PTR checkDspDataStatusTimer;
#endif
	U16 dspClock;
	
	BOOL OTAVoicepromptsExist;
	
	OST XDATA_PTR checkATSwitchDelayTimer;
}DriverCtrlInfo;

#ifdef _DRIVER_C_
	#define DRIVER_EXTERN 
#else
	#define DRIVER_EXTERN extern
#endif

DRIVER_EXTERN DriverCtrlInfo XDATA gDriver_ctl;

DRIVER_EXTERN OSMQ XDATA OSMQ_DRIVER_Command;
#define OSMQ_DRIVER_Command_ptr				&OSMQ_DRIVER_Command


PUBLIC void DRIVER(void);
PUBLIC void DRIVER_VP_ReadVPData(void);
PUBLIC void DRIVER_ResetLineInKeyTimer(void);
PUBLIC void DRIVER_SendMsgToDriverQue(U8 opCode);

PUBLIC void DRIVER_SendStopVoiceCommandCmd(void);
PUBLIC void DRIVER_SendStopSpecificRTCmd(U16 id);
PUBLIC void DRIVER_SendStopSpecificVPCmd(U16 id);
PUBLIC void DRIVER_ClearStopDSPCmd(U8 linkIndex);
PUBLIC void DRIVER_ClearEnableSCODSPCmd(U8 linkIndex);
PUBLIC void DRIVER_ClearEnableA2DPDSPCmd(U8 linkIndex);

PUBLIC void DRIVER_LoadLanguageCtlFromROM(U8 LangIndex);

PUBLIC void DRIVER_SetA2DPVolPara(DriverVolumeSetCmd XDATA_PTR volPtr);
PUBLIC void DRIVER_SetLineInVolPara(DriverVolumeSetCmd XDATA_PTR volPtr);
PUBLIC void DRIVER_SetNormalVolPara(DriverVolumeSetCmd XDATA_PTR volPtr);
PUBLIC void DRIVER_SetATVolPara(DriverVolumeSetCmd XDATA_PTR volPtr);

PUBLIC U16 DRIVER_CurrentPlayingMediaID(void);

PUBLIC void DRIVER_TurnOffPureOn(void);
PUBLIC BOOL DRIVER_CheckAudioTransparencyReady(void);


//////// Suspend DSP //////////////////////
PUBLIC void DRIVER_Cmd_SuspendDSP(U8 reason, VFUN callback);
PUBLIC void DRIVER_Cmd_ClearSuspendDSPReason(U8 reason);
PUBLIC BOOL DRIVER_Query_IsDSPSuspending(void);
PUBLIC BOOL DRIVER_Query_IsDSPSuspendingReason(U8 reason);
PUBLIC BOOL DRIVER_Query_IsWaitingDSPSuspend(U8 reason);

#ifdef DEMOSOUND
PUBLIC BOOL DRIVER_IsDemoSoundReady(void);
#endif

PUBLIC BOOL DRIVER_CheckATEnablePlay(void);
PUBLIC void DRIVER_ATSwitchVPRTSCO(void);
enum
{
	AT_NOT_IN_SWITCH,
	AT_IN_SWITCH,
};
PUBLIC U8 DRIVER_CheckAudioComponetEnable(U8 componet);
#endif
