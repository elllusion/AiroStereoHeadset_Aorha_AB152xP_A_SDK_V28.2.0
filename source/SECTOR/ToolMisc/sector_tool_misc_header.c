/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_config1_header.c defines the rest parameters.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
 // -
#include "config.h"
#include "cpu.h"
#include "sectors.h"
#include "sector_tool_misc.h"

#pragma userclass (HCONST = CFGHEAD)


EXTERN SECTOR_AE_INFO_STRU CODE gSector_AE_Info;
EXTERN SECTOR_TOOL_INFO_STRU CODE gSector_Tool_Info;


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	{(U8 CODE_PTR)SECTOR_INIT, SECTOR_SIZE, ATTR},



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_TOOL_MISC_HEADER_STRU CODE gSectorToolMiscHeader =
{
    NUM_OF_SECTOR_TOOL_MISC,

    {
	    #include "sector_tool_misc_member.h"
	},

	//- CRC
	{0,0}
};




