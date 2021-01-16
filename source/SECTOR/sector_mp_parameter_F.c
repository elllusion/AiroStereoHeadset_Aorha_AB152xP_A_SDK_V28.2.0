/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_mp_parameter_F.h defines the calibration parameters that added in flash code.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"
#include "sector_mp_parameter_F.h"

#pragma userclass (HCONST = MP_PARAM_F)



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_MP_PARAMETER_F_STRU CODE gSector_MpParameter_F =
{
    {
        0,
    	{
        	{0xFF, 0, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        	},
        	{0xFF, 0, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        	},
    	},
    	{
    		0, 0, 0, 0, 0, 0, //two step charging
    	},
    	{
    		0, //enableNTCRatio
		10, // aioAverageCount
    		{
			0.63, 0.571, 0.476, 0.425, 0.361, 0.277
    		},
		{
    			2,
    			15,
    			14
		},
    	},
    	50, //discountNumber
    	{
    		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
    	},
    	0,
    	{
        	0, //reserved
    	},
    	{
        	0, //Customer_Reserved1[15][32]
    	},

    	{
        	0, //Customer_Reserved2[30]
    	},

    {
        {0x66, 0x66, 0x66, 0x33, 0x03, 0xFF},       //Static Random Address - Fixed
        #ifdef SUPPORT_STATIC_RANDOM_ADDRESS
        LE_RANDOMBDADDR_FIXED,
        #else
        0,
        #endif
    },

    },

	//- CRC
	{0,0}
};
