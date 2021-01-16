//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define AUDIOCONTROL1500_C
#include "os_flash.h"
#include "bt_config_profile.h"
#include "audiocontrol.h"
#include "drivers.h"
#include "../MMI/MMI.h"
#include "align_flash.h"
#ifdef PEQ_ENABLE
#include "Peq_HPF.h"
#endif

#include "../LC/LC.h"
PRIVATE void AUDIO_SPDIFEnable(void);
PRIVATE void AUDIO_SPDIFDisable(void);
PRIVATE void AUDIO_SetDSPOff(U8 component);


U8 XDATA SPDIF_CONSUMER_CH_STATUS[] =
{
	//- x	
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,	
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	
	//- y	
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,	
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};


PUBLIC void Audio1500_Init(void)
{
	//SFR:59
	gAUDIO_CtrlInfo.micGain = gSYS_audio_ctl.audio_C2_59 ;
	SET_REG_MIC_GAIN(gAUDIO_CtrlInfo.micGain);
	
	//SFR:60
	gAUDIO_CtrlInfo.voiceGain = gSYS_audio_ctl.audio_C2_60; //min voice level -21dB
	SET_REG_VOICE_GAIN(gAUDIO_CtrlInfo.voiceGain);
	
	//SFR:58
	gAUDIO_CtrlInfo.audSel = gSYS_audio_ctl.audio_C2_58;
	SET_AUDIO_SEL(gAUDIO_CtrlInfo);
	
	//SFR:63
	gAUDIO_CtrlInfo.analogDaGain = gSYS_audio_ctl.audio_C2_63;
}

PUBLIC void AUDIO_3WireMuteAudio(BOOL isMute)
{
	U8 valueB1, valueB0;
	OST XDATA_PTR daadWaitingTimer;

	if(gAUDIO_CtrlInfo.is3WiredAudioMute == isMute)
		return;

	valueB1 = gAUDIO_CtrlInfo.RF3wire0x49Byte1;
	valueB0 = gAUDIO_CtrlInfo.RF3wire0x49Byte0;


	if(isMute)
	{
		valueB0 |= 0x06;
		gAUDIO_CtrlInfo.is3WiredAudioMute = TRUE;
		SYS_SetTimer(&gAUDIO_CtrlInfo.threeWireMuteTimer, (U32) gMMI_nvram.timerCtl.threeWireMuteTimer*32L);
	}
	else
	{
		valueB0 &= (~0x06);
		gAUDIO_CtrlInfo.is3WiredAudioMute = FALSE;
		if (gAUDIO_CtrlInfo.threeWireMuteTimer)
		{
			while(!SYS_IsTimerExpired(&gAUDIO_CtrlInfo.threeWireMuteTimer));	//avoid pop noise
		}
	}
	SYS_3WireWrite(0x49,valueB1,valueB0);

	daadWaitingTimer = OST_GetTimer();
	OST_SetTimer(daadWaitingTimer, 8L);
	while(!OST_TimerExpired(daadWaitingTimer));
	OST_PutTimer(daadWaitingTimer);

}

