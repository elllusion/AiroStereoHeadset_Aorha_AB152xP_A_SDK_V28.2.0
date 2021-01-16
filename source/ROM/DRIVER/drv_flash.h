#ifndef _FLASH_DRV_H_
#define _FLASH_DRV_H_
//////////////////////////////////////////////////////////
// Flash Function, Structure and Define
//
// Added by Dinga, 2014.02.06
// For identify the manufacturer of the serial flash
//////////////////////////////////////////////////////////
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// CONSTANT DEFINITIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* Flash control command */
typedef U8 FLASH_CMD_CODE;
#define FLASH_BYTE_READ                 (FLASH_CMD_CODE)0
#define FLASH_READ_STATUS_REG           (FLASH_CMD_CODE)3
#define FLASH_SECTOR_ERASE              (FLASH_CMD_CODE)4
#define FLASH_BLOCK_ERASE_32KB          (FLASH_CMD_CODE)5
#define FLASH_BLOCK_ERASE_64KB          (FLASH_CMD_CODE)6
#define FLASH_CHIP_ERASE                (FLASH_CMD_CODE)7
#define FLASH_READ_VERIFY               (FLASH_CMD_CODE)8
#define FLASH_BYTE_PROGRAM              (FLASH_CMD_CODE)12
#define FLASH_PAGE_PROGRAM              (FLASH_CMD_CODE)13
#define FLASH_SET_PROTECT_BIT           (FLASH_CMD_CODE)14
#define FLASH_WRITE_MANUAL_INSTR        (FLASH_CMD_CODE)15
#define FLASH_MANUAL_INSTR              (FLASH_CMD_CODE)19
#define FLASH_PAGE_READ                 (FLASH_CMD_CODE)24

#define WB_SECURITY_REG                 0x20


/* Manufacturer ID */
#define FLASH_MANUFACTURER_MXIC         (U8)0xC2
#define FLASH_MANUFACTURER_WB           (U8)0xEF
#define FLASH_MANUFACTURER_GD           (U8)0xC8
#define FLASH_MANUFACTURER_MD			(U8)0x51
#define FLASH_MANUFACTURER_FM           (U8)0xA1
#define FLASH_MANUFACTURER_EN           (U8)0x1C
/* Memory Type */
#define FLASH_MEMORYTYPE_MX25U          (U8)0x25
#define FLASH_MEMORYTYPE_MX25R          (U8)0x28
#define FLASH_MEMORYTYPE_MX25L          (U8)0x20
#define FLASH_MEMORYTYPE_W25Q           (U8)0x50
#define FLASH_MEMORYTYPE_GD25LQ         (U8)0x60
#define FLASH_MEMORYTYPE_GD25Q          (U8)0x40
#define FLASH_MEMORYTYPE_MD25D          (U8)0x40
#define FLASH_MEMORYTYPE_FM25Q          (U8)0x40
#define FLASH_MEMORYTYPE_EN25S          (U8)0x38

/* Memory Identify */
/* MX25U Series */
#define FLASH_MEMORYTYPE2_MX25U4033E    (U8)0x33
#define FLASH_MEMORYTYPE2_MX25U8033E    (U8)0x34
#define FLASH_MEMORYTYPE2_MX25U1635E    (U8)0x35
/* MX25R Series */
#define FLASH_MEMORYTYPE2_MX25R4035F    (U8)0x13
#define FLASH_MEMORYTYPE2_MX25R8035F    (U8)0x14
#define FLASH_MEMORYTYPE2_MX25R1635F    (U8)0x15
#define FLASH_MEMORYTYPE2_MX25R3235F    (U8)0x16
/* MX25L Series */
#define FLASH_MEMORYTYPE2_MX25L8035E    (U8)0x14
/* M25Q Series */
#define FLASH_MEMORYTYPE2_W25Q40BW      (U8)0x13
#define FLASH_MEMORYTYPE2_W25Q80BW      (U8)0x14
/* GD25LQ Series */
#define FLASH_MEMORYTYPE2_GD25LQ40      (U8)0x13
#define FLASH_MEMORYTYPE2_GD25LQ80      (U8)0x14
#define FLASH_MEMORYTYPE2_GD25LQ16      (U8)0x15
/* GD25Q Series */
#define FLASH_MEMORYTYPE2_GD25Q80C      (U8)0x14
#define FLASH_MEMORYTYPE2_GD25Q32C      (U8)0x16
#define FLASH_MEMORYTYPE2_GD25Q64C      (U8)0x17
#define FLASH_MEMORYTYPE2_GD25Q128C     (U8)0x18
/* MD25D Series */
#define FLASH_MEMORYTYPE2_MD25D40		(U8)0x13
#define FLASH_MEMORYTYPE2_MD25D80       (U8)0x14
/* FM25Q Series */
#define FLASH_MEMORYTYPE2_FM25Q08A      (U8)0x14
/* EN25S Series */
#define FLASH_MEMORYTYPE2_EN25S80       (U8)0x14


