#ifndef _SECTOR_MP_PARAMETER_F_H_
#define _SECTOR_MP_PARAMETER_F_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_mp_parameter_F.h defines the calibration parameters that added in flash code.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"
#include "bt_types.h"

#define AIO_TOTAL_SET_NUMBER 2
#define AIO_DATA_POINT_NUM_FOR_SLOPE 2
#define AIO_BOUNDARY_DATA_NUM 5

typedef struct stru_mp_adc_data_type
{
	U16 voltage;
	S16 adcValue;
}MP_ADC_DATA_TYPE;

typedef struct stru_mp_aio_related_field
{
	U8 aioSource;
	U8 isAIOEnable;
	MP_ADC_DATA_TYPE adcSlopeData[AIO_DATA_POINT_NUM_FOR_SLOPE];
	MP_ADC_DATA_TYPE adcBoundata[AIO_BOUNDARY_DATA_NUM];
}MP_AIO_RELATED_FIELD_STRU;

#define LE_PUBLIC_DEVICE_ADDR       (0)
typedef U8 LE_RANDOM_BDADDR_SEL;
#define LE_RANDOMBDADDR_FIXED       (LE_RANDOM_BDADDR_SEL)(1)
#define LE_RANDOMBDADDR_RANDOM      (LE_RANDOM_BDADDR_SEL)(2)

typedef struct
{
    BD_ADDR staticRandomAddress;
    U8 type;
}LE_BD_ADDR_TYPE;

typedef struct stru_adc1_two_step_charging
{
	U8 enableTwoStepCharger;
	U16 ChargeCurrent1;
	U16 valueSet0x24;
	U8 valueSet0x2C;
	U16 changingThreValueToolUsed;
	U16 changingThreValueToolADC;
}TWO_STEP_CHARGING_STRU;


typedef struct stru_ntc_ratio_value
{
	FLOAT ratio1;
	FLOAT ratio2;
	FLOAT ratio3;
	FLOAT ratio4;
	FLOAT ratio5;
	FLOAT ratio6;
}NTC_RATIO_VALUE;

typedef struct stru_ntc_raio_channel_set
{
	U8 aioChannelSel;
	U8 pioVCCChannel;
	U8 pioCTRChannel;
}NTC_RATIO_CHANNEL_SET;

typedef struct stru_ntc_ratio_func
{
	U8 enableNTCRatio;
	U8 aioAverageCount;
	NTC_RATIO_VALUE ratioValue;
	NTC_RATIO_CHANNEL_SET channelSet;
}NTC_RATION_FUNC;

typedef struct stru_ntc_temp_point_mp_tool_use
{
	S8 chargingRiskHighBoundPoint;
	S8 chargingSecureHighBoundPoint;
	S8 chargingRechargeHighBoundPoint;
	S8 chargingRechargeLowBoundPoint;
	S8 chargingSecureLowBoundPoint;
	S8 chargingRiskLowBoundPoint;
	S8 dischargingRiskHighBoundPoint;
	S8 dischargingRiskLowBoundPoint;
	S16 V1ADC;
	S16 GNDADC;
	S8 factoryTemperature;
	U8 offsetValue;
	U8 temperatureInterval;
}NTC_TEMP_POINT_MP_TOOL_USE;

typedef struct stru_mp_parameter_f
{
	U8 rsvd;
	MP_AIO_RELATED_FIELD_STRU AioSet[AIO_TOTAL_SET_NUMBER];
	TWO_STEP_CHARGING_STRU twoStepPara;
	NTC_RATION_FUNC ntcRatioPara;
	U8 discountNumber;
	NTC_TEMP_POINT_MP_TOOL_USE ntcMPToolInformation;
	U8 functionEnabled;
	U8 reserved[34];
	U8 Customer_Reserved1[15][32];
	U8 Customer_Reserved2[30];
	LE_BD_ADDR_TYPE leBDAddr;           //LE BD address
} MP_PARAMETER_F_STRU;

typedef struct stru_sector_mp_parameter_f
{
    MP_PARAMETER_F_STRU MpParameterF;

    CRC16 CRC;

} SECTOR_MP_PARAMETER_F_STRU;


#endif /* _SECTOR_MP_PARAMETER_F_H_ */


