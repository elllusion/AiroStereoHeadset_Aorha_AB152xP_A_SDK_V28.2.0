//////////////////////////////////////////////////////////
// Copyright@ 2016 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _DRIVER_LED_H_
#define _DRIVER_LED_H_

/*  DRIVER_LED_LoadPattern: Load led pattern fro sector_led_ctl_type.c.
 *	<input 1>
 *		U8 ledIndex				: assign the led data to display. (ledData[ledDataIndex] of sector_driver_led_ctl_type.c)
 *  	value range				: 0 - (totalLedPatternNo - 1).
 *	<input 2>
 *		U8 XDATA_PTR destPtr	: destination pointer
 *  	value range				: pointer
 *	<output>
 *		BOOL					: result of loading
 *  	value range				: TRUE--> OK, FALSE --> Fail
 */
PUBLIC BOOL DRIVER_LED_LoadPattern(U8 ledIndex, U8 XDATA_PTR destPtr);


/*  DRIVER_Cmd_LED_Restart: Restart led patterns.
 *	<input 1>
 *		U8 para			: type of patterns 
 *  	value range		: DRIVER_CMD_LED_RESTART_PARA_FG_AND_BG means restarting current foreground and background led patterns.
 *						  DRIVER_CMD_LED_RESTART_PARA_ONLY_FG means only restarting current foreground led pattern.
 */
enum
{
	DRIVER_CMD_LED_RESTART_PARA_FG_AND_BG,
	DRIVER_CMD_LED_RESTART_PARA_ONLY_FG,
};
PUBLIC void DRIVER_Cmd_LED_Restart(U8 para);



/*  DRIVER_Cmd_LED_FGLed: Trigger leds as foreground pattern.
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
PUBLIC void DRIVER_Cmd_LED_FGLed(U8 ledDataIndex, U8 timeout, U16 id, U8 isOverrideFilter);



/*  DRIVER_Cmd_LED_BGLed: Trigger leds as background pattern.
 *	<input 1>
 *		U8 ledDataIndex	: assign the led data to display. (ledData[ledDataIndex] in DRIVER_LED_DATA_TYPE in sector_driver_led_ctl_type.h)
 *  	value range		: 0 - (totalLedPatternNo - 1). (totalLedPatternNo is in DRIVER_LED_DATA_TYPE in sector_driver_led_ctl_type.h)
 */
PUBLIC void DRIVER_Cmd_LED_BGLed(U8 ledDataIndex);



/*  DRIVER_Cmd_LED_FilterLed: Trigger led filter.
 *	<input 1>
 *		U8 type			: to assign led filter or to cancel specific led filter.
 *  	value range		: LED_FILTER_CANCEL, or LED_FILTER_1 - LED_FILTER_8.
 *	<input 2>
 *		U8 para			: if type is LED_FILTER_CANCEL, para means the filter index to be cancelled.
 *						  if type is LED_FILTER_x, para means filter applied settings of ledFilterData[para] in DRIVER_LED_DATA_TYPE in sector_driver_led_ctl_type.h.
 *  	value range		: if type is LED_FILTER_CANCEL, LED_FILTER_CANCEL_FILTER_1 - LED_FILTER_CANCEL_FILTER_8
 *						: if type is LED_FILTER_x, 0 - (totalLedFilterNo-1). (totalLedFilterNo is in DRIVER_LED_DATA_TYPE in sector_driver_led_ctl_type.h)
 */

#define LED_FILTER_CANCEL_FILTER_1	0
#define LED_FILTER_CANCEL_FILTER_2	1
#define LED_FILTER_CANCEL_FILTER_3	2
#define LED_FILTER_CANCEL_FILTER_4	3
#define LED_FILTER_CANCEL_FILTER_5	4
#define LED_FILTER_CANCEL_FILTER_6	5
#define LED_FILTER_CANCEL_FILTER_7	6
#define LED_FILTER_CANCEL_FILTER_8	7


#define LED_FILTER_CANCEL	0
#define LED_FILTER_1		(U8_BIT_MASK(LED_FILTER_CANCEL_FILTER_1))
#define LED_FILTER_2		(U8_BIT_MASK(LED_FILTER_CANCEL_FILTER_2))
#define LED_FILTER_3		(U8_BIT_MASK(LED_FILTER_CANCEL_FILTER_3))
#define LED_FILTER_4		(U8_BIT_MASK(LED_FILTER_CANCEL_FILTER_4))
#define LED_FILTER_5		(U8_BIT_MASK(LED_FILTER_CANCEL_FILTER_5))
#define LED_FILTER_6		(U8_BIT_MASK(LED_FILTER_CANCEL_FILTER_6))
#define LED_FILTER_7		(U8_BIT_MASK(LED_FILTER_CANCEL_FILTER_7))
#define LED_FILTER_8		(U8_BIT_MASK(LED_FILTER_CANCEL_FILTER_8))

PUBLIC void DRIVER_Cmd_LED_FilterLed(U8 type, U8 para);
#if !defined AB1520S && !defined AB1525S && !defined AB1520U
PUBLIC void DRIVER_LED_MapGpioSetup(void);
#endif
#endif //_DRIVER_LED_H_
