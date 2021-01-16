//////////////////////////////////////////////////////////
// Copyright@ 2016 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define _DRIVER_LED_C_

#include <rc.h>
#include "drv_led.h"
#include "..\MMI\MMI.h"
#include "..\MMI\MMI_Driver.h"
#include "sector_config1.h"
#include "Driver_LED.h"
#include "Driver.h"

typedef struct
{
	LedFilterSettingType filterSetting;
}LedFilterCtlType;

extern struct driver_led_data_st;

typedef struct
{
	U8 isLedPwrSaving:1;
	U8 ledPatternNo:7;
	U8 ledFilterNo;
	U8 ledFilterEableMask;
	struct driver_led_data_st CODE_PTR pDriverLedData;
	LedFilterCtlType XDATA_PTR ledFilterCtlPtr[LED_FILTER_DEFAULT_NO];
	OST XDATA_PTR ledPowerSavingTimer;	
	OST XDATA_PTR fgLedTimerPtr;
	U8 fgLedMediaOpcode;
	U8 fgLedOnOff[LED_NO];
	
	LedPatternType bgLedData;
}DriverLEDCtlType;

DriverLEDCtlType XDATA gDriver_LED_ctl;

OSMQ XDATA OSMQ_LED_Command;
#define OSMQ_LED_Command_ptr				&OSMQ_LED_Command

#define LED_IS_PWR_SAVING	(gDriver_LED_ctl.isLedPwrSaving);
PUBLIC BOOL DRIVER_LED_IsFGLEDOn(void);
#if !defined AB1520S && !defined AB1525S && !defined AB1520U
PUBLIC void DRIVER_LED_MapGpioSetup(void);
#endif

PRIVATE BOOL DRIVER_LED_IsFilterOn(void)
{
	if(gDriver_LED_ctl.ledFilterEableMask)
		return TRUE;

	return FALSE;
}

PUBLIC BOOL DRIVER_LED_LoadPattern(U8 ledIndex, U8 XDATA_PTR destPtr)
{
	if(ledIndex >= gDriver_LED_ctl.ledPatternNo)
		return FALSE;

	OSMEM_memcpy_xdata_code(destPtr, (U8 CODE_PTR)&gDriver_LED_ctl.pDriverLedData->ledData[ledIndex], sizeof(LedPatternType));

	return TRUE;
}

PRIVATE void DRIVER_LED_SetFilter(U8 type, U8 para)
{
	LedFilterCtlType XDATA_PTR destPtr;

	if(type == LED_FILTER_CANCEL)
	{
		SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gDriver_LED_ctl.ledFilterCtlPtr[para]);
		gDriver_LED_ctl.ledFilterEableMask &= (~(U8_BIT_MASK(para)));
	}
	else
	{
		if(para >= gDriver_LED_ctl.ledFilterNo)
			return;

		destPtr = (LedFilterCtlType XDATA_PTR)SYS_MemoryGet(sizeof(LedFilterCtlType));

		if(destPtr == (LedFilterCtlType XDATA_PTR)NULL)
			return;


		OSMEM_memcpy_xdata_code((U8 XDATA_PTR)destPtr,
								(U8 CODE_PTR)(&gDriver_LED_ctl.pDriverLedData->totalLedPatternNo)+((U16)gDriver_LED_ctl.ledPatternNo * sizeof(LedPatternType))+1+1+(U16)para *sizeof(LedFilterCtlType),
								sizeof(LedFilterCtlType) );

		gDriver_LED_ctl.ledFilterEableMask |= type;

		SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gDriver_LED_ctl.ledFilterCtlPtr[para]);
		gDriver_LED_ctl.ledFilterCtlPtr[para] = destPtr;
	}
}

