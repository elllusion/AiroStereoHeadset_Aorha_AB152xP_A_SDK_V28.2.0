#ifndef _HC_CONN_HDL_H_
#define _HC_CONN_HDL_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * hc_conn_hdl.c is the file defines the utilities of connection handle.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 *              CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define RESERVED_CONNECTION_HANDLE_FOR_FLASH_WRITE  (0xF00)
#define RESERVED_CONNECTION_HANDLE_FOR_VENDOR_DATA  (0xF01)

/* Number of supported connection handles */
#define NUM_OF_SUPPORTED_CONN_HDL   (NUM_OF_SUPPORTED_ACL_LINK)
/* Connection handle starting base number */
#define CONNHDL_BASE                (0x80)
/* ACL connection handle boundaries */
#define ACL_CONNHDL_LOWER_BOUND         (CONNHDL_BASE)
#define ACL_CONNHDL_UPPER_BOUND         (CONNHDL_BASE + NUM_OF_SUPPORTED_CONN_HDL)
/* SCO/eSCO connection handle boundries */
#define SCO_ESCO_CONNHDL_LOWER_BOUND    (0xF0)
#define SCO_ESCO_CONNHDL_UPPER_BOUND    (0xF2)


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//-
typedef struct stru_btlink_conn_info
{
    BD_ADDR_STRU BdAddr;
    PARITY_ARR Parity;

} BTLINK_CONN_INFO_STRU;

/* Type definition of connection handle number */
typedef U16 CONN_HDL;
/* Type definition of connection handle structure */
typedef struct stru_conn_hdl
{
    CONN_HDL ConnHdl;
    OS_TASK_ID TaskID;
    OSMQ_PTR pOSMQ_HCI_ACL_Data_From_Host;    /* PDU from Host */
    OSMQ_PTR pOSMQ_HCI_ACL_Data_To_Host;      /* PDU to Host */
    OSMQ_PTR pOSMQ_HCI_Command_Relay_To_LM;
    OSMQ_PTR pOSMQ_HCI_Command_To_LC;
    OSMQ_PTR pOSMQ_LMP_Data_From_LM;
    OSMQ_PTR pOSMQ_LMP_Data_To_LM;

    union
    {
        BTLINK_CONN_INFO_STRU Bt;

    } LinkInfo;

    U16 FialedContactCounter;
    U16 FlushTimeout;
    U16 SupervisionTimeout;
    S8 Rssi;
    U8 IsBle;

} CONN_HDL_STRU, OS_PTR CONN_HDL_STRU_PTR;


#define MAX_SUPPORTED_PAGE  2

typedef struct
{
    U8 lookupIdx;
    U8 type;
    U8 occupySlotNo;
    U16 len;
}PktTypeInfo;

typedef struct
{
    U8 tblSize;
    PktTypeInfo CODE_PTR pktTypeInfo[7]; //edr mode has max tbl size of 7
}PktTypeTblInfo;

typedef struct
{
    U8 tx;
    U8 txReqd; //requested
    U8 rx;
    #ifdef LC_TEST_CASE_CONFIG
    //Added by Ken for BQT 250509
    U8 txReNego;
    U8 txReNegoSlot;
    #endif
}MaxSlotType;

typedef union
{
    struct
    {
        U8 roleswitch:1;
        U8 hold:1;
        U8 sniff:1;
        U8 park:1;
        U8 reserved1:4;
    }bits;
    U8 value;
}LinkPolicyType;

enum
{
        LM_NON_EDR_MODE = 0,
        LM_EDR_MODE,
        LM_MODE_NO,
};

typedef struct
{
    CONN_HDL_STRU_PTR pConnHdl;
    BD_ADDR_STRU BlockedBdAddr;
    //U8 reduced_connection_handle;
    U8 piconet_number;
    //U8 LT_ADDR;
    //BD_ADDR link_bd_addr;
    //U8 link_parity[5];
    U8 class_of_device[3];
    SUPPORT_VERSION LMP_version;
    SUPPORT_FEATURES LMP_feature_ext[MAX_SUPPORTED_PAGE];
    U8 link_activity;
    MaxSlotType maxslot;
    U8 tx_gfsk_gain_gc1;
    U8 tx_gfsk_gain_gc2;
    U8 tx_edr_gain_gc1;
    U8 tx_edr_gain_gc2;
    PktTypeTblInfo pktTypeTbl[LM_MODE_NO]; //0: for non-edr mode; 1: edr-mode
    U8 capaibleSlotNo; //according to prefer rate lmp
    U16 pktTypeCapaiblity; //according to prefer rate lmp, packet type index -> according to EnumPktType, total packet type no = 12
    LinkPolicyType linkPolicy;
    /* workaround for issue #1631 */
    U8 ConnCplEvtSent;
    U8 Drift;
    U8 Jitter;
    U8 allow_role_switch; //parameter from HCI Create Connection
} SYS_CONNECTION_HANDLE_INFO;

extern SYS_CONNECTION_HANDLE_INFO XDATA gSYS_connection_handle_info[MAX_NUM_ACL_LINK];

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * HCI_CONNHDL_Init
 *
 * Initialize connection handle and bit mask.
 *
 * @Author : DingaCheng <DingaCheng@airoha.com.tw>
 *
 */
EXTERN VOID HCI_CONNHDL_Init(VOID);

/**
 * HCI_CONNHDL_New
 *
 * Get connection handle control structure.
 * If all handle are used, return NULL.
 *
 * @Author : DingaCheng <DingaCheng@airoha.com.tw>
 * @Return : The pointer of the connection handle control structure.
 *
 */
EXTERN CONN_HDL_STRU_PTR HCI_CONNHDL_New(
    IN      OS_TASK_ID TaskID);

/**
 * HCI_CONNHDL_Delete
 *
 * Release connection handle control structure.
 *
 * @Author : DingaCheng <DingaCheng@airoha.com.tw>
 * @ConnHdl : Connection handle
 *
 */
EXTERN VOID HCI_CONNHDL_Delete(
    IN      CONN_HDL_STRU_PTR pConnHdl);

/**
 * HCI_CONNHDL_XportTxForwarder
 *
 * HCI transport TX forwarder
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HCI_CONNHDL_XportTxForwarder(VOID);

/**
 * HCI_CONNHDL_Query
 *
 * Get connection handle control structure.
 * If all handle are used, return NULL.
 *
 * @Author : DingaCheng <DingaCheng@airoha.com.tw>
 * @ConnHdl : Connection handle
 * @Return : The pointer of the connection handle control structure.
 *
 */
EXTERN CONN_HDL_STRU_PTR HCI_CONNHDL_Query(
    IN      CONN_HDL ConnHdl);

#endif /* _HC_CONN_HDL_H_ */

