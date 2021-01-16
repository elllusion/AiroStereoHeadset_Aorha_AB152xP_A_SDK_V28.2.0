#ifndef _MMI_LINEIN_H_
	#define _MMI_LINEIN_H_

#ifdef LINEIN_ENABLE
	PUBLIC void MMI_LineIn_Init(void);
	PUBLIC void MMI_LineIn_VolumeControl(BOOL isVolumeUp);
	PUBLIC void MMI_LineIn_SetDriverVolPara(DriverVolumeSetCmd XDATA_PTR volPtr);
	PUBLIC void MMI_LineIn_SendDriverVolCmd(void);
	PUBLIC void MMI_LineIn_SetVolByLevel(U8 soundLevel, U8 smallStep);
	PUBLIC void MMI_LineIn_SetDSPSoundbarDownsampleSel(U8 DSPSoundbarDownsampleSel);
	PUBLIC void MMI_LineIn_NotifyDSPStatus(BOOL isEnable);
	PUBLIC BOOL MMI_LineIn_IsDSPOpen(void);
	PUBLIC void MMI_LineIn_DriverCmd(U8 LineInCmd);
	PUBLIC U8 MMI_LineIn_GetSoundLevel(void);
	PUBLIC U8 MMI_LineIn_GetSmallStepVol(void);
	PUBLIC void MMI_LineIn_MuteControl(BOOL isMuteOn);

	PUBLIC void MMI_LineIn_ProcessKeyEvent(U8 linkIndex, U8 index);
	PUBLIC U16 MMI_LineIn_CheckValidKey(U8 index, U16 keyCode);
	
	PUBLIC void MMI_LineIn_EncodedDataHandler(void);
	PUBLIC void MMI_LineIn_DisableFunction(void);
	PUBLIC void MMI_LineIn_EnableFunction(void);
	PUBLIC void MMI_LineIn_AbsVolToSoundLevel(U8 absVol);
	PUBLIC U8 MMI_LineIn_GetAbsVolByLineIn(void);
	
	PUBLIC void MMI_LineIn_EnablePayloadEncoder(void);
	PUBLIC void MMI_LineIn_DisablePayloadEncoder(void);

	typedef struct
	{
		U8 isDSPOpen:1;
		U8 DSPSoundbarDownsampleSel:3;
		U8 isLineInProhibited:1;
		U8 isPayloadEncode:1;
		U8 reserved:2;
		U8 currentsoundLevel;
		U8 smallStepVol;
		U8 absVolume;
	}MmiLineInInfoCtrl;

#endif

#endif