PUBLIC BOOL AUDIO_SetDSPMode(U8 component)
{
	if(AUDIO_ENABLED(component))
	{
		AUDIO_SetDSPModeSelect(component);
		return TRUE;
	}
	
	switch(component)
	{
		case AUDIO_LINE_IN:
			if(((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->LineInModeSel == LINE_IN_DSP_MODE || gAlignDSP.DSPOUT_MUTE == 1)
			{
				AUDIO_SetAudioMode(LINE_IN_DSP_MODE, TRUE);
			}
			else if(((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->LineInModeSel == LINE_IN_BYPASS_MODE)
			{
				AUDIO_SetAudioMode(LINE_IN_BYPASS_MODE, TRUE);
			}		
			break;
		case AUDIO_RINGTONE:
		case AUDIO_VOICEPROMPT:
		case AUDIO_VOICECOMMAND:
			if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
			{
				if(((DRIVER_CTL_TYPE CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_CTL_INIT)))->LineInModeSel == LINE_IN_BYPASS_MODE)
				{
					AUDIO_SetAudioMode(LINE_IN_DSP_MODE, TRUE);
				}
			}
			break;
		case AUDIO_A2DP:
			AUDIO_SetA2DPMode(A2DP_ON_DA);
			break;
		case AUDIO_SCO:
			AUDIO_SetSCOMode(TRUE);
			break;
		default: 
			break;
	}
	return FALSE;
}

//A mechanism to disable DA & AD when all related devices do not enable DA & AD.
PUBLIC void AUDIO_SetAudioOutEnabled(U8 enableAudio, U8 component)
{
	U8 mask;
	U8 originalgAUDIO_audioEnabledOut = gAUDIO_audioOutMask;
	U8 audio_enable;

	//Error Checking, value of device should be less than DAAD_DEVICE_NO
	ASSERT(component < AUDIO_COMPONENT_NO);

	mask = U8_BIT_MASK(component);
	if(enableAudio)
	{
		gAUDIO_audioOutMask |= mask;
	}
	else
	{
		gAUDIO_audioOutMask &= ~mask;
	}

	if(!originalgAUDIO_audioEnabledOut && gAUDIO_audioOutMask)
	{
LABLE_AUDIO_ENABLE:
		audio_enable = RC_AUDIO_ENABLE_IN_EEPROM;
		if((gAlignDSP.DSP_nvram.nb_voice_parameter.mic_LR_select & 0x02)||(gAlignDSP.DSP_nvram.lineinout_parameter.LINEINOUT_SEL&0x10))	 //- L mic
		{
			audio_enable |= 0x20;
		}

		if (component == AUDIO_LINE_IN)
		{
			audio_enable |= 0x10;
		}

 		//SFR:57
		SET_AUDIO_ENABLE(audio_enable);

		gSYS_audio_ctl.isAudioEnabled = TRUE;
	}
	else if(originalgAUDIO_audioEnabledOut && !gAUDIO_audioOutMask)
	{
	 	//SFR:57
		SET_AUDIO_ENABLE(RC_AUDIO_DISABLE_IN_EEPROM);

		gSYS_audio_ctl.isAudioEnabled = FALSE;
	}
	else if (component == AUDIO_LINE_IN && gAUDIO_audioOutMask)
	{
		if(enableAudio)
		{
			goto LABLE_AUDIO_ENABLE;
		}
		else
		{
			OS_ENTER_CRITICAL();
			audio_enable = LC_SFR_READ_BY_C2(C2OFFS_RW_AUDIO_ENABLE);
			OS_EXIT_CRITICAL();
			audio_enable&= 0xEF;
			SET_AUDIO_ENABLE(audio_enable);
		}
	}
}

PUBLIC void AUDIO_EnableADDAWhenOtherAudioExist(U8 enabledMask, U8 component)
{
	//If i < AUDIO_COMPONENT_NO, it means there are other components
	//need audio being on. So, just set the bit represented this compoent
	//in the mask of DAAD and Audio_Enable to 0.
	AUDIO_SetComponentDAADEnabled(enabledMask, component);
	AUDIO_SetAudioOutEnabled(TRUE, component);
}

PUBLIC void AUDIO_EnableADDAHandler(U8 enabledMask, U8 component)
{
	OST XDATA_PTR daadWaitingTimer;

	OS_SetPowerSavingLevel(OS_POWERLEVEL_MCU_PAUSE);

	if(!gAUDIO_CtrlInfo.is3WiredAudioMute)
		AUDIO_3WireMuteAudio(TRUE);
	//If i == AUDIO_COMPONENT_NO, it means only this component may have turned
	//Audio on, depending on gAUDIO_daadEnabledMasks[component].
	//If gAUDIO_daadEnabledMasks[component] != 0x00, then no need to do anything

	//SFR:60 Digital Speaker Gain (Soft Mute)
	SET_REG_VOICE_GAIN(0);

	//SFR:63 Analog Speaker Gain (Soft Mute)
	SET_REG_DA_ANALOG_GAIN(0);

	//SFR:57
	AUDIO_SetAudioOutEnabled(TRUE, component);

	//SFR:62 : bit1, bit2
	AUDIO_SetComponentDAADEnabled(0x06, component);


	//Get timer
	daadWaitingTimer = OST_GetTimer();

	//Next, wait for at last 6ms
	OST_SetTimer(daadWaitingTimer, 24L);//24*312.5 = 7.5 ms
	while(!OST_TimerExpired(daadWaitingTimer));

	//SFR:63 Analog Speaker Gain (Set to current volume)
	SET_REG_DA_ANALOG_GAIN(gAUDIO_CtrlInfo.analogDaGain);

	//SFR:60 Digital Speaker Gain (Set to current volume)
	SET_REG_VOICE_GAIN(gAUDIO_CtrlInfo.voiceGain);

	//wait for at last 1ms
	OST_SetTimer(daadWaitingTimer, 4);
	while(!OST_TimerExpired(daadWaitingTimer));

	//SFR:62 : bit0
	AUDIO_SetComponentDAADEnabled(enabledMask, component);


	OST_PutTimer(daadWaitingTimer);

	AUDIO_SetComponentDAADEnabled(AUDIO_ENABLE_DA, AUDIO_PURE_ON);
	AUDIO_SetAudioOutEnabled(TRUE, AUDIO_PURE_ON);

	//open AMP
	if (gMMI_nvram.timerCtl.openAudioAmpDelayTime && AUDIO_CheckIfAmpAllowedOpen(component) && !AUDIO_POWER_ON_RESET_ADDA(enabledMask, component))
		AUDIO_Open_AMP();
}

PUBLIC void AUDIO_SetDSPOn (U8 component)
{
	#ifdef USING_AD_DA
	U8 mask;
	#endif

	if ((component == AUDIO_A2DP) || (component == AUDIO_LINE_IN))
	{
		HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_CKSEL, (gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel>>4));
		#ifdef USING_AD_DA

		if(component == AUDIO_A2DP)
		{
			LC_SFR_WRITE_BY_C2(C2OFFS_RW_DIG_AUDIO_CLKSEL,  gSYS_audio_ctl.audio_C2_66  | (0x08));
		}
		else
		{
			LC_SFR_WRITE_BY_C2(C2OFFS_RW_DIG_AUDIO_CLKSEL,  gSYS_audio_ctl.audio_C2_66 | (0x1b));
		}
		#endif


		if(gAlignDSP.DSP_nvram.mcu_parameter.spdif_enable)
		{
			AUDIO_SPDIFEnable();
		}
	}
	else if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_CKSEL, (gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel&0xf));
		#ifdef USING_AD_DA
		if((component == AUDIO_SCO))
		{
			LC_SFR_WRITE_BY_C2(C2OFFS_RW_DIG_AUDIO_CLKSEL,  gSYS_audio_ctl.audio_C2_66 &(~0x08) );
		}
		#endif
	}


	if (!Enable_DSP)
	{
		if (component == AUDIO_A2DP)
		{
			AUDIO_SetDSPModeSelect(AUDIO_A2DP);
			gAUDIO_CtrlInfo.dspSel = DSP_SEL_A2DP;

			if(gAlignDSP.DSP_nvram.mcu_parameter.zsp_enable)
			{
				if(gAlignDSP.DSP_nvram.uart_parameters.UART_INBUFSIZE  && gAlignDSP.DSP_nvram.uart_parameters.UART_MASK)
				{
					//enable DSP interrupt
					DSP_EnableDSPInterruptForAudio();
				}

				#ifdef PEQ_ENABLE
				AUDIO_LoadPEQConfigData();
				#endif
				gAlign.MEDIA_CLOSE = 0;
				gAlign.MEDIA_CLOSE_READY = 0;
				OS_ENTER_CRITICAL();
				DSP_Enable(gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel);
				OS_EXIT_CRITICAL();

				while(gAlign.AUDIO_MODE_SEL & AUDIO_MODE_BUSY_FLAG);

				OS_ENTER_CRITICAL();
				#ifdef OPERATION_IN_BB_FPGA_VERSION
					LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_DAC_GC_SEL, 0x9);  //FPGA only support DAC_GC value 0x09
				#endif

				HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_A2DP]);	 //stereo:2
				#ifdef USING_AD_DA
					mask = LC_SFR_READ_BY_C2 (C2OFFS_RW_AUDIO_SEL);
					LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_SEL, mask & (~0x80));
				#endif
				Enable_DSP = TRUE;
				OS_EXIT_CRITICAL();
			}
		}
		else if (component == AUDIO_SCO)
		{
			gOS_PowerSavingMCUPauseRequest = TRUE;
			AUDIO_SetDSPModeSelect(AUDIO_SCO);
			gAUDIO_CtrlInfo.dspSel = DSP_SEL_SCO;
			if(gAlignDSP.DSP_nvram.mcu_parameter.zsp_enable)
			{
				if(gAlignDSP.DSP_nvram.uart_parameters.UART_INBUFSIZE  && gAlignDSP.DSP_nvram.uart_parameters.UART_MASK)
				{
					//enable DSP interrupt
					DSP_EnableDSPInterruptForAudio();
				}

				OS_ENTER_CRITICAL();
				DSP_Enable(gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel);
				OS_EXIT_CRITICAL();
				while(gAlign.AUDIO_MODE_SEL & AUDIO_MODE_BUSY_FLAG);

				OS_ENTER_CRITICAL();
				LC_SFR_WRITE_BY_C2(C2OFFS_RW_RX_EVLENGTH_H, 0x0); //enable clock drift
				if(gAlignDSP.DSP_nvram.nb_voice_parameter.mic_LR_select & 0x01)
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_SCO]);	//EC_NR
				}
				else   //- R mic disable
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_SCO]&(~0x08));	//EC_NR
				}

				if(gAlignDSP.DSP_nvram.nb_voice_parameter.mic_LR_select & 0x02)
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL2, 0x02);
				}

				#ifdef USING_AD_DA
					mask = LC_SFR_READ_BY_C2 (C2OFFS_RW_AUDIO_SEL);
					LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_SEL, mask | (0x80));
				#endif
				Enable_DSP = TRUE;
				OS_EXIT_CRITICAL();
			}
		}
		else if (component == AUDIO_VOICEPROMPT || component == AUDIO_RINGTONE)
		{
			if(gAlignDSP.DSP_nvram.mcu_parameter.zsp_enable)
			{
				//enable DSP interrupt
				DSP_EnableDSPInterruptForAudio();

			//	AUDIO_LoadPEQConfigData();
				OS_ENTER_CRITICAL();
				DSP_Enable(gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel);
				HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_VP]);
				Enable_DSP = TRUE;
				OS_EXIT_CRITICAL();
			}
		}
		else if (component == AUDIO_LINE_IN)
		{
			AUDIO_SetDSPModeSelect(AUDIO_LINE_IN);
			gAUDIO_CtrlInfo.dspSel = DSP_SEL_LINE_IN;
			if(gAlignDSP.DSP_nvram.mcu_parameter.zsp_enable)
			{
				if(gAlignDSP.DSP_nvram.uart_parameters.UART_INBUFSIZE  && gAlignDSP.DSP_nvram.uart_parameters.UART_MASK)
				{
				    //enable DSP interrupt
				    DSP_EnableDSPInterruptForAudio();
				}

				#ifdef PEQ_ENABLE
				AUDIO_LoadPEQConfigData();
				#endif

				OS_ENTER_CRITICAL();
				DSP_Enable(gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel);
				OS_EXIT_CRITICAL();

				while(gAlign.AUDIO_MODE_SEL & AUDIO_MODE_BUSY_FLAG);

				OS_ENTER_CRITICAL();
				LC_SFR_WRITE_BY_C2(C2OFFS_RW_RX_EVLENGTH_H, 0x30); //disable clock drift
				if(gAlignDSP.DSP_nvram.lineinout_parameter.LINEINOUT_SEL & 0x01)
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_LINE_IN]);
				}
				else   //- R mic disable
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_LINE_IN]&(~0x08));
				}
 				if(gAlignDSP.DSP_nvram.lineinout_parameter.LINEINOUT_SEL & 0x10)
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL2, 0x03);
				}
				else
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL2, 0x01);
				}


				#ifdef USING_AD_DA
					mask = LC_SFR_READ_BY_C2 (C2OFFS_RW_AUDIO_SEL);
					LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_SEL, mask | (0x80));
				//	LC_SFR_WRITE_BY_C2(C2OFFS_RW_DIG_AUDIO_CLKSEL,  gSYS_audio_ctl.audio_C2_66 | 0x13);
				#endif
				Enable_DSP = TRUE;
				OS_EXIT_CRITICAL();

				OS_delay_ms(20);  //17 is ok
			}
		}
	}
	else
	{
	 	if (component == AUDIO_A2DP)
		{
			if(gAlignDSP.DSP_nvram.mcu_parameter.zsp_enable)
			{
				#ifdef PEQ_ENABLE
				AUDIO_LoadPEQConfigData();
				#endif

				gAlign.MEDIA_CLOSE = 0;
				gAlign.MEDIA_CLOSE_READY = 0;

				AUDIO_SetDSPModeSelect(AUDIO_A2DP);
				gAUDIO_CtrlInfo.dspSel = DSP_SEL_A2DP;

				while(gAlign.AUDIO_MODE_SEL & AUDIO_MODE_BUSY_FLAG);

				OS_ENTER_CRITICAL();
				#ifdef OPERATION_IN_BB_FPGA_VERSION
					LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_DAC_GC_SEL, 0x9);  //FPGA only support DAC_GC value 0x09
				#endif

				HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_A2DP]);	 //stereo:2
				#ifdef USING_AD_DA
					mask = LC_SFR_READ_BY_C2 (C2OFFS_RW_AUDIO_SEL);
					LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_SEL, mask & (~0x80));
				#endif
				OS_EXIT_CRITICAL();
			}
		}
		else if (component == AUDIO_SCO)
		{
			gOS_PowerSavingMCUPauseRequest = TRUE;
			if(gAlignDSP.DSP_nvram.uart_parameters.UART_INBUFSIZE && gAlignDSP.DSP_nvram.uart_parameters.UART_MASK)
			{
				//enable DSP interrupt
				DSP_EnableDSPInterruptForAudio();
			}

			if(gAlignDSP.DSP_nvram.mcu_parameter.zsp_enable)
			{

				AUDIO_SetDSPModeSelect(AUDIO_SCO);
				gAUDIO_CtrlInfo.dspSel = DSP_SEL_SCO;
				while(gAlign.AUDIO_MODE_SEL & AUDIO_MODE_BUSY_FLAG);

				OS_ENTER_CRITICAL();
				LC_SFR_WRITE_BY_C2(C2OFFS_RW_RX_EVLENGTH_H, 0x0); //enable clock drift
				if(gAlignDSP.DSP_nvram.nb_voice_parameter.mic_LR_select & 0x01)
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_SCO]);	//EC_NR
				}
				else   //- R mic disable
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_SCO]&(~0x08));	//EC_NR
				}

				if(gAlignDSP.DSP_nvram.nb_voice_parameter.mic_LR_select & 0x02)
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL2, 0x02);
				}

				#ifdef USING_AD_DA
					mask = LC_SFR_READ_BY_C2 (C2OFFS_RW_AUDIO_SEL);
					LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_SEL, mask | (0x80));
				#endif
				OS_EXIT_CRITICAL();
			}
		}
		else if (component == AUDIO_LINE_IN)
		{

			if(gAlignDSP.DSP_nvram.mcu_parameter.zsp_enable)
			{
				#ifdef PEQ_ENABLE
				AUDIO_LoadPEQConfigData();
				#endif

				AUDIO_SetDSPModeSelect(AUDIO_LINE_IN);
				gAUDIO_CtrlInfo.dspSel = DSP_SEL_LINE_IN;
				while(gAlign.AUDIO_MODE_SEL & AUDIO_MODE_BUSY_FLAG);

				OS_ENTER_CRITICAL();
				LC_SFR_WRITE_BY_C2(C2OFFS_RW_RX_EVLENGTH_H, 0x30); //disable clock drift
				if(gAlignDSP.DSP_nvram.lineinout_parameter.LINEINOUT_SEL & 0x01)
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_LINE_IN]);
				}
				else   //- R mic disable
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, gAlignDSP.DSP_nvram.mcu_parameter.dsp_audio_ctrl[AUDIOCTL_LINE_IN]&(~0x08));
				}
				if(gAlignDSP.DSP_nvram.lineinout_parameter.LINEINOUT_SEL & 0x10)
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL2, 0x03);
				}
				else
				{
					HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL2, 0x01);
				}

				#ifdef USING_AD_DA
					mask = LC_SFR_READ_BY_C2 (C2OFFS_RW_AUDIO_SEL);
					LC_SFR_WRITE_BY_C2(C2OFFS_RW_AUDIO_SEL, mask | (0x80));
					//LC_SFR_WRITE_BY_C2(C2OFFS_RW_DIG_AUDIO_CLKSEL,  gSYS_audio_ctl.audio_C2_66 | 0x13);
				#endif
				OS_EXIT_CRITICAL();
			}
		}
		else if (component == AUDIO_VOICEPROMPT)
		{
			//enable DSP interrupt
			DSP_EnableDSPInterruptForAudio();
		}
	}
