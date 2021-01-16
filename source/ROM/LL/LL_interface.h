#ifndef _LL_INTERFACE_H_
#define _LL_INTERFACE_H_
#include "config.h"
#include "ctlr_config.h"
#if (LE_SUPPORTED_CONTROLLER)



#include "cpu.h"
#include "..\LC\LC_interface.h"
#include "..\HC\HC_interface.h"

#include "..\LC\LC.h"
#include "LL_semaphore.h"
#include "hci_conn_hdl.h"
#include "LC_BLE.h"
#ifndef AB1520D
#include "..\..\sector\sector_le_misc_ctl.h"
#endif
#define LL_PROC_TIMER   128000  //40s unit:half slot(312.5us)




typedef struct
{
    U8 PDU_Type:4;
    U8 RFU:2;
    U8 TxAdd:1;
    U8 RxAdd:1;
    U8 Length:6;
    U8 RFU2:2;

} LL_ADV_HEADER_STRU;

typedef struct
{
    U8 AA[4];
    U8 CRCInit[3];
    U8 WinSize;
    U8 WinOffset[2];
    U8 Interval[2];
    U8 Latency[2];
    U8 Timeout[2];
    U8 ChM[5];
    U8 Hop:5;
    U8 SCA:3;

} ADV_CONN_REQ_LL_DATA_STRU, LC_PTR ADV_CONN_REQ_LL_DATA_STRU_PTR;

typedef struct
{
    BD_ADDR_STRU AdvA;
    U8 AdvData[31];

} ADV_PAYLOAD_ADV_IND_STRU;

typedef struct
{
    BD_ADDR_STRU AdvA;
    BD_ADDR_STRU InitA;

} ADV_PAYLOAD_ADV_DIRECT_IND_STRU;

typedef struct
{
    BD_ADDR_STRU AdvA;
    U8 AdvData[31];

} ADV_PAYLOAD_ADV_NONCONN_IND_STRU;

typedef struct
{
    BD_ADDR_STRU AdvA;
    U8 AdvData[31];

} ADV_PAYLOAD_SCAN_IND_STRU;

typedef struct
{
    BD_ADDR_STRU ScanA;
    BD_ADDR_STRU AdvA;

} ADV_PAYLOAD_SCAN_REQ_STRU;

typedef struct
{
    BD_ADDR_STRU AdvA;
    U8 ScanRspData[31];

} ADV_PAYLOAD_SCAN_RSP_STRU;

typedef struct
{
    BD_ADDR_STRU InitA;
    BD_ADDR_STRU AdvA;
    ADV_CONN_REQ_LL_DATA_STRU LLData;

} ADV_PAYLOAD_CONN_REQ_STRU;

typedef union
{
    ADV_PAYLOAD_ADV_IND_STRU            AdvInd;
    ADV_PAYLOAD_ADV_DIRECT_IND_STRU     AdvDirectInd;
    ADV_PAYLOAD_ADV_NONCONN_IND_STRU    AdvNonConnInd;
    ADV_PAYLOAD_SCAN_IND_STRU           ScanInd;
    ADV_PAYLOAD_SCAN_REQ_STRU           ScanReq;
    ADV_PAYLOAD_SCAN_RSP_STRU           ScanRsp;
    ADV_PAYLOAD_CONN_REQ_STRU           ConnReq;

} LL_ADV_PAYLOAD_UNION;

typedef struct
{
    LL_ADV_HEADER_STRU Header;
    LL_ADV_PAYLOAD_UNION Payload;

} LL_ADV_PDU_STRU, OS_PTR LL_ADV_PDU_STRU_PTR;


typedef struct
{
    U8              offset[6];
    LL_ADV_PDU_STRU pdu;

}LcllConnReqPduType;

typedef struct
{
    U8              offset[6];
    LL_ADV_PDU_STRU pdu;

}LcllAdvReportPduType;




typedef struct
{
    LcllConnReqPduType XDATA_PTR    ConnReqPtr;
    U8 Reason;
}LC_EVT_CONN_COMPLETE_PARAM_STRU;

typedef struct
{
    U8 AAGC_cs;
    U8 AAGC_rssi;
    LcllAdvReportPduType XDATA_PTR pAdvReport;
 }LC_EVT_ADV_REPORT_PARAM_STRU;

typedef struct
{
    U8 Reason;
    U8 NocpCnt;
}LC_EVT_LINK_RELEASE_PARAM_STRU;

