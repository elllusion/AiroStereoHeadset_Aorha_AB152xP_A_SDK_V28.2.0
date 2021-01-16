//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _SAP_H_
#define _SAP_H_

#include "SAP_InterFace.h"
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_interface.h"
#include "..\HFP\HFP_Interface.h"

enum
{
	SAP_IDLE_STATE,
	SAP_COMMAND_STATE,
	SAP_EVENT_STATE,
};

enum
{
	SAP_NOT_READY,
	SAP_NOT_CONNECTED,
	SAP_CONNECTION_UNDER_NEGOTIATION,
	SAP_IDLE,
	SAP_PROCESSING_APDU_REQUEST,
	SAP_PROCESSING_ATR_REQUEST,
	SAP_PROCESSING_SET_TRANSPORT_PROTOCOL_REQUEST,
	SAP_PROCESSING_RESET_REQUEST,
	SAP_PROCESSING_POWER_OFF_REQUEST,
	SAP_PROCESSING_POWER_ON_REQUEST,
	SAP_PROCESSING_CARD_READER_STATUS_REQUEST,
};

enum
{
	SAP_SUBSTATE0,
	SAP_SUBSTATE1,
	SAP_SUBSTATE2,
	SAP_SUBSTATE3,
	SAP_SUBSTATE4,
	SAP_SUBSTATE5,
	SAP_SUBSTATE6
	
};

enum
{
	SAP_MSG_CONNECT_REQ,						//Client -> Server 0x00
	SAP_MSG_CONNECT_RESP,						//Server -> Client 0x01
	SAP_MSG_DISCONNECT_REQ,						//Client -> Server 0x02
	SAP_MSG_DISCONNECT_RESP,					//Server -> Client 0x03	
	SAP_MSG_DISCONNECT_IND,						//Server -> Client 0x04
	SAP_MSG_TRANSFER_APDU_REQ,					//Client -> Server 0x05
	SAP_MSG_TRANSFER_APDU_RESP, 				//Server -> Client 0x06
	SAP_MSG_TRANSFER_ATR_REQ,					//Client -> Server 0x07
	SAP_MSG_TRANSFER_ATR_RESP,					//Server -> Client 0x08
	SAP_MSG_POWER_SIM_OFF_REQ,					//Client -> Server 0x09
	SAP_MSG_POWER_SIM_OFF_RESP,					//Server -> Client 0x0a
	SAP_MSG_POWER_SIM_ON_REQ,					//Client -> Server 0x0b
	SAP_MSG_POWER_SIM_ON_RESP,					//Server -> Client 0x0c
	SAP_MSG_RESET_SIM_REQ,						//Client -> Server 0x0d
	SAP_MSG_RESET_SIM_RESP,						//Server -> Client 0x0e
	SAP_MSG_TRANSFER_CARD_READER_STATUS_REQ,	//Client -> Server 0x0f
	SAP_MSG_TRANSFER_CARD_READER_STATUS_RESP,	//Server -> Client 0x10
	SAP_MSG_STATUS_IND,							//Server -> Client 0x11
	SAP_MSG_ERROR_RESP,							//Server -> Client 0x12
	SAP_MSG_SET_TRANSPORT_PROTOCOL_REQ,			//Client -> Server 0x13
	SAP_MSG_SET_TRANSPORT_PROTOCOL_RESP,		//Server -> Client 0x14
};

enum
{
	SAP_PARA_MAX_MSG_SIZE,
	SAP_PARA_CONNECTION_STATUS,
	SAP_PARA_RESULT_CODE,
	SAP_PARA_DISCONNECTION_TYPE,
	SAP_PARA_COMMAND_APDU,
	SAP_PARA_RESPONSE_APDU,
	SAP_PARA_ATR,
	SAP_PARA_CARD_READER_STATUS,
	SAP_PARA_STATUS_CHANGE,
	SAP_PARA_TRANSPORT_PROTOCOL,
	SAP_PARA_COMMAND_APDU_7816 = 0x10,
};

typedef U16 SAPParaMaxMsgSize; 

enum
{
	OK_SERVER_CAN_FULFILL_REQUIREMENTS,
	ERROR_SERVER_UNABLE_TO_ESTABLISH_CONNECTION,
	ERROR_SERVER_DOES_NOT_SUPPORT_MAXIMUM_MESSAGE_SIZE,
	ERROR_MAXIMUM_MESSAGE_SIZE_BY_CLIENT_IS_TOO_SMALL,
	OK_ONGOING_CALL,
};

enum
{	
	GRACEFUL,
	IMMEDIATE,
};

enum
{
	OK_REQUEST_PROCESSED_CORRECTLY,
	ERROR_NO_REASON_DEFINED,
	ERROR_CARD_NOT_ACCESSIBLE,
	ERROR_CARD_ALREADY_POWER_OFF,
	ERROR_CARD_REMOVED,
	ERROR_CARD_ALREADY_POWER_ON,
	ERROR_DATA_NOT_AVAILABLE,
	ERROR_NOT_SUPPORTED,
};


typedef U8 SAPParaCardReaderStatus;

