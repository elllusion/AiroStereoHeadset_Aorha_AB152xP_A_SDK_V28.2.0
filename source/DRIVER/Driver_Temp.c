#include "reside_flash.inc"

#include "rc.h"
#include "bt_config_profile.h"

#include "drv_sector.h"
#include "drv_adc1.h"
#if (defined AB1520S) || (defined AB1520U)
#include "..\ABSDRIVER\drv_adc0_flash.h"
#endif
#include "drivers.h"
#include "..\MMI\MMI_Interface.h"
#include "..\MMI\MMI_Base.h"
#include "Driver_Temp.h"
#include "..\ABSDRIVER\drv_sector_flash.h"
#include "..\MMI\MMI_ChgBat.h"
#include "..\MMI\MMI_AirModule.h"
#include "..\ABSDRIVER\drv_gpio_flash.h"


#if (defined AB1520C) || (defined AB1520D)
#define ADC_SIGNED_BIT 				0x1000
#define COMPLEMENT_2_13_TO_16BIT 	0xE000
#elif (defined AB1520S) || (defined AB1520U)
#define ADC_DATA_MASK_10BIT 0x03FF 
#endif

#define ADC_3WIRE_CTL_BIT			0xFC00
#define ADC_3WIRE_DATA_BIT			0x03FF
#define CURRENT_DISCNT  			2 
#define CHARGING_OFFSET_VALUE 65

enum
{
	TEMP_DETECT_DISABLED,
	TEMP_DETECT_ENABLED_INIT,
	TEMP_DETECT_ENABLED_WAITING_SIGNAL_STABLE,
	TEMP_DETECT_ENABLED_DETECTING,
	TEMP_DETECT_ENABLED_CHECK_STATUS,
	TEMP_DETECT_ENABLED_SLEEP,		
};

enum
{
	TEMP_DETECT_NTC_RATIO_DISABLED,
	TEMP_DETECT_NTC_RATIO_ENABLED_INIT,
	TEMP_DETECT_NTC_RATIO_ENABLED_WAITING_READ_ADC_VALUE1,
	TEMP_DETECT_NTC_RATIO_ENABLED_WAITING_READ_ADC_VALUE2,
	TEMP_DETECT_NTC_RATIO_ENABLED_CHECK_STATUS,
	TEMP_DETECT_NTC_RATIO_ENABLED_SLEEP,		
};

enum
{
	VOLTAGE_LOW,
	VOLTAGE_HIGH,
};

enum
{
	CHARGING_NORMAL_EVENT = 0,
	CHARGING_ABNORMAL_EVENT,
	CHARGING_STOP_EVENT,
	CHARGE_KEEP_PREV_EVENT,
	CHARGING_EVENT_NO,
};

enum
{
	AIO_0_NTC,
	AIO_1_NTC,
};

#ifdef TWO_STEP_CHARGING_FUNCTION
PRIVATE void DRIVER_TEMP_TwoStepsNormal(void);
PRIVATE void DRIVER_TEMP_TwoStepsAbnormal(void);
#endif
PRIVATE S16 DRIVER_TEMP_ParaTypeTransform(U32 tempValue);
PRIVATE void DRIVER_TEMP_ExchangeRatioValue(FLOAT XDATA_PTR ptr1, FLOAT XDATA_PTR ptr2);
PRIVATE void DRIVER_TEMP_ExchangeValue(S16 XDATA_PTR ptr1, S16 XDATA_PTR ptr2);
PRIVATE S16 DRIVER_TEMP_NTCGetADCValue(U8 aioSource);

NTC_VFUN_TYPE CODE gADCFunctionHandler=
{
DRIVER_TEMP_ADC_Init,
DRIVER_TEMP_ReadADC,
DRIVER_TEMP_ParaTypeTransform,
};

DRIVER_TEMP_CTL_TYPE XDATA gDriver_Bat_Temp_ctl;


PUBLIC void DRIVER_TEMP_ADC_Init(U8 channel)
{
#if (defined AB1520C) || (defined AB1520D)
	DRV_ADC1_Init(channel);
	//UNUSED(channel);
	
#elif (defined AB1520S) || (defined AB1520U)
	if(channel == AIO_0_NTC)
		DRV_ADC0_AIO4_Init();
	else
		DRV_ADC0_AIO5_Init();
#else
	UNUSED(channel);
#endif
}

PUBLIC U16 DRIVER_TEMP_ReadADC(U8 channel)
{
#if (defined AB1520C) || (defined AB1520D)
	return DRV_ADC1_ChannelRead(channel);
#elif (defined AB1520S) || (defined AB1520U)
	if(channel == AIO_0_NTC)
		return DRV_ADC0_ReadAIO4();
	else if(channel == AIO_1_NTC)
		return DRV_ADC0_ReadAIO5();
	else			
		return 0;
#else
	UNUSED(channel);
	return 0;
#endif

}