/*	if ((gAlign.AUDIO_MODE_SEL == AUDIO_MODE_STEREO) || (gAlign.AUDIO_MODE_SEL == AUDIO_MODE_LINEIN))
	{
        #ifdef USING_AD_DA
		mask = LC_SFR_READ_BY_C2 (C2OFFS_RW_DIG_AUDIO_CLKSEL);
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_DIG_AUDIO_CLKSEL, mask | (0x08));
		#endif
		HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_CKSEL, (gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel>>4));
		if(gAlignDSP.DSP_nvram.mcu_parameter.spdif_enable)
		{
			AUDIO_SPDIFEnable();
		}
	}
	else if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_CKSEL, (gAlignDSP.DSP_nvram.mcu_parameter.dsp_cksel&0xf));
	}		*/

}

PUBLIC void AUDIO_SetADDAandDSPOn(U8 enabledMask, U8 component)
{
	if( AUDIO_COMPONENT_NO > AUDIO_CheckOtherAudioEnabled(component))
	{
		AUDIO_EnableADDAWhenOtherAudioExist(enabledMask, component);
		if (AUDIO_CheckIfAmpAllowedOpen(component))
		{
			AUDIO_Open_AMP();
		}
	}
	else if(!AUDIO_ENABLED(component))
	{
		AUDIO_EnableADDAHandler(enabledMask, component);
	}

	AUDIO_SetDSPOn(component);

	SYS_ReleaseTimer(&gAUDIO_CtrlInfo.delayForCloseAudiotimer);

	if (!gMMI_nvram.timerCtl.openAudioAmpDelayTime && AUDIO_CheckIfAmpAllowedOpen(component)&& !AUDIO_POWER_ON_RESET_ADDA(enabledMask, component))
		AUDIO_Open_AMP();

}

