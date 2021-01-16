#ifndef _LC_BLEDEVICECTL_H_
#define _LC_BLEDEVICECTL_H_

#include "os.h"
#include "hci_conn_hdl.h"

typedef struct
{
    U8 channel_map[5];
}DATA_CHANNEL_MAP;

typedef struct
{
    U8 rsvd1[4];
    U8 rsvd2 :5;
    U8 channel_map :3;
}ADV_CHANNEL_MAP;

typedef union
{
    DATA_CHANNEL_MAP data_channel;
    ADV_CHANNEL_MAP adv_channel;
}BLE_CHANNEL_MAP;


typedef struct
{
    U8 rx_enc_en :1;
    U8 tx_enc_en :1;
    U8 reserved  :6;
}BLE_ENC_CTL;

typedef struct
{
    U8     access_addr[4];                //- 0~3
    U8     crc_init[3];                   //- 4~6
    U8                  hop_increment;              //- 7
    BLE_CHANNEL_MAP     channel_map;                //- 8~12
    BLE_ENC_CTL         enc_ctl;                    //- 13
    U8                  supervision_timeout_b0;     //- 14
    U8                  supervision_timeout_b1;     //- 15
    U8                  tx_ptr[2];                  //- 16~17
    U8                  reserved1[2];               //- 18~19
    U8                  tx_gc[2];                   //- 20~21
    U8                  tx_bias[2];                 //- 22~23
    U8                  init_vector[8];             //- 24~31
    U8                  encryption_key[16];         //- 32~47
    U8                  mic_tx_pkt_cnt[5];          //- 48~52
    U8                  reserved_1[3];              //- 53~55
    U8                  mic_rx_pkt_cnt[5];          //- 56~60
    U8                  reserved_2[3];              //- 61~63
    U8                  unmapped_channel;           //- 64
    U8                  reserved_3;                 //- 65
    U8                  connection_event_cnt_b0;    //- 66
    U8                  connection_event_cnt_b1;    //- 67
} LC_BLE_DEVICE_SW_HW_SHARE_CTL;


#define MAX_LE_DEFAULT_DATA_SEG_LEN             27



#define MIC_LEN         4
typedef struct
{
    U8 max_seg_len;
    U8 seg_len;
    U8 XDATA_PTR return_ptr;
    #ifdef LE_TX_SPEEDUP_SUPPORTED    
    U8 XDATA_PTR next_return_ptr;    
    #endif
    U8 XDATA_PTR remain_pkt_ptr;
    #ifndef LE_TX_SPEEDUP_SUPPORTED
    U8 XDATA_PTR seg_mem_ptr;
    #endif
    U16 remain_pkt_len;
    U16 compl_pkt_cnt;

} LC_LE_DATA_TRANSPORT_STRU, OS_PTR LC_LE_DATA_TRANSPORT_STRU_PTR;

typedef union
{
    struct
    {
        U8  adv_pdu_type        :4;
        U8  reserved            :2;
        U8  TxAddr              :1;
        U8  RxAddr              :1;
    }adv_type;

    struct
    {
        U8  llid                :2;
        U8  NESN                :1;
        U8  SN                  :1;
        U8  MD                  :1;
        U8  reserved            :3;
    }data_type;

    U8  member;
} LC_LE_PACKET_HEADER_B0;


/*
typedef union
{
    struct
    {
        U8  length              :6;
        U8  reserved            :2;

    }adv_type;

    struct
    {
        U8  length              :5;
        U8  reserved            :3;

    }data_type;
    U8 member;
} LC_LE_PACKET_HEADER_B1;
*/


typedef union
{
    U8 length;
} LC_LE_PACKET_HEADER_B1;

typedef struct
{
    U8 offset;
    U8 need_acked;
}ControlPktAppendInfoStru;

typedef struct
{
    U8 XDATA_PTR return_ptr;
}L2capPktAppendInfoStru;

typedef union
{
    ControlPktAppendInfoStru Control_pkt;
    L2capPktAppendInfoStru L2cap_pkt;
}APPENDED_TX_PACKET_INFO;


typedef struct
{
    U8 opcode;
}CtrlPktPayloadStru;

typedef struct
{
    U8 first_para;
}L2capPktPayloadStru;

typedef union
{
    CtrlPktPayloadStru Control_pkt;
    L2capPktPayloadStru L2cap_pkt;
}TxPktPayload;



typedef struct
{
    APPENDED_TX_PACKET_INFO tx_pkt_info;
    LC_LE_PACKET_HEADER_B0  tx_payload_header_b0;
    LC_LE_PACKET_HEADER_B1  tx_payload_header_b1;
    TxPktPayload tx_payload;

} LC_TX_DATA_PKT_STRU, OS_PTR LC_TX_DATA_PKT_STRU_PTR;

typedef struct
{


    U8 rx_freq;
    U8 tx_freq;
    U8 len_of_test_data;
    U8 packet_payload;
} LC_LE_TEST_PARAM;


typedef struct
{
    U8 connection_to_be_established      :1;
    U8 connection_established            :1;
    U8 connection_updated                :1;
    U8 terminated_by_remote              :1;
    U8 terminated_by_remote_acked        :1;
    U8 terminated_by_remote_ready        :1;
    U8 terminated_by_local               :1;
    U8 update_req                        :1;
}CONN_BIT_CTL;
typedef struct
{
    U8 ac_sync_only_for_rssi             :1;
    U8 rserved                           :7;

}CONN_BIT_CTL2;

