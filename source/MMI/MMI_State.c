
#include "reside_flash.inc"

#define _MMI_STATE_C_


#include "rc.h"
#include "bt_config_profile.h"

#include "os_flash.h"
#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Timer.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_HFP.h"
#include "MMI_HCI.h"
#include "MMI_A2DP.h"
#include "MMI_AirModule.h"
#include "MMI_LED.h"
#include "MMI_Reconnect.h"
#include "MMI_ChgBat.h"
#ifdef OLED_ENABLE
#include "..\Driver\OLED.h"
#endif
#ifdef MMI_LE_ENABLE
#include "..\MMI_LE\MMI_LE_interface.h"
#endif
#include "..\HC\HC_Interface.h"
#include "..\GAP\GAP_interface.h"
#include "..\A2DP\A2DP_Interface.h"


//===================Function for internal use=============================
PRIVATE void MMI_SendMMIReadyCmd(void);
PRIVATE void MMI_SendUpdateDriverParameterCmd(void);
PRIVATE void MMI_ConnectDiscoverableEntry(U8 linkIndex);
PRIVATE void MMI_ConnectDiscoverableHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_ConnectDiscoverableExit(U8 linkIndex);
PRIVATE void MMI_ConnectableEntry(U8 linkIndex);
PRIVATE void MMI_ConnectableHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);

PRIVATE void MMI_OffEntry(U8 linkIndex);
PRIVATE void MMI_OffHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_FakeonEntry(U8 linkIndex);
PRIVATE void MMI_FakeonHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_DetachEntry(U8 linkIndex);
PRIVATE void MMI_DetachHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
PRIVATE void MMI_DetachExit(U8 linkIndex);
PRIVATE void MMI_FakeoffEntry(U8 linkIndex);
PRIVATE void MMI_FakeoffHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);

PRIVATE BOOL MMI_AddTopState_ByLink(U8 linkIndex, U8 state);
PRIVATE BOOL MMI_InsertStateByLink(U8 linkIndex, U8 state, U8 beforeState);
PRIVATE BOOL MMI_ReplaceStateByLink(U8 i, U8 newState, U8 oldState);
PRIVATE void MMI_CleanNestStateByLink(U8 linkIndex, U8 isPowerOff);


PRIVATE MmiHandlerSetType CODE	MMI_StateOffHandler =
{
	MMI_OffEntry, MMI_OffHandler, NULL, //MMI_OFF
};

PRIVATE MmiHandlerSetType CODE	MMI_StateConnectDiscoverableHandler =
{
	MMI_ConnectDiscoverableEntry, MMI_ConnectDiscoverableHandler, MMI_ConnectDiscoverableExit, //MMI_CONDISCABLE
};

PRIVATE MmiHandlerSetType CODE	MMI_StateConnectableHandler =
{
	MMI_ConnectableEntry, MMI_ConnectableHandler, NULL, //MMI_CONNECTABLE
};

PRIVATE MmiHandlerSetType CODE	MMI_StateConnectedHandler =
{
	MMI_ConnectedEntry, NULL, MMI_ConnectedExit, //MMI_CONNECTED
};

PRIVATE MmiHandlerSetType CODE	MMI_StateFakeonHandler =
{
	MMI_FakeonEntry, MMI_FakeonHandler, NULL, //MMI_FAKEON
};

PRIVATE MmiHandlerSetType CODE	MMI_StateFakeoffHandler =
{
	MMI_FakeoffEntry, MMI_FakeoffHandler, NULL, //MMI_FAKEOFF
};

PRIVATE MmiHandlerSetType CODE	MMI_StateDetachHandler =
{
	MMI_DetachEntry, MMI_DetachHandler, MMI_DetachExit, //MMI_DETACHING_LINK
};

#ifdef SUPPORT_VOICE_PROMPT
PRIVATE void MMI_VoicePromptLangSelectEntry(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
	MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);

	gMMI_ctl.vpRuntime.vpLangStatus &= ~VP_STATUS_LANG_CONFIRM;

	gMMI_ctl.vpRuntime.vpSelectIndex = 0;
	gMMI_ctl.vpRuntime.vpSelectRound = 0;

	MMI_DRV_ChangeVoicePromptLangCandidate();
}

PRIVATE void MMI_VoicePromptLangSelectExit(U8 linkIndex)
{
	UNUSED(linkIndex);
	SYS_ReleaseTimer(&gMMI_ctl.vpRuntime.vpSelectTimerPtr);

	if (gMMI_ctl.vpRuntime.vpLangStatus & VP_STATUS_LANG_CONFIRM)
	{
		gMMI_driver_variation_nvram.misc_para.init.vpLangIndex = gMMI_ctl.vpRuntime.vpSelectIndex;
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}

	MMI_DRV_LoadVoiceLangCtl(gMMI_driver_variation_nvram.misc_para.init.vpLangIndex);

	if (gMMI_ctl.vpRuntime.vpLangStatus & VP_STATUS_LANG_CONFIRM)
	{
		gMMI_ctl.vpRuntime.vpLangStatus &= ~VP_STATUS_LANG_CONFIRM;
		MMI_PushMediaEvent(MEDIA_EVT_VOICE_PROMPT_LANG_CONFIRM);
	}

	MMI_ConnectabilityHandle(CMD_SET_CONNECTABLE);
}

PRIVATE MmiHandlerSetType CODE	MMI_StateVoicePromptLangSelectHandler =
{
	MMI_VoicePromptLangSelectEntry, NULL, MMI_VoicePromptLangSelectExit, //MMI_VOICE_PROMPT_LANG_SELECT
};
#endif

PUBLIC MmiHandlerSetType CODE_PTR XDATA MMI_StateHandlerSet[MMI_TOTAL_STATE_NO] =
{
	&MMI_StateOffHandler, //MMI_OFF
	&MMI_StateConnectDiscoverableHandler, //MMI_CONDISCABLE
	&MMI_StateConnectableHandler, //MMI_CONNECTABLE
	&MMI_StateConnectedHandler, //MMI_CONNECTED
	NULL, //MMI_HFP_INCOMMING
	NULL, //MMI_HFP_OUTGOING
	NULL, //MMI_HFP_CALLACTIVE
	NULL, //MMI_HFP_CALLACTIVE_WITHOUT_SCO
	NULL, //MMI_HFP_CAIMG,
	NULL, //MMI_HFP_CAOGG,
	NULL, //MMI_HFP_CAMULTY,
	&MMI_StateFakeonHandler, //MMI_FAKEON
	&MMI_StateFakeoffHandler, //MMI_FAKEOFF
	&MMI_StateDetachHandler, //MMI_DETACHING_LINK
	NULL, //MMI_TEST_MODE
	NULL, //MMI_FM
	NULL, //MMI_LINE_IN
	NULL, //MMI_UPDATING
	NULL, //MMI_UPDATED_OK
	NULL, //MMI_UPDATED_FAIL
#ifdef SUPPORT_VOICE_PROMPT
	&MMI_StateVoicePromptLangSelectHandler, //MMI_VOICE_PROMPT_LANG_SELECT
#else
	NULL, //MMI_VOICE_PROMPT_LANG_SELECT
#endif
	NULL, //MMI_TWS_PAIRING
	NULL, //MMI_INQUIRY
};

