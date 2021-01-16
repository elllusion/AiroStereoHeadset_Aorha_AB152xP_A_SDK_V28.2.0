
#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];

static U8 CODE HFP_Indicators[HFP_INDICATOR_ARRAY_SIZE][HFP_INDICATOR_MAX_LENGTH] =
{
	{"service\""},
	{"call\""},
	{"callsetup\""},
	{"callheld\""},
	{"signal\""},
	{"roam\""},
	{"battchg\""}
};

static U8 CODE HFP_error_result_code[HFP_ERROR_RESULT_CODE_ARRAY_SIZE][HFP_ERROR_RESULT_CODE_MAX_LENGTH] =
{
	{"NOCARRIER"},		//0
	{"BUSY"},
	{"NOANSWER"},
	{"DELAYED"},
	{"BLACKLISTED"}, 
};

static U8 CODE HFP_result_code[HFP_RESULT_CODE_ARRAY_SIZE][HFP_RESULT_CODE_MAX_LENGTH] = 
{
	{"RING"},			//0
	{"+BRSF"},
	{"+CIND"},
	{"+CHLD"},
	{"+VGM"},
	{"+VGS"},			//5
	{"OK"},	
	{"ERROR"},
	{"+CIEV"},
	{"+BVRA"},
	{"+COPS"},			//10
	{"+BTRH"},
	{"+BSIR"},
	{"+CNUM"},
	{"+CME"},
	{"+CLCC"},			//15
	{"+CLIP"},
	{"+CCWA"},
	{"+BINP"},
	{"+XAPL"},
	{"+BCS"},			//20
	{"+BIND"},
	{"+APLSIRI"}
};

///////////////////////////////////////////////////////////
//	AT_MatchResultCode:
//
//	Input:
//
//	Return value:
//
///////////////////////////////////////////////////////////
PRIVATE U8 AT_MatchResultCode (U8 XDATA_PTR ptr1, U8 type)
{
	U8 i;
	
	while(*ptr1 == AT_CR || *ptr1 == AT_LF)
	{
		ptr1++;
	}

	switch(type)
	{
		case HFP_INDICATORS:
			for (i = 0; i < HFP_INDICATOR_ARRAY_SIZE; i++)
			{
				if (AT_CompareString (ptr1, &HFP_Indicators[i][0], HFP_INDICATOR_MAX_LENGTH) == 0)
				{
					return i;
				}		
			}
			break;

		case HFP_RESULT_CODE:
			for (i = 0; i < HFP_RESULT_CODE_ARRAY_SIZE; i++)
			{
				if (AT_CompareString (ptr1, &HFP_result_code[i][0], HFP_RESULT_CODE_MAX_LENGTH) == 0)
				{
					return i;
				}		
			}
			break;

		case HFP_ERROR_RESULT_CODE:
			for (i = 0; i < HFP_ERROR_RESULT_CODE_ARRAY_SIZE; i++)
			{
				if (AT_CompareString (ptr1, &HFP_error_result_code[i][0], HFP_ERROR_RESULT_CODE_MAX_LENGTH) == 0)
				{
					return i;
				}		
			}
			break;
	}
	return AT_EOF;
}

PRIVATE U8 AT_FindResultCode (U8 XDATA_PTR ptr1) 
{
	U8 index;

	if 	( AT_EOF != (index = AT_MatchResultCode (ptr1, HFP_RESULT_CODE)) )
	{
		return index;
	}
	else if ( AT_EOF != (index = AT_MatchResultCode (ptr1, HFP_ERROR_RESULT_CODE)) )
	{
		return index + HFP_RESULT_CODE_ARRAY_SIZE;
	}
	else
	{
		return AT_EOF; 
	}
}

///////////////////////////////////////////////////////////
//
//	HFP AT Command String
//
///////////////////////////////////////////////////////////
#ifdef AB1520U
static U8 CODE HFP_CodecAVAIL[] = {SCO_CODEC_CVSD};
#else
static U8 CODE HFP_CodecAVAIL[] = {SCO_CODEC_CVSD, SCO_CODEC_MSBC};
#endif

static U8 CODE HFP_at_ckpd_200[] = 
{
	12, 'A', 'T', '+', 'C', 'K', 'P', 'D', '=', '2', '0', '0', AT_CR	//To reserve 1-byte length for RFCOMM CRC
};

static U8 CODE HFP_at_answer[]=
{
	4, 'A','T','A',AT_CR	
};

static U8 CODE HFP_at_brsf[]=
{
	14, 'A','T','+','B','R','S','F','=',AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,AT_CR
};

static U8 CODE HFP_at_bvra[]=
{
	10, 'A','T','+','B','V','R','A','=','0',AT_CR	
};

static U8 CODE HFP_at_chup[]=
{
	 8, 'A','T','+','C','H','U','P',AT_CR	
};

static U8 CODE HFP_at_chld_equal_ques[]=
{
	10, 'A','T','+','C','H','L','D','=','?',AT_CR
};

static U8 CODE HFP_at_cind_equal_ques[]=
{
	10, 'A','T','+','C','I','N','D','=','?',AT_CR
};

static U8 CODE HFP_at_cind_ques[]=
{
	 9, 'A','T','+','C','I','N','D','?',AT_CR
};

static U8 CODE HFP_at_cmee[]=
{
	10, 'A','T','+','C','M','E','E','=','0',AT_CR
};

static U8 CODE HFP_at_cmer[]=
{
	16, 'A','T','+','C','M','E','R','=','3',',','0',',','0',',','1',AT_CR
};

static U8 CODE HFP_at_vgs[]=
{
	10, 'A','T','+','V','G','S','=','0','0',AT_CR	
};

static U8 CODE HFP_at_vgm[]=
{
	10, 'A','T','+','V','G','M','=','0','0',AT_CR	
};


static U8 CODE HFP_at_ccwa[]=
{
	10, 'A','T','+','C','C','W','A','=','1',AT_CR
};

static U8 CODE HFP_at_BAC[]=
{
	14, 'A','T','+','B','A','C','=','0',AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,AT_CR
};

static U8 CODE HFP_at_BCS[]=
{
	 9, 'A','T','+','B','C','S','=','0',AT_CR
};

static U8 CODE HFP_at_BCC[]=
{
	 7, 'A','T','+','B','C','C',AT_CR
};

static U8 CODE HFP_at_BIA[]=
{
	8, 'A','T','+','B','I','A','=',AT_CR	//actual parameters determined by code
};

static U8 CODE HFP_at_atd_digit[] =
{
	21, 'A','T','D',
	AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,
	AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,
	AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,
	AT_CR,AT_CR, AT_CR	  //CALLER_ID_DIGIT_NO = 16 digits, ';', and AT_CR
};

static U8 CODE HFP_at_atd_number[]=
{
	11, 'A','T','D','>',AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,AT_CR	 //65536  data sheets and ;
};

static U8 CODE HFP_at_bldn[]=
{
	 8, 'A','T','+','B','L','D','N',AT_CR	
};

static U8 CODE HFP_at_nrec[]=
{
	10, 'A','T','+','N','R','E','C','=','0',AT_CR	
};

static U8 CODE HFP_at_binp[]=
{
	10, 'A','T','+','B','I','N','P','=','1',AT_CR
};

static U8 CODE HFP_at_clcc[]=
{
	 8, 'A','T','+','C','L','C','C',AT_CR
};

static U8 CODE HFP_at_cscs[]=
{
	16, 'A','T','+','C','S','C','S','=','\"','U','T','F','-','8','\"',AT_CR	
};

static U8 CODE HFP_at_clip[]=
{
	10, 'A','T','+','C','L','I','P','=','1',AT_CR	
};

static U8 CODE HFP_at_cnum[]=
{
	 8, 'A','T','+','C','N','U','M',AT_CR
};

static U8 CODE HFP_at_cnum_equal_ques[]=
{
	10, 'A','T','+','C','N','U','M','=','?',AT_CR
};

static U8 CODE HFP_at_cops[]=
{
	12, 'A','T','+','C','O','P','S','=','3',',','0',AT_CR	
};

static U8 CODE HFP_at_cops_ques[]=
{
	 9, 'A','T','+','C','O','P','S','?',AT_CR	
};

static U8 CODE HFP_at_vts[]=
{
	 9, 'A','T','+','V','T','S','=','0',AT_CR
};

//CDMA and PDC only
static U8 CODE HFP_at_btrh[]=
{
	10, 'A','T','+','B','T','R','H','=','0',AT_CR	
};

static U8 CODE HFP_at_chld[]=
{
	11, 'A','T','+','C','H','L','D','=','0',AT_CR,AT_CR
};

static U8 CODE HFP_at_xpal[]=
{
	21, 'A','T','+','X','A','P','L','=','A','B','-','1','2','-','0','1','0','0',',','1',AT_CR
};

static U8 CODE HFP_at_iphoneaccev[]=
{
	21, 'A','T','+','I','P','H','O','N','E','A','C','C','E','V','=','1',',','1',',','3',AT_CR
};

static U8 CODE HFP_at_xevent_bat_inquiry[]=
{
	23, 'A','T','+','X','E','V','E','N','T','=','A','B','-','1','2','-','0','1','0','0',',','1',AT_CR
};

static U8 CODE HFP_at_xevent_bat_send[]=
{
	16, 'A','T','+','X','E','V','E','N','T','=','1',',','1',',','3',AT_CR
};

static U8 CODE HFP_at_aplsiri[]=
{
	12, 'A','T','+','A','P','L','S','I','R','I','?',AT_CR
};

static U8 CODE HFP_at_aplefm[]=
{
	12, 'A','T','+','A','P','L','E','F','M','=','0',AT_CR
};

static U8 CODE HFP_at_bind_avails[]=
{
	10, 'A','T','+','B','I','N','D','=','2',AT_CR
};

