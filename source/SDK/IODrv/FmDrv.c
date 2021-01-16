#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"
#include "..\Driver\Driver_FM.h"
#include "..\MMI\MMI.h"



#if SDK_FM_ENABLE

extern VFUN XDATA api_iosys_evt_func[API_GRP_IOSYS_CB_CNT];

PRIVATE void FMDrv_AUDIO_FMMode(void)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_FM_MODE])
	{
		((API_IOSYS_FM_FM_MODE_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_FM_MODE])();
	}
}

PRIVATE void FMDrv_AUDIO_BTMode(void)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_BT_MODE])
	{
		((API_IOSYS_FM_BT_MODE_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_BT_MODE])();
	}
}
	
PRIVATE U16 FMDrv_GetFreq(void)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_GET_FREQ])
	{
		return ((API_IOSYS_FM_GET_FREQ_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_GET_FREQ])();
	}
	else
	{
		LightDbgPrint("FM HAL is not ready!");
		return 0;
	}
}

PRIVATE BOOL FMDrv_SetFreq( S16 curf )  /* input value: 875 - 1080 ( 87.5 MHz - 108.0 MHz)*/
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_SET_FREQ])
	{
		return ((API_IOSYS_FM_SET_FREQ_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_SET_FREQ])((U16)curf);
	}
	else
	{
		LightDbgPrint("FM HAL is not ready!");
		ASSERT(FALSE);
		return FALSE;
	}
}

PRIVATE BOOL FMDrv_Seek(BOOL isUp)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_SEEK])
	{
		return ((API_IOSYS_FM_SEEK_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_SEEK])(isUp);
	}
	else
	{
		LightDbgPrint("FM HAL is not ready!");
		ASSERT(FALSE);
		return FALSE;
	}
}

PRIVATE BOOL FMDrv_Mute(U8 mute)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_MUTE])
	{
		return ((API_IOSYS_FM_MUTE_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_MUTE])(mute);
	}
	else
	{
		LightDbgPrint("FM HAL is not ready!");
		ASSERT(FALSE);
		return FALSE;
	}
}

PRIVATE void FMDrv_SetVolume (U8 vol)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_VOL])
	{
		((API_IOSYS_FM_VOL_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_VOL])(vol);
	}
	else
	{
		LightDbgPrint("FM HAL is not ready!");
		ASSERT(FALSE);
	}
} 

PUBLIC void FMDrv_PowerOn(U8 vol,U8 seekTH)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_ON])
	{
		((API_IOSYS_FM_ON_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_ON])(vol, seekTH);
	}
}

PUBLIC void FMDrv_PowerOff(void)
{
	if(api_iosys_evt_func[API_GRP_IOSYS_CB_FM_OFF])
	{
		((API_IOSYS_FM_OFF_CB_FUNC)api_iosys_evt_func[API_GRP_IOSYS_CB_FM_OFF])();
	}
}


typedef struct
{
	U8 state;
	U8 vol;
	S16 freq;
	U8 isMute;
	OST XDATA_PTR	timer;
	OST XDATA_PTR	cmdTimeOutTimer;
}SDK_FM_CTL_STRU;

SDK_FM_CTL_STRU XDATA gSDK_FM_ctl;

PRIVATE void FM_ClearState(void)
{
	gSDK_FM_ctl.state = 0;
	SYS_ReleaseTimer(&gSDK_FM_ctl.cmdTimeOutTimer);
	SYS_ReleaseTimer(&gSDK_FM_ctl.timer);
}

enum
{
	FM_POWER_ON_STATE_PWR_ON,
	FM_POWER_ON_STATE_MUTE_ON,
	FM_POWER_ON_STATE_SET_FREQ,
	FM_POWER_ON_STATE_SET_VOL,
	FM_POWER_ON_STATE_ON_OFF_MUTE,
	FM_POWER_ON_STATE_WAIT_SIGNAL_READY,
};

