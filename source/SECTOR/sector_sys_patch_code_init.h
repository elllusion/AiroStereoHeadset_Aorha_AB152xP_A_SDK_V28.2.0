#ifndef _SECTOR_SYS_PATCH_CODE_INIT_H_
#define _SECTOR_SYS_PATCH_CODE_INIT_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sys_patch_code_init.h defines the patch code initialization.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"



#define MAX_NUM_OF_PATCH   32

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_patch_code_reg
{
    U8 CODE_PTR PatchCodeRegInit[MAX_NUM_OF_PATCH];

} PATCH_CODE_REG_STRU;

typedef struct stru_patch_map_bank
{
    U16 PatchBankSize;
    U8 XDATA_PTR pEdataAddr;
    U8 PatchCode[1];

} PATCH_MAP_BANK_STRU;


typedef struct stru_patch_code_init
{
    PATCH_CODE_REG_STRU PatchCodeReg;
    PATCH_MAP_BANK_STRU PatchMapBank;

} PATCH_CODE_INIT_STRU;

typedef struct stru_sector_patch_code_init
{
    PATCH_CODE_INIT_STRU PatchCodeInit;

    CRC16 CRC;

} SECTOR_PATCH_CODE_INIT_STRU;


#endif /* _SECTOR_SYS_PATCH_CODE_INIT_H_ */


