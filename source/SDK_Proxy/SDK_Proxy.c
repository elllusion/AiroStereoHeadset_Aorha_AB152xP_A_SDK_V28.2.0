#include "reside_flash.inc"

#include "rc.h"
#include "bt_config_profile.h"
#include "drivers.h"
#include "SDK_Proxy.h"

#if SDK_ENABLE
	extern void SDK_Init(void);
	extern void SDK_Polling(void);
	VFUN XDATA sdk_init_func = SDK_Init;
	VFUN XDATA sdk_polling_func = SDK_Polling;
#else
	VFUN XDATA sdk_init_func;
	VFUN XDATA sdk_polling_func;
#endif

	VFUN XDATA sdk_pcb_func;

VFUN XDATA api_iosys_evt_func[API_GRP_IOSYS_CB_CNT];
VFUN XDATA api_iodrv_evt_func[API_GRP_IODRV_CB_CNT];
VFUN XDATA api_profile_func[API_GRP_PF_CB_CNT];


void SDK_Proxy_RegisterAPI(U8 apiId, U8 apiGrp, VFUN func)
{
	switch(apiGrp)
	{
		case API_GRP_IOSYS:
			api_iosys_evt_func[apiId] = func;
			break;
		case API_GRP_IODRV:
			api_iodrv_evt_func[apiId] = func;
			break;
		case API_GRP_PROFILE:
			api_profile_func[apiId] = func;
			break;
	}
}


API_PERIODIC_FUNC XDATA api_iosys_periodic_func[PERIODIC_CALLBACK_FUNC_CNT];

void SDK_Proxy_PollingPeriodicCB(void)
{
	U8 apiId;
	

	
	for(apiId = 0; apiId < PERIODIC_CALLBACK_FUNC_CNT; apiId++)
	{
		if(api_iosys_periodic_func[apiId].isTimerSet)
		{
			if(OST_TimerExpired(&api_iosys_periodic_func[apiId].timer))
			{
				if(api_iosys_periodic_func[apiId].cbFunc)
					api_iosys_periodic_func[apiId].cbFunc();
				
				if(api_iosys_periodic_func[apiId].oneTimeOnly)
				{	
					api_iosys_periodic_func[apiId].isTimerSet = FALSE;
				}
				else
					OST_SetTimer(&api_iosys_periodic_func[apiId].timer, api_iosys_periodic_func[apiId].duration);
			}
		}
	}
}

void SDK_Proxy_RegisterPeriodicAPI(U8 apiId, VFUN func, U32 time, BOOL oneTime)
{
	api_iosys_periodic_func[apiId].cbFunc = func;
	api_iosys_periodic_func[apiId].duration = time;		
	OST_SetTimer(&api_iosys_periodic_func[apiId].timer, time);
	api_iosys_periodic_func[apiId].isTimerSet = TRUE;
	api_iosys_periodic_func[apiId].oneTimeOnly = oneTime;
	
	if(!sdk_pcb_func)
		sdk_pcb_func = (VFUN)SDK_Proxy_PollingPeriodicCB;
}