/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_dsp_data_header.c defines DSP parameters.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
 // -
#include "config.h"
#include "cpu.h"
#include "sectors.h"
#include "sector_dsp_data.h"
#include "srom.h"

#pragma userclass (HCONST = CFGHEAD)

SROM_MC(HCONST_DSP_ROM)


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	{(U8 CODE_PTR)SECTOR_INIT, SECTOR_SIZE, ATTR},



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_DSP_DATA_HEADER_STRU CODE gSectorDspDataHeader =
{
    NUM_OF_SECTOR_DSP_DATA,

    {
	    #include "sector_dsp_data_member.h"
	},

	//- CRC
	{0,0}
};