PUBLIC void AUDIO_SetADDAandDSPOff(U8 component)
{
	//First, check if any component requires audio to be on.
	//If not, then do audio off process.
	U8 i;

	i = AUDIO_CheckOtherAudioEnabled(component);

	if(i < AUDIO_COMPONENT_NO)
	{
		AUDIO_DisableADDAWhenOtherAudioExist(component);
	}
	else if(AUDIO_ENABLED(component))
	{
		AUDIO_DisableADDA(component);
	}

	// For AMP delay off
	i = AUDIO_CheckAnyAudioEnabled();

	if(i == AUDIO_PURE_ON)
	{
		if(gMMI_nvram.timerCtl.delayforCloseAudioTimer)
			SYS_SetTimer(&gAUDIO_CtrlInfo.delayForCloseAudiotimer, (U32) gMMI_nvram.timerCtl.delayforCloseAudioTimer * 320L);
		
		if(!gAUDIO_CtrlInfo.is3WiredAudioMute)
			AUDIO_3WireMuteAudio(TRUE);
	}

	if(component != AUDIO_RINGTONE)
	{
		AUDIO_SetDSPOff(component);
	}
}

PUBLIC void AUDIO_DisableADDAWhenOtherAudioExist(U8 component)
{
	//If i < AUDIO_COMPONENT_NO, it means there are other components
	//need audio being on. So, just set the bit represented this compoent
	//in the mask of DAAD and Audio_Enable to 0.
	AUDIO_SetComponentDAADEnabled(0x00, component);
	AUDIO_SetAudioOutEnabled(FALSE, component);
}

