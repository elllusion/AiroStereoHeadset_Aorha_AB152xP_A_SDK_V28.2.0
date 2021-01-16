
//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _AUDIOCONTROL_H_1500
#define _AUDIOCONTROL_H_1500

	#define SET_REG_DA_ANALOG_GAIN(gain) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_ANALOG_DA_GAIN, gain); \
		OS_EXIT_CRITICAL()
	
	#define SET_REG_AD_ANALOG_GAIN(gain) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_IN_GAIN_CTRL_R_MCU, gain); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_IN_GAIN_CTRL_L_MCU, gain); \
		OS_EXIT_CRITICAL()
		
		//0XC2 offset 221, 222
	#define SET_RINGTONE_STARTADD(add) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_WO_RING_START_ADDR_L, (U8)((U32)add & 0X00FF)) \
		LC_SFR_WRITE_BY_C2(C2OFFS_WO_RING_START_ADDR_H, (U8)(((U32)add & 0XFF00) >> 8)) \
		OS_EXIT_CRITICAL()

	//0XC2 offset 223
	#define SET_RINGTONE_ENABLE(onOff) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_RING_ENABLE, onOff); \
		OS_EXIT_CRITICAL()
		
	#define AUDIO_MODE_SELECT(A)	\
		OS_ENTER_CRITICAL(); \
		gAlign.AUDIO_MODE_SEL = (A); \
		OS_EXIT_CRITICAL()
		
	#define SET_AUDIO_IN_OUT_ENABLE(value) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_IN_OUT_ENABLE, value); \
		OS_EXIT_CRITICAL()	
	
	#define SET_AUDIO_ENABLE(value) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_ENABLE, value); \
		OS_EXIT_CRITICAL()
		
	 #define SET_REG_VOICE_GAIN(gain) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_DAC_GC_SEL, gain); \
		OS_EXIT_CRITICAL()
		
	#define SET_REG_MIC_GAIN(gain) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_ADC_GC_SEL, gain); \
		OS_EXIT_CRITICAL()
	
	//0XC2 offset 58
	#define SET_AUDIO_SEL(reg) \
		OS_ENTER_CRITICAL(); \
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_SEL, reg.audSel); \
		OS_EXIT_CRITICAL()
		
	#define AUDIO_MODE_STEREO		0x0100
	#define AUDIO_MODE_MONO			0x0200
	#define AUDIO_MODE_LINEIN		0x0400
	#define AUDIO_MODE_BUSY_FLAG	0x0010
	
	#define AUDIO_ENABLED(A)				(gAUDIO_daadEnabledMasks[A])
	
	#define AUDIO_ENABLE_BOTH_AD_DA		0x07
	#define AUDIO_ENABLE_DA		0x03
	#define AUDIO_ENABLE_AD		0x04
	#define AUDIO_DISABLE_BOTH_AD_DA	0xF8
	#define AUDIO_DISBALE_DA_ONLY		0xFC
	#define AUDIO_DISBALE_AD_ONLY		0xFB

PUBLIC void Audio1500_Init(void);
PUBLIC void AUDIO_MONOTRxReset(void);
PUBLIC void AUDIO_SetDSPModeSelect(U8 component);
PUBLIC void AUDIO_SetAnalogGains(U8 analogDaGain, U8 analogAdGain);
PUBLIC void AUDIO_SetSpeakerDigitalGain(U8 cmd, U8 level);
PUBLIC void AUDIO_SetMicDigitalGain(U8 cmd, U8 level);


#endif //_AUDIOCONTROL_H_1500