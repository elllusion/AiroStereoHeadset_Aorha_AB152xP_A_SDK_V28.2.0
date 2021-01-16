//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////


#ifndef _LC_INTERFACE_H_
#define _LC_INTERFACE_H_

#include "config.h"
#include "cpu.h"
#include "os.h"
#include "rc.h"

#include "lc_types.h"
#include "lc_bledevicectl.h"

#include "sector_lc_misc_ctl.h"
#include "LC_BLEDeviceCtl.h"
#include "hci.h"
#include "lmci.h"
#include "sector_mp_parameter.h"
#include "ctlr_config.h"
#ifdef _LC_C_
#define LC_EXTERN
#else
#define LC_EXTERN extern
#endif

//- from lm_interface.h
//time for periodically checking poll for slaves. enlarge lc search window size if not get one before expiration
#define DEFAULT_POLL_CHK_TIME_INSEC 3

#define GET_HALFSLOT_FROM_SEC(x) (x*3200L)
#define DEFAULT_POLL_CHK_TIME_IN_HALFSLOT GET_HALFSLOT_FROM_SEC(DEFAULT_POLL_CHK_TIME_INSEC)


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
    U8 firstPara;
}LclmDataType;



#define LM_DATA_GET_OPCODE(ptr)         (((LclmDataType XDATA_PTR)ptr)->opcodeTid >> 1)
#define LM_DATA_IS_BC(ptr)              ((((LclmDataType XDATA_PTR)ptr)->misc>>5)&0x01) //get the broadcast flag
#define LM_DATA_PARAM_PTR(ptr)          (&((LclmDataType XDATA_PTR)ptr)->firstPara)
#define LM_DATA_SET_LINKIDX(ptr, idx)   ((LclmDataType XDATA_PTR)ptr)->attachedLinkIdx = idx
#define LM_DATA_GET_LINKIDX(ptr)        ((LclmDataType XDATA_PTR)ptr)->attachedLinkIdx


#define LM_PDU_TX_NEED_BBACK_MASK           (0x01)
#define LM_PDU_TX_NEED_BBACK                (0x01)
#define LM_PDU_TX_HIGH_PRIORITY             (0x02)

#define LM_PDU_ESCAPE1                          124
#define LM_PDU_ESCAPE2                          125
#define LM_PDU_ESCAPE3                          126
#define LM_PDU_ESCAPE4                          127
#define LM_PDU_LMP_ACCEPTED                     3
#define LM_PDU_LMP_AU_RAND                      11
#define LM_PDU_AUTO_RATE                        35
#define LM_PDU_LMP_CLKOFFSET_REQ                5
#define LM_PDU_LMP_CLKOFFSET_RES                6
#define LM_PDU_LMP_COMB_KEY                     9
#define LM_PDU_LMP_DECR_POWER_REQ               32
#define LM_PDU_LMP_DETACH                       7
#define LM_PDU_LMP_DHKEY_CHECK                  65
#define LM_PDU_LMP_ENCAPSULATED_HEADER          61
#define LM_PDU_LMP_ENCAPSULATED_PAYLOAD         62
#define LM_PDU_LMP_ENCRYPTION_KEY_SIZE_MASK_REQ 58
#define LM_PDU_LMP_ENCRYPTION_KEY_SIZE_MASK_RES 59
#define LM_PDU_LMP_ENCRYPTION_KEY_SIZE_REQ      16
#define LM_PDU_LMP_ENCRYPTION_MODE_REQ          15
#define LM_PDU_LMP_FEATURES_REQ                 39
#define LM_PDU_LMP_FEATURES_RES                 40
#define LM_PDU_LMP_HOST_CONNECTION_REQ          51
#define LM_PDU_LMP_HOLD                         20
#define LM_PDU_LMP_HOLD_REQ                     21
#define LM_PDU_LMP_INCR_POWER_REQ               31
#define LM_PDU_LMP_IN_RAND                      8
#define LM_PDU_LMP_MAX_POWER                    33
#define LM_PDU_LMP_MAX_SLOT                     45
#define LM_PDU_LMP_MAX_SLOT_REQ                 46
#define LM_PDU_LMP_MIN_POWER                    34
#define LM_PDU_LMP_MODIFY_BEACON                28
#define LM_PDU_LMP_NAME_REQ                     1
#define LM_PDU_LMP_NAME_RES                     2
#define LM_PDU_LMP_NOT_ACCEPTED                 4
#define LM_PDU_LMP_PAGE_MODE_REQ                53
#define LM_PDU_LMP_PAGE_SCAN_MODE_REQ           54
#define LM_PDU_LMP_PARK_REQ                     25
#define LM_PDU_LMP_PREFERRED_RATE               36
#define LM_PDU_LMP_QUALITY_OF_SERVICE           41
#define LM_PDU_LMP_QUALITY_OF_SERVICE_REQ       42
#define LM_PDU_LMP_REMOVE_SCO_LINK_REQ          44
#define LM_PDU_LMP_SCO_LINK_REQ                 43
#define LM_PDU_LMP_SET_AFH                      60
#define LM_PDU_LMP_SET_BROADCAST_SCAN_WINDOW    27
#define LM_PDU_LMP_SETUP_COMPLETE               49
#define LM_PDU_LMP_SIMPLE_PAIRING_CONFIRM       63
#define LM_PDU_LMP_SIMPLE_PAIRING_NUMBER        64
#define LM_PDU_LMP_SLOT_OFFSET                  52
#define LM_PDU_LMP_SNIFF_REQ                    23
#define LM_PDU_LMP_SRES                         12
#define LM_PDU_LMP_START_ENCRYPTION_REQ         17
#define LM_PDU_LMP_STOP_ENCRYPTION_REQ          18
#define LM_PDU_LMP_SUPERVISION_TIMEOUT          55
#define LM_PDU_LMP_SWITCH_REQ                   19
#define LM_PDU_LMP_TEMP_RAND                    13
#define LM_PDU_LMP_TEMP_KEY                     14
#define LM_PDU_LMP_TEST_ACTIVATE                56
#define LM_PDU_LMP_TEST_CONTROL                 57
#define LM_PDU_LMP_TIMING_ACCURACY_REQ          47
#define LM_PDU_LMP_TIMING_ACCURACY_RES          48
#define LM_PDU_LMP_UNIT_KEY                     10
#define LM_PDU_LMP_UNPARK_BD_ADDR_REQ           29
#define LM_PDU_LMP_UNPARK_PM_ADDR_REQ           30
#define LM_PDU_LMP_UNSNIFF_REQ                  24
#define LM_PDU_LMP_USE_SEMI_PERMANENT_KEY       50
#define LM_PDU_LMP_VERSION_REQ                  37
#define LM_PDU_LMP_VERSION_RES                  38


//LM TX PDU format: offset control len_L len_H TID_OP (TID_OP_EXT) payload1 ...
#define LM_PDU_MISC_FLAG(ptr)           ((LclmDataType XDATA_PTR)ptr)->misc
#define LM_PDU_LENGTH_B0(ptr)           ((LclmDataType XDATA_PTR)ptr)->lengthB0
#define LM_PDU_LENGTH_B1(ptr)           ((LclmDataType XDATA_PTR)ptr)->lengthB1
#define LM_PDU_OPCODE_TID(ptr)          ((LclmDataType XDATA_PTR)ptr)->opcodeTid
#define LM_PDU_LMP_PARAM_PTR(ptr)       (&((LclmDataType XDATA_PTR)ptr)->firstPara)


//- LM CMD to LC
//#define DATA_OFFSET_LCLM_CMDEVT         1
typedef struct
{
    U8 offset; // 1
    U8 opcode;
    U8 firstPara;

} LclmCmdEvtType;


#define LM_CMD_DATA_OFFSET(ptr_msg)     ((LclmCmdEvtType XDATA_PTR)ptr_msg)->offset
#define LM_CMD_OPCODE(ptr_msg)          ((LclmCmdEvtType XDATA_PTR)ptr_msg)->opcode
#define LM_CMD_PARAM_PTR(ptr_msg)       (&((LclmCmdEvtType XDATA_PTR)ptr_msg)->firstPara)

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
}LcExtDataType;


#define LC_DATA_GET_EXT_OPCODE(ptr)     ((LcExtDataType XDATA_PTR)ptr)->extOpcode


typedef struct
{
    U8  acl_edr_on;
    U8  acl_edr_just_set;
} LM_ACLU_PARAM;

typedef struct
{
    LM_ACLU_PARAM aclu_ctl;
#ifdef SUPVIS_TIMEOUT_BY_LM
    U8 lc_addressed;
#endif

}LM_CONNECTION_HANDLE_INFO_ROM;
//---------------------



///////////////////////////////////////////////////////////
//  Global type definition;
///////////////////////////////////////////////////////////
#define HC_DATA_OFFSET                      5
#define DATA_OFFSET_LCLM_DATA               3
#define HC_DATA_MEMBLK_OFFSET               3
#define NUM_OF_MAX_SG_SEGMENT               4

#define LC_TO_LM_EVENT_DATA_OFFSET          1
#define LC_TO_HC_EVENT_DATA_OFFSET          1
#define LC_LM_HC_PDU_DATA_OFFSET            3

//- LC<->LM , LC<->HC PDU
typedef struct
{
    U8  next_ptr_B0;
    U8  next_ptr_B1;
    U8  len_B0;
    U8  len_B1;
    U8  payload;
} LC_TXPDU_MEM_UNIT;

typedef struct
{
    U8  next_ptr_B0;
    U8  next_ptr_B1;
    U8  len_include_ph_B0;
    U8  len_include_ph_B1;
    U8  ph;
    U8  payload;
} LC_TXBCPDU_MEM_UNIT;

typedef struct
{
    TWO_BYTE_UNION_TYPE             ptr;
    TWO_BYTE_UNION_TYPE             len;
    TWO_BYTE_UNION_TYPE             rptr;
} SCATTER_GATHER_UNIT;




#define LC_PDU_DATA_OFFSET(ptr)         (*(U8 XDATA_PTR)ptr)
#define LC_PDU_CONTENT(ptr,N)           (*(U8 XDATA_PTR)(ptr + *ptr + (N)))
#define LC_PDU_CONTENT_PTR(ptr,N)       ((U8 XDATA_PTR)(ptr + *ptr + (N)))
#define LC_PDU_MISC_FLAG(ptr)           LC_PDU_CONTENT(ptr, 0)
#define LC_PDU_LENGTH_B0(ptr)           LC_PDU_CONTENT(ptr, 1)
#define LC_PDU_LENGTH_B1(ptr)           LC_PDU_CONTENT(ptr, 2)
#define LC_PDU_PARAM(ptr,N)             LC_PDU_CONTENT(ptr, (3+N))
#define LC_PDU_PARAM_PTR(ptr, N)        ((U8 XDATA_PTR)(ptr + *ptr + (N+3)))

#define LC_PDU_SCO_DATA_OFFSET(ptr)     (*(U8 XDATA_PTR)ptr)
#define LC_PDU_SCO_CONTENT(ptr,N)       (*(U8 XDATA_PTR)(ptr + *ptr + (N)))
#define LC_PDU_SCO_CONTENT_PTR(ptr,N)   ((U8 XDATA_PTR)(ptr + *ptr + (N)))
#define LC_PDU_SCO_CONN_HANDLE(ptr)     LC_PDU_CONTENT(ptr, 0)
#define LC_PDU_SCO_STATUS_FLAG(ptr)     LC_PDU_CONTENT(ptr, 1)
#define LC_PDU_SCO_LENGTH_B0(ptr)       LC_PDU_CONTENT(ptr, 2)
//-#define LC_PDU_SCO_LENGTH_B1(ptr)    LC_PDU_CONTENT(ptr, 3)
#define LC_PDU_SCO_PARAM(ptr,N)         LC_PDU_CONTENT(ptr, (3+N))
#define LC_PDU_SCO_PARAM_PTR(ptr, N)    ((U8 XDATA_PTR)(ptr + *ptr + (N+3)))
#define LC_PDU_SCO_PAYLOAD_PTR(ptr)     LC_PDU_SCO_PARAM_PTR(ptr, 0)

#define LC_PDU_PHEADER_1B_B0(ptr)       LC_PDU_CONTENT(ptr, 2)
#define LC_PDU_PHEADER_2B_B0(ptr)       LC_PDU_CONTENT(ptr, 1)
#define LC_PDU_PHEADER_2B_B1(ptr)       LC_PDU_CONTENT(ptr, 2)