PUBLIC void AUDIO_DisableADDA(U8 component)
{
	OST XDATA_PTR daadWaitingTimer;

	//If i == AUDIO_COMPONENT_NO, it means only this component may have turned
	//Audio on, depending on gAUDIO_daadEnabledMasks[component].
	//If gAUDIO_daadEnabledMasks[component] == 0x00, then no need to do anything

	//close AMP  // Now when FM is enabled, DS is also enabled.
	//if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	AUDIO_Close_AMP();

	//Get timer
	daadWaitingTimer = OST_GetTimer();

	//SFR:60 Digital Speaker Gain (Soft Mute)
	SET_REG_VOICE_GAIN(0);

	//SFR:63 Analog Speaker Gain (Soft Mute)
	SET_REG_DA_ANALOG_GAIN(0);


	if (AUDIO_ENABLED(component))
	{
		//SFR:62 : bit1, bit2
		AUDIO_SetComponentDAADEnabled(0x02, component);
	}

	//Next, wait for at last 8ms
	OST_SetTimer(daadWaitingTimer, 30L);
	while(!OST_TimerExpired(daadWaitingTimer));

	//SFR:62
	AUDIO_SetComponentDAADEnabled(0x00, component);


	//Next, wait for at last 1ms
	OST_SetTimer(daadWaitingTimer, 4L);
	while(!OST_TimerExpired(daadWaitingTimer));

	//SFR:57
	AUDIO_SetAudioOutEnabled(FALSE, component);

	//SFR:63 Analog Speaker Gain (Set to current volume)
	SET_REG_DA_ANALOG_GAIN(gAUDIO_CtrlInfo.analogDaGain);

	//SFR:60 Digital Speaker Gain (Set to current volume)
	SET_REG_VOICE_GAIN(gAUDIO_CtrlInfo.voiceGain);

	OST_PutTimer (daadWaitingTimer);

	OS_SetPowerSavingLevel(OS_POWERLEVEL_SLEEP_MCU_PAUSE);
}

