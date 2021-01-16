//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _LC_H_
#define _LC_H_

#include "config.h"
#include "ctlr_config.h"
#include "cpu.h"
#include "os.h"
#include "LC_interface.h"
#include "LC_DirectTestmode.h"
#include "..\LL\LL_interface.h"
#include "..\HC\HC_interface.h"
#define LC_ROLE_SWITCH_SUCCESS               0x00
#define LC_ROLE_SWITCH_FULL                  0x01
#define LC_ROLE_SWITCH_ONGOING               0x02
#define LC_ROLE_SWITCH_INSTANT_PASSED        0x03
#define LC_ROLE_SWITCH_NEWCONNECTION_TIMEOUT 0x04
#define LC_ROLE_SWITCH_LINK_INDEX_CONFLICT   0x05
#define LC_ROLE_SWITCH_RX_BUF_USED           0x06

#define NewConnTO 32
#define LC_DEBUG_SAC_MASTERADDSLAVE         0
#define LC_DEBUG_SAC_ADDSLAVE               0

#define LC_DEBUG_SAC_INQUIRYSCAN            0
#define LC_DEBUG_SAC_PAGESCAN               0
#define LC_DEBUG_SAC_PAGE                   0
#define LC_DEBUG_SAC_AFH                    0
#define LC_DEBUG_SAC_HOLD                   0
#define LC_DEBUG_SAC_SNIFF                  0
#define LC_DEBUG_SAC_ROLESWITCH             0
#define LC_DEBUG_SAC_TESTMODE               0
#define LC_DEBUG_SAC_BARGE_IN_INQUIRY       0
#define LC_DEBUG_SAC_BARGE_IN_INQUIRYSCAN   0
#define LC_DEBUG_SAC_BARGE_IN_PAGE          0
#define LC_DEBUG_SAC_BARGE_IN_PAGESCAN      0
#define LC_DEBUG_SAC_BARGE_IN_M_RS          0
#define LC_DEBUG_SAC_BARGE_IN_S_RS          0
#define LC_DEBUG_SAC_MASTER_CLK_TRACKING    0

#define LC_DEBUG_TEST_HW_STANDBY            0

#ifdef SUPPORT_SCO_ESCO
    #define LC_SCO_AUDIO_PRIORITY_FIRST_LINK    1   //- first sco/esco link has hw audio resource
    #define LC_SUPPORT_SAC_BARGE_IN             1
    #define LC_SUPPORT_SAC_BARGE_IN_SCO_ESCO    1
    #define LC_SUPPORT_SAC_BARGE_IN_SNIFF       0
#else
    //- Shall NOT set these definition to 1 if SCO/eSCO is not supported
    #define LC_BARGE_IN_NOT_SUPPORTED
    #define LC_SCO_AUDIO_PRIORITY_FIRST_LINK    0
    #define LC_SUPPORT_SAC_BARGE_IN             0
    #define LC_SUPPORT_SAC_BARGE_IN_SCO_ESCO    0
    #define LC_SUPPORT_SAC_BARGE_IN_SNIFF       0
#endif

#define LC_NEW_MODEM                        1

//////////////////////////////////////////////////////- don't change this section
#if !(LC_SUPPORT_SAC_BARGE_IN)
    #undef LC_DEBUG_SAC_BARGE_IN_INQUIRY
    #undef LC_DEBUG_SAC_BARGE_IN_INQUIRYSCAN
    #undef LC_DEBUG_SAC_BARGE_IN_PAGE
    #undef LC_DEBUG_SAC_BARGE_IN_PAGESCAN
    #undef LC_DEBUG_SAC_BARGE_IN_M_RS
    #undef LC_DEBUG_SAC_BARGE_IN_S_RS
    #undef LC_SUPPORT_SAC_BARGE_IN_SCO_ESCO
    #undef LC_SUPPORT_SAC_BARGE_IN_SNIFF

    #define LC_SUPPORT_SAC_BARGE_IN_SCO_ESCO    0
    #define LC_SUPPORT_SAC_BARGE_IN_SNIFF       0
    #define LC_DEBUG_SAC_BARGE_IN_INQUIRY       0
    #define LC_DEBUG_SAC_BARGE_IN_INQUIRYSCAN   0
    #define LC_DEBUG_SAC_BARGE_IN_PAGE          0
    #define LC_DEBUG_SAC_BARGE_IN_PAGESCAN      0
    #define LC_DEBUG_SAC_BARGE_IN_M_RS          0
    #define LC_DEBUG_SAC_BARGE_IN_S_RS          0
#endif


/**
 * Null function. A null function means the function will not change any states.
 *
 * @author Charles Su <CharlesSu@airoha.com.tw>
 */
#define LCS_NullFunc OS_NullFunc

//////////////////////////////////////////////////////-

//========================================== LM CMD
typedef struct
{
    BD_ADDR     bd_addr;
    PARITY      parity;
    U8          sr;
    U8          rteserved;
    U8          clk_offset[2];
    U8          conn_handle;
    CONN_HDL_STRU_PTR pConnHdl;

} LM_CMD_PAGE_TYPE;

typedef struct
{
    U8 test_scenario;
    U8 hopping_mode;
    U8 TX_freq;
    U8 RX_freq;
    U8 power_control_mode;
    U8 poll_period;
    U8 packet_type;
    U8 length_of_test_data[2];
} LM_CMD_TEST_CTL_TYPE;

typedef struct
{
    U8  tx_freq;
    U8  length_of_tx_test;
    U8  packet_payload;
} LM_CMD_DIRECT_TEST_TX_CTL_TYPE;

#ifdef SUPPORT_SCO_ESCO
typedef struct
{
    U8 SCO_handle;
    FOUR_BYTE_UNION_TYPE Initial_Instant_in_slot;
    U8 interval;
    U8 packet_type;
    U8 air_mode;

} LM_CMD_ADD_SCO_TYPE;
#endif

#ifdef SUPPORT_SCO_ESCO
typedef struct
{
    U8 eSCO_handle;
    U8 eSCO_LT_ADDR;
    FOUR_BYTE_UNION_TYPE Initial_Instant_in_slot;
    U8 interval;
    U8 w_esco;
    U8 m2s_packet_type;
    U8 s2m_packet_type;
    union {
        U16 value16;
        struct {
            U8 B0;
            U8 B1;
        } field;
    } m2s_packet_len;
    union {
        U16 value16;
        struct {
            U8 B0;
            U8 B1;
        } field;
    } s2m_packet_len;
    U8 air_mode;

} LM_CMD_ADD_ESCO_TYPE, OS_PTR LM_CMD_ADD_ESCO_TYPE_PTR;
#endif

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
} LM_CMD_ADD_SNIFF_TYPE;
#if (NEW_LC_SNIFF_TRANSITION)
typedef struct
{
    U8                      SNIFF_TRANSITION_handle;
    FOUR_BYTE_UNION_TYPE    next_instant_in_slot;
    U8                      interval[2];
    U8                      attempt[2];
    U8                      timeout[2];

    U8                      t32k_625u;
    U8                      t32k_8u;
    U8                      t12m_625u;
    U8                      t12m_8u;

}LM_CMD_ADD_SNIFF_TRANSITION_TYPE;
#endif
typedef struct
{
    U8                      SNIFF_subrate_handle;
    FOUR_BYTE_UNION_TYPE    subrate_next_instant_in_slot;
    U8                      subrate_timeout[2];
    U8                      subrate_num;

} LM_CMD_ADD_SNIFF_SUBRATE_TYPE;

typedef struct
{
    U8                      HOLD_handle;            //- unused
    FOUR_BYTE_UNION_TYPE    hold_instant_in_slot;   //- unused
    U16                     hold_time_in_slot;      //- unused
} LM_CMD_ADD_HOLD_TYPE;

typedef struct
{
    U8                      AFH_handle;
    U8                      AFH_acked_by_slave;
    U8                      LM_timeout;
    FOUR_BYTE_UNION_TYPE    AFH_exact_instant;
    U8                      AFH_mode;
    U8                      AFH_channel_map[10];
} LM_CMD_SET_AFH_TYPE;

typedef struct
{
    U8                      ACLU_handle;
    U8                      ACLU_go;
    U8                      ACLU_edr;
} LM_CMD_ACLU_TYPE;

typedef struct
{
    U8                      ENCR_handle;
    U8                      ENCR_on;
    U8                      ENCR_Kc_[16];
} LM_CMD_ENCRYPTION_TYPE;

typedef struct
{
    U8                      RS_handle;
    U8                      RS_new_handle;
    U8                      RS_instant[4];
    U8                      RS_slot_offset[2];
    U8                      RS_instant_passed;
} LM_CMD_ROLESWITCH_TYPE;

typedef struct
{
    U8                      handle;
    U8                      rx_max_slot;
    U8                      tx_max_slot;
} LM_CMD_MAX_SLOT_TYPE;

typedef struct
{
    U8                      handle;
} LM_CMD_SERVICE_CHANGE_TYPE;

typedef struct
{
    U8                      BC_attach_handle;
    U8                      bc_encr_on;
} LM_CMD_BC_ENCRYPTION_TYPE;

#ifdef SUPPORT_SCO_ESCO
typedef struct
{
    U8                      audio_handle;
    U8                      mute;
} LM_CMD_AUDIO_SWITCH_TYPE;
#endif

#ifdef SUPPORT_SCO_ESCO
typedef struct
{
    U8                      audio_handle;
    U8                      is_null;
} LM_CMD_ESCO_NULL_PACKET_TYPE;
#endif

#ifdef SUPPORT_SCO_ESCO
typedef struct
{
    U8                      audio_handle;
} LM_CMD_REMOVE_SCO_ESCO_TYPE;
#endif

typedef struct
{
    U8                      handle;
    U8                      extend_level;
} LM_CMD_NORMAL_SEARCH_EXTEND_TYPE;

typedef struct
{
    U8                      handle;
    U8                      disable;
} LM_CMD_RETXWIN_MOD_TYPE;

typedef struct
{
    U8 opcode;
    union
    {
        U8                                  first_byte;
        LM_CMD_PAGE_TYPE                    page_ctl;
        LM_CMD_ADD_SNIFF_TYPE               add_sniff;
#if (NEW_LC_SNIFF_TRANSITION)
        LM_CMD_ADD_SNIFF_TRANSITION_TYPE    add_siff_transition;
#endif
        LM_CMD_ADD_SNIFF_SUBRATE_TYPE       add_sniff_subrate;
#ifdef SUPPORT_SCO_ESCO
        LM_CMD_ADD_SCO_TYPE                 add_sco;
        LM_CMD_ADD_ESCO_TYPE                add_esco;
        LM_CMD_AUDIO_SWITCH_TYPE            audio_switch;
        LM_CMD_ESCO_NULL_PACKET_TYPE        esco_null_packet;
        LM_CMD_REMOVE_SCO_ESCO_TYPE         remove_sco_esco;
#endif
        LM_CMD_SET_AFH_TYPE                 set_afh;
        LM_CMD_ACLU_TYPE                    aclu_ctl;
        LM_CMD_ENCRYPTION_TYPE              encr_ctl;
        LM_CMD_ROLESWITCH_TYPE              rs_ctl;
        LM_CMD_TEST_CTL_TYPE                test_ctl;
        LM_CMD_ADD_HOLD_TYPE                add_hold;
        LM_CMD_MAX_SLOT_TYPE                max_slot_change;
        LM_CMD_BC_ENCRYPTION_TYPE           master_bc_encr;
        LM_CMD_NORMAL_SEARCH_EXTEND_TYPE    normal_search_ext;
        LM_CMD_RETXWIN_MOD_TYPE             retxwin_mod;
    } cmd;
} LM_CMD_TYPE;

/*
 * LC event to LM
 */

typedef struct
{
    U8 offset;
    U8 opcode;
    U8 idx;
    U8 status;
    U8 reason;
}LC_EVT_ROLE_SWITCH_EVT_TYPE;


//========================================== HC CMD
typedef struct
{
    U8 connection_handle;
    U8 flag;                                //- bit 0 : stop(0), start(1); bit 1 : enhance_flush(1)
} HC_CMD_FLUSH_TYPE;

typedef struct
{
    U8 connection_handle;
} HC_CMD_ACL_RECYCLE_TYPE;

typedef struct
{
    U8  channel;
    U8  gc1;
    U8  gc2;
    U8  modulat_freq;
} HC_CMD_RFTESTTX1_TYPE;

typedef struct
{
    U8  channel;
    U8  tx_gc1;
    U8  packet_type;
    U8  packet_size_L;
    U8  packet_size_H;
    U8  data_type;
    U8  edr_enable;
    U8  hopping_enable;
    U8  tx_gc2;
    U8  tx_bias2_1;
    U8  auto_comp;
    U8  esco_enable;
    U8 tx_bias2_2;
} HC_CMD_RFTESTBTX_TYPE;

typedef struct
{
    U8  channel;
    U8  high_side;
    U8  Rx_attenuation;
    U8  report_rssi;
} HC_CMD_RFTESTCRX_TYPE;

typedef struct
{
    U8  channel;
    U8  hopping_enable;
    U8  high_side;
    U8  Rx_attenuation;
    U8  edr_enable;
    U8  report_ber;
    U8  esco_enable;
    U8  esco_payload_length_L;
    U8  esco_payload_length_H;
    U8  dv_enable;
} HC_CMD_RFTESTBRX_TYPE;

typedef struct
{
    U8  channel;
    U8  hopping_enable;
    U8  high_side;
    U8  Rx_attenuation;
    U8  edr_enable;
    U8  esco_enable;
    U8  esco_payload_length_L;
    U8  esco_payload_length_H;

} HC_CMD_RFTESTLOOPBACK_TYPE;

typedef struct
{
    U8  channel;
    U8  hopping_enable;
    U8  gc1;
    U8  gc2;
    U8  packet_type;
    U8  packet_size_L;
    U8  packet_size_H;
    U8  data_type;
    U8  edr_enable;
    U8  esco_enable;
} HC_CMD_RFTESTBERLOOPBACK_TYPE;

typedef struct
{
    U8 opcode;
    union
    {
        HC_CMD_FLUSH_TYPE               aclu_flush;
        HC_CMD_ACL_RECYCLE_TYPE         acl_recycle;
        HC_CMD_RFTESTTX1_TYPE           rftest_tx1;
        HC_CMD_RFTESTBTX_TYPE           rftest_btx;
        HC_CMD_RFTESTCRX_TYPE           rftest_crx;
        HC_CMD_RFTESTBRX_TYPE           rftest_brx;

        HC_CMD_RFTESTLOOPBACK_TYPE      rftest_loopback;
        HC_CMD_RFTESTBERLOOPBACK_TYPE   rftest_berloopback;
    } cmd;
} HC_CMD_TYPE;

//===============OSMQ Define===============

//========================================== mem addr pin
//#define LC_SET_MEM_DRV(A)                       LC_SFR_WRITE_BY_C2(C2OFFS_RW_MEM_DRV_SEL_MCU, A);
//#define LC_SET_MEM_DRV_30()                     LC_SFR_WRITE_BY_C2(C2OFFS_RW_MEM_DRV_SEL_MCU, 0x03);
//========================================== LC misc support feature

//- feature 0 bank
#define LC_MISC_SUPP_FEA_SUPPORT_SLEEP()                    (gLC_ctl.misc_ctl.lc_misc_supp_fea0 & LC_MISC_SUPP_FEA0_SUPPORT_SLEEP)
#define LC_MISC_SUPP_FEA_MCU_PAUSE()                        (gLC_ctl.misc_ctl.lc_misc_supp_fea0 & LC_MISC_SUPP_FEA0_MCU_PAUSE)
#define LC_MISC_SUPP_FEA_SUPPORT_STANDBY_SLEEP()            (gLC_ctl.misc_ctl.lc_misc_supp_fea0 & LC_MISC_SUPP_FEA0_SUPPORT_STANDBY_SLEEP)
#define LC_MISC_SUPP_FEA_MASTER_TRACKING()                  (gLC_ctl.misc_ctl.lc_misc_supp_fea0 & LC_MISC_SUPP_FEA0_MASTER_TRACKING)
#define LC_MISC_SUPP_FEA_SUPPORT_EIR()                      (gLC_ctl.misc_ctl.lc_misc_supp_fea0 & LC_MISC_SUPP_FEA0_SUPPORT_EIR)
#define LC_MISC_SUPP_FEA_CLKE_CONTROL_POWER_SAVING()        (gLC_ctl.misc_ctl.lc_misc_supp_fea0 & LC_MISC_SUPP_FEA0_CLKE_CONTROL_POWER_SAVING)