static U8 CODE HFP_at_bind_equal_ques[]=
{
	10, 'A','T','+','B','I','N','D','=','?',AT_CR
};

static U8 CODE HFP_at_bind_ques[]=
{
	 9, 'A','T','+','B','I','N','D','?',AT_CR
};

static U8 CODE HFP_at_biev_bat_send[]=
{
	14, 'A','T','+','B','I','E','V','=','2',',',AT_CR,AT_CR,AT_CR,AT_CR
};

static U8 CODE_PTR CODE at_code_ptr[HFP_TABLE_LOOKUP_ATCMD_COUNT-HFP_CMD_OFFSET]=
{
	HFP_at_bvra,   				//HFP_ATCMD_INIT_VOICE_DIAL // voice dial 
	HFP_at_bvra,   				//HFP_ATCMD_CANCEL_VOICE_DIAL // cancel voice dial 
	HFP_at_bldn,				//HFP_ATCMD_LAST_NUMBER_REDIAL // last number redial 
	HFP_at_chup,				//HFP_ATCMD_CANCEL_OUTGOING_CALL // cancel outgoing call,  hang up the call, and reject the call 
	HFP_at_chup,				//HFP_ATCMD_REJECT_CALL
	HFP_at_chup,				//HFP_ATCMD_END_CALL
	HFP_at_answer,				//HFP_ATCMD_ACCEPT_CALL // accept call
	HFP_at_vgs,					//HFP_ATCMD_ADJUST_VOLUME // vgs
	HFP_at_vgm,					//HFP_ATCMD_ADJUST_MIC_GAIN // vsm
	HFP_at_nrec,				//HFP_ATCMD_SET_NREC // ec nr
	HFP_at_ckpd_200,			//HFP_ATCMD_CKPD_200 // ckpd 200
	HFP_at_xpal, 				//HFP_ATCMD_IPHONE_BATTERY_INQUIRY // iphone battery enable
	HFP_at_iphoneaccev,			//HFP_ATCMD_IPHONE_BATTERY_SEND // iphone battery update
	HFP_at_xevent_bat_inquiry, 	//HFP_ATCMD_XEVENT_BATTERY_INQUIRY // xevent battery enable
	HFP_at_xevent_bat_send,		//HFP_ATCMD_XEVENT_BATTERY_SEND // xevent battery update
	HFP_at_biev_bat_send,		//HFP_ATCMD_BIEV_BATTERY_SEND
	HFP_at_atd_digit, 			//HFP_ATCMD_PLACE_CALL_WITH_PHONE_NUMBER //call with phone number        
	HFP_at_atd_number, 			//HFP_ATCMD_MEMORY_DIALING // call with memory dialing
	HFP_at_chld, 				//HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING // multiparty call
	HFP_at_cops,				//HFP_ATCMD_QUERY_OPERATOR_SELECTION
	HFP_at_cnum,				//HFP_ATCMD_GET_SUBSCRIBER_NUMBER //get subscribe number
	HFP_at_cscs,				//HFP_ATCMD_SELECT_CHAR_SET_UTF8 //select character set
	HFP_at_clip, 				//HFP_ATCMD_ENABLE_CLI_NOTIFICATION //enable clip
	HFP_at_clip, 				//HFP_ATCMD_DISABLE_CLI_NOTIFICATION //disable clip
	HFP_at_vts, 				//HFP_ATCMD_SEND_DTMF // send DTFM
	HFP_at_ccwa,				//HFP_ATCMD_ENABLE_CALL_WAITING_NOTIFICATION // Call waiting line
	HFP_at_ccwa,				//HFP_ATCMD_DISABLE_CALL_WAITING_NOTIFICATION //Disable call waiting line
	HFP_at_cmee,				//HFP_ATCMD_ENABLE_COMMAND_ERROR // Enable error code
	HFP_at_cmee, 				//HFP_ATCMD_DISABLE_COMMAND_ERROR // Disable error code
	HFP_at_clcc, 				//HFP_ATCMD_QUERY_LIST_OF_CURRENT_CALLS // query list of current call
	HFP_at_binp, 				//HFP_ATCMD_ATTACH_LAST_VOICE_TAG_RECORDED //attach voice tag
	HFP_at_cnum_equal_ques, 	//HFP_ATCMD_TEST_GET_SUBSCRIBER_NUMBER // test get suscriber numbers
	HFP_at_BAC,					//HFP_ATCMD_AVAILABLE_CODEC
	HFP_at_BCS,					//HFP_ATCMD_SELECT_CODEC
	HFP_at_BCC,					//HFP_ATCMD_CODEC_CONNECTION // codec connection
	HFP_at_BIA,					//HFP_ATCMD_ACTIVATE_INDICATORS //indicators activation
	NULL,						//HFP_ATCMD_PUT_INCOMING_CALL_ON_HOLD
	HFP_at_btrh,				//HFP_ATCMD_ACCEPT_HELD_INCOMING_CALL
	HFP_at_btrh,				//HFP_ATCMD_REJECT_HELD_INCOMING_CALL
	HFP_at_btrh,				//HFP_ATCMD_READ_CURRENT_RSP_AND_HOLD_STATUS
	HFP_at_aplsiri,				//HFP_ATCMD_APPLE_SIRI_STATUS
	HFP_at_aplefm,				//HFP_ATCMD_ENABLE_APPLE_EYES_FREE_MODE
	HFP_at_aplefm,				//HFP_ATCMD_DISABLE_APPLE_EYES_FREE_MODE
};

PRIVATE U8 HFP_GetATCmdLength (U8 XDATA_PTR at_cmd)
{
	U8 i;
	U8 XDATA_PTR code_array;

	code_array = at_cmd + 3;
	while( *code_array++ !=AT_CR ){}
	i = code_array - at_cmd;

	return i;
}

PRIVATE U8 XDATA_PTR HFP_GetATCmd(HfpLinkInfo XDATA_PTR hfpLinkInfo)
{
	U8 XDATA_PTR sendCmdPtr = OSMEM_Get(OSMEM_ptr1);
	U8 CODE_PTR codePtr = at_code_ptr[hfpLinkInfo->currentCmd-HFP_CMD_OFFSET];
	
	if(sendCmdPtr != (U8 XDATA_PTR)NULL)
	{
		OSMEM_memcpy_xdata_code(sendCmdPtr, codePtr, codePtr[0]+1);
	}
	return sendCmdPtr;

}

PRIVATE void HFP_Set_AvailableCodec(U8 XDATA_PTR sendCmdPtr)
{
	U8 i;
	U8 XDATA_PTR ptr;

	ptr = &sendCmdPtr[8];
	for ( i = 0; i < sizeof(HFP_CodecAVAIL); i++ )
	{
		ptr[0] = '0' + HFP_CodecAVAIL[i];
		ptr[1] = ',';
		ptr += 2;
	}
	ptr[-1] = AT_CR;
}

PRIVATE void HFP_Cmd_AvailableCodec(U8 XDATA_PTR atCmdPtr)
{
	OSMEM_memcpy_xdata_code(atCmdPtr, HFP_at_BAC, HFP_at_BAC[0]+1);

	HFP_Set_AvailableCodec(atCmdPtr);

	atCmdPtr[0] = HFP_GetATCmdLength(atCmdPtr+1);
}

PRIVATE U8 HFP_FindMatchCodec(U8 CodecID)
{
 	U8 i;

	CodecID -= '0';
	for ( i = 0; i < sizeof(HFP_CodecAVAIL); i++ )
	{
		if ( CodecID == HFP_CodecAVAIL[i] )
		{
			return i;
		}		
	}
	return AT_EOF;
}

PRIVATE BOOL HFP_Cmd_ActivateIndicators(HfpLinkInfo XDATA_PTR hfpLinkInfo, U8 XDATA_PTR sendCmdPtr)
{
	U8 i, pos, maxIndex;

	if ((maxIndex = MAX_HFP_CIND_INDEX) > 16)
		maxIndex = 16;
	
	for (; maxIndex > 0; maxIndex--)
	{
		if ((hfpLinkInfo->cindBitMask & U16_BIT_MASK((maxIndex - 1))) == 0)
			break;
	}
	
	if (maxIndex == 0)
		return FALSE;
		
	for (pos = 8, i = 0; i < maxIndex; pos += 2, i++)
	{
		sendCmdPtr[pos] = (hfpLinkInfo->cindBitMask & U16_BIT_MASK(i)) ? '1' : '0';
		sendCmdPtr[pos+1] = ',';
	}
	
	sendCmdPtr[pos-1] = AT_CR;
	sendCmdPtr[0] = pos - 1;
	return TRUE;
}

PRIVATE void HFP_SetTimer(HfpLinkInfo XDATA_PTR hfpLinkInfo)
{
	#ifdef DBG_UART_ENABLE_HFP
	if(hfpLinkInfo->currentCmd == HFP_NO_COMMAND)
	{
		ASSERT(FALSE);
	}
	#endif
	SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, ((U32)gMMI_nvram.timerCtl.HFPSLCTimer) * ONE_SEC);
}

