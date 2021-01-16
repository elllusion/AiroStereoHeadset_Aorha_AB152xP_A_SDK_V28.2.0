#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_gpio.h is the file defines the driver of GPIO.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* GPIO ID Definitions */
typedef U8 GPIO_ID;
#define GPIO0       ((GPIO_ID)(0))
#define GPIO1       ((GPIO_ID)(1))
#define GPIO2       ((GPIO_ID)(2))
#define GPIO3       ((GPIO_ID)(3))
#define GPIO4       ((GPIO_ID)(4))
#define GPIO5       ((GPIO_ID)(5))
#define GPIO6       ((GPIO_ID)(6))
#define GPIO7       ((GPIO_ID)(7))
#define GPIO8       ((GPIO_ID)(8))
#define GPIO9       ((GPIO_ID)(9))
#define GPIO10      ((GPIO_ID)(10))
#define GPIO11      ((GPIO_ID)(11))
#define GPIO12      ((GPIO_ID)(12))
#define GPIO13      ((GPIO_ID)(13))
#define GPIO14      ((GPIO_ID)(14))
#define GPIO15      ((GPIO_ID)(15))
#define GPIO16      ((GPIO_ID)(16))
#define GPIO17      ((GPIO_ID)(17))
#define GPIO18      ((GPIO_ID)(18))
#define GPIO19      ((GPIO_ID)(19))
#define GPIO20      ((GPIO_ID)(20))
#define GPIO21      ((GPIO_ID)(21))
#define GPIO22      ((GPIO_ID)(22))
#define GPIO23      ((GPIO_ID)(23))
#define GPIO24      ((GPIO_ID)(24))
#define GPIO25      ((GPIO_ID)(25))
#define GPIO26      ((GPIO_ID)(26))
#define GPIO27      ((GPIO_ID)(27))
#define GPIO28      ((GPIO_ID)(28))
#define GPIO29      ((GPIO_ID)(29))
#define GPIO30      ((GPIO_ID)(30))



////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_GPIO_Init
 *
 * Init GPIO bank
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @GpioBank :
 */
VOID DRV_GPIO_Init(
	IN		U8 GpioBankId,
	IN		U8 InitVal);
	
VOID DRV_GPIO_OutputEnableInit(
	IN		U8 GpioBankId,
	IN		U8 InitVal);

#define OUTPUT_ENABLE                       1
#define OUTPUT_DISABLE                      0
VOID DRV_GPIO_SetIOStatus(U8 outputEnable, U8 outPut, U8 ioIndex);
/**
 * DRV_GPIO_OutputEnable
 *
 * Enable specific GPIO as output.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @GpioId : GPIO ID
 */
VOID DRV_GPIO_OutputEnable(
    IN      GPIO_ID GpioId);

/**
 * DRV_GPIO_OutputDisable
 *
 * Disable specific GPIO as output.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @GpioId : GPIO ID
 */
VOID DRV_GPIO_OutputDisable(
    IN      GPIO_ID GpioId);

/**
 * DRV_GPIO_Output
 *
 * Send specific GPIO output
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @GpioId : GPIO ID
 * @Val : Output value
 */
VOID DRV_GPIO_Output(
    IN      GPIO_ID GpioId,
    IN      U8      Val);

/**
 * DRV_GPIO_Input
 *
 * Read specific GPIO input
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @GpioId : GPIO ID
 */
U8 DRV_GPIO_Input(
    IN      GPIO_ID GpioId);

#endif /* _DRV_GPIO_H_ */