//- feature 1 bank
#define LC_MISC_SUPP_FEA_HID_SNIFF_WINDOW()                                     (gLC_ctl.misc_ctl.lc_misc_supp_fea1 & LC_MISC_SUPP_FEA1_HID_SNIFF_WINDOW)
#define LC_MISC_SUPP_FEA_PAGE_POWER_DECREASE()                                  (gLC_ctl.misc_ctl.lc_misc_supp_fea1 & LC_MISC_SUPP_FEA1_PAGE_POWER_DECREASE)
#define LC_MISC_SUPP_FEA_HID_CLK3_SPEEDUP_POWER_SAVING()                        (gLC_ctl.misc_ctl.lc_misc_supp_fea1 & LC_MISC_SUPP_FEA1_HID_CLK3_SPEEDUP_POWER_SAVING)
#define LC_MISC_SUPP_FEA_SLAVE_SNIFF_A1T0_WAIT_LMP_ACKED_EXTEND_TIMEOUT()       (gLC_ctl.misc_ctl.lc_misc_supp_fea1 & LC_MISC_SUPP_FEA1_SLAVE_SNIFF_A1T0_WAIT_LMP_ACKED_EXTEND_TIMEOUT)
#define LC_MISC_SUPP_FEA_SLAVE_SNIFF_ANT0_WAIT_LMP_ACKED_EXTEND_TIMEOUT()       (gLC_ctl.misc_ctl.lc_misc_supp_fea1 & LC_MISC_SUPP_FEA1_SLAVE_SNIFF_ANT0_WAIT_LMP_ACKED_EXTEND_TIMEOUT)
#define LC_MISC_SUPP_FEA_AVOID_CONSECUTIVE_INQUIRY_SCAN()                       (gLC_ctl.misc_ctl.lc_misc_supp_fea1 & LC_MISC_SUPP_FEA1_AVOID_CONSECUTIVE_INQUIRY_SCAN)
#define LC_MISC_SUPP_FEA_INTERNAL_32K_POWER_ON_K()                              (gLC_ctl.misc_ctl.lc_misc_supp_fea1 & LC_MISC_SUPP_FEA1_INTERNAL_32K_POWER_ON_K)
#define LC_MISC_SUPP_FEA_FORCE_RSSI_SCAN()                                      (gLC_ctl.misc_ctl.lc_misc_supp_fea1 & LC_MISC_SUPP_FEA1_FORCE_RSSI_SCAN)

//- feature 2 bank
#define LC_MISC_SUPP_FEA_60M_HOLD()                         (gLC_ctl.misc_ctl.lc_misc_supp_fea2 & LC_MISC_SUPP_FEA2_60M_HOLD)
#define LC_MISC_SUPP_FEA_32K_HOLD()                         (gLC_ctl.misc_ctl.lc_misc_supp_fea2 & LC_MISC_SUPP_FEA2_32K_HOLD)
#define LC_MISE_SUPP_FEA_RSSI_SCAN()                        (gLC_ctl.misc_ctl.lc_misc_supp_fea2 & LC_MISC_SUPP_FEA2_RSSI_SCAN)
#define LC_MISC_SUPP_FEA_CHECK_60M_LOCKED()                 (gLC_ctl.misc_ctl.lc_misc_supp_fea2 & LC_MISC_SUPP_FEA2_CHECK_60M_LOCKED)
#define LC_MISC_SUPP_FEA_MOD_GAIN_COMPESATION()             (gLC_ctl.misc_ctl.lc_misc_supp_fea2 & LC_MISC_SUPP_FEA2_MOD_GAIN_COMPESATION)
#define LC_MISC_SUPP_FEA_PROCESS_TEMP_SENSOR()              (gLC_ctl.misc_ctl.lc_misc_supp_fea2 & LC_MISC_SUPP_FEA2_PROCESS_TEMP_SENSOR)
#define LC_MISC_SUPP_FEA_ENABLE_TEMP_SENSOR()               (gLC_ctl.misc_ctl.lc_misc_supp_fea2 & LC_MISC_SUPP_FEA2_ENABLE_TEMP_SENSOR)
#define LC_MISC_SUPP_FEA_SCO_ESCO_POWER_SAVING()            (gLC_ctl.misc_ctl.lc_misc_supp_fea2 & LC_MISC_SUPP_FEA2_SCO_ESCO_POWER_SAVING)



//- feature 3 bank
#define LC_MISC_SUPP_FEA_FIXED_PAGE_INQ_SEGMENTATION_TIME() (gLC_ctl.misc_ctl.lc_misc_supp_fea3 & LC_MISC_SUPP_FEA3_FIXED_PAGE_INQ_SEGMENTATION_TIME)
#define LC_MISC_SUPP_FEA_NORMAL_SEARCH_LM_CMD()             (gLC_ctl.misc_ctl.lc_misc_supp_fea3 & LC_MISC_SUPP_FEA3_NORMAL_SEARCH_LM_CMD)
#define LC_MISC_SUPP_FEA_NORMAL_SEARCH_EXIT_HOLD()          (gLC_ctl.misc_ctl.lc_misc_supp_fea3 & LC_MISC_SUPP_FEA3_NORMAL_SEARCH_EXIT_HOLD)
#define LC_MISC_SUPP_FEA_DUMMY_SNIFF_ENABLE()               (gLC_ctl.misc_ctl.lc_misc_supp_fea3 & LC_MISC_SUPP_FEA3_DUMMY_SNIFF_ENABLE)
#define LC_MISC_SUPP_FEA_BARGE_IN_ESCO_WESCO()              (gLC_ctl.misc_ctl.lc_misc_supp_fea3 & LC_MISC_SUPP_FEA3_BARGE_IN_ESCO_WESCO)
#define LC_MISC_SUPP_FEA_ONE_LINK_RX_FULL()                 (gLC_ctl.misc_ctl.lc_misc_supp_fea3 & LC_MISC_SUPP_FEA3_ONE_LINK_RX_FULL)

//- feature 4 bank
#define LC_MISC_SUPP_FEA_LC_SELF_UNSNIFF_PROC()             (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_LC_SELF_UNSNIFF_PROC)
//Remove
//#define LC_MISC_SUPP_FEA_INQUIRY_PAGE_SEGMENT()             (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_INQUIRY_PAGE_SEGMENT)
#define LC_MISC_SUPP_FEA_PKT_BLOCKED_CONTROL()              (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_PKT_BLOCKED_CONTROL)
#define LC_MISC_SUPP_FEA_SUPPROT_EIR_INQUIRY()              (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_SUPPORT_EIR_INQUIRY)
#define LC_MISC_SUPP_FEA_SUPPORT_FLOW_STOP_IGNORANCE()      (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_SUPPORT_FLOW_STOP_IGNORANCE)
#define LC_MISC_SUPP_FEA_A2DP_POWER_SAVING_MECHANISM()      (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_A2DP_POWER_SAVING_MECHANISM)
#define LC_MISC_SUPP_FEA_AFH_POWER_SAVING_DELAY()           (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_AFH_POWER_SAVING_DELAY)
#define LC_MISC_SUPP_FEA_RANDOM_SCAN_WINDOW()               (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_RANDOM_SCAN_WINDOW)
#define LC_MISC_SUPP_FEA_SLOTTICK_DELAY_IN_SNIFF_MODE()     (gLC_ctl.misc_ctl.lc_misc_supp_fea4 & LC_MISC_SUPP_FEA4_SLOTTICK_DELAY_IN_SNIFF_MODE)


//========================================== Genernal
typedef U8 LC_INTR_GROUP;
#define LC_30_HW_INTR           ((LC_INTR_GROUP)BIT_MASK8(0))
#define LC_30_SW_INTR           ((LC_INTR_GROUP)BIT_MASK8(1))
#define LC_40_DEVICE0_INTR      ((LC_INTR_GROUP)BIT_MASK8(2))
#define LC_40_DEVICE1_INTR      ((LC_INTR_GROUP)BIT_MASK8(3))

#define LC_ROLE_MASTER                              (1)
#define LC_ROLE_SLAVE                               (0)


#define ACCESS_CODE_SEARCH_WINDOW_FIXED         0
#define ACCESS_CODE_SEARCH_WINDOW_INFINITE      1
#define HV3_TX_RX                               0
#define EV3_TX_RX                               1
#define TRANSMIT_NORMAL_PACKET                  0
#define TRANSMIT_ID_PACKET                      1
#define EDR_1MBPS                               0
#define EDR_2_3MBPS                             1
#define WHITENING_SEL_CLK61                     0
#define WHITENING_SEL_XIR40                     1
#define ENABLE_WHITENING                        0
#define DISABLE_WHITENING                       1
#define WHITENING_IN_CONNECTION_STATE           0
#define WHITENING_IN_OTHER_STATE                1

#define  LC_WRITE_U32_TO_SFR_FE(p, fe_sfr)      (SFR_RW_FE_OFFSET = fe_sfr, SFR_RW_FE = p[3],\
                                                 SFR_RW_FE = p[2], SFR_RW_FE = p[1], SFR_RW_FE = p[0])

#define  LC_WRITE_U24_TO_SFR_FE(p, fe_sfr)      (SFR_RW_FE_OFFSET = fe_sfr, SFR_RW_FE = p[2],\
                                                 SFR_RW_FE = p[1], SFR_RW_FE = p[0])



#define LC_MASTER_LINK_E7_ENABLE                (0x01)
#define LC_MASTER_LINK_E7_DISABLE               (0x00)
#define LC_SLAVE_LINK_E7_ENABLE                 (0x80)
#define LC_SLAVE_LINK_E7_DISABLE                (0x00)

#define LC_SET_EDR(A)                           SFRB_RW_EDR_SEL = A

#define LC_SWC_SET_TX_GFSK_INQUIRY_GAIN()       {LC_SFR_WRITE_BY_FE(FEOFFS_WO_SWC_TX_GC1, gLC_ctl.rf_ctl.inquiry_txgain_gc1);                          \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.inquiry_txgain_gc2);                           \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1);                      \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2);                      \
                                                }

#define LC_SWC_SET_TX_GFSK_DEFAULT_GAIN()       {LC_SFR_WRITE_BY_FE(FEOFFS_WO_SWC_TX_GC1, gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc1);         \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc2);                          \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1);                      \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2);                      \
                                                }

#define LC_SWC_SET_TX_GFSK_GAIN_FROM_SAC(A)     {LC_SFR_WRITE_BY_FE(FEOFFS_WO_SWC_TX_GC1, gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[A].rf_gfsk_txgc1);      \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[A].rf_gfsk_txgc2);                       \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[A].rf_gfsk_bias2_1);                     \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[A].rf_gfsk_bias2_2);                     \
                                                }
#define LC_SWC_SET_TX_EDR_GAIN_FROM_SAC(A)      {LC_SFR_WRITE_BY_FE(FEOFFS_WO_SWC_TX_GC1, gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[A].rf_edr_txgc1);       \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[A].rf_edr_txgc2);                        \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[A].rf_edr_bias2_1);                      \
                                                 LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[A].rf_edr_bias2_2);                      \
                                                }

VOID LC_SetSacTxGain(U8 idx);

#define LC_FHS_TR_BUFFER_PTR                    gLC_ctl.lc_fhs_tr_share_buffer.fhs_tr_next_ptr
#define LC_FHS_TR_BUFFER_LENGTH_B0              gLC_ctl.lc_fhs_tr_share_buffer.fhs_tr_len_B0
#define LC_FHS_TR_BUFFER_LENGTH_B1              gLC_ctl.lc_fhs_tr_share_buffer.fhs_tr_len_B1
#define LC_FHS_TR_BUFFER                        gLC_ctl.lc_fhs_tr_share_buffer.fhs_tr_share_buffer
#define LC_FHS_R_BUFFER_PTR_B0                  PTR16_LSB(&gLC_ctl.lc_fhs_tr_share_buffer.fhs_tr_share_buffer)
#define LC_FHS_R_BUFFER_PTR_B1                  PTR16_MSB(&gLC_ctl.lc_fhs_tr_share_buffer.fhs_tr_share_buffer)
#define LC_FHS_T_BUFFER_PTR_B0                  PTR16_LSB(&gLC_ctl.lc_fhs_tr_share_buffer.fhs_tr_next_ptr)
#define LC_FHS_T_BUFFER_PTR_B1                  PTR16_MSB(&gLC_ctl.lc_fhs_tr_share_buffer.fhs_tr_next_ptr)

VOID LC_FHSBuf_WriteParity(U8 XDATA_PTR parityPtr);
VOID LC_FHSBuf_ReadParity(PARITY_ARR XDATA pParity);

VOID LC_FHSBuf_WriteBDAddr(U8 XDATA_PTR addrPtr);
VOID LC_FHSBuf_ReadBDAddr(BD_ADDR_STRU_PTR pBdAddr);

VOID LC_FHSBuf_WriteUndefine(U8 value);
U8 LC_FHSBuf_ReadUndefine(VOID);

VOID LC_FHSBuf_WriteSR(U8 value);
U8 LC_FHSBuf_ReadSR(VOID);

VOID LC_FHSBuf_WriteReserved(U8 value);
U8 LC_FHSBuf_ReadReserved(VOID);

VOID LC_FHSBuf_WriteCOD(U8 XDATA_PTR codPtr);
VOID LC_FHSBuf_ReadCOD(U8 XDATA_PTR codPtr);

VOID LC_FHSBuf_WriteLTAddr(U8 ltaddr);
U8 LC_FHSBuf_ReadLTAddr(VOID);

VOID LC_FHSBuf_WriteClk27_2(U32 clk);
U32 LC_FHSBuf_ReadClk(VOID);

VOID LC_FHSBuf_WritePageScanMode(U8 mode);
U8 LC_FHSBuf_ReadScanMode(VOID);



#define LC_READ_FHS_BUF_LAP_BYT0()              ((LC_FHS_TR_BUFFER[4] >> 2) | (LC_FHS_TR_BUFFER[5] << 6 & 0xC0))
#define LC_READ_FHS_BUF_LAP_BYT1()              ((LC_FHS_TR_BUFFER[5] >> 2) | (LC_FHS_TR_BUFFER[6] << 6 & 0xC0))
#define LC_READ_FHS_BUF_LAP_BYT2()              ((LC_FHS_TR_BUFFER[6] >> 2) | (LC_FHS_TR_BUFFER[7] << 6 & 0xC0))


#define LC_FHS_SR0                              0
#define LC_FHS_SR1                              1
#define LC_FHS_SR2                              2

//========================================== Packet header
#define SEQN_INIT                               1
#define ARQN_NAK                                0
#define ARQN_ACK                                1
#define FLOW_STOP                               0
#define FLOW_GO                                 1

#define LC_SET_TX_FLOW_GO()                     LC_SFR_WRITE(SFRB_RW_TX_FLOW, FLOW_GO)
#define LC_SET_TX_FLOW_STOP()                   LC_SFR_WRITE(SFRB_RW_TX_FLOW, FLOW_STOP)

#define LC_SET_TX_ARQN_NAK()                    LC_SFR_WRITE(SFRB_RW_TX_ARQN, ARQN_NAK)
#define LC_SET_TX_ARQN_ACK()                    LC_SFR_WRITE(SFRB_RW_TX_ARQN, ARQN_ACK)
#define LC_SET_TX_SEQN_0()                      LC_SFR_WRITE(SFRB_RW_TX_SEQN, 0)
#define LC_SET_TX_SEQN_1()                      LC_SFR_WRITE(SFRB_RW_TX_SEQN, 1)
#define LC_TOGGLE_TX_SEQN()                     LC_SFR_WRITE(SFRB_RW_TX_SEQN, ~SFRB_RW_TX_SEQN)
#define LC_GET_CURRENT_TX_SEQN()                LC_SFR_READ(SFRB_RW_TX_SEQN)
#define LC_SET_CURRENT_TX_SEQN(A)               LC_SFR_WRITE(SFRB_RW_TX_SEQN, A)
#define LC_GET_CURRENT_TX_ARQN()                LC_SFR_READ(SFRB_RW_TX_ARQN)
#define LC_SET_CURRENT_TX_ARQN(A)               LC_SFR_WRITE(SFRB_RW_TX_ARQN, A)
#define LC_GET_CURRENT_TX_FLOW()                LC_SFR_READ(SFRB_RW_TX_FLOW)
#define LC_SET_CURRENT_TX_FLOW(A)
#define LC_SAC_SET_TX_FLOW(A)                   LC_SFR_WRITE_BY_E7(E7OFFS_RW_TX_ARQ_FLOW_CTRL, A);
#define LC_SAC_SET_FLOW_STOP_IGNORED()          LC_SFR_WRITE_BY_E7(E7OFFS_RW_TX_ARQ_FLOW_CTRL, 7);
#define LC_SAC_SET_FLOW_STOP_CONSIDERED()       LC_SFR_WRITE_BY_E7(E7OFFS_RW_TX_ARQ_FLOW_CTRL, 1);