//declaration of state info table relatives
#define MAX_STATE_NESTING_LEVEL (MAX_MULTI_POINT_NO*7)
MmiStateType XDATA mmiSitPool[MAX_STATE_NESTING_LEVEL];
MmiStateType XDATA_PTR XDATA freeMmiLlSitPtr; //free link list of sit

PRIVATE void MMI_UnloadState(U8 linkIndex, U8 state)
{
	if (MMI_StateHandlerSet[state] && MMI_StateHandlerSet[state]->ExitAction != (void*)NULL)
		MMI_StateHandlerSet[state]->ExitAction(linkIndex);
}

PRIVATE void MMI_LoadState(U8 linkIndex, U8 state)
{
	if (MMI_StateHandlerSet[state] && MMI_StateHandlerSet[state]->InitAction != (void*)NULL)
		MMI_StateHandlerSet[state]->InitAction(linkIndex);

	#ifdef AIR_MODULE
	if(MMI_AIR_LoadState(linkIndex, state))
		return;
	#endif

	if (state == MMI_GetTopStateByLink(linkIndex))
	{
		if( (state == MMI_CONNECTED) && (CURRENT_ACTIVE_LINK_CNT == gMMI_ctl.currentSupportDeviceNo) )
		{
			#ifdef TWS_SETTINGS
			if(CURRENT_ACTIVE_LINK_CNT==2)
			{
				if(MMI_EOF == MMI_GetNextNormalLink(linkIndex) || !MMI_CheckLinkInCallRelatedState(MMI_GetNextNormalLink(linkIndex)))
					MMI_LED_SendBGLedCmd(gMMI_nvram.maxLinkLedSetting.entryLedDisplayIndex);
				else
					MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_GetTopStateByLink(MMI_GetNextNormalLink(linkIndex))].entryLedDisplayIndex);
			}
			else
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
			}
			#else
			if(MMI_EOF == MMI_GetNextNormalLink(linkIndex) || !MMI_CheckLinkInCallRelatedState(MMI_GetNextNormalLink(linkIndex)))
				MMI_LED_SendBGLedCmd(gMMI_nvram.maxLinkLedSetting.entryLedDisplayIndex);
			else
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[MMI_GetTopStateByLink(MMI_GetNextNormalLink(linkIndex))].entryLedDisplayIndex);
			#endif
		}
		else if( MMI_CheckProfileOnAllLinks() && (state == MMI_CONDISCABLE || state == MMI_CONNECTABLE))
		{
			return;
		}
		else
		{
			#ifdef TWS_SETTINGS
			if(( state == MMI_CONNECTED ) && (gMMI_ctl.currentSupportDeviceNo > 1))
			{
				MMI_LED_SendBGLedCmd(5);
			}
			else
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
			}
			#else
			#if (defined AB1520S) || (defined AB1520U) || (defined AB1525S)
			if( ! ( ( ( state == MMI_FAKEON ) || ( state == MMI_FAKEOFF ) ) && MMI_IsChargerIn() ) )
			#endif
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
			}
			#endif
		}
	}
	else //insert state or replace state
	{
		if(MMI_SHOW_CONDISCABLE_LED_ONLY_ONE_CONNECTED_DEVICE_FEAT)
		{
			if( (state == MMI_CONDISCABLE) && (CURRENT_ACTIVE_LINK_CNT == 1) )
			{
				MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
			}
		}
	}
}

PUBLIC void MMI_STATE_PassThruMsg(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	MmiStateType XDATA_PTR  currentStatePtr = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr;

	while(currentStatePtr != (MmiStateType XDATA_PTR)NULL)
	{
		if(MMI_StateHandlerSet[currentStatePtr->nestState] && MMI_StateHandlerSet[currentStatePtr->nestState]->MsgHandler)
		{
			MMI_StateHandlerSet[currentStatePtr->nestState]->MsgHandler(linkIndex, msgPtr);
		}

		if (gMMI_ctl.msgHandled != NOTPROCD)
			break;

		currentStatePtr = currentStatePtr->nextStatePtr;
	}
}

PUBLIC void MMI_STATE_Init(void)
{
	U8 idx;
	//To initialize the free Mmi state memory block and add basic state : FAKE ON
	for (idx=1; idx<MAX_STATE_NESTING_LEVEL; idx++)
	{
		mmiSitPool[idx].nextStatePtr = (MmiStateType XDATA_PTR)(&mmiSitPool[idx-1]);
	}
	freeMmiLlSitPtr = &mmiSitPool[MAX_STATE_NESTING_LEVEL-1];
	MMI_AddTopState(MMI_EOF, MMI_FAKEON);

	MMI_ConnectabilityHandle(CMD_SYSTEM_INIT);
}

PUBLIC U8 MMI_GetTopStateByLink(U8 linkIndex)
{
	U8 topState = MMI_TOTAL_STATE_NO;

	if (gMMI_ctl.mmiInfo[linkIndex].nestStatePtr != (MmiStateType XDATA_PTR)NULL)
	{
	 	topState = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr->nestState;
	}

	ASSERT(topState != MMI_TOTAL_STATE_NO);

 	return topState;
}

PRIVATE MmiStateType XDATA_PTR MMI_AllocMmiState(void)
{
	MmiStateType XDATA_PTR mmiStatePtr;

	ASSERT(freeMmiLlSitPtr != (MmiStateType XDATA_PTR)NULL);

	mmiStatePtr = freeMmiLlSitPtr;
	freeMmiLlSitPtr = mmiStatePtr->nextStatePtr;
	return mmiStatePtr;
}

PRIVATE U8 MMI_ReadStateType(U8 state)
{
	switch(state)
	{
		case MMI_DETACHING_LINK:
		case MMI_CONNECTABLE:
		case MMI_CONDISCABLE:
		case MMI_FAKEON:
		case MMI_FAKEOFF:
		case MMI_FM:
		case MMI_LINE_IN:
		case MMI_OFF:
		case MMI_VOICE_PROMPT_LANG_SELECT:
		case MMI_TWS_PAIRING:
		case MMI_TEST_MODE:
		case MMI_INQUIRY:
		case MMI_MP_STATE:
		case MMI_UPDATING:
		case MMI_UPDATED_FAIL:
			return TYPE_ALL_LINK;
		default:
			return TYPE_ONE_LINK;
	}
}

