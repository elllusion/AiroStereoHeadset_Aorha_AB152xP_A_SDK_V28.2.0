#ifndef _MMI_A2DP_H_
	#define _MMI_A2DP_H_

	#include "MMI_Base.h"

	#ifdef _MMI_A2DP_C_
		#define MMI_A2DP_EXTERN
	#else
		#define MMI_A2DP_EXTERN extern
	#endif
	
	typedef struct
	{
		U8 tws_audio_setting;
	}MmiA2dpCtlType;
	
	#ifdef DEMOSOUND
	typedef struct
	{
		U8 soundLevel;
		U8 smallStep;
	}MmiDemoSoundCtlType;
	#endif
	
	#define UART_CMD_HQ_CODEC_ENABLED	0x01
	typedef struct
	{
		MmiA2dpCtlType A2dpCtl[MAX_MULTI_POINT_NO];
		U8 isHQCodecDisabled:1;
		U8 auxGainReplaced:1;
		U8 reserved:6;
		#ifdef DEMOSOUND
		MmiDemoSoundCtlType demoSoundCtl;
		#endif
	}GMmiA2dpCtlType;
	
	MMI_A2DP_EXTERN GMmiA2dpCtlType XDATA gMMI_A2DP_ctl;

	PUBLIC void MMI_A2DP_EventHandler(U8 linkIndex, U8 eventCode);
	PUBLIC void MMI_A2DP_ConnectedHandle(U8 linkIndex);
	PUBLIC void MMI_A2DP_ClearCtlData(U8 linkIndex);
	PUBLIC BOOL MMI_A2DP_IsConnected(U8 linkIndex);
	PUBLIC void MMI_A2DP_ClearResumeState(U8 linkInd);
	PUBLIC void MMI_A2DP_SetDriverVolPara(U8 soundLevel, U8 smallStepLevel, DriverVolumeSetCmd XDATA_PTR volPtr);
	PUBLIC void MMI_A2DP_SetDriverVolParaforMicCtlInA2DP(U8 soundLevel, U8 smallStepLevel, DriverVolumeSetCmd XDATA_PTR volPtr);
	PUBLIC void MMI_A2DP_SendDriverVolCmd(U8 soundLevel, U8 smallStepLevel);
	PUBLIC void MMI_A2DP_VolumeControl(U8 linkIndex, BOOL isVolumeUp, BOOL isFollowerCmd);
	PUBLIC void MMI_A2DP_HighQualityCodecHandle(U8 settings);	
	PUBLIC BOOL MMI_A2DP_IsHighQualityCodecDisbaled(void);
	
	PUBLIC BOOL MMI_A2DP_IsInStreaming(U8 linkIndex);
	PUBLIC void MMI_A2DP_SendResume(U8 linkIndex);
	PUBLIC void MMI_A2DP_SendPauseMusic(U8 linkIndex, U8 isCall);
	PUBLIC void MMI_A2DP_ConnectMediaChannelSource(U8 linkIndex);
	PUBLIC BOOL MMI_A2DP_EncodedDataHandler(U8 XDATA_PTR ptr1);
	PUBLIC void MMI_A2DP_CheckPlayStatusConsistency(U8 linkIndex, U8 playStatus, U8 needDelay);
	PUBLIC void MMI_A2DP_PlayStatusChanged(U8 linkIndex, U8 playStatus);
	PUBLIC void MMI_A2DP_PlayStatusTheSame(U8 linkIndex, U8 playStatus);
	PUBLIC void MMI_A2DP_NotifyDSPStatus(BOOL isEnable, U8 linkIndex);
	#ifdef A2DP_SRC_Profile
	PUBLIC void MMI_A2DP_LineInEnableNotify(void);
	#endif
	PUBLIC BOOL MMI_A2DP_IsAudioOn(U8 linkIndex);
	
	PUBLIC void MMI_A2DP_SetVolByLevel(U8 linkIndex, U8 soundLevel, U8 smallStep);
	PUBLIC void MMI_A2DP_SendDSPA2DPEnableRelatedCmd(U8 linkIndex, U8 opCode);
	
#ifdef A2DP_Profile
	PUBLIC void MMI_A2DP_SendCmd(U8 linkIndex, U8 cmdCode);
	PUBLIC void MMI_A2DP_StopDSP(U8 linkIndex);
	PUBLIC void MMI_A2DP_SendDSPEnableCmd(U8 linkIndex);
#else
	#define MMI_A2DP_SendCmd(linkIndex, cmdCode)
	#define MMI_A2DP_StopDSP(linkIndex)
	#define MMI_A2DP_SendDSPEnableCmd(linkIndex)
#endif

	#define MUSIC_EXISTS(A)							MMI_A2DP_IsInStreaming(A)
	
#ifdef DEMOSOUND
	PUBLIC void MMI_A2DP_DemoSoundPlayCmd(void);
	PUBLIC void MMI_A2DP_DemoSoundStopCmd(void);
	PUBLIC void MMI_A2DP_KeyDemoSoundPlay(void);
	PUBLIC void MMI_A2DP_KeyDemoSoundPause(void);
	PUBLIC void MMI_A2DP_KeyDemoSoundStop(void);
	PUBLIC void MMI_A2DP_KeyDemoSoundForward(void);
	PUBLIC void MMI_A2DP_KeyDemoSoundBackward(void);
	PUBLIC void MMI_A2DP_DemoSoundVolumeControl(U8 isVolumeUp);
#endif

#endif
