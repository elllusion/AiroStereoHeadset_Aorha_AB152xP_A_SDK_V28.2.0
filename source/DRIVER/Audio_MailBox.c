#include "reside_flash.inc"
#include "drv_sector.h"
#include "bt_config_profile.h"
#include "Audio_MailBox.h"
#include "Audio_Transparency.h"
#ifdef RINGTONE_ENABLE
#include "ringtone.h"
#endif
#ifdef SUPPORT_VOICE_PROMPT
#include "VoicePrompt.h"
#endif
#ifdef SUPPORT_VOICE_COMMAND
#include "VoiceCommand.h"
#endif
#include "Driver_1520.h"

#define EVT_CODE 0x4

#define OGF_PROMPT_NUM		4

enum
{
	PROMPT_OGF_VP,
	PROMPT_OGF_RT,
	PROMPT_OGF_VC,
	PROMPT_OGF_AT,
};

MAILBOX_PROMPT_STRU gPrompt_ctl[OGF_PROMPT_NUM];

MAILBOX_STATE_STRU XDATA MailBox_State[OGF_MAX_NUM];

MAILBOX_CTL_TYPE XDATA gMailBox_ctl;
#define OSMQ_MAILBOX_PTR		&MAILBOX_QUEUE
void SetPrompVPStatus(U8 status)		{gPrompt_ctl[PROMPT_OGF_VP].end_status = status;}
void SetPrompRTStatus(U8 status)		{gPrompt_ctl[PROMPT_OGF_RT].end_status = status;}
void SetPrompVCStatus(U8 status)		{gPrompt_ctl[PROMPT_OGF_VC].end_status = status;}
void SetPrompATStatus(U8 status)		{gPrompt_ctl[PROMPT_OGF_AT].end_status = status;}
static void (*Set_Prompt_Ctl[OGF_MAX_NUM])(U8 status) = 
{
	{SetPrompVPStatus},
	{SetPrompRTStatus},
	{SetPrompVCStatus},
	{(void *)0},
	{(void *)0},
	{SetPrompATStatus},
};

U8 GetPrompVPStatus(void)		{return gPrompt_ctl[PROMPT_OGF_VP].end_status;}
U8 GetPrompRTStatus(void)		{return gPrompt_ctl[PROMPT_OGF_RT].end_status;}
U8 GetPrompVCStatus(void)		{return gPrompt_ctl[PROMPT_OGF_VC].end_status;}
U8 GetPrompATStatus(void)		{return gPrompt_ctl[PROMPT_OGF_AT].end_status;}
static U8 (*Get_Prompt_Ctl[OGF_MAX_NUM])(void) = 
{
	{GetPrompVPStatus},
	{GetPrompRTStatus},
	{GetPrompVCStatus},
	{(void *)0},
	{(void *)0},
	{GetPrompATStatus},
};

void MailBox_Init(void);

void SubState_Init_Handler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT;
void SubState_Completing_Handler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT;

void PromptStart_SubStateHandler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT;
void PromptStop_SubStateHandler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT;
void OneStep_SubStateHandler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT;
void MailboxState_Reset(U8 OGF);


void CMD_REPLY_EvtHandler(void);
void VP_END_EvtHandler(void);
void RT_END_EvtHandler(void);
void VC_END_EvtHandler(void);
void APP_END_EvtHandler(void);
void AT_END_EvtHandler(void);
void MailboxSendEvtToDriver(DRIVERMSG_STRU XDATA_PTR EVT_PTR, U8 ogf, U8 ocf);

MAILBOXFUN CODE DSP_StateHandler[OCF_DSP_MAX_NUM] =
{
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
};


MAILBOXFUN CODE VP_StateHandler[OCF_VP_MAX_NUM] =
{
	PromptStart_SubStateHandler,
	PromptStop_SubStateHandler,
	OneStep_SubStateHandler
};
MAILBOXFUN CODE RT_StateHandler[OCF_RT_MAX_NUM] =
{
	PromptStart_SubStateHandler,
	PromptStop_SubStateHandler
};
MAILBOXFUN CODE VC_StateHandler[OCF_VC_MAX_NUM] =
{
	PromptStart_SubStateHandler,
	PromptStop_SubStateHandler
};