#define LC_PDU_PHEADER_0B_PTR(ptr)      LC_PDU_CONTENT_PTR(ptr, 3)
#define LC_PDU_PHEADER_1B_PTR(ptr)      LC_PDU_CONTENT_PTR(ptr, 2)
#define LC_PDU_PHEADER_2B_PTR(ptr)      LC_PDU_CONTENT_PTR(ptr, 1)

//- LC EVENT OPCODE
#define LC_EVENT_INQUIRY_COMPLETE               (0x01)
#define LC_EVENT_INQUIRY_RESULTS                (0x02)

#define LC_EVENT_CONNECTION_STATE_ENTERED       (0x03)
#define LC_EVENT_CREATE_CONNECTION_FAILED       (0x04)

#ifdef SUPPORT_SCO_ESCO
    #define LC_EVENT_SCO_ESCO_ESTABLISHED       (0x05)
#endif
#define LC_EVENT_ACL_RELEASED                   (0x06)
#ifdef SUPPORT_SCO_ESCO
    #define LC_EVENT_SCO_ESCO_RELEASED          (0x07)
#endif
#define LC_EVENT_ROLE_CHANGE                    (0x12)
#define LC_EVENT_SNIFF_COMPLETE                 (0x14)
#define LC_EVENT_HOLD_COMPLETE                  (0x15)
#define LC_EVENT_UNSNIFF_OK                     (0x16)
#define LC_EVENT_HOLD_EXITED                    (0x17)
#define LC_EVENT_ACL_U_GO_STOP_CONFIRM          (0x20)
#define LC_EVENT_SNIFF_SUBRATING_COMPLETE       (0x21)
#define LC_EVENT_SUPERVISION_TO                 (0x22)

#define LC_EVENT_LE_CONNECTION_COMPLETE             (0x30)
#define LC_EVENT_LE_LINK_RELEASED                   (0x31)
#define LC_EVENT_LE_DIRECTED_ADVERTISING_TIMEOUT    (0x32)
#define LC_EVENT_LE_COLLISION                       (0x33)


#define LC_EVENT_SCO_NULL_PKT_COMPLETE			(0x40)
#define LC_EVENT_SWITCH_SCO_OK					(0x41)
#define LC_EVENT_SWITCH_SCO_FAIL				(0x42)

#define LC_EVENT_TUNE_SEARCH_WINDOW_COMPLETE    (0x43)

#define LC_EVENT_START_ENC_LOADED    (0x50)
#define LC_EVENT_STOP_ENC_LOADED    (0x51)

#define LC_EVENT_DATA_OFFSET(ptr_msg)           (*(U8 XDATA_PTR)ptr_msg)
#define LC_EVENT_OPCODE(ptr_msg)                (*(U8 XDATA_PTR)(ptr_msg + *ptr_msg))
#define LC_EVENT_PARAM_PTR(ptr_msg,N)           (U8 XDATA_PTR)(ptr_msg + *ptr_msg + (N+1))
#define LC_EVENT_PARAM(ptr_msg,N)               (*(U8 XDATA_PTR)(ptr_msg + *ptr_msg + (N+1)))

#define EVT_PAGE_PARAM_PAGE_CANCEL              0
#define EVT_PAGE_PARAM_PAGE_TIMEOUT             1
#define EVT_PAGE_PARAM_PAGE_EXISTS              2

#define HC_TO_LC_PB_MASK                        0x03

#define LC_PB_L2CAP_START_NON_AUTO_FLUSHABLE  0x00
#define LC_PB_L2CAP_CONT                      0x01
#define LC_PB_L2CAP_START_AUTO_FLUSHABLE      0x02
#define HC_TO_LC_PB_L2CAP_COMPLETE            0x03

//- Lab Test
#ifdef LABTEST_ENABLED
VOID LC_SendVCMDComplToHC(U8 status, U8 vcmd_opcode, U8 vcmd_ogf);
VOID LC_SendEVTReturnToHC(U8 XDATA_PTR ptr_event_to_HC, U8 vcmd_opcode, U8 param_length, U8 return_type);
#endif
#define LC_LABTEST_ONLY_CTL                     gLC_ctl.lc_union_share_labtest_afh.union_share_labtest_afh.labtest_only_ctl

//------For Error Report------
#ifdef SUPPORT_SCO_ESCO
    #define LC_SCO_ESCO_ROUTE_TO_CODEC          0x01
    #define LC_SCO_ESCO_ROUTE_TO_HC             0x02
    #define LC_SCO_ESCO_MUTE                    0x03
#endif
//------For Error Report------

///////////////////////////////////
//Constant Definition
///////////////////////////////////


enum
{
    INQUIRY_SCAN_ENABLE = (1 << 0),
    PAGE_SCAN_ENABLE = (1 << 1),
    INQUIRY_PAGE_SCAN_BOTH_ENABLE = (INQUIRY_SCAN_ENABLE | PAGE_SCAN_ENABLE)
};
enum
{
    BLE_TXDONE = (U8)0,
    BLE_RXDONE = (U8)1,
    BLE_SLOTTICK = (U8)2,
    BLE_GUARD = (U8)3,
    BLE_CONNEND = (U8)4,
    BLE_DATALOADED = (U8)5,
};
//========================================== packet type defination

#define BT_PKT_NULL                     0x0
#define BT_PKT_POLL                     0x1
#define BT_PKT_FHS                      0x2
#define BT_PKT_DM1                      0x3
#define BT_PKT_DH1                      0x4
#define BT_PKT_2DH1                     0x4
#define BT_PKT_HV1                      0x5
#define BT_PKT_HV2                      0x6
#define BT_PKT_2EV3                     0x6
#define BT_PKT_HV3                      0x7
#define BT_PKT_EV3                      0x7
#define BT_PKT_3EV3                     0x7
#define BT_PKT_DV                       0x8
#define BT_PKT_3DH1                     0x8
#define BT_PKT_AUX1                     0x9
#define BT_PKT_DM3                      0xA
#define BT_PKT_2DH3                     0xA
#define BT_PKT_DH3                      0xB
#define BT_PKT_3DH3                     0xB
#define BT_PKT_EV4                      0xC
#define BT_PKT_2EV5                     0xC
#define BT_PKT_EV5                      0xD
#define BT_PKT_3EV5                     0xD
#define BT_PKT_DM5                      0xE
#define BT_PKT_2DH5                     0xE
#define BT_PKT_DH5                      0xF
#define BT_PKT_3DH5                     0xF
#define BT_CRC_OK                       0x10
#define BT_HEC_OK                       0x08

#define BT_MAX_LENGTH_DM1               17
#define BT_MAX_LENGTH_DH1               27
#define BT_MAX_LENGTH_DM3               121
#define BT_MAX_LENGTH_DH3               183
#define BT_MAX_LENGTH_DM5               224
#define BT_MAX_LENGTH_DH5               339
#define BT_MAX_LENGTH_AUX1              29

#define BT_MAX_LENGTH_2DH1              54
#define BT_MAX_LENGTH_2DH3              367
#define BT_MAX_LENGTH_2DH5              679
#define BT_MAX_LENGTH_3DH1              83
#define BT_MAX_LENGTH_3DH3              552
#define BT_MAX_LENGTH_3DH5              1021

#define BT_MAX_LENGTH_HV1           10
#define BT_MAX_LENGTH_HV2           20
#define BT_MAX_LENGTH_HV3           30
#define BT_MAX_LENGTH_DV_VOICE      10
#define BT_MAX_LENGTH_DV_DATA       9
#define BT_MAX_LENGTH_DV_TOTAL      19
#define BT_MAX_LENGTH_EV3           30
#define BT_MAX_LENGTH_EV4           120
#define BT_MAX_LENGTH_EV5           180
#define BT_MAX_LENGTH_2EV3          60
#define BT_MAX_LENGTH_2EV5          360
#define BT_MAX_LENGTH_3EV3          90
#define BT_MAX_LENGTH_3EV5          540

//#define LC_SLAVE_RESPONSE_TO                      8       //slots
//#define LC_MASTER_RESPONSE_TO                     8       //slots
//#define LC_NEW_CONNECTION_TIMEOUT                 32      // 32 slots (default)
#define MASTER_POLL_INTERVAL_IN_SLOT                (40)    // 20 tx-slots (default)
#define SLAVE0_POLL_INTERVAL_IN_SLOT                (40)    // 20 tx-slots (default)
#define SLAVE1_POLL_INTERVAL_IN_SLOT                (40)    // 20 tx-slots (default)

//========================================== AFH
#define LC_AFH_MODE_ENABLING                        0
#define LC_AFH_MODE_DISABLING                       1
#define LC_AFH_MODE_UPDATING                        2
#define LC_AFH_MODE_RECOVERY_SEQUENCE               3

#define LC_AFH_AGC_TIMEOUT_150US_MASK               0x0F
#define LC_AFH_AGC_SUBTIME_20US_MASK                0x50
#define LC_AFH_AGC_CONTIMEAS_ENABLE_MASK            0x80
#define LC_AFH_AGC_CONTIMEAS_DISABLE_MASK           0x00

#define AFH_SW_HW_SHARE_CTL                         gLC_ctl.lc_union_share_labtest_afh.union_share_labtest_afh.afh_sw_hw_share_meas_ctl

//---------- UPLAYER CALL ------------
#define LC_SET_HID_UNSNIFF_TRANSATION               {OS_ENTER_CRITICAL();gLC_ctl.misc_bit_ctl1.hid_unsniff_transation = TRUE;OS_EXIT_CRITICAL();}

//---------- HC CALL ------------
//#define LC_GET_TXGC_TEMP_COMPESATION_INTERVAL()   (gLC_ctl.lc_mod_gain_comp_ctl.mod_gain_comp_ctl.tx_mod_gain_comp_interval_in_80ms)
//#define LC_SET_TXGC_TEMP_COMPESATION_INTERVAL(A)  (gLC_ctl.lc_mod_gain_comp_ctl.mod_gain_comp_ctl.tx_mod_gain_comp_interval_in_80ms = (A))

#define LC_GET_LABTEST_RUN()                        (gLC_LabTest_Going)


#define LC_MMI2_ENABLE(A)                           LC_SFR_WRITE_BY_C2(C2OFFS_RW_MMI2_CTL_B0, A | 0x80)
#define LC_MMI2_DISABLE()                           LC_SFR_WRITE_BY_C2(C2OFFS_RW_MMI2_CTL_B0, 0x00)

#define LC_SAC_LINK_S0_M                            8
#define LC_SAC_LINK_S1_M                            9

VOID LC_WriteSacScan(U16 interval);

#define LC_DISABLE_SAC_SCAN()                   {                                                           \
                                                    LC_WriteSacScan (0);                                    \
                                                }

#define LC_ENABLE_SAC_SCAN(A)                   {                                                           \
                                                    LC_WriteSacScan (A);                                    \
                                                }

#if 0
LC_EXTERN VOID LC_CheckSlowClockHandler (VOID);
#endif

//---------- LM CALL ------------
#define LC_GET_ANY_LINK30_EXIST()                       (gLC_30_Link_Exist)
#define LC_GET_RX_RSSI_VALUE(A)                         (gLC_ctl.rf_ctl.rssi_real[A])
#define LC_GET_LABTEST_ONLY()                           (gLC_LabTest_Going)

#define LC_GET_MASTER_ADDRESSED(A)                      (A = LC_SFR_READ_BY_E7(E7OFFS_RO_RX_POLL_RD_MASTER))


#define LC_GET_SLAVE_ADDRESSED(A)                       (A = LC_SFR_READ_BY_E7(E7OFFS_RO_RX_POLL_RD_SLAVE))



//#define LC_GET_AFH_CRC_MEASURMENT_DONE()              (gLC_ctl.AFH_CRC_Measurement_done != 0)
//#define LC_CLR_AFH_CRC_MEASURMENT_DONE()              (gLC_ctl.AFH_CRC_Measurement_done = 0)
//#define LC_GET_AFH_RSSI_MEASURMENT_DONE()             (gLC_ctl.AFH_RSSI_Measurement_done != 0)
//#define LC_CLR_AFH_RSSI_MEASURMENT_DONE()             (gLC_ctl.AFH_RSSI_Measurement_done = 0)
//#define LC_GET_AFH_MEASURMENT_PROCESSING()            (gLC_ctl.afh_measurment_ctl.AFH_Measurment_processing)
//#define LC_GET_AFH_RSSI_PROCESSING()                  (gLC_ctl.afh_measurment_ctl.AFH_RSSI_processing)
//#define LC_GET_AFH_CRC_PROCESSING()                   (gLC_ctl.afh_measurment_ctl.AFH_CRC_processing)
#define LC_GET_DUMMY_SNIFF_MASK()                       (gLC_ctl.sche_ctl.dummy_sniff_mask)

