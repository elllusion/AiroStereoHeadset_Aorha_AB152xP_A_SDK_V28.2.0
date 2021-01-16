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
#include "bt_config_profile.h"
#include "sector_sys_memory_config.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_SYS_MEM_CONF_STRU CODE gSector_SysMemoryConfig =
{
    {
/* CallArrayEntries */          16,
/* CallArrayEntrySize */        4,
/* TimerArrayEntries */         50,
/* TimerArrayEntrySize */       4,
#if defined SCO_WOOFER || defined SOUND_BAR || defined TWS_SETTINGS
#ifdef APP_ANCS_C
/* OSMEM1ArrayEntries */         45,
#else
/* OSMEM1ArrayEntries */         45,
#endif
#else
/* OSMEM1ArrayEntries */         45,
#endif
/* OSMEM1ArrayEntrySize */      50,


#if defined FAST_CONNECT_DONGLE
/* OSMEM2SmallTxEntries */      9,
/* OSMEM2SmallTxEntrySize */    702,
#else
#if defined TWS_SETTINGS
#if defined AB1520D
/* OSMEM2SmallTxEntries */      7,
#else
/* OSMEM2SmallTxEntries */      4,
#endif
#else
/* OSMEM2SmallTxEntries */      8,
#endif
/* OSMEM2SmallTxEntrySize */    390,
#endif


/* OSMEM2TinyRxEntries */       3,
/* OSMEM2TinyRxEntrySize */     62,

#if defined TWS_SETTINGS || defined FAST_CONNECT_DONGLE
#if defined AB1520D
/* OSMEM2SmallRxEntries */      4,
#else
/* OSMEM2SmallRxEntries */      3,
#endif
#else
/* OSMEM2SmallRxEntries */      6,
#endif

#if 0
#if 0//SUPPORT_AAC_SNK
/* OSMEM2SmallRxEntrySize */    1100,	//For AAC of Lenovo K3 NOTE
#else
/* OSMEM2SmallRxEntrySize */    1100,//702,
#endif
#endif

#if defined FAST_CONNECT_DONGLE
/* OSMEM2SmallRxEntrySize */    702,
#else
#if defined AB1520D
/* OSMEM2SmallRxEntrySize */    1050,
#else
/* OSMEM2SmallRxEntrySize */    702,
#endif
#endif


/* OSMEM2LargeRxEntries */      2,
/* OSMEM2LargeRxEntrySize */    1100,

#ifdef AIR_UPDATE_DONGLE
/* UartDmaRxBufSize */          1400,
#else
/* UartDmaRxBufSize */          380,
#endif

/* new add large TX */
#if defined TWS_SETTINGS
#if defined AB1520D
/* OSMEM2LargeTxEntries */      8,
/* OSMEM2LargeTxEntrySize */    1050,
#else
/* OSMEM2LargeTxEntries */      8,
/* OSMEM2LargeTxEntrySize */    702,
#endif

#else
/* OSMEM2LargeTxEntries */      0,
/* OSMEM2LargeTxEntrySize */    390,
#endif


    },

    //- CRC
    {0,0}
};