MAILBOXFUN CODE APP_StateHandler[OCF_APP_MAX_NUM] =
{
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
	OneStep_SubStateHandler,
};

MAILBOXFUN CODE AT_StateHandler[OCF_AT_MAX_NUM] =
{
	PromptStart_SubStateHandler,
	PromptStop_SubStateHandler
};

MAILBOXFUN CODE_PTR CODE Mailbox_StateHandler[] =
{
	VP_StateHandler,
	RT_StateHandler,
	VC_StateHandler,
	APP_StateHandler,
	DSP_StateHandler,
	AT_StateHandler,
};
VFUN CODE Mailbox_EVENTCB_Handler[] =
{
	CMD_REPLY_EvtHandler,
	VP_END_EvtHandler,
	RT_END_EvtHandler,
    VC_END_EvtHandler,
	APP_END_EvtHandler,
	AT_END_EvtHandler,
};

void MailBox_Init(void)
{
	U8 i;
	for(i=0 ; i<OGF_MAX_NUM ; ++i)
	{
		MailBox_State[i].state = SUBSTATE_INIT;
		MailBox_State[i].ocf = 0xff;
		MailBox_State[i].CmdPtr = (DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR)NULL;
	}
	gAlignDSP.Mailbox.m2d_ctl.rw_word = MCU_Owned;
	gAlignDSP.Mailbox.d2m_ctl.rw_word = DSP_Owned;
}

void MailBox_CmdHandler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR)
{
    #ifdef OS_PRINT_ASSERT
    U16 cnt = 0;
    #endif

    #ifdef OS_PRINT_ASSERT
	while(gAlignDSP.Mailbox.m2d_ctl.rw_word == DSP_Owned)
	{
	    if (!(cnt % 2000))
	    {
	        LightDbgPrint("MailBox Jam:%d, %d",(U8)CMD_PTR->m2d_ctl.cmd.Ogf, (U8)CMD_PTR->m2d_ctl.cmd.Ocf);
	    }
	    cnt++;
	}
	#else
	while(gAlignDSP.Mailbox.m2d_ctl.rw_word == DSP_Owned){}
	#endif

    if(CMD_PTR->m2d_ctl.cmd.Ogf == OGF_APP && CMD_PTR->m2d_ctl.cmd.Ocf == OCF_APP_ASYNC_MONO_MODE)
	{
		LightDbgPrint("MailCMD:ogf:OGF_APP,ocf:ASYNC_MONO");
	}
	else
    {
        LightDbgPrintWithOpcode((U8)HCI_VEVT_OCF_DRIVER_MAILBOX_CMD,
                                (U8)HCI_VENDOR_OGF_UART_DRIVER_DBGPRINT,
                                (U8)CMD_PTR->m2d_ctl.cmd.Ogf, (U8)CMD_PTR->m2d_ctl.cmd.Ocf);
    }

	if(gAlignDSP.Mailbox.m2d_ctl.rw_word == MCU_Owned && MailBox_State[CMD_PTR->m2d_ctl.cmd.Ogf].state == SUBSTATE_INIT)
	{
		Mailbox_StateHandler[CMD_PTR->m2d_ctl.cmd.Ogf][CMD_PTR->m2d_ctl.cmd.Ocf] (CMD_PTR);
 	}
 	else
	{
		LightDbgPrintWithOpcode((U8)HCI_VEVT_OCF_DRIVER_MAILBOX_CMD_STATE_ERROR,
								(U8)HCI_VENDOR_OGF_UART_DRIVER_DBGPRINT,
								(U8)gAlignDSP.Mailbox.m2d_ctl.rw_word,
								(U8)MailBox_State[CMD_PTR->m2d_ctl.cmd.Ogf].state);
		ASSERT(FALSE);
	}
}

