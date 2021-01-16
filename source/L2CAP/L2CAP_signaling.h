
#ifndef __L2CAP_SIGNALING_H__
#define __L2CAP_SIGNALING_H__

// CMD Status---------------------------------------------------
enum
{
	L2CAP_STATUS_NO_FURTHER_INFORMATION_AVAILABLE,
	L2CAP_STATUS_AUTHENTICATION_PENDING,
	L2CAP_STATUS_AUTHORIZATION_PENDING
};

//CMD Result-------------------The value sould be 0x00XX  ---------------
enum
{
	L2CAP_RESULT_CONNECTION_SUCCESSFUL = 0x00,
	L2CAP_RESULT_CONNECTION_PENDING,
	L2CAP_RESULT_CONNECTION_REFUSED_PSM_NOT_SUPPORTED,
	L2CAP_RESULT_CONNECTION_REFUSED_SECURITY_BLOCK,
	L2CAP_RESULT_CONNECTION_REFUSED_NORESOURCE_AVALIABLE
};

enum
{
	CONNECTIONLESS_MTU = 0x0001,
	EXT_FEATURES_SUPPORTED,
	FIXED_CHANNELS_SUPPORTED
};

enum
{
	L2CAP_SIGNALLING_RESERVED = 0x00,
	L2CAP_SIGNALLING_COMMAND_REJECT,
	L2CAP_SIGNALLING_CONNECTION_REQUEST,
	L2CAP_SIGNALLING_CONNECTION_RESPONSE,
	L2CAP_SIGNALLING_CONFIG_REQUEST,
	L2CAP_SIGNALLING_CONFIG_RESPONSE,
	L2CAP_SIGNALLING_DISCONNECTION_REQUEST,
	L2CAP_SIGNALLING_DISCONNECTION_RESPONSE,
    L2CAP_SIGNALLING_ECHO_REQUEST,
	L2CAP_SIGNALLING_ECHO_RESPONSE,
	L2CAP_SIGNALLING_INFORMATION_REQUEST,
	L2CAP_SIGNALLING_INFORMATION_RESPONSE,
	L2CAP_SIGNAL_30_NO = L2CAP_SIGNALLING_INFORMATION_RESPONSE,
#ifdef LE_SUPPORTED_HOST_FLASH
	L2CAP_SIGNALLING_CREATE_CHANNEL_REQUEST,
	L2CAP_SIGNALLING_CREATE_CHANNEL_RESPONSE,
	L2CAP_SIGNALLING_MOVE_CHANNEL_REQUEST,
	L2CAP_SIGNALLING_MOVE_CHANNEL_RESPONSE,
	L2CAP_SIGNALLING_MOVE_CHANNEL_CONFIRMATION,
	L2CAP_SIGNALLING_MOVE_CHANNEL_CONFIRMATION_RESPONSE,
	L2CAP_SIGNALLING_CONNECTION_PARAMETER_UPDATE_REQUEST,
	L2CAP_SIGNALLING_CONNECTION_PARAMETER_UPDATE_RESPONSE,
#endif
	L2CAP_SIGNALLING_END,
};

typedef struct
{	
	U32	delay_variation;
	U32 latency;
	U32 peak_bandwidth;	
	U32 token_bucket_size;
	U32	token_rate;
	U8	service_type;
	U8 	reserved[3];
} QOS_TYPE; 

typedef struct
{
	U16	MPS;
	U16	monitor_time_out; 	
	U16	retransmission_time_out;	
	U8 	max_transmit;
	U8 	txwindow_size;
	U8 	mode;
	U8	reserved[2];
} RFC_TYPE;

typedef struct
{
	U16 DCID;
	U16 SCID;
}L2capCIDType;

typedef struct
{
	U8  reasonLowByte;
	U8  reasonHighByte;
}L2capReasonType;

typedef struct
{
	U8 resultLowByte;
	U8 resultHighByte;
}L2capResultType;