typedef struct
{
    U8 Opcode;
}LC_EVT_LL_PDU_ACKED_PARAM;

typedef struct
{
    U8 ConnInterval[2];
    U8 ConnLatency[2];
    U8 ConnTimeout[2];
}LC_EVT_CONNECTION_UPDATE_PARAM;

typedef struct
{
    U8 opcode;
}LC_EVT_COLLISION_OCCURS_PARAM;

typedef struct
{
    U8  lc_link_idx;
    U8  opcode;
}LL_CMD_HDR_STRU;
typedef struct
{
    U8  lc_link_idx;
    U8  opcode;
}LC_EVT_HDR_STRU;

typedef struct
{
    LC_EVT_HDR_STRU  Hdr;
    union
    {
        LC_EVT_ADV_REPORT_PARAM_STRU AdvReport;
        LC_EVT_CONN_COMPLETE_PARAM_STRU ConnCompl;
        LC_EVT_LINK_RELEASE_PARAM_STRU LinkReleased;
        LC_EVT_LL_PDU_ACKED_PARAM LlPduAcked;
        LC_EVT_CONNECTION_UPDATE_PARAM ConnUpdated;
        LC_EVT_COLLISION_OCCURS_PARAM Collision;
    } LcEvtParam;

} LC_EVT_STRU, OS_PTR LC_EVT_STRU_PTR;




typedef struct
{
    U8 offset[6];
    LL_ADV_HEADER_STRU Header;
    ADV_PAYLOAD_CONN_REQ_STRU Payload;

} LL_CONN_REQ_PDU_STRU, OS_PTR LL_CONN_REQ_PDU_STRU_PTR;


typedef struct
{
    U8 WinSize;
    U8 WinOffset[2];
    U8 ConnInterval[2];
    U8 ConnLatency[2];
    U8 SuperviTo[2];
    U8 Instant[2];

} LC_LMP_DATA_CONNECTION_UPDATE_REQ_STRU, OS_PTR LC_LMP_DATA_CONNECTION_UPDATE_REQ_STRU_PTR;




/////////////////////////////////// definition of LL Ctrl Data /////////////////////////////////

typedef struct
{
    U8 WinSize;
    U8 WinOffset[2];
    U8 ConnInterval[2];
    U8 ConnLatency[2];
    U8 SuperviTo[2];
    U8 Instant[2];
}LL_CTRDATA_CONNECTION_UPDATE_REQ_STRU;

typedef struct
{
    U8 ChM[5];
    U8 Instant[2];
}LL_CTRDATA_CHANNEL_MAP_REQ_STRU;
typedef struct
{
    U8 ErrCode;
}LL_CTRDATA_TERMINATE_IND_STRU;
typedef struct
{
    U8 Rand[8];
    U8 EDIV[2];
    U8 SKDm[8];
    U8 IVm[4];
}LL_CTRDATA_ENC_REQ_STRU;
typedef struct
{
    U8 SKDs[8];
    U8 IVs[4];
}LL_CTRDATA_ENC_RSP_STRU;
typedef struct
{
    U8 UnknownType[1];
}LL_CTRDATA_UNKNOWN_RSP_STRU;

typedef struct
{
    U8 FeatureSet[8];
}LL_CTRDATA_FEATURE_REQ_STRU;
typedef struct
{
    U8 FeatureSet[8];
}LL_CTRDATA_FEATURE_RSP_STRU;
typedef struct
{
    U8 VersNr;
    U8 Compld[2];
    U8 SubVersNr[2];
}LL_CTRDATA_VERSION_IND_STRU;
typedef struct
{
    U8 ErrCode;
}LL_CTRDATA_REJECT_IND_STRU;

typedef struct
{
	U8 FeatureSet[8];
}LL_CTRDATA_SLAVE_FEATURE_REQ_STRU;


typedef struct
{
    U8 IntervalMin[2];
    U8 IntervalMax[2];
    U8 Latency[2];
    U8 Timeout[2];
    U8 PreferredPeriodicity;
    U8 RefConnEventCnt[2];
    U8 Offset0[2];
    U8 Offset1[2];
    U8 Offset2[2];
    U8 Offset3[2];
    U8 Offset4[2];
    U8 Offset5[2];
}LL_CTRDATA_CONNECTION_PARAM_REQ_STRU;