PRIVATE S16 DRIVER_TEMP_ParaTypeTransform(U32 gTempValue)
{
#if (defined AB1520C) || (defined AB1520D)
	return ((U16)gTempValue& ADC_SIGNED_BIT) ? ((S16)gTempValue | COMPLEMENT_2_13_TO_16BIT) : ((S16)gTempValue);
#elif (defined AB1520S) || (defined AB1520U)
	return ((U16)gTempValue & ADC_DATA_MASK_10BIT);
#else
	UNUSED(gTempValue);
	return 0;
#endif
}


PRIVATE U8 DRIVER_TEMP_ChargerAction (void)
{	
	S16 chargingRiskHighBound, chargingSecureHighBound, chargingRechargeHighBound;
	S16 chargingRechargeLowBound, chargingSecureLowBound, chargingRiskLowBound;

	chargingRiskHighBound 		= pMpParameter->MP_BatteryPara.chargingInfo.chargingRiskHighBound;
	chargingSecureHighBound 	= pMpParameter->MP_BatteryPara.chargingInfo.chargingSecureHighBound;
	chargingRechargeHighBound 	= pMpParameter->MP_BatteryPara.chargingInfo.chargingRechargeHighBound;
	chargingRechargeLowBound 	= pMpParameter->MP_BatteryPara.chargingInfo.chargingRechargeLowBound;
	chargingSecureLowBound 	= pMpParameter->MP_BatteryPara.chargingInfo.chargingSecureLowBound;
	chargingRiskLowBound 		= pMpParameter->MP_BatteryPara.chargingInfo.chargingRiskLowBound;

	if(chargingRiskHighBound < chargingRiskLowBound)
	{
		DRIVER_TEMP_ExchangeValue((S16 XDATA_PTR)&chargingRiskHighBound, (S16 XDATA_PTR)&chargingRiskLowBound);
		DRIVER_TEMP_ExchangeValue((S16 XDATA_PTR)&chargingSecureHighBound, (S16 XDATA_PTR)&chargingSecureLowBound);
		DRIVER_TEMP_ExchangeValue((S16 XDATA_PTR)&chargingRechargeHighBound, (S16 XDATA_PTR)&chargingRechargeLowBound);
	}
	
	LightDbgPrint("NTC ADC data %X",(U32)gDriver_Bat_Temp_ctl.signTempADCValue);
	if(gDriver_Bat_Temp_ctl.signTempADCValue > chargingRiskHighBound || gDriver_Bat_Temp_ctl.signTempADCValue < chargingRiskLowBound)
	{
		LightDbgPrint("NTC stop event");
		return (U8)CHARGING_STOP_EVENT;
	}			
	else if(gDriver_Bat_Temp_ctl.signTempADCValue > chargingSecureHighBound  && gDriver_Bat_Temp_ctl.signTempADCValue < chargingRiskHighBound)
	{
		LightDbgPrint("NTC abnormal event1");
		return (U8)CHARGING_ABNORMAL_EVENT;
	}
	else if(gDriver_Bat_Temp_ctl.signTempADCValue < chargingSecureLowBound && gDriver_Bat_Temp_ctl.signTempADCValue > chargingRiskLowBound)
	{
		
		LightDbgPrint("NTC abnormal event2");
		return (U8)CHARGING_ABNORMAL_EVENT;
	}
	else if(gDriver_Bat_Temp_ctl.signTempADCValue < chargingRechargeHighBound && gDriver_Bat_Temp_ctl.signTempADCValue > chargingRechargeLowBound)
	{
		LightDbgPrint("NTC normal event");
		return (U8)CHARGING_NORMAL_EVENT;
	}
	else 
	{
		LightDbgPrint("NTC keep event");
		return (U8)CHARGE_KEEP_PREV_EVENT;
	}
}

PRIVATE U16 DRIVER_TEMP_NTCCurrentDiscnt (U16 ADCData)
{
#if (defined AB1520C) || (defined AB1520D)
	U16 DisCntCurrent;
	U16 DisADCData;
	U32 tempData;

	tempData = (U32)(ADCData & ADC_3WIRE_DATA_BIT)*(U32)(pMpParameterF->discountNumber);
	DisCntCurrent=(U16)(tempData/100);
	DisADCData=(ADCData & ADC_3WIRE_CTL_BIT) | (DisCntCurrent & ADC_3WIRE_DATA_BIT);

	return DisADCData;

#elif (defined AB1520S) || (defined AB1520U)
	U16 DisCntCurrent;
	U16 DisADCData;
	U32 tempData;

	tempData = (U32)(ADCData & ADC_3WIRE_DATA_BIT)*(U32)(pMpParameterF->discountNumber);
	DisCntCurrent=(U16)(tempData/100);
	DisADCData=(ADCData & ADC_3WIRE_CTL_BIT) | (DisCntCurrent & ADC_3WIRE_DATA_BIT);

	return DisADCData;
#else
	UNUSED(ADCData);
	return 0;
#endif
}

#define CHG_HW_RECHARGING	0x0000
#define CHG_HW_STOPCHARGING	0x0010

