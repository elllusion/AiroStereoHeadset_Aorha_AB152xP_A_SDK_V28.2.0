#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"
#include "..\SDK\include\Pio.h"

#if AMP_ENABLE
PUBLIC void SDK_AMP_On(void)
{
	SDK_GPIO_SetIO(PIN_SET_OUTPUT, LEVEL_HIGH, AMP_PIO);
}

PUBLIC void SDK_AMP_Off(void)
{
	SDK_GPIO_SetIO(PIN_SET_OUTPUT, LEVEL_LOW, AMP_PIO);
}
#endif //AMP_ENABLE

PUBLIC void SDK_AMP_DRV_Init(void)
{
#if AMP_ENABLE
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_AMP_ON, API_GRP_IODRV, (VFUN)SDK_AMP_On);
	SDK_Proxy_RegisterAPI(API_GRP_IODRV_CB_AMP_OFF, API_GRP_IODRV, (VFUN)SDK_AMP_Off);	
#endif
}