#ifndef _DRV_SECTOR_H_
#define _DRV_SECTOR_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_sector.h is the file defines the function about sector access
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "sector_sys_memory_config.h"
#include "sector_uart_parameter.h"
#include "sector_afe_init.h"
#include "sector_sfr_init.h"
#include "sector_rf_misc_ctl.h"
#include "sector_hw_misc_ctl.h"
#include "sector_sys_local_device_info.h"
#include "sector_hc_parameter.h"
#include "sector_mp_parameter.h"



////////////////////////////////////////////////////////////////////////////////
// Constant ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*Use -1 in 2's complement as CRC initial value*/
#define CRC16_INIT_VALUE            0xFFFF



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SYS_MEMORY_CONFIG_STRU CODE_PTR XDATA pSysMemoryConfig;
EXTERN UART_PARAMETER_STRU CODE_PTR XDATA pUartParameter;
EXTERN RF_MISC_CTL_STRU XDATA_PTR XDATA pRfMiscCtl;
EXTERN HW_MISC_CTL_STRU XDATA_PTR XDATA pHwMiscCtl;
EXTERN SYS_LOCAL_DEVICE_INFO_STRU XDATA_PTR XDATA pSysLocalDeviceInfo;
EXTERN HC_PARAMETER_TYPE XDATA gHC_parameter;
EXTERN MP_PARAMETER_STRU CODE_PTR XDATA pMpParameter;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_SECTOR_Init_InEngMode
 *
 * Init Sectors
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_SECTOR_Init_InEngMode(VOID);

//warning: Charles
#define OSMEM1_BLKSIZE          pSysMemoryConfig->OSMEM1ArrayEntrySize
#define OSMEM2_SMALL_TX_NBLKS   pSysMemoryConfig->OSMEM2SmallTxEntries
#define OSMEM2_SMALL_TX_BLKSIZE pSysMemoryConfig->OSMEM2SmallTxEntrySize
#define OSMEM2_TINY_RX_NBLKS    pSysMemoryConfig->OSMEM2TinyRxEntries
#define OSMEM2_TINY_RX_BLKSIZE  pSysMemoryConfig->OSMEM2TinyRxEntrySize
#define OSMEM2_SMALL_RX_NBLKS   pSysMemoryConfig->OSMEM2SmallRxEntries
#define OSMEM2_SMALL_RX_BLKSIZE pSysMemoryConfig->OSMEM2SmallRxEntrySize
#define OSMEM2_LARGE_RX_NBLKS   pSysMemoryConfig->OSMEM2LargeRxEntries
#define OSMEM2_LARGE_RX_BLKSIZE pSysMemoryConfig->OSMEM2LargeRxEntrySize
#define OSMEM2_LARGE_TX_NBLKS   pSysMemoryConfig->OSMEM2LargeTxEntries
#define OSMEM2_LARGE_TX_BLKSIZE pSysMemoryConfig->OSMEM2LargeTxEntrySize


/**
 * gSysMemoryConfig_InAirMode
 *
 * Init Sectors
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_SECTOR_Init_InAirMode(VOID);
EXTERN BOOL IsFlashDescriptorValid(U8 GENERIC_PTR pFLashDescriptor,U16 length);
EXTERN BOOL IsSectorHdrValid(U8 CODE_PTR pSectorHdr, U16 size);
EXTERN BOOL IsSectorMemberValid(U8 CODE_PTR pSectorAddr, U32 SectorSize);

#endif /* _DRV_SECTOR_H_ */

