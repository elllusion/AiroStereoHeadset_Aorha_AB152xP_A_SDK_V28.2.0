#ifndef _DRV_BTCLK_H_
#define _DRV_BTCLK_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_adc0.h is the file defines the driver of ADC0 access.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * HAL_ReadSwClock
 * HAL_ReadMasterClock
 * HAL_ReadSlave0Clock
 * HAL_ReadSlave1Clock
 *
 * Get CLKN/0/1/2/3
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Return : Bluetooth Clock 0/1/2/3
 */
EXTERN U32 DRV_BTCLK_ReadClock(
    IN      U8 Offset);

#define DRV_BTCLK_ReadSwClock()       DRV_BTCLK_ReadClock(C2OFFS_RW_CLK0_B0)
#define DRV_BTCLK_ReadMasterClock()   DRV_BTCLK_ReadClock(C2OFFS_RW_CLK1_B0)
#define DRV_BTCLK_ReadSlave0Clock()   DRV_BTCLK_ReadClock(C2OFFS_RW_CLK2_B0)
#define DRV_BTCLK_ReadSlave1Clock()   DRV_BTCLK_ReadClock(C2OFFS_RW_CLK3_B0)



/**
 * HAL_WriteSwClock
 * HAL_WriteMasterClock
 * HAL_WriteSlave0Clock
 * HAL_WriteSlave1Clock
 *
 * Write CLK0/1/2/3
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_BTCLK_WriteClock(
    IN      U8 Offset,
    IN      U32 Clk);

#define DRV_BTCLK_WriteSwClock(c)     DRV_BTCLK_WriteClock(C2OFFS_RW_CLK0_B0,(c))
#define DRV_BTCLK_WriteMasterClock(c) DRV_BTCLK_WriteClock(C2OFFS_RW_CLK1_B0,(c))
#define DRV_BTCLK_WriteSlave0Clock(c) DRV_BTCLK_WriteClock(C2OFFS_RW_CLK2_B0,(c))
#define DRV_BTCLK_WriteSlave1Clock(c) DRV_BTCLK_WriteClock(C2OFFS_RW_CLK3_B0,(c))

#endif /* _DRV_BTCLK_H_ */

