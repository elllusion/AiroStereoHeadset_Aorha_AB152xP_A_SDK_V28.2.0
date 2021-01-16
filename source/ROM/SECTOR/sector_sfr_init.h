#ifndef _SECTOR_SFR_INIT_H_
#define _SECTOR_SFR_INIT_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sfr_init.h defines the initialization sequence to setup CSR.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define NUM_OF_SUPPORTED_SFR_INIT   150



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_sfr_reg_init
{
	U8	Addr;
	U8  Offset;
	U8	Value;

} SFR_REG_INIT_STRU;

typedef struct stru_sfr_init
{
    U8  NumOfSfrRegInit;
    SFR_REG_INIT_STRU SfrRegInit[NUM_OF_SUPPORTED_SFR_INIT];

} SFR_INIT_STRU;

typedef struct stru_sector_sfr_init
{
    SFR_INIT_STRU SfrInit;

    CRC16 CRC;

} SECTOR_SFR_INIT_STRU;


#endif /* _SECTOR_SFR_INIT_H_ */

