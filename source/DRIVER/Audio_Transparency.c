#define	AUDIO_TRANSPARENCY_C

#include "reside_flash.inc"

#include "rc.h"
#include "bt_config_profile.h"

#include "Audio_Transparency.h"
#include "AudioControl.h"
#include "Audio_Mailbox.h"
#include "..\DSP\DSP_Interface.h"

#include "sector_config1.h"

#include "sector_boundary.h"

enum
{
	AT_PEQ_DISABLE,
	AT_PEQ_ENABLE,
};

PUBLIC BOOL AudioTrspr_Start(void)
{
	U8 a2dpPeqNum, lineInPeqNum;
	U8 XDATA_PTR peqPtr;
	U8 peqExpansionEnable;
	U8 peqATEnable;
	U8 CODE_PTR peqATPtr;
	DriverMsg XDATA_PTR msgPtr;
	
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
		return TRUE;

	if(IS_DSP_BUSY)
		return FALSE;
	
	if(msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);

		AUDIO_SetAudioOn(AUDIO_TRANSPARENCY);
		
		Mailbox_TaskOgfVPRTAT(msgPtr, OGF_AT, OCF_AT_START);

		AUDIO_SetAudioInOutToMailBoxCmd(AUDIO_TRANSPARENCY, &msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.at_start_para.audio_scenario);
		
		a2dpPeqNum = ((DSP_PEQ_PARAMETER_STRU CODE_PTR)GET_SECTOR_CFG1_ADDR(SECTOR_DSP_PEQ_PARAMETER))->num_of_a2dp_peq_config;
		lineInPeqNum = ((DSP_PEQ_PARAMETER_STRU CODE_PTR)GET_SECTOR_CFG1_ADDR(SECTOR_DSP_PEQ_PARAMETER))->num_of_linein_peq_config;
		peqExpansionEnable = *(U8 CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DSP_PEQ_PARAMETER) + sizeof(U8) + sizeof(U8) + (a2dpPeqNum + lineInPeqNum)*sizeof(PEQ_CONFIG_SECTOR));
		peqExpansionEnable = (peqExpansionEnable)?1:0;
		peqATPtr = (U8 CODE_PTR)(GET_SECTOR_CFG1_ADDR(SECTOR_DSP_PEQ_PARAMETER) + sizeof(U8) + sizeof(U8) + (a2dpPeqNum + lineInPeqNum)*sizeof(PEQ_CONFIG_SECTOR)*(1+peqExpansionEnable) + sizeof(U8));
		peqATEnable = *peqATPtr;
		if(peqATEnable)
		{
			peqPtr = SYS_MemoryGet(sizeof(PEQ_CONFIG_SECTOR));
			if(peqPtr != (U8 XDATA_PTR)NULL)
			{
				peqATPtr++;
				OSMEM_memcpy_xdata_code ((U8 XDATA_PTR) peqPtr,	peqATPtr, sizeof(PEQ_CONFIG_SECTOR));
				msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.at_start_para.at_PEQ_En = AT_PEQ_ENABLE;
				msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.at_start_para.at_PEQ_Ptr = (U16 XDATA_PTR)peqPtr;
				
				LightDbgPrint("AT Peq:%d,%x,%x",(U8) peqATEnable,(U8) *(peqPtr),(U8) *(peqPtr+1));
			}
			else
				msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.at_start_para.at_PEQ_En = AT_PEQ_DISABLE;
		}
		else
		{
			msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.at_start_para.at_PEQ_En = AT_PEQ_DISABLE;
		}
		
		LightDbgPrint("AT START:%d,%d,%d",(U8) a2dpPeqNum, (U8) lineInPeqNum, (U8) msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.at_start_para.at_PEQ_En);
		DRIVER_SendCmdToMailBox(msgPtr);
		
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL AudioTrspr_Stop(void)
{
	DriverMsg XDATA_PTR msgPtr;
	if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_TRANSPARENCY))
		return TRUE;
	
	if(IS_DSP_BUSY)
		return FALSE;
	
	if(msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		Mailbox_TaskOgfVPRTAT(msgPtr, OGF_AT, OCF_AT_STOP);

		DRIVER_SendCmdToMailBox(msgPtr);
		LightDbgPrint("AudioTrans STOP");
		return TRUE;
	}
	
	return FALSE;
}