
static U8 CODE HFP_at_command_code[HFP_AT_COMMAND_CODE_ARRAY_SIZE][HFP_AT_COMMAND_CODE_MAX_LENGTH] =
{
	{"AT+CKPD=200"},		
	{"AT+VGS"},
	{"AT+VGM"},
	{"AT+BRSF"},
	{"AT+CIND=?"},
	{"AT+CIND?"},
	{"AT+CMER="},
	{"AT+CHLD=?"},
	{"AT+CCWA"},
	{"AT+CLIP"},
	{"AT+NREC"},
	{"AT+BIA"},
	{"AT+CBC"},
	{"AT+COPS"},
	{"AT+CMEE"},
	{"AT+XAPL"},
	{"AT+XEVENT"},
	{"AT+IPHONEACCEV"},
	{"AT+BVRA"},
	{"AT+BLDN"},
	{"AT+CSCS"},
	{"AT+BAC"},
	{"AT+BCS"},
	{"AT+BCC"},
	{"ATA"},
	{"AT+CHUP"},
};

///////////////////////////////////////////////////////////
//	AT_MatchCommandCode:
//
//	Input:
//
//	Return value:
//
///////////////////////////////////////////////////////////
PRIVATE U8 AT_MatchCommandCode (U8 XDATA_PTR ptr1, U8 type)
{
	U8 i;
	
	while(*ptr1 == AT_CR)
	{
		ptr1++;
	}

	switch(type)
	{
		case HFP_AT_COMMAND_CODE:
			for (i = 0; i < HFP_AT_COMMAND_CODE_ARRAY_SIZE; i++)
			{
				if (AT_CompareString (ptr1, &HFP_at_command_code[i][0], HFP_AT_COMMAND_CODE_MAX_LENGTH) == 0)
				{
					return i;
				}		
			}
			break;
	}
	return AT_EOF;
}

PRIVATE U8 AT_FindCommandCode (U8 XDATA_PTR ptr1) 
{
	U8 index;

	if ( AT_EOF != (index = AT_MatchCommandCode(ptr1, HFP_AT_COMMAND_CODE )) )
	{
		return index;
	}
	else
	{
		return AT_EOF; 
	}
}

///////////////////////////////////////////////////////////
//
//	HFP AT Result String
//
///////////////////////////////////////////////////////////

static U8 CODE HFP_result_ok[]=
{
	6, AT_CR, AT_LF, 'O', 'K', AT_CR, AT_LF	
};

static U8 CODE HFP_result_error[]=
{
	9, AT_CR, AT_LF, 'E', 'R', 'R', 'O', 'R', AT_CR, AT_LF	
};

static U8 CODE HFP_result_ring[]=
{
	8, AT_CR, AT_LF, 'R', 'I', 'N', 'G', AT_CR, AT_LF
};

static U8 CODE HFP_result_brsf[]=
{
	13, AT_CR, AT_LF, '+', 'B', 'R', 'S', 'F', ':', '1', '0', '3', AT_CR, AT_LF
};

static U8 CODE HFP_result_brsf_codec_nego[]=
{
	13, AT_CR, AT_LF, '+', 'B', 'R', 'S', 'F', ':', '6', '1', '5', AT_CR, AT_LF
};

static U8 CODE HFP_result_cind_format[]=
{
#if 0
	81+50, AT_CR, AT_LF, 
	'+', 'C', 'I', 'N', 'D', ':', '(', '"', 's', 'e', 'r', 'v', 'i', 'c', 'e', '"', ',', '(', '0', '-',
	'1', ')', ')', ',', '(', '"', 'c', 'a', 'l', 'l', '"', ',', '(', '0', '-', '1', ')', ')', ',', '(', 
	'"', 'c', 'a', 'l', 'l', 's', 'e', 't', 'u', 'p', '"', ',', '(', '0', '-', '3', ')', ')', ',', '(',
	'"', 'c', 'a', 'l', 'l', 'h', 'e', 'l', 'd', '"', ',', '(', '0', '-', '2', ')', ')', 	
	
	',', '(', '"', 's', 'i', 'g', 'n', 'a', 'l', '"', ',', '(', '0', '-', '5', ')', ')', ',',
	'(', '"', 'r', 'o', 'a', 'm', '"', ',', '(', '0', '-', '1', ')', ')', ',',
	'(', '"', 'b', 'a', 't', 't', 'c', 'h', 'g', '"', ',', '(', '0', '-', '5', ')', ')',
	AT_CR, AT_LF
#else
	80, AT_CR, AT_LF, 
	'+', 'C', 'I', 'N', 'D', ':', '(', '"', 's', 'i', 'g', 'n', 'a', 'l',  '"', ',', '(', '0', '-',
	'5', ')', ')', ',', '(', '"', 'c', 'a', 'l', 'l', '"', ',', '(', '0', '-', '1', ')', ')', ',', '(', 
	'"', 'c', 'a', 'l', 'l', 's', 'e', 't', 'u', 'p', '"', ',', '(', '0', '-', '3', ')', ')', ',', '(',
	'"', 'c', 'a', 'l', 'l', 'h', 'e', 'l', 'd', '"', ',', '(', '0', '-', '2', ')', ')',
	AT_CR, AT_LF
#endif
};