//#define LC_GET_LINK_ADDRESSED_MASK(A)                 (gLC_ctl.sche_ctl.link_addressed_mask & U16_BIT_MASK(A))
//#define LC_SET_LINK_ADDRESSED_MASK(A)                 (gLC_ctl.sche_ctl.link_addressed_mask |= U16_BIT_MASK(A))
//#define LC_CLR_LINK_ADDRESSED_MASK(A)                 (gLC_ctl.sche_ctl.link_addressed_mask &= ~(U16_BIT_MASK(A)))
//#define LC_RESET_LINK_ADDRESSED_MASK()                    (gLC_ctl.sche_ctl.link_addressed_mask = 0)

//======Broadcast AFH======
#define BC_CHANNEL_MAP_VALID                            0x01
#define LC_GET_BC_AFH_CHANNEL_MAP()                     (gLC_ctl.master_broadcast_ctl.bc_channel_map)
#define LC_SET_BC_AFH_CHANNEL_MAP(A)                    (gLC_ctl.master_broadcast_ctl.bc_channel_map = A)
#define LC_GET_BC_CHANNEL_MODIFIED()                    (gLC_ctl.master_broadcast_ctl.bc_map_modified)
#define LC_SET_BC_CHANNEL_MODIFIED(A)                   (gLC_ctl.master_broadcast_ctl.bc_map_modified = ((BIT)A))
//======Broadcast AFH======

//======Broadcast Encryption======
#define S0_S1_DISABLED                                  0x00
#define S0_ENABLED_ONLY                                 0x01
#define S1_ENABLED_ONLY                                 0x02
#define S0_S1_ENABLED                                   0x03
#define LC_SET_RX_BROADCAST_ENCR_MODE(A)                {OS_ENTER_CRITICAL();LC_SFR_WRITE_BY_C2(C2OFFS_RW_SLAVE_RX_BROADCAST_ENCR, A);OS_EXIT_CRITICAL();}
//======Broadcast Encryption======


#if 0
//=========================================== ADC
#define LC_MCU_ADC_SET_REQUEST(A)           LC_SFR_WRITE_BY_FE(FEOFFS_RW_MCU_ADC_REQUEST, A)
#define LC_MCU_ADC_CLR_REQUEST()            LC_SFR_WRITE_BY_FE(FEOFFS_RW_MCU_ADC_REQUEST, 0x00)
#define LC_MCU_GET_ADC_REQUEST_ACKED()      LC_SFR_READ_BY_FE(FEOFFS_RO_MCU_ADC_REQUEST_ACK)


#define LC_ADC_SAR_SET_REQUEST(A)           LC_SFR_WRITE_BY_FE(FEOFFS_RW_ADC_SEL, A)
#define LC_ADC_SAR_CLR_REQUEST()            LC_SFR_WRITE_BY_FE(FEOFFS_RW_ADC_SEL, 0x00)
#define LC_ADC_SAR_REQUEST_ACKED()          LC_SFR_READ_BY_C2(C2OFFS_RW_ADC_READY)
#define LC_LabtestChargerControl(A)         LC_SFR_WRITE_BY_FE(FEOFFS_RW_CHG_DRV_ENABLE, A)
#endif

//========================================== PMU

#define LC_SET_PMU_POWER_OFF()          {LC_SFR_WRITE_BY_C2(C2OFFS_WO_PMU_ENABLE, (gLC_ctl.misc_ctl.sfr_pmu_settings | LC_PMU_POWER_OFF));}

///////////////////////////////////////////////////////////
//  Global variables declaration;
///////////////////////////////////////////////////////////
typedef struct rx_buffer_stru
{
    U8 offset;
    U8 rsvd[5];
    LC_LE_PACKET_HEADER_B0 packet_header_b0;
    LC_LE_PACKET_HEADER_B1 packet_header_b1;
    U8 first_param;

} RX_BUFFER_STRU, LC_PTR RX_BUFFER_STRU_PTR;

typedef struct rx_buffer_stru_30_lm
{
    U8 offset;
    U8 rsvd[2];
    U8 BC;
    U8 LenB0;
    U8 LenB1;
    U8 Payload[1];

} LMP_DATA_STRU, LC_PTR LMP_DATA_STRU_PTR;

typedef struct rx_buffer_stru_30_l2cap
{
    U8 offset;
    U8 rsvd[1];
    HCI_DATA_HDL_FLAGS_UNION hdl_flg;
    U8 LenB0;
    U8 LenB1;
    U8 Payload[1];

} L2CAP_30_DATA_TO_HOST_STRU, LC_PTR L2CAP_30_DATA_TO_HOST_STRU_PTR;


typedef struct
{
    U8  reserved;
    U16 labtest_status;

    U8  sfr_E4_228;
    U8  sfr_E4_230;
    U8  sfr_E4_231;
    U8  sfr_c2_220;

} LC_LABTEST_TOP_CTL;

typedef struct
{
    U8                      rxdone_stauts;
    U16                     timer_count;
    U8                      edr_enable;
    TWO_BYTE_UNION_TYPE     rpkt_count;
    TWO_BYTE_UNION_TYPE     epkt_count;
    TWO_BYTE_UNION_TYPE     cpkt_count;
    FOUR_BYTE_UNION_TYPE    bits_count;
    FOUR_BYTE_UNION_TYPE    errorbits_count;
    TWO_BYTE_UNION_TYPE     loss_count;
    TWO_BYTE_UNION_TYPE     HEC_error_count;
    TWO_BYTE_UNION_TYPE     CRC_error_count;
    U8                      hopping_channel;
    U8                      rx_conti_loss;
} LC_LABTEST_GROUP0_CTL;

typedef struct
{
    TWO_BYTE_UNION_TYPE     total_packet_number;
    TWO_BYTE_UNION_TYPE     tpkt_count;
} LC_LABTEST_GROUP1_CTL;


typedef struct
{
    U8  channel;

    union
    {
        LC_LABTEST_GROUP0_CTL   group0_ctl;
        LC_LABTEST_GROUP1_CTL   group1_ctl;
    } union_group;

    U8  hopping_enable;
    U8  payload_header_byte;
    U8  packet_type;
    U8  brx_rssi;

    U8  XDATA_PTR   share_buffer;
    U8  labtest_ltaddr;
    U8  rx_len_B0;
    U8  rx_len_B1;
    U8  esco_enable;

} LC_LABTEST_CTL;

typedef struct
{
    U8 test_mode_activated;
    U8 test_scenario;
    U8 hopping_mode;
    U8 TX_freq;
    U8 RX_freq;
    U8 power_control_mode;
    U8 poll_period;
    U8 packet_type;
    U8 length_of_test_data[2];
    U8 release_SEQN_check;
    U8 loopback_rx_dma_go;
    U8 edr_on;

    U8 rx_len_B0;
    U8 rx_len_B1;

    U8 XDATA_PTR loopback_buf_ptr;

    U8  addressed_status;

    U8 tx_mode_test_flag;   //For transmitter test
    U8 tx_test_status;
    U8 XDATA_PTR transmitter_test_buf_ptr;
    U16 timer_for_rx_extended;
    U8  flagg_for_rx_extended;
    U8 Tx_filled_in_HW_buffer_done;
} TESTMODE_CTL;

typedef struct
{
    U16 Rx_packet_count;
    U8  clk_rate;
} DIRECT_TESTMODE_CTL;

typedef struct
{
    U8  tracking_source;        //- 1 : tracking to slave0; 2 : tracking to slave1
    U8  clk1_last;
    U8  stop_range;
} LC_MASTER_CLK_TRACKING_CTL;

typedef struct
{
    U8  monitor_start           :1;
    U8  timer_expired           :1;
    U8  audio_pause             :1;
    U8  monitor_service_now     :1;
    U8  b4                      :1;
    U8  b5                      :1;
    U8  b6                      :1;
    U8  b7                      :1;

    U8                  service_idx;
    U8                  audio_pause_service_idx;
    BTCLK_UNION SlavePktBlockedExpiredTime;
    BTCLK_UNION SlaveAudioPauseExpiredTime;
} LC_SLAVE_PKT_BLOCKED_MOD_CTL;


typedef struct
{
    U8  service_active          :1;
    U8  reserved_b0             :1;
    U8  reserved_b1             :1;
    U8  reserved_b2             :1;
    U8  reserved_b3             :1;
    U8  reserved_b4             :1;
    U8  reserved_b5             :1;
    U8  reserved_b6             :1;
    U8  reserved_b7             :1;
}SERVICE_INFO;

typedef struct
{
    SERVICE_INFO                lc_service_info;

}LC_SERVICE_ADJUSTMENT_CTL;



#ifdef SUPPORT_SCO_ESCO
typedef struct
{
    U8  type;                   //- bit0: sco ; bit1 : esco
    U8  sco_esco_handle;
    U8  audio_sel;              //- sfr c2_58
    U16 sco_esco_rx_len;
    U16 sco_esco_tx_len;

    U8  w_esco;

    U8  audio_source;           // for audio src selection

    U8  reserve_slot_value;
	U8	is_null_pkt;

    U8  s2m_pkt_type;
    U8  m2s_pkt_type;

    U8 interval;
} SCO_ESCO_CTL, OS_PTR SCO_ESCO_CTL_PTR;

typedef struct
{
    U8 sco0         :1;
    U8 sco1         :1;
    U8 sco2         :1;
    U8 unused       :5;

}SELECTED_AUDIO_SRC;

#endif

typedef struct
{
    U8              Tx_request                      : 1;
    U8              Tx_LM_packet                    : 1;
    U8              local_aclu_go                   : 1;
    U8              aclu_edr                        : 1;
    U8              aclu_go_stop_processing         : 1;
    U8              aclu_flushable                  : 1;
    U8              aclu_flush_dummy_dm1            : 1;
    U8              remote_aclu_go                  : 1;
} ACL_BIT_CTL0;

typedef struct
{
    U8              aclu_load                       : 1;
    U8              aclu_llid_start                 : 1;
    U8              aclu_last_packet                : 1;
    U8              aclu_start_pkt_flushed          : 1;
    U8              aclu_start_pkt_loaded           : 1;
    U8              aclu_flush_occurred             : 1;
    U8              aclu_enhanced_flush_enabled     : 1;
    U8              send_enhanced_flush_cpl         : 1;
} ACL_BIT_CTL1;

typedef struct
{
    U8              aclu_nb                         : 4;
    U8              aclu_curr_nb                    : 4;
} ACL_BIT_CTL2;

typedef union
{
    struct
    {
        U8  sg                  : 1;
        U8  nb                  : 3;
        U8  b4                  : 1;
        U8  b5                  : 1;
        U8  ra                  : 1;
        U8  eof                 : 1;
    } member;
    U8 value;
} LC_TXPDU_HC_BIT_CTRL_B0;

typedef union
{
    struct
    {
        U8  packet_type         : 4;
        U8  edr                 : 1;
        U8  b6                  : 1;
        U8  pb                  : 2;
    } member0;
    struct
    {
        U8  edr_packet_type     : 5;
        U8  b6                  : 1;
        U8  pb                  : 2;
    } member1;
    U8 value;
} LC_TXPDU_HC_BIT_CTRL_B1;

typedef union
{
    struct
    {
        U8  bb_acked            : 1;
        U8  high_priority       : 1;
        U8  b2                  : 1;
        U8  b3                  : 1;
        U8  b4                  : 1;
        U8  b5                  : 1;
        U8  b6                  : 1;
        U8  b7                  : 1;
    } member;
    U8 value;
} LC_TXPDU_LM_BIT_CTRL_B1;

typedef struct
{
    U8  next_ptr_B0;
    U8  next_ptr_B1;
    U8  len_ph_B0;
    U8  len_ph_B1;
    U8  ph_B0;
    U8  ph_B1;
} LC_TX_ACL_PAYLOAD_HEADER;

typedef struct
{
    U8  offset;
    U8  dummy[DATA_OFFSET_LCLM_DATA-2];
    U8  reserved_ptr;
    U8  ctrl_B1;
    U8  total_len_B0;
    U8  total_len_B1;
    U8  ptr_param0;
} LC_TXPDU_LM_SCR_TYPE;

