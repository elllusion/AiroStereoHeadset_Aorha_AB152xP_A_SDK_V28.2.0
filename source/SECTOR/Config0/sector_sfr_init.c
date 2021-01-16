/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sfr_init.c defines the initialization sequence to setup CSR.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"
#include "sector_sfr_init.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_SFR_INIT_STRU CODE gSector_SfrInit =
{
    {
        #ifdef OPERATION_IN_BB_ASIC_VERSION
			#ifdef AB1520B        
			144, //NumOfSfrRegInit
			#else
			146,
			#endif
        #else
			#ifdef AB1520B       
			142, //NumOfSfrRegInit
			#else
			144,
			#endif
    	#endif

        {
            #ifdef OPERATION_IN_BB_ASIC_VERSION
			#if defined SCO_WOOFER || defined SOUND_BAR || defined TWS_SETTINGS
			{0xC2, 201, 0x01}, //setup MUC CLK to 24M
			#else
			{0xC2, 201, 0x00}, //setup MUC CLK to 12M
			#endif
			#endif
			
			{0xE6, 30, 0},
			#if defined SOUND_BAR
			{0xE6, 71, 0},
			#else
			{0xE6, 71, 3},
			#endif
			{0xE6, 97, 0xEA},

			{0xE6, 30, 1},
			#if defined SOUND_BAR
			{0xE6, 71, 0},
			#else
			{0xE6, 71, 3},
			#endif			
			{0xE6, 97, 0xEA},

            {0xC2, 121, 100},   //- RF PAON
            {0xC2, 123, 15},    //- RF PLLON
            {0xC2, 124, 110},   //- RF RXON
            {0xC2, 125, 100},   //- RF TXON

            {0xE6, 1, 100},      //- 4.0 RF PAON time
            {0xE6, 2, 100},      //- 4.0 RF TXON time
            {0xE6, 3, 15},       //- 4.0 RF PLLON time
            {0xE6, 4, 100},      //- 4.0 RF RXON time
            {0xE6, 6, 130},
            {0xE6, 12, 127},
            {0xE6, 13, 127},
            #if (defined OPERATION_IN_RF_AB1520) || (defined OPERATION_IN_RF_AB1520S)
            {0xFE, 145, 0x35},  //- tx_gc_wire3_address
            #else
            {0xFE, 145, 0x0C},  //- tx_gc_wire3_address
            #endif
            {0xFE, 146, 0x0E},  //- tx_bias_wire3_address

            {0xFE, 62, 0},      //- PLL_XTAL_DIV_H_L_SIDE

            //- RF TRX APPEND
            #if (defined OPERATION_IN_RF_AB1520) || (defined OPERATION_IN_RF_AB1520S)
            {0xFE, 70, 0x11},   //- RF_TRX_APPEND_EN, bit0:tx_en, bit1:tx_num, bit4:rx_en, bit5:rx_num
            #else
            {0xFE, 70, 0x00},   //- RF_TRX_APPEND_EN, bit0:tx_en, bit1:tx_num, bit4:rx_en, bit5:rx_num
            #endif
            {0xFE, 71, 0},      //- RF_3WIRE_INDEX:0
            {0xFE, 72, 0x13},   //- RF_TRX_VALUE_B0
            {0xFE, 73, 0xAA},   //- RF_TRX_VALUE_B1
            {0xFE, 74, 0x07},   //- RF_TRX_VALUE_B3
            {0xFE, 71, 1},      //- RF_3WIRE_INDEX:1
            {0xFE, 72, 0x00},   //- RF_TRX_VALUE_B0
            {0xFE, 73, 0x00},   //- RF_TRX_VALUE_B1
            {0xFE, 74, 0x00},   //- RF_TRX_VALUE_B3
            {0xFE, 71, 2},      //- RF_3WIRE_INDEX:2
            {0xFE, 72, 0x17},   //- RF_TRX_VALUE_B0
            {0xFE, 73, 0xAA},   //- RF_TRX_VALUE_B1
            {0xFE, 74, 0x07},   //- RF_TRX_VALUE_B3
            {0xFE, 71, 3},      //- RF_3WIRE_INDEX:3
            {0xFE, 72, 0x00},   //- RF_TRX_VALUE_B0
            {0xFE, 73, 0x00},   //- RF_TRX_VALUE_B1
            {0xFE, 74, 0x00},   //- RF_TRX_VALUE_B3
            //----------------

			#ifdef OPERATION_IN_BB_ASIC_VERSION
    		//{0xC2, 57, 0x01}, // test audio, MXT_CTRL enable
    		//{0xC2, 61, 0x41}, // test audio, 1K TONE
    		#endif
    		{0xE4, 128, 0x00},
    		{0xE4, 129, 0x00},
    		{0xE4, 130, 0x84},
    		{0xE4, 131, 0xC0},
    		{0xE4, 132, 0x54},
    		{0xE4, 133, 0x0D},
    		{0xE4, 134, 0x00},
    		{0xE4, 135, 0x22},
    		{0xE4, 136, 0xA5},
    		{0xE4, 137, 20},
    		{0xE4, 138, 12},
    		{0xE4, 139, 24},
    		{0xE4, 140, 36},
    		{0xE4, 141, 24},
    		{0xE4, 142, 0x4E},
    		{0xE4, 143, 0xA9},
    		{0xE4, 144, 16},
    		{0xE4, 145, 0x02},
    		{0xE4, 146, 0x27},
    		{0xE4, 147, 0x01},

    		//- new modem bm matrix, (E4,152)~(E4,154), (E4,158)~(E4,160), (E4,164)~(E4,166)
    		//- number of bm matrix setting setting = 9
    		{0xE4, 152, 0x05},
    		{0xE4, 153, 0x0B},
    		{0xE4, 154, 0x14},

    		{0xE4, 158, 0x42},
    		{0xE4, 159, 0x45},
    		{0xE4, 160, 0x42},

    		{0xE4, 164, 0x14},
    		{0xE4, 165, 0x0B},
    		{0xE4, 166, 0x05},

    		//- BT4.0 AGC, (E4,170)~(E4,190)
    		//- number of 4.0 AGC setting = 21
    		{0xE4, 170, 54},  //- BLE AGC target level
    		{0xE4, 171, 21},  //- BLE AGC uhg1 threshold
    		{0xE4, 172, 7},   //- BLE AGC uhg2 threshold
    		{0xE4, 173, 7},   //- BLE AGC uhg3 threshold
    		{0xE4, 174, 53},  //- BLE AGC hg1 threshold
    		{0xE4, 175, 53},  //- BLE AGC hg2 threshold
    		{0xE4, 176, 53},  //- BLE AGC hg3 threshold
    		{0xE4, 177, 7},   //- BLE AGC hg4 threshold
    		{0xE4, 178, 35},  //- BLE AGC lg1 threshold
    		{0xE4, 179, 35},  //- BLE AGC lg2 threshold
    		{0xE4, 180, 30},  //- BLE AGC lg3 threshold
    		{0xE4, 181, 30},  //- BLE AGC lg4 threshold
			#if defined(OPERATION_IN_RF_AB1500F_FPGA)
			{0xE4, 182, 4},  //- BLE AGC ultra high gain
    		{0xE4, 183, 12},  //- BLE AGC high gain
    		{0xE4, 184, 12},  //- BLE AGC middle gain
    		{0xE4, 185, 19},  //- BLE AGC low gain
    		{0xE4, 186, 19},  //- BLE AGC ultra low gain
			#else
    		{0xE4, 182, 59},  //- BLE AGC ultra high gain
    		{0xE4, 183, 60},  //- BLE AGC high gain
    		{0xE4, 184, 60},  //- BLE AGC middle gain
    		{0xE4, 185, 61},  //- BLE AGC low gain
    		{0xE4, 186, 61},  //- BLE AGC ultra low gain
			#endif
    		{0xE4, 187, 0x0A},//- BLE AGC gc settle time and tolerance
    		{0xE4, 188, 0x03},//- BLE AGC fix gain ctrl
    		{0xE4, 189, 0x01},//- BLE AGC state tperiod: Period for state to update
    		{0xE4, 190, 0x00},//- BLE AAGC apply lpf at BT4.0 & BT3.0

    		//- BT3.0 AGC, (E4,192)~(E4,197)
    		//- number of 3.0 AGC setting = 6
    		//{0xE4, 192, 0x5E},//{0xE4, 192, 0x80},
    		//{0xE4, 193, 0x03},//{0xE4, 193, 0x07},//{0xE4, 193, 0x06},
    		{0xE4, 192, 0x5C},//{0xE4, 192, 0x7C},//{0xE4, 192, 0x5C},//{0xE4, 192, 0x80},
    		{0xE4, 193, 0x03},//{0xE4, 193, 0x07},//{0xE4, 193, 0x06},
    		{0xE4, 194, 0x6B},
    		{0xE4, 195, 0x05},
    		{0xE4, 196, 0x00},
    		{0xE4, 197, 0xFC},

    		//- BT4.0 RX modem, (E4,208)~(E4,218)
    		//- number of bt4.0 rx setting = 11
    		{0xE4, 208, 0xFF},
    		{0xE4, 209, 3},
    		{0xE4, 210, 0x00},
    		{0xE4, 211, 0x00},
    		{0xE4, 212, 0x02},
    		{0xE4, 213, 0x02},
    		{0xE4, 214, 0x02},
    		{0xE4, 215, 0x68},
    		{0xE4, 216, 0x46},
    		{0xE4, 217, 40},
    		{0xE4, 218, 0x00},

    		//- TX modem
    		//- number of tx modem setting = 8
    		{0xE4, 224, 41},
    		{0xE4, 225, 64},//{0xE4, 225, 60},
    		{0xE4, 226, 0x29},
    		{0xE4, 227, 0x80},//{0xE4, 227, 0x84},//{0xE4, 227, 0x80}, //TX DAC scale
    		{0xE4, 228, 0x80},
                {0xE4, 229, 0x0F},//TX DAC Offset
    		#ifdef OPERATION_IN_BB_FPGA_VERSION
    		{0xE4, 229, 0x78},
    		{0xE4, 230, 0x42},
    		#else
    		{0xE4, 230, 0x40},
    		#endif

    		{0xE4, 231, 0x00},

    		//- btc ctl
    		{0xC2, 130, 23},
    		{0xC2, 132, 0x05},
    		{0xC2, 80, 10},
    		{0xFE, 63, 0x00},
    		#if (defined OPERATION_IN_RF_AB1500D || defined OPERATION_IN_RF_AB1500F || defined OPERATION_IN_RF_AB1500G || defined OPERATION_IN_RF_AB1500F_FPGA || defined OPERATION_IN_RF_AB1520 || defined OPERATION_IN_RF_AB1520S)
    		{0xC2, 134, 0x04},
    		{0xFE, 64, 0x06},
    		{0xFE, 65, 0x06},
    		#else
    		{0xC2, 134, 0x07},
    		{0xFE, 64, 0x02},
    		{0xFE, 65, 0x03},
    		#endif

    		{0xFE, 130, 0x30}, //- FEOFFS_RW_AFH_RSSI_CTL_PARAM

    		//- RF PLL CTL
    		#if ((defined OPERATION_IN_BB_FPGA_VERSION )&& (defined OPERATION_IN_RF_AB1000D))
    		{0xFE, 79, 0x28},
    		{0xFE, 80, 0x0A},
    		{0xFE, 81, 0x4F},
    		{0xFE, 82, 0xEC},
    		{0xFE, 83, 0x04},
    		#else
    		{0xFE, 79, 0x80},
    		{0xFE, 80, 0x0C},
    		{0xFE, 81, 0x00},
    		{0xFE, 82, 0x00},
    		{0xFE, 83, 0x04},
    		#endif

            #ifdef OPERATION_IN_BB_ASIC_VERSION
            {0xFB, 190, 0x0F},  //MCU clock gating
            #endif

            //- Clock gating control
			{0xC2,200,0xA6},//0x20, (0x20:slot tick is enabled in MCU frozen)

			//-Slow CLK Calibration
            #ifdef OPERATION_IN_BB_ASIC_VERSION
                #ifdef BLUETOOTH_SPEAKER
				{0xE7, 97, 0x87},
				{0xE7, 93, 0x22},  //- min_sleep_duration_enter_in_slot // no sleep
                {0xE7, 112, 0x31},  //- slow_clk_cal
                {0xE7, 113, 0x01},  //- slow_clk_ctl
               	{0xE7, 114, 0x20},  //- slow_clk_int
               	{0xE7, 115, 0x00},  //- slow_clk_dec1
               	{0xE7, 116, 0x00},  //- slow_clk_dec2
		        #else
				{0xE7, 97, 0x87},	//- U8	osc_pll_wakeup_guard;
				{0xE7, 93, 0x12},  //- min_sleep_duration_enter_in_slot
		        {0xE7, 112, 0x30},  //- slow_clk_cal
              	{0xE7, 113, 0x01},  //- slow_clk_ctl
              	{0xE7, 114, 0x1F},  //- slow_clk_int
              	{0xE7, 115, 0x52},  //- slow_clk_dec1
               	{0xE7, 116, 0x00},  //- slow_clk_dec2
                #endif
            #else
				#if 1 //- for crystal 32.768K
				{0xE7, 97, 0x87},   //- osc_pll_wakeup_guard
	    		{0xE7, 93, 0x12},   //- min_sleep_duration_enter_in_slot
	            {0xE7, 112, 0x31},  //- slow_clk_cal
	            {0xE7, 113, 0x01},  //- slow_clk_ctl
	            {0xE7, 114, 0x1E},  //- slow_clk_int
	            {0xE7, 115, 0x15},  //- slow_clk_dec1
	            {0xE7, 116, 0x07},  //- slow_clk_dec2
				#else //- for crystal 32K
	            {0xE7, 97, 0x87},   //- osc_pll_wakeup_guard
	    		{0xE7, 93, 0x12},   //- min_sleep_duration_enter_in_slot
	            {0xE7, 112, 0x30},  //- slow_clk_cal
	            {0xE7, 113, 0x01},  //- slow_clk_ctl
	            {0xE7, 114, 0x1F},  //- slow_clk_int
	            {0xE7, 115, 0x52},  //- slow_clk_dec1
	            {0xE7, 116, 0x00},  //- slow_clk_dec2
				#endif
            #endif

		    {0xFE, 147, 1},    //- channel
		    {0xFE, 148, 79},   //- channe2
		    {0xFE, 149, 0},    //- GC offsetl
		    {0xFE, 150, 0},    //- GC offset2

			{0xE7, 127, 0xE0},	// reserved slot weight
			{0xE7, 128, 0x08},	// reserved slot weight
			{0xE7, 22,  0x01},	// reserved slot weight
			{0xE7, 12,  0x03},	
			{0xE7, 20,  0x40},
	
			#ifndef AB1520B
			{0xDB, 0, 0x01},  //- BT4.0 rx interrupt priority higher than BT3.0 rx interrupt priority	
			#endif
			
			#ifdef AB1520S
			{0xFE, 175, 0},
			#else
				#ifndef AB1520B
				{0xE7, 232, 0},		// set the Tx band1 offset for GFSK
				#endif
			#endif
        }
    },

    //- CRC
    {0xDE,0x9C}
};
