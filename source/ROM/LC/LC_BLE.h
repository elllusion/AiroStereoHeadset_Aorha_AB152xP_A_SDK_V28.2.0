#ifndef _LC_BLE_H_
#define _LC_BLE_H_
#include "ctlr_config.h"
#if LE_SUPPORTED_CONTROLLER
EXTERN VOID LE_MicCountSubOne(U8 XDATA_PTR pMicCountLE);

U8 LC_LEGetAvailableLinkIdx(void);
void LC_LEReleaseLinkIdx(U8 idx);
U8 LC_LE_CmdHandler(U8 link_idx);


#define LC_BLE_MASTER 0x01
#define LC_BLE_SLAVE 0x00

#define MAX_ADV_LENGTH 37


#define LC_BLE_PUBLIC_TXADDR                    0x00
#define LC_BLE_RANDOM_TXADDR                    0x40

#define LC_BLE_PUBLIC_RXADDR                    0x00
#define LC_BLE_RANDOM_RXADDR                    0x80



//- 4.0 trx len ctl
#define LE_TX_LEN_BASED_ON_SW_TX_LEN            0x02
#define LE_TX_LEN_BASED_ON_PAYLOAD_HEADER       0x00

#define LE_RX_LEN_MODE1                         0x00
#define LE_RX_LEN_MODE2                         0x01

#define LC_SET_ADV_CH_TRX_LEN_SRC()  LC_SFR_WRITE_BY_E6(E6OFFS_RW_SW_TRX_LEN_CTRL, LE_TX_LEN_BASED_ON_PAYLOAD_HEADER | LE_RX_LEN_MODE1);
#define LC_LE_SET_DATA_CH_TRX_LEN_SRC() LC_SFR_WRITE_BY_E6(E6OFFS_RW_SW_TRX_LEN_CTRL, LE_TX_LEN_BASED_ON_PAYLOAD_HEADER | LE_RX_LEN_MODE2);



//========================================= RXDone control (E6,14)
#define LE_RXDONE_IF_CRC_AND_SN_OK              0x00
#define LE_RXDONE_IF_CRC_OK                     0x01
#define LE_RXDONE_IF_RX_COMPLETE                0x02

//========================================= TRX control b0
enum trx_type
{
    RTRT_Mode,
    TRTR_Mode,
    T_Only_Mode,
    R_Only_Mode
};

#define TRX_MODE_MASK      (0x03)
#define FIRST_RX_MODE_MASK (0x0C)

#define RX_BASED_ON_SEARCH_WINDOW 0x00
#define CRX_MODE1 0x04
#define CRX_MODE2 0x08
#define CRX_MODE3 0x0C

#define LE_DEVICE_SYNC_PHASE_WITH_TRANSMITTER 0x20

#define LE_WHITENING_ENABLED            0x80
#define LE_WHITENING_DISABLED           0x00

#define LC_LE_SET_TRX_MODE(A) {U8 trx_mode = LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B0)&(~TRX_MODE_MASK) ; \ 
LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B0,trx_mode|A)}

#define LC_LE_SET_1ST_RX_TYPE(A) {U8 rx_type = LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B0)&(~FIRST_RX_MODE_MASK) ; \ 
LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B0,rx_type|A)}

#define LC_LE_ENABLE_WHITENING() {U8 tmp = LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B0)| LE_WHITENING_ENABLED; \
LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B0, tmp)}
#define LC_LE_DISABLE_WHITENING() {U8 tmp = LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B0) & (~LE_WHITENING_ENABLED); \
LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B0, tmp)}

#define LC_LE_ENABLE_PHASE_SYNC_WITH_TRANSMITTER() {U8 tmp = LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B0)| LE_DEVICE_SYNC_PHASE_WITH_TRANSMITTER; \
LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B0, tmp)}

#define LC_BLE_DISABLE_PHASE_SYNC_WITH_TRANSMITTER() {U8 tmp = LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B0) &(~LE_DEVICE_SYNC_PHASE_WITH_TRANSMITTER); \
LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B0, tmp)}



