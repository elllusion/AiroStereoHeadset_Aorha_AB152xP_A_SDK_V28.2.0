#include "reside_flash.inc"

#define _MMI_CHGBAT_C_

#include "MMI.h"
#include "MMI_State.h"
#include "MMI_Driver.h"
#include "MMI_Protocol.h"
#include "MMI_HCI.h"
#include "MMI_ChgBat.h"
#include "..\LC\LC_Interface.h"
#include "..\LC\LC.h"
#include "..\LC\hal.h"
#include "drivers.h"
#include "..\ROM\DRIVER\drv_adc1.h"
#include "..\Driver\Driver_LED.h"
#include "..\Driver\Driver_Temp.h"

#ifdef AIRAPP_Profile
#include "..\AirApp\AirApp_interface.h"
#endif

#ifdef MMI_BATCHG_ENABLE

#include "..\SDK_Proxy\SDK_Proxy.h"
extern VFUN XDATA api_iosys_evt_func[API_GRP_IOSYS_CB_CNT];

PRIVATE void MMI_CHG_SetTimeOutDectionTimer(void)
{
	if (gMMI_nvram.timerCtl.CheckChargingTimer)
	{
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.checkChargingTimer,
						gMMI_nvram.timerCtl.CheckChargingTimer*0x57E400L);//unit 0.5 hours
	}
}

PRIVATE void MMI_CHG_ReleaseTimeOutDectionTimer(void)
{
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.checkChargingTimer);
}

PRIVATE void MMI_CHGBAT_PushBatChgMediaEvt(void)
{
	// Here the timer is use to prevent the condition that user plug in and out the charge rapidly.
	// we set timer to prevent the media events are sent a lot continuously.
	SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.delayChgMediaEvtTimer, (U32)(gMMI_nvram.timerCtl.DelayChgMediaEvtTimer*32));
}

PRIVATE void MMI_BAT_SetSwitchOffInLowBatTimer(void)
{
	if (gMMI_nvram.timerCtl.AutoSwitchOffInLowBatTimer && (gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer == (OST XDATA_PTR)NULL))
	{
		SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer,
		            (U32)(gMMI_nvram.timerCtl.AutoSwitchOffInLowBatTimer*192000));
	}
}

PRIVATE void MMI_BAT_ReleaseSwitchOffInLowBatTimer(void)
{
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffInLowBatTimer);
}

#define HW_CHARGER_STATUS_IDLE		0
#define HW_CHARGER_STATUS_TRICKLE	1
#define HW_CHARGER_STATUS_CC_MODE	2
#define HW_CHARGER_STATUS_CV_INIT	3
#define HW_CHARGER_STATUS_CV_MODE	4
#define HW_CHARGER_STATUS_COMPLETE	5
PRIVATE U8 MMI_CHG_GetStatus(void)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_CHG_EXT_CHG_STATUS])
	{
		return ((API_IOSYS_CHG_EXT_CHG_STATUS_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_CHG_EXT_CHG_STATUS])();
	}
	else
	{
		return DRV_CHARGER.fGetStatus();
	}
}

PUBLIC BOOL MMI_CHG_IsComplete(void)
{
	return (HW_CHARGER_STATUS_COMPLETE == MMI_CHG_GetStatus()) ? TRUE : FALSE;
}

PUBLIC void MMI_CHG_ResetTimeOutDectionTimer(void)
{
	if (gMMI_ctl.mmiTimerInfo.sysTimer.checkChargingTimer != (OST XDATA_PTR)NULL)
	{
		MMI_CHG_SetTimeOutDectionTimer();
	}
}

PUBLIC void MMI_CHGBAT_Init(void)
{
	MMI_PollingBatStatus();
	if (!MMI_IsChargerIn())
	{
		//Default shut down low bat should be zero,
		//if low bat threashold is not k by tool.
		if (MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.initBatVolt))
		{
			MMI_SystemOff();
		}
	}
}

