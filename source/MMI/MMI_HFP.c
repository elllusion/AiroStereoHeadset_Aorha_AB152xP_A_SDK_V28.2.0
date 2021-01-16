#include "reside_flash.inc"

#define _MMI_HFP_C_

#include "rc.h"
#include "bt_config_profile.h"

#ifdef HFP_Profile

#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_HFP.h"
#include "MMI_HCI.h"
#include "MMI_ChgBat.h"
#include "MMI_Reconnect.h"
#include "MMI_AirModule.h"
#ifdef SUPPORT_VOICE_PROMPT
	#include "..\Driver\VoicePrompt.h"
#endif
#include "MMI_A2DP.h"
#include "drivers_flash.h"

#ifdef AIRAPP_Profile
#include "..\AirApp\AirApp_Interface.h"
#endif

#ifdef BLE_AIRApp
#include "..\APP\APP_Interface.h"
#include "..\ATT\ATT_interface.h"
#endif

#include "..\A2DP\a2dp_interface.h"

PRIVATE U8 MMI_HFP_CountCallsInCallStatus(U8 linkInd, U8 callStatus);

PRIVATE void MMI_HFP_CreateAudioConnection(U8 linkIndex)
{
	if (!SCO_EXISTS(linkIndex))
	{
		if(MMI_HFP_IsHFPConnected(linkIndex) && MMI_CODEC_NEGOTIATION_FEAT && (MMI_HFP_GET_AGOPTION(linkIndex) & MMI_AG_OPTION_CODEC_NEGOTIATION))
		{
			MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_CODEC_CONNECTION);
		}
		else
		{
			//New feature - MMI_FORCE_USE_SCO_IN_AUDIO_TRANSFER_FEAT
			if (MMI_FORCE_USE_ESCO_IN_AUDIO_TRANSFER_FEAT)
			{
				MMI_HCI_SendSetupESCO(linkIndex, gMMI_nvram.scoPktPara.scoPktType);
			}
			else
			{
				//Retrieve bits represents HV1, HV2 and HV3 of LM Version 2.0 and transform
				//to LM Version 1.1 by shift 5 bits to the left.
				//#define HCIPKTYPE_VER20_HV1 0x0001 //may be used
				//#define HCIPKTYPE_VER20_HV2 0x0002 //may be used
				//#define HCIPKTYPE_VER20_HV3 0x0004 //may be used
				//#define HCIPKTYPE_VER20_EV3 0x0008 //may be used
				//#define HCIPKTYPE_VER20_EV4 0x0010 //may be used
				//#define HCIPKTYPE_VER20_EV5 0x0020 //may be used
				//#define HCIPKTYPE_VER20_2EV3 0x0040 //may not be used
				//#define HCIPKTYPE_VER20_3EV3 0x0080 //may not be used
				//#define HCIPKTYPE_VER20_2EV5 0x0100 //may not be used
				//#define HCIPKTYPE_VER20_3EV5 0x0200 //may not be used

				MMI_HCI_SendSetupESCO(linkIndex, (gMMI_nvram.scoPktPara.scoPktType & 0x0007) | 0x03C0);
			}
		}
	}
}

PRIVATE U8 MMI_HFP_GetIPhoneBatteryLevels(void)
{
	U8 i;

	for( i = 0; i < MMI_IPHONE_BATTERY_LEVELS; i++)
	{
		if(DRV_ADC0.fVBat() < pMpParameter->MP_BatteryPara.batteryIPhoneCheckPoints[i])
			break;
	}

	return i;
}

