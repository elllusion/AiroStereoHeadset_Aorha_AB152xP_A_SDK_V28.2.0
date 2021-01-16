
#ifndef __RFCOMM_H__
#define __RFCOMM_H__

#include "..\MMI\MMI_Base.h"
#include "RFCOMM_interface.h"

#define RFCOMM_DEFAULT_FRAME_SIZE	127

//GSM 07.10 address field
#define RFCOMM_CTRL_DLCI			0

#define BASIC_FRAME_LEN				4 //address, control, length & fcs fields

//macros to from specified field for rfcomm packets
#define SHORT_LEN_FIELD(len)	((len<<1) | 0x01/*no extension*/)

//GSM 07.10 control field
#define CTRL_FIELD_SABM				0x2F
#define CTRL_FIELD_UA				0x63
#define CTRL_FIELD_DM				0x0F
#define CTRL_FIELD_DISC				0x43
#define CTRL_FIELD_UIH				0xEF
#define CTRL_FIELD_PF_MASK			0x10
#define CTRL_FIELD_FRAME_TYPE_MASK	0xEF

//below is for control messages
#define CTRL_MSG_CR_RES				0x00
#define CTRL_MSG_CR_CMD				0x02
#define CTRL_MSG_EA_NOEXT			0x01
#define CTRL_MSG_CODE_MASK			0xFC
#define CTRL_MSG_DLCI_BIT12			(CTRL_MSG_EA_NOEXT | CTRL_MSG_CR_CMD)
#define CTRL_MSG_HEADER_LEN			2

#define RFCOMM_T1	20*3200L//1000/0.3125  //unit: half time slot - 312.5us  //  Acknowledgement Timer  10-60 seconds. Recommended value: 20 seconds
#define RFCOMM_T2	20*3200L//1000/0.3125 //unit: half time slot - 312.5us // Response Timer for Multiplexer Control Channel  10-60 seconds. Recommended value: 20 seconds
#define RFCOMM_TCUS 3*3200L//1000/0.3125 //unit: half time slot - 312.5us

// Control Message
#define CTRL_MSG_TYPE_NSC		0x10
#define CTRL_MSG_TYPE_TEST		0x20
#define CTRL_MSG_TYPE_RLS		0x50
#define CTRL_MSG_TYPE_FCOFF		0x60
#define CTRL_MSG_TYPE_PN		0x80
#define CTRL_MSG_TYPE_RPN		0x90
#define CTRL_MSG_TYPE_FCON		0xA0
#define CTRL_MSG_TYPE_MSC		0xE0

//PN
#define PN_DATA_LEN				8
#define PN_INIT_CREDITS			0

#define PN_D_MASK				0x3F
#define PN_I_MASK				0x0F
#define PN_CL_MASK				0xF0
#define PN_CL_OFFSET			4
#define PN_P_MASK				0x3F
#define PN_K_MASK				0x07

//MSC
#define MSC_DATA_LEN			2

#define MSC_EA_NOEXTENSION		0x01
#define MSC_FC_UNABLE_ACCEPT	0x02
#define MSC_RTC_READY2COM		0x04
#define MSC_RTR_READY2RX		0x08
#define MSC_IC_RING				0x40
#define MSC_DV_DATASENT			0x80

//RPN
#define RPN_DATA_LEN 			8

enum
{
	BAUDRATE_2400,
	BAUDRATE_4800,
	BAUDRATE_7200,
	BAUDRATE_9600,	/* default */
	BAUDRATE_19200,
	BAUDRATE_38400,
	BAUDRATE_57600,
	BAUDRATE_115200,
	BAUDRATE_230400
};

enum
{
	DATABIT_5,
	DATABIT_6,
	DATABIT_7,
	DATABIT_8	/* default */
};

enum
{
	STOPBIT_1,	/* default */
	STOPBIT_5
};

enum
{
	PARITY_NO,	/* default */
	PARITY_YES
};

enum
{
	PARITY_ODD,
	PARITY_EVEN,
	PARITY_MARK,
	PARITY_SPACE
};

//RLS
#define RLS_DATA_LEN			2

