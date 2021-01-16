
//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _AUDIOCONTROL_H_1520
#define _AUDIOCONTROL_H_1520
	
	#define GAIN_OWNER_MCU	FALSE
	#define GAIN_OWNER_DSP	TRUE
	
	
	#define AUDIO_ENABLE_MIC			0x80
	#define AUDIO_ENABLE_BOTH_AD_DA		0x06
	#define AUDIO_ENABLE_DA		0x02
	#define AUDIO_ENABLE_AD		0x04

	#define AUDIO_DISABLE_BOTH_AD_DA	0xF9
	#define AUDIO_DISBALE_DA_ONLY		0xFD
	#define AUDIO_DISBALE_AD_ONLY		0xFB

	//gAlignDSP.DSP_nvram.DSP_Reserved_PARAM.Feature_Sel
	#define MIC_PTR_L_TO_R		0x2000
	#define ECNR_DISABLE		(0x0001 << 12)	
	#define VP_FEEDBACK		(0x0001 << 15)

PUBLIC void Audio1520_Init(void);

PUBLIC BOOL AUDIO_IsAllowVoicePrompt(void);
PUBLIC BOOL AUDIO_IsAllowSCOEnabled(U8 linkIndex);
PUBLIC void AUDIO_SetGainCtlOwner(BOOL isOwnedByDSP);
PUBLIC void AUDIO_SetMXTCtrlTo2MForVPRing(void);


PUBLIC void AUDIO_SetAudioInOut(U8 audioIn, U8 audioOut);
PUBLIC void AUDIO_SetAudioInOutAsDefault(void);
PUBLIC void AUDIO_ResetADDA(void);
PUBLIC void AUDIO_SetDSPOn (U8 component);
PUBLIC BOOL AUDIO_SetDSPOff (U8 component);
PUBLIC void AUDIO_SetADDAOn(U8 component);
PUBLIC void AUDIO_SetADDAOff(U8 component);
PUBLIC void AUDIO_ADControl(U8 finalMask);
PUBLIC void AUDIO_DAControl(U8 finalMask);
PUBLIC void AUDIO_DisbaleCurrentDigitalInOut();
PUBLIC void AUDIO_EnableDigitalInOut(void);
#ifdef SUPPORT_VOICE_COMMAND
PUBLIC void AUDIO_ChangeVoiceCmdDSPSpeed(void);
PUBLIC BOOL AUDIO_CheckVoiceCmdDSPSpeed(void);
#endif
#ifndef BLUETOOTH_SPEAKER
PUBLIC void AUDIO_SetMicEnable(U8 enableType, U8 isLineIn);
#endif
PUBLIC void AUDIO_SetDSPECNR(U8 setECNR);
PUBLIC void AUDIO_SetLineInMicEnable(BOOL isEnable);
#include "Audio_MailBox_Structure.h"
PUBLIC void AUDIO_SetAudioInOutToMailBoxCmd(U8 component, AUDIO_IN_OUT_SCENARIO_STRU XDATA_PTR scenarioPtr);
PUBLIC void AUDIO_SwitchStereoMono1520(void);


#endif