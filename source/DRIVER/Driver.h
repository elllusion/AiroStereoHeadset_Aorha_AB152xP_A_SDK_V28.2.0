#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "driver_interface.h"


#define VOICE_PROMPT_ALL_CLEAN 0xFF

#ifdef _SOUND_EFFECT_C
#define SOUND_EFFECT_EXTERN
#else
#define SOUND_EFFECT_EXTERN extern 
#endif

#ifdef _DBB_ON_OFF_C
#define DBB_ON_OFF_EXTERN
#else
#define DBB_ON_OFF_EXTERN extern 
#endif


enum
{
	MEMORY_PUT,
	QUE_PUTFRONT,
	CONTINUE_LOOP,
	RETURN_FUNCTION,
	QUE_PUT,
};

typedef struct
{
	U8 (* MMICmdHandler)(DriverMsg XDATA_PTR msgPtr);
}DRIVER_MMI_CMD_HANDLER_TYPE;

enum
{
	DRIVER_POWER_SAVING_FM	= 0x01,
	DRIVER_POWER_ROTARY_ENCODER	= 0x02,
	DRIVER_POWER_SAVING_DSP_A2DP = 0x10,
	DRIVER_POWER_SAVING_KEY_TIMER = 0x20,
};

enum
{
	SOUND_EFFECT_A2DP,
	SOUND_EFFECT_LINE_IN,
	SOUND_EFFECT_TOTAL_NO,
};

typedef struct
{
	U8 modeSoundEffect;
}SOUND_EFFECT_TYPE;

SOUND_EFFECT_EXTERN SOUND_EFFECT_TYPE XDATA soundEffect_Ctl[SOUND_EFFECT_TOTAL_NO];
DBB_ON_OFF_EXTERN U8 XDATA dbbOnOff_Ctl;

PUBLIC void DRIVER_ProcMmiCmd(void);
PUBLIC void DRIVER_CheckTimer(void);
PUBLIC void DRIVER_SetMiscCtrlMap(U8 mistCtrlType, BOOL enable);
PUBLIC DriverMsg XDATA_PTR DRIVER_GetGeneralEventMemory(U8 driverMsgType);

PUBLIC void DRIVER_RingtoneVPFMATCmdScheduler(void);
PUBLIC BOOL DRIVER_RingtoneCmdHandler(DriverMsg XDATA_PTR msgPtr);

PUBLIC BOOL DRIVER_VoicePromptCmdHandler(DriverMsg XDATA_PTR msgPtr);

PUBLIC BOOL DRIVER_AudioTransparencyCmdHandler(DriverMsg XDATA_PTR msgPtr);

PUBLIC void DRIVER_ClearAudioCmd(U8 opCode, U16 id);
PUBLIC void DRIVER_KeyDoubleCheck(U8 scanInd);
PUBLIC void DRIVER_KeyTrippleCheck(U8 scanInd);
PUBLIC void DRIVER_KeyPressCheck(U8 scanInd);
PUBLIC void DRIVER_KeyLongpressCheck(U8 scanInd);
PUBLIC void DRIVER_KeyLongLongPressCheck(U8 scanInd);
PUBLIC void DRIVER_KeyExtremelyLongPressCheck(U8 scanInd);
PUBLIC void DRIVER_KeyToggleLongpressCheck(U8 scanInd);
PUBLIC void DRIVER_KeyToggleLongReleaseCheck(U8 scanInd);

PUBLIC void DRIVER_LedFGTimeCalculatorAndSet(U8 beginIndex, U8 endIndex);





#ifdef PEQ_ENABLE
PUBLIC void DRIVER_PEQSetMuteTimer(void);
PUBLIC void DRIVER_PEQReload(void);
PUBLIC void DRIVER_ChangePEQMode(void);
#endif
PUBLIC void DRIVER_SetVprtVolumeToHW(void);
PUBLIC void DRIVER_SetLineInVolumeToHW(void);
PUBLIC void DRIVER_SetA2DPVolumeToHW(void);
PUBLIC void DRIVER_SetNormalVolumeToHW(void);

PUBLIC void DRIVER_SendA2DPEnableEvent(void);
PUBLIC void DRIVER_SendA2DPDisableEvent(void);
PUBLIC void DRIVER_SendSCOEnableEvent(BOOL isLightMono);
PUBLIC void DRIVER_SendSCODisableEvent(void);
PUBLIC void DRIVER_SendLineInEnableEvent(void);
PUBLIC void DRIVER_SendLineInDisableEvent(void);
PUBLIC void DRIVER_SendRequestCloseFMEvent(void);
PUBLIC void DRIVER_SendAsyncSCOEnabledEvent(void);
PUBLIC void DRIVER_SendAsyncSCODisabledEvent(void);

PUBLIC void DRIVER_TurnOffA2DP(void);
PUBLIC void DRIVER_TurnOffSCO(void);
PUBLIC void DRIVER_TurnOffLineIn(void);
PUBLIC void DRIVER_TurnOffAsyncSCO(void);

PUBLIC BOOL DRIVER_IsRingToneOrVPOrATPlaying(void);
PUBLIC BOOL DRIVER_IsMicMuted(void);
PUBLIC BOOL DRIVER_IsMiscCtrlTypeEnabled(U8 mistCtrlType);

PUBLIC void DRIVER_SendRingtoneEndedEvent(U16 id);
PUBLIC void DRIVER_SendVoiceCommandEndedEvent(U8 id, U8 matchedNo, U8 stoppedByUser);
PUBLIC void DRIVER_SetPowerSaving(BOOL isPowerSaving, U8 device);
PUBLIC void DRIVER_RejectCallMuteSCOCmd(BOOL value);

PUBLIC DriverMsg XDATA_PTR DRIVER_GetCmdMsg(U8 opCode);

PUBLIC U8 DRIVER_GetVoicepromptsNumberInQueue(void);

#endif //__DRIVER_H__
