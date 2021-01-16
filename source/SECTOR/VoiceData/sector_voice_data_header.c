/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_voice_data_header.c defines the voice data.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
 // -
#include "config.h"
#include "cpu.h"
#include "sectors.h"
#include "..\Driver\Driver_Interface.h"
#include "sector_voice_data.h"

#pragma userclass (HCONST = CFGHEAD)


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	{(U8 CODE_PTR)SECTOR_INIT, SECTOR_SIZE, ATTR},

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_VOICE_DATA_HEADER_STRU CODE gSectorVoiceDataHeader =
{
    NUM_OF_SECTOR_VOICE_DATA,

    {
	    #include "sector_voice_data_member.h"
	},

	//- CRC
	{0,0}
};




