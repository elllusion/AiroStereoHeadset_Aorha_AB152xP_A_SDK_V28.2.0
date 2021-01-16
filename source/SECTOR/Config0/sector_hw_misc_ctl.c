/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_hw_misc_ctl.c defines the HW parameters.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "sector_hw_misc_ctl.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_HW_MISC_CTL_STRU CODE gSector_HwMiscCtl =
{
    {
		//For Power Control
		//Default in BITE : -36 ~ -56, so we use -36 + 90 = 54 and -56 + 90 = 34.

		//Commented by Eric, 09.5.18
		//The RSSI upper and lower bound value we used for LM qualification is 40 and 20
		//BITE settings :
		//		Autennuation 	= 32   dB
		//		Lower Limit 	= -72  dBm
		//		Upper Limit		= -52  dBm
		//		Fixed Decrease	= -20  dB
		//
		//Besides, there is an autennuator on our RF board, its autennuation is -10 dB
		//
		//To sum up, the final RSSI value we get is :
		//		-72 + 32 - 20 - 10 + 90 = 20
		//		-52 + 32 - 20 - 10 + 90 = 40

		0,   //U8 RSSI_GoldenRange_UpperBound;
		-45,    //U8 RSSI_GoldenRange_LowerBound;
		#ifdef TWS_SETTINGS
        25,
		#else
        #ifdef BLUETOOTH_SPEAKER //- no sleep
		10,
		#else
		25,
		#endif
		#endif
	    50,//25,		//- 25 * 10 = 250ppm

        //- ADC0UpdatePeriod;   // in ms. This vaiable is better same as SFR setting
        {
            //- update_period : 0 is disable
            1000,
            //- ViChg_avg_depth
            1,
            //- VBat_avg_depth
            1,
            //- VChg_avg_depth
            1,
            //- Temp_avg_depth
            1,
            //- VBatCal_avg_depth
            1,
            //- Vin_avg_depth
            1
        },

        //- TemperatureCompensation;
        1,

    },

	//- CRC
    {0x10,0x77}

};

