//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define DRIVER1500_C
#include <rc.h>
#include "os_flash.h"
#include "bt_config_profile.h"
#include "align_flash.h"
//#include "drv_sector.h"
#include "Driver.h"
#include "Driver_1500.h"
#include "AudioControl.h"
#ifdef RINGTONE_ENABLE
	#include "ringtone.h"
#endif
#ifdef SUPPORT_VOICE_PROMPT
	#include "VoicePrompt.h"
#endif

PUBLIC U8 DRIVER_SCOCmdHandler1500(DriverMsg XDATA_PTR msgPtr)
{
	DRIVER_TurnOffLineIn();
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendGeneralEvent(DRIVER_REQUEST_CLOSE_FM_EVT);
		return QUE_PUTFRONT;
	}
	else if(!DRIVER_TurnOffA2DP())
	{
		return QUE_PUTFRONT;
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
	{
		if(gDriver_ctl.dspLink != msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.linkInd)
		{
			DRIVER_TurnOffSCO();
			return QUE_PUTFRONT;
		}
		else
		{
			return MEMORY_PUT;
		}
	}
	else if(DRIVER_IsRingToneOrVPOrATPlaying())
	{
		return QUE_PUTFRONT;
	}
	else if(gAUDIO_CtrlInfo.dspSel == DSP_SEL_NONE)
	{
		goto DSP_OPEN_SCO;
	}
	else
	{
		DRIVER_TurnOffPureOn();
		DSP_OPEN_SCO:
		if (!AUDIO_COMPONENT_IS_ACTIVE (AUDIO_SCO))
		{
			gDriver_ctl.dspLink = msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.linkInd;
			DRIVER_SetNormalVolumeToHW();

			if(gDriver_ctl.redialMuteTimerPtr)
				SYS_SetTimer(&gDriver_ctl.redialMuteTimerPtr,0);
			//dma_reset
			AUDIO_MONOTRxReset();

			AUDIO_SetAudioOn (AUDIO_ENABLE_BOTH_AD_DA, AUDIO_SCO);
			DRIVER_SendSCOEnableEvent(FALSE);
		}
	}
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_A2DPCmdHandler1500(DriverMsg XDATA_PTR msgPtr)
{
	DRIVER_TurnOffLineIn();
	DRIVER_TurnOffSCO();
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendGeneralEvent(DRIVER_REQUEST_CLOSE_FM_EVT);
		return QUE_PUTFRONT;
	}
	else if(DRIVER_IsRingToneOrVPOrATPlaying())
	{
		return QUE_PUTFRONT;
	}
	else if(gAUDIO_CtrlInfo.dspSel == DSP_SEL_A2DP	||gAUDIO_CtrlInfo.dspSel== DSP_SEL_NONE)
	{
		goto OPEN_DSP_A2DP;
	}
	else
	{
		DRIVER_TurnOffPureOn();
		OPEN_DSP_A2DP:
		if(gAlign.MEDIA_CLOSE)
			DRIVER_SendA2DPDisableEvent();
		gAlign.MEDIA_CLOSE = 0;
		gAlignDSP.DEC_A2DP_START_ANCHOR = 0;

		DRIVER_SetA2DPVolumeToHW();
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.stereo_start_para.codec_type = msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.enablePara.a2dpPara.codec;
		gAlignDSP.DSP_A2DP_AUDIO_CHANNEL_SEL = msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.enablePara.a2dpPara.tws_audio_setting;
		gAlign.SBC_TARGET_BLOCKS_OFFSET = msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.enablePara.a2dpPara.sbc_target_blocks_offset;
		gAlign.TWS_RESYNC_SIGNAL = 0;//0 is TWS_RESYNC_REQUEST_FROM_DSP
		AUDIO_SetAudioOn (AUDIO_ENABLE_DA, AUDIO_A2DP);
		gDriver_ctl.dspLink = msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.linkInd;
		DRIVER_SendA2DPEnableEvent();
		return MEMORY_PUT;
	}
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_LineInCmdHandler1500(DriverMsg XDATA_PTR msgPtr)
{
	DRIVER_TurnOffSCO();
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendGeneralEvent(DRIVER_REQUEST_CLOSE_FM_EVT);
		return QUE_PUTFRONT;
	}
	else if(!DRIVER_TurnOffA2DP())
	{
		return QUE_PUTFRONT;
	}
	else if(DRIVER_IsRingToneOrVPOrATPlaying())
	{
		return QUE_PUTFRONT;
	}
	else if(gAUDIO_CtrlInfo.dspSel == DSP_SEL_LINE_IN  ||gAUDIO_CtrlInfo.dspSel== DSP_SEL_NONE)
	{
		goto OPEN_DSP_LINE_IN;
	}
	else
	{
		DRIVER_TurnOffPureOn();
		//// Set volume////////////////////////
		OPEN_DSP_LINE_IN:
		if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
		{
				//Set Volume
				gDriver_ctl.dspLink = 0xFF;
				DRIVER_SetLineInMuteUnMute(msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.enablePara.isLineInMute);
				// Open Line in
				AUDIO_SetAudioOn (AUDIO_ENABLE_BOTH_AD_DA, AUDIO_LINE_IN);

				DRIVER_SetLineInVolumeToHW();
				DRIVER_SendLineInEnableEvent();
		}
	}
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_StopDSPCmdHandler1500(DriverMsg XDATA_PTR msgPtr)
{
	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP) && gDriver_ctl.dspLink == msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.linkInd)
	{
		if(!DRIVER_TurnOffA2DP())
		{
			return QUE_PUTFRONT;
		}
	}
	else if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_SCO))
	{
		DRIVER_TurnOffSCO();
	}
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_StopLineInCmdHandler1500(DriverMsg XDATA_PTR msgPtr)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		UNUSED(msgPtr);
		DRIVER_TurnOffLineIn();
		DRIVER_SetNormalVolumeToHW();
	}
	return MEMORY_PUT;
}

PUBLIC U8 DRIVER_StopDSPPowerOffCmdHandler1500(DriverMsg XDATA_PTR msgPtr)
{
	UNUSED(msgPtr);
	DRIVER_TurnOffLineIn();
	DRIVER_TurnOffSCO();

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_FM))
	{
		DRIVER_SendGeneralEvent(DRIVER_REQUEST_CLOSE_FM_EVT);
		return QUE_PUTFRONT;
	}
	if(!DRIVER_TurnOffA2DP())
	{
		return QUE_PUTFRONT;
	}
	DRIVER_TurnOffPureOn();
	gDriver_ctl.dspLink = 0xFF;
	return MEMORY_PUT;
}

PUBLIC void DRIVER_TurnOffSCO(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
	{
		AUDIO_SetAudioOff (AUDIO_SCO);
		DRIVER_SendSCODisableEvent();
	}
}

PUBLIC BOOL DRIVER_TurnOffA2DP(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE (AUDIO_A2DP))
	{
		gAlign.MEDIA_CLOSE = 1;
		if(gAlign.MEDIA_CLOSE_READY)
		{
			AUDIO_SetAudioOff (AUDIO_A2DP);
			DRIVER_SendA2DPDisableEvent();
			gDriver_ctl.dspLink = 0xFF;
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}
PUBLIC void DRIVER_TurnOffLineIn(void)
{
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN))
	{
		AUDIO_SetAudioOff (AUDIO_LINE_IN);
		DRIVER_SendGeneralEvent (DRIVER_LINE_IN_DSP_DISABLE_EVENT);
		gDriver_ctl.dspLink = 0xFF;
	}
}