PRIVATE void DRIVER_LED_EnableBGLed(LedPatternType XDATA_PTR ledCtlPtr)
{
	U8 i, ledIndex;
	U8 rptT12, pwm;
	U16 timeTemp;
	LedSettingsType LDATA tempLedData;
	LedPatternType LDATA ledData;

	OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&ledData.ledSettings[LED1_INDEX].ledFollowMultiplierControl, (U8 XDATA_PTR)&ledCtlPtr->ledSettings[LED1_INDEX].ledFollowMultiplierControl, sizeof(LedPatternType));

	for(i = 0; i < LED_FILTER_DEFAULT_NO && gDriver_LED_ctl.ledFilterEableMask; i++)
	{
		if(gDriver_LED_ctl.ledFilterCtlPtr[i])
		{
			switch(gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.filterType)
			{
				case FILTER_USER_DEF:
					for(ledIndex = LED1_INDEX; ledIndex < LED_NO; ledIndex++)
					{
						if(gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.userDefineFilter.userdefData.ledSettings[ledIndex].onOff  != LED_DONT_CARE)
						{
							OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&ledData.ledSettings[ledIndex].ledFollowMultiplierControl, (U8 XDATA_PTR)&(gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.userDefineFilter.userdefData.ledSettings[ledIndex].ledFollowMultiplierControl), sizeof(LedSettingsType));
						}
					}
					break;
				case FILTER_MULTI_DIVIDE:
					for(ledIndex = LED1_INDEX; ledIndex < LED_NO; ledIndex++)
					{
						if(ledData.ledSettings[ledIndex].onOff != LED_DONT_CARE)
						{
							if(gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].isDivide)
							{
								ledData.ledSettings[ledIndex].t1 /= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t1;
								ledData.ledSettings[ledIndex].t2 /= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t2;
								ledData.ledSettings[ledIndex].t3 /= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t3;
								ledData.ledSettings[ledIndex].t4 /= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t4;
								rptT12 = ledData.ledSettings[ledIndex].pwmSetting & 0x0F;
								pwm =  ledData.ledSettings[ledIndex].pwmSetting & 0xF0;
								rptT12 /= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].RptT12;
								ledData.ledSettings[ledIndex].pwmSetting = (pwm |rptT12 );
							}
							else if(gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].isMultiply)
							{
								if((0xFF / gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t1)  < ledData.ledSettings[ledIndex].t1)
									break;
								if((0xFF / gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t2)  < ledData.ledSettings[ledIndex].t2)
									break;
								if((0xFF / gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t2)  < ledData.ledSettings[ledIndex].t2)
									break;
								if((0xFF / gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t2)  < ledData.ledSettings[ledIndex].t2)
									break;

								rptT12 = ledData.ledSettings[ledIndex].pwmSetting & 0x0F;
								if((0x0F / gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].RptT12)  < rptT12)
									break;
								ledData.ledSettings[ledIndex].t1 *= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t1;
								ledData.ledSettings[ledIndex].t2 *= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t2;
								ledData.ledSettings[ledIndex].t3 *= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t3;
								ledData.ledSettings[ledIndex].t4 *= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].t4;
								rptT12 = ledData.ledSettings[ledIndex].pwmSetting & 0x0F;
								pwm =  ledData.ledSettings[ledIndex].pwmSetting & 0xF0;
								rptT12 *= gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.multiDivideFilter.para[ledIndex].RptT12;
								ledData.ledSettings[ledIndex].pwmSetting = (pwm |rptT12 );
							}
						}
					}
					break;
				case FILTER_SWAP_CHANGE:
					if(gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.isSwap)
					{
						OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&tempLedData.ledFollowMultiplierControl, (U8 XDATA_PTR)&ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.swapTarget1].ledFollowMultiplierControl, sizeof(LedSettingsType));
						OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.swapTarget1].ledFollowMultiplierControl, (U8 XDATA_PTR)&ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.swapTarget2].ledFollowMultiplierControl, sizeof(LedSettingsType));
						OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.swapTarget2].ledFollowMultiplierControl, (U8 XDATA_PTR)&tempLedData.ledFollowMultiplierControl, sizeof(LedSettingsType));
					}
					if(gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.isChangeLED && ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.sourceLed].onOff == LED_ON)
					{
						OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.destinationLED].ledFollowMultiplierControl, (U8 XDATA_PTR)&ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.sourceLed].ledFollowMultiplierControl, sizeof(LedSettingsType));
						ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.swapChangeFilter.sourceLed].onOff = LED_OFF;
					}
					break;
				case FILTER_ADD:
					if(ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].onOff == LED_DONT_CARE || ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].onOff == LED_OFF)
					{
						if(ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].onOff != LED_OFF && ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].onOff != LED_DONT_CARE)
						{
							timeTemp = (gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.gapWithBgLed*2 + (gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.t1 + gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.t2)*(gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.RptT12+1))*2;
							if((U16)(ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].t3*5L) < timeTemp)
								break;

							timeTemp = ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].t1 + ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].t2;
							timeTemp= timeTemp *( (ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].pwmSetting &0x0F)+1);
							timeTemp += gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.gapWithBgLed;
							timeTemp /= 2;

							if(timeTemp > 0x00FF)
								break;

							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].t4 = (U8)timeTemp;
							if(ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].onOff == LED_ON)
								ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].onOff = LED_ALT_FIRST;

							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].ledFollowMultiplierControl = ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].ledFollowMultiplierControl;
							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].pwmSetting = (ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].pwmSetting & 0xf0) |gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.RptT12 ;
							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].t1 = gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.t1;
							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].t2 = gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.t2;
							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].rampOnStep = gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.rampOnStep;
							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].rampOffStep =  gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.rampOffStep;
							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].onOff = ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].onOff +1;


							timeTemp = ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].t1 + ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].t2;
							timeTemp= timeTemp * ((ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].pwmSetting &0x0F)+1)*2 + (U16)ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.targetLedForAdd].t3 * 2;
							timeTemp -= (((U16)gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.t1 + (U16)gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.t2)*((U16)gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.RptT12+1)*2);
							timeTemp /= 5;
							ledData.ledSettings[gDriver_LED_ctl.ledFilterCtlPtr[i]->filterSetting.addFilter.ledIndex].t3 = (U8)timeTemp;
						}
					}
					break;
			}
		}
	}


	if(gMMI_DRV_ctl.isMiscEnabled & IS_LED_DISABLED)
	{
		for(ledIndex = LED1_INDEX; ledIndex < LED_NO; ledIndex++)
		{
			ledData.ledSettings[ledIndex].onOff = LED_OFF;
		}
	}

	LED_SetPatternToHW(&ledData);
}

