#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"


#include "..\SDK\include\Key.h"
#include "..\SDK\include\Fm_Hal.h"
#include "..\SDK\include\IIC.h"
#include "..\SDK\include\Charger.h"
#include "..\SDK\include\Battery.h"
#include "..\SDK\include\System.h"
#include "..\SDK\include\Uart_Hal.h"
#include "..\SDK\include\LED.h"

void SDK_Init(void)
{
	SDK_Key_Init();
	SDK_FM_Init();
	SDK_IIC_Init();
	SDK_Charger_Init();
	SDK_Battery_Init();
	SDK_System_Init();
	SDK_Uart_HAL_Init();
}

void SDK_Polling(void)
{
    U8 mmistate; 
	//Get MMI state
    mmistate = SDK_System_GetTopState();    
	switch(mmistate)
	{
		case MMI_CONDISCABLE:
			SDK_LED_SetHWBySectorDataSample(4);//Load LED Pattern 4 to HW
			break;
		case MMI_CONNECTED:
            SDK_LED_SetHWSample(); //Configure by code
    		break;
		default:
			break;
	}
}

/* 
 * SDK_LED_SetHWSample: sample code to set led HW
 */
PUBLIC void SDK_LED_SetHWSample(void)
{
	LedPatternType XDATA_PTR LedCtrlPtr;
	
	LedCtrlPtr = SYS_MemoryGet(sizeof(LedPatternType));
	
	if(!LedCtrlPtr)
		ASSERT(FALSE);
	
	OSMEM_memset_xdata((U8 XDATA_PTR)LedCtrlPtr, 0, sizeof(LedPatternType));

	//RED
	LedCtrlPtr->ledSettings[0].onOff  = LED_ON;
	LedCtrlPtr->ledSettings[0].ledFollowMultiplierControl = 0x01;
	LedCtrlPtr->ledSettings[0].pwmSetting = 0xF0;
	LedCtrlPtr->ledSettings[0].t1 = 50;	//1s, unit:20ms
	LedCtrlPtr->ledSettings[0].t2 = 50;	//1s, unit:20ms
	LedCtrlPtr->ledSettings[0].t3 = 6;		//3s, unit:50ms
	LedCtrlPtr->ledSettings[0].rampOnStep = 0x20;	//64ms, unit:2ms
	LedCtrlPtr->ledSettings[0].rampOffStep = 0x20;	//64ms, unit:2ms
	//BLUE
	LedCtrlPtr->ledSettings[1].onOff  = LED_ON;
	LedCtrlPtr->ledSettings[1].ledFollowMultiplierControl = 0x01;
	LedCtrlPtr->ledSettings[1].pwmSetting = 0xF0;
	LedCtrlPtr->ledSettings[1].t1 = 50;	//1s, unit:20ms
	LedCtrlPtr->ledSettings[1].t2 = 50;	//1s, unit:20ms
	LedCtrlPtr->ledSettings[1].t3 = 6;		//3s, unit:50ms
	LedCtrlPtr->ledSettings[1].rampOnStep = 0x20;	//64ms, unit:2ms
	LedCtrlPtr->ledSettings[1].rampOffStep = 0x20;	//64ms, unit:2ms
	
	SDK_LED_HW_SetPattern(LedCtrlPtr);
	
	OSMEM_Put((U8 XDATA_PTR)LedCtrlPtr);
}


/* 
 * SDK_LED_SetHWBySectorDataSample: sample code to set led HW by sector data
 */
PUBLIC void SDK_LED_SetHWBySectorDataSample(U8 ledIndex)
{
	LedPatternType XDATA_PTR LedCtrlPtr;
	
	LedCtrlPtr = SYS_MemoryGet(sizeof(LedPatternType));
	
	if(!LedCtrlPtr)
		ASSERT(FALSE);
	
	SDK_LED_DRV_LoadPattern(ledIndex, LedCtrlPtr);
	
	SDK_LED_HW_SetPattern(LedCtrlPtr);
	
	OSMEM_Put((U8 XDATA_PTR)LedCtrlPtr);
}

