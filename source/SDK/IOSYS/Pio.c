#include "reside_flash.inc"

#include "rc.h"
#include "drv_gpio_flash.h"


void SDK_GPIO_SetIO(U8 isOutputPin, U8 isHigh, U8 ioID)
{
	DRV_GPIO_F_SetIOStatus(isOutputPin, isHigh, ioID);
}

U8 SDK_GPIO_GetInputHighLow(U8 ioID)
{
	return DRV_GPIO_F_GetIOStatus(ioID);
}
