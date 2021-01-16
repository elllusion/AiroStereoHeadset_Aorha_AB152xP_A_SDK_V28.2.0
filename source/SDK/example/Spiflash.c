#define _SDK_SPIFLASH_C_

#include "rc.h"

#include "..\SDK\include\Spiflash.h"
#include "..\SDK\include\Spi.h"
////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define SPIFLASH_CMD_BUFFER_SIZE      5

#define SPIFLASH_OK					0x00
#define SPIFLASH_ERROR				0x01



typedef struct stru_spiflh_cmd_buffer
{
	U8 contents[SPIFLASH_CMD_BUFFER_SIZE];
} SPIFLH_CMD_BUFFER_STRU;

typedef struct stru_spiflh_data_buffer
{
	U8 contents[SPIFLASH_CMD_BUFFER_SIZE];
} SPIFLH_DATA_BUFFER_STRU;

SPIFLH_CMD_BUFFER_STRU XDATA gSPIFLHCmd;
SPIFLH_DATA_BUFFER_STRU XDATA gSPIFLHData;
////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*  SPIFLH_ReadFlashID: read SPI flash ID. 
  *	<input 1>
  *	  U8  *content_ptr		:  three bytes  (Manufactory ID, Memory type, Memory density).
  *
  */
void SPIFLH_ReadFlashID(U8 XDATA_PTR content_ptr)
{

    gSPIFLHCmd.contents[0] = SPIFLASH_READ_ID;
	SPI_WriteReadBlockingCmd(SPI_ID_EXT_FLASH, 1, &gSPIFLHCmd.contents[0], 3, content_ptr);

}



/*  SPIFLH_ReadRegister: read SPI flash register.
  *	<input 1>
  *	  U8  cmd		              :  input SPI flash command value.
  *	<output>
  *	 U8					       :  result of SPI flash register value.
  */
U8 SPIFLH_ReadRegister (U8 cmd)
{
	gSPIFLHCmd.contents[0] = cmd;
	SPI_WriteReadBlockingCmd(SPI_ID_EXT_FLASH, 1, &gSPIFLHCmd.contents[0], 1, &gSPIFLHData.contents[0]);
	return gSPIFLHData.contents[0];
}



/*  SPIFLH_WriteSingleByteCmd:  Writer single byte command.
  *	<input 1>
  *	  U8  cmd		              :  input SPI flash command value.
  */
VOID SPIFLH_WriteSingleByteCmd (U8 cmd)
{

	gSPIFLHCmd.contents[0] = cmd;
	SPI_WriteReadBlockingCmd(SPI_ID_EXT_FLASH, 1, &gSPIFLHCmd.contents[0], 0, &gSPIFLHData.contents[0]);
}



/*  SPIFLH_WriteRegister: Write  SPI flash register.
  *	<input 1>
  *	  U8  cmd		              :  input SPI flash command value.
  *	<input 2>
  *	 U8	value			       :  input SPI flash register value.
  */
VOID SPIFLH_WriteRegister (U8 cmd, U8 value)
{
	U8 status;

	do {
		SPIFLH_WriteSingleByteCmd(SPIFLASH_WRITE_ENABLE);
		status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
	} while( !(status & SPIFLASH_STATUS_WEL_BIT) );

	gSPIFLHCmd.contents[0] = cmd;
	gSPIFLHCmd.contents[1] = value;
	
	SPI_WriteReadBlockingCmd(SPI_ID_EXT_FLASH, 2, &gSPIFLHCmd.contents[0], 0, &gSPIFLHData.contents[0]);

	do {
		status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
	} while(status & SPIFLASH_STATUS_WIP_BIT);
}


/*  SPIFLH_SectorErase: Erase SPI flash sector.
  *	<input 1>
  *	  U8  addr		                              :  input SPI flash address, Erase unit is 4K .
  * 	 value range			                       :  24 bits.
  *					       
  */
VOID SPIFLH_SectorErase (U32 addr)
{
	U8 status;

	do {
		SPIFLH_WriteSingleByteCmd(SPIFLASH_WRITE_ENABLE);
		status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
	} while( !(status & SPIFLASH_STATUS_WEL_BIT) );

	gSPIFLHCmd.contents[0] = SPIFLASH_SPI_SECTOR_ERASE;
	gSPIFLHCmd.contents[1] = (U8)((addr >>16) & 0xff);
	gSPIFLHCmd.contents[2] = (U8)((addr >>8) & 0xf0);
	gSPIFLHCmd.contents[3] = (U8)(addr & 0x00);

	SPI_WriteReadBlockingCmd(SPI_ID_EXT_FLASH, 4, &gSPIFLHCmd.contents[0], 0, &gSPIFLHData.contents[0]);

}


/*  SPIFLH_ChipErase : SPI flash Chip Erase.
  *
  *					       
  */
