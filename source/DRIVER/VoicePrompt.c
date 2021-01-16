//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define VOICE_PROMPT_C
#include "rc.h"
#include "bt_config_profile.h"

#ifdef SUPPORT_VOICE_PROMPT

#include "driver_interface.h"

#include "hal.h"
#include "Audio_Mailbox.h"
#include "Driver_1520.h"

#include "sector_voice_data.h"
#include "sector_dsp_data.h"
#include "sector_runtime.h"
#include "sector_config1.h"

#include "VoicePrompt.h"
#include "AudioControl.h"

#ifdef AIRAPP_Profile
	#include "..\AirApp\AirApp_Interface.h"
#endif

#if (HW_SPI_SUPPORTED)
#include "drv_spi_flash.h"
#include "drv_spiflash.h"
#endif

PRIVATE U8 XDATA_PTR VoicePrompt_GetScript_CallerID(DriverMsg XDATA_PTR msgPtr);
PRIVATE U8 XDATA_PTR VoicePrompt_GetScript_FMChannelNumber(DriverMsg XDATA_PTR msgPtr);
PRIVATE U8 XDATA_PTR VoicePrompt_GetScript_BySettings(U8 vpIndex);
PRIVATE BOOL VoicePrompt_GetVPCtlMemory(U8 scriptCount);
PRIVATE U8 VoicePrompt_GetItemDataMap(U8 XDATA_PTR scriptPtr);
PRIVATE U8 VoicePrompt_GetItemData(U8 voicePromptIndex, VoicePromptItem XDATA_PTR itemPtr);
PRIVATE void VoicePrompt_ReadInitVPData(void);


PRIVATE void VoicePrompt_ReleaseCtlResource(void);

extern PUBLIC void DRIVER_VoicePromptOffCallback(void);

U8 CODE gVP_nb_silence[15] =
{
	0x8c,0xe1,0xe9,0xdc,0x0b,0xe0,0x1b,0x40,0x74,0xff,0x02,0xe8,0x4e,0x00,0xdd
};

U8 CODE gVP_nbh_silence[31] =
{
	0x21,0x17,0x5b,0xe8,0xfa,0x56,0x01,0x00,0x00,0x00,0x00,0xc0,0x06,0x00,0x00,
	0x00,0x00,0x00,0xab,0x00,0x00,0x00,0x00,0x00,0x60,0x03,0x00,0x00,0x00,0x00,0x00
};

U8 CODE gVP_wb_silence[32] =
{
	0x10,0x76,0x38,0xc2,0x43,0xc9,0x80,0x20,0x00,0x00,0x00,0x01,0x18,0x98,0x03,0x21,
	0x90,0x84,0x00,0x09,0xda,0x01,0xa8,0x02,0x40,0xfd,0x98,0x00,0x02,0x00,0x0b,0x00,
};

U8 CODE gVP_ADPCM_silence[80] =
{
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};



PUBLIC BOOL VoicePrompt_GetData(DriverMsg XDATA_PTR msgPtr)
{
	U8 dq_state;
	U8 XDATA_PTR scriptPtr = (U8 XDATA_PTR)NULL;

	if(msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpIndex == VP_EVT_CALLER_ID)
	{
		scriptPtr = VoicePrompt_GetScript_CallerID(msgPtr);
	}
	else if(msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpIndex == VP_EVT_FM_CHANNEL_NUMBER)
	{
		scriptPtr = VoicePrompt_GetScript_FMChannelNumber(msgPtr);
	}
	else if(msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpIndex == VP_EVT_CALLER_NAME)
	{
	#ifdef AIRAPP_Profile
		#if AIRAPP_CALLER_NAME
		U16 length;
		U8 dq;
		VoicePromptItem XDATA_PTR itemPtr;
		SECTOR_APP_CALLERNAME_DATA_STRU CODE_PTR ptr1 = (SECTOR_APP_CALLERNAME_DATA_STRU CODE_PTR)GET_SECTOR_RUNTIME_ADDR(SECTOR_APP_CALLERNAME_DATA);

		extern U8 XDATA_PTR largeP;


		if(!AirApp_IsCallerNameDataValid())
			return VP_GET_DATA_FAIL;

		if( !VoicePrompt_GetVPCtlMemory(1) )
			return VP_GET_DATA_BUSY;

		itemPtr = gVP_ctl.dataCtlPtr->voice_prompt_item_ptr;
		itemPtr->play_ctl.vp_ptr = (U32)&ptr1->callername_data.callerNameData[0];
		length = *(U16 GENERIC_PTR)(itemPtr->play_ctl.vp_ptr);
		if(length == 0)
		{
			SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gVP_ctl.dataCtlPtr->voice_prompt_item_ptr);
			SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gVP_ctl.dataCtlPtr);
			return VP_GET_DATA_FAIL;
		}
		dq = length >> 14;
		itemPtr->length = (length & 0x3FFF);
		itemPtr->play_ctl.vp_ptr += 2;

		gVP_ctl.dataCtlPtr->vp_data_quality = dq;

		gVP_ctl.dataCtlPtr->item_count++;
		return VP_GET_DATA_OK;
		#else
		return VP_GET_DATA_FAIL;
		#endif
	#endif
	}
	else
	{
		scriptPtr = VoicePrompt_GetScript_BySettings(msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpIndex);
	}

	if (scriptPtr == NULL)
		return VP_GET_DATA_FAIL;

	dq_state = VoicePrompt_GetItemDataMap(scriptPtr);
	OSMEM_Put(scriptPtr);
	return (dq_state) ? VP_GET_DATA_FAIL : VP_GET_DATA_OK;
}