typedef struct
{
    U8 IntervalMin[2];
    U8 IntervalMax[2];
    U8 Latency[2];
    U8 Timeout[2];
    U8 PreferredPeriodicity;
    U8 RefConnEventCnt[2];
    U8 Offset0[2];
    U8 Offset1[2];
    U8 Offset2[2];
    U8 Offset3[2];
    U8 Offset4[2];
    U8 Offset5[2];
}LL_CTRDATA_CONNECTION_PARAM_RSP_STRU;


typedef struct
{
    U8 RejectOpcode;
    U8 ErrCode;
}LL_CTRDATA_REJECT_IND_EXT_STRU;

typedef struct
{
    U8 MaxRxOctetsB0;
    U8 MaxRxOctetsB1;    
    U8 MaxRxTimeB0;
    U8 MaxRxTimeB1;    
    U8 MaxTxOctetsB0;
    U8 MaxTxOctetsB1;    
    U8 MaxTxTimeB0;
    U8 MaxTxTimeB1;    
}LL_CTRDATA_LENGTH_REQ_STRU, OS_PTR LL_CTRDATA_LENGTH_REQ_STRU_PTR;


typedef struct
{
    U8 MaxRxOctetsB0;
    U8 MaxRxOctetsB1;    
    U8 MaxRxTimeB0;
    U8 MaxRxTimeB1;    
    U8 MaxTxOctetsB0;
    U8 MaxTxOctetsB1;    
    U8 MaxTxTimeB0;
    U8 MaxTxTimeB1;    
}LL_CTRDATA_LENGTH_RSP_STRU;


typedef union
{
    LL_CTRDATA_CONNECTION_UPDATE_REQ_STRU   ConnUpdateReq;
    LL_CTRDATA_CHANNEL_MAP_REQ_STRU         ChMapReq;
    LL_CTRDATA_TERMINATE_IND_STRU           TermInd;
    LL_CTRDATA_ENC_REQ_STRU                 EncReq;
    LL_CTRDATA_ENC_RSP_STRU                 EncRsp;
    LL_CTRDATA_UNKNOWN_RSP_STRU             UnknownRsp;
    LL_CTRDATA_FEATURE_REQ_STRU             FEAReq;
    LL_CTRDATA_FEATURE_RSP_STRU             FEARsp;
    LL_CTRDATA_VERSION_IND_STRU             VersInd;
    LL_CTRDATA_REJECT_IND_STRU              RejectInd;
    LL_CTRDATA_SLAVE_FEATURE_REQ_STRU       SlaveFEAReq;
    LL_CTRDATA_CONNECTION_PARAM_REQ_STRU    ConnParamReq;
    LL_CTRDATA_CONNECTION_PARAM_RSP_STRU    ConnParamRsp;
    LL_CTRDATA_REJECT_IND_EXT_STRU          RejectIndExt;
    LL_CTRDATA_LENGTH_REQ_STRU              LengthReq;
    LL_CTRDATA_LENGTH_RSP_STRU              LengthRsp;

}LL_CtrDATA_STRU;



typedef struct
{
    U8 offset;
    U8 waitBBAck;
    LC_LE_PACKET_HEADER_B0  tx_payload_header_b0;
    LC_LE_PACKET_HEADER_B1  tx_payload_header_b1;
}LL_CtrDATA_HDR_STRU;

typedef struct
{
    LL_CtrDATA_HDR_STRU Hdr;
    U8 Opcode;
    union
    {
        LL_CTRDATA_CONNECTION_UPDATE_REQ_STRU   ConnUpdateReq;
        LL_CTRDATA_CHANNEL_MAP_REQ_STRU         ChMapReq;
        LL_CTRDATA_TERMINATE_IND_STRU           TermInd;
        LL_CTRDATA_ENC_REQ_STRU                 EncReq;
        LL_CTRDATA_ENC_RSP_STRU                 EncRsp;
        LL_CTRDATA_UNKNOWN_RSP_STRU             UnknownRsp;
        LL_CTRDATA_FEATURE_REQ_STRU             FEAReq;
        LL_CTRDATA_FEATURE_RSP_STRU             FEARsp;
        LL_CTRDATA_VERSION_IND_STRU             VersInd;
        LL_CTRDATA_REJECT_IND_STRU              RejectInd;
        LL_CTRDATA_SLAVE_FEATURE_REQ_STRU       SlaveFEAReq;
        LL_CTRDATA_CONNECTION_PARAM_REQ_STRU    ConnParamReq;
        LL_CTRDATA_CONNECTION_PARAM_RSP_STRU    ConnParamRsp;
        LL_CTRDATA_REJECT_IND_EXT_STRU          RejectIndExt;
        LL_CTRDATA_LENGTH_REQ_STRU              LengthReq;
        LL_CTRDATA_LENGTH_RSP_STRU              LengthRsp;

    } CtrData;

} LL_CTRL_DATA_STRU, OS_PTR LL_CTRL_DATA_STRU_PTR;


