#include "reside_flash.inc"

#define _DRIVER_KEY_C_
#include "rc.h"
#include "bt_config_profile.h"


#include "keypad.h"
#include "Driver.h"
#include "..\MMI\MMI.h"
#include "..\MMI\MMI_Driver.h"
#include "..\HC\hc_interface.h"

extern PUBLIC void DRIVER_LED_PowerSavingExit(BOOL isResetTimer);

#define GET_CURRENT_PRESSED_KEY_COUNT	(gDriver_Key_Ctl.currentKeyStatus & 0X0000FFFF)
#define GET_CURRENT_PRESSED_KEY_MAP   ((U8)((gDriver_Key_Ctl.currentKeyStatus & 0XFFFF0000) >> 24))


#define GET_KEY_PRESS_TIME(A)					(U32)(gMMI_nvram.keypadGpioCtl.keyTime.keyPressTime[A])
#define GET_KEY_LONG_PRESS_TIME(A)				(U32)(gMMI_nvram.keypadGpioCtl.keyTime.keyLongPressTime[A])
#define GET_KEY_LONGLONG_PRESS_TIME(A)			(U32)(gMMI_nvram.keypadGpioCtl.keyTime.keyLongLongPressTime[A])
#define GET_KEY_EXTREMELYLONG_PRESS_TIME(A)		(U32)(gMMI_nvram.keypadGpioCtl.keyTime.keyExtremelyLongPressTime[A])
#define GET_KEY_MULTI_TIME(A)					(U32)(gMMI_nvram.keypadGpioCtl.keyTime.keyMultiTime[A])
#define GET_KEY_CONT_PRESS_TIME(A)				(U32)(gMMI_nvram.keypadGpioCtl.keyTime.keyContPressTime[A])

enum
{
	DRIVER_KEY_NONE,	
	DRIVER_KEY_PRESS,
	DRIVER_KEY_LONG,
	DRIVER_KEY_MULTI,
	DRIVER_KEY_LONGLONG,
	DRIVER_KEY_EXTREMELY_LONG,
	DRIVER_KEY_VERYLONG,
	DRIVER_KEY_INTERNAL_TOGGLE_LONG,
	DRIVER_KEY_INTERNAL_TOGGLE_NONE,
	DRIVER_KEY_TAP,
	DRIVER_KEY_DOUBLE,
	DRIVER_KEY_TRIPPLE,
};

#define PWR_SAVING_KEY_CONTINUE	15
	
typedef struct
{
	OST XDATA_PTR ptr;
	U8 type;
}DriverKeyTimer;
	
typedef struct
{
	U32 currKeyBitmap;
	U32 oldKeyBitmap;
	U32 currentKeyStatus;	//key index(4 significant bits) , number of key pressed
	U8 currentPressedKeyIndex;
	U16 rotaryEncoderKeyStatus;
	DriverKeyTimer keyPressTimer[TOTAL_KEY_NO];
	OST XDATA_PTR keyContinuousPressTimer;
	U16 pwrSavingMask; //TOTAL_KEY_NO = 13
	OST XDATA_PTR keyDelayPwrSavingTimer;
}DriverKeyCtlType;

DriverKeyCtlType XDATA gDriver_Key_Ctl;

