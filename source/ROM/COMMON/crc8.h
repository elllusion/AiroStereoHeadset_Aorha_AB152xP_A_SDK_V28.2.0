#ifndef _CRC8_H_
#define _CRC8_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * crc8.H defined the function prototypes of CRC8
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
#include "config.h"
#include "cpu.h"
#include "os_types.h"


////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U8 CRC8_Generate(
	IN      U8 GENERIC_PTR ptr,
	IN      U16 length,
	IN      U8 crc_init);

#endif /* _CRC8_H_ */

