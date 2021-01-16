#ifndef _SECTOR_LC_MISC_CTL_H_
#define _SECTOR_LC_MISC_CTL_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_lc_misc_ctl.c defines the LC parameters.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */

#include <crc16.h>
#include "ctlr_config.h"
////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- feature 0 bank
//#define LC_MISC_SUPP_FEA0_MEM_IO_OUTPUT_ANYWAY                0x01
//#define LC_MISC_SUPP_FEA0_MEM_IO_ROM_DEFAULT_INPUT            0x02
#define LC_MISC_SUPP_FEA0_SUPPORT_SLEEP                     0x04
#define LC_MISC_SUPP_FEA0_MCU_PAUSE                         0x08
#define LC_MISC_SUPP_FEA0_SUPPORT_STANDBY_SLEEP             0x10
#define LC_MISC_SUPP_FEA0_MASTER_TRACKING                   0x20
#define LC_MISC_SUPP_FEA0_SUPPORT_EIR                       0x40
#define LC_MISC_SUPP_FEA0_CLKE_CONTROL_POWER_SAVING         0x80

//- feature 1 bank
#define LC_MISC_SUPP_FEA1_HID_SNIFF_WINDOW                                  0x01
#define LC_MISC_SUPP_FEA1_PAGE_POWER_DECREASE                               0x02
#define LC_MISC_SUPP_FEA1_HID_CLK3_SPEEDUP_POWER_SAVING                     0x04
#define LC_MISC_SUPP_FEA1_SLAVE_SNIFF_A1T0_WAIT_LMP_ACKED_EXTEND_TIMEOUT    0x08
#define LC_MISC_SUPP_FEA1_SLAVE_SNIFF_ANT0_WAIT_LMP_ACKED_EXTEND_TIMEOUT    0x10
#define LC_MISC_SUPP_FEA1_AVOID_CONSECUTIVE_INQUIRY_SCAN                    0x20
#define LC_MISC_SUPP_FEA1_INTERNAL_32K_POWER_ON_K                           0x40
#define LC_MISC_SUPP_FEA1_FORCE_RSSI_SCAN                                   0x80

//- feature 2 bank
#define LC_MISC_SUPP_FEA2_60M_HOLD                          0x01
#define LC_MISC_SUPP_FEA2_32K_HOLD                          0x02
#define LC_MISC_SUPP_FEA2_RSSI_SCAN                         0x04
#define LC_MISC_SUPP_FEA2_CHECK_60M_LOCKED                  0x08
#define LC_MISC_SUPP_FEA2_MOD_GAIN_COMPESATION              0x10
#define LC_MISC_SUPP_FEA2_SCO_ESCO_POWER_SAVING             0x20
#define LC_MISC_SUPP_FEA2_PROCESS_TEMP_SENSOR               0x40
#define LC_MISC_SUPP_FEA2_ENABLE_TEMP_SENSOR                0x80

//- feature 3 bank
/* Out of service */
#define LC_MISC_SUPP_FEA3_FIXED_PAGE_INQ_SEGMENTATION_TIME  0x01
#define LC_MISC_SUPP_FEA3_NORMAL_SEARCH_LM_CMD              0x04
#define LC_MISC_SUPP_FEA3_NORMAL_SEARCH_EXIT_HOLD           0x08
#define LC_MISC_SUPP_FEA3_DUMMY_SNIFF_ENABLE                0x10
#define LC_MISC_SUPP_FEA3_BARGE_IN_ESCO_WESCO               0x20
#define LC_MISC_SUPP_FEA3_ONE_LINK_RX_FULL                  0x40

