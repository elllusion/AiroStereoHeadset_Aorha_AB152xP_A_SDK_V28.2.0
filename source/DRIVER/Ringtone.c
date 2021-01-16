//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define RINGTONE_C
#include "rc.h"
#include "bt_config_profile.h"

#ifdef RINGTONE_ENABLE

#include "ringtone.h"
#include "AudioControl.h"
#include "Audio_Mailbox.h"
#include "..\DSP\DSP_Interface.h"
#include "Driver_1520.h"

#include "sector_config1.h"
#include "drv_sector.h"

extern PUBLIC void DRIVER_RingtoneOffCallback(void);

void Ring_Init(void)
{
	gRT_ctl.pDriverRingtoneData = (DRIVER_RINGTONE_DATA_TYPE CODE_PTR)GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_RINGTONE_DATA_INIT);
	gRT_ctl.ringtoneNumber = gRT_ctl.pDriverRingtoneData->totalRingtoneNo;
}

PUBLIC BOOL Ring_GetData(U8 beepIndex)
{
	RINGTONE_RECORD_TYPE LDATA ringtoneInfo;

	if(beepIndex >= gRT_ctl.ringtoneNumber)
		return FALSE;

	ringtoneInfo = gRT_ctl.pDriverRingtoneData->ringtoneRecord[beepIndex];


	if(ringtoneInfo.dataLength == 0)
		return FALSE;


	gRT_ctl.playAddr = 0;
	gRT_ctl.playAddrInCode = (RINGTONE_DATA_MEMORY_PTR_TYPE CODE_PTR)DSP_GetCODEAddr((U8 CODE_PTR)gRT_ctl.pDriverRingtoneData + ringtoneInfo.offset);


	gRT_ctl.ringtoneLength = ringtoneInfo.dataLength;
	return TRUE;
}

#ifdef RINGTONE_COPY_XDATA_SRC
PUBLIC BOOL Ring_CopyData(U8 XDATA_PTR srcPtr, U16 ringtoneLen)
{
	RINGTONE_DATA_MEMORY_PTR_TYPE XDATA_PTR tmpPtr;

	if(srcPtr == (U8 XDATA_PTR)NULL || ringtoneLen == 0)
		return FALSE;

	RINGTINE_DATA_MEMORY_PTR = (RINGTONE_DATA_MEMORY_PTR_TYPE XDATA_PTR)SYS_MemoryGet(ringtoneLen+2); // +1 for DSP align

	if(RINGTINE_DATA_MEMORY_PTR == (RINGTONE_DATA_MEMORY_PTR_TYPE XDATA_PTR)NULL)
		return FALSE;

	tmpPtr = RINGTINE_DATA_MEMORY_PTR;

	if(PTR16_LSB(tmpPtr) & 0x01)
	{
		tmpPtr = (U16 XDATA_PTR)((U8 XDATA_PTR)tmpPtr + 1);
	}

	OSMEM_memcpy_xdata_xdata(tmpPtr, srcPtr, ringtoneLen);
	gRT_ctl.playAddr = tmpPtr;
	gRT_ctl.ringtoneLength = ringtoneLen;
	return TRUE;

}
#endif

PUBLIC BOOL Ring_Start(BOOL suspendApp)
{
	DriverMsg XDATA_PTR msgPtr;

	if(gRT_ctl.playAddr == (RINGTONE_DATA_MEMORY_PTR_TYPE XDATA_PTR)NULL && gRT_ctl.playAddrInCode == (RINGTONE_DATA_MEMORY_PTR_TYPE CODE_PTR)NULL)
		return FALSE;

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

	if(msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		AUDIO_SetAudioOn(AUDIO_RINGTONE);

		RINGTONE_STOP_FLAG = 0;
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.rt_start_para.rt_no = (gRT_ctl.ringtoneLength>>2);
		if(gRT_ctl.playAddr)
		{
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.rt_start_para.rt_pattern_ptr = (U16 XDATA_PTR)gRT_ctl.playAddr;
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.rt_start_para.rt_pattern_ptr_code = 0;
		}
		else if(gRT_ctl.playAddrInCode)
		{
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.rt_start_para.rt_pattern_ptr = 0;
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.rt_start_para.rt_pattern_ptr_code = (U16 CODE_PTR)gRT_ctl.playAddrInCode;
		}

        // reserve dynamic config for each tone
		msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.rt_start_para.mute_frame_before_fade_in = 4;

        Mailbox_TaskOgfVPRTAT(msgPtr, OGF_RT, OCF_RT_START);

		AUDIO_SetAudioInOutToMailBoxCmd(AUDIO_RINGTONE, &msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.rt_start_para.audio_scenario);
        DRIVER_SendCmdToMailBox(msgPtr);
		return TRUE;
	}

	//Fail case
	SYS_MemoryRelease(&RINGTINE_DATA_MEMORY_PTR);
	gRT_ctl.playAddrInCode = 0;
	return FALSE;
}

PUBLIC BOOL Ring_Pause(void)
{
	DriverMsg XDATA_PTR msgPtr;

	if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE))
		return TRUE;

	if(IS_MAILBOX_TASK_VPRT_BUSY)
		return FALSE;

	if(IS_RINGTONE_OVER)
		return TRUE;

	if(IS_DSP_BUSY)
		return FALSE;

	if(msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get (OSMEM_ptr1))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		Mailbox_TaskOgfVPRTAT(msgPtr, OGF_RT, OCF_RT_STOP);

		DRIVER_SendCmdToMailBox(msgPtr);
		return TRUE;
	}
	return FALSE;
}

PUBLIC void Ring_Stopped(void)
{
	AUDIO_SetAudioOff(AUDIO_RINGTONE);
	DRIVER_CheckATEnablePlay();
	RINGTONE_STOP_FLAG = 0;
	SYS_MemoryRelease(&RINGTINE_DATA_MEMORY_PTR);
	gRT_ctl.playAddrInCode = 0;
	DRIVER_RingtoneOffCallback();

	if(gDriver_ctl.resumeAppAfterVPRT)
	{
		SYS_SetTimer(&gDriver_ctl.resumeAppTimer, ONE_SEC);
		gDriver_ctl.resumeAppAfterVPRT = FALSE;
	}
}

#endif
