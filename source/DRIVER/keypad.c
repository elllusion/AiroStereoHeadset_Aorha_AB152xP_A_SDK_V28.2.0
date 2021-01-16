//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _KEYPAD_C_

#include "keypad.h"
#include "..\MMI\MMI.h"

#define DEFAULT_WAKE_PIO	0

#define KEY_MASK 	(gMMI_nvram.keypadGpioCtl.keyMask)

#define KEYB0_MASK ((U8)KEY_MASK)
#define KEYB1_MASK ((U8)(KEY_MASK>>8))
#define KEYB2_MASK ((U8)(KEY_MASK>>16))
#define KEYB3_MASK ((U8)(KEY_MASK>>24))

#define KEY_WAKEUP_MASK 0x01

#define KEY_GPIO_INDEX(keyIndex)	(Keypad_GetGPIOIndex(keyIndex))

void KeypadInit(void)
{
	KeypadSetDebounce(TRUE);
}

U8 Keypad_GetGPIOIndex(KeyIndex keyIndex)
{
	if(keyIndex == KEY1)
		return (gMMI_nvram.key1GpioIndex & KEY_GPIO_MASK);
	else
		return (gMMI_nvram.keypadGpioCtl.keyGpioIndex[keyIndex-1] & KEY_GPIO_MASK);
}

BOOL KeypadScanKeyDebounce(U32 XDATA_PTR keymapPtr)
{
	U8 keyB0BitmapLatch, keyB0BitmapDeb;
	U8 keyB1BitmapLatch, keyB1BitmapDeb;
	U8 keyB2BitmapLatch, keyB2BitmapDeb;
	U8 keyB3BitmapLatch, keyB3BitmapDeb;
	U8 keyWakeUpLatch, keyWakeUpDeb;
	U32 chgKeyBitmap, keyDeb;
	
	/// WAKE UP Key SFR READ/////////
	READ_WAKEUP_LATCH(keyWakeUpLatch);
	keyWakeUpLatch &= KEY_WAKEUP_MASK;
	if(keyWakeUpLatch)
	{
		SET_WAKEUP_LATCHRD(keyWakeUpLatch);
	}
	READ_WAKEUP_DEBOUNCE(keyWakeUpDeb);
	keyWakeUpDeb &= KEY_WAKEUP_MASK;
	keyWakeUpDeb |= keyWakeUpLatch;
	
	/////Key Map : GPIO 0~7///////////////
	READ_KEYB0LATCH(keyB0BitmapLatch);
	keyB0BitmapLatch &= KEYB0_MASK;
	if (keyB0BitmapLatch)
	{
		SET_KEYB0LATCHRD(keyB0BitmapLatch);
	}	
	READ_KEYB0DEBOUNCE(keyB0BitmapDeb);
	keyB0BitmapDeb &= KEYB0_MASK;
	keyB0BitmapDeb |= keyB0BitmapLatch;

	/////Key Map : GPIO 8~15///////////////
	READ_KEYB1LATCH(keyB1BitmapLatch);
	keyB1BitmapLatch &= KEYB1_MASK;
	if (keyB1BitmapLatch)
	{
		SET_KEYB1LATCHRD(keyB1BitmapLatch);
	}	
	READ_KEYB1DEBOUNCE(keyB1BitmapDeb);
	keyB1BitmapDeb &= KEYB1_MASK;
	keyB1BitmapDeb |= keyB1BitmapLatch;

	/////Key Map : GPIO 16~23///////////////
	READ_KEYB2LATCH(keyB2BitmapLatch);
	keyB2BitmapLatch &= KEYB2_MASK;
	if (keyB2BitmapLatch)
	{
		SET_KEYB2LATCHRD(keyB2BitmapLatch);
	}	
	READ_KEYB2DEBOUNCE(keyB2BitmapDeb);
	keyB2BitmapDeb &= KEYB2_MASK;
	keyB2BitmapDeb |= keyB2BitmapLatch;
	
	/////Key Map : GPIO 24~30///////////////
	READ_KEYB3LATCH(keyB3BitmapLatch);
	keyB3BitmapLatch &= KEYB3_MASK;
	if (keyB3BitmapLatch)
	{
		SET_KEYB3LATCHRD(keyB3BitmapLatch);
	}	
	READ_KEYB3DEBOUNCE(keyB3BitmapDeb);
	keyB3BitmapDeb &= KEYB3_MASK;
	keyB3BitmapDeb |= keyB3BitmapLatch;
	
	
	keyDeb  = (((U32)keyWakeUpDeb<<31)|((U32)keyB3BitmapDeb<<24)|((U32)keyB2BitmapDeb<<16)|((U32)keyB1BitmapDeb<<8)|(U32)keyB0BitmapDeb);
	chgKeyBitmap =  keyDeb ^ gKeypadCtrl.keyOldBitmap;

	if (chgKeyBitmap)
	{
		//key bitmap is different from old state
		gKeypadCtrl.keyOldBitmap = keyDeb;
		*keymapPtr = gKeypadCtrl.keyOldBitmap;
		return TRUE;
	}

	return FALSE;
}

