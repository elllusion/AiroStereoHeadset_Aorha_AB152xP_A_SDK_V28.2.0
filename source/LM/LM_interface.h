//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _LM_INTERFACE_H_
#define _LM_INTERFACE_H_

#include "rc_flash.h"
#include "lmp.h"
#include "hc_interface.h"
#include "drv_sector_flash.h"
#include "lmci.h"
#include "lc_interface.h"

#ifdef _LM_C_
#define LM_EXTERN
#else
#define LM_EXTERN extern
#endif

//#define SUPPORT_EIR_INQUIRY

//Added by Eric Chou, 2009.4.13
//There are many functions that won't be used in most situations of HeadsetSOC,
//however, we will use those in tests, so we define ACT_FUNCTION.
#define ACT_FUNCTION

//Added by Eric Chou, 09.5.21
//If SUPPORT_POWER_CTRL_REQUEST is not defined, we'll not check remote TX power.
#define SUPPORT_POWER_CTRL_REQUEST


//========================================== LM misc support feature
//- feature 0 bank

#define LM_MISC_SUPP_FEA_TRANSACTION_TIMER_ENABLE()                 (pLmParameter->misc_ctl0 & LM_MISC_SUPP_FEA0_TRANSACTION_TIMER_ENABLE)
#define LM_MISC_SUPP_FEA_EXECUTEONESUBSTATEBYBBACK()                (pLmParameter->misc_ctl0 & LM_MISC_SUPP_FEA0_EXECUTEONESUBSTATEBYBBACK)
#define LM_MISC_SUPP_FEA_LOCALSUBRATEALWAYSONE()                    (pLmParameter->misc_ctl0 & LM_MISC_SUPP_FEA0_LOCALSUBRATEALWAYSONE)
#define LM_MISC_SUPP_FEA_SLAVE_ACT_UNSNIFF_WHEN_RECV_LMP_ACCEPTED() (pLmParameter->misc_ctl0 & LM_MISC_SUPP_FEA0_SLAVE_ACT_UNSNIFF_WHEN_RECV_LMP_ACCEPTED)
#define LM_MISC_SUPP_FEA_NO_ACTIVE_SNIFF_SUBRATING()                (pLmParameter->misc_ctl0 & LM_MISC_SUPP_FEA0_NO_ACTIVE_SNIFF_SUBRATING)
#define LM_MISC_SUPP_FEA_DISCARD_CLASSIFICATION_WHEN_SCO_ON()       (pLmParameter->misc_ctl0 & LM_MISC_SUPP_FEA0_DISCARD_CLASSIFICATION_WHEN_SCO_ON)
#define LM_MISC_SUPP_FEA_TURN_ON_TX_3M_MODE()                       (pLmParameter->misc_ctl0 & LM_MISC_SUPP_FEA0_TURN_ON_TX_3M_MODE)
#define LM_MISC_SUPP_FEA_PLC_ON()                                   (pLmParameter->misc_ctl0 & LM_MISC_SUPP_FEA0_PLC_ON)

#define LM_MISC_SUPP_FEA_TWS_AFH_ENABLE()                           (pLmParameter->misc_ctl1 & LM_MISC_SUPP_FEA1_TWS_AFH_ENABLE)
#define LM_MISC_SUPP_FEA_TWS_CHANNELCLASSIFICATION()                (pLmParameter->misc_ctl1 & LM_MISC_SUPP_FEA1_TWS_CHANNELCLASSIFICATION)
#define LM_MISC_SUPP_FEA_ESCO_NEGOTIATION_ENABLE()                  (pLmParameter->misc_ctl1 & LM_MISC_SUPP_FEA1_ESCO_NEGOTIATION_ENABLE)

#define LM_MISC_SUPP_FEA_SNIFF_NEGOTIATION_ENABLE()                 (pLmParameter->misc_ctl1 & LM_MISC_SUPP_FEA1_SNIFF_NEGOTIATION_ENABLE)
#define LM_MISC_SUPP_FEA_MCU24M_FOR_PAIRING_ENABLE()                (pLmParameter->misc_ctl1 & LM_MISC_SUPP_FEA1_MCU24M_FOR_PAIRING_ENABLE)

#ifdef SUPPORT_SIMPLE_PAIRING
    //#define SIMPLE_PAIRING_TEST_MODE  //for specific pattern passkey, nonce,...
#endif


#define LM_COLLISION_RECOVERY
#define LM_RS_ANYTIME
//#define LM_DEBUG
//#define LM_ADVANCE_LOG
//#define LM_INTERNAL_LOG //i.e., for aclu logs

/*
#define LM_AFH_THRESHOLD4_HIGH                  69
#define LM_AFH_THRESHOLD4_LOW                   60  //default
#define LM_AFH_THRESHOLD3_HIGH                  51
#define LM_AFH_THRESHOLD3_LOW                   42  //default
#define LM_AFH_THRESHOLD2_HIGH                  33
*/
#define LM_AFH_THRESHOLD2_LOW                   24  //default
#define LM_AFH_THRESHOLD1_HIGH                  20  //default
#define LM_AFH_THRESHOLD1_LOW                   15

#define LM_AFH_THRESHOLD_NON_OVERLAP            9

#define LM_AFH_RSSI_REFRESH_DELAY               12
#define LM_AFH_RSSI_LOW_DUTY_REFRESH_DELAY      12
#define LM_AFH_RSSI_THRESHOLD_NUM               4
#define LM_AFH_MASTER                           1
#define LM_AFH_SLAVE                            0
//#define LM_AFH_RSSI_SEND_FIRST_DETECT
//#define LM_AFH_RSSI_SINGLE_REFRESH_COUNTER
//#define LM_AFH_RSSI_WLAN_CH14_NO_DETECT

#define LOW_DUTY_DETECT_TIMER0_ADD_MAX  1
#define LOW_DUTY_DETECT_TIMER1          3
#define RSSI_REPEAT_NUM_NEW             2
#define RSSI_REPEAT_NUM_OLD             1
#define RSSI_GROUP_NUM_NEW              10
#define RSSI_GROUP_NUM_OLD              10

#ifdef OPERATION_IN_RF_AB1000D
#define RSSI_CNT_THD_NEW                8
#else
#define RSSI_CNT_THD_NEW                7
#endif

#define RSSI_CNT_THD_OLD                7
#define NML_DETECT_GOOD                 2
#define LOW_DUTY_NO_DETECT              3

#define RSSI_WINDOW_WIDTH       10
#define COUNTER_THRESHOLD       (RSSI_WINDOW_WIDTH + 20)	//0xFFFF

//For QoS
#define DEFAULT_QOS_LATENCY     25000       //25000 microseconds, means 40 slots
#define MIN_QOS_LATENCY         1250        //1250 microseconds, means 2 slots
#define MAX_QOS_LATENCY         40958750    //40958750 microseconds, means 65534 slots, follow CSR dongle

//For general transaction timer
#define MAX_TRANS_WAITING_TIME 3200L * 30

#define THD4_GP 13
#define THD3_GP 12
#define THD2_GP 11
#define THD1_GP 11
#define PLUS    1
#define MINUS   0

#define LM_TESTMODE_START_WITH_DELAY            0
#define LM_TESTMODE_TRANSCTION_TO               5
#define LM_ACTIVE_DETACH_TRANSCTION_TO(linkIdx)          gLC_ctl.PollInvl[linkIdx]*6//240//100
#define LM_ACTIVE_DETACH_TO_AFTER_ACK(linkIdx)           gLC_ctl.PollInvl[linkIdx]*3//240//100
#define LM_PASIVE_DETACH_TIMER_MASTER(linkIdx)           gLC_ctl.PollInvl[linkIdx]*6//240//100
#define LM_PASIVE_DETACH_TIMER_SLAVE(linkIdx)            gLC_ctl.PollInvl[linkIdx]*3//240//100


#define LM_INVALID_CONNECTION_LINK_IDX          ((U8)0x7F)

#define GROWING_UP_FACTOR 1

#define LM_SET_CURRENT_LINK(A)          SwitchLink(A)
#define LM_GET_CURRENT_LINK()           (gLM_ctl.sche_ctl.current_idx)
#define IS_LINK_EXISTING(linkIdx)       (U16_BIT_MASK(linkIdx)&gLM_ctl.sche_ctl.used_mask)

PUBLIC CONN_HDL LM_GetConnHandle(U8 idx);
PUBLIC CONN_HDL LM_GetCurrentConnHandle(void);

PUBLIC void LM_LinkIdx_Add(U8 linkInd);
PUBLIC void LM_LinkIdx_Del(U8 linkInd);

PUBLIC void LM_LTAddrIdx_Add(U8 ltAddr);
PUBLIC void LM_LTAddrIdx_Del(U8 ltAddr);

#define IS_QUEUE_AVAILABLE(A)   (TRUE)

#define LM_SLAVE 0

#define LM_SCO_HANDLE     120   //120~126, Max 7 sco
#define NEW_LM_SCO_HANDLE 0xF0

#ifdef SUPPORT_2_SLAVE
#define LM_SLAVE2 (MAX_NUM_ACL_LINK-1)
#endif

#define LM_PROCESS_STATE_ACTIVATE_BY_LMP_PDU        0x01
#define LM_PROCESS_STATE_ACTIVATE_BY_LC_EVENT       0x02
#define LM_PROCESS_STATE_ACTIVATE_BY_LM_SCHEDULER   0x03
#define LM_PROCESS_STATE_ACTIVATE_BY_SEQUENTIAL     0x04 //sequtial processes right after other types
#define LM_PROCESS_STATE_ACTIVATE_BY_LOCAL          0x05 //local processes
#define LM_PROCESS_STATE_ACTIVATE_BY_HCI_CMD        0x06
#define LM_PROCESS_STATE_ACTIVATE_BY_HCI_GNL_CMD    0x07 //hci general type command -non-link-related
#define LM_PROCESS_STATE_ACTIVATE_BY_HCI_VCMD       0x08
#define LM_PROCESS_STATE_ACTIVATE_IF_NO_REMOTE_LMP  0x09 //proc activates if no remote lmp in a fixed period of time

#define LM_IDLE_TOP_STATE   0

#define LM_SUBSTATE01               1
#define LM_SUBSTATE02               2
#define LM_SUBSTATE03               3
#define LM_SUBSTATE04               4
#define LM_SUBSTATE05               5
#define LM_SUBSTATE06               6
#define LM_SUBSTATE07               7
#define LM_SUBSTATE08               8
#define LM_SUBSTATE09               9
#define LM_SUBSTATE10               10
#define LM_SUBSTATE11               11
#define LM_SUBSTATE12               12
#define LM_SUBSTATE13               13
#define LM_SUBSTATE14               14
#define LM_SUBSTATE15               15
#define LM_SUBSTATE20               20
#define LM_SUBSTATE21               21
#define LM_SUBSTATE22               22
#define LM_SUBSTATE23               23
#define LM_SUBSTATE24               24
#define LM_SUBSTATE25               25
#define LM_SUBSTATE31               31
#define LM_SUBSTATE32               32
#define LM_SUBSTATE33               33
#define LM_SUBSTATE34               34
#define LM_SUBSTATE_CLEAR_SUCCESS   35
#define LM_SUBSTATE_CLEAR_FAIL      36
#define LM_SUBSTATE_COLLISION       50
#define LM_DISC_SCO_SUBSTATE        51

