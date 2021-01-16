#ifndef _SDK_SPI_H_
#define _SDK_SPI_H_

#define SPI_BLOCKING_CMD_STATUS_OK				0
#define SPI_BLOCKING_CMD_STATUS_NO_HW_SETTINGS	1
#define SPI_BLOCKING_CMD_STATUS_NO_MEMORY		2

/*  SPI_WriteReadBlockingCmd: SPI tx and rx blocking command.
 *	<input 1>
 *		U8 devID				: device id in sector_spi_config.h. each device has its own id 
 *  	value range				: SPI_DEVICE_ID_XX
 *	<input 2>
 *		U16 txLen				: data length to be transmitted 
 *  	value range				: 0 - 800
 *	<input 3>
 *		U8 XDATA_PTR txBufPtr	: transmitted data address 
 *  	value range				: data address in ram
 *	<input 4>
 *		U16 rxLen				: data length to be received 
 *  	value range				: 0 - 800
 *	<input 5>
 *		U8 XDATA_PTR rxBufPtr	: received data address 
 *  	value range				: data address in ram
 *	<output>
 *		U8						: status of command
 *      value range				: SPI_BLOCKING_CMD_STATUS_OK: command is complete
 *								  SPI_BLOCKING_CMD_STATUS_NO_HW_SETTINGS: no hardware settings in sector_spi_config
 */	
U8 SPI_WriteReadBlockingCmd(U8 devID, U16 txLen, U8 XDATA_PTR txBufPtr, U16 rxLen, U8 XDATA_PTR rxBufPtr);



/*  SPI_WriteReadBlockingCmd: SPI tx and rx non-blocking command.
 *	<input 1>
 *		U8 devID				: device id in sector_spi_config.h. each device has its own id 
 *  	value range				: SPI_DEVICE_ID_XX
 *	<input 2>
 *		U16 txLen				: data length to be transmitted 
 *  	value range				: 0 - 800
 *	<input 3>
 *		U8 XDATA_PTR txBufPtr	: transmitted data address 
 *  	value range				: data address in ram
 *	<input 4>
 *		U16 rxLen				: data length to be received 
 *  	value range				: 0 - 800
 *	<input 5>
 *		U8 XDATA_PTR rxBufPtr	: received data address 
 *  	value range				: data address in ram
 *	<input 6>
 *		VFUN callback			: callback function would be called when command is complete
 *  	value range				: NULL or real function
 *	<output in callback>
 *		void (*callbackFunc)(U8 status, U16 rxLen, U8 XDATA_PTR rxBufPtr);
 *	<output1>
 *		U8 status				: status of command
 *      value range				: SPI_BLOCKING_CMD_STATUS_OK: command is complete
 *								  SPI_BLOCKING_CMD_STATUS_NO_HW_SETTINGS: no hardware settings in sector_spi_config
 *	<output2>
 *		U16 rxLen				: data length to be received 
 *      value range				: 0 - 800
 *	<output3>
 *		U8 XDATA_PTR rxBufPtr	: received data address  
 *      value range				: data address in ram
 */	
void SPI_WriteReadNonBlockingCmd(U8 devID, U16 txLen, U8 XDATA_PTR txBufPtr, U16 rxLen, U8 XDATA_PTR rxBufPtr, VFUN callback);

#endif