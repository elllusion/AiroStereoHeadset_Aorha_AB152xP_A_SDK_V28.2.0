#ifndef _MMI_LE_ROM_C_
    #error "only MMI_LE_ROM.c should include this header file "
#endif

#include "MMI_LE_ROM_LED.h"

#ifdef DFU_LOADER_IN_BOOT_CODE
EXTERN LED_SETTING_STRU CODE LED_LOADER_Mode;
#else
LED_SETTING_STRU CODE LED_LOADER_Mode =
{
    0,						//LED t1t2_repeat
    160,					//LED unit
    1,						//LED t0
    30,						//LED t1
    20,						//LED t2
    10,						//LED t3
    10,						//LED pwm_OnStep
    10,						//LED pwm_OffStep
    0x0F,					//LED pwm_DurationSet
};
#endif

LED_SETTING_STRU CODE LED_AU_Mode =
{
    0,						//LED t1t2_repeat
    160,					//LED unit
    1,						//LED t0
    5,						//LED t1
    5,						//LED t2
    1,						//LED t3
    0,						//LED pwm_OnStep
    0,						//LED pwm_OffStep
    0x0F,					//LED pwm_DurationSet
};

LED_SETTING_STRU CODE LED_Low_Battery =
{
    1,						//LED t1t2_repeat
    160,					//LED unit
    1,						//LED t0
    8,						//LED t1
    3,						//LED t2
    60,						//LED t3
    0,						//LED pwm_OnStep
    0,						//LED pwm_OffStep
    0x0F,					//LED pwm_DurationSet
};

LED_SETTING_STRU CODE LED_EngineeringMode =
{
    1,						//LED t1t2_repeat
    160,					//LED unit
    1,						//LED t0
    1,						//LED t1
    0,						//LED t2
    0,						//LED t3
    0,						//LED pwm_OnStep
    0,						//LED pwm_OffStep
    0x0F,					//LED pwm_DurationSet
};