PUBLIC BOOL MMI_HFP_SendATCmdByLink(U8 linkIndex, U8 opcode)
{
	MMIMsgType XDATA_PTR msgPtr = MMI_GetCommandMessageMem(opcode);

	if (msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		if(MMI_HFP_IsHSPConnected(linkIndex))
		{
			switch(opcode)
			{
				case HFP_ATCMD_ADJUST_MIC_GAIN:
					gMMI_HFP_ctl.HfpCtl[linkIndex].sendingVGSVGM = TRUE;
					msgPtr->msgBodyPtr.hfpCmd.VGMCmd.para = CURRENT_SCO_VGM(linkIndex);
					break;
				case HFP_ATCMD_ADJUST_VOLUME:
					gMMI_HFP_ctl.HfpCtl[linkIndex].sendingVGSVGM = TRUE;
					msgPtr->msgBodyPtr.hfpCmd.VGSCmd.para = GET_SCO_VGS_BY_LEVEL(CURRENT_SCO_SOUND_LEVEL(linkIndex));
					break;
				case HFP_CMD_FROM_MMI_RING_GET:
				case HFP_CMD_FROM_MMI_EXIT_INCOMING:
					break;
				default:
					msgPtr->msgOpcode = opcode = HFP_ATCMD_CKPD_200;
					break;
			}
		}
		else
		{
			switch(opcode)
			{
				case HFP_ATCMD_INIT_VOICE_DIAL:
					gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STARTING;
					MMI_PushMediaEvent(MEDIA_EVT_VOICE_DIAL_ENABLE);
					break;
					
				case HFP_ATCMD_ADJUST_VOLUME:
					gMMI_HFP_ctl.HfpCtl[linkIndex].sendingVGSVGM = TRUE;
					msgPtr->msgBodyPtr.hfpCmd.VGSCmd.para = GET_SCO_VGS_BY_LEVEL(CURRENT_SCO_SOUND_LEVEL(linkIndex));
					break;
				case HFP_ATCMD_ADJUST_MIC_GAIN:
					gMMI_HFP_ctl.HfpCtl[linkIndex].sendingVGSVGM = TRUE;
					msgPtr->msgBodyPtr.hfpCmd.VGMCmd.para = CURRENT_SCO_VGM(linkIndex);
					break;
					
				#ifdef _EXTENSION_ENTITIES_		
				case  HFP_ATCMD_SEND_DTMF:
					msgPtr->msgBodyPtr.hfpCmd.DTMFCmd.para = gMMI_HFP_ctl.HfpCtl[linkIndex].DTMFPara;//0 - 9 and * and #
					break;
				#endif
				#ifdef _MEMORY_DIALING_	
				case HFP_ATCMD_MEMORY_DIALING:
					msgPtr->msgBodyPtr.hfpCmd.DialMemCmd.index = gMMI_HFP_ctl.HfpCtl[linkIndex].memoryDialIndex;
					break;
				#endif
				#ifdef _PLACE_CALL_WITH_PHONE_NUMBER_
				case HFP_ATCMD_PLACE_CALL_WITH_PHONE_NUMBER:
					OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hfpCmd.DialNumCmd, 
											 (U8 XDATA_PTR)&gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall, 
											 sizeof(PhoneNumber));
					break;
				#endif

				#ifdef _THREE_WAY_HANDLING_
				case HFP_ATCMD_3WAY_RELNUDUB:
					msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;
					msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 0;
					if( gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus > MMI_CALL_EMPTY && gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus > MMI_CALL_EMPTY)
						gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_NO_3WAY;
					break;

				case HFP_ATCMD_3WAY_RELNACP:
					msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;
					msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 1;
					if( gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus != MMI_CALL_HELD || gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus != MMI_CALL_HELD)
						gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_NO_3WAY;
					break;

				case HFP_ATCMD_3WAY_RELNACP_X:
					msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;
					msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 12;
					if( gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[0].callStatus != MMI_CALL_HELD || gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[1].callStatus != MMI_CALL_HELD)
						gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_NO_3WAY;
					break;
					
				case HFP_ATCMD_3WAY_HOLDNACP:
					msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;
					msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 2;
					gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_IN_3WAY;
					break;
				
				case HFP_ATCMD_3WAY_HOLDNACP_X:
					msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;
					msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 22;
					gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_IN_3WAY;
					break;

				case HFP_ATCMD_3WAY_ADD:	
					msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;
					msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 3;
					gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_IN_3WAY;
					break;

				case HFP_ATCMD_3WAY_CALLTRANSFER:
					msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;
					msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 4;
					gMMI_HFP_ctl.HfpCtl[linkIndex].is3WayAdd = MMI_CALL_NO_3WAY;
					break;
				#endif

				case HFP_ATCMD_XEVENT_BATTERY_SEND:
				case HFP_ATCMD_IPHONE_BATTERY_SEND:
					msgPtr->msgBodyPtr.hfpCmd.IPhoneCmd.batteryLevel = MMI_HFP_GetIPhoneBatteryLevels();
					break;
				
				case HFP_ATCMD_BIEV_BATTERY_SEND:
					msgPtr->msgBodyPtr.hfpCmd.IPhoneCmd.batteryLevel = MMI_BAT_GetLevelInPercent();
					break;

				case HFP_ATCMD_CANCEL_VOICE_DIAL:
					gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STOPPING;
					MMI_PushMediaEvent(MEDIA_EVT_VOICE_DIAL_ENABLE);
					break;
				case HFP_ATCMD_LAST_NUMBER_REDIAL:
					gMMI_ctl.mmiInfo[linkIndex].subState = MMI_RDIAL_STARTING;
					MMI_PushMediaEvent(MEDIA_EVT_REDIAL_ENABLE);
					break;
					
				case HFP_ATCMD_SELECT_CODEC:
					msgPtr->msgBodyPtr.hfpCmd.BCSCmd.scoCodec = gMMI_ctl.mmiInfo[linkIndex].linkPara.scoCodecType;
					break;
			}
		}
		HFP_CommandHandle(linkIndex, opcode, msgPtr);
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL MMI_SendATCmdUnderConnection(U8 linkIndex, U8 opcode)
{
	if(MMI_HFP_IsConnected(linkIndex))
	{
		return MMI_HFP_SendATCmdByLink(linkIndex, opcode);
	}
	return FALSE;
}

#ifndef HFP_AG_Profile
PUBLIC U8 MMI_HFP_GetNextHFPLink(U8 linkIndex)
{
#ifdef SUPPORT_MULTI_POINT	
	U8 i, j;

	for(i = linkIndex+1; i < linkIndex+MAX_MULTI_POINT_NO; i++)
	{
		j = i % MAX_MULTI_POINT_NO;
		if(MMI_IsActiveLink(j) && MMI_HFP_IsConnected(j))
			return j;
	}
#else
	UNUSED(linkIndex);
#endif	
	return MMI_EOF;
}
#endif

PRIVATE BOOL MMI_HFP_IsSuportAGOptions(U8 linkIndex, U16 agOption)
{
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].agOption & agOption)
		return TRUE;

	return FALSE;
}

PUBLIC void MMI_HFP_StopIncomingVPRing(U8 linkIndex)
{
	#ifdef SUPPORT_VOICE_COMMAND
	MMI_DRV_SendStopVoiceCommandCmd();
	#endif

	MMI_DRV_SpecificEventRingtoneStopCmd(MEDIA_EVT_RINGTONE);
	MMI_DRV_SpecificEventRingtoneStopCmd(MEDIA_EVT_ENTER_INCOMING);

	#ifdef SUPPORT_VOICE_PROMPT
	MMI_DRV_SpecificEventVPStopCmd(MEDIA_EVT_RINGTONE);
	MMI_DRV_SpecificEventVPStopCmd(MEDIA_EVT_ENTER_INCOMING);
	#endif
	gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerNameGet = FALSE;
	#ifdef BLE_AIRApp
	SendPhoneNumberToApp(linkIndex, gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length, 0);
	#endif
	
	MMI_DRV_ScoModeControl(linkIndex, FALSE, FALSE);
}