//below is for local processes only
#define LM_LOCPROC_ACL_ON 0X66
#define LM_LOCPROC_ACL_OFF 0X67
#define LM_LOCPROC_ROLESWITCH 0X68
#define LM_LOCPROC_RESEG 0X69
#define LM_LOCPROC_CHGSCOSETTING 0X6B
#define LM_LOCPROC_AUDIOOFF 0X6C
#define LM_LOCPROC_ACL_DETACH 0x6D
#define LM_LOCPROC_SEND_DECREASE_POWER_REQ 0x6E
#define LM_LOCPROC_SEND_INCREASE_POWER_REQ 0x6F


//-Escape 4
#define LM_PDU_LMP_ACCEPTED_EXT                 1
#define LM_PDU_LMP_CHANNEL_CLASSIFICATION_REQ   16
#define LM_PDU_LMP_CHANNEL_CLASSIFICATION       17
#define LM_PDU_LMP_ESCO_LINK_REQ                12
#define LM_PDU_LMP_FEATURES_REQ_EXT             3
#define LM_PDU_LMP_FEATURES_RES_EXT             4
#define LM_PDU_LMP_IO_CAPABILITY_REQ            25
#define LM_PDU_LMP_IO_CAPABILITY_RES            26
#define LM_PDU_LMP_KEYPRESS_NOTIFICATION        30
#define LM_PDU_LMP_NOT_ACCEPTED_EXT             2
#define LM_PDU_LMP_NUMERIC_COMPARISON_FAILED    27
#define LM_PDU_LMP_OOB_FAILED                   29
#define LM_PDU_LMP_PACKET_TYPE_TABLE_REQ        11
#define LM_PDU_LMP_PASSKEY_FAILED               28
#define LM_PDU_LMP_PAUSE_ENCRYPTION_REQ         23
#define LM_PDU_LMP_POWER_CONTROL_REQ            31
#define LM_PDU_LMP_POWER_CONTROL_RES            32
#define LM_PDU_LMP_REMOVE_ESCO_LINK_REQ         13
#define LM_PDU_LMP_RESUME_ENCRYPTION_REQ        24
#define LM_PDU_LMP_SNIFF_SUBRATING_REQ          21
#define LM_PDU_LMP_SNIFF_SUBRATING_RES          22

#define GENERAL_LMP_PDU_NUMBER 65
#define EXT_LMP_PDU_NUMBER 30

#define LM_NOTIFY_HOST_WITH_CONN_REQ_EVT       1
#define LM_RECEIVING_CONN_REQ_WITHOUT_INTEREST 2
#define LM_AUTO_ACCEPT_CONN_REQ                3


//End of Define LM_PDU OP CODE

//LM RX PDU format: offset len_L len_H TID_OP (TID_OP_EXT) payload1 ...
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
    U8 extOpcode;
    U8 firstPara;
}LclmExtDataType;

typedef struct stru_lm_pdu_msg
{
    U8 Offset;
    U8 RSVD1;
    U8 RSVD2;
    U8 Misc;
    U8 LengthB0;
    U8 LengthB1;
    LMP_STRU LmpPdu;

} LM_PDU_MSG_STRU;


#define LM_DATA_GET_EXT_OPCODE(ptr)     ((LclmExtDataType XDATA_PTR)ptr)->extOpcode
#define LM_DATA_EXT_PARAM_PTR(ptr)      (&((LclmExtDataType XDATA_PTR)ptr)->firstPara)

//LM TX PDU format: offset control len_L len_H TID_OP (TID_OP_EXT) payload1 ...
#define DATA_OFFSET_LCLM_DATA           3

#define LM_PDU_EXT_OPCODE(ptr)          ((LclmExtDataType XDATA_PTR)ptr)->extOpcode
#define LM_PDU_LMP_EXT_PARAM_PTR(ptr)   (&((LclmExtDataType XDATA_PTR)ptr)->firstPara)


//- LM CMD to LC
//#define DATA_OFFSET_LCLM_CMDEVT         1

#define DATA_OFFSET_LMHC_CMDEVT         1
typedef struct
{
    U8 offset; // 1
    U8 rsvd1;
    U8 rsvd2;
    U8 opcode;
    U8 length; //parameter length
    U8 firstPara;
}LmhcCmdEvtType;

#define HCI_EVENT_DATA_OFFSET(ptr_msg)  ((LmhcCmdEvtType XDATA_PTR)ptr_msg)->offset
#define HCI_EVENT_OPCODE(ptr_msg)       ((LmhcCmdEvtType XDATA_PTR)ptr_msg)->opcode
#define HCI_EVENT_PARAM_LENGTH(ptr_msg) ((LmhcCmdEvtType XDATA_PTR)ptr_msg)->length
#define HCI_EVENT_PARAM_PTR(ptr_msg)    (&((LmhcCmdEvtType XDATA_PTR)ptr_msg)->firstPara)

#define ROLE_MASTER_PREFER              (0x00)
#define ROLE_SLAVE_PREFER               (0x01)

#define LINK_POLICY_DISABLED            (0x00)
#define LINK_POLICY_ENABLED             (0x01)

#define ALLOW_ROLE_SWITCH_REJECTED      (0x00)
#define ALLOW_ROLE_SWITCH_ACCPETED      (0x01)
#define ALLOW_ROLE_SWITCH_IGNORED       (0xFF)

//- #define MAX_NUM_ACL_LINK    1

typedef struct
{
    U8 Len[2];
} LM_PDU_LEN_STRU;

typedef U8 LM_PDU_LENGTH;
#define LM_PDU_LENGTH_NONE  (LM_PDU_LENGTH)0
#define LM_PDU_LENGTH_1     (LM_PDU_LENGTH)1
#define LM_PDU_LENGTH_2     (LM_PDU_LENGTH)2
#define LM_PDU_LENGTH_3     (LM_PDU_LENGTH)3
#define LM_PDU_LENGTH_4     (LM_PDU_LENGTH)4
#define LM_PDU_LENGTH_5     (LM_PDU_LENGTH)5
#define LM_PDU_LENGTH_6     (LM_PDU_LENGTH)6
#define LM_PDU_LENGTH_7     (LM_PDU_LENGTH)7
#define LM_PDU_LENGTH_8     (LM_PDU_LENGTH)8
#define LM_PDU_LENGTH_9     (LM_PDU_LENGTH)9
#define LM_PDU_LENGTH_10    (LM_PDU_LENGTH)10
#define LM_PDU_LENGTH_11    (LM_PDU_LENGTH)11
#define LM_PDU_LENGTH_12    (LM_PDU_LENGTH)12
#define LM_PDU_LENGTH_13    (LM_PDU_LENGTH)13
#define LM_PDU_LENGTH_14    (LM_PDU_LENGTH)14
#define LM_PDU_LENGTH_15    (LM_PDU_LENGTH)15
#define LM_PDU_LENGTH_16    (LM_PDU_LENGTH)16
#define LM_PDU_LENGTH_17    (LM_PDU_LENGTH)17
#define LM_PDU_NON_EXIST    (LM_PDU_LENGTH)0xFF


//- SYNCHRONOUS CONNECTION STATUS
typedef enum
{
    LM_SCO_DISABLE,
    LM_SCO_ENABLE,
    LM_ESCO_ENABLE,
    LM_SCO_CREATING,
    LM_ESCO_CREATING,
    LM_SCO_REMOVING,
    LM_ESCO_REMOVING
}SyncStateType;

//- ROLE SWITCH
#define LM_ROLE_SWITCH_DELAY_SLOT           0x1E0       //- 480 slot : 300 ms
#define LM_ROLE_SWITCH_EXTRA_SLOT           16 //additional slot no for the case of passed switch instant

//- SNIFF & UNSNIFF
#define LM_PRE_UNSNIFF_NEW_TIMEOUT          200

#define LM_SNIFF_STATE_OFF                  0x00
#define LM_SNIFF_STATE_ON                   0x01
#define LM_SNIFF_STATE_SLOW_CLOCK_ON        0x02

#define LM_SNIFF_TRANSISTION_ON             0x01

#define LM_SNIFF_DELAY_SLOT                 16

//- Encryption
#define LM_DECRY_OFF_ENCRY_OFF              0x00
#define LM_DECRY_OFF_ENCRY_ON               0x01
#define LM_DECRY_ON_ENCRY_OFF               0x02
#define LM_DECRY_ON_ENCRY_ON                0x03

//IO Capability Feature
#define LM_DISPLAY_ONLY                     0x00
#define LM_DISPLAY_YESNO                    0x01
#define LM_KEYBOARD_ONLY                    0x02
#define LM_NO_INPUT_NO_OUTPUT               0x03

#define LM_OOB_AUTH_NOT_PRESENT             0x00
#define LM_OOB_AUTH_DATA                    0x01

#define LM_NOMITM_NOBONDING_NUMERIC         0x00
#define LM_MITM_NOBONDING_IOCAP             0x01
#define LM_NOMITM_DEDICATEDBONDING_NUMERIC  0x02
#define LM_MITM_DEDICATEDBONDING_IOCAP      0x03
#define LM_NOMITM_GENERALBONDING_NUMERIC    0x04
#define LM_MITM_GENERALBONDING_IOCAP        0x05

//Authentication Strategies
#define LM_USING_NUMERIC_COMPARISON         0x00
#define LM_USING_OOB                        0x01
#define LM_USING_PASSKEY_ENTRY              0x02


//Key Status
#define LM_LINK_KEY_MISS                    0x00
#define LM_KEY_IN_GENERAL                   0x01
#define LM_LK2MK_PROCESS                    0x02
#define LM_MK2LK_PROCESS                    0x03
#define LM_CREATE_LK_PROCESS                0x04
#define LM_CHANGE_LK_PROCESS                0x05
#define LM_SIMPLE_PAIRING_PROCESS           0x06

//Notification Type
#define PASSKEY_ENTRY_STARTED               0x00
#define PASSKEY_DIGIT_ENTERED               0x01
#define PASSKEY_DIGIT_ERASED                0x02
#define PASSKEY_CLEARED                     0x03
#define PASSKEY_ENTRY_COMPLETED             0x04

//Key Type
#define LM_COMBINATION_KEY                  0x00
#define LM_LOCAL_UNIT_KEY                   0x01
#define LM_REMOTE_UNIT_KEY                  0x02
#define LM_DEBUG_COMBINATION_KEY            0x03
#define LM_UNAUTHENTICATED_COMBINATION_KEY  0x04
#define LM_AUTHENTICATED_COMBINATION_KEY    0x05
#define LM_CHANGED_COMBINATION_KEY          0x06



#define LM_ENCRYPTION_BD_ADDR_LOCAL         0x00
#define LM_ENCRYPTION_BD_ADDR_REMOTE        0x01
#define LM_ENCRYPTION_KEY_SEMI              0x00
#define LM_ENCRYPTION_KEY_MASTER            0x01
#define LINK_LEVEL_ENCRYPTION_OFF       0x00
#define LINK_LEVEL_ENCRYPTION_ON        0x01

#define LM_SLAVE_ROLE (gLM_link_info[LM_GET_CURRENT_LINK()].lm_state.role_of_slave)