PRIVATE BOOL MMI_AddTopState_ByLink(U8 linkIndex, U8 state)
{
	MmiStateType XDATA_PTR newStatePtr;

	//Get memory from pool
	newStatePtr = MMI_AllocMmiState();

	newStatePtr->nextStatePtr = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr;
	newStatePtr->nestState = state;

	gMMI_ctl.mmiInfo[linkIndex].nestStatePtr = newStatePtr;
	return TRUE;
}

PUBLIC void MMI_AddTopState(U8 linkIndex, U8 state)
{
	U8 i;

	ASSERT(state < MMI_TOTAL_STATE_NO);

	if (MMI_CheckNestState(linkIndex, state))
	{
		return;
	}
	else
	{
		switch(MMI_ReadStateType(state))
		{
			case TYPE_ONE_LINK:
				if(MMI_AddTopState_ByLink(linkIndex, state))
				{
					MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_STATE_CHANGE, (U8 XDATA_PTR)NULL, linkIndex);
					MMI_LoadState(linkIndex, state);
				}
				break;
			case TYPE_ALL_LINK:
				for (i = 0;i < MAX_MULTI_POINT_NO;i++)
				{
					if(!MMI_AddTopState_ByLink(i, state))
						return;
				}
				MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_STATE_CHANGE, (U8 XDATA_PTR)NULL, MMI_EOF);
				MMI_LoadState(DEFAULT_LINK_IND, state);
				break;
		}
	}
}

PRIVATE BOOL MMI_InsertStateByLink(U8 linkIndex, U8 state, U8 beforeState)
{
	U8 LDATA statePara[2];
	MmiStateType XDATA_PTR currStatePtr, XDATA_PTR prevStatePtr;
	MmiStateType XDATA_PTR newStatePtr;

	prevStatePtr = currStatePtr = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr;
	while (currStatePtr != (MmiStateType XDATA_PTR)NULL)
	{
		if (currStatePtr->nestState == beforeState)
			break;
		prevStatePtr = currStatePtr;
		currStatePtr = currStatePtr->nextStatePtr;
	}

	//before state not found
	if (currStatePtr == (MmiStateType XDATA_PTR)NULL)
		return FALSE;

	//Get memory from pool
	newStatePtr = MMI_AllocMmiState();

	newStatePtr->nextStatePtr = currStatePtr;
	newStatePtr->nestState = state;

	if (prevStatePtr == currStatePtr) //the before state is the top state
	{
		gMMI_ctl.mmiInfo[linkIndex].nestStatePtr = newStatePtr;
		if (MMI_ReadStateType(state) == TYPE_ONE_LINK)
		{
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_STATE_CHANGE, (U8 XDATA_PTR)NULL, linkIndex);
		}
		else
		{
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_STATE_CHANGE, (U8 XDATA_PTR)NULL, MMI_EOF);
		}
	}
	else
	{
		prevStatePtr->nextStatePtr = newStatePtr;

		statePara[0] = state;
		statePara[1] = beforeState;
		MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_INSERT_STATE, (U8 XDATA_PTR)statePara, linkIndex);
	}
	return TRUE;
}

PRIVATE void MMI_CallStateSwitchVP(U8 linkIndex)
{
	if(gMMI_ctl.mmiInfo[linkIndex].stateEntryExit & MMI_EXIT_OUTGOING_STATE)
	{
		if(gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey)
			MMI_PushMediaEvent(MEDIA_EVT_KEY_CNLOUTGOING);
		else
			MMI_PushMediaEvent(MEDIA_EVT_AG_CNLOUTGOING);
	}
}

PUBLIC void MMI_InsertState(U8 linkIndex, U8 state, U8 beforeState)
{
	U8 i;

	ASSERT(state < MMI_TOTAL_STATE_NO);

	if (MMI_CheckNestState(linkIndex, state))
	{
		if(state == MMI_CONDISCABLE  && CURRENT_ACTIVE_LINK_CNT < gMMI_ctl.currentSupportDeviceNo && MMI_CONDISCABLE != MMI_GetTopStateByLink(DEFAULT_LINK_IND))
		{
			MMI_LoadState(DEFAULT_LINK_IND, state);
		}
		return;
	}
	else
	{
		switch(MMI_ReadStateType(state))
		{
			case TYPE_ONE_LINK:
				if (MMI_InsertStateByLink(linkIndex, state, beforeState))
				{
					MMI_LoadState(linkIndex, state);
				}
				break;
			case TYPE_ALL_LINK:
				ASSERT(MMI_ReadStateType(beforeState) == TYPE_ALL_LINK);
				for (i = 0;i < MAX_MULTI_POINT_NO;i++)
				{
					if (!MMI_InsertStateByLink(i, state, beforeState))
						return;
				}
				MMI_LoadState(DEFAULT_LINK_IND, state);
				break;
		}
	}


}

PRIVATE void MMI_FreeMmiState(MmiStateType XDATA_PTR mmiStatePtr)
{
	mmiStatePtr->nextStatePtr = freeMmiLlSitPtr;
	freeMmiLlSitPtr = mmiStatePtr;
}

PRIVATE BOOL MMI_RmvStateByLink(U8 linkIndex, U8 state)
{
	MmiStateType XDATA_PTR currStatePtr, XDATA_PTR prevStatePtr;

	prevStatePtr = currStatePtr = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr;
	while (currStatePtr != (MmiStateType XDATA_PTR)NULL)
	{
		if (currStatePtr->nestState == state)
			break;
		prevStatePtr = currStatePtr;
		currStatePtr = currStatePtr->nextStatePtr;
	}

	if (currStatePtr != (MmiStateType XDATA_PTR)NULL)
	{
		if (prevStatePtr == currStatePtr) //the removed state is the top state
			gMMI_ctl.mmiInfo[linkIndex].nestStatePtr = currStatePtr->nextStatePtr;
		else
			prevStatePtr->nextStatePtr = currStatePtr->nextStatePtr;

	   	MMI_FreeMmiState(currStatePtr);
		return TRUE;
	}
	return FALSE;
}

