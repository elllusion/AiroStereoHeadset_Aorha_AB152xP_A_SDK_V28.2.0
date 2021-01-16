#ifndef _DRV_ADC0_HAL_H_
#define _DRV_ADC0_HAL_H_
#ifndef _DRV_ADC0_C_
#error "This file shall only be included in drv_gpio.c."
#endif
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_adc0_hal.h is the file defines the hardware abstraction layer of
 * drv_adc0.c.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
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
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U16 HAL_ADC0_ReadViChg(VOID);
EXTERN U16 HAL_ADC0_ReadVBat(VOID);
EXTERN U16 HAL_ADC0_ReadVChg(VOID);
EXTERN U16 HAL_ADC0_ReadTemp(VOID);
EXTERN U16 HAL_ADC0_ReadVBatCal(VOID);
EXTERN U16 HAL_ADC0_ReadVin(VOID);

#endif /* _DRV_ADC0_HAL_H_ */