PRIVATE void DRIVER_TEMP_HWStopCharging(void)
{
	SYS_3WireWrite( 0x2E, (U8)(CHG_HW_STOPCHARGING>> 8), (U8)CHG_HW_STOPCHARGING);
}

PRIVATE void DRIVER_TEMP_HWReCharging(void)
{
	SYS_3WireWrite( 0x2E, (U8)( CHG_HW_RECHARGING>> 8), 	(U8)CHG_HW_RECHARGING);
}

PRIVATE void DRIVER_TEMP_HWDACCurrent(U16 value)
{
	SYS_3WireWrite( 0x24, (U8)(value>> 8), (U8)value);
}

#ifdef TWO_STEP_CHARGING_FUNCTION
PRIVATE void DRIVER_TEMP_HWCurrentControl(U16 value)
{
	SYS_3WireWrite( 0x2C, 0x00, (U8)value);
}
#endif

PRIVATE void DRIVER_TEMP_ChargerHWCtl(U8 ctlIdx)
{
	U16 ChargerDacCurrent;
	MP_ChargerDiscountCtl CODE_PTR discountCtl;

	switch(ctlIdx)
	{
		case CHARGING_NORMAL_STATE:
			DRIVER_TEMP_HWStopCharging();
			LightDbgPrint("charging normal state");
			#ifdef TWO_STEP_CHARGING_FUNCTION
			if (pMpParameterF->twoStepPara.enableTwoStepCharger)
			{
				DRIVER_TEMP_TwoStepsNormal();
			}
			else
			#endif
			{
				discountCtl = &pMpParameter->MP_ChargerDiscountCtl[CHARGING_CTL_NORMAL_IND];
				SYS_3WireRead(0x24, gDriver_Bat_Temp_ctl.mpValue0x24);
				DRIVER_TEMP_HWDACCurrent((discountCtl->chargerDacCurrent & 0x03FF) |(gDriver_Bat_Temp_ctl.mpValue0x24 & 0xFC00));
				OS_delay_us(33);
			}
			DRIVER_TEMP_HWReCharging();
			break;
		case CHARGING_ABNORMAL_STATE:
			DRIVER_TEMP_HWStopCharging();

			#ifdef TWO_STEP_CHARGING_FUNCTION
			if (pMpParameterF->twoStepPara.enableTwoStepCharger)
			{	
				DRIVER_TEMP_TwoStepsAbnormal();
			}
			else
			#endif
			{
				discountCtl = &pMpParameter->MP_ChargerDiscountCtl[CHARGING_CTL_ABNORMAL_IND];
				ChargerDacCurrent	= DRIVER_TEMP_NTCCurrentDiscnt (discountCtl->chargerDacCurrent);
				SYS_3WireRead(0x24, gDriver_Bat_Temp_ctl.mpValue0x24);
				DRIVER_TEMP_HWDACCurrent((ChargerDacCurrent & 0x03FF) |(gDriver_Bat_Temp_ctl.mpValue0x24 & 0xFC00));
				OS_delay_us(33);
			}
			DRIVER_TEMP_HWReCharging();
			break;
		case CHARGING_STOP_STATE:
			LightDbgPrint("charging stop state");
			DRIVER_TEMP_HWStopCharging();
			break;

		case CHARGING_UNKNOW_STATE:
		default:
			break;
	}
}

PUBLIC void DRIVER_TEMP_SetChargeNormal(void)
{
	if(gDriver_Bat_Temp_ctl.chargingState == CHARGING_NORMAL_STATE)
		return;

	DRIVER_TEMP_ChargerHWCtl(CHARGING_NORMAL_STATE);
	MMI_PushMediaEvent(MEDIA_EVT_BAT_SECURE_TEMP);
	gDriver_Bat_Temp_ctl.chargingState = CHARGING_NORMAL_STATE;
}

PRIVATE void DRIVER_TEMP_SetChargeAbnormal(void)
{
	if(gDriver_Bat_Temp_ctl.chargingState == CHARGING_ABNORMAL_STATE || gDriver_Bat_Temp_ctl.chargingState == CHARGING_STOP_STATE)
		return;

	DRIVER_TEMP_ChargerHWCtl(CHARGING_ABNORMAL_STATE);
	MMI_PushMediaEvent(MEDIA_EVT_BAT_DISCOUNT_TEMP);
	gDriver_Bat_Temp_ctl.chargingState = CHARGING_ABNORMAL_STATE;
}

PRIVATE void DRIVER_TEMP_SetChargeStop (void)
{
	if(gDriver_Bat_Temp_ctl.chargingState == CHARGING_STOP_STATE)
		return;

	DRIVER_TEMP_ChargerHWCtl(CHARGING_STOP_STATE);
	MMI_PushMediaEvent(MEDIA_EVT_BAT_RISKY_TEMP);
	gDriver_Bat_Temp_ctl.chargingState = CHARGING_STOP_STATE;
}