static U8 CODE HFP_result_cind_status[]=
{
	//23, AT_CR, AT_LF, '+', 'C', 'I', 'N', 'D', ':', '0', ',', '0', ',', '0', ',', '0', ',', '5', ',', '0', ',', '2', AT_CR, AT_LF
	17, AT_CR, AT_LF, '+', 'C', 'I', 'N', 'D', ':', '5', ',', '0', ',', '0', ',', '0',  AT_CR, AT_LF
};

static U8 CODE HFP_result_cmer[]=
{
	9, AT_CR, AT_LF, '+', 'C', 'M', 'E', 'R', AT_CR, AT_LF
};

static U8 CODE HFP_result_chld_supported[]=
{
	19, AT_CR, AT_LF, '+', 'C', 'H', 'L', 'D', ':', '(', '0', ',', '1', ',', '2', ',', '3', ')', AT_CR, AT_LF
};

static U8 CODE HFP_result_bcs[]=
{
	8, AT_CR, AT_LF, '+', 'B', 'C', 'S', AT_CR, AT_LF
};

static U8 CODE HFP_result_bcs_1[]=
{
	10, AT_CR, AT_LF, '+', 'B', 'C', 'S', ':', '1', AT_CR, AT_LF
};

static U8 CODE HFP_result_bcs_2[]=
{
	10, AT_CR, AT_LF, '+', 'B', 'C', 'S', ':', '2', AT_CR, AT_LF
};

static U8 CODE HFP_result_ciev_3_1[]=
{
	13, AT_CR, AT_LF, '+', 'C', 'I', 'E', 'V', ':', '3', ',', '1', AT_CR, AT_LF
};

static U8 CODE HFP_result_ciev_3_0[]=
{
	13, AT_CR, AT_LF, '+', 'C', 'I', 'E', 'V', ':', '3', ',', '0', AT_CR, AT_LF
};

static U8 CODE HFP_result_ciev_2_1[]=
{
	13, AT_CR, AT_LF, '+', 'C', 'I', 'E', 'V', ':', '2', ',', '1', AT_CR, AT_LF
};

static U8 CODE HFP_result_ciev_2_0[]=
{
	13, AT_CR, AT_LF, '+', 'C', 'I', 'E', 'V', ':', '2', ',', '0', AT_CR, AT_LF
};

static U8 CODE HFP_result_clip[]=
{
	24, AT_CR, AT_LF, '+', 'C', 'L', 'I', 'P', ':', '"', '0', '1', '2', '3', '4', '5', '6', '7', '"', ',', '1', '2', '9', AT_CR, AT_LF 	
};

PRIVATE void HFP_SendAtResult(U8 linkIndex, U8 CODE_PTR code_array)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	U8 XDATA_PTR ptr1;

	ptr1 = RFCOMM_AllocProfileData( code_array[0], hfpLinkInfo->mmiProfileInfo.profileDesc->profileId );
	if(ptr1 != (U8 XDATA_PTR)NULL)
	{
		OSMEM_memcpy_xdata_code ( ( ptr1 + *ptr1 ) , code_array+1 , code_array[0]);
		RFCOMM_SendProfileData(linkIndex, ptr1);
	}
}

PRIVATE void HFP_IssueAudioGatewayeEvent(U8 linkIndex, U8 event_op, U8 status)
{
	HfpToMMIEvtParamType LDATA hfpToMMIParam;
	hfpToMMIParam.agEvt.status = status;
	MMI_HFP_EventHandler(linkIndex, event_op, &hfpToMMIParam);
}

