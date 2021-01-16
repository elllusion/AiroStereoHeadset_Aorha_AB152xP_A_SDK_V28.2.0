//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "os.h"
#include "keypad_hal.h"

enum
{
	KEY1,
	KEY2,
	KEY3,
	KEY4,
	KEY5,
	KEY6,
	KEY7,
	KEY8,
	KEY9,
	KEY10,
	TOTAL_KEY_NO,
	KEYALL,
	KEYNONE,
	KEY_PWR = KEY1, // power key gpio 0
	KEY_COMB_34 = (0x80 >> KEY3) | (0x80 >> KEY4),
	KEY_COMB_23 = (0x80 >> KEY2) | (0x80 >> KEY3),
	KEY_COMB_13 = (0x80 >> KEY1) | (0x80 >> KEY3),
	KEY_COMB_24 = (0x80 >> KEY2) | (0x80 >> KEY4),
	KEY_COMB_14 = (0x80 >> KEY1) | (0x80 >> KEY4),
	KEY_COMB_12 = (0x80 >> KEY1) | (0x80 >> KEY2),
	KEY_COMB_123 = (0x80 >> KEY1) | (0x80 >> KEY2) | (0x80 >> KEY3),
	KEY_COMB_124 = (0x80 >> KEY1) | (0x80 >> KEY2) | (0x80 >> KEY4),
	KEY_COMB_134 = (0x80 >> KEY1) | (0x80 >> KEY3) | (0x80 >> KEY4),
	KEY_COMB_234 = (0x80 >> KEY2) | (0x80 >> KEY3) | (0x80 >> KEY4),
	KEY_COMB_1234 = 0xF0
};
typedef U8	KeyIndex;
typedef U16	KeyIndex2;

#define		KEY1_PRESS   0X80000000
#define		KEY2_PRESS 	 0X40000000
#define		KEY3_PRESS 	 0X20000000
#define		KEY4_PRESS 	 0X10000000	
#define		KEY5_PRESS 	 0X08000000
#define		KEY6_PRESS 	 0X04000000
#define		KEY7_PRESS 	 0X02000000
#define		KEY8_PRESS 	 0X01000000
#define		KEY9_PRESS 	 0X00800000
#define		KEY10_PRESS  0X00400000
#define		KEY_PWR_PRESS  KEY1_PRESS
#define		KEY1_RELEASE  0X00008000
#define		KEY2_RELEASE  0X00004000
#define		KEY3_RELEASE  0X00002000
#define		KEY4_RELEASE  0X00001000
#define		KEY5_RELEASE  0X00000800
#define		KEY6_RELEASE  0X00000400
#define		KEY7_RELEASE  0X00000200
#define		KEY8_RELEASE  0X00000100
#define		KEY9_RELEASE  0X00000080
#define		KEY10_RELEASE  0X00000040
#define		KEY_PWR_RELEASE  KEY1_RELEASE

#define KEY_TYPE_NORMAL	0x00
#define KEY_TYPE_SWITCH	0x80
#define KEY_TYPE_ROTARY	0x40
#define KEY_TYPE_TOGGLE	0xC0
#define KEY_TYPE_MASK		0xE0

#define KEY_GPIO_MASK		0x1F

	

#define KEY_IS_SWITCH(keyIndex)		(Keypad_IsSwitch(keyIndex))	
#define KEY_IS_ROTARY(keyIndex)		(Keypad_IsRotary(keyIndex))	
#define KEY_IS_TOGGLE(keyIndex)		(Keypad_IsToggle(keyIndex))	


typedef struct
{
	U32 keyOldBitmap;
	U8 isConfiged;
}KeypadCtrlInfo;


#ifdef _KEYPAD_C_
#define KEYPAD_EXTERN  
#else
#define KEYPAD_EXTERN extern
#endif
		  

KEYPAD_EXTERN KeypadCtrlInfo XDATA gKeypadCtrl;

KEYPAD_EXTERN PUBLIC void KeypadInit(void);
KEYPAD_EXTERN PUBLIC BOOL KeypadScanKeyDebounce(U32 XDATA_PTR keymapPtr);
KEYPAD_EXTERN PUBLIC U32 KeypadCheckKeyEvent(U32 oldKeyBitmap);
KEYPAD_EXTERN PUBLIC BOOL KeypadCheckKeyPressed(KeyIndex keyIndex);

KEYPAD_EXTERN PUBLIC BOOL Keypad_IsSwitch(KeyIndex keyIndex);
KEYPAD_EXTERN PUBLIC BOOL Keypad_IsRotary(KeyIndex keyIndex);
KEYPAD_EXTERN PUBLIC BOOL Keypad_IsToggle(KeyIndex keyIndex);

#endif