#define FLASH_COMBINED_INFO(ManuID,MemType,MemDensity) \
    (((U32)(ManuID)<<16)|((U32)(MemType)<<8)|((U32)(MemDensity)))

/* MX25U Series */
#define FLASH_MXIC_MX25U4033E           \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MXIC,        \
        FLASH_MEMORYTYPE_MX25U,         \
        FLASH_MEMORYTYPE2_MX25U4033E)
#define FLASH_MXIC_MX25U8033E           \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MXIC,        \
        FLASH_MEMORYTYPE_MX25U,         \
        FLASH_MEMORYTYPE2_MX25U8033E)
#define FLASH_MXIC_MX25U1635E           \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MXIC,        \
        FLASH_MEMORYTYPE_MX25U,         \
        FLASH_MEMORYTYPE2_MX25U1635E)

/* MX25R Series */
#define FLASH_MXIC_MX25R4035F           \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MXIC,        \
        FLASH_MEMORYTYPE_MX25R,         \
        FLASH_MEMORYTYPE2_MX25R4035F)
#define FLASH_MXIC_MX25R8035F           \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MXIC,        \
        FLASH_MEMORYTYPE_MX25R,         \
        FLASH_MEMORYTYPE2_MX25R8035F)
#define FLASH_MXIC_MX25R1635F           \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MXIC,        \
        FLASH_MEMORYTYPE_MX25R,         \
        FLASH_MEMORYTYPE2_MX25R1635F)
#define FLASH_MXIC_MX25R3235F           \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MXIC,        \
        FLASH_MEMORYTYPE_MX25R,         \
        FLASH_MEMORYTYPE2_MX25R3235F)

/* MX25L Series */
#define FLASH_MXIC_MX25L8035E           \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MXIC,        \
        FLASH_MEMORYTYPE_MX25L,         \
        FLASH_MEMORYTYPE2_MX25L8035E)

/* W25Q Series */
#define FLASH_WB_W25Q40BW               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_WB,          \
        FLASH_MEMORYTYPE_W25Q,          \
        FLASH_MEMORYTYPE2_W25Q40BW)
#define FLASH_WB_W25Q80BW               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_WB,          \
        FLASH_MEMORYTYPE_W25Q,          \
        FLASH_MEMORYTYPE2_W25Q80BW)

/* GD25LQ Series */
#define FLASH_GD_GD25LQ40               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_GD,          \
        FLASH_MEMORYTYPE_GD25LQ,        \
        FLASH_MEMORYTYPE2_GD25LQ40)
#define FLASH_GD_GD25LQ80               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_GD,          \
        FLASH_MEMORYTYPE_GD25LQ,        \
        FLASH_MEMORYTYPE2_GD25LQ80)
#define FLASH_GD_GD25LQ16               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_GD,          \
        FLASH_MEMORYTYPE_GD25LQ,        \
        FLASH_MEMORYTYPE2_GD25LQ16)

/* GD25Q Series */
#define FLASH_GD_GD25Q80C               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_GD,          \
        FLASH_MEMORYTYPE_GD25Q,         \
        FLASH_MEMORYTYPE2_GD25Q80C)
#define FLASH_GD_GD25Q32C               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_GD,          \
        FLASH_MEMORYTYPE_GD25Q,         \
        FLASH_MEMORYTYPE2_GD25Q32C)
#define FLASH_GD_GD25Q64C               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_GD,          \
        FLASH_MEMORYTYPE_GD25Q,         \
        FLASH_MEMORYTYPE2_GD25Q64C)
#define FLASH_GD_GD25Q128C              \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_GD,          \
        FLASH_MEMORYTYPE_GD25Q,         \
        FLASH_MEMORYTYPE2_GD25Q128C)

/* MD25D Series */
#define FLASH_MD_MD25D40                \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MD,          \
        FLASH_MEMORYTYPE_MD25D,         \
        FLASH_MEMORYTYPE2_MD25D40)
#define FLASH_MD_MD25D80                \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_MD,          \
        FLASH_MEMORYTYPE_MD25D,         \
        FLASH_MEMORYTYPE2_MD25D80)