PRIVATE void MMI_HFP_AudioTransferKeyHandler(U8 linkIndex, BOOL isViaKey)
{
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransfer)
		return;

	if (MMI_IsActiveLink(linkIndex))
	{
		if (!SCO_EXISTS(linkIndex))
		{
			AUDIO_TRANSFER_ESTABLISH_SCO:
			if(isViaKey)
				MMI_HFP_CreateAudioConnection(linkIndex);
			else
			{
				//New feature - MMI_FORCE_USE_SCO_IN_AUDIO_TRANSFER_FEAT
				if (MMI_FORCE_USE_ESCO_IN_AUDIO_TRANSFER_FEAT)
				{
					MMI_HCI_SendSetupESCO(linkIndex, gMMI_nvram.scoPktPara.scoPktType);
				}
				else
				{
					//Retrieve bits represents HV1, HV2 and HV3 of LM Version 2.0 and transform
					//to LM Version 1.1 by shift 5 bits to the left.
					//#define HCIPKTYPE_VER20_HV1 0x0001 //may be used
					//#define HCIPKTYPE_VER20_HV2 0x0002 //may be used
					//#define HCIPKTYPE_VER20_HV3 0x0004 //may be used
					//#define HCIPKTYPE_VER20_EV3 0x0008 //may be used
					//#define HCIPKTYPE_VER20_EV4 0x0010 //may be used
					//#define HCIPKTYPE_VER20_EV5 0x0020 //may be used
					//#define HCIPKTYPE_VER20_2EV3 0x0040 //may not be used
					//#define HCIPKTYPE_VER20_3EV3 0x0080 //may not be used
					//#define HCIPKTYPE_VER20_2EV5 0x0100 //may not be used
					//#define HCIPKTYPE_VER20_3EV5 0x0200 //may not be used

					MMI_HCI_SendSetupESCO(linkIndex, (gMMI_nvram.scoPktPara.scoPktType & 0x0007) | 0x03C0);
				}		
			}
		}
		else
		{
			U8 anotherLink;
			MMI_HCI_SendRelSco(linkIndex);
			SYS_SetTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransferDetachTimer, (U32)(gMMI_nvram.timerCtl.AudioTransferDetachTime) * HALF_SEC);
			anotherLink = MMI_GetNextActiveLink(linkIndex);
			if (anotherLink < MAX_MULTI_POINT_NO && (gMMI_HFP_ctl.HfpCtl[anotherLink].callCtrlTbl[0].callStatus > MMI_CALL_EMPTY || gMMI_HFP_ctl.HfpCtl[anotherLink].callCtrlTbl[1].callStatus > MMI_CALL_EMPTY))
			{
				if(!SCO_EXISTS(anotherLink))
				{
					linkIndex = anotherLink;
					goto AUDIO_TRANSFER_ESTABLISH_SCO;
				}
			}
		}
		gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransfer = TRUE;
	}
	else if (gMMI_ctl.miscMask2 & MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER)
	{
		//Active Conn, if it exists.
		MMI_Reconnect(RECONNECT_LAST_ONCE_AUDIOTRANSFER);
		gMMI_ctl.miscMask2 &= ( ~MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER);
	}
}

PRIVATE void MMI_HFP_KeyAudioTransfer(U8 linkIndex)
{
	MMI_HFP_AudioTransferKeyHandler(linkIndex, TRUE);
}

PRIVATE void MMI_HFP_KeyDialMemory(U8 linkIndex)
{
#ifdef _MEMORY_DIALING_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_MEMORY_DIALING);
#endif
}

PRIVATE void MMI_HFP_KeyDialNumber(U8 linkIndex)
{
	UNUSED(linkIndex);
#ifdef _PLACE_CALL_WITH_PHONE_NUMBER_
	if(gMMI_HFP_ctl.incomingLinkInd == MMI_EOF)
		return;
	MMI_SendATCmdUnderConnection(gMMI_HFP_ctl.incomingLinkInd, HFP_ATCMD_PLACE_CALL_WITH_PHONE_NUMBER);
#endif
}

PRIVATE void MMI_HFP_KeyCancelOutGoingCall(U8 linkIndex)
{
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_CANCEL_OUTGOING_CALL);
	gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey = TRUE;
	gMMI_ctl.mmiInfo[linkIndex].stateEntryExit |= MMI_EXIT_OUTGOING_STATE;
}

PRIVATE void MMI_HFP_KeyRejectCall(U8 linkIndex)
{
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_REJECT_CALL);
	if(MMI_PLAY_DEFAULT_RING_INCOMING_FEAT)
		MMI_DRV_RejectCallMuteSCOCmd(TRUE);

	if (MMI_HFP_INCOMMING == MMI_GetTopStateByLink(linkIndex))
	{
		MMI_HFP_StopIncomingVPRing(linkIndex);
	}
	gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey = TRUE;
	gMMI_ctl.mmiInfo[linkIndex].stateEntryExit |= MMI_EXIT_IMGCALL_STATE;
}

PRIVATE void MMI_HFP_KeyRejectCallSecondPhone(U8 linkIndex)
{
	U8 nextLink;

	if(MMI_SUPPORT_MULTI_HFP_FEAT)
	{
		nextLink = MMI_GetNextActiveLink(linkIndex);
		if(nextLink != MMI_EOF && MMI_CheckLinkInIncomingState(nextLink))
		{
			MMI_HFP_SendATCmdByLink(nextLink, HFP_ATCMD_REJECT_CALL);
		}
	}
}

PRIVATE void MMI_HFP_KeyOnHoldCall(U8 linkIndex)
{
	#ifdef _HFP_CALLHELD_PREDICT_ 
	if (MMI_HFP_CountCallsInCallStatus(linkIndex, MMI_CALL_HELD) == 0 
		&& MMI_HFP_CountCallsInCallStatus(linkIndex, MMI_CALL_ACTIVE) == 1)
	//will receive callheld = 2 (call on hold, no active call) 
	{
		MMI_HFP_CallheldIndicationHandler_ForNoActiveCalls(linkIndex);
	}
	else if (MMI_HFP_CountCallsInCallStatus(linkIndex, MMI_CALL_HELD) == 1 
		&& MMI_HFP_CountCallsInCallStatus(linkIndex, MMI_CALL_ACTIVE) == 0)
	//will receive callheld = 0 (no calls held)
	{
		MMI_HFP_CallheldIndicationHandler_ForNoCallsHeld(linkIndex);
	}
	#endif
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_3WAY_HOLDNACP);
}