PRIVATE void HFP_SendAtCommand(U8 linkIndex, U8 CODE_PTR code_array)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	U8 XDATA_PTR ptr1;
	U8 XDATA_PTR ptr2;

	ptr1 = RFCOMM_AllocProfileData( code_array[0], hfpLinkInfo->mmiProfileInfo.profileDesc->profileId );
	if(ptr1 != (U8 XDATA_PTR)NULL)
	{
		OSMEM_memcpy_xdata_code ( ( ptr1 + *ptr1 ) , code_array+1 , code_array[0]);
		RFCOMM_SendProfileData(linkIndex, ptr1);
		
		switch(hfpLinkInfo->currentCmd)
		{
			case HFP_ATCMD_IPHONE_BATTERY_INQUIRY:
			case HFP_ATCMD_XEVENT_BATTERY_INQUIRY:
				SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, 2 * ONE_SEC);
				if(hfpLinkInfo->currentCmd == HFP_ATCMD_IPHONE_BATTERY_INQUIRY)
				{
					ptr2 = ( ptr1 + *ptr1 );
					if(MMI_IPHONE_SIRI_FEAT)
						ptr2[19] += 4;
					if(MMI_IPHONE_DEVICE_DOCKED_FEAT)
						ptr2[19] += 2;
				}			
				break;
			case HFP_CMD_INTERNAL_SLC_INIT_ROUTINE:
				break;
			case HFP_ATCMD_IPHONE_BATTERY_SEND:
			case HFP_ATCMD_XEVENT_BATTERY_SEND:
			case HFP_ATCMD_BIEV_BATTERY_SEND:
				SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, HALF_SEC);
				break;
			case HFP_ATCMD_INIT_VOICE_DIAL:
			case HFP_ATCMD_CANCEL_VOICE_DIAL:
				SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, 2 * ONE_SEC);
				break;
			case HFP_ATCMD_LAST_NUMBER_REDIAL:
				SYS_SetTimer(&hfpLinkInfo->ost_command_ptr, 5 * HALF_SEC);
				break;
			default:
				HFP_SetTimer(hfpLinkInfo);
				break;
		}
	
		LightDbgPrintWithConnectionHandle (
			HCI_VOPCODE_VOGF(HCI_VENDOR_OGF_UART_HFP_DBGPRINT,HCI_VEVT_OCF_HFP_DBGPRINT_TX),
		 	MMI_GetConnHandleByLinkIndex(linkIndex),
			ptr1 + *ptr1);
	}
}

PRIVATE void HFP_IssueCompleteEvent(U8 linkIndex, U8 event_op, U8 status)
{
	HfpToMMIEvtParamType LDATA hfpToMMIParam;
	hfpToMMIParam.cmdCplEvt.param = status;
	MMI_HFP_EventHandler(linkIndex, event_op, &hfpToMMIParam);
}

PRIVATE void HFP_Cmd_Query_Operator_Selection (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	switch( hfpLinkInfo->subState )
	{
		case HFP_SUBSTATE_WAIT_COPS_OK:
			break;
		case HFP_SUBSTATE_GET_COPS_OK:
			HFP_SendAtCommand(linkIndex, HFP_at_cops_ques);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_PLUS_COPS;
			break;

		case HFP_SUBSTATE_WAIT_PLUS_COPS:
			break;
			
		case HFP_SUBSTATE_GET_PLUS_COPS:
			HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, PROFILE_NO_ERROR );
			HFP_CommandComplete(linkIndex);
			break;
	}
}

PRIVATE void HFP_PushCmd(U8 linkIndex, U8 cmdCode)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	if(hfpLinkInfo->pushedCmdCtlPtr == (HFP_PUSH_CMD_CTL_TYPE XDATA_PTR)NULL)
	{
		hfpLinkInfo->pushedCmdCtlPtr = SYS_MemoryGet(sizeof(HFP_PUSH_CMD_CTL_TYPE));
		ASSERT(hfpLinkInfo->pushedCmdCtlPtr != (HFP_PUSH_CMD_CTL_TYPE XDATA_PTR)NULL);
		OSMEM_memset_xdata((U8 XDATA_PTR)hfpLinkInfo->pushedCmdCtlPtr, 0, sizeof(HFP_PUSH_CMD_CTL_TYPE));
	}

	if( (cmdCode > HFP_NO_COMMAND) && (cmdCode < HFP_TABLE_LOOKUP_ATCMD_COUNT || cmdCode == HFP_CMD_INTERNAL_SLC_VGS_CMD))
	{
		hfpLinkInfo->pushedCmdCtlPtr->cmdCode[hfpLinkInfo->pushedCmdCtlPtr->pushIndex] = cmdCode;
		hfpLinkInfo->pushedCmdCtlPtr->pushIndex++;
		hfpLinkInfo->pushedCmdCtlPtr->pushIndex %= HFP_PUSH_CMD_NO;
	}
}

PRIVATE void HFP_PopCmd(U8 linkIndex)
{
	U8 cmdCode, index;
	MMIMsgType XDATA_PTR msgPtr;
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	if(hfpLinkInfo->pushedCmdCtlPtr && !OSMQ_Entries(&hfpLinkInfo->OSMQ_HFP_Command))
	{
		index = hfpLinkInfo->pushedCmdCtlPtr->popIndex;
		cmdCode = hfpLinkInfo->pushedCmdCtlPtr->cmdCode[index];
		
		if(cmdCode == HFP_CMD_INTERNAL_SLC_VGS_CMD)
			cmdCode = HFP_ATCMD_ADJUST_VOLUME;
		
		if(msgPtr = MMI_GetCommandMessageMem(cmdCode))
		{
			if(cmdCode == HFP_ATCMD_ADJUST_VOLUME)
			{
				*(U8 XDATA_PTR)&msgPtr->msgBodyPtr = MMI_HFP_GET_VGS(linkIndex);
			}
			HFP_CommandHandle(linkIndex, cmdCode, msgPtr);
			
			hfpLinkInfo->pushedCmdCtlPtr->cmdCode[index] = HFP_NO_COMMAND;
			hfpLinkInfo->pushedCmdCtlPtr->popIndex++;
			hfpLinkInfo->pushedCmdCtlPtr->popIndex %= HFP_PUSH_CMD_NO;
		}

		if(hfpLinkInfo->pushedCmdCtlPtr->popIndex == hfpLinkInfo->pushedCmdCtlPtr->pushIndex)
		{
			SYS_MemoryRelease(&hfpLinkInfo->pushedCmdCtlPtr);
		}
	}
}

PRIVATE void HFP_SlcEstablishmentDone(U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);

	hfpLinkInfo->mmiProfileInfo.state = HFP_SLC_CONNECTED;
	MMI_ActivateProfile(linkIndex, &hfpLinkInfo->mmiProfileInfo);
	if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE)
	{
		MMI_HFP_ConnectedHandle(linkIndex);

		if(MMI_CALL_WAITING_3WAYCALLING_FEAT && (MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_THREEWAY_CALLING) )
		{
			HFP_PushCmd(linkIndex, HFP_ATCMD_ENABLE_CALL_WAITING_NOTIFICATION);
		}
		if(MMI_CLI_PRESENTATION_FEAT)
		{
			HFP_PushCmd(linkIndex, HFP_ATCMD_SELECT_CHAR_SET_UTF8);
			HFP_PushCmd(linkIndex, HFP_ATCMD_ENABLE_CLI_NOTIFICATION);
		}
		if(MMI_ECNR_FEAT && MMI_DISABLE_AG_ECNR_IN_SCO && (MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_EC_NR))
		{
			HFP_PushCmd(linkIndex, HFP_ATCMD_SET_NREC);
		}
		if(!(gMMI_nvram.mmiHfpCINDMask & MMI_HFP_CIND_SHOWALL))
		{
			HFP_PushCmd(linkIndex, HFP_ATCMD_ACTIVATE_INDICATORS);
		}
		if(MMI_IPHONE_BATTERY_DISPLAY_FEAT)
		{
			HFP_PushCmd(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY);
		}
		else if(MMI_XEVENT_BATTERY_DISPLAY_FEAT)
		{
			HFP_PushCmd(linkIndex, HFP_ATCMD_XEVENT_BATTERY_INQUIRY);
		}
		else //for MMI to start BIEV battery report if exists
		{
			HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY, HFP_BATTERY_REPORT_NONE);
		}
		HFP_PushCmd(linkIndex, HFP_CMD_INTERNAL_SLC_VGS_CMD);
	}
	else if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE_HEADSET)
	{
		MMI_HSP_ConnectedHandle(linkIndex);
	}
}