#define LC_SAC_TX_ACL_NAK                       0x00
#define LC_SAC_TX_ACL_ACK                       0x01
#define LC_SAC_TX_ACL_ACK_MASK                  0x01
#define LC_SAC_TX_ACL_SEQN_0                    0x00
#define LC_SAC_TX_ACL_SEQN_1                    0x02
#define LC_SAC_TX_ACL_SEQN_MASK                 0x02
#define LC_SAC_TX_ESCO_ACK                      0x00
#define LC_SAC_TX_ESCO_NAK                      0x04
#define LC_SAC_RX_ACL_SEQN_0                    0x00
#define LC_SAC_RX_ACL_SEQN_1                    0x20
#define LC_SAC_RX_ACL_SEQN_MASK                 0x20
#define LC_SAC_SET_TX_RX_ARQN_SEQN(A, B)        {LC_SFR_WRITE_BY_E7(E7OFFS_RW_ARQ_SEQ_SEL, A);     \
                                                 _nop_();                                          \
                                                 LC_SFR_WRITE_BY_E7(E7OFFS_RW_ARQ_SEQ_BITMAP, B); \
                                                 _nop_();                                          \
                                                }


//========================================== Payload header
#define LLID_UNDEFININED                            0
#define LLID_CONTI_L2CAP_MSG                        0x40
#define LLID_START_L2CAP_MSG                        0x80
#define LLID_START_LMP_MSG                          0xC0
//#define LC_RX_PACKET_INFO_FLOW_MASK                   0x01
//#define LC_RX_PACKET_INFO_ARQN_MASK                   0x02
//#define LC_RX_PACKET_INFO_SEQN_MASK                   0x04
//#define LC_RX_PACKET_INFO_HEC_CORRECT_MASK            0x08
//#define LC_RX_PACKET_INFO_CRC_CORRECT_MASK            0x10
//#define LC_RX_PACKET_INFO_PAYLOAD_HEADER_FLOW_MASK    0x20
//#define LC_RX_PACKET_INFO_PAYLOAD_HEADER_LLID_MASK    0xC0

#define LC_RX_PACKET_INFO_LTADDR_MASK               0x07   //New Rx Status
#define LC_RX_PACKET_INFO_TYPE_MASK                 0x78
#define LC_RX_PACKET_INFO_FLOW_MASK                 0x80
#define LC_RX_PACKET_INFO_ARQN_MASK                 0x01
#define LC_RX_PACKET_INFO_SEQN_MASK                 0x02
#define LC_RX_PACKET_INFO_HEC_CORRECT_MASK          0x80
#define LC_RX_PACKET_INFO_CRC_CORRECT_MASK          0x80
#define LC_RX_PACKET_INFO_TIME_STAMP_MASK           0x78


#define LC_RX_PACKET_INFO_PAYLOAD_HEADER_FLOW_MASK  0x10
#define LC_RX_PACKET_INFO_PAYLOAD_HEADER_LLID_MASK  0x0C


#if 0
#ifdef AB1520S
#define LC_SFR_RX_PACKET_INFO_PAYLOAD_HEADER_FLOW_MASK  0x04
#define LC_SFR_RX_PACKET_INFO_PAYLOAD_HEADER_LLID_MASK  0x03
#endif
#endif


//========================================== hopping control
#define HOPPING_PAGE_SCAN                           0
#define HOPPING_INTERLACED_PAGE_SCAN                1
#define HOPPING_INQUIRY_SCAN                        2
#define HOPPING_INTERLACED_INQUIRY_SCAN             3
#define HOPPING_PAGE                                4
#define HOPPING_INQUIRY                             5
#define HOPPING_MASTER_PAGE_RESPONSE                6
#define HOPPING_SLAVE_PAGE_RESPONSE                 7
#define HOPPING_INQUIRY_RESPONSE                    8
#define HOPPING_BASIC_CHANNEL                       9
#define HOPPING_ADAPTED_CHANNEL                     10
#define HOPPING_INTERLACED_MASTER_PAGE_RESPONSE     11
#define HOPPING_INTERLACED_SLAVE_PAGE_RESPONSE      12
#define HOPPING_INTERLACED_INQUIRY_RESPONSE         13
#define A_TRAIN                                     1
#define B_TRAIN                                     0
#define LC_SET_PAGE_A_TRAIN()                       LC_SFR_WRITE(SFRB_RW_KOFFSET, A_TRAIN)
#define LC_SET_PAGE_B_TRAIN()                       LC_SFR_WRITE(SFRB_RW_KOFFSET, B_TRAIN)
#define LC_GET_PAGE_AB_TRAIN()                      LC_SFR_READ(SFRB_RW_KOFFSET)
#define LC_SET_PAGE_AB_TRAIN(A)                     LC_SFR_WRITE(SFRB_RW_KOFFSET, A)
#define LC_SET_INQUIRY_A_TRAIN()                    LC_SFR_WRITE(SFRB_RW_KOFFSET, A_TRAIN)
#define LC_SET_INQUIRY_B_TRAIN()                    LC_SFR_WRITE(SFRB_RW_KOFFSET, B_TRAIN)
#define LC_GET_INQUIRY_AB_TRAIN()                   LC_SFR_READ(SFRB_RW_KOFFSET)
#define LC_SET_INQUIRY_AB_TRAIN(A)                  LC_SFR_WRITE(SFRB_RW_KOFFSET, A)

//- #define BT_HOPPING_DURATION_MUL_2                   300     // 2 x Hopping duration (us)
//- #define LC_MANUAL_RX_WINDOW_LENGTH                  (100)

//========================================== interrupt control


//- BT 4.0 Clock and Event mask
typedef U8 LC_40_CLK_EVENT_MASK;
#define LC_LE_ENABLE_DEVICE_CLK                             ((LC_40_CLK_EVENT_MASK)BIT_MASK8(0))
#define LC_LE_ENABLE_DEVICE_CONN_EVENT                      ((LC_40_CLK_EVENT_MASK)BIT_MASK8(1))
#define LC_LE_ENABLE_DEVICE_CURRENT_CONN_EVENT              ((LC_40_CLK_EVENT_MASK)BIT_MASK8(3))
#define LC_LE_ENABLE_DEVICE_SUPERVISION_TO_EVENT            ((LC_40_CLK_EVENT_MASK)BIT_MASK8(4))
#define LC_LE_ENABLE_DEVICE_CONN_EVENT_COUNT_MATCH_EVENT    ((LC_40_CLK_EVENT_MASK)BIT_MASK8(5))

/**
 * LC_LE_SET_EVENT_CTL
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 *
 * @m : Clock and Event mask
 */
#define LC_LE_SET_EVENT_CTL(m) { do {                                           \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE_CLK_EVENT_CTRL,(m));                    \
    } while (0); }

/**
 * LC_LE_SET_SLOT_TICK_PERIOD
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 *
 * @m : Multiple of 625us
 */
#define LC_LE_SET_SLOT_TICK_PERIOD(m) { do {                                    \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_SLOT_TICK_EVENT_CTRL,(m));                     \
    } while (0); }


/**
 *
 * LC_LE_SET_MATCH_EVT_COUNT
 *
 * @Author : Huichuan Lin <HuichuanLin@airoha.com.tw>
 *
 * @m : match event count
 */

#define LC_LE_SET_MATCH_EVT_CNT(m) { do {                                           \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_CONN_MATCH_EVT_CNT_B0,(U8)(m));                    \
    LC_SFR_CONT_WRITE_BY_E6((U8)(m >> 8));                                          \
    } while (0); }

//-
typedef U8 LC_SWC_INTR_MASK;
#define LC_SWC_INTR_MASK_SLOT_TICK              ((LC_SWC_INTR_MASK)BIT_MASK8(0))
#define LC_SWC_INTR_MASK_RX_DONE                ((LC_SWC_INTR_MASK)BIT_MASK8(1))
#define LC_SWC_INTR_MASK_AC_SYNC                ((LC_SWC_INTR_MASK)BIT_MASK8(2))
#define LC_SWC_INTR_MASK_TX_DONE                ((LC_SWC_INTR_MASK)BIT_MASK8(3))
#define LC_SWC_INTR_MASK_HEADER_READY           ((LC_SWC_INTR_MASK)BIT_MASK8(4))
#define LC_SWC_INTR_MASK_AC_LOSS                ((LC_SWC_INTR_MASK)BIT_MASK8(5))

/**
 * LC_SWC_ENABLE_INTR
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 *
 * @m : The interrupt mask
 */
#define LC_SWC_ENABLE_INTR(m) { do { LC_SFR_WRITE(SFR_RW_SWC_INTERRUPT_ENABLE,(m)); } while (0); }




//============== BT 3.0 interrupt control
#define LC_SWC_INTERRUPT_VECTOR_SLOT_TICK               0
#define LC_SWC_INTERRUPT_VECTOR_RX_DONE                 1
#define LC_SWC_INTERRUPT_VECTOR_AC_SYNC                 2
#define LC_SWC_INTERRUPT_VECTOR_TX_DONE                 3
#define LC_SWC_INTERRUPT_VECTOR_HEADER_READY            4
#define LC_SWC_INTERRUPT_VECTOR_AC_LOSS                 5

#define LC_SAC_INTERRUPT_ENABLE_SLOT_TICK               0x0001
#define LC_SAC_INTERRUPT_ENABLE_RX_DONE                 0x0002
#define LC_SAC_INTERRUPT_ENABLE_ACKED_ACL               0x0004
#define LC_SAC_INTERRUPT_ENABLE_SCO                     0x0008
#define LC_SAC_INTERRUPT_ENABLE_ESCO                    0x0010
#define LC_SAC_INTERRUPT_ENABLE_SNIFF                   0x0020
#define LC_SAC_INTERRUPT_ENABLE_AFH                     0x0040
#define LC_SAC_INTERRUPT_ENABLE_ROLE_SWITCH             0x0080
#define LC_SAC_INTERRUPT_ENABLE_HOLD                    0x0100
#define LC_SAC_INTERRUPT_ENABLE_SERVICE                 0x0200
#define LC_SAC_INTERRUPT_ENABLE_LOSS_CONNECTION         0x0400
#define LC_SAC_INTERRUPT_ENABLE_AFH_MEASURMENT          0x0800
#define LC_SAC_INTERRUPT_ENABLE_PMU                     0x1000
#define LC_SAC_INTERRUPT_ENABLE_SCO_ESCO_TRX_DONE       0x2000
#define LC_SAC_INTERRUPT_ENABLE_LARGE_RX_FIFO_EMPTY     0x4000
#define LC_SAC_INTERRUPT_ENABLE_ALL_LINK_SETTINGS       (LC_SAC_INTERRUPT_ENABLE_LARGE_RX_FIFO_EMPTY | LC_SAC_INTERRUPT_ENABLE_AFH_MEASURMENT | LC_SAC_INTERRUPT_ENABLE_SERVICE)

#define LC_SAC_INTERRUPT_ENABLE_SNIFF_WHEN_FROZEN       0x01
#define LC_SAC_INTERRUPT_ENABLE_RECEIVED_NULL_PACKET    0x02
#define LC_SAC_INTERRUPT_ENABLE_ESCO_WHEN_FROZEN        0x04

#define LC_SAC_DISABLE_INTERRUPT()                      LC_SFR_WRITE_BY_E7(E7OFFS_RW_SAC_INTERRUPT_LINK_SEL, 0x00)
#define LC_SAC_SET_INTERRUPT_ENABLE(A, B)               {LC_SFR_WRITE_BY_E7(E7OFFS_RW_SAC_INTERRUPT_LINK_SEL, A);  \
                                                        LC_SFR_CONT_WRITE_BY_E7(U16_LSB(B)); LC_SFR_CONT_WRITE_BY_E7(U16_MSB(B))}

#define LC_SAC_GET_INTERRUPT_ENABLE(A, B)               {LC_SFR_WRITE_BY_E7(E7OFFS_RW_SAC_INTERRUPT_LINK_SEL, A);  \
                                                        B = LC_SFR_CONT_READ_BY_E7 (); B |= ((U16)LC_SFR_CONT_READ_BY_E7 () << 8);}

#ifdef AB1520B
#define LC_SAC_GET_INTERRUPT3_ENABLE(A, B)               {LC_SFR_WRITE_BY_E7(E7OFFS_RW_SAC_INTERRUPT_LINK_SEL, A);  \
                                                        B = LC_SFR_READ_BY_E7 (E7OFFS_RW_SAC_INTERRUPT_ENABLE3);}
#endif


#define LC_SAC_INTERRUPT_VECTOR0_MASTER_SLOT_TICK       0
#define LC_SAC_INTERRUPT_VECTOR0_SLAVE0_SLOT_TICK       1
#define LC_SAC_INTERRUPT_VECTOR0_SLAVE1_SLOT_TICK       2
#define LC_SAC_INTERRUPT_VECTOR0_RX_DONE                3
#define LC_SAC_INTERRUPT_VECTOR0_ACKED_TX               4
#define LC_SAC_INTERRUPT_VECTOR0_SCO                    5
#define LC_SAC_INTERRUPT_VECTOR0_ESCO                   6
#define LC_SAC_INTERRUPT_VECTOR0_SNIFF                  7
#define LC_SAC_INTERRUPT_VECTOR0_HOLD                   8
#define LC_SAC_INTERRUPT_VECTOR0_SERVICE                9
#define LC_SAC_INTERRUPT_VECTOR0_AFH_INSTANT            10
#define LC_SAC_INTERRUPT_VECTOR0_ROLE_SWITCH_INSTANT    11
#define LC_SAC_INTERRUPT_VECTOR0_SCAN                   12
#define LC_SAC_INTERRUPT_VECTOR0_LOSS_CONNECTION        13
#define LC_SAC_INTERRUPT_VECTOR0_SCO_ESCO_TX_DONE       14
#define LC_SAC_INTERRUPT_VECTOR0_SCO_ESCO_RX_DONE       15
#define LC_SAC_INTERRUPT_VECTOR0_RESERVE16              16
#define LC_SAC_INTERRUPT_VECTOR0_RESERVE17              17
#define LC_SAC_INTERRUPT_VECTOR0_RSSI_MEAS_DONE         18
#define LC_SAC_INTERRUPT_VECTOR0_MMI                    19
#define LC_SAC_INTERRUPT_VECTOR0_PMUGPIO                20

#define LC_SAC_INTERRUPT_VECTOR1_MASTER_BROADCAST       0
#define LC_SAC_INTERRUPT_VECTOR1_SLAVE0                 8
#define LC_SAC_INTERRUPT_VECTOR1_SLAVE1                 9

#define LC_PMUGPIO_INTERRUPT_VECTOR1_GPIO0              0
#define LC_PMUGPIO_INTERRUPT_VECTOR1_GPIO1              1
#define LC_PMUGPIO_INTERRUPT_VECTOR1_GPIO2              2
#define LC_PMUGPIO_INTERRUPT_VECTOR1_GPIO3              3
#define LC_PMUGPIO_INTERRUPT_VECTOR1_LOW_BATTERY        4
#define LC_PMUGPIO_INTERRUPT_VECTOR1_FULL_BATTERY       5
#define LC_PMUGPIO_INTERRUPT_VECTOR1_CHARGE_COMPLETE    6
#define LC_PMUGPIO_INTERRUPT_VECTOR1_RECHARGE           7
#define LC_PMUGPIO_INTERRUPT_VECTOR1_CHARGE_OFF         8
#define LC_PMUGPIO_INTERRUPT_VECTOR1_CHARGE_ON          9



#ifdef SUPPORT_2_SLAVE
    #define LC_SAC_INTERRUPT_VECTOR1_MASTER_VALID(A)    (A < (MAX_NUM_ACL_LINK - 1))
#else
    #define LC_SAC_INTERRUPT_VECTOR1_MASTER_VALID(A)    (A < MAX_NUM_ACL_LINK)
#endif

#define LC_SAC_INTERRUPT_VECTOR1_SLAVE0_VALID(A)        (A == LC_SAC_INTERRUPT_VECTOR1_SLAVE0)
#define LC_SAC_INTERRUPT_VECTOR1_SLAVE1_VALID(A)        (A == LC_SAC_INTERRUPT_VECTOR1_SLAVE1)


#define LC_SAC_INTERRUPT_VECTOR2_IN                     0
#define LC_SAC_INTERRUPT_VECTOR2_OUT                    1
#define LC_SAC_INTERRUPT_VECTOR2_FAIL                   0
#define LC_SAC_INTERRUPT_VECTOR2_SUCCESS                1

//------For Error Report------
#define LC_SAC_INTERRUPT_VECTOR2_ACLOSS                 0x02
#define LC_SAC_INTERRUPT_VECTOR2_HEC_ERROR              0x04
#define LC_SAC_INTERRUPT_VECTOR2_CRC_ERROR              0x08
//------For Error Report------

//========================================== clk control
#define LC_CLK12_MASK                   (0x01 << 4)
#define LC_CLK13_MASK                   (0x01 << 5)
#define LC_CLKE_1_28_SEC                ((U16)0x0800)

#define CLKSEL_CLK0                     0
#define CLKSEL_CLK1                     1
#define CLKSEL_CLK2                     2
#define CLKSEL_CLK3                     3
#define CLKSEL_CLK_BLE_LINK0            4
#define CLKSEL_CLK_BLE_LINK1            5

