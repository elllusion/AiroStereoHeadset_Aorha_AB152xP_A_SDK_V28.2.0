/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_lc_misc_ctl.c defines the LC parameters.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"
#include "sector_lc_misc_ctl.h"
#include "ctlr_config.h"
#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_LC_MISC_CONTROL_STRU CODE gMISC_ctl_init =
{
    {
        //- U8  lc_misc_supp_fea0;
		#if defined SOUND_BAR || defined SCO_WOOFER
			//	LC_MISC_SUPP_FEA0_SUPPORT_STANDBY_SLEEP     |
		#else
			LC_MISC_SUPP_FEA0_SUPPORT_STANDBY_SLEEP     |
		#endif
    #ifdef HID_SOC
    #else
		#if defined SOUND_BAR || defined SCO_WOOFER || defined KB_App
			//	LC_MISC_SUPP_FEA0_SUPPORT_SLEEP             |
			//	LC_MISC_SUPP_FEA0_MCU_PAUSE                 |
		#else
			LC_MISC_SUPP_FEA0_SUPPORT_SLEEP             |
			LC_MISC_SUPP_FEA0_MCU_PAUSE                 |
		#endif
    //	LC_MISC_SUPP_FEA0_MCU_PAUSE                 |
    #endif
        LC_MISC_SUPP_FEA0_SUPPORT_EIR               |
//-     LC_MISC_SUPP_FEA0_CLKE_CONTROL_POWER_SAVING |
        LC_MISC_SUPP_FEA0_MASTER_TRACKING,

        //- U8  lc_misc_supp_fea1;
//            LC_MISC_SUPP_FEA1_PAGE_POWER_DECREASE |
            LC_MISC_SUPP_FEA1_FORCE_RSSI_SCAN  |
        #ifdef HID_SOC
            #ifdef HID_IIM_SOC
            LC_MISC_SUPP_FEA1_HID_SNIFF_WINDOW |
            #else
    //-     LC_MISC_SUPP_FEA1_SLAVE_SNIFF_ANT0_WAIT_LMP_ACKED_EXTEND_TIMEOUT |
            LC_MISC_SUPP_FEA1_SLAVE_SNIFF_A1T0_WAIT_LMP_ACKED_EXTEND_TIMEOUT |
            LC_MISC_SUPP_FEA1_HID_CLK3_SPEEDUP_POWER_SAVING |
            LC_MISC_SUPP_FEA1_HID_SNIFF_WINDOW,
            #endif
        #elif defined(HEADSET_SOC)
    //-     LC_MISC_SUPP_FEA1_SLAVE_SNIFF_ANT0_WAIT_LMP_ACKED_EXTEND_TIMEOUT |
    //-     LC_MISC_SUPP_FEA1_INTERNAL_32K_POWER_ON_K                        |
    //-     LC_MISC_SUPP_FEA1_AVOID_CONSECUTIVE_INQUIRY_SCAN                 |
            LC_MISC_SUPP_FEA1_SLAVE_SNIFF_A1T0_WAIT_LMP_ACKED_EXTEND_TIMEOUT,
        #else
    //-     LC_MISC_SUPP_FEA1_SLAVE_SNIFF_ANT0_WAIT_LMP_ACKED_EXTEND_TIMEOUT |
            LC_MISC_SUPP_FEA1_SLAVE_SNIFF_A1T0_WAIT_LMP_ACKED_EXTEND_TIMEOUT |
            LC_MISC_SUPP_FEA1_INTERNAL_32K_POWER_ON_K                        |
        #endif

        //- U8  lc_misc_supp_fea2;
        LC_MISC_SUPP_FEA2_RSSI_SCAN                 |
        #ifdef OPERATION_IN_BB_ASIC_VERSION
        LC_MISC_SUPP_FEA2_ENABLE_TEMP_SENSOR        |
		#ifndef AB1520SA
        LC_MISC_SUPP_FEA2_MOD_GAIN_COMPESATION      |
		#endif
        LC_MISC_SUPP_FEA2_32K_HOLD                  |
        #ifdef BLUETOOTH_SPEAKER
            #ifdef CELT_MODULE
            LC_MISC_SUPP_FEA2_SCO_ESCO_POWER_SAVING     |
            #else
            //- LC_MISC_SUPP_FEA2_SCO_ESCO_POWER_SAVING     |
            #endif
        #else
        LC_MISC_SUPP_FEA2_SCO_ESCO_POWER_SAVING     |
        #endif
        0x00,
        #else
        0x00,
        #endif

        //- U8  lc_misc_supp_fea3;
        LC_MISC_SUPP_FEA3_FIXED_PAGE_INQ_SEGMENTATION_TIME      |
        LC_MISC_SUPP_FEA3_NORMAL_SEARCH_LM_CMD                  |
        LC_MISC_SUPP_FEA3_NORMAL_SEARCH_EXIT_HOLD               |
        #if (defined (SOUND_BAR) || defined(SCO_WOOFER))
        //LC_MISC_SUPP_FEA3_BARGE_IN_ESCO_WESCO                   |
        #else
        LC_MISC_SUPP_FEA3_BARGE_IN_ESCO_WESCO                   |
        #endif

        #if defined BLUETOOTH_SPEAKER
        LC_MISC_SUPP_FEA3_ONE_LINK_RX_FULL                      |
         #else
        //LC_MISC_SUPP_FEA3_ONE_LINK_RX_FULL                      |
        #endif

        0x00,

        //- U8  lc_misc_supp_fea4;
        LC_MISC_SUPP_FEA4_SUPPORT_EIR_INQUIRY               |
//      LC_MISC_SUPP_FEA4_LC_SELF_UNSNIFF_PROC              |
        LC_MISC_SUPP_FEA4_RANDOM_SCAN_WINDOW                |
        LC_MISC_SUPP_FEA4_PKT_BLOCKED_CONTROL               |
//      LC_MISC_SUPP_FEA4_SUPPORT_FLOW_STOP_IGNORANCE       |
        #if defined BLUETOOTH_SPEAKER
            //LC_MISC_SUPP_FEA4_A2DP_POWER_SAVING_MECHANISM   |
        #else
            LC_MISC_SUPP_FEA4_A2DP_POWER_SAVING_MECHANISM   |
        #endif
//      LC_MISC_SUPP_FEA4_AFH_POWER_SAVING_DELAY            |
        LC_MISC_SUPP_FEA4_SLOTTICK_DELAY_IN_SNIFF_MODE      |
        0x00,

        //- U8  lc_misc_supp_fea5;
        0x00,

        150,    //- U8  hopping_duration_in_us;
        100,    //- U8  manual_rx_window_in_us;
        #ifdef HID_SOC
        16,     //- U8  master_response_timeout_in_2slot
        16,     //- U8  slave_response_timeout_in_2slot
        16,     //- U8  newconnection_timeout_in_2slot
        #else
        4,      //- U8  master_response_timeout_in_2slot
        4,      //- U8  slave_response_timeout_in_2slot
        16,     //- U8  newconnection_timeout_in_2slot
        #endif

        #ifdef CELT_MODULE
        12,      //- U8  lc_sco_esco_power_saving_active_timer;
        96,      //- U8  lc_sco_esco_power_saving_frozen_timer;
        #else
        96,      //- U8  lc_sco_esco_power_saving_active_timer;
        96,      //- U8  lc_sco_esco_power_saving_frozen_timer;
        #endif

        2,      //- U8  master_adjusted_tick_rate;
        2,      //- U8  slave_adjusted_tick_rate;
        1,      //- U8  ble_tick_rate


        0x03,   //- U8  master_service_poll_prefer_mask;                //- bit 0: M0, bit 6 : M6
        0x03,   //- U8  slave_service_rx_null_notx_null_prefer_mask;    //- bit 0: S0, bit 1 : S1

        16,     //- U8  next_min_sniff_instant;
        0x15,   //- U8  slave_extra_high_priority_interval_and_size
        0x00,   //- U8  service_high_priority_control
        0x11,   //- U8  sniff_loss_count
        //0x33,   //- U8  sniff_high_prior_advance_reserved
        0x00,   //- U8  sniff_high_prior_advance_reserved

        #ifdef HID_SOC
        2,      //- U8  standby_sleep_delay
        #else
        10,     //- U8  standby_sleep_delay
        #endif

        LC_PMU_SLEEP_EN,                    //- U8  sleep_type;
        LC_PMU_MCU_FROZEN,                  //- U8  mcu_frozen_type;

        #ifdef HID_SOC
        0x00,                                                                                   //- U8  mmi2_interval;
        LC_NON_DELAY_POWER_SAVING_RX_LMP_L2CAP,                                                 //- U8  lc_delay_power_saving
        #elif defined(HEADSET_SOC)
        0,                                                                              //- U8  mmi2_interval;
        LC_DELAY_POWER_SAVING_RX_LMP | 4,   //- 12 : 12 * 252ms = 3.024 s, 4: 1.008 s                                   //- U8  lc_delay_power_saving
        #else
        0,                                                                          //- U8  mmi2_interval;
        LC_DELAY_POWER_SAVING_RX_LMP | 12,  //- 12 : 12 * 252ms = 3.024 s                                   //- U8  lc_delay_power_saving
        #endif

        2480,   //- master_tracking_offset, Unit: 0.5usec
        200,    //- allowed_pkt_blocked_time Unit: slot

        {0, 0}, //backup vcmd entry point
        0,     //- sfr_pmu_settings
#if LE_SUPPORTED_CONTROLLER

#endif
		0
    },

    //- CRC
    {0,0}
};
