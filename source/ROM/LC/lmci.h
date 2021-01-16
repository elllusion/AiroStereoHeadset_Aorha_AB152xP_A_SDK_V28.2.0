#ifndef _LMCI_H_
#define _LMCI_H_
#include "config.h"
#include "cpu.h"
#include "os.h"

////////////////////////////////////////////////////////////////////////////////
// LM-to-LC Command Definitions ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 LMLC_CMD;

#define LMLC_CMD_CONNECTION_SUCCESS                 (LMLC_CMD)0x01
#define LMLC_CMD_MAX_SLOT_CHANGE                    (LMLC_CMD)0x02
#define LMLC_CMD_ADD_SCO                            (LMLC_CMD)0x03
#define LMLC_CMD_ADD_ESCO                           (LMLC_CMD)0x04
#define LMLC_CMD_REMOVE_SCO_ESCO                    (LMLC_CMD)0x05
#define LMLC_CMD_SNIFF                              (LMLC_CMD)0x06
#define LMLC_CMD_UNSNIFF                            (LMLC_CMD)0x07
#define LMLC_CMD_HOLD                               (LMLC_CMD)0x08
#define LMLC_CMD_SWITCH_ROLE                        (LMLC_CMD)0x09
#define LMLC_CMD_SET_AFH                            (LMLC_CMD)0x0A
#define LMLC_CMD_ENCRYPTION                         (LMLC_CMD)0x0B
#define LMLC_CMD_ACLU                               (LMLC_CMD)0x0C
#define LMLC_CMD_TEST_ACTIVATE                      (LMLC_CMD)0x0D
#define LMLC_CMD_TEST_CONTROL                       (LMLC_CMD)0x0E
#define LMLC_CMD_SNIFF_SUBRATING                    (LMLC_CMD)0x0F
#define LMLC_CMD_DETACH                             (LMLC_CMD)0x10
#define LMLC_CMD_SERVICE_CHANGE                     (LMLC_CMD)0x11
#define LMLC_CMD_QOS_SETUP                          (LMLC_CMD)0x12
#define LMLC_CMD_MASTER_BC_ENCR_SETUP               (LMLC_CMD)0x13
#define LMLC_CMD_AUDIO_SWITCH                       (LMLC_CMD)0x14
#define LMLC_CMD_NORMAL_SEARCH_EXTEND               (LMLC_CMD)0x15
#define LMLC_CMD_BARGE_IN_RXTXWIN_MODIFY_SETUP      (LMLC_CMD)0x16
#define LMLC_CMD_DUMMY_SNIFF_EXIT                   (LMLC_CMD)0x17
#define LMLC_CMD_ENTER_SNIFF_TRANSITION             (LMLC_CMD)0x18
#define LMLC_CMD_EXIT_SNIFF_TRANSITION              (LMLC_CMD)0x19
#define LMLC_CMD_SUPERVISION_TIMEOUT_VALUE          (LMLC_CMD)0x1A
#define LMLC_CMD_ESCO_NULL_PKT_CTL                  (LMLC_CMD)0x1B
#define LMLC_CMD_TUNE_SEARCH_WINDOW                 (LMLC_CMD)0x1C
#define LMLC_CMD_INQUIRY                            (LMLC_CMD)0xF1
#define LMLC_CMD_INQUIRY_CANCEL                     (LMLC_CMD)0xF2
#define LMLC_CMD_PAGE                               (LMLC_CMD)0xF4
#define LMLC_CMD_PAGE_CANCEL                        (LMLC_CMD)0xF5


////////////////////////////////////////////////////////////////////////////////
// LM-to-LC Command Format Definitions /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- OFFSET constant definition
#define LMLC_CMD_OFFSET OS_OFFSET_OF(LMLC_CMD_MSG_STRU,Cmd)

/* Generic LM-to-LC command format */
typedef struct
{
    U8 Opcode;
    U8 Para[1];

} LMLC_CMD_STRU, OS_PTR LMLC_CMD_STRU_PTR;

/* Generic LM-to-LC command message format */
typedef struct
{
    U8 Offset;
    LMLC_CMD_STRU Cmd;

} LMLC_CMD_MSG_STRU, OS_PTR LMLC_CMD_MSG_STRU_PTR;

/* LM-to-LC Page Request Command */
typedef struct
{
    U8                  opcode;
    BD_ADDR_STRU        bd_addr;
    PARITY_ARR          parity;
    U8                  sr;
    U8                  reserved;
    U8                  clk_offset[2];
    U8                  conn_handle;
    CONN_HDL_STRU_PTR   pConnHdl;

} LMLC_CMD_PAGE_STRU, OS_PTR LMLC_CMD_PAGE_STRU_PTR;

/* LM-to-LC Page Request Command Message */
typedef struct
{
    U8 Offset;
    LMLC_CMD_PAGE_STRU Cmd;

} LMLC_CMD_MSG_PAGE_STRU, OS_PTR LMLC_CMD_MSG_PAGE_STRU_PTR;

/* LM-to-LC Inquiry Request Command */
typedef struct
{
    U8              Opcode;
	BD_ADDR_STRU    BdAddr;
	PARITY_ARR      Parity;

} LMLC_CMD_INQUIRY_STRU, OS_PTR LMLC_CMD_INQUIRY_STRU_PTR;

/* LM-to-LC Tune Search Window Command */
typedef struct
{
    U8              Opcode;
	U8              Drift;
	U8              Jitter;

} LMLC_CMD_TUNE_SEARCH_WINDOW_STRU, OS_PTR LMLC_CMD_TUNE_SEARCH_WINDOW_PTR;