PUBLIC BOOL MMI_BAT_IsVoltageBelow(VoltageType voltage)
{
#if defined(OPERATION_IN_BB_FPGA_VERSION)
	UNUSED(voltage);
	return FALSE;
#else
	return (DRV_ADC0.fVBat() < voltage) ? TRUE : FALSE;
#endif
}

PUBLIC U8 MMI_IsChargerIn(void)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_CHG_EXT_CHG_IN])
	{
		return ((API_IOSYS_CHG_EXT_CHG_IN_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_CHG_EXT_CHG_IN])();
	}
	else
	{
		return DRV_CHARGER.fIsChargerIn();
	}
}

PUBLIC void MMI_CHGBAT_BatteryCheckPoint(void)
{
	switch(MMI_BAT_GetLevelInQuater())
	{
		case IND_BATTERY_LOW:
		case IND_BATTERY_INTERVAL_0:
			MMI_PushMediaEvent(MEDIA_EVT_BATTERY_INTERVAL_0);
			MMI_PushMediaEvent(MEDIA_EVT_BAT_OK);
			break;
		case IND_BATTERY_INTERVAL_1:
			MMI_PushMediaEvent(MEDIA_EVT_BATTERY_INTERVAL_1);
			MMI_PushMediaEvent(MEDIA_EVT_BAT_OK);
			break;
		case IND_BATTERY_INTERVAL_2:
			MMI_PushMediaEvent(MEDIA_EVT_BATTERY_INTERVAL_2);
			MMI_PushMediaEvent(MEDIA_EVT_BAT_OK);
			break;
		case IND_BATTERY_INTERVAL_3:
			MMI_PushMediaEvent(MEDIA_EVT_BATTERY_INTERVAL_3);
			MMI_PushMediaEvent(MEDIA_EVT_BAT_OK);
			break;
		default:
			ASSERT(FALSE);
			break;
	}
}

extern PUBLIC void DRIVER_Cmd_LED_UpdateDuration(U8 ledDuration);
PUBLIC void MMI_CheckCurrentBatLevel(void)
{
	U8 ledDurationPara;

	if (gMMI_ctl.mmiBatInfo.batStatus >= MMI_BAT_CHARGER_IN)
	{//Already Charger-In do nothing now.
		if (!MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.lowBatVolt.lowBatLevel))
		{
			MMI_ClearActionOfPowerOnInLowBat();
		}
		if(MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.lowBatVolt.shutDownLowBat))
		{
			MMI_PowerOffHandler(POWEROFF_REASON_SHUT_DOWN_LOW_BAT);
		}
	}
	else
	{
		if (MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.lowBatVolt.shutDownLowBat))
		{
			MMI_UpdateBatStatus(MMI_BAT_LOW);
			MMI_PowerOffHandler(POWEROFF_REASON_SHUT_DOWN_LOW_BAT);
		}
		else if (MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.lowBatVolt.lowBatLevel))
		{
			//Under Low battery threshold
			MMI_UpdateBatStatus(MMI_BAT_LOW);
			if(gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryLEDtimer == (OST XDATA_PTR)NULL)
			{
				SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryLEDtimer, (U32)gMMI_nvram.batteryPara.lowBatteryLEDTimer * ONE_SEC);
				if(gMMI_nvram.batteryPara.lowBatteryLEDTimer != gMMI_nvram.batteryPara.lowBatteryRingTimer)
					SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.lowBatteryRingtimer, (U32)gMMI_nvram.batteryPara.lowBatteryRingTimer * ONE_SEC);

				MMI_PushMediaEvent(MEDIA_EVT_BAT_LOW_LED);
				MMI_PushMediaEvent(MEDIA_EVT_BAT_LOW_RING);

			}

			MMI_BAT_SetSwitchOffInLowBatTimer();
			MMI_CHGBAT_PushBatChgMediaEvt();
		}
		else
		{
			MMI_UpdateBatStatus(MMI_BAT_WELL);
			MMI_ClearActionOfPowerOnInLowBat();
		}
	}
	
	switch(MMI_BAT_GetLevelInQuater())
	{
		case IND_BATTERY_LOW:
		case IND_BATTERY_INTERVAL_0:
			ledDurationPara = gMMI_nvram.batteryPara.ledDuration.level0;
			break;
		case IND_BATTERY_INTERVAL_1:
			ledDurationPara = gMMI_nvram.batteryPara.ledDuration.level1;
			break;
		case IND_BATTERY_INTERVAL_2:
			ledDurationPara = gMMI_nvram.batteryPara.ledDuration.level2;
			break;
		case IND_BATTERY_INTERVAL_3:
			ledDurationPara = gMMI_nvram.batteryPara.ledDuration.level3;
			break;
		default:
			return;
	}

	if (gMMI_ctl.mmiBatInfo.currentLedDuration != ledDurationPara)
	{
		gMMI_ctl.mmiBatInfo.currentLedDuration = ledDurationPara;
		DRIVER_Cmd_LED_UpdateDuration(ledDurationPara);
	}
}

