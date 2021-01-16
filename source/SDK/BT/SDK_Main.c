#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\SDK\include\SDK_Config.h"


#include "..\SDK\include\Key.h"
#include "..\SDK\include\Fm_Hal.h"
#include "..\SDK\include\IIC.h"
#include "..\SDK\include\Charger.h"
#include "..\SDK\include\Battery.h"
#include "..\SDK\include\System.h"
#include "..\SDK\include\Uart.h"
#include "..\SDK\include\Uart_Hal.h"
#include "..\SDK\include\FmDrv.h"
#include "..\SDK\include\AmpDrv.h"
#include "..\SDK\include\NvRam.h"
#include "..\SDK\include\A2DP_VendorCodec.h"
#include "..\SDK\include\SDK_HCI.h"
#include "..\SDK\include\SDK_HFP.h"
#include "..\SDK\include\SDK_A2DP.h"
#include "..\SDK\include\SDK_AVRCP.h"
#include "..\SDK\include\SDK_Profile.h"
#include "..\SDK\include\Mem.h"
#include "..\SDK\include\VendorProfile.h"
#include "..\SDK\example\Pedometer\Pedometer.h"
#include "..\SDK\include\SDK_AirAppCustomer.h"
#include "..\SDK\include\SDK_SDP.h"

void SDK_Init(void)
{
    #if SDK_MEMORY_ALLOCATION
    unsigned char *ucPtr;
    #endif
    
	SDK_Key_Init();
	SDK_FM_Init();
	SDK_IIC_Init();
	SDK_Charger_Init();
	SDK_Battery_Init();
	SDK_System_Init();
	SDK_UART_Init();
	SDK_Uart_HAL_Init();
	SDK_FM_DRV_Init();
	SDK_AMP_DRV_Init();
#if SDK_NV_RAM    
	SDK_NVKEY_Init();
#endif
	SDK_A2DP_VC_Init();
	SDK_HCI_Init();
	SDK_HFP_Init();
	SDK_Profile_Init();
	SDK_AVRCP_Init();
	SDK_A2DP_Init();
	SDK_AirAppCustomerInit();
	SDK_SDP_Init();

    #if SDK_MEMORY_ALLOCATION
    ucPtr=SDK_Mem_Alloc(500);
    if(!ucPtr){
        LightDbgPrint("SDK_Mem_Alloc() NG");
    }
    else{
        LightDbgPrint("SDK_Mem_Alloc() OK");
        SDK_Mem_Free(ucPtr);
    }
    #endif
	#if VENDOR_PROFILE
	SDK_VProfile_Init();
	#endif

    #ifdef SDK_SUPPORTED_PEDOMETER
    Pedometer_Init();
    #endif
}

void SDK_Polling(void)
{
	#if SDK_A2DP
	SDK_A2DP_Polling();
	#endif

	#ifdef SDK_SUPPORTED_PEDOMETER
	Pedometer();
	#endif
}