PRIVATE void DRIVER_Key_SetKeyTimer(U8 keyInd, U8 type)
{
	U32 time;
	
	if(keyInd < TOTAL_KEY_NO)
	{
		switch(type)
		{
			case DRIVER_KEY_PRESS:
				time = GET_KEY_PRESS_TIME(keyInd);
				break;
			case DRIVER_KEY_LONG:
				time = GET_KEY_LONG_PRESS_TIME(keyInd);
				break;
			case DRIVER_KEY_LONGLONG:
				time = GET_KEY_LONGLONG_PRESS_TIME(keyInd);
				break;
			case DRIVER_KEY_EXTREMELY_LONG:
				time = GET_KEY_EXTREMELYLONG_PRESS_TIME(keyInd);
				break;
			case DRIVER_KEY_INTERNAL_TOGGLE_LONG:
				time = GET_KEY_LONG_PRESS_TIME(keyInd);
				break;
			case DRIVER_KEY_INTERNAL_TOGGLE_NONE:
				time = GET_KEY_LONG_PRESS_TIME(keyInd);
				break;
			case DRIVER_KEY_MULTI:
				time = GET_KEY_MULTI_TIME(keyInd);
				break;
			default:
				return;
		}
		gDriver_Key_Ctl.keyPressTimer[keyInd].type = type;
		SYS_SetTimer(&gDriver_Key_Ctl.keyPressTimer[keyInd].ptr, (U32)(time * 320L));
		gDriver_Key_Ctl.pwrSavingMask |= BIT_MASK16(keyInd);		
		DRIVER_SetPowerSaving(FALSE, DRIVER_POWER_SAVING_KEY_TIMER);
		SYS_ReleaseTimer(&gDriver_Key_Ctl.keyDelayPwrSavingTimer);
	}

}

PRIVATE void DRIVER_Key_ReleaseKeyTimer(U8 keyInd, U8 type)
{
	if(keyInd < TOTAL_KEY_NO)
	{
		gDriver_Key_Ctl.keyPressTimer[keyInd].type = type;
		SYS_ReleaseTimer(&gDriver_Key_Ctl.keyPressTimer[keyInd].ptr);
		gDriver_Key_Ctl.pwrSavingMask &= ~BIT_MASK16(keyInd);
		if(!gDriver_Key_Ctl.pwrSavingMask)
		{
			SYS_SetTimer(&gDriver_Key_Ctl.keyDelayPwrSavingTimer, HALF_SEC);
		}
	}
}

PRIVATE void DRIVER_Key_SetContinueTimer(U8 scanInd)
{
	if(GET_KEY_CONT_PRESS_TIME(scanInd) && !gDriver_Key_Ctl.keyContinuousPressTimer)
	{
		SYS_SetTimer(&gDriver_Key_Ctl.keyContinuousPressTimer, GET_KEY_CONT_PRESS_TIME(scanInd) * 320);
		DRIVER_SetPowerSaving(FALSE, DRIVER_POWER_SAVING_KEY_TIMER);
		gDriver_Key_Ctl.pwrSavingMask |= BIT_MASK16(PWR_SAVING_KEY_CONTINUE);
		SYS_ReleaseTimer(&gDriver_Key_Ctl.keyDelayPwrSavingTimer);		
	}
}

PRIVATE void DRIVER_Key_ReleaseContinueTimer(void)
{
	SYS_ReleaseTimer(&gDriver_Key_Ctl.keyContinuousPressTimer);
	gDriver_Key_Ctl.pwrSavingMask &= ~BIT_MASK16(PWR_SAVING_KEY_CONTINUE);
	if(!gDriver_Key_Ctl.pwrSavingMask)
	{
		SYS_SetTimer(&gDriver_Key_Ctl.keyDelayPwrSavingTimer, HALF_SEC);
	}
}

PRIVATE void DRIVER_Key_SendEvent(KeyIndex keyIndex, U8 keyAction)
{
	if(gMMI_nvram.timerCtl.PowerSavingLightTimer)
	{
		DRIVER_LED_PowerSavingExit(TRUE);
	}
	LightDbgPrint("Key_SendEvent:%d,%d",(U8)keyIndex, (U8)keyAction);
	SetPowerSavingForDevice(FALSE, PWRSAV_KEY);
	gMMI_Drv_Handler.fDriverKeyEvent(keyIndex, keyAction);
}

PRIVATE void DRIVER_Key_MultiCheck(U8 scanInd)
{
	if (KeypadCheckKeyPressed(scanInd))
	{
		DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_PRESS);
	}
	else
	{
		DRIVER_Key_SendEvent(scanInd, KEY_TAP);
	}
}

PRIVATE void DRIVER_Key_TapCheck(U8 scanInd)
{
	if (!KeypadCheckKeyPressed(scanInd))
	{
		DRIVER_Key_SendEvent(scanInd, KEY_TAP);
	}
}

