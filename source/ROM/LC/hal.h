#ifndef _HAL_H_
#define _HAL_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * hal.h defines the hardware abstraction layer
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "drivers.h"



//#warning "try to remove"
////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//-
#define HAL_ReadSwClock()       DRV_BTCLK_ReadSwClock()
#define HAL_ReadMasterClock()   DRV_BTCLK_ReadMasterClock()
#define HAL_ReadSlave0Clock()   DRV_BTCLK_ReadSlave0Clock()
#define HAL_ReadSlave1Clock()   DRV_BTCLK_ReadSlave1Clock()
//-
#define HAL_WriteSwClock(c)     DRV_BTCLK_WriteSwClock(c)
#define HAL_WriteMasterClock(c) DRV_BTCLK_WriteMasterClock(c)
#define HAL_WriteSlave0Clock(c) DRV_BTCLK_WriteSlave0Clock(c)
#define HAL_WriteSlave1Clock(c) DRV_BTCLK_WriteSlave1Clock(c)

#endif /* _HAL_H_ */