typedef struct
{
    U8 chM_update_evt_cnt_match    :1;
    U8 chM_updating                :1;
    U8 chM_to_be_updated           :1;
    U8 send_ll_chM_updated_evt     :1;
    U8 reserved                    :4;
}CHM_UPDATE_BIT_CTL;

typedef struct
{
    CHM_UPDATE_BIT_CTL      chM_update_bit_ctl;
    TWO_BYTE_UNION_TYPE     chM_update_instant;
    U8 chM_update_tick_counter;

}CHANNEL_MAP_UPDATE_CTL;

typedef struct
{
    U8 conn_updating             :1;
    U8 conn_to_be_updated        :1;
    U8 conn_para_changed         :1;
    U8 send_ll_conn_updated_evt  :1;
    U8 timeout_to_be_updated     :1;
    U8 reserved                  :3;
}CONN_UPDATE_BIT_CTL;

typedef struct
{
    U8 offset[6];
    U8 Payload_Header_b0;
    U8 Payload_Header_b1;
    U8 Opcode;
    U8 WinSize;
    U8 WinOffset[2];
    U8 ConnInterval[2];
    U8 ConnSlaveLatency[2];
    U8 ConnTimeout[2];
    U8 ConnInstant[2];
}RX_CONN_UPDATE_REQ_STRU;
typedef struct
{
    U8 offset[2];
    U8 Payload_Header_b0;
    U8 Payload_Header_b1;
    U8 Opcode;
    U8 WinSize;
    U8 WinOffset[2];
    U8 ConnInterval[2];
    U8 ConnSlaveLatency[2];
    U8 ConnTimeout[2];
    U8 ConnInstant[2];
}TX_CONN_UPDATE_REQ_STRU;
typedef struct
{
    U8 Payload_Header_b0;
    U8 Payload_Header_b1;
    U8 Opcode;
    U8 WinSize;
    U8 WinOffset[2];
    U8 ConnInterval[2];
    U8 ConnSlaveLatency[2];
    U8 ConnTimeout[2];
    U8 ConnInstant[2];
}CONN_UPDATE_REQ_DATA_STRU;
typedef union
{
    U8 member;
    TX_CONN_UPDATE_REQ_STRU TxPtr;
    RX_CONN_UPDATE_REQ_STRU RxPtr;
}CONN_UPDATE_REQ_STRU;
typedef struct
{
    CONN_UPDATE_REQ_STRU XDATA_PTR conn_update_pdu_ptr;
    CONN_UPDATE_BIT_CTL     conn_update_bit_ctl;
    TWO_BYTE_UNION_TYPE     conn_update_instant;
    U8 collision_cnt;
    OSMQ OSMQ_Queued_ConnUpdateLLCP;
}CONN_UPDATE_CTL;


typedef struct
{
    U8  Tx_paused       :1;
    U8  Tx_L2cap_paused :1;
    U8  reserved        :6;

}LC_TX_CTL_B0;

typedef struct
{
    LC_TX_CTL_B0 tx_ctl_b0;
    LC_LE_DATA_TRANSPORT_STRU le_transport_ctl;

}LC_TX_CTL;

typedef struct
{
    U8 AAGC_rssi;
    U8 AAGC_cs;
}LC_BLE_RSSI_CTL;

typedef struct
{
    U8 role;
    U8             access_address[4];                  //- 4 bytes             /////////////////
    U8             crc_init[3];                        //- 3 bytes             /////////////////    Same as the content of connection request
    U8                          window_size;                     //- 1 byte              /////////////////
    TWO_BYTE_UNION_TYPE         window_offset;                   //- 2 bytes             /////////////////
    TWO_BYTE_UNION_TYPE         conn_interval;                   //- 2 bytes             /////////////////
    TWO_BYTE_UNION_TYPE         conn_latency;                    //- 2 bytes             /////////////////
    TWO_BYTE_UNION_TYPE         conn_supervision_timeout;        //- 2 bytes             /////////////////
    U8                          channel_map[5];                  //- 5 bytes             /////////////////
    U8                          hop_and_SCA;                     //- 1 byte              /////////////////
    CONN_BIT_CTL                conn_bit_ctl;
    CONN_BIT_CTL2               conn_bit_ctl2;
    TWO_BYTE_UNION_TYPE         connection_loss_cnt;
    U16                         conn_loss_threshold;

    CHANNEL_MAP_UPDATE_CTL      chM_update_ctl;
    CONN_UPDATE_CTL             conn_update_ctl;
    LC_TX_DATA_PKT_STRU XDATA_PTR  tx_data_ptr;
    #ifdef LE_TX_SPEEDUP_SUPPORTED
    LC_TX_DATA_PKT_STRU XDATA_PTR  next_tx_l2cap_ptr;
    #endif
    #ifdef LE_TX_MIC_WORKAROUND
    LC_TX_DATA_PKT_STRU XDATA_PTR  paused_tx_data_ptr;
    #endif
    OST XDATA_PTR               guard_protect_timer;
    LC_TX_CTL                   lc_tx_ctl;
    CONN_HDL_STRU_PTR   ConnHdlPtr;
    U8                          detach_reason;
    LC_BLE_RSSI_CTL             rssi_ctl;
    U16                         Latency_Period;
    U8                          ACSync_flag;

}LC_BLE_DEVICE_CTL;

typedef struct
{
    U8 device0 :1;
    U8 device1 :1;
    U8 rsvd :6;
}LE_DEVICE_STATUS;

typedef struct
{
    U8                  event_link_idx;
    BD_ADDR             AdvA;
    LE_DEVICE_STATUS    device_status;

}LC_LE_CTL;

#endif