PUBLIC void MMI_RmvState(U8 linkIndex, U8 state)
{
	U8 i;
	U8 LDATA statePara[2];

	switch(MMI_ReadStateType(state))
	{
		case TYPE_ONE_LINK:
			if(!MMI_RmvStateByLink(linkIndex, state))
				return;
			statePara[1] = 0;
			break;
		case TYPE_ALL_LINK:
			for (i = 0;i < MAX_MULTI_POINT_NO;i++)
			{
				if (!MMI_RmvStateByLink(i, state))
					return;
			}
			statePara[1] = MMI_EOF;
			linkIndex = DEFAULT_LINK_IND;
			break;
	}

	statePara[0] = state;
	MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_REMOVE_STATE, (U8 XDATA_PTR)statePara, linkIndex);

	MMI_UnloadState(linkIndex, state);

	if( state== MMI_DETACHING_LINK)
		return;

	state = MMI_GetTopStateByLink(linkIndex);

	if (state != MMI_FAKEON && state != MMI_LINE_IN && state != MMI_FM)
	{
	 	MMI_LoadState(linkIndex, state);
	}
	else
	{
		MMI_LED_SendBGLedCmd(gMMI_nvram.defaultLedSetting[state].entryLedDisplayIndex);
	}
}

PRIVATE BOOL MMI_ReplaceStateByLink(U8 i, U8 newState, U8 oldState)
{
	U8 LDATA statePara[2];
	MmiStateType XDATA_PTR currentMmiLlSit = gMMI_ctl.mmiInfo[i].nestStatePtr;

	//Traversal the state list to find out specific state.
	while(currentMmiLlSit != (MmiStateType XDATA_PTR)NULL)
	{
		if (currentMmiLlSit->nestState == oldState)
		{
			currentMmiLlSit->nestState = newState;

			statePara[0] = newState;
			statePara[1] = oldState;
			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_REPLACE_STATE, (U8 XDATA_PTR)statePara, i);

			return TRUE;
		}
		else
		{
			currentMmiLlSit = currentMmiLlSit->nextStatePtr;
		}
	}
	return FALSE;
}

PUBLIC void MMI_ReplaceState(U8 linkIndex, U8 newState, U8 oldState)
{
	MMI_ReplaceStateWithEntry(linkIndex, newState, oldState, TRUE);
}

PUBLIC void MMI_ReplaceStateWithEntry(U8 linkIndex, U8 newState, U8 oldState, BOOL withEntry)
{
	U8 i;

	switch (MMI_ReadStateType(newState))
	{
		case TYPE_ONE_LINK:
			if (!MMI_ReplaceStateByLink(linkIndex, newState, oldState))
				return;
			break;
		case TYPE_ALL_LINK:
			for (i = 0;i < MAX_MULTI_POINT_NO;i++)
			{
				if (!MMI_ReplaceStateByLink(i, newState, oldState))
					return;
			}
			linkIndex = DEFAULT_LINK_IND;
			break;
	}


	MMI_UnloadState(linkIndex, oldState);

	if(withEntry)
		MMI_LoadState(linkIndex, newState);
}


PRIVATE void MMI_CleanNestStateByLink(U8 linkIndex, U8 isPowerOff)
{
	MmiStateType XDATA_PTR nsPtr;

	nsPtr = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr;

	while(nsPtr->nestState != MMI_FAKEON)
	{
		switch (MMI_ReadStateType(nsPtr->nestState))
		{
			case TYPE_ONE_LINK:
				MMI_RmvState(linkIndex, nsPtr->nestState);
				nsPtr = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr;
				break;
			case TYPE_ALL_LINK:
				if (isPowerOff)
				{
					MMI_RmvState(linkIndex, nsPtr->nestState);
					nsPtr = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr;
				}
				else
				{
					nsPtr = nsPtr->nextStatePtr;
				}
				break;
		}
	}
}

PRIVATE void _CleanNestStateByLink(void)//Mantis 8599 - avoid Warning C183 : Dead Assignment Eliminated
{
	U8 i;
	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		MMI_CleanNestStateByLink(i, TRUE);
	}
}

PUBLIC void MMI_CleanNestState(U8 linkIndex)
{
	if (linkIndex == MMI_EOF)
	{
		_CleanNestStateByLink();
	}
	else
	{
		MMI_CleanNestStateByLink(linkIndex, FALSE);
	}
}

PUBLIC BOOL MMI_CheckNestStateByLink(U8 linkIndex, U8 state)
{
	MmiStateType XDATA_PTR nsPtr = gMMI_ctl.mmiInfo[linkIndex].nestStatePtr;

	while(nsPtr != (MmiStateType XDATA_PTR)NULL && nsPtr->nestState != MMI_FAKEON)
	{
		if (nsPtr->nestState == state)
		{
			return TRUE;
		}
		else
		{
			nsPtr = nsPtr->nextStatePtr;
		}
	}

	return FALSE;
}

PUBLIC BOOL MMI_CheckNestState(U8 linkIndex, U8 state)
{
	if(linkIndex == MMI_EOF)
		return 	MMI_CheckNestStateByLink(DEFAULT_LINK_IND, state);
	else
		return 	MMI_CheckNestStateByLink(linkIndex, state);
}


PUBLIC BOOL MMI_CheckLinkInCallRelatedState(U8 link)
{
	if (gMMI_ctl.mmiInfo[link].nestStatePtr->nestState >= MMI_HFP_INCOMMING &&
			gMMI_ctl.mmiInfo[link].nestStatePtr->nestState <= MMI_HFP_CAMULTY)
	{
		return TRUE;
	}
	else
		return FALSE;
}

