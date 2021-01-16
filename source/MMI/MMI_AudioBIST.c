#include "MMI_FCD.h"

enum audio_bist_mode_enum
{
    AUDIO_BIST_DISABLE = 0,
    AUDIO_6M_MUTE_PATTERN_MODE,
    AUDIO_6M_L_INTERNAL_PATTERN_MODE,
    AUDIO_6M_R_INTERNAL_PATTERN_MODE,
    VOICE_2M_MUTE_PATTERN_MODE,
    VOICE_2M_L_INTERNAL_PATTERN_MODE,	//0x05
    VOICE_2M_R_INTERNAL_PATTERN_MODE,
	VOICE_2M_L_REPORT_SINGLE_SNP,
	VOICE_2M_R_REPORT_SINGLE_SNP,
	VOICE_2M_REPORT_SINGLE_GET,
	VOICE_2M_REPORT_SINGLE_CLOSE,		//0x0A
	AUDIO_6M_RELAY_MODE,
	VOICE_WOOFER_EXTERNAL_MODE,
	RT_NOISE_THRESHOLD_MODE,
	VOICE_2M_L_REPORT_SINGLE_SNP_4000,
	VOICE_2M_R_REPORT_SINGLE_SNP_4000,	//0x0F
	VOICE_2M_L_REPORT_SINGLE_SNP_1000,
	VOICE_2M_R_REPORT_SINGLE_SNP_1000,
	VOICE_2M_L_REPORT_SINGLE_SNP_500,
	VOICE_2M_R_REPORT_SINGLE_SNP_500,
	VOICE_2M_L_REPORT_SINGLE_SNP_250,
	VOICE_2M_R_REPORT_SINGLE_SNP_250,	//0x15
	AUDIO_BIST_MODE_NUM, 
};

#ifdef AB1122
	#define AUDIO_BIST_LINEIN_MUTE_WAIT_TIME		(160L)
	#define AUDIO_BIST_LINEIN_L_CHANNEL_WAIT_TIME	(160L)
	#define AUDIO_BIST_LINEIN_R_CHANNEL_WAIT_TIME	(160L)

	#define AUDIO_BIST_SCO_MUTE_WAIT_TIME		(160L)
	#define AUDIO_BIST_SCO_L_CHANNEL_WAIT_TIME	(5*320L)
	#define AUDIO_BIST_SCO_R_CHANNEL_WAIT_TIME	(160L)

	#define	AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP	(160L)
	#define	AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP	(160L)
	#define	AUDIO_BIST_SCO_WOOFER_EXTERNAL_MODE	(160L)
#else
	#ifdef FT_AUDIO_BIST
		#define AUDIO_BIST_LINEIN_MUTE_WAIT_TIME		(320L)
		#define AUDIO_BIST_LINEIN_L_CHANNEL_WAIT_TIME	(0.5*160L)
		#define AUDIO_BIST_LINEIN_R_CHANNEL_WAIT_TIME	(0.5*160L)
		#define AUDIO_BIST_SCO_MUTE_WAIT_TIME		(320L)
		#define AUDIO_BIST_SCO_L_CHANNEL_WAIT_TIME	(160L)
		#define AUDIO_BIST_SCO_R_CHANNEL_WAIT_TIME	(160L)
		#define	AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP	(160L)
		#define	AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP	(160L)
		#define	AUDIO_BIST_SCO_WOOFER_EXTERNAL_MODE	(160L)
	#else
		#define AUDIO_BIST_LINEIN_MUTE_WAIT_TIME			(3*320L)
		#define AUDIO_BIST_LINEIN_L_CHANNEL_WAIT_TIME		(3*320L)
		#define AUDIO_BIST_LINEIN_R_CHANNEL_WAIT_TIME		(2*320L)
		#define AUDIO_BIST_SCO_MUTE_WAIT_TIME				(320L)
		#define AUDIO_BIST_SCO_L_CHANNEL_WAIT_TIME			(1*3200L)//(160L)
		#define AUDIO_BIST_SCO_R_CHANNEL_WAIT_TIME			(1*3200L)//(160L)
		#define	AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP			(1*3200L)//(160L)
		#define	AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP			(1*3200L)//(160L)
		#define AUDIO_BIST_SCO_REPORT_SIGLE_GET_WAIT_TIME	(1*3200L)
		#define AUDIO_BIST_SCO_REPORT_SIGLE_CLOSE_WAIT_TIME	(1*3200L)
		#define AUDIO_BIST_LINEIN_RELAY_WAIT_TIME			(1*3200L)
		#define	AUDIO_BIST_SCO_WOOFER_EXTERNAL_MODE			(160L)
		#define AUDIO_BIST_RT_NOISE_THRESHOLD				(1*3200L)
		#define AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP_4000		(1*3200L)
		#define AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP_4000		(1*3200L)
		#define AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP_1000		(1*3200L)
		#define AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP_1000		(1*3200L)
		#define AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP_500		(1*3200L)
		#define AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP_500		(1*3200L)
		#define AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP_250		(1*3200L)
		#define AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP_250		(1*3200L)
	#endif
