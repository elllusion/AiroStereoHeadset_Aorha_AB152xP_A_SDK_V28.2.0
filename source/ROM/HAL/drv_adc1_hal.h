#ifndef _DRV_ADC1_HAL_H_
#define _DRV_ADC1_HAL_H_
#ifndef _DRV_ADC1_C_
#error "This file shall only be included in drv_adc1.c."
#endif
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_adc1_hal.c is the file defines the hardware abstraction layer of
 * drv_adc1.c.
 *
 * Programmer : ChiaHoHu@airoha.com.tw, Ext.3369
 */
#include "config.h"
#include "cpu.h"
#include "hw.h"



////////////////////////////////////////////////////////////////////////////////
// Version Control /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if !(CHIP_MODEL == 1520)
#error "Unsuitable Driver"
#endif



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Hardware Abstraction Layer //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_ADC1_SetAdcEnable
 *
 * Set ADC1 Enable
 *
 * @Author : ChiaHoHu <ChiaHoHu@airoha.com.tw>
 *
 */
EXTERN VOID DRV_ADC1_SetAdcEnable(U8 ch);

/**
/**
 * DRV_ADC1_SetOp1Sel
 *
 * Set ADC1 OP1 select
 *
 * @Author : ChiaHoHu <ChiaHoHu@airoha.com.tw>
 *
 */
EXTERN VOID DRV_ADC1_SetOp1Sel(U8 op);

/**
 * DRV_ADC1_SetOp1Sel
 *
 * Set ADC1 Average select
 *
 * @Author : ChiaHoHu <ChiaHoHu@airoha.com.tw>
 *
 */
EXTERN VOID DRV_ADC1_AverageSel(U8 op);

/**
 * DRV_ADC1_SetChannelEnable
 *
 * Set ADC1 Channel Enable bit mask.
 *
 * @Author : ChiaHoHu <ChiaHoHu@airoha.com.tw>
 *
 */
EXTERN VOID DRV_ADC1_SetChannelEnable(U32 ch_en);

/**
 * DRV_ADC1_StartWaitBusy
 *
 * Start to sweep ADC1 channel data and wait ADC1 control busy.
 *
 * @Author : ChiaHoHu <ChiaHoHu@airoha.com.tw>
 *
 */
EXTERN VOID DRV_ADC1_StartWaitBusy(VOID);

/**
 * DRV_ADC1_ReadChannelData
 *
 * Read ADC1 Channel Data.
 *
 * @Author : ChiaHoHu <ChiaHoHu@airoha.com.tw>
 *
 */
EXTERN U16 DRV_ADC1_ReadChannelData(U8 ch);

#endif /* _DRV_ADC1_HAL_H_ */

