#ifndef _BT_TIMER_H_
#define _BT_TIMER_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * bt_timer.h defines the timer interface by Bluetooth Clock.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"
#include "bt_types.h"



////////////////////////////////////////////////////////////////////////////////
// BT Timer Interfaces /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * BT_TIMER_RdSysClk
 *
 * Get native system Bluetooth Clock
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Return : Native Bluetooth Clock
 */
EXTERN BTCLK BT_TIMER_RdSysClk(VOID);

/**
 * BT_TIMER_GetExpiredTime
 *
 * Initiate LC Timer
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Timeout : Timeout Value
 * @Return : Time to expire
 */
EXTERN BTCLK BT_TIMER_GetExpiredTime(
    IN      BTCLK Timeout);

/**
 * BT_TIMER_IsExpired
 *
 * Check whether the LC Timer is expired
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @ExpiredTime : Time to expire
 * @Return : TRUE stands for LC Timer has been expired
 */
EXTERN BOOL BT_TIMER_IsExpired(
    IN      BTCLK ExpiredTime);

#endif /* _BT_TIMER_H_ */

