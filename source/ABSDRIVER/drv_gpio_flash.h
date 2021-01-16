#ifndef _DRV_GPIO_FLASH_H_
#define _DRV_GPIO_FLASH_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2016 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_gpio_flash.h is the file defines the driver of GPIO.
 *
 */
#include "config.h"
#include "cpu.h"
#include "drv_gpio.h"

#define GRP_SEL_CHECK		0x01
#define SIG_SEL_CHECK		0x02
#define LED_SCL_CHECK		0x04
#define DMICI_0_CHECK		0x08
#define DMICI_1_CHECK		0x10
#define I2SIN_0_CHECK		0x20
#define I2SIN_1_CHECK		0x40
#define UART_NC_CHECK		0x80

#define PIO_INPUT_TYPE		1
#define PIO_OUTPUT_TYPE		2

#define CLK_SLOW 5
#define CLK_FAST 6

VOID DRV_GPIO_F_SetIOStatus(U8 outputEnable, U8 outPut, U8 ioIndex);
U8 	 DRV_GPIO_F_GetIOStatus(U8 ioIndex);
VOID DRV_GPIO_F_SetSigSel(U8 ioIndex, U8 val);
VOID DRV_GPIO_F_SetDMIC(U8 ioIndex, U8 grp);

#endif /* _DRV_GPIO_FLASH_H_ */