typedef struct
{
    U8  offset;
    U8  dummy[HC_DATA_OFFSET-1];
    U8  ctrl_B0;
    U8  ctrl_B1;
    TWO_BYTE_UNION_TYPE total_len;
    SCATTER_GATHER_UNIT scatter_gather_unit[NUM_OF_MAX_SG_SEGMENT];
} LC_TXPDU_HC_SCR_TYPE;

typedef union
{
    TWO_BYTE_UNION_TYPE                 TX_return_ptr_value;
    U8                      XDATA_PTR   TX_return_ptr;
    LC_TXPDU_LM_SCR_TYPE    XDATA_PTR   ptr_lc_txpdu_lm_scr;        //- point to LC TXPDU SCR from LM
} LC_TXPDU_LM_SCR_UNION_TYPE;

typedef union
{
    TWO_BYTE_UNION_TYPE                 TX_return_ptr_value;
    U8                      XDATA_PTR   TX_return_ptr;
    LC_TXPDU_HC_SCR_TYPE    XDATA_PTR   ptr_lc_txpdu_hc_scr;        //- point to LC TXPDU SCR from HC
} LC_TXPDU_HC_SCR_UNION_TYPE;

typedef struct stru_hc_data_from_profile
{
    U8 Offset;
    U8 reserved[3];
    HCI_DATA_HDL_FLAGS_UNION HdlFlags;

    U16 Len;
    U8 Payload[1];
} LC_TXPDU_STRU_FROM_PROFILE;

typedef struct stru_hc_data_from_transport
{
    U8 Offset;
    //U8 reserved[4];
    BTCLK_UNION TimeStamp;
    HCI_DATA_HDL_FLAGS_UNION HdlFlags;

    U16 Len;
    U8 Payload[1];
} LC_TXPDU_STRU_FROM_TRANSPORT;

typedef struct
{
    ACL_BIT_CTL0    bit_ctl0;
    ACL_BIT_CTL1    bit_ctl1;
    ACL_BIT_CTL2    bit_ctl2;

    LC_TXPDU_HC_BIT_CTRL_B1     TX_pdu_HC_ctrl1;
    LC_TXPDU_LM_BIT_CTRL_B1     TX_pdu_LM_ctrl1;

    U8              TX_packet_type;
                                        //- point to current LC TXPDU MEM from HC
    U8 XDATA_PTR    TX_mem_blk_ptr;     //- point to next LC TXPDU MEM from HC

    LC_TXPDU_LM_SCR_UNION_TYPE      union_lc_txpdu_lm_scr;
    LC_TXPDU_HC_SCR_UNION_TYPE      union_lc_txpdu_hc_scr;
    TWO_BYTE_UNION_TYPE             residual_payload_length;

    U16             TX_length_include_packet_header;
    U8              aclu_flush_processing;
    U8              RX_packet_info;// {5'b0, SEQN, ARQN, FLOW}
    U16             NOCP;

    LC_TX_ACL_PAYLOAD_HEADER    TX_acl_payload_header;
    U8 SlaveLtAddr;
    BTCLK_UNION TimeStamp;
    BTCLK_UNION EnhancedFlushTimeStamp;
    U8  Loss_count_isr;
    U8 need_reset_weight;
    U32 tx_loss_count_timestamp;
    U8  Low_priority_loss;
    U32 Low_priority_loss_timestamp;

} ACL_CTL;


typedef struct
{
    U8                      subrate_handle;                   //-----------------------------------------
    FOUR_BYTE_UNION_TYPE    subrate_next_instant_in_slot;     //-   must be the same LM_CMD_ADD_SNIFF_SUBRATE_TYPE
    U16                     subrate_timeout;                  //-
    U8                      subrate_num;                      //-------------------------------------------

//------for manual extended slave search window------
    U8                      subrate_t32k_625u;
    U8                      subrate_t32k_8u;
    U8                      subrate_t12m_625u;
    U8                      subrate_t12m_8u;
//------for manual extended slave search window------
} SNIFF_SUBRATE_CTL;

typedef struct
{
    U8                      sniff_search_window_boosted     :1;
    U8                      reserved                        :7;
}SNIFF_INFO_B0;

typedef struct
{
    U8                      misc_ctl;
    U8                      SNIFF_handle;               //-----------------------------------------
    FOUR_BYTE_UNION_TYPE    next_instant_in_slot;       //- master : Tx slot, Slave : Rx slot
                                                        //-
    U16                     T_sniff;                    //-
    U16                     sniff_attempt;              //-
    U16                     sniff_timeout;              //-
                                                        //-
    //- CRX param                                       //- must be the same  LM_CMD_ADD_SNIFF_TYPE
    U8                      t32k_625u;                  //-
    U8                      t32k_8u;                    //-
    U8                      t12m_625u;                  //-
    U8                      t12m_8u;                    //-------------------------------------------

    SNIFF_SUBRATE_CTL       sniff_subrate_ctl;

    //====== For Sniff Ext. Attempt ======
    U8                      ExtAttemptNeeded;
    //====== For Sniff Ext. Attempt ======
    //====== For Search Window Boost ======
    U8                      sniff_loss_cnt;
    SNIFF_INFO_B0           sniff_info_b0;
    //====== For Search Window Boost ======

	// to workaround hw slave sniff issue
	// rx null or poll do not extend sniff timeout
    U8                      sniff_slot_cnt;
    U8                      need_reset_weight;
    U32                     last_increase_weight_clk;

} SNIFF_CTL;

#if (NEW_LC_SNIFF_TRANSITION)
typedef struct
{
    U8 sniff_transition_ongoing : 1;
    U8 anchor_valid       : 1;
    U8 reserved                 : 6;

} SNIFF_TRANSITION_INFO;

typedef struct
{
    U8                      SNIFF_TRANSITION_handle;                //-----------------------------------------
    FOUR_BYTE_UNION_TYPE    next_instant_in_slot;                   //- master : Tx slot, Slave : Rx slot
                                                                    //-
    U16                     T_sniff;                                //-
    U16                     sniff_attempt;
    SNIFF_TRANSITION_INFO   sniff_transition_info ;
//  SNIFF_TRANSITION_INFO   Sniff_transition_info;
                         //-
//  U16                     sniff_timeout;                          //-
//                                                                  //-
//  //- CRX param                                                   //- must be the same  LM_CMD_ADD_SNIFF_TYPE
//  U8                      t32k_625u;                              //-
//  U8                      t32k_8u;                                //-
//  U8                      t12m_625u;                              //-
//  U8                      t12m_8u;                                //-------------------------------------------

}SNIFF_TRANSITION_CTL, OS_PTR SNIFF_TRANSITION_CTL_PTR;

#endif
//------ For SW Sniff Transition Mode ------
typedef struct
{
    U8                      SNIFF_handle;
    FOUR_BYTE_UNION_TYPE    next_instant_in_slot;
    U8                      interval[2];
    U8                      attempt[2];
    U8                      timeout[2];

    U8                      t32k_625u;
    U8                      t32k_8u;
    U8                      t12m_625u;
    U8                      t12m_8u;
} LM_TEMP_CMD_ADD_SNIFF_TYPE;
//------ For SW Sniff Transition Mode ------

typedef struct{
    U8                      AFH_pending;
    U8                      enable;
    U8                      old_channel_map[10];

    U8                      Master_AFH_in_Tx_Buf;
    U8                      need_acked;
    U8                      map_sel_store;
    U8                      dummy_freq;

    U8                      AFH_handle;                             //---------
    U8                      acked_by_slave; //- only "MASTER" used  //
    U8                      LM_timeout;     //- only "SLAVE" used   // must be the same  LM_CMD_SET_AFH_TYPE
    FOUR_BYTE_UNION_TYPE    AFH_instant_in_slot;                    //
    U8                      mode;                                   //
    U8                      channel_map[10];                        //----------
    U8                      channel_map_share[2][10];
} AFH_CTL;

typedef struct
{
    U8                      encr_on;
    U8                      encr_kc_[16];
    U8                      encr_cpl;
} ENCR_CTL;

typedef struct
{
    U8                      enable              : 1;
    U8                      rs_first_poll       : 1;
    U8                      clk0_sync_clk1      : 1;
    U8                      TDD_switch_setup    : 1;
    U8                      Rx_FHS_Again        : 1;
    U8                      Using_Second_FHS_CLK: 1;
    U8                      b6                  : 1;
    U8                      b7                  : 1;
} RS_BIT_CTL;

typedef struct
{
    U8                      rsvd[2];
    U8                      rs_handle;                              //---------
    U8                      rs_new_handle;                          //
    FOUR_BYTE_UNION_TYPE    rs_instant_in_slot;                     // must be the same  LM_CMD_ROLESWITCH_TYPE
    U16                     rs_slot_offset;                         //
    U8                      rs_instant_passed;                      //---------

    RS_BIT_CTL              rs_bit_ctl;

    U8                      acl_link_attach;
    U8                      rs_new_ltaddr;
    U8                      poll_count;
    FOUR_BYTE_UNION_TYPE    fhs_clke;
    FOUR_BYTE_UNION_TYPE    inst_in_abs_value;

} RS_CTL;
#ifdef SUPVIS_TIMEOUT_BY_LC
typedef struct
{
    U8              searchExtLevel;
    BTCLK_UNION     SupervisionExpiredTime;
    BTCLK_UNION     SupervisionTimeout;
    BTCLK_UNION     PollChk4SlaveExpiredTime;
}SUPVISTO_CTL, OS_PTR SUPVISTO_CTL_PTR;
#endif

typedef struct
{
#ifdef SUPVIS_TIMEOUT_BY_LC
    SUPVISTO_CTL    SupVisTimeout_ctl;
#endif
    ACL_CTL         ACL_ctl;
    AFH_CTL         AFH_ctl;
    ENCR_CTL        ENCR_ctl;
    SNIFF_CTL       SNIFF_ctl;
#if (NEW_LC_SNIFF_TRANSITION)
    SNIFF_TRANSITION_CTL SniffTran_ctl;
#endif
#ifdef SUPPORT_SCO_ESCO
    SCO_ESCO_CTL    SCO_ESCO_ctl[MAX_NUM_SCO_LINK_OF_EACH_ACL];
    FOUR_BYTE_UNION_TYPE  ESCO_Instant;
#endif
    CONN_HDL_STRU_PTR pConnHdl;

} CONN_CTL;

typedef struct
{
    U8      n_master_link;
    U8      n_slave_link;
    U8      master_tick_service_idx;
    U8      master_tick_service[7];

    U8      slave_lc_used_mask;                 //- slave used only  bit_0:slave0, bit_1:slave1
    U8      master_lc_used_mask;                //- master used only
    U8      ble_lc_used_mask;                   //- 4.0 used only
    U8      conn_setup_idx;

#ifdef SUPPORT_SCO_ESCO
    U8      n_sco_esco_link;
    U16     sco_esco_mask;              //- (1 << service_idx)

    //- Support only one sco esco link
    U8      sco_esco_attach_link;
#endif

    U16     pre_connection_going;       //- (1 << service_idx)
    U16     afh_instant_wait;           //- (1 <<sac_conn_idx)
    U16     pre_unsniff_mask;           //- (1 <<sac_conn_idx)
    U16     sche_sniff_mask;            //- (1 <<sac_conn_idx)
    U16     wait_first_sniff_anchor_mask;   //- (1 <<sac_conn_idx)
    U8      new_conn_first_service_mask; // (1 << service_idx)

    U8      dummy_sniff_mask;           //- slave used only  bit_0:slave0, bit_1:slave1
    U8      sniff_trans_timer_mask;     //- slave used only  bit_0:slave0, bit_1:slave1

#ifdef SUPVIS_TIMEOUT_BY_LM
    U16     link_addressed_mask;        //- (1 << service_idx)
#endif
} LC_SCHEDULER_CTL;

#ifdef SUPPORT_SCO_ESCO
typedef struct
{
    U8  audio_codec_link_idx;
    U8  n_sco_esco_link;
    U16 sco_esco_link_mask;
    U8  sco_esco_block_mask;

    SCO_ESCO_CTL    sco_esco_ctl;

} LC_SCO_ESCO_TOP_CTL;
#endif