/*	GSM 11.14 specification   12.33	Card reader status
This subclause applies only if class "a" is supported.

	Byte(s)		Description				Length
	------		-------------------		-----
	1			Card reader status tag		1
	2			Length					1
	3			Card reader status			1

-	Card reader status:
Contents :
This contains the identity of the card reader, and flags to indicate the status of the reader with respect to:
-	whether the card reader is removable or permanently connected;
-	whether the card reader is present (this can only be false if the card reader is removable);
-	whether the card reader present accepts ID-1 size cards (this can only be true if the card reader is present);
-	whether there is a card present in the card reader (this can only be true if the card reader is present);
-	whether power is being applied to the card (this can only be true if a card is present).

Coding :
The value of this byte indicates the identity and status of a card reader.
bits 1-3	   = identity of card reader x.
bit 4		0 = Card reader is not removable
		1 = Card reader is removable
bit 5		0 = Card reader is not present
		1 = Card reader is present
bit 6		0 = Card reader present is not ID-1 size 
		1 = Card reader present is ID-1 size
bit 7		0 = No card present 
		1 = Card is present in reader 
bit 8		0 = No card powered
		1 = Card in reader is powered
*/

enum
{
	UNKNOWN_ERROR,
	CARD_RESET,
	CARD_NOT_ACCESSIBLE,
	CARD_REMOVED,
	CARD_INSERTED,
	CARD_RECOVERED,
};

enum
{
	T0,
	T1,
};

#define DEFAULT_TRANSPORT_PROTOCOL T0


typedef U8 SAPParaATR;


typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	SAPParaMaxMsgSize maxMsgSize;
	U8 paddingBytes[2];
} SAPConnectReqType;

typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 connStatus;
}SAPConnectResp1Type;

typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 connStatus;
	U8 paddingBytes[3];
	U8 paraID2;
	U8 reserved2;
	U16 parameterLength2;
	SAPParaMaxMsgSize maxMsgSize;
	U8 paddingBytes2[2];
}SAPConnectResp2Type;


typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 disconnType;
	U8 paddingBytes[3];
}SAPDisconnectIndType;

typedef union
{
	U8 CLA;
	U8 INS;
	U8 P1;
	U8 P2;
	U8 Lc;
	U8 Le;
	
}SAPCommandAPDUT0Type;

typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	SAPCommandAPDUT0Type APDUdata;
	U8 reserved2[2];
}SAPTransferAPDUT0ReqType;


typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 resultCode;
	U8 paddingBytes[3];
	U8 paraID2;
	U8 reserved2;
	U8 resultCode2;
	U8 paddingBytes2[3];
}SAPTransferAPDURespType;


typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 resultCode;
	U8 paddingBytes[3];
	U8 paraID2;
	U8 reserved2;
	U16 parameterLength2;
	SAPParaATR ATR[33]; // max size of ATR is 33 bytes
}SAPTransferATRRespType;
typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];				//Resp:disconnect, error
}SAPNoParameterReqRespType; //Req: disconnect, ATR, simOff, simOn, resetsim, cardRead

typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 resultCode;
	U8 paddingBytes[3];
}SAPOnlyResultCodeRespType;


typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 statusChange;
	U8 paddingBytes[3];
}SAPStatusIndType;

typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 resultCode;
	U8 paddingBytes[3];
	U8 paraID2;
	U8 reserved2;
	U16 parameterLength2;
	SAPParaCardReaderStatus cardReaderStatus;
	U8 paddingBytes2[3];
}SAPTransferCardReaderStatusRespType;

typedef struct
{
	U8 msgID;
	U8 paraNum;
	U8 reserved[2];
	U8 paraID;
	U8 reserved1;
	U16 parameterLength;
	U8 transProtocol;
	U8 paddingBytes[3];
}SAPSetTransportProtocolReqType;

typedef struct
{
	U8 resultCode;
}SAPSetTransportProtocolRespType;

typedef struct
{
	U8 resultCode;
}SAPResultCodeType;

typedef struct 
{
	U8 resultCode;
	SAPParaATR ATR[33]; // max size of ATR is 33 bytes
}SAPATRRespType;

typedef struct
{
	U8 powerSIMInfo;
	SAPParaCardReaderStatus cardStatus;
}SAPSIMInfoType;

typedef struct
{
	U8 resultCode;
	U8 responseAPDU;
}SAPAPDURespType;

typedef struct
{
	U8 connected;
	U8 mainState;
	U8 sapState;
	U8 subState;
	U8 msgId;
	SAPParaMaxMsgSize maxMsgSize;
	U8 disconnectType;
	U8 connectType;
	U8 cardStatus;
	SAPSIMInfoType SIMInfo;
	SAPATRRespType ATRResp;
	SAPAPDURespType APDUResp;
	U8 T0T1;
	OST XDATA_PTR ost_command_ptr;
}SAPLinkInfoType;

typedef struct
{
	SAPLinkInfoType linkInfo[MAX_MULTI_POINT_NO];
}SAPCtlType;

SAP_EXTERN SAPCtlType XDATA gSAP_ctl;


#endif
