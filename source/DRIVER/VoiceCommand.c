//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define VOICECOMMAND_C
#include "rc.h"
#include "bt_config_profile.h"

#ifdef SUPPORT_VOICE_COMMAND

#include "driver_interface.h"

#include "sector_voice_data.h"

#include "hal.h"
#include "Audio_Mailbox.h"
#include "Driver_1520.h"

#include "VoiceCommand.h"
#include "AudioControl.h"
#include "VoicePrompt.h"
#include "..\MMI\MMI_event_opcode.h"
#include "..\SECTOR\sector_mmi_driver_variation_nvram.h"

#include "voice_command_adaption_data_ctl.h"

#if (defined AB1520C) || (defined AB1520D)
#include "..\SECTOR\SectorBoundary\sector_boundary.h"
#endif

extern PUBLIC void DRIVER_VoiceCommandOffCallback(void);

PUBLIC DRIVER_VC_ACTION_TYPE CODE_PTR VoiceCommand_GetActionTable(U8 actionIndex)
{
	return (DRIVER_VC_ACTION_TYPE CODE_PTR)
	((U32)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA)
	+sizeof(DRIVER_VOICE_COMMAND_ADDRESS_TABLE)*MAX_VC_LANG_COUNT
	+sizeof(U8) *3
	+sizeof(U8)*VC_SET_NO
	+sizeof(DRIVER_VC_EVENT_TYPE)*(((DRIVER_VOICE_COMMAND_DATA_TYPE CODE_PTR)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA))->eventNo)
	+sizeof(DRIVER_VC_ACTION_TYPE)*actionIndex);
}

PUBLIC DRIVER_VOICE_COMMAND_ACTION_APD_TYPE CODE_PTR VoiceCommand_GetAPDActionTable(U8 actionIndex)
{
	return (DRIVER_VOICE_COMMAND_ACTION_APD_TYPE CODE_PTR)
	((U32)GET_SECTOR_BOUNDARY_ADDR(SECTOR_VOICE_COMMAND_APD_DATA)
	+sizeof(DRIVER_VOICE_COMMAND_APD_ADDRESS_TABLE)
	+sizeof(U8) *3
	+sizeof(DRIVER_VC_EVENT_TYPE)*(((DRIVER_VOICE_COMMAND_APD_DATA_TYPE CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_VOICE_COMMAND_APD_DATA))->eventNo)
	+sizeof(DRIVER_VOICE_COMMAND_ACTION_APD_TYPE)*actionIndex);
}

PRIVATE U32 VoiceCommand_GetDataBits(U8 actionIndex, U16 vcEventId)
{
	U8 i = 0;
	U8 setIndex;
	U32 evtBits = 0x00000000;
	U32 dataBits = 0;
	U8 rootIndex1= ((DRIVER_VOICE_COMMAND_DATA_TYPE CODE_PTR)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA))->rootEvtNum[VC_MULTI_LAYER_SET_1];
	U8 rootIndex2= ((DRIVER_VOICE_COMMAND_DATA_TYPE CODE_PTR)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA))->rootEvtNum[VC_MULTI_LAYER_SET_2];
	DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE_PTR evtDataMapPtr = (DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE_PTR) gVC_ctl.dataMapPtr;

	if(evtDataMapPtr)
	{
		#ifdef VC_MULTI_LAYER
			UNUSED(setIndex);
			UNUSED(rootIndex1);
			UNUSED(rootIndex2);
		
			if( vcEventId == MEDIA_EVT_KEY_VOICE_COMMAND_ENABLE || vcEventId == MEDIA_EVT_ENTER_INCOMING || vcEventId == MEDIA_EVT_VOICE_COMMAND_MULTILAYER_TRIGGER)
			{
				evtBits = evtBits | ((VoiceCommand_GetActionTable(actionIndex))->multiLayerBits[gVC_ctl.vcnextLayer]) ;					
			}
		#else
			UNUSED(vcEventId);
			
			for(setIndex = 0 ; setIndex < VC_MULTI_LAYER_SET_NO; setIndex++)
			{
				evtBits = evtBits | (VoiceCommand_GetActionTable(actionIndex))->multiLayerBits[setIndex];
			}

			if ((0x00000001L<<rootIndex1)& evtBits)
			{
				evtBits = evtBits^(0x00000001L << rootIndex1);
			}

			if ((0x00000001L<<rootIndex2)& evtBits)
			{
				evtBits = evtBits^(0x00000001L << rootIndex2);
			}
		#endif

		while(i < 32)
		{
			if((evtBits >> i)&0x00000001L)
			{
				dataBits |= evtDataMapPtr->evtDataMapBits[i];
			}
			i++;
		}
	}
	return dataBits;
}

