//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _OBEX_INTERFACE_H_
#define _OBEX_INTERFACE_H_

#include "..\RFCOMM\RFCOMM_Interface.h"

#ifndef _OBEX_C_
  #define OBEX_EXTERN extern
#else
  #define OBEX_EXTERN  
#endif

#define OBEX_OFFSET		RFCOMM_RX_OFFSET
#define OBEX_VERSION	0x10
#define OBEX_FINAL		0x80
#define OBEX_MASK		0x7F

enum 
{
	OBEX_CONNECT	= 0x00,
	OBEX_DISCONNECT	= 0x01,
	OBEX_PUT		= 0x02,
	OBEX_GET		= 0x03,
	OBEX_SETPATH	= 0x05,
	OBEX_SESSION	= 0x07,
	OBEX_USERDEF_0	= 0x10,
	OBEX_USERDEF_1	= 0x11,
	OBEX_USERDEF_2	= 0x12,
	OBEX_USERDEF_3	= 0x13,
	OBEX_USERDEF_4	= 0x14,
	OBEX_USERDEF_5	= 0x15,
	OBEX_USERDEF_6	= 0x16,
	OBEX_USERDEF_7	= 0x17,
	OBEX_USERDEF_8	= 0x18,
	OBEX_USERDEF_9	= 0x19,
	OBEX_USERDEF_A	= 0x1A,
	OBEX_USERDEF_B	= 0x1B,
	OBEX_USERDEF_C	= 0x1C,
	OBEX_USERDEF_D	= 0x1D,
	OBEX_USERDEF_E	= 0x1E,
	OBEX_USERDEF_F	= 0x1F,
	OBEX_ABORT		= 0x7F,
};

enum 
{
	OBEX_CONTINUE				= 0x10,

	OBEX_SUCCESS				= 0x20,
	OBEX_CREATED,
	OBEX_ACCETPED,
	OBEX_NON_AUTHORITATIVE_INFO,
	OBEX_NO_CONTENT,
	OBEX_RESET_CONTENT,
	OBEX_PARTIAL_CONTENT,

	OBEX_MULTIPLE_CHOICES		= 0x30,
	OBEX_MOVED_PERMANENTLY,
	OBEX_MOVED_TEMPORARILY,
	OBEX_SEE_OTHER,
	OBEX_NOT_MODIFIED,
	OBEX_USE_PROXY,

	OBEX_BAD_REQUEST			= 0x40,
	OBEX_UNAUTHORIZED,
	OBEX_PAYMENT_REQUIRED,
	OBEX_FORBIDDEN,
	OBEX_NOT_FOUND,
	OBEX_METHOD_NOT_ALLOWED,
	OBEX_NOT_ACCEPTABLE,
	OBEX_PROXY_AUTHENTICATION_REQUIRED,
	OBEX_REQUEST_TIME_OUT,
	OBEX_CONFILICT,
	OBEX_GONE,
	OBEX_LENGTH_REQUIRED,
	OBEX_PRECONDITION_FAILED,
	OBEX_REQUESTED_ENTITY_TOO_LARGE,
	OBEX_REQUEST_URL_TOO_LARGE,
	OBEX_UNSUPPORTED_MEDIA_TYPE,

	OBEX_INTERNAL_SERVER_ERROR	= 0x50,
	OBEX_NOT_IMPLEMENTED,
	OBEX_BAD_GATEWAY,
	OBEX_SERVICE_UNAVAILABLE,
	OBEX_GATEWAY_TIMEOUT,
	OBEX_HTTP_VERSION_NOT_SUPPORTED,

	OBEX_DATABASE_FULL			= 0x60,
	OBEX_DATABASE_LOCKED,
};

enum 
{
	//Bits 8 and 7 of HI
	OBEXH_UNICODE_TEXT	= 0x00,
	OBEXH_BYTE_SEQUENCE	= 0x01,
	OBEXH_BYTE_1		= 0x02,
	OBEXH_BYTE_4		= 0x03
};

enum
{
	OBEXH_COUNT = 0x00,
	OBEXH_NAME,
	OBEXH_TYPE,
	OBEXH_LENGTH,
	OBEXH_TIME,
	OBEXH_DESCRIPTION,
	OBEXH_TARGET,
	OBEXH_HTTP,
	OBEXH_BODY,
	OBEXH_END_OF_BODY,
	OBEXH_WHO,
	OBEXH_CONNECTION_ID,
	OBEXH_APP_PARAMETERS,
	OBEXH_AUTH_CHALLENGE,
	OBEXH_AUTH_RESPONSE,
	OBEXH_CREATOR_ID,
	OBEXH_WAN_UUID,
	OBEXH_OBJECT_CLASS,
	OBEXH_SESSION_PARAMETERS,
	OBEXH_SESSION_SEQUENCE_NUMBER,
};