PUBLIC void DRIVER_TEMP_Check(void)
{
	//No temp detect
	if(gMMI_nvram.mcuADCSource != MCU_SOURCE_EXTERNAL)
		return;

	if(pMpParameterF->ntcRatioPara.enableNTCRatio)
		return;

	switch(gDriver_Bat_Temp_ctl.detectorState)
	{
		case TEMP_DETECT_DISABLED:
			gADCFunctionHandler.fADCInit(gMMI_nvram.batteryPara.tempInfo.tempDetectorSource);
			gDriver_Bat_Temp_ctl.detectorState = TEMP_DETECT_ENABLED_INIT;
			break;

		case TEMP_DETECT_ENABLED_INIT:
			if(pMpParameterF->ntcRatioPara.channelSet.pioVCCChannel != HV_MV_CHANNEL)
			{
				DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, VOLTAGE_HIGH, pMpParameterF->ntcRatioPara.channelSet.pioVCCChannel);
			}
			SYS_SetTimer(&gDriver_Bat_Temp_ctl.detectorTimer, gMMI_nvram.batteryPara.tempInfo.tempDetectorOnWaitStableTime * (ONE_SEC/100));
			gDriver_Bat_Temp_ctl.detectorState = TEMP_DETECT_ENABLED_WAITING_SIGNAL_STABLE;
			break;

		case TEMP_DETECT_ENABLED_WAITING_SIGNAL_STABLE:
			if(SYS_IsTimerExpired(&gDriver_Bat_Temp_ctl.detectorTimer))
			{
				OS_ENTER_CRITICAL ();
				gDriver_Bat_Temp_ctl.tempValue = DRIVER_TEMP_NTCGetADCValue(gMMI_nvram.batteryPara.tempInfo.tempDetectorSource);
				OS_EXIT_CRITICAL ();

				SYS_SetTimer(&gDriver_Bat_Temp_ctl.detectorTimer, gMMI_nvram.batteryPara.tempInfo.tempDetectorDetectingTime* (ONE_SEC/100));
				gDriver_Bat_Temp_ctl.detectorState = TEMP_DETECT_ENABLED_DETECTING;
			}
			break;

		case TEMP_DETECT_ENABLED_DETECTING:
			if(SYS_IsTimerExpired(&gDriver_Bat_Temp_ctl.detectorTimer))
			{
				S16 currentTemp;
				
				OS_ENTER_CRITICAL ();
				currentTemp = DRIVER_TEMP_NTCGetADCValue(gMMI_nvram.batteryPara.tempInfo.tempDetectorSource);
				OS_EXIT_CRITICAL ();

				gDriver_Bat_Temp_ctl.signTempADCValue=(gDriver_Bat_Temp_ctl.tempValue + currentTemp) /2;				
				gDriver_Bat_Temp_ctl.detectorState = TEMP_DETECT_ENABLED_CHECK_STATUS;
			}
			break;

		case TEMP_DETECT_ENABLED_CHECK_STATUS:
			if(MMI_IsChargerIn())
			{
				switch(DRIVER_TEMP_ChargerAction())
				{
					case CHARGING_NORMAL_EVENT:
						DRIVER_TEMP_SetChargeNormal();
						break;
					case CHARGING_ABNORMAL_EVENT:	
						DRIVER_TEMP_SetChargeAbnormal();
						break;
					case CHARGING_STOP_EVENT:
						DRIVER_TEMP_SetChargeStop();
						break;
					case CHARGE_KEEP_PREV_EVENT:
						break;
				}
			}
			else
			{
				S16 dischargingRiskHighBound, dischargingRiskLowBound;
				dischargingRiskHighBound = pMpParameter->MP_BatteryPara.chargingInfo.dischargingRiskHighBound;
				dischargingRiskLowBound = pMpParameter->MP_BatteryPara.chargingInfo.dischargingRiskLowBound;
				
				if(dischargingRiskHighBound < dischargingRiskLowBound)
				{
					DRIVER_TEMP_ExchangeValue((S16 XDATA_PTR)&dischargingRiskHighBound, (S16 XDATA_PTR)&dischargingRiskLowBound);
				}
				if(	gDriver_Bat_Temp_ctl.signTempADCValue > dischargingRiskHighBound || 
					gDriver_Bat_Temp_ctl.signTempADCValue < dischargingRiskLowBound)
				{
					MMI_PowerOffHandler(POWEROFF_REASOM_TEMP_PROTECT);
				}			
			}
			SYS_SetTimer(&gDriver_Bat_Temp_ctl.detectorTimer, gMMI_nvram.batteryPara.tempInfo.tempDetectorSleepTime* HALF_SEC);
			gDriver_Bat_Temp_ctl.detectorState= TEMP_DETECT_ENABLED_SLEEP;	
			if(pMpParameterF->ntcRatioPara.channelSet.pioVCCChannel != HV_MV_CHANNEL)
			{
				DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, VOLTAGE_LOW, pMpParameterF->ntcRatioPara.channelSet.pioVCCChannel);
			}
			break;
			
		case TEMP_DETECT_ENABLED_SLEEP:
			if(SYS_IsTimerExpired(&gDriver_Bat_Temp_ctl.detectorTimer))
			{
				gDriver_Bat_Temp_ctl.detectorState= TEMP_DETECT_ENABLED_INIT;
			}
			break;
	}
}