#define CLK_INTERVAL_312_5              0
#define CLK_INTERVAL_625                1
#define CLK_INTERVAL_1250               2

#define LC_MASTER_SELECT_CLK0()                 LC_SFR_WRITE_BY_FE(FEOFFS_WO_CLKSEL_MASTER, CLKSEL_CLK0)
#define LC_MASTER_SELECT_CLK1()                 LC_SFR_WRITE_BY_FE(FEOFFS_WO_CLKSEL_MASTER, CLKSEL_CLK1)
#define LC_MASTER_SELECT_CLK2()                 LC_SFR_WRITE_BY_FE(FEOFFS_WO_CLKSEL_MASTER, CLKSEL_CLK2)

#define LC_SELECT_CLK(A)                        LC_SFR_WRITE(SFR_RW_CLKSEL, A)
#define LC_SELECT_CLK0()                        LC_SFR_WRITE(SFR_RW_CLKSEL, CLKSEL_CLK0)
#define LC_SELECT_CLK1()                        LC_SFR_WRITE(SFR_RW_CLKSEL, CLKSEL_CLK1)
#define LC_SELECT_CLK2()                        LC_SFR_WRITE(SFR_RW_CLKSEL, CLKSEL_CLK2)
#define LC_SELECT_CLK3()                        LC_SFR_WRITE(SFR_RW_CLKSEL, CLKSEL_CLK3)

#define LC_SET_CLK0_INTERVAL(A)                 LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLK0_INTERVAL, A)
#define LC_SET_CLK1_INTERVAL(A)                 LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLK1_INTERVAL, A)
#define LC_SET_CLK2_INTERVAL(A)                 LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLK2_INTERVAL, A)
#define LC_SET_CLK3_INTERVAL(A)                 LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLK3_INTERVAL, A)

#define MASTER_CLK_TRACKING_ENABLE              0x01
#define MASTER_CLK_TRACKING_DISABLE             0x00
#define MASTER_CLK_TRACKING_P_STEP_1            0x00
#define MASTER_CLK_TRACKING_P_STEP_2            0x10
#define MASTER_CLK_TRACKING_N_STEP_1            0x20
#define MASTER_CLK_TRACKING_N_STEP_2            0x30
#define LC_SET_MASTER_TRACKING(A, B)            LC_SFR_WRITE_BY_C2(C2OFFS_RW_MASTER_CLK_TRACKING_ENABLE, (A | B))

#define LC_MASTER_CLK_TRACKING_TO_NONE          0x00
#define LC_MASTER_CLK_TRACKING_TO_SLAVE0        0x01
#define LC_MASTER_CLK_TRACKING_TO_SLAVE1        0x02
#define LC_MASTER_CLK_TRACKING_MASK             0x03

#define LC_GET_CLK_OFFSET_M_S0(A)               {A = LC_SFR_READ_BY_C2 (C2OFFS_RW_MASTER_SLAVE0_CLK_OFFSET_B0);         \
                                                    A |= ((U16)LC_SFR_CONT_READ_BY_C2() << 8);}
#define LC_GET_CLK_OFFSET_M_S1(A)               {A = LC_SFR_READ_BY_C2 (C2OFFS_RW_MASTER_SLAVE1_CLK_OFFSET_B0);         \
                                                    A |= ((U16)LC_SFR_CONT_READ_BY_C2() << 8);}

enum
{
    UPDATE_OFFSET_CLK0 = 1,
    UPDATE_OFFSET_CLK1 = (1 << 1),
    UPDATE_OFFSET_CLK2 = (1 << 2),
    UPDATE_OFFSET_CLK3 = (1 << 3),
    UPDATE_OFFSET_CLK0_RT1_TT1 = 0,
    UPDATE_OFFSET_CLK1_RT1_TT1 = 0,
    UPDATE_OFFSET_CLK2_RT1_TT1 = 0,
    UPDATE_OFFSET_CLK3_RT1_TT1 = 0
};

VOID LC_EnableCLK(U8 clk_bitmask);
VOID LC_DisableCLK(U8 clk_bitmask);

#define LC_ENABLE_CLK0()                        LC_EnableCLK(U8_BIT_MASK(CLKSEL_CLK0))
#define LC_ENABLE_CLK1()                        LC_EnableCLK(U8_BIT_MASK(CLKSEL_CLK1))
#define LC_ENABLE_CLK2()                        LC_EnableCLK(U8_BIT_MASK(CLKSEL_CLK2))
#define LC_ENABLE_CLK3()                        LC_EnableCLK(U8_BIT_MASK(CLKSEL_CLK3))
#define LC_DISABLE_CLK0()                       LC_DisableCLK(U8_BIT_MASK(CLKSEL_CLK0))
#define LC_DISABLE_CLK1()                       LC_DisableCLK(U8_BIT_MASK(CLKSEL_CLK1))
#define LC_DISABLE_CLK2()                       LC_DisableCLK(U8_BIT_MASK(CLKSEL_CLK2))
#define LC_DISABLE_CLK3()                       LC_DisableCLK(U8_BIT_MASK(CLKSEL_CLK3))
#define LC_ENABLE_ALL_CLKE()                    LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLKE_CONTROL, (U8_BIT_MASK(CLKSEL_CLK0) | U8_BIT_MASK(CLKSEL_CLK1) | U8_BIT_MASK(CLKSEL_CLK2) | U8_BIT_MASK(CLKSEL_CLK3)))
#define LC_DISABLE_ALL_CLKE()                   LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLKE_CONTROL, 0x00)

#define LC_60M_CLK_CONTROL                          (0x01)
#define LC_32K_CLK_CONTROL                          (0x02)
//#define LC_GET_60M_CLK_LOCKED()                   (LC_SFR_READ_BY_C2 (C2OFFS_RW_60M_32K_CLK_CONTROL) & LC_60M_CLK_CONTROL)
//#define LC_GET_32K_CLK_LOCKED()                   (LC_SFR_READ_BY_C2 (C2OFFS_RW_60M_32K_CLK_CONTROL) & LC_32K_CLK_CONTROL)
//#define LC_SET_60M_32K_CLK_CONTROL(A)             LC_SFR_WRITE_BY_C2(C2OFFS_RW_60M_32K_CLK_CONTROL, (A))

//========================================== clock gating
#define LC_SFR_FROZEN_TICK_ENABLE                       (0x20)
#define LC_SFR_FROZEN_TICK_DISABLE                      (~0x20)

#define LC_MCU_WAKEN_BY_TICK   (LC_SFR_READ_BY_C2(C2OFFS_RW_CLK_GATING_CONTROL) & LC_SFR_FROZEN_TICK_ENABLE)

#define LC_SET_CLOCK_GATING_CTRL(A)                     LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLK_GATING_CONTROL, (A))
#define LC_ENABLE_MCU_FROZEN_WAKEUP_BY_HW_SLOTTCIK(A)   LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLK_GATING_CONTROL, (A | LC_SFR_FROZEN_TICK_ENABLE))
#define LC_DISABLE_MCU_FROZEN_WAKEUP_BY_HW_SLOTTCIK(A)  LC_SFR_WRITE_BY_C2(C2OFFS_RW_CLK_GATING_CONTROL, (A & LC_SFR_FROZEN_TICK_DISABLE))

//========================================== mcu clk rate
#define LC_SET_MCU_CLK_12M ()                   LC_SFR_WRITE_BY_C2(C2OFFS_RW_MCU_CLOCK_RATE_SEL, 0x00)
#define LC_SET_MCU_CLK_24M ()                   LC_SFR_WRITE_BY_C2(C2OFFS_RW_MCU_CLOCK_RATE_SEL, 0x01)

//========================================== Scatternet control
#define LC_SAC_INVALID_HANDLE   0xFF

#define LC_SAC_LINK_IDLE        0
#define LC_SAC_LINK_M_S0        1
#define LC_SAC_LINK_M_S1        2
#define LC_SAC_LINK_M_S2        3
#define LC_SAC_LINK_M_S3        4
#define LC_SAC_LINK_M_S4        5
#define LC_SAC_LINK_M_S5        6
#define LC_SAC_LINK_M_S6        7
#define LC_SAC_LINK_S0_M        8
#define LC_SAC_LINK_S1_M        9
#define LC_SAC_LINK_SW          10
#define LC_SAC_LINK_HW_SCHE     11
#define LC_SAC_BLE_DEVICE0      12
#define LC_SAC_BLE_DEVICE1      13

#define LC_E7_PARAM_BROADCAST_IDX           10

#define LC_MASTER0_LINK                     1
#define LC_MASTER1_LINK                     2
#define LC_SLAVE0_LINK                      0
#define LC_SLAVE1_LINK                      (MAX_NUM_ACL_LINK-1)

#define LC_SAC_SET_SW_HW_SHARE_BASE_ADDR(A) {LC_SFR_WRITE_BY_E7(E7OFFS_RW_SCATTERNET_CONTROL_BASE_ADDRESS_B0, PTR16_LSB(A));   \
                                             LC_SFR_CONT_WRITE_BY_E7(PTR16_MSB(A));                                        \
                                             }
//-
typedef U8 LE_DEVICE_IDX;
#define LC_BLE_DEVICE0                      ((LE_DEVICE_IDX)0)
#define LC_BLE_DEVICE1                      ((LE_DEVICE_IDX)1)

#define LC_SET_BLE_DEVICE_LINK_PTR(A,B)     {LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE_IDX, A);                  \
                                             LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE_LINK_PTR_B0,PTR16_LSB(B));     \
                                             LC_SFR_CONT_WRITE_BY_E6(PTR16_MSB(B));                        \
                                            }
#if (AB1520B_LC_RX_BUFFER_WORKAROUND)
#define LC_BLE_CLR_RXDONE_INT_STATUS() { \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE0_INT_STATUS_B0,  \
    (U8)(LC_40_INTERRUPT_ENABLE_DATA_CHANNEL_RXDONE | LC_40_INTERRUPT_ENABLE_ADV_CHANNEL_RXDONE ))}
#endif
#define LC_BLE_CLR_ACSYNC_INT_STATUS(link_idx) { \
    LC_SFR_WRITE_BY_E6(E6OFFS_RW_DEVICE0_INT_STATUS_B0+link_idx*4,  \
    (U8)(LC_40_INTERRUPT_ENABLE_AC_SYNC))}


#define LC_SAC_SET_CURRENT_LINK(A)                      LC_SFR_WRITE_BY_E7(E7OFFS_RW_CURRENT_LINK_CTRL, A)
#define LC_SAC_GET_CURRENT_LINK()                       LC_SFR_READ_BY_E7(E7OFFS_RW_CURRENT_LINK_CTRL)

#define LC_SAC_ACTIVE_VECTOR_SNIFF                      0x01
#define LC_SAC_ACTIVE_VECTOR_SCO_ESCO_312_5             0x02
#define LC_SAC_ACTIVE_VECTOR_SCO_ESCO_625               0x04
#define LC_SAC_ACTIVE_VECTOR_SNIFF_312_5                0x08
#define LC_SAC_ACTIVE_VECTOR_SNIFF_625                  0x10
#define LC_SAC_ACTIVE_VECTOR_RF_TX_ON                   0x20
#define LC_SAC_ACTIVE_VECTOR_RF_RX_ON                   0x40
#define LC_SAC_ACTIVE_VECTOR_RF_TX_RAMP_DOWN            0x80
//========================================== LC RF
#define LC_SAC_GET_RF_TX_RAMP_DOWN_ACTIVE()             (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & LC_SAC_ACTIVE_VECTOR_RF_TX_RAMP_DOWN)
#define LC_SAC_GET_RF_TX_ON()                           (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & LC_SAC_ACTIVE_VECTOR_RF_TX_ON)
#define LC_SAC_GET_RF_RX_ON()                           (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & LC_SAC_ACTIVE_VECTOR_RF_RX_ON)

//========================================== LC FLUSH
#define LC_FLUSH_START                                  0x01
#define LC_FLUSH_STOP                                   0x00
#define LC_FLUSH_ENHANCE_FLUSH                          0x02

#define LC_FLUSH_PB_FLUSHABLE                           0x20
#define LC_FLUSH_PB_NON_FLUSHABLE                       0x00

#define LC_FLUSH_PROCESSING_NON                         0x00
#define LC_FLUSH_PROCESSING_TX_DUMMY_DM1                0x01
#define LC_FLUSH_PROCESSING_WAIT_STOP_CMD               0x02

//========================================== LC MMI
#define LC_MMI1_DISABLE_FOR_LABTEST_ONLY()              LC_SFR_WRITE_BY_C2(C2OFFS_RW_MMI1_CTL_B1, 0x00)

//========================================== LC master broadcast
#define LC_MASTER_TX_BROADCAST_ENABLE   1
#define LC_MASTER_TX_BROADCAST_DISABLE  0
#define LC_MASTER_TX_AFH_BITMAP0    0x00
#define LC_MASTER_TX_AFH_BITMAP1    0x02

//========================================== LC inquiry
#define LC_INQUIRY_START                0x00
#define LC_INQUIRY_CONTINUE             0x01


//========================================== LC scan & conti_search
#define LC_SAC_INQUIRYSCAN_MODE                 (0x00)
#define LC_SAC_PAGESCAN_MODE                    (0x01)

#define LC_QUEUE_INQUIRY_SCAN                   (0x01)
#define LC_QUEUE_PAGE_SCAN                      (0x02)

#define LC_SET_QUEUE_INQUIRY_SCAN()             gLC_ctl.scan_ctl.queue_scan |= LC_QUEUE_INQUIRY_SCAN
#define LC_SET_QUEUE_PAGE_SCAN()                gLC_ctl.scan_ctl.queue_scan |= LC_QUEUE_PAGE_SCAN
#define LC_GET_QUEUE_INQUIRY_SCAN()             (gLC_ctl.scan_ctl.queue_scan & LC_QUEUE_INQUIRY_SCAN)
#define LC_GET_QUEUE_PAGE_SCAN()                (gLC_ctl.scan_ctl.queue_scan & LC_QUEUE_PAGE_SCAN)
#define LC_CLR_QUEUE_INQUIRY_SCAN()             gLC_ctl.scan_ctl.queue_scan &= (~LC_QUEUE_INQUIRY_SCAN)
#define LC_CLR_QUEUE_PAGE_SCAN()                gLC_ctl.scan_ctl.queue_scan &= (~LC_QUEUE_PAGE_SCAN)

#define LC_FHS_EIR_MASK                         (0x01)

#define LC_SET_SLAVE_NORMAL_SEARCH_ENABLE(A)    LC_SFR_WRITE_BY_E7(E7OFFS_RW_SLAVE_NORMAL_SEARCH_CTL, A);

#define LC_SLAVE_NORMAL_SEARCH_160              0x00
#define LC_SLAVE_NORMAL_SEARCH_320              0x10

//========================================== LC state control
LC_EXTERN VFUN CODE_PTR PDATA                   gLCS_swc_current_state;

#define LC_SWC_SET_STATE(A)                     (gLCS_swc_current_state = A)
#define LC_SWC_GET_STATE()                      (gLCS_swc_current_state)

//========================================== LC sw state control (page/ page scan / inquirt / inquiry scan)
#define LC_SW_STATE_NON                         0x00
#define LC_SW_STATE_INQUIRY                     0x01
#define LC_SW_STATE_PAGE                        0x02
#define LC_SW_STATE_INQUIRYSCAN                 0x04
#define LC_SW_STATE_PAGESCAN                    0x08
#define LC_SW_STATE_NEW_CONNECTION              0x09
#define LC_SW_STATE_SLAVE_RESPONSE              0x0A
#define LC_SW_STATE_MASTER_ROLESWITCH           0x10
#define LC_SW_STATE_SLAVE_ROLESWITCH            0x20
#define LC_SW_STATE_SLAVE_TESTMODE              0x40
#define LC_SW_STATE_LABTEST                     0x80
#define LC_SW_STATE_RSSISCAN                    0x90

#define LC_SW_STATE_SCAN                        (LC_SW_STATE_INQUIRYSCAN | LC_SW_STATE_PAGESCAN)
#define LC_SW_STATE_ROLESWITCH                  (LC_SW_STATE_MASTER_ROLESWITCH | LC_SW_STATE_SLAVE_ROLESWITCH)
#define LC_SW_STATE_AVOID_MASTER_SCO_ESCO       (LC_SW_STATE_SCAN | LC_SW_STATE_ROLESWITCH)

#define LC_SW_STATE_CTL_INQUIRYSCAN             gLC_ctl.InquiryScan
#define LC_SW_STATE_CTL_PAGESCAN                gLC_ctl.PageScan
#define LC_SW_STATE_CTL_INQUIRY                 gLC_ctl.Overlay.Inquiry
#define LC_SW_STATE_CTL_PAGE                    gLC_ctl.Overlay.Page
#define LC_SW_STATE_CTL_TESTMODE                gLC_ctl.Overlay.TestMode
#define LC_SW_STATE_CTL_LABTEST                 gLC_ctl.Overlay.LabTest
#define LC_SW_STATE_CTL_DIRECTTESTMODE          gLC_ctl.Overlay.DirectTestMode