//========================================= TRX control b1
#define LE_TX_ENABLED           0x01
#define LE_TX_DISABLED          0x02
#define LE_RX_ENABLED           0x04
#define LE_RX_DISABLED          0x08

#define LE_DATA_CHANNEL_MODE    0x00
#define LE_ADV_CHANNEL_MODE     0x10

#define LC_SET_ADV_MODE()   LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_ADV_CHANNEL_MODE);
#define LC_LE_SET_DATA_MODE()  LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_DATA_CHANNEL_MODE);


#define LC_ENABLE_ADV_TRX()     LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_ADV_CHANNEL_MODE | LE_TX_ENABLED | LE_RX_ENABLED );
#define LC_ENABLE_ADV_RX()      LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_ADV_CHANNEL_MODE | LE_RX_ENABLED);
#define LC_DISABLE_ADV_RX()      LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_ADV_CHANNEL_MODE | LE_RX_DISABLED);
#define LE_ENABLE_ADV_2ND_TX()  LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_ADV_CHANNEL_MODE | LE_TX_ENABLED);
#define LE_DISABLE_ADV_2ND_TX() LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_ADV_CHANNEL_MODE | LE_TX_DISABLED);

#define LC_ENABLE_DATA_TRX()    LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_DATA_CHANNEL_MODE | LE_TX_ENABLED | LE_RX_ENABLED);
#define LE_ENABLE_DATA_TX()     LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_DATA_CHANNEL_MODE | LE_TX_ENABLED );
#define LC_LE_ENABLE_DATA_RX()     LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_DATA_CHANNEL_MODE | LE_RX_ENABLED );
#define LC_DISABLE_DATA_TRX()   LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B1, LE_DATA_CHANNEL_MODE | LE_TX_DISABLED | LE_RX_DISABLED);



enum rx_empty_pkt_ctl
{
    Rx_empty_pkt_filtered,
    Rxdone_int_issued_when_receiving_empty    
};

enum slave_rsp_to_data_pkt_ctl
{
    slave_no_auto_rsp_to_data_pkt,
    slave_auto_rsp_to_data_pkt
};

enum master_tx_ctl
{
    master_no_tx_empty_when_no_data,
    master_tx_empty_when_no_data    
};


#define LC_ROLE_MASTER (1)
#define LC_ROLE_SLAVE  (0)

#define LE_ADV_AA_AND_CRCINIT_MASK 0x01
#define LE_HW_CRC_APPEND_MASK      0x10
#define LE_CHANNEL_HOP_CTL_MASK    0x20

#define LE_DEFAULT_ADV_AA_AND_CRCINIT      0x00
#define LE_AA_AND_CRC_INIT_FROM_MEMORY     0x01

#define LE_HW_CRC_APPEND 0x00
#define LE_NO_CRC_APPEND 0x10

#define LE_CHANNEL_HOP_BY_HW                            0x00
#define LE_CHANNEL_SET_BY_SW                            0x20

#define LE_RXDONE_INT_ISSUED_WHEN_RCV_EMPTY_PKT         0x02
#define LE_SLAVE_HW_AUTO_RSP_EMPTY_PKT                  0x08
#define LE_MASTER_TX_EMPTY_WHEN_NO_DATA                 0x40

#define LC_LE_SET_DEFAULT_ADV_AA_AND_CRC() {                                                                          \
    U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B2) & (~LE_ADV_AA_AND_CRCINIT_MASK))|LE_DEFAULT_ADV_AA_AND_CRCINIT; \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B2, tmp)}

#define LC_BLE_SET_HOP_BY_HW() {                                                                             \
    U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B2) & (~LE_CHANNEL_HOP_CTL_MASK)) | LE_CHANNEL_HOP_BY_HW; \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B2, tmp)}

#define LC_LE_SET_HOP_BY_SW() {\
    U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B2) & (~LE_CHANNEL_HOP_CTL_MASK)) | LE_CHANNEL_SET_BY_SW; \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B2, tmp)}

