/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_merge_runtime_header.c defines the runtime updated parameters.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
 // -
#include "config.h"
#include "cpu.h"
#include "sectors.h"
#include "sector_merge_runtime.h"

#pragma userclass (HCONST = CFGHEAD)


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	{(U8 CODE_PTR)SECTOR_INIT, SECTOR_SIZE, ATTR},



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_MERGE_RUNTIME_HEADER_1_STRU CODE gSectorRuntimeHeader_1 =
{
    NUM_OF_SECTOR_MERGE_RUNTIME_1,

    {
        #include "sector_merge_runtime_member_1.h"
    },

    //- CRC
    {0,0}
};



