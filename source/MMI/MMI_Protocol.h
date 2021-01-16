#ifndef _MMI_PROTOCOL_H_
	#define _MMI_PROTOCOL_H_

	#include "MMI_Base.h"
	#include "MMI_SCN.h"

	extern OSMQ XDATA OSMQ_Protocol_Event_To_MMI[MAX_MULTI_POINT_NO];
	#define OSMQ_Protocol_Event_To_MMI_ptr	&OSMQ_Protocol_Event_To_MMI
	
	//=============Macro==========================================
	#define MMI_CMD_PARA_NOT_ALLOW_ROLE_SWITCH	0x00
	#define MMI_CMD_PARA_ALLOW_ROLE_SWITCH		0x01

	#define NVRAM_KEY_OK			1
	#define NVRAM_KEY_NG			0

	#define MMI_ADD_LINK(linkInd) gMMI_ctl.activeLinkRecord |= U8_BIT_MASK(linkInd)
	#define MMI_DEL_LINK(linkInd) gMMI_ctl.activeLinkRecord &= ~U8_BIT_MASK(linkInd)

	#define CURRENT_ACTIVE_LINK_CNT     (number_of_ones(gMMI_ctl.activeLinkRecord))
	//===========================================================
	PUBLIC void MMI_PROTOCOL_Init(void);
	PUBLIC void MMI_PROTOCOL_EventHandler(void);

	PUBLIC U8 MMI_GetLinkIndexByConnHandle(U16 connHandle);
	PUBLIC U8 MMI_GetLinkIndexByBdA(U8 XDATA_PTR bdAddr);
	
	#define MMI_GET_EMPTY_LINK_SAME_BD_ADDR			0xEE
	PUBLIC U8 MMI_GetEmptyLinkIndex(U8 XDATA_PTR bdAddrPtr, BOOL isActive);
	
	PUBLIC BOOL MMI_CheckProfileOnAllLinks(void);
	PUBLIC void MMI_PROTOCOL_CreateProfileConnection(U8 linkIndex);
	PUBLIC void MMI_CancelCreateAcl(void);
	PUBLIC void MMI_PROTOCOL_RelAllLinks(void);
	PUBLIC void MMI_PageHandler(BOOL isStart);
	PUBLIC void MMI_PageFailHandler(U8 linkIndex);
	PUBLIC void MMI_PageOKHandler(U8 linkIndex);
	PUBLIC BOOL MMI_IsUnderPage(void);
	PUBLIC BOOL MMI_IsLinkActiveConnected(U8 linkIndex);
	PUBLIC BOOL MMI_CreateActiveProfileConnection(U8 linkIndex);
	PUBLIC void MMI_PrepareActiveProfileConnection(U8 linkIndex);
	PUBLIC void MMI_ConnectableStateChangeWhenConnected(void);	

	PUBLIC MMI_PROFILE_INFO XDATA_PTR MMI_ConnectProfile(U8 linkIndex, U8 profileId);
	PUBLIC U8 MMI_DisconnectProfile(U8 linkIndex, MMI_PROFILE_INFO XDATA_PTR profileInfo);
	
	#ifdef __SEQUENTIAL_ACTIVE_PROFILE__
	PUBLIC void MMI_ClearSequentialActiveProfileList(U8 linkIndex);
	PUBLIC void MMI_SequentialActiveProfileConnection(U8 linkIndex);
	PUBLIC void MMI_AppendActiveProfileConnection(U8 linkIndex, U8 profileId);
	#define MMI_CONNECT_ACTIVE_PROFILE		MMI_AppendActiveProfileConnection
	#else
	#define MMI_CONNECT_ACTIVE_PROFILE		MMI_ConnectProfile
	#endif


	//---------------------------------------------------
	//------------------HCI Cmd Handler-------------------
	//---------------------------------------------------

	///////////////  HFP /////////////////////
	#ifdef A2DP_Profile	
	PUBLIC BOOL MMI_CheckAllLinksInA2DPState(void);
	#endif
	PUBLIC MMIMsgType XDATA_PTR MMI_PROTOCOL_CommandFactory(U8 opcode);
#endif

