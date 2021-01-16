#ifndef _SECTOR_RF_MISC_H_
#define _SECTOR_RF_MISC_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_rf_misc.h defines the control options of RF.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"
#include "ctlr_config.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_mode_gain_comp
{
	U8	reserved;
	//U16	tx_mod_gain_temp_step[7];
	U8	tx_bdr_mod_gain_comp[8];
	U8	tx_bdr_fil_gain_comp[8];
	U8  tx_bdr_mod_bias_comp[8];
    U8	tx_edr_mod_gain_comp[8];
	U8	tx_edr_fil_gain_comp[8];
	U8  tx_edr_mod_bias_comp[8];
    U8	tx_ble_mod_gain_comp[8];
	U8	tx_ble_fil_gain_comp[8];
	U8  tx_ble_mod_bias_comp[8];
	//U8  bdr_txagc_gc[8];
	//U8  edr_txagc_gc[8];
    U8  ble_txagc_gc[8];
	U8  bdr_max_gc[8];
	U8  edr_max_gc[8];
    U8  ble_max_gc[8];

} MOD_GAIN_COMP_STRU;

typedef struct stru_bt_tx_pwr_cfg
{
    //U8  tx_default_gfsk_gc1;
    //U8  tx_default_gfsk_gc2;
    U8  tx_default_gfsk_bias2_1;
    U8  tx_default_gfsk_bias2_2;
    //U8  tx_default_edr_gc1;
    //U8  tx_default_edr_gc2;
    U8  tx_default_edr_bias2_1;
    U8  tx_default_edr_bias2_2;
    U8  tx_default_ble_gc1;
    U8  tx_default_ble_gc2;
    U8  tx_default_ble_bias2_1;
    U8  tx_default_ble_bias2_2;
    //U8  tx_gfsk_max_gain;
    U8  tx_gfsk_min_gain;
    //U8  tx_edr_max_gain;
    U8  tx_edr_min_gain;
    U8  tx_ble_max_gain;
    U8  tx_ble_min_gain;
    U8  tx_2402_gc1_offset;
    U8  tx_gfsk_gain_step_threshold_hi;
    U8  tx_gfsk_gain_step_threshold_lo;
    U8  tx_gfsk_gain_step_1;
    U8  tx_gfsk_gain_step_2;
    U8  tx_gfsk_gain_step_3;
    U8  tx_edr_gain_step;

} BT_TX_PWR_CFG_STRU;

typedef struct stru_ble_rx_gc_cfg
{
    U8 gc_table[3];

} BLE_RX_GC_CFG_STRU;

typedef struct stru_rf_misc_ctl
{
    BLE_RX_GC_CFG_STRU      ble_rx_gc_cfg;

    BT_TX_PWR_CFG_STRU  	bt_tx_pwr_cfg;

    MOD_GAIN_COMP_STRU      mod_gain_comp_cfg;

    U8 rssi_gap;

    U8 le_rssi_gap;

    U8 le_tx_power_in_dBm;

} RF_MISC_CTL_STRU;

typedef struct stru_sector_rf_misc_ctl
{
    RF_MISC_CTL_STRU RfMiscCtl;

    CRC16 CRC;

} SECTOR_RF_MISC_CTL_STRU;


#endif /* _SECTOR_RF_MISC_H_ */