PUBLIC void AUDIO_DisableDSP(void)
{
	if (Enable_DSP)
	{
		AUDIO_MODE_SELECT(0);

		if(gAlignDSP.DSP_nvram.mcu_parameter.zsp_enable)
		{
			gAlign.DSP_CLOSE_NOTIFY = 1;
			while(gAlign.DSP_CLOSE_NOTIFY);

			DSP_DisableDSPInterruptForAudio();
			OS_ENTER_CRITICAL();
			DSP_Disable();
			HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, 0x10);
			HC_SFR_CONT_WRITE_BY_FB(0);//FBOFFS_WO_DSP_AUDIO_CTL2
			LC_SFR_WRITE_BY_C2(C2OFFS_RW_DIG_AUDIO_CLKSEL,  gSYS_audio_ctl.audio_C2_66);
			LC_SFR_WRITE_BY_C2(C2OFFS_RW_RX_EVLENGTH_H, 0x30);
			OS_EXIT_CRITICAL();

			gAlign.MEDIA_CLOSE = 0;
			gAlign.MEDIA_CLOSE_READY = 0;

			Enable_DSP = FALSE;
		}
		#ifdef PEQ_ENABLE
		PEQ_ReleaseResource();
		#endif
		gAUDIO_CtrlInfo.dspSel = DSP_SEL_NONE;
	}
}

PRIVATE BOOL AUDIO_CheckOtherAudioComponentActive(U8 component, U8 targetComponent);

PRIVATE void AUDIO_SetDSPOff(U8 component)
{
	if (component == AUDIO_SCO)
	{
		gOS_PowerSavingMCUPauseRequest = FALSE;
	}

	if(AUDIO_CheckOtherAudioComponentActive(component, AUDIO_SCO))
		return;

	if(AUDIO_CheckOtherAudioComponentActive(component, AUDIO_A2DP))
		return;

	if(AUDIO_CheckOtherAudioComponentActive(component, AUDIO_LINE_IN))
		return;

	AUDIO_MODE_SELECT(0);

	if(AUDIO_CheckOtherAudioComponentActive(component, AUDIO_VOICEPROMPT))
		return;
	
	if(gAlignDSP.DSP_nvram.mcu_parameter.spdif_enable)
	{
		AUDIO_SPDIFDisable();
	}

	if(AUDIO_CheckOtherAudioComponentActive(component, AUDIO_RINGTONE))
		return;

	#ifdef SUPPORT_VOICE_COMMAND
	if(AUDIO_CheckOtherAudioComponentActive(component, AUDIO_VOICECOMMAND))
		return;
	#endif

	if (gAlignDSP.DSP_nvram.mcu_parameter.DSP_always_on)
		return;

	AUDIO_DisableDSP();
}

