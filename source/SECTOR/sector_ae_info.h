#ifndef _SECTOR_AE_INFO_H_
#define _SECTOR_AE_INFO_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_ae_info.h defines the LM parameters.
 *
 * Programmer : ChunYiChen@airoha.com.tw, Ext.3356
 */
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Version Information
typedef	struct 
{
    U8 majorVersion;
    U8 minorVersion;
    U16 buildNumber;
    U16 revisionNumber;
} VerInfo;

// Release Data
typedef	struct 
{
    U8 year;
    U8 month;
    U8 date;
} ReleaseDate;

typedef struct stru_ae_info
{
    VerInfo verInfo;
    ReleaseDate releaseDate;
    U8 companyName[20];
    U8 modelName[20];
    // *************************** IMPORTANT************************************
    // flash code version: 001.62971.3204.0
    // NOTICE - structure definition above matches specified flash code version.
    //          DO NOT modify it. ONLY APPENDing fields is allowed.
    // *************************** IMPORTANT************************************	
} AE_INFO_STRU;

typedef struct stru_sector_ae_info
{
    AE_INFO_STRU AeInfo;

    CRC16 CRC;

} SECTOR_AE_INFO_STRU;

EXTERN SECTOR_AE_INFO_STRU CODE gSector_AE_Info;

#endif /* _SECTOR_AE_INFO_H_ */

