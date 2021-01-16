#ifndef _SECTOR_HW_MISC_CTL_H_
#define _SECTOR_HW_MISC_CTL_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_hw_misc_ctl.h defines the HW parameters.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_adc0_cfg
{
    U16 update_period;
    U8 ViChg_avg_depth;
    U8 VBat_avg_depth;
    U8 VChg_avg_depth;
    U8 Temp_avg_depth;
    U8 VBatCal_avg_depth;
    U8 Vin_avg_depth;

} ADC0_CONFIG_STRU;

typedef struct stru_hw_misc_ctl
{
    S8 RSSI_GoldenRange_UpperBound;
    S8 RSSI_GoldenRange_LowerBound;
    U8 normal_clock_drift;
    U8 slow_clock_drift;

    //- ADC0 Related
    ADC0_CONFIG_STRU adc0_cfg;   // in ms. This vaiable is better same as SFR setting
    BOOL temp_compensation;

} HW_MISC_CTL_STRU;

typedef struct
{
    HW_MISC_CTL_STRU HwMiscCtl;

    CRC16 CRC;

} SECTOR_HW_MISC_CTL_STRU;


#endif /* _SECTOR_HW_MISC_CTL_H_ */

