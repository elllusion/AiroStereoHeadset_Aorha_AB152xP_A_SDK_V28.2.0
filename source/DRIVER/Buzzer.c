//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#define BUZZER_C
#include "rc.h"
#include "bt_config_profile.h"

#ifdef BUZZER_ENABLE

#include "sector_config1.h"
#include "Buzzer.h"

extern PUBLIC void DRIVER_SendBuzzerEndedEvent(U8 id);

PRIVATE void Buzzer_ReadSector(U8 buzzerIndex);
PRIVATE void Buzzer_EnableTimer1(void);

PUBLIC void Buzzer_Init(void)
{
	gBuzzer_ctl.pDriverBuzzerData = (DRIVER_BUZZER_DATA_TYPE CODE_PTR) GET_SECTOR_CFG1_ADDR(SECTOR_DRIVER_BUZZER_DATA_INIT);
}

PUBLIC BOOL Buzzer_Start(U8 buzzerIndex, U8 mediaEvent)
{
	//1.allocate new memory for gBuzzer_ctl.buzzDataPtr
	if(gBuzzer_ctl.buzzDataPtr == (BuzzType XDATA_PTR)NULL)
	{
		gBuzzer_ctl.buzzDataPtr = (BuzzType XDATA_PTR)OSMEM_Get(OSMEM_ptr1);
	}
	
	if(gBuzzer_ctl.buzzDataPtr)
	{
		//2.release the previous buzzer setting
		OSMEM_memset_xdata ((U8 XDATA_PTR)gBuzzer_ctl.buzzDataPtr, 0, OSMEM1_BLKSIZE);
		gBuzzer_ctl.buzzDataPtr->mediaEventCode = mediaEvent;
		
		//3.load the current buzzer setting
		if(buzzerIndex != BUZZER_OFF)
		{
			Buzzer_ReadSector(buzzerIndex);
			gBuzzer_ctl.buzzDataPtr->isOn = TRUE;
			gBuzzer_ctl.buzzDataPtr->state = BUZZ_BEEP_SET_ON_PARA;
			gBuzzer_ctl.buzzDataPtr->IndexCount = 0;
			gBuzzer_ctl.buzzDataPtr->baseCntDown = gBuzzer_ctl.buzzDataPtr->buzzPattern.WindowCyc;
			SetPowerSavingForDevice(FALSE, PWRSAV_BUZZER);
			Buzzer_EnableTimer1();
		}
		return TRUE;
	}
	return FALSE;		
}


PRIVATE void Buzzer_ReadSector(U8 buzzerIndex)
{
	OSMEM_memcpy_xdata_code ((U8 XDATA_PTR)&gBuzzer_ctl.buzzDataPtr->buzzPattern,
		(U8 CODE_PTR)&(gBuzzer_ctl.pDriverBuzzerData->totalBuzzerNo) + 1 + buzzerIndex*sizeof(BuzzPatternType),
		sizeof(BuzzPatternType));
}

PUBLIC void Buzzer_SetFreq(U8 freq)
{
	gBuzzer_ctl.buzzDataPtr->regVal &= 0x8F;
	gBuzzer_ctl.buzzDataPtr->regVal |= (freq << 4);
	OS_ENTER_CRITICAL();
	LC_SFR_WRITE_BY_FE(FEOFFS_WO_BUZZER_CTRL, gBuzzer_ctl.buzzDataPtr->regVal);
	OS_EXIT_CRITICAL();
}

PUBLIC void Buzzer_SwitchOnOff(U8 isOn)
{
	//bit 1 should be always high
	if (isOn)
		gBuzzer_ctl.buzzDataPtr->regVal |= 0x03;
	else
		gBuzzer_ctl.buzzDataPtr->regVal &= 0xFE;
	OS_ENTER_CRITICAL();
	LC_SFR_WRITE_BY_FE(FEOFFS_WO_BUZZER_CTRL, gBuzzer_ctl.buzzDataPtr->regVal);
	OS_EXIT_CRITICAL();
}

PRIVATE void Buzzer_EnableTimer1(void)
{
	ET1=1;// Timer1Interrupt Enable
	TMOD = ( ( TMOD & 0x0F ) | 0x10 );//Timer1 mode = 1 16bit


	//count to 65536, ( (50000) * ( 1 / (12M/12) )  ) = 50ms
	TL1 = U16_LSB(15536);
	TH1 = U16_MSB(15536);

	TR1=1;//Timer1 count Enable
}