PRIVATE void DRIVER_LED_EnableFGLed(LedPatternType XDATA_PTR ledCtlPtr)
{
	U8 ledIndex;

	if(MMI_MEDIA_LED_WITH_FILTER_EFFECT_FEAT)
	{
		DRIVER_LED_EnableBGLed(ledCtlPtr);
		return;
	}
	if(gMMI_DRV_ctl.isMiscEnabled & IS_LED_DISABLED)
	{
		for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
		{
			ledCtlPtr->ledSettings[ledIndex].onOff = LED_OFF;
		}
	}

	LED_SetPatternToHW(ledCtlPtr);
}

PRIVATE void _DRIVER_LED_FGLEDEnded(U8 id)
{
	if(gMMI_Drv_Handler.fDriverFGLedEndEvent)
		gMMI_Drv_Handler.fDriverFGLedEndEvent(id);
}

PRIVATE void _DRIVER_LED_ClearQue(void)
{
	DriverMsg XDATA_PTR msgPtr;

	while(OSMQ_Entries(OSMQ_LED_Command_ptr))
	{
		msgPtr= (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_LED_Command_ptr);
		_DRIVER_LED_FGLEDEnded(msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.eventCode);
		OSMEM_Put((U8 XDATA_PTR)msgPtr);
	}
}

PRIVATE void _DRIVER_BGLedResume(void)
{
	U8 i;
	LedPatternType XDATA_PTR ledDataPtr;

	if(gDriver_LED_ctl.isLedPwrSaving )
		return;

	ledDataPtr = (LedPatternType XDATA_PTR)SYS_MemoryGet(sizeof(LedPatternType));
	if(ledDataPtr)
	{
		OSMEM_memset_xdata((U8 XDATA_PTR)ledDataPtr, 0, sizeof(LedPatternType));
		for(i = LED1_INDEX; i < LED_NO; i++)
		{
			if(gDriver_LED_ctl.fgLedOnOff[i] !=LED_DONT_CARE)
			{
				OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&ledDataPtr->ledSettings[i].ledFollowMultiplierControl, (U8 XDATA_PTR)&gDriver_LED_ctl.bgLedData.ledSettings[i].ledFollowMultiplierControl, sizeof(LedSettingsType));
			}
			else
				ledDataPtr->ledSettings[i].onOff = LED_DONT_CARE;

		}
		DRIVER_LED_EnableBGLed(ledDataPtr);
		OSMEM_Put((U8 XDATA_PTR) ledDataPtr);
	}
	else
		DRIVER_LED_EnableBGLed(&gDriver_LED_ctl.bgLedData);
}