/* FM25Q Series */
#define FLASH_FM_FM25Q08A               \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_FM,          \
        FLASH_MEMORYTYPE_FM25Q,         \
        FLASH_MEMORYTYPE2_FM25Q08A)
		
/* EN25S Series */
#define FLASH_EN_EN25S80                \
    FLASH_COMBINED_INFO(                \
        FLASH_MANUFACTURER_EN,          \
        FLASH_MEMORYTYPE_EN25S,         \
        FLASH_MEMORYTYPE2_EN25S80)		


typedef U8 FLASH_ERRCODE;
#define FLASH_SUCCESS                   (FLASH_ERRCODE)0
#define FLASH_WRITE_SOURCE_CRC_FAIL     (FLASH_ERRCODE)2
#define FLASH_VERIFY_CRC_FAIL           (FLASH_ERRCODE)3
#define FLASH_VERIFY_DATA_FAIL          (FLASH_ERRCODE)4
#define FLASH_UNSUPPORTED_FUNCTION      (FLASH_ERRCODE)5
#define FLASH_ADDR_NOT_AT_BOUNDARY      (FLASH_ERRCODE)6
#define FLASH_DATA_ADDR_NOT_4BYTE_ALIGN (FLASH_ERRCODE)7
#define FLASH_INVALID_LENGTH            (FLASH_ERRCODE)8
#define FLASH_UNSUPPORTED_FLASH_IC      (FLASH_ERRCODE)9
#define FLASH_UNSUPPORTED_FLASH_SIZE    (FLASH_ERRCODE)10

typedef U8 FLASH_STATUS;
#define FLASH_UNKNOWN_MAMUFACTURER_ID   (FLASH_STATUS)(1 << 0)
#define FLASH_UNKNOWN_MEMORYTYPE        (FLASH_STATUS)(1 << 1)



/* Flash Size */
#define FLASH_SIZE_4M_BIT       (U8)4
#define FLASH_SIZE_8M_BIT       (U8)8
#define FLASH_SIZE_16M_BIT      (U8)16
#define FLASH_SIZE_32M_BIT      (U8)32
#define FLASH_SIZE_64M_BIT      (U8)64
#define FLASH_SIZE_128M_BIT     (U8)128


////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U32 FLASH_ADDR;

