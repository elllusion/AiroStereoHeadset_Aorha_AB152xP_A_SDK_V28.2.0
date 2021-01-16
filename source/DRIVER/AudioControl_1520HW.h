
//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _AUDIOCONTROL_H_1520_HW
#define _AUDIOCONTROL_H_1520_HW

	#define AUDIO_I2S_MASTER	(TRUE)
	#define AUDIO_I2S_SLAVE		(FALSE)

	#define AUDIO_I2S_IN	0x01
	#define AUDIO_I2S_OUT	0x02
	
	#define MXT_CTL_2M		0x01
	#define MXT_CTL_3M		0x02

#ifdef OPERATION_IN_BB_ASIC_VERSION
PUBLIC void AUDIO_SetAudioIn(BOOL isEnable);
PUBLIC void AUDIO_SetAudioOut(BOOL isEnable);
PUBLIC void AUDIO_ResetAudioInAudioOut(void);
PUBLIC void AUDIO_SetMicOnOff(BOOL isEnableL, BOOL isEnableR);
PUBLIC void AUDIO_SetAD(BOOL isEnable);
PUBLIC void AUDIO_SetDA(BOOL isEnable);
#endif

PUBLIC void AUDIO_SetMicPinAndClock(U8 digitalMicDataInPin, U8 digitalMicClkOutPin);
PUBLIC VOID AUDIO_SetI2SGrp(U8 i2sGrpSel, BOOL isEnable, BOOL isMaster, U8 devType);
PUBLIC VOID AUDIO_SetI2SDataInPio(U8 dataInPio, U8 isMaster);
PUBLIC VOID AUDIO_SetSPDIF(U8 spdifPIO, BOOL isEnable);
PUBLIC VOID AUDIO_SetMClock(BOOL isEnable, BOOL isInternal, U8 divideNum, BOOL isPhaseInvert);
PUBLIC void AUDIO_SetMClockOutDisable(BOOL isDisable);
PUBLIC void AUDIO_SetMXTCtl(U8 speed);
PUBLIC U8 AUDIO_GetMXTCtl(void);
PUBLIC void AUDIO_SetLineInMicEnableHW(BOOL isEnable);
PUBLIC void AUDIO_AudioEnable(BOOL isEnable);
#ifdef OPERATION_IN_BB_FPGA_VERSION
PUBLIC void AUDIO_1520FPGAEnableADDA(U8 component);
PUBLIC void AUDIO_1520FPGADisableADDA(void);
#endif



#endif