PUBLIC S16 DRIVER_TEMP_ADCDataReport(void)
{
	return gDriver_Bat_Temp_ctl.signTempADCValue;
}

#ifdef TWO_STEP_CHARGING_FUNCTION
PRIVATE void DRIVER_TEMP_TwoStepChargerHWCtl(U8 ctlstate)
{
	if (ctlstate == CHARGING_TWO_STEP_FAST_CHARGE_STATE)
	{
		DRIVER_TEMP_HWStopCharging();
		DRIVER_TEMP_HWDACCurrent((pMpParameterF->twoStepPara.valueSet0x24 & 0x03FF) |(gDriver_Bat_Temp_ctl.SFRValue0x24 & 0xFC00));
		DRIVER_TEMP_HWCurrentControl((pMpParameterF->twoStepPara.valueSet0x2C & 0x03FF) | (gDriver_Bat_Temp_ctl.SFRValue0x2C & 0xFC00));	
		OS_delay_us(33);
		DRIVER_TEMP_HWReCharging();
	}
	else if (ctlstate == CHARGING_TWO_STEP_CHARGE_STATE )
	{
		DRIVER_TEMP_HWStopCharging();
		DRIVER_TEMP_HWDACCurrent(gDriver_Bat_Temp_ctl.SFRValue0x24);
		DRIVER_TEMP_HWCurrentControl(gDriver_Bat_Temp_ctl.SFRValue0x2C);
		OS_delay_us(33);
		DRIVER_TEMP_HWReCharging();
	}
	else if (ctlstate == CHARGING_TWO_STEP_STOP_STATE)
	{
		DRIVER_TEMP_HWStopCharging();
	}
}

PRIVATE void DRIVER_TEMP_TwoStepSetFastCharge(void)
{
	if (gDriver_Bat_Temp_ctl.chargingState !=CHARGING_ABNORMAL_STATE)
		DRIVER_TEMP_TwoStepChargerHWCtl(CHARGING_TWO_STEP_FAST_CHARGE_STATE);
}

PUBLIC void DRIVER_TEMP_TwoStepSetNormalCharge(void)
{
	DRIVER_TEMP_TwoStepChargerHWCtl(CHARGING_TWO_STEP_CHARGE_STATE);
}

PRIVATE void DRIVER_TEMP_TwoStepsNormal(void)
{
	switch(gDriver_Bat_Temp_ctl.twoStepsState)
	{
		case TWO_STEPS_STATE_IDLE_2C:
		case TWO_STEPS_STATE_FAST_2C:
			DRIVER_TEMP_HWDACCurrent((pMpParameterF->twoStepPara.valueSet0x24 & 0x03FF) |(gDriver_Bat_Temp_ctl.SFRValue0x24 & 0xFC00));
			DRIVER_TEMP_HWCurrentControl((pMpParameterF->twoStepPara.valueSet0x2C & 0x03FF) | (gDriver_Bat_Temp_ctl.SFRValue0x2C & 0xFC00));
			break;

		case TWO_STEPS_STATE_IDLE_HALF_C:
		case TWO_STEPS_STATE_NORMAL_HALF_C:
		case TWO_STEPS_STATE_ALWAYS_HALF_C:
			DRIVER_TEMP_HWDACCurrent(gDriver_Bat_Temp_ctl.SFRValue0x24);
			DRIVER_TEMP_HWCurrentControl(gDriver_Bat_Temp_ctl.SFRValue0x2C);
			break;

		default:
			break;
	}
}

PRIVATE void DRIVER_TEMP_TwoStepsAbnormal(void)
{
	U16 ChargerDacCurrent;

	switch(gDriver_Bat_Temp_ctl.twoStepsState)
	{
		case TWO_STEPS_STATE_IDLE_2C:
		case TWO_STEPS_STATE_FAST_2C:
			ChargerDacCurrent	= DRIVER_TEMP_NTCCurrentDiscnt ((pMpParameterF->twoStepPara.valueSet0x24 & 0x03FF) |(gDriver_Bat_Temp_ctl.SFRValue0x24 & 0xFC00));
			DRIVER_TEMP_HWDACCurrent(ChargerDacCurrent);
			DRIVER_TEMP_HWCurrentControl((pMpParameterF->twoStepPara.valueSet0x2C & 0x03FF) | (gDriver_Bat_Temp_ctl.SFRValue0x2C & 0xFC00));
			break;

		case TWO_STEPS_STATE_IDLE_HALF_C:
		case TWO_STEPS_STATE_NORMAL_HALF_C:
		case TWO_STEPS_STATE_ALWAYS_HALF_C:
			DRIVER_TEMP_HWDACCurrent(gDriver_Bat_Temp_ctl.SFRValue0x24);
			DRIVER_TEMP_HWCurrentControl(gDriver_Bat_Temp_ctl.SFRValue0x2C);
			break;

		default:
			break;
	}
	OS_delay_us(33);
}


