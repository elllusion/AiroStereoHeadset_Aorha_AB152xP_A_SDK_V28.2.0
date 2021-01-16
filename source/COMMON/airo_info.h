#ifndef _AIRO_INFO_H_
#define _AIRO_INFO_H_

#include "svn_info_MCU.h"
#include "svn_info_DSP.h"
#include "hci_airoha.h"

////////////////////////////////////////////////////////////////////////////////
// Type Definition /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
 * 0x02 Flash Code Version Descriptor
 */
#define SYS_FLASH_CODE_VERSION_STRING "028."SYS_ASIC_SVN_MCU_REVISION_STRING"."SYS_ASIC_SVN_DSP_REVISION_STRING".0"

typedef struct stru_airo_info_para_flash_code_ver
{
    U8 FlashCodeVer[sizeof(SYS_FLASH_CODE_VERSION_STRING)];

} AIRO_INFO_PARA_FLASH_CODE_VER_STRU;

typedef struct stru_airo_info_flash_code_ver
{
    U8 Type;
    U8 InfoLen;
    AIRO_INFO_PARA_FLASH_CODE_VER_STRU Info;

} AIRO_INFO_FLASH_CODE_VER_STRU;


/*
 * 0x03 Flash Code CRC64 Descriptor
 */
typedef U8 CRC64[8];
typedef struct stru_airo_info_flash_code_crc64
{
    U8 Type;
    U8 InfoLen;
    CRC64 FlashCodeCrc64;

} AIRO_INFO_FLASH_CODE_CRC64_STRU;

typedef struct stru_airo_info_content
{
    AIRO_INFO_FLASH_CODE_VER_STRU    FlashCodeVer;
	AIRO_INFO_FLASH_CODE_CRC64_STRU  FlashcodeCrc64;
	AIRO_INFO_ROM_VER_STRU           RomVersion;

} AIRO_INFO_CONTENT_STRU;


/* Airo-Info Descriptor Prototype */
typedef struct stru_airo_info
{
    U16 TotalLen;
    AIRO_INFO_CONTENT_STRU  AiroInfoContent;
	CRC16 crc16;

} AIRO_INFO_STRU;

#endif