PRIVATE void HFP_SlcEstablishmentDone(U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);

	hfpLinkInfo->mmiProfileInfo.state = HFP_SLC_CONNECTED;
	MMI_ActivateProfile(linkIndex, &hfpLinkInfo->mmiProfileInfo);
	if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE)
	{
		MMI_HFP_ConnectedHandle(linkIndex);
	}
	else if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE_HEADSET)
	{
		MMI_HSP_ConnectedHandle(linkIndex);
	}
}

PRIVATE void HFP_Cmd_SlcInitRoutine (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
						  
	switch( hfpLinkInfo->subState )
	{
		case HFP_SUBSTATE_SEND_PLUS_BRSF:
			HFP_SendAtResult(linkIndex, MMI_CODEC_NEGOTIATION_FEAT ? HFP_result_brsf_codec_nego : HFP_result_brsf);
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CIND_EQUAL_QUESTION;
			break;
		
		case HFP_SUBSTATE_SEND_PLUS_CIND_FORMAT:
			HFP_SendAtResult(linkIndex, HFP_result_cind_format);
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CIND_QUESTION;	
			break;
		
		case HFP_SUBSTATE_SEND_PLUS_CIND_STATUS:
			HFP_SendAtResult(linkIndex, HFP_result_cind_status);
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CMER_EQUAL;	
			break;
		
		case HFP_SUBSTATE_SLC_PROCESS_COMPLETE:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			HFP_SlcEstablishmentDone(linkIndex);
			HFP_CommandComplete(linkIndex);
			break;
	}
}

PRIVATE void HFP_Cmd_SlcInitRoutineHeadset (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	
	switch( hfpLinkInfo->subState )
	{
		case HSP_SUBSTATE_SEND_RING:
			HFP_SendAtResult(linkIndex, HFP_result_ring);
			hfpLinkInfo->subState = HSP_SUBSTATE_WAIT_AT_CKPD200;			
			SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, (U32)1*ONE_SEC);
			break;		
			
		case HSP_SUBSTATE_SLC_PROCESS_COMPLETE:
			HFP_SlcEstablishmentDone(linkIndex);
			HFP_CommandComplete(linkIndex);
			break;
	}
}

PRIVATE void HFP_Cmd_CodecSelectionRoutine(U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	
	switch( hfpLinkInfo->subState )
	{
		case HFP_SUBSTATE_SEND_PLUS_BCS:
			HFP_SendAtResult(linkIndex, (hfpLinkInfo->miscFlags & HFP_HF_USING_CODEC_MSBC) ? HFP_result_bcs_2 : HFP_result_bcs_1);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_ATCMD_BCS;
			break;
		
		case HFP_SUBSTATE_RECV_ATCMD_BCS:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			HFP_IssueAudioGatewayeEvent(linkIndex, HFP_AGEVT_TO_MMI_EST_SCO,
										(hfpLinkInfo->miscFlags & HFP_HF_USING_CODEC_MSBC) ? SCO_CODEC_MSBC : SCO_CODEC_CVSD);
			HFP_CommandComplete(linkIndex);	
			break;
	}
}

PRIVATE void HFP_Cmd_AutoCallSetupRoutine (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	
	switch( hfpLinkInfo->subState )
	{
		case HFP_SUBSTATE_WAIT_CALL_ANSWER:
			if(hfpLinkInfo->ost_command_ptr == NULL)
			{
				SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, 5*ONE_SEC);
			}
			break;
			
		case HFP_SUBSTATE_ACCEPT_CALL_SETUP:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			HFP_SendAtResult(linkIndex, HFP_result_ciev_2_1);
			HFP_SendAtResult(linkIndex, HFP_result_ciev_3_0);
			if (hfpLinkInfo->miscFlags & HFP_HF_SUPPORT_CODEC_NEGO)
			{
				HFP_SendAtResult(linkIndex, (hfpLinkInfo->miscFlags & HFP_HF_USING_CODEC_MSBC) ? HFP_result_bcs_2 : HFP_result_bcs_1);
				hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_ATCMD_BCS;
				break;
			}
		
		case HFP_SUBSTATE_RECV_ATCMD_BCS:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CALL_HANGUP;
			HFP_IssueAudioGatewayeEvent(linkIndex, HFP_AGEVT_TO_MMI_EST_SCO,
										(hfpLinkInfo->miscFlags & HFP_HF_USING_CODEC_MSBC) ? SCO_CODEC_MSBC : SCO_CODEC_CVSD);
			break;
			
		case HFP_SUBSTATE_REJECT_CALL_SETUP:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			HFP_SendAtResult(linkIndex, HFP_result_ciev_3_0);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CALL_SETUP;
			SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, 2*ONE_SEC);
			HFP_IssueAudioGatewayeEvent(linkIndex, HFP_AGEVT_TO_MMI_REJECT_CALL_SETUP, PROFILE_NO_ERROR);
			break;
		
		case HFP_SUBSTATE_ACCEPT_CALL_HANGUP:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			hfpLinkInfo->subState = HFP_SUBSTATE_CALL_HANGUP_COMPLETE;
			HFP_IssueAudioGatewayeEvent(linkIndex, HFP_AGEVT_TO_MMI_REL_SCO, PROFILE_NO_ERROR);
			break;
		
		case HFP_SUBSTATE_CALL_HANGUP_COMPLETE:
			if (!SCO_EXISTS(linkIndex))
			{
				HFP_SendAtResult(linkIndex, HFP_result_ciev_2_0);
				HFP_CommandComplete(linkIndex);	
			}
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
///
/// Profile Entry Functions
///
//////////////////////////////////////////////////////////////////////////