PRIVATE U32 VoiceCommand_GetAPDDataBits(U8 actionIndex)
{
	U8 i = 0;
	U32 evtBits = 0x00000000;
	U32 dataBits = 0;

	DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE_PTR evtDataMapPtr = (DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE_PTR) gVC_ctl.apddataMapPtr;
	
	if(evtDataMapPtr)
	{
		//evtBits = evtBits |((DRIVER_VOICE_COMMAND_APD_DATA_TYPE CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_VOICE_COMMAND_APD_DATA))->actionTable[actionIndex].triggerEventBits;

		evtBits = evtBits | (VoiceCommand_GetAPDActionTable(actionIndex))->triggerEventBits;

		LightDbgPrint("APD evtBits %X", (U32)evtBits);
	
		while(i < 32)
		{
			if((evtBits >> i)&0x00000001L)
			{
				dataBits |= evtDataMapPtr->evtDataMapBits[i];
			}
			i++;
		}
	}
	return dataBits;
}

PUBLIC U8 VoiceCommand_GetEvtIndex(U8 dataIndex)
{
	U8 evtIndex = 0;
	U32 dataBit = 0x00000001L << dataIndex;
	DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE_PTR evtDataMapPtr = (DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE_PTR) gVC_ctl.dataMapPtr;
	
	if(gVC_ctl.vcIdentificationMode == APD_VC_IDENTIFY)
	{
		evtDataMapPtr =  (DRIVER_VOICE_COMMAND_EVT_DATA_MAP_TYPE CODE_PTR)gVC_ctl.apddataMapPtr;
	}
	
	while(evtIndex < 32)
	{
		if(evtDataMapPtr->evtDataMapBits[evtIndex] & dataBit)
			return evtIndex;
			
		evtIndex++;
	}
	
	return VC_INVALID_EVT;
}

PUBLIC U8 VoiceCommand_IsEnabled(void)
{
	if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND) || (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO)))
		return  TRUE;
	else
		return FALSE;
}

PUBLIC U8 VoiceCommand_VCLayer(U8 dataIndex)
{
	#ifdef VC_MULTI_LAYER
	
	U8 rootIndex1= ((DRIVER_VOICE_COMMAND_DATA_TYPE CODE_PTR)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA))->rootEvtNum[VC_MULTI_LAYER_SET_1];
	U8 rootIndex2= ((DRIVER_VOICE_COMMAND_DATA_TYPE CODE_PTR)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA))->rootEvtNum[VC_MULTI_LAYER_SET_2];
	
	if ( (0x00000001<<dataIndex) & (VoiceCommand_GetActionTable(VC_EVT_INDEX_0))->multiLayerBits[VC_MULTI_LAYER_SET_0])
	{
		if (dataIndex == rootIndex1)
		{
			return VC_MULTI_LAYER_SET_1;
		}
		else  if (dataIndex ==rootIndex2)
		{
			return VC_MULTI_LAYER_SET_2;
		}
		else
		{
			return VC_MULTI_LAYER_SET_0;
		}
	}
	else if (( (0x00000001<<dataIndex) &  (VoiceCommand_GetActionTable(VC_EVT_INDEX_0))->multiLayerBits[VC_MULTI_LAYER_SET_1]) 
			||( (0x00000001<<dataIndex) &  (VoiceCommand_GetActionTable(VC_EVT_INDEX_0))->multiLayerBits[VC_MULTI_LAYER_SET_2]) )
	{	
		return VC_MULTI_LAYER_SET_0;
	}
	else if ( (0x00000001<<dataIndex) &  (VoiceCommand_GetActionTable(VC_EVT_INDEX_1))->multiLayerBits[VC_MULTI_LAYER_SET_0]) 
	{
		return VC_MULTI_LAYER_SET_0;
	}		
	else
	{
		return VC_MULTI_LAYER_SET_0;
	}
	#else
		UNUSED(dataIndex);
		return VC_MULTI_LAYER_SET_0;
	#endif
}