void SPIFLH_ChipErase (VOID)
{
	U8 status;

	do {
		SPIFLH_WriteSingleByteCmd(SPIFLASH_WRITE_ENABLE);
		status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
	} while( !(status & SPIFLASH_STATUS_WEL_BIT) );

	gSPIFLHCmd.contents[0] = SPIFLASH_SPI_CHIP_ERASE;


	SPI_WriteReadBlockingCmd(SPI_ID_EXT_FLASH, 1, &gSPIFLHCmd.contents[0], 0, &gSPIFLHData.contents[0]);

	do {
		status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
	} while(status & SPIFLASH_STATUS_WIP_BIT);

}



/*  SPIFLH_ChipUnlock : SPI flash protect none.
  *
  *					       
  */
VOID SPIFLH_ChipUnlock (VOID)
{
	U8 status;

	status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
	SPIFLH_WriteRegister(SPIFLASH_WRITE_STATUS_REG, (status & 0x03));
}


/*  SPIFLH_ChipLock : SPI flash protect all.
  *
  *					       
  */
VOID SPIFLH_ChipLock (VOID)
{
	U8 status;

	status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
	SPIFLH_WriteRegister(SPIFLASH_WRITE_STATUS_REG, (status | 0x3C));

}


/*  SPIFLH_PageWrite: write  SPI flash page data.
  *  <input 1>
  *	  U8  addr		                              :  input read about SPI flash address 
  * 	 value range			                       :  24 bits.
  *  <input 2>
  * 	 U8 XDATA_PTR content_ptr	        : writer SPI flash data store in pointer.
  * 	 value range						 : pointer.
  *  <output>
  * 	 U8					                      : result of read SPIflash
  * 	 value range			                      : SPIFLASH_OK--> OK,  SPIFLASH_ERROR --> Fail
  */
U8 SPIFLH_PageWrite (U32 flash_addr, U8 XDATA_PTR content_ptr)
{
    U8 status;

	U8 XDATA_PTR spiWritebuftr;

	spiWritebuftr = SYS_MemoryGet(SPIFLASH_PAGE_SIZE +4);

    if(spiWritebuftr == NULL) 
	    return  SPIFLASH_ERROR;	

    do {
		    SPIFLH_WriteSingleByteCmd(SPIFLASH_WRITE_ENABLE);
		    status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
    } while( !(status & SPIFLASH_STATUS_WEL_BIT) );


	spiWritebuftr[0] = SPIFLASH_PAGE_PROGRAM;  
	spiWritebuftr[1] = (U8)((flash_addr >>16) & 0xff);
	spiWritebuftr[2] = (U8)((flash_addr >>8) & 0xff);
	spiWritebuftr[3] = (U8)(flash_addr & 0x00);

	OS_memcpy(&spiWritebuftr[4], content_ptr, SPIFLASH_PAGE_SIZE);

	SPI_WriteReadBlockingCmd(SPI_ID_EXT_FLASH, SPIFLASH_PAGE_SIZE + 4, &spiWritebuftr[0], 0, content_ptr);

    do {
		    status = SPIFLH_ReadRegister(SPIFLASH_READ_STATUS_REG);
    } while(status & SPIFLASH_STATUS_WIP_BIT);

	SYS_MemoryRelease(&spiWritebuftr);

	return SPIFLASH_OK;
}


/*  SPIFLH_ReadBytes: read SPI flash data.
  *  <input 1>
  *	  U8  addr		                              :  input read about SPI flash address 
  * 	 value range			                       :  24 bits.
  *  <input 2>
  * 	 U8 XDATA_PTR content_ptr	        : read SPI flash data store destination pointer.
  * 	 value range						 : pointer.
  *  <input 3>
  * 	 U8 content_size	                              : read data length .
  * 	 value range						 : 1 to 256 bytes, one page is 256 bytes
  *  <output>
  * 	 U8					                      : result of read SPIflash
  * 	 value range			                      : SPIFLASH_OK--> OK,  SPIFLASH_ERROR --> Fail
  */
U8 SPIFLH_ReadBytes (U32 flash_addr, U8 XDATA_PTR content_ptr, U16 content_size)
{
    if ( (content_size > SPIFLASH_PAGE_SIZE) || (!content_size) )
	    return  SPIFLASH_ERROR;	
	    
    gSPIFLHCmd.contents[0] = SPIFLASH_READ_DATA;
	gSPIFLHCmd.contents[1] = (U8)((flash_addr >>16) & 0xff);
	gSPIFLHCmd.contents[2] = (U8)((flash_addr >>8) & 0xff);
	gSPIFLHCmd.contents[3] = (U8)(flash_addr & 0xff);

	SPI_WriteReadBlockingCmd(SPI_ID_EXT_FLASH, 4, &gSPIFLHCmd.contents[0],content_size, content_ptr);
	return SPIFLASH_OK; 

}