#define LC_BLE_SET_HW_CRC_APPEND() {\
U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B2) & (~LE_HW_CRC_APPEND_MASK)) | LE_HW_CRC_APPEND; \
LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B2, tmp)}

#define LC_LE_ENABLE_RXDONE_INT_WHEN_RECEIVING_EMPTY() {\
    U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B2) & (~LE_RXDONE_INT_ISSUED_WHEN_RCV_EMPTY_PKT)) | LE_RXDONE_INT_ISSUED_WHEN_RCV_EMPTY_PKT; \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B2, tmp)}

#define LC_BLE_ENABLE_SLAVE_AUTO_RSP() {\
    U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B2) & (~LE_SLAVE_HW_AUTO_RSP_EMPTY_PKT)) | LE_SLAVE_HW_AUTO_RSP_EMPTY_PKT; \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B2, tmp)}

#define LC_LE_ENABLE_MASTER_TX_EMPTY_WHEN_NO_DATA() {\
    U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_TRX_CTRL_B2) & (~LE_MASTER_TX_EMPTY_WHEN_NO_DATA)) | LE_MASTER_TX_EMPTY_WHEN_NO_DATA; \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_TRX_CTRL_B2, tmp)}

#define LC_BLE_ENABLE_SEARCH_WINDOW_CTL 0x01
#define LC_BLE_ENABLE_CONNECTION_END_CTL 0x02
#define LC_LE_ENABLE_CONN_END() {\
    U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_SEARCH_CTRL))  | LC_BLE_ENABLE_CONNECTION_END_CTL; \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_SEARCH_CTRL, tmp)}
#define LC_BLE_ENABLE_SEARCH_WINDOW() {\
    U8 tmp = (LC_SFR_READ_BY_E6(E6OFFS_RW_SEARCH_CTRL))  | LC_BLE_ENABLE_SEARCH_WINDOW_CTL; \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_SEARCH_CTRL, tmp)}

#define LC_LE_SET_HOP_CHANNEL_BY_SW(A){ LC_LE_SET_HOP_BY_SW();\    
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_SW_HOP_CHANNEL, A)}
    
#define LC_BLE_GUARD_INT_ENABLE 0x80
#define LC_LE_ENABLE_GUARD(){\
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_SW_ANCHOR_INT_GUARD_IN_8_US, 0x00);\
    LC_SFR_CONT_WRITE_BY_E6(LC_BLE_GUARD_INT_ENABLE | 0x01)}    

#define LC_LE_RESET_TX_STATUS(idx) {\
    U8 tmp = LC_SFR_READ_BY_E7(E7OFFS_RW_BT4_TX_CTRL); \
    LC_SFR_WRITE_BY_E7(E7OFFS_RW_BT4_TX_CTRL,tmp&(0x7<<((idx^1)*3)));\
    LC_SFR_WRITE_BY_E7(E7OFFS_RW_BT4_TX_CTRL+idx+1,0x00);\
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_DATA_PKT_SN_NESN,0x00);}    

#define LC_LE_END_CURRENT_INTERVAL() {\
    U8 tmp = LC_SFR_READ_BY_E6(E6OFFS_RW_DEVICE_CLK_EVENT_CTRL) ; \
    tmp |= BIT_MASK8(3);\
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE_CLK_EVENT_CTRL, tmp)}

#define LC_WAIT_HW_IDLE_STATE(idx)  {\
    while ((LC_SFR_READ_BY_E7(E7OFFS_RW_CURRENT_LINK_CTRL)&(BIT_MASK8(4)-1)) == 12+idx){;}}

#define LC_LE_SET_LATENCY(N)   {\
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_CURRENT_LATENCY_CTRL_B0, 0x1);\
    LC_SFR_CONT_WRITE_BY_E6(0x0);\
    LC_SFR_CONT_WRITE_BY_E6((U8)N);\
    LC_SFR_CONT_WRITE_BY_E6((U8)((U16)N>>8));\
    LL_CONN_CTL(gLC_ctl.lc_le_ctl.event_link_idx).Latency_Period = N;}