PUBLIC BOOL VoicePrompt_Start(BOOL suspendApp, BOOL isOverSCO)
{
	U16 vp_len;
	U8 item_count;
	VoicePromptItem XDATA_PTR ptr1;
	DriverMsg XDATA_PTR msgPtr;

	if(IS_DSP_BUSY)
		return FALSE;

	if(suspendApp)
	{
		if(gDriver_ctl.resumeAppTimer)
		{
			gDriver_ctl.resumeAppAfterVPRT = TRUE;
			SYS_ReleaseTimer(&gDriver_ctl.resumeAppTimer);
		}

		if(DRIVER_IsDSPOgfAppOn())
		{
			gDriver_ctl.resumeAppAfterVPRT = TRUE;
			DRIVER_SendAppIdleToMailBox();
			return FALSE;
		}
	}

	if((msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1)) == (DriverMsg XDATA_PTR)NULL)
	{
		VoicePrompt_ReleaseCtlResource();
		return FALSE;
	}
	OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);

	if(gVP_ctl.dataCtlPtr->vp_data_quality == VP_DAT_ADPCM)
	{
		gVP_ctl.dataCtlPtr->vp_pattern_ptr = OSMEM_Get(OSMEM_ptr2);
		OSMEM_memset_xdata(gVP_ctl.dataCtlPtr->vp_pattern_ptr, 0, OSMEM2_BLKSIZE);
	}
	else
	{
		gVP_ctl.dataCtlPtr->vp_pattern_ptr = OSMEM_Get(OSMEM_ptr1);
		OSMEM_memset_xdata(gVP_ctl.dataCtlPtr->vp_pattern_ptr, 0, OSMEM1_BLKSIZE);
	}

	VP_RAW_PTR = (U16 XDATA_PTR) gVP_ctl.dataCtlPtr->vp_pattern_ptr;
	if(PTR16_LSB(VP_RAW_PTR) & 0x01)
	{
		VP_RAW_PTR = (U16 XDATA_PTR)((U8 XDATA_PTR)VP_RAW_PTR + 1);
	}

	switch (gVP_ctl.dataCtlPtr->vp_data_quality)
	{
		case VP_DAT_NB:
			gVP_ctl.dataCtlPtr->frame_length = sizeof(gVP_nb_silence);
			gVP_ctl.dataCtlPtr->vp_silence_ptr = gVP_nb_silence;
			break;
		case VP_DAT_WB:
			gVP_ctl.dataCtlPtr->frame_length = sizeof(gVP_wb_silence);
			gVP_ctl.dataCtlPtr->vp_silence_ptr = gVP_wb_silence;
			break;
		case VP_DAT_NBH:
			gVP_ctl.dataCtlPtr->frame_length = sizeof(gVP_nbh_silence);
			gVP_ctl.dataCtlPtr->vp_silence_ptr = gVP_nbh_silence;
			break;
		case VP_DAT_ADPCM:
			gVP_ctl.dataCtlPtr->frame_length = sizeof(gVP_ADPCM_silence);
			gVP_ctl.dataCtlPtr->vp_silence_ptr = gVP_ADPCM_silence;
			break;
	}


	vp_len = 0;
	item_count = gVP_ctl.dataCtlPtr->item_count;
	ptr1 = gVP_ctl.dataCtlPtr->voice_prompt_item_ptr;

	while (item_count--)
	{
		vp_len += ptr1->length;
		ptr1++;
	}
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.vp_len = ENDIAN_TRANSFORM_U16(vp_len);

	VoicePrompt_ReadInitVPData();

	//enable DSP
	AUDIO_SetAudioOn (AUDIO_VOICEPROMPT);

	VP_STOP_FLAG = 0;


	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.vp_pattern_ptr = VP_RAW_PTR;
	switch (gVP_ctl.dataCtlPtr->vp_data_quality)
	{
		case VP_DAT_WB:
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.vp_quality = AUDIO_MODE_WB_VP;
			break;

		case VP_DAT_NB:
		case VP_DAT_NBH:
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.vp_quality = (gVP_ctl.dataCtlPtr->vp_data_quality == VP_DAT_NB)? AUDIO_MODE_NB_VP : AUDIO_MODE_NBH_VP;
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.dsp_vp_nb_ptr = (U32)DSP_GetCODEAddr((U8 CODE_PTR)GET_SECTOR_DSP_DATA_ADDR(SECTOR_DSP_VP_NB));
			if(!msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.dsp_vp_nb_ptr)
				ASSERT(FALSE);
			break;
		case VP_DAT_ADPCM:
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.vp_quality = AUDIO_MODE_ADPCM_VP;
			break;
	}
	//replace with mailbox command
	if(isOverSCO)
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.vp_resv = VP_TO_ESCO_RELAY;

    // reserve dynamic config for each prompt
    msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.mute_frame_before_fade_in = 4;

	Mailbox_TaskOgfVPRTAT(msgPtr, OGF_VP, OCF_VP_START);
	AUDIO_SetAudioInOutToMailBoxCmd(AUDIO_VOICEPROMPT, &msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vp_start_para.audio_scenario);
	DRIVER_SendCmdToMailBox(msgPtr);
	return TRUE;
}