PUBLIC BOOL MMI_AllLinksInCallRelatedState(void)
{
	U8 i;

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(MMI_CheckLinkInCallRelatedState(i))
			return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL MMI_CheckLinkInCallActiveState(U8 link)
{
	if (link < MAX_MULTI_POINT_NO && gMMI_ctl.mmiInfo[link].nestStatePtr->nestState >= MMI_HFP_CALLACTIVE&&
		gMMI_ctl.mmiInfo[link].nestStatePtr->nestState <= MMI_HFP_CAMULTY)
	{
		return TRUE;
	}
	else
		return FALSE;
}

PUBLIC BOOL MMI_AllLinksInCallActiveState(void)
{
	U8 i;

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(MMI_CheckLinkInCallActiveState(i))
			return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL MMI_CheckLinkInCallNotifyState(U8 link)
{
	if (link < MAX_MULTI_POINT_NO && gMMI_ctl.mmiInfo[link].nestStatePtr->nestState >= MMI_HFP_INCOMMING&&
		gMMI_ctl.mmiInfo[link].nestStatePtr->nestState <= MMI_HFP_OUTGOING)
	{
		return TRUE;
	}
	else
		return FALSE;
}

PUBLIC BOOL MMI_CheckLinkInOutGoingState(U8 link)
{
	if ( gMMI_ctl.mmiInfo[link].nestStatePtr->nestState == MMI_HFP_OUTGOING)
	{
		return TRUE;
	}
	else
		return FALSE;
}

PUBLIC BOOL MMI_CheckLinkInIncomingState(U8 link)
{
	if ( gMMI_ctl.mmiInfo[link].nestStatePtr->nestState == MMI_HFP_INCOMMING)
	{
		return TRUE;
	}
	else
		return FALSE;
}

PUBLIC BOOL MMI_CheckInFMState(void)
{
	U8 i;
	for(i = 0;  i < MAX_MULTI_POINT_NO; i++)
	{
		if(gMMI_ctl.mmiInfo[i].nestStatePtr->nestState == MMI_FM)
			return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL MMI_CheckInLineInState(void)
{
	U8 i;
	for(i = 0;  i < MAX_MULTI_POINT_NO; i++)
	{
		if(gMMI_ctl.mmiInfo[i].nestStatePtr->nestState == MMI_LINE_IN)
			return TRUE;
	}
	return FALSE;
}

PUBLIC void MMI_MMI1Handle(BOOL isOn)
{
	if(isOn)
	{
		if(CURRENT_ACTIVE_LINK_CNT < 2)
			OS_MMI1Enable ((U16)gMMI_nvram.mmi1IntInterval.interval_1To1);
		else
			OS_MMI1Enable ((U16)gMMI_nvram.mmi1IntInterval.interval_1To2);

		SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.delayDisableMMI1Timer);
	}
	else
	{
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.delayDisableMMI1Timer,
			(U32)(gHC_parameter.page_scan_interval * 2 * 2));//interval(slots) * 2 = half_slots
			//wait for 2 intervals to avoid scan missing;
	}
}

PUBLIC void MMI_ConnectabilityHandle(U8 action)
{
	// MMI1 interrupt should be set when page scan is off. Otherwise system will fall asleep.
	// init(MMI1) --> connectable (scan) --> connected(MMI1)
	switch(action)
	{
		case CMD_SYSTEM_INIT:
			MMI_MMI1Handle(TRUE);
			break;

		case CMD_SECURITY_MODE_COMPLETE:
			MMI_GAP_WriteConnectabilityMode(SAC_CONNECTABLE_MODE | CONNECTABLE_MODE);
			#ifdef AIR_MODULE
			MMI_AIR_WriteSecurityModeComplete();
			#endif
			break;

		case CMD_SET_CONNECTABLE:
			if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
				return;
			if (CURRENT_ACTIVE_LINK_CNT < gMMI_ctl.currentSupportDeviceNo)
			{
				MMI_GAP_WriteConnectabilityMode(SAC_CONNECTABLE_MODE | CONNECTABLE_MODE);
				if (gMMI_nvram.timerCtl.ConnectableTimeout)
				{
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer, (U32)(gMMI_nvram.timerCtl.ConnectableTimeout) * ONE_SEC);
				}
			}
			break;

		case CMD_SET_RESUME_CONNECTABLE:
			if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
				return;
			if((IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL && CURRENT_ACTIVE_LINK_CNT < gMMI_ctl.currentSupportDeviceNo) ||
				(!IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL && gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer))
			{
				MMI_GAP_WriteConnectabilityMode(SAC_CONNECTABLE_MODE | CONNECTABLE_MODE);
			}
			break;

		case CMD_SET_NON_CONNECTABLE:
			if(IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL)
			{
				return;
			}
			/* fallthrough */
		case CMD_SET_NON_CONNECTABLE_ANYWAY:
			if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
			{
				SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer);
				return;
			}
			/* fallthrough */
		case CMD_SYSTEM_OFF:
			MMI_GAP_WriteConnectabilityMode(SAC_NON_CONNECTABLE_MODE | NON_CONNECTABLE_MODE);
			SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.connectableTimeoutTimer);
			break;

	}
}

PUBLIC void MMI_DiscoverabilityHandle(U8 action)
{
	switch(action)
	{
		case CMD_SPECIAL_LIMITED_DISCOVERABLE:
			if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
				return;
			MMI_GAP_WriteDiscoverabilityMode(SPECIAL_LIMITED_DISCOVERABLE_MODE |LIMITED_DISCOVERABLE_MODE | SAC_LIMITED_DISCOVERABLE_MODE);
			break;
		case CMD_LIMITED_DISCOVERABLE:
			if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
				return;
			MMI_GAP_WriteDiscoverabilityMode(LIMITED_DISCOVERABLE_MODE | SAC_LIMITED_DISCOVERABLE_MODE);
			break;

		case CMD_SET_NORMAL_DISCOVERABLE:
			if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
				return;
			MMI_GAP_WriteDiscoverabilityMode(SAC_GENERAL_DISCOVERABLE_MODE | GENERAL_DISCOVERABLE_MODE);
			if(!MMI_ALWAYS_ENA_DISCOVER_MODE_FEAT)
			{
				if (MMI_LinkKey_GetPDLNumber() == 0)
				{
					if(gMMI_nvram.timerCtl.PairModeTimeoutIfPDLIsEmpty)
						SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.pairModeTimer, (U32)(gMMI_nvram.timerCtl.PairModeTimeoutIfPDLIsEmpty) * ONE_SEC);
				}
				else
				{
					if(gMMI_nvram.timerCtl.PairModeTimeout)
						SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.pairModeTimer, (U32)(gMMI_nvram.timerCtl.PairModeTimeout) * ONE_SEC);
				}
			}
			break;

		case CMD_SET_NON_DISCOVERABLE:
			if(MMI_ALWAYS_ENA_DISCOVER_MODE_FEAT)
				return;
		case CMD_SET_NON_DISCOVERABLE_ANYWAY:
			if(MMI_KEY_STATE_POWER_OFF == MMI_GetKeyState())
			{
				SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.pairModeTimer);
				return;
			}
		case CMD_SYSTEM_OFF:
			MMI_GAP_WriteDiscoverabilityMode(NON_DISCOVERABLE_MODE | SAC_NON_DISCOVERABLE_MODE);
			SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.pairModeTimer);
			break;

		case CMD_SET_RESUME_DISCOVERABLE:
			if((MMI_ALWAYS_ENA_DISCOVER_MODE_FEAT && gMMI_ctl.currentSupportDeviceNo > CURRENT_ACTIVE_LINK_CNT)  ||
				(!MMI_ALWAYS_ENA_DISCOVER_MODE_FEAT && gMMI_ctl.mmiTimerInfo.sysTimer.pairModeTimer) )
				MMI_GAP_WriteDiscoverabilityMode(SAC_GENERAL_DISCOVERABLE_MODE | GENERAL_DISCOVERABLE_MODE);
			break;


	}
}

