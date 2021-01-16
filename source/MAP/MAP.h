 //////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _MAP_H_
#define _MAP_H_

#include "MAP_interface.h"
#include "..\OBEX\OBEX_interface.h"
#include "..\MMI\MMI_Base.h"


enum
{
	MAP_IDLE_STATE = MAP_INVALID,
	MAP_CREATING_STATE = MAP_CONNECT_CMD,
	MAP_DISCONNECTING_STATE = MAP_DISCONNECT_CMD,
	MAP_SET_NOTIFI_REG_ON_STATE = MAP_SET_NOTIFI_REG_ON_CMD,
	MAP_SET_NOTIFI_REG_OFF_STATE = MAP_SET_NOTIFI_REG_OFF_CMD,
	MAP_SET_FOLDER_STATE = MAP_SET_FOLDER_CMD,
	MAP_GET_FOLDER_LISTING_STATE = MAP_GET_FOLDER_LISTING_CMD,
	MAP_GET_MESSAGES_LISTING_STATE = MAP_GET_MESSAGES_LISTING_CMD,
	MAP_GET_MESSAGE_STATE = MAP_GET_MESSAGE_CMD,
	MAP_SET_MESSAGE_STATUS_READ_STATE = MAP_SET_MESSAGE_STATUS_READ_CMD,
	MAP_SET_MESSAGE_STATUS_DELETE_STATE = MAP_SET_MESSAGE_STATUS_DELETE_CMD,
  	MAP_PUSH_MESSAGE_STATE = MAP_PUSH_MESSAGE_CMD,
  	MAP_UPDATE_INBOX_STATE = MAP_UPDATE_INBOX_CMD,
	MAP_NUM_OF_STATE,
};

enum
{
	MAP_SUBSTATE_INIT,
	MAP_SUBSTATE_01,
	MAP_SUBSTATE_02,
	MAP_SUBSTATE_03,
	MAP_SUBSTATE_04,
	MAP_SUBSTATE_05,
	MAP_SUBSTATE_END = 0xFF,
};

enum
{
	MAP_SET_MSG_READ,
	MAP_SET_MSG_DELETE,
};

typedef struct
{
	U8 state;
	U8 subState;
		
	U16 totalDataLength;
	U16 leftDataLength;
	U16 obexFragmentPktOffset;
	ObexPacket XDATA_PTR completeFragmentObexPktPtr;
	U8 isContinueObexPacket;
	U16 obexContinuePktOffset;
    ObexPacket XDATA_PTR completeContinueObexPktPtr;
	U8 lastObexOpcode;	//For continue request
	U8 msgHandleLength;
	U8 msgHandleName[20];
} MAPLinkInfoType;

typedef struct
{
	MAPLinkInfoType linkInfo[MAX_MULTI_POINT_NO];
} MAPCtlType;

#define MAP_SetSubState(L, A)	(gMAP_ctl.linkInfo[L].subState = A)

MAP_EXTERN MAPCtlType XDATA gMAP_ctl;

#endif