PUBLIC void Buzzer_DisableTimer1(void)
{
	ET1=0;// Timer1Interrupt Enable
	TR1=0;//Timer1 count Enable
}

PUBLIC void BuzzerTimer1Int (void) INTERRUPT(3)
{
#ifdef __C251__
	U8 c2_offset;
#endif

	_push_ (SFR_RW_C2_OFFSET);

	if(gBuzzer_ctl.buzzDataPtr->isOn)
	{
		switch(gBuzzer_ctl.buzzDataPtr->state)
		{
			case BUZZ_BEEP_SET_ON_PARA:
				FLAG_BUZZER_START:
				gBuzzer_ctl.buzzDataPtr->timerCntDown = gBuzzer_ctl.buzzDataPtr->buzzPattern.Buzzsettings[gBuzzer_ctl.buzzDataPtr->IndexCount].Window;
				Buzzer_SetFreq(gBuzzer_ctl.buzzDataPtr->buzzPattern.Buzzsettings[gBuzzer_ctl.buzzDataPtr->IndexCount].Freq);
				Buzzer_SwitchOnOff(TRUE);
				gBuzzer_ctl.buzzDataPtr->state = BUZZ_BEEP_START;
				break;

			case BUZZ_BEEP_START:
				gBuzzer_ctl.buzzDataPtr->timerCntDown--;
				if(!gBuzzer_ctl.buzzDataPtr->timerCntDown)
				{
					gBuzzer_ctl.buzzDataPtr->timerCntDown = gBuzzer_ctl.buzzDataPtr->buzzPattern.Buzzsettings[gBuzzer_ctl.buzzDataPtr->IndexCount].Rest;
					Buzzer_SwitchOnOff(FALSE);
					gBuzzer_ctl.buzzDataPtr->state = BUZZ_BEEP_REST;
				}
				break;

			case BUZZ_BEEP_REST:
				gBuzzer_ctl.buzzDataPtr->timerCntDown--;
				if(!gBuzzer_ctl.buzzDataPtr->timerCntDown)
				{
					if(gBuzzer_ctl.buzzDataPtr->IndexCount < (gBuzzer_ctl.buzzDataPtr->buzzPattern.BeepInWindow-1))
					{
						gBuzzer_ctl.buzzDataPtr->IndexCount++;
						goto FLAG_BUZZER_START;
					}
					else
					{
						if(gBuzzer_ctl.buzzDataPtr->baseCntDown)
						{
							gBuzzer_ctl.buzzDataPtr->baseCntDown--;
							gBuzzer_ctl.buzzDataPtr->IndexCount = 0;
							goto FLAG_BUZZER_START;
						}
						else
						{
							if(gBuzzer_ctl.buzzDataPtr->buzzPattern.Interval)
							{
								gBuzzer_ctl.buzzDataPtr->timerCntDown = gBuzzer_ctl.buzzDataPtr->buzzPattern.Interval;
								Buzzer_SwitchOnOff(FALSE);
								gBuzzer_ctl.buzzDataPtr->state = BUZZ_BEEP_INTERVAL;
							}
							else
							{
								goto DRIVER_SET_BUZZ_BASE_ON;
							}
						}
					}
				}
				break;

			case BUZZ_BEEP_INTERVAL:
				gBuzzer_ctl.buzzDataPtr->timerCntDown--;
				if(!gBuzzer_ctl.buzzDataPtr->timerCntDown)
				{
					DRIVER_SET_BUZZ_BASE_ON:
					if(gBuzzer_ctl.buzzDataPtr->buzzPattern.BigCycle)
					{
						gBuzzer_ctl.buzzDataPtr->buzzPattern.BigCycle--;
						gBuzzer_ctl.buzzDataPtr->IndexCount=0;
						gBuzzer_ctl.buzzDataPtr->baseCntDown = gBuzzer_ctl.buzzDataPtr->buzzPattern.WindowCyc;
						goto FLAG_BUZZER_START;
					}
					else
					{
						goto DRIVER_SWITCHOFF_BUZZER;
					}
				}
				break;


		}
	}
	else
	{
		DRIVER_SWITCHOFF_BUZZER:

		Buzzer_SwitchOnOff(FALSE);
		Buzzer_DisableTimer1();
		DRIVER_SendBuzzerEndedEvent(gBuzzer_ctl.buzzDataPtr->mediaEventCode);
		SYS_MemoryRelease(U8_XDATA_PTR_PTR_CASTING &gBuzzer_ctl.buzzDataPtr);
	}
	_pop_ (SFR_RW_C2_OFFSET);
}

#endif