typedef struct
{
    U8  lt_addr_edr;                //-  0
    U8  encr_mode_rx_max_slot;      //-  1
    U8  reserved;                   //-  2
    U8  afh_table_ptr[2];           //-  3 ~ 4
    U8  kc_ptr[2];                  //-  5 ~ 6
    U8  esco0_rx_len_type[2];       //-  7 ~ 8
    U8  audio0_rx_dma_ptr[2];       //-  9 ~ 10
    U8  rx_rssi;                    //-  11
    U8  audio0_tx_dma_ptr[2];       //-  12 ~ 13
    U8  esco0_tx_len_type[2];       //-  14 ~ 15
    U8  acl_tx_len_type[2];         //-  16 ~ 17
    U8  acl_tx_dma_ptr[2];          //-  18 ~ 19
    U8  rf_gfsk_txgc1;              //-  20
    U8  rf_gfsk_txgc2;              //-  21
    U8  rf_gfsk_bias2_1;            //-  22
    U8  rf_gfsk_bias2_2;            //-  23
    U8  rf_edr_txgc1;               //-  24
    U8  rf_edr_txgc2;               //-  25
    U8  rf_edr_bias2_1;             //-  26
    U8  rf_edr_bias2_2;             //-  27
#ifdef SUPPORT_SCO_ESCO
        U8  esco1_rx_len_type[2];       //-  28 ~ 29
        U8  audio1_rx_dma_ptr[2];       //-  30 ~ 31
        U8  audio1_tx_dma_ptr[2];       //-  32 ~ 33
        U8  esco1_tx_len_type[2];       //-  34 ~ 35

        U8  esco2_rx_len_type[2];       //-  36 ~ 37
        U8  audio2_rx_dma_ptr[2];       //-  38 ~ 39
        U8  audio2_tx_dma_ptr[2];       //-  40 ~ 41
        U8  esco2_tx_len_type[2];       //-  42 ~ 43
#endif
    //- sw used
    U16 sac_int_enable;
    U8  n_sco_esco_this_link;

} SAC_LINK_CTL, OS_PTR SAC_LINK_CTL_PTR;

typedef struct
{
    U8  attach_link;
    U8  acl_tx_len_type[2];
    U8  acl_tx_dma_ptr[2];

} SAC_BROADCASR_CTL;

typedef struct{
    U8  crc_result[10];
    U8  crc_unknown[10];
    U8  crc_ratio[79];
    U8  crc_count[79];
    U8  ac_sync_result[10];
    U8  ac_loss_count[79];
    U8  hec_result[10];
    U8  hec_err_count[79];
    U8  rssi_ch_bimap[2];
    U8  rssi_group[4][2];
    U8  rssi_count[4][16];
} AFH_MEAS_CTL;

typedef struct
{
    BD_ADDR master_bd_addr;
    PARITY  master_parity;
    U8      reserve_0[3];

    BD_ADDR slave0_bd_addr;
    PARITY  slave0_parity;
    U8      reserve_1[3];

    BD_ADDR slave1_bd_addr;
    PARITY  slave1_parity;
    U8      reserve_2[3];

    U8  master_link0_ptr[2];
    U8  master_link1_ptr[2];
    U8  master_link2_ptr[2];
    U8  master_link3_ptr[2];
    U8  master_link4_ptr[2];
    U8  master_link5_ptr[2];
    U8  master_link6_ptr[2];
    U8  slave_link0_ptr[2];
    U8  slave_link1_ptr[2];
    U8  master_broadcast_ptr[2];

    U8  lc_dummy[3];

    U8  lc_resv_4_sco_esco[30];   //- reserved 4 SCO, eSCO

    U8  m_rx_header[2];
    U8  m_rx_payload[2];
    U8  s0_rx_header[2];
    U8  s0_rx_payload[2];
    U8  s1_rx_header[2];
    U8  s1_rx_payload[2];
    U8  sw_rx_header[2];
    U8  sw_rx_payload[2];

    SAC_LINK_CTL        sac_link_ctl[MAX_NUM_ACL_LINK]; //- S0,M0~6,S1

    SAC_LINK_CTL        sac_master_bc_link_ctl;

} LC_SAC_SW_HW_SHARE_CTL;

/**
 * The data structure defines the structure of TX EIR packet which payload
 * header size is 1 */
typedef struct
{
    U8 PayloadHeader;
    EIR_ARR EIR;

} SMALL_TX_EIR_PKT_STRU;

/**
 * The data structure defines the structure of TX EIR packet which payload
 * header size is 2 */
typedef struct
{
    U8 PayloadHeader[2];
    EIR_ARR EIR;

} LARGE_TX_EIR_PKT_STRU;

typedef union
{
    SMALL_TX_EIR_PKT_STRU Small;
    LARGE_TX_EIR_PKT_STRU Large;
    U8 WholePayload[2 + MAX_EIR_SIZE];  // consider with payload header
}
TX_EIR_PKT_UNION, OS_PTR TX_EIR_PKT_UNION_PTR;

typedef struct
{
    FOUR_BYTE_UNION_TYPE eir_advance_clk;
    U8 eir_dummy_tick;
    U8 TxPktType;
    U8 MaxEirSize;
    U8 FecRequired;
    U8 PayloadHeaderSize;
    U8 PayloadSize;
    TX_EIR_PKT_UNION_PTR pTxPayload;
    RX_BUFFER_STRU_PTR eir_rx_ptr;

} LC_EIR_CTL;

typedef struct
{
    U8                      enable;
    U8                      standby_count;
    U8                      dummy_sniff_sleep;
    FOUR_BYTE_UNION_TYPE    next_inquiryscan_instant;
    FOUR_BYTE_UNION_TYPE    next_pagescan_instant;

    U8                      queue_scan;
} LC_SCAN_CTL;

typedef struct
{
    BOOL SacScanEnable;
    BOOL IssueScan;
    BTCLK NextIntervalBoundary;
    BTCLK NextScanInstant;
    U16 LatchedClk;

    U8  clk0_sync_clk1;
    U8  number_of_iac_scan_this_time;
    U8  current_iac_idx;

    U8  clk0_15_8;
} LC_INQUIRY_SCAN_CTL;

typedef struct
{
    BOOL SacScanEnable;
    BOOL IssueScan;
    BOOL PageScanWinTimerRst;
    BTCLK NextIntervalBoundary;
    BTCLK NextScanInstant;

    U8  pagescan_idx;
    U8  clk0_sync_clk1;
    U8  new_connection_state;

    U8  clk0_15_8;
    U8  Lock;
} LC_PAGE_SCAN_CTL;

typedef struct
{
    BOOL SacScanEnable;
    BOOL IssueScan;
    BTCLK NextScanInstant;
} LC_RSSI_SCAN_CTL;


typedef struct
{
    BD_ADDR_STRU    BdAddr;
    PARITY_ARR      Parity;

    BTCLK_UNION     TrainRepetitionTimeoutExpiredTime;
    U8 XDATA_PTR    pCmdMsg;    // plan to remove

    S8              FhsRssi;
    BOOL            DoSyncToClk1;

    U16             Ninquiry_timeout;
    U8              train;

} LC_INQUIRY_CTL;

typedef struct
{
    U16                     page_timeout;
    U8                      page_response_timeout;
    U8                      new_connection_timeout;

    U8                      page_idx;
    U8                      page_lt_addr;

    U8                      sr;
    FOUR_BYTE_UNION_TYPE    clke_slave_offset;

    U16                     Npage_timeout;
    U8                      train;

    U8                      clk0_sync_clk1;
    U8                      tx_id_flag;
    LMLC_CMD_MSG_PAGE_STRU_PTR  pCmdMsg;
    CONN_HDL_STRU_PTR       pConnHdl;
    U8                      skip_reset_timer; //add by phlo 09.04.17
    U8                      ACLoss_count;
} LC_PAGE_CTL;

typedef struct
{
    U8  sw_state;
    U8  pagescan_additional;
    U8  page_inquiry_active;    //1:inquiry 2:page;
    U16 SAC_Interrupt_backup;
    U8  SAC_Index_backup;       //0:M0, 1:M1, 8:S0, 9:S1
    U8  SAC_Enable_Service;

} LC_SW_STATE_CTL;



typedef struct
{
    BT_TX_PWR_CFG_STRU      tx_gain_ctl;
    MP_BT_TX_PWR_CFG_STRU   mp_tx_gain_ctl;
    U8 inquiry_txgain_gc1;
    U8 inquiry_txgain_gc2;
    S8 pwr_ctl_level[MAX_NUM_ACL_LINK];
    U8 rssi_gap;
    S8 rssi_real[MAX_NUM_ACL_LINK];
} LC_RF_CTL;

typedef union
{
    struct
    {
        U8              bc_channel_map_current_idx      : 4;
        U8              bc_load                         : 1;
        U8              TX_request                      : 1;
        U8              bc_last_packet                  : 1;
        U8              bc_llid_start                   : 1;
    }member;
    U8  value;
} LC_MASTER_BC_BIT_CTL0;

typedef struct
{
    LC_MASTER_BC_BIT_CTL0   bit_ctl0;

    LC_TXPDU_HC_SCR_UNION_TYPE      union_lc_txpdu_hc_scr;
    TWO_BYTE_UNION_TYPE             residual_payload_length;
    LC_TXPDU_HC_BIT_CTRL_B1         TX_pdu_HC_ctrl1;
    U8 XDATA_PTR                    TX_next_mem_blk_ptr;        //- point to next LC TXPDU MEM from HC
    U8 XDATA_PTR                    TX_mem_blk_ptr;             //- point to curr LC TXPDU MEM from HC

    U8 bc_channel_map;  //- B0: Enable/Disable; B1~B7: M0~M6
    U8 bc_map_modified;
    CONN_HDL conn_hdl;
} LC_MASTER_BROADCAST_CTL;

typedef struct
{
    U8                  temp_bank_last;
    MOD_GAIN_COMP_STRU      mod_gain_comp_ctl;
    MP_MOD_GAIN_COMP_STRU   mp_mod_gain_comp_ctl;
} LC_MOD_GAIN_COMP_CTL;


//=== Davi: For Temp AudioSwitch ===
#ifdef SUPPORT_SCO_ESCO
typedef struct
{
    U8 SCO_handle;
    FOUR_BYTE_UNION_TYPE Initial_Instant_in_slot;
    U8 interval;
    U8 packet_type;
    U8 air_mode;

} LM_TEMP_CMD_ADD_SCO_TYPE;

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

} LM_TEMP_CMD_ADD_ESCO_TYPE;

typedef struct
{
    U8 dummy;
    union
    {
        LM_TEMP_CMD_ADD_SCO_TYPE    temp_add_sco;
        LM_TEMP_CMD_ADD_ESCO_TYPE   temp_add_esco;
        LM_TEMP_CMD_ADD_SNIFF_TYPE  temp_add_sniff;
    } temp_cmd;
} LM_TEMP_CMD_TYPE;


typedef struct
{
    U8 FromAudioSwitch      :1;
    U8 FromAudioMute        :1;
    U8 MuteAudioExist       :1;
    U8 UnMuteAudio          :1;
    U8 Index_0_IsSCO        :1;
    U8 Index_1_IsSCO        :1;
    U8 Idx_0_Maintain_Need  :1;
    U8 Idx_1_Maintain_Need  :1;

    U8 TempAudioIdxonCODEC;
    U8 interval[2];
    U8 service_idx[2];
}LC_AUDIOSWITCH_CTL_TYPE;
#endif
//=== Davi: For Temp AudioSwitch ===

typedef struct
{
    U8 s0_sniff_attempt_1_timeout_0 : 1;
    U8 s0_wait_LMP_acked_a1t0       : 1;
    U8 s0_sniff_timeout_recover     : 1;
    U8 s0_unsniff_process           : 1;
    U8 s1_sniff_attempt_1_timeout_0 : 1;
    U8 s1_wait_LMP_acked_a1t0       : 1;
    U8 s1_sniff_timeout_recover     : 1;
    U8 s1_unsniff_process           : 1;
} LC_MISC_BIT_CTL0;

typedef struct
{
    U8 hid_tx_one_aclu_pkt_in_sniff_window  : 1;
    U8 hid_mask_flag                        : 1;
    U8 hid_unsniff_transation               : 1;
    U8 b3                                   : 1;
    U8 b4                                   : 1;
    U8 b5                                   : 1;
    U8 b6                                   : 1;
    U8 b7                                   : 1;
} LC_MISC_BIT_CTL1;

typedef struct
{
    U8  XDATA_PTR           fhs_tr_next_ptr;
    U8                      fhs_tr_len_B0;
    U8                      fhs_tr_len_B1;
    U8                      fhs_tr_share_buffer[18];
} LC_FHS_TR_SHARE_BUFFER_TYPE;

