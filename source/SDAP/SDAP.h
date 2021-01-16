//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _SDAP_H_
#define _SDAP_H_

#include "..\L2CAP\L2CAP_interface.h"
#include "SDAP_Interface.h"

#define SDP_MTU_SIZE				200

#define SDP_PDU_HEADER_SIZE			5
#define SDP_RESERVED_BYTE_COUNT		(SDP_PDU_HEADER_SIZE + 17/*size of continuation state*/ + 10/*extra*/)

#define MAX_SEARCH_PATTERN_SIZE		34
#define MAX_SERVICE_MATCH_COUNT		16

#define MAX_SDP_RECORD_COUNT		5
#define MAX_SDP_ATTR_BYTE_COUNT		(SDP_MTU_SIZE - SDP_PDU_HEADER_SIZE - 17/*size of continuation state*/ - 2)

#define SDAP_CPLEVT_HEADER_SIZE		7	//SdapEvent and SdapSearchCompleteEvent both header

#define INVALID_ATTRIBUTE_INDEX		0xFF

///////////////////////////////////////////////////
// SDAP Protocol
///////////////////////////////////////////////////

enum
{
	SDP_PDUID_ERROR_RESPONSE = 0x01,
	SDP_PDUID_SERVICE_SEARCH_REQUEST = 0x02,
	SDP_PDUID_SERVICE_SEARCH_RESPONSE = 0x03,
	SDP_PDUID_SERVICE_ATTRIBUTE_REQUEST = 0x04,
	SDP_PDUID_SERVICE_ATTRIBUTE_RESPONSE = 0x05,
	SDP_PDUID_SERVICE_SEARCH_ATTRIBUTE_REQUEST = 0x06,
	SDP_PDUID_SERVICE_SEARCH_ATTRIBUTE_RESPONSE = 0x07,
};

enum
{
	SDP_ERROR_INVALID_OR_UNSUPPORTED_SDP_VERSION = 0x0001,
	SDP_ERROR_INVALID_SERVICE_RECORD_HANDLE = 0x0002,
	SDP_ERROR_INVALID_REQUEST_SYNTAX = 0x0003,
	SDP_ERROR_INVALID_PDU_SIZE = 0x0004,
	SDP_ERROR_INVALID_CONTINUATION_STATE = 0x0005,
	SDP_ERROR_INSUFFICIENT_RESOURCE = 0x0006,
};

typedef struct
{
	U16 errorCode;
} SdpErrorRsp;

typedef struct
{
	U8 sdpData[1];
} SdpServiceSearchReq;

typedef struct
{
	U16 totalServiceRecordCount;
	U16 currentServiceRecordCount;
	U8 sdpData[1];
} SdpServiceSearchRsp;

typedef struct
{
	U32 serviceRecordHandle;
	U16 maximumAttributeByteCount;
	U8 attributeListData[1];
} SdpAttributeSearchReq;

typedef struct
{
	U16 attributeListsByteCount;
	U8 sdpData[1];
} SdpAttributeSearchRsp;

typedef struct
{
	U8 sdpData[1];
} SdpServiceSearchAttributeReq;

typedef struct
{
	U16 attributeListsByteCount;
	U8 sdpData[1];
} SdpServiceSearchAttributeRsp;

typedef union									
{
	SdpErrorRsp errorRsp;
	SdpServiceSearchReq serviceSearchReq;			 
	SdpServiceSearchRsp serviceSearchRsp;			  
	SdpAttributeSearchReq attributeSearchReq;
	SdpAttributeSearchRsp attributeSearchRsp;
	SdpServiceSearchAttributeReq serviceSearchAttributeReq;
	SdpServiceSearchAttributeRsp serviceSearchAttributeRsp;
} SdpProtocolData;

typedef struct
{
	U8 offset;
	U8 reserved[L2CAP_TX_PDU_OFFSET - 1];
	U16 l2capLength;
	U16 l2capCID;
	U8 pduId;
	U16 transactionId;
	U16 parameterLength;
	SdpProtocolData payload;
} SdpCommand;

typedef struct
{
	U8 offset;
	U8 reserved[L2CAP_RX_PDU_OFFSET - 1];
	U16 l2capLength;
	U16 l2capCID;
	U8 pduId;
	U16 transactionId;
	U16 parameterLength;
	U8 sdpData[1];
} SdpEvent;

///////////////////////////////////////////////////
// SDAP Search
///////////////////////////////////////////////////

#if 0
typedef U16 AttributeMatchMask;
#else
typedef U32 AttributeMatchMask;
#endif

typedef struct
{
	U16 totalByteCount;
	U8 profileId;
	AttributeMatchMask attributeMatchMask;
} AttributeMatchResult;

typedef struct
{
	U16 totalByteCount;
	U8 matchCount;
	AttributeMatchResult attributeMatchResult[MAX_SERVICE_MATCH_COUNT];
} ServiceSearchAttributeResult;

typedef struct
{
	U8 count;
	U8 pattern[MAX_SEARCH_PATTERN_SIZE];
} SdapSearchPattern;

typedef struct
{
	U16 offset;
	U16 maxByteCount;
} SdapTransactionCtl;

///////////////////////////////////////////////////
// SDAP Channel
///////////////////////////////////////////////////

enum
{
	SDAP_SERVER_CHANNEL,
	SDAP_CLIENT_CHANNEL
};

typedef struct stSdapChannel
{
	MMI_PROFILE_INFO mmiProfileInfo;
	L2CAP_CHANNEL_HDL l2capChannelHdl;
	U8 sdapChannelType;
	U8 XDATA_PTR currentCmdPtr;
	U8 XDATA_PTR continueRspPtr;
	U16 continueRspLength;
	U8 XDATA_PTR remoteDataPtr;
	struct stSdapChannel XDATA_PTR nextChannel;
} SDAP_CHANNEL;

///////////////////////////////////////////////////
// SDAP Control
///////////////////////////////////////////////////

typedef struct
{
	SdpServiceNode XDATA_PTR serviceNodeList;
	U16 nextTransactionId;
	SDAP_CHANNEL XDATA_PTR sdapChannelList[MAX_MULTI_POINT_NO];
	SDAP_CHANNEL channel[SDP_L2CAP_CHANNEL_NUM];
	U8 isIOS[MAX_MULTI_POINT_NO];
} SDAP_CTL_TYPE;


#endif

