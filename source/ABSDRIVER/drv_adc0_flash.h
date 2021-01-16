#ifndef _DRV_ADC0_FLASH_H_
#define _DRV_ADC0_FLASH_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_adc0.flash.h is the file defines the driver of ADC0 access.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "drv_adc0.h"

////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define     ADC0_AIO4CHEn         BIT_MASK16(14)
#define     ADC0_AIO5CHEn         BIT_MASK16(15)

#ifdef AB1520SC
#define     ADC0_PIO4AIOEn        BIT_MASK16(4)
#define     ADC0_PIO5AIOEn        BIT_MASK16(5)
#else
#define     ADC0_PIO4AIOEn        BIT_MASK16(14)
#define     ADC0_PIO5AIOEn        BIT_MASK16(15)
#endif

#define     ADC0_Enable           BIT_MASK16(0)


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_adc0_avg_ctl
{
    U8 Depth;
    U8 Count;
    U8 Idx;
    ADC_VALUE AvgValue;
    ADC_VALUE Sum;  /* NOTE : Max availabe depth depends on the width of Sum and ADC bit numbers */
    ADC_VALUE EDATA_PTR ValueArray;

} ADC0_AVG_CTL_STRU, OS_PTR ADC0_AVG_CTL_STRU_PTR;


EXTERN ADC0_AVG_CTL_STRU ViChgAvgCtl;
EXTERN ADC0_AVG_CTL_STRU VBatAvgCtl;
EXTERN ADC0_AVG_CTL_STRU VChgAvgCtl;
EXTERN ADC0_AVG_CTL_STRU TempAvgCtl;
EXTERN ADC0_AVG_CTL_STRU VBatCalAvgCtl;
EXTERN ADC0_AVG_CTL_STRU VinAvgCtl;


////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_ADC0_Init
 *
 * This function is used to reinit the driver function entries for flash code.
 * By the way, it also init the average control data structure.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_ADC0_Init(VOID);

/**
 * DRV_ADC0_Update
 *
 * Update the ADC0 read-out values
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID DRV_ADC0_Update(VOID);

#ifdef AB1520S
/**
 * DRV_ADC0_AIO4_Init
 *
 * Initialize the ADC1_AIO4 hardware parameters
 *
 * @Author : YugoSu <YugoSu@airoha.com.tw>
 */
EXTERN VOID DRV_ADC0_AIO4_Init(VOID);


/**
 * DRV_ADC0_AIO5_Init
 *
 * Initialize the ADC1_AIO5 hardware parameters
 *
 * @Author : YugoSu <YugoSu@airoha.com.tw>
 */
EXTERN VOID DRV_ADC0_AIO5_Init(VOID);


/**
 * DRV_ADC0_ReadAIO4
 *
 * Read AIO4 ADC value
 *
 * @Author : YugoSu <YugoSu@airoha.com.tw>
 */
EXTERN U16 DRV_ADC0_ReadAIO4(VOID);


/**
 * DRV_ADC0_ReadAIO5
 *
 * Read AIO5 ADC value
 *
 * @Author : YugoSu <YugoSu@airoha.com.tw>
 */
EXTERN U16 DRV_ADC0_ReadAIO5(VOID);

#endif //-AB1520S

#endif /* _DRV_ADC0_FLASH_H_ */