PUBLIC BOOL VoicePrompt_Pause(void)
{
	DriverMsg XDATA_PTR msgPtr;

	if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT))
		return TRUE;

	if(IS_MAILBOX_TASK_VPRT_BUSY)
		return FALSE;

	if(IS_VOICE_PROMPT_OVER)
		return TRUE;

	if(IS_DSP_BUSY)
		return FALSE;

	if(msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		Mailbox_TaskOgfVPRTAT(msgPtr, OGF_VP, OCF_VP_STOP);
		DRIVER_SendCmdToMailBox(msgPtr);
		return TRUE;
	}
	return FALSE;
}

PUBLIC void VoicePrompt_Stopped(void)
{
	//Audio off
	AUDIO_SetAudioOff (AUDIO_VOICEPROMPT);
	DRIVER_CheckATEnablePlay();
	VP_STOP_FLAG = 0;

	DRIVER_VoicePromptOffCallback();

	VoicePrompt_ReleaseCtlResource();

	if(gDriver_ctl.resumeAppAfterVPRT)
	{
		SYS_SetTimer(&gDriver_ctl.resumeAppTimer, ONE_SEC);
		gDriver_ctl.resumeAppAfterVPRT = FALSE;
	}
}

PRIVATE U8 XDATA_PTR VoicePrompt_GetScript_CallerID(DriverMsg XDATA_PTR msgPtr)
{
	U8 scriptCount;
	U8 XDATA_PTR scriptPtr;

	if ((scriptCount = msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.callerIDData.callerIdLength) != 0)
	{
		if(scriptPtr = SYS_MemoryGet( scriptCount+3 ))
		{
			scriptPtr[0] = scriptCount + 2;
			scriptPtr[1] = VP_DAT_CALLER_ID_PREFIX;
			OSMEM_memcpy_xdata_xdata(&scriptPtr[2], msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.callerIDData.callerID, scriptCount);
			scriptPtr[scriptCount+2] = VP_DAT_CALLER_ID_SUFFIX;
			return scriptPtr;
		}
	}

	return (U8 XDATA_PTR)NULL;
}

