
//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _AUDIOCONTROL_H_
#define _AUDIOCONTROL_H_

#include "os.h"
#include "rc.h"
#include "AudioControl_1520.h"


//Aware : The max volume of ringtone is 0

#define SPEAKER_MAX_VOL 15
#define SPEAKER_MIN_VOL 0
#define MIC_MAX_VOL 15
#define MIC_MIN_VOL 0

#define RC_AUDIO_ENABLE_IN_EEPROM			(gSYS_audio_ctl.audio_C2_57 | 0x05)
#define RC_AUDIO_DISABLE_IN_EEPROM			(gSYS_audio_ctl.audio_C2_57 & 0xFA)	

#define AUDIO_COMPONENT_IS_ACTIVE(A)            gAUDIO_daadEnabledMasks[A]

#define SPK_STEREO			0
#define SPK_MONO_L		0x0100

enum
{
	LINE_IN_EXIT_MODE,
	LINE_IN_BYPASS_MODE,
	LINE_IN_DSP_MODE,
	LINE_IN_MODE_TOTAL_NO,
#ifndef BLUETOOTH_SPEAKER
	LINE_IN_MIC_ENABLE_DEFAULT,
	LINE_IN_MIC_ENABLE_L,
	LINE_IN_MIC_ENABLE_R,
	LINE_IN_MIC_ENABLE_ALL,
	LINE_IN_MIC_TEST_NO,
#endif
	SPK_MONO_SOUND,
	SPK_STEREO_SOUND,
};

enum
{
	DAC,
	ADC
};

enum
{
	AMP_OFF,
	AMP_ON,
	AMP_WAIT_CLOSE,
};


typedef struct
{
	U8 daadCtl;
	U8 isADMute:1;
	U8 isDAMute:1;
	U8 is3WiredAudioMute:1;
	U8 isMuteForPEQ:1;
	U8 isMuteForRedial:1;
	U8 isAMPEnabled:1;
	U8 isGainOwnByDSP:1;
	U8 isADEnabled:1;
	/////////////////////////////////////////
	U8 isDAEnabled:1;
	U8 isI2SGrp0EnabledDev:2;
	U8 isI2SGrp2EnabledDev:2;
	U8 MCLKEnabled:1;
	U8 isLineInMute:1;
	U8 isMuteBySoundLevel:1;
	///////////////////////////////
	U8 isAiroStereoDisabled:1;
	U8 isNormalLineMode:1;
	U8 isMicEnabledL:1;
	U8 isMicEnabledR:1;
	U8 isNeedPilotTone:1;
	U8 isEscoRelayDisable:1;
	U8 reserved:2;
	//////////////////////////////
	U8 dspAudioModeSel;
	U8 muteCloseAmpState;
	OST XDATA_PTR delayForCloseAudiotimer;
	OST XDATA_PTR threeWireMuteTimer;
	U8 AUDIO_I2S_MASTER_GROUP_SEL;
	U8 AUDIO_I2S_SLAVE_GROUP_SEL;
	U8 SPDIF_PIO;
	U8 I2SIN_PIO;
	U8 MCLKDivideNum;
	U8 MCLK_INTERNAL:1;
	U8 MCLK_PHASE_INVERT:1;
	U8 isAudioInEnable:1;
	U8 isAudioOutEnable:1;
	U8 isMuteForVPRingtone:1;
	U8 I2SMCLKPioDisable:1;
	U8 isVCEnabled:1;
	U8 isLineInEnabled:1;
	U8 AUDIO_IN_SCENARIO_SEL;
	U8 AUDIO_OUT_SCENARIO_SEL;
	U8 digitalMicInPin;
} AUDIO_CtrlRegType;

enum
{
	VOICE_VOLUP,
	VOICE_VOLDN,
	VOICE_VOLSET,
	MIC_VOLUP,
	MIC_VOLDN,
	MIC_VOLSET,
	VM_VOLSET, //set voice & mic vol level in one cmd
};

enum
{
	AUDIO_RINGTONE,
	AUDIO_SCO,
	AUDIO_VOICEPROMPT,
	AUDIO_A2DP,
	AUDIO_LINE_IN,
	AUDIO_VOICECOMMAND,
	AUDIO_TRANSPARENCY,
	AUDIO_ASYNC_SCO,
	AUDIO_FM,
	AUDIO_DSP_RELATED_ENUM = AUDIO_FM,
	AUDIO_PURE_ON,
	AUDIO_COMPONENT_NO
};
	
enum
{
	A2DP_OFF,
	A2DP_ON_DA,
	A2DP_ON_ADDA_WITHLINEIN,
};
	

#ifdef AUDIOCONTROL_C
	#define AUDIOCONTROL_EXT 
#else
	#define AUDIOCONTROL_EXT extern
#endif

AUDIOCONTROL_EXT AUDIO_CtrlRegType XDATA gAUDIO_CtrlInfo;
AUDIOCONTROL_EXT U8 XDATA gAUDIO_daadEnabledMasks[AUDIO_COMPONENT_NO];
AUDIOCONTROL_EXT U8 XDATA gAUDIO_audioOutMask;

PUBLIC void AUDIO_Init(void);
PUBLIC void AUDIO_CheckTimer(void);
PUBLIC void AUDIO_SwitchMute(U8 dev, BOOL isMute);
PUBLIC BOOL AUDIO_CheckOtherAudioEnabled(U8 component);
PUBLIC U8 AUDIO_CheckAnyAudioEnabled(void);
PUBLIC BOOL AUDIO_CheckIfAmpAllowedOpen(U8 component);
PUBLIC void AUDIO_SetAudioOff(U8 component); 
PUBLIC void AUDIO_SetAudioOn(U8 component); 
PUBLIC void AUDIO_Open_AMP(void);
PUBLIC void AUDIO_Close_AMP(void);
PUBLIC void AUDIO_CloseAudios(U8 lastComponent);
PUBLIC U8 AUDIO_GetdaadEnabledFinalMask(void);
PUBLIC void AUDIO_LineInCheckChangeToByPass(void);
PUBLIC void AUDIO_SetMicSwitch(U8 enableType);
PUBLIC void AUDIO_SetECNROnOff(U8 setECNR);
PUBLIC void AUDIO_PresetLineInMode(U8 mode);
PUBLIC void AUDIO_SwitchStereoMono(void);

// Defined in rest files
PUBLIC void AUDIO_3WireMuteAudio(BOOL isMute);
PUBLIC BOOL AUDIO_SetDSPMode(U8 component);


PUBLIC void AUDIO_SetAudioMode(U8 mode, BOOL isOn);
PUBLIC void AUDIO_SetA2DPMode(U8 isOn);
PUBLIC void AUDIO_SetSCOMode(BOOL isOn);
#endif