#endif

#if defined AB1520S
	#define HEADSET_ANALOG_GAIN		7
	#define HEADSET_ANALOG_GAIN_L	7
	#define HEADSET_ANALOG_GAIN_R	7
	#define SPEAKER_ANALOG_GAIN			13
	#define SPEAKER_ANALOG_GAIN_L		13
	#define SPEAKER_ANALOG_GAIN_R		13
#elif defined AB1520U
	#define HEADSET_ANALOG_GAIN		9
	#define HEADSET_ANALOG_GAIN_L	9
	#define HEADSET_ANALOG_GAIN_R	9
	#define SPEAKER_ANALOG_GAIN			9
	#define SPEAKER_ANALOG_GAIN_L		9
	#define SPEAKER_ANALOG_GAIN_R		9
#else
	#define HEADSET_ANALOG_GAIN		9
	#define HEADSET_ANALOG_GAIN_L	9
	#define HEADSET_ANALOG_GAIN_R	9
	#define SPEAKER_ANALOG_GAIN			11
	#define SPEAKER_ANALOG_GAIN_L		11
	#define SPEAKER_ANALOG_GAIN_R		11
#endif

U32 CODE MMIAudioBistTimer[AUDIO_BIST_MODE_NUM] = 
{
	0,
	AUDIO_BIST_LINEIN_MUTE_WAIT_TIME,
	AUDIO_BIST_LINEIN_L_CHANNEL_WAIT_TIME,
	AUDIO_BIST_LINEIN_R_CHANNEL_WAIT_TIME,
	AUDIO_BIST_SCO_MUTE_WAIT_TIME,
	AUDIO_BIST_SCO_L_CHANNEL_WAIT_TIME,
	AUDIO_BIST_SCO_R_CHANNEL_WAIT_TIME,
	AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP,
	AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP,
	AUDIO_BIST_SCO_REPORT_SIGLE_GET_WAIT_TIME,
	AUDIO_BIST_SCO_REPORT_SIGLE_CLOSE_WAIT_TIME,
	AUDIO_BIST_LINEIN_RELAY_WAIT_TIME,
	AUDIO_BIST_SCO_WOOFER_EXTERNAL_MODE,
	AUDIO_BIST_RT_NOISE_THRESHOLD,
	AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP_4000,
	AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP_4000,
	AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP_1000,
	AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP_1000,
	AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP_500,
	AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP_500,
	AUDIO_BIST_SCO_L_REPORT_SINGLE_SNP_250,
	AUDIO_BIST_SCO_R_REPORT_SINGLE_SNP_250,
};

enum audio_chnl_sel_enum
{
	CHANNEL_NONE,
	CHANNEL_L,
	CHANNEL_R,
	CHANNEL_LR,
};

typedef struct
{
	U8 state;
	U8 mode;
	U32 waitingTime;
	U8 checkSNDR:1;
	U8 isProcessOver:1;
	U8 isFTTest:1;
	U8 isLineInFailed:1;
	U8 isSCOFailed:1;
	U8 isDSPEnabled:1;
	U8 reserved:2;
	OST XDATA_PTR waitTimer;
	U16 SNRResult;
	U16 SNDRResult;
}AudioBISTCtlType;

AudioBISTCtlType XDATA gMMI_AudioBIST_ctl;

PRIVATE VOID MMI_DRV_AudioBistGainSet(VOID)
{
	U8 i;

    for(i = 0; i < SOUND_LEVEL_NO; i++)
	{
		MMI_SPK_GAINSET[i].digitalGain = 85;
		MMI_SPK_GAINSET[i].digitalGainL = 85;
		MMI_SPK_GAINSET[i].digitalGainA = 85;
		
		if(!MMI_IsSpeakerProduct())
		{
			MMI_SPK_GAINSET[i].analogGain = HEADSET_ANALOG_GAIN;
			MMI_SPK_GAINSET[i].analogGainL = HEADSET_ANALOG_GAIN_L;
			MMI_SPK_GAINSET[i].analogGainA = HEADSET_ANALOG_GAIN_R;
		}
		else
		{
			MMI_SPK_GAINSET[i].analogGain = SPEAKER_ANALOG_GAIN;
			MMI_SPK_GAINSET[i].analogGainL = SPEAKER_ANALOG_GAIN_L;
			MMI_SPK_GAINSET[i].analogGainA = SPEAKER_ANALOG_GAIN_R;
		}
	}
}