PRIVATE void HFP_Cmd_SlcInitRoutine (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	U8 XDATA_PTR atCmdPtr;
						  
	switch( hfpLinkInfo->subState )
	{
		//To obtain correct index
		//HTC may cause this fail when we give 0D 0D ... in one payload.	
		case HFP_SUBSTATE_SEND_BRSF_CMD:
			HFP_SetTimer(hfpLinkInfo);
			atCmdPtr = OSMEM_Get(OSMEM_ptr1);
			OSMEM_memcpy_xdata_code(atCmdPtr, HFP_at_brsf, HFP_at_brsf[0]+1);
			HFP_U16_To_ASCII(&atCmdPtr[9], (U16)gMMI_nvram.mmiFeatureCtl.HFP);
			atCmdPtr[0] = HFP_GetATCmdLength(atCmdPtr+1);
			HFP_SendAtCommand(linkIndex, atCmdPtr);
			OSMEM_Put(atCmdPtr);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_PLUS_BRSF;
			break;
		
		case HFP_SUBSTATE_GET_BRSF_OK:
			if(MMI_CODEC_NEGOTIATION_FEAT && (MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_CODEC_NEGOTIATION))
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_BAC_CMD;
			} 
			else
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_CIND_EQU;
			}
			break;
			
		case HFP_SUBSTATE_SEND_BAC_CMD:
			atCmdPtr = OSMEM_Get(OSMEM_ptr1);
			if(atCmdPtr)
			{
				HFP_Cmd_AvailableCodec(atCmdPtr);
				HFP_SendAtCommand(linkIndex, atCmdPtr);
				OSMEM_Put(atCmdPtr);
				hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_BAC_OK;
			}
			break;
			
		case HFP_SUBSTATE_SEND_CIND_EQU:
			HFP_SendAtCommand(linkIndex, HFP_at_cind_equal_ques);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_PLUS_CIND;
			break;
			
		case HFP_SUBSTATE_GET_CIND_OK:
			HFP_SendAtCommand(linkIndex, HFP_at_cind_ques);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_2ND_PLUS_CIND;	
			break;
			
		case HFP_SUBSTATE_GET_2ND_CIND_OK:
			HFP_SendAtCommand(linkIndex, HFP_at_cmer);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CMER_OK;	 
			break;
		
		case HFP_SUBSTATE_GET_CMER_OK:
			if(MMI_CALL_WAITING_3WAYCALLING_FEAT && (MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_THREEWAY_CALLING))
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_CHLD_CMD;
			}
			else if(MMI_HF_INDICATORS_FEAT && (MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_HF_INDICATORS))
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_BIND_CMD;
			}
			else
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SLC_PROCESS_COMPLETE;
			}
			break;

		case HFP_SUBSTATE_SEND_CHLD_CMD:
			HFP_SendAtCommand(linkIndex, HFP_at_chld_equal_ques);	
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_PLUS_CHLD;
			break;
			
		case HFP_SUBSTATE_GET_CHLD_OK:
			if(MMI_HF_INDICATORS_FEAT && (MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_HF_INDICATORS))
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_BIND_CMD;
			}
			else
			{
				hfpLinkInfo->subState = HFP_SUBSTATE_SLC_PROCESS_COMPLETE;
			}
			break;
			
		case HFP_SUBSTATE_SEND_BIND_CMD:
			HFP_SendAtCommand(linkIndex, HFP_at_bind_avails);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_BIND_OK;
			break;
		
		case HFP_SUBSTATE_GET_BIND_OK:
			HFP_SendAtCommand(linkIndex, HFP_at_bind_equal_ques);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_2ND_BIND_OK;
			break;
		
		case HFP_SUBSTATE_GET_2ND_BIND_OK:
			HFP_SendAtCommand(linkIndex, HFP_at_bind_ques);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_3RD_BIND_OK;
			break;
		
		case HFP_SUBSTATE_GET_3RD_BIND_OK:
			hfpLinkInfo->subState = HFP_SUBSTATE_SLC_PROCESS_COMPLETE;
			break;
			
		case HFP_SUBSTATE_SLC_PROCESS_COMPLETE:
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
		case HSP_SUBSTATE_SEND_CKPD200:
			HFP_SetTimer(hfpLinkInfo);
			HFP_SendAtCommand(linkIndex,  HFP_at_ckpd_200);
			hfpLinkInfo->subState = HSP_SUBSTATE_WAIT_CKPD200_OK;
			break;
		case HSP_SUBSTATE_WAIT_CKPD200_OK:
			break;
		case HSP_SUBSTATE_SLC_PROCESS_COMPLETE:
			OSMQ_FlushAndReleaseMemoryBlock(&hfpLinkInfo->OSMQ_HFP_Command);
			HFP_SlcEstablishmentDone(linkIndex);
			HFP_CommandComplete(linkIndex);	
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
///
/// Profile Entry Functions
///
//////////////////////////////////////////////////////////////////////////

PUBLIC void HFP_TimeoutMonitor_HF (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	
	if ( SYS_IsTimerExpired(&hfpLinkInfo->ost_command_ptr) )
	{		
		#ifdef DBG_UART_ENABLE_HFP
		LightDbgPrint("HFP - Watchdog Timeout %d, %x", (U8)linkIndex, (U8)hfpLinkInfo->currentCmd);	
		#endif

		switch (hfpLinkInfo->currentCmd)
		{
			case HFP_ATCMD_IPHONE_BATTERY_INQUIRY:
				if (MMI_XEVENT_BATTERY_DISPLAY_FEAT)
				{
					hfpLinkInfo->currentCmd = HFP_ATCMD_XEVENT_BATTERY_INQUIRY;
					HFP_SendAtCommand(linkIndex, HFP_at_xevent_bat_inquiry);
					return;
				}
				/* fallthrough *//*comment to prevent coverity warning*/
			case HFP_ATCMD_XEVENT_BATTERY_INQUIRY:
				HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY, HFP_BATTERY_REPORT_NONE);
				break;
			case HFP_ATCMD_IPHONE_BATTERY_SEND:
			case HFP_ATCMD_XEVENT_BATTERY_SEND:
			case HFP_ATCMD_BIEV_BATTERY_SEND:
				//Do not issue event
				break;
			case HFP_ATCMD_INIT_VOICE_DIAL:
				//HTC ButterflyS and HTC M8 had no response to AT+BVRA 
				HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_INIT_VOICE_DIAL, HFP_EVENT_TO_MMI_BVRA_TIMEOUT);
				break;
			case HFP_ATCMD_CANCEL_VOICE_DIAL:
				//HTC ButterflyS and HTC M8 had no response to AT+BVRA 
				HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_CANCEL_VOICE_DIAL, HFP_EVENT_TO_MMI_BVRA_TIMEOUT);
				break;
			case HFP_ATCMD_LAST_NUMBER_REDIAL:
				//Samsung J7 had no response "OK" to AT+BLDN 
				HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_LAST_NUMBER_REDIAL, HFP_EVENT_TO_MMI_BVRA_TIMEOUT);
				break;
			case HFP_CMD_INTERNAL_SLC_INIT_ROUTINE:
			case HFP_CMD_INTERNAL_SLC_INIT_ROUTINE_HEADSET:				
			default:
				HFP_DisconnectRequest(linkIndex, &hfpLinkInfo->mmiProfileInfo);
				break;
		}
		HFP_CommandComplete(linkIndex);
	}
}

PUBLIC void HFP_MainStateEntry_HF (U8 linkIndex)
{
	switch (HFP_GetLinkInfo(linkIndex)->currentCmd)
	{
		case HFP_CMD_INTERNAL_SLC_INIT_ROUTINE:
			HFP_Cmd_SlcInitRoutine(linkIndex);
			break;	
			
		case HFP_CMD_INTERNAL_SLC_INIT_ROUTINE_HEADSET:
			HFP_Cmd_SlcInitRoutineHeadset(linkIndex);
			break;	

		case HFP_ATCMD_QUERY_OPERATOR_SELECTION:
			HFP_Cmd_Query_Operator_Selection(linkIndex);
			break;
			
		case HFP_NO_COMMAND:
			HFP_PopCmd(linkIndex);
			break;
		
		default:
			break;
	}
}