PUBLIC void HFP_TimeoutMonitor_AG (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);

	if ( SYS_IsTimerExpired(&hfpLinkInfo->ost_command_ptr) )
	{
		switch (hfpLinkInfo->subState)
		{
				case HFP_SUBSTATE_WAIT_CALL_SETUP:
				HFP_SendAtResult(linkIndex, HFP_result_ciev_3_1);
				HFP_SendAtResult(linkIndex, HFP_result_ring);
				HFP_SendAtResult(linkIndex, HFP_result_clip);
				hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CALL_ANSWER;
				break;
			
			case HFP_SUBSTATE_WAIT_CALL_ANSWER:
				HFP_SendAtResult(linkIndex, HFP_result_ring);
				HFP_SendAtResult(linkIndex, HFP_result_clip);
				break;
			
			case HSP_SUBSTATE_WAIT_AT_CKPD200:
				HFP_SendAtResult(linkIndex, HFP_result_ring);
				SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, (U32)2*ONE_SEC);
				break;
		}
	}	
}

PUBLIC void HFP_MainStateEntry_AG (U8 linkIndex)
{
	switch (HFP_GetLinkInfo(linkIndex)->currentCmd)
	{
		case HFP_CMD_INTERNAL_SLC_INIT_ROUTINE:
			HFP_Cmd_SlcInitRoutine(linkIndex);
			break;	
			
		case HFP_CMD_INTERNAL_SLC_INIT_ROUTINE_HEADSET:
			HFP_Cmd_SlcInitRoutineHeadset(linkIndex);
			break;	

		case HFP_AGCMD_FROM_MMI_CODEC_SELECTION:
			HFP_Cmd_CodecSelectionRoutine(linkIndex);
			break;	

		case HFP_AGCMD_FROM_MMI_AUTO_CALL_SETUP:
			HFP_Cmd_AutoCallSetupRoutine(linkIndex);
			break;
	}
}

PUBLIC void HFP_ProcessCmdFromMMI_AG (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	U8 XDATA_PTR cmdPtr;
	
	if ((cmdPtr = OSMQ_Get(&hfpLinkInfo->OSMQ_HFP_Command)) != (U8 XDATA_PTR)NULL)
	{
		if (hfpLinkInfo->currentCmd != HFP_NO_COMMAND)
		{
		 	OSMQ_PutFront(&hfpLinkInfo->OSMQ_HFP_Command, cmdPtr);
		}
		else
		{
			hfpLinkInfo->currentCmd = HFP_COMMAND_EVENT_CODE(cmdPtr);
			
			switch (hfpLinkInfo->currentCmd)
			{
				case HFP_AGCMD_FROM_MMI_CODEC_SELECTION:
					hfpLinkInfo->subState = HFP_SUBSTATE_SEND_PLUS_BCS;
					break;
				
				case HFP_AGCMD_FROM_MMI_AUTO_CALL_SETUP:
					hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CALL_SETUP;
					SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, ONE_SEC);
					break;
			}
			
			OSMEM_Put(cmdPtr);
		}
	}
}

///////////////////////////////////////////////////////////
//
//	HFP Command Code Handler
//
///////////////////////////////////////////////////////////

