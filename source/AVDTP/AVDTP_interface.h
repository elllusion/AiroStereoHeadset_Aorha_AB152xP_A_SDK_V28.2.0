//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
			
#ifndef _AVDTP_INTERFACE_H_
#define _AVDTP_INTERFACE_H_

#include "os.h"
#include "..\L2CAP\L2CAP_Interface.h"

#ifdef _AVDTP_C_
  #define AVDTP_EXTERN  
#else
  #define AVDTP_EXTERN extern
#endif

#define AVDTP_TX_OFFSET		L2CAP_TX_PDU_OFFSET

/////////////////////////////////////
// For PTS test case generation
/////////////////////////////////////
//#define AVDTP_PTS_TP_SIG_SEC_BV_01_C
//#define AVDTP_PTS_TP_SIG_SMG_BV_11_C
//#define AVDTP_PTS_TP_SIG_SMG_BV_13_C
//#define AVDTP_PTS_TP_SIG_SMG_BV_19_C
//#define AVDTP_PTS_TP_SIG_SMG_BV_23_C
//#define AVDTP_PTS_TP_SIG_SMG_BV_25_28_C_BI_35_36_C
//#define AVDTP_PTS_TP_SIG_SMG_ESR05_BV_13_C
//#define AVDTP_PTS_TP_SIG_SYN_BV_02_C

//#define GAVDP_PTS_TP_APP_TRC_BV_01_C	/*Initiator*/
//#define GAVDP_PTS_TP_APP_TRC_BV_02_C	/*Initiator*/

enum
{
	AVDTPSignalChannelType,
	AVDTPMediaChannelType,
	//AVDTPReportingChannelType,
	//AVDTPRecoveryChannelType,
	MAX_AVDTP_CHANNEL_TYPE_NUM,
};
// in design, no reporting and recovery channel 

enum
{
	AVDTP_COMMAND_MSG,
	AVDTP_GENERAL_REJECT_MSG,
	AVDTP_RESPONSE_ACCEPT_MSG,
	AVDTP_RESPONSE_REJECT_MSG,
};

enum
{
	AVDTP_SINGLE_PACKET,
	AVDTP_START_PACKET,
	AVDTP_CONTINUE_PACKET,
	AVDTP_END_PACKET,
};

enum
{
	AVDTP_DISCOVER = 0x01,
	AVDTP_GET_CAPABILITIES,
	AVDTP_SET_CONFIGURATION,
	AVDTP_GET_CONFIGURATION,
	AVDTP_RECONFIGURE,
	AVDTP_OPEN,
	AVDTP_START,
	AVDTP_CLOSE,
	AVDTP_SUSPEND,
	AVDTP_ABORT,
	AVDTP_SECURITY_CONTROL,
	AVDTP_GET_ALL_CAPABILITIES,
	AVDTP_DELAYREPORT,
	AVDTP_LAST_SIGNALING
};

enum
{
	AVDTP_CREATE_SIGNALING_CHANNEL_CMD,
	AVDTP_CREATE_MEDIA_CHANNEL_CMD,
	AVDTP_DISCONNECT_SIGNALING_CHANNEL_CMD,
	AVDTP_DISCONNECT_MEDIA_CHANNEL_CMD,
	AVDTP_START_REJECT_RESP,
	AVDTP_COMMAND_REJECT_RESP,
	AVDTP_PROTOCOL_COUNT,
	// Internal Use
};

enum
{
	AVDTP_NO_ERROR,

	//ACP to INT, Signal Response header Error Code
	AVDTP_BAD_HEADER_FORMAT = 0x01,	//All signals

	//ACP to INT, Signal Response Payload Error Codes
	AVDTP_BAD_LENGTH = 0x11,		//All signals
	AVDTP_BAD_ACP_SEID,				//All signals
	AVDTP_SEP_IN_USE,				//Support in "Set Configuration" signal only.
	AVDTP_SEP_NOT_IN_USE,			//Support in "ReConfigure" signal only.
	AVDTP_BAD_SERV_CATEGORY = 0x17,	//Support in "Set Configuration and "ReConfigure"" signals only.
	AVDTP_BAD_PAYLOAD_FORMAT,		//All signals
	AVDTP_NOT_SUPPORT_COMMAND,		//All signals
	AVDTP_INVALID_CAPABILITIES,		//All signals