PUBLIC void MMI_PollingBatStatus(void)
{
	if (MMI_IsChargerIn())
	{
		//Charger in now.

		//issue1608

		if(MMI_CHG_IsComplete()) //charging complete now.
		{
			if (gMMI_ctl.mmiBatInfo.batStatus != MMI_BAT_CHARGING_FULL)
			{
				MMI_UpdateBatStatus(MMI_BAT_CHARGING_FULL);
				MMI_CHGBAT_PushBatChgMediaEvt();
			}
		}
		else //charging is not complete yet.
		{
			// the moment when charge is plug in
			if (gMMI_ctl.mmiBatInfo.batStatus != MMI_BAT_CHARGER_IN && gMMI_ctl.mmiBatInfo.batStatus != MMI_BAT_CHARGER_RECHARGE)
			{
				if (gMMI_ctl.mmiBatInfo.batStatus == MMI_BAT_CHARGING_FULL)
				{
					MMI_UpdateBatStatus(MMI_BAT_CHARGER_RECHARGE);
				}
				else
				{
					MMI_UpdateBatStatus(MMI_BAT_CHARGER_IN);
				}
				MMI_CHGBAT_PushBatChgMediaEvt();
				MMI_CHG_SetTimeOutDectionTimer();
				MMI_BAT_ReleaseSwitchOffInLowBatTimer();
			}
		}
		
		if (!MMI_IS_POWER_ON())
		{
			if (!MMI_CheckNestState(MMI_EOF, MMI_FAKEOFF))
			{
				MMI_AddTopState(MMI_EOF, MMI_FAKEOFF);
			}
		}
	}
	else
	{
		//Charger out now
		if (gMMI_ctl.mmiBatInfo.batStatus >= MMI_BAT_CHARGER_IN)
		{
			MMI_UpdateBatStatus(MMI_BAT_WELL);
			DRIVER_TEMP_SetChargeNormal();
			MMI_CHG_ReleaseTimeOutDectionTimer();
			MMI_CHGBAT_PushBatChgMediaEvt();

			if (MMI_CheckNestState(MMI_EOF, MMI_FAKEOFF))
			{
				if(MMI_CHARGER_OUT_THEN_POWER_ON_FEAT)
				{
					MMI_PowerOnLoader();
				}
				else
				{
					MMI_SystemOff();
				}
			}
		}
	}
}

