//////////////////////////////////////////////////////////
// Copyright@ 2016 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _DRV_LED_H_
#define _DRV_LED_H_
	
/*  LED_SetPwm: Set brightness of all leds.
 *	<input 1>
 *		U8 pwm			: the brightness level. 
 *  	value range		: 0 - 15. 0 means no brightness. 15 means the maximun brightness.
 *		note			: default system pwm is 15.
 */
PUBLIC void LED_SetPwm(U8 pwm);



/*  LED_SetPatternToHW: Set pattern to all leds.
 *	<input 1>
 *		LedPatternType XDATA_PTR ledCtlPtr	: edata address of displaying pattern. 
 *  	usage								: set LedPatternType before LED_SetPatternToHW is called. [sample code]:SDK\IOSYS\LED.c
 */
enum
{
	LED1_INDEX,
	LED2_INDEX,
	LED3_INDEX,
	LED4_INDEX,
	LED5_INDEX,
	LED_NO,
};

enum
{
	LED_OFF = 0,
	LED_ON = 1,
	LED_ALT_FIRST,
	LED_ALT_SECOND,
	LED_ALT_THIRD,
	LED_ALT_FORTH,
	LED_DONT_CARE,
};

typedef struct
{
	U8 ledFollowMultiplierControl; //ledfollow delay[7:4], multiplier[3:2], followEable[1], repeatAlways[0]
	U8 pwmSetting; // pwmDruation[7:4], t1t2Repetion[3:0]
	U8 t1; // 20ms
	U8 t2; // 20ms
	U8 t3; // 20ms
	U8 rampOnStep; //2ms
	U8 rampOffStep; //2ms
	U8 onOff;
	U8 t4; ////40ms
	U8 loopForExtendRepeat; 	//loop 0~15 times
}LedSettingsType;
typedef struct
{
	LedSettingsType ledSettings[LED_NO];
}LedPatternType;
PUBLIC void LED_SetPatternToHW(LedPatternType XDATA_PTR ledCtlPtr);



/*  LED_TurnOffAllLeds: Turn off all leds.
 *
 */
PUBLIC void LED_TurnOffAllLeds(void);

#endif //_DRV_LED_H_

