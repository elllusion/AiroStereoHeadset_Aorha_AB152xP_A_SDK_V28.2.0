#ifndef _SECTOR_MP_PARAMETER_H_
#define _SECTOR_MP_PARAMETER_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_mp_parameter.h defines the calibration parameters and Rom code may uses.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"
#include "bt_types.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions  ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define NUM_OF_SUPPORTED_MP_AFE_INIT	20
#define NUM_OF_SUPPORTED_MP_SFR_INIT    10
#define NUM_OF_CALIBRATION_INFO         30
#define NUM_OF_MANUFACTURING_INFO       30
#define PRIVATE_KEY_LENGTH				24
#define CHARGE_KEEP_PREV_ACTION 0xFF

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U16 VoltageType;

typedef struct
{
	VoltageType lowBatLevel;
	VoltageType shutDownLowBat;
}LowBatteryType;

typedef struct
{
	VoltageType checkpoint1;
	VoltageType checkpoint2;
	VoltageType checkpoint3;
}BatteryCheckpoint;

typedef struct
{
	S16 chargingRiskHighBound;
	S16 chargingSecureHighBound;
	S16 chargingRechargeHighBound;
	S16 chargingRechargeLowBound;
	S16 chargingSecureLowBound;
	S16 chargingRiskLowBound;

	S16 dischargingRiskHighBound;
	S16 dischargingRiskLowBound;
}ChargingInfo;

//MP_ChargerDiscountCtl
enum
{
	CHARGING_UNKNOW_STATE = 0,
	CHARGING_NORMAL_STATE ,
	CHARGING_ABNORMAL_STATE,
	CHARGING_STOP_STATE,
	CHARGING_EFF_STATE_NO = 3,
};

#define CHARGING_CTL_NORMAL_IND		(CHARGING_NORMAL_STATE -1)
#define CHARGING_CTL_ABNORMAL_IND	(CHARGING_ABNORMAL_STATE -1)

enum
{
	CHARGING_TWO_STEP_UNKNOW_STATE = 0,
	CHARGING_TWO_STEP_CHARGE_STATE,
	CHARGING_TWO_STEP_FAST_CHARGE_STATE,
	CHARGING_TWO_STEP_STOP_STATE,
	CHARGING_TWO_STEP_EFF_STATE_NO=3,
};

typedef struct
{
	U16 chargerDacCurrent;
	U16 CCAdcCurrent;
	U16 CCAdcCurrent2;
}MP_ChargerDiscountCtl;

//MP_SysLocalDeviceInfo
typedef struct stru_mp_sys_local_device_info
{
    BD_ADDR_STRU  BdAddr;
    U8  PrivatKey[PRIVATE_KEY_LENGTH];
} MP_SYS_LOCAL_DEVICE_INFO_STRU;

//MP_AfeInit
typedef struct stru_mp_afe_reg_init
{
    U8 Addr;
    U8 HiByte;
    U8 LoByte;
    U8 HiByteMask;
    U8 LoByteMask;

} MP_AFE_REG_INIT_STRU;

typedef struct stru_mp_afe_init
{
    U8  NumOfAfeRegInit;
    MP_AFE_REG_INIT_STRU AfeRegInit[NUM_OF_SUPPORTED_MP_AFE_INIT];

} MP_AFE_INIT_STRU;

//MP_SfrInit
typedef struct stru_mp_sfr_reg_init
{
	U8	Addr;
	U8  Offset;
	U8	Value;

} MP_SFR_REG_INIT_STRU;

typedef struct stru_mp_sfr_init
{
    U8  NumOfSfrRegInit;
    MP_SFR_REG_INIT_STRU SfrRegInit[NUM_OF_SUPPORTED_MP_SFR_INIT];

} MP_SFR_INIT_STRU;

//MP_RfMiscCtl
typedef struct stru_mp_mode_gain_comp
{
	U16	tx_mod_gain_temp_step[7];
	U8  bdr_txagc_gc[8];
	U8  edr_txagc_gc[8];

} MP_MOD_GAIN_COMP_STRU;

typedef struct stru_mp_bt_tx_pwr_cfg
{
    U8  tx_default_gfsk_gc1;
    U8  tx_default_gfsk_gc2;
    U8  tx_default_edr_gc1;
    U8  tx_default_edr_gc2;
    U8  tx_gfsk_max_gain;
    U8  tx_edr_max_gain;

} MP_BT_TX_PWR_CFG_STRU;

typedef struct stru_mp_rf_misc_ctl
{
    MP_BT_TX_PWR_CFG_STRU  	mp_bt_tx_pwr_cfg;

    MP_MOD_GAIN_COMP_STRU   mp_mod_gain_comp_cfg;

} MP_RF_MISC_CTL_STRU;

//BatteryPara
#define MMI_IPHONE_BATTERY_LEVELS	9
typedef struct
{
	VoltageType initBatVolt;
	LowBatteryType lowBatVolt;
	//-- 4 levels
	BatteryCheckpoint checkpoints;
	//--
	ChargingInfo chargingInfo;
	U16 batteryIPhoneCheckPoints[MMI_IPHONE_BATTERY_LEVELS];
}MP_BATTERY_PARA_STRU;

//ADC Field
typedef struct stru_mp_adc_field
{
    U8  Adc0SlopePer1V;
    U8  Adc1SlopePer1V;
    U16 RsvdAdc0[10];
    U16 RsvdAdc1[10];

} MP_ADC_FIELD_STRU;

typedef struct stru_mp_parameter
{
	MP_SYS_LOCAL_DEVICE_INFO_STRU   MP_SysLocalDeviceInfo;
	MP_AFE_INIT_STRU		MP_AfeInit;
	MP_SFR_INIT_STRU        MP_SfrInit;
	MP_RF_MISC_CTL_STRU		MP_RfMiscCtl;
	MP_BATTERY_PARA_STRU	MP_BatteryPara;
	MP_ChargerDiscountCtl	MP_ChargerDiscountCtl[CHARGING_EFF_STATE_NO];
	MP_ADC_FIELD_STRU       MP_AdcField;
	U8 CalibrationInfo[NUM_OF_CALIBRATION_INFO];
	U8 ManufacturingInfo[NUM_OF_MANUFACTURING_INFO];
} MP_PARAMETER_STRU;

typedef struct stru_sector_mp_parameter
{
    MP_PARAMETER_STRU MpParameter;

    CRC16 CRC;

} SECTOR_MP_PARAMETER_STRU;


#endif /* _SECTOR_MP_PARAMETER_H_ */


