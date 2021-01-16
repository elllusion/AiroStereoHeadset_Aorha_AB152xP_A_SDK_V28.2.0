//////////////////////////////////////////////////////////
// Copyright@ 2016 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "reside_flash.inc"

#include "os.h"
#include "bt_config_profile.h"
#include "MMI.h"
#include "MMI_Driver.h"
#include "MMI_SectorManager.h"
#include "MMI_LED.h"
#include "..\Driver\Driver_LED.h"
#include "MMI_AirModule.h"

PUBLIC void MMI_LED_SendLedFilterCmd(U8 index)
{
	if(MMI_SDK_DISABLE_LED_FILTER_FEAT)
		return;

	DRIVER_Cmd_LED_FilterLed(gMMI_nvram.ledFilterMap[index].type, gMMI_nvram.ledFilterMap[index].para);
}

PUBLIC void MMI_LED_SendFGLedCmd(U16 eventCode, U8 evtTableIndex)
{
	if(gMMI_nvram.evtParaCtl[evtTableIndex].ledDataIndex == LED_INVALID)
		return;

	if(MMI_SDK_DISABLE_FOREGROUND_LED_FEAT)
		return;
		
	if(gMMI_nvram.evtParaCtl[evtTableIndex].ledDataIndex & LED_RESET_FG_BIT)
		DRIVER_Cmd_LED_Restart(DRIVER_CMD_LED_RESTART_PARA_ONLY_FG);

	DRIVER_Cmd_LED_FGLed((gMMI_nvram.evtParaCtl[evtTableIndex].ledDataIndex & ~(LED_RESET_FG_BIT|LED_OVERRIDE_BIT)),
						(gMMI_nvram.evtParaCtl[evtTableIndex].Timeout & EVENT_LED_FG_TIMEOUT_MASK),
						eventCode,
						((gMMI_nvram.evtParaCtl[evtTableIndex].ledDataIndex & LED_OVERRIDE_BIT)? TRUE:FALSE ));

	if(eventCode == MEDIA_EVT_KEY_RESET_PAIRED_DEVICES)
	{
		gMMI_ctl.driverCtl.resetPairedDeviceMediaCount++;
	}
}

PUBLIC void MMI_LED_SendBGLedCmd(U8 ledDataIndex)
{
	if(MMI_SDK_DISABLE_BACKGROUND_LED_FEAT)
		return;

	#ifdef AIR_MODULE
	MMI_AIR_SendBGLedCmd(ledDataIndex);
	#endif
	DRIVER_Cmd_LED_BGLed(ledDataIndex);
}

PUBLIC void MMI_LED_KeyOnOffHandler(U8 ledCmd)
{
	U8 misc = gMMI_DRV_ctl.isMiscEnabled;
	
	switch(ledCmd)
	{
	 	case ENABLE_LED:
			gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled &= (~IS_LED_DISABLED);
			gMMI_DRV_ctl.isMiscEnabled &= (~IS_LED_DISABLED);
			break;

		case DISABLE_LED:
			gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled |= IS_LED_DISABLED;
			gMMI_DRV_ctl.isMiscEnabled |= IS_LED_DISABLED;
			break;

		case TOGGLE_LED:
			gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled ^= IS_LED_DISABLED;
			gMMI_DRV_ctl.isMiscEnabled ^= IS_LED_DISABLED;
			if (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_LED_DISABLED)
			{
			 	//LED is enabled, so toggle to disabled
				MMI_PushMediaEvent(MEDIA_EVT_KEY_DISABLE_LED);
			}
			else
			{
				//LED is disabled, so toggle to enabled
				MMI_PushMediaEvent(MEDIA_EVT_KEY_ENABLE_LED);
			}
			break;
	}

	if (gMMI_DRV_ctl.isMiscEnabled != misc)
		DRIVER_Cmd_LED_Restart(DRIVER_CMD_LED_RESTART_PARA_FG_AND_BG);

	MMI_SECTOR_SetUpdateFlag(TRUE);
}