typedef struct
{
 	U16 length;
	U8 content;
} ObexByteSequence;

typedef union 
{
	//0x40 : byte seq, 2 bytes length indicator
	ObexByteSequence byteSequence;

	//0xC0 : 4 bytes
	U32 count;
	U32 length;
	U32 conectionId;
} ObexHeaderValue;

typedef struct
{
	U8 headerId:6;
	U8 encoding:2;
	ObexHeaderValue value;
} ObexHeader;

typedef struct
{
	U8 uuidValue[16];
} ObexTargetUUID;

typedef struct
{
	U8 obexVersion;
	U8 flags;
	U16 maxPacketLength;
	U8 optionalHeaders[3+sizeof(ObexTargetUUID)];//3bytes for opcode and length
} ObexConnectReq;


typedef struct
{
	U8 Id;			//0xCB
	U32 Value;
}ObexConnectionId;

typedef struct
{
	U8 Id;			//0x01
	U16 Length;		//0x0F, 3 + given obex Name
	U8 Name[12];	//Five UTF-16 texts. "*.vcf ": UTF-16
}ObexName;

typedef struct
{
	U8 Id;			//0x42
	U16 Length;		//0x12
	U8 Type[15];	//"x-bt/phonebook ": Byte Sequence in ASCII
}ObexObjectType;

typedef struct
{
	U8 Id;			//0x4C
	U8 FilterTagID;		//0x06
	U8 FilterLength;	//0x08
	U8 FilterValues[8];	//64bits Mask
	
	U8 FormatTagID;		//0x07
	U8 FormatLength;	//0x01
	U8 FormatValues;	//vcad: 0x00->2.1, 0x01->3.0
	
	U8 MaxListCountTagID;//0x04
	U8 MaxListCountLength;//0x02
	U16 MaxListCountValues;//0x0000~oxFFFF

	U8 ListStartOffsetTagID;//0x05
	U8 ListStartOffsetCountLength;//0x02
	U16 ListStartOffsetCountValues;//0x0000~oxFFFF
}ObexAppParameters;

typedef struct
{
	U8 getReqBody[1];
} ObexGetReq;

typedef struct
{
	U8 obexVersion;
	U8 flags;
	U16 maxPacketLength;
	U8 optionalHeaders[1];
} ObexConnectRsp;

typedef struct
{
	U8 optionalHeaders[1];
} ObexDisconnectReq;

typedef struct
{
	U8 optionalHeaders[1];
} ObexDisconnectRsp;

typedef union 
{
   ObexGetReq getReq;
   ObexConnectReq connReq;
   ObexConnectRsp connRsp;
   ObexDisconnectReq discReq;
   ObexDisconnectRsp discRsp;
} ObexDataType;

typedef struct
{
	U8 opCode;
	U16 packetLength;
	ObexDataType dataBody;
} ObexControlPacket;

typedef struct
{
	U8 continuousData[1];
} ObexContinuousDataPacket;

typedef union 
{
   ObexControlPacket ctrlPacket;
   ObexContinuousDataPacket dataPacket; 
} ObexPacketPayload;

typedef struct
{
	U8 offset;
	U8 reserved[OBEX_OFFSET - 1];
	U8 profileId;
	U16 dataLength;
	ObexPacketPayload packetPayload;
} ObexPacket;

PUBLIC void OBEX_SendConnectReq(U8 linkIndex, U8 profileId, U16 maxPacketLength, ObexTargetUUID CODE_PTR targetUUID);
PUBLIC void OBEX_SendConnectRsp(U8 linkIndex, U8 profileId, U16 maxPacketLength);
PUBLIC void OBEX_SendSimplePacket(U8 linkIndex, U8 profileId, U8 opCode);
PUBLIC void OBEX_SendPacket(U8 linkIndex, U8 profileId, U8 opcode, U8 XDATA_PTR paraPtr, U16 paraLength);
PUBLIC U8 XDATA_PTR OBEX_MD5(U8 XDATA_PTR msgPtr);

#endif