PRIVATE void DRIVER_Key_DoubleCheck(U8 scanInd)
{
	if (!KeypadCheckKeyPressed(scanInd))
	{
		DRIVER_Key_SendEvent(scanInd, KEY_DOUBLE);
	}
}

PRIVATE void DRIVER_Key_TrippleCheck(U8 scanInd)
{
	if (!KeypadCheckKeyPressed(scanInd))
	{
		DRIVER_Key_SendEvent(scanInd, KEY_TRIPLE);
	}
}

PRIVATE void DRIVER_Key_PressCheck(U8 scanInd)
{
	if(KeypadCheckKeyPressed(scanInd))
	{
		DRIVER_Key_SendEvent((GET_CURRENT_PRESSED_KEY_COUNT > 1)? (GET_CURRENT_PRESSED_KEY_MAP& 0XF0) : scanInd, KEY_PRESS);
		DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_LONG);
	}
	else
	{
		DRIVER_Key_SendEvent(scanInd, KEY_TAP);
	}
}

PRIVATE void DRIVER_Key_LongpressCheck(U8 scanInd)
{
	if(KEY_IS_SWITCH(scanInd))
	{
		DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_LONGLONG);
		DRIVER_Key_SendEvent(scanInd, KEY_LONGPRESS);
	}
	else
	{
		if (KeypadCheckKeyPressed(scanInd))
		{
			DRIVER_Key_SendEvent((GET_CURRENT_PRESSED_KEY_COUNT > 1)? (GET_CURRENT_PRESSED_KEY_MAP& 0XF0) : scanInd, KEY_LONGPRESS);
			DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_LONGLONG);
		}
		else
		{
			DRIVER_Key_SendEvent(scanInd, KEY_PRESS_RELEASE);
		}
	}
}

PRIVATE void DRIVER_Key_LongLongPressCheck(U8 scanInd)
{
	if(KEY_IS_SWITCH(scanInd))
	{
		DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_EXTREMELY_LONG);
		DRIVER_Key_SendEvent(scanInd, KEY_LONGLONGPRESS);
	}
	else
	{
		if (KeypadCheckKeyPressed(scanInd))
		{
			DRIVER_Key_SendEvent((GET_CURRENT_PRESSED_KEY_COUNT > 1)? (GET_CURRENT_PRESSED_KEY_MAP& 0XF0) : scanInd, KEY_LONGLONGPRESS);
			DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_EXTREMELY_LONG);
		}
		else
		{
			DRIVER_Key_SendEvent(scanInd, KEY_LONGPRESS_RELEASE);
		}
	}
}

PRIVATE void DRIVER_Key_ExtremelyLongPressCheck(U8 scanInd)
{
	if(KEY_IS_SWITCH(scanInd))
	{
		gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_NONE;
		DRIVER_Key_SendEvent(scanInd, KEY_EXTREMELY_LONGPRESS);
	}
	else
	{
		if (KeypadCheckKeyPressed(scanInd))
		{
			DRIVER_Key_SendEvent((GET_CURRENT_PRESSED_KEY_COUNT > 1)? (GET_CURRENT_PRESSED_KEY_MAP& 0XF0) : scanInd, KEY_EXTREMELY_LONGPRESS);

			gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_VERYLONG;
		}
		else
		{
			DRIVER_Key_SendEvent(scanInd, KEY_LONGLONGPRESS_RELEASE);
		}
	}
}

PRIVATE void DRIVER_Key_ToggleLongpressCheck(U8 scanInd)
{
	DRIVER_Key_SendEvent(scanInd, KEY_LONGPRESS);
	gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_INTERNAL_TOGGLE_LONG;
}

PRIVATE void DRIVER_Key_ToggleLongReleaseCheck(U8 scanInd)
{
	DRIVER_Key_SendEvent(scanInd, KEY_LONGPRESS_RELEASE);
	gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_INTERNAL_TOGGLE_NONE;
}

PUBLIC void DRIVER_Key_Init(void)
{
	KeypadInit();
}

