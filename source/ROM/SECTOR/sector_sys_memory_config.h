#ifndef _SECTOR_SYS_MEMORY_CONFIG_H_
#define _SECTOR_SYS_MEMORY_CONFIG_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sys_memory_config.c defines the system memory configurations.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_sys_mem_config
{
    U8 CallArrayEntries;
    U8 CallArrayEntrySize;
    U8 TimerArrayEntries;
    U8 TimerArrayEntrySize;
    U8 OSMEM1ArrayEntries;
    U8 OSMEM1ArrayEntrySize;
    U8 OSMEM2SmallTxEntries;
    U16 OSMEM2SmallTxEntrySize;
    U8 OSMEM2TinyRxEntries;
    U16 OSMEM2TinyRxEntrySize;
    U8 OSMEM2SmallRxEntries;
    U16 OSMEM2SmallRxEntrySize;
    U8 OSMEM2LargeRxEntries;
    U16 OSMEM2LargeRxEntrySize;
    //-
    U16 UartDmaRxBufSize;
    U8 OSMEM2LargeTxEntries;
    U16 OSMEM2LargeTxEntrySize;
} SYS_MEMORY_CONFIG_STRU;

typedef struct stru_sector_sys_memory_config
{
    SYS_MEMORY_CONFIG_STRU SysMemoryConfig;

    CRC16 CRC;

} SECTOR_SYS_MEM_CONF_STRU;


#endif /* _SECTOR_SYS_MEMORY_CONFIG_H_ */

