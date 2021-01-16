#ifndef _OS_TIMER_H_
#define _OS_TIMER_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * OS_timer.h is the file defines the OS timer functions
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"
#include "bt_types.h"
#include "os_types.h"



////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    BTCLK_UNION ExpiredTime;

} OST, OS_PTR OST_PTR;



////////////////////////////////////////////////////////////////////////////////
// OST FUNCTION DECLARATIONS ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * OS_TIMER_Init
 *
 * Initialization
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
VOID OS_TIMER_Init(VOID);

/**
 * OS_RdSysClk
 *
 * Get Bluetooth native clock
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Return : Bluetooth native clock
 */
EXTERN U32 OS_RdSysClk(VOID);

/**
 * OST_GetTimer
 *
 * Get OS Timer
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @Return : Pointer of OS Timer
 */
EXTERN OST_PTR OST_GetTimer(VOID);

/**
 * OST_PutTimer
 *
 * Release OS Timer
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pOst : Pointer of OS timer
 */
EXTERN VOID OST_PutTimer(
    IN      OST_PTR pOst);

/**
 * OST_SetTimer
 *
 * Do OS timer initialization
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pOst : Pointer of OS timer
 * @time_in_half_slots : Expected timeout value.
 */
EXTERN VOID OST_SetTimer(
    IN      OST_PTR pOst,
    IN      BTCLK time_in_half_slots);

/**
 * OST_TimerExpired
 *
 * Check whether the specified timer is expired
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @pOst : Pointer of OS timer
 * @Return : Boolean value.
 */
EXTERN BOOL OST_TimerExpired(
    IN      OST_PTR pOst);

/**
 * OS_delay_us
 *
 * Use timer 0 to implement the delay function. This delay function is protected
 * by semaphore.
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @us : delay time in us (1~16383)
 */
EXTERN VOID OS_delay_us(
    IN      U16 us);

/**
 * OS_delay_ms
 *
 * Use timer 0 to implement the delay function. This delay function is protected
 * by semaphore.
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 *
 * @us : delay time in us (1~65535)
 */
EXTERN VOID OS_delay_ms(
    IN      U16 ms);

#endif /* _OS_TIMER_H_ */