typedef struct
{
	U8 statusLowByte;
	U8 statusHighByte;
}L2capStatusType;

typedef struct
{
	U8 type;
	U8 length;
	U16 mtu;
}L2capOptionMTU;

typedef union
{
	U16 maxMtuSize;
	L2capCIDType CID;
}L2capPktCmdRejDataType;

typedef struct
{
	L2capReasonType reason;
	L2capPktCmdRejDataType rejectData;
}L2capPktCmdRejType;

typedef struct
{
	U16 PSM;
	U16 SCID;
}L2capPktConnectionReqType;

typedef struct
{
	L2capCIDType CID;
	L2capResultType result;
	L2capStatusType status;
}L2capPktConnectionRespType;


typedef struct
{
	U16 DCID;
	U16 flag;
	U8 options[43];
}L2capPktConfigReqType;

typedef struct
{
	U16 SCID;
	U16 flag;
	L2capResultType result;
	L2capOptionMTU optionMTU;
}L2capPktConfigRespType;

typedef struct
{
	L2capCIDType CID;
}L2capPktDisconnectReqType;

typedef struct
{
	L2capCIDType CID;
}L2capPktDisconnectRespType;

typedef struct
{
	U8 echoData[1];
}L2capPktEchoReqType;

typedef struct
{
	U8 echoData[1];
}L2capPktEchoRespType;

typedef struct
{
	U16 infoType;
}L2capPktInformationReqType;

typedef struct
{
	U16 infoType;
	U8 resultLowByte;
	U8 resultHighByte;
	U8 dataBody[1];
}L2capPktInformationRespType;

typedef union
{
	L2capPktCmdRejType cmdReject;
	L2capPktConnectionReqType connectionReq;
	L2capPktConnectionRespType connectionResp;
	L2capPktConfigReqType configReq;
	L2capPktConfigRespType configResp;
	L2capPktDisconnectReqType disconnectReq;
	L2capPktDisconnectRespType disconnectResp;
	L2capPktEchoReqType echoReq;
	L2capPktEchoRespType echoResp;
	L2capPktInformationReqType informationReq;
	L2capPktInformationRespType informationResp;
}L2capMsgType;

typedef struct
{
	U16 l2capLength;
	U16 channelID;
	U8 opCode;
	U8 identifier;
	U16 length;
	L2capMsgType msgBody;
}L2capPktType;

typedef struct
{
	U8 Offset;
	U8 reserved[4];
	U8 flag;
	U16 totalLength;
	L2capPktType l2capPkt;
}SIGNALLING_PACKET_TYPE;	/* TX signal data format */

PUBLIC void L2CAP_SignalingHandler(U8 linkIndex, U8 XDATA_PTR packetPtr);
PUBLIC void L2CAP_CheckTimer(U8 linkIndex);

PUBLIC void L2CAP_SendSignallingPacket(U8 linkIndex, U8 XDATA_PTR s_ptr);
PUBLIC void L2CAP_SendCmdRejectCmd(U8 linkIndex, L2capPktType XDATA_PTR pduPtr, U8 reason);
PUBLIC void L2CAP_SendConnectionRequest(L2CAP_CHANNEL_HDL l2capChannelHdl, U16 psm);
PUBLIC void L2CAP_SendConfigRequest(L2CAP_CHANNEL_HDL l2capChannelHdl);
PUBLIC void L2CAP_SendDisconnectionRequest(L2CAP_CHANNEL_HDL l2capChannelHdl);
PUBLIC void L2CAP_SendEchoRequest(U8 linkIndex);
PUBLIC void L2CAP_SendInformationRequest(U8 linkIndex, U16 infoType);

////////////////////////////////////////////////////////////////////////////////
// Timer Related;
/////////////////////////////////////////////////////////////////////////////////
PUBLIC void L2CAP_SetRtxTimer(L2CAP_CHANNEL_HDL l2capChannelHdl, U32 time);

#endif //__L2CAP_SIGNALING_H__
