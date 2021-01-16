#ifndef _MMI_CHGBAT_H_

	#include "..\ROM\SECTOR\sector_mp_parameter.h"
	
	#define _MMI_CHGBAT_H_
	
	//-----------------------------Battery Macro---------------------------
	#define MMI_CHGBAT_GetBatStatus() (gMMI_ctl.mmiBatInfo.batStatus)
		
	enum
	{
		MMI_BAT_WELL,		//CHARGER_OUT
		MMI_BAT_LOW,		//CHARGER_OUT
		MMI_BAT_CHARGER_IN,
		MMI_BAT_CHARGER_RECHARGE,
		MMI_BAT_CHARGING_FULL,
	};

	enum
	{
		IND_BATTERY_LOW  = 0x70,
		IND_BATTERY_INTERVAL_0,
		IND_BATTERY_INTERVAL_1,
		IND_BATTERY_INTERVAL_2,
		IND_BATTERY_INTERVAL_3,
	};
	
	PUBLIC void MMI_CHG_ResetTimeOutDectionTimer(void);
	
	#ifdef MMI_BATCHG_ENABLE
	PUBLIC void MMI_CHGBAT_Init(void);
	PUBLIC void MMI_CHGBAT_BatteryCheckPoint(void);
	#endif
	
	PUBLIC BOOL MMI_BAT_IsVoltageBelow(VoltageType voltage);
	
	PUBLIC U8 MMI_BAT_GetLevelInQuater(void);
	PUBLIC U8 MMI_BAT_GetLevelInPercent(void);
	
	PUBLIC U16 MMI_BAT_GetLowBatLevel(void);
	PUBLIC U16 MMI_BAT_GetShutDownBatLevel(void);
	PUBLIC BOOL MMI_CHG_IsComplete(void);
	
#endif
