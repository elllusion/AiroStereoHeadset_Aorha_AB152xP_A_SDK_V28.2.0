#ifndef _LE_COMMON_HAL_H_
#define _LE_COMMON_HAL_H_

#include "cpu.h"
#include "rc.h"
#include "..\ROM\HC\HC_interface.h"

#ifdef LE_SUPPORTED_HOST_FLASH
/******************************************************************************
 * FILE NAME :      le_common_hal.h
 *
 * DESIGNER :
 *
 * COPYRIGHT :      Copyright (c) Airoha Tek. Cop.
 *
 * DESCRIPTION :
 *
 * REVISION HISTORY :
 *  MAINTAINER  DATE        DESCRIPTION
 *  Michelle    20141001    Creation
 *
 ******************************************************************************/

#ifdef AB1600
#include "HC_interface.h"

#define __LITTLE_ENDIAN_MCU__

#define HALF_MIN (30*3200L)


#define BD_ADDR_LEN                 6
#define MAX_NUM_OF_LINK             16
#define MMI_NVRAM_PAGE_SIZE         512
#define HCI_XPORT_HDR_LEN			4

typedef U8 HCI_UART_XPORT_ACTION;
#define HCI_UART_XPORT_COMMAND			(HCI_UART_XPORT_ACTION)1
#define HCI_UART_XPORT_DATA				(HCI_UART_XPORT_ACTION)2
#define HCI_UART_XPORT_EVENT			(HCI_UART_XPORT_ACTION)4

typedef	struct
{
	U8 len;
	U8 spiCmd;
	U8 xportPktId;
	U8 dataPtr[1];
}HCI_XPORT_PKT_STRU;

typedef struct stru_hci_evt
{
    U8 evtCode;
    U8 len;
    HCI_EVT_PAYLOAD_UNION_STRU payload;
}HCI_LE_EVT_STRU;

typedef struct
{
	U8 HCI_XPORT_Hdr[HCI_XPORT_HDR_LEN];
}LE_HAL_EVT_STRU;

HCI_XPORT_PKT_STRU XDATA_PTR HC_XportPktGetMem(HCI_UART_XPORT_ACTION act);

#else   // AB1520

#define __BIG_ENDIAN_MCU__
//#define SM_SUPPORTED

#define BD_ADDR_LEN                 sizeof(BD_ADDR)
#define MAX_NUM_OF_LINK             1
#define PACKED_STRUCT
#define LE_HCI_CMD_OFFSET           2

typedef	struct
{
	U8 offset;
	U8 rsvd[1];	
}HCI_XPORT_PKT_STRU;

typedef struct
{
	U8 offset;
	U8 rsvd[3];
}LE_HAL_EVT_STRU;

typedef struct
{	
	HCI_DATA_HDR_STRU hciHdr;
	U16 len;
	U16 cid;
}PACKED_STRUCT L2CAP_LE_DATA_HDR_STRU;

typedef struct
{
	LE_HAL_EVT_STRU halHdr;
	L2CAP_LE_DATA_HDR_STRU l2capHdr;
	U8  payload[1];
}L2CAP_LE_DATA_STRU;

/* HCI Event Parameter Structure Format*/
typedef struct stru_hci_evt_disconn_cmp
{
    U8 status;
    U16 connHandle;//:12;
    U8 reason;
}PACKED_STRUCT HCI_EVT_DISCONN_CMP_STRU;

typedef struct stru_hci_evt_enc_change
{
    U8 status;
    U16 connHandle;//:12;
    U8 encEnabled;
}PACKED_STRUCT HCI_EVT_ENC_CHANGE_STRU;

typedef struct stru_hci_evt_cmd_cmp
{
	U8 numHciCmdPkt;
	U8 cmdCode[2];	// little eddian
    U8 status;
	U8 cmdPara[1];
}PACKED_STRUCT HCI_EVT_CMD_COMPLETE_STRU;

typedef struct stru_hci_evt_le_cmd_status
{
	U8 status;
	U8 numCmdPkts;
	U16 cmdCode;
}PACKED_STRUCT HCI_EVT_LE_CMD_STATUS_STRU;

typedef struct stru_hci_evt_num_cmp_pkt
{
	U8 numHandles;
	struct
	{
		U16 connHandle;//:12;
		U16 numCmpPkts;
	}pair[1];
}PACKED_STRUCT HCI_EVT_NUM_CMP_PKT_STRU;

typedef struct stru_hci_evt_vendor_evt     //TODO: this structure is not confirmed yet
{
	U8 para[10];
}PACKED_STRUCT HCI_EVT_VENDOR_EVT_STRU;

typedef struct stru_hci_evt_le_conn_cmp
{
    U8 subEvtCode;
    U8 status;
    U16 connHandle;//:12;
    U8 role;
    U8 peerAddrType;
    U8 peerAddr[6];
    U8 connInterval[2];
    U8 connLatency[2];
    U8 supervisionTimeout[2];
    U8 masterClockAccuracy;
}PACKED_STRUCT HCI_EVT_LE_CONN_CMP_STRU;

typedef struct stru_hci_evt_le_conn_update_cmp
{
	U8 subEvtCode;
	U8 status;
	U16 connHandle;//:12;
	U16 connInterval;
	U16 connLatency;
	U16 supervisionTimeout;
}PACKED_STRUCT HCI_EVT_LE_CONN_UPDATE_CMP_STRU;