PUBLIC U8 SDK_FM_PowerOnHandle(void)
{
	if(SYS_IsTimerExpired(&gSDK_FM_ctl.cmdTimeOutTimer))
	{
		FM_ClearState();
		return FM_CMD_STATUS_FAIL;
	}
	
	switch(gSDK_FM_ctl.state)
	{
		case FM_POWER_ON_STATE_PWR_ON:
			SYS_SetTimer(&gSDK_FM_ctl.cmdTimeOutTimer, SDK_FM_CMD_TIMEOUT);
			FMDrv_PowerOn(gSDK_FM_ctl.vol, gMMI_nvram.fmSeekTH);
			gSDK_FM_ctl.state = FM_POWER_ON_STATE_MUTE_ON;
			break;
		case FM_POWER_ON_STATE_MUTE_ON:
			if(FMDrv_Mute(1))
			{
				gSDK_FM_ctl.state = FM_POWER_ON_STATE_SET_FREQ;
			}		
			break;
		case FM_POWER_ON_STATE_SET_FREQ:
			if(FMDrv_SetFreq(gSDK_FM_ctl.freq))
			{
				gSDK_FM_ctl.state = FM_POWER_ON_STATE_SET_VOL;
			}		
			break;
		case FM_POWER_ON_STATE_SET_VOL:
			FMDrv_SetVolume(gSDK_FM_ctl.vol);
			gSDK_FM_ctl.state = FM_POWER_ON_STATE_ON_OFF_MUTE;
			break;
		case FM_POWER_ON_STATE_ON_OFF_MUTE:
			if(FMDrv_Mute(gSDK_FM_ctl.isMute))
			{
				SYS_SetTimer(&gSDK_FM_ctl.timer,  SDK_FM_WAIT_ON_SIGNAL_READY_TIME);  
				gSDK_FM_ctl.state = FM_POWER_ON_STATE_WAIT_SIGNAL_READY;
			}
			break;
		case FM_POWER_ON_STATE_WAIT_SIGNAL_READY:
			if(SYS_IsTimerExpired(&gSDK_FM_ctl.timer))
			{
				FM_ClearState();
				return FM_CMD_STATUS_OK;
			}		
			break;
	}
	return FM_CMD_STATUS_WAITING;
}

PUBLIC void SDK_FM_AbortPowerOn(void)
{
	if(gSDK_FM_ctl.state)
	{
		FMDrv_PowerOff();
	}
	FM_ClearState();
}

PUBLIC void SDK_FM_PowerOffHandle(void)
{
	FMDrv_PowerOff();
	FM_ClearState();
}

enum
{
	FM_SEEK_STATE_INIT,
	FM_SEEK_STATE_SEEK,
	FM_SEEK_STATE_MUTE,
	FM_SEEK_STATE_SET_FREQ,
	FM_SEEK_STATE_UNMUTE,
};
PUBLIC U8 SDK_FM_SeekHandle(BOOL isUp)
{
	if(SYS_IsTimerExpired(&gSDK_FM_ctl.cmdTimeOutTimer))
	{
		FM_ClearState();
		return FM_CMD_STATUS_FAIL;
	}
	
	switch(gSDK_FM_ctl.state)
	{
		case FM_SEEK_STATE_INIT:
			SYS_SetTimer(&gSDK_FM_ctl.cmdTimeOutTimer, SDK_FM_CMD_TIMEOUT);
			gSDK_FM_ctl.state = FM_SEEK_STATE_SEEK;
			break;
		case FM_SEEK_STATE_SEEK:
			if(FMDrv_Seek(isUp))
				gSDK_FM_ctl.state = FM_SEEK_STATE_MUTE;
			break;
		case FM_SEEK_STATE_MUTE:
			if(FMDrv_Mute(1))
				gSDK_FM_ctl.state = FM_SEEK_STATE_SET_FREQ;		
			break;
		case FM_SEEK_STATE_SET_FREQ:
			if(FMDrv_SetFreq(FMDrv_GetFreq()))
			{
				gSDK_FM_ctl.freq = FMDrv_GetFreq();
				gSDK_FM_ctl.state = FM_SEEK_STATE_UNMUTE;	
			}		
			break;
		case FM_SEEK_STATE_UNMUTE:
			if(MMI_UNMUTE_WHEN_SEEK_TUNE_CHANNEL)
			{
				if(FMDrv_Mute(0))
				{
					gSDK_FM_ctl.isMute = FALSE;
					FM_ClearState();
					return FM_CMD_STATUS_OK;
				}
			}
			else
			{
				if(FMDrv_Mute(gSDK_FM_ctl.isMute))
				{
					FM_ClearState();
					return FM_CMD_STATUS_OK;
				}
			}
			break;		
	}
	return FM_CMD_STATUS_WAITING;
}

enum
{
	FM_TUNE_STATE_INIT,
	FM_TUNE_STATE_SEEK,
	FM_TUNE_STATE_MUTE,
	FM_TUNE_STATE_SET_FREQ,
	FM_TUNE_STATE_UNMUTE,
};
PUBLIC U8 SDK_FM_TuneHandle(BOOL isUp)
{
	U16 curFreq;
	
	if(SYS_IsTimerExpired(&gSDK_FM_ctl.cmdTimeOutTimer))
	{
		FM_ClearState();
		return FM_CMD_STATUS_FAIL;
	}
	
	switch(gSDK_FM_ctl.state)
	{
		case FM_TUNE_STATE_INIT:
			SYS_SetTimer(&gSDK_FM_ctl.cmdTimeOutTimer, SDK_FM_CMD_TIMEOUT);
			gSDK_FM_ctl.state = FM_TUNE_STATE_MUTE;
			break;

		case FM_TUNE_STATE_MUTE:
			if(FMDrv_Mute(1))
				gSDK_FM_ctl.state = FM_TUNE_STATE_SET_FREQ;		
			break;
		case FM_TUNE_STATE_SET_FREQ:
			curFreq = FMDrv_GetFreq();
			if(isUp)
			{
				curFreq = (curFreq>= MAX_FREQUENCE) ? MIN_FREQUENCE: curFreq+1;
			}
			else
			{
				curFreq = (curFreq<= MIN_FREQUENCE) ? MAX_FREQUENCE: curFreq-1;
			}

			if(FMDrv_SetFreq(curFreq))
			{
				gSDK_FM_ctl.freq = curFreq;
				gSDK_FM_ctl.state = FM_TUNE_STATE_UNMUTE;
			}		
			break;
		case FM_TUNE_STATE_UNMUTE:
			if(MMI_UNMUTE_WHEN_SEEK_TUNE_CHANNEL)
			{
				if(FMDrv_Mute(0))
				{
					gSDK_FM_ctl.isMute = FALSE;
					FM_ClearState();
					return FM_CMD_STATUS_OK;
				}
			}
			else
			{
				if(FMDrv_Mute(gSDK_FM_ctl.isMute))
				{
					FM_ClearState();
					return FM_CMD_STATUS_OK;
				}
			}
			break;	
	}
	return FM_CMD_STATUS_WAITING;
}

