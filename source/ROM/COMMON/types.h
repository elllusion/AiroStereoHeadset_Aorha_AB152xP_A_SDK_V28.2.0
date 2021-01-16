#ifndef _TYPES_H_
#define _TYPES_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * cpu.h defines the CPU depended header file
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef union
{
    U16 value;
    struct
    {
        U8 B1;
        U8 B0;
    } in_byte;

} TWO_BYTE_UNION;

typedef union
{
    U32 value;
    struct
    {
        U8 B3;
        U8 B2;
        U8 B1;
        U8 B0;
    } in_byte;

} FOUR_BYTE_UNION;

#if 0
typedef struct stru_byte
{
    U8 Bit0 : 1;
    U8 Bit1 : 1;
    U8 Bit2 : 1;
    U8 Bit3 : 1;
    U8 Bit4 : 1;
    U8 Bit5 : 1;
    U8 Bit6 : 1;
    U8 Bit7 : 1;

} BYTE_STRU;
#endif

typedef union union_byte
{
    U8 value;
    BYTE_STRU field;

} BYTE_UNION;


#endif /* _TYPES_H_ */