PRIVATE void MMI_DRV_AudioBistEnableLineInCmd(U8 mode)
{
	MMIMsgType XDATA_PTR msgPtr;

	msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_LINE_IN_CMD);

	msgPtr->msgBodyPtr.driverCmd.auxCmd.m2d_ctl.para.linein_start_para.feature_sel.SOUNDBAR_ESCO_RELAY = FALSE;
	gAlignDSP.IC_BIST_MODE = mode;
	
	switch(mode)
	{
		case AUDIO_6M_MUTE_PATTERN_MODE:
			gAlignDSP.IC_BIST_IN_CHNL_SEL = CHANNEL_LR;
			break;
		case AUDIO_6M_L_INTERNAL_PATTERN_MODE:
			gAlignDSP.IC_BIST_IN_CHNL_SEL = CHANNEL_L;
			break;
		case AUDIO_6M_R_INTERNAL_PATTERN_MODE:
			gAlignDSP.IC_BIST_IN_CHNL_SEL = CHANNEL_R;
			break;
	}
	
	gMMI_AudioBIST_ctl.isProcessOver = FALSE;
	OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	MMI_DRV_AudioBistGainSet();
	#if defined FT_AUDIO_BIST && !defined AB1520U
	DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 0, 16);
	#endif
}

PRIVATE void MMI_DRV_AudioBistEnableVoiceCmd(U8 mode)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_SCO_CMD);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.driverCmd.scoCmd.linkInd = DEFAULT_LINK_IND;
		msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.sco_type = gMMI_ctl.mmiInfo[DEFAULT_LINK_IND].linkPara.scoType;
		msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.pkt_type = gMMI_ctl.mmiInfo[DEFAULT_LINK_IND].linkPara.scoPktType;
		msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.sco_interval = gMMI_ctl.mmiInfo[DEFAULT_LINK_IND].linkPara.txInterval;
		msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.codec_type = gMMI_ctl.mmiInfo[DEFAULT_LINK_IND].linkPara.scoCodecType;
		msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.pkt_length = gMMI_ctl.mmiInfo[DEFAULT_LINK_IND].linkPara.rxPacketLen;
		msgPtr->msgBodyPtr.driverCmd.scoCmd.m2d_ctl.para.mono_para.feature_sel.WOOFER_LPF_ENABLE = 0;
		
		gAlignDSP.IC_BIST_MODE = mode;
		gMMI_AudioBIST_ctl.isProcessOver = FALSE;
		
		switch(mode)
		{
			case VOICE_2M_MUTE_PATTERN_MODE:
				gAlignDSP.IC_BIST_IN_CHNL_SEL = CHANNEL_LR;
				break;
			case VOICE_2M_L_INTERNAL_PATTERN_MODE:
			case VOICE_2M_L_REPORT_SINGLE_SNP_4000:
			case VOICE_2M_L_REPORT_SINGLE_SNP_1000:
			case VOICE_2M_L_REPORT_SINGLE_SNP_500:
			case VOICE_2M_L_REPORT_SINGLE_SNP_250:
				gAlignDSP.IC_BIST_IN_CHNL_SEL = CHANNEL_L;
				break;
			case VOICE_2M_R_INTERNAL_PATTERN_MODE:
			case VOICE_2M_R_REPORT_SINGLE_SNP_4000:
			case VOICE_2M_R_REPORT_SINGLE_SNP_1000:
			case VOICE_2M_R_REPORT_SINGLE_SNP_500:
			case VOICE_2M_R_REPORT_SINGLE_SNP_250:
				if(((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->isDualMic)
				{
					gAlignDSP.IC_BIST_IN_CHNL_SEL = CHANNEL_R;
				}
				else
				{
					gAlignDSP.IC_BIST_IN_CHNL_SEL = CHANNEL_L;
				}
				break;
		}
		
		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
		#ifdef FT_AUDIO_BIST
		#ifndef AB1520U
		DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 1, 16);
		#else
		DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 1, 3);
		#endif
		#endif
	}
	MMI_DRV_AudioBistGainSet();
}

