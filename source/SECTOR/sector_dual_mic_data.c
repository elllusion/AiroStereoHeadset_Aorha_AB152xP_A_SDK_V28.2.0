/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_dual_mic_data.c defines the data calculated by runtime.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"
#include "sector_dual_mic_data.h"

//#ifdef DUAL_MIC_ENABLE

#pragma userclass (HCONST = DUAL_MIC)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#ifdef DSP_ENABLE
SECTOR_DUAL_MIC_DATA_STRU CODE gSector_DualMicData ALIGN(2)=
{
    {
        0
    },

    //- CRC
    {0,0}
};
//#endif

//#endif // DUAL_MIC_ENABLE