typedef struct link_schedule_ctl_stru
{
    U8                      LinkIndex;
    FOUR_BYTE_UNION_TYPE    LeaseExpireTime;
    U8                      DataTxRxInd;
    U8                      DataActiveInd;
    FOUR_BYTE_UNION_TYPE    ActiveTimeoutTime;
    FOUR_BYTE_UNION_TYPE    DataTimeoutTime;
    FOUR_BYTE_UNION_TYPE    NextSchedulableTime;
    FOUR_BYTE_UNION_TYPE    NullPacketTimeoutTime;
    U8                      RxDataAfterNull;
    U8                      NullPacketCnt;
    U8                      NullPacket;
    U8                      L2CAPRxCnt;
    U8                      AckTxCnt;
    FOUR_BYTE_UNION_TYPE    NoPacketTimeoutTime;

} LK_SCH_CTL_STRU;

typedef struct lc_qos_ctrl_stru
{
    U8                      ServiceType;
    U32                     QosRestTime;

} LC_QOS_CTL_STRU;

#if 0
#ifdef AB1520S
typedef struct
{
    U8 packet_header_b0;
    U8 packet_header_b1;
    U8 payload_header_b0;
    U8 payload_header_b1;

} LC_30_RX_PKT_INFO;
#endif
#endif


typedef struct
{
    BTCLK_UNION InquiryScanWindowExpiredTime;
    BTCLK_UNION InquiryScanIntervalExpiredTime;
    BTCLK_UNION InquiryPageTimeoutExpiredTime;
    BTCLK_UNION PageScanWindowExpiredTime;
    BTCLK_UNION PageScanIntervalExpiredTime;
    BTCLK_UNION PageTimeoutExpiredTime;
    BTCLK_UNION PageInqSegmentExpiredTime;
    BTCLK_UNION PageInqSegmentIntervalExpiredTime;
    BTCLK_UNION ScoEscoPowerSavingExpiredTime;
    U8 sco_esco_self_power_saving;

    #if 0
    union
    {
        struct
        {
            BTCLK_UNION Slave0ExpiredTime;
            BTCLK_UNION Slave1ExpiredTime;
        } slave_sniff_transcation_timer;

    } union_share_timer_ptr;
    #endif

    U16                                 inquiry_scan_backup;
    U16                                 page_scan_backup;
    LC_FHS_TR_SHARE_BUFFER_TYPE         lc_fhs_tr_share_buffer;

    #ifdef SUPPORT_SCO_ESCO
    U8                                  audio_hw_codec_link_idx;
    U8                                  audio_hw_codec_audio_handle;
    #endif

    RX_BUFFER_STRU_PTR                  acl_share_rx_return_ptr[3];

    #if 0
    #ifdef AB1520S
    LC_30_RX_PKT_INFO                   rx_pkt_info;
    #endif
    #endif

    U8                                  num_of_queued_rs;
    U8 XDATA_PTR                        rs_command_ptr1;
    U8 XDATA_PTR                        rs_command_ptr2;

    LC_RF_CTL                           rf_ctl;
    CONN_CTL                            conn_ctl[MAX_NUM_ACL_LINK];

    LC_SCHEDULER_CTL                    sche_ctl;
    LC_EIR_CTL                          eir_ctl;
    LC_SCAN_CTL                         scan_ctl;

    LC_SAC_SW_HW_SHARE_CTL              sac_sw_hw_share_ctl;

    LC_MASTER_CLK_TRACKING_CTL          master_clk_tracking_ctl;
#ifdef SUPPORT_SCO_ESCO
    LC_SLAVE_PKT_BLOCKED_MOD_CTL        slave_pkt_blocked_mod_ctl;
#endif
    LC_SERVICE_ADJUSTMENT_CTL           lc_service_adjustment_ctl;


    U8                                  sac_idx_2_conn_idx[10];
    U8                                  conn_idx_2_sac_idx[MAX_NUM_ACL_LINK];

//  U8                                  AFH_RSSI_Measurement_done;
//  U8                                  AFH_CRC_Measurement_done;

    LC_MASTER_BROADCAST_CTL             master_broadcast_ctl;

    LC_MISC_CONTROL                     misc_ctl;
#ifdef LABTEST_ENABLED
    LC_LABTEST_TOP_CTL                  lc_labtest_top_ctl;
#endif

    LC_MOD_GAIN_COMP_CTL                lc_mod_gain_comp_ctl;

    AFE_REG_INIT_STRU                   lc_misc_3wired_by_temp_ctl[8];

    U8                                  lc_delay_power_saving_timer;

    LC_MISC_BIT_CTL0                    misc_bit_ctl0;
    LC_MISC_BIT_CTL1                    misc_bit_ctl1;
#if LE_SUPPORTED_CONTROLLER
    U16 le_link_used_mask;
    LC_LE_CTL                           lc_le_ctl;
    LC_BLE_DEVICE_CTL                   BLE_device_ctl[MAX_NUM_LE_LINK];
    LC_BLE_DEVICE_SW_HW_SHARE_CTL       BLE_device_sw_hw_share_ctl[MAX_NUM_LE_LINK];
    FOUR_BYTE_UNION_TYPE                Last_Anchor_CLK[MAX_NUM_LE_LINK];
#endif
    U8                                  LastSchedLinkCtlIdx[MAX_NUM_ACL_LINK-1];
    U8                                  CurrSchedLinkCtlIdx;
    LK_SCH_CTL_STRU                     LinkSchedCtl[MAX_NUM_ACL_LINK];
    U8                                  LinkSchedStatus;
    LC_QOS_CTL_STRU                     RestCtrl[MAX_NUM_ACL_LINK];
    U8                                  NumberOfGuaranteed;
    U8                                  FlowGoStatus[MAX_NUM_ACL_LINK];
    U8                                  RxFull[MAX_NUM_ACL_LINK];
    U8                                  QoSBestEffortWeight;
    U16                                 PollInvl[MAX_NUM_ACL_LINK];

    LC_SW_STATE_CTL                     lc_sw_state_ctl;

    /* Link Controller Operations Control */
    LC_INQUIRY_SCAN_CTL             InquiryScan;
    LC_PAGE_SCAN_CTL                PageScan;
    LC_RSSI_SCAN_CTL                RssiScan;

    union
    {
        LC_INQUIRY_CTL              Inquiry;
        LC_PAGE_CTL                 Page;
        TESTMODE_CTL                TestMode;
        DIRECT_TESTMODE_CTL         DirectTestMode;
        #ifdef LABTEST_ENABLED
        LC_LABTEST_CTL              LabTest;
        #endif

    } Overlay;

    U8 Need_Reset_Pag_Inq_Seg_Timer;

    U8 active_audio_interval;

    U8 Active_Audio_Handle;

    U8 Active_eSCO_ACL_Idx;

    U8 Barge_In_eSCO_WeSCO_Going;

    U8 critical_guard_time;

    U8 AdaptivePageScanWeight;
#if LE_SUPPORTED_CONTROLLER
    #ifdef LE_ADV_ANCHOR_UPDATE_SUPPORTED
    U8 UpdateAdvAnchorReq;
    #endif
#endif
    U8 Check_Anchor_For_ACL;

    U8 Critical_Anchor_State;

    U8 SoundbarTuningFlag;

    U8 Scan_Going;

    U8 GuaranteedLinkAfterScan;

    U8 NumOfActiveACLLink;

    VOID (*SwModeResumeEntry)(VOID);

    U8 SW_Mode_Going;

    U8 Scan_One_Shot;

    U8 DSP_Sco_eSco_Isr;

    U8 ESco_Null_Packet;

	U8 ESco_In_Null_Packet_Check;

    OST ESco_In_Null_Packet_Check_TimerPtr;

    U8 eSCO_in_out[MAX_NUM_ACL_LINK];

    U8 Scan_or_Master_Role_Switch_Ongoing;

    U8 Resume_ACL_Service;
    U8 SwIntrMaskSave;
    U8 int_vec_b2;
    U8 curr_used_rx_buf_num;
    U16 rx_buf_released_cnter;

    U16 LinkIntrRecover[MAX_NUM_ACL_LINK];
    U8 LinkIntrRecover3;
    U8 InquiryTx_flag;

} LC_CTL_TYPE;

typedef struct
{
    U8 offset;
    U8 event_code;
    U8 length;
}LC_HC_EVT_HDR_STRU;


LC_EXTERN LC_CTL_TYPE XDATA             gLC_ctl;
LC_EXTERN SELECTED_AUDIO_SRC XDATA      selected_audio_src;


//=== Davi: For Temp AudioSwitch ===
#ifdef SUPPORT_SCO_ESCO
LC_EXTERN LC_AUDIOSWITCH_CTL_TYPE XDATA gLC_AudioSwitch_bit_ctl;
//LC_EXTERN LM_TEMP_CMD_TYPE XDATA      gLC_Temp_cmd[2];
LC_EXTERN FOUR_BYTE_UNION_TYPE XDATA    MaintainInst[2];
#endif
//=== Davi: For Temp AudioSwitch ===


//=== huichuan: for 1-to-2 Role Switch =====
LC_EXTERN U8 XDATA rs_is_ongoing;
//=== huichuan: for 1-to-2 Role Switch =====

typedef struct {
    U8  Link_Exist30                     : 1;
    U8  Link_Exist40                     : 1;
    U8  HWStandby                        : 1;
    U8  interlaced_done                  : 1;
    U8  LabTest_Trigger                  : 1;
    U8  LabTest_Going                    : 1;
    U8  audio_exist                      : 1;
    U8  a2dp_power_saving_enabled        : 1;
} LC_BIT_CTL0;


LC_EXTERN LC_BIT_CTL0   gLC_bit_ctl0;

#define gLC_HWStandby                           gLC_bit_ctl0.HWStandby
#define gLC_interlaced_done                     gLC_bit_ctl0.interlaced_done
#define gLC_interlace_on                        gLC_interlaced_done
#define gLC_LabTest_Trigger                     gLC_bit_ctl0.LabTest_Trigger
#define gLC_LabTest_Going                       gLC_bit_ctl0.LabTest_Going
#define gLC_A2DP_Power_Saving_Enabled           gLC_bit_ctl0.a2dp_power_saving_enabled
#define gLC_30_Link_Exist                       gLC_bit_ctl0.Link_Exist30
#define gLC_40_Link_Exist                       gLC_bit_ctl0.Link_Exist40

#define gLC_Link_Exist                          (gLC_bit_ctl0.Link_Exist30 || gLC_bit_ctl0.Link_Exist40)
#define gLC_40_Link_ongoing (gLC_ctl.lc_le_ctl.device_status.device0 || gLC_ctl.lc_le_ctl.device_status.device1)


typedef struct {
    U8  SAC_Scan_Need               : 1;
    U8  SAC_Inquiry_Scan_Enable     : 1;
    U8  SAC_Page_Scan_Enable        : 1;
    U8  SAC_SCO_ESCO_Going          : 1;
    U8  SAC_ESCO_Active             : 1;
    U8  SAC_Sniff_Active            : 1;
    U8  SAC_New_Connection_Going    : 1;
    U8  SAC_Sleep_Support           : 1;
} LC_BIT_CTL1;

typedef struct {
    U8  SAC_MCU_Pause_Support                   : 1;
    U8  SAC_Power_Saving_Delay                  : 1;
    U8  SAC_Power_Saving_Disable                : 1;
    U8  BargeIn_ReTxWin_Mod_Start               : 1;
    U8  BargeIn_ReTxWin_Mod_End                 : 1;
    U8  BargeIn_ReTxWin_Mod_Going               : 1;
    U8  BargeIn_ReTxWin_Mod_Disable             : 1;
    U8  mcu_frozen_wakeup_by_hw_slottick        : 1;
} LC_BIT_CTL2;

typedef struct {
    U8 SAC_BLE_update_ongoing              : 1;
    U8 rsvd                                : 7;
} LC_BIT_CTL3;


LC_EXTERN LC_BIT_CTL1   gLC_BIT_ctl1;
LC_EXTERN LC_BIT_CTL2   gLC_BIT_ctl2;
LC_EXTERN LC_BIT_CTL3   gLC_BIT_ctl3;