PUBLIC void HFP_ProcessCmdFromMMI_HF (U8 linkIndex)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	U8 XDATA_PTR cmdPtr;
	U8 para0, recievedOpCode;
	U8 XDATA_PTR sendCmdPtr;
	U8 XDATA_PTR src;		//for place phone number using
	U8 dst_index;			//for place phone number using
	U8 src_data_length;		//for place phone number using

	if ((cmdPtr = OSMQ_Get(&hfpLinkInfo->OSMQ_HFP_Command)) == (U8 XDATA_PTR)NULL)
		return;
		

	recievedOpCode = HFP_COMMAND_EVENT_CODE(cmdPtr);

	if(recievedOpCode == HFP_CMD_FROM_MMI_RING_GET)
	{
		hfpLinkInfo->miscFlags |= HFP_GET_MMI_CMD_NO_UPDATE_RING;
		goto PUT_CMD_MEMORY;
	}
	else if(recievedOpCode == HFP_CMD_FROM_MMI_EXIT_INCOMING)
	{
		hfpLinkInfo->miscFlags &= (~HFP_GET_MMI_CMD_NO_UPDATE_RING);
		goto PUT_CMD_MEMORY;
	}
	
	if(recievedOpCode == HFP_ATCMD_CODEC_CONNECTION && hfpLinkInfo->currentCmd == HFP_ATCMD_CODEC_CONNECTION)
	{
		goto PUT_CMD_MEMORY;
	}
	
	if(recievedOpCode == HFP_ATCMD_SELECT_CODEC && hfpLinkInfo->currentCmd == HFP_ATCMD_INIT_VOICE_DIAL)
	{
		//HTC ButterflyS and HTC M8 had no response to AT+BVRA 
		HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, PROFILE_NO_ERROR);
		HFP_CommandComplete(linkIndex);		
	}

	if (hfpLinkInfo->currentCmd != HFP_NO_COMMAND)
	{
		OSMQ_PutFront(&hfpLinkInfo->OSMQ_HFP_Command, cmdPtr);
		return;
	}

	if(recievedOpCode >= HFP_ATCMD_FROM_SDK_MASK)
	{
		if((sendCmdPtr = SYS_MemoryGet(((MMIMsgType XDATA_PTR)cmdPtr)->msgBodyPtr.hfpCmd.vendorCmd.atStringLen + 1)) != (U8 XDATA_PTR)NULL)
		{
			OSMEM_memcpy_xdata_xdata(sendCmdPtr, &((MMIMsgType XDATA_PTR)cmdPtr)->msgBodyPtr.hfpCmd.vendorCmd, ((MMIMsgType XDATA_PTR)cmdPtr)->msgBodyPtr.hfpCmd.vendorCmd.atStringLen + 1);
		}
		else
		{
			OSMQ_PutFront(&hfpLinkInfo->OSMQ_HFP_Command, cmdPtr);
			return;		
		}
		
		hfpLinkInfo->currentCmd = recievedOpCode;
		HFP_SendAtCommand(linkIndex, sendCmdPtr);

		goto PUT_DATA_MEMORY;
	}

	//According to HSP Spec Ver1.2 - 4.2.1 Using In-Band Ringing, if in-band ringtone
	//is ringing, AG SHALL NOT send "Ring" in HSP 1.2. For this reason, MMI cannot recognize 
	//the in-band ringtone smartly. However, if user tries to press a key to stop the ringing,
	//our system HAS to be very smart to send CKPD=200 instead of all the other HANDSFREE commands.
	//I think this transformation should be implemented in HFP, not in MMI. That's why we need
	//the following codes.
	para0 = cmdPtr[HFP_AT_COMMAND_PARA0_OFFSET];
	hfpLinkInfo->currentCmd = recievedOpCode;

	if( recievedOpCode >= HFP_TABLE_LOOKUP_ATCMD_COUNT || (sendCmdPtr = HFP_GetATCmd(hfpLinkInfo))== (U8 XDATA_PTR)NULL)
	{
		hfpLinkInfo->currentCmd = HFP_NO_COMMAND;
		OSMEM_Put(cmdPtr);
		return;
	}

	switch (hfpLinkInfo->currentCmd)
	{
		case HFP_ATCMD_ADJUST_MIC_GAIN:	
		case HFP_ATCMD_ADJUST_VOLUME:
			if (para0 > 9)
			{
				sendCmdPtr[8] = 0x31;
				sendCmdPtr[9] = HFP_DIGIT_TO_CHAR(para0 % 10);
			}
			else
			{
				sendCmdPtr[8] = HFP_DIGIT_TO_CHAR(para0);
				sendCmdPtr[9] = AT_CR;	
			}
			hfpLinkInfo->atCmdParaFromMMI = para0;
			break;
		
		case HFP_ATCMD_CANCEL_VOICE_DIAL:
			sendCmdPtr[9] = 0x30;
			break;
		case HFP_ATCMD_INIT_VOICE_DIAL:
			sendCmdPtr[9] = 0x31;
			break;
		
		case HFP_ATCMD_SEND_DTMF:
			sendCmdPtr[8] = para0;
			break;
		
		case HFP_ATCMD_ENABLE_CALL_WAITING_NOTIFICATION:
			sendCmdPtr[9] = 0x31;
			break;
		case HFP_ATCMD_DISABLE_CALL_WAITING_NOTIFICATION:
			sendCmdPtr[9] = 0x30;
			break;
		
		case HFP_ATCMD_ENABLE_COMMAND_ERROR:
		case HFP_ATCMD_DISABLE_COMMAND_ERROR:
			sendCmdPtr[9] = 0x31;
			break;
		
		//************** SPECIAL **************
		case HFP_ATCMD_QUERY_OPERATOR_SELECTION:
			//HFP_Cmd_Query_Operator_Selection(linkIndex);
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_COPS_OK; //jump into state machine
			break;
		//************** SPECIAL **************

		case HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING:
			if (para0 > 19) 	//2x
			{
				sendCmdPtr[9] = '2';
				sendCmdPtr[10] = HFP_DIGIT_TO_CHAR(para0 % 10);
			}
			else if (para0 > 9) //1x
			{
				sendCmdPtr[9] = '1';
				sendCmdPtr[10] = HFP_DIGIT_TO_CHAR(para0 % 10);
			}
			else												//0, 1, 2, 3, 4
			{
				sendCmdPtr[9] = HFP_DIGIT_TO_CHAR(para0);
				sendCmdPtr[10] = AT_CR;
			}
			if(MMI_SUPPORT_HFP_VERSION16_FEAT &&  sendCmdPtr[8] == 0x33)		  //zabeth
			{
				sendCmdPtr[9] = 0x33;
			}
			break;

		case HFP_ATCMD_PLACE_CALL_WITH_PHONE_NUMBER: 					
			src = cmdPtr + HFP_AT_COMMAND_PARA0_OFFSET;
			dst_index = 4;
			src_data_length = *src++;
			do
			{
				sendCmdPtr[dst_index++] = (*src++);
			} while( --src_data_length );
			sendCmdPtr[dst_index] = ';';
			break;
	
		case HFP_ATCMD_MEMORY_DIALING:
			*HFP_U16_To_ASCII(&sendCmdPtr[5], *(U16 XDATA_PTR)&cmdPtr[HFP_AT_COMMAND_PARA0_OFFSET]) = ';';
			break;

		case HFP_ATCMD_ACCEPT_HELD_INCOMING_CALL:
			sendCmdPtr[9] = 0x31;
			break;
			
		case HFP_ATCMD_REJECT_HELD_INCOMING_CALL:
			sendCmdPtr[9] = 0x32;
			break;
			
		case HFP_ATCMD_READ_CURRENT_RSP_AND_HOLD_STATUS:
			sendCmdPtr[8] = '?';
			sendCmdPtr[9] = AT_CR;
			break;

		case HFP_ATCMD_DISABLE_CLI_NOTIFICATION:
			sendCmdPtr[9] = 0x30;
			break;
	
		case HFP_ATCMD_IPHONE_BATTERY_SEND:
			sendCmdPtr[20] = HFP_DIGIT_TO_CHAR(para0);
			break;
		
		case HFP_ATCMD_XEVENT_BATTERY_SEND:
			sendCmdPtr[15] = HFP_DIGIT_TO_CHAR(para0);
			break;
		
		case HFP_ATCMD_BIEV_BATTERY_SEND:
			HFP_U16_To_ASCII(&sendCmdPtr[11], para0);
			break;
		
		case HFP_ATCMD_CODEC_CONNECTION:
			if(!MMI_CODEC_NEGOTIATION_FEAT || !(MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_CODEC_NEGOTIATION))
			{
				HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, HFP_EVENT_TO_MMI_ERROR_RESULT_CODE_RESPONSE);
				hfpLinkInfo->currentCmd = HFP_NO_COMMAND;
				goto PUT_DATA_MEMORY;
			}
			break;

		case HFP_ATCMD_ACTIVATE_INDICATORS:
			if (!HFP_Cmd_ActivateIndicators(hfpLinkInfo, sendCmdPtr))
			{
				hfpLinkInfo->currentCmd = HFP_NO_COMMAND;
				goto PUT_DATA_MEMORY;
			}
			break;

		case HFP_ATCMD_SET_NREC:
			if(!(MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_EC_NR))
			{
				hfpLinkInfo->currentCmd = HFP_NO_COMMAND;
				goto PUT_DATA_MEMORY;
			}
			break;
		case HFP_ATCMD_ENABLE_APPLE_EYES_FREE_MODE:
			sendCmdPtr[11] = '1';
			break;
		case HFP_ATCMD_AVAILABLE_CODEC:
			HFP_Set_AvailableCodec(sendCmdPtr);
			break;
		case HFP_ATCMD_SELECT_CODEC:
			sendCmdPtr[8] = HFP_DIGIT_TO_CHAR(para0);
			break;
	}
	sendCmdPtr[0] = HFP_GetATCmdLength(sendCmdPtr+1);
	HFP_SendAtCommand(linkIndex, sendCmdPtr);
	
PUT_DATA_MEMORY:
	OSMEM_Put(sendCmdPtr);
PUT_CMD_MEMORY:
	OSMEM_Put(cmdPtr);
}

///////////////////////////////////////////////////////////
//
//	HFP Result Code Handler
//
///////////////////////////////////////////////////////////

PRIVATE void HFP_Result_Code_RING(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	if(!(HFP_GetLinkInfo(linkIndex)->miscFlags & HFP_GET_MMI_CMD_NO_UPDATE_RING))
	{	
		MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_RING_INDICATION, (HfpToMMIEvtParamType XDATA_PTR)NULL);
	}
	UNUSED(dataPtr);
}

PRIVATE void HFP_Result_Code_Plus_BRSF(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE && hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_PLUS_BRSF)
	{
		hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_BRSF_OK;
		MMI_HFP_SET_AGOPTION(linkIndex, HFP_ASCII_TO_U16(&dataPtr[6]));
	}
}

PRIVATE void HFP_TransferIndicators(HfpLinkInfo XDATA_PTR hfpLinkInfo, U8 XDATA_PTR ptr1, HfpToMMIEvtParamType XDATA_PTR hfpToMMIParamPtr)
{	
	U8 i, index, maxIndex;
	BOOL isLhsQuote = TRUE, showIndicator;
	U8 XDATA_PTR cindMsgPtr;

	hfpLinkInfo->cindBitMask = 0xFFFF;
	OSMEM_memset_xdata((U8 XDATA_PTR)&hfpToMMIParamPtr->cievOrderEvt.ind[0], 0, HFP_CIEVIND_NO);

	if ((maxIndex = MAX_HFP_CIND_INDEX) > 16)
		maxIndex = 16;
	
	for (i = 1, cindMsgPtr = ptr1 + 2; *cindMsgPtr != AT_CR && i < maxIndex; cindMsgPtr++)
	{
		if (*cindMsgPtr == '"')
		{
			if (isLhsQuote)			  //LHS-quote
			{
				cindMsgPtr++;

				showIndicator = FALSE;
				if ( AT_EOF != (index = AT_MatchResultCode ( cindMsgPtr, HFP_INDICATORS )) )
				{
					hfpToMMIParamPtr->cievOrderEvt.ind[index] = i;
					switch (index)
					{
						case HFP_CIND_SERVICE:
						case HFP_CIND_SIGNAL:
						case HFP_CIND_ROAM:
						case HFP_CIND_BATTCHG:
							if ((gMMI_nvram.mmiHfpCINDMask & (U8_BIT_MASK(index) | MMI_HFP_CIND_SHOWALL)) == 0)
								break;
						default:
							showIndicator = TRUE;
							break;
					}
				}
				else
				{
					if (gMMI_nvram.mmiHfpCINDMask & (MMI_HFP_CIND_SHOWEXT | MMI_HFP_CIND_SHOWALL))
						showIndicator = TRUE;
				}

				if (!showIndicator)
				{
					hfpLinkInfo->cindBitMask &= ~(U16_BIT_MASK((i-1)));
				}

				i++;
			}

			isLhsQuote = !isLhsQuote;
		}
	}
}

