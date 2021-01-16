/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_boundary_header.c defines the mp parameters.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
 // -
#include "config.h"
#include "cpu.h"
#include "sectors.h"
#include "sector_boundary.h"

#pragma userclass (HCONST = CFGHEAD)


EXTERN SECTOR_MP_PARAMETER_STRU CODE gSector_MpParameter;
#ifdef AB1520B
EXTERN SECTOR_PATCH_CODE_INIT_STRU CODE gSector_SysPatchCodeInit;
#endif
EXTERN SECTOR_MP_PARAMETER_F_STRU CODE gSector_MpParameter_F;

#ifdef SUPPORT_VOICE_COMMAND
EXTERN SECTOR_DRIVER_VOICE_COMMAND_APD_STRU CODE gDriverVoiceCommandAPDData;
#endif

#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	{(U8 CODE_PTR)SECTOR_INIT, SECTOR_SIZE, ATTR},



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_BOUNDARY_HEADER_STRU CODE gSectorBoundaryHeader =
{
    NUM_OF_SECTOR_BOUNDARY,

    {
	    #include "sector_boundary_member.h"
	},

	//- CRC
	{0,0}
};