#define gLC_SAC_Scan_Need               gLC_BIT_ctl1.SAC_Scan_Need
#define gLC_SAC_Inquiry_Scan_Enable     gLC_BIT_ctl1.SAC_Inquiry_Scan_Enable
#define gLC_SAC_Page_Scan_Enable        gLC_BIT_ctl1.SAC_Page_Scan_Enable
#define gLC_SAC_SCO_ESCO_Going          gLC_BIT_ctl1.SAC_SCO_ESCO_Going
#define gLC_SAC_ESCO_Active             gLC_BIT_ctl1.SAC_ESCO_Active
#define gLC_SAC_Sniff_Active            gLC_BIT_ctl1.SAC_Sniff_Active
#define gLC_SAC_New_Connection_Going    gLC_BIT_ctl1.SAC_New_Connection_Going
#define gLC_SAC_Sleep_Support           gLC_BIT_ctl1.SAC_Sleep_Support
#define gLC_SAC_MCU_Pause_Support       gLC_BIT_ctl2.SAC_MCU_Pause_Support
#define gLC_SAC_Power_Saving_Delay      gLC_BIT_ctl2.SAC_Power_Saving_Delay
#define gLC_SAC_Power_Saving_Disable    gLC_BIT_ctl2.SAC_Power_Saving_Disable   //- combine flag of gLC_ctl.sche_ctl.pre_unsniff_mask
                                                                        //-                 gLC_ctl.sche_ctl.afh_instant_wait
                                                                        //-                 gLC_ctl.sche_ctl.dummy_sniff_mask
#ifndef LC_BARGE_IN_NOT_SUPPORTED
#define gLC_BargeIn_ReTxWin_Mod_Start   gLC_BIT_ctl2.BargeIn_ReTxWin_Mod_Start
#define gLC_BargeIn_ReTxWin_Mod_End     gLC_BIT_ctl2.BargeIn_ReTxWin_Mod_End
#define gLC_BargeIn_ReTxWin_Mod_Going   gLC_BIT_ctl2.BargeIn_ReTxWin_Mod_Going
#define gLC_BargeIn_ReTxWin_Mod_Disable gLC_BIT_ctl2.BargeIn_ReTxWin_Mod_Disable
#endif
#define gLC_MCU_Frozen_Wakeup_by_HW_Slottick    gLC_BIT_ctl2.mcu_frozen_wakeup_by_hw_slottick




///////////////////////////////////////////////////////////
//  Function declaration;
///////////////////////////////////////////////////////////
VOID             LC_Init (VOID);
VOID             LC_INT0_Init (VOID);
EXTERN VOID LC_SetupTxEIR(HCI_CMD_WR_EIR_STRU_PTR pCmd);
VOID             LC_ForceLCIntoIdleState (VOID);
VOID             LC_ModGainCompensation (VOID);
VOID             LC_SoftReset (VOID);

///////////////////////////////////////////////////////////
//  Macro definition;
///////////////////////////////////////////////////////////
#define LC_SET_SLOT_TICK_STATE(S, B)            (S = B)
#define LC_CHECK_SLOT_TICK_STATE(S, C)          (S == C)
#define LC_CHECK_SLOT_TICK_STATE_BEFORE(S, C)   (S < C)
#define LC_CHECK_SLOT_TICK_STATE_AFTER(S, C)    (S > C)

#define LC_GET_LL_CMD_OPCODE(ptr)               *ptr
#define LC_GET_LL_CMD_PARAM(ptr,N)              *(ptr+1+N)

#define LC_GET_LM_CMD_OPCODE(ptr)               *(ptr + *ptr)
#define LC_GET_LM_CMD_OPCODE_PTR(ptr)           (U8 XDATA_PTR)(ptr + *ptr)
#define LC_GET_LM_CMD_PARAM_PTR(ptr)            (U8 XDATA_PTR)(ptr + *ptr + 1)
#define LC_GET_HC_CMD_OPCODE(ptr)               *(ptr + *ptr)
#define LC_GET_HC_CMD_OPCODE_PTR(ptr)           (U8 XDATA_PTR)(ptr + *ptr)
#define LC_GET_HC_CMD_PARAM_PTR(ptr)            (U8 XDATA_PTR)(ptr + *ptr + 1)


#define LC_SET_TX_PAON_TIME(A)                  LC_SFR_WRITE_BY_C2(C2OFFS_RW_PAON_TIME, A)
#define LC_SET_RF_PLLON_TIME(A)                 LC_SFR_WRITE_BY_C2(C2OFFS_RW_RF_PLLON_TIME, A)
#define LC_SET_RF_RXON_TIME(A)                  LC_SFR_WRITE_BY_C2(C2OFFS_RW_RF_RXON_TIME, A)
#define LC_SET_RF_TXON_TIME(A)                  LC_SFR_WRITE_BY_C2(C2OFFS_RW_RF_TXON_TIME, A)

#define LC_SET_TX_DELAY_AFTER_SLOT_TICK(A)      LC_SFR_WRITE_BY_C2(C2OFFS_RW_TX_DELAY_AFTER_SLOT_TICK, A)

#define LC_HOP_SELECT(A)                        LC_SFR_WRITE(SFR_WO_HOP_SEQUENCE, A)
#define LC_START_HOP_GEN()                      LC_SFR_WRITE(SFR_RW_HOP_FREQ, TRUE)
#define LC_HOP_OK()                             LC_SFR_READ(SFRB_RO_HOP_OK)
#define LC_RD_HOP_FREQ()                        LC_SFR_READ(SFR_RW_HOP_FREQ)
#define LC_WRITE_HOP_FREQ(A)                    LC_SFR_WRITE(SFR_RW_HOPFREQ, A)

#define LC_HOP_RX_SELECT()                      LC_SFR_WRITE(SFRB_RW_HOP_TR, 1)
#define LC_HOP_TX_SELECT()                      LC_SFR_WRITE(SFRB_RW_HOP_TR, 0)
#define LC_DISABLE_TX()                         LC_SFR_WRITE(SFRB_RW_ENABLE_TX, 0)
#define LC_DISABLE_RX()                         LC_SFR_WRITE(SFRB_RW_ENABLE_RX, 0)
#define LC_DISABLE_TRX()                        {LC_SFR_WRITE(SFRB_RW_ENABLE_RX, 0); LC_SFR_WRITE(SFRB_RW_ENABLE_TX, 0);}

#define LC_ENABLE_TX()                          {LC_DISABLE_TRX(); while(LC_SAC_GET_RF_TX_RAMP_DOWN_ACTIVE());LC_SFR_WRITE(SFRB_RW_ENABLE_TX, 1);}
#define LC_ENABLE_RX()                          {LC_DISABLE_TRX(); LC_SFR_WRITE(SFRB_RW_ENABLE_RX, 1);}

#define LC_GET_TX_ENABLE()                      (SFRB_RW_ENABLE_TX)
#define LC_GET_RX_ENABLE()                      (SFRB_RW_ENABLE_RX)

#define LC_ENABLE_MANUAL_RX_WINDOW()            LC_SFR_WRITE(SFRB_RW_MANUAL_RXWINDOW, 1)
#define LC_DISABLE_MANUAL_RX_WINDOW(A)          {LC_SFR_WRITE(SFRB_RW_MANUAL_RXWINDOW, 0);     \
                                                 LC_SFR_WRITE_BY_C2(C2OFFS_RW_RX_WINDOW_LENGTH, A);}

#define LC_SELECT_ID_PACKET()                   LC_SFR_WRITE(SFRB_RW_ID_SEL, TRANSMIT_ID_PACKET)
#define LC_SELECT_NON_ID_PACKET()               LC_SFR_WRITE(SFRB_RW_ID_SEL, TRANSMIT_NORMAL_PACKET)

#define LC_WHITENING_SEL_XIR40()                LC_SFR_WRITE(SFRB_RW_WHITENING_SEL, WHITENING_SEL_XIR40)
#define LC_WHITENING_SEL_CLK61()                LC_SFR_WRITE(SFRB_RW_WHITENING_SEL, WHITENING_SEL_CLK61)
#define LC_ENABLE_WHITENING()                   LC_SFR_WRITE(SFRB_RW_ENABLE_WHITENING, 1)
#define LC_DISABLE_WHITENING()                  LC_SFR_WRITE(SFRB_RW_ENABLE_WHITENING, 0)


#define LC_GET_CLK0_15_8()                      LC_SFR_READ_BY_C2(C2OFFS_RW_CLK0_B1)
#define LC_GET_CLK0_1()                         (LC_SFR_READ_BY_C2(C2OFFS_RW_CLK0_B0) & 0x02)
#define LC_GET_CLK1_1()                         (LC_SFR_READ_BY_C2(C2OFFS_RW_CLK1_B0) & 0x02)
#define LC_GET_CLK2_1()                         (LC_SFR_READ_BY_C2(C2OFFS_RW_CLK2_B0) & 0x02)
#define LC_GET_CLK3_1()                         (LC_SFR_READ_BY_C2(C2OFFS_RW_CLK3_B0) & 0x02)
#define LC_GET_CLK0_5()                         (LC_SFR_READ_BY_C2(C2OFFS_RW_CLK0_B0) & 0x20)

#define LC_SET_MASTER_POLL_INTERVAL(A)          {LC_SFR_WRITE_BY_C2(C2OFFS_RW_MASTER_POLL_TIMER, (U8)A);       \
                                                 LC_SFR_WRITE_BY_FE(FEOFFS_RW_POLL_NEXT_INTERVAL_B0, (U8)A);   \
                                                 LC_SFR_CONT_WRITE_BY_FE((U16)A>>8);                           \
                                                }
#define LC_SET_MASTER_POLL_INSTANT(A)           {LC_SFR_WRITE_BY_FE(FEOFFS_RW_POLL_NEXT_INSTANT_B0, (U8)A);    \
                                                 LC_SFR_CONT_WRITE_BY_FE((U16)A>>8);                           \
                                                }


#define LC_MASTER_POLL_INTERVAL_ANCHOR()            (LC_SFR_READ_BY_C2(C2OFFS_RW_MASTER_POLL_TIMER) == 0)

#define LC_SET_TX_LT_ADDR_AND_PACKET_TYPE(A,B)      LC_SFR_WRITE_BY_C2(C2OFFS_RW_TX_PACKET_LT_ADDR, A); \
                                                    LC_SFR_CONT_WRITE_BY_C2(B)

#define LC_SET_AC_SYNC_THRESHOLD(A)             LC_SFR_WRITE_BY_C2(C2OFFS_RW_AC_SYNC_THRESHOLD_L, A);

#define LC_MASTER_READ_PACKET_TYPE_IN_PACKET_HEADER()       ((gLC_ctl.sac_sw_hw_share_ctl.m_rx_header[0] & LC_RX_PACKET_INFO_TYPE_MASK) >> 3)
#define LC_SLAVE0_READ_PACKET_TYPE_IN_PACKET_HEADER()       ((gLC_ctl.sac_sw_hw_share_ctl.s0_rx_header[0] & LC_RX_PACKET_INFO_TYPE_MASK) >> 3)
#define LC_SLAVE1_READ_PACKET_TYPE_IN_PACKET_HEADER()       ((gLC_ctl.sac_sw_hw_share_ctl.s1_rx_header[0] & LC_RX_PACKET_INFO_TYPE_MASK) >> 3)
#define LC_SW_READ_PACKET_TYPE_IN_PACKET_HEADER()           ((gLC_ctl.sac_sw_hw_share_ctl.sw_rx_header[0] & LC_RX_PACKET_INFO_TYPE_MASK) >> 3)

#define LC_MASTER_READ_PACKET_LT_ADDR_IN_PACKET_HEADER()    (gLC_ctl.sac_sw_hw_share_ctl.m_rx_header[0] & LC_RX_PACKET_INFO_LTADDR_MASK)
#define LC_SLAVE0_READ_PACKET_LT_ADDR_IN_PACKET_HEADER()    (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_header[0] & LC_RX_PACKET_INFO_LTADDR_MASK)
#define LC_SLAVE1_READ_PACKET_LT_ADDR_IN_PACKET_HEADER()    (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_header[0] & LC_RX_PACKET_INFO_LTADDR_MASK)
#define LC_SW_READ_PACKET_LT_ADDR_IN_PACKET_HEADER()        (gLC_ctl.sac_sw_hw_share_ctl.sw_rx_header[0] & LC_RX_PACKET_INFO_LTADDR_MASK)

#define LC_MASTER_CLEAR_PACKET_LT_ADDR_IN_PACKET_HEADER()    (gLC_ctl.sac_sw_hw_share_ctl.m_rx_header[0] &= 0xF8)
#define LC_SLAVE0_CLEAR_PACKET_LT_ADDR_IN_PACKET_HEADER()    (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_header[0] &= 0xF8)
#define LC_SLAVE1_CLEAR_PACKET_LT_ADDR_IN_PACKET_HEADER()    (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_header[0] &= 0xF8)