#define LC_LE_DISABLE_LATENCY()   LC_LE_SET_LATENCY(1)

#define LC_LE_REFRESH_LINK(){\
    U8 tmp;\
    tmp = LC_SFR_READ_BY_E6(E6OFFS_RW_DEVICE_CLK_EVENT_CTRL);\
    tmp &= ~BIT_MASK8(1);\
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE_CLK_EVENT_CTRL, tmp);\
    tmp |= BIT_MASK8(1);\
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE_CLK_EVENT_CTRL, tmp);\
}

//===============================================   MAX_Search_Window PPM calculation
#define LE_MAX_SEARCH_WINDOW_500PPM                 (U16) 2048
#define LE_MAX_SEARCH_WINDOW_1000PPM                (U16) 1024
#define LE_MAX_SEARCH_WINDOW_2000PPM                (U16) 512
#define LE_MAX_SEARCH_WINDOW_4000PPM                (U16) 256
#define LE_MAX_SEARCH_WINDOW_8000PPM                (U16) 128
//===============================================


//============== BT 4.0 interrupt mask
//- BT 4.0 Interrupt mask
typedef U8 LE_INTR_ID;
#define LC_40_INTE_ID_SLOT_TICK                             ((LE_INTR_ID)(0))
#define LC_40_INTE_ID_ADV_CHANNEL_TXDONE                    ((LE_INTR_ID)(1))
#define LC_40_INTE_ID_ADV_CHANNEL_RXDONE                    ((LE_INTR_ID)(2))
#define LC_40_INTE_ID_DATA_CHANNEL_TXDONE                   ((LE_INTR_ID)(3))
#define LC_40_INTE_ID_DATA_CHANNEL_RXDONE                   ((LE_INTR_ID)(4))
#define LC_40_INTE_ID_AC_SYNC                               ((LE_INTR_ID)(5))
#define LC_40_INTE_ID_GUARD                                 ((LE_INTR_ID)(6))
#define LC_40_INTE_ID_RX_TIMEOUT                            ((LE_INTR_ID)(7))
#define LC_40_INTE_ID_RX_NAK                                ((LE_INTR_ID)(8))
#define LC_40_INTE_ID_SUPERVISION_TO                        ((LE_INTR_ID)(9))
#define LC_40_INTE_ID_CONN_END                              ((LE_INTR_ID)(10))
#define LC_40_INTE_ID_MIC_ERROR                             ((LE_INTR_ID)(11))
#define LC_40_INTE_ID_EVENT_COUNT_MATCH                     ((LE_INTR_ID)(12))
#define LC_40_INTE_ID_TX_PACKET_COMPLETE                    ((LE_INTR_ID)(13))

