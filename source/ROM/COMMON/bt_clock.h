#ifndef _BT_CLOCK_H_
#define _BT_CLOCK_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * bt_clock.c defines the clock interface by Bluetooth Clock.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"
#include "bt_types.h"



////////////////////////////////////////////////////////////////////////////////
// Constant ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define BTCLK_HALF_SEC  (1600L)
#define BTCLK_ONE_SEC   (3200L)
#define BTCLK_HALF_MIN  (30 * BTCLK_ONE_SEC)
#define BTCLK_ONE_MIN   (60 * BTCLK_ONE_SEC)



////////////////////////////////////////////////////////////////////////////////
// BT Timer Interfaces /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * BT_CLOCK_IsGE
 *
 * Greater than and equal
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @ClkA : Operand A
 * @ClkB : Operand B
 * @Return : TRUE or FALSE
 */
BOOL BT_CLOCK_IsGE(
    IN      BTCLK ClkA,
    IN      BTCLK ClkB);

/**
 * BT_CLOCK_Add
 *
 * Adder of Bluetooth clock
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @ClkA : Operand A
 * @ClkB : Operand B
 * @Return : Bluetooth clock
 */
BTCLK BT_CLOCK_Add(
    IN      BTCLK ClkA,
    IN      BTCLK ClkB);

/**
 * BT_CLOCK_Sub
 *
 * subtractor of Bluetooth clock
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @ClkA : Operand A
 * @ClkB : Operand B
 * @Return : Bluetooth clock
 */
BTCLK BT_CLOCK_Sub(
    IN      BTCLK ClkA,
    IN      BTCLK ClkB);

#endif /* _BT_CLOCK_H_ */