#ifdef LIGHTING_ENABLE
//U8 led_R_tmp,led_G_tmp,led_B_tmp; //[Vanessa]This is unused. Only assign in code.
//U8 RGB_old_v[3]; //[Vanessa]This is unused. Only assign in code.
U8 RGB_new_v[3];
//U8 rgb_val_out[3]; //[Vanessa]This is unused. Only assign in code.
//U8 RGB_change_time,RGB_update_key,RGB_shift_k,RGB_shift_v; //[Vanessa]This is unused.

//0  16	 33  50  67  84  101  118  135  152  169  186  203  220  237  254
//	1	2	3	4   5	6	7	 8	   9    10  11   12   13   14   15
U8 CODE Led_R[16][16]={ 
// 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15    <-- Band
{  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  // Vol  0
{  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  // Vol  1
{  1,  7,  1,  9,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  // Vol  2
{  1, 10,  7,  4,  4,  4,  4, 15, 15,  4,  4,  1,  4, 15,  4,  1},  // Vol  3
{  1,  1,  7,  4,  4,  4,  4, 12, 11, 12,  4,  1,  4, 11,  4,  1},  // Vol  4
{  0,  5,  5,  4,  4,  4,  4, 12, 12, 12,  4,  0,  4, 12,  4,  0},  // Vol  5
{  0,  5,  5,  5,  5,  5,  5, 13, 13, 12,  5,  0,  5, 13,  5,  0},  // Vol  6
{  0,  7,  7,  6,  6,  6,  6, 14, 14, 12,  6,  0,  6, 14,  6,  0},  // Vol  7
{  1,  6, 10,  6,  6,  6,  6, 15, 15, 12,  6,  1,  6, 15,  6,  1},  // Vol  8
{  1,  6,  6,  6,  6, 11, 11, 11, 15, 11,  6,  1, 11, 15,  6,  1},  // Vol  9
{  1, 11, 11,  6,  7, 13, 13, 13, 15, 13,  7,  1, 13, 15,  7,  1},  // Vol 10
{  3, 11, 11,  7, 10, 14, 14, 14, 15, 14, 10,  3,  3, 15, 10,  3},  // Vol 11
{  4, 11, 11, 10, 13, 13, 13, 13, 15, 13, 13,  4, 12, 15, 13,  4},  // Vol 12
{  6, 11, 11, 11, 13, 15, 13, 13, 15, 13, 13,  6, 13, 15, 13,  6},  // Vol 13
{  6, 11, 11,  6, 14, 15, 15, 15, 15, 15, 14,  6, 15, 15, 15,  6},  // Vol 14
{ 11, 11,  6,  6, 15, 15, 15, 15, 15, 15, 15, 11, 15, 15, 15, 11}}; // Vol 15

//0  16	 33  50  67  84  101  118  135  152  169  186  203  220  237  254
//	1	2	3	4   5	6	7	 8	   9    10  11   12   13   14   15                       
U8 CODE Led_G[16][16]={ 
// 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15    <-- Band
{  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4},	// Vol  0
{  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4}, 	// Vol  1
{  4, 13,  4, 12,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4},	// Vol  2
{  4, 15, 13,  7,  7,  7,  7, 15, 15,  7,  7,  4,  7, 15,  7,  4},	// Vol  3
{  4,  7, 13,  7,  7,  7,  7,  2,  0, 12,  7,  4,  7,  0,  7,  4},	// Vol  4
{  4,  9,  9,  7,  7,  7,  7,  0,  0, 12,  7,  4,  7,  0,  7,  4},	// Vol  5
{  4,  9,  9,  9,  9,  9,  9,  0,  0, 12,  9,  4,  9,  0,  9,  4},	// Vol  6
{  4, 13, 13, 11, 11, 11, 11,  0,  0, 12, 11,  4, 11,  0, 11,  4},	// Vol  7
{  5, 14, 15, 11, 11, 13, 13, 13,  0, 13, 11,  5, 13,  0, 11,  5},	// Vol  8
{  5, 14, 14, 11, 11, 14, 14, 14,  0, 14, 11,  5, 14,  0, 11,  5},	// Vol  9
{  5, 15, 15, 11, 13,  8,  8,  0,  0, 11, 13,  5,  8,  0, 13,  5},	// Vol  10
{  7, 15, 15, 13, 15, 12, 12, 12,  0, 12, 15,  7, 12,  0, 15,  7},	// Vol  11
{  8, 15, 15, 15,  8, 13, 13, 13,  0, 13, 14,  8, 12, 15, 14,  8},	// Vol  12
{  9, 15, 15, 15, 14, 15, 13, 13,  0, 13, 14,  9, 13,  0, 14,  9},	// Vol  13
{ 14, 15, 15, 14, 15, 15, 15, 15,  0, 15, 15, 14, 15,  0, 15, 14},	// Vol  14
{ 15, 15, 14, 14, 15, 15, 15, 15,  0, 15, 15, 15, 15,  0, 15, 15}};	// Vol  15

//0  16	 33  50  67  84  101  118  135  152  169  186  203  220  237  254
//	1	2	3	4   5	6	7	 8	   9    10  11   12   13   14   15
U8 CODE Led_B[16][16]={ 
// 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15    <-- Band
{ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},	// Vol  0
{ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},	// Vol  1
{ 10, 10, 10, 15, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},	// Vol  2
{ 10,  5, 10,  1,  1,  1,  1,  1,  0,  1,  1, 10,  1,  0,  1, 10},	// Vol  3
{ 10, 12, 10,  1,  1,  1,  1,  0,  0,  0,  1, 10,  1,  0,  1, 10},	// Vol  4
{ 13,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1, 15,  1,  0,  1, 13},	// Vol  5
{ 13,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1, 15,  1,  0,  1, 13},	// Vol  6
{ 13, 10, 10,  1,  1,  1,  1,  0,  0,  0,  1, 15,  1,  0,  1, 13},	// Vol  7
{ 15, 15,  5,  1,  1,  1,  1,  0,  0,  0,  1, 15,  1,  0,  1, 15},	// Vol  8
{ 15, 15, 15,  1,  1,  1,  1,  1,  0,  1,  1, 15,  1,  0,  1, 15},	// Vol  9
{ 15, 15, 15,  1,  1,  1,  1,  1,  0,  1,  1, 15,  1,  0, 10, 15},	// Vol  10
{ 15, 15, 15,  1,  5,  1,  1,  1,  0,  1,  5, 15,  1,  0,  5, 15},	// Vol  11
{ 15, 15, 15,  5,  1,  0,  0,  0,  0,  0,  1, 15,  0,  0,  1, 15},	// Vol  12
{ 15, 15, 15, 15,  1,  0,  0,  0,  0,  0,  1, 15,  0,  0,  1, 15},	// Vol  13
{ 15, 15, 15, 15,  3,  0,  0,  0,  0,  0,  3, 15,  0,  0,  0, 15},	// Vol  14
{ 15, 15, 15, 15,  0,  0,  0,  0,  0,  0,  0, 15,  0,  0,  0, 15}};	// Vol  15

PUBLIC void MMI_LED_AlarmClock(U8 enable)
{
	U8 ledIndex;
	LedPatternType XDATA_PTR ledDataPtr = (LedPatternType XDATA_PTR)SYS_MemoryGet(sizeof(LedPatternType));
	OSMEM_memset_xdata((U8 XDATA_PTR)ledDataPtr, 0, sizeof(LedPatternType));

	for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
	{		
		ledDataPtr->ledSettings[ledIndex].ledFollowMultiplierControl = 0x01;		
		ledDataPtr->ledSettings[ledIndex].pwmSetting = 0x80;
		ledDataPtr->ledSettings[ledIndex].t1 = 100;
		ledDataPtr->ledSettings[ledIndex].t2 = 30;
		ledDataPtr->ledSettings[ledIndex].t3 = 0;
		ledDataPtr->ledSettings[ledIndex].rampOnStep= 15;
		ledDataPtr->ledSettings[ledIndex].rampOffStep= 15;
		if(enable)
			ledDataPtr->ledSettings[ledIndex].onOff = LED_ON;
		else
			ledDataPtr->ledSettings[ledIndex].onOff = LED_OFF;
	}
	if(enable)
		LED_SetPatternToHW(ledDataPtr);
	else
		LED_TurnOffAllLeds();
	
	OSMEM_Put((U8 XDATA_PTR)ledDataPtr);
}
PUBLIC void MMI_LED_LightingMode(U8 LightMode)
{
	U8 ledIndex;
	LedPatternType XDATA_PTR ledDataPtr = (LedPatternType XDATA_PTR)SYS_MemoryGet(sizeof(LedPatternType));
	OSMEM_memset_xdata((U8 XDATA_PTR)ledDataPtr, 0, sizeof(LedPatternType));
	switch(LightMode)
	{
		case 00:
			for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
			{		
				ledDataPtr->ledSettings[ledIndex].ledFollowMultiplierControl = 0x01;		
				ledDataPtr->ledSettings[ledIndex].pwmSetting = 0xF0;
				ledDataPtr->ledSettings[ledIndex].t1 = 80;
				ledDataPtr->ledSettings[ledIndex].t2 = 10;
				ledDataPtr->ledSettings[ledIndex].t3 = 0;
				ledDataPtr->ledSettings[ledIndex].rampOnStep= 15;
				ledDataPtr->ledSettings[ledIndex].rampOffStep= 7;
			}
			break;
		case 01:
			for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
			{		
				ledDataPtr->ledSettings[ledIndex].ledFollowMultiplierControl = 0x01;		
				ledDataPtr->ledSettings[ledIndex].pwmSetting = 0xF0;				
				ledDataPtr->ledSettings[ledIndex].t3 = 0;
				ledDataPtr->ledSettings[ledIndex].rampOnStep= 15;
				ledDataPtr->ledSettings[ledIndex].rampOffStep= 0;
			}
			ledDataPtr->ledSettings[0].t1 = 50;
			ledDataPtr->ledSettings[0].t2 = 20;
			ledDataPtr->ledSettings[1].t1 = 40;
			ledDataPtr->ledSettings[1].t2 = 20;
			ledDataPtr->ledSettings[2].t1 = 30;
			ledDataPtr->ledSettings[2].t2 = 20;
			break;		
		case 02:
			for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
			{		
				ledDataPtr->ledSettings[ledIndex].ledFollowMultiplierControl = 0x01;		
				ledDataPtr->ledSettings[ledIndex].pwmSetting = 0xF0;				
				ledDataPtr->ledSettings[ledIndex].t3 = 0;
				ledDataPtr->ledSettings[ledIndex].rampOnStep= 15;
				ledDataPtr->ledSettings[ledIndex].rampOffStep= 0;
			}
			ledDataPtr->ledSettings[0].t1 = 5;
			ledDataPtr->ledSettings[0].t2 = 2;
			ledDataPtr->ledSettings[1].t1 = 4;
			ledDataPtr->ledSettings[1].t2 = 2;
			ledDataPtr->ledSettings[2].t1 = 3;
			ledDataPtr->ledSettings[2].t2 = 2;		
			break;
	}
	for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
	{
		ledDataPtr->ledSettings[ledIndex].onOff = LED_ON;		
	}		
	LED_SetPatternToHW(ledDataPtr);
	OSMEM_Put((U8 XDATA_PTR)ledDataPtr);
}
PUBLIC void MMI_LED_Set_RGB(U8 LedR, U8 LedG, U8 LedB)
{
	U8 ledIndex;
	LedPatternType XDATA_PTR ledDataPtr = (LedPatternType XDATA_PTR)SYS_MemoryGet(sizeof(LedPatternType));
	OSMEM_memset_xdata((U8 XDATA_PTR)ledDataPtr, 0, sizeof(LedPatternType));
	
	LedR = (LedR/17)<<4;
	LedG = (LedG/17)<<4;
	LedB = (LedB/17)<<4;
	for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
	{		
		ledDataPtr->ledSettings[ledIndex].ledFollowMultiplierControl = 0x01;
		switch(ledIndex)
		{
			case 0:
				ledDataPtr->ledSettings[ledIndex].pwmSetting = LedR;
				break;
			case 1:
				ledDataPtr->ledSettings[ledIndex].pwmSetting = LedG;
				break;
			case 2:
				ledDataPtr->ledSettings[ledIndex].pwmSetting = LedB;
				break;
			default:					
				ledDataPtr->ledSettings[ledIndex].pwmSetting = 0xF0;
				break;
		}
		ledDataPtr->ledSettings[ledIndex].t1 = 0xFF;
		ledDataPtr->ledSettings[ledIndex].t2 = 0;
		ledDataPtr->ledSettings[ledIndex].t3 = 0;
		ledDataPtr->ledSettings[ledIndex].rampOnStep= 15;
		ledDataPtr->ledSettings[ledIndex].rampOffStep= 0;
	}
	for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
	{
		ledDataPtr->ledSettings[ledIndex].onOff = LED_ON;
	}		
	LED_SetPatternToHW(ledDataPtr);
	OSMEM_Put((U8 XDATA_PTR)ledDataPtr);
}
PUBLIC void MMI_LED_Set_MusicYouCanSee(U8 DSP_Index, U8 DSP_Volume)
{
	U8 ledIndex;
	U32 led_tmp;
	LedPatternType XDATA_PTR ledDataPtr = (LedPatternType XDATA_PTR)SYS_MemoryGet(sizeof(LedPatternType));
	OSMEM_memset_xdata((U8 XDATA_PTR)ledDataPtr, 0, sizeof(LedPatternType));

	for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
	{		
		ledDataPtr->ledSettings[ledIndex].ledFollowMultiplierControl = 0x01;
		switch(ledIndex)
		{
			case 0:
                led_tmp = ((Led_R[DSP_Volume][DSP_Index]<<4)&0xF0);//*DSP_scaling;
                //RGB_old_v[ledIndex] = RGB_new_v[ledIndex]; //[Vanessa]RGB_old_v is unused
                RGB_new_v[ledIndex] = led_tmp;
                break;
			case 1:
                led_tmp = ((Led_G[DSP_Volume][DSP_Index]<<4)&0xF0);//*DSP_scaling;
                //RGB_old_v[ledIndex] = RGB_new_v[ledIndex]; //[Vanessa]RGB_old_v is unused
                RGB_new_v[ledIndex] = led_tmp;
                break;
			case 2:
                led_tmp = ((Led_B[DSP_Volume][DSP_Index]<<4)&0xF0);//*DSP_scaling;
                //RGB_old_v[ledIndex] = RGB_new_v[ledIndex]; //[Vanessa]RGB_old_v is unused
                RGB_new_v[ledIndex] = led_tmp;
                break;
			default:					
                ledDataPtr->ledSettings[ledIndex].pwmSetting = 0xF0;
                break;
		}
		ledDataPtr->ledSettings[ledIndex].t1 = 0xFF;
		ledDataPtr->ledSettings[ledIndex].t2 = 0;
		ledDataPtr->ledSettings[ledIndex].t3 = 0;
		ledDataPtr->ledSettings[ledIndex].rampOnStep = 0;
		ledDataPtr->ledSettings[ledIndex].rampOffStep = 0;
	}			
	for(ledIndex = 0; ledIndex < 3; ledIndex++)
	{
		//These two lines are equal to the below 
		//rgb_val_out[ledIndex] = RGB_new_v[ledIndex];
		//ledDataPtr->ledSettings[ledIndex].pwmSetting = rgb_val_out[ledIndex]&0xF0;
		
		ledDataPtr->ledSettings[ledIndex].pwmSetting = RGB_new_v[ledIndex] & 0xF0;
	}	
	//led_R_tmp = rgb_val_out[0]&0xF0;
	//led_G_tmp = rgb_val_out[1]&0xF0;
	//led_B_tmp = rgb_val_out[2]&0xF0;
	for(ledIndex = 0; ledIndex < LED_NO; ledIndex++)
	{
		ledDataPtr->ledSettings[ledIndex].onOff = LED_ON;
	}
    LED_SetPatternToHW(ledDataPtr);
    OSMEM_Put((U8 XDATA_PTR)ledDataPtr);
}
#endif
