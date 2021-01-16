PUBLIC void DRIVER_TEMP_Check(void);
PUBLIC void DRIVER_TEMP_SetChargeNormal(void);
PUBLIC void DRIVER_TEMP_ADC_Init(U8 channel);
PUBLIC U16 DRIVER_TEMP_ReadADC(U8 channel);
PUBLIC void DRIVER_TEMP_TwoStepsCharging(void);
PUBLIC void DRIVER_TEMP_NTCRatio(void);
PUBLIC S16 DRIVER_TEMP_ADCDataReport(void);

#define TWO_STEP_CHARGING_FUNCTION
#define NTC_RATIO_FUNCTION

#define HV_MV_CHANNEL 0xFF
typedef struct
{
	void (*fADCInit)(U8 channel);
	U16 (*fADCRead)(U8 channel);
	S16 (*fADCTransform)(U32 tempValue);
}NTC_VFUN_TYPE;

enum
{
	TWO_STEPS_STATE_INIT,
	TWO_STEPS_STATE_IDLE_2C,
	TWO_STEPS_STATE_IDLE_HALF_C,
	TWO_STEPS_STATE_FAST_2C,
	TWO_STEPS_STATE_NORMAL_HALF_C,
	TWO_STEPS_STATE_ALWAYS_HALF_C
};

typedef struct
{
	U8 detectorState;
	S16 tempValue;
	S16 signTempADCValue;
	U8 chargingState;
	OST XDATA_PTR detectorTimer;
	U8 count;
	S16 signTempADCValue2;
	S16 signTempADCGroundValue;
	FLOAT ratioValue;
	U16 mpValue0x24;
#ifdef TWO_STEP_CHARGING_FUNCTION
	U8 twoStepsState;
	U16 SFRValue0x24;
	U16 SFRValue0x2C;
#endif

}DRIVER_TEMP_CTL_TYPE;

/*
typedef struct
{
	FLOAT ratio1;
	FLOAT ratio2;
	FLOAT ratio3;
	FLOAT ratio4;
	FLOAT ratio5;
	FLOAT ratio6;
}RATIO_VALUE_TYPE;
*/
