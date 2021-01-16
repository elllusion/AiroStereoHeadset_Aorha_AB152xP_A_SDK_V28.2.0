//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _L2CAP_INTERFACE_H_
#define _L2CAP_INTERFACE_H_ 

#include "os.h"
#include "hci.h"
#include "bt_config_profile.h"
#include "..\MMI\MMI_interface.h"

#ifdef _L2CAP_C_
#define L2CAP_EXTERN
#else
#define L2CAP_EXTERN extern
#endif

typedef struct
{
    U16 len;
    U16 cid;
}L2CAP_DATA_HDR_STRU;

typedef struct
{
    U8  opCode;
    U8  identifier;
    U16 len;
}L2CAP_SIGNALING_HEADER_STRU;

typedef union
{
    U8 payload[1];
    L2CAP_SIGNALING_HEADER_STRU signaling;
}L2CAP_DATA_PAYLOAD_UNION;

typedef struct
{
    HCI_DATA_HDR_STRU hciDataHdr;
    L2CAP_DATA_HDR_STRU l2capDataHdr;
    L2CAP_DATA_PAYLOAD_UNION l2capPayload;
}L2CAP_RX_HCI_DATA_STRU;

#define HCIPKT_HEADER_LEN		sizeof(HCI_DATA_HDR_STRU)

#define L2CAP_HEADER_LEN		sizeof(L2CAP_DATA_HDR_STRU)
#define L2CAP_SIG_HEADER_LEN	sizeof(L2CAP_SIGNALING_HEADER_STRU)

#define L2CAP_RX_PDU_OFFSET		6
#define L2CAP_TX_PDU_OFFSET		8


#define L2CAP_SIGNALING_CID						0x0001
#define L2CAP_CONNECTIONLESS_CID				0x0002
//for 1500 update, use HCI_L2CAP_CID_FOR_REMOTE_EEPROM_UPDATE which is  0x0022 
//for 1520 update, use L2CAP_REMOTE_UPDATE_FLASH_CID instead 
#define L2CAP_REMOTE_UPDATE_FLASH_CID			0x0023
#define L2CAP_FCD_CID							0x0024
#define L2CAP_AIROHA_CID						0x0025

//Result Value to Profile-----------------------------------------------------------
#define L2CAP_STATUS_SUCCESS                            0x00
#define L2CAP_STATUS_PENDING                            0x01
#define L2CAP_STATUS_CONNECTION_REFUSED_NO_PSM          0x02
#define L2CAP_STATUS_CONNECTION_REFUSED_SECURITY_BLOCK  0x03
#define L2CAP_STATUS_CONNECTION_REFUSED_NO_RESOURCE     0x04
#define L2CAP_STATUS_TIMEOUT                            0x05
#define L2CAP_STATUS_CONFIG_REFUSED                     0x06
#define L2CAP_STATUS_REJECT_BY_REMOTE                   0x07
#define L2CAP_STATUS_LOCAL_AUTH_FAIL					0x08
//End Result Value to Profile-------------------------------------------------------


#define L2CAP_RFC_MODE_BASIC_L2CAP_MODE					0
#define L2CAP_RFC_MODE_RETRANSMISSION_MODE				1
#define L2CAP_RFC_MODE_FLOW_CONTROL_MODE				2


typedef struct
{
	U16 			remain_length;
	U8 	XDATA_PTR 	current_ptr;
	U8 	XDATA_PTR	memory_ptr;	 
} L2CAP_RECOMBINE_CTL;

typedef struct
{
	OSMQ XDATA_PTR	aclDataInQueue;
	OSMQ XDATA_PTR	aclDataOutQueue;
	U8 dataOutPktCnt;
} L2CAP_DATA_CTL;

#define INVALID_L2CAP_CHANNEL		((L2CAP_CHANNEL_HDL)NULL)

//--------------------------------------------------------------
//------------------------Event To Protocol---------------------
//--------------------------------------------------------------
typedef struct
{
	U8 needAuth;
	U16 mtuSize;
	void (*ChannelDataHandler)(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 XDATA_PTR dataPtr);
}L2CAPConnReqParams;