//- AFH
#define LM_AFH_MODE_ENABLE                  1
#define LM_AFH_MODE_DISABLE                 0
#define LM_AFH_CC_ENABLE                    1
#define LM_AFH_CC_DISABLE                   0
#define LM_AFH_CC_DEFAULT_MIN_INTERVAL_IN_SLOT      0x0960
#define LM_AFH_CC_DEFAULT_MAX_INTERVAL_IN_SLOT      0x12C0
#define LM_AFH_SCAN_BITMAP_BT_ALL_CH                0x00
#define LM_AFH_SCAN_BITMAP_WLAN_ALL_CH              0x01
#define LM_AFH_SCAN_BITMAP_WLAN_NONOVERLAP_CH       0x02
#define LM_AFH_RSSI_MAX_SLEEP_CRC_PERIOD_NUM        0x01


#define HCI_EVENT_INQUIRY_RESULTS_PARAM_NUM_RESPONSES(ptr_param)                ( *(U8 XDATA_PTR)(ptr_param + 0) )
#define HCI_EVENT_COMMAND_COMPLETE_PARAM_STATUS(ptr_param, offset)              ( *(U8 XDATA_PTR)(ptr_param + 0 + offset))

#define LM_INQUIRY_EXIT_FROM_HC_CMD_CANCEL      0x01
#define LM_INQUIRY_EXIT_FROM_TIMER_EXPIRED      0x02
#define LM_INQUIRY_EXIT_FROM_REACH_RESPONSE     0x03
#define LM_INQUIRY_EXIT_FROM_MAX_BD_ADDR_BANK   0x04

//- testmode define
//-.......................

//- Timer constant
#define LM_CONNECTION_STATE_ENTERED_TO                                  (48000)


typedef struct
{
    U8      current_idx;
    U8      active_link_count;
    U16     used_mask;
    U16     ltaddr_mask; //meaningful only for master link index
    U8      LastAssignReducedConnHandle;
} LM_SCHEDULER;

typedef struct
{

    U8 sniff_on;
    U8 bb_acl_on:1;
    U8 lm_acl_on:1;
    U8 holdNegd:1;
    U8 hold_on:1;
    U8 testmode_on:1;
    U8 reserved:3;
    U8 role_of_slave;
    U8 encryption_on;
    U8 acluNestLevel;
    U8 sniff_transition_on;

}LM_STABLE_STATE;

typedef struct{
    U8 test_scenario;
    U8 hopping_mode;
    U8 tx_freqency;
    U8 rx_freqency;
    U8 power_control_mode;
    U8 poll_period;
    U8 packet_type;
    U8 length_of_test_data[2];
}LM_TESTMODE_PARAM;

typedef struct
{
    //U8 sco_handle;          //- Master (8), Slave (Any)
    U16 sco_handle;
    U8 acl_handle;          //- 0~7
    U8 activity;            //- 0:None, 1:SCO, 2:eSCO
    U8 escoLtaddr;
    U8 Timing_control_flags;
    U8 Dsco;
    U8 tsco;
    U8 pktType; //packet type of lmp sco_req
    U8 airMode;
    U8 EscoActiveSlots;
} LM_SCO_PARAM;

#if 0
typedef struct
{
    U8  acl_edr_on;
    U8  acl_edr_just_set;
} LM_ACLU_PARAM;
#endif

#define AFH_MODE_CHANGE() (ptr_afh_ctl->mode_change_by_HCI & 0x01)
#define AFH_MODE_ENABLE() (ptr_afh_ctl->mode_change_by_HCI & 0x02)

typedef struct
{
    U8                      feature_enable;             //- B1:AFH_chan_class_flag, B0:AFH_capable_flag
    U8                      cc_cm_need_to_calc_by_LC;
    U8                      cc_cm_change_by_HCI;        //- B3:changed by role switch, B2:changed by eafh/sacc B1:enable/disable, B0:change/unchange
    U8                      lm_timeout_flag;
    //- AFH ctrl
    U8                      mode;
    U8                      mode_to_change;
    U8                      channel_map_used[10];
    OST XDATA_PTR           afh_timeout_timer_ptr;
    FOUR_BYTE_UNION_TYPE    afh_instant;
    OST XDATA_PTR           afh_instant_timer_ptr;      //we can not send new LMP_set_AFH before last afh instant.

    //- AFH channel classification ctrl
    U8                      afh_classification_on;
    U16                     min_interval;               //- Use default value : LM_AFH_CC_DEFAULT_MIN_INTERVAL_IN_SLOT now
    U16                     max_interval;               //- Use default value : LM_AFH_CC_DEFAULT_MAX_INTERVAL_IN_SLOT now
    FOUR_BYTE_UNION_TYPE    report_interval_in_312_5;   //- SLAVE used only
    U8                      report[10];
    U8                      reportbitmap[10];

    U8                      remote_cc_recerived;

    //- pupa chagne for LM test case
    U8                      send_channel_classification_flag; //- bit 0 : wait to send, bit 1 : timer block
    U8                      AFH_init_flag;
} LM_AFH_PARAM;

#define LM_SET_AUTH_STRATEGY(A)         gLM_ctl.sp_ctl.ssp_data.auth_strategy = A
#define LM_GET_AUTH_STRATEGY(A)         gLM_ctl.sp_ctl.ssp_data.auth_strategy

#define LM_GET_LOCAL_PK()               gLM_ctl.sp_ctl.local.public_key.member
#define LM_GET_REMOTE_PK()              gLM_ctl.sp_ctl.remote[LM_GET_CURRENT_LINK()].public_key.member
#define LM_GET_PRIVATE_KEY()            gLM_ctl.sp_ctl.local.private_key

#define LM_SET_PK_GROUP_INDEX(A)        gLM_ctl.sp_ctl.ssp_data.payload_index = A
#define LM_GET_PK_GROUP_INDEX()         gLM_ctl.sp_ctl.ssp_data.payload_index

#define LM_SET_PASSKEY_ROUND(A)         gLM_ctl.sp_ctl.ssp_data.payload_index = A
#define LM_GET_PASSKEY_ROUND()          gLM_ctl.sp_ctl.ssp_data.payload_index

#define LM_GET_LOCAL_SP_NONCE()         gLM_ctl.sp_ctl.ssp_data.local_nonce
#define LM_GET_REMOTE_SP_NONCE()        gLM_ctl.sp_ctl.ssp_data.remote_nonce

#define LM_GET_LOCAL_SP_RN()            gLM_ctl.sp_ctl.local_random_number
#define LM_GET_REMOTE_SP_RN()           gLM_ctl.sp_ctl.remote_random_number

#define LM_GET_COMMIT()                 gLM_ctl.sp_ctl.ssp_data.commitment_value
#define LM_GET_PASSKEY_NUMERIC_VALUE()  gLM_ctl.sp_ctl.ssp_data.passkey_numeric_value

#define LM_GET_DHKEY()                  gLM_ctl.sp_ctl.ssp_data.dhkey
//#define LM_GET_DHKEY_CHECK_VALUE()        gLM_ctl.sp_ctl.dhkey_check_value

#define LM_GET_LOCAL_IOCAP_CTL()        gLM_ctl.sp_ctl.local.io_capability_ctl
#define LM_GET_REMOTE_IOCAP_CTL()       gLM_ctl.sp_ctl.remote[LM_GET_CURRENT_LINK()].io_capability_ctl


//HC to LM
#define NOT_RECV                0
#define RECV_REPLY              1
#define RECV_NEGATIVE_REPLY     2
#define WRONG_BD_ADDR           3

//LC to LM
#define NOT_RECV                0
#define RECV_ACCEPTED           1
#define RECV_NOT_ACCEPTED       2
#define WRONG_CONTENT           3

typedef union
{
    struct
    {
        U8  B0[16];
        U8  B1[16];
        U8  B2[16];
    } group;
    U8  member[48];
}PUBLIC_KEY;

#ifdef SUPPORT_SIMPLE_PAIRING

#define SSP_USING_K_BIT_PASSKEY 20

typedef struct
{
    U8 io_capability;
    U8 oob_data_present;
    U8 auth_requirments;
}IOCAPABILITY;


typedef struct
{
    PUBLIC_KEY public_key;              //Big Endian
    IOCAPABILITY io_capability_ctl;
    U8 private_key[24];                 //Big Endian

}LOCAL_SIMPLE_PAIRING_INFO;

typedef struct
{
    PUBLIC_KEY public_key;
    IOCAPABILITY io_capability_ctl;
}REMOTE_SIMPLE_PAIRING_INFO;

//Modified by Soga
/*
typedef struct
{
    //SHA256:
    U8 pre_process_msg[192];    //At most 80bytes-> 2 chuncks
    U32 w[64];
    U32 AH_Vector[8];

    //HMAC
    U8 digest_of_msg[32];
    U8 k0[64];
    U8 si[64+72];//To present "Si+Y", "So+hash value" and "temple valuable"

}LM_SHA256Type;
*/

//4*64 + 4*8 + 32 + 64 = 384
typedef struct
{
    U32 w[64];
    U8 digest_of_msg[32];
    U8 k0[64];
}LM_SSPMemType1;

//192+136 = 328
typedef struct
{
    U8 pre_process_msg[192];    //At most 80bytes-> 2 chuncks
    U8 si[64+72];//To present "Si+Y", "So+hash value" and "temple valuable"

}LM_SSPMemType2;

typedef struct
{
    U8 auth_strategy;

    U8 local_nonce[16];         //Na: unique random number
    U8 remote_nonce[16];        //Nb: unique random number

    U8 commitment_value[16];    //check C

    U32 passkey_numeric_value;  //for passkey strategy

    U8 dhkey[24];
    U8 payload_index;

    U8 isResourceUsedForSSP;

    //Modified by Soga, 20090305
    //LM_SHA256Type XDATA_PTR sha256BufPtr;
    LM_SSPMemType1 XDATA_PTR sspBufPtr1;
    LM_SSPMemType2 XDATA_PTR sspBufPtr2;
}LMSSPCtlType;

typedef struct
{
    //for OOB and Passkey strategies
    U8 local_random_number[16]; //ra
    U8 remote_random_number[16];//rb

    LOCAL_SIMPLE_PAIRING_INFO local;
    REMOTE_SIMPLE_PAIRING_INFO remote[MAX_NUM_ACL_LINK];

    LMSSPCtlType ssp_data;
    /*
    U8 auth_strategy;

    U8 local_nonce[16];         //Na: unique random number
    U8 remote_nonce[16];        //Nb: unique random number

    U8 commitment_value[16];    //check C

    U32 passkey_numeric_value;  //for passkey strategy

    U8 dhkey[24];
    U8 payload_index;

    U8 isResourceUsedForSSP;

    //Modified by Soga, 20090305
    //LM_SHA256Type XDATA_PTR sha256BufPtr;
    LM_SSPMemType1 XDATA_PTR sspBufPtr1;
    LM_SSPMemType2 XDATA_PTR sspBufPtr2; */
}SIMPLE_PAIRING_INFO;
#endif

typedef struct
{
    U8  key_info;                               //1xxx xxxx: change link key process, x1xx xxxx: Link Key Miss, xxxx xxx1: Link Key to Master Key process
    U8  link_key[16];
    U8  encryption_b4_lm_acl_on;
    U8  encryption_mode;
    U8  encryption_bd_addr;                     //- bd_addr use in E_series functions  (0:local,1:remote)
    U8  encryption_key_inuse;
    U8  active_key[16];
    U8  master_key[16];
    U8  random_number[16];
    U8  encryption_cof[12];
    U8  Kc[16];
    U8  encryption_keysize;
    U8  set_enc_on_process;                     //TRUE: set enc connection on, FALSE: set enc connection off
}LM_ENCRYPTION_HANDLE_INFO;

