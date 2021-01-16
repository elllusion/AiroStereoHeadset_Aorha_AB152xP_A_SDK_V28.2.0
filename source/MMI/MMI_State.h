#ifndef _MMI_STATE_H_
	#define _MMI_STATE_H_

	#include "MMI_Base.h"
		
	#define MMI_EXIT_IMGCALL_STATE				0x01
	#define MMI_EXIT_CALLACTIVE_STATE			0x04
	#define MMI_EXIT_OUTGOING_STATE				0x08
	
	enum
	{
		CMD_SYSTEM_INIT,
		CMD_SECURITY_MODE_COMPLETE,
		CMD_SYSTEM_OFF,

		CMD_SET_CONNECTABLE,
		CMD_SET_NON_CONNECTABLE,
		CMD_SET_NON_CONNECTABLE_ANYWAY,
		CMD_SET_RESUME_CONNECTABLE,
		
		CMD_SPECIAL_LIMITED_DISCOVERABLE,
		CMD_LIMITED_DISCOVERABLE,
		CMD_SET_NORMAL_DISCOVERABLE,
		CMD_SET_NON_DISCOVERABLE,
		CMD_SET_NON_DISCOVERABLE_ANYWAY,
		CMD_SET_RESUME_DISCOVERABLE,
	};
	#define IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL	(!gMMI_nvram.timerCtl.ConnectableTimeout)


	typedef struct
	{
		void (*InitAction)(U8 linkIndex);
		void (*MsgHandler)(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
		void (*ExitAction)(U8 linkIndex);
	}MmiHandlerSetType;

	enum
	{
		TYPE_ONE_LINK, //add/remove on one link
		TYPE_ALL_LINK, //add/remove on all links
	};

//===================Function for external use=============================
	PUBLIC void MMI_STATE_Init(void);
	PUBLIC void MMI_STATE_PassThruMsg(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr);
	PUBLIC U8 MMI_GetTopStateByLink(U8 linkIndex);
	PUBLIC void MMI_AddTopState(U8 linkIndex, U8 state);
	PUBLIC void MMI_InsertState(U8 linkIndex, U8 state, U8 beforeState);
	PUBLIC void MMI_RmvState(U8 linkIndex, U8 state);
	PUBLIC void MMI_ReplaceStateWithEntry(U8 linkIndex, U8 newState, U8 oldState, BOOL withEntry);
	PUBLIC void MMI_ReplaceState(U8 linkIndex, U8 newState, U8 oldState);
	PUBLIC void MMI_CleanNestState(U8 linkIndex);
	PUBLIC BOOL MMI_CheckNestStateByLink(U8 linkIndex, U8 state);
	PUBLIC BOOL MMI_CheckNestState(U8 linkIndex, U8 state);
	PUBLIC BOOL MMI_CheckLinkInCallRelatedState(U8 link);
	PUBLIC BOOL MMI_AllLinksInCallRelatedState(void);
	PUBLIC BOOL MMI_CheckLinkInCallActiveState(U8 link);
	PUBLIC BOOL MMI_AllLinksInCallActiveState(void);
	PUBLIC BOOL MMI_CheckLinkInCallNotifyState(U8 link);
	PUBLIC BOOL MMI_CheckLinkInOutGoingState(U8 link);
	PUBLIC BOOL MMI_CheckLinkInIncomingState(U8 link);
	PUBLIC BOOL MMI_CheckInFMState(void);
	PUBLIC BOOL MMI_CheckInLineInState(void);
	PUBLIC void MMI_InitHfpParameters(void);
	
	PUBLIC void MMI_MMI1Handle(BOOL isOn);
	PUBLIC void MMI_ConnectabilityHandle(U8 action);
	PUBLIC void MMI_DiscoverabilityHandle(U8 action);
	PUBLIC void MMI_UpdateReconnectLinkInfo(void);
	PUBLIC U8 MMI_CountConnectedDev(void);
	
	PUBLIC void MMI_ConnectedEntry(U8 linkIndex);
	PUBLIC void MMI_ConnectedExit(U8 linkIndex);
#endif