//////////////////// LC_LMP definitinition///////////////////////

typedef struct{
    U8 offset[6];
    LC_LE_PACKET_HEADER_B0  tx_payload_header_b0;
    LC_LE_PACKET_HEADER_B1  tx_payload_header_b1;
}LC_LMP_HDR_STRU;


typedef struct
{
    LC_LMP_HDR_STRU Hdr;
    U8 opcode;
    union
    {
        LL_CTRDATA_CONNECTION_UPDATE_REQ_STRU   ConnUpdateReq;
        LL_CTRDATA_CHANNEL_MAP_REQ_STRU         ChMapReq;
        LL_CTRDATA_TERMINATE_IND_STRU           TermInd;
        LL_CTRDATA_ENC_REQ_STRU                 EncReq;
        LL_CTRDATA_ENC_RSP_STRU                 EncRsp;
        LL_CTRDATA_UNKNOWN_RSP_STRU             UnknownRsp;
        LL_CTRDATA_FEATURE_REQ_STRU             FEAReq;
        LL_CTRDATA_FEATURE_RSP_STRU             FEARsp;
        LL_CTRDATA_VERSION_IND_STRU             VersInd;
        LL_CTRDATA_REJECT_IND_STRU              RejectInd;
        LL_CTRDATA_SLAVE_FEATURE_REQ_STRU       SlaveFEAReq;
        LL_CTRDATA_CONNECTION_PARAM_REQ_STRU    ConnParamReq;
        LL_CTRDATA_CONNECTION_PARAM_RSP_STRU    ConnParamRsp;
        LL_CTRDATA_REJECT_IND_EXT_STRU          RejectIndExt;
        LL_CTRDATA_LENGTH_REQ_STRU              LengthReq;
        LL_CTRDATA_LENGTH_RSP_STRU              LengthRsp;

    } CtrData;

} LC_LMP_STRU, OS_PTR LC_LMP_STRU_PTR;



// OPCODE DEFINITION OF LC_EVT
#define LC_EVT_ADV_REPORT                         0x00
#define LC_EVT_ADV_ENDED_BY_HOST                  0x01
#define LC_EVT_DIRECT_ADV_TIMEOUT                 0x02
#define LC_EVT_ADV_ENDED_WHEN_CONNECTION_CREATED  0x03
#define LC_EVT_CONNECTION_COMPLETE                0x04
#define LC_EVT_LINK_RELEASED                      0x05
#define LC_EVT_COLLISION_OCCURS                   0x06
#define LC_EVT_SCAN_REQ_RECEIVED                  0x07
#define LC_EVT_LL_PDU_ACKED                       0x08
#define LC_EVT_CONNECTION_UPDATED                 0x09
#define LC_EVT_CHM_UPDATED                        0x0A
#define LC_EVT_INIT_ENDED_BY_HOST                 0x0B
#define LC_EVT_INIT_ENDED_WHEN_CONNECTION_CREATED 0x0C
#define LC_EVT_SCAN_ENDED_BY_HOST                 0x0D




#define LC_CONN_DUE_TO_ADV
#define LC_CONN_DUE_TO_INITIATE


#define LC_PRE_CONNECTION_LINK_IDX  (NUM_OF_SUPPORTED_LE_ACL_LINK)


enum
{
    CONN_FROM_ADV_STATE = 0x0,
    CONN_FROM_INITIATE_STATE,
};


#define CONN_INTERVAL_BASE  16


#define LL_ADV_DISABLE_CMD      0x1
#define LL_SCAN_DISABLE_CMD     0x2
#define LL_INIT_DISABLE_CMD     0x3
#define LL_DETACH_CMD           0xFF

typedef struct
{
    U8 Lc_link_idx;
    U8 Opcode;
    U8 Para[1];

} LL_CMD_STRU, OS_PTR LL_CMD_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    U16         advertising_interval_min;
    U16         advertising_interval_max;
    union
    {
        struct
        {
            U8              advertising_type;
            U8              own_addr_type;
            U8              direct_addr_type;
            BD_ADDR_STRU    direct_addr;
            U8              advertising_channel_map;
            U8              advertising_filter_policy;
        } member;
        U8 group[11];
    } param_group;

} LE_ADV_PARAM, OS_PTR LE_ADV_PARAM_PTR;