PRIVATE void MMI_HFP_KeyAcceptCall(U8 linkIndex)
{
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_ACCEPT_CALL);
	MMI_HFP_StopIncomingVPRing(linkIndex);
}

PRIVATE void MMI_HFP_KeyEndCall(U8 linkIndex)
{
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callheld == CALLHELD_ON_HOLD_NOACTIVE)
		MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_3WAY_RELNUDUB);
	else
		MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_END_CALL);
}

PRIVATE void MMI_HFP_Key3WayRelNUDB(U8 linkIndex)
{
#ifdef _THREE_WAY_HANDLING_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_3WAY_RELNUDUB);
#endif
}

PRIVATE void MMI_HFP_Key3WayRelNAcp(U8 linkIndex)
{
#ifdef _THREE_WAY_HANDLING_
	MMI_SendATCmdUnderConnection(linkIndex, MMI_HFP_IsSuportAGOptions(linkIndex, MMI_AG_OPTION_THREEWAY_CALLING)? HFP_ATCMD_3WAY_RELNACP : HFP_ATCMD_END_CALL);
#endif
}

PRIVATE void MMI_HFP_Key3WayRelNAcp_X(U8 linkIndex)
{
#ifdef _THREE_WAY_HANDLING_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_3WAY_RELNACP_X);
#endif
}

PRIVATE void MMI_HFP_Key3WayHoldNAcp(U8 linkIndex)
{
#ifdef _THREE_WAY_HANDLING_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_3WAY_HOLDNACP);
#endif
}

PRIVATE void MMI_HFP_Key3WayHoldNAcp_X(U8 linkIndex)
{
#ifdef _THREE_WAY_HANDLING_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_3WAY_HOLDNACP_X);
#endif
}

PRIVATE void MMI_HFP_Key3WayAdd(U8 linkIndex)
{
#ifdef _THREE_WAY_HANDLING_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_3WAY_ADD);
#endif
}

PRIVATE void MMI_HFP_Key3WayCallTransfer(U8 linkIndex)
{
#ifdef _THREE_WAY_HANDLING_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_3WAY_CALLTRANSFER);
#endif
}

PRIVATE void MMI_HFP_KeyHSPButtonPressed(U8 linkIndex)
{
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_CKPD_200);
}

PRIVATE void MMI_HFP_KeySetNREC(U8 linkIndex)
{
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_SET_NREC);
}

PRIVATE void MMI_HFP_KeyGetCallNum(U8 linkIndex)
{
#ifdef _EXTENSION_ENTITIES_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_GET_SUBSCRIBER_NUMBER);
#endif
}

PRIVATE void MMI_HFP_KeyQueryCallList(U8 linkIndex)
{
#ifdef _EXTENSION_ENTITIES_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_QUERY_LIST_OF_CURRENT_CALLS);
#endif
}

PRIVATE void MMI_HFP_KeySendDTMF(U8 linkIndex)
{
#ifdef _EXTENSION_ENTITIES_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_SEND_DTMF);
#endif
}

PRIVATE void MMI_HFP_KeyAttachLastVoiceTagRecord(U8 linkIndex)
{
#ifdef _EXTENSION_ENTITIES_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_ATTACH_LAST_VOICE_TAG_RECORDED);
#endif
}

PRIVATE void MMI_HFP_KeySendCLIP(U8 linkIndex)
{
#ifdef _EXTENSION_ENTITIES_
	MMI_SendATCmdUnderConnection(linkIndex, HFP_ATCMD_ENABLE_CLI_NOTIFICATION);
#endif
}

static MMI_KEY_EVT CODE MMI_HFP_KeyEvtHandler[HFP_KEY_EVENT_NO] =
{
	{MMI_HFP_KeyAudioTransfer}, //HFP_KEY_EVENT_KEY_AUDIO_TRANSFER
	{NULL}, 					//HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION //Wait until voice prompt is over
	{NULL}, 					//HFP_KEY_EVENT_KEY_RDIAL //Wait until voice prompt is over 
	{MMI_HFP_KeyDialMemory}, 	//HFP_KEY_EVENT_KEY_DIALMEM
	{MMI_HFP_KeyDialNumber}, 	//HFP_KEY_EVENT_KEY_DIALNUM, 
	{MMI_HFP_KeyCancelOutGoingCall}, //HFP_KEY_EVENT_KEY_CNLOUTGOING,	 	 
	{MMI_HFP_KeyRejectCall},	//HFP_KEY_EVENT_KEY_REJCALL,
	{MMI_HFP_KeyRejectCallSecondPhone}, //HFP_KEY_EVENT_KEY_REJCALL_SECOND_PHONE,	 				  
	{MMI_HFP_KeyOnHoldCall}, 	//HFP_KEY_EVENT_KEY_ONHOLD_CALL,		//0x36
	{MMI_HFP_KeyAcceptCall}, 	//HFP_KEY_EVENT_KEY_ACPCALL, 
	{MMI_HFP_KeyEndCall}, 		//HFP_KEY_EVENT_KEY_ENDCALL,  
	{MMI_HFP_Key3WayRelNUDB}, 	//HFP_KEY_EVENT_KEY_3WAYRELNUDUB,	 
	{MMI_HFP_Key3WayRelNAcp}, 	//HFP_KEY_EVENT_KEY_3WAYRELNACP,
	{MMI_HFP_Key3WayRelNAcp_X}, //HFP_KEY_EVENT_KEY_3WAYRELNACP_X,	//0x3B
	{MMI_HFP_Key3WayHoldNAcp}, 	//HFP_KEY_EVENT_KEY_3WAYHOLDNACP,
	{MMI_HFP_Key3WayHoldNAcp_X}, //HFP_KEY_EVENT_KEY_3WAYHOLDNACP_X,
	{MMI_HFP_Key3WayAdd}, 		//HFP_KEY_EVENT_KEY_3WAYADD,	 
	{MMI_HFP_Key3WayCallTransfer}, //HFP_KEY_EVENT_KEY_3WAYCALLTRANSFER,
	{MMI_HFP_KeyHSPButtonPressed}, //HFP_KEY_EVENT_KEY_HSP_BUTTON_PRESSED,	 
	{MMI_HFP_KeySetNREC}, 		//HFP_KEY_EVENT_KEY_DISABLE_NREC,		//0x46
	{MMI_HFP_KeyGetCallNum}, 	//HFP_KEY_EVENT_KEY_GETCALLNUM,
	{MMI_HFP_KeyQueryCallList}, //HFP_KEY_EVENT_KEY_QUERY_CALL_LIST,
	{MMI_HFP_KeySendDTMF}, 		//HFP_KEY_EVENT_KEY_SEND_DTMF,					
	{MMI_HFP_KeyAttachLastVoiceTagRecord}, //HFP_KEY_EVENT_KEY_ATTACH_LAST_VOICE_TAG_RECORDED,
	{MMI_HFP_KeySendCLIP}, 		//HFP_KEY_EVENT_KEY_SEND_CLIP,			//0x4B
	{NULL}, 					//HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_SECONDARY //Wait until voice prompt is over
	{NULL}, 					//HFP_KEY_EVENT_KEY_RDIAL_SECONDARY,   
	{NULL}, 					//HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_CANCEL,
	{NULL}, 					//HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_SECONDARY_CANCEL,
	{NULL}, 					//HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE,		//Wait until voice prompt is over
	{NULL}, 					//HFP_KEY_EVENT_KEY_SIRI_END,			//Wait until voice prompt is over
	{NULL}, 					//HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE_SECONDARY, //Wait until voice prompt is over
	{NULL}, 					//HFP_KEY_EVENT_KEY_SIRI_END_SECONDARY, //Wait until voice prompt is over
};