PUBLIC void MMI_DRV_AudioBistHCICommand(MMIMsgType XDATA_PTR msgPtr)
{
	msgPtr->msgBodyPtr.hciBISTCmd.audioSel.audioInSel = msgPtr->msgBodyPtr.hciBISTCmd.AudioIn;
	msgPtr->msgBodyPtr.hciBISTCmd.audioSel.audioOutSel = msgPtr->msgBodyPtr.hciBISTCmd.AudioOut;
	
	if(msgPtr->msgBodyPtr.hciBISTCmd.AudioIn != AU_IN_AD || ((msgPtr->msgBodyPtr.hciBISTCmd.AudioOut != AU_OUT_DA) && (msgPtr->msgBodyPtr.hciBISTCmd.AudioOut != AU_OUT_DA_96K)))
	{
		msgPtr->msgBodyPtr.hciBISTCmd.audioSel.audioI2SMasterGroupSel = ((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->MPAudioInOutCtl.AUDIO_I2S_MASTER_GROUP_SEL;
		msgPtr->msgBodyPtr.hciBISTCmd.audioSel.audioI2SSlaveGroupSel = ((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->MPAudioInOutCtl.AUDIO_I2S_SLAVE_GROUP_SEL;
		msgPtr->msgBodyPtr.hciBISTCmd.audioSel.SPDIFPIO = ((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->MPAudioInOutCtl.SPDIF_PIO;
		msgPtr->msgBodyPtr.hciBISTCmd.audioSel.I2SInPIO = ((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->MPAudioInOutCtl.I2SIN_PIO;
		msgPtr->msgBodyPtr.hciBISTCmd.audioSel.MCLKInternal = (((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->MPAudioInOutCtl.MCLK_INTERNAL)?TRUE:FALSE;
		msgPtr->msgBodyPtr.hciBISTCmd.audioSel.MCLKDivideNum = ((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->MPAudioInOutCtl.MCLK_DIVIDE_NUM;
		msgPtr->msgBodyPtr.hciBISTCmd.audioSel.MCLKPhaseInvert = (((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->MPAudioInOutCtl.MCLK_PHASE_INVERT)?TRUE:FALSE;
		msgPtr->msgBodyPtr.hciBISTCmd.audioSel.I2SMCLKPioDisable = ((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->MPAudioInOutCtl.I2SMCLKPioDisable;
	}
	msgPtr->msgBodyPtr.hciBISTCmd.audioSel.isResetToDefault = FALSE;
	MMI_DRV_AudioSelCmd((DriverAudioSelCmd XDATA_PTR)&msgPtr->msgBodyPtr.hciBISTCmd.audioSel);

	switch(msgPtr->msgBodyPtr.hciBISTCmd.BISTScenario)
	{
		case AUDIO_BIST_DISABLE:
			if(gMMI_AudioBIST_ctl.mode >= AUDIO_6M_MUTE_PATTERN_MODE && gMMI_AudioBIST_ctl.mode <= AUDIO_6M_R_INTERNAL_PATTERN_MODE)
			{
				#ifdef LINEIN_ENABLE
				MMI_LineIn_DriverCmd(DRIVER_STOP_LINE_IN_CMD);
				#endif
			}
			else if(gMMI_AudioBIST_ctl.mode >= VOICE_2M_MUTE_PATTERN_MODE && gMMI_AudioBIST_ctl.mode <= VOICE_2M_R_INTERNAL_PATTERN_MODE)
			{
				MMI_StopA2DPOrSCOCmd(DEFAULT_LINK_IND, FALSE);
			}
			break;
		case AUDIO_6M_MUTE_PATTERN_MODE:
		case AUDIO_6M_L_INTERNAL_PATTERN_MODE:
		case AUDIO_6M_R_INTERNAL_PATTERN_MODE:
			MMI_DRV_AudioBistEnableLineInCmd(msgPtr->msgBodyPtr.hciBISTCmd.BISTScenario);
			break;
		
		case VOICE_2M_MUTE_PATTERN_MODE:
		case VOICE_2M_L_INTERNAL_PATTERN_MODE:
		case VOICE_2M_R_INTERNAL_PATTERN_MODE:
		case VOICE_2M_L_REPORT_SINGLE_SNP_4000:
		case VOICE_2M_R_REPORT_SINGLE_SNP_4000:
		case VOICE_2M_L_REPORT_SINGLE_SNP_1000:
		case VOICE_2M_R_REPORT_SINGLE_SNP_1000:
		case VOICE_2M_L_REPORT_SINGLE_SNP_500:
		case VOICE_2M_R_REPORT_SINGLE_SNP_500:
		case VOICE_2M_L_REPORT_SINGLE_SNP_250:
		case VOICE_2M_R_REPORT_SINGLE_SNP_250:
			MMI_DRV_AudioBistEnableVoiceCmd(msgPtr->msgBodyPtr.hciBISTCmd.BISTScenario);
			break;
	}
	gMMI_AudioBIST_ctl.mode = msgPtr->msgBodyPtr.hciBISTCmd.BISTScenario;
	gMMI_AudioBIST_ctl.waitingTime = MMIAudioBistTimer[msgPtr->msgBodyPtr.hciBISTCmd.BISTScenario];
	OSMEM_Put((U8 XDATA_PTR)msgPtr);
}

PUBLIC void MMI_DRV_AudioBistSetTimer(void)
{
	if(gMMI_AudioBIST_ctl.mode && gMMI_AudioBIST_ctl.waitTimer == (OST XDATA_PTR)NULL)
	{
		SYS_SetTimer(&gMMI_AudioBIST_ctl.waitTimer, gMMI_AudioBIST_ctl.waitingTime);
		gMMI_AudioBIST_ctl.isDSPEnabled = TRUE;
	}
}

PUBLIC void MMI_DRV_AudioBistCheckTimer(void)
{
#ifdef FT_AUDIO_BIST
	U16 IC_BIST_SNR_RESULT, IC_BIST_SNDR_RESULT;
#endif
	if(SYS_IsTimerExpired(&gMMI_AudioBIST_ctl.waitTimer))
	{
		switch(gMMI_AudioBIST_ctl.mode)
		{
			case AUDIO_BIST_DISABLE:
				break;
				
			
		#ifdef FT_AUDIO_BIST
			case AUDIO_6M_R_INTERNAL_PATTERN_MODE:
				#ifdef LINEIN_ENABLE
				MMI_LineIn_DriverCmd(DRIVER_STOP_LINE_IN_CMD);
				#endif
			case AUDIO_6M_MUTE_PATTERN_MODE:
			case AUDIO_6M_L_INTERNAL_PATTERN_MODE:	
				OS_ENTER_MUTEX_CRITICAL();
				IC_BIST_SNR_RESULT = gAlignDSP.IC_BIST_SNR_RESULT;
				IC_BIST_SNDR_RESULT = gAlignDSP.IC_BIST_SNDR_RESULT;
				OS_EXIT_MUTEX_CRITICAL();
				MMI_DRV_AudioBistSetResult(ENDIAN_TRANSFORM_U16(IC_BIST_SNR_RESULT), ENDIAN_TRANSFORM_U16(IC_BIST_SNDR_RESULT));
				break;
			
			case VOICE_2M_R_INTERNAL_PATTERN_MODE:
			case VOICE_2M_MUTE_PATTERN_MODE:
			case VOICE_2M_L_INTERNAL_PATTERN_MODE:
				MMI_StopA2DPOrSCOCmd(DEFAULT_LINK_IND, FALSE);
				break;
		#else
			case AUDIO_6M_R_INTERNAL_PATTERN_MODE:			
			case AUDIO_6M_MUTE_PATTERN_MODE:
			case AUDIO_6M_L_INTERNAL_PATTERN_MODE:
				#ifdef LINEIN_ENABLE
				MMI_FCD_AudioBistControl();
				MMI_LineIn_DriverCmd(DRIVER_STOP_LINE_IN_CMD);
				#endif
				break;
			
			case VOICE_2M_R_INTERNAL_PATTERN_MODE:
			case VOICE_2M_MUTE_PATTERN_MODE:
			case VOICE_2M_L_INTERNAL_PATTERN_MODE:
			case VOICE_2M_L_REPORT_SINGLE_SNP_4000:
			case VOICE_2M_R_REPORT_SINGLE_SNP_4000:
			case VOICE_2M_L_REPORT_SINGLE_SNP_1000:
			case VOICE_2M_R_REPORT_SINGLE_SNP_1000:
			case VOICE_2M_L_REPORT_SINGLE_SNP_500:
			case VOICE_2M_R_REPORT_SINGLE_SNP_500:
			case VOICE_2M_L_REPORT_SINGLE_SNP_250:
			case VOICE_2M_R_REPORT_SINGLE_SNP_250:
				MMI_FCD_AudioBistControl();
				MMI_StopA2DPOrSCOCmd(DEFAULT_LINK_IND, FALSE);
				break;		
		#endif
		}
	}
}
/*
PRIVATE VOID MMI_DRV_AudioBistAudioSelToDefault(VOID)
{
	DriverAudioSelCmd LDATA audioSel;
	
	audioSel.isResetToDefault = TRUE;
	MMI_DRV_AudioSelCmd((DriverAudioSelCmd XDATA_PTR)&audioSel);
}*/

PUBLIC void MMI_DRV_AudioBistVendEvtHandler(VOID)
{
    HC_EVT_MSG_STRU_PTR pEvtMsg;
    typedef struct stru_this_hci_cmd_cpl_vevt
    {
        HCI_EVT_HDR_STRU Hdr;
        HCI_VEVT_OPCODE_STRU VEvtOpCode;
        HCI_VCMD_OPCODE_STRU VCmdOpcode;
        HCI_VCMD_RTN_PARA_AUDIO_BIST RtnPara;
    } THIS_HCI_VEVT_CMD_CPL_STRU;

    if ((pEvtMsg = HC_NewHciVCmdCplEvt(HCI_VCMDCODE_RF_BIST_AUDIO_BIST, sizeof(HCI_VCMD_RTN_PARA_AUDIO_BIST))) != NEAR_NULL)
    {
        THIS_HCI_VEVT_CMD_CPL_STRU XDATA_PTR pEvt = (THIS_HCI_VEVT_CMD_CPL_STRU XDATA_PTR)&pEvtMsg->HciEvt;
		OS_ENTER_MUTEX_CRITICAL();
        pEvt->RtnPara.SignalPower       = (U8)ENDIAN_TRANSFORM_U16(gAlignDSP.IC_BIST_SIGNSE_POWER);
        pEvt->RtnPara.SNDR              = (U8)ENDIAN_TRANSFORM_U16(gAlignDSP.IC_BIST_SNDR_VAL);
		OS_EXIT_MUTEX_CRITICAL();

        //MMI_DRV_AudioBistAudioSelToDefault();
		LightDbgPrint("AudioBISTEvt S:%x P:%x",(U8)pEvt->RtnPara.SNDR, (U8)pEvt->RtnPara.SignalPower);
        HC_SendEventToHost(pEvtMsg);
    }
}

PUBLIC void MMI_DRV_AudioBistSetResult(U16 SNRResult, U16 SNDRResult)
{
	if(gMMI_AudioBIST_ctl.mode)
	{
		if(gMMI_AudioBIST_ctl.waitTimer)
		{
			SYS_ReleaseTimer(&gMMI_AudioBIST_ctl.waitTimer);
			gMMI_AudioBIST_ctl.isProcessOver = TRUE;
			gMMI_AudioBIST_ctl.mode = AUDIO_BIST_DISABLE;
			gMMI_AudioBIST_ctl.isDSPEnabled = FALSE;
		}
		else if(gMMI_AudioBIST_ctl.isDSPEnabled)
		{
			gMMI_AudioBIST_ctl.SNRResult = SNRResult;
			gMMI_AudioBIST_ctl.SNDRResult = SNDRResult;
			gMMI_AudioBIST_ctl.isProcessOver = TRUE;
			if(!gMMI_AudioBIST_ctl.isFTTest)
			{
				if(gMMI_FCD_ctl.remoteAudioBist)
				{
					MMI_FCD_AudioBistResult();
				}
				else
				{
					MMI_DRV_AudioBistVendEvtHandler();
				}
				
				gMMI_FCD_ctl.remoteAudioBist = FALSE;
				gMMI_AudioBIST_ctl.mode = AUDIO_BIST_DISABLE;
				OS_ENTER_MUTEX_CRITICAL();
				gAlignDSP.IC_BIST_MODE = AUDIO_BIST_DISABLE;
				OS_EXIT_MUTEX_CRITICAL();
			}
			gMMI_AudioBIST_ctl.isDSPEnabled = FALSE;
		}
	}
}

#ifdef FT_AUDIO_BIST
enum audio_bist_result_enum
{
    AUDIO_BIST_FAIL = 0,
    AUDIO_BIST_SUCCESS,
};

PRIVATE void MMI_DRV_AudioBISTLineInFail(void)
{
#ifdef AB1122

#else
	#ifndef AB1520U
	DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 1, 15);
	#endif
#endif
}

PRIVATE void MMI_DRV_AudioBISTLineInPass(void)
{
#ifdef AB1122
	
#else
	#ifndef AB1520U
	DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 0, 15);
	#endif
#endif
}

PRIVATE void MMI_DRV_AudioBISTSCOFail(void)
{
#ifdef AB1520U
	DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 1, 3);
#else
	if(!gAlignDSP.IC_BIST_MEMORY_VERIFICATION)
		DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 1, 17);
		
	DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 1, 15);	
#endif
}

PRIVATE void MMI_DRV_AudioBISTSCOPass(void)
{
#ifdef AB1520U
	DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 0, 3);
#else
	if(!gAlignDSP.IC_BIST_MEMORY_VERIFICATION)
		DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 1, 17);
		
	DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 0, 15);
#endif	
}

enum
{
	AUDIO_FT_BIST_STATE_INIT,
	AUDIO_FT_BIST_STATE_SWITCH_TO_NEXT_MODE,
	AUDIO_FT_BIST_STATE_LINE_IN_START,
	AUDIO_FT_BIST_STATE_LINE_IN_WAITING_DSP_DISABLED,
	AUDIO_FT_BIST_STATE_SCO_START,
	AUDIO_FT_BIST_STATE_SCO_WAITING_DSP_DISABLED,	
	AUDIO_FT_BIST_STATE_ENDED,
};

extern PUBLIC void DRIVER_ProcMmiCmd(void);
extern PUBLIC void DRIVER_MailBoxHandler(void);
PUBLIC void MMI_DRV_AudioFTBIST(void)
{
	DRIVER_MailBoxHandler();
	switch(gMMI_AudioBIST_ctl.state)
	{
		case AUDIO_FT_BIST_STATE_INIT:
			LightDbgPrint("USR - FTBIST1");
			gMMI_AudioBIST_ctl.isFTTest = TRUE;
			gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_SWITCH_TO_NEXT_MODE;
			#ifdef AB1520U
			DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 0, 3);
			#else
			DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, 0, 17);
			#endif
			goto NEXT_NODE;
			break;
			
		case AUDIO_FT_BIST_STATE_SWITCH_TO_NEXT_MODE:
			NEXT_NODE:
			LightDbgPrint("USR - FTBIST Switch To NextMode");
			switch(gMMI_AudioBIST_ctl.mode)
			{
				case AUDIO_BIST_DISABLE:
					#ifdef AB1122
					gMMI_AudioBIST_ctl.mode = AUDIO_6M_R_INTERNAL_PATTERN_MODE;
					gMMI_AudioBIST_ctl.waitingTime = AUDIO_BIST_LINEIN_R_CHANNEL_WAIT_TIME;
					gMMI_AudioBIST_ctl.checkSNDR = TRUE;
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_LINE_IN_START;
					goto LINE_IN_START;	
					#elif defined AB1526 || defined AB1520U
					gMMI_AudioBIST_ctl.mode = VOICE_2M_MUTE_PATTERN_MODE;
					gMMI_AudioBIST_ctl.waitingTime = AUDIO_BIST_SCO_MUTE_WAIT_TIME;
					gMMI_AudioBIST_ctl.checkSNDR = FALSE;
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_SCO_START;
					goto SCO_START; 
					#else
					gMMI_AudioBIST_ctl.mode = AUDIO_6M_MUTE_PATTERN_MODE;
					gMMI_AudioBIST_ctl.waitingTime = AUDIO_BIST_LINEIN_MUTE_WAIT_TIME;
					gMMI_AudioBIST_ctl.checkSNDR = FALSE;
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_LINE_IN_START;
					goto LINE_IN_START;
					#endif
					break;
				case AUDIO_6M_MUTE_PATTERN_MODE:
					gMMI_AudioBIST_ctl.mode = AUDIO_6M_L_INTERNAL_PATTERN_MODE;
					gMMI_AudioBIST_ctl.waitingTime = AUDIO_BIST_LINEIN_L_CHANNEL_WAIT_TIME;
					gMMI_AudioBIST_ctl.checkSNDR = TRUE;
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_LINE_IN_START;
					goto LINE_IN_START;
					break;
				case AUDIO_6M_L_INTERNAL_PATTERN_MODE:
					gMMI_AudioBIST_ctl.mode = AUDIO_6M_R_INTERNAL_PATTERN_MODE;
					gMMI_AudioBIST_ctl.waitingTime = AUDIO_BIST_LINEIN_R_CHANNEL_WAIT_TIME;
					gMMI_AudioBIST_ctl.checkSNDR = TRUE;
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_LINE_IN_START;
					goto LINE_IN_START;
					break;
				case AUDIO_6M_R_INTERNAL_PATTERN_MODE:
					gMMI_AudioBIST_ctl.mode = VOICE_2M_MUTE_PATTERN_MODE;
					gMMI_AudioBIST_ctl.waitingTime = AUDIO_BIST_SCO_MUTE_WAIT_TIME;
					gMMI_AudioBIST_ctl.checkSNDR = FALSE;
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_SCO_START;
					goto SCO_START;
					break;
				case VOICE_2M_MUTE_PATTERN_MODE:
					gMMI_AudioBIST_ctl.mode = VOICE_2M_L_INTERNAL_PATTERN_MODE;
					gMMI_AudioBIST_ctl.waitingTime = AUDIO_BIST_SCO_L_CHANNEL_WAIT_TIME;
					gMMI_AudioBIST_ctl.checkSNDR = TRUE;
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_SCO_START;
					goto SCO_START;
					break;
				case VOICE_2M_L_INTERNAL_PATTERN_MODE:
					#ifdef AB1122
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_ENDED;
					#else
					gMMI_AudioBIST_ctl.mode = VOICE_2M_R_INTERNAL_PATTERN_MODE;
					gMMI_AudioBIST_ctl.waitingTime = AUDIO_BIST_SCO_R_CHANNEL_WAIT_TIME;
					gMMI_AudioBIST_ctl.checkSNDR = TRUE;
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_SCO_START;
					goto SCO_START;
					#endif
					break;
				case VOICE_2M_R_INTERNAL_PATTERN_MODE:
					gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_ENDED;
					break;
			}
			break;
			
		case AUDIO_FT_BIST_STATE_LINE_IN_START:
			LINE_IN_START:
			LightDbgPrint("USR - FTBIST LINE_IN_START:%d",(U8)gMMI_AudioBIST_ctl.mode);
			MMI_DRV_AudioBistEnableLineInCmd(gMMI_AudioBIST_ctl.mode);
			gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_LINE_IN_WAITING_DSP_DISABLED;
			break;
			
		case AUDIO_FT_BIST_STATE_LINE_IN_WAITING_DSP_DISABLED:
			if(gMMI_AudioBIST_ctl.isProcessOver)
			{
				U16 SNDR, POWER;
				OS_ENTER_MUTEX_CRITICAL();
				SNDR = gAlignDSP.IC_BIST_SNDR_VAL;
				POWER = gAlignDSP.IC_BIST_SIGNSE_POWER;
				OS_EXIT_MUTEX_CRITICAL();
				
				LightDbgPrint("USR - SNDR_VAL:%D", (S32)ENDIAN_TRANSFORM_U16(SNDR));
				LightDbgPrint("USR - SIGNSE_POWER:%D", (S32)ENDIAN_TRANSFORM_U16(POWER));

				if(gMMI_AudioBIST_ctl.mode == AUDIO_BIST_DISABLE || gMMI_AudioBIST_ctl.SNRResult != AUDIO_BIST_SUCCESS ||
					(gMMI_AudioBIST_ctl.checkSNDR && gMMI_AudioBIST_ctl.SNDRResult != AUDIO_BIST_SUCCESS))
				{
					LightDbgPrint("USR - FTBIST LINE_IN Fail:%d",(U8)gMMI_AudioBIST_ctl.mode);
                    MMI_DRV_AudioBISTLineInFail();
					gMMI_AudioBIST_ctl.isLineInFailed = TRUE;
				}
				else
				{
					LightDbgPrint("USR - FTBIST LINE_IN Pass:%d",(U8)gMMI_AudioBIST_ctl.mode);
					if(gMMI_AudioBIST_ctl.mode == AUDIO_6M_R_INTERNAL_PATTERN_MODE && !gMMI_AudioBIST_ctl.isLineInFailed)
					{	
						MMI_DRV_AudioBISTLineInPass();
					}
				}
				gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_SWITCH_TO_NEXT_MODE;
				goto NEXT_NODE;
			}		
			break;
			
		case AUDIO_FT_BIST_STATE_SCO_START:
			SCO_START:
			LightDbgPrint("USR - FTBIST VOICE Start:%d",(U8)gMMI_AudioBIST_ctl.mode);
			MMI_DRV_AudioBistEnableVoiceCmd(gMMI_AudioBIST_ctl.mode);
			gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_SCO_WAITING_DSP_DISABLED;
			break;
			
		case AUDIO_FT_BIST_STATE_SCO_WAITING_DSP_DISABLED:
			if(gMMI_AudioBIST_ctl.isProcessOver)
			{
				U16 SNDR, POWER;
				OS_ENTER_MUTEX_CRITICAL();
				SNDR = gAlignDSP.IC_BIST_SNDR_VAL;
				POWER = gAlignDSP.IC_BIST_SIGNSE_POWER;
				OS_EXIT_MUTEX_CRITICAL();
				
				LightDbgPrint("USR - SNDR_VAL:%D", (S32)ENDIAN_TRANSFORM_U16(SNDR));
				LightDbgPrint("USR - SIGNSE_POWER:%D", (S32)ENDIAN_TRANSFORM_U16(POWER));
				
				if(gMMI_AudioBIST_ctl.mode == AUDIO_BIST_DISABLE || gMMI_AudioBIST_ctl.SNRResult != AUDIO_BIST_SUCCESS ||
					(gMMI_AudioBIST_ctl.checkSNDR && gMMI_AudioBIST_ctl.SNDRResult != AUDIO_BIST_SUCCESS))
				{
					LightDbgPrint("USR - FTBIST VOICE Fail:%d",(U8)gMMI_AudioBIST_ctl.mode);
					#ifdef AB1122
					if(gMMI_AudioBIST_ctl.mode == VOICE_2M_L_INTERNAL_PATTERN_MODE)
					{
						MMI_DRV_AudioBISTSCOFail();
					}
					#else
                    MMI_DRV_AudioBISTSCOFail();
					#endif
					gMMI_AudioBIST_ctl.isSCOFailed = TRUE;
				}
				else
				{
					LightDbgPrint("USR - FTBIST VOICE Pass:%d",(U8)gMMI_AudioBIST_ctl.mode);
					#ifdef AB1122
					if(gMMI_AudioBIST_ctl.mode == VOICE_2M_L_INTERNAL_PATTERN_MODE && !gMMI_AudioBIST_ctl.isSCOFailed)
					{	
						MMI_DRV_AudioBISTSCOPass();
					}					
					#else
					if(gMMI_AudioBIST_ctl.mode == VOICE_2M_R_INTERNAL_PATTERN_MODE && !gMMI_AudioBIST_ctl.isSCOFailed)
					{	
						MMI_DRV_AudioBISTSCOPass();
					}
					#endif
				}
				gMMI_AudioBIST_ctl.state = AUDIO_FT_BIST_STATE_SWITCH_TO_NEXT_MODE;
				goto NEXT_NODE;				
			}
			break;
			
		case AUDIO_FT_BIST_STATE_ENDED:
		    ASSERT(FALSE);
			break;
	}
	DRIVER_ProcMmiCmd();
}
#endif