void MailBox_EvtHandler(void)
{
	if(gAlignDSP.Mailbox.d2m_ctl.rw_word == MCU_Owned && gAlignDSP.Mailbox.d2m_ctl.evt_word.Evt >= EVT_CODE)
	{
		Mailbox_EVENTCB_Handler[gAlignDSP.Mailbox.d2m_ctl.evt_word.Evt-EVT_CODE] ();
		gAlignDSP.Mailbox.d2m_ctl.rw_word = DSP_Owned;
	}
	else
	{
		ASSERT(FALSE);
	}
}

void SubState_Init_Handler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT
{

	DSP_Enable(CMD_PTR->m2d_ctl.para.dsp_gain_para.DspClock);

	//HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_AUDIO_CTL, 0x90);
	MailBox_State[CMD_PTR->m2d_ctl.cmd.Ogf].ocf = CMD_PTR->m2d_ctl.cmd.Ocf;
	MailBox_State[CMD_PTR->m2d_ctl.cmd.Ogf].state = SUBSTATE_COMPLETING;
	MailBox_State[CMD_PTR->m2d_ctl.cmd.Ogf].CmdPtr = CMD_PTR;

	DSP_EnableDSPInterruptForAudio();
	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)(&gAlignDSP.Mailbox.m2d_ctl), (U8 XDATA_PTR)(&CMD_PTR->m2d_ctl),sizeof(M2D_CTL_STRU));
	gAlignDSP.Mailbox.m2d_ctl.rw_word = DSP_Owned;
    //CPU_INT_CH9
	HC_SFR_WRITE_BY_FB (FBOFFS_WO_DSP_INT_CTL,0x8);
}

void SubState_Completing_Handler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT
{
	MailboxState_Reset(CMD_PTR->m2d_ctl.cmd.Ogf);

	if (((CMD_PTR->m2d_ctl.cmd.Ogf == OGF_VC) && (CMD_PTR->m2d_ctl.cmd.Ocf == OCF_VC_STOP))
	 || ((CMD_PTR->m2d_ctl.cmd.Ogf == OGF_VP) && (CMD_PTR->m2d_ctl.cmd.Ocf == OCF_VP_STOP))
	 || ((CMD_PTR->m2d_ctl.cmd.Ogf == OGF_RT) && (CMD_PTR->m2d_ctl.cmd.Ocf == OCF_RT_STOP))
	 || ((CMD_PTR->m2d_ctl.cmd.Ogf == OGF_AT) && (CMD_PTR->m2d_ctl.cmd.Ocf == OCF_AT_STOP)))
	{
		OSMEM_Put((U8 XDATA_PTR)CMD_PTR);
	}
	else
	{
		MailboxSendEvtToDriver((DRIVERMSG_STRU XDATA_PTR)CMD_PTR,CMD_PTR->m2d_ctl.cmd.Ogf, CMD_PTR->m2d_ctl.cmd.Ocf);
	}
}


void OneStep_SubStateHandler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT
{
	if(!CMD_PTR)
	{
		ASSERT(FALSE);
	}

	switch(MailBox_State[CMD_PTR->m2d_ctl.cmd.Ogf].state)
	{
		case SUBSTATE_INIT:
			SubState_Init_Handler(CMD_PTR);
			break;

		case SUBSTATE_COMPLETING:
			SubState_Completing_Handler(CMD_PTR);
			break;

		default:
			break;
	}
}
void PromptStart_SubStateHandler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT
{
	if(!CMD_PTR)
	{
		ASSERT(FALSE);
	}

	switch(MailBox_State[CMD_PTR->m2d_ctl.cmd.Ogf].state)
	{
		case SUBSTATE_INIT:
			if(Set_Prompt_Ctl[CMD_PTR->m2d_ctl.cmd.Ogf] != NULL)
				Set_Prompt_Ctl[CMD_PTR->m2d_ctl.cmd.Ogf](0);
			SubState_Init_Handler(CMD_PTR);
			break;
		case SUBSTATE_COMPLETING:
			SubState_Completing_Handler(CMD_PTR);
			break;

		default:
			break;
	}
}
void PromptStop_SubStateHandler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR) compact REENTRANT
{
	if(!CMD_PTR)
	{
		ASSERT(FALSE);
	}

	switch(MailBox_State[CMD_PTR->m2d_ctl.cmd.Ogf].state)
	{
		case SUBSTATE_INIT:
			if((Get_Prompt_Ctl[CMD_PTR->m2d_ctl.cmd.Ogf] != NULL) && !Get_Prompt_Ctl[CMD_PTR->m2d_ctl.cmd.Ogf]())
			{
				SubState_Init_Handler(CMD_PTR);
			}
			else
			{
				MailboxState_Reset(OGF_APP);
				OSMEM_Put((U8 XDATA_PTR)CMD_PTR);
			}
			break;

		case SUBSTATE_COMPLETING:
			SubState_Completing_Handler(CMD_PTR);
			break;

		default:
			break;
	}
}