PUBLIC void MMI_UpdateBatStatus(U8 batStatus)
{
	if(gMMI_ctl.mmiBatInfo.batStatus != batStatus)
	{
		MMI_NotifyDriverBatteryStatus(batStatus);
		
		if(batStatus == MMI_BAT_WELL || batStatus == MMI_BAT_LOW)
		{
			if(gMMI_ctl.mmiBatInfo.batStatus >= MMI_BAT_CHARGER_IN && gMMI_ctl.mmiBatInfo.batStatus <= MMI_BAT_CHARGING_FULL)
			{
				if(api_iosys_evt_func[API_GRP_IOSYS_CB_CHG_STATUS])
				{
					((API_IOSYS_CHG_STATUS_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_CHG_STATUS])(0x01);
				}
			}
			
			if(batStatus == MMI_BAT_LOW)
			{
				if(api_iosys_evt_func[API_GRP_IOSYS_CB_BAT_LOW])
				{
					((API_IOSYS_BAT_LOW_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_BAT_LOW])();
				}			
			}
		}
		else
		{
			if(api_iosys_evt_func[API_GRP_IOSYS_CB_CHG_STATUS])
			{
				((API_IOSYS_CHG_STATUS_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_CHG_STATUS])(batStatus);
			}
		}
		
		#ifdef AIRAPP_Profile
		AirApp_ChgBatStatusUpdateEvent(batStatus);
		#endif
	}
	
	gMMI_ctl.mmiBatInfo.batStatus = batStatus;
}

PUBLIC U8 MMI_BAT_GetLevelInPercent(void)
{
	U8 i;
	U16 lowBd, highBd;
	U16 batLevel = DRV_ADC0.fVBat();

	for( i = 0; i < MMI_IPHONE_BATTERY_LEVELS; i++)
	{
		if(batLevel < pMpParameter->MP_BatteryPara.batteryIPhoneCheckPoints[i])
			break;
	}

	if(i == 0)
	{
		lowBd = pMpParameter->MP_BatteryPara.lowBatVolt.shutDownLowBat;
		highBd = pMpParameter->MP_BatteryPara.batteryIPhoneCheckPoints[0];
		if(batLevel < lowBd)
			return 0;		
	}
	else if(i == MMI_IPHONE_BATTERY_LEVELS)
	{
		lowBd = pMpParameter->MP_BatteryPara.batteryIPhoneCheckPoints[MMI_IPHONE_BATTERY_LEVELS-1];
		highBd = 2 * pMpParameter->MP_BatteryPara.batteryIPhoneCheckPoints[MMI_IPHONE_BATTERY_LEVELS-1] - pMpParameter->MP_BatteryPara.batteryIPhoneCheckPoints[MMI_IPHONE_BATTERY_LEVELS-2];
		if(batLevel >= highBd)
			return 100;
	}
	else
	{
		lowBd = pMpParameter->MP_BatteryPara.batteryIPhoneCheckPoints[i-1];
		highBd = pMpParameter->MP_BatteryPara.batteryIPhoneCheckPoints[i];
	}

	return (U8)((U16)((10 * (U32)(batLevel - lowBd)) / (U32)(highBd - lowBd))+ (U16)i*10);
}

PUBLIC U8 MMI_BAT_GetLevelInQuater(void)
{
	if(MMI_CHGBAT_GetBatStatus() == MMI_BAT_LOW)
		return IND_BATTERY_LOW;
	else if (MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.checkpoints.checkpoint1))
		return IND_BATTERY_INTERVAL_0;
	else if(MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.checkpoints.checkpoint2))
		return IND_BATTERY_INTERVAL_1;
	else if(MMI_BAT_IsVoltageBelow(pMpParameter->MP_BatteryPara.checkpoints.checkpoint3))
		return IND_BATTERY_INTERVAL_2;
	else
		return IND_BATTERY_INTERVAL_3;
}

PUBLIC U16 MMI_BAT_GetLowBatLevel(void)
{
	return pMpParameter->MP_BatteryPara.lowBatVolt.lowBatLevel;
}

PUBLIC U16 MMI_BAT_GetShutDownBatLevel(void)
{
	return pMpParameter->MP_BatteryPara.lowBatVolt.shutDownLowBat;
}
#endif