PUBLIC void DRIVER_LED_Init(void)
{
	gDriver_LED_ctl.pDriverLedData = (DRIVER_LED_DATA_TYPE CODE_PTR) GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_LED_DATA_INIT);
	gDriver_LED_ctl.ledPatternNo = gDriver_LED_ctl.pDriverLedData->totalLedPatternNo;
	gDriver_LED_ctl.ledFilterNo = *((U8 CODE_PTR)&(gDriver_LED_ctl.pDriverLedData->totalLedPatternNo)+1+(sizeof(LedPatternType)*gDriver_LED_ctl.ledPatternNo));
}

PUBLIC void DRIVER_LED_Scheduler(void)
{
	U8 i;
	DriverMsg XDATA_PTR msgPtr;
	LedPatternType XDATA_PTR ledDataPtr;
	U8 ledDisplayIndex, isOverrideFilter, timeout, eventCode;

	//////////////////////////
	////////FG LEd ///////////
	//////////////////////////
	if(DRIVER_LED_IsFGLEDOn()) // FG Led is playing
	{
		if(!MMI_QUEUE_LED_FEAT)
			_DRIVER_LED_ClearQue();

		if(SYS_IsTimerExpired(&gDriver_LED_ctl.fgLedTimerPtr))
		{
			_DRIVER_LED_FGLEDEnded(gDriver_LED_ctl.fgLedMediaOpcode);

			if(gMMI_nvram.timerCtl.PowerSavingLightTimer)
				SYS_SetTimer(&gDriver_LED_ctl.ledPowerSavingTimer, gMMI_nvram.timerCtl.PowerSavingLightTimer*HALF_MIN);

			if(OSMQ_Entries(OSMQ_LED_Command_ptr))
				goto DRIVER_PLAY_FG_LED;
			else
				_DRIVER_BGLedResume();
		}
	}
	else  // No FG LED now
	{
		DRIVER_PLAY_FG_LED:
		msgPtr= (DriverMsg XDATA_PTR)OSMQ_Get(OSMQ_LED_Command_ptr);
		if(msgPtr)
		{
			// 1. Set Para
			ledDisplayIndex = msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.ledDisplayIndex;
			timeout = msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.timeout;
			eventCode = msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.eventCode;
			isOverrideFilter= msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.isOverrideFilter;
			OSMEM_Put((U8 XDATA_PTR) msgPtr);

			if(eventCode == MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT1 ||eventCode == MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT2 ||
				eventCode == MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT3||eventCode == MEDIA_EVT_POWER_ON)
			{
				if(gSYS_init_led_exist)
				{
					SYS_SetTimer(&gDriver_LED_ctl.fgLedTimerPtr,(timeout-5) * 320L);
					gDriver_LED_ctl.fgLedMediaOpcode = eventCode;
					return;
				}
			}

			// 2. Read LED stiings
			ledDataPtr = (LedPatternType XDATA_PTR)SYS_MemoryGet(sizeof(LedPatternType));
			if(ledDataPtr)
			{
				if(DRIVER_LED_LoadPattern(ledDisplayIndex,(U8 XDATA_PTR) ledDataPtr))
				{
					// 3. Check if Filter is on
					if(!DRIVER_LED_IsFilterOn() ||isOverrideFilter)
					{
						SYS_SetTimer(&gDriver_LED_ctl.fgLedTimerPtr,timeout * 320L);
						DRIVER_LED_EnableFGLed(ledDataPtr);
						gDriver_LED_ctl.fgLedMediaOpcode = eventCode;

						//Keep OnoffSettings
						for(i = LED1_INDEX; i < LED_NO; i++)
						{
							gDriver_LED_ctl.fgLedOnOff[i] = ledDataPtr->ledSettings[i].onOff;
						}

						// 4. Release power saving timer
						SYS_ReleaseTimer(&gDriver_LED_ctl.ledPowerSavingTimer);
						gDriver_LED_ctl.isLedPwrSaving = FALSE;
					}
				}
				OSMEM_Put((U8 XDATA_PTR) ledDataPtr);
			}
		}
	}
	
	//////////////////////////
	////////Check Timer ///////////
	//////////////////////////	
	if(SYS_IsTimerExpired(&gDriver_LED_ctl.ledPowerSavingTimer))
	{
		if(!DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_CHARGER_IN))
		{
			if(!gDriver_LED_ctl.isLedPwrSaving )
				LED_TurnOffAllLeds();

			gDriver_LED_ctl.isLedPwrSaving = TRUE;
		}
		else
		{
			gDriver_LED_ctl.isLedPwrSaving = FALSE;
		}
	}
}

