#define _DFU_LOADER_H_

VOID DFU_LaunchLoader(VOID);

typedef U8 DFU_ERRCODE;
#define DFU_ERR_0x00_0x0F_ARE_RSVD  "reference to the following error code defined by the Ex/internal flash driver"
#define DFU_ERR_UNKNOWN_FLASH_SIZE  0x10
#define DFU_ERR_INVALID_DATA_LEN    0x11

/*          list as reference at 2015/01/25
#define SPIFLH_ERR_NO_ERR					0x00
#define SPIFLH_ERR_PROG_ERR					0x01
#define SPIFLH_ERR_ERASE_ERR				0x02
#define SPIFLH_ERR_CANNOT_LOCATE_HEADER		0x03
#define SPIFLH_ERR_CRC_ERR					0x04
#define SPIFLH_ERR_NULL_DST_ADDR            0x05

#define FLASH_SUCCESS                   (FLASH_ERRCODE)0
#define FLASH_WRITE_SOURCE_CRC_FAIL     (FLASH_ERRCODE)2
#define FLASH_VERIFY_CRC_FAIL           (FLASH_ERRCODE)3
#define FLASH_VERIFY_DATA_FAIL          (FLASH_ERRCODE)4
#define FLASH_UNSUPPORTED_FUNCTION      (FLASH_ERRCODE)5
#define FLASH_ADDR_NOT_AT_BOUNDARY      (FLASH_ERRCODE)6
#define FLASH_DATA_ADDR_NOT_4BYTE_ALIGN (FLASH_ERRCODE)7
#define FLASH_INVALID_SIZE              (FLASH_ERRCODE)8
*/


#define DFU_FLASH_PROTECT_BIT_NONE  0
#define DFU_FLASH_PROTECT_BIT_ALL   15
#define DFU_FLASH_PAGE_SIZE         256

#define DFU_HDR_CRC_INIT_VALUE      0xFFFF

#define DFU_DATA_SIZE_4KB           0x00001000
#define DFU_DATA_SIZE_32KB          0x00008000
#define DFU_DATA_SIZE_64KB          0x00010000

#define DFU_EXT_FLASH_UNKNOWN           0x00
#define DFU_EXT_FLASH_VALID             0x01
#define DFU_EXT_FLASH_NOT_EXIST         0x02

#define DFU_FIRST_BYTE_OF_SPI_FLASH_ADDR    0x10

/* DFU_HDR_ADDR would be placed at fifth-last sector */
#define DFU_HDR_PTR                 (pDfuHeader)
#define DFU_TRIPLET_ADDR            0xEF00  /* 0xEF00~0xEFFF */
#define DFU_SPILASH_ID_ADDR         0xEE00  /* 0xEE00~0xEE0F */
#define DFU_SPI_GPIO_INFO_ADDR      0xEE10  /* 0xEE00~0xEE1F */
#define DFU_LOADER_HDR              0xEE20  /* 0xEE20~0xEE5F */
#define DFU_REUSED_ADDR             0xEE60  /* 0xEE60~0xEEFF */

#ifdef AB1520B
#define pBuffer  ((OS_DATA_PTR)0xE000)
#endif

typedef union
{
    U32 value;
    U32 CODE_PTR addr;
    U8 Byte[4];
}DFU_ADDR;

typedef struct
{
    U32 DataLen;
    DFU_ADDR SrcAddr;
    DFU_ADDR DstAddr;
}DFU_DATA_TRIPLET;

typedef struct
{
    U16 HeaderLen;      //excluding CRC size
    U16 NumOfTriplet;
    DFU_DATA_TRIPLET pDataTriplet[3];
}DFU_LOADER_HDR_STRU;

typedef struct
{
    U8 E3_Offset;
    U8 E3_Value;

} SPI_INFO_STRU, XDATA_PTR SPI_INFO_STRU_PTR;


/*
 *  private function prototypes
 */
VOID DFU_ErrHandler(U8 ErrCode);
BOOL DFU_IsSpiFlashSrcAddr(DFU_DATA_TRIPLET CODE_PTR pTriplet);
VOID DFU_SPIFLASH_ChipUnlock(VOID);
VOID DFU_SPIFLASH_ChipLock(VOID);
SPI_INFO_STRU_PTR DFU_IsSpiExtFlashValid(VOID);