typedef struct
{
	U8 status;
	L2CAPConnReqParams params;
}L2CAPRemoteConnReqType;

typedef struct
{
	U16 psm;
	L2CAPConnReqParams params;
}L2CAPLocalConnReqType;

typedef struct
{
	U16 psm;
	void (*TaskEntry)(L2CAP_CHANNEL_HDL l2capChannelHdl);
	void (*RemoteConnReqEvt)(L2CAP_CHANNEL_HDL l2capChannelHdl, L2CAPRemoteConnReqType XDATA_PTR reqPtr);
	void (*ConnCplEvt)(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 status);
	void (*DisconnectCplEvt)(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 reason);
} L2CAP_PROTOCOL_DESC;

typedef struct stL2capProtocolNode L2CAP_PROTOCOL_NODE;

struct stL2capProtocolNode
{
	L2CAP_PROTOCOL_DESC CODE_PTR protocolDesc;
	L2CAP_PROTOCOL_NODE XDATA_PTR nextNode;
};

struct stL2capChannel
{
	L2CAP_CHANNEL XDATA_PTR nextChannel;
	OST	XDATA_PTR	rtx_timer_ptr;
	U8 XDATA_PTR	last_response_ptr;
	U8				out_identifier;
	U8				channel_state;
	U8				linkIndex;
	U16				SCID;
	U16				DCID;
	U16				in_MTU;
	U16				out_MTU;
	L2CAP_PROTOCOL_DESC CODE_PTR protocolDesc;
	void (*ChannelDataHandler)(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 XDATA_PTR dataPtr);
};

#define L2CAP_GetChannelLinkIndex(hdl)	((hdl)->linkIndex)
#define L2CAP_GetChannelOutMTU(hdl)		((hdl)->out_MTU)

// Global Functions
PUBLIC void L2CAP_Init (void);
PUBLIC void L2CAP (void);
PUBLIC void L2CAP_AclDiscHandle(U8 linkIndex);
PUBLIC BOOL L2CAP_IsResourceReleased(U8 linkIndex);
PUBLIC void L2CAP_AclTxCountHandler(U8 linkIndex, U16 numCplPkt);
PUBLIC U8 L2CAP_GetAclTxCount(U8 linkIndex);
PUBLIC void L2CAP_SetupLinkInfo(U8 linkIndex, U16 aclHandle);

PUBLIC void L2CAP_RegisterProtocol(L2CAP_PROTOCOL_NODE XDATA_PTR protocolNode);

PUBLIC void L2CAP_ActivateRemoteUpdateFlashChannel(U8 linkIndex);
PUBLIC void L2CAP_SendLRemoteUpdateFlashChannelData(U8 XDATA_PTR dataPtr, U16 dataLen);

PUBLIC void L2CAP_SendFCDChannelData(U8 linkIndex, U8 XDATA_PTR dataPtr, U16 dataLen);
PUBLIC void L2CAP_SendAirohaChannelData(U8 linkIndex, U8 XDATA_PTR dataPtr, U16 dataLen);

/////////////////////////////////////////////////////////
////////////Tool For upper Layer to send Cmd to L2cap////
/////////////////////////////////////////////////////////
PUBLIC L2CAP_CHANNEL_HDL UpperLayer_SendL2capConnectionRequest(U8 linkIndex, L2CAPLocalConnReqType XDATA_PTR reqPtr);
PUBLIC U8 UpperLayer_SendL2capDisconnectionRequest(L2CAP_CHANNEL_HDL l2capChannelHdl);
PUBLIC void UpperLayer_SendAuthenticationResult(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 status); 
PUBLIC void UpperLayer_SendL2capChannelData(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 XDATA_PTR dataPtr);
PUBLIC OSMQ XDATA_PTR UpperLayer_GetL2capAclInQueue(U8 linkIndex);

#endif