PRIVATE U8 XDATA_PTR VoicePrompt_GetScript_FMChannelNumber(DriverMsg XDATA_PTR msgPtr)
{
	U8 scriptCount;
	U8 XDATA_PTR scriptPtr;

	if ((scriptCount = msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.fmChannel.digitLength) != 0)
	{
		if(scriptPtr = SYS_MemoryGet( scriptCount+4 ))
		{
			scriptPtr[0] = scriptCount + 3;
			scriptPtr[1] = VP_DAT_FM_CHANN_PREFIX;
			OSMEM_memcpy_xdata_xdata(&scriptPtr[2], &msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.fmChannel.number[0], scriptCount-1);
			scriptPtr[scriptCount+1] = VP_DAT_POINT;
			scriptPtr[scriptCount+2] = msgPtr->msgBodyPtr.driverCmd.voicePromptCmd.vpData.fmChannel.number[scriptCount-1];
			scriptPtr[scriptCount+3] = VP_DAT_FM_CHANN_SUFFIX;
			return scriptPtr;
		}
	}

	return (U8 XDATA_PTR)NULL;
}

PRIVATE U8 XDATA_PTR VoicePrompt_GetScript_BySettings(U8 vpIndex)
{
	U8 scriptCount;
	U8 XDATA_PTR scriptPtr;
	VoicePromptOffset vpOffset;

	U16 LDATA scriptOffsetNext;
	U16 LDATA scriptOffset;


	// Check valid vp index
	if (gDriver_ctl.voiceLangCtl.vpScriptOffsetEntries == VOICE_PROMPT_INVALID || vpIndex >= gDriver_ctl.voiceLangCtl.vpScriptOffsetEntries)
	{
		return (U8 XDATA_PTR)NULL;
	}

	//  1. Read Script count and offset
	vpOffset.vp_ptr = gDriver_ctl.voiceLangCtl.vpScriptOffsetTbl.vp_ptr + (vpIndex * sizeof(U16));

	scriptOffset = ((U16 GENERIC_PTR)vpOffset.vp_ptr)[0];
	scriptOffsetNext = ((U16 GENERIC_PTR)vpOffset.vp_ptr)[1];

	scriptCount = scriptOffsetNext - scriptOffset;

	if (scriptCount == 0)
		return (U8 XDATA_PTR)NULL;

	scriptPtr = SYS_MemoryGet( scriptCount+1 );

	if(scriptPtr != (U8 XDATA_PTR)NULL)
	{
		*scriptPtr = scriptCount;

		vpOffset.vp_ptr = gDriver_ctl.voiceLangCtl.vpScriptDataTbl.vp_ptr + scriptOffset;

		OSMEM_memcpy_xdata_code(scriptPtr+1, (U8 CODE_PTR)vpOffset.vp_ptr, scriptCount);

		return scriptPtr;
	}

	return (U8 XDATA_PTR)NULL;
}


PRIVATE BOOL VoicePrompt_GetVPCtlMemory(U8 scriptCount)
{
	// Get memory
	if((gVP_ctl.dataCtlPtr = (VoicePromptDataCtlType XDATA_PTR)OSMEM_Get(OSMEM_ptr1)) == (VoicePromptDataCtlType XDATA_PTR)NULL)
	{
		return FALSE;
	}

	OSMEM_memset_xdata((U8 XDATA_PTR)gVP_ctl.dataCtlPtr, 0, OSMEM1_BLKSIZE);

	// Get item ptr memory
	if(scriptCount > (OSMEM1_BLKSIZE/sizeof(VoicePromptItem)))
		gVP_ctl.dataCtlPtr->voice_prompt_item_ptr = (VoicePromptItem XDATA_PTR)OSMEM_Get(OSMEM_ptr2);
	else
		gVP_ctl.dataCtlPtr->voice_prompt_item_ptr = (VoicePromptItem XDATA_PTR)OSMEM_Get(OSMEM_ptr1);

	if(gVP_ctl.dataCtlPtr->voice_prompt_item_ptr == (VoicePromptItem XDATA_PTR)NULL)
	{
		SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gVP_ctl.dataCtlPtr);
		return FALSE;
	}

	// Set parameter
	gVP_ctl.dataCtlPtr->item_count = 0;
	gVP_ctl.dataCtlPtr->voice_prompt_item_ptr_init = gVP_ctl.dataCtlPtr->voice_prompt_item_ptr;

	return TRUE;
}

