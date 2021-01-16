#ifndef _MMILE_ROM_KEYPAD_H_
#define _MMILE_ROM_KEYPAD_H_

#define AU_KEYB0_MASK			0x0E  //only check GPIO  1 2 3 
#define AU_KEY_WAKEUP_MASK		0x01

#define AU_KEY1_PRESS			0x8000
#define AU_KEY1_RELEASE			0x0080

#define MMI_ROM_KEY_PRESS_TIMER 		    5  //unit: 10 ms

#define MMI_ROM_KEY_LONG_PRESS_ENG_MODE    400 //unit: 10 ms
#define MMI_ROM_KEY_LONG_PRESS_AIR_MODE    600 //unit: 10 ms
enum
{
	AU_KEY_WAKEUP,
	AU_KEY_GPIO1,
	AU_KEY_GPIO2,
	AU_KEY_GPIO3,
	AU_TOTAL_KEY_NO,
};


enum
{
	AU_KEY_NONE,	
	AU_KEY_PRESS,
	AU_KEY_LONG,
//	AU_KEY_MULTI,
//	AU_KEY_LONGLONG,
//	AU_KEY_EXTREMELY_LONG,
//	AU_KEY_VERYLONG,
//	AU_KEY_INTERNAL_TOGGLE_LONG,
//	AU_KEY_INTERNAL_TOGGLE_NONE,
//	AU_KEY_TAP,
//	AU_KEY_DOUBLE,
//	AU_KEY_TRIPPLE,
};

typedef struct
{
	U16 keyOldBitmap;
	U8 keyGpioIndex[AU_TOTAL_KEY_NO];
	U8 isConfiged;
}AU_KeypadCtrlInfo;

#ifdef _MMI_LE_ROM_C_
	#define AU_EXTERN 
#else
	#define AU_EXTERN extern
#endif


AU_EXTERN AU_KeypadCtrlInfo XDATA gAUKeypadCtrl;

#endif