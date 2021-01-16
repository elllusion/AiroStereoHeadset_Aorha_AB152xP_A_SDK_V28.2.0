#include "reside_flash.inc"

#define _MMI_TIMER_C_

#include "rc.h"
#include "bt_config_profile.h"
	
#include "MMI.h"
#include "MMI_Timer.h"
#include "MMI_HFP.h"
#include "MMI_Driver.h"
#include "MMI_AVRCP.h"
#include "MMI_A2DP.h"
#include "MMI_State.h"

PUBLIC void MMI_TIMER_Init(void)
{
	//Initialize : Auto Power ON timer
	SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoPowerOnTimer, (U32)(gMMI_nvram.timerCtl.AutoPowerOnTimer) * 320L);
}


PUBLIC void MMI_TIMER_ReleaseConnectionTimer(U8 linkIndex)
{
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].sniffTimerPtr);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].checkRemoteAGAcceptEnterSniffTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].refreshEncKeyTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].nfcDetachIfNoProfileTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].roleSwitchTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delaySDPTimer);
	SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].delayRemoveConnectedTimer);
	
}

PUBLIC void MMI_TIMER_ReleaseAllLinksConnectionTimer(void)
{
	U8 i;
	for(i = 0; i < MAX_MULTI_POINT_NO ; i++)
	{
		MMI_TIMER_ReleaseConnectionTimer(i);
		SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[i].waitDetachTimer);
	}
}

PUBLIC void MMI_TIMER_ResetAutoSwitchOffTimer(void)
{
	if(!gMMI_ctl.activeLinkRecord)
	{
		if(gMMI_nvram.timerCtl.AutoSwitchOffTimer && !MMI_CheckNestState(MMI_EOF, MMI_CONDISCABLE))
		{
			SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffTimer, (U32)(gMMI_nvram.timerCtl.AutoSwitchOffTimer) * ONE_SEC);
		}
	}
	else
	{
		SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.sysTimer.autoSwitchOffTimer);
	}
}

PUBLIC void MMI_TIMER_ReleaseDetachTimer(U8 linkIndex)
{
	if(MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState())
	{
		SYS_ReleaseTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].waitDetachTimer);
	}
}

PUBLIC void MMI_TIMER_SetDetachTimer(U8 linkIndex, U32 time)
{
	SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[linkIndex].waitDetachTimer, time);
}