PRIVATE void _UpdateReconnectLinkInfo(U8 i)//Mantis 8599 - avoid Warning C183 : Dead Assignment Eliminated
{
	U8 j;
	for(j = 0; j < MAX_MULTI_POINT_NO; j++)
	{
		if(SYS_IsBDAddrIsTheSame(&(gMMI_ctl.mmiInfo[i].bdAddr[0]), &gMMI_driver_variation_nvram.misc_para.agInd[j].member[0]))
		{
			if (MMI_IsActiveLink(i))
				gMMI_ctl.miscMask4|= (MMI_MASK4_RECONNECT_LINK0 << j);
			else
				gMMI_ctl.miscMask4 &= ~(MMI_MASK4_RECONNECT_LINK0 << j);
		}
	}
}

//This function is used to update agInd[0] and agInd[1] with current active links
PUBLIC void MMI_UpdateReconnectLinkInfo(void)
{
	U8 i;

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		_UpdateReconnectLinkInfo(i);
	}
}

PUBLIC U8 MMI_CountConnectedDev(void)
{
	U8 link, count = 0;

	for(link = 0;link < MAX_MULTI_POINT_NO;link++)
	{
		if(MMI_CheckNestState(link,MMI_CONNECTED))
			count++;
	}
	return count;
}

///////////////////////////////////////////////////////////////////////////
PRIVATE void MMI_SendMMIReadyCmd(void)
{

	MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_MMI_READY_CMD);

	if(msgPtr != (MMIMsgType XDATA_PTR)NULL)
	{
		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PRIVATE void MMI_SendUpdateDriverParameterCmd(void)
{
	MMIMsgType XDATA_PTR msgPtr;

	//Send update msg for driver paramenters to DRIVER task
	if ((msgPtr = MMI_GetCommandMessageMem(DRIVER_UPDATE_GENERAL_PARA_CMD)) != (MMIMsgType XDATA_PTR)NULL)
	{
		OSMQ_Put (OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}
}


//========================================================================
//========================================================================
//=============================State Functions=================================
//========================================================================
//========================================================================


PRIVATE void MMI_ConnectDiscoverableEntry(U8 linkIndex)
{
	UNUSED(linkIndex);
	if (!gMMI_ctl.mmiTimerInfo.sysTimer.watchDogRebootNoMediaTimer)
	{
		if(	!gMMI_ctl.activeLinkRecord && !MMI_DRV_IsAudioDeviceRegistered(MMI_EOF, AUDIO_DEVICE_FM)
			&& !MMI_DRV_IsAudioDeviceRegistered(MMI_EOF, AUDIO_DEVICE_LINE_IN)
			#ifdef DEMOSOUND
			&& !MMI_DRV_IsAudioDeviceRegistered(MMI_EOF, AUDIO_DEVICE_DEMO_SOUND)
			#endif
			)
		{
			MMI_PushMediaEvent(MEDIA_EVT_ENTER_DISCOVERABLE);
		}
	}


	MMI_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);

	if(MMI_CheckNestState(MMI_EOF, MMI_CONNECTABLE))
		MMI_ConnectabilityHandle(CMD_SET_CONNECTABLE);

	//Connectable state to ConDiscoverable state
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffTimer);
}

PRIVATE void MMI_ConnectDiscoverableHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(linkIndex);
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch (msgPtr->eventCode)
	{
		case MMI_CHECK_POWERON_TIMER:
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}


PRIVATE void MMI_ConnectDiscoverableExit(U8 linkIndex)
{
	UNUSED(linkIndex);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.pairModeTimer);

	MMI_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
	MMI_PushMediaEvent(MEDIA_EVT_EXIT_DISCOVERABLE);
	gMMI_ctl.miscMask2 |= MMI_EXIT_DISCOVERABLE_STATE;
}


PRIVATE void MMI_ConnectableEntry(U8 linkIndex)
{
	UNUSED(linkIndex);
	if(!(gMMI_ctl.miscMask2 & MMI_EXIT_DISCOVERABLE_STATE))
	{
		if(MMI_ALWAYS_ENA_DISCOVER_MODE_FEAT||(MMI_DIS_PDL_LESS_THAN > MMI_LinkKey_GetPDLNumber()))
		{
			MMI_SetEnterDiscoverable(TRUE);
		}
	}
	gMMI_ctl.miscMask2 &= ~MMI_EXIT_DISCOVERABLE_STATE;
}

PRIVATE void MMI_ConnectableHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(linkIndex);
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch (msgPtr->eventCode)
	{
		case MMI_CHECK_POWERON_TIMER:
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}


PUBLIC void MMI_ConnectedEntry(U8 linkIndex)
{
	if(gMMI_ctl.mmiInfo[linkIndex].stateEntryExit & MMI_EXIT_CALLACTIVE_STATE)
	{
		if(gMMI_ctl.mmiInfo[linkIndex].stateEntryExit & MMI_EXIT_IMGCALL_STATE)
			gMMI_ctl.mmiInfo[linkIndex].stateEntryExit |= ~MMI_EXIT_IMGCALL_STATE;
	}
	#ifdef HFP_Profile
	MMI_DRV_ScoModeControl(linkIndex, TRUE, FALSE);
	if((gMMI_ctl.mmiInfo[linkIndex].stateEntryExit &(MMI_EXIT_OUTGOING_STATE|MMI_EXIT_CALLACTIVE_STATE)))
	{
		if(gMMI_ctl.mmiInfo[linkIndex].linkPara.scoIsActiveEstablished)
		{
			if(SCO_EXISTS(linkIndex))
			{
				MMI_HCI_SendRelSco(linkIndex);
			}
			else
			{
				gMMI_HFP_ctl.HfpCtl[linkIndex].relSCOCozExitOggState = TRUE;
			}
		}
	}
	#ifndef HFP_AG_Profile
	if((gMMI_ctl.mmiInfo[linkIndex].stateEntryExit &(MMI_EXIT_OUTGOING_STATE | MMI_EXIT_CALLACTIVE_STATE | MMI_EXIT_IMGCALL_STATE)))
	{
		if(MMI_SUPPORT_MULTI_HFP_FEAT)
		{
			U8 nextLink;
			nextLink = MMI_HFP_GetNextHFPLink(linkIndex);
			if(nextLink != MMI_EOF && MMI_CheckLinkInCallRelatedState(nextLink) && SCO_EXISTS(nextLink))
			{
				MMI_DRV_SetSpkMuteByLink(SET_MUTE_OFF, nextLink);
			}
		}
	}
	#endif
	#endif

	MMI_CallStateSwitchVP(linkIndex);

	if(gMMI_ctl.mmiInfo[linkIndex].stateEntryExit & (MMI_EXIT_IMGCALL_STATE|MMI_EXIT_OUTGOING_STATE|MMI_EXIT_CALLACTIVE_STATE))
	{
		gMMI_ctl.mmiInfo[linkIndex].stateEntryExit &= ~(MMI_EXIT_IMGCALL_STATE|MMI_EXIT_OUTGOING_STATE|MMI_EXIT_CALLACTIVE_STATE);
		#ifdef HFP_Profile
		if(MMI_HFP_IsHSPConnected(linkIndex))
		{
			MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_SCO_HSP);
			MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO_HSP);
		}
		else
		#endif
		{
			MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_SCO);
			MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
			MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_SCO);
			MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
		}
	}
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffTimer);

	#ifdef MMI_BATCHG_ENABLE
	MMI_CHG_ResetTimeOutDectionTimer();
	#endif
	if(gMMI_ctl.mmiInfo[linkIndex].subState < MMI_VDIAL_CONNECTING_HFP || gMMI_ctl.mmiInfo[linkIndex].subState > MMI_RDIAL_STARTING)
	{
		gMMI_ctl.mmiInfo[linkIndex].subState = MMI_SBY_NONE_SUBSTATE;
	}

	/// Set Volume as history
	if(gMMI_ctl.serviceInd == linkIndex && !MMI_CheckInLineInState() && !MMI_CheckInFMState())
	{
		MMI_DRV_SendDriverVolCmd(linkIndex, CURRENT_SCO_SOUND_LEVEL(linkIndex), CURRENT_SCO_SMALL_STEP(linkIndex), VOLUME_MIC_FOLLOW_SPK);
	}
	gMMI_ctl.mmiInfo[linkIndex].isCallRejectedByKey = FALSE;
	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);
	MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_CALL_STATE);
}

