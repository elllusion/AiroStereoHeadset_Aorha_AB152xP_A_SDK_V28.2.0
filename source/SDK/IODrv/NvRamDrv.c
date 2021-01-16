#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"
#include "..\SDK\include\NvRam.h"
#include "sector_merge_runtime.h"


#if SDK_NV_RAM
extern CUSTOMIZE_DATA_TYPE XDATA gSDK_nvram ALIGN(4);
BOOL XDATA isChecked;
PUBLIC void SDK_NvRamDrv_Polling(void)
{
	if(!isChecked)
	{
		if(gSDK_nvram.para[0] == 0xEE)
		{
			
			LightDbgPrint("SDK_NVRAM is set!!!");
		}
		isChecked = TRUE;		
	}
	
	if(gSDK_nvram.para[0] == 0)
	{
		gSDK_nvram.para[0] = 0xEE;
		SDK_NvRam_SetUpdateOn();
		LightDbgPrint("Try to set SDK_NVRAM!!!");
	}

	SDK_NvRam_PollingWriteFlash(); //If SDK_NvRam_PollingWriteFlash is polled, it will write flash as soon as possible. Otherwise flash will be written HALF_SEC later.
}
#endif