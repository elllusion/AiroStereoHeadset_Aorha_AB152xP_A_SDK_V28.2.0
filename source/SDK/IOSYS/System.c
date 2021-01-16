#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\MMI\MMI_Base.h"

#define SYSTEM_PWR_OFF_CB	0

/*  SDK_System_IsPowerOn: query if system is powered-on
 *	<output>
 *		BOOL: TRUE-->On; FALSE-->Off
 */
PUBLIC BOOL SDK_System_IsPowerOn(void)
{
	return MMI_IsPowerOn();
}

#if SYSTEM_PWR_OFF_CB
/*  SDK_CB_System_PowerOff: callback function which is called in NDK when power event is triigered
 */
PRIVATE void SDK_CB_System_PowerOff(void)
{

}
#endif

PUBLIC void SDK_System_Init()
{
#if SYSTEM_PWR_OFF_CB
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_SYS_POWER_OFF, API_GRP_IOSYS, (VFUN)SDK_CB_System_PowerOff);
#endif
}