#ifdef OPERATION_IN_BB_ASIC_VERSION
PUBLIC BOOL AUDIO_SetAD(BOOL isEnable)
{
	if(gAUDIO_CtrlInfo.isADEnabled == isEnable)
		return FALSE;
		
	gAUDIO_CtrlInfo.isADEnabled = isEnable;
	
	return TRUE;
}

PUBLIC BOOL AUDIO_SetDA(BOOL isEnable)
{
	if(gAUDIO_CtrlInfo.isDAEnabled == isEnable)
		return FALSE;
		
	gAUDIO_CtrlInfo.isDAEnabled = isEnable;
	
	return TRUE;
}
#endif

PRIVATE BOOL AUDIO_CheckOtherAudioComponentActive(U8 component, U8 targetComponent)
{
	UNUSED(component);

	if (AUDIO_COMPONENT_IS_ACTIVE(targetComponent))
	{
		return TRUE;
	}
	return FALSE;
}

PUBLIC void AUDIO_SetAudioMode(U8 mode, BOOL isOn)
{
	U8 i;
	LINEIN_RF_3_WIRED_TYPE XDATA_PTR LDATA ptr1;
	U8 LDATA daadEnabledMasks[AUDIO_COMPONENT_NO];

	if(gAUDIO_CtrlInfo.dspAudioModeSel == mode)
		return;

	switch(mode)
	{
		case LINE_IN_DSP_MODE:
			ptr1 = gAlignDSP.DSP_nvram.mcu_parameter.LineInDSPMode;
			break;
		case LINE_IN_BYPASS_MODE:
			ptr1 = gAlignDSP.DSP_nvram.mcu_parameter.LineInBypassMode;
			break;
		case LINE_IN_EXIT_MODE:
			ptr1 = gAlignDSP.DSP_nvram.mcu_parameter.LineInExit;
			break;
		default:
			return;
	}

	if(!isOn  && !gAUDIO_CtrlInfo.is3WiredAudioMute)
		isOn = FALSE;
	else
		isOn = TRUE;

	OSMEM_memset_xdata((U8 XDATA_PTR)daadEnabledMasks, 0, AUDIO_COMPONENT_NO);

	if(mode == LINE_IN_BYPASS_MODE || gAUDIO_CtrlInfo.dspAudioModeSel == LINE_IN_BYPASS_MODE)
	{
		for(i =0; i < AUDIO_COMPONENT_NO; i++)
		{
			if(AUDIO_COMPONENT_IS_ACTIVE(i))
			{
				daadEnabledMasks[i] = gAUDIO_daadEnabledMasks[i];
				//close DSP before 3wire write to avoid pop noise
				AUDIO_SetADDAandDSPOff(i);
			}
		}
	}

	gAUDIO_CtrlInfo.dspAudioModeSel = mode;

	AUDIO_3WireMuteAudio(TRUE);

	for(i = 0; i < RF_3WIRE_NO; i++)
	{
		SYS_3WireWrite(ptr1->B2,ptr1->B1,ptr1->B0);
		ptr1++;
	}

	for(i = 0; i < AUDIO_PURE_ON; i++)
	{
		if(daadEnabledMasks[i])
		{
			//reopen DSP
			AUDIO_SetADDAandDSPOn(daadEnabledMasks[i], i);
		}
	}
	AUDIO_3WireMuteAudio(isOn);
}

PUBLIC void AUDIO_SetA2DPMode(U8 isOn)
{
	UNUSED(isOn);
}

PUBLIC void AUDIO_SetSCOMode(BOOL isOn)
{
	UNUSED(isOn);
}

PUBLIC void AUDIO_MONOTRxReset(void)
{
	LC_RESET_AUDIO_RX_FIFO();
	LC_ENABLE_AUDIO_RX_DMA();
	LC_RESET_AUDIO_TX_FIFO();
	LC_ENABLE_AUDIO_TX_DMA();
}

PUBLIC void AUDIO_SetDSPModeSelect(U8 component)
{
	if (component == AUDIO_A2DP)
	{
		AUDIO_MODE_SELECT(AUDIO_MODE_STEREO|AUDIO_MODE_BUSY_FLAG);
	}
	else if(component == AUDIO_SCO)
	{
		AUDIO_MODE_SELECT(AUDIO_MODE_MONO|AUDIO_MODE_BUSY_FLAG);
	}
	else if(component == AUDIO_LINE_IN)
	{
		AUDIO_MODE_SELECT(AUDIO_MODE_LINEIN|AUDIO_MODE_BUSY_FLAG);
	}
}