PRIVATE U8 MMI_HFP_CheckKeyIsValid(U16 keyCode, U8 isSeperateKey, U8 XDATA_PTR linkIndexPtr, U8 targetIndex)
{
	U8 i;

	if(isSeperateKey)
	{
		//Check link active
		for(i = 0; i < MAX_MULTI_POINT_NO; i++)
		{
			if(SYS_IsBDAddrIsTheSame((U8 XDATA_PTR)&gMMI_driver_variation_nvram.misc_para.agInd[*linkIndexPtr].member[0], (U8 XDATA_PTR)(&gMMI_ctl.mmiInfo[i].bdAddr[0])))
			{
				if(!MMI_HFP_IsConnected(i))
				{
					keyCode = KEY_INVALID;
				}
				*linkIndexPtr = i;
				return keyCode;
			}
		}
		*linkIndexPtr = MAX_MULTI_POINT_NO;
	}
	else
	{
		if(targetIndex == MMI_EOF)
		{
			*linkIndexPtr= MAX_MULTI_POINT_NO;
			if(!(gMMI_driver_variation_nvram.link_history_info[0].miscMask & NVRAM_LINK_KEY_MISC_MASK_HFP) )  //last disc link in history
			{
				keyCode = KEY_INVALID;
			}
		}
		else
		{
			*linkIndexPtr= targetIndex;
		}
	}
	
	if(*linkIndexPtr != MAX_MULTI_POINT_NO && gMMI_ctl.sdapCtl[*linkIndexPtr].remoteApplicationID != PRODUCT_AIR_NONE)
	{
		keyCode = KEY_INVALID;
	}
	return keyCode;
}

PUBLIC void MMI_HFP_ProcessKeyEvent(U8 linkIndex, U8 index)
{
	if(MMI_HFP_KeyEvtHandler[index].fKeyEvt && (MMI_HFP_IsHFPConnected(linkIndex) || MMI_HFP_IsHSPConnected(linkIndex)))
	{
		MMI_HFP_KeyEvtHandler[index].fKeyEvt(linkIndex);
	}
}


PRIVATE U16 MMI_HFP_CheckKeySetVoiceRecognition(U16 keyCode)
{
	U8 LDATA linkIndex;
	
	linkIndex = gMMI_HFP_ctl.rdialLinkInd;
	if(MMI_SEPARATE_VOICE_DIAL_BUTTONS_FEAT)
	{
		linkIndex = (keyCode == KEY_SET_VOICE_RECOGNITION)? 0:1;
	}

	keyCode = MMI_HFP_CheckKeyIsValid(keyCode, MMI_SEPARATE_VOICE_DIAL_BUTTONS_FEAT, (U8 XDATA_PTR)&linkIndex, gMMI_HFP_ctl.rdialLinkInd);

	if(linkIndex == MAX_MULTI_POINT_NO ||keyCode == KEY_INVALID )
		return keyCode;

	if(gMMI_ctl.mmiInfo[linkIndex].subState != MMI_VDIAL_STOP && gMMI_ctl.mmiInfo[linkIndex].subState != MMI_VDIAL_STARTED &&
	gMMI_ctl.mmiInfo[linkIndex].subState != MMI_VDIAL_STOPPING && gMMI_ctl.mmiInfo[linkIndex].subState != MMI_VDIAL_STARTING)
	{
		return KEY_INVALID;
	}

	if(gMMI_HFP_ctl.HfpCtl[linkIndex].appleSiriStatus == HFP_APLSIRI_AVAILABLE_ENABLE) // ios device
	{
		if(MMI_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT && 
		(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTED || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTING))
		{
			if(keyCode == KEY_SET_VOICE_RECOGNITION_SECONDARY)
				keyCode = KEY_SIRI_END_SECONDARY;
			else
				keyCode = KEY_SIRI_END;
		}
		else
		{
			if(keyCode == KEY_SET_VOICE_RECOGNITION_SECONDARY)
				keyCode = KEY_SIRI_START_OR_CONTINUE_SECONDARY;
			else
				keyCode = KEY_SIRI_START_OR_CONTINUE;
		}
	}
	else
	{
		if(!MMI_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT && (gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTED || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTING))
		{
			keyCode = KEY_INVALID;
		}
	}
	return keyCode;
}

