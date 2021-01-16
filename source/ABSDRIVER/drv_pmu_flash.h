#ifndef _DRV_PMU_FLASH_H_
#define _DRV_PMU_FLASH_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_pmu.h is the file defines the interface of PMU register access
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "drv_pmu.h"




////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * FLASH_DRV_PMU_Init
 *
 * Reinit PMU driver for flash
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID FLASH_DRV_PMU_Init(VOID);

#endif /* _DRV_PMU_FLASH_H_ */