PUBLIC void DRIVER_Key_Update(void)
{
	U8 mask;

	gKeypadCtrl.isConfiged = TRUE;
	
	if(MMI_FAST_DETECT_FOR_ROTARY_ENCODER_FEAT)
	{
		OS_ENTER_CRITICAL ();
		mask = LC_SFR_READ_BY_FE(FEOFFS_RW_INTF_INT_MASK);
		LC_SFR_WRITE_BY_FE(FEOFFS_RW_INTF_INT_MASK, (mask|INT1_ENABLE_PMU));
		mask = PMU_SYS_GET_BOOT_SW_STATUS_B1 () &(~SYS_SW1_CTL_ENABLE_GPIO_INT_FOR_S_NORMAL);
		PMU_SYS_SET_BOOT_SW_STATUS_B1(mask);
		OS_EXIT_CRITICAL ();
		DRIVER_SetPowerSaving(FALSE, DRIVER_POWER_ROTARY_ENCODER);
	}
}

PUBLIC void DRIVER_Key_Polling(void)
{
	U8 scanInd;
	U32 keyEvent, chkingKeyEvent;

	if (KeypadScanKeyDebounce(&gDriver_Key_Ctl.currKeyBitmap))
	{
		//Key is pressed or released now.
		//keyEvent: High 16 bits->KEY_PRESS
		//keyEvent: LOW 16 bits->KEY_RELEASE
		keyEvent = KeypadCheckKeyEvent(gDriver_Key_Ctl.oldKeyBitmap);

		for (scanInd = KEY1; scanInd < TOTAL_KEY_NO; scanInd++)
		{
			//Press
			chkingKeyEvent = keyEvent & (KEY1_PRESS >> scanInd);

			if(chkingKeyEvent)  // key is pressed
			{
				#ifdef DBG_UART_ENABLE_DRV
				LightDbgPrint("DRV - Press KeyID: %d", (U8)scanInd);
				#endif

				if(scanInd == KEY1 && !KEY_IS_SWITCH(scanInd))   // power key
				{
					DRIVER_SetBusyMask(TRUE, DRIVER_BUSY_POWER_KEY);
				}
				
				if(KEY_IS_ROTARY(scanInd))
				{
					if(!gDriver_Key_Ctl.rotaryEncoderKeyStatus)
						DRIVER_Key_SendEvent(scanInd, KEY_DOWN);

					gDriver_Key_Ctl.rotaryEncoderKeyStatus |=  (0x0001<<scanInd);
				}
				else if(KEY_IS_TOGGLE(scanInd))
				{
					gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_INTERNAL_TOGGLE_LONG;
					if (gDriver_Key_Ctl.keyPressTimer[scanInd].ptr == (OST XDATA_PTR)NULL)
					{
						DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_INTERNAL_TOGGLE_LONG);
					}
				}
				else if(KEY_IS_SWITCH(scanInd))
				{
					if (gDriver_Key_Ctl.keyPressTimer[scanInd].ptr == (OST XDATA_PTR)NULL  && gDriver_Key_Ctl.keyPressTimer[scanInd].type == DRIVER_KEY_NONE)
					{
						DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_LONG);
						DRIVER_Key_SendEvent(scanInd, KEY_DOWN);
					}
				}
				else if(scanInd < KEY5 && !MMI_NO_COMBO_KEY_NODE_FEAT)  //KEY is normal with combo effect
				{
					//Send KEY_DOWN event as soon as we sense any key has been pressed.
					DRIVER_Key_SendEvent(scanInd, KEY_DOWN);

					//Keep current key status
					gDriver_Key_Ctl.currentKeyStatus |= chkingKeyEvent;
					gDriver_Key_Ctl.currentKeyStatus++;

					if(GET_CURRENT_PRESSED_KEY_COUNT == 1)  //First key
					{
						gDriver_Key_Ctl.currentPressedKeyIndex = scanInd;

						if(gDriver_Key_Ctl.keyPressTimer[scanInd].type == DRIVER_KEY_NONE)
						{
							DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_MULTI);
							DRIVER_Key_SetContinueTimer(scanInd);
						}
					}
					else // combo
					{
						//key is pressed, trigger keyPressTimer
						if(gDriver_Key_Ctl.keyPressTimer[gDriver_Key_Ctl.currentPressedKeyIndex].type == DRIVER_KEY_MULTI)
						{
							DRIVER_Key_SetKeyTimer(gDriver_Key_Ctl.currentPressedKeyIndex, DRIVER_KEY_MULTI);
						}
						else  // invalid combo
						{
							DRIVER_Key_ReleaseKeyTimer(gDriver_Key_Ctl.currentPressedKeyIndex, DRIVER_KEY_NONE);
							gDriver_Key_Ctl.currentPressedKeyIndex = 0xFF;
						}
						DRIVER_Key_ReleaseContinueTimer();
					}
				}
				else  //KEY is normal.
				{
					//Send KEY_DOWN event as soon as we sense any key has been pressed.
					DRIVER_Key_SendEvent(scanInd, KEY_DOWN);

					if(!gDriver_Key_Ctl.currentKeyStatus) //First key
					{
						gDriver_Key_Ctl.currentPressedKeyIndex = scanInd;
						DRIVER_Key_SetContinueTimer(scanInd);
					}
					//Keep current key status
					gDriver_Key_Ctl.currentKeyStatus |= chkingKeyEvent;

					if(gDriver_Key_Ctl.keyPressTimer[scanInd].type == DRIVER_KEY_NONE)
					{
						DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_MULTI);
					}
				}
			}


			//Release
			chkingKeyEvent = keyEvent & (KEY1_RELEASE >> scanInd);

			if(chkingKeyEvent)  // key is released.
			{
				#ifdef DBG_UART_ENABLE_DRV
				LightDbgPrint("DRV - Release KeyID:%d", (U8)scanInd);
				#endif

				if(scanInd == KEY1 && !KEY_IS_SWITCH(scanInd))   // power key
				{
					DRIVER_SetBusyMask(FALSE, DRIVER_BUSY_POWER_KEY);
				}

				if(KEY_IS_ROTARY(scanInd))
				{
					gDriver_Key_Ctl.rotaryEncoderKeyStatus &= (~(0x0001<<scanInd));

					if(gDriver_Key_Ctl.rotaryEncoderKeyStatus)
					{
						DRIVER_Key_SendEvent(scanInd, KEY_UP);
					}
				}
				else if(KEY_IS_TOGGLE(scanInd))
				{
					gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_INTERNAL_TOGGLE_NONE;
					if (gDriver_Key_Ctl.keyPressTimer[scanInd].ptr == (OST XDATA_PTR)NULL)
					{
						DRIVER_Key_SetKeyTimer(scanInd, DRIVER_KEY_INTERNAL_TOGGLE_NONE);
					}
				}
				else if(KEY_IS_SWITCH(scanInd))
				{
					switch(gDriver_Key_Ctl.keyPressTimer[scanInd].type)
					{
						case DRIVER_KEY_LONGLONG:
							DRIVER_Key_SendEvent(scanInd, KEY_LONGLONGPRESS_RELEASE);
							break;

						case DRIVER_KEY_NONE:
						case DRIVER_KEY_EXTREMELY_LONG:
							DRIVER_Key_SendEvent(scanInd, KEY_EXTREMELY_LONGPRESS_RELEASE);
							break;
					}
					DRIVER_Key_ReleaseKeyTimer(scanInd, DRIVER_KEY_NONE);
					DRIVER_Key_SendEvent(scanInd, KEY_UP);
				}
				else if(scanInd < KEY5 && !MMI_NO_COMBO_KEY_NODE_FEAT)
				{
					
					if(GET_CURRENT_PRESSED_KEY_COUNT == 1)
					{
						gDriver_Key_Ctl.currentKeyStatus &= ~(KEY1_PRESS >> scanInd);
						gDriver_Key_Ctl.currentKeyStatus--;

						if(gDriver_Key_Ctl.currentPressedKeyIndex == scanInd)
						{
							goto NORMAL_SINGLE_KEY_RELEASE;
						}
					}
					else
					{
						if(gDriver_Key_Ctl.currentPressedKeyIndex != 0xFF)
						{
							switch(gDriver_Key_Ctl.keyPressTimer[gDriver_Key_Ctl.currentPressedKeyIndex].type)
							{
								case DRIVER_KEY_MULTI:
								case DRIVER_KEY_PRESS:
									DRIVER_Key_SendEvent(GET_CURRENT_PRESSED_KEY_MAP & 0XF0, KEY_TAP);
									DRIVER_Key_ReleaseKeyTimer(gDriver_Key_Ctl.currentPressedKeyIndex, DRIVER_KEY_NONE);
									break;

								case DRIVER_KEY_LONG:
									DRIVER_Key_SendEvent(GET_CURRENT_PRESSED_KEY_MAP & 0XF0, KEY_PRESS_RELEASE);
									DRIVER_Key_ReleaseKeyTimer(gDriver_Key_Ctl.currentPressedKeyIndex, DRIVER_KEY_NONE);
									break;

								case DRIVER_KEY_LONGLONG:
									DRIVER_Key_SendEvent(GET_CURRENT_PRESSED_KEY_MAP & 0XF0, KEY_LONGPRESS_RELEASE);
									DRIVER_Key_ReleaseKeyTimer(gDriver_Key_Ctl.currentPressedKeyIndex, DRIVER_KEY_NONE);
									break;

								case DRIVER_KEY_EXTREMELY_LONG:
									DRIVER_Key_SendEvent(GET_CURRENT_PRESSED_KEY_MAP & 0XF0, KEY_LONGLONGPRESS_RELEASE);
									DRIVER_Key_ReleaseKeyTimer(gDriver_Key_Ctl.currentPressedKeyIndex, DRIVER_KEY_NONE);
									break;

								case DRIVER_KEY_VERYLONG:
									DRIVER_Key_SendEvent(GET_CURRENT_PRESSED_KEY_MAP & 0XF0, KEY_EXTREMELY_LONGPRESS_RELEASE);
									DRIVER_Key_ReleaseKeyTimer(gDriver_Key_Ctl.currentPressedKeyIndex, DRIVER_KEY_NONE);
									break;

								default:
									break;
							}
						}
						gDriver_Key_Ctl.currentPressedKeyIndex = 0xFF;
						gDriver_Key_Ctl.currentKeyStatus --;
						gDriver_Key_Ctl.currentKeyStatus &= ~(KEY1_PRESS >> scanInd);
					}
				}
				else
				{					
					NORMAL_SINGLE_KEY_RELEASE:
					if(gDriver_Key_Ctl.currentPressedKeyIndex == scanInd)
					{
						gDriver_Key_Ctl.currentPressedKeyIndex = 0xFF;
						DRIVER_Key_ReleaseContinueTimer();
					}
					//Send KEY_UP event as soon as we sense any key has been released
					DRIVER_Key_SendEvent(scanInd, KEY_UP);

					gDriver_Key_Ctl.currentKeyStatus &= ~(KEY1_PRESS >> scanInd);

					switch(gDriver_Key_Ctl.keyPressTimer[scanInd].type)
					{
						case DRIVER_KEY_PRESS:
							DRIVER_Key_SendEvent(scanInd, KEY_TAP);
							DRIVER_Key_ReleaseKeyTimer(scanInd, DRIVER_KEY_NONE);
							break;

						case DRIVER_KEY_LONG:
							DRIVER_Key_SendEvent(scanInd, KEY_PRESS_RELEASE);
							DRIVER_Key_ReleaseKeyTimer(scanInd, DRIVER_KEY_NONE);
							break;

						case DRIVER_KEY_LONGLONG:
							DRIVER_Key_SendEvent(scanInd, KEY_LONGPRESS_RELEASE);
							DRIVER_Key_ReleaseKeyTimer(scanInd, DRIVER_KEY_NONE);
							break;

						case DRIVER_KEY_EXTREMELY_LONG:
							DRIVER_Key_SendEvent(scanInd, KEY_LONGLONGPRESS_RELEASE);
							DRIVER_Key_ReleaseKeyTimer(scanInd, DRIVER_KEY_NONE);
							break;
						case DRIVER_KEY_VERYLONG:
							DRIVER_Key_SendEvent(scanInd, KEY_EXTREMELY_LONGPRESS_RELEASE);
							DRIVER_Key_ReleaseKeyTimer(scanInd, DRIVER_KEY_NONE);
							break;

						case DRIVER_KEY_MULTI:
							gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_TAP;
							break;

						case DRIVER_KEY_TAP:
							gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_DOUBLE;
							break;

						case DRIVER_KEY_DOUBLE:
							gDriver_Key_Ctl.keyPressTimer[scanInd].type = DRIVER_KEY_TRIPPLE;
							break;

						default:
							// Keep for tap double and tripple
							break;
					}
				}
			}
		}
		gDriver_Key_Ctl.oldKeyBitmap = gDriver_Key_Ctl.currKeyBitmap;
	}
	
	if(SYS_IsTimerExpired(&gDriver_Key_Ctl.keyDelayPwrSavingTimer))
	{
		DRIVER_SetPowerSaving(TRUE, DRIVER_POWER_SAVING_KEY_TIMER);
	}
}