//========================================== LE 4.0 device0 state control
LC_EXTERN VFUN CODE_PTR PDATA                   gLCS_device_current_state[MAX_NUM_LE_LINK];
LC_EXTERN VFUN PDATA                            gLCS_device_current_ac_sync_state[MAX_NUM_LE_LINK];



/**
 * LC_LE_SET_DEVICE_STATE
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 *
 * @d : The device index
 */
#define LC_LE_SET_DEVICE_STATE(d,s) { do {                                      \
    gLCS_device_current_state[(d)] = (s);                                       \
    } while (0); }

/* LC_LE_SET_DEVICE_AC_SYNC_STATE
 *
 * @Author : Gailong Hong
 *
 * @d : The device index
 */
#define LC_LE_SET_DEVICE_AC_SYNC_STATE(d,s) { do {                                      \
    gLCS_device_current_ac_sync_state[(d)] = (s);                                       \
    } while (0); }

/**
 * LC_LE_GET_DEVICE_STATE
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 *
 * @d : The device index
 * @Return : Device state
 */
#define LC_LE_GET_DEVICE_STATE(d) (gLCS_device_current_state[(d)])




//========================================== DMA
#define LC_RX_DMA_RESET_FIRST                   0

/* Workaround for 24M MCU clock */
#define LC_ENABLE_BTC_TX_DMA()                  {                                                                   \
                                                    U8 clk_rate;                                                    \
                                                    clk_rate = LC_SFR_READ_BY_C2(C2OFFS_RW_MCU_CLOCK_RATE_SEL);     \
                                                    LC_SFR_WRITE_BY_C2(C2OFFS_RW_MCU_CLOCK_RATE_SEL, 0x00);         \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    LC_SFR_WRITE(SFR_WO_DMA_CONTROL, 0x01);                         \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    LC_SFR_WRITE_BY_C2(C2OFFS_RW_MCU_CLOCK_RATE_SEL, clk_rate);     \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                }
#define LC_ENABLE_BTC_RX_DMA()                  //LC_SFR_WRITE(SFR_WO_DMA_CONTROL, 0x02)
#define LC_ENABLE_AUDIO_TX_DMA()                LC_SFR_WRITE(SFR_WO_DMA_CONTROL, 0x04)
#define LC_ENABLE_AUDIO_RX_DMA()                LC_SFR_WRITE(SFR_WO_DMA_CONTROL, 0x08)
#define LC_RESET_BTC_TX_FIFO()                  LC_SFR_WRITE(SFR_WO_DMA_CONTROL, 0x10)
#define LC_RESET_BTC_RX_FIFO()                  LC_SFR_WRITE(SFR_WO_DMA_CONTROL, 0x20)
#define LC_RESET_AUDIO_TX_FIFO()                LC_SFR_WRITE(SFR_WO_DMA_CONTROL, 0x40)
#define LC_RESET_AUDIO_RX_FIFO()                LC_SFR_WRITE(SFR_WO_DMA_CONTROL, 0x80)

#define LC_BTC_TX_DMA_BUSY()                    (LC_SFR_READ(SFR_RO_DMA_STATUS) & 0x01)
#define LC_BTC_RX_DMA_BUSY()                    (LC_SFR_READ(SFR_RO_DMA_STATUS) & 0x02)
#define LC_BTC_RX_DMA_OVERRUN()                 (LC_SFR_READ(SFR_RO_DMA_STATUS) & 0x04)

#define LC_SET_MCU_CLK_TO_12_24MHZ(A)           {                                                                   \
                                                    LC_SFR_WRITE_BY_C2(C2OFFS_RW_MCU_CLOCK_RATE_SEL, A);            \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                    _nop_();                                                        \
                                                }

#define LC_READ_MCU_CLK()                       LC_SFR_READ_BY_C2(C2OFFS_RW_MCU_CLOCK_RATE_SEL)

//========================================== SERVICE

#define LC_SAC_SW_GUARD_0_5_SLOT                    0x00
#define LC_SAC_SW_GUARD_1_0_SLOT                    0x01
#define LC_SAC_SW_GUARD_1_5_SLOT                    0x02

//========================================== LC Scheduler
#define LC_INVALID_CONNECTION_HANDLE                0xFF

#define LC_SAC_SCHEDULER_TYPE_AUDIO                 0x01
#define LC_SAC_SCHEDULER_TYPE_SNIFF                 0x02
#define LC_SAC_SCHEDULER_TYPE_SERVICE               0x03

//- conn_idx
#define LC_MASTER_LINK_IDX_ADD(A)                   (gLC_ctl.sche_ctl.master_lc_used_mask |= U8_BIT_MASK(A))
#define LC_MASTER_LINK_IDX_DEL(A)                   (gLC_ctl.sche_ctl.master_lc_used_mask &= (~U8_BIT_MASK(A)))

#define LC_M0_USED                                  0x02
#define LC_M1_USED                                  0x04

#define LC_S0_USED                                  0x01
#define LC_S1_USED                                  0x02

//- sac_conn_idx
/*
#define LC_SCHEDULER_SERVICE_IN(A)                  (gLC_ctl.sche_ctl.sche_service_in_mask |= (1<<A))
#define LC_SCHEDULER_SERVICE_OUT(A)                 (gLC_ctl.sche_ctl.sche_service_in_mask &= (~(1<<A)))
#define LC_SCHEDULER_AUDIO_IN(A)                    (gLC_ctl.sche_ctl.sche_audio_in_mask |= (1<<A))
#define LC_SCHEDULER_AUDIO_OUT(A)                   (gLC_ctl.sche_ctl.sche_audio_in_mask &= (~(1<<A)))
#define LC_SCHEDULER_SNIFF_IN(A)                    (gLC_ctl.sche_ctl.sche_sniff_in_mask |= (1<<A))
#define LC_SCHEDULER_SNIFF_OUT(A)                   (gLC_ctl.sche_ctl.sche_sniff_in_mask &= (~(1<<A)))
*/

#define LC_SCHEDULER_PRE_CONNECTION_ADD(A)          (gLC_ctl.sche_ctl.pre_connection_going |= U16_BIT_MASK(A))
#define LC_SCHEDULER_PRE_CONNECTION_DEL(A)          (gLC_ctl.sche_ctl.pre_connection_going &= (~U16_BIT_MASK(A)))
#define LC_SCHEDULER_SNIFF_ADD(A)                   (gLC_ctl.sche_ctl.sche_sniff_mask |= U16_BIT_MASK(A))
#define LC_SCHEDULER_SNIFF_DEL(A)                   (gLC_ctl.sche_ctl.sche_sniff_mask &= (~U16_BIT_MASK(A)))
#define LC_SCHEDULER_SNIFF_ANCHOR_ADD(A)            (gLC_ctl.sche_ctl.wait_first_sniff_anchor_mask |= U16_BIT_MASK(A), gLC_SAC_Power_Saving_Disable = TRUE)
#define LC_SCHEDULER_SNIFF_ANCHOR_DEL(A)            (gLC_ctl.sche_ctl.wait_first_sniff_anchor_mask &= (~U16_BIT_MASK(A)))
#define LC_GET_SNIFF_ANCHOR_MASK_BY_SAC_LINK(A)     (gLC_ctl.sche_ctl.wait_first_sniff_anchor_mask & U16_BIT_MASK(A))
#define LC_SCHEDULER_PRE_UNSNIFF_ADD(A)             (gLC_ctl.sche_ctl.pre_unsniff_mask |= U16_BIT_MASK(A), gLC_SAC_Power_Saving_Disable = TRUE)
#define LC_SCHEDULER_PRE_UNSNIFF_DEL(A)             (gLC_ctl.sche_ctl.pre_unsniff_mask &= (~U16_BIT_MASK(A)))
#define LC_SCHEDULER_AFH_INSTANT_WAIT_ADD(A)        (gLC_ctl.sche_ctl.afh_instant_wait |= U16_BIT_MASK(A))
#define LC_SCHEDULER_AFH_INSTANT_WAIT_DEL(A)        (gLC_ctl.sche_ctl.afh_instant_wait &= (~U16_BIT_MASK(A)))
#define LC_GET_SNIFF_MASK()                         (gLC_ctl.sche_ctl.sche_sniff_mask)
#define LC_SCHEDULER_DUMMY_SNIFF_ADD(A)             (gLC_ctl.sche_ctl.dummy_sniff_mask |= U8_BIT_MASK(A), gLC_SAC_Power_Saving_Disable = TRUE)
#define LC_SCHEDULER_DUMMY_SNIFF_DEL(A)             (gLC_ctl.sche_ctl.dummy_sniff_mask &= (~U8_BIT_MASK(A)))

#if 0
#define LC_SCHEDULER_SNIFF_TRANS_TIMER_ADD(A)       (gLC_ctl.sche_ctl.sniff_trans_timer_mask |= (A))
#define LC_SCHEDULER_SNIFF_TRANS_TIMER_DEL(A)       (gLC_ctl.sche_ctl.sniff_trans_timer_mask &= (~(A)))
#define LC_GET_SNIFF_TRANS_TIMER_MASK()             (gLC_ctl.sche_ctl.sniff_trans_timer_mask)
#endif

#define LC_SCHEDULER_POWER_SAVING_BLOCK()           (gLC_ctl.sche_ctl.pre_unsniff_mask || gLC_ctl.sche_ctl.afh_instant_wait || gLC_ctl.sche_ctl.wait_first_sniff_anchor_mask)

//========================================== ACL
#define LC_ACL_RX_BUFFER_READY()                    (gLC_ACL_buffer_ready)
#define LC_SET_ACL_RX_BUFFER_READY(A)               (gLC_ACL_buffer_ready = A)
#define LC_SET_ACL_RX_PACKET_INFO_FLOW_GO(A)        (A |= LC_RX_PACKET_INFO_FLOW_MASK)
#define LC_SET_ACL_RX_PACKET_INFO_FLOW_STOP(A)      (A &= (~LC_RX_PACKET_INFO_FLOW_MASK))
#define LC_SET_ACL_RX_PACKET_INFO_ARQN_AR(A)        (A |= LC_RX_PACKET_INFO_ARQN_MASK)
#define LC_SET_ACL_RX_PACKET_INFO_ARQN_NAK(A)       (A &= (~LC_RX_PACKET_INFO_ARQN_MASK))
#define LC_SET_ACL_RX_PACKET_INFO_SEQN_1(A)         (A |= LC_RX_PACKET_INFO_SEQN_MASK)
#define LC_SET_ACL_RX_PACKET_INFO_SEQN_0(A)         (A &= (~LC_RX_PACKET_INFO_SEQN_MASK))

#define LC_DV_TRANSFORM_SEL(A)                      LC_SFR_WRITE_BY_E7(E7OFFS_WO_DV_TRANSFORM_ENABLE, A);
#define LC_CURR_USED_RX_BUFF_NUM()                  (LC_SFR_READ_BY_FE (FEOFFS_RO_RXBUF_STATUS1) & 0x07)



////////////////////////////////////////////////////////////////////////////////
// RX Buffer Index Interface ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- mask
typedef U8 LC_RXBUF_MASK;
#define LC_RXBUF_BUF0_MASK                          ((LC_RXBUF_MASK)BIT_MASK8(0))
#define LC_RXBUF_BUF1_MASK                          ((LC_RXBUF_MASK)BIT_MASK8(1))
#define LC_RXBUF_BUF2_MASK                          ((LC_RXBUF_MASK)BIT_MASK8(2))