PUBLIC void MMI_ConnectedExit(U8 linkIndex)
{
	U8 keyState;

	MMI_DRV_AudioDeviceDeRegister(linkIndex, AUDIO_DEVICE_CALL_STATE);

	keyState = MMI_GetKeyState();
	// Not detach after audio transfer
	if (MMI_KEY_STATE_POWER_OFF != keyState
		#ifdef AIR_MODULE
		&& MMI_KEY_STATE_POWER_OFF_SYNC != keyState
		#endif
		)
	{
		#ifdef HFP_Profile
		if(!gMMI_HFP_ctl.HfpCtl[linkIndex].audioTransferDetachTimer)
		#endif
		{
			if(gMMI_ctl.mmiInfo[linkIndex].linkPara.miscMask & MMI_LINK_LOSS)
				MMI_PushMediaEvent(MEDIA_EVT_LINK_LOSS);
			else
			{
    			MMI_DRV_SetOTAVoicepromptsExist(TRUE);
				MMI_PushMediaEvent(MEDIA_EVT_SLC_DISCONNECTED);
			}
		}
		MMI_DRV_ResumeOtherAudioDevice(linkIndex, AUDIO_DEVICE_CALL_STATE);
	}
	MMI_SetEnterDiscoverable(FALSE);

	if(MMI_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && MMI_DRV_IsActiveAudioDevInAUX())
	{
		SYS_SetTimer(&gMMI_ctl.audioDevCtl.resumeTimer, 2*ONE_SEC);
	}

	#ifdef AIR_MODULE
	MMI_AIR_ConnectedExit(linkIndex);
	#endif
}

PRIVATE void MMI_OffEntry(U8 linkIndex)
{
	UNUSED(linkIndex);
	//Power off procedure will be activated while ringtone is finished
	MMI_PushMediaEvent(MEDIA_EVT_OFF_STATE);
	//Release timers
	MMI_TIMER_ReleaseAllLinksConnectionTimer();

	MMI_ConnectabilityHandle(CMD_SYSTEM_OFF);
}

PRIVATE void MMI_OffHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(linkIndex);
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT:
			if(	msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id == MEDIA_EVT_OFF_STATE)
			{
				MMI_DRV_SendRingtoneStopCmd();
				MMI_DRV_SendVoicepromptStopCmd();
				MMI_SendMMIReadyCmd();
			}
			break;

		case MMI_PWROFF_CMD:
			MMI_SystemOff();
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

PRIVATE void MMI_FakeonEntry(U8 linkIndex)
{
	UNUSED(linkIndex);

	MMI_SendUpdateDriverParameterCmd();

	MMI_SendInternalCmdToMMI(MMI_CHECK_POWERON_TIMER);

	SetPowerSavingForDevice(FALSE, PWRSAV_FAKEON);
}

PRIVATE void MMI_FakeonHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	U8 isWakeUpPressed;
	UNUSED(linkIndex);
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case MMI_CHECK_POWERON_TIMER:
			// The watchdog reset condition
			// After watchdog reset, SYS_SW1_BOOT_FLAG_POWER_ON_READY will not be cleared.
			// System will rely on this condition to self power-on.
		#ifndef OPERATION_IN_BB_FPGA_VERSION
			if (MMI_CheckBootFlag(SYS_SW1_BOOT_FLAG_POWER_ON_READY) && MMI_GetKeyState() == MMI_KEY_STATE_NONE  && !MMI_UKEY_SWITCH_ONOFF_TYPE_FEAT)
			{
				if(MMI_DO_NOT_RING_WHEN_WATCHDOG_TO_PWON_FEAT)
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.watchDogRebootNoMediaTimer, 2*ONE_SEC);

				MMI_PowerOnLoader();
				#ifdef OLED_ENABLE
				OLED_DisplayOn(OLED_SHOW_START);
				#endif

				if(MMI_CheckBootFlag(SYS_SW1_BOOT_FLAG_ENTER_MP_TEST_MODE))
				{
					gMMI_ctl.currentSupportDeviceNo = 1;
					gMMI_nvram.timerCtl.PairModeTimeout = 0;
					gMMI_nvram.timerCtl.PairModeTimeoutIfPDLIsEmpty = 0;
					MMI_HCI_SendEnableDeviceUnderTestMode();
					MMI_SetEnterDiscoverable(TRUE);
				}
				else
					MMI_SetWatchDogReboot(TRUE);
			}
			else
		#endif
			{
				if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.autoPowerOnTimer))
				{
					if(gMMI_nvram.timerCtl.AutoPowerOnTimer)  // Not Auto Power On
					{
						READ_WAKEUP_DEBOUNCE(isWakeUpPressed);
						if (isWakeUpPressed & 0x01)
						{
							SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoPowerOnTimer, (U32)(gMMI_nvram.timerCtl.AutoPowerOnTimer)*320);

							MMI_SendInternalCmdToMMI(MMI_CHECK_POWERON_TIMER);
						}
						else
						{
							MMI_SystemOff();   //MMI should power off immediately
						}
					}
					else  // Auto Power On
					{
						READ_WAKEUP_DEBOUNCE(isWakeUpPressed);
						//Key Pressed wait until key event
						if (isWakeUpPressed & 0x01)
						{
							MMI_SendInternalCmdToMMI(MMI_CHECK_POWERON_TIMER);
						}
						else
						{
							//KeyReleased
							if (1
								#ifndef BLUETOOTH_SPEAKER
								&& !MMI_IsChargerIn() && !gSYS_init_charger_exist
								#endif
								#ifdef OPERATION_IN_BB_ASIC_VERSION
								&& gSYS_init_wakeupkey_exist
								#endif
								)
							{
								MMI_PowerOnLoader();
								#ifdef OLED_ENABLE
								OLED_DisplayOn(OLED_SHOW_START);
								#endif
							}
							else
							{
								MMI_SystemOff();
							}
						}
					}
				}
				else  // Not Auto Power On
				{
					MMI_SendInternalCmdToMMI(MMI_CHECK_POWERON_TIMER);
				}
			}
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