PUBLIC void DRIVER_LED_PowerSavingExit(BOOL isResetTimer)
{
	if(gDriver_LED_ctl.isLedPwrSaving )
		DRIVER_LED_EnableBGLed(&gDriver_LED_ctl.bgLedData);
		
	if(isResetTimer)
		SYS_SetTimer(&gDriver_LED_ctl.ledPowerSavingTimer, gMMI_nvram.timerCtl.PowerSavingLightTimer*HALF_MIN);
		
	gDriver_LED_ctl.isLedPwrSaving = FALSE;
}

PUBLIC void DRIVER_Cmd_LED_UpdateDuration(U8 ledDuration)
{
	if(!MMI_LED_PWM_COMPENSATE_IN_DIFFERENT_BATTERY_LEVEL)
		return;
		
	LED_SetPwm(ledDuration);
}

PUBLIC void DRIVER_Cmd_LED_Restart(U8 para)
{
	_DRIVER_LED_ClearQue();
	if(para == DRIVER_CMD_LED_RESTART_PARA_ONLY_FG)
	{
		if(DRIVER_LED_IsFGLEDOn())
			DRIVER_LED_EnableBGLed(&gDriver_LED_ctl.bgLedData);
	}
	else if(para == DRIVER_CMD_LED_RESTART_PARA_FG_AND_BG)
	{
		DRIVER_LED_EnableBGLed(&gDriver_LED_ctl.bgLedData);
	}
	else
	{
		return;
	}

	if(DRIVER_LED_IsFGLEDOn())
	{
		SYS_ReleaseTimer(&gDriver_LED_ctl.fgLedTimerPtr);
		_DRIVER_LED_FGLEDEnded(gDriver_LED_ctl.fgLedMediaOpcode);
	}

	if(gMMI_nvram.timerCtl.PowerSavingLightTimer)
	{
		SYS_SetTimer(&gDriver_LED_ctl.ledPowerSavingTimer, gMMI_nvram.timerCtl.PowerSavingLightTimer*HALF_MIN);
		gDriver_LED_ctl.isLedPwrSaving = FALSE;
	}
}

PUBLIC void DRIVER_Cmd_LED_FGLed(U8 ledDataIndex, U8 timeout, U16 id, U8 isOverrideFilter)
{
	DriverMsg XDATA_PTR msgPtr;

	if((msgPtr = DRIVER_GetCmdMsg(DRIVER_FG_LED_CMD)) != (DriverMsg XDATA_PTR)NULL)
	{
		msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.ledDisplayIndex = ledDataIndex;
		msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.timeout = timeout;
		msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.eventCode = id;
		msgPtr->msgBodyPtr.driverCmd.fgLEDCmd.isOverrideFilter = isOverrideFilter;
		OSMQ_Put(OSMQ_LED_Command_ptr, (U8 XDATA_PTR)msgPtr);
	}
}