typedef struct
{
    U8      adv_data_len;
    U8      adv_data[31];

} LE_ADV_DATA_PARAM;

typedef struct
{
    U8      scan_rsp_data_len;
    U8      scan_rsp_data[31];

} LE_SCAN_RSP_DATA_PARAM;

typedef struct
{
    U8  key[16];
    U8  plaintext_data[16];
} LE_ENCRYPT_PARAM_STRU;

typedef struct
{
    LE_ENCRYPT_PARAM_STRU   encrypt_param;
    U8                      encrypted_data[16];
} LE_ENCRYPT_STRU;

typedef struct
{
    U8 member[5];    
} LE_CH_MAP_STRU;


#ifdef AB1520D

typedef struct
{
    U8 autoInitiateLenUpdateReq;
    U16 connInitMaxRxOctets;    
    U16 connInitMaxRxTime;        
    U16 connInitMaxTxOctets;
    U16 connInitMaxTxTime;    
    U16 supportedMaxRxOctets;
    U16 supportedMaxRxTime;
    U16 supportedMaxTxOctets;
    U16 supportedMaxTxTime;    
    
}LE_DATA_LENGTH_CTL_STRU_ROM;

typedef struct
{
    LE_DATA_LENGTH_CTL_STRU_ROM le_data_length_ctl;
}LE_MISC_CONTROL_STRU_ROM;
#endif

typedef struct
{
    LE_ADV_PARAM                        adv_param;
    LE_ADV_DATA_PARAM                   adv_data_param;
    LE_SCAN_RSP_DATA_PARAM              scan_rsp_data_param;    
    HCI_CMD_PARA_LE_SET_SCAN_PARA_STRU  scan_para;
    LE_ENCRYPT_STRU                     le_encrypt;
    LE_CH_MAP_STRU                      ChMap;
    #ifdef AB1520D
    LE_MISC_CONTROL_STRU_ROM            le_misc_ctl;
    #else
    LE_MISC_CONTROL_STRU                le_misc_ctl;    
    #endif
}LL_PARAM_STRU;

EXTERN LL_PARAM_STRU XDATA gLL_param;

#define BLE_RX_GC_BASE		59



typedef struct
{
    U8  SKDm[8];
    U8  SKDs[8];
}SKD_STRU;

typedef struct
{
    U8  IVm[4];
    U8  IVs[4];
}IV_STRU;

typedef struct
{
    IV_STRU IV;
    U8 LTK[16];
    SKD_STRU SKD;
    U8 SessionKey[16];

}LL_LINK_ENC_RES_STRU, OS_PTR LL_LINK_ENC_RES_STRU_PTR;

 
typedef VOID (*LINK_TRANS_CTL_ENTRY)(VOID_NEAR *);

enum ll_link_trans_status
{
    TRANS_SUSPEND = 0,
    TRANS_IN_PROC,
    TRANS_WAIT_FOR_MSG,
    NUM_TRANS_STAUS
};
typedef struct
{
    LINK_TRANS_CTL_ENTRY entry;    
    U8 reason;    
}LINK_TRANS_EXCEPTION_CTL_STRU;

typedef struct
{
    U8 status;
    HC_CMD_MSG_STRU_PTR pCmdMsg; //pCmdMsg is the CmdMsg trigger the trans and param is needed when collision occurs, put at trans release  
    OS_DATA_PTR msgPtr;          //msgPtr is any other msg except for pCmdMsg
    OST_PTR ProcRspTimerPtr;
    LINK_TRANS_CTL_ENTRY entry;
    LINK_TRANS_EXCEPTION_CTL_STRU exc_ctl;
} LINK_TRANS_CTL_STRU, OS_PTR LINK_TRANS_CTL_STRU_PTR;

typedef struct
{
    HC_CMD_MSG_STRU_PTR pCmdMsg;
    LINK_TRANS_CTL_ENTRY entry;
    LINK_TRANS_EXCEPTION_CTL_STRU exc_ctl;
}LINK_QUEUED_TRANS_CTL_STRU, OS_PTR LINK_QUEUED_TRANS_CTL_STRU_PTR;