PRIVATE void AT_CheckCommandCode (U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	U8 index;

	index = AT_FindCommandCode(dataPtr);
	switch (index)
	{
		case HFP_AT_COMMAND_CKPD200:
			if (hfpLinkInfo->subState == HSP_SUBSTATE_SEND_RING || hfpLinkInfo->subState == HSP_SUBSTATE_WAIT_AT_CKPD200)
			{
				HFP_SendAtResult(linkIndex, HFP_result_ok);
				hfpLinkInfo->subState = HSP_SUBSTATE_SLC_PROCESS_COMPLETE;
			}
			else
			{
				goto label_send_result_error;
			}
			break;

		case HFP_AT_COMMAND_BRSF:
			if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_BRSF_CMD)
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_PLUS_BRSF;
			}
			else
			{
				goto label_send_result_error;
			}
			break;
		
		case HFP_AT_COMMAND_CIND_EQUAL_QUESTION:
			if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_CIND_EQUAL_QUESTION)
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_PLUS_CIND_FORMAT;
			}
			else
			{
				goto label_send_result_error;
			}
			break;
		
		case HFP_AT_COMMAND_CIND_QUESTION:
			if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_CIND_QUESTION)
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_PLUS_CIND_STATUS;
			}
			else
			{
				goto label_send_result_error;
			}
			break;
		
		case HFP_AT_COMMAND_CMER_EQUAL:
			if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_CMER_EQUAL)
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SLC_PROCESS_COMPLETE;
			}
			else
			{
				goto label_send_result_error;
			}
			break;
			
		case HFP_AT_COMMAND_CHLD_EQUAL_QUESTION:
			HFP_SendAtResult(linkIndex, HFP_result_chld_supported);
		case HFP_AT_COMMAND_VGM:
		case HFP_AT_COMMAND_CCWA:
		case HFP_AT_COMMAND_CLIP:
		case HFP_AT_COMMAND_NREC:
		case HFP_AT_COMMAND_BIA:
		case HFP_AT_COMMAND_CBC:
		case HFP_AT_COMMAND_COPS:
		case HFP_AT_COMMAND_CMEE:
		case HFP_AT_COMMAND_XEVENT:	
		case HFP_AT_COMMAND_CSCS:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			break;
			
		//case HFP_AT_COMMAND_XAPL:
		//	HFP_SendAtResult(linkIndex, HFP_result_error);
		//	break;
			
		case HFP_AT_COMMAND_BVRA:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			HFP_IssueAudioGatewayeEvent(linkIndex, HFP_AGEVT_TO_MMI_AT_PLUS_BVRA, PROFILE_NO_ERROR);
			break;
			
		case HFP_AT_COMMAND_BLDN:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			HFP_IssueAudioGatewayeEvent(linkIndex, HFP_AGEVT_TO_MMI_AT_PLUS_BLDN, PROFILE_NO_ERROR);
			break;
			
		case HFP_AT_COMMAND_VGS:
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			HFP_IssueAudioGatewayeEvent(linkIndex, HFP_AGEVT_TO_MMI_AT_PLUS_VGS, (U8)HFP_ASCII_TO_U16(&dataPtr[7]));
			break;
		
		case HFP_AT_COMMAND_BAC:
			hfpLinkInfo->miscFlags |= HFP_HF_SUPPORT_CODEC_NEGO;
			hfpLinkInfo->miscFlags |= HFP_HF_USING_CODEC_MSBC;
			HFP_SendAtResult(linkIndex, HFP_result_ok);
			break;
		
		case HFP_AT_COMMAND_BCS:
			if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_ATCMD_BCS)
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_RECV_ATCMD_BCS;
			}
			else
			{
				goto label_send_result_error;
			}
			break;
			
		case HFP_AT_COMMAND_BCC:
			if (hfpLinkInfo->miscFlags & HFP_HF_SUPPORT_CODEC_NEGO)
			{
				HFP_SendAtResult(linkIndex, HFP_result_ok);
				HFP_IssueAudioGatewayeEvent(linkIndex, HFP_AGEVT_TO_MMI_CODEC_CONNECTION, PROFILE_NO_ERROR);
			}
			else
			{
				goto label_send_result_error;
			}
			break;
			
		case HFP_AT_COMMAND_ATA:
			if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_CALL_ANSWER)
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_ACCEPT_CALL_SETUP;
			}
			else
			{
				goto label_send_result_error;
			}
			break;
		
		case HFP_AT_COMMAND_CHUP:
			if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_CALL_ANSWER)
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_REJECT_CALL_SETUP;
			}
			else if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_CALL_HANGUP)
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_ACCEPT_CALL_HANGUP;
			}
			else
			{
				goto label_send_result_error;
			}
			break;

		default:
			label_send_result_error:
			HFP_SendAtResult(linkIndex, HFP_result_error);
			break;
	}
	OSMEM_Put(dataPtr);
}