typedef U16 LE_INTR_MASK;
typedef LE_INTR_MASK LE_INTR_STATUS;
#define LC_40_INTERRUPT_ENABLE_SLOT_TICK                    ((LE_INTR_MASK)BIT_MASK16(0))
#define LC_40_INTERRUPT_ENABLE_ADV_CHANNEL_TXDONE           ((LE_INTR_MASK)BIT_MASK16(1))
#define LC_40_INTERRUPT_ENABLE_ADV_CHANNEL_RXDONE           ((LE_INTR_MASK)BIT_MASK16(2))
#define LC_40_INTERRUPT_ENABLE_DATA_CHANNEL_TXDONE          ((LE_INTR_MASK)BIT_MASK16(3))
#define LC_40_INTERRUPT_ENABLE_DATA_CHANNEL_RXDONE          ((LE_INTR_MASK)BIT_MASK16(4))
#define LC_40_INTERRUPT_ENABLE_AC_SYNC                      ((LE_INTR_MASK)BIT_MASK16(5))
#define LC_40_INTERRUPT_ENABLE_GUARD                        ((LE_INTR_MASK)BIT_MASK16(6))
#define LC_40_INTERRUPT_ENABLE_RX_TIMEOUT                   ((LE_INTR_MASK)BIT_MASK16(7))
#define LC_40_INTERRUPT_ENABLE_TX_NAK                       ((LE_INTR_MASK)BIT_MASK16(8))
#define LC_40_INTERRUPT_ENABLE_SUPERVISION_TO               ((LE_INTR_MASK)BIT_MASK16(9))
#define LC_40_INTERRUPT_ENABLE_CONN_END                     ((LE_INTR_MASK)BIT_MASK16(10))
#define LC_40_INTERRUPT_ENABLE_MIC_ERROR                    ((LE_INTR_MASK)BIT_MASK16(11))
#define LC_40_INTERRUPT_ENABLE_EVENT_COUNT_MATCH            ((LE_INTR_MASK)BIT_MASK16(12))
#define LC_40_INTERRUPT_ENABLE_TX_PACKET_COMPLETE           ((LE_INTR_MASK)BIT_MASK16(13))


//============== BLE 4.0 interrupt control
/**
 * LC_LE_DEVICE_SELECT
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 *
 * @d : The device ID
 */
#define LC_LE_DEVICE_SELECT(d) { do {                                          \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE_IDX,(d));                               \
    } while (0); }

VOID LC_LE_ENABLE_DEVICE_INTERRUPT(U8 device_idx, U16 mask);
VOID LC_LE_ENABLE_DEVICE_INTERRUPT2(U8 device_idx, U16 mask);
VOID LC_LE_DISABLE_DEVICE_INTERRUPT(U8 device_idx, U16 mask);
VOID LC_LE_DISABLE_ALL_DEVICE_INTERRUPT(U8 device_idx);



/**
 * LC_LE_READ_DEVICEx_INTR_STATUS
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 *
 * @d : The device ID
 * @m : The interrupt mask
 */
//#define LC_LE_READ_DEVICE0_INTR_STATUS() (SFR_RW_OFFSET = E6OFFS_RW_DEVICE0_INT_STATUS_B0, SFR_RW_E6 + (U16)(SFR_RW_E6 << 8))
//#define LC_LE_READ_DEVICE1_INTR_STATUS() (SFR_RW_OFFSET = E6OFFS_RW_DEVICE1_INT_STATUS_B0, SFR_RW_E6 + (U16)(SFR_RW_E6 << 8))
#define LC_LE_READ_DEVICE_INTR_STATUS(N,V) { \
    U8 HI,LO; \
    SFR_RW_OFFSET = E6OFFS_RW_DEVICE0_INT_STATUS_B0+N*4; \
    LO = SFR_RW_E6; \
    HI = SFR_RW_E6; \
    V = ((U16)HI << 8)|((U16)LO); }
#define LC_LE_READ_DEVICE_INTR_BIT_STATUS(N,BITNUM)   (SFR_RW_OFFSET = (E6OFFS_RW_DEVICE0_INT_STATUS_B0+N*4+BITNUM/8), (SFR_RW_E6&BIT_MASK8(BITNUM&7)))                                             
#define LC_LE_CLR_DEVICE_INTR_STATUS(N,BITNUM)   (SFR_RW_OFFSET = (E6OFFS_RW_DEVICE0_INT_STATUS_B0+N*4+BITNUM/8), (SFR_RW_E6 = BIT_MASK8(BITNUM&7)))   


#define LL_DATA_PDU_CONTI_L2CAP     0x01
#define LL_DATA_PDU_START_L2CAP     0x02
#define LL_CONTROL_PDU              0x03