/**
 * LC_RXBUF_INIT()
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
#define LC_RXBUF_INIT() { do {                                                  \
    gLC_ctl.acl_share_rx_return_ptr[0] = NULL;                                  \
    gLC_ctl.acl_share_rx_return_ptr[1] = NULL;                                  \
    gLC_ctl.acl_share_rx_return_ptr[2] = NULL;                                  \
    LC_SFR_WRITE_BY_C2(C2OFFS_RO_RXBUF_IDX,BIT_MASK8(1));                       \
    } while (0); }

/**
 * LC_BXBUF_DROP_RXBUF_IDX()
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
#define LC_BXBUF_DROP_RXBUF_IDX() { do { LC_SFR_WRITE_BY_C2(C2OFFS_RO_RXBUF_IDX,BIT_MASK8(0)); } while (0); }

/**
 * LC_BXBUF_READ_RXBUF_IDX()
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
#define LC_BXBUF_READ_RXBUF_IDX() (LC_SFR_READ_BY_C2(C2OFFS_RO_RXBUF_IDX) & (LC_RXBUF_BUF0_MASK | LC_RXBUF_BUF1_MASK | LC_RXBUF_BUF2_MASK))

/**
 * LC_RXBUF_IS_BUF0_EMPTY()
 * LC_RXBUF_IS_BUF1_EMPTY()
 * LC_RXBUF_IS_BUF2_EMPTY()
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
#define LC_RXBUF_IS_BUF0_EMPTY() (gLC_ctl.acl_share_rx_return_ptr[0] == NULL)
#define LC_RXBUF_IS_BUF1_EMPTY() (gLC_ctl.acl_share_rx_return_ptr[1] == NULL)
#define LC_RXBUF_IS_BUF2_EMPTY() (gLC_ctl.acl_share_rx_return_ptr[2] == NULL)

/**
 * LC_RXBUF_READ_BUF0()
 * LC_RXBUF_READ_BUF1()
 * LC_RXBUF_READ_BUF2)
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
#define LC_RXBUF_READ_BUF0() (gLC_ctl.acl_share_rx_return_ptr[0])
#define LC_RXBUF_READ_BUF1() (gLC_ctl.acl_share_rx_return_ptr[1])
#define LC_RXBUF_READ_BUF2() (gLC_ctl.acl_share_rx_return_ptr[2])

/**
 * LC_RXBUF_CLEAR_BUF0()
 * LC_RXBUF_CLEAR_BUF1()
 * LC_RXBUF_CLEAR_BUF2()
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
#define LC_RXBUF_CLEAR_BUF0() { do { gLC_ctl.acl_share_rx_return_ptr[0] = NULL; } while(0); }
#define LC_RXBUF_CLEAR_BUF1() { do { gLC_ctl.acl_share_rx_return_ptr[1] = NULL; } while(0); }
#define LC_RXBUF_CLEAR_BUF2() { do { gLC_ctl.acl_share_rx_return_ptr[2] = NULL; } while(0); }

/**
 * LC_RXBUF_SAVE_BUF0()
 * LC_RXBUF_SAVE_BUF1()
 * LC_RXBUF_SAVE_BUF2()
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
#define LC_RXBUF_SAVE_BUF0(ptr) { do { gLC_ctl.acl_share_rx_return_ptr[0] = (ptr); } while(0); }
#define LC_RXBUF_SAVE_BUF1(ptr) { do { gLC_ctl.acl_share_rx_return_ptr[1] = (ptr); } while(0); }
#define LC_RXBUF_SAVE_BUF2(ptr) { do { gLC_ctl.acl_share_rx_return_ptr[2] = (ptr); } while(0); }

/**
 * LC_RXBUF_SET_HW_BUF_PTR0()
 * LC_RXBUF_SET_HW_BUF_PTR1()
 * LC_RXBUF_SET_HW_BUF_PTR2()
 *
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
#define LC_RXBUF_SET_HW_BUF0_PTR(ptr) { do {                                    \
    LC_SFR_WRITE_BY_FE(FEOFFS_RW_DMA_BTCRX0_ADDR_L,BYTE0((ADDR16)(ptr)));       \
    LC_SFR_CONT_WRITE_BY_FE(BYTE1((ADDR16)(ptr)));                              \
    } while(0); }
#define LC_RXBUF_SET_HW_BUF1_PTR(ptr) { do {                                    \
    LC_SFR_WRITE_BY_FE(FEOFFS_RW_DMA_BTCRX1_ADDR_L,BYTE0((ADDR16)(ptr)));       \
    LC_SFR_CONT_WRITE_BY_FE(BYTE1((ADDR16)(ptr)));                              \
    } while(0); }
#define LC_RXBUF_SET_HW_BUF2_PTR(ptr) { do {                                    \
    LC_SFR_WRITE_BY_FE(FEOFFS_RW_DMA_BTCRX2_ADDR_L,BYTE0((ADDR16)(ptr)));       \
    LC_SFR_CONT_WRITE_BY_FE(BYTE1((ADDR16)(ptr)));                              \
    } while(0); }








#define LC_INT_NUM_MASK                             0xF0

#define LC_RXBUF_READY_MASK                         0x03    //- num_of_rx_buf

#define LC_DV_TRANSFORM_ENABLE                      0x10
#define LC_DV_TRANSFORM_DISABLE                     0x00


//========================================== SCO / ESCO
#ifdef SUPPORT_SCO_ESCO
    #define LC_GET_SCO_ESCO_CTL_PTR(A, B)               &gLC_ctl.conn_ctl[(A)].SCO_ESCO_ctl[(B)]
    #define LC_GET_SCO_ESCO_CTL(A, B)                   gLC_ctl.conn_ctl[(A)].SCO_ESCO_ctl[(B)]
    #define LC_SET_NUM_SCO_ESCO_LINK(A)                 (gLC_ctl.sche_ctl.n_sco_esco_link = A)
    #define LC_GET_NUM_SCO_ESCO_LINK                    gLC_ctl.sche_ctl.n_sco_esco_link
    #define LC_SET_SCO_ESCO_MASK(A)                     (gLC_ctl.sche_ctl.sco_esco_mask = A)
    #define LC_GET_SCO_ESCO_MASK                        gLC_ctl.sche_ctl.sco_esco_mask
    #define LC_SET_AUDIO_CODEC_LINK_IDX(A)              (gLC_ctl.audio_hw_codec_link_idx = A)
    #define LC_GET_AUDIO_CODEC_LINK_IDX                 gLC_ctl.audio_hw_codec_link_idx
    #define LC_SET_AUDIO_CODEC_AUDIO_HANDLE(A)          (gLC_ctl.audio_hw_codec_audio_handle = A)
    #define LC_GET_AUDIO_CODEC_AUDIO_HANDLE             gLC_ctl.audio_hw_codec_audio_handle

    #define SCO_ESCO_DATA_RX_OK                         0x00
    #define SCO_ESCO_DATA_RX_INVAILD                    0x10
    #define SCO_ESCO_DATA_RX_NO_DATA                    0x20
    #define SCO_ESCO_DATA_RX_PARTIALLY_LOST             0x30


    #define LC_SCO_ESCO_NULL                            0x00
    #define LC_SCO_TYPE                                 0x01
    #define LC_ESCO_TYPE                                0x02

    #define LC_SAC_VOICE_SOURCE_CTL_SETUP(A)            LC_SFR_WRITE_BY_E7(E7OFFS_RW_AUDIO_SOURCE_CTRL, (A))
    #define LC_SAC_VOICE_SOURCE_SELECT_SCO              0x00
    #define LC_SAC_VOICE_SOURCE_SELECT_ESCO             0x01

    #define LC_SAC_SCO_ESCO_RESERVE_SLOT_HV3            0x00
    #define LC_SAC_SCO_ESCO_RESERVE_SLOT_HV2            0x01
    #define LC_SAC_SCO_ESCO_RESERVE_SLOT_HV1            0x02
    #define LC_SAC_SCO_ESCO_RESERVE_SLOT_ESCO_2SLOT     0x00
    #define LC_SAC_SCO_ESCO_RESERVE_SLOT_ESCO_4SLOT     0x10
    #define LC_SAC_SCO_ESCO_RESERVE_SLOT_ESCO_6SLOT     0x20

    //------For SCO/eSCO Error Report------
    #define LC_SCO_ESCO_ERROR_REPORT_COUNT              8
    //------For SCO/eSCO Error Report------


    #define LC_SCO_AIR_TYPE_u_LAW                       0x00
    #define LC_SCO_AIR_TYPE_A_LAW                       0x01
    #define LC_SCO_AIR_TYPE_CVSD                        0x02
    #define LC_SCO_AIR_TYPE_TRANSPARENT_DATA            0x03

    #define LC_SCO_ESCO_ATTACH_ACL_LINK_NONE            0xFF

    #define LC_AUDIO_MUTE                               0x01
    #define LC_AVAILABLE_AUDIO_IDX                      0x00

    #define LC_SAC_SET_SCO_ESCO_SW_GUARD(A)             LC_SFR_WRITE_BY_E7(E7OFFS_WO_SCO_ESCO_SW_GUARD, A);

    #define LC_SAC_SET_ESCO_RETRANSMITTION_WONDOW(A, B, C)  {LC_SFR_WRITE_BY_E7(E7OFFS_RW_MASTER_SLAVE_PARAMETER, A|B);    \
                                                                LC_SFR_WRITE_BY_E7(E7OFFS_WO_ESCO_RETRANSMIT_TIMEOUT, C);  \
                                                            }
    #define LC_SAC_SET_SCO_ESCO_RELEVANT_PARA(A, B)     LC_SFR_WRITE_BY_E7(E7OFFS_RW_MASTER_SLAVE_PARAMETER, A|B);

#endif

#define LC_SAC_GET_SCO_ESCO_312_5_ACTIVE()          (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & LC_SAC_ACTIVE_VECTOR_SCO_ESCO_312_5)
#define LC_SAC_GET_SCO_ESCO_625_ACTIVE()            (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & LC_SAC_ACTIVE_VECTOR_SCO_ESCO_625)

#define LC_SAC_GET_SNIFF_312_5_ACTIVE()             (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & LC_SAC_ACTIVE_VECTOR_SNIFF_312_5)
#define LC_SAC_GET_SNIFF_625_ACTIVE()               (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & LC_SAC_ACTIVE_VECTOR_SNIFF_625)

#define LC_SAC_GET_SNIFF_SCO_ESCO_312_5_ACTIVE()    (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & (LC_SAC_ACTIVE_VECTOR_SCO_ESCO_312_5 | LC_SAC_ACTIVE_VECTOR_SNIFF_312_5))
#define LC_SAC_GET_SNIFF_SCO_ESCO_625_ACTIVE()      (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & (LC_SAC_ACTIVE_VECTOR_SCO_ESCO_625 | LC_SAC_ACTIVE_VECTOR_SNIFF_625))

#define LC_SAC_GET_SCO_ESCO_COUNT_DOWN_B0()         (LC_SFR_READ_BY_E7(E7OFFS_RO_ESCO_SCO_DOWN_COUNT0))
#define LC_SAC_GET_SCO_ESCO_COUNT_DOWN_B1()         (LC_SFR_READ_BY_E7(E7OFFS_RO_ESCO_SCO_DOWN_COUNT1))

#define LC_SLAVE_EXTRA_HIGH_PRIOR_INTERVAL_AND_SIZE     (gLC_ctl.misc_ctl.slave_extra_high_priority_interval_and_size)

#define LC_SERVICE_HIGH_PRIORITY_CONTROL            (gLC_ctl.misc_ctl.service_high_priority_control)
#define LC_SERVICE_HIGH_PRIORITY_ENABLED            0x00
//========================================== SNIFF
#define LC_SAC_SNIFF_SW_GUARD_0_5_SLOT              0x00
#define LC_SAC_SNIFF_SW_GUARD_0_7_5_SLOT            0x01
#define LC_SAC_SNIFF_SW_GUARD_1_0_SLOT              0x02

#define LC_SAC_OSC_PLL_WAKEUP_GURAD                 (gLC_ctl.misc_ctl.osc_pll_wakeup_guard)
#define LC_SAC_MIN_SLEEP_DURATION_ENTER             (gLC_ctl.misc_ctl.min_sleep_duration_enter_in_slot)
#define LC_SAC_RX_IDLE_SLAVE_SNIFF_TRANSCATION      (gLC_ctl.misc_ctl.slave_sniff_trans_in_slot)

#define LC_SAC_SET_OSC_PLL_WAKEUP_GURAD(A)          LC_SFR_WRITE_BY_E7(E7OFFS_RW_OSC_PLL_WAKEUP_GURAD, A);

#define LC_SAC_SET_OSC_MIN_SLEEP_TIME_IN_HALF_SLOT(A)   LC_SFR_WRITE_BY_E7(E7OFFS_WO_MIN_SLEEP_TIME, A);

#define LC_SNIFF_LOSS_COUNT                         (gLC_ctl.misc_ctl.sniff_loss_count)

#define LC_SNIFF_HIGH_PRIOR_ADVANCE_RESERVED        (gLC_ctl.misc_ctl.sniff_high_prior_advance_reserved)
#define LC_MASTER_SNIFF_POLL_INTERVAL               (gLC_ctl.misc_ctl.master_sniff_poll_interval)

#define LC_SAC_GET_SNIFF_ACTIVE()                   (LC_SFR_READ_BY_E7(E7OFFS_RO_ACTIVE_VECTOR) & LC_SAC_ACTIVE_VECTOR_SNIFF)


#define LC_AFH_MEASURMENT_CRC_ENABLE                0x01
#define LC_AFH_MEASURMENT_RSSI_ENABLE               0x02
#define LC_AFH_SET_MEASURMENT_ENABLE(A)             LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_MEASURE_ENABLE, A);

#define LC_AFH_SET_MEASURMENT_RSSI_PARAM(A, B, C, D, E, F, G, H, I, J, K, L)                                            \
                                                    {LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_MEASURMENT_DURATION, A);         \
                                                     LC_SFR_CONT_WRITE_BY_FE(B);                                       \
                                                     LC_SFR_WRITE_BY_FE(FEOFFS_WO_AFH_RSSI_CH_STEP_START, C);          \
                                                     LC_SFR_CONT_WRITE_BY_FE(D);                                       \
                                                     LC_SFR_CONT_WRITE_BY_FE(E);                                       \
                                                     LC_SFR_CONT_WRITE_BY_FE(F);                                       \
                                                     LC_SFR_CONT_WRITE_BY_FE(G);                                       \
                                                     LC_SFR_CONT_WRITE_BY_FE(H);                                       \
                                                     LC_SFR_CONT_WRITE_BY_FE(I);                                       \
                                                     LC_SFR_CONT_WRITE_BY_FE(J);                                       \
                                                     LC_SFR_CONT_WRITE_BY_FE(K);                                       \
                                                     LC_SFR_CONT_WRITE_BY_FE(L);                                       \
                                                    }

#define LC_AFH_SET_AGC_PARAM(A,B,C)                 LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_AGC_PARAM, A | B | C);

#define LC_AFH_SET_RSSI_REPEAT_NUM(A)               LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_RSSI_COUNT_PARAM, ((A-1) & 0x03))
#define LC_AFH_SET_RSSI_GROUP_NUM(A)                LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_RSSI_COUNT_PARAM, (((A-1) & 0x3F) << 2))

#define LC_AFH_SET_MEASURMENT_CRC_PARAM(A, B)       {LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_CRC_RATIO, A);   \
                                                     LC_SFR_CONT_WRITE_BY_FE(B);                       \
                                                    }

#define LC_AFH_SET_AC_LOSS_COUNT_THRESHOLD(A)       {LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_AC_LOSS_COUNT_THRESHOLD, A);     \
                                                    }

#define LC_AFH_SET_HEC_ERR_COUNT_THRESHOLD(A)       {LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_HEC_ERR_COUNT_THRESHOLD, A);     \
                                                    }


#define LC_AFH_GET_MEASURMENT_RSSI_RESULT_COUNT()   LC_SFR_READ_BY_FE(FEOFFS_RO_AFH_RSSI_SUCCESS_NUM)


#define LC_AFH_MEAS_SET_SW_HW_SHARE_BASE_ADDR(A)    {LC_SFR_WRITE_BY_FE(FEOFFS_RW_AFH_START_ADDRESS_B0, (U8)(A));  \
                                                     LC_SFR_CONT_WRITE_BY_FE((U16)(A) >> 8);                       \
                                                    }

//========================================== Role Switch
#define LC_RS_CMD_PTR1()                     (gLC_ctl.rs_command_ptr1)
#define LC_RS_CMD_PTR2()                     (gLC_ctl.rs_command_ptr2)
#define LC_SAC_SET_ROLE_SWITCH_SW_GUARD(A)          LC_SFR_WRITE_BY_E7(E7OFFS_WO_ROLE_SWITCH_SW_GUARD, A);

//#define LC_SET_ROLE_SWITCH_INSTANT_MIN_IN_CLOCK       0x10
#define LC_SET_ROLE_SWITCH_INSTANT_MIN_IN_CLOCK     (0x10 + LC_ROLE_SWITCH_INSTANT_ADVANCE_IN_SLOT << 1)
#define LC_SLAVE_TDD_SWITCH_FAIL        0x01
#define LC_SLAVE_PICONET_SWITCH_FAIL    0x02
#define LC_MASTER_TDD_SWITCH_FAIL       0x01
#define LC_MASTER_PICONET_SWITCH_FAIL   0x02

#define LC_ROLE_SWITCH_INSTANT_ADVANCE_IN_SLOT  10
#define LC_AFH_INSTANT_ADVANCE_IN_SLOT  10

//========================================== LC Testmode
#define TEST_MODE_PAUSE_TEST                        0
#define TEST_MODE_TX_TEST_0                         1
#define TEST_MODE_TX_TEST_1                         2
#define TEST_MODE_TX_TEST_1010                      3
#define TEST_MODE_TX_TEST_PRBS                      4
#define TEST_MODE_LOOPBACK_ACL                      5
#define TEST_MODE_LOOPBACK_SCO                      6
#define TEST_MODE_LOOPBACK_ACL_WITHOUT_WHITENING    7
#define TEST_MODE_LOOPBACK_SCO_WITHOUT_WHITENING    8
#define TEST_MODE_TX_TEST_11110000                  9
#define TEST_MODE_EXIT                              255

#define TEST_MODE_TX_RX_ON_SINGLE_FREQUENCY         0
#define TEST_MODE_NORMAL_HOPPING                    1

#define TEST_MODE_PACKET_TYPE_FORMAT_MASK           0xF0
#define TEST_MODE_PACKET_TYPE_FORMAT_ACL_SCO        0x00
#define TEST_MODE_PACKET_TYPE_FORMAT_ESCO           0x10
#define TEST_MODE_PACKET_TYPE_FORMAT_EDR_ACL        0x20
#define TEST_MODE_PACKET_TYPE_FORMAT_EDR_ESCO       0x30

#define TEST_MODE_RX_AUDIO_PACKET_TO_DATA_BUFFER    0x02
#define TEST_MODE_RX_ACL_SCO                        0x00
#define TEST_MODE_RX_ESCO                           0x04
#define TEST_MODE_RX_PAYLOAD_SIZE_SET_TO_ZERO       0x08

#define PRBS_RESET                                  0x80

#define LC_BTC_TX_DATA_PRBS1    0x00
#define LC_BTC_TX_DATA_PRBS2    0x01
#define LC_BTC_TX_DATA_00000000 0x02
#define LC_BTC_TX_DATA_11111111 0x03
#define LC_BTC_TX_DATA_01010101 0x04
#define LC_BTC_TX_DATA_10101010 0x05
#define LC_BTC_TX_DATA_11110000 0x06
#define LC_BTC_TX_DATA_00001111 0x07

//============================================== LC LE Control
#define LC_LE_TX_DATA_PTR(A)                        (gLC_ctl.BLE_device_ctl[A].tx_data_ptr)
#define LC_LE_NEXT_TX_L2CAP_PTR(A)                  (gLC_ctl.BLE_device_ctl[A].next_tx_l2cap_ptr)
#define LC_LE_PAUSED_TX_DATA_PTR(A)                 (gLC_ctl.BLE_device_ctl[A].paused_tx_data_ptr)
#define LL_CONN_CTL(A)                              (gLC_ctl.BLE_device_ctl[A])


/* Critical Anchor State*/
#define LC_Critical_None         0x00
#define LC_Critical_BLE0         0x01
#define LC_Critical_BLE1         0x02
#define LC_Critical_eSCO         0x03
#define LC_Critical_Sniff        0x04
//===============================================

/* LC Link Scheduling status*/
#define LC_NormalSched         0x00
#define LC_RxFull              0x01
//===============================================


/* For QoS */
typedef U8  QOS_SETVICE_TYPE;
#define QOS_SERVICE_TYPE_NO_TRAFFIC         ((QOS_SETVICE_TYPE)0x00)
#define QOS_SERVICE_TYPE_BEST_EFFORT        ((QOS_SETVICE_TYPE)0x01)
#define QOS_SERVICE_TYPE_GUARANTEED_EFFORT  ((QOS_SETVICE_TYPE)0x02)
//===============================================


enum
{
    LC_SLAVE_NOT_ADDRESSED,
    LC_SLAVE_MASTER_POLL_OTHER_SLAVE,
    LC_SLAVE_ADDRESSED_BY_MASTER,
    LC_SLAVE_POLLED_BY_MASTER
};


#define STATE_TESTMODE_TR_MASK                      0xC0
#define STATE_TESTMODE_TX                           0x80
#define STATE_TESTMODE_RX                           0x40
#define STATE_TESTMODE_SLAVE_TRX_IDLE               (0x00)
#define STATE_TESTMODE_SLAVE_RX                     (STATE_TESTMODE_RX | 0x00)
#define STATE_TESTMODE_SLAVE_ACL_TX                 (STATE_TESTMODE_TX | 0x00)

#define TESTMODE_RX_SLOT                            0x00
#define TESTMODE_TX_SLOT                            0x01

//========================================== Charger
#define LC_GET_CHARGER_PLUG_ON()                    (LC_SFR_READ_BY_C2 (C2OFFS_RW_CHGFLO_BYPASS_1P8V) & 0x01)