///////////////////////////////////////////////////////////
//	AT_SegmentAndEliminateSpace_AG:
//
//	Input:
//
//	Return value:
//
///////////////////////////////////////////////////////////
PUBLIC void AT_SegmentAndEliminateSpace_AG (U8 linkIndex, U8 XDATA_PTR ptr1)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	U16 length, realDataLength;
	U8 XDATA_PTR ptr2;
	U8 XDATA_PTR ptr3;

	length = ((ProfileDataEvent XDATA_PTR)ptr1)->dataLength;
	ptr2 = &((ProfileDataEvent XDATA_PTR)ptr1)->dataBody[0];
	
	while (length)
	{
		if( hfpLinkInfo->at_string_control.at_string_byte_num == 0 )
		{
			if(*ptr2 == 'A' && *(ptr2 +1) == 'T')
			{
				ptr3 = ptr2;
				realDataLength = 0;

				while (length && (!(*ptr3 == AT_CR )))
				{
					ptr3 ++;			
					length --;
					realDataLength ++;
				}
			
				if (length) //second CR match
				{
					if((ptr3 = SYS_MemoryGet(realDataLength+1)) != (U8 XDATA_PTR)NULL)
					{
						OSMEM_memcpy_xdata_xdata (ptr3, ptr2, realDataLength+1);
						AT_UpperCaseAndEliminateSpace(ptr3, realDataLength+1);
						AT_CheckCommandCode(linkIndex, ptr3);
					}
					ptr2 += (realDataLength+1);
					length -=1;
				}						
				else if(realDataLength)//second CR not found
				{
					if((ptr3 = SYS_MemoryGet(realDataLength)) != (U8 XDATA_PTR)NULL)
					{
						OSMEM_memcpy_xdata_xdata (ptr3, ptr2, realDataLength);
						hfpLinkInfo->at_string_control.at_string_recv_ptr = ptr3;
						hfpLinkInfo->at_string_control.at_string_byte_num = realDataLength;
					}
				}		   
			}
			else
			{
				ptr2 ++;
				length --;
			}
		}		
		else //wait continute state
		{
			ptr3 = ptr2;
			realDataLength = 0; 		
			while (length && (!(*ptr3 == AT_CR)))
			{
				ptr3 ++;
				length --;				
				realDataLength ++;					
			}			
			
			if (length) //second CR LF match
			{				
				if((ptr3 = SYS_MemoryGet(hfpLinkInfo->at_string_control.at_string_byte_num+realDataLength+1)) != (U8 XDATA_PTR)NULL)
				{
					OSMEM_memcpy_xdata_xdata (ptr3, hfpLinkInfo->at_string_control.at_string_recv_ptr, hfpLinkInfo->at_string_control.at_string_byte_num);
					OSMEM_memcpy_xdata_xdata (ptr3+hfpLinkInfo->at_string_control.at_string_byte_num, ptr2, realDataLength+1);
					OSMEM_Put(hfpLinkInfo->at_string_control.at_string_recv_ptr);
					AT_UpperCaseAndEliminateSpace(ptr3, hfpLinkInfo->at_string_control.at_string_byte_num+realDataLength+1);
					AT_CheckCommandCode(linkIndex, ptr3);
				}
				else
				{
					OSMEM_Put(hfpLinkInfo->at_string_control.at_string_recv_ptr);
				}
				hfpLinkInfo->at_string_control.at_string_byte_num = 0;							
				ptr2 += (realDataLength+1);
				length -=1;
			}			
			else if(realDataLength)//second CR LF not found
			{	
				if((ptr3 = SYS_MemoryGet(hfpLinkInfo->at_string_control.at_string_byte_num+realDataLength)) != (U8 XDATA_PTR)NULL)
				{
					OSMEM_memcpy_xdata_xdata (ptr3, hfpLinkInfo->at_string_control.at_string_recv_ptr, hfpLinkInfo->at_string_control.at_string_byte_num);
					OSMEM_memcpy_xdata_xdata (ptr3+hfpLinkInfo->at_string_control.at_string_byte_num, ptr2, realDataLength);
					OSMEM_Put(hfpLinkInfo->at_string_control.at_string_recv_ptr);
					hfpLinkInfo->at_string_control.at_string_byte_num += realDataLength;
				}
				else
				{
					OSMEM_Put(hfpLinkInfo->at_string_control.at_string_recv_ptr);
					hfpLinkInfo->at_string_control.at_string_byte_num = 0; 
				}
			}	
		}
	}
}