enum
{
	RLS_NO_ERR		= 0,
	RLS_FRAMING_ERR = 0x3,
	RLS_PARITY_ERR	= 0x5,
	RLS_OVERRUN_ERR	= 0x9
};

//no of bytes when calculating FCS. for UIH it's add & ctrl. for others it's add, ctrl & len.
#define NORMAL_FCS_CAL_BYTES	3
#define UIH_FCS_CAL_BYTES		2

#define RFCOMM_LENGTH_HIGH_BYTE_SHIFT	7

//For RFCOMM debug
//#define RFCOMM_KERNEL_DEBUG

enum
{
	RESPONDER,
	INITATOR
};

enum
{
	RFCOMM_OK,
	RFCOMM_FAIL,
	RFCOMM_NO_TX_CREDIT, 
	RFCOMM_AGG_FC_ERROR,
	RFCOMM_V24_FC_ERROR,
};

enum
{
	RFCOMM_STATE_IDLE,
	RFCOMM_L2CAP_CONNECTING,
	RFCOMM_L2CAP_CONNECTED,
	RFCOMM_SESSION_CONNECTED,
	RFCOMM_SESSION_WAIT_TO_BE_RELEASED,
	RFCOMM_SESSION_DISC0_SENT_WAIT_UA,
};

/////////////////////////////////////
// For PTS test case generation
/////////////////////////////////////
//#define RFCOMM_PTS_TP_RFC_BV_07_C
//#define RFCOMM_PTS_TP_RFC_BV_14_C		//send remote line status (framing error)
//#define RFCOMM_PTS_TP_RFC_BV_21_22_C

enum
{
	DLC_NOT_ESTABLISHED,
	//active
	WAIT_PN_RSP, //PN command sent, wait PN response
	SABM_SENT_WAIT_UA, //SABM sent, wait UA
	//passive
	PN_RSP_SENT_WAIT_SABM, //on reception of PN command, send PN response and wait for SABM
	WAIT_MSC_CMD, //on reception of SABM, send UA. DLC established at the same time
	//below states are only available after DLCs are established
	WAIT_MSC_RSP, // MSC command has been sent, wait for the response
	MSC_ONEWAY_OK,
	READY_FOR_DATA_COM, //MSCs are sent in both directions, data communication is ready
	
	DISC_SENT_WAIT_UA,
	DISC_SENT_WAIT_UA_DISC_NEXT,
};

enum
{
	CMD_INIT_TO_RSP = 0x02, //0x01<<2
	CMD_RSP_TO_INIT = 0x00,
	RSP_INIT_TO_RSP = 0x00,
	RSP_RSP_TO_INIT = 0x02  //0x01<<2
};

enum
{
	SABM = 0x01,
	DISC,
	UA,
	DM,
	UIH_CTRL,
	UIH_DATA_NOCREDIT,
	UIH_DATA_CREDITS //UIH frames with credits
};

enum
{
	PN = 0x01,
	MSC,
	RLS,
	RPN,
	TEST
};

enum
{
	RFCOMM_NO_TIMER,
	RFCOMM_TIMER_T1,
	RFCOMM_TIMER_T2,
	RFCOMM_TIMER_WAIT_MSC_CMD,
	RFCOMM_TIMER_NODLCI_WAIT_DISC,
	RFCOMM_TIMER_T1_ON_DISC,
	RFCOMM_TIMER_PTS_TEST_1,
	RFCOMM_TIMER_PTS_TEST_2,
	L2CAP_TIMER_WAIT_FOR_DISC_COMPLETE,
};

typedef struct
{
	U8 addField;
	U8 ctlField;
	U8 lenField;
	U8 fcsField;
}RfcommBasicFrameType;

typedef struct
{
	U16 nextOffset;
	U8 lenField;
}RfcommTxProcHeader;

typedef struct
{
	U8 addField;
	U8 ctlField;
	U8 lenField;
}RfcommFrameHeaderType;

typedef struct
{
	U8 addField;
	U8 ctlField;
	U8 lenField;
	U8 lenFieldEx;
}RfcommFrameHeaderExType;