PRIVATE U16 MMI_HFP_CheckKeyRedial(U16 keyCode)
{
	U8 LDATA linkIndex;
	linkIndex= gMMI_HFP_ctl.rdialLinkInd;
	if(MMI_SEPARATE_LNR_BUTTONS_FEAT)
	{
		linkIndex = (keyCode == KEY_RDIAL)? 0:1;
	}
	return (MMI_HFP_CheckKeyIsValid(keyCode, MMI_SEPARATE_LNR_BUTTONS_FEAT, (U8 XDATA_PTR)&linkIndex, gMMI_HFP_ctl.rdialLinkInd));
}

PRIVATE U16 MMI_HFP_CheckKeyDialNumber(U16 keyCode)
{
	U8 LDATA linkIndex;
	keyCode = MMI_HFP_CheckKeyIsValid(keyCode, FALSE, (U8 XDATA_PTR)&linkIndex, gMMI_HFP_ctl.incomingLinkInd);

	if(linkIndex!= MAX_MULTI_POINT_NO && !gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length)
	{
		keyCode = KEY_INVALID;
	}

	return keyCode;
}

PRIVATE U16 MMI_HFP_CheckKeyIncomingCallDial(U16 keyCode)
{
	U8 LDATA linkIndex;
	keyCode = MMI_HFP_CheckKeyIsValid(keyCode, FALSE, (U8 XDATA_PTR)&linkIndex, gMMI_ctl.serviceInd);
	if(linkIndex!= MAX_MULTI_POINT_NO && !gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length)
	{
		keyCode = KEY_INVALID;
	}
	return keyCode;
}

PRIVATE U16 MMI_HFP_CheckKeyCancelVoiceRecognition(U16 keyCode)
{
	U8 LDATA linkIndex;

	linkIndex= gMMI_HFP_ctl.rdialLinkInd;
	if(MMI_SEPARATE_VOICE_DIAL_BUTTONS_FEAT)
	{
		linkIndex = (keyCode == KEY_SET_VOICE_RECOGNITION_CANCEL)? 0:1;
	}
	keyCode = MMI_HFP_CheckKeyIsValid(keyCode, MMI_SEPARATE_VOICE_DIAL_BUTTONS_FEAT, (U8 XDATA_PTR)&linkIndex, gMMI_HFP_ctl.rdialLinkInd);

	// Normal voice recognition
	if(linkIndex == MAX_MULTI_POINT_NO || keyCode == KEY_INVALID|| 
	(gMMI_ctl.mmiInfo[linkIndex].subState != MMI_VDIAL_STARTED && gMMI_ctl.mmiInfo[linkIndex].subState != MMI_VDIAL_STARTING))
	{
		return KEY_INVALID;
	}

	if(gMMI_HFP_ctl.HfpCtl[linkIndex].appleSiriStatus == HFP_APLSIRI_AVAILABLE_ENABLE) // ios device
	{
		if(keyCode == KEY_SET_VOICE_RECOGNITION_SECONDARY_CANCEL)
			keyCode = KEY_SIRI_END_SECONDARY;
		else
			keyCode = KEY_SIRI_END;
	}
	return keyCode;
}