//========================================== AFE Init
typedef U8 AFE_INIT_EXT_MODE;
#define AFE_INIT_NO_EXT_MODE        (AFE_INIT_EXT_MODE)0
#define AFE_INIT_EXT_WAIT_FOR_0     (AFE_INIT_EXT_MODE)1
#define AFE_INIT_EXT_WAIT_FOR_1     (AFE_INIT_EXT_MODE)2


//-------------------------------------------------------

//========================================== RSSI Computatuin
typedef S8 RX_GAIN;
#define RX_GAIN_MAX                 (RX_GAIN)70
#define RX_GAIN_STEP                (RX_GAIN)2

//-------------------------------------------------------

EXTERN U8  XDATA   gAgcGainSel;

///////////////////////////////////////////////////////////
//  Function declaration;
///////////////////////////////////////////////////////////
//- LC.c

VOID             LC_SetupHopFreq (VOID);

VOID             LC_MaintainPageScanTimer (VOID);
BOOL             LC_ScanScheduler (VOID);
//VOID             LC_SetCLK(U32 clk, U8 clkn) REENTRANT;
VOID             LC_UpdateClkOffset(U8 clk_sel);
VOID             LC_MaintainScanTimer (VOID);
BOOL             LC_SAC_CHECKInquiryAndPageCmd (VOID);
#ifdef INQUIRY_SUPPORT
VOID             LC_SACEnterInquiry (VOID);
#endif
VOID             LC_SACEnterPage (VOID);
VOID             LC_SACEnterInquiryScan(U8 from_standby);
VOID             LC_SACEnterPageScan(U8 from_standby);
VOID             LC_SWC2SAC (VOID);
VOID             LC_SWCExit (VOID);

EXTERN VOID      LC_WriteAC(BD_ADDR_STRU XDATA_PTR pBdAddr,PARITY_ARR XDATA Parity);
VOID             LC_EnterRX (VOID);
VOID             LC_EnterTX(VOID);
VOID             LC_RSEnterTRX(U8 is_rx, U8 service_idx, U8 restore_freq);
VOID             LC_SACDisableHWSlotTick(VOID);
VOID             LC_SACEnableHWSlotTick(VOID);
BOOL             LC_Check_Anchor(VOID);
VOID             LC_Check_SW_State(VOID);

EXTERN U32 LC_GetClkByLink(
    IN      U8 LinkIdx);




////////////////////////////////////////////////////////////////////////////////
// LC Timer Interfaces /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * LC_RdSysClk
 *
 * Get native system Bluetooth Clock
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Return : Native Bluetooth Clock
 */
EXTERN U32 LC_RdSysClk(VOID) REENTRANT;



/**
 * LC_TIMER_GetExpiredTime
 *
 * Initiate LC Timer
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Timeout : Timeout Value
 */
EXTERN U32 LC_TIMER_GetExpiredTime(
    IN      U32 Timeout) REENTRANT;

/**
 * LC_TIMER_IsExpired
 *
 * Check whether the LC Timer is expired
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @ExpiredTime : Time to expire
 * @Return : TRUE stands for LC Timer has been expired
 */
EXTERN BOOL LC_TIMER_IsExpired(
    IN      U32 ExpiredTime);



/**
 * LC_ConvertToClkPhase
 *
 * Convert signed phase (0.5 us resolution) into the format which hardware
 * prefers.
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @ExpiredTime : Time to expire
 * @Return : TRUE stands for LC Timer has been expired
 */
EXTERN U16 LC_ConvertToClkPhase(
    IN      S16 PhaseInHalfUs);



/**
 * LC_TunePhase
 *
 * Tune phase by inputing phase offset.
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @PhaseOffset : The phase offset
 */
EXTERN VOID LC_TunePhase(
    IN      S16 PhaseOffset);

VOID LC_SyncClk0toClk1625Boundary(void);
VOID LC_TmpCLKmSyncToCLKn (U8 clkm, U8 clkn);
VOID LC_CLKmSyncToCLKn (U8 clkm, U8 clkn, U8 clknoffset);

#define LC_CLK0SyncToCLK1()     LC_TmpCLKmSyncToCLKn(C2OFFS_RW_CLK0_B0, C2OFFS_RW_CLK1_B0)
#define LC_CLK0SyncToCLK2()     LC_CLKmSyncToCLKn(C2OFFS_RW_CLK0_B0, C2OFFS_RW_CLK2_B0, C2OFFS_RW_CLK2_OFFSET_B0)
#define LC_CLK0SyncToCLK3()     LC_CLKmSyncToCLKn(C2OFFS_RW_CLK0_B0, C2OFFS_RW_CLK3_B0, C2OFFS_RW_CLK3_OFFSET_B0)
#define LC_CLK1SyncToCLK2()     LC_CLKmSyncToCLKn(C2OFFS_RW_CLK1_B0, C2OFFS_RW_CLK2_B0, C2OFFS_RW_CLK2_OFFSET_B0)
#define LC_CLK1SyncToCLK3()     LC_CLKmSyncToCLKn(C2OFFS_RW_CLK1_B0, C2OFFS_RW_CLK3_B0, C2OFFS_RW_CLK3_OFFSET_B0)

////////////////////////////////////////////////////////////////////////////////
// Bluetooth Clock Interface ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define LC_RdCLKN() BT_TIMER_RdSysClk()
#define LC_GetCLKN() BT_TIMER_RdSysClk()

U16 LC_GetCLKPhase(U8 clk_sel);

#define LC_GetMasterClkPhase() LC_GetCLKPhase(CLKSEL_CLK1)
#define LC_GetSlave0ClkPhase() LC_GetCLKPhase(CLKSEL_CLK2)
#define LC_GetSlave1ClkPhase() LC_GetCLKPhase(CLKSEL_CLK3)
#define LC_GetBLELink0ClkPhase() LC_GetCLKPhase(CLKSEL_CLK_BLE_LINK0)
#define LC_GetBLELink1ClkPhase() LC_GetCLKPhase(CLKSEL_CLK_BLE_LINK1)

VOID             LC_ScanInitial      (VOID);

//- LC_Standby.c
VOID             LC_UpdateSWCSleepParam (VOID);
VOID             LC_CalcScanTimer (VOID);
VOID             LC_HWStandbyInitial(VOID);
VOID             LCS_HWStanbyTickHandler (VOID);

//- LC_Page.c
VOID             LC_EnterPageState       (VOID);
//VOID           LC_EnterPrePageState    (U8 XDATA_PTR ptr_cmd_from_LM);
VOID             LC_EnterPrePageState    (VOID);
VOID             LC_Page_Scheduler (VOID);
VOID             LC_SendPageEventToLM (U8 evt, U8 status);

//- LC_Inquiry.c
#ifdef INQUIRY_SUPPORT
VOID             LC_EnterInquiryState    (VOID);
VOID             LC_EnterPreInquiryState (VOID);
VOID             LC_SendInquiryEventToLM (U8 evt);
VOID             LC_Inquiry_Scheduler (VOID);
#endif

//- LC_InquiryScan.c
VOID             LC_EnterInquiryScanState (VOID);

//- LC_PageScan.c
VOID             LC_EnterPageScanState (VOID);

//- LC_RssiScan.c
VOID LC_EnterRssiScanState (VOID);


//- LC_CommonConnection.c
VOID             LC_LoadLMPPDU (U8 service_idx, ACL_CTL XDATA_PTR ptr_ACL_ctl, BOOL from_testmode);
VOID             LC_SegmentPDU (U8 service_idx, ACL_CTL XDATA_PTR ptr_ACL_ctl);
VOID             LC_ProcessACLPacket (U8 conn_idx);
VOID             LC_LE_Detach(U8 device_idx, U8 reason);
VOID             LC_ProcessDataPacket (U8 conn_idx);
VOID             LC_SetupACLTXContents(U8 service_idx, U8 role);
BOOL             LC_LoadPDU(U8 service_idx);

/**
 * LC_GetRxBufPtr
 *
 * Get the RX buffer pointer of this related RX DONE interrupt.
 *
 * @Author : HuiChuan Lin <HuiChuanLin@airoha.com.tw>
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 *
 * @Return : The pointer of RX_BUFFER_STRU
 */
EXTERN RX_BUFFER_STRU_PTR LC_GetRxBufPtr(VOID);


/**
 * LC_LE_GetRxBufPtr
 *
 * Get the RX buffer pointer of this related 4.0 RXDone interrupt
 * and copy to the suitable rx memory
 *
 * @Author : HuiChuan Lin <HuiChuanLin@airoha.com.tw>
 *
 * @Return : The pointer of RX_BUFFER_STRU
 */

EXTERN RX_BUFFER_STRU_PTR LC_LE_GetRxBufPtr(VOID);

/**
 * LC_GetSwRxBufPtr
 *
 * Get the RX buffer pointer of this related Sw RXDone interrupt
 * and copy to the suitable rx memory
 *
 * @Author : HuiChuan Lin <HuiChuanLin@airoha.com.tw>
 *
 * @Return : The pointer of RX_BUFFER_STRU
 */
EXTERN RX_BUFFER_STRU_PTR LC_GetSwRxBufPtr(VOID);

EXTERN RX_BUFFER_STRU_PTR LC_LE_FindSmallerRxBuf(RX_BUFFER_STRU_PTR rxbuf_ptr);


/**
 * LC_SetupRxBuffer
 *
 * Setup RX Buffer if buffer is empty
 *
 * @Author : HuiChuan Lin <HuiChuanLin@airoha.com.tw>
 * @Author : Charles Su <CharlesSu@airoha.com.tw>
 */
EXTERN VOID LC_SetupRxBuffer(VOID);

/**
 * LC_RealRssiComputation
 *
 * Compute real RSSI in unit of dBm .
 *
 * @Author : WenChiehLo <WenChiehLo@airoha.com.tw>
 *
 * @Return : Real RSSI value in unit of dBm
 */
EXTERN S8 LC_RealRssiComputation(VOID);

VOID             LC_ReleasePDU (U8 conn_idx);
VOID             LC_SendEventToLM (U8 service_idx, U8 event_code);
VOID             LC_ACLUSetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_AFHSetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_EncryptionSetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_RoleSwitchSetup(U8 service_idx, RS_CTL XDATA_PTR rs_cmd_ptr);
VOID             LC_SendRoleChangeEvent(U8 service_idx, U8 status, U8 reason, LC_EVT_ROLE_SWITCH_EVT_TYPE XDATA_PTR cmd_ptr);
U8               LC_CheckRoleSwitch(U8 service_idx);
U8               LC_AutoFlushHandler(U8 service_idx);
U8               LC_CheckEnhancedFlushCmd(CONN_CTL XDATA_PTR pConnCtl);
U8               LC_EnhancedFlushHandler(U8 service_idx);



