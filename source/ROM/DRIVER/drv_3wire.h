#ifndef _DRV_3WIRE_H_
#define _DRV_3WIRE_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * drv_3wire.h is the file defines the driver function of 3-wire
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * DRV_3WIRE_Write
 *
 * This function is used to write analog front end register by 3-wire SPI
 * interface. And this function handle the interface wrapper hardware is
 * busy, then wait the hardware is ready to write next register.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Addr : Register address
 * @Data : Register value
 */
EXTERN VOID DRV_3WIRE_Write(
    IN  U8      Addr,
    IN  U16     Data);

/**
 * DRV_3WIRE_Read
 *
 * This function is used to read analog front end register by 3-wire SPI
 * interface. And this function handle the interface wrapper hardware is
 * busy, then wait the hardware is ready to read next register.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @Addr : Register address
 * @Return : Read value
 */
EXTERN U16 DRV_3WIRE_Read(
    IN  U8      Addr);

#endif /* _DRV_3WIRE_H_ */