U32 KeypadCheckKeyEvent(U32 oldKeyBitmap)
{
	U32 chgKeyBitmap, keyEvent;
	U8  keyIndex;
	U32 chgKeyBitmap31BitsUsed;

	keyEvent = 0;
	chgKeyBitmap = oldKeyBitmap ^ gKeypadCtrl.keyOldBitmap;
	keyIndex = KEY1;
	if(gKeypadCtrl.isConfiged)
	{
		if(KEY_GPIO_INDEX(KEY1) == DEFAULT_WAKE_PIO)
		{
			if((chgKeyBitmap >> 31 )& KEY_WAKEUP_MASK)
			{		
				//key with index "scanInd" is changed
				if (((U8)(oldKeyBitmap>>31)) & KEY_WAKEUP_MASK)
				{
					//key press in earlier time, now it's key release
					keyEvent |= KEY1_RELEASE >> KEY1;
				}
				else
				{
					//key pressed
					keyEvent |= KEY1_PRESS >> KEY1;
				}
			}
			keyIndex = KEY2;		
		}
		
		chgKeyBitmap31BitsUsed = (chgKeyBitmap & 0x7FFFFFFF);
		for( ; keyIndex < TOTAL_KEY_NO; keyIndex++)
		{
			if(chgKeyBitmap31BitsUsed & U32_BIT_MASK(KEY_GPIO_INDEX(keyIndex)) & KEY_MASK)
			{			
				//key with index "scanInd" is changed
				if ((oldKeyBitmap & 0x7FFFFFFF )& U32_BIT_MASK(KEY_GPIO_INDEX(keyIndex)))
				{
					//key press in earlier time, now it's key release
					keyEvent |= KEY1_RELEASE >> keyIndex;
				}
				else
				{
					//key pressed
					keyEvent |= KEY1_PRESS >> keyIndex;
				}
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return keyEvent;
}

BOOL KeypadCheckKeyPressed(KeyIndex keyIndex)
{
	U8 keyBitmap = 0;
	U8 keyMask = 0;
	U8 keyGPIOIndex = KEY_GPIO_INDEX(keyIndex);

	if(keyIndex == KEY1 && keyGPIOIndex == DEFAULT_WAKE_PIO)
	{
		READ_WAKEUP_DEBOUNCE(keyBitmap);
		keyMask = KEY_WAKEUP_MASK;
	}
	else if(keyGPIOIndex < 8)
	{
		READ_KEYB0DEBOUNCE(keyBitmap);
		keyMask = U8_BIT_MASK(keyGPIOIndex);
	}
	else if(keyGPIOIndex < 16)
	{
		READ_KEYB1DEBOUNCE(keyBitmap);
		keyMask = U8_BIT_MASK((keyGPIOIndex-8)) ;
	}
	else if(keyGPIOIndex < 24)
	{
		READ_KEYB2DEBOUNCE(keyBitmap);
		keyMask = U8_BIT_MASK((keyGPIOIndex-16)) ;
	}
	else if(keyGPIOIndex < 31)
	{
		READ_KEYB3DEBOUNCE(keyBitmap);
		keyMask = U8_BIT_MASK((keyGPIOIndex-24)) ;
	}

	return (keyBitmap & keyMask) ? TRUE : FALSE;
}

BOOL Keypad_IsSwitch(KeyIndex keyIndex)
{
	if(keyIndex == KEY1)
		return ((gMMI_nvram.key1GpioIndex & KEY_TYPE_MASK) == KEY_TYPE_SWITCH) ? TRUE : FALSE;
	else
		return ((gMMI_nvram.keypadGpioCtl.keyGpioIndex[keyIndex-1] & KEY_TYPE_MASK) == KEY_TYPE_SWITCH) ? TRUE : FALSE;
}

BOOL Keypad_IsRotary(KeyIndex keyIndex)
{
	if(keyIndex == KEY1)
		return ((gMMI_nvram.key1GpioIndex & KEY_TYPE_MASK) == KEY_TYPE_ROTARY) ? TRUE : FALSE;
	else
		return ((gMMI_nvram.keypadGpioCtl.keyGpioIndex[keyIndex-1] & KEY_TYPE_MASK) == KEY_TYPE_ROTARY) ? TRUE : FALSE;
}

BOOL Keypad_IsToggle(KeyIndex keyIndex)
{
	if(keyIndex == KEY1)
		return ((gMMI_nvram.key1GpioIndex & KEY_TYPE_MASK) == KEY_TYPE_TOGGLE) ? TRUE : FALSE;
	else
		return ((gMMI_nvram.keypadGpioCtl.keyGpioIndex[keyIndex-1] & KEY_TYPE_MASK) == KEY_TYPE_TOGGLE) ? TRUE : FALSE;
}