//================== LL control PDU opcode =======================================//
#define LL_CONNECTION_UPDATE_REQ    0x00
#define LL_CHANNEL_MAP_REQ          0x01
#define LL_TERMINATE_IND            0x02
#define LL_ENC_REQ                  0x03
#define LL_ENC_RSP                  0x04
#define LL_START_ENC_REQ            0x05
#define LL_START_ENC_RSP            0x06
#define LL_UNKNOWN_RSP              0x07
#define LL_FEATURE_REQ              0x08
#define LL_FEATURE_RSP              0x09
#define LL_PAUSE_ENC_REQ            0x0A
#define LL_PAUSE_ENC_RSP            0x0B
#define LL_VERSION_IND              0x0C
#define LL_REJECT_IND               0x0D
#define LL_SLAVE_FEATURE_REQ        0x0E
#define LL_CONNECTION_PARAM_REQ     0x0F
#define LL_CONNECTION_PARAM_RSP     0x10
#define LL_REJECT_IND_EXT           0x11
#define LL_PING_REQ                 0x12
#define LL_PING_RSP                 0x13
#define LL_LENGTH_REQ               0x14
#define LL_LENGTH_RSP               0x15

enum Adv_Channel_Pdu_Type
{
    ADV_IND_PDU,
    ADV_DIRECT_IND_PDU,
    ADV_NONCONN_IND_PDU,
    SCAN_REQ_PDU,
    SCAN_RSP_PDU,
    CONNECT_REQ_PDU,
    ADV_SCAN_IND_PDU,
};


typedef union
{
    U8 member[31];
    U8 InitA[6];
}ADV_DATA;

typedef struct
{
    U8 member[31];
}SCAN_RSP_DATA;

typedef struct
{
    BD_ADDR_STRU ScanA;
    BD_ADDR_STRU AdvA;
}SCAN_REQ;

typedef struct
{
    BD_ADDR_STRU AdvA;
    SCAN_RSP_DATA ScanRspData;
}SCAN_RSP;

typedef union
{
    SCAN_REQ ScanReq;
    SCAN_RSP ScanRsp;
}SCANNING_PDU;


typedef struct
{
    ACCESS_ADDR_ARR AA;
    LE_CRC_ARR CrcInit;
    U8 WinSize;
    U8 WinOffsetB0;
    U8 WinOffsetB1;
    U8 IntervalB0;
    U8 IntervalB1;
    U8 LatencyB0;
    U8 LatencyB1;
    U8 TimeoutB0;
    U8 TimeoutB1;
    LE_CH_MAP_ARR ChMap;
    U8 HopSca;
}LL_DATA_FIELD;

typedef union
{
    U8 member[22];
    LL_DATA_FIELD LlDataField;
}LL_DATA;

typedef struct
{
    BD_ADDR_STRU InitA;
    BD_ADDR_STRU AdvA;
    LL_DATA LlData;
}CONN_REQ;

typedef union
{
    CONN_REQ ConnReq;
}INITIATING_PDU;


typedef struct
{
    BD_ADDR_STRU AdvA;
    ADV_DATA AdvData;
}ADVERTISING_PDU;

typedef union
{
    ADVERTISING_PDU adv_pdu;
    SCANNING_PDU scan_pdu;
    INITIATING_PDU init_pdu;
}ADV_CHANNEL_PDU_PAYLOAD;

typedef struct
{
    U8 adv_pdu_type:4;
    U8 reserved:2;
    U8 TxAddr:1;
    U8 RxAddr:1;
}ADV_CHANNEL_PDU_HDR_B0;

typedef struct
{
    U8 length:6;
    U8 reserved:2;
}ADV_CHANNEL_PDU_HDR_B1;

typedef struct
{
    ADV_CHANNEL_PDU_HDR_B0 b0;
    ADV_CHANNEL_PDU_HDR_B1 b1;
}ADV_CHANNEL_PDU_HDR;


typedef struct
{
    ADV_CHANNEL_PDU_HDR header;
    ADV_CHANNEL_PDU_PAYLOAD payload;
}ADV_CHANNEL_PDU;


enum Data_Pdu_llid
{
    Conti_L2cap_Pdu = 0x01,
    Start_L2cap_Pdu,
    Control_Pdu
};


#endif
#endif