// define this to compatible with little endian format
typedef struct
{
    U8 lowByte;
    U8 highByte;
}LmPktType;

typedef struct
{
    FOUR_BYTE_UNION_TYPE n_clk_0, Initial_Instant;
    U16 Tsniff, attempt, timeout;
    U8 t32k_625u, t32k_8u;
    // NEVER change the declaration order of the above !!!
    U8 t12m_625u, t12m_8u, Timing_control_flags;
    U16 Dsniff, tmp16, TsniffMax;
    U16 attemptBackup;
    // added for sniff transition mode
    U8 sniffTotal;
    U8 sniffCnt;
    U8 unsniffTotal;
    U8 unsniffCnt;
}LmSniffInfoType;

typedef struct
{
    U8 isEsco;
    U8 scoCtlArrayIdx;
    U8 existingJustChgSetting;
    U8 scoCmdVer;
    //U8 scoHandle;
    U16 scoHandle;
    U8 Timing_control_flags;
    U8 Dsco;
    U8 Tsco;
    U8 pktTyp;
    U8 airMode;
    FOUR_BYTE_UNION_TYPE Initial_Instant_in_slot ;
    FOUR_BYTE_UNION_TYPE clk;
}LmScoInfoType;

typedef union
{
    U8 value;
    struct
    {
        BOOL    MayUse2EV3           : 1;
        BOOL    MayUse3EV3           : 1;
        BOOL    MayUseEV3            : 1;
        BOOL    MayUse2EV5           : 1;
        BOOL    MayUse3EV5           : 1;
        BOOL    MayUseEV5            : 1;
        BOOL    MayUseEV4            : 1;
        BOOL    reservedpkt_b7       : 1;

    } field;

    struct
    {
        U8     OneSlotPkt           : 3;
        U8     ThreeSlotsPkt        : 4;
        U8     Rsvd                 : 1;

    } slot;

} LM_ESCO_PACKET_USED_PRIORITY;

typedef struct
{
    U8 isEsco;
    U8 scoCtlArrayIdx;
    U8 existingJustChgSetting;
    U8 scoCmdVer;
    //U8 scoHandle; //lm sco handle
    U16 scoHandle;
    U8 Timing_control_flags;
    U8 Desco;
    U8 Tesco;
    U8 pktTypM2S;
    U8 airMode;
    FOUR_BYTE_UNION_TYPE Initial_Instant_in_slot;
    FOUR_BYTE_UNION_TYPE clk; //above parameters are in the same allignment of ScoInfoType to make it possible of taking advangtage of sco existing functions
    U8 escoLtaddr;
    U8 Wesco;
    U8 pktTypS2M;
    U16 pktLenM2S;
    U16 pktLenS2M;
    U8 negoState;
    U32 TxBandWidth;
    U32 RxBandWidth;
    U32 MaxLatency;
    U8 TxPacket;
    U8 RxPacket;
    U16 TxLen;
    U16 RxLen;
    U8 ReTxEffort;
    LM_ESCO_PACKET_USED_PRIORITY PktUsedPrior;
    U8 eSCONegoCnt;
}LmEScoInfoType;

typedef struct
{
    FOUR_BYTE_UNION_TYPE holdInstant;   //unit: clk in beheadset but slot in btdevice
    TWO_BYTE_UNION_TYPE holdTime;
}LmHoldInfoType;

typedef struct
{
    FOUR_BYTE_UNION_TYPE switchInstant;
    U16 slotOffset;
    U8 oldLinkInd;
    U8 newLinkInd;
    U8 redoEncryOn;
    U8 isRetry;
    U8 RetryCnt;
    U8 isSlotOffsetReceived;
    U8 AFH_Init_is_Unfinished;
}LmRSInfoType;

typedef struct
{
    FOUR_BYTE_UNION_TYPE initialInstant;
    U16 timeout;
    U8 lclSubrate;
    U8 rmtSubrate;
}LmSniffSubratType;

typedef struct
{
    U8 nameOffset;
    U8 nameLength;
    U8 XDATA_PTR remoteNameHeadPtr;

    #ifdef LC_TEST_CASE_CONFIG
    OST XDATA_PTR detachTimer;
    #endif
}LmRemoteNameInfoType;

typedef union
{
    LmSniffInfoType sniffInfo;
    LmHoldInfoType holdInfo;
    LmSniffSubratType subrateInfo;
    LmRemoteNameInfoType remoteNameInfo;
    OST XDATA_PTR contTimer;
}LmProcCtrlType;

typedef union
{
    LmEScoInfoType escoInfo;
    LmScoInfoType scoInfo;
}LmProcCtrlType2;

typedef struct
{
    U8 on;
    U16 maxLatency;
    U16 minRmtTimeout;
    U16 minLclTimeout;
}SniffSubratingInfoType;

typedef enum
{
    UNSUPPORTED_FEATURE,//NORMAL_POWER,
    NORMAL_POWER,//UNSUPPORTED_FEATURE,
    MAX_POWER,
    MIN_POWER
}LM_POWER_CONTROL_STATUS;

typedef enum
{
    NO_POWER_REQUEST,
    INCREASE_POWER,
    DECREASE_POWER
}LM_POWER_CONTROL_REQ_TYPE;

typedef struct
{
    S8 RSSIarray[RSSI_WINDOW_WIDTH];
    U8 pivot;
    U16 counter;
    S16 sum;
    LM_POWER_CONTROL_STATUS status;
    LM_POWER_CONTROL_REQ_TYPE lastRequest;
    OST XDATA_PTR waitingForNextSendingTimer;
    void (*StateEntry) (void);
    U8 state;
    U8 waitingMaxPowerBBack;
}PowerControlInfoType;

typedef struct
{
    U8 serviceType;
    U32 tokenRate;
    U32 peakBandwidth;
    U32 latency;
    U32 delayVariation;
    U8 FlowStatus;
    U8 RxFull;
}QosSetupInfoType;

#ifdef SUPPORT_SET_WAITED_COMMAND
    #define MAX_WAITED_CMDS 4
#endif

typedef struct
{
    #if (FEA_NEW_CONNHDL)
    CONNHDL_CTL_STRU XDATA_PTR ConnHdlPtr;
    U8 LinkIdx;
    #endif
    LM_STABLE_STATE lm_state;
    LM_TESTMODE_PARAM testmode_ctl;
    LM_AFH_PARAM afh_ctl;
    U8 MaxSlotReq_ctl;
    #if 0
    LM_ACLU_PARAM aclu_ctl;
    #endif
    U16 Tsniff;
    U32 sniffInstant;
    U8 last_error_code;
    U8 errToLocal;
    U8 errToRemote;
    U8 testmode_type;
    REPEATED_ATTEMPT_INFO_TYPE repeat_attempt_info_type;
    LM_ENCRYPTION_HANDLE_INFO encryption_info;
    U8 version;
#ifdef SUPVIS_TIMEOUT_BY_LM
    #if 0
    U8 lc_addressed;
    #endif
#endif
    LmPktType pktType;
    LmProcCtrlType procCtrlInfo;
    LmProcCtrlType2 procCtrlInfo2;
    LmRSInfoType rsInfo;
    QosSetupInfoType qosSetupInfo;
    PowerControlInfoType powerCtrlInfo;
    SniffSubratingInfoType subratingInfo;
    //When adding timer pointer, we should check the timer memory is released in LM_ClearLinkInfo.
    //When using OST_PutTimer, we should set the timer pointer to NULL after the timer memory is released.
    OST XDATA_PTR waitTimer;
    U8 wait_state_opcode;

    /* Expired Times */
    BTCLK_UNION AudioCtlExpiredTime;
    BTCLK_UNION TestModeTransactionTimeoutExpiredTime;
    BTCLK_UNION DetachTimeoutExpiredTime;
    BTCLK_UNION AuthWaitingIntervalExpiredTime;
    BTCLK_UNION ConnectionSetupExpiredTime;
#ifdef SUPVIS_TIMEOUT_BY_LM
    BTCLK_UNION PollChk4SlaveExpiredTime;
    BTCLK_UNION SupervisionExpiredTime;
#endif

    /* Timeout Values */
    BTCLK_UNION SupervisionTimeout;

    //poll checking timer
#ifdef SUPVIS_TIMEOUT_BY_LM
    U8 searchExtLevel;
#endif
    //for qualification - hci event & lmp timing issue
    #ifdef LC_TEST_CASE_CONFIG
    OST XDATA_PTR hciEvt2SendTimer;
    #endif

    U8 last_top_state;
    U8 last_sub_state;
    U32 currentQosLatency;

    #ifdef ACT_FUNCTION
    U8 sending_disconnected_command;
    #endif
    U8 NameTransOngoing;

    #ifdef SUPPORT_SET_WAITED_COMMAND
    //Added by Eric Chou, 09.5.15
    U8 waitedCmds[MAX_WAITED_CMDS];
    #endif

    U8 not_anytimePDU : 1;                          //To guard au_rand any-time or not
    U8 active_llSit : 1;                            //True: runSitPtr = llSit2; False: runSitPtr = llSit
    U8 llSit_act_sniff_proc : 1;                    //True: There is an active sniff process in llSit;
    U8 reserved : 5;
    U8 PasAuthDone;
    U8 check_auth : 1;
    U8 ActSniffNego_counter;
    U8 PasSniffNego_counter;
	U8 MaxSlot_Sent;
}LM_CONNECTION_HANDLE_INFO;

#define LM_MAX_STATE_INFO_TBL_NO (MAX_NUM_ACL_LINK/2*7)
typedef struct Sit_t
{
        struct Sit_t XDATA_PTR prevSitPtr; //previous nest state pointer in the link list
        U8 top_state;
        U8 sub_state;
        U8 activate;
        U8 XDATA_PTR trigger;
        U8 cmpltdTrans;     //complicated transition, i.e., more than two messages back and forth between two ends of lm
        U8 needChkCollision;
    #ifdef LM_COLLISION_RECOVERY
        U8 isAclOff;
    #endif
        OST XDATA_PTR general_transaction_timeout_timer_ptr;
        U8 initOpcodeTableIndex;
}LmStateInfoTblType;

typedef enum
{
    DETACH_NONE,
    DETACH_ACL = 0X01,
    DETACH_SCO = 0X02,
    DETACH_ACLNSCO = 0X03
}LM_DETACH_TYPE;

typedef struct
{
    U8  lk2mk[MAX_NUM_ACL_LINK];
    U8  temp_rand[16];
    U8  temp_key[16];
    U16 broadcast_key_size_mask_res[MAX_NUM_ACL_LINK];
    U16 broadcast_key_size_mask;
    U8  start_enc_para[16];
}LM_MASTER_KEY_INFO;

//LM nest state in ring data structure
typedef enum PktType_e
{
    ENUM_DM1,
    ENUM_DH1,
    ENUM_DM3,
    ENUM_DH3,
    ENUM_DM5,
    ENUM_DH5,
    ENUM_2DH1,
    ENUM_3DH1,
    ENUM_2DH3,
    ENUM_3DH3,
    ENUM_2DH5,
    ENUM_3DH5,
    ENUM_PKT_TPE_TOTALNO
}EnumPktType;

