#include "reside_flash.inc"

#include "rc.h"
#include "I2C.h"
#include "..\SDK_Proxy\SDK_Proxy.h"

#define I2C_ENABLE	0


PUBLIC U8 SDK_IIC_ReadU8_400K(U8 device_addr, U8 reg_addr)
{
	return I2C_StdReadU8(device_addr, reg_addr);
}

PUBLIC U16 SDK_IIC_ReadU16_400K(U8 device_addr, U8 reg_addr)
{
    return I2C_StdReadU16(device_addr, reg_addr);
}

PUBLIC U32 SDK_IIC_ReadU32_400K(U8 device_addr, U8 reg_addr)
{
    return I2C_StdReadU32(device_addr, reg_addr);
}

PUBLIC U8 SDK_IIC_ReadBytes_400K(U8 device_addr, U8 reg_addr, U16 datLength, U8 *dataPtr)
{
    return I2C_StdReadChunk(device_addr, reg_addr, datLength, (U8 XDATA_PTR)dataPtr);
}

PUBLIC void SDK_IIC_WriteU8_400K(U8 device_addr, U8 reg_addr, U8 value)
{
    I2C_StdWriteU8(device_addr, reg_addr, value);
}

PUBLIC void SDK_IIC_WriteU16_400K(U8 device_addr, U8 reg_addr, U16 value)
{
    I2C_StdWriteU16(device_addr, reg_addr, value);
}

PUBLIC void SDK_IIC_WriteU32_400K(U8 device_addr, U8 reg_addr, U32 value)
{
    I2C_StdWriteU32(device_addr, reg_addr, value);
}

PUBLIC U8 SDK_IIC_WriteBytes_400K(U8 device_addr, U8 reg_addr, U16 datLength, U8 *dataPtr)
{
    return I2C_StdWriteChunk(device_addr, reg_addr, datLength, dataPtr);
}

#if I2C_ENABLE
PRIVATE void SDK_CB_IIC_Start(U8 device_addr)
{
	//if(device_addr == EEPROM_WRITE_ADDRESS)
	//{
		//Set write protection pin low
	//}
	UNUSED(device_addr);
}

PRIVATE void SDK_CB_IIC_Stop(U8 device_addr)
{
	//if(device_addr == EEPROM_WRITE_ADDRESS)
	//{
		//Set write protection pin high
	//}
	UNUSED(device_addr);
}
#endif //I2C_ENABLE

PUBLIC void SDK_IIC_Init(void)
{
#if I2C_ENABLE
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_IIC_START, API_GRP_IOSYS, (VFUN)SDK_CB_IIC_Start);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_IIC_STOP, API_GRP_IOSYS, (VFUN)SDK_CB_IIC_Stop);
#endif
}


///////////////////////////////////////////////////////////////////////////////
////////////////////////////  Sample code  ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
PUBLIC void SDK_IIC_ReadChunkSample(U8 device_addr, U8 reg_addr, U8 length)
{
	U8 XDATA_PTR rxBufPtr = SYS_MemoryGet(length);
	if(rxBufPtr)
	{
		SDK_IIC_ReadBytes_400K(device_addr, reg_addr, length, rxBufPtr);

		//rxBufPtr store all the read data. Do things here.

		OSMEM_Put(rxBufPtr);
	}
}

PUBLIC void SDK_IIC_WriteChunkSample(U8 device_addr, U8 reg_addr, U8 length)
{
	U8 XDATA_PTR txBufPtr = SYS_MemoryGet(length);
	if(txBufPtr)
	{
		txBufPtr[0] = 0xAA;
		txBufPtr[1] = 0xAA;
		txBufPtr[2] = 0xAA;
		
		SDK_IIC_WriteBytes_400K(device_addr, reg_addr, length, txBufPtr);

		//txBufPtr store all the read data. Do things here.

		OSMEM_Put(txBufPtr);
	}
}
 */