PRIVATE U8 VoicePrompt_GetItemDataMap(U8 XDATA_PTR scriptPtr)
{
	U8 scriptCount;
	U8 dq, dq_state;
	VoicePromptItem XDATA_PTR itemPtr;

	scriptCount = *scriptPtr++;
	if( !VoicePrompt_GetVPCtlMemory(scriptCount) )
		return VP_DQ_INVALID;

	dq_state = VP_DQ_NOTSET;
	itemPtr = gVP_ctl.dataCtlPtr->voice_prompt_item_ptr;
	for ( ; scriptCount; scriptPtr++, scriptCount--)
	{
		if (scriptPtr[0] == VP_DAT_BREAK)
		{
			itemPtr->length = *(U16 XDATA_PTR)&scriptPtr[1];
			scriptPtr += 2;
			scriptCount -= 2;
			if (itemPtr->length == 0)
			{
				continue;
			}

			itemPtr->play_ctl.vp_ptr = 0L;
			itemPtr->play_ctl.play_type = VP_DAT_BREAK;

			if (dq_state & VP_DQ_NOTSET)
			{
				if(scriptCount == 1)
				{
					dq_state &= ~VP_DQ_NOTSET;
					#if VP_NB_LQ_ENABLE
					gVP_ctl.dataCtlPtr->vp_data_quality = VP_DAT_NB;
					#elif VP_NB_HQ_ENABLE
					gVP_ctl.dataCtlPtr->vp_data_quality = VP_DAT_NBH;
					#elif VP_WB_ENABLE
					gVP_ctl.dataCtlPtr->vp_data_quality = VP_DAT_WB;
					#endif
				}
			}
		}
		else
		{
			dq = VoicePrompt_GetItemData(scriptPtr[0], itemPtr);
			if (itemPtr->length == 0)
				continue;

			if (dq_state & VP_DQ_NOTSET)
			{
				dq_state &= ~VP_DQ_NOTSET;
				gVP_ctl.dataCtlPtr->vp_data_quality = dq;
			}
			else
			{
				if (gVP_ctl.dataCtlPtr->vp_data_quality != dq)
					dq_state |= VP_DQ_INVALID;
			}
		}

		if (dq_state & VP_DQ_INVALID)
		{
			SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gVP_ctl.dataCtlPtr->voice_prompt_item_ptr);
			SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gVP_ctl.dataCtlPtr);
			break;
		}

		itemPtr++;
		gVP_ctl.dataCtlPtr->item_count++;
	}

	return dq_state;
}

#define EXTERNAL_VP_DATA		0x80000000L

PRIVATE U8 VoicePrompt_GetItemData(U8 voicePromptIndex, VoicePromptItem XDATA_PTR itemPtr)
{
	U16 LDATA length;
	U8 dq;

	if (voicePromptIndex >= gDriver_ctl.voiceLangCtl.vpDataOffsetEntries)
	{
		itemPtr->length = 0;
		return 0;
	}

	itemPtr->play_ctl.vp_ptr = (U32)(*(U8 GENERIC_PTR GENERIC_PTR)(gDriver_ctl.voiceLangCtl.vpDataOffsetTbl.vp_ptr + (voicePromptIndex * sizeof(VoicePromptOffset))));

	#if (HW_SPI_SUPPORTED)
	if((U32)itemPtr->play_ctl.vp_ptr & EXTERNAL_VP_DATA)
	{
		U32 LDATA ext_address;

		if(!gDriver_ctl.extFlashCtl.vpDataSector)
		{
			itemPtr->length = 0;
			return 0;
		}

		itemPtr->play_ctl.vp_ptr += gDriver_ctl.extFlashCtl.vpDataSector;
		ext_address = (U32)itemPtr->play_ctl.vp_ptr & (~EXTERNAL_VP_DATA);
		DRV_SPIFLH_ReadBytes(ext_address, (U8 XDATA_PTR)&itemPtr->play_ctl.vp_ptr, sizeof(U32));
		itemPtr->play_ctl.vp_ptr += gDriver_ctl.extFlashCtl.vpDataSector;
		DRV_SPIFLH_ReadBytes(itemPtr->play_ctl.vp_ptr, (U8 XDATA_PTR)&length, sizeof(U16));
		itemPtr->play_ctl.vp_ptr |= EXTERNAL_VP_DATA;
	}
	else
	#endif
	{
		itemPtr->play_ctl.vp_ptr += (U32)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_LANG_CTL);
		length = *(U16 GENERIC_PTR)(itemPtr->play_ctl.vp_ptr);
	}

	dq = length >> 14;
	itemPtr->length = (length & 0x3FFF);
	itemPtr->play_ctl.vp_ptr += 2;

	return dq;
}

PUBLIC void DRIVER_VP_CopySilence(U8 XDATA_PTR destPtr)
{
	OSMEM_memcpy_xdata_generic (destPtr,
								gVP_ctl.dataCtlPtr->vp_silence_ptr,
								gVP_ctl.dataCtlPtr->frame_length);
    gAlignDSP.DSP_nvram.DSP_Reserved_PARAM.Feature_Sel &= ~VP_FEEDBACK;
}