PUBLIC void Driver_Key_CheckTimer(void)
{
	U8 scanInd;
	for(scanInd = KEY1; scanInd < TOTAL_KEY_NO; scanInd++)
	{
		//check key pressed debounce timer
		if(SYS_IsTimerExpired(&gDriver_Key_Ctl.keyPressTimer[scanInd].ptr))
		{
			U8 pressType = gDriver_Key_Ctl.keyPressTimer[scanInd].type;

			DRIVER_Key_ReleaseKeyTimer(scanInd, DRIVER_KEY_NONE);

			switch (pressType)
			{
				case DRIVER_KEY_MULTI:
					DRIVER_Key_MultiCheck(scanInd);
					break;

				case DRIVER_KEY_TAP:
					DRIVER_Key_TapCheck(scanInd);
					break;

				case DRIVER_KEY_DOUBLE:
					DRIVER_Key_DoubleCheck(scanInd);
					break;

				case DRIVER_KEY_TRIPPLE:
					DRIVER_Key_TrippleCheck(scanInd);
					break;

				case DRIVER_KEY_PRESS:
					DRIVER_Key_PressCheck(scanInd);
					break;

				case DRIVER_KEY_LONG:
					DRIVER_Key_LongpressCheck(scanInd);
					break;

				case DRIVER_KEY_LONGLONG:
					DRIVER_Key_LongLongPressCheck(scanInd);
					break;

				case DRIVER_KEY_EXTREMELY_LONG:
					DRIVER_Key_ExtremelyLongPressCheck(scanInd);
					break;

				case DRIVER_KEY_INTERNAL_TOGGLE_LONG:
					DRIVER_Key_ToggleLongpressCheck(scanInd);
					break;

				case DRIVER_KEY_INTERNAL_TOGGLE_NONE:
					DRIVER_Key_ToggleLongReleaseCheck(scanInd);
					break;
			}
		}
	}

	if(SYS_IsTimerExpired(&gDriver_Key_Ctl.keyContinuousPressTimer))
	{
		//it's a continuous key tap, send out the event
		DRIVER_Key_SendEvent (gDriver_Key_Ctl.currentPressedKeyIndex, KEY_REPEAT);
		SYS_SetTimer (&gDriver_Key_Ctl.keyContinuousPressTimer, GET_KEY_CONT_PRESS_TIME(gDriver_Key_Ctl.currentPressedKeyIndex) * 320L);
	}
}

PUBLIC void DRIVER_ResetLineInKeyTimer(void)
{
	if(MMI_LINE_IN_DETECT_BY_GPIO)
	{
		DRIVER_Key_SetKeyTimer(	gMMI_nvram.lineInAutoDetectKeyIndex, gDriver_Key_Ctl.keyPressTimer[gMMI_nvram.lineInAutoDetectKeyIndex].type);
	}
}