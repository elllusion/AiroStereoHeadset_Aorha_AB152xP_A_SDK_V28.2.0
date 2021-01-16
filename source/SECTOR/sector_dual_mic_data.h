#ifndef _SECTOR_DUAL_MIC_DATA_H_
#define _SECTOR_DUAL_MIC_DATA_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_dual_mic_data.h defines the data calculated by runtime.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define MAX_NUM_OF_DATA   300



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_dual_mic_data
{
    U8 DualMicData[MAX_NUM_OF_DATA];

} DUAL_MIC_DATA_STRU;

typedef struct stru_sector_dual_mic_data
{
    DUAL_MIC_DATA_STRU DualMic;

    CRC16 CRC;

} SECTOR_DUAL_MIC_DATA_STRU;


#endif /* _SECTOR_DUAL_MIC_DATA_H_ */