PUBLIC U8 VoiceCommand_MultiLayerRootEvt(U8 vcSetNum)
{
	U8 rootIndex, vcMultiLayerEvt;

	rootIndex=((DRIVER_VOICE_COMMAND_DATA_TYPE CODE_PTR)GET_SECTOR_VOICE_DATA_ADDR(SECTOR_VOICE_COMMAND_DATA))->rootEvtNum[vcSetNum];
	vcMultiLayerEvt=(VoiceCommand_GetActionTable(VC_EVT_INDEX_0))->triggeredAction[rootIndex];
	return vcMultiLayerEvt;
}

PUBLIC BOOL VoiceCommand_GetData(DriverMsg XDATA_PTR msgPtr)
{
	if(gVC_ctl.dataBasePtr)
	{
		gVC_ctl.triggerBits = VoiceCommand_GetDataBits(msgPtr->msgBodyPtr.driverCmd.vcCmd.actionIndex, msgPtr->msgBodyPtr.driverCmd.vcCmd.id);
		LightDbgPrint("vc Original trigger bits %X", (U32)gVC_ctl.triggerBits);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

PUBLIC BOOL VoiceCommand_GetAPDData(DriverMsg XDATA_PTR msgPtr)
{
	LightDbgPrint("APD data base %X", (U32)gVC_ctl.apddataBasePtr);
	
	if(gVC_ctl.apddataBasePtr)
	{
		gVC_ctl.apdtriggerBits = VoiceCommand_GetAPDDataBits(msgPtr->msgBodyPtr.driverCmd.vcCmd.actionIndex);
		LightDbgPrint("APD trigger bits %X", (U32)VoiceCommand_GetAPDDataBits(msgPtr->msgBodyPtr.driverCmd.vcCmd.actionIndex));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


PUBLIC BOOL VoiceCommand_Start(void)
{
	DriverMsg XDATA_PTR cmdPtr;

	SYS_ReleaseTimer(&gDriver_ctl.resumeAppTimer);
	
	if(!gAUDIO_CtrlInfo.isVCEnabled)
		return TRUE;
	
	if(!gVC_ctl.triggerBits || !gVC_ctl.dataBasePtr)
		return TRUE;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
		return TRUE;
	
	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO))
	{
		if(!(gDriver_ctl.lightMonoModeMask & (0x01 << gDriver_ctl.dspLink)))
			return TRUE;
	}

	if(IS_DSP_BUSY)
		return FALSE;

	if(AUDIO_COMPONENT_IS_ACTIVE(AUDIO_LINE_IN) || AUDIO_COMPONENT_IS_ACTIVE(AUDIO_A2DP) /*|| AUDIO_COMPONENT_IS_ACTIVE(AUDIO_SCO)*/)
	{
		DRIVER_SendAppIdleToMailBox();
		return FALSE;
	}

	if (AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICEPROMPT) ||AUDIO_COMPONENT_IS_ACTIVE(AUDIO_RINGTONE) )
	{
		if(!AUDIO_CheckVoiceCmdDSPSpeed())
			return FALSE;
	}
	
	if(cmdPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
	{		
		OSMEM_memset_xdata((U8 XDATA_PTR)cmdPtr, 0, OSMEM1_BLKSIZE);

		AUDIO_ChangeVoiceCmdDSPSpeed();
		AUDIO_SetAudioOn(AUDIO_VOICECOMMAND);
		
		VC_STOP_FLAG = 0;

		switch(gVC_ctl.vcIdentificationMode)
		{
			case DEFAULT_VC_APD_MIXED_IDENTIFY:
				LightDbgPrint("Default VC+APD");
				LightDbgPrint("Original data base %X", (U32)gVC_ctl.dataBasePtr);
				LightDbgPrint("Original trigger event bits %X", (U32)gVC_ctl.triggerBits);
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.ena_map = gVC_ctl.triggerBits;
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.VC_Table_ptr = (U32)DSP_GetCODEAddr((U8 CODE_PTR)gVC_ctl.dataBasePtr);
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.adp_ena_map = gVC_ctl.apdtriggerBits;
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.adp_VC_Table_ptr = (U32)DSP_GetCODEAddr((U8 CODE_PTR)gVC_ctl.apddataBasePtr);
				break;
			case APD_VC_IDENTIFY:
				LightDbgPrint("APD only");
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.ena_map = 0;
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.VC_Table_ptr = (U32)0;
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.adp_ena_map = gVC_ctl.apdtriggerBits;
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.adp_VC_Table_ptr = (U32)DSP_GetCODEAddr((U8 CODE_PTR)gVC_ctl.apddataBasePtr);
				break;
			case DEFAULT_VC_IDENTIFY:
			default:
				LightDbgPrint("Default VC only");
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.ena_map = gVC_ctl.triggerBits;
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.VC_Table_ptr = (U32)DSP_GetCODEAddr((U8 CODE_PTR)gVC_ctl.dataBasePtr);
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.adp_ena_map =0;
				cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.adp_VC_Table_ptr = (U32)0;
				break;
		}

		AUDIO_SetAudioInOutToMailBoxCmd(AUDIO_VOICECOMMAND, &cmdPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.para.vc_start_para.audio_scenario);
		Mailbox_TaskOgfVC(cmdPtr, OGF_VC, OCF_VC_START);
		DRIVER_SendCmdToMailBox(cmdPtr);
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL VoiceCommand_Pause(void)
{
	DriverMsg XDATA_PTR msgPtr;

	if(!AUDIO_COMPONENT_IS_ACTIVE(AUDIO_VOICECOMMAND))
		return TRUE;

	if(IS_DSP_BUSY)
		return FALSE;
		
	if(IS_VOICE_COMMAND_OVER)
		return TRUE;

	if(msgPtr = (DriverMsg XDATA_PTR)OSMEM_Get(OSMEM_ptr1))
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)msgPtr, 0, OSMEM1_BLKSIZE);
		Mailbox_TaskOgfVC(msgPtr, OGF_VC, OCF_VC_STOP);
		DRIVER_SendCmdToMailBox(msgPtr);
		return TRUE;
	}
	return FALSE;
}

PUBLIC void VoiceCommand_Stopped(void)
{
	//Audio off
	AUDIO_SetAudioOff (AUDIO_VOICECOMMAND);

	VC_STOP_FLAG = 0;

	DRIVER_VoiceCommandOffCallback();

	SYS_SetTimer(&gDriver_ctl.resumeAppTimer, ONE_SEC);
}


PRIVATE void VoiceCommand_ChangeAPDDatabase(void)
{
	//if(gVC_ctl.vcIdentificationMode != DEFAULT_VC_IDENTIFY)
	{
		gVC_ctl.apddataBasePtr = (U32)((DRIVER_VOICE_COMMAND_APD_DATA_TYPE CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_VOICE_COMMAND_APD_DATA))->vcAddrTable.vcDataBasePtr;//+offset
		gVC_ctl.apddataMapPtr = (U32)((DRIVER_VOICE_COMMAND_APD_DATA_TYPE CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_VOICE_COMMAND_APD_DATA))->vcAddrTable.vcEvtDataMapPtr;//+offset
	}
}

PUBLIC void VoiceCommand_ChooseIdentificationMode(void)
{
	LightDbgPrint("VC identify mode %d", (U8)gVC_ctl.vcIdentificationMode);
	if(gVC_ctl.vcIdentificationMode != 
		(((DRIVER_VOICE_COMMAND_APD_DATA_TYPE CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_VOICE_COMMAND_APD_DATA))->vcMode)
	)
	{
		gVC_ctl.vcIdentificationMode = (((DRIVER_VOICE_COMMAND_APD_DATA_TYPE CODE_PTR)GET_SECTOR_BOUNDARY_ADDR(SECTOR_VOICE_COMMAND_APD_DATA))->vcMode);
		LightDbgPrint("VC identify 2");
		VoiceCommand_ChangeAPDDatabase();
	}
}


#endif
