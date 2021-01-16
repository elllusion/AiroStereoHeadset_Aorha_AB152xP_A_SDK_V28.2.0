#include "reside_flash.inc"

#include "rc.h"
#include "drv_led.h"
#include "..\Driver\Driver_LED.h"

/*
 * drv_led.h and Driver_LED.h provide all the led functions
 * <SDK LED API>
 * 1. Hw level control APIs are in drv_led.h
 * 2. Foreground, background and filter led APIs are in Driver_LED.h 
 */


/* Feature manual
 * 1. MMI_QUEUE_LED_FEAT 										--> source\Driver\Driver_LED.h, DRIVER_Cmd_LED_FGLed
 * 2. MMI_MEDIA_LED_WITH_FILTER_EFFECT_FEAT 					--> source\Driver\Driver_LED.h, DRIVER_Cmd_LED_FGLed
 * 3. MMI_LED_PWM_COMPENSATE_IN_DIFFERENT_BATTERY_LEVEL			--> if MMI_SHOW_CONDISCABLE_LED_ONLY_ONE_CONNECTED_DEVICE_FEAT is on, device would adjust led brightness by itself according to battery level
 * 4. MMI_IGNORE_KEYEVENT_JUST_ENABLE_LED_FEAT					--> if MMI_SHOW_CONDISCABLE_LED_ONLY_ONE_CONNECTED_DEVICE_FEAT is on, KEY_BUTTONLOCKTOGGLE resumes BG LED under led power saving mode
 * 5. MMI_SHOW_CONDISCABLE_LED_ONLY_ONE_CONNECTED_DEVICE_FEAT	--> if MMI_SHOW_CONDISCABLE_LED_ONLY_ONE_CONNECTED_DEVICE_FEAT, condiscoverable led is displayed when only one connection.
 * 6. MMI_SDK_DISABLE_LED_FILTER_FEAT							--> Disbale all led filter control in NDK
 * 7. MMI_SDK_DISABLE_BACKGROUND_LED_FEAT						--> Disbale all background led control in NDK
 * 8. MMI_SDK_DISABLE_FOREGROUND_LED_FEAT						--> Disbale all foreground led control in NDK
 */

 
/*  
 * Hardware related APIs 
 */
void SDK_LED_HW_SetPWM(U8 pwm)
{
	LED_SetPwm(pwm);
}

void SDK_LED_HW_SetPattern(LedPatternType XDATA_PTR ledCtlPtr)
{
	LED_SetPatternToHW(ledCtlPtr);
}

void SDK_LED_HW_TurnOffAllLeds(void)
{
	LED_TurnOffAllLeds();
}

/* 
 * software related APIs
 */
PUBLIC BOOL SDK_LED_DRV_LoadPattern(U8 ledIndex, LedPatternType XDATA_PTR destPtr)
{
	return DRIVER_LED_LoadPattern(ledIndex, (U8 XDATA_PTR)destPtr);
}

PUBLIC void SDK_LED_DRV_Cmd_Restart(U8 para)
{
	DRIVER_Cmd_LED_Restart(para);
}

PUBLIC void SDK_LED_DRV_Cmd_FGLed(U8 ledDataIndex, U8 timeout, U8 id, U8 isOverrideFilter)
{
	DRIVER_Cmd_LED_FGLed(ledDataIndex, timeout, id, isOverrideFilter);
}

PUBLIC void SDK_LED_DRV_Cmd_BGLed(U8 ledDataIndex)
{
	DRIVER_Cmd_LED_BGLed(ledDataIndex);
}

PUBLIC void SDK_LED_DRV_Cmd_FilterLed(U8 type, U8 para)
{
	DRIVER_Cmd_LED_FilterLed(type, para);
}
 
/* 
 * SDK_LED_SetHWSample: sample code to set led HW
 */
PUBLIC void SDK_LED_SetHWSample(void)
{
	LedPatternType XDATA_PTR LedCtrlPtr;
	
	LedCtrlPtr = SYS_MemoryGet(sizeof(LedPatternType));
	
	if(!LedCtrlPtr)
		ASSERT(FALSE);
	
	OSMEM_memset_xdata((U8 XDATA_PTR)LedCtrlPtr, 0, sizeof(LedPatternType));

	//RED
	LedCtrlPtr->ledSettings[0].onOff  = LED_ON;
	LedCtrlPtr->ledSettings[0].ledFollowMultiplierControl = 0x01;
	LedCtrlPtr->ledSettings[0].pwmSetting = 0xF0;
	LedCtrlPtr->ledSettings[0].t1 = 50;	//1s, unit:20ms
	LedCtrlPtr->ledSettings[0].t2 = 50;	//1s, unit:20ms
	LedCtrlPtr->ledSettings[0].t3 = 15;		//0.3s, unit:20ms
	LedCtrlPtr->ledSettings[0].rampOnStep = 0x20;	//64ms, unit:2ms
	LedCtrlPtr->ledSettings[0].rampOffStep = 0x20;	//64ms, unit:2ms
	//BLUE
	LedCtrlPtr->ledSettings[1].onOff  = LED_ON;
	LedCtrlPtr->ledSettings[1].ledFollowMultiplierControl = 0x01;
	LedCtrlPtr->ledSettings[1].pwmSetting = 0xF0;
	LedCtrlPtr->ledSettings[1].t1 = 50;	//1s, unit:20ms
	LedCtrlPtr->ledSettings[1].t2 = 50;	//1s, unit:20ms
	LedCtrlPtr->ledSettings[1].t3 = 15;		//0.3s, unit:20ms
	LedCtrlPtr->ledSettings[1].rampOnStep = 0x20;	//64ms, unit:2ms
	LedCtrlPtr->ledSettings[1].rampOffStep = 0x20;	//64ms, unit:2ms
	
	LED_SetPatternToHW(LedCtrlPtr);
	
	OSMEM_Put((U8 XDATA_PTR)LedCtrlPtr);
}


/* 
 * SDK_LED_SetHWBySectorDataSample: sample code to set led HW by sector data
 */
PUBLIC void SDK_LED_SetHWBySectorDataSample(U8 ledIndex)
{
	LedPatternType XDATA_PTR LedCtrlPtr;
	
	LedCtrlPtr = SYS_MemoryGet(sizeof(LedPatternType));
	
	if(!LedCtrlPtr)
		ASSERT(FALSE);
	
	SDK_LED_DRV_LoadPattern(ledIndex, LedCtrlPtr);
	
	LED_SetPatternToHW(LedCtrlPtr);
	
	OSMEM_Put((U8 XDATA_PTR)LedCtrlPtr);
}