PRIVATE void VoicePrompt_ReadInitVPData(void)
{
	if (gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.play_type == VP_DAT_BREAK)
	{
		DRIVER_VP_CopySilence((U8 XDATA_PTR)VP_COPY_PTR);
		return;
	}

	#if (HW_SPI_SUPPORTED)
	if((U32)(gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.vp_ptr) & EXTERNAL_VP_DATA)
	{
		U32 LDATA txData = (U32)gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.vp_ptr & (~EXTERNAL_VP_DATA);

		DRV_SPIFLH_ReadBytes(txData, (U8 XDATA_PTR)VP_COPY_PTR, gVP_ctl.dataCtlPtr->frame_length);
	}
	else
	#endif
	{
		OSMEM_memcpy_xdata_generic ((U8 XDATA_PTR)VP_COPY_PTR,
									(U8 GENERIC_PTR)gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.vp_ptr,
									gVP_ctl.dataCtlPtr->frame_length);
	}
}

PUBLIC void DRIVER_VP_ReadVPData(void)
{
	//things should be done within 20ms here.
	if(IS_VOICE_PROMPT_OVER)
		return;

	if (!gVP_ctl.dataCtlPtr)
		return;

	if (!gVP_ctl.dataCtlPtr->item_count)
	{
		return;
	}

	//The following operation may occur overflow.
	//for CODE, it is ok.
	//for XDATA, the size must be less than 65535 which is ok for AB1500;
	//for EEPROM, we must use EEPROM which size is less than 65535 (512Kbit);

	gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.vp_ptr = (U8 GENERIC_PTR) ((U32)gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.vp_ptr + gVP_ctl.dataCtlPtr->frame_length);
	gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->length -= gVP_ctl.dataCtlPtr->frame_length;
	if (!gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->length)
	{
		if (--gVP_ctl.dataCtlPtr->item_count)
		{
			SYS_MemoryRelease(&gVP_ctl.dataCtlPtr->cmd_ptr1);
			SYS_MemoryRelease(&gVP_ctl.dataCtlPtr->cmd_ptr2);

			gVP_ctl.dataCtlPtr->voice_prompt_item_ptr++;
		}
		else
		{
			//Make silence
			DRIVER_VP_CopySilence((U8 XDATA_PTR)VP_COPY_PTR);
			return;
		}
	}

	if (gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.play_type == VP_DAT_BREAK)
	{
		DRIVER_VP_CopySilence((U8 XDATA_PTR)VP_COPY_PTR);
		return;
	}

	#if (HW_SPI_SUPPORTED)
	if((U32)(gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.vp_ptr) & EXTERNAL_VP_DATA)
	{
		U32 LDATA txData = (U32)gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.vp_ptr & (~EXTERNAL_VP_DATA);
		DRV_SPIFLH_ReadBytes(txData, (U8 XDATA_PTR)VP_COPY_PTR, gVP_ctl.dataCtlPtr->frame_length);
	}
	else
	#endif
	{
		OSMEM_memcpy_xdata_generic ((U8 XDATA_PTR)VP_COPY_PTR,
									(U8 GENERIC_PTR)gVP_ctl.dataCtlPtr->voice_prompt_item_ptr->play_ctl.vp_ptr,
									gVP_ctl.dataCtlPtr->frame_length);
	}
    gAlignDSP.DSP_nvram.DSP_Reserved_PARAM.Feature_Sel &= ~VP_FEEDBACK;
}


PRIVATE void VoicePrompt_ReleaseCtlResource(void)
{
	if (gVP_ctl.dataCtlPtr != (VoicePromptDataCtlType XDATA_PTR)0)
	{
		if (gVP_ctl.dataCtlPtr->voice_prompt_item_ptr != (VoicePromptItem XDATA_PTR)0)
		{
			SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gVP_ctl.dataCtlPtr->voice_prompt_item_ptr_init);
			gVP_ctl.dataCtlPtr->voice_prompt_item_ptr = (VoicePromptItem XDATA_PTR)0;
			SYS_MemoryRelease(&gVP_ctl.dataCtlPtr->vp_pattern_ptr);

			SYS_MemoryRelease(&gVP_ctl.dataCtlPtr->cmd_ptr1);
			SYS_MemoryRelease(&gVP_ctl.dataCtlPtr->cmd_ptr2);
		}
		SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gVP_ctl.dataCtlPtr);
	}
}

#endif //SUPPORT_VOICE_PROMPT
