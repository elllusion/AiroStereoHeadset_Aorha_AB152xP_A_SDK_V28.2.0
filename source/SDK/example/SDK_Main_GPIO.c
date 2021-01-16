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


#include "..\SDK\include\Pio.h"
void SDK_GPIO_Sample_Code(void) {
	
	
	/* GPIO Sample Code */
    
    U8 GPIO_NUM=12;
    
    /* Get GPIO pin status */
    if(SDK_GPIO_GetInputHighLow(GPIO_NUM)==LEVEL_LOW) {
    	LightDbgPrint("SDK GPIO LOW");
    }
    else if(SDK_GPIO_GetInputHighLow(GPIO_NUM)==LEVEL_HIGH){
    	LightDbgPrint("SDK GPIO HIGH");
    }
    
    /* Set GPIO pin 12 to output and high level */
    SDK_GPIO_SetIO(PIN_SET_OUTPUT, LEVEL_HIGH, GPIO_NUM);
    
    /* Set GPIO pin 12 to input and pull up */
    SDK_GPIO_SetIO(PIN_SET_INPUT, LEVEL_HIGH, GPIO_NUM);
}

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

}