typedef struct
{
	U8 addField;
	U8 ctlField;
	U8 lenField;
	U8 credits;
}RfcommUIHCreditFCHeaderType;

//PN (ParameterNegotiation)
typedef struct RfcommPNTag
{
	U8 Dbits;
	U8 IbitsCLbits;
	U8 Pbits;
	U8 Tbits;
	U8 NLowerbits;
	U8 NHigherbits;
	U8 NAbits;
	U8 Kbits;
}RfcommPN;

typedef struct
{
	U8 dlci;
	U8 v24;
}RfcommMSC;

typedef struct
{
	U8 dlci;
	U8 optionalValue[7];
}RfcommRPN;

//RLS
typedef struct RfcommRLSTag
{
	U8 dlci;
	U8 status;
}RfcommRLS;

//NSC
typedef struct RfcommNSCTag
{
	U8 non_supported_cmd;
}RfcommNSC;

typedef union
{
	RfcommPN pn;
	RfcommMSC msc;
	RfcommRPN rpn;
	RfcommRLS rls;
	RfcommNSC nsc;
}CtrlMsgType;

typedef struct RfcommCtrlMsgTag
{
	U8 type;
	U8 len;
	CtrlMsgType ctrlMsg;
}RfcommCtrlMsg;

typedef struct
{
	OST XDATA_PTR ptr;
	U8 type;
}RfcommTimer;

typedef struct CreditBaseFCTag
{
	U8 rxAccCredits; //incoming accumulated credits
	U8 txAccCredits; //outgoing accumulated credits
}RfcommCreditBaseFC;

typedef union
{
	U8 byte;
	struct
	{
		U8 ea:1;
		U8 fc:1;
		U8 rtc:1;
		U8 rtr:1;
		U8 reserved:2;
		U8 ic:1;
		U8 dv:1;
	} bits;
}V24Type;

#ifdef RFCOMM_KERNEL_DEBUG
typedef struct 
{
	U8 errorCode;
	U8 currDlci;
}RfcommKernelDebugType;
#endif


typedef struct
{
	U8 noExtension:1;
	U8 cr:1;
	U8 dlci:6;
	U8 ctlField;
	U8 lengthNoExtended:1;
	U8 lengthLowByte:7;
	U8 lengthHighByte;  // if no extended, data begins from here
	U8 dataBegin;
}RfcommPktType;

typedef union
{
	U8 extendedLength;
	RfcommPN pnInfo;
	RfcommMSC mscInfo;
	RfcommRLS rlsInfo;
	RfcommRPN rpnInfo;
}RfcommUIHDataType;

typedef struct
{
	U8 commandCode;
	U8 lengthNoExtend:1;
	U8 UIHlength:7;
	RfcommUIHDataType dataBody;
}RfcommUIHMsgType;

/*
for the 1st version, rfcommSessionTbl only supports one rfcommDlciInfoTbl
each application willing to create a new server channel on the same rfcomm session shall dynamically create a new rfcommSessionTbl, which attached the same bdaddr
*/
typedef struct stRfcommDlciInfoTag
{
	struct stRfcommDlciInfoTag XDATA_PTR dlciInfoNext;
	RFCOMM_PROFILE_DESC CODE_PTR profileDesc;
	U8 dlci;
	U8 dlcState;
	U16 maxFrameSize;
	RfcommCreditBaseFC creditFC;
	V24Type remoteV24Info;
	//V24Type localV24Info;
	RfcommTimer timer;
}RfcommDlciInfo;

#define MAX_RFCOMM_DLCI_COUNT   (3 * RFCOMM_L2CAP_CHANNEL_NUM)   //active server channel total count

typedef struct stRfcommProfileConnReqTag
{
	struct stRfcommProfileConnReqTag XDATA_PTR nextReq;
	RFCOMM_PROFILE_DESC CODE_PTR profileDesc;
	U8 remoteSCN;
}RfcommProfileConnReq;