typedef struct
{
    LmStateInfoTblType XDATA_PTR runSitPtr; //the running State Info Table pointer
    LmStateInfoTblType XDATA_PTR llSit[MAX_NUM_ACL_LINK]; //link list of sit of acl links
    LmStateInfoTblType XDATA_PTR llSit2[MAX_NUM_ACL_LINK]; //for concurrent process, avoid going to collision process

    LmStateInfoTblType noneLinkSit;

//  LmRingStateInfoTblType XDATA_PTR runRingStatePtr;
//  LmRingStateInfoTblType ringStateInfo[MAX_NUM_ACL_LINK];

    LM_DETACH_TYPE detachType[MAX_NUM_ACL_LINK];

    LM_SCHEDULER sche_ctl;

    #ifdef SUPPORT_SCO_ESCO
    //The value may be 0, 1, 2 in each link. By the way the summation of all sco link is 3.
    U8 detachScoLink[MAX_NUM_ACL_LINK];
    LM_SCO_PARAM    sco_ctl[MAX_NUM_SCO_LINK];
    U8 scoNo; //total sco no
    U8 scoNoByLink[MAX_NUM_ACL_LINK]; //sco no of each acl link
    U8 activeScoCtlIdx; // the active sco control block index - only one can be actived due to codec capaiblity
    #endif
    U8 auth_resource_in_use;
    LM_MASTER_KEY_INFO master_key_ctl;

#ifdef SUPPORT_SIMPLE_PAIRING
    SIMPLE_PAIRING_INFO sp_ctl;
#endif
    U16 AFH_LM_Processing;

    U8 Baseband_Page_Inquiry_Ongoing;
    U8 lmCreateConnectionCnl;
    //added for packet type change
    //U8 pktChgConnHandle;
    U16 pktChgConnHandle;
    OST XDATA_PTR waitTimer;
    U8 lookup_table[GENERAL_LMP_PDU_NUMBER+EXT_LMP_PDU_NUMBER+1];//zero is not include
    U16 pwrSavingRecords;
    BD_NAME XDATA_PTR local_name_ptr;
} LM_CONTROL_TYPE;

/////////////////////////////////////////////////////////////////////////////
//- runtime allocate mem block OSMEM_PTR2
//- sizeof OSMEM_PTR2 is 390
//- size of LM_INQUIRY_CONTROL_TYPE MUST < OSMEM_PTR2
//- """"""""""DON'T MODIFY LM_MAX_INQUIRY_COUNT 50"""""
#define LM_MAX_INQUIRY_COUNT        50
typedef struct
{
    U8  bd_addr[6];
}LM_INQUIRY_INFO_TYPE;

typedef struct
{
    U8                      exit_state;
    U8                      num_responses;
    U8                      num_inquiry_stored;
    LM_INQUIRY_INFO_TYPE    inquiry_info[LM_MAX_INQUIRY_COUNT];
    OST XDATA_PTR           inquiry_window_timer_ptr;
}LM_INQUIRY_CONTROL_TYPE;
////////////////////////////////////////////////////////////////////////////

typedef struct
{
    U8 eSCO_handle;
    U8 eSCO_LT_ADDR;
    FOUR_BYTE_UNION_TYPE Initial_Instant_in_slot;
    U8 interval;
    U8 w_esco;
    U8 m2s_packet_type;
    U8 s2m_packet_type;
    U8 m2s_packet_len_B0;
    U8 m2s_packet_len_B1;
    U8 s2m_packet_len_B0;
    U8 s2m_packet_len_B1;
    U8 air_mode;
} LcEScoInfoType;

//Added by Eric, 2009.2.16
//For LM_CheckFeatures.c
#ifdef LM_AUTO_NOT_ACCEPT
typedef struct
{
    U8 escape;
    U8 opCode;
    SUPPORT_FEATURES features;
    U8 errorCode;
} LMFeaturesOpCodeMap;
#endif

//Global variables
LM_EXTERN LM_CONTROL_TYPE XDATA gLM_ctl;
LM_EXTERN U8 XDATA   gLM_rs_going[MAX_NUM_ACL_LINK];
LM_EXTERN LM_CONNECTION_HANDLE_INFO XDATA   gLM_link_info[MAX_NUM_ACL_LINK];
EXTERN LM_CONNECTION_HANDLE_INFO_ROM XDATA   gLM_link_info_rom[MAX_NUM_ACL_LINK];

//- LM.C
PUBLIC void LM (void);
PUBLIC void LM_Init (void);
PUBLIC void LM_LmpAccepted(U8 op_code, U8 misc_flag, U8 tid);
PUBLIC void LM_LmpNotAccepted(U8 op_code, U8 err_code, U8 tid);
PUBLIC void LM_TxHciEvtCmdStatus(U16 OpCode, U8 err_report);
PUBLIC void LM_TxPDUWithoutContent(U8 op_code, U8 misc_flag, U8 tid);
PUBLIC void LM_TxPDUWithOneContent(U8 opcode, U8 misc_flag, U8 tid, U8 content);
PUBLIC void LM_SendHcModeChgEvt(U8 mode, U16 interval);
PUBLIC void LM_MaintainSupervisionTimeout (void);
PUBLIC U8 XDATA_PTR LM_AllocateLCData(U8 length);
PUBLIC BOOL LM_WaitBBAckForLMP (U8 op_code, U8 escape, U8 accepted_parameter);
PUBLIC U8   LM_GetMasterEmptyLink (U8 forRS);
PUBLIC U8   LM_GetMappingIdxFromBDAddr (U8 XDATA_PTR ptr_addr1);
PUBLIC void LM_SendHcConnCpl(U8 status);
PUBLIC void LM_SendHcDiscCpl(U16 connnHandle, U8 status, U8 errCode);
PUBLIC LmStateInfoTblType XDATA_PTR LM_AddNestState(U8 top, U8 sub, U8 act);
PUBLIC void LM_ClearSit(LmStateInfoTblType XDATA_PTR sitPtr);
PUBLIC void LM_ClearSitProce(LmStateInfoTblType XDATA_PTR clearSitPtr/*, BOOL isSubProce*/);
PUBLIC void LM_ClearSpecifiedNestState(LmStateInfoTblType XDATA_PTR sitPtr);
PUBLIC void LM_ClearNestState(void);
PUBLIC void LM_ClearNestStateProce(void);
PUBLIC void LM_ClearNoneLinkNestState(void);
PUBLIC void LM_LmpAcceptedExt(U8 op_code, U8 misc_flag, U8 tid);
PUBLIC void LM_LmpNotAcceptedExt(U8 op_code, U8 err_code, U8 tid);
PUBLIC U8 XDATA_PTR LM_GetHCIEvtMemory(U8 hciOpCode, U8 length);
PUBLIC U8 XDATA_PTR LM_GetHCICmdCplEvtMemory(U8 length, U16 OpCode);
PUBLIC void LM_SendLcDiscSco (U8 scoLinkInd);
PUBLIC void LM_OLinksMaxslot(void);
VOID LM_MaxSlotReqEntry(VOID);
VOID LM_MaxSlotReqScheduler(VOID);
PUBLIC void LM_SendDetach(void);
PUBLIC void LM_SendSetupCpl(void);
PUBLIC void LM_SendMaxslot(void);
PUBLIC void LM_SendMaxslotReq(U8 maxReqSlot);
PUBLIC void LM_SendRmvScoReq(void);
//PUBLIC void LM_SendFeatReq(void);
PUBLIC U32  LM_GetClkInSlot(void);
PUBLIC U32  LM_GetClk(void);
PUBLIC U32  LM_GetClkByLink(IN U8 LinkIdx);
PUBLIC U8   LM_GetTimingControlFlag(U8 b3);
U8   LM_EScoNegotiation(U8 XDATA_PTR scoReq, LmEScoInfoType XDATA_PTR escoInfoPtr);
PUBLIC void LM_ClkInitAdj(U8 timingCtrlFlag,    FOUR_BYTE_UNION_TYPE XDATA_PTR clk);
PUBLIC void LM_ReadEScoLmpPara(U8 XDATA_PTR ptr_msg, LmEScoInfoType XDATA_PTR escoInfoPtr);
PUBLIC void LM_SendEScoReq(LmEScoInfoType XDATA_PTR escoInfoPtr, U8 tid);
PUBLIC void LM_ReadScoLmpPara(U8 XDATA_PTR ptr_msg, LmScoInfoType XDATA_PTR scoInfoPtr);
PUBLIC void LM_SendScoReq(LmScoInfoType XDATA_PTR scoInfoPtr, U8 tid);
PUBLIC void LM_CalScoInitInst(LmScoInfoType XDATA_PTR scoInfoPtr);
PUBLIC void LM_SendLcAddSco(LmScoInfoType XDATA_PTR scoInfoPtr);
PUBLIC U8   LM_GetLcPktType (U8 packet_type);
PUBLIC void LM_SendLcAddESco(LmEScoInfoType XDATA_PTR escoInfoPtr);
PUBLIC BOOL LM_SetPktTypeTbl(U8 XDATA_PTR pktType);
PUBLIC U8   LM_GetErrCode(void);    //To get and reset error code
PUBLIC void LM_SetErrCode(U8 error_code);
PUBLIC void LM_PasHoldModeReq (void);
PUBLIC void LM_PasHoldMode (void);
PUBLIC void LM_ActHoldModeReq (void);
PUBLIC void LM_ActHoldMode (void);
PUBLIC void LM_ExitHoldMode(void);
PUBLIC void LM_PasSwitchReq (void);
PUBLIC void LM_ClearLinkMQ (U8 link_idx);
PUBLIC void LM_ClearLinkInfo(void);
PUBLIC void LM_HciSwitchHandler (void);
PUBLIC void LM_ActSwitchProc (void);
PUBLIC void LM_SendHcSlotChg(void);
PUBLIC void LM_PasPageModeHandler(void);
PUBLIC void LM_SendLcDetach(void);
PUBLIC void LM_SendLcMaxslotChg(U8 linkInd);
PUBLIC void LM_SendRmvEScoReq(void);
PUBLIC void LM_SendHcSyncConnCpl(U8 scoType, LmEScoInfoType XDATA_PTR escoInfoPtr);
PUBLIC U16   LM_SetSco(U8 activity, U16 lmScoHandle, U8 XDATA_PTR dScoPtr);
PUBLIC void LM_ActSniffSubrating (void);
PUBLIC void LM_PasSniffSubrating (void);
PUBLIC U8   LM_SearchScoViaAclHandle(U8 aclHandle);
PUBLIC U8   LM_HaveActiveScoOrEsco(void);
PUBLIC U8   LM_SearchScoViaScoHandle(U16 scoHandle);
U8   LM_GetMaxSlotInScoMode(VOID);
PUBLIC void LM_UpdateLmpVer (U8 XDATA_PTR ptr_msg);
PUBLIC void LM_UpdateLmpFeature (U8 XDATA_PTR ptr_msg);
#ifdef SUPVIS_TIMEOUT_BY_LM
PUBLIC void LM_SetSupvisTimer (void);
#endif
PUBLIC void LM_SendHcReSegmentReq(U16 linkInd);
PUBLIC void LM_ReSegProc(void);
PUBLIC void LM_ActClkOffset(void);
PUBLIC void LM_SetScoPktInfo(LmScoInfoType XDATA_PTR scoInfoPtr);
PUBLIC void LM_SendHcSyncChgEvt(LmEScoInfoType XDATA_PTR escoInfoPtr);
PUBLIC void LM_PasQosHandler(U8 XDATA_PTR qosPtr);
PUBLIC void LM_LinkCurrSit2List(LmStateInfoTblType XDATA_PTR sitPtr, U8 linkIdx);
PUBLIC void LM_LinkCurrSit2List2(LmStateInfoTblType XDATA_PTR sitPtr, U8 linkIdx);
PUBLIC void LM_LinkNestState(U8 top, U8 sub, U8 act);
PUBLIC void LM_LinkNestState2(U8 top, U8 sub, U8 act);
PUBLIC void LM_FreeSit(LmStateInfoTblType XDATA_PTR sitPtr);
PUBLIC void LM_FreeLlsit(U8 linkIdx);
PUBLIC void LM_FreeLlsit2(U8 linkIdx);
PUBLIC void LM_SendErrCplToHC(U8 activate, U8 hciCmdOpcode);
PUBLIC void LM_SendSupervisionTimeout (U8 link_idx);
PUBLIC void SwitchAudioViaLm(void);
PUBLIC void ESCOTxNullPacketCtl(void);
PUBLIC U8   IsLmAclOn(U8 linkIdx);
PUBLIC void LM_WaitRemoteLmpProc(U16 halfTimeSlot, U8 OpCode, U8 isExt);
PUBLIC void SetGeneralTransactionTimer(void);
PUBLIC U8   ChkEScoAvailabllity(U8 pktType);
PUBLIC void LM_SET_PWR_SAVING(BOOL isEnablePwrSaving);
PUBLIC LmStateInfoTblType XDATA_PTR LM_SearchFirstState();
PUBLIC void LM_MoveStateToTop(LmStateInfoTblType XDATA_PTR statePtr);
PUBLIC LmStateInfoTblType XDATA_PTR LM_GetStatePtr(U8 state, BOOL ext, U8 activate);
PUBLIC U8   LM_AssignedReducedConnectionHandle(void);
PUBLIC BOOL IsSniffAcceptible(void);
PUBLIC void LM_SetLocalNamePtr(BD_NAME XDATA_PTR local_name_ptr);
#ifdef SUPVIS_TIMEOUT_BY_LC
void LM_SendLcSupVisTimeoutValue(BTCLK_UNION XDATA_PTR supvisTOInfoPtr);
#endif
PUBLIC U8 LM_DataGetTID(U8 XDATA_PTR ptr);