void CMD_REPLY_EvtHandler(void)
{
	U8 ogf = gAlignDSP.Mailbox.d2m_ctl.evt_word.Cmd.Ogf;
	U8 ocf = gAlignDSP.Mailbox.d2m_ctl.evt_word.Cmd.Ocf;

	if(MailBox_State[ogf].ocf == ocf && gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta == CMD_COMPL)
	{
        if(ogf == OGF_APP && ocf == OCF_APP_ASYNC_MONO_MODE)
		{
			LightDbgPrint("MailEvt:ogf:OGF_APP,ocf:ASYNC_MONO");
		}
		else
        {
            LightDbgPrintWithOpcode((U8)HCI_VEVT_OCF_DRIVER_MAILBOX_EVT,
                                    (U8)HCI_VENDOR_OGF_UART_DRIVER_DBGPRINT,
                                    (U8)ogf, (U8)ocf);
        }

		Mailbox_StateHandler[ogf][ocf] (MailBox_State[ogf].CmdPtr);//(Mailbox_StateHandler[CMD_PTR->m2d_ctl.cmd.Ogf]+sizeof(U8 CODE_PTR)*(CMD_PTR->m2d_ctl.cmd.Ocf));
	}
	else
	{
		ASSERT(FALSE);
	}
}

void VP_END_EvtHandler(void)
{
	DRIVERMSG_STRU XDATA_PTR EVT_PTR;

	LightDbgPrint("DSP - DSP VP_END");
	if(Set_Prompt_Ctl[OGF_VP] != NULL)
		Set_Prompt_Ctl[OGF_VP](gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta);
	#ifdef SUPPORT_VOICE_PROMPT
	gVP_ctl.end_status = gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta;
	#endif

	EVT_PTR = (DRIVERMSG_STRU XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	MailboxSendEvtToDriver(EVT_PTR, OGF_VP, OCF_VP_STOP);

}
void RT_END_EvtHandler(void)
{
	DRIVERMSG_STRU XDATA_PTR EVT_PTR;

	LightDbgPrint("DSP - DSP RT_END");
	if(Set_Prompt_Ctl[OGF_RT] != NULL)
		Set_Prompt_Ctl[OGF_RT](gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta);
	#ifdef RINGTONE_ENABLE
	gRT_ctl.end_status = gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta;
	#endif

	EVT_PTR = (DRIVERMSG_STRU XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	MailboxSendEvtToDriver(EVT_PTR, OGF_RT, OCF_RT_STOP);
}
void VC_END_EvtHandler(void)
{
	DRIVERMSG_STRU XDATA_PTR EVT_PTR;

	LightDbgPrintWithOpcode((U8)HCI_VEVT_OCF_DRIVER_MAILBOX_VOICE_CMD_RESULT,
							(U8)HCI_VENDOR_OGF_UART_DRIVER_DBGPRINT,
							(U32)gAlignDSP.Mailbox.d2m_ctl.evt_word.para.vc_evt_para.vc_result);

	if(Set_Prompt_Ctl[OGF_VC] != NULL)
		Set_Prompt_Ctl[OGF_VC](gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta);
	#ifdef SUPPORT_VOICE_COMMAND
	gVC_ctl.end_status = gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta;
	gVC_ctl.result = gAlignDSP.Mailbox.d2m_ctl.evt_word.para.vc_evt_para.vc_result;
	LightDbgPrint("VC result: %d", (U8)gVC_ctl.result);
	#endif

	EVT_PTR = (DRIVERMSG_STRU XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	MailboxSendEvtToDriver(EVT_PTR, OGF_VC, OCF_VC_STOP);
}

void APP_END_EvtHandler(void)
{
	LightDbgPrint("APPEND");
}

void AT_END_EvtHandler(void)
{
	DRIVERMSG_STRU XDATA_PTR EVT_PTR;

	LightDbgPrint("DSP - DSP AT_END:%d",(U8) gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta);
	if(Set_Prompt_Ctl[OGF_AT] != NULL)
		Set_Prompt_Ctl[OGF_AT](gAlignDSP.Mailbox.d2m_ctl.evt_word.Sta);

	EVT_PTR = (DRIVERMSG_STRU XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	MailboxSendEvtToDriver(EVT_PTR, OGF_AT, OCF_AT_STOP);
}

void MailboxState_Reset(U8 OGF)
{

	MailBox_State[OGF].state = SUBSTATE_INIT;
	MailBox_State[OGF].ocf = 0xff;
	MailBox_State[OGF].CmdPtr = (DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR)NULL;

}

void MailboxSendEvtToDriver(DRIVERMSG_STRU XDATA_PTR EVT_PTR, U8 ogf, U8 ocf)
{
	if(!EVT_PTR)
	{
		//process for MEMGET FAIL, to do...
		ASSERT(FALSE);
	}
	else
	{
		//OSMEM_memset_xdata((U8 XDATA_PTR)EVT_PTR, 0, OSMEM1_BLKSIZE);
		EVT_PTR->ogf = ogf;
		EVT_PTR->ocf = ocf;
		DRIVER_ProcMailboxEvent((U8 XDATA_PTR)EVT_PTR);
	}
}

PUBLIC void Mailbox_TaskOgfVPRTAT(DriverMsg XDATA_PTR msgPtr, U8 ogf, U8 ocf)
{
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.task = TASK_VPRTAT;
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.cmd.Ogf = ogf;
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.cmd.Ocf = ocf;
	gMailBox_ctl.waitingDSPRingToneVPAT = TRUE;
}

PUBLIC void Mailbox_TaskOgfAPP(DriverMsg XDATA_PTR msgPtr, U8 ocf)
{
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.task = TASK_APP;
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.cmd.Ogf = OGF_APP;
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.cmd.Ocf = ocf;
	gMailBox_ctl.waitingDSPVoiceMusicAUX = TRUE;
}

PUBLIC void Mailbox_TaskOgfDSP(DriverMsg XDATA_PTR msgPtr, U8 ocf)
{
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.task = TASK_DSP;
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.cmd.Ogf = OGF_DSP;
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.cmd.Ocf = ocf;
	gMailBox_ctl.waitingDSPVolumeSet = TRUE;
	gMailBox_ctl.setVolumeAgain = FALSE;
}

#ifdef SUPPORT_VOICE_COMMAND
PUBLIC void Mailbox_TaskOgfVC(DriverMsg XDATA_PTR msgPtr, U8 ogf, U8 ocf)
{
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.task = TASK_VC;
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.cmd.Ogf = ogf;
	msgPtr->msgBodyPtr.driverCmd.mailBoxCmd.m2d_ctl.cmd.Ocf = ocf;
	gMailBox_ctl.waitingDSPVC = TRUE;
}
#endif

PUBLIC void Mailbox_ReleaseTASKVPRTATBusyFlag(void)
{
	gMailBox_ctl.waitingDSPRingToneVPAT = FALSE;
}

PUBLIC void Mailbox_ReleaseTASKAppBusyFlag(void)
{
	gMailBox_ctl.waitingDSPVoiceMusicAUX = FALSE;
}

PUBLIC void Mailbox_ReleaseTASKDSPBusyFlag(void)
{
	gMailBox_ctl.waitingDSPVolumeSet = FALSE;
}

PUBLIC void Mailbox_ReleaseTASKVCBusyFlag(void)
{
	gMailBox_ctl.waitingDSPVC = FALSE;
}