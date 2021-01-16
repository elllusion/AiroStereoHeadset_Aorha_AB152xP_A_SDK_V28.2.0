//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _L2CAP_H_ 
#define _L2CAP_H_

#include "rc.h"
#include "drv_sector.h"

#include "L2CAP_interface.h"

// Constant;
#define L2CAP_USER_CID_MIN			0x0040
#define L2CAP_USER_CID_MAX			0x00EE

#define L2CAP_DEFAULT_MTU_SIZE		672
#define L2CAP_MINIMUN_MTU_SIZE		48
#define L2CAP_DEFAULT_FLUSH_TO		0xFFFF

#define HCI_PB_MASK						(0x03 << 4)
#define HCI_PB_L2CAP_START_NON_FLUSH	(0x00 << 4)
#define HCI_PB_L2CAP_START_AUTO_FLUSH	(0x02 << 4)
#define HCI_PB_L2CAP_CONTINUE			(0x01 << 4)
#define HCI_BC_MASK						(0x03 << 6)
#define HCI_BC_ASB						(0x01 << 6)
#define HCI_BC_PSB						(0x02 << 6)	

#define HCI_FIELD_PB_L2CAP_START	(0x02)

//CMD Reject Reason-------- The value sould be 0x00XX  ----------------
enum
{
	L2CAP_REJECT_REASON_CMD_NOT_UNDERSTOOD = 0x00,
	L2CAP_REJECT_REASON_SIGNALLING_MTU_EXCEEDED ,
	L2CAP_REJECT_REASON_INVALID_CID_IN_REQUEST
};
//CMD Reject Reason End--------------------------------------------


// Config Result--------------------------------------------------
enum
{
	L2CAP_CONFIG_RESULT_SUCCESS,
	L2CAP_CONFIG_RESULT_FAILURE_UNACCEPTABLE_PARA,	
	L2CAP_CONFIG_RESULT_FAILURE_REJECTED, 
	L2CAP_CONFIG_RESULT_FAILURE_UNKNOWN_OPTIONS 
};
//-------------------------------------------------------------

#define L2CAP_CONFIGURATION_OPTIONS_MTU					0x01
#define L2CAP_CONFIGURATION_OPTIONS_FLUSH_TO			0x02
#define L2CAP_CONFIGURATION_OPTIONS_QOS					0x03
#define L2CAP_CONFIGURATION_OPTIONS_RFC					0x04

#define L2CAP_LOAD_PARAM_STATE_UNLOADED					0
#define L2CAP_LOAD_PARAM_STATE_LOADED					1
#define L2CAP_LOAD_PARAM_STATE_CONFIG_REQ_SENT			2
//for c-flag is TRUE
#define L2CAP_LOAD_PARAM_STATE_LOADED_WAIT_FALSE_CFLAG	3

#define L2CAP_CONFIGURATION_OPTIONS_MTU_LENGTH			4
#define L2CAP_CONFIGURATION_OPTIONS_FLUSH_TO_LENGTH		4
#define L2CAP_CONFIGURATION_OPTIONS_QOS_LENGTH			24
#define L2CAP_CONFIGURATION_OPTIONS_RFC_LENGTH			11


#define SUPPORT_L2CAP_IGNORE_COPY_WHEN_RECOMBINE_HCI_DATA

/////////////////////////////////////
// For PTS test case generation
/////////////////////////////////////
//#define L2CAP_PTS_TC_COS_CED_BV_03_C
//#define L2CAP_PTS_TC_COS_ECH_BV_02_C
//#define L2CAP_PTS_TC_COS_IEX_BV_01_C
//#define L2CAP_PTS_TC_CLS_CLR_BV_01_C
//#define L2CAP_PTS_TC_CLS_UCD_BV_01_C

//---------------------------------------------------------------
//#define SUPPORT_L2CAP_DISCONNECT_AFTER_CONFIG_ERROR	//ALT2 if defined, for TP_COS_CED_BV_08_C, otherwise ALT1
//#define SUPPORT_L2CAP_CONNECTIONLESS_MTU
//#define SUPPORT_L2CAP_FLOW_CONTROL_MODE_AND_RETRANSMISSION_MODE
//---------------------------------------------------------------

enum
{
	L2CAP_STATE_CLOSED,			 //0x0
	L2CAP_STATE_WAIT_CONNECT_ACCEPT,
	L2CAP_STATE_WAIT_CONNECT_SEND,
	L2CAP_STATE_WAIT_CONNECT_RSP,
	L2CAP_STATE_WAIT_SEND_DISCONNECT_CMD,
	L2CAP_STATE_WAIT_DISCONNECT_RSP,
	L2CAP_STATE_OPEN,
	L2CAP_STATE_WAIT_CONFIG,

	L2CAP_STATE_CONFIG_SEND_CONFIG_REQ = 0x10,
	L2CAP_STATE_CONFIG_SEND_CONFIG_RSP = 0x20,
	L2CAP_STATE_CONFIG_GET_CONFIG_REQ  = 0x40,
	L2CAP_STATE_CONFIG_GET_CONFIG_RSP  = 0x80,
};

////////////////////////////////////////////////////////////////////////////////
// Control Type to L2cap
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	L2CAP_DATA_CTL				data_ctl;
	L2CAP_RECOMBINE_CTL			recombine_ctl;
	L2CAP_CHANNEL XDATA_PTR		l2capChannelList;
} L2CAP_LINK_INFO;

typedef struct
{
	U8 cid_seed;
	U8 out_identifier;
	L2CAP_PROTOCOL_NODE XDATA_PTR l2capProtocolNodeList;
	L2CAP_LINK_INFO linkInfo[MAX_MULTI_POINT_NO];
	L2CAP_CHANNEL channel[MAX_NUM_L2CAP_CHANNEL];
} L2CAP_CTL;

////////////////////////////////////////////////////////////////////////////////
// Utility Functions;
////////////////////////////////////////////////////////////////////////////////

PUBLIC L2CAP_PROTOCOL_DESC CODE_PTR L2CAP_GetProtocolDesc(U16 psm);

PUBLIC U8 L2CAP_GenNextOutIdentifier(void);
PUBLIC L2CAP_LINK_INFO XDATA_PTR L2CAP_GetLinkInfo(U8 linkIndex);

PUBLIC L2CAP_CHANNEL_HDL L2CAP_GetVacantChannel(U8 linkIndex);
PUBLIC void L2CAP_InsertChannel(L2CAP_CHANNEL_HDL l2capChannelHdl);
PUBLIC void L2CAP_RemoveChannel(L2CAP_CHANNEL_HDL l2capChannelHdl);
PUBLIC void L2CAP_CloseChannel(L2CAP_CHANNEL_HDL l2capChannelHdl);

PUBLIC L2CAP_CHANNEL_HDL L2CAP_FindChannel(U8 linkIndex, U16 cid);

#endif

