#ifndef _STRUCT_DRIVER_LED_H_
#define _STRUCT_DRIVER_LED_H_

#include "cpu.h"
#include "drv_led.h"


#define LED_PATTERN_DEFAULT_NO	13
#define LED_FILTER_DEFAULT_NO	8
#define	LED_MAP_GPIO_DEFAULT_NO		2


enum
{
	FILTER_USER_DEF = 1,
	FILTER_MULTI_DIVIDE,
	FILTER_SWAP_CHANGE,
	FILTER_ADD,
};
//SIG_SEL	SFR_E3 
enum
{
	GRP_SEL,
	DMIC_CLK_0,
	DMIC_CLK_1,
	PWM3,
	PWM4,
	//CLK_SLOW,		Has been defined
	//CLK_FAST,		Has been defined
	UART_NRTS = 7,
	SPDIF_O,
	DEBUG_PORT,
};

////////////////////////////// Filter/////////////////
typedef struct
{
	U8 filterType;
	LedPatternType userdefData;
}LedFilterUserDefType;

typedef struct
{
	U8 isMultiply;
	U8 isDivide;
	U8 t1;
	U8 t2;
	U8 t3;
	U8 t4;
	U8 RptT12;
	U8 reserved[3];
}LedFilterMultiDivideParaType;

typedef struct
{
	U8 filterType;
	LedFilterMultiDivideParaType para[LED_NO];
}LedFilterMultiDivideType;

typedef struct
{
	U8 filterType;
	U8 isSwap;
	U8 swapTarget1;
	U8 swapTarget2;
	U8 isChangeLED;
	U8 sourceLed;
	U8 destinationLED;
}LedFilterSwapChangeType;

typedef struct
{
	U8 filterType;
	U8 targetLedForAdd;
	U8 gapWithBgLed; //in 20ms
	U8 ledIndex;
	U8 t1;
	U8 t2;
	U8 RptT12;
	U8 rampOnStep;
	U8 rampOffStep;	
	U8 addedT3;  //gap +(t1+t2)*Rpt
}LedFilterAddType;

typedef union
{
	LedFilterUserDefType userDefineFilter;
	LedFilterMultiDivideType multiDivideFilter;
	LedFilterSwapChangeType swapChangeFilter;	
	LedFilterAddType addFilter;
	U8 filterType;	
}LedFilterSettingType;

typedef struct led_map_setting
{
	U8 mapGpio;
	U8 outputType;
}LedMapSetting;

typedef struct driver_led_data_st
{
	U8 totalLedPatternNo;
	LedPatternType ledData[LED_PATTERN_DEFAULT_NO];
	U8 totalLedFilterNo;
	LedFilterSettingType ledFilterData[LED_FILTER_DEFAULT_NO];
	U8 ledMapGpioNo;
	LedMapSetting ledMapSet[LED_MAP_GPIO_DEFAULT_NO];
}DRIVER_LED_DATA_TYPE;

typedef struct
{
	DRIVER_LED_DATA_TYPE	driver_led_data;
	CRC16 CRC;
} SECTOR_DRIVER_LED_DATA_STRU;

#endif

