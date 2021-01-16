#ifndef _CRC16_H_
#define _CRC16_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * crc16.c defined the function prototypes of CRC16
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "os_types.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definition /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 CRC16[2];



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * CRC16_Generate
 *
 * Generate CRC16 with specific CRC init.
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @ptr : The start pointer of data stream
 * @length : The length of data stream
 * @crc_init : The CRC init
 * @Return : 16-bits CRC result
 */
EXTERN U16 CRC16_Generate(
    IN      U8 GENERIC_PTR ptr,
    IN      U16 length,
    IN      U16 crc_init);

EXTERN U16 CRC16_Generate_LongLenth(
    IN      U8 GENERIC_PTR ptr,
    IN      U32 length,
    IN      U16 crc_init);

EXTERN U16 CRC16_Generate_Spi(
    IN      U8 GENERIC_PTR ptr,
    IN      U16 length,
    IN      U16 crc_init);

#endif /* _CRC16_H_ */

