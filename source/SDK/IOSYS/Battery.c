#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\MMI\MMI_Base.h"
#include "..\MMI\MMI_ChgBat.h"

#define SDK_BAT_MONITOR		0
#define SDK_BAT_LOW_BAT_CB	0


/*  SDK_Battery_GetLowBatLevel: get low battery level setting
 *	<output>
 *		U16	: battery level after calibration
 */
PUBLIC U16 SDK_Battery_GetLowBatLevel(void)
{
	return MMI_BAT_GetLowBatLevel();
}

/*  SDK_Battery_GetShutDownBatLevel: get shut down battery level setting
 *	<output>
 *		U16	: battery level after calibration
 */
PUBLIC U16 SDK_Battery_GetShutDownBatLevel(void)
{
	return MMI_BAT_GetShutDownBatLevel();
}

/*  SDK_Battery_IsVoltageBelow: check if current system battery level is lower than input battery level
 *	<input1> 
 *		U16	voltage	: battery level 
 *  <output>
 *		BOOL		: TRUE-->Yes, FALSE-->No
 */
PUBLIC BOOL SDK_Battery_IsVoltageBelow(U16 voltage)
{
	return MMI_BAT_IsVoltageBelow(voltage);
}

/*  SDK_Battery_GetLevelInPercent: get voltage level in percentage
 *  <output>
 *		U8		: 0 - 100, (0% - 100%)
 */
PUBLIC U8 SDK_Battery_GetLevelInPercent(void)
{
	return MMI_BAT_GetLevelInPercent();
}


#if SDK_BAT_MONITOR
/*  SDK_Battery_Polling: battery polling function to monitor battery level
 */
PRIVATE void SDK_Battery_Polling(void)
{
	if(SDK_Battery_IsVoltageBelow(MMI_BAT_GetLowBatLevel()))
	{
		//Low battery case
		
	}
}
#endif


#if SDK_BAT_LOW_BAT_CB
/*  SDK_CB_Battery_Low: callback function which is called in NDK when battery is lower than low battery level
 */
PRIVATE void SDK_CB_Battery_Low(void)
{
	
}
#endif

PUBLIC void SDK_Battery_Init(void)
{
#if SDK_BAT_MONITOR
	SDK_Proxy_RegisterPeriodicAPI(0, SDK_Battery_Polling, 30*ONE_SEC, FALSE);
#endif
#if SDK_BAT_LOW_BAT_CB
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_BAT_LOW, API_GRP_IOSYS, (VFUN)SDK_CB_Battery_Low);
#endif
}