VOID             LC_CheckAFH(U8 service_idx);
VOID             LC_ReleaseRoleSwitchCmdPtr(U8 service_idx);
VOID             LC_HoldSetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_HoldExit(U8 sac_conn_idx);
VOID             LC_SniffSetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_SniffExit(U8 service_idx);
#if (NEW_LC_SNIFF_TRANSITION)
VOID             LC_SniffTransitionSetup (U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_SniffTransitionExit (U8 service_idx);
BOOL             LC_CheckSniffAnchorInSniffTransitionMode(U8 idx, U8 guard_time);
#endif
VOID             LC_SniffSubrateSetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_MaxSlotChange(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_MasterArrangeServiceTick (VOID);
VOID             LC_Flush(U8 service_idx, HC_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_QoSSetup(U8 service_idx, LMLC_CMD_QOS_STRU_PTR pCmd);
#ifdef SUPPORT_SCO_ESCO
VOID             LC_SCOSetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_AudioExit(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr, BOOL event_to_LM);
VOID             LC_ESCOSetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_AudioSwitch (U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_EScoNullPktCtl(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);

#endif

VOID LC_SetFlowGotoGuaranteedLink(U8 service_idx, U8 tick_rate);

#ifdef SUPVIS_TIMEOUT_BY_LC
VOID LC_SupVisTimeoutInit(SUPVISTO_CTL_PTR pSupVisTo);
#endif

#if LC_SUPPORT_SAC_BARGE_IN
VOID             LC_ReTxWinModifySetup(U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
#endif
#ifdef SUPVIS_TIMEOUT_BY_LC

VOID             LC_UpdateSupervisionTO (U8 service_idx, LM_CMD_TYPE XDATA_PTR cmd_ptr);
VOID             LC_CheckSupVisTimeout(U8 service_idx);
VOID LC_ResetSlaveSupVisTimer(U8 slave_conn_idx);
#endif

BOOL LC_GET_VALID_LARGE_RX_BUF(VOID);
//- LC_MasterRoleSwitch.c
VOID             LC_MasterTDDSwitchEnter (VOID);

//- LC_SlaveRoleSwitch.c
VOID             LC_SlaveTDDSwitchEnter (VOID);

//- LC_Testmode.c
VOID LC_TestmodeSetup(VOID);
VOID LCS_TestModeSlotTick(VOID);
VOID LCS_TestModeRXDone(VOID);
VOID LCS_TestModeTXDone(VOID);
VOID LCS_TestModeHeaderReady(VOID);
VOID LCS_TestModeACLoss(VOID);
#if LE_SUPPORTED_CONTROLLER
//- LC_Advertising.c
PUBLIC U8 LC_EnterAdvertising(CONN_HDL_STRU_PTR ConnHdlPtr);


//- LC_DirecTestmode.c
VOID LC_LeRxTestSetup(TEST_CMD_STRU_PTR cmd_ptr);
VOID LC_LeTxTestSetup(TEST_CMD_STRU_PTR cmd_ptr);
#endif
//- LC_LabTest.c
VOID             LC_EnterLabTest (VOID);




#define LC_COMPARE_TO_CLK_CURRENT_SLOT_MATCH        0x01
#define LC_COMPARE_TO_CLK_NEXT_SLOT_MATCH           0x02

#define  LC_COMPARE_TO_CLKE_b0_b17_IN_CURRENT_SLOT(A)   (LC_COMPARE_TO_CLK (A, 18) == LC_COMPARE_TO_CLK_CURRENT_SLOT_MATCH)
#define  LC_COMPARE_TO_CLKE_b0_b17_IN_NEXT_SLOT(A)      (LC_COMPARE_TO_CLK (A, 18) == LC_COMPARE_TO_CLK_NEXT_SLOT_MATCH)


#define LC_SendHoldCompleteEvent(idx)           LC_SendEventToLM(idx, LC_EVENT_HOLD_COMPLETE)
#define LC_SendHoldExitedEvent(idx)             LC_SendEventToLM(idx, LC_EVENT_HOLD_EXITED)
#define LC_SendSniffCompleteEvent(idx)          LC_SendEventToLM(idx, LC_EVENT_SNIFF_COMPLETE)
#define LC_SendSniffSubrateCompleteEvent(idx)   LC_SendEventToLM(idx, LC_EVENT_SNIFF_SUBRATING_COMPLETE)
#define LC_SendUnsniffOkEvent(idx)              LC_SendEventToLM(idx, LC_EVENT_UNSNIFF_OK)
#ifdef SUPVIS_TIMEOUT_BY_LC
#define LC_SendSupervisionTOEvent(idx)          LC_SendEventToLM(idx, LC_EVENT_SUPERVISION_TO)
#endif

///////////////////////////////////////////////////////////
//  SAC scheduler control
///////////////////////////////////////////////////////////

#define LC_CONNECTION(A)                        (gLC_ctl.conn_ctl[A])


///////////////////////////////////////////////////////////
//  LC_Master.c
///////////////////////////////////////////////////////////
VOID LC_MasterAddNewSlave (U8 conn_idx, U8 role_switch);

VOID LCS_MasterSlotTickHandler(VOID);
VOID LCS_MasterRXDoneHandler (U8 sac_conn_idx);
VOID LCS_MasterAckedTxHandler (U8 sac_conn_idx);
VOID LCS_MasterSniffHandler (U8 sac_conn_idx);
VOID LCS_MasterHoldHandler(U8 sac_conn_idx);
VOID LCS_MasterServiceHandler (U8 sac_conn_idx);
VOID LCS_MasterAFHHandler (U8 sac_conn_idx);
VOID LCS_MasterRoleSwitchHandler (U8 sac_conn_idx);
VOID LCS_MasterLossConnectionHandler (U8 sac_conn_idx);
#ifdef SUPPORT_SCO_ESCO
VOID LCS_MasterSCOHandler (U8 sac_conn_idx);
VOID LCS_MasterESCOHandler (U8 sac_conn_idx);
VOID LCS_MasterSCOESCOTxDoneHandler (U8 sac_conn_idx);
VOID LCS_MasterSCOESCORxDoneHandler (U8 sac_conn_idx);
#endif
VOID LCS_MasterReleaseBroadcastPDU (VOID);
///////////////////////////////////////////////////////////
//  LC_Slave.c
///////////////////////////////////////////////////////////

VOID LCS_Slave0SlotTickHandler(VOID);
VOID LCS_Slave1SlotTickHandler(VOID);
VOID LCS_SlaveRXDoneHandler(U8 sac_conn_idx);
VOID LCS_SlaveAckedTxHandler(U8 sac_conn_idx);
VOID LCS_SlaveSniffHandler(U8 sac_conn_idx);
VOID LCS_SlaveHoldHandler(U8 sac_conn_idx);
VOID LCS_SlaveServiceHandler(U8 sac_conn_idx);
VOID LCS_SlaveAFHHandler(U8 sac_conn_idx);
VOID LCS_SlaveRoleSwitchHandler(U8 sac_conn_idx);
VOID LCS_SlaveLossConnectionHandler(U8 sac_conn_idx);
#ifdef SUPPORT_SCO_ESCO
VOID LCS_SlaveSCOHandler(U8 sac_conn_idx);
VOID LCS_SlaveESCOHandler(U8 sac_conn_idx);
VOID LCS_SlaveSCOESCOTxDoneHandler(U8 sac_conn_idx);
VOID LCS_SlaveSCOESCORxDoneHandler(U8 sac_conn_idx);
#endif
VOID LC_AddNewSlave(U8 conn_idx, U8 role_switch);
VOID LC_ChangeSlaveSniffTimeoutInCaseAttempt1Timeout0(U8 service_idx);

///////////////////////////////////////////////////////////
//  LC_LabTest.c
///////////////////////////////////////////////////////////
VOID LCS_LabTestSlotTickHandler (VOID);
VOID LCS_LabTestTXDoneHandler (VOID);
VOID LCS_LabTestRXDoneHandler (VOID);
VOID LCS_LabTestHeaderReadyHandler (VOID);
VOID LCS_LabTestSyncHandler (VOID);
VOID LCS_LabTestACLossHandler (VOID);

VOID LCS_LabTestloopbackSlotTickHandler (VOID);
VOID LCS_LabTestSYNCHandler (VOID);
VOID LCS_LabTestloopbackRXDoneHandler (VOID);
VOID LCS_LabTestloopbackTXDoneHandler (VOID);
VOID LCS_LabTestloopbackHeaderReadyHandler (VOID);
VOID LCS_LabTestloopbackACLossHandler (VOID);


///////////////////////////////////////////////////////////
//  State function tables;
///////////////////////////////////////////////////////////
EXTERN VFUN CODE_PTR pLCS_SAC_TopHandler;
extern VFUN CODE gLCS_SAC_TopHandler[] ALIGN(4);

extern VFUN CODE gLCS_Idle[] ALIGN(4);

extern VFUN CODE gLCS_LabTest[] ALIGN(4);
//extern VFUN CODE gLCS_LabTest_loopback[];

extern VFUN CODE gLCS_PrepareStandby[] ALIGN(4);
extern VFUN CODE gLCS_Standby[] ALIGN(4);

extern VFUN CODE gLCS_InquiryScan[] ALIGN(4);
extern VFUN CODE gLCS_ExtendedInquiryScan[] ALIGN(4);

extern VFUN CODE gLCS_PreInquiry[] ALIGN(4);
extern VFUN CODE gLCS_Inquiry[] ALIGN(4);

extern VFUN CODE gLCS_PrePageScan[] ALIGN(4);
extern VFUN CODE gLCS_PageScan[] ALIGN(4);
extern VFUN CODE gLCS_SlaveResponse[] ALIGN(4);
extern VFUN CODE gLCS_SlaveNewConnection[] ALIGN(4);

extern VFUN CODE gLCS_Page[] ALIGN(4);
extern VFUN CODE gLCS_PrePage[] ALIGN(4);
extern VFUN CODE gLCS_PageScan[] ALIGN(4);
extern VFUN CODE gLCS_PageMasterResponse[] ALIGN(4);
//extern VFUN CODE gLCS_PrePageMasterResponse[];
extern VFUN CODE gLCS_SlaveResponse[] ALIGN(4);
extern VFUN CODE gLCS_SlaveNewConnection[] ALIGN(4);

//extern VFUN CODE gLCS_MasterPreRoleSwitch[] ALIGN(4);
extern VFUN CODE_PTR XDATA pLCS_MasterPreRoleSwitch;
extern VFUN CODE gLCS_MasterTDDSwitch[] ALIGN(4);
extern VFUN CODE gLCS_MasterPiconetSwitch[] ALIGN(4);
extern VFUN CODE gLCS_MasterPiconetSwitchDummy[] ALIGN(4);

//extern VFUN CODE gLCS_SlavePreRoleSwitch[] ALIGN(4);
extern VFUN CODE_PTR XDATA pLCS_SlavePreRoleSwitch;
extern VFUN CODE gLCS_SlaveTDDSwitch[] ALIGN(4);
extern VFUN CODE gLCS_SlavePiconetSwitch[] ALIGN(4);
extern VFUN CODE gLCS_SlavePiconetSwitchDummy[] ALIGN(4);


extern VFUN CODE gLCS_TestModeConnection[] ALIGN(4);

#if (LE_SUPPORTED_CONTROLLER)

extern VFUN CODE gLCS_DirectTestMode[] ALIGN(4);
extern VFUN CODE gLCS_PreAdvertising[] ALIGN(4);
extern VFUN CODE gLCS_Advertising[] ALIGN(4);
extern VFUN CODE gLCS_ScanRsp[] ALIGN(4);
extern VFUN CODE gLCS_LEConnection[] ALIGN(4);
extern VFUN CODE gLCS_Scanning[] ALIGN(4);
EXTERN VFUN CODE gLCS_Initiating[] ALIGN(4);
#endif
extern VOID Printlink (U8 val);
extern VOID PrintS0Slottick (U8 val);
extern VOID PrintS1Slottick (U8 val);
extern VOID PrintSleep (U8 val);
extern VOID PrintMSlottick (U8 val);


//- labtest section
extern VFUN CODE gLCF_LabTestChargerTest[];


#define D4_DATA_TIMEOUT             (8*2)
#define D4_NULL_TIMEOUT             (8*2)
#define D4_POLL_INTERVAL            (40*2)
#define D4_DATA_ACTIVE_TIMEOUT      (D4_POLL_INTERVAL*5)
#define D4_SCHEDULE_REST_TIME       (((U32)(gHC_parameter.qos_best_effort_rest_weight)*D4_POLL_INTERVAL)/4)
#define HALF_ROUND_TIME             (0x7FFFFFFUL)
#define INVALID_LINK_SCH_CTL_IDX    ((U8)0xFF)

#define LC_SCAN_DISTANCE            (192*2)  //120ms
#define LC_PAGE_INQ_SEG_TIME        (16)     //unit slot (at least 16 slots (length of a train))
#define LC_PAGE_INQ_SEG_INTERVAL    (32)     //unit slot 20ms

extern VOID LC_LinkSched_EnableLink (U8 ConnIdx);
extern VOID LC_LinkSched_DisableLink (U8 ConnIdx);
extern U8 LC_LinkSched_GetCtlIdx (U8 ConnIdx);
extern VOID LC_LinkSched_SetDataActive (LK_SCH_CTL_STRU XDATA_PTR pLinkSchedCtl);
extern VOID LC_LinkSched_SetNullPacketTime (LK_SCH_CTL_STRU XDATA_PTR pLinkSchedCtl);
extern U32 LC_LinkSched_GetClk (U8 ConnIdx);
extern VOID LC_LinkSched_DisableService (U8 ConnIdx);
extern U8 LC_LinkSched_CheckExpire (U32 Clk, U32 CurrClk);
extern U8 LC_LinkSched_DataTimeoutExpire (LK_SCH_CTL_STRU XDATA_PTR pLinkSchedCtl);
extern U8 LC_LinkSched_LeaseExpire (LK_SCH_CTL_STRU XDATA_PTR pLinkSchedCtl);
extern U8 LC_LinkSched_NextSchedulabeExpire (LK_SCH_CTL_STRU XDATA_PTR pLinkSchedCtl);
extern U8 LC_LinkSched_ActiveTimeoutExpire (LK_SCH_CTL_STRU XDATA_PTR pLinkSchedCtl);
extern U8 LC_LinkSched_NullPacketTimeoutExpire (LK_SCH_CTL_STRU XDATA_PTR pLinkSchedCtl);
extern VOID LC_LinkSched_EnableService (U8 ConnIdx);
extern VOID LC_LinkSched (VOID);
extern VOID LC_LinkSched_Init (U8 conn_idx);
extern VOID LC_LinkSched_Deinit (U8 conn_idx);
//====== Calculate SCO, eSCO, Sniff, Sniff Subrate Instant ======
U32      LC_CalInstant (U8 service_idx, U32 inst_in_slot, U16 interval, U8 proc_guard) REENTRANT;
//====== Calculate SCO, eSCO, Sniff, Sniff Subrate Instant ======


#define LC_IsBdAddrTheSame(A,B) (!OS_memcmp((A),(B),sizeof(BD_ADDR_STRU)))

typedef struct stru_conn_req_pdu_ll_data
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

} CONN_REQ_PDU_LL_DATA_STRU;



typedef struct stru_conn_req_pdu
{
    BD_ADDR_STRU InitAddr;
    BD_ADDR_STRU AdvAddr;
    CONN_REQ_PDU_LL_DATA_STRU LlData;

} CONN_REQ_PDU_STRU;


////////////////////////////////////////////////////////////////////////////////
// New LabTest use  ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define FORCE_ENABLE_LC_MISC_SUPP_FEA_MOD_GAIN_COMPESATION()                                        \
    do {                                                                                            \
        gLC_ctl.misc_ctl.lc_misc_supp_fea2 |= (U8)LC_MISC_SUPP_FEA2_MOD_GAIN_COMPESATION;           \
    } while (0)

#define FORCE_DISABLE_LC_MISC_SUPP_FEA_MOD_GAIN_COMPESATION()                                       \
    do {                                                                                            \
        gLC_ctl.misc_ctl.lc_misc_supp_fea2 &= (U8)~LC_MISC_SUPP_FEA2_MOD_GAIN_COMPESATION;          \
    } while (0)

#define LABTEST_INIT_TX_GAIN_TO_SAC()                                                                      \
    do {                                                                                            \
        gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_edr_gc1    = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc1;       \
        gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_edr_gc2    = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc2;       \
        gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_edr_bias2_1   = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1;   \
        gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_edr_bias2_2   = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2;   \
        gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_gfsk_txgc1   = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc1;     \
        gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_gfsk_txgc2   = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc2;     \
        gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_gfsk_bias2_1 = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1; \
        gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_gfsk_bias2_2 = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2; \
        gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_edr_txgc1   = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_edr_gc1;     \
        gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_edr_txgc2   = gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_edr_gc2;     \
        gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_edr_bias2_1 = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_edr_bias2_1; \
        gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_edr_bias2_2 = gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_edr_bias2_2; \
    } while (0)

#define LABTEST_INIT_TX_GAIN_TO_SFR()                                                                      \
    do {                                                                                            \
        LC_SFR_SET_FE_OFFSET(FEOFFS_WO_SWC_TX_GC1);                                                 \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc1);   \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.mp_tx_gain_ctl.tx_default_gfsk_gc2);   \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1); \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2); \
    } while (0)

#define LABTEST_UPDATE_BDR_TX_GAIN()                                                                    \
    do {                                                                                            \
        LC_SFR_SET_FE_OFFSET(FEOFFS_WO_SWC_TX_GC1);                                                 \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_gfsk_txgc1);   \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_gfsk_txgc2);   \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_gfsk_bias2_1); \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_gfsk_bias2_2); \
    } while (0)

#define LABTEST_UPDATE_EDR_TX_GAIN()                                                                    \
    do {                                                                                            \
        LC_SFR_SET_FE_OFFSET(FEOFFS_WO_SWC_TX_GC1);                                                 \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_edr_txgc1);   \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_edr_txgc2);   \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_edr_bias2_1); \
        LC_SFR_CONT_WRITE_BY_FE(gLC_ctl.sac_sw_hw_share_ctl.sac_link_ctl[LABTEST_D4_LINK].rf_edr_bias2_2); \
    } while (0)

typedef U8  BT_TECHNOLOGY_MODE;
#define     BT_BR_EDR_MODE          (BT_TECHNOLOGY_MODE)0
#define     BT_BLE_MODE             (BT_TECHNOLOGY_MODE)1

/**
 * LC_LabTest_TempComp_Init
 *
 * LC LABTEST TEMPERATURE COMPENSATION INITIALIZATION
 *
 * @Author : WenChiehLo <WenChiehLo@airoha.com.tw>
 *
 */
VOID LC_LabTest_TempComp_Init(
    IN      BT_TECHNOLOGY_MODE mode);



//- MCU Power Saving
typedef struct stru_mcu_pwr_save
{
	VOID (*fEnterSleepMode)(VOID);
	VOID (*fEnterFrozenMode)(VOID);

} MCU_PWR_SAVE_STRU;

//- LC Function Pointer To Connect ROM and FLASH
typedef struct stru_lc_func_ptr
{
	VOID (*fLC_TestmodeSetup) (VOID);
	VOID (*fLC_ExitScanning) (VOID);
	VOID (*fLC_ExitInitiating) (U8);

} LC_FUNC_PTR_STRU;

EXTERN VOID EnterSleepMode(VOID);
EXTERN VOID EnterFrozenMode(VOID);
#if LE_SUPPORTED_CONTROLLER
EXTERN VOID LC_ExitScanning(VOID);
EXTERN VOID LC_ExitInitiating(U8 reason);
#endif
EXTERN MCU_PWR_SAVE_STRU XDATA MCU_PWR_SAVE;
EXTERN LC_FUNC_PTR_STRU XDATA LC_FUNC;
//-


EXTERN LCLM_EVT_MSG_STRU_PTR LC_NewLCLMEvt(
    IN      U8 EvtCode);

VOID LC_CheckSWStateAfterDetach (VOID);
VOID LC_ResumeAclServiceFromAnchor(VOID);
VOID LC_ResumeStandbyFromAnchor(VOID);

EXTERN BTCLK LC_RdDeviceClk(VOID);
EXTERN BTCLK LC_RdDeviceAnchor(VOID);
EXTERN BTCLK LC_RdSniffInstant(VOID);


EXTERN BOOL LC_CheckResourceForInquiryPageScan(VOID);
EXTERN VOID LC_CheckInquiryScanIssue(VOID);
EXTERN VOID LC_CheckPageScanIssue(VOID);
EXTERN VOID LC_CheckRssiScanIssue(VOID);
EXTERN VOID LC_UpdateInquiryScanInstant(VOID);
EXTERN VOID LC_UpdatePageScanInstant(VOID);
EXTERN VOID LC_UpdateRssiScanInstant(U8 Interval);
EXTERN VOID LC_KickNextScan(VOID);
EXTERN BOOL LC_ScanScheduler(VOID);

EXTERN U8 LC_CheckAnchor2(U8 NumOfNeededSlot);
EXTERN BOOL LC_LowPriorityLinkSched(U8 Page_Inq);
EXTERN VOID LC_DisableHWSlotTick(VOID);
EXTERN VOID LC_RecoverHWSlotTick(VOID);
EXTERN VOID LC_EnterSwMode(VOID);
EXTERN VOID LC_EnterHwMode(VOID);
EXTERN VOID LC_LinkSched2(VOID);
EXTERN U8   LC_CompensatedSWandACLServiceTimer(U8 isSW);
EXTERN BOOL LC_AclLinkSched(VOID);

EXTERN BOOL LC_Check_Page_Inquiry_Active(VOID);
EXTERN VOID LC_ResumePrePageState(VOID);
EXTERN VOID LCS_EnterStandby(VOID);
EXTERN VOID LC_CriticalEndCheckResumeEntry(VOID);
EXTERN VOID LC_DetachCheckACLDataFromHost(U8 service_idx);
EXTERN VOID LC_CheckSniffToOverlapEscoSlot(VOID);

EXTERN VOID LC_BargeInESCOWESCO(U8 service_idx, U8 is_barge_in);
#endif

EXTERN VOID LC_AdjustSlotDelayBeforeSniff(U8 service_idx);
EXTERN VOID LC_AdjustSlotDelayBeforeESco(U8 service_idx);
EXTERN U8   LC_GetSlotDelayFEoffset(U8 sac_conn_idx);
EXTERN U8   LC_GetAdjustTickRate(U8 sac_conn_idx);
EXTERN U8   LC_CheckSniffAnchor(U8 service_idx, U8 NumOfNeededSlot);
EXTERN U8   LC_CheckEScoAnchor(U8 service_idx, U8 NumOfNeededSlot);
EXTERN S32  LC_CalDeltaGuardTime(U8 service_idx, U32 next_instant_in_slot);
EXTERN U8   LC_AudioHandleToAudioIdx (U8 service_idx, U8 audio_handle);
EXTERN BOOL LC_SleepCheckSniffAnchor(VOID);
#ifdef LE_SUPPORTED_CONTROLLER
EXTERN VOID LC_ResumeAdvTick(VOID);
EXTERN VOID LC_DisableAdvTick(VOID);
#endif


