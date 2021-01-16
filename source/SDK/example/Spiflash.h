#ifndef _SDK_SPIFLASH_H_
#define _SDK_SPIFLASH_H_


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define SPI_ID_EXT_FLASH			0x01


//===============command===================
#define SPIFLASH_WRITE_ENABLE 		0x06
#define SPIFLASH_WRITE_DISABLE 		0x04
#define SPIFLASH_READ_ID 			0x9F
#define SPIFLASH_READ_STATUS_REG	0x05
#define SPIFLASH_WRITE_STATUS_REG	0x01
#define SPIFLASH_READ_DATA 			0x03
#define SPIFLASH_PAGE_PROGRAM 		0x02
#define SPIFLASH_READ_SCU_REG		0x2B
#define SPIFLASH_WRITE_SCU_REG		0x2F
#define SPIFLASH_GANG_BLOCK_LOCK	0x7E
#define SPIFLASH_GANG_BLOCK_UNLOCK	0x98
#define SPIFLASH_SPI_SECTOR_ERASE 	0x20
#define SPIFLASH_SPI_CHIP_ERASE 	0x60

//========================================
#define SPIFLASH_STATUS_WIP_BIT	    0x01
#define SPIFLASH_STATUS_WEL_BIT	    0x02

//=================buffer size===============


#define SPIFLASH_PAGE_SIZE            256

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*  SPIFLH_ReadFlashID: read SPI flash ID. 
  *	<input 1>
  *	  U8  *content_ptr		:  three bytes  (Manufactory ID, Memory type, Memory density).
  *
  */
EXTERN void SPIFLH_ReadFlashID(U8 XDATA_PTR content_ptr);



/*  SPIFLH_SectorErase: Erase SPI flash sector.
  *	<input 1>
  *	  U8  addr		                              :  input SPI flash address, Erase unit is 4K .
  * 	 value range			                       :  24 bits.
  *					       
  */
EXTERN VOID SPIFLH_SectorErase (U32 addr);



/*  SPIFLH_ChipErase : SPI flash Chip Erase.
  *
  *					       
  */
EXTERN void SPIFLH_ChipErase (VOID);



/*  SPIFLH_ChipUnlock : SPI flash protect none.
  *
  *					       
  */
EXTERN VOID SPIFLH_ChipUnlock (VOID);



/*  SPIFLH_ChipLock : SPI flash protect all.
  *
  *					       
  */
EXTERN VOID SPIFLH_ChipLock (VOID);


/*  SPIFLH_PageWrite: write  SPI flash page data.
  *  <input 1>
  *	  U8  addr		                              :  input read about SPI flash address 
  * 	 value range			                       :  24 bits.
  *  <input 2>
  * 	 U8 XDATA_PTR content_ptr	        : writer SPI flash data buffer in pointer.
  * 	 value range						 : pointer.
  *  <output>
  * 	 U8					                      : result of read SPIflash
  * 	 value range			                      : SPIFLASH_OK--> OK,  SPIFLASH_ERROR --> Fail
  */
EXTERN U8 SPIFLH_PageWrite (U32 flash_addr, U8 XDATA_PTR content_ptr);




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
EXTERN U8 SPIFLH_ReadBytes (U32 flash_addr, U8 XDATA_PTR content_ptr, U16 content_size);

#endif /* _DRV_SPIFLH_H_ */

