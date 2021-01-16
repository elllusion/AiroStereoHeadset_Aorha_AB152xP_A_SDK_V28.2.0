//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _PBAP_H_
#define _PBAP_H_

#include "PBAP_interface.h"
#include "..\OBEX\OBEX_interface.h"
#include "..\MMI\MMI_Base.h"

enum
{
	PBAP_IDLE_STATE = PBAP_INVALID,
	PBAP_CREATING_STATE = PBAP_CONNECT_CMD,
	PBAP_DISCONNECTING_STATE = PBAP_DISCONNECT_CMD,
	PBAP_GET_PHONEBOOK_STATE = PBAP_GET_PHONEBOOK_CMD,
	PBAP_SET_FOLDER_STATE = PBAP_SET_FOLDER_CMD,
	PBAP_PUT_VCARD_LISTING_STATE = PBAP_PUT_VCARD_LISTING_CMD,
	PBAP_NUM_OF_STATE,
};

enum
{
	PBAP_SUBSTATE_INIT,
	PBAP_SUBSTATE_01,
	PBAP_SUBSTATE_02,
	PBAP_SUBSTATE_03,
	PBAP_SUBSTATE_04,
	PBAP_SUBSTATE_05,
	PBAP_SUBSTATE_END = 0xFF,
};

typedef struct
{
	U16 totalDataLength;
	U16 leftDataLength;
	U8 state;
	U8 subState;
} PbapLinkInfoType;

typedef struct
{
	PbapLinkInfoType linkInfo[MAX_MULTI_POINT_NO];
} PbapCtlType;

#define PBAP_SetSubState(L, A)	(gPBAP_ctl.linkInfo[L].subState = A)

PBAP_EXTERN PbapCtlType XDATA gPBAP_ctl; 

#endif