PRIVATE void MMI_FakeoffEntry(U8 linkIndex)
{
	UNUSED(linkIndex);
	MMI_DiscoverabilityHandle(CMD_SYSTEM_OFF);
	MMI_ConnectabilityHandle(CMD_SYSTEM_OFF);

	//SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoPowerOnTimer);


	MMI_PowerSaving(TRUE, MMI_PWRSAVING_WRITE_CONNECTABILITY, MAX_MULTI_POINT_NO);
	SetPowerSavingForDevice(TRUE, PWRSAV_PWRON);
	SetPowerSavingForDevice(TRUE, PWRSAV_FAKEON);

	//Power off procedure will be activated while ringtone is finished
	if (!MMI_IS_POWER_ON())
		return;
	//Close BLE ADV
	#ifdef MMI_LE_ENABLE
	MMI_LE_DisableBleLink();
	#endif

	MMI_PushMediaEvent(MEDIA_EVT_OFF_STATE);

	//Release timers
	MMI_TIMER_ReleaseAllLinksConnectionTimer();

	//Clear low battery power on flag
	MMI_DRV_SetNoConnectivityPwOnLowBat(FALSE);
	MMI_DRV_SetKeyInvalidPwOnLowBat(FALSE);

	//Clear	discoverable flag
	gMMI_ctl.miscMask2 &= ~(MMI_EXIT_DISCOVERABLE_STATE);
	MMI_SetEnterDiscoverable(FALSE);
}

PRIVATE void MMI_FakeoffHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(linkIndex);
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT:
			if(	msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id == MEDIA_EVT_OFF_STATE)
			{
				MMI_DRV_SendRingtoneStopCmd();
				MMI_DRV_SendVoicepromptStopCmd();
				MMI_SendMMIReadyCmd();
			}
			break;

		#ifdef BLUETOOTH_SPEAKER
		case MMI_CHECK_POWERON_TIMER:
			// The watchdog reset condition
			// After watchdog reset, SYS_SW1_BOOT_FLAG_POWER_ON_READY will not be cleared.
			// System will rely on this condition to self power-on.
			#ifndef OPERATION_IN_BB_FPGA_VERSION
			if (MMI_CheckBootFlag(SYS_SW1_BOOT_FLAG_POWER_ON_READY) && MMI_GetKeyState() == MMI_KEY_STATE_NONE  && !MMI_UKEY_SWITCH_ONOFF_TYPE_FEAT)
			{
				MMI_SetWatchDogReboot(TRUE);

				if(MMI_DO_NOT_RING_WHEN_WATCHDOG_TO_PWON_FEAT)
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.watchDogRebootNoMediaTimer, 2*ONE_SEC);

				MMI_PowerOnLoader();
				#ifdef OLED_ENABLE
				OLED_DisplayOn(OLED_SHOW_START);
				#endif
			}
			else
			#endif
			{
				if(SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.autoPowerOnTimer))
				{
					U8 isWakeUpPressed;

					// Auto Power On
					READ_WAKEUP_DEBOUNCE(isWakeUpPressed);
					//Key Pressed wait until key event
					if (isWakeUpPressed & 0x01)
					{
						MMI_SendInternalCmdToMMI(MMI_CHECK_POWERON_TIMER);
					}
					else
					{
						//KeyReleased
						if (1
							#ifdef OPERATION_IN_BB_ASIC_VERSION
							&& gSYS_init_wakeupkey_exist
							#endif
							)
						{
							MMI_PowerOnLoader();
							#ifdef OLED_ENABLE
							OLED_DisplayOn(OLED_SHOW_START);
							#endif
						}
						else
						{
							SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoPowerOnTimer, (U32)(gMMI_nvram.timerCtl.AutoPowerOnTimer)*320);
							MMI_SendInternalCmdToMMI(MMI_CHECK_POWERON_TIMER);
						}
					}
				}
			}
			break;
		#endif

		case MMI_PWROFF_CMD:
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

PRIVATE void MMI_DetachEntry(U8 linkIndex)
{
	UNUSED(linkIndex);

	MMI_SendInternalCmdToMMI(MMI_DETACH_CMD_TO_MMI);
	MMI_ConnectabilityHandle(CMD_SYSTEM_OFF);

	SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.detachLinkTimeExpireTimer, (U32) 6* ONE_SEC);
}

PRIVATE void MMI_DetachHandler(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	UNUSED(linkIndex);
	gMMI_ctl.msgHandled = PROCD_RELEASE;

	switch(msgPtr->eventCode)
	{
		case MMI_DETACH_CMD_TO_MMI:
			if (gMMI_ctl.activeLinkRecord != 0x00)
			{
				//To loop checking
				if (SYS_IsTimerExpired(&gMMI_ctl.mmiTimerInfo.sysTimer.detachLinkTimeExpireTimer))
				{
					if (MMI_IsChargerIn())
					{
						PMU_SYS_SET_BOOT_SW_STATUS_B1 (0);
						MMI_Reset();
					}
					else
					{
						MMI_PowerOffProce();
					}
				}
				else
				{
					MMI_SendInternalCmdToMMI(MMI_DETACH_CMD_TO_MMI);
				}
			}
			else
			{
				MMI_PowerOffProce();
			}
			break;
		case DRIVER_FAKE_MEDIA_VP_RINGTONE_EVENT:
			if(	msgPtr->msgBodyPtr.driverEvt.fakeMediaEvt.id == MEDIA_EVT_OFF_STATE)
			{
				MMI_DRV_SendRingtoneStopCmd();
				MMI_DRV_SendVoicepromptStopCmd();
				MMI_SendMMIReadyCmd();
			}
			break;

		case MMI_PWROFF_CMD:
			MMI_SendInternalCmdToMMI(MMI_PWROFF_CMD);
			break;

		default:
			gMMI_ctl.msgHandled = NOTPROCD;
			break;
	}
}

PRIVATE void MMI_DetachExit(U8 linkIndex)
{
	UNUSED(linkIndex);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.detachLinkTimeExpireTimer);
}