typedef struct
{
    U8 ch_map_update_on:1;
    U8 conn_update_on:1;   
    U8 data_len_update_req:1;
    U8 data_len_update_unknown:1;    
    U8 RSVD:4;
}LINK_BIT_CTL_STRU;

typedef struct
{
    U8 TerminateTO:1;
    U8 ProcRspTO:1;
    U8 RemoteVerInfoTO:1;
    U8 DetachCmdSended:1;
    U8 RSVD:4;
}LINK_TIMEOUT_CTL_STRU;

typedef struct
{
    LINK_TRANS_CTL_ENTRY AES_entry;
    U8 AES_sem_key;
}LINK_AES_CTL_STRU, OS_PTR LINK_AES_CTL_STRU_PTR;

typedef struct 
{
    U16 MaxRxOctets;
    U16 MaxRxTime;
    U16 MaxTxOctets;    
    U16 MaxTxTime;    
    U16 RemoteMaxRxOctets;
    U16 RemoteMaxRxTime;
    U16 RemoteMaxTxOctets;    
    U16 RemoteMaxTxTime;      
    U16 EffectiveMaxRxOctets;
    U16 EffectiveMaxRxTime;    
    U16 EffectiveMaxTxOctets;    
    U16 EffectiveMaxTxTime;    
}LINK_DATA_LENGTH_CTL_STRU, OS_PTR LINK_DATA_LENGTH_CTL_STRU_PTR;

typedef U8 LINK_STATUS;
#define LINK_STATUS_RELEASED                (LINK_STATUS)(0)
#define LINK_STATUS_CONNECTED               (LINK_STATUS)(1)
#define LINK_STATUS_TERMINATE_ON            (LINK_STATUS)(2)


typedef struct
{
    U8 lc_link_idx;
    U8 role;
    U8 enc_enabled;
    LINK_BIT_CTL_STRU BitCtl;
    LL_CTRDATA_VERSION_IND_STRU XDATA_PTR remote_ver_ptr;
    LINK_STATUS link_status;
    SUPPORTTED_FEATURES CFeatures;
    LL_LINK_ENC_RES_STRU_PTR EncResPtr;
    OST_PTR TerminateTimerPtr;
    CONN_HDL_STRU_PTR ConnHdlCtlPtr;
    LINK_TRANS_CTL_STRU TransCtl;
    LINK_QUEUED_TRANS_CTL_STRU QueuedTransCtl;
    LINK_TIMEOUT_CTL_STRU TimeOutCtl;
    LINK_AES_CTL_STRU AesCtl;
    LINK_DATA_LENGTH_CTL_STRU_PTR DataLenCtlPtr;

} LL_LINK_CONTROL_STRU, OS_PTR LL_LINK_CONTROL_STRU_PTR;

typedef void (*LL_SMENTRY)(VOID);

typedef struct
{
    LL_SMENTRY entry;
    U8 XDATA_PTR cmd_ptr;
    U8 sem_key;
}LL_SMCTL_STRU;

typedef struct ll_func_entry_stru
{
    VOID (*fLL_init)(VOID);
    VOID (*fLL)(VOID);
    VOID (*fLL_HciCmdHdlr)(VOID);
    VOID (*fLL_LcEvtHdlr)(VOID);
    VOID (*fLL_LinkProc)(LL_LINK_CONTROL_STRU_PTR);
    VOID (*fLL_LcEvtBylinkHdlr)(LL_LINK_CONTROL_STRU_PTR);
    VOID (*fLL_ChMapUpdateHdlr)(LL_LINK_CONTROL_STRU_PTR);
    VOID (*fLL_LcLmpHdlr)(LL_LINK_CONTROL_STRU_PTR);
    VOID (*fLL_HciCmdBylinkHdlr)(LL_LINK_CONTROL_STRU_PTR);
    VOID (*fLL_LinkTransProc)(LL_LINK_CONTROL_STRU_PTR);
    VOID (*fLL_LinkTimeoutHdlr)(LL_LINK_CONTROL_STRU_PTR);
}LL_FUNC_ENTRY_STRU;



EXTERN LL_FUNC_ENTRY_STRU gLL_FunEntry;

EXTERN LL_SMCTL_STRU    XDATA LL_SMCtl;
EXTERN LL_LINK_CONTROL_STRU XDATA gLL_LinkCtl[NUM_OF_SUPPORTED_LE_ACL_LINK];
EXTERN LL_SEMCTL_STRU    XDATA gLL_sem_ctl;

#endif
#endif
