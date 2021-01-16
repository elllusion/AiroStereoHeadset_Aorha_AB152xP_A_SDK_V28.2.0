//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _AVCTP_H_
#define _AVCTP_H_

#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_PSM.h"
#include "..\HC\HC_interface.h"
#include "..\L2CAP\L2CAP_interface.h"


#define AVCTP_MTU_SIZE		335


enum
{
	AVCTP_PKT_TYPE_SINGLE = 0,
	AVCTP_PKT_TYPE_START,	
 	AVCTP_PKT_TYPE_CONTINUE,
  	AVCTP_PKT_TYPE_END
};

enum
{
	AVCTP_STATE_IDLE,
	AVCTP_STATE_DISCONNECTING,
	AVCTP_STATE_CONNECTING,
	AVCTP_STATE_CONNECTED,
};

enum
{
	AVCTP_CONNECT_REQ = 0x01,
	AVCTP_CONNECT_RES,
	AVCTP_DISCONNECT_REQ,
	AVCTP_SEND_MESSAGE,
};

typedef union
{
	struct {
  	  U16 PID;
  	  U8 payloadStart[1];
	} single;
  	struct {
  	  U8 numberOfPkt;
  	  U16 PID;
  	  U8 payloadStart[1];
  	} start;
  	struct {
  	  U8 payloadStart[1];
  	} conti;
  	struct {
  	  U8 payloadStart[1];
  	} end;
} AvctpPkt;

typedef	struct
{
  U8 dataOffset;
  U8 reserveByte[L2CAP_RX_PDU_OFFSET-1];
  
  U16 l2capLength;
  U16 l2capCID;
  
  union {
  	U8 b;
  	struct {
	  U8 isPIDInvalid:1;
	  U8 msgType:1; 
	  U8 pktType:2;
	  U8 transactionLabel:4;	  
  	} f;
  } pktHead;
} AvctpMsg;

typedef	struct
{
	U8 state;
	L2CAP_CHANNEL_HDL l2capChannelHdl;
	OSMQ OSMQ_AVCTP_Data_From_Remote;
	U16 reassembleLength;
	U8 XDATA_PTR reassemblePayloadPtr;
} AVCTP_LINK_INFO;

typedef	struct
{
	AVCTP_PROFILE_DESC CODE_PTR entryProfileDesc;
	AVCTP_LINK_INFO linkInfo[MAX_MULTI_POINT_NO]; 	 
} AVCTP_CTL_TYPE;


#endif