	//ACP to INT, Signal Response Transport Service Capabilities Error Codes
	AVDTP_BAD_RECOVERY_TYPE = 0x22,
	AVDTP_BAD_MEDIA_TRANSPORT_FORMAT,
	AVDTP_BAD_RECOVERY_FORMAT = 0x25,
	AVDTP_BAD_ROHC_FORMAT,
	AVDTP_BAD_CP_FORMAT,
	AVDTP_BAD_MULTIPLEXING_FORMAT,
	AVDTP_UNSUPPORTED_CONFIGURATION,

	//ACP to INT, Procedure Error Codes
	AVDTP_BAD_STATE = 0x31,
};


typedef union
{
	U8 para[1];		//place holder
	struct {
		U8 rfa:2;
		U8 seid:6;
	} getCapabilityCmd;
}AvdtpMsgBodyType;

typedef struct
{
	U8 offset;
	U8 dummy[AVDTP_TX_OFFSET - 1];
	U16 l2capLength;
	U16 l2capCID;
	U8 para[1];		//place holder
}AvdtpMediaType;

typedef struct
{
	U8 offset;
	U8 dummy[AVDTP_TX_OFFSET - 1];
	U16 l2capLength;
	U16 l2capCID;
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 para[1];		//place holder
}AvdtpSignalType;

typedef struct
{
	U8 offset;
	U8 dummy[AVDTP_TX_OFFSET - 1];
	U16 l2capLength;
	U16 l2capCID;
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 acpSEID; 
	U8 intSEID;
	U8 configData[1];
}AvdtpSignalConfigType;


typedef struct
{
	MMI_PROFILE_DESC mmiProfileDesc;
	void (*MediaLinkEvent)(U8 linkIndex, U8 evtCode, LINK_EVENT_PARAMS XDATA_PTR evtParams);
	void (*MediaDataEvent)(U8 linkIndex, U8 XDATA_PTR dataPtr);
} AVDTP_PROFILE_DESC;


PUBLIC L2CAP_CHANNEL_HDL AVDTP_ConnectSignalChannel(U8 linkIndex);
PUBLIC U8 AVDTP_DisconnectSignalChannel(U8 linkIndex);

PUBLIC L2CAP_CHANNEL_HDL AVDTP_ConnectMediaChannel(U8 linkIndex);
PUBLIC U8 AVDTP_DisconnectMediaChannel(U8 linkIndex);
PUBLIC void AVDTP_SendMediaChannelData(U8 linkIndex, AvdtpMediaType XDATA_PTR dataPtr);
PUBLIC BOOL AVDTP_IsAacAllowedInLink(U8 linkIndex);

PUBLIC void AVDTP_SendDiscoverCmd(U8 linkIndex);
PUBLIC void AVDTP_SendGetCapabilitiesCmd(U8 linkIndex, U8 seid);
PUBLIC void AVDTP_SendGetAllCapabilitiesCmd(U8 linkIndex, U8 seid);
PUBLIC void AVDTP_SendSetConfigurationCmd(U8 linkIndex, U8 acpSeid, U8 intSeid, U8 configLen, U8 XDATA_PTR configParam);
PUBLIC void AVDTP_SendGetConfigurationCmd(U8 linkIndex, U8 acpSeid);
PUBLIC void AVDTP_SendReconfigureCmd(U8 linkIndex, U8 acpSeid, U8 configLen, U8 XDATA_PTR configParam);
PUBLIC void AVDTP_SendOpenCmd(U8 linkIndex, U8 acpSeid);
PUBLIC void AVDTP_SendCloseCmd(U8 linkIndex, U8 acpSeid);
PUBLIC void AVDTP_SendStartCmd(U8 linkIndex, U8 acpSeid);
PUBLIC void AVDTP_SendSuspendCmd(U8 linkIndex, U8 acpSeid);
PUBLIC void AVDTP_SendAbortCmd(U8 linkIndex, U8 acpSeid);
PUBLIC void AVDTP_SendDelayReportCmd(U8 linkIndex, U8 acpSeid, U16 delay);
PUBLIC void AVDTP_SendSecurityControlCmd(U8 linkIndex, U8 acpSeid, U8 cpDataLen, U8 XDATA_PTR cpDataPtr);

PUBLIC void AVDTP_SendAcceptRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr);
PUBLIC void AVDTP_SendRejectRsp(U8 linkIndex, AvdtpSignalType XDATA_PTR cmdPtr, U8 errorCode);

#endif