#define TWO_STEPS_TH_H 	(pMpParameterF->twoStepPara.changingThreValueToolADC)
#define TWO_STEPS_TH_L 	(pMpParameterF->twoStepPara.changingThreValueToolADC-CHARGING_OFFSET_VALUE)


PUBLIC void DRIVER_TEMP_TwoStepsCharging(void)
{
	switch(gDriver_Bat_Temp_ctl.twoStepsState)
	{
		case TWO_STEPS_STATE_INIT:
			if (MMI_IsChargerIn() & pMpParameterF->twoStepPara.enableTwoStepCharger)
			{
				SYS_3WireRead(0x24, gDriver_Bat_Temp_ctl.SFRValue0x24);
				SYS_3WireRead(0x2C, gDriver_Bat_Temp_ctl.SFRValue0x2C);
				gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_IDLE_2C;
			}
			break;
		case TWO_STEPS_STATE_IDLE_2C:
			if (MMI_IsChargerIn() & pMpParameterF->twoStepPara.enableTwoStepCharger)
			{
				if (gDriver_Bat_Temp_ctl.chargingState != CHARGING_STOP_STATE)
				{
					if(DRV_ADC0.fVBat() < TWO_STEPS_TH_H)
					{
						DRIVER_TEMP_TwoStepSetFastCharge();
						gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_FAST_2C;
					}
					else
					{
						DRIVER_TEMP_TwoStepSetNormalCharge();
						gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_NORMAL_HALF_C;
					}
				}
			}
			break;
		case TWO_STEPS_STATE_IDLE_HALF_C:
			if (MMI_IsChargerIn() & pMpParameterF->twoStepPara.enableTwoStepCharger)
			{
				if (gDriver_Bat_Temp_ctl.chargingState != CHARGING_STOP_STATE)
				{
					if(DRV_ADC0.fVBat() > TWO_STEPS_TH_H)
					{
						DRIVER_TEMP_TwoStepSetNormalCharge();
						gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_NORMAL_HALF_C;

					}
					else
					{
						DRIVER_TEMP_TwoStepSetFastCharge();
						gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_FAST_2C;
					}
				}
			}
			break;
		case TWO_STEPS_STATE_FAST_2C:
			if(MMI_IsChargerIn())
			{
				if (gDriver_Bat_Temp_ctl.chargingState != CHARGING_STOP_STATE)
				{
					if(DRV_ADC0.fVBat() > TWO_STEPS_TH_H)
					{
						DRIVER_TEMP_TwoStepSetNormalCharge();
						gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_NORMAL_HALF_C;
					}
				}
			}
			else
			{
				gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_IDLE_2C;
			}
			break;

		case TWO_STEPS_STATE_NORMAL_HALF_C:
			if(MMI_IsChargerIn())
			{
				if(MMI_CHG_IsComplete())
				{
					gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_ALWAYS_HALF_C;
				}
				else if (gDriver_Bat_Temp_ctl.chargingState != CHARGING_STOP_STATE)
				{
					if(DRV_ADC0.fVBat() < TWO_STEPS_TH_L)
					{
						DRIVER_TEMP_TwoStepSetFastCharge();
						gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_FAST_2C;
					}
				}
			}
			else
			{
				gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_IDLE_HALF_C;
			}
			break;
		case TWO_STEPS_STATE_ALWAYS_HALF_C:
			if(!MMI_IsChargerIn())
			{
				gDriver_Bat_Temp_ctl.twoStepsState = TWO_STEPS_STATE_IDLE_HALF_C;
			}

			break;
		default:

			break;
	}
}
#endif

PRIVATE void DRIVER_TEMP_ExchangeValue(S16 XDATA_PTR ptr1, S16 XDATA_PTR ptr2)
{
	S16 tempValue;
	tempValue=*ptr2;
	*ptr2=*ptr1;
	*ptr1=tempValue;
}

#ifdef NTC_RATIO_FUNCTION

#define INTERNAL_GND_PIO	GPIO16

PRIVATE void DRIVER_TEMP_NTCRatioInit(void)
{
	gADCFunctionHandler.fADCInit(pMpParameterF->ntcRatioPara.channelSet.aioChannelSel);
	gADCFunctionHandler.fADCInit(INTERNAL_GND_PIO);
	if(pMpParameterF->ntcRatioPara.channelSet.pioVCCChannel != HV_MV_CHANNEL)
	{
		DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, VOLTAGE_HIGH, pMpParameterF->ntcRatioPara.channelSet.pioVCCChannel);
	}
	DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, VOLTAGE_HIGH, pMpParameterF->ntcRatioPara.channelSet.pioCTRChannel);
}