#ifdef AB1520B
typedef struct stru_flash_functions
{
    VOID (*CfgRegister)                 (VOID);
    VOID (*Protection)                  (U8);
    VOID (*ProtectExceptLastBLK)        (VOID);//do not use
    VOID (*ChipErase)                   (VOID);
    FLASH_ERRCODE (*SectorErase)        (FLASH_ADDR);
    FLASH_ERRCODE (*Block_32KB_Erase)   (FLASH_ADDR);
    FLASH_ERRCODE (*Block_64KB_Erase)   (FLASH_ADDR);
    FLASH_ERRCODE (*PageProgram)        (FLASH_ADDR, U8 XDATA_PTR, U8 XDATA_PTR);
    FLASH_ERRCODE (*PageRead)           (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*ByteProgram)        (FLASH_ADDR, U8 XDATA_PTR, U16);
    VOID (*ByteRead)                    (FLASH_ADDR, U8 XDATA_PTR, U16);
    VOID (*ReadOtpData)                 (FLASH_ADDR, U8 XDATA_PTR, U16);
    VOID (*WrGangBlkLock)               (VOID);
    VOID (*WrGangBlkUnlock)             (VOID);
    VOID (*WrSingleBlkSectorLock)       (FLASH_ADDR);
    VOID (*WrSingleBlkSectorUnlock)     (FLASH_ADDR);
    FLASH_ERRCODE (*RdSecurityRegister) (VOID);
    VOID (*WrProtectSelection)          (VOID);

} FLASH_DRV_STRU;
#elif (defined AB1520C) || (defined AB1520D)
typedef struct stru_flash_functions
{
    FLASH_ERRCODE (*CfgRegister)                 (VOID);
    FLASH_ERRCODE (*Protection)                  (U8);
    FLASH_ERRCODE (*ProtectExceptLastBLK)        (VOID);//do not use
    VOID (*ChipErase)                   (VOID);
    FLASH_ERRCODE (*SectorErase)        (FLASH_ADDR);
    FLASH_ERRCODE (*Block_32KB_Erase)   (FLASH_ADDR);
    FLASH_ERRCODE (*Block_64KB_Erase)   (FLASH_ADDR);
    FLASH_ERRCODE (*PageProgram)        (FLASH_ADDR, U8 XDATA_PTR, U8 XDATA_PTR);
    FLASH_ERRCODE (*PageRead)           (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*ByteProgram)        (FLASH_ADDR, U8 XDATA_PTR, U16);
    VOID (*ByteRead)                    (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*ReadOtpData)                 (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*WrGangBlkLock)               (VOID);
    FLASH_ERRCODE (*WrGangBlkUnlock)             (VOID);
    FLASH_ERRCODE (*WrSingleBlkSectorLock)       (FLASH_ADDR);
    FLASH_ERRCODE (*WrSingleBlkSectorUnlock)     (FLASH_ADDR);
    FLASH_ERRCODE (*RdSecurityRegister)          (U8 XDATA_PTR);
    FLASH_ERRCODE (*WrProtectSelection)          (VOID);
    FLASH_ERRCODE (*WriteOtpData)       (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*SetSecurityLock)    (U8);

} FLASH_DRV_STRU;
#else
typedef struct stru_flash_functions
{
    FLASH_ERRCODE (*CfgRegister)                 (VOID);
    FLASH_ERRCODE (*Protection)                  (U8);
    VOID (*ChipErase)                   (VOID);
    FLASH_ERRCODE (*SectorErase)        (FLASH_ADDR);
    FLASH_ERRCODE (*Block_32KB_Erase)   (FLASH_ADDR);
    FLASH_ERRCODE (*Block_64KB_Erase)   (FLASH_ADDR);
    FLASH_ERRCODE (*PageProgram)        (FLASH_ADDR, U8 XDATA_PTR, U8 XDATA_PTR);
    FLASH_ERRCODE (*PageRead)           (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*ByteProgram)        (FLASH_ADDR, U8 XDATA_PTR, U16);
    VOID (*ByteRead)                    (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*ReadOtpData)                 (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*WrGangBlkLock)               (VOID);
    FLASH_ERRCODE (*WrGangBlkUnlock)             (VOID);
    FLASH_ERRCODE (*WrSingleBlkSectorLock)       (FLASH_ADDR);
    FLASH_ERRCODE (*WrSingleBlkSectorUnlock)     (FLASH_ADDR);
    FLASH_ERRCODE (*RdSecurityRegister)          (U8 XDATA_PTR);
    FLASH_ERRCODE (*WrProtectSelection)          (VOID);
    FLASH_ERRCODE (*WriteOtpData)       (FLASH_ADDR, U8 XDATA_PTR, U16);
    FLASH_ERRCODE (*SetSecurityLock)    (U8);
    FLASH_ERRCODE (*SeletProgramArea)            (FLASH_ADDR);
    FLASH_ERRCODE (*fFlashfun)                   (VOID);

} FLASH_DRV_STRU;
#endif


typedef union
{
    U8 value;
    struct
    {
        U8 Bit0 : 1;
        U8 LDSO : 1;
        U8 Bit2 : 1;
        U8 Bit3 : 1;
        U8 Bit4 : 1;
        U8 Bit5 : 1;
        U8 Bit6 : 1;
        U8 WPSEL : 1;
    } in_bit;

} MXIC_SECU_REG_UNION;

#ifdef AB1520B
typedef struct stru_flash_info
{
    U8 ManuId;
    U8 MemType2;
    U8 FlashSize;   //unit: Mbit
    FLASH_STATUS Status;

} FLASH_INFO_STRU;
#else
typedef struct stru_flash_info
{
    U8 ManuId;
    U8 MemType;
    U8 MemType2;
    U8 FlashSize;   //unit: Mbit
    FLASH_STATUS Status;

} FLASH_INFO_STRU;
#endif



////////////////////////////////////////////////////////////////////////////////
// EXTERNAL GLOBAL VARIABLES ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN FLASH_DRV_STRU CODE_PTR XDATA fFlashDrv;
EXTERN FLASH_INFO_STRU XDATA gFlashInfo;
EXTERN FLASH_DRV_STRU CODE MXIC_FLASH_FUNC;
EXTERN FLASH_DRV_STRU CODE WB_FLASH_FUNC;
EXTERN FLASH_DRV_STRU CODE DEFAULT_FLASH_FUNC;


////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * Flash_Identify
 *
 * Identify the Flash
 *
 * @Author : CharlesSu <CharlesSu@airoha.com.tw>
 */
EXTERN VOID Flash_Identify(VOID);
EXTERN U8 Flash_GetFlashSize(U8 ManuId, U8 MemType, U8 MemDensity);

#endif /* _FLASH_DRV_H_ */