PRIVATE void HFP_Result_Code_Plus_CIND(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	U8 i;
	U8 XDATA_PTR index_of_ptr1;
	HfpToMMIEvtParamType LDATA hfpToMMIParam;
	
	if ( dataPtr[6] == '(' )					//HFP_AT_COMMAND_CODE_PLUS_CIND_EQUAL_QUESTION
	{
		if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE && hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_PLUS_CIND)
		{
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CIND_OK;	
		}
		//Transfer to index of indicators
		HFP_TransferIndicators(hfpLinkInfo, dataPtr, &hfpToMMIParam);
		LightDbgPrintWithConnectionHandle (
			HCI_VOPCODE_VOGF(HCI_VENDOR_OGF_UART_HFP_DBGPRINT,HCI_VEVT_OCF_HFP_DBGPRINT_PLUS_CIND_ORDER),
			MMI_GetConnHandleByLinkIndex(linkIndex),
			&hfpToMMIParam.cievOrderEvt.ind[0]);	
		MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_INDICATOR_ORDER, &hfpToMMIParam);
	}
	else				   					//HFP_AT_COMMAND_CODE_PLUS_CIND_QUESTION
	{
		if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE && hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_2ND_PLUS_CIND)
		{
			hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_2ND_CIND_OK;	
		}
		
		index_of_ptr1 = &dataPtr[2];
		i = 0;
		while( *index_of_ptr1 != AT_CR )
		{
			if ( *index_of_ptr1 == ',' ) //count the number of indicator value
			{	
				hfpToMMIParam.cindEvt.indicatorVal[i] = HFP_CHAR_TO_DIGIT(*(index_of_ptr1-1));
				i++;
			}
			index_of_ptr1++;
			
			if(i == HFP_CIEVIND_NO - 1)
				break;
		}
		hfpToMMIParam.cindEvt.indicatorVal[i] = HFP_CHAR_TO_DIGIT(*index_of_ptr1);
		hfpToMMIParam.cindEvt.indicatorNo = i+1;
		MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_INDICATION_VALUES, &hfpToMMIParam);
	}
}

PRIVATE void HFP_Result_Code_Plus_CHLD(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE && hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_PLUS_CHLD)
	{
		hfpLinkInfo->subState = HFP_SUBSTATE_WAIT_CHLD_OK;
	}
	UNUSED(dataPtr);
}

PRIVATE void HFP_Result_Code_Plus_VGM(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;

	hfpToMMIEvtParam.vgmEvt.level = (U8)HFP_ASCII_TO_U16(&dataPtr[5]);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_VGM_UPDATE_INDICATION, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_Plus_VGS(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;

	hfpToMMIEvtParam.vgsEvt.level = (U8)HFP_ASCII_TO_U16(&dataPtr[5]);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_VGS_UPDATE_INDICATION, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_OK(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE)
	{
		switch(hfpLinkInfo->subState)
		{
			case HFP_SUBSTATE_WAIT_BRSF_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_GET_BRSF_OK;
				break;
			case HFP_SUBSTATE_WAIT_BAC_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_CIND_EQU;
				break;
			case HFP_SUBSTATE_WAIT_CIND_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_GET_CIND_OK;
				break;
			case HFP_SUBSTATE_WAIT_2ND_CIND_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_GET_2ND_CIND_OK;
				break;
			case HFP_SUBSTATE_WAIT_CMER_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_GET_CMER_OK;
				break;
			case HFP_SUBSTATE_WAIT_CHLD_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_GET_CHLD_OK;
				break;
			case HFP_SUBSTATE_WAIT_BIND_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_GET_BIND_OK;
				break;
			case HFP_SUBSTATE_WAIT_2ND_BIND_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_GET_2ND_BIND_OK;
				break;
			case HFP_SUBSTATE_WAIT_3RD_BIND_OK:
				hfpLinkInfo->subState = HFP_SUBSTATE_GET_3RD_BIND_OK;
				break;
		}
	}
	else if(hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE_HEADSET)
	{
		if(hfpLinkInfo->subState == HSP_SUBSTATE_WAIT_CKPD200_OK)
			hfpLinkInfo->subState = HSP_SUBSTATE_SLC_PROCESS_COMPLETE;
	}
	else if(hfpLinkInfo->currentCmd == HFP_ATCMD_QUERY_OPERATOR_SELECTION)
	{
		if(hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_COPS_OK)
			hfpLinkInfo->subState = HFP_SUBSTATE_GET_COPS_OK;
	}
	else if(hfpLinkInfo->currentCmd == HFP_ATCMD_XEVENT_BATTERY_INQUIRY)
	{
		HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY, HFP_BATTERY_REPORT_XEVENT);
		HFP_CommandComplete(linkIndex);	
	}
	else if(hfpLinkInfo->currentCmd == HFP_ATCMD_IPHONE_BATTERY_INQUIRY)
	{
		HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY, HFP_BATTERY_REPORT_IPHONE);
		HFP_CommandComplete(linkIndex);	
	}
	else if(hfpLinkInfo->currentCmd == HFP_ATCMD_ADJUST_VOLUME || hfpLinkInfo->currentCmd == HFP_ATCMD_ADJUST_MIC_GAIN)
	{
		HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, hfpLinkInfo->atCmdParaFromMMI);
		HFP_CommandComplete(linkIndex);	
	}
	else if(hfpLinkInfo->currentCmd >= HFP_ATCMD_FROM_SDK_MASK)
	{
		if(api_profile_func[API_GRP_PF_CB_HFP_AT_RESULT_CODE_OK])
		{
			((API_PROFILE_HFP_RESULT_CODE_OK_FUNC)api_profile_func[API_GRP_PF_CB_HFP_AT_RESULT_CODE_OK])(linkIndex, hfpLinkInfo->currentCmd);
		}		
		HFP_CommandComplete(linkIndex);	
	}
	else if(hfpLinkInfo->currentCmd != HFP_NO_COMMAND)
	{
		//Other commands are all waiting for [OK] or [ERROR]
		HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, PROFILE_NO_ERROR);
		HFP_CommandComplete(linkIndex);	
	}
	UNUSED(dataPtr);
}

PRIVATE void HFP_Result_Code_ERROR(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	switch(hfpLinkInfo->currentCmd)
	{
		case HFP_CMD_INTERNAL_SLC_INIT_ROUTINE:
			if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_BAC_OK)
			{ /* mantis 8777: false setting of codec negotiation in AG's BRSF result code */
				MMI_HFP_CLEAR_AGOPTION(linkIndex, MMI_AG_OPTION_CODEC_NEGOTIATION);
				hfpLinkInfo->subState = HFP_SUBSTATE_SEND_CIND_EQU;
			}
			else if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_BIND_OK)
			{ /* to prevent false setting of HF indicators in AG's BRSF result code */
				MMI_HFP_CLEAR_AGOPTION(linkIndex, MMI_AG_OPTION_HF_INDICATORS);
				hfpLinkInfo->subState = HFP_SUBSTATE_SLC_PROCESS_COMPLETE;
			}
			break;
		case HFP_ATCMD_XEVENT_BATTERY_INQUIRY:
			HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY, HFP_BATTERY_REPORT_NONE);
			HFP_CommandComplete(linkIndex);			
			break;
		case HFP_ATCMD_IPHONE_BATTERY_INQUIRY:
			if(MMI_XEVENT_BATTERY_DISPLAY_FEAT)
			{
				hfpLinkInfo->currentCmd = HFP_ATCMD_XEVENT_BATTERY_INQUIRY;
				HFP_SendAtCommand(linkIndex, HFP_at_xevent_bat_inquiry);
			}
			else
			{
				HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY, HFP_BATTERY_REPORT_NONE);
				HFP_CommandComplete(linkIndex);
			}		
			break;
		case HFP_ATCMD_ADJUST_VOLUME:
		case HFP_ATCMD_ADJUST_MIC_GAIN:
			HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, VGS_VGM_ERROR);
			HFP_CommandComplete(linkIndex);			
			break;
		case HFP_ATCMD_INIT_VOICE_DIAL:
		case HFP_ATCMD_CANCEL_VOICE_DIAL:
			HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, HFP_EVENT_TO_MMI_ERROR_RESULT_CODE_RESPONSE);
			HFP_CommandComplete(linkIndex);				
			break;
		default:
			if(hfpLinkInfo->currentCmd >= HFP_ATCMD_FROM_SDK_MASK)
			{
				if(api_profile_func[API_GRP_PF_CB_HFP_AT_RESULT_CODE_ERROR])
				{
					((API_PROFILE_HFP_RESULT_CODE_ERROR_FUNC)api_profile_func[API_GRP_PF_CB_HFP_AT_RESULT_CODE_ERROR])(linkIndex, hfpLinkInfo->currentCmd);
				}		
				HFP_CommandComplete(linkIndex);	
			}
			else if(hfpLinkInfo->currentCmd != HFP_NO_COMMAND)
			{
				//Other commands are all waiting for [OK] or [ERROR]
				HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, HFP_EVENT_TO_MMI_ERROR_RESULT_CODE_RESPONSE);
				HFP_CommandComplete(linkIndex);				
			}
			break;
	}
	UNUSED(dataPtr);
}