PRIVATE S16 DRIVER_TEMP_NTCGetADCValue(U8 aioSource)
{
	S32 sumValue;
	U8 index;

	sumValue = 0;
	
	for (index = 0; index < pMpParameterF->ntcRatioPara.aioAverageCount ; index++)
	{
		OS_ENTER_CRITICAL ();
		sumValue += gADCFunctionHandler.fADCTransform((U32)gADCFunctionHandler.fADCRead(aioSource));
		OS_EXIT_CRITICAL ();
	}
	
	return (S16)(sumValue/pMpParameterF->ntcRatioPara.aioAverageCount);
}

PRIVATE FLOAT DRIVER_TEMP_NTCGetRatioValue(void)
{
	FLOAT ratioValue;
	S16 maxValue, minValue;

	if(gDriver_Bat_Temp_ctl.signTempADCValue2 > gDriver_Bat_Temp_ctl.signTempADCValue)
	{
		maxValue = gDriver_Bat_Temp_ctl.signTempADCValue2;
		minValue = gDriver_Bat_Temp_ctl.signTempADCValue;
	}
	else
	{
		maxValue = gDriver_Bat_Temp_ctl.signTempADCValue;
		minValue = gDriver_Bat_Temp_ctl.signTempADCValue2;
	}

	#if (defined AB1520C) || (defined AB1520D)
	ratioValue =((FLOAT)(minValue - gDriver_Bat_Temp_ctl.signTempADCGroundValue))/
	((FLOAT)(maxValue - gDriver_Bat_Temp_ctl.signTempADCGroundValue));

	#elif (defined AB1520S) || (defined AB1520U)
	
	ratioValue =((FLOAT)(minValue))/((FLOAT)(maxValue));
	#endif

	return ratioValue;
}

PRIVATE void DRIVER_TEMP_ExchangeRatioValue(FLOAT XDATA_PTR ptr1, FLOAT XDATA_PTR ptr2)
{
	FLOAT tempValue;
	tempValue=*ptr2;
	*ptr2=*ptr1;
	*ptr1=tempValue;
}

PRIVATE U8 DRIVER_TEMP_NTCRatioChargerAction(void)
{
	FLOAT ratio1, ratio2, ratio3, ratio4, ratio5, ratio6;

	ratio1= pMpParameterF->ntcRatioPara.ratioValue.ratio1;
	ratio2= pMpParameterF->ntcRatioPara.ratioValue.ratio2;
	ratio3= pMpParameterF->ntcRatioPara.ratioValue.ratio3;
	ratio4= pMpParameterF->ntcRatioPara.ratioValue.ratio4;
	ratio5= pMpParameterF->ntcRatioPara.ratioValue.ratio5;
	ratio6= pMpParameterF->ntcRatioPara.ratioValue.ratio6;
	
	if(ratio1 < ratio6)
	{
		DRIVER_TEMP_ExchangeRatioValue((FLOAT XDATA_PTR)&ratio1, (FLOAT XDATA_PTR)&ratio6);
		DRIVER_TEMP_ExchangeRatioValue((FLOAT XDATA_PTR)&ratio2, (FLOAT XDATA_PTR)&ratio5);
		DRIVER_TEMP_ExchangeRatioValue((FLOAT XDATA_PTR)&ratio3, (FLOAT XDATA_PTR)&ratio4);
	}

	if (gDriver_Bat_Temp_ctl.ratioValue> ratio4 && gDriver_Bat_Temp_ctl.ratioValue< ratio3)
	{
		LightDbgPrint("NTC Ratio normal event");
		return (U8)CHARGING_NORMAL_EVENT;
	}
	else if (gDriver_Bat_Temp_ctl.ratioValue> ratio1 ||gDriver_Bat_Temp_ctl.ratioValue<ratio6)
	{
		LightDbgPrint("NTC Ratio stop event");
		return (U8)CHARGING_STOP_EVENT;
	}
	else if(gDriver_Bat_Temp_ctl.ratioValue> ratio2 && gDriver_Bat_Temp_ctl.ratioValue< ratio1)
	{
		LightDbgPrint("NTC Ratio abnormal event1");
		return (U8)CHARGING_ABNORMAL_EVENT;
	}
	else if(gDriver_Bat_Temp_ctl.ratioValue> ratio6 && gDriver_Bat_Temp_ctl.ratioValue< ratio5)
	{
		LightDbgPrint("NTC Ratio abnormal event2");
		return (U8)CHARGING_ABNORMAL_EVENT;
	}
	else
	{
		LightDbgPrint("NTC Ratio keep event");
		return (U8)CHARGE_KEEP_PREV_EVENT;
	}

}