/* LM-to-LC QoS Setup Command */
typedef struct
{
    U8      Opcode;
    U8      ServiceType;
    U8      FlowStatus;
    U8      RxFull;

} LMLC_CMD_QOS_STRU, OS_PTR LMLC_CMD_QOS_STRU_PTR;

/* LM-to-LC QoS Setup Command Message */
typedef struct
{
    U8 Offset;
    LMLC_CMD_QOS_STRU Cmd;

} LMLC_CMD_MSG_QOS_STRU, OS_PTR LMLC_CMD_MSG_QOS_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// LC-to-LM Event Format Definitions ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- OFFSET constant definition
#define LCLM_EVT_OFFSET OS_OFFSET_OF(LCLM_EVT_MSG_STRU,Evt)

/* Generic LC-to-LM event format */
typedef struct
{
    U8 EvtCode;
    U8 Para[1];

} LCLM_EVT_STRU, OS_PTR LCLM_EVT_STRU_PTR;

/* Generic LC-to-LM event message format */
typedef struct
{
    U8 Offset;
    LCLM_EVT_STRU Evt;

} LCLM_EVT_MSG_STRU, OS_PTR LCLM_EVT_MSG_STRU_PTR;

/* LC-to-LM Connected Event */
// LC_EVENT_CONNECTION_STATE_ENTERED
typedef struct
{
    U8 EvtCode;
    U8 LinkIdx;
    CONN_HDL_STRU_PTR pConnHdl;

} LCLM_EVT_CONN_STRU, OS_PTR LCLM_EVT_CONN_STRU_PTR;

typedef struct
{
    U8 EvtCode;
    FHS_ARR FHS;
    BTCLK_UNION LatchedNativeClock;
    RSSI Rssi;
    U8 XDATA_PTR pEirMsg;

} LCLM_EVT_INQUIRY_RESP_STRU, OS_PTR LCLM_EVT_INQUIRY_RESP_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// LC-to-LM Event Format Definitions ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//LM RX PDU format: offset len_L len_H TID_OP (TID_OP_EXT) payload1 ...
#if 0
//rx offset: 3
typedef struct
{
    U8 offset; // default 3
    U8 attachedLinkIdx;
    U8 dummy;
    U8 misc;
    U8 lengthB0;
    U8 lengthB1;
    U8 opcodeTid;
    U8 firstPara;
}LclmDataType;
#endif



typedef struct
{
    U8  LLID        : 2;
    U8  NESN        : 1;
    U8  SN          : 1;
    U8  MD          : 1;
    U8  reserved    : 3;

} DATA_CH_PDU_HDR_B0_STRU;

typedef struct
{
    U8  Length;

} DATA_CH_PDU_HDR_B1_STRU;

typedef struct
{
    U8 Opcode;
    U8 CtrData[22];

} LL_CTL_PDU_STRU;

typedef struct
{
    U8 L2capData[31];

} LL_DATA_PDU_STRU;

typedef struct
{
    DATA_CH_PDU_HDR_B0_STRU PduHdrB0;
    DATA_CH_PDU_HDR_B1_STRU PduHdrB1;
    union
    {
        LL_CTL_PDU_STRU LLCP;
        LL_DATA_PDU_STRU LLDP;

    } Payload;

} RX_DATA_CH_PDU_STRU, OS_PTR RX_DATA_CH_PDU_STRU_PTR;

typedef struct
{
    HCI_DATA_HDR_STRU HciHdr;
    LL_DATA_PDU_STRU LLDP;

} RX_L2CAP_40_PDU_STRU, OS_PTR RX_L2CAP_40_PDU_STRU_PTR;


/* 3.0 */
typedef union
{
    U8 value;
    struct
    {
        U8 TID      : 1;
        U8 Opcode   : 7;

    } field;

} OPCODE_TID_STRU;

typedef struct
{
    OPCODE_TID_STRU OpcodeTid;
    U8 Data[16];

} LM_PDU_STRU;

typedef struct
{
    OPCODE_TID_STRU OpcodeTid;
    U8 ExtendedOpcode;
    U8 Data[15];

} EXT_LM_PDU_STRU;

typedef struct
{
    U8 Data[1021];

} L2CAP_30_PDU_STRU;

typedef struct
{
    union
    {
        LM_PDU_STRU LMP;
        EXT_LM_PDU_STRU EXT_LMP;
        L2CAP_30_PDU_STRU L2CAP;

    } Payload;

} RX_BT_30_PDU_STRU, OS_PTR RX_BT_30_PDU_STRU_PTR;

typedef struct
{
    U8 MISC;
    U8 Length;
    union
    {
        LM_PDU_STRU LMP;
        EXT_LM_PDU_STRU EXT_LMP;

    } Payload;

} RX_LMP_PDU_STRU, OS_PTR RX_LMP_PDU_STRU_PTR;

typedef struct
{
    HCI_DATA_HDR_STRU HciHdr;
    union
    {
        L2CAP_30_PDU_STRU L2CAP;
        LL_DATA_PDU_STRU LLDP;

    } Payload;

} RX_L2CAP_PDU_STRU, OS_PTR RX_L2CAP_PDU_STRU_PTR;

typedef struct
{
    U8 next_ptr_B0;
    U8 next_ptr_B1;
    U8 len_B0;
    U8 len_B1;
    U8 Payload[1021];

} RX_LB_L2CAP_PDU_STRU, OS_PTR RX_LB_L2CAP_PDU_STRU_PTR;


typedef struct
{
    U16 Length;
    U8 Payload[240];

} RX_EIR_PDU_STRU, OS_PTR RX_EIR_PDU_STRU_PTR;

typedef struct
{
    U8 Length;
    U8 EirData[1];

} EIR_DATA_STRU, OS_PTR EIR_DATA_STRU_PTR;

#endif /* _LMCI_H_ */