void AUDIO_SetAnalogGains(U8 analogDaGain, U8 analogAdGain)
{
	//DAAD gain settings
	if(analogDaGain != VOLUME_DONT_CARE)
	{
		gAUDIO_CtrlInfo.analogDaGain = analogDaGain;
		SET_REG_DA_ANALOG_GAIN(analogDaGain);
	}

	if(analogAdGain != VOLUME_DONT_CARE)
	{
		gAUDIO_CtrlInfo.analogAdGain = analogAdGain;
		SET_REG_AD_ANALOG_GAIN(analogAdGain);
	}
}

void AUDIO_SetSpeakerDigitalGain(U8 cmd, U8 level)
{
	switch(cmd)
	{
		//AUDIO_VOLUP
		case VOICE_VOLUP:
			if ((gAUDIO_CtrlInfo.voiceGain & 0x0F) < SPEAKER_MAX_VOL)
			{
				gAUDIO_CtrlInfo.voiceGain++;
			}
			break;

		//AUDIO_VOLDN
		case VOICE_VOLDN:
		default:
			if ((gAUDIO_CtrlInfo.voiceGain & 0x0F) > SPEAKER_MIN_VOL)
			{
				gAUDIO_CtrlInfo.voiceGain--;
			}
			break;

		//AUDIO_VOLSET
		case VOICE_VOLSET:
			gAUDIO_CtrlInfo.voiceGain = level;
			break;
	}

	//SFR:60
	SET_REG_VOICE_GAIN(gAUDIO_CtrlInfo.voiceGain);
}

void AUDIO_SetMicDigitalGain(U8 cmd, U8 level)
{
	switch(cmd)
	{
		case MIC_VOLUP: /*MIC_VOLUP*/
			if ((gAUDIO_CtrlInfo.micGain & 0x0F) < MIC_MAX_VOL)
			{
				gAUDIO_CtrlInfo.micGain++;
			}
			break;

		case MIC_VOLDN: /*MIC_VOLDN*/
		default:
			if ((gAUDIO_CtrlInfo.micGain & 0x0F) > MIC_MIN_VOL)
			{
				gAUDIO_CtrlInfo.micGain--;
			}
			break;

		case MIC_VOLSET: /*MIC_VOLSET*/
			gAUDIO_CtrlInfo.micGain = level | 0x10;
			break;
	}

	SET_REG_MIC_GAIN(gAUDIO_CtrlInfo.micGain);
}


PRIVATE void AUDIO_SPDIFEnable(void)
{
	U8 isEnabled;

	OS_ENTER_CRITICAL();
	isEnabled = LC_SFR_READ_BY_C2(C2OFFS_RW_SFR_SPDIF_ENC_EN);
	OS_EXIT_CRITICAL();

	if(!(isEnabled&0x01))
	{
		//for 48k
		OS_ENTER_CRITICAL();
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_SFR_ERR_PER_BIT_B0, 0x27);
		LC_SFR_CONT_WRITE_BY_C2(0x00);
		LC_SFR_CONT_WRITE_BY_C2(0x30);
		LC_SFR_CONT_WRITE_BY_C2(0x00);
		LC_SFR_CONT_WRITE_BY_C2(0x07);

		LC_SFR_WRITE_BY_C2(C2OFFS_RW_SFR_SPDIF_ENC_EN, 0x02);	// SPDIF Encoder enable
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_SFR_SPDIF_ENC_EN, 0x03);	// SPDIF cloke enable of SPDIF Encoder


		LC_SFR_WRITE_BY_C2(C2OFFS_RW_SFR_SPDIF_ENC_INI_ADDR_B0, PTR16_LSB(SPDIF_CONSUMER_CH_STATUS));	//SPDIF Encoder DMA initial address (LSB)
		LC_SFR_CONT_WRITE_BY_C2(PTR16_MSB(SPDIF_CONSUMER_CH_STATUS));	 //SPDIF Encoder DMA initial address (MSB)
		OS_EXIT_CRITICAL();
	}
}

PRIVATE void AUDIO_SPDIFDisable(void)
{
	U8 isEnabled;
	OS_ENTER_CRITICAL();
    //#warning "The following SFR doesn't exist"
    #if 0
	isEnabled = LC_SFR_READ_BY_C2(C2OFFS_RW_SFR_SPDIF_ENC_EN);
    #else
    isEnabled = 0;
    #endif
	OS_EXIT_CRITICAL();
	if(isEnabled & 0x01)
	{
		OS_ENTER_CRITICAL();
        //#warning "The following SFR doesn't exist"
        #if 0
		LC_SFR_WRITE_BY_C2(C2OFFS_RW_SFR_SPDIF_ENC_EN,	0x00);	// SPDIF Encoder disable
        #endif
		OS_EXIT_CRITICAL();
	}
}