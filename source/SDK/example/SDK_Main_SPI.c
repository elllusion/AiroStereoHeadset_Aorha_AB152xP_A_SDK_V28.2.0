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
#include "..\SDK\include\Spi.h"
#include "..\SDK\include\Spiflash.h"

void SDK_Read_SPIFlashID()
{
    U8 spiflash_ID[3];
    
	//spiflash_ID[0] = Manufactory ID, spiflash_ID[1] = Memory type, spiflash_ID[2] = Memory density
	SPIFLH_ReadFlashID((U8 XDATA_PTR)&spiflash_ID[0]);	
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
	SDK_SPI_Init();
	
	//read SPIflash ID
	//SDK_Read_SPIFlashID();

	//init Pedometer
	#ifdef SDK_SUPPORTED_PEDOMETER
    Pedometer_Init();
    #endif
}


void SDK_Polling(void)
{
    #ifdef SDK_SUPPORTED_PEDOMETER
    Pedometer();
    #endif
}
