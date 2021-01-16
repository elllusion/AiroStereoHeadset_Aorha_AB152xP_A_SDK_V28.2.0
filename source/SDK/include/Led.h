#ifndef _SDK_LED_H_
#define _SDK_LED_H_


enum
{
	SDK_LED1_ID,
	SDK_LED2_ID,
	SDK_LED3_ID,
	SDK_LED4_ID,
	SDK_LED_CNT,
};

enum
{
	SDK_LED_OFF = 0,
	SDK_LED_ON = 1,
	SDK_LED_ALT_FIRST,
	SDK_LED_ALT_SECOND,
	SDK_LED_ALT_THIRD,
	SDK_LED_ALT_FORTH,
	SDK_LED_DONT_CARE,
};

#define SDK_DRV_RESTART_LED_FG_BG	0
#define SDK_DRV_RESTART_LED_FG		1


#define SDK_CANCEL_FILTER_1			0
#define SDK_CANCEL_FILTER_2			1
#define SDK_CANCEL_FILTER_3			2
#define SDK_CANCEL_FILTER_4			3
#define SDK_CANCEL_FILTER_5			4
#define SDK_CANCEL_FILTER_6			5
#define SDK_CANCEL_FILTER_7			6
#define SDK_CANCEL_FILTER_8			7


#define SDK_FILTER_CANCEL	0
#define SDK_FILTER_1		(U8_BIT_MASK(SDK_CANCEL_FILTER_1))
#define SDK_FILTER_2		(U8_BIT_MASK(SDK_CANCEL_FILTER_2))
#define SDK_FILTER_3		(U8_BIT_MASK(SDK_CANCEL_FILTER_3))
#define SDK_FILTER_4		(U8_BIT_MASK(SDK_CANCEL_FILTER_4))
#define SDK_FILTER_5		(U8_BIT_MASK(SDK_CANCEL_FILTER_5))
#define SDK_FILTER_6		(U8_BIT_MASK(SDK_CANCEL_FILTER_6))
#define SDK_FILTER_7		(U8_BIT_MASK(SDK_CANCEL_FILTER_7))
#define SDK_FILTER_8		(U8_BIT_MASK(SDK_CANCEL_FILTER_8))

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
	LedSettingsType ledSettings[LED_CNT];
}LedPatternType;





/*  SDK_LED_HW_SetPWM: Set brightness of all leds.
 *	<input 1>
 *		U8 pwm			: the brightness level. 
 *  	value range		: 0 - 15. 0 means no brightness. 15 means the maximun brightness.
 *		note			: default system pwm is 15.
 */
void SDK_LED_HW_SetPWM(U8 pwm);


/*  SDK_LED_HW_SetPattern: Set pattern to all leds.
 *	<input 1>
 *		LedPatternType XDATA_PTR ledCtlPtr	: edata address of displaying pattern. 
 *  	usage								: set LedPatternType before LED_SetPatternToHW is called. [sample code]:SDK\IOSYS\LED.c
 */
PUBLIC void SDK_LED_HW_SetPattern(LedPatternType XDATA_PTR ledCtlPtr);



/*  SDK_LED_HW_TurnOffAllLeds: Turn off all leds.
 *
 */
PUBLIC void SDK_LED_HW_TurnOffAllLeds(void);


/*  SDK_LED_DRV_LoadPattern: Load led pattern fro sector_led_ctl_type.c.
 *	<input 1>
 *		U8 ledIndex				: assign the led data to display. (ledData[ledDataIndex] of sector_driver_led_ctl_type.c)
 *  	value range				: 0 - (totalLedPatternNo - 1).
 *	<input 2>
 *		LedPatternType XDATA_PTR destPtr	: destination pointer
 *  	value range							: pointer
 *	<output>
 *		BOOL					: result of loading
 *  	value range				: TRUE--> OK, FALSE --> Fail
 */
PUBLIC BOOL SDK_LED_DRV_LoadPattern(U8 ledIndex, LedPatternType XDATA_PTR destPtr);

/*  SDK_LED_DRV_Cmd_Restart: Restart led patterns.
 *	<input 1>
 *		U8 para			: type of patterns 
 *  	value range		: SDK_DRV_RESTART_LED_FG_BG means restarting current foreground and background led patterns.
 *						  SDK_DRV_RESTART_LED_FG means only restarting current foreground led pattern.
 */
PUBLIC void SDK_LED_DRV_Cmd_Restart(U8 para);



/*  SDK_LED_DRV_Cmd_FGLed: Trigger leds as foreground pattern.
 *	<input 1>
 *		U8 ledDataIndex	: assign the led data to display. (ledData[ledDataIndex] of sector_driver_led_ctl_type.c)
 *  	value range		: 0 - (totalLedPatternNo - 1).
 *	<input 2>
 *		U8 timeout		: duration of displaying led.
 *  	value range		: 0 - 255. unit: 0.1 second.
 *	<input 3>
 *		U8 id			: media event id to trigger foreground led.
 *  	value range		: MEDIA_EVT_xxxxxxxxx defined in mmi_event_opcode.h
 *	<input 4>
 *		U8 isOverrideFilter	: to indicate that foreground led is prior to led filter.
 *  	value range		: 0 means led filter is prior to foreground led. 1 mean foreground led is prior to led filter.
 *		note			: if MMI_MEDIA_LED_WITH_FILTER_EFFECT_FEAT is on, isOverrideFilter is invalid.
 *						  if MMI_MEDIA_LED_WITH_FILTER_EFFECT_FEAT is off, isOverrideFilter is valid.
 *  <note 1>			: if MMI_QUEUE_LED_FEAT is on, consecutive DRIVER_Cmd_LED_FGLed would be put into queue.
 * 						: if MMI_QUEUE_LED_FEAT is off, new DRIVER_Cmd_LED_FGLed would be discard when foreground led is busy.
 */
PUBLIC void SDK_LED_DRV_Cmd_FGLed(U8 ledDataIndex, U8 timeout, U8 id, U8 isOverrideFilter);



/*  SDK_LED_DRV_Cmd_BGLed: Trigger leds as background pattern.
 *	<input 1>
 *		U8 ledDataIndex	: assign the led data to display. (ledData[ledDataIndex] in DRIVER_LED_DATA_TYPE in sector_driver_led_ctl_type.h)
 *  	value range		: 0 - (totalLedPatternNo - 1). (totalLedPatternNo is in DRIVER_LED_DATA_TYPE in sector_driver_led_ctl_type.h)
 */
PUBLIC void SDK_LED_DRV_Cmd_BGLed(U8 ledDataIndex);




/*  SDK_LED_DRV_Cmd_FilterLed: Trigger led filter.
 *	<input 1>
 *		U8 type			: to assign led filter or to cancel specific led filter.
 *  	value range		: SDK_FILTER_CANCEL, or SDK_FILTER_1 - SDK_FILTER_8.
 *	<input 2>
 *		U8 para			: if type is SDK_FILTER_CANCEL, para means the filter index to be cancelled.
 *						  if type is SDK_FILTER_x, para means filter applied settings of ledFilterData[para] in DRIVER_LED_DATA_TYPE in sector_driver_led_ctl_type.h.
 *  	value range		: if type is SDK_FILTER_CANCEL, SDK_CANCEL_FILTER_1 - SDK_CANCEL_FILTER_8
 *						: if type is SDK_FILTER_x, 0 - (totalLedFilterNo-1). (totalLedFilterNo is in DRIVER_LED_DATA_TYPE in sector_driver_led_ctl_type.h)
 */
PUBLIC void SDK_LED_DRV_Cmd_FilterLed(U8 type, U8 para);

#endif