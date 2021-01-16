#ifndef _SECTOR_TOOL_INFO_H_
#define _SECTOR_TOOL_INFO_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2015 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_tool_info.h defines fields used by tool
 *
 * Programmer : hsianglin@airoha.com.tw, Ext.2861
 */
#include "cpu.h"
#include "crc16.h"
#include "sector_ae_info.h"

////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Version Information // 用 sector_ae_info.h 中的定義即可
/*
typedef	struct 
{
    U8 MajorVersion;
    U8 MinorVersion;
    U8 BuildNumber;
    U8 Revision;
} VerInfo;

typedef	struct 
{
    U8 year;
    U8 month;
    U8 date;
} ReleaseDate;
*/

typedef struct stru_tool_info
{
    VerInfo LastAccessedToolVersion;
    ReleaseDate LastAccessedToolDate;
    U8 Reserved[30];
	U8 dspparatooluse[2048];
    // *************************** IMPORTANT ************************************
    // flash code version: 001.62971.3204.0
    // NOTICE - structure definition above matches specified flash code version.
    //          DO NOT modify it. ONLY APPENDing fields is allowed.
    // *************************** IMPORTANT ************************************
    U8 ReservedForMP[256];
    U8 Reserved1[800];
} TOOL_INFO_STRU;

typedef struct stru_sector_tool_info
{
    TOOL_INFO_STRU ToolInfo;

    CRC16 CRC;
} SECTOR_TOOL_INFO_STRU;


#endif /* _SECTOR_TOOL_INFO_H_ */