PRIVATE void HFP_Result_Code_CIEV(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	
	hfpToMMIEvtParam.cievEvt.index = HFP_ASCII_TO_U16(&dataPtr[6]);
	hfpToMMIEvtParam.cievEvt.val = HFP_ASCII_TO_U16(HFP_FindComma(&dataPtr[6]) + 1);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_INDICATOR_UPDATE_INDICATION, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_Plus_BVRA(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	
	if(hfpLinkInfo->currentCmd == HFP_ATCMD_INIT_VOICE_DIAL || hfpLinkInfo->currentCmd == HFP_ATCMD_CANCEL_VOICE_DIAL)
	{
		//HTC ButterflyS and HTC M8 had no response to AT+BVRA 
		HFP_IssueCompleteEvent(linkIndex, hfpLinkInfo->currentCmd, PROFILE_NO_ERROR);
		HFP_CommandComplete(linkIndex);
	}
	hfpToMMIEvtParam.bvraEvt.voiceRecog = HFP_CHAR_TO_DIGIT(dataPtr[6]);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_BVRA_INDICATION, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_Plus_COPS(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	if(hfpLinkInfo->currentCmd == HFP_ATCMD_QUERY_OPERATOR_SELECTION && hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_PLUS_COPS)
	{
		hfpLinkInfo->subState = HFP_SUBSTATE_GET_PLUS_COPS;
	}	
	hfpToMMIEvtParam.copsEvt.para1 = dataPtr[6];
	hfpToMMIEvtParam.copsEvt.para1 = 0;
	hfpToMMIEvtParam.copsEvt.para1 = dataPtr[10];
	MMI_HFP_EventHandler(linkIndex, HFP_ATCMD_QUERY_OPERATOR_SELECTION, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_Plus_BTRH (U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	if (dataPtr[8] == '0')
	{
		HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_PUT_INCOMING_CALL_ON_HOLD, 0);
	}
	else if (dataPtr[8] == '1')
	{
		HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_ACCEPT_HELD_INCOMING_CALL, 1);																					
	}
	else
	{
		HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_REJECT_HELD_INCOMING_CALL, 2);
	}
}

PRIVATE void HFP_Result_Code_Plus_BSIR(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	
	hfpToMMIEvtParam.bsirEvt.agSuportIR = HFP_CHAR_TO_DIGIT(dataPtr[6]);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_SET_INBAND_RING_TONE, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_Plus_CNUM(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;

	U8 XDATA_PTR 	index_of_string_start;
	U8 XDATA_PTR 	index_of_ptr1;
	U8 				phone_nmber_length;
	
	index_of_ptr1 = dataPtr;	
	
	//Copy phone number, type, service fields 	
	
	while( *index_of_ptr1++ != '"' );

	index_of_string_start = index_of_ptr1;		//point to the first nmber

	while( *index_of_ptr1++ != '"' );			//index_of_ptr1 point to the comma or AT_CR
	
	phone_nmber_length = index_of_ptr1 - index_of_string_start - 1;	   	// -1 for right length
	
	if (phone_nmber_length < 16)
	{
		hfpToMMIEvtParam.cnumEvt.length = phone_nmber_length;
		OSMEM_memcpy_xdata_xdata (&hfpToMMIEvtParam.cnumEvt.callNumId[0], index_of_string_start, phone_nmber_length);
	}
	
	if ( *index_of_ptr1 != AT_CR )
	{																	//Is there type field?
		hfpToMMIEvtParam.cnumEvt.type = (U8)HFP_ASCII_TO_U16 (index_of_ptr1+1); //index_of_ptr1 pointer to comma or AT_CR
	}
	
	if ( *(HFP_FindComma(index_of_ptr1+1)) != AT_CR )					//Is there any field else, yes->+cnum service field 
	{
		while( *index_of_ptr1 != AT_CR )
		{
			index_of_ptr1++;
		}	  						//find the AT_CR
		hfpToMMIEvtParam.cnumEvt.service = HFP_CHAR_TO_DIGIT(*(--index_of_ptr1));
	}
	
	MMI_HFP_EventHandler(linkIndex, HFP_GetLinkInfo(linkIndex)->currentCmd, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_Plus_CME(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	
	hfpToMMIEvtParam.cmeErrorEvt.errorCode = (U8)HFP_ASCII_TO_U16(HFP_FindColon(dataPtr) + 1);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_ERROR_RESULT_CODE_RESPONSE, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_Plus_CLCC(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	U8 XDATA_PTR index_of_ptr1;
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	
	index_of_ptr1 = &dataPtr[6];
	hfpToMMIEvtParam.clccEvt.idx = HFP_ASCII_TO_U16(index_of_ptr1);
	
	index_of_ptr1 = HFP_FindComma(index_of_ptr1) + 1;
	hfpToMMIEvtParam.clccEvt.dir = HFP_CHAR_TO_DIGIT(index_of_ptr1);
	index_of_ptr1 = HFP_FindComma(index_of_ptr1) + 1;
	hfpToMMIEvtParam.clccEvt.status = HFP_CHAR_TO_DIGIT(index_of_ptr1);
	index_of_ptr1 = HFP_FindComma(index_of_ptr1) + 1;
	hfpToMMIEvtParam.clccEvt.mode = HFP_CHAR_TO_DIGIT(index_of_ptr1);
	index_of_ptr1 = HFP_FindComma(index_of_ptr1) + 1;
	hfpToMMIEvtParam.clccEvt.mpty = HFP_CHAR_TO_DIGIT(index_of_ptr1);

	MMI_HFP_EventHandler(linkIndex, HFP_GetLinkInfo(linkIndex)->currentCmd, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_Plus_CLIP(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	U8 quotes = 0;
	U8 length = 0;
	U8 XDATA_PTR result_code_Ptr;
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	
	result_code_Ptr = dataPtr;
	while( *result_code_Ptr != AT_CR && *result_code_Ptr != AT_LF)
	{
		if(*result_code_Ptr == '"')
		{	
			quotes++;	
		}
		else if (length < CALLER_ID_DIGIT_NO && quotes == 1)
		{	
			hfpToMMIEvtParam.clipEvt.callNumId[length] = *result_code_Ptr; 			
			length++;
		}
		else if(quotes > 1)
		{
			hfpToMMIEvtParam.clipEvt.length = length;
			MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_CALLING_LINE_NUMBER, &hfpToMMIEvtParam);
			return;
		}
		result_code_Ptr++;
	}	
}

PRIVATE void HFP_Result_Code_Plus_CCWA(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	U8 quotes = 0;
	U8 length = 0;
	U8 XDATA_PTR result_code_Ptr;
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;

	result_code_Ptr = dataPtr;
	while( *result_code_Ptr != AT_CR && *result_code_Ptr != AT_LF)
	{
		if(*result_code_Ptr == '"')
		{	
			quotes++;	
		}
		else if (length < CALLER_ID_DIGIT_NO && quotes == 1)
		{	
			hfpToMMIEvtParam.clipEvt.callNumId[length] = *result_code_Ptr; 			
			length++;
		}
		else if(quotes > 1)
		{
			hfpToMMIEvtParam.clipEvt.length = length;
			MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_CALL_WAITING_NUMBER, &hfpToMMIEvtParam);
			return;
		}
		result_code_Ptr++;
	}	
}

PRIVATE void HFP_Result_Code_Plus_XAPL(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	U8 XDATA_PTR ptr2;
	
	ptr2= HFP_FindComma(dataPtr) + 1;
	HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY, ((HFP_CHAR_TO_DIGIT(*(ptr2)))&0x01 > 0)?0:1);
}

PRIVATE void HFP_Result_Code_Plus_BCS(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	
	hfpToMMIEvtParam.bcsEvt.codecType = (AT_EOF == HFP_FindMatchCodec(dataPtr[HFP_AT_COMMAND_PARA0_OFFSET]))?
										MMI_EOF:
										HFP_CHAR_TO_DIGIT(dataPtr[HFP_AT_COMMAND_PARA0_OFFSET]);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_BCS_UPDATE_INDICATION, &hfpToMMIEvtParam);
}

PRIVATE void _Plus_BIND_Query(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	U8 XDATA_PTR indPtr, XDATA_PTR endPtr;
	
	UNUSED(linkIndex);
	
	endPtr = HFP_FindToken(dataPtr, ')');
	
	//find each support item, actually don't care the parsing result
	if (dataPtr[6] == '(' && *endPtr == ')')
	{
		*endPtr = ',';
		for (indPtr = &dataPtr[7]; indPtr < endPtr; indPtr = HFP_FindComma(indPtr) + 1)
		{
			HFP_ASCII_TO_U16(indPtr);
		}
	}
}

PRIVATE void _Plus_BIND_InitState(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	U8 XDATA_PTR indPtr, XDATA_PTR endPtr;
	U16 hfIndicator;
	
	indPtr = &dataPtr[6];
	hfIndicator = HFP_ASCII_TO_U16(indPtr);
	endPtr = HFP_FindComma(indPtr) + 1;
	switch (hfIndicator)
	{
		case HFP_BIND_BATTERY_LEVEL:
			HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY,
						(HFP_ASCII_TO_U16(endPtr) == 0) ? HFP_BATTERY_REPORT_BIEV_SLC_0 : HFP_BATTERY_REPORT_BIEV_SLC_1);
			break;
	}
}

PRIVATE void _Plus_BIND_Unsolicited(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	U8 XDATA_PTR indPtr, XDATA_PTR endPtr;
	U16 hfIndicator;
	
	indPtr = &dataPtr[6];
	hfIndicator = HFP_ASCII_TO_U16(indPtr);
	endPtr = HFP_FindComma(indPtr) + 1;
	switch (hfIndicator)
	{
		case HFP_BIND_BATTERY_LEVEL:
			HFP_IssueCompleteEvent(linkIndex, HFP_ATCMD_IPHONE_BATTERY_INQUIRY,
						(HFP_ASCII_TO_U16(endPtr) == 0) ? HFP_BATTERY_REPORT_BIEV_UNS_0 : HFP_BATTERY_REPORT_BIEV_UNS_1);
			break;
	}
}

PRIVATE void HFP_Result_Code_Plus_BIND(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpLinkInfo XDATA_PTR hfpLinkInfo = HFP_GetLinkInfo(linkIndex);
	
	if (hfpLinkInfo->currentCmd == HFP_CMD_INTERNAL_SLC_INIT_ROUTINE)
	{
		if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_2ND_BIND_OK)
		{
			_Plus_BIND_Query(linkIndex, dataPtr);
		}
		else if (hfpLinkInfo->subState == HFP_SUBSTATE_WAIT_3RD_BIND_OK)
		{
			_Plus_BIND_InitState(linkIndex, dataPtr);
		}
	}
	else
	{
		_Plus_BIND_Unsolicited(linkIndex, dataPtr);
	}
}

PRIVATE void HFP_Result_Code_Plus_SIRI(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;

	hfpToMMIEvtParam.siriEvt.siriStatus = HFP_CHAR_TO_DIGIT(dataPtr[9]);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_APPLE_SIRI_STATUS_INDICATION, &hfpToMMIEvtParam);
}