#ifdef SUPPORT_SET_WAITED_COMMAND
//Added by Eric Chou, 09.5.19
//Waiting a specific command
PUBLIC BOOL LM_CheckWaitedCmdTable(U8 opCode, U8 linkIndex);
PUBLIC void LM_AddWaitedCmd(U8 opCode, U8 linkIndex);
PUBLIC void LM_ClearWaitedCmdTable(U8 linkIndex);
#endif

#ifdef SUPPORT_SCO_ESCO
PUBLIC void LM_ChgScoSetting(void);
PUBLIC void LM_AudioOnProc(void);
PUBLIC void LM_AudioOffProc(void);
U8 LM_GetVacantLtaddr(void);
#endif

PUBLIC void LM_TxExtPDUWithoutContent(U8 ext_opcode, U8 misc_flag, U8 tid);

//calculate offsets of eSCO and sniff
U32 LM_CalInstant(U16 interval, U32 inst_in_slot);
U16 LM_GetESCOSniffOffset (U8 is_sniff,U8 timing_control_flags, U16 interval);
U16 LM_CalOffsetForThisAnchor (U16 interval, U8 timing_control_flags, U32 inst_in_slot);
U16 LM_GetDsniff(U8 LinkIdx,U8 timing_control_flags, U16 interval);
U16 LM_GetDeSCO(U8 LinkIdx,U8 timing_control_flags, U16 interval);


#ifdef SUPPORT_POWER_CTRL_REQUEST
//Power Control
PUBLIC void LM_SendIncreasePowerReq();
PUBLIC void LM_SendDecreasePowerReq();
PUBLIC void LM_PowerControl();
#endif

//Multi-link related in LM.c
PUBLIC void SwitchLink(U8 linkIdx);
PUBLIC void Switch2AddingLink(U8 linkInd);

//LM_InformationRequest.c
PUBLIC void LM_RemoteNameReq(void);
U8 LM_SendHcRemoteNameReqCancelCpl(HC_CMD_MSG_STRU_PTR pCmdMsg, U8 Status);
PUBLIC void LM_SupportedFeatureReq(void);
PUBLIC void LM_ExtendedFeaturesReq(void);
PUBLIC void LM_RemoteVersionReq(void);
PUBLIC U16 GetClockOffset();
PUBLIC BOOL CheckSupportedFeature(U8 bitNumber);
//PUBLIC void LM_SendHcRemoteHostSupportedFeatureNotiEvt(U8 featurePageNumber);
PUBLIC U16 LM_EscoDecideTW(U8 pktType); /*return T in MSB, W in LSB*/
U8 LM_EscoGetRsvdSlotsBySelPriorPkt(
    IN      U8 PktTypePrior);
U16 LM_EscoGetMaxPktLenBySelPriorPkt(
    IN      U8 PktTypePrior);

    //LM_QoS.c
VOID LM_QosSetup(VOID);
PUBLIC void LM_SendQosLmp();

//Send PDU, many places use these functions.
PUBLIC void LM_SendSupportedFeaturesRequestPDU();
PUBLIC void LM_SendNameRequestPDU();
PUBLIC void LM_SendExtendedFeaturesReqPDU(U8 featurePageNumber);

#if 0
PUBLIC void LM_LmpPtruHandler(U8 XDATA_PTR msgPtr);
#endif

#define LM_MODE_ACTIVE 0x00
#define LM_MODE_HOLD   0x01
#define LM_MODE_SNIFF  0x02
#define LM_MODE_PARK   0x03

typedef U8 LM_POWER_CONTROL_ADJUSTMENT_REQ;
#define LM_POWER_CONTROL_ADJUSTMENT_REQ_DEC_ONE_STEP    ((LM_POWER_CONTROL_ADJUSTMENT_REQ)0)
#define LM_POWER_CONTROL_ADJUSTMENT_REQ_INC_ONE_STEP    ((LM_POWER_CONTROL_ADJUSTMENT_REQ)1)
#define LM_POWER_CONTROL_ADJUSTMENT_REQ_INC_TO_MAX      ((LM_POWER_CONTROL_ADJUSTMENT_REQ)2)

typedef U8 LM_POWER_CONTROL_ADJUSTMENT_RES;
#define LM_POWER_CONTROL_ADJUSTMENT_RES_NOT_SUPPORT     ((LM_POWER_CONTROL_ADJUSTMENT_RES)0)
#define LM_POWER_CONTROL_ADJUSTMENT_RES_CHANGE_ONE_STEP ((LM_POWER_CONTROL_ADJUSTMENT_RES)1)
#define LM_POWER_CONTROL_ADJUSTMENT_RES_MAX_POWER       ((LM_POWER_CONTROL_ADJUSTMENT_RES)2)
#define LM_POWER_CONTROL_ADJUSTMENT_RES_MIN_POWER       ((LM_POWER_CONTROL_ADJUSTMENT_RES)3)


typedef union u_lm_power_ctl_res
{
    U8 value;

    struct
    {
        U8 Gfsk_status : 2;
        U8 Dqpsk_status : 2;
        U8 Dpsk8_status : 2;
        U8 Rsvd : 2;
    }group;
} LM_POWER_ADJ_RES_UNION, OS_PTR LM_POWER_ADJ_RES_UNION_PTR;