enum
{
	FM_MUTE_STATE_INIT,
	FM_MUTE_STATE_MUTE,
	FM_MUTE_STATE_SET_FREQ,
	FM_MUTE_STATE_UNMUTE,
};
PUBLIC U8 SDK_FM_MuteHandle(BOOL isMute)
{
	if(SYS_IsTimerExpired(&gSDK_FM_ctl.cmdTimeOutTimer))
	{
		FM_ClearState();
		return FM_CMD_STATUS_FAIL;
	}
	
	switch(gSDK_FM_ctl.state)
	{
		case FM_MUTE_STATE_INIT:
			SYS_SetTimer(&gSDK_FM_ctl.cmdTimeOutTimer, SDK_FM_CMD_TIMEOUT);
			gSDK_FM_ctl.state = FM_TUNE_STATE_MUTE;
			break;

		case FM_MUTE_STATE_MUTE:
			if(FMDrv_Mute(isMute))
			{
				gSDK_FM_ctl.isMute = isMute;
				FM_ClearState();
				return FM_CMD_STATUS_OK;
			}
			break;
	}
	return FM_CMD_STATUS_WAITING;
}


PUBLIC void SDK_FM_VolumeHandle(U8 vol)
{
	FMDrv_SetVolume(vol);
	gSDK_FM_ctl.vol = vol;
}

PUBLIC void SDK_FM_AudioOn(void)
{
	FMDrv_AUDIO_FMMode();
}

PUBLIC void SDK_FM_AudioOff(void)
{
	FMDrv_AUDIO_BTMode();
}

PUBLIC void SDK_FM_SetVolume(U8 vol)
{
	gSDK_FM_ctl.vol = vol;
}

PUBLIC void SDK_FM_SetFrequency(S16 freq)
{
	gSDK_FM_ctl.freq = freq;
}

PUBLIC void SDK_FM_SetMute(U8 isMute)
{
	gSDK_FM_ctl.isMute = (isMute) ? TRUE : FALSE;
}

PUBLIC S16 SDK_FM_GetFrequency(void)
{
	return gSDK_FM_ctl.freq;
}

PUBLIC BOOL SDK_FM_GetMute(void)
{
	return (gSDK_FM_ctl.isMute) ? TRUE : FALSE;
}

#endif //SDK_FM_ENABLE

PUBLIC void SDK_FM_DRV_Init(void)
{
#if SDK_FM_ENABLE
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_GETMUTE, API_GRP_IODRV, (VFUN)SDK_FM_GetMute);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_GETFREQ, API_GRP_IODRV, (VFUN)SDK_FM_GetFrequency);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_SETMUTE, API_GRP_IODRV, (VFUN)SDK_FM_SetMute);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_SETFREQ, API_GRP_IODRV, (VFUN)SDK_FM_SetFrequency);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_SETVOL, API_GRP_IODRV, (VFUN)SDK_FM_SetVolume);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_AUDIO_OFF, API_GRP_IODRV, (VFUN)SDK_FM_AudioOff);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_AUDIO_ON, API_GRP_IODRV, (VFUN)SDK_FM_AudioOn);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_PWR_OFF, API_GRP_IODRV, (VFUN)SDK_FM_PowerOffHandle);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_PWR_ON, API_GRP_IODRV, (VFUN)SDK_FM_PowerOnHandle);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_ABT_PWR_ON, API_GRP_IODRV, (VFUN)SDK_FM_AbortPowerOn);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_SEEK, API_GRP_IODRV, (VFUN)SDK_FM_SeekHandle);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_TUNE, API_GRP_IODRV, (VFUN)SDK_FM_TuneHandle);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_MUTE, API_GRP_IODRV, (VFUN)SDK_FM_MuteHandle);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_FM_VOL, API_GRP_IODRV, (VFUN)SDK_FM_VolumeHandle);
#endif
}