PUBLIC void DRIVER_Cmd_LED_BGLed(U8 ledDataIndex)
{
	U8 i;
	BOOL isPlayBgNow;
	LedPatternType XDATA_PTR ledDataPtr;
	
	if(ledDataIndex == LED_INVALID)
		return;

	if((ledDataPtr = (LedPatternType XDATA_PTR)SYS_MemoryGet(sizeof(LedPatternType))) == (LedPatternType XDATA_PTR)NULL)
		return;
	
	if(DRIVER_LED_LoadPattern((ledDataIndex & (~LED_OVERRIDE_BIT)), (U8 XDATA_PTR)ledDataPtr))
	{
		//Keep settings
		isPlayBgNow = TRUE;
		for(i = LED1_INDEX; i < LED_NO; i++)
		{
			if(ledDataPtr->ledSettings[i].onOff != LED_DONT_CARE)
			{
				OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&gDriver_LED_ctl.bgLedData.ledSettings[i].ledFollowMultiplierControl, (U8 XDATA_PTR)&ledDataPtr->ledSettings[i].ledFollowMultiplierControl, sizeof(LedSettingsType));
			}

			if(DRIVER_LED_IsFGLEDOn() && gDriver_LED_ctl.fgLedOnOff[i] !=LED_DONT_CARE && ledDataPtr->ledSettings[i].onOff != LED_DONT_CARE)
				isPlayBgNow = FALSE;
		}

		if (isPlayBgNow)
		{
			if(!gDriver_LED_ctl.isLedPwrSaving)
				DRIVER_LED_EnableBGLed(ledDataPtr);
		}
		else
		{
			// Need to be resume
			for(i = LED1_INDEX; i < LED_NO; i++)
			{
				if(ledDataPtr->ledSettings[i].onOff != LED_DONT_CARE)
					gDriver_LED_ctl.fgLedOnOff[i] = LED_ON;
			}
		}

	}
	OSMEM_Put((U8 XDATA_PTR) ledDataPtr);
}

PUBLIC void DRIVER_Cmd_LED_FilterLed(U8 type, U8 para)
{
	// 1. SetLEdFilterData
	DRIVER_LED_SetFilter(type, para);

	//2.
	if(!DRIVER_LED_IsFGLEDOn() && !gDriver_LED_ctl.isLedPwrSaving)
	{
		DRIVER_LED_EnableBGLed(&gDriver_LED_ctl.bgLedData);
	}
}

PUBLIC BOOL DRIVER_LED_IsFGLEDOn(void)
{
	return (gDriver_LED_ctl.fgLedTimerPtr) ? TRUE : FALSE;
}
#if !defined AB1520S && !defined AB1525S && !defined AB1520U
PUBLIC void DRIVER_LED_MapGpioSetup()
{
	U8 setCount, sfrValue, offset, ledCnt;
	LedMapSetting CODE_PTR ledIOSettingPtr;
           
	ledCnt = *((U8 CODE_PTR)&(gDriver_LED_ctl.pDriverLedData->totalLedPatternNo) + 1 + (sizeof(LedPatternType)*gDriver_LED_ctl.ledPatternNo) +
		1 + (sizeof(LedFilterSettingType)*gDriver_LED_ctl.ledFilterNo));
	//Set from Led Map GPIO
	for (setCount = 0; setCount < ledCnt; setCount++)
	{
		ledIOSettingPtr = (LedMapSetting CODE_PTR)((&gDriver_LED_ctl.pDriverLedData) + 1 + (sizeof(LedPatternType)*gDriver_LED_ctl.ledPatternNo) +
			1 + (sizeof(LedFilterSettingType) * gDriver_LED_ctl.ledFilterNo) + 1 + (sizeof(LedMapSetting) * setCount));
                      
		if(gMMI_nvram.keypadGpioCtl.keyMask & BIT_MASK32(ledIOSettingPtr->mapGpio))
		{
			LightDbgPrint("DRV - LED CheckConflick:%d",(U32) ledIOSettingPtr->mapGpio);
			continue;
		}
		
		if (ledIOSettingPtr->outputType == PWM3 || ledIOSettingPtr->outputType == PWM4)
		{
			offset = E3OFFS_RW_SIG_SEL_0304 + ((ledIOSettingPtr->mapGpio - 3) / 2);
			sfrValue = SFR_READ_BY_E3(offset);
			if(ledIOSettingPtr->mapGpio & 1)
			{
				sfrValue &= 0xF0;
				sfrValue |= ledIOSettingPtr->outputType;
			}
			else
			{
				sfrValue &= 0x0F;
				sfrValue |= (ledIOSettingPtr->outputType << 4);
			}
			OS_ENTER_CRITICAL();
			SFR_WRITE_BY_E3(offset, sfrValue);
			OS_EXIT_CRITICAL();
		}
	}
}
#endif