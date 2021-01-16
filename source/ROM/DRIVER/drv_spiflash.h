#ifndef _DRV_SPIFLH_H_
#define _DRV_SPIFLH_H_

#include "config.h"

#if (HW_SPI_SUPPORTED)
#include "cpu.h"
#include "drv_spi.h"

////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//===============command===================
#define SPIFLH_WRITE_ENABLE 		0x06
#define SPIFLH_WRITE_DISABLE 		0x04
#define SPIFLH_READ_ID 				0x9F
#define SPIFLH_READ_STATUS_REG		0x05
#define SPIFLH_WRITE_STATUS_REG		0x01
#define SPIFLH_READ_DATA 			0x03
#define SPIFLH_PAGE_PROGRAM 		0x02
#define SPIFLH_READ_SCU_REG			0x2B
#define SPIFLH_WRITE_SCU_REG		0x2F
#define SPIFLH_GANG_BLOCK_LOCK		0x7E
#define SPIFLH_GANG_BLOCK_UNLOCK	0x98
#define SPIFLH_SPI_SECTOR_ERASE 	0x20
#define SPIFLH_SPI_CHIP_ERASE 		0x60
#define SPIFLH_READ_STATUS2_REG		0x35
#define SPIFLH_READ_STATUS3_REG		0x15
#define SPIFLH_WRITE_STATUS2_REG	0x31
#define SPIFLH_WRITE_STATUS3_REG	0x11

// ===============status=====================
#define SPIFLH_ERR_NO_ERR					0x00
#define SPIFLH_ERR_PROG_ERR					0x01
#define SPIFLH_ERR_ERASE_ERR				0x02
#define SPIFLH_ERR_CANNOT_LOCATE_HEADER		0x03
#define SPIFLH_ERR_CRC_ERR					0x04
#define SPIFLH_ERR_NULL_DST_ADDR            0x05
//========================================
#define SPIFLH_STATUS_WIP_BIT	0x01
#define SPIFLH_STATUS_WEL_BIT	0x02
//========================================
#define SPIFLH_SCU_CP_MODE		0x10
#define SPIFLH_SCU_P_FAIL		0x20
#define SPIFLH_SCU_E_FAIL		0x40
#define SPIFLH_SCU_WPSEL		0x80

//=================buffer size===============
#define SPI_LARGE_BUFFER_SIZE 40
#define SPI_SMALL_BUFFER_SIZE 5

#define SPIFLH_PAGE_SIZE (SPI_LARGE_BUFFER_SIZE-8)

#define NEWSPIFLH_PAGE_SIZE    256
////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


typedef struct stru_spiflh_data_buffer_large
{
	U8 contents[SPI_LARGE_BUFFER_SIZE];
} SPIFLH_DATA_BUFFER_LARGE_STRU;

typedef struct stru_spiflh_data_buffer_small
{
	U8 contents[SPI_SMALL_BUFFER_SIZE];
} SPIFLH_DATA_BUFFER_SMALL_STRU;

typedef struct stru_spiflh_control
{
    SPIFLH_DATA_BUFFER_LARGE_STRU spiflhLargeBuf;
    SPIFLH_DATA_BUFFER_SMALL_STRU spiflhSmallBuf;
} SPIFLH_CONTROL_STRU;

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SPIFLH_CONTROL_STRU XDATA gSPIFLHControl;


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN VOID DRV_SPIFLH_Initial(U8 GrpSel);
EXTERN BOOL DRV_SPIFLH_IsExtFlhExist(VOID);
EXTERN U8   DRV_SPIFLH_ReadFlashID(VOID);
EXTERN U8   DRV_SPIFLH_ReadRegister (U8 cmd);
EXTERN U8   DRV_SPIFLH_ReadBytes (U32 addr_ptr, U8 XDATA_PTR content_ptr, U16 content_size);
EXTERN U8   DRV_SPIFLH_PageRead (U32 *addr_ptr, U16 write_count);

EXTERN VOID DRV_SPIFLH_WriteRegister (U8 cmd, U8 value);
EXTERN VOID DRV_SPIFLH_WriteSingleByteCmd (U8 cmd);
EXTERN U8   DRV_SPIFLH_WriteBytes (U32 addr_ptr, U8 XDATA_PTR content_ptr, U16 content_size);
EXTERN U8   DRV_SPIFLH_PageWrite (U32 *addr_ptr, U8 XDATA_PTR content_ptr, U16 write_count);

EXTERN U8   DRV_SPIFLH_ChipErase (VOID);
EXTERN U8   DRV_SPIFLH_SectorErase (U32 addr);
EXTERN U8   DRV_SPIFLH_32KErase (U32 addr);
EXTERN U8   DRV_SPIFLH_64KErase (U32 addr);
EXTERN VOID DRV_SPIFLH_ChipUnlock (VOID);
EXTERN VOID DRV_SPIFLH_ChipLock(VOID);

EXTERN U8 DRV_NEWSPIFLH_ReadFlashID(U8 XDATA_PTR content_ptr);
EXTERN U8 DRV_NewSPIFLH_PageWriteVerify(U8 XDATA_PTR content_ptr, U8 CRC8);
EXTERN U8 DRV_NEWSPIFLH_PageRead(U32 flash_addr, U8 XDATA_PTR content_ptr);

EXTERN VOID DRV_SPIFLH_WriteProtectionGD (U8 BpVal);
EXTERN VOID DRV_SPIFLH_WriteProtectionMX25R(U8 BpVal);
EXTERN VOID DRV_SPIFLH_WriteProtectionMX25L (U8 BpVal);
EXTERN VOID DRV_SPIFLH_WriteProtectionMX25U(U8 BpVal);
EXTERN VOID DRV_SPIFLH_WriteProtectionMD25D(U8 BpVal);
EXTERN VOID DRV_SPIFLH_WriteProtectionFM25Q (U8 BpVal);

#endif

#endif /* _DRV_SPIFLH_H_ */