PUBLIC void DRIVER_TEMP_NTCRatio(void)
{
	if (!pMpParameterF->ntcRatioPara.enableNTCRatio)
		return;

	switch(gDriver_Bat_Temp_ctl.detectorState)
	{
		case TEMP_DETECT_NTC_RATIO_DISABLED:
			DRIVER_TEMP_NTCRatioInit();
			gDriver_Bat_Temp_ctl.detectorState = TEMP_DETECT_NTC_RATIO_ENABLED_INIT;
			break;

		case TEMP_DETECT_NTC_RATIO_ENABLED_INIT:
			SYS_SetTimer(&gDriver_Bat_Temp_ctl.detectorTimer, gMMI_nvram.batteryPara.tempInfo.tempDetectorOnWaitStableTime*(ONE_SEC/100));
			gDriver_Bat_Temp_ctl.detectorState = TEMP_DETECT_NTC_RATIO_ENABLED_WAITING_READ_ADC_VALUE1;
			break;
			
		case TEMP_DETECT_NTC_RATIO_ENABLED_WAITING_READ_ADC_VALUE1:
			if(SYS_IsTimerExpired(&gDriver_Bat_Temp_ctl.detectorTimer))
			{
				gDriver_Bat_Temp_ctl.signTempADCValue2 = DRIVER_TEMP_NTCGetADCValue(pMpParameterF->ntcRatioPara.channelSet.aioChannelSel);
				DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, VOLTAGE_LOW, pMpParameterF->ntcRatioPara.channelSet.pioCTRChannel);
				SYS_SetTimer(&gDriver_Bat_Temp_ctl.detectorTimer, gMMI_nvram.batteryPara.tempInfo.tempDetectorDetectingTime*(ONE_SEC/100));
				gDriver_Bat_Temp_ctl.detectorState = TEMP_DETECT_NTC_RATIO_ENABLED_WAITING_READ_ADC_VALUE2;
			}
			break;

		case TEMP_DETECT_NTC_RATIO_ENABLED_WAITING_READ_ADC_VALUE2:
			if(SYS_IsTimerExpired(&gDriver_Bat_Temp_ctl.detectorTimer))
			{
				U16 ratioValue;

				gDriver_Bat_Temp_ctl.signTempADCValue = DRIVER_TEMP_NTCGetADCValue(pMpParameterF->ntcRatioPara.channelSet.aioChannelSel);

				#if (defined AB1520C) || (defined AB1520D)
				gDriver_Bat_Temp_ctl.signTempADCGroundValue = DRIVER_TEMP_NTCGetADCValue(INTERNAL_GND_PIO);
				#endif

				gDriver_Bat_Temp_ctl.ratioValue = DRIVER_TEMP_NTCGetRatioValue();
				ratioValue = gDriver_Bat_Temp_ctl.ratioValue*1000;
				LightDbgPrint("NTC ratio value %D", (U32)ratioValue);
				gDriver_Bat_Temp_ctl.detectorState = TEMP_DETECT_NTC_RATIO_ENABLED_CHECK_STATUS;
			}
			break;

		case TEMP_DETECT_NTC_RATIO_ENABLED_CHECK_STATUS:
			if(MMI_IsChargerIn())
			{
				switch(DRIVER_TEMP_NTCRatioChargerAction())
				{
					case CHARGING_NORMAL_EVENT:
						DRIVER_TEMP_SetChargeNormal();
						break;
					case CHARGING_ABNORMAL_EVENT:
						DRIVER_TEMP_SetChargeAbnormal();
						break;
					case CHARGING_STOP_EVENT:
						DRIVER_TEMP_SetChargeStop();
						break;
					case CHARGE_KEEP_PREV_EVENT:
						break;
				}
			}
			else
			{
				FLOAT ratio1, ratio6;
				ratio1 = pMpParameterF->ntcRatioPara.ratioValue.ratio1;
				ratio6 = pMpParameterF->ntcRatioPara.ratioValue.ratio6;

				if(ratio1 < ratio6)
				{
					DRIVER_TEMP_ExchangeRatioValue((FLOAT XDATA_PTR)&ratio1, (FLOAT XDATA_PTR)&ratio6);
				}

				if (gDriver_Bat_Temp_ctl.ratioValue> ratio1 ||gDriver_Bat_Temp_ctl.ratioValue< ratio6)
				{
					MMI_PowerOffHandler(POWEROFF_REASOM_TEMP_PROTECT);
				}	
			}
			SYS_SetTimer(&gDriver_Bat_Temp_ctl.detectorTimer, gMMI_nvram.batteryPara.tempInfo.tempDetectorSleepTime* HALF_SEC);
			if(pMpParameterF->ntcRatioPara.channelSet.pioVCCChannel != HV_MV_CHANNEL)
			{
				DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, VOLTAGE_LOW, pMpParameterF->ntcRatioPara.channelSet.pioVCCChannel);
			}
			DRV_GPIO_F_SetIOStatus(OUTPUT_ENABLE, VOLTAGE_LOW, pMpParameterF->ntcRatioPara.channelSet.pioCTRChannel);
			gDriver_Bat_Temp_ctl.detectorState= TEMP_DETECT_NTC_RATIO_ENABLED_SLEEP;	
			break;

		case TEMP_DETECT_NTC_RATIO_ENABLED_SLEEP:
			if(SYS_IsTimerExpired(&gDriver_Bat_Temp_ctl.detectorTimer))
			{
				gDriver_Bat_Temp_ctl.detectorState= TEMP_DETECT_NTC_RATIO_DISABLED;	
			}
		default:
			break;
	}
	
}
#endif