typedef struct stru_hci_evt_le_read_remote_used_fea_cmp
{
	U8 subEvtCode;
	U8 status;
	U16 connHandle;//:12;
	struct{
		U8 leEncryption		:1;
		U8 connParamReqProc		:1;
		U8 extRejectionInd		:1;
		U8 slaveInitFeaExchange	:1;
		U8 lePing				:1;
		U8 rfu[7];
	}BitMask;
}PACKED_STRUCT HCI_EVT_LE_READ_REMOTE_CMP_STRU;

typedef struct stru_hci_evt_le_remote_conn_param_req
{
	U8 subEvtCode;
	U16 connHandle:12;
	U16 intervalMin;
	U16 intervalMax;
	U16 latency;
	U16 timeOut;
}PACKED_STRUCT HCI_EVT_LE_REMOTE_CONN_PARAM_REQ_STRU;

typedef struct
{
	U8 numHciCmdPkt;
	U8 status;
	U8 rand[8];
}PACKED_STRUCT HCI_EVT_LE_RAND_PARAM_STRU;

typedef struct
{
    U8 ntfType;
    U8 etrPasskey;
}PACKED_STRUCT HCI_LE_PASSKEY_RSP;

typedef union
{
    HCI_LE_PASSKEY_RSP pkRsp;
    U8 option;
    U8 dataBegin;
}UART_MSG_PARA;

typedef struct
{
	U8 vocf;
	U8 vogf;
	BD_ADDR bdAddr;
    UART_MSG_PARA payload;
}PACKED_STRUCT HCI_EVT_LE_UART_MSG_STRU;

typedef struct
{
	U8 vocf;
	U8 vogf;
	U8 status;
}PACKED_STRUCT HCI_LE_UART_CMD_RSP_STRU;

typedef union union_hci_evt_payload
{
    U8 paraPtr[1];
    HCI_EVT_DISCONN_CMP_STRU disconnCmp;
    HCI_EVT_ENC_CHANGE_STRU encChange;
    HCI_EVT_CMD_COMPLETE_STRU cmdCmp;
    HCI_EVT_LE_CMD_STATUS_STRU cmdSts;
    HCI_EVT_NUM_CMP_PKT_STRU numCmpPkt;
    HCI_EVT_VENDOR_EVT_STRU vendorEvt;

    /*the following events are LE Meta event*/
	U8 subEvtCode;
    HCI_EVT_LE_CONN_CMP_STRU connCmp;
    HCI_EVT_LE_CONN_UPDATE_CMP_STRU updateCmp;
    HCI_EVT_LE_READ_REMOTE_CMP_STRU	remoteFeature;
    HCI_EVT_LE_REMOTE_CONN_PARAM_REQ_STRU connParamReq;
    HCI_EVT_LE_UART_MSG_STRU uartCmd;
    HCI_EVT_LE_UART_MSG_STRU uartEvt;
    HCI_LE_UART_CMD_RSP_STRU uartRsp;
}HCI_EVT_PAYLOAD_UNION_STRU;

/* HCI Event Format */
typedef struct stru_hci_evt
{
    U8 offset;
    U8 rsvd[1];
    U8 msgCode;
    U8 evtCode;
    U8 len;
    HCI_EVT_PAYLOAD_UNION_STRU payload;
}HCI_LE_EVT_STRU;

#endif

typedef struct
{
#ifndef AB1600
	U8 Offset;
    U8 rsvd[3];
#endif	//AB1520
    HCI_DATA_HDL_FLAGS_UNION hciHdr;
	U16 hciLen;
    U16 L2CLen;
	U16 CID;
	U8 Opcode;
}LIC_L2CAP_DATA_PKT_TYPE;

//// Switch Endianness dependent MACRO////
///////*********ATTENTION*********///////
/////// make sure input endianness///////
///////        is predictable.    ///////
///////***************************///////

#ifdef __BIG_ENDIAN_MCU__
    // Used when input endian is the same as MCU. Force output to be little endian
    #define TO_LITTLE_ENDIAN_U16 ENDIAN_TRANSFORM_U16
    #define TO_LITTLE_ENDIAN_U32 ENDIAN_TRANSFORM_U32

    // Used when input is always little endian, and you want output to be the same as MCU
    #define TO_MCU_ENDIAN_U16 ENDIAN_TRANSFORM_U16
    #define TO_MCU_ENDIAN_U32 ENDIAN_TRANSFORM_U32
#else
    #define TO_LITTLE_ENDIAN_U16 
    #define TO_LITTLE_ENDIAN_U32 
    #define TO_MCU_ENDIAN_U16 
    #define TO_MCU_ENDIAN_U32 
#endif
PUBLIC BOOL SYS_ValueCompare(U8 XDATA_PTR ptr1, U8 GENERIC_PTR ptr2, U8 len);
PUBLIC void SYS_SwitchEndianBDAddr(U8 XDATA_PTR bdAddr);

//#define TEST_3_PARTY_CTRLOR   
/////////////////////tmp end

#endif /* _LE_COMMON_HAL_H_ */
#endif /* LE_SUPPORTED_HOST_FLASH */