#define LC_MASTER_READ_PACKET_TIME_STAMP()                  (gLC_ctl.sac_sw_hw_share_ctl.m_rx_header[1] & LC_RX_PACKET_INFO_TIME_STAMP_MASK)
#define LC_SLAVE0_READ_PACKET_TIME_STAMP()                  (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_header[1] & LC_RX_PACKET_INFO_TIME_STAMP_MASK)
#define LC_SLAVE1_READ_PACKET_TIME_STAMP()                  (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_header[1] & LC_RX_PACKET_INFO_TIME_STAMP_MASK)

#define LC_MASTER_CRC_OK()                                  (gLC_ctl.sac_sw_hw_share_ctl.m_rx_payload[1] & LC_RX_PACKET_INFO_CRC_CORRECT_MASK)
#define LC_SLAVE0_CRC_OK()                                  (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_payload[1] & LC_RX_PACKET_INFO_CRC_CORRECT_MASK)
#define LC_SLAVE1_CRC_OK()                                  (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_payload[1] & LC_RX_PACKET_INFO_CRC_CORRECT_MASK)
#define LC_SW_CRC_OK()                                      (gLC_ctl.sac_sw_hw_share_ctl.sw_rx_payload[1] & LC_RX_PACKET_INFO_CRC_CORRECT_MASK)

#define LC_MASTER_HEC_OK()                                  (gLC_ctl.sac_sw_hw_share_ctl.m_rx_header[1] & LC_RX_PACKET_INFO_HEC_CORRECT_MASK)
#define LC_SLAVE0_HEC_OK()                                  (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_header[1] & LC_RX_PACKET_INFO_HEC_CORRECT_MASK)
#define LC_SLAVE1_HEC_OK()                                  (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_header[1] & LC_RX_PACKET_INFO_HEC_CORRECT_MASK)
#define LC_SW_HEC_OK()                                      (gLC_ctl.sac_sw_hw_share_ctl.sw_rx_header[1] & LC_RX_PACKET_INFO_HEC_CORRECT_MASK)

#define LC_MASTER_READ_PACKET_LLID_IN_PACKET_PAYLOAD()      ((gLC_ctl.sac_sw_hw_share_ctl.m_rx_payload[1] & LC_RX_PACKET_INFO_PAYLOAD_HEADER_LLID_MASK) << 4)
#define LC_SLAVE0_READ_PACKET_LLID_IN_PACKET_PAYLOAD()      ((gLC_ctl.sac_sw_hw_share_ctl.s0_rx_payload[1] & LC_RX_PACKET_INFO_PAYLOAD_HEADER_LLID_MASK) << 4)
#define LC_SLAVE1_READ_PACKET_LLID_IN_PACKET_PAYLOAD()      ((gLC_ctl.sac_sw_hw_share_ctl.s1_rx_payload[1] & LC_RX_PACKET_INFO_PAYLOAD_HEADER_LLID_MASK) << 4)
#define LC_SW_READ_PACKET_LLID_IN_PACKET_PAYLOAD()          ((gLC_ctl.sac_sw_hw_share_ctl.sw_rx_payload[1] & LC_RX_PACKET_INFO_PAYLOAD_HEADER_LLID_MASK) << 4)
#define LC_SW_CLEAR_LLID()                                  (gLC_ctl.sac_sw_hw_share_ctl.sw_rx_payload[1] &= 0xF3)

#define LC_MASTER_READ_PACKET_FLOW_IN_PACKET_PAYLOAD()      ((gLC_ctl.sac_sw_hw_share_ctl.m_rx_payload[1] & LC_RX_PACKET_INFO_PAYLOAD_HEADER_FLOW_MASK) << 1)
#define LC_SLAVE0_READ_PACKET_FLOW_IN_PACKET_PAYLOAD()      ((gLC_ctl.sac_sw_hw_share_ctl.s0_rx_payload[1] & LC_RX_PACKET_INFO_PAYLOAD_HEADER_FLOW_MASK) << 1)
#define LC_SLAVE1_READ_PACKET_FLOW_IN_PACKET_PAYLOAD()      ((gLC_ctl.sac_sw_hw_share_ctl.s1_rx_payload[1] & LC_RX_PACKET_INFO_PAYLOAD_HEADER_FLOW_MASK) << 1)
#define LC_SW_READ_PACKET_FLOW_IN_PACKET_PAYLOAD()          ((gLC_ctl.sac_sw_hw_share_ctl.sw_rx_payload[1] & LC_RX_PACKET_INFO_PAYLOAD_HEADER_FLOW_MASK) << 1)

#define LC_MASTER_READ_PACKET_ARQN_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.m_rx_header[1] & LC_RX_PACKET_INFO_ARQN_MASK)
#define LC_SLAVE0_READ_PACKET_ARQN_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_header[1] & LC_RX_PACKET_INFO_ARQN_MASK)
#define LC_SLAVE1_READ_PACKET_ARQN_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_header[1] & LC_RX_PACKET_INFO_ARQN_MASK)
#define LC_SW_READ_PACKET_ARQN_IN_PACKET_HEADER()           (gLC_ctl.sac_sw_hw_share_ctl.sw_rx_header[1] & LC_RX_PACKET_INFO_ARQN_MASK)

#define LC_MASTER_READ_PACKET_SEQN_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.m_rx_header[1] & LC_RX_PACKET_INFO_SEQN_MASK)
#define LC_SLAVE0_READ_PACKET_SEQN_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_header[1] & LC_RX_PACKET_INFO_SEQN_MASK)
#define LC_SLAVE1_READ_PACKET_SEQN_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_header[1] & LC_RX_PACKET_INFO_SEQN_MASK)
#define LC_SW_READ_PACKET_SEQN_IN_PACKET_HEADER()           (gLC_ctl.sac_sw_hw_share_ctl.sw_rx_header[1] & LC_RX_PACKET_INFO_SEQN_MASK)

#define LC_MASTER_READ_PACKET_FLOW_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.m_rx_header[0] & LC_RX_PACKET_INFO_FLOW_MASK)
#define LC_SLAVE0_READ_PACKET_FLOW_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_header[0] & LC_RX_PACKET_INFO_FLOW_MASK)
#define LC_SLAVE1_READ_PACKET_FLOW_IN_PACKET_HEADER()       (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_header[0] & LC_RX_PACKET_INFO_FLOW_MASK)
#define LC_SW_READ_PACKET_FLOW_IN_PACKET_HEADER()           (gLC_ctl.sac_sw_hw_share_ctl.sw_rx_header[0] & LC_RX_PACKET_INFO_FLOW_MASK)

#define LC_MASTER_READ_PAYLOAD_LENGTH_B0()                  gLC_ctl.sac_sw_hw_share_ctl.m_rx_payload[0]
#define LC_SLAVE0_READ_PAYLOAD_LENGTH_B0()                  gLC_ctl.sac_sw_hw_share_ctl.s0_rx_payload[0]
#define LC_SLAVE1_READ_PAYLOAD_LENGTH_B0()                  gLC_ctl.sac_sw_hw_share_ctl.s1_rx_payload[0]
#define LC_SW_READ_PAYLOAD_LENGTH_B0()                      gLC_ctl.sac_sw_hw_share_ctl.sw_rx_payload[0]

#define LC_MASTER_READ_PAYLOAD_LENGTH_B1()                  (gLC_ctl.sac_sw_hw_share_ctl.m_rx_payload[1] & 0x03)
#define LC_SLAVE0_READ_PAYLOAD_LENGTH_B1()                  (gLC_ctl.sac_sw_hw_share_ctl.s0_rx_payload[1] & 0x03)
#define LC_SLAVE1_READ_PAYLOAD_LENGTH_B1()                  (gLC_ctl.sac_sw_hw_share_ctl.s1_rx_payload[1] & 0x03)
#define LC_SW_READ_PAYLOAD_LENGTH_B1()                      (gLC_ctl.sac_sw_hw_share_ctl.sw_rx_payload[1] & 0x03)

#define LC_READ_FHS_LT_ADDR()                   (LC_SFR_READ_BY_C2(C2OFFS_RW_FHS_LT_ADDR))

#define LC_RF_CONTROL_BY_BTC()                  LC_SFR_WRITE_BY_C2(C2OFFS_RW_RFCTRL_SW, 0x00);
#define LC_RF_CONTROL_BY_MCU()                  LC_SFR_WRITE_BY_C2(C2OFFS_RW_RFCTRL_SW, 0x01);
#define LC_RF_WRITE_TR(A)                       LC_SFR_WRITE_BY_C2(C2OFFS_RW_REG3_TR_MCU, A);
#define LC_RF_READ_TR()                         LC_SFR_READ_BY_C2(C2OFFS_RW_REG3_TR_MCU);

//=== For Slave Packet Blocked Monitor ===
#ifdef SUPPORT_SCO_ESCO
#define LC_SLAVE_PKT_BLOCKED_CTL                gLC_ctl.slave_pkt_blocked_mod_ctl
#endif
//=== For Slave Packet Blocked Monitor ===

#define LC_SLAVE_SERVICE_ADJUSTMENT_CTL         gLC_ctl.lc_service_adjustment_ctl


//====== For For Page Segmentation ======
//====== For For Page Segmentation ======

////////////////-- AVOID BTEEPROM complier error
//#define LC_MISC_SUPP_FEA_MEM_IO_ROM_DEFAULT_INPUT()           (gLC_ctl.misc_ctl.lc_misc_supp_fea0 & LC_MISC_SUPP_FEA0_MEM_IO_ROM_DEFAULT_INPUT)

EXTERN U8 LC_CaculateGfskGc(
    IN      S8 PwrLevel,
    IN      U8 CurrBaseGc);



EXTERN SYS_ERRCODE LC_AllocateEIR(VOID);
EXTERN VOID LC_SetupTxEIR(HCI_CMD_WR_EIR_STRU_PTR pCmd);



#define LC_LE_LINK_EXIST_MASK 0x03



EXTERN VOID LC_FHSPKT_WriteEirBit(
    IN      FHS_ARR XDATA FHS,
    IN      U8 EirBit);

EXTERN U8 LC_FHSPKT_ReadEirBit(
    IN      FHS_ARR XDATA FHS);

EXTERN VOID LC_FHSPKT_WriteSR(
    IN      FHS_ARR XDATA FHS,
    IN      U8 SR);

EXTERN U8 LC_FHSPKT_ReadSR(
    IN      FHS_ARR XDATA FHS);

EXTERN VOID LC_FHSPKT_WriteReserved(
    IN      FHS_ARR XDATA FHS,
    IN      U8 Reserved);

EXTERN U8 LC_FHSPKT_ReadReserved(
    IN      FHS_ARR XDATA FHS);

EXTERN VOID LC_FHSPKT_WriteLTAddr(
    IN      FHS_ARR XDATA FHS,
    IN      U8 LtAddr);

EXTERN U8 LC_FHSPKT_ReadLTAddr(
    IN      FHS_ARR XDATA FHS);

EXTERN VOID LC_FHSPKT_WriteParity(
    IN      FHS_ARR XDATA FHS,
    IN      PARITY_ARR XDATA Parity);

EXTERN VOID LC_FHSPKT_ReadParity(
    IN      FHS_ARR XDATA FHS,
    OUT     PARITY_ARR XDATA Parity);

EXTERN VOID LC_FHSPKT_WriteBdAddr(
    IN      FHS_ARR XDATA FHS,
    IN      BD_ADDR_STRU_PTR pBdAddr);

EXTERN VOID LC_FHSPKT_ReadBdAddr(
    IN      FHS_ARR XDATA FHS,
    IN      BD_ADDR_STRU_PTR pBdAddr);

EXTERN VOID LC_FHSPKT_WriteCod(
    IN      FHS_ARR XDATA FHS,
    IN      COD_ARR XDATA COD);

EXTERN U8 LC_FHSPKT_ReadCod(
    IN      FHS_ARR XDATA FHS,
    OUT     COD_ARR XDATA COD);

EXTERN VOID LC_FHSPKT_WriteClk27_2(
    IN      FHS_ARR XDATA FHS,
    IN      BTCLK Clock);

EXTERN BTCLK LC_FHSPKT_ReadClk(
    IN      FHS_ARR XDATA FHS);




#endif

