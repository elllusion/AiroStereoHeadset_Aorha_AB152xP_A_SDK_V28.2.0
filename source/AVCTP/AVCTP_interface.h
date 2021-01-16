//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _AVCTP_INTERFACE_H_
#define _AVCTP_INTERFACE_H_

#include "..\L2CAP\L2CAP_Interface.h"

#ifdef _AVCTP_C_
  #define AVCTP_EXTERN  
#else
  #define AVCTP_EXTERN extern
#endif

enum
{
  AVCTP_COMMAND,
  AVCTP_RESPONSE
};


#define AVRCP_PAYLOAD_OFFSET	(L2CAP_RX_PDU_OFFSET + L2CAP_HEADER_LEN + 3/*1(transaction label)+2(profile id)*/)


typedef struct
{
	MMI_PROFILE_DESC mmiProfileDesc;
	U16 PID; /* AVCTP Profile ID */
} AVCTP_PROFILE_DESC;


//////////////////////////////////////////////////////////////////////////

PUBLIC void AVCTP_RegisterProfile(MMI_PROFILE_NODE XDATA_PTR profileNode);

// Application Service Calls
PUBLIC L2CAP_CHANNEL_HDL AVCTP_ConnectChannel(U8 linkIndex);
PUBLIC U8 AVCTP_DisconnectChannel(U8 linkIndex);

PUBLIC U8 XDATA_PTR AVCTP_GetMessageBuffer(U8 dataLength, U16 PID);
PUBLIC void AVCTP_SendMessageCmd(U8 linkIndex, U8 XDATA_PTR msgPtr, U8 transaction);
PUBLIC void AVCTP_SendMessageRsp(U8 linkIndex, U8 XDATA_PTR msgPtr, U8 transaction);

// Global variables


#endif