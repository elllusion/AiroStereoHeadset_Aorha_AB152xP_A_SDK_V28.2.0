#ifndef _HC_INTR_H_
#define _HC_INTR_H_
/* 
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * HC_intr.h is the file defines Interrupt 1 interface
 * 
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */



#define INT1_ENABLE_SHOW_PMU_STATUS		0x00
#define INT1_ENABLE_SHOW_NUM_INT_IN_Q	0x01

//- FE 84 [7:4]
//- read vector
#define INT1_SOURCE_MASK				0xF0
#define INT1_SOURCE_SPI					0x00
#define INT1_SOURCE_UART_USB			0x10
#define INT1_SOURCE_I2C					0x20
#define INT1_SOURCE_MMI1				0x30
#define INT1_SOURCE_MMI2				0x40
#define INT1_SOURCE_SNIFF_IN			0x50
#define INT1_SOURCE_PMU					0x60
#define INT1_SOURCE_DSP_INT0			0x70
#define INT1_SOURCE_DSP_INT1			0x80
#define INT1_SOURCE_DSP_INT2			0x90


//- interrupt1 enable mask
#define INT1_ENABLE_SPI					0x01
#define INT1_ENABLE_UART				0x02
#define INT1_ENABLE_USB					0x04
#define INT1_ENABLE_I2C					0x08
#define INT1_ENABLE_MMI					0x10
#define INT1_ENABLE_SNIFFIN				0x40
#define INT1_ENABLE_PMU					0x40
#define INT1_ENABLE_DSP					0x80

//- FE 84 [3:0]
#define INT1_SOURCE_PMU_MASK				0x0F
#define INT1_SOURCE_PMU_GPIO0				0x00
#define INT1_SOURCE_PMU_GPIO1				0x01
#define INT1_SOURCE_PMU_GPIO2				0x02
#define INT1_SOURCE_PMU_GPIO3				0x03
#define INT1_SOURCE_PMU_LOW_BATTERY			0x04
#define INT1_SOURCE_PMU_FULL_BATTERY		0x05
#define INT1_SOURCE_PMU_CHARGE_COMPLETE		0x06
#define INT1_SOURCE_PMU_RECHARGE			0x07
#define INT1_SOURCE_PMU_CHARGE_OFF			0x08
#define INT1_SOURCE_PMU_CHARGE_ON			0x09



#endif /* _HC_INTR_H_ */

