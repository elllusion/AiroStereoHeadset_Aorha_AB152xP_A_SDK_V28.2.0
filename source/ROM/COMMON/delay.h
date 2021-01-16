#ifndef _DELAY_H_
#define _DELAY_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * bt_clock.c defines the clock interface by Bluetooth Clock.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_delay_us
 *
 * delay by SPIN Lock
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @us : delay time in us
 */
VOID OS_delay_us (
    IN  U16     us);

/**
 * OS_delay_ms
 *
 * delay by SPIN Lock
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @us : delay time in ms
 */
VOID OS_delay_ms (
    IN  U16     ms);

#endif /* _DELAY_H_ */

