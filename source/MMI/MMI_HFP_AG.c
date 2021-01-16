
#ifdef FAST_CONNECT_DONGLE
#include "MMI_FCD.h"
#endif

PRIVATE BOOL MMI_HFP_SendAGCmdByLink(U8 linkIndex, U8 opcode)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetCommandMessageMem(opcode);
	
	if (msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		HFP_CommandHandle(linkIndex, opcode, msgPtr);
		return TRUE;
	}
	return FALSE;
}

PUBLIC void MMI_HFP_Init_AG(void)
{
}

PUBLIC void MMI_HFP_CheckTimer_AG(U8 linkIndex)
{
	UNUSED(linkIndex);
}

PUBLIC void MMI_HFP_ConnectedHandle_AG(U8 linkIndex)
{
	LinkHistoryType XDATA_PTR linkListPtr = MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], TRUE, FALSE);
	if(linkListPtr != (LinkHistoryType XDATA_PTR)NULL)
	{
		linkListPtr->miscMask |= NVRAM_LINK_KEY_MISC_MASK_HFP;
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
	
	#ifdef AUTO_INCOMING_CALL
	MMI_HFP_SendAGCmdByLink(linkIndex, HFP_AGCMD_FROM_MMI_AUTO_CALL_SETUP);
	#endif
}

PUBLIC void MMI_HSP_ConnectedHandle_AG(U8 linkIndex)
{
	LinkHistoryType XDATA_PTR linkListPtr= MMI_LinkKey_SearchHistoryByBdAddr((U8 XDATA_PTR)&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], TRUE, FALSE);
	if(linkListPtr != (LinkHistoryType XDATA_PTR)NULL)
	{
		linkListPtr->miscMask |= NVRAM_LINK_KEY_MISC_MASK_HSP;
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}	
	
}

PUBLIC void MMI_HFP_ProfileDiscHandle_AG(U8 linkIndex)
{
		UNUSED(linkIndex);
}

PUBLIC void MMI_HFP_EventHandler_AG(U8 linkIndex, U8 evtCode, HfpToMMIEvtParamType XDATA_PTR hfpParamPtr)
{
	switch(evtCode)
	{
		case HFP_AGEVT_TO_MMI_AT_PLUS_VGS:
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportVolume(HCI_VEVT_OCF_FCD_VOL_HFP, hfpParamPtr->agEvt.status);
			#endif
			break;

		case HFP_AGEVT_TO_MMI_AT_PLUS_BVRA:
		case HFP_AGEVT_TO_MMI_AT_PLUS_BLDN:
			break;
		
		case HFP_AGEVT_TO_MMI_EST_SCO:
			gMMI_ctl.mmiInfo[linkIndex].linkPara.scoCodecType = hfpParamPtr->agEvt.status;
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportAction(HCI_VEVT_OCF_FCD_ACP_CALL);
			#endif
			MMI_HCI_SendSetupESCO(linkIndex, gMMI_nvram.scoPktPara.scoPktType);
			break;
		
		case HFP_AGEVT_TO_MMI_REL_SCO:
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportAction(HCI_VEVT_OCF_FCD_END_CALL);
			#endif
			MMI_HCI_SendRelSco(linkIndex);
			break;

		case HFP_AGEVT_TO_MMI_CODEC_CONNECTION:
			MMI_HFP_SendAGCmdByLink(linkIndex, HFP_AGCMD_FROM_MMI_CODEC_SELECTION);
			break;
		
		#ifdef AUTO_INCOMING_CALL
		case HFP_AGEVT_TO_MMI_REJECT_CALL_SETUP:
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportAction(HCI_VEVT_OCF_FCD_REJ_CALL);
			#endif
			break;
		#endif
	}
}

PUBLIC void MMI_HFP_FakeMediaEvt_AG(U16 id)
{
	UNUSED(id);
}

PUBLIC void MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls_AG(U8 linkIndex)
{
	UNUSED(linkIndex);
}

PUBLIC void MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld_AG(U8 linkIndex)
{
	UNUSED(linkIndex);
}