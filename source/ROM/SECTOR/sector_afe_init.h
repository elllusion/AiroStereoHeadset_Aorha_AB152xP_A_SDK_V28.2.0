#ifndef _SECTOR_AFE_INIT_H_
#define _SECTOR_AFE_INIT_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_afe_init.h defines the initialization sequence to setup AFE.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if (defined OPERATION_IN_RF_AB1520) || (defined OPERATION_IN_RF_AB1520S)
#define NUM_OF_SUPPORTED_AFE_INIT   170
#else
#define NUM_OF_SUPPORTED_AFE_INIT   150
#endif



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_afe_reg_init
{
    U8  Addr;
    U8  HiByte;
    U8  LoByte;

} AFE_REG_INIT_STRU;

typedef struct stru_afe_init
{
    U8  NumOfAfeRegInit;
    AFE_REG_INIT_STRU AfeRegInit[NUM_OF_SUPPORTED_AFE_INIT];

} AFE_INIT_STRU;

typedef struct stru_sector_afe_init
{
    AFE_INIT_STRU AfeInit;

    CRC16 CRC;

} SECTOR_AFE_INIT_STRU;


#endif /* _SECTOR_AFE_INIT_H_ */

