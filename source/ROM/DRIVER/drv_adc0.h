#ifndef _DRV_ADC0_H_
#define _DRV_ADC0_H_
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
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- 10Bit ADC0 output value
typedef U16 ADC_VALUE;
//-
typedef struct stru_drv_adc0
{
    ADC_VALUE (*fViChg)(VOID);
    ADC_VALUE (*fVBat)(VOID);
    ADC_VALUE (*fVChg)(VOID);
    ADC_VALUE (*fTemp)(VOID);
    ADC_VALUE (*fVBatCal)(VOID);
    ADC_VALUE (*fVin)(VOID);

} DRV_ADC0_STRU;



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN DRV_ADC0_STRU DRV_ADC0;

#endif /* _DRV_ADC0_H_ */