static MMI_KEY_CHECK CODE MMI_HFP_KeyCheckValidHandler[HFP_KEY_EVENT_NO] =
{
	{NULL}, 					//HFP_KEY_EVENT_KEY_AUDIO_TRANSFER}
	{MMI_HFP_CheckKeySetVoiceRecognition}, //HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION //Wait until voice prompt is over
	{MMI_HFP_CheckKeyRedial}, 	//HFP_KEY_EVENT_KEY_RDIAL //Wait until voice prompt is over 
	{NULL}, //HFP_KEY_EVENT_KEY_DIALMEM
	{MMI_HFP_CheckKeyDialNumber}, //HFP_KEY_EVENT_KEY_DIALNUM, 
	{NULL}, //HFP_KEY_EVENT_KEY_CNLOUTGOING,	 	 
	{NULL}, //HFP_KEY_EVENT_KEY_REJCALL,
	{NULL}, //HFP_KEY_EVENT_KEY_REJCALL_SECOND_PHONE,	 				  
	{NULL}, //HFP_KEY_EVENT_KEY_ONHOLD_CALL,		//0x36
	{NULL}, //HFP_KEY_EVENT_KEY_ACPCALL, 
	{NULL}, //HFP_KEY_EVENT_KEY_ENDCALL,  
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYRELNUDUB,	 
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYRELNACP,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYRELNACP_X,	//0x3B
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYHOLDNACP,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYHOLDNACP_X,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYADD,	 
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYCALLTRANSFER,
	{MMI_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_HSP_BUTTON_PRESSED,	 
	{NULL}, //HFP_KEY_EVENT_KEY_DISABLE_NREC,		//0x46
	{MMI_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_GETCALLNUM,
	{MMI_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_QUERY_CALL_LIST,
	{MMI_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_SEND_DTMF,					
	{MMI_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_ATTACH_LAST_VOICE_TAG_RECORDED,
	{MMI_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_SEND_CLIP,			//0x4B
	{MMI_HFP_CheckKeySetVoiceRecognition}, //HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_SECONDARY //Wait until voice prompt is over
	{MMI_HFP_CheckKeyRedial}, //HFP_KEY_EVENT_KEY_RDIAL_SECONDARY,   
	{MMI_HFP_CheckKeyCancelVoiceRecognition}, //HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_CANCEL,
	{MMI_HFP_CheckKeyCancelVoiceRecognition}, //HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_SECONDARY_CANCEL,
	{NULL}, //HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE,		//Wait until voice prompt is over
	{NULL}, //HFP_KEY_EVENT_KEY_SIRI_END,			//Wait until voice prompt is over
	{NULL}, //HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE_SECONDARY, //Wait until voice prompt is over
	{NULL}, //HFP_KEY_EVENT_KEY_SIRI_END_SECONDARY, //Wait until voice prompt is over
};

PUBLIC U16 MMI_HFP_CheckValidKey(U8 index, U16 keyCode)
{
	if(MMI_HFP_KeyCheckValidHandler[index].fKeyCheckValid)
	{
		keyCode = MMI_HFP_KeyCheckValidHandler[index].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

PUBLIC BOOL MMI_HFP_SetMemoryDialIndex(U8 linkIndex, U16 index)
{
#ifdef _MEMORY_DIALING_
	if(	MMI_HFP_IsConnected(linkIndex))
	{
		gMMI_HFP_ctl.HfpCtl[linkIndex].memoryDialIndex = index;
		return TRUE;
	}
	return FALSE;
#else
	UNUSED(linkIndex);
	UNUSED(index);
	return FALSE;
#endif
}

PUBLIC BOOL MMI_HFP_SetDialNumber(U8 linkIndex, U8 XDATA_PTR numberPtr, U8 length)
{
#ifdef _PLACE_CALL_WITH_PHONE_NUMBER_
	if(	MMI_HFP_IsConnected(linkIndex))
	{
		OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall, 
						numberPtr,
						length);
		gMMI_HFP_ctl.incomingLinkInd = linkIndex;
		return TRUE;
	}
	return FALSE;
#else
	UNUSED(linkIndex);
	UNUSED(indenumberPtrx);
	UNUSED(length);
	return FALSE;
#endif
}

PUBLIC BOOL MMI_HFP_SetDTMF(U8 linkIndex, U8 index)
{
#ifdef _EXTENSION_ENTITIES_
	if(	MMI_HFP_IsConnected(linkIndex))
	{
		gMMI_HFP_ctl.HfpCtl[linkIndex].DTMFPara = index; //0 - 9 and * and #
		return TRUE;
	}
	return FALSE;
#else
	UNUSED(linkIndex);
	UNUSED(indenumberPtrx);
	UNUSED(length);
	return FALSE;
#endif
}

PUBLIC void MMI_HFP_SCOConnectedHandle(U8 linkIndex)
{
	if(MMI_HFP_IsHSPConnected(linkIndex))
	{
		MMI_RmvState(linkIndex, MMI_HFP_INCOMMING);

		if(!MMI_NOT_CHG_STATE_ACP_HSP_RING_FEAT)
			MMI_InsertState(linkIndex, MMI_HFP_CALLACTIVE, MMI_CONNECTED);
	}
	else
	{
		MMI_ReplaceState(linkIndex, MMI_HFP_CALLACTIVE, MMI_HFP_CALLACTIVE_WITHOUT_SCO);
	}
	
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].relSCOCozExitOggState)
	{
		MMI_HCI_SendRelSco(linkIndex);
	}
	gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransfer = FALSE;
	gMMI_HFP_ctl.HfpCtl[linkIndex].relSCOCozExitOggState = FALSE;
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer);
}

PUBLIC void MMI_HFP_SCOConnectFailHandle(U8 linkIndex)
{
	//sco retry on hv3
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransfer)
	{
		if (!SCO_EXISTS(linkIndex))
		{
			MMI_HCI_SendSetupESCO(linkIndex, 0x03c4 /*HV3*/);
		}
		gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransfer = FALSE;
		gMMI_HFP_ctl.HfpCtl[linkIndex].relSCOCozExitOggState = FALSE;
	}
}

PUBLIC void MMI_HFP_SCODiscHandle(U8 linkIndex)
{
	if(MMI_CheckNestState(linkIndex, MMI_HFP_CALLACTIVE))
	{
		if(MMI_HFP_IsHSPConnected(linkIndex))
		{
			MMI_RmvState(linkIndex, MMI_HFP_CALLACTIVE);
		}
		else
		{
			MMI_ReplaceState(linkIndex, MMI_HFP_CALLACTIVE_WITHOUT_SCO, MMI_HFP_CALLACTIVE);
		}
	}
	gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransfer = FALSE;
	SYS_ReleaseTimer(&gMMI_HFP_ctl.HfpCtl[linkIndex].waitToEstSCOInOggCallTimer);
	if(gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTED || gMMI_ctl.mmiInfo[linkIndex].subState == MMI_VDIAL_STARTING)
	{
		gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STOP;
	}
	
	#ifdef BLE_AIRApp
	//Handle press vol down button when incomming call on iphone
	if(MMI_CheckNestState(linkIndex, MMI_HFP_INCOMMING))
	{
		if(AirApp_BleIsConnected() && (ATT_FindServerIndexByAddr((U8 CODE_PTR)&AppCtl.phoneNumber) != 0) && ((((ATT_GetCCCDByLink(ATT_FindServerIndexByAddr((U8 CODE_PTR)AppCtl.phnoeNumber_CCCD), 0)) >> 8) & 0x01) == 0x01))
		{
			MMI_HFP_ImgAudioVoiceHandler(linkIndex, IMG_AUDIO_VOICE_TRIGGER);
		}
	}
	#endif
}

PUBLIC void MMI_HFP_VoiceControl(U8 linkIndex, BOOL isVolumeUp, BOOL isFollowerCmd)
{
	U8  spkSoundLevel;

	#ifdef HFP_Profile
	if(gMMI_HFP_ctl.HfpCtl[linkIndex].sendingVGSVGM)
		return;
	#endif

	spkSoundLevel = CURRENT_SCO_SOUND_LEVEL(linkIndex);

	if (isVolumeUp)
	{
		CURRENT_SCO_SMALL_STEP(linkIndex) = MMI_DRV_IncreaseSmallStepVolume(CURRENT_SCO_SOUND_LEVEL(linkIndex),
																			gMMI_nvram.totalSoundLevel,
																			MMI_SCO_SMALL_STEP_LEVELS(),
																			CURRENT_SCO_SMALL_STEP(linkIndex));
		if(CURRENT_SCO_SMALL_STEP(linkIndex) == 0)
		{
			if (spkSoundLevel < gMMI_nvram.totalSoundLevel - 1)
			{
				spkSoundLevel++;
			}
			else if(MMI_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				spkSoundLevel = 0;
			}
			else
			{
				LightDbgPrint("sco HFP volume reach max");
				MMI_PushMediaEvent(MEDIA_EVT_VOLUME_REACH_MAX);
				return;
			}
		}
	}
	else
	{
		if(CURRENT_SCO_SMALL_STEP(linkIndex) == 0)
		{
			if (spkSoundLevel > 0)
			{
				spkSoundLevel--;
			}
			else if(MMI_CYCLIC_VOLUME_ENABLED_FEAT)
			{
				spkSoundLevel = gMMI_nvram.totalSoundLevel - 1;
				goto MMI_SEND_SCO_VOL;
			}
			else
			{
				LightDbgPrint("sco HFP volume reach min");
				MMI_PushMediaEvent(MEDIA_EVT_VOLUME_REACH_MIN);
				return;
			}
		}
		CURRENT_SCO_SMALL_STEP(linkIndex) = MMI_DRV_DecreaseSmallStepVolume(MMI_SCO_SMALL_STEP_LEVELS(), CURRENT_SCO_SMALL_STEP(linkIndex));
	}
	MMI_SEND_SCO_VOL:

	MMI_DRV_SetSCOVolValue(linkIndex, spkSoundLevel, CURRENT_SCO_SMALL_STEP(linkIndex));
	if(CURRENT_SCO_SMALL_STEP(linkIndex) == 0)
	{
		#ifdef HFP_Profile
		MMI_HFP_SendATCmdByLink(linkIndex, HFP_ATCMD_ADJUST_VOLUME);
		#endif
	}

	if(gMMI_ctl.audioDevCtl.currentActiveLinkIndex == linkIndex)
	{
		MMI_DRV_SendDriverVolCmd(linkIndex, spkSoundLevel, CURRENT_SCO_SMALL_STEP(linkIndex), VOLUME_MIC_FOLLOW_SPK);
		//Beep!
		if(!isFollowerCmd)
			MMI_DRV_Beep(spkSoundLevel, AUDIO_DEVICE_SCO);
	}
	
	if (MMI_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT)
	{
		MMI_LinkKey_SaveHistoryInRunTime(linkIndex);
	}
}

PUBLIC BOOL MMI_HFP_IsHSPConnected(U8 linkIndex)
{
	if(HFP_SLC_CONNECTED == HFP_GetState(linkIndex) && PROFILE_HEADSET == HFP_GetProfileId(linkIndex))
		return TRUE;
		
	return FALSE;
}

PUBLIC BOOL MMI_HFP_IsHFPConnected(U8 linkIndex)
{
	if(HFP_SLC_CONNECTED == HFP_GetState(linkIndex) && PROFILE_HANDSFREE == HFP_GetProfileId(linkIndex))
		return TRUE;
		
	return FALSE;
}

PUBLIC BOOL MMI_HFP_IsConnected(U8 linkIndex)
{
	if(HFP_SLC_CONNECTED == HFP_GetState(linkIndex))
		return TRUE;
		
	return FALSE;
}

PUBLIC void MMI_HFP_SetReconnectReason(U8 linkIndex, U8 reason)
{
	gMMI_HFP_ctl.HfpCtl[linkIndex].reconnectReason = reason;
}

PUBLIC U8 MMI_HFP_SearchCallStatusInd(U8 linkInd, U8 callStatus)
{
	U8 callInd;

	for (callInd = 0; callInd < MMI_MAX_ONGOING_CALL_NO; callInd++)
	{
		if (callStatus == MMI_CALL_ALL_ONGOING)
		{
			if (gMMI_HFP_ctl.HfpCtl[linkInd].callCtrlTbl[callInd].callStatus != MMI_CALL_EMPTY)
				break;
		}
		else
		{
			if (gMMI_HFP_ctl.HfpCtl[linkInd].callCtrlTbl[callInd].callStatus == callStatus)
				break;
		}
	}

	return callInd;
}

PRIVATE U8 MMI_HFP_CountCallsInCallStatus(U8 linkInd, U8 callStatus)
{
	U8 callInd, count = 0;

	for (callInd = 0; callInd < MMI_MAX_ONGOING_CALL_NO; callInd++)
	{
		if (callStatus == MMI_CALL_ALL_ONGOING)
		{
			if (gMMI_HFP_ctl.HfpCtl[linkInd].callCtrlTbl[callInd].callStatus != MMI_CALL_EMPTY)
				count++;
		}
		else
		{
			if (gMMI_HFP_ctl.HfpCtl[linkInd].callCtrlTbl[callInd].callStatus == callStatus)
				count++;
		}
	}

	return count;
}

////////////////////// For HFP //////////////////////////////////
PUBLIC U16 MMI_HFP_GET_AGOPTION(U8 linkIndex) 
{
	return gMMI_HFP_ctl.HfpCtl[linkIndex].agOption;
}

PUBLIC void MMI_HFP_SET_AGOPTION(U8 linkIndex, U16 agOption) 
{
	gMMI_HFP_ctl.HfpCtl[linkIndex].agOption = agOption;
}

PUBLIC void MMI_HFP_CLEAR_AGOPTION(U8 linkIndex, U16 agOption) 
{
	gMMI_HFP_ctl.HfpCtl[linkIndex].agOption &= ~agOption;
}

PUBLIC U8 MMI_HFP_GET_VGS(U8 linkIndex)
{
	return GET_SCO_VGS_BY_LEVEL(CURRENT_SCO_SOUND_LEVEL(linkIndex));
}


#ifdef HFP_AG_Profile
  #include "MMI_HFP_AG.c"
#else
  #include "MMI_HFP_HF.c"
#endif


#endif //HFP_Profile
