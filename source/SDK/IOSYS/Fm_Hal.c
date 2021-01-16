#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"


#if SDK_FM_ENABLE
	#if !SDK_ENABLE
		#error "SDK_ENABLE is not defined"
	#endif
#endif

#ifdef FM_AR1000_CHIP
	#include "Fm_Hal_AR1000.c"
#endif

#if SDK_FM_ENABLE

/*  SDK_CB_FM_HAL_FMMode: callback function which is called in NDK when audio is turned on for FM
 */
PRIVATE void SDK_CB_FM_HAL_FMMode(void)
{
#ifdef FM_AR1000_CHIP
	AR1000_FM_Mode();
#endif
}

/*  SDK_CB_FM_HAL_BTMode: callback function which is called in NDK when audio is turned on for BT
 */
PRIVATE void SDK_CB_FM_HAL_BTMode(void)
{
#ifdef FM_AR1000_CHIP
	AR1000_BT_Mode();
#endif
}

/*  SDK_CB_FM_HAL_GetFreq: callback function which is called in NDK when frequency is needed
 *	<output> U16	: FM frequency
 */
PRIVATE U16 SDK_CB_FM_HAL_GetFreq(void)
{
#ifdef FM_AR1000_CHIP
	return AR1000_GetReadFREQ();
#else
	return 875;
#endif
}

/*  SDK_CB_FM_HAL_SetFreq: callback function which is called in NDK when frequency is set
 *	<input1> 
 *		U16	freq	: FM frequency. 
 * 		value range	: 875 - 1080 ( 87.5 MHz - 108.0 MHz)
 *  <output>
 *		BOOL		: TRUE-->OK, FALSE-->Fail
 */
PRIVATE BOOL SDK_CB_FM_HAL_SetFreq(U16 freq)
{
#ifdef FM_AR1000_CHIP
	return AR1000_SetFreq(freq);
#else
	UNUSED(freq);
	return TRUE;
#endif
}

/*  SDK_CB_FM_HAL_Seek: callback function which is called in NDK when seek is needed
 *	<input1> 
 *		BOOL isUp	: seek up or seek down 
 * 		value range	: TRUE--> seek up; FALSE --> seek down.
 *  <output>
 *		BOOL		: TRUE-->OK, FALSE-->Fail
 */
PRIVATE BOOL SDK_CB_FM_HAL_Seek(BOOL isUp)
{
#ifdef FM_AR1000_CHIP
	return AR1000_Seek(isUp);
#else 
	UNUSED(isUp);
	return TRUE;
#endif
}

/*  SDK_CB_FM_HAL_Mute: callback function which is called in NDK when mute or unmute is needed
 *	<input1> 
 *		BOOL isMute	: mute or unmute 
 * 		value range	: TRUE--> mute; FALSE --> unmute.
 *  <output>
 *		BOOL		: TRUE-->OK, FALSE-->Fail
 */
PRIVATE BOOL SDK_CB_FM_HAL_Mute(U8 isMute)
{
#ifdef FM_AR1000_CHIP
	return AR1000_Mute(isMute);
#else 
	UNUSED(isMute);
	return TRUE;
#endif
}

/*  SDK_CB_FM_HAL_Vol: callback function which is called in NDK when vol is changed
 *	<input1> 
 *		U8 vol		: volume level 
 * 		value range	: 0 - 15
 */
PRIVATE void SDK_CB_FM_HAL_Vol(U8 vol)
{
#ifdef FM_AR1000_CHIP
	AR1000_SetVol(vol);
#else 
	UNUSED(vol);
#endif
}

/*  SDK_CB_FM_HAL_PowerOn: callback function which is called in NDK when fm needs to be on
 *	<input1> 
 *		U8 vol		: volume level 
 * 		value range	: 0 - 15
 *	<input2> 
 *		U8 seekTH	: seek threshold
 * 		value range	: depends on IC
 */
PRIVATE void SDK_CB_FM_HAL_PowerOn(U8 vol, U8 seekTH)
{
#ifdef FM_AR1000_CHIP
	AR1000_PowerOn(vol, seekTH);
#else 
	UNUSED(vol);
	UNUSED(seekTH);
#endif
}

/*  SDK_CB_FM_HAL_PowerOff: callback function which is called in NDK when fm needs to be off
 */
PRIVATE void SDK_CB_FM_HAL_PowerOff(void)
{
#ifdef FM_AR1000_CHIP
	AR1000_PowerOff();
#endif
}

PRIVATE void SDK_FM_HAL_Init(void)
{
#ifdef FM_AR1000_CHIP
	AR1000_Init();
#endif
}

#endif//SDK_FM_ENABLE

PUBLIC void SDK_FM_Init(void)
{
#if SDK_FM_ENABLE
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_FM_MODE, 	API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_FMMode);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_BT_MODE, 	API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_BTMode);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_GET_FREQ, 	API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_GetFreq);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_SET_FREQ, 	API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_SetFreq);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_SEEK, 		API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_Seek);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_MUTE, 		API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_Mute);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_VOL, 		API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_Vol);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_ON, 		API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_PowerOn);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_FM_OFF, 		API_GRP_IOSYS, (VFUN)SDK_CB_FM_HAL_PowerOff);
	SDK_FM_HAL_Init();
#endif
}