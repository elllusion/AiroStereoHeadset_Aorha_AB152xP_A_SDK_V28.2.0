#ifndef _LC_ADVERTISING_H_
#define _LC_ADVERTISING_H_
#include "ctlr_config.h"
#if (LE_SUPPORTED_CONTROLLER)
#include "LC_BLE.h"

enum Advertising_Filter_Policy
{
    ALL_DEVICES,
    SCAN_REQ_FROM_DEVICES_ON_WHITE_LIST,
    CONN_REQ_FROM_DEVICES_ON_WHITE_LIST,
    DEVICES_ON_WHITE_LIST    
};

enum Advertising_Type
{
    ADV_IND,
    ADV_DIRECT_IND_HIGH_DUTY_CYCLE,
    ADV_SCAN_IND,
    ADV_NONCONN_IND,
    ADV_DIRECT_IND_LOW_DUTY_CYCLE    
};

#define LC_BLE_PUBLIC_TXADDR                    0x00
#define LC_BLE_RANDOM_TXADDR                    0x40

#define LC_BLE_PUBLIC_RXADDR                    0x00
#define LC_BLE_RANDOM_RXADDR                    0x80


#define ADV_ENABLED 0x01
#define ADV_DISABLED 0x00

#define LC_ENABLE_ADV_TX 0x01
#define LC_DISABLE_ADV_TX 0x00

#define ADVERTISING_CH37 0x01
#define ADVERTISING_CH38 0x02
#define ADVERTISING_CH39 0x04
#define ADVERTISING_ALL_CHANNELS 0x07


#define LC_CONNECTABLE_DIRECTED_ADV_TIMEOUT 341
#define MAX_ADV_INTERVAL_FOR_HDC_ADV 6
#define MAX_ADV_INTERVAL_FOR_LDC_ADV 16

#define ENABLE_CLK_AND_PHASE_LATCH_AT_TXDONE 0x01
#define ENABLE_CLK_AND_PHASE_LATCH_AT_RXDONE 0x02

#define LC_LE_ENABLE_CLKN_LATCH_AT_RXDONE() LC_SFR_WRITE_BY_E6(E6OFFS_RW_ADV_LATCH_CLKN_CTL, ENABLE_CLK_AND_PHASE_LATCH_AT_RXDONE)
#define LC_LE_ENABLE_CLKN_LATCH_AT_TXDONE() LC_SFR_WRITE_BY_E6(E6OFFS_RW_ADV_LATCH_CLKN_CTL, ENABLE_CLK_AND_PHASE_LATCH_AT_TXDONE)



typedef struct
{
    U8 adv_enabled;
    U8 conn_req_received;
    U8 adv_idx;
    U8 adv_channel_map;
    U8 curr_adv_channel_map_when_eSco;
    CONN_HDL_STRU XDATA_PTR ConnHdlPtr;   
    ADV_CHANNEL_PDU XDATA_PTR adv_tx_data_buffer_ptr;
    ADV_CHANNEL_PDU XDATA_PTR pScanResp;
    U16 AdvEventCounter;
}LC_ADV_CTL;

typedef struct
{
    U8 scan_idx;
    U8 scan_enabled;
    U8 scan_type;
    U8 curr_scan_channel_idx;
    U8 backoffCount;
    U16 upperLimit;
    ADV_CHANNEL_PDU XDATA_PTR ScanReqPtr;
    U8 last_Waiting_ScanRsp_Status;
    U8 WaitingforScanRsp;
    U8 WaitingforScanRspTickCnt;
}LC_BLE_SCAN_CTL;
typedef struct
{
    U8 init_enabled;
    U8 init_idx;
    U8 curr_init_channel_idx;
    CONN_HDL_STRU_PTR ConnHdlPtr;
    LL_CONN_REQ_PDU_STRU_PTR ConnReqPduPtr;
    HCI_CMD_PARA_LE_CREATE_CONN_STRU_PTR CreareConnParaPtr;    
}LC_INIT_CTL;
#define LC_CONNECTION_ENTERED 0x00
#define LC_DIRECT_ADV_TIMEOUT 0x01
#define LC_ADV_DISABLED_BY_HOST 0x02




#endif
#endif