//LM_ANYTIMEPDU.C
PUBLIC void LM_AnyTimePDU_clkoffset_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_clkoffset_res(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_features_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_features_res(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_features_req_ext(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_features_res_ext(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_timing_accuracy_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_timing_accuracy_res(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_name_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_name_res(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_version_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_version_res(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_supervision_timeout(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_max_slot_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_incr_power_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_decr_power_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_power_ctl_req (U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_channel_classificaion_req(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_channel_classificaion(U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_AnyTimePDU_slot_offset(U8 XDATA_PTR lmp_ptr);
PUBLIC U8  LM_AllowedMaxSlotNo(U8 link_index);
PUBLIC void LM_PasMaxSlotProc(void);
VOID LM_TimingAccuracyProc(VOID);
PUBLIC void LM_SendHcAcluReq(void);
PUBLIC void LM_SendLcAcluReq(U8 isGo, U8 edrOn);
PUBLIC void LM_SendHcConnReq(U8 linkType);
PUBLIC U8 XDATA_PTR LM_AllocLcCmd(U8 opCode);
PUBLIC BOOL LM_ChkInstPassed(FOUR_BYTE_UNION_TYPE XDATA_PTR nClkPtr);
PUBLIC void LM_AnyTimePDU_autorate(void);

//Role Switch
PUBLIC void LM_SendHcRoleChgEvt(U8 status);
PUBLIC void LM_SRCheckRedoAFH(void);

//- LM_AFH.c
PUBLIC void LM_AFHUpdateEntry (void);
PUBLIC void LM_AFHScheduler (void);
PUBLIC BOOL LM_MasterAFHInit (void);
PUBLIC BOOL LM_SlaveAFHInit (U8 conn_idx);
PUBLIC void LM_MasterSendLMPSetAFH(U8 mode);
PUBLIC void LM_SlaveGetLMPSetAFH (U8 XDATA_PTR lmp_ptr);
PUBLIC void LM_SendLMCMDSetAFH (U8 acked, U8 mode);
PUBLIC void LM_MasterSendLMPChannelClassificationReq (U8 enable);
PUBLIC void LM_SlaveSendLMPChannelClassification (void);
PUBLIC void LM_AFHClassification2BitMap (U8 XDATA_PTR in, U8 XDATA_PTR out);
PUBLIC void LM_SetAFHChannelClassification(U8 XDATA_PTR ptr_cmd_from_HC);
PUBLIC void LM_AFHFlagClear (void);
PUBLIC void LM_AFHLowDutySignalDetect(U8 status);
//PUBLIC void LM_ReadAFHChannelMap (void);

PUBLIC void LM_PasCreateConn (void);
PUBLIC void LM_TestmodeEntry (void);

//- LM_Detach.C
PUBLIC void LM_ActDetach (void);
PUBLIC void LM_PasDetach (void);
PUBLIC void LM_ActDiscAcl(void);
PUBLIC void LM_PasDiscSco(void);
PUBLIC void LM_ExtendedSynchronousDisconnectionEntry (void);
PUBLIC void LM_ActSniffMode (void);
PUBLIC void LM_ActExitSniffMode (void);
PUBLIC void LM_PasSniffMode (void);
PUBLIC void LM_PasExitSniffMode (void);

PUBLIC void LM_ExitSniffTransForCollision();

//- Sniff
PUBLIC void LM_SendHcSubratingEvt(LmSniffSubratType XDATA_PTR subrateInfoPtr);

//- General
PUBLIC void LM_CmdComplEVT(U16 OpCode, U8 type);
PUBLIC void LM_SendBDAddrToHC(U8 op_code);

typedef enum
{
    STATUSONLY,
    STATUS_BDADDR,
    STATUS_CONNECTIONHANDLE,
    STATUS_LINK_POLICY,
    STATUS_DEFAULT_LINK_POLICY,
    STATUS_ROLE_DISCOVERY,
    STATUS_ONLY_OK,
}CmdComplEvtType;

//- LM_AUTHENC.C
PUBLIC BOOL LM_CheckBDAddr(U8 XDATA_PTR ptr_param);
PUBLIC void LM_Tx16BytesNum(U8 bb_ack, U8 op_code, U8 XDATA_PTR para_ptr);
PUBLIC U8 LM_RxAccept(U8 expected_para);        //0->Not recv, 1->Rx Accept, 2->Rx Not_accept, 3->Other PDU
PUBLIC U8 LM_GetTID(void);
PUBLIC void LM_SendLinkKeyNotificationToHC(U8 key_type);
PUBLIC void LM_TxAuRand(void);
PUBLIC void LM_SendARComplToHC(void);
PUBLIC void LM_SendPINCodeRequestToHC(void);
PUBLIC void LM_SendLinkKeyRequestToHC(void);

PUBLIC void LM_SetKeyInfo(U8 key_info);
PUBLIC U8   LM_GetKeyInfo(void);
PUBLIC void LM_SetEncPara(U8 enc_para);
PUBLIC U8   LM_GetEncPara(void);

PUBLIC void LM_Act_Auth (void);
PUBLIC void LM_Pas_Auth (void);
PUBLIC void LM_Pas_Pairing (void);

PUBLIC void LM_ActChangeConnectionLK (void);
PUBLIC void LM_PasChangeConnectionLK (void);

PUBLIC void LM_Master_LKMKSwitch(void);
PUBLIC void LM_Slave_LKMKSwitch(void);
PUBLIC void LM_MasterEncKeySizeMask(void);
PUBLIC void LM_SlaveEncKeySizeMask(void);
//PUBLIC void LM_Slave_MK2LKEntry(void);

//LinkKey
PUBLIC void LM_SendMasterLKComplEVT(void);
PUBLIC void LM_SendCCLKComplToHC(void);

//Encryption
//PUBLIC void LM_StopEncryptionEntry (void);
PUBLIC void LM_SlaveEncModeReq (void);
PUBLIC void LM_MasterEncModeReq (void);
PUBLIC void LM_SendEncryptionChangeEvtToHC(void);

//PUBLIC void LM_SlaveEncStartReq (void);

PUBLIC void LM_SlavePauseEnc(void);
PUBLIC void LM_MasterPauseEnc(void);
PUBLIC void LM_SlaveResumeEnc(void);
PUBLIC void LM_MasterResumeEnc(void);
PUBLIC void LM_ActEnc(U8 encOnOff);
PUBLIC void LM_RefreshEncKey(void);

//- Simple Pairing
PUBLIC void LM_IOCapExchange(void);
PUBLIC void LM_PKExchange(void);
PUBLIC void NumericComparison(void);
PUBLIC void OOB(void);
PUBLIC void PassKey(void);
PUBLIC void LM_DHKey_Checks(void);
PUBLIC void LM_ReadLocalOOBData(void);
PUBLIC void LM_SendSimplePairingComplToHC(void);
PUBLIC void LM_EndOfSP(void);
#ifdef SUPPORT_SIMPLE_PAIRING
//Added by Eric, 2009.3.24
//Transform Little-Endian to Big-Endian and vice versa.
PUBLIC U8 XDATA_PTR TransformEndian(U8 XDATA_PTR destPtr, U8 XDATA_PTR sourcePtr, U8 length);
#endif

//- LM_SECURITY.C
PUBLIC void LM_E1( U8 XDATA_PTR rand_input, U8 XDATA_PTR key_input);
PUBLIC void LM_E21( U8 XDATA_PTR rand);
PUBLIC void LM_E22( U8 XDATA_PTR rand, U8 XDATA_PTR PIN, U8 PIN_L);
PUBLIC void LM_E3 (U8 XDATA_PTR rand_input, U8 XDATA_PTR key_input, U8 XDATA_PTR aco_input);
PUBLIC void LM_PreE0 (U8 XDATA_PTR key_input, U8 key_length);
PUBLIC void LM_RAND_GEN(U8 XDATA_PTR input);
PUBLIC U32  LM_g(U8 XDATA_PTR U, U8 XDATA_PTR V, U8 XDATA_PTR X, U8 XDATA_PTR Y) compact;
PUBLIC void LM_f1(U8 XDATA_PTR C, U8 XDATA_PTR U, U8 XDATA_PTR V, U8 XDATA_PTR X, U8 Z) compact;
PUBLIC void LM_f2(U8 XDATA_PTR C, U8 XDATA_PTR W, U8 XDATA_PTR N1, U8 XDATA_PTR N2, /*U8 XDATA_PTR KeyID,*/ U8 XDATA_PTR A1, U8 XDATA_PTR A2) compact;
PUBLIC void LM_f3(U8 XDATA_PTR C, U8 XDATA_PTR W, U8 XDATA_PTR N1, U8 XDATA_PTR N2, U8 XDATA_PTR R, U8 XDATA_PTR IOcap, U8 XDATA_PTR A1, U8 XDATA_PTR A2) compact;
PUBLIC void LM_SHA256(  U8 XDATA_PTR digest_of_msg, U8 length_of_msg, U8 XDATA_PTR ptr_msg);
//- LM_INQUIRY.C
#ifdef INQUIRY_SUPPORT
VOID LM_InquiryEntry (void);
PUBLIC void LM_InquiryCancelEntry (void);
#endif

//- LM_CREATECONNECTION.C
PUBLIC void LM_CreateConnection (void);
PUBLIC void LM_SendVersionInfoPDU(U8 opCode, U8 tid);
PUBLIC void LM_SendLcPageCmd(HCI_CMD_CREATE_CONN_STRU EDATA_PTR pHciCmd,CONN_HDL_STRU_PTR pConnHdl);
PUBLIC void LM_SendLcPageCnl (void);
//#define LM_SendHcConnCnlCmdCpl() LM_CmdComplEVT(HCI_CMD_CREATE_CONNECTION_CANCEL, OGF_1, STATUS_BDADDR)
U8 LM_SendHcConnCnlCmdCpl(HC_CMD_MSG_STRU_PTR pCmdMsg, U8 Status);


//- LM_SYNCHRONOUSCONNECTIONSETUP.C
PUBLIC void LM_PasCreateSco(void);
PUBLIC void LM_PasCreateESco (void);


//- LM_CREATESYNCHRONOUSCONNECTION.C
PUBLIC void LM_ActCreateSco(void);
PUBLIC void LM_SynchronousDisconnection (void);
PUBLIC U8   LM_EscoPktTypeGetTimeSlots(U8 PktType);
//- LM_PacketTypeChange.c
PUBLIC void LM_PasPacketTypeProc (void);
#ifndef HID_SOC
PUBLIC void LM_ActPacketTypeProc (void);
#endif
PUBLIC void LM_SendHcPktChgEvt(U8 status, U16 pktType);


//- LM_VCMD.c
PUBLIC void LM_EnableAFH(void);
PUBLIC void LM_SendAFHChangeEvtToHC(U8 enable);

//- LM_CheckFeatures.c
#ifdef LM_AUTO_NOT_ACCEPT
    PUBLIC BOOL LM_CheckRemoteCmdIsSupported(U8 XDATA_PTR msgPtr);
#endif

PUBLIC void LM_SendPduToRemote(U8 XDATA_PTR pdu_ptr, U8 opcode, BOOL isExt);
PUBLIC void LM_SendPduToRemoteWithoutTransactionTimer(U8 XDATA_PTR pdu_ptr);

void LM_SendLcUnsniffCmd();
void LM_PrepareSniff(LmSniffInfoType XDATA_PTR sniffInfoPtr, U8 state);
void LM_SendLcUnsniffTransCmd();
void LM_SendLcSniffTransCmd(LmSniffInfoType XDATA_PTR sniffInfoPtr);

U8 LM_HcCmdFirstParaBdAddr(HC_CMD_MSG_STRU_PTR pCmdMsg);




//-LM.c
extern VOID LM_TxMaxSlotByLink(U8 tid, U8 content, U8 linkInd);


//sco content format of hci cmd
#define AIRFMT_VER20_CVSD 0x0000
#define AIRFMT_VER20_ULAW 0x0001
#define AIRFMT_VER20_ALAW 0x0002
#define AIRFMT_VER20_TRANSDATA 0x0003

//sco packet type of hci cmd
#define HCIPKTYPE_VER11_HV1 0x0020
#define HCIPKTYPE_VER11_HV2 0x0040
#define HCIPKTYPE_VER11_HV3 0x0080


#define HCIPKTYPE_VER20_HV1 0x0001
#define HCIPKTYPE_VER20_HV2 0x0002
#define HCIPKTYPE_VER20_HV3 0x0004
#define HCIPKTYPE_VER20_EV3 0x0008
#define HCIPKTYPE_VER20_EV4 0x0010
#define HCIPKTYPE_VER20_EV5 0x0020
#define HCIPKTYPE_VER20_2EV3 0x0040
#define HCIPKTYPE_VER20_3EV3 0x0080
#define HCIPKTYPE_VER20_2EV5 0x0100
#define HCIPKTYPE_VER20_3EV5 0x0200
//Airoha specified cmds
#define HCIPKTYPE_AIROHA_PKT1 0X0400 //M2S: 2EV3, S2M: 2EV5
#define HCIPKTYPE_AIROHA_PKT_2EV5_FOR_CELT 0X0800 //for Airoha_Line_in_Woofer_Celt(48K):Line-in
#define HCIPKTYPE_AIROHA_PKT_20SLOTS 0x1000 //for Airoha_BT_Line_in_Woofer(2K):Line-in + page scan
#define HCIPKTYPE_AIROHA_PKT_6SLOTS 0X2000 //for Airoha_Line_in_Woofer(8K):Line-in + No A2DP
#define HCIPKTYPE_AIROHA_PKT_16SLOTS 0x4000 //for Airoha_BT_Line_in_Woofer(2K):Line-in + Drop A2DP
#define HCIPKTYPE_AIROHA_PKT_24SLOTS 0x8000 //for Airoha_BT_Line_in_Woofer(2K):Line-in + Play A2DP
//Airoha eSCO packet type setting
#define Airoha_Line_in_Woofer       1  //2-EV3 len:60 interval:6
#define Airoha_BT_Line_in_Woofer    2  //2-EV3 len:60 interval:24

// SCO/eSCO Packet Selection
#define LM_ESCO_PACKET_PRIORITY_1                      ((U8)1 << 0)
#define LM_ESCO_PACKET_PRIORITY_2                      ((U8)1 << 1)
#define LM_ESCO_PACKET_PRIORITY_3                      ((U8)1 << 2)
#define LM_ESCO_PACKET_PRIORITY_4                      ((U8)1 << 3)
#define LM_ESCO_PACKET_PRIORITY_5                      ((U8)1 << 4)
#define LM_ESCO_PACKET_PRIORITY_6                      ((U8)1 << 5)
#define LM_ESCO_PACKET_PRIORITY_7                      ((U8)1 << 6)

#define ESCO_PACKET_SELECT_GROUP_TX_3SLOT_RX_3SLOT     ((U8)1)
#define ESCO_PACKET_SELECT_GROUP_TX_3SLOT_RX_1SLOT     ((U8)2)
#define ESCO_PACKET_SELECT_GROUP_TX_1SLOT_RX_3SLOT     ((U8)3)
#define ESCO_PACKET_SELECT_GROUP_TX_1SLOT_RX_1SLOT     ((U8)4)

#define NUM_OF_ESCO_3SLOT_PACKET                       ((U8)4)
#define NUM_OF_ESCO_1SLOT_PACKET                       ((U8)3)

#define SCO_PACKET_HV1_INFORMATION_BYTE                ((U16)10)
#define SCO_PACKET_HV2_INFORMATION_BYTE                ((U16)20)
#define SCO_PACKET_HV3_INFORMATION_BYTE                ((U16)30)

#define SCO_PACKET_HV1_INTERVAL                        ((U8)2)
#define SCO_PACKET_HV2_INTERVAL                        ((U8)4)
#define SCO_PACKET_HV3_INTERVAL                        ((U8)6)

#define SCO_PACKET_OCCUPIED_SLOT   1
#define ESCO_WESCO_CONSTRAINED_BY_POWER_CONSUMPTION             ((U8)0X01)
#define ESCO_WESCO_CONSTRAINED_BY_LINK_QUALITY                  ((U8)0X02)

#define SCO_ESCO_MAXLATENCY_DONT_CARE                  ((U16)0xFFFF)


#define HCIPKTYPE_VER11_HV1 0x0020
#define HCIPKTYPE_VER11_HV2 0x0040
#define HCIPKTYPE_VER11_HV3 0x0080

//packet type of lmp sco_req
#define LMPKTYPE_HV1 0
#define LMPKTYPE_HV2 1
#define LMPKTYPE_HV3 2

//packet type of lmp sco_req
#define LMPKTYPE_NP 0 //NULL/POLL
#define LMPKTYPE_EV3 0x07
#define LMPKTYPE_EV4 0x0C
#define LMPKTYPE_EV5 0x0D
#define LMPKTYPE_2EV3 0x26
#define LMPKTYPE_2EV5 0x2C
#define LMPKTYPE_3EV3 0x37
#define LMPKTYPE_3EV5 0x3D

//air mode of lmp sco_req
#define LMPAIRMD_ULAW 0
#define LMPAIRMD_ALAW 1
#define LMPAIRMD_CVSD 2
#define LMPAIRMD_TRANSDATA 3

#define LM_VER_1_1 0
#define LM_VER_2_0 1

//Encryption
#define ACTIVE_KEY  gLM_link_info[LM_GET_CURRENT_LINK()].encryption_info.active_key
#define MASTER_KEY  gLM_link_info[LM_GET_CURRENT_LINK()].encryption_info.master_key
#define AU_RAND     gLM_link_info[LM_GET_CURRENT_LINK()].encryption_info.random_number
#define COF         gLM_link_info[LM_GET_CURRENT_LINK()].encryption_info.encryption_cof
#define KEY_KC      gLM_link_info[LM_GET_CURRENT_LINK()].encryption_info.Kc
#define KEY_LENGTH  gLM_link_info[LM_GET_CURRENT_LINK()].encryption_info.encryption_keysize

//General
PUBLIC void LM_SetTopState(U8 top_state);
PUBLIC U8 LM_GetTopState(void);
PUBLIC U8 LM_GetTopState2(void);

PUBLIC void LM_SetSubState(U8 sub_state);
PUBLIC U8 LM_GetSubState(void);
PUBLIC U8 LM_GetSubState2(void);

PUBLIC void LM_SetActivate(U8 activate);
PUBLIC U8 LM_GetActivate(void);

PUBLIC void LM_SetTrigger(U8 XDATA_PTR trigger);
PUBLIC U8 XDATA_PTR LM_GetTrigger(void);

PUBLIC void LM_SetCmpltdTrans(U8 onOff);
PUBLIC BOOL LM_GetCmpltdTrans(void);

PUBLIC void LM_SetChkCollision(U8 onOff);
PUBLIC BOOL LM_GetChkCollision(void);

PUBLIC void LM_SetLinkSameErr(U8 err);
PUBLIC void LM_SetLinkDiffErr(U8 err2Local, U8 err2Remote);

#define LM_SET_NOT_ANYTIME_PDU_TRANS(onOff)     (gLM_link_info[LM_GET_CURRENT_LINK()].not_anytimePDU = onOff)
#define LM_GET_NOT_ANYTIME_PDU_TRANS()          (gLM_link_info[LM_GET_CURRENT_LINK()].not_anytimePDU)

#define LM_GET_LOCAL_BD_ADDR()                  gHC_ctl.BdAddr
#define LM_GET_CURRENT_LINK_REMOTE_BD_ADDR()    gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].pConnHdl->LinkInfo.Bt.BdAddr
#define LM_GET_LINK_REMOTE_BD_ADDR(A)           gSYS_connection_handle_info[A].pConnHdl->LinkInfo.Bt.BdAddr

#define LM_GET_CURRENT_LINK_DETACH_TYPE()       gLM_ctl.detachType[LM_GET_CURRENT_LINK()]
#define LM_SET_CURRENT_LINK_DETACH_TYPE(A)      gLM_ctl.detachType[LM_GET_CURRENT_LINK()] = A
#define GET_DETACH_SCOCTL_IDX()                 gLM_ctl.detachScoLink[LM_GET_CURRENT_LINK()]
#define SET_DETACH_SCOCTL_IDX(idx)              gLM_ctl.detachScoLink[LM_GET_CURRENT_LINK()] = idx


#define LM_ACL_3SLOT_FEAT(A) \
    (pSysLocalDeviceInfo->LmpFeatures[0][0].field.Bit0 && (gSYS_connection_handle_info[A].LMP_feature_ext[0].group.B0 & 0x01))
#define LM_ACL_5SLOT_FEAT(A) \
    (pSysLocalDeviceInfo->LmpFeatures[0][0].field.Bit1 && (gSYS_connection_handle_info[A].LMP_feature_ext[0].group.B0 & 0x02))
#define LM_ENC_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][0].field.Bit2 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B0 & 0x04))
#define LM_ROLE_SWITCH_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][0].field.Bit5 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B0 & 0x20))
#define LM_HOLD_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][0].field.Bit6 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B0 & 0x40))
#define LM_SNIFF_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][0].field.Bit7 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B0 & 0x80))
#define LM_SCO_HV2_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][1].field.Bit4 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B1 & 0x10))
#define LM_SCO_HV3_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][1].field.Bit5 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B1 & 0x20))
#define LM_SCO_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][1].field.Bit3 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B1 & 0x08))
#define LM_BROADCAST_ENC_FEAT(A) \
    (pSysLocalDeviceInfo->LmpFeatures[0][2].field.Bit7 && (gSYS_connection_handle_info[A].LMP_feature_ext[0].group.B2 & 0x80))
#define LM_ACL_EDR2M_FEAT(A) \
    (pSysLocalDeviceInfo->LmpFeatures[0][3].field.Bit1 && (gSYS_connection_handle_info[A].LMP_feature_ext[0].group.B3 & 0x02))
#define LM_ACL_EDR3M_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][3].field.Bit2 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B3 & 0x04))
#define LM_ACL_EDR_FEAT (LM_ACL_EDR3M_FEAT || LM_ACL_EDR2M_FEAT(LM_GET_CURRENT_LINK()))
#define LM_ESCO_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][3].field.Bit7 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B3 & 0x80))
#define LM_EV4_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][4].field.Bit0 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B4 & 0x01))
#define LM_EV5_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][4].field.Bit1 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B4 & 0x02))
#define LM_AFH_CAPABLE_MASTER \
    (pSysLocalDeviceInfo->LmpFeatures[0][5].field.Bit3 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B4 & 0x08))
#define LM_AFH_CLASSIFICATION_MASTER \
    (pSysLocalDeviceInfo->LmpFeatures[0][5].field.Bit4 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B4 & 0x10))
#define LM_ACL_EDR3SLOT_FEAT(A) \
    (pSysLocalDeviceInfo->LmpFeatures[0][4].field.Bit7 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B4 & 0x80))
#define LM_ACL_EDR5SLOT_FEAT(A) \
    (pSysLocalDeviceInfo->LmpFeatures[0][5].field.Bit0 && (gSYS_connection_handle_info[A].LMP_feature_ext[0].group.B5 & 0x01))
#define LM_SNIFF_SUBRATING_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][5].field.Bit1 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B5 & 0x02))
#define LM_ACL_ENCPTPAUSE_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][5].field.Bit2 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B5 & 0x04))
#define LM_AFH_CAPABLE_SLAVE \
    (pSysLocalDeviceInfo->LmpFeatures[0][4].field.Bit3 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B5 & 0x08))
#define LM_AFH_CLASSIFICATION_SLAVE \
    (pSysLocalDeviceInfo->LmpFeatures[0][4].field.Bit4 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B5 & 0x10))
#define LM_ESCO_EDR2M_FEAT(A) \
    (pSysLocalDeviceInfo->LmpFeatures[0][5].field.Bit5 && (gSYS_connection_handle_info[A].LMP_feature_ext[0].group.B5 & 0x20))
#define LM_ESCO_EDR3M_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][5].field.Bit6 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B5 & 0x40))
#define LM_ESCO_EDR3SLOT_FEAT(A) \
    (pSysLocalDeviceInfo->LmpFeatures[0][5].field.Bit7 && (gSYS_connection_handle_info[A].LMP_feature_ext[0].group.B5 & 0x80))
#define LM_SIMPLE_PAIRING_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][6].field.Bit3 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B6 & 0x08))
#define LM_ENHANCED_POWER_CONTROL_FEAT \
    (pSysLocalDeviceInfo->LmpFeatures[0][7].field.Bit2 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[0].group.B7 & 0x04))

#define LM_SIMPLE_PAIRING_FEAT_EXT \
    (pSysLocalDeviceInfo->LmpFeatures[1][0].field.Bit0 && (gSYS_connection_handle_info[LM_GET_CURRENT_LINK()].LMP_feature_ext[1].group.B0 & 0x01))

#define LM_CREATE_SCO(activity, dScoPtr) LM_SetSco(activity, 0, dScoPtr)
#define LM_ENABLE_SCO(lmScoHandle, activity) LM_SetSco(activity, lmScoHandle, 0)
#define LM_CLEAR_SCO(lmScoHandle) LM_SetSco(LM_SCO_DISABLE, lmScoHandle, 0)
#define LM_RMVING_SCO(lmScoHandle, activity) LM_SetSco(activity, lmScoHandle, 0)


#define     Control_SFR_E7_OFFSET       SFR_RW_E7_OFFSET
#define     pair_key_x                  SFR_RW_E7
#define     pair_key_y                  SFR_RW_E7
#define     secret_key                  SFR_RW_E7
#define     enable_control              SFR_RW_F8
sbit        enable_control_5            = enable_control^5;



PUBLIC BOOL LM_ClockCompare27 (U32 a, U32 b) compact REENTRANT;

PUBLIC BOOL LM_CheckCmdOpCode(U8 XDATA_PTR msgPtr, U16 Opcode);

HC_EVT_MSG_STRU_PTR LM_CmdCplEvt_BdAddr(
    IN      HCI_OPCODE OpCode,
    IN      U8 RtnParaSize);

#endif


