#ifndef _IIC_H_
#define _IIC_H_



/*  SDK_IIC_ReadU8_400K: IIC read 1 byte of data in 400k.
 *	<input 1>
 *		U8 device_addr	: device address.
 *  	value range		: device address is defined by applications.
 *	<input 2>
 *		U8 reg_addr		: register address
 *  	value range		: register address is defined by application and device.
 *  <output 1>
 *		U8				: read data
 */
PUBLIC U8 SDK_IIC_ReadU8_400K(U8 device_addr, U8 reg_addr);


/*  SDK_IIC_ReadU16_400K: IIC read 2 bytes of data in 400k.
 *	<input 1>
 *		U8 device_addr	: device address.
 *  	value range		: device address is defined by applications.
 *	<input 2>
 *		U8 reg_addr		: register address
 *  	value range		: register address is defined by application and device.
 *  <output 1>
 *		U16				: read data
 */
PUBLIC U16 SDK_IIC_ReadU16_400K(U8 device_addr, U8 reg_addr);



/*  SDK_IIC_WriteBytes_400K: IIC read 4 bytes of data in 400k.
 *	<input 1>
 *		U8 device_addr	: device address.
 *  	value range		: device address is defined by applications.
 *	<input 2>
 *		U8 reg_addr		: register address
 *  	value range		: register address is defined by application and device.
 *  <output 1>
 *		U32				: read data
 */
PUBLIC U32 SDK_IIC_ReadU32_400K(U8 device_addr, U8 reg_addr);



/*  SDK_IIC_WriteBytes_400K: IIC read variable length of data in 400k.
 *	<input 1>
 *		U8 device_addr	: device address.
 *  	value range		: device address is defined by applications.
 *	<input 2>
 *		U8 reg_addr		: register address
 *  	value range		: register address is defined by application and device.
 *	<input 3>
 *		U16 datLength	: data length 
 *  	value range		: data length in bytes. (device_addr and reg_addr are excluded)
 *	<input 4>
 *		U8 *dataPtr		: data address to be read.
 *  	value range		: start address of data memory.
 *  <output 1>
 *		U8				: 0xFF : fail in memory allocate
 *						  0x02 : data nak
 *						  0x04 : address nak
 *						  0x08 : contention fail
 *						  0x00 : successful
 */
PUBLIC U8 SDK_IIC_ReadBytes_400K(U8 device_addr, U8 reg_addr, U16 datLength, U8 *dataPtr);


/*  SDK_IIC_WriteU8_400K: IIC writes variable length of data in 400k.
 *	<input 1>
 *		U8 device_addr	: device address.
 *  	value range		: device address is defined by applications.
 *	<input 2>
 *		U8 reg_addr		: register address
 *  	value range		: register address is defined by application and device.
 *	<input 3>
 *		U8 value		: data value to be write 
 */
PUBLIC void SDK_IIC_WriteU8_400K(U8 device_addr, U8 reg_addr, U8 value);


/*  SDK_IIC_WriteU16_400K: IIC writes variable length of data in 400k.
 *	<input 1>
 *		U8 device_addr	: device address.
 *  	value range		: device address is defined by applications.
 *	<input 2>
 *		U8 reg_addr		: register address
 *  	value range		: register address is defined by application and device.
 *	<input 3>
 *		U16 value		: data value to be write 
 */
PUBLIC void SDK_IIC_WriteU16_400K(U8 device_addr, U8 reg_addr, U16 value);


/*  SDK_IIC_WriteU32_400K: IIC writes variable length of data in 400k.
 *	<input 1>
 *		U8 device_addr	: device address.
 *  	value range		: device address is defined by applications.
 *	<input 2>
 *		U8 reg_addr		: register address
 *  	value range		: register address is defined by application and device.
 *	<input 3>
 *		U32 value		: data value to be write 
 */
PUBLIC void SDK_IIC_WriteU32_400K(U8 device_addr, U8 reg_addr, U32 value);


/*  SDK_IIC_WriteBytes_400K: IIC writes variable length of data in 400k.
 *	<input 1>
 *		U8 device_addr	: device address.
 *  	value range		: device address is defined by applications.
 *	<input 2>
 *		U8 reg_addr		: register address
 *  	value range		: register address is defined by application and device.
 *	<input 3>
 *		U16 datLength	: data length 
 *  	value range		: data length in bytes. (device_addr and reg_addr are excluded)
 *	<input 4>
 *		U8 *dataPtr		: data to be sent.
 *  	value range		: start address of data memory.
 *  <output 1>
 *		U8				: 0xFF : fail in memory allocate
 *						  0x02 : data nak
 *						  0x04 : address nak
 *						  0x08 : contention fail
 *						  0x00 : successful
 */
U8 SDK_IIC_WriteBytes_400K(U8 device_addr, U8 reg_addr, U16 datLength, U8 *dataPtr);



/*  SDK_IIC_Init: Initial I2C notify functions.
 *		usage			: two notify functions are provided here, I2c_start and I2c_stop
 *		I2c_start		: is called before sending I2c start signal.
 *  	I2c_stop		: is called after sending I2c stop signal.
 *		para			: U8 device_addr is the current device address.
 */
PUBLIC void SDK_IIC_Init(void);

#endif //_IIC_H_