typedef struct
{
	U8 seRole : 1; //initator or responder in the current session
	U8 reserved : 7;
	U8 seState;
	L2CAP_CHANNEL_HDL l2capChannelHdl;
	OSMQ OSMQ_Profile_Data_To_RFCOMM;
	//U8 dlciCount; /*total number of dlci in between*/
	RfcommDlciInfo XDATA_PTR dlciInfoList;
	RfcommProfileConnReq XDATA_PTR profileConnReqList;
	RfcommTimer sessionTimer;
#ifdef RFCOMM_KERNEL_DEBUG
	RfcommKernelDebugType debugInfo;
#endif
}RfcommSessionInfo;

typedef struct
{
	U16	maxFrameSize; /* max frame size among all registered profiles */
	RfcommSessionInfo seInfo[MAX_MULTI_POINT_NO];
}RfcommCtlType;


PUBLIC U8 RFCOMM_CalcFCS(U8 dataLen, U8 XDATA_PTR dataPtr);

PUBLIC U8 RFCOMM_GetExtMTUSize(void);
PUBLIC RfcommSessionInfo XDATA_PTR RFCOMM_GetSessionInfo(U8 linkIndex);

PUBLIC RfcommDlciInfo XDATA_PTR RFCOMM_SearchVacantDlciInfo(void);
PUBLIC RfcommDlciInfo XDATA_PTR RFCOMM_SearchProfileScn(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 profileId);

PUBLIC BOOL RFCOMM_LocalDlciEstablish(RfcommSessionInfo XDATA_PTR rfcommSeInfo, RFCOMM_PROFILE_DESC CODE_PTR profileDesc, U8 remoteSCN);
PUBLIC void RFCOMM_SendProfileDiscEvt(RFCOMM_PROFILE_DESC CODE_PTR profileDesc, U8 linkIndex, U8 evtCode);
PUBLIC void RFCOMM_SendAllProfileDiscEvt(U8 linkIndex, U8 evtCode);
PUBLIC void RFCOMM_SendProfileConnectFailEvt(RfcommSessionInfo XDATA_PTR rfcommSeInfo, RFCOMM_PROFILE_DESC CODE_PTR profileDesc, U8 status);
PUBLIC void RFCOMM_RelDlciResource(RfcommSessionInfo XDATA_PTR rfcommSeInfo, RfcommDlciInfo XDATA_PTR dlciInfo);
PUBLIC void RFCOMM_RelAllDlciResource(RfcommSessionInfo XDATA_PTR rfcommSeInfo);
PUBLIC void RFCOMM_RelSessionResource(RfcommSessionInfo XDATA_PTR rfcommSeInfo);

PUBLIC void RFCOMM_ReleaseTimer(RfcommTimer XDATA_PTR timerPtr);
PUBLIC void RFCOMM_StartTimer(RfcommTimer XDATA_PTR timerPtr, U8 timerType);
PUBLIC U8 XDATA_PTR RFCOMM_AllocFrameData(U16 infoLen);

PUBLIC void RFCOMM_SendBasicFrame(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 dlci, U8 frameType);
PUBLIC void RFCOMM_SendDiscFrame(RfcommSessionInfo XDATA_PTR rfcommSeInfo);
PUBLIC void RFCOMM_SendUIHFrame(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 dlci, U16 len, U8 XDATA_PTR packetPtr);
PUBLIC void RFCOMM_SendCreditViaUIH(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 dlci, U8 givenCredits);
PUBLIC void RFCOMM_SendTEST(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 XDATA_PTR dataPtr, U16 dataLen, U8 crType);
PUBLIC void RFCOMM_SendRLS(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 dlci, U8 status, U8 crType);
PUBLIC void RFCOMM_SendAggFC(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 aggFC, U8 crType);
PUBLIC void RFCOMM_SendPN(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 dlci, U16 maxFrameSize, U8 crType);
PUBLIC void RFCOMM_SendRPN(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 dlci, U8 isQuery, U8 crType);
PUBLIC void RFCOMM_SendMSC(RfcommSessionInfo XDATA_PTR rfcommSeInfo, U8 dlci, U8 v24, U8 crType);

PUBLIC void RFCOMM_ChannelDataHandler(L2CAP_CHANNEL_HDL l2capChannelHdl, U8 XDATA_PTR dataPtr);

#endif //__RFCOMM_H__
