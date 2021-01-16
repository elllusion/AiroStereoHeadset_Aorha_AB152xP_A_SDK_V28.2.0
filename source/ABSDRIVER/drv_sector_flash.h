#ifndef _DRV_SECTOR_FLASH_H_
#define _DRV_SECTOR_FLASH_H_
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
#include "sector_config0_flash.h"
#include "sector_config1.h"
#include "sector_boundary.h"



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SYS_LOCAL_DEVICE_CONTROL_STRU CODE_PTR XDATA pSysLocalDeviceControl;
EXTERN SYS_LOCAL_DEVICE_INFO_STRU XDATA_PTR XDATA pSysLocalDeviceInfo;
EXTERN RF_MISC_CTL_STRU XDATA_PTR XDATA pRfMiscCtl;
EXTERN HW_MISC_CTL_STRU XDATA_PTR XDATA pHwMiscCtl;
EXTERN LM_PARAMETER_STRU CODE_PTR XDATA pLmParameter;
EXTERN MP_PARAMETER_F_STRU CODE_PTR XDATA pMpParameterF;



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SYS_LOCAL_DEVICE_INFO_STRU XDATA gSysLocalDeviceInfo;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_SECTOR_Init_FLASH
 *
 * Init Sectors
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_SECTOR_Init_FLASH(VOID);

#endif /* _DRV_SECTOR_FLASH_H_ */