//- feature 4 bank
#define LC_MISC_SUPP_FEA4_LC_SELF_UNSNIFF_PROC              0x01
#define LC_MISC_SUPP_FEA4_RANDOM_SCAN_WINDOW                0x02
#define LC_MISC_SUPP_FEA4_PKT_BLOCKED_CONTROL               0x04
#define LC_MISC_SUPP_FEA4_SUPPORT_EIR_INQUIRY               0x08
#define LC_MISC_SUPP_FEA4_SUPPORT_FLOW_STOP_IGNORANCE       0x10
#define LC_MISC_SUPP_FEA4_SLOTTICK_DELAY_IN_SNIFF_MODE      0x20
#define LC_MISC_SUPP_FEA4_A2DP_POWER_SAVING_MECHANISM       0x40
#define LC_MISC_SUPP_FEA4_AFH_POWER_SAVING_DELAY            0x80

//- feature 5 bank

//========================================== LC Power saving
#define LC_DELAY_POWER_SAVING_MASK                  0x3F
#define LC_DELAY_POWER_SAVING_RX_LMP                0x80
#define LC_DELAY_POWER_SAVING_RX_L2CAP              0x40
#define LC_DELAY_POWER_SAVING_RX_LMP_L2CAP          0xC0
#define LC_NON_DELAY_POWER_SAVING_RX_LMP_L2CAP      0x00

//========================================== PMU

#define LC_PMU_SLEEP_EN                             0x80
#define LC_PMU_DEEP_SLEEP_EN                        0x40
#define LC_PMU_POWER_OFF                            0x20
#define LC_PMU_SLEEP_CLK_EN                         0x10
#define LC_PMU_MCU_FROZEN                           0x04



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8  inquiry_and_inquiry_scan_priority;
    U8  page_and_page_scan_priority ;
    U8  master_and_slave_role_switch_priority;
} LC_SCATTERNET_BARGEIN_CTL;

typedef struct
{
    void (* LC_VCMDHandler)(U8 XDATA_PTR cmd_ptr);
} LC_LABTEST_VCMD_HANDLER;

typedef struct
{
    U8  lc_misc_supp_fea0; //- reference as LC misc support feature0 in LC.h
    U8  lc_misc_supp_fea1; //- reference as LC misc support feature1 in LC.h
    U8  lc_misc_supp_fea2; //- reference as LC misc support feature2 in LC.h
    U8  lc_misc_supp_fea3; //- reference as LC misc support feature3 in LC.h
    U8  lc_misc_supp_fea4; //- reference as LC misc support feature4 in LC.h
    U8  lc_misc_supp_fea5; //- reference as LC misc support feature5 in LC.h

    U8  hopping_duration_in_us;
    U8  manual_rx_window_in_us;
    U8  master_response_timeout_in_2slot;
    U8  slave_response_timeout_in_2slot;
    U8  newconnection_timeout_in_2slot;

    U8  lc_sco_esco_power_saving_active_timer;
    U8  lc_sco_esco_power_saving_frozen_timer;
    U8  master_adjusted_tick_rate;      //- for adjusting tick rate
    U8  slave_adjusted_tick_rate;       //- for adjusting tick rate
    U8  le_tick_rate;

    U8  master_service_poll_prefer_mask;
    U8  slave_service_rx_null_notx_null_prefer_mask;

    U8  next_min_sniff_instant;

    U8  slave_extra_high_priority_interval_and_size;
    U8  service_high_priority_control;
    U8  sniff_loss_count;
    U8  sniff_high_prior_advance_reserved;

    U8  standby_sleep_delay;
    U8  sleep_type;
    U8  mcu_frozen_type;
    U8  mmi2_interval;
    U8  lc_delay_power_saving;      //- bit7    bit6        bit5-0
                                    //- LMP L2CAP   value_in_252ms
    U16 master_tracking_offset;     //- master_tracking_offset, Unit: 0.5usec
    U8  allowed_pkt_blocked_time;

    LC_LABTEST_VCMD_HANDLER     LC_VcmdHandlerSet[2];
    U8  sfr_pmu_settings;
#if LE_SUPPORTED_CONTROLLER

#endif
	U8 page_scan_rssi_threshold;
} LC_MISC_CONTROL;

typedef struct
{
	LC_MISC_CONTROL	lc_misc;

	CRC16 CRC;

} SECTOR_LC_MISC_CONTROL_STRU;

#endif /* _SECTOR_LC_MISC_CTL_H_ */