PRIVATE void HFP_Result_Code_OtherCMEError(U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	HfpToMMIEvtParamType LDATA hfpToMMIEvtParam;
	
	hfpToMMIEvtParam.cmeErrorEvt.errorCode = AT_FindResultCode(dataPtr);
	MMI_HFP_EventHandler(linkIndex, HFP_EVENT_TO_MMI_ADDITIONAL_ERROR_RESULT_CODE, &hfpToMMIEvtParam);
}

typedef struct
{
	void (*HFP_Result_Code)(U8 linkIndex, U8 XDATA_PTR dataPtr);
}HFP_RESULT_CODE_FUNC;

static HFP_RESULT_CODE_FUNC CODE HFP_ResultCodeFunc[HFP_ERROR_RESULT_CODE_END] = 
{
	{HFP_Result_Code_RING}, //HFP_RESULT_CODE_RING
	{HFP_Result_Code_Plus_BRSF}, //HFP_RESULT_CODE_PLUS_BRSF
	{HFP_Result_Code_Plus_CIND}, //HFP_RESULT_CODE_PLUS_CIND
	{HFP_Result_Code_Plus_CHLD}, //HFP_RESULT_CODE_PLUS_CHLD
	{HFP_Result_Code_Plus_VGM}, //HFP_RESULT_CODE_PLUS_VGM
	{HFP_Result_Code_Plus_VGS}, //HFP_RESULT_CODE_PLUS_VGS
	{HFP_Result_Code_OK}, //HFP_RESULT_CODE_OK	
	{HFP_Result_Code_ERROR}, //HFP_RESULT_CODE_ERROR				
	{HFP_Result_Code_CIEV}, //HFP_RESULT_CODE_CIEV					
	{HFP_Result_Code_Plus_BVRA}, //HFP_RESULT_CODE_PLUS_BVRA			
	{HFP_Result_Code_Plus_COPS}, //HFP_RESULT_CODE_PLUS_COPS
	{HFP_Result_Code_Plus_BTRH}, //HFP_RESULT_CODE_PLUS_BTRH			
	{HFP_Result_Code_Plus_BSIR}, //HFP_RESULT_CODE_PLUS_BSIR
	{HFP_Result_Code_Plus_CNUM}, //HFP_RESULT_CODE_PLUS_CNUM
	{HFP_Result_Code_Plus_CME}, //HFP_RESULT_CODE_PLUS_CME
	{HFP_Result_Code_Plus_CLCC}, //HFP_RESULT_CODE_PLUS_CLCC
	{HFP_Result_Code_Plus_CLIP}, //HFP_RESULT_CODE_PLUS_CLIP
	{HFP_Result_Code_Plus_CCWA}, //HFP_RESULT_CODE_PLUS_CCWA
	{HFP_Result_Code_Plus_CNUM}, //HFP_RESULT_CODE_PLUS_BINP					
	{HFP_Result_Code_Plus_XAPL}, //HFP_RESULT_CODE_PLUS_XAPL
	{HFP_Result_Code_Plus_BCS}, //HFP_RESULT_CODE_PLUS_BCS
	{HFP_Result_Code_Plus_BIND}, //HFP_RESULT_CODE_PLUS_BIND
	{HFP_Result_Code_Plus_SIRI}, //HFP_RESULT_CODE_PLUS_APLSIRI
	{HFP_Result_Code_OtherCMEError}, //HFP_RESULT_CODE_NO_CARRIER
	{HFP_Result_Code_OtherCMEError}, //HFP_RESULT_CODE_BUSY
	{HFP_Result_Code_OtherCMEError}, //HFP_RESULT_CODE_NO_ANSWER
	{HFP_Result_Code_OtherCMEError}, //HFP_RESULT_CODE_DELAYED
	{HFP_Result_Code_OtherCMEError}, //HFP_RESULT_CODE_BLACKLISTED
};

PRIVATE void AT_CheckResultCode (U8 linkIndex, U8 XDATA_PTR dataPtr)
{
	U8 index;
	
	index = AT_FindResultCode(dataPtr);
	if(index < HFP_ERROR_RESULT_CODE_END)
	{
		if(HFP_ResultCodeFunc[index].HFP_Result_Code)
			HFP_ResultCodeFunc[index].HFP_Result_Code(linkIndex, dataPtr);
			
		OSMEM_Put(dataPtr);
	}
	else
	{
		if (HFP_GetLinkInfo(linkIndex)->subState == HFP_SUBSTATE_NO_SUBSTATE)
		{
			if(api_profile_func[API_GRP_PF_CB_HFP_AT_RESULT_CODE])
			{
				((API_PROFILE_HFP_RESULT_CODE_FUNC)api_profile_func[API_GRP_PF_CB_HFP_AT_RESULT_CODE])(linkIndex, dataPtr);
			}
			OSMEM_Put(dataPtr);
		}
		else
		{
			HFP_Result_Code_OtherCMEError(linkIndex, dataPtr);
			OSMEM_Put(dataPtr);
			HFP_CommandComplete(linkIndex);
		}		
	}	
}

///////////////////////////////////////////////////////////
//	AT_SegmentAndEliminateSpace_HF:
//
//	Input:
//
//	Return value:
//
///////////////////////////////////////////////////////////
PUBLIC void AT_SegmentAndEliminateSpace_HF (U8 linkIndex, U8 XDATA_PTR ptr1)
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
			if(*ptr2 == AT_CR && *(ptr2 +1) == AT_LF)
			{
				ptr2 += 2;
				length -= 2;
			}
			else
			{
				ptr3 = ptr2;
				realDataLength = 0;
				while (length && (!(*ptr3 == AT_CR && *(ptr3 +1) == AT_LF)))
				{
					ptr3 ++;			
					length --;
					realDataLength ++;
				}
				
				if (length) //second CR LF match
				{
					if((ptr3 = SYS_MemoryGet(realDataLength+2)) != (U8 XDATA_PTR)NULL)
					{
						OSMEM_memcpy_xdata_xdata (ptr3, ptr2, realDataLength+2);
						AT_UpperCaseAndEliminateSpace(ptr3, realDataLength+2);
						LightDbgPrintWithConnectionHandle (
							HCI_VOPCODE_VOGF(HCI_VENDOR_OGF_UART_HFP_DBGPRINT,HCI_VEVT_OCF_HFP_DBGPRINT_RX),
						 	MMI_GetConnHandleByLinkIndex(linkIndex),
							ptr3);
						AT_CheckResultCode(linkIndex, ptr3);
					}
					ptr2 += (realDataLength+2);
					length -=2;
				}						
				else if(realDataLength)//second CR LF not found
				{
					if((ptr3 = SYS_MemoryGet(realDataLength)) != (U8 XDATA_PTR)NULL)
					{
						OSMEM_memcpy_xdata_xdata (ptr3, ptr2, realDataLength);
						hfpLinkInfo->at_string_control.at_string_recv_ptr = ptr3;
						hfpLinkInfo->at_string_control.at_string_byte_num = realDataLength;
					}
				}		   
			}
		}
		else//wait continute state
		{
			ptr3 = ptr2;
			realDataLength = 0;			
			while (length && (!(*ptr3 == AT_CR && *(ptr3 +1) == AT_LF)))
			{
				ptr3 ++;
				length --;				
				realDataLength ++;					
			}
			
			if (length) //second CR LF match
			{				
				if((ptr3 = SYS_MemoryGet(hfpLinkInfo->at_string_control.at_string_byte_num+realDataLength+2)) != (U8 XDATA_PTR)NULL)
				{
					OSMEM_memcpy_xdata_xdata (ptr3, hfpLinkInfo->at_string_control.at_string_recv_ptr, hfpLinkInfo->at_string_control.at_string_byte_num);
					OSMEM_memcpy_xdata_xdata (ptr3+hfpLinkInfo->at_string_control.at_string_byte_num, ptr2, realDataLength+2);
					OSMEM_Put(hfpLinkInfo->at_string_control.at_string_recv_ptr);
					AT_UpperCaseAndEliminateSpace(ptr3, hfpLinkInfo->at_string_control.at_string_byte_num+realDataLength+2);
					LightDbgPrintWithConnectionHandle (
						HCI_VOPCODE_VOGF(HCI_VENDOR_OGF_UART_HFP_DBGPRINT,HCI_VEVT_OCF_HFP_DBGPRINT_RX),
						MMI_GetConnHandleByLinkIndex(linkIndex),
						ptr3);
					AT_CheckResultCode(linkIndex, ptr3);
				}
				else
				{
					OSMEM_Put(hfpLinkInfo->at_string_control.at_string_recv_ptr);
				}
				hfpLinkInfo->at_string_control.at_string_byte_num = 0;			
				ptr2 += (realDataLength+2);
				length -=2;
			}			
			else if(realDataLength)//second CR LF not found
			{	
				if((ptr3 = SYS_MemoryGet(hfpLinkInfo->at_string_control.at_string_byte_num+realDataLength)) != (U8 XDATA_PTR)NULL)
				{
					OSMEM_memcpy_xdata_xdata (ptr3, hfpLinkInfo->at_string_control.at_string_recv_ptr, hfpLinkInfo->at_string_control.at_string_byte_num);
					OSMEM_memcpy_xdata_xdata (ptr3+hfpLinkInfo->at_string_control.at_string_byte_num, ptr2, realDataLength);
					OSMEM_Put(hfpLinkInfo->at_string_control.at_string_recv_ptr);
					hfpLinkInfo->at_string_control.at_string_recv_ptr = ptr3;
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
