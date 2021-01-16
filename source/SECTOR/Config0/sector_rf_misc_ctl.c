/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_rf_misc.c defines the control options of RF.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "sector_rf_misc_ctl.h"

#pragma userclass (HCONST = CONFIG)


#define RF_DEPEND_TX_NUM    0x01
#define RF_DEPEND_



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if (defined OPERATION_IN_RF_AB1500F_FPGA)
SECTOR_RF_MISC_CTL_STRU CODE gSector_RfMiscCtl =
{
	{
        {
            0x00,               //- 4.0 RX GC TABLE
            0x00,
            0x00,
        },
	    {
	        //0x01,               //- tx_default_gfsk_gc1
	        //0x7C,               //- tx_default_gfsk_gc2
	        0xA4,               //- tx_default_gfsk_bias2_1
	        0x0C,               //- tx_default_gfsk_bias2_2
	        //0x01,               //- tx_default_edr_gc1
	        //0x78,               //- tx_default_edr_gc2
	        0xA4,               //- tx_default_edr_bias2_1
	        0x0C,               //- tx_default_edr_bias2_2
	        0x0D,				//- tx_default_ble_gc1;
    		0x76,				//- tx_default_ble_gc2
    		0xBC,				//- tx_default_ble_bias2_1
    		0xA8,				//- tx_default_ble_bias2_2
	        //56,                 //- tx_gfsk_max_gain
	        16,                 //- tx_gfsk_min_gain
	        //48,                 //- tx_edr_max_gain
	        16,                 //- tx_edr_min_gain
	        63,                 //- tx_ble_max_gain;
    		16,                 //- tx_ble_min_gain;
	        0x00,               //- tx_2402_gc1_offset
	        50,                 //- tx_gfsk_gain_step_threshold_hi
            40,                 //- tx_gfsk_gain_step_threshold_lo
	        0x05,               //- tx_gfsk_gain_step_1
	        0x0A,               //- tx_gfsk_gain_step_2
	        0x0C,               //- tx_gfsk_gain_step_3
            0x0A,               //- tx_edr_gain_step
	    },

	    {
	        {0},                                                // reserved
            //{750, 770, 810, 850, 880, 910, 925},                // tx_mod_temp_step
            /* BDR */
            {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06},   // tx_bdr_mod_gain_comp
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // tx_bdr_fil_gain_comp
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // tx_bdr_mod_bias_comp
            /* EDR */
            {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06},   // tx_edr_mod_gain_comp
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // tx_edr_fil_gain_comp
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // tx_edr_mod_bias_comp
            /* BLE */
            {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06},   // tx_ble_mod_gain_comp
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // tx_ble_fil_gain_comp
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // tx_ble_mod_bias_comp

            //{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // BDR (signed) delta of txagc_gc b/w BDR d4 value
            //{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // EDR (signed) delta of txagc_gc b/w EDR d4 value
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // BLE (signed) delta of txagc_gc b/w BLE d4 value
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // keep all zero
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    // keep all zero
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // keep all zero
    	},

        102, //rssi_gap
        102, //le_rssi_gap
        7    //le_tx_power_in_dbm
	},

	//- CRC
    {0x00,0x00}
};

#elif (defined OPERATION_IN_RF_AB1520)
#if (defined AB1520C)
SECTOR_RF_MISC_CTL_STRU CODE gSector_RfMiscCtl =
{
	{
        {
            65,               //- 4.0 RX GC TABLE
            46,
            34,
        },
	    {
		#ifdef AB1122
	        //- tx_gain_ctl
    		//0x0D,				//- tx_default_gfsk_gc1
    		//0x75,				//- tx_default_gfsk_gc2
    		0xBC,				//- tx_default_gfsk_bias2_1
    		0xA8,				//- tx_default_gfsk_bias2_2
    		//0x8D,				//- tx_default_edr_gc1
    		//0x74,				//- tx_default_edr_gc2
    		0xBC,				//- tx_default_edr_bias2_1
    		0xA8,				//- tx_default_edr_bias2_2
    		0x85,				//- tx_default_ble_gc1
    		0x78,				//- tx_default_ble_gc2
    		0xBC,				//- tx_default_ble_bias2_1
    		0xA8,				//- tx_default_ble_bias2_2
    		//42,			        //- tx_gfsk_max_gain;
    		18,			        //- tx_gfsk_min_gain;
    		//41,				//- tx_edr_max_gain;
    		0,				//- tx_edr_min_gain;
    		49,                             //- tx_ble_max_gain;
    		18,                             //- tx_ble_min_gain;
    		0x00,				//- tx_2402_gc1_offset;
                56,                 //- tx_gfsk_gain_step_threshold
                44,                 //- tx_gfsk_gain_step_threshold_lo
                0x0C,               //- tx_gfsk_gain_step_1
                0x09,               //- tx_gfsk_gain_step_2
                0x09,               //- tx_gfsk_gain_step_3
                0x0A,               //- tx_edr_gain_step
		#else
        		#ifdef AB1528C_ONLY
		      	      #ifdef BLUETOOTH_SPEAKER
					//- tx_gain_ctl
    					//0x0D,				//- tx_default_gfsk_gc1
    					//0x75,				//- tx_default_gfsk_gc2
    					0xBC,				//- tx_default_gfsk_bias2_1
    					0xA8,				//- tx_default_gfsk_bias2_2
    					//0x8D,				//- tx_default_edr_gc1
    					//0x74,				//- tx_default_edr_gc2
    					0xBC,				//- tx_default_edr_bias2_1
    					0xA8,				//- tx_default_edr_bias2_2
    					0x8D,				//- tx_default_ble_gc1
    					0x79,				//- tx_default_ble_gc2
    					0xBC,				//- tx_default_ble_bias2_1
    					0xA8,				//- tx_default_ble_bias2_2
    					//42,			        //- tx_gfsk_max_gain;
    					18,			        //- tx_gfsk_min_gain;
    					//41,				//- tx_edr_max_gain;
    					0,				//- tx_edr_min_gain;
    					51,                             //- tx_ble_max_gain;
    					18,                             //- tx_ble_min_gain;
    					0x00,				//- tx_2402_gc1_offset;
					            56,                 //- tx_gfsk_gain_step_threshold
                			44,                 //- tx_gfsk_gain_step_threshold_lo
                			0x0C,               //- tx_gfsk_gain_step_1
               				0x09,               //- tx_gfsk_gain_step_2
                			0x09,               //- tx_gfsk_gain_step_3
                			0x0A,               //- tx_edr_gain_step
			      #else
					//- tx_gain_ctl
    					//0x0D,				//- tx_default_gfsk_gc1
    					//0x75,				//- tx_default_gfsk_gc2
    					0xBC,				//- tx_default_gfsk_bias2_1
    					0xA8,				//- tx_default_gfsk_bias2_2
    					//0x8D,				//- tx_default_edr_gc1
    					//0x74,				//- tx_default_edr_gc2
    					0xBC,				//- tx_default_edr_bias2_1
    					0xA8,				//- tx_default_edr_bias2_2
    					0x0D,				//- tx_default_ble_gc1
    					0x79,				//- tx_default_ble_gc2
    					0xBC,				//- tx_default_ble_bias2_1
    					0xA8,				//- tx_default_ble_bias2_2
    					//42,			        //- tx_gfsk_max_gain;
    					18,			        //- tx_gfsk_min_gain;
    					//41,				//- tx_edr_max_gain;
    					0,				//- tx_edr_min_gain;
    					50,                             //- tx_ble_max_gain;
    					18,                             //- tx_ble_min_gain;
    					0x00,				//- tx_2402_gc1_offset;
					            55,                 //- tx_gfsk_gain_step_threshold
                			40,                 //- tx_gfsk_gain_step_threshold_lo
                			0x0F,               //- tx_gfsk_gain_step_1
               				0x0D,               //- tx_gfsk_gain_step_2
                			0x0C,               //- tx_gfsk_gain_step_3
                			0x0A,               //- tx_edr_gain_step
                              #endif
        		#else
                              #ifdef BLUETOOTH_SPEAKER
					//- tx_gain_ctl
    		                        //0x0D,				//- tx_default_gfsk_gc1
    		                        //0x75,				//- tx_default_gfsk_gc2
    		                        0xBC,				//- tx_default_gfsk_bias2_1
    		                        0xA8,				//- tx_default_gfsk_bias2_2
    		                        //0x8D,				//- tx_default_edr_gc1
    		                        //0x74,				//- tx_default_edr_gc2
    		                        0xBC,				//- tx_default_edr_bias2_1
    		                        0xA8,				//- tx_default_edr_bias2_2
    		                        0x85,				//- tx_default_ble_gc1
    		                        0x78,				//- tx_default_ble_gc2
    		                        0xBC,				//- tx_default_ble_bias2_1
    		                        0xA8,				//- tx_default_ble_bias2_2
    		                        //42,			        //- tx_gfsk_max_gain;
    		                        18,			        //- tx_gfsk_min_gain;
    		                        //41,				//- tx_edr_max_gain;
    		                        0,				//- tx_edr_min_gain;
    		                        49,                             //- tx_ble_max_gain;
    		                        18,                             //- tx_ble_min_gain;
    		                        0x00,				//- tx_2402_gc1_offset;
                                        56,                 //- tx_gfsk_gain_step_threshold
                                        44,                 //- tx_gfsk_gain_step_threshold_lo
                                        0x0C,               //- tx_gfsk_gain_step_1
                                        0x09,               //- tx_gfsk_gain_step_2
                                        0x09,               //- tx_gfsk_gain_step_3
                                        0x0A,               //- tx_edr_gain_step
			      #else
					//- tx_gain_ctl
    					//0x0D,				//- tx_default_gfsk_gc1
    					//0x75,				//- tx_default_gfsk_gc2
    					0xBC,				//- tx_default_gfsk_bias2_1
    					0xA8,				//- tx_default_gfsk_bias2_2
    					//0x8D,				//- tx_default_edr_gc1
    					//0x74,				//- tx_default_edr_gc2
    					0xBC,				//- tx_default_edr_bias2_1
    					0xA8,				//- tx_default_edr_bias2_2
    					0x0D,				//- tx_default_ble_gc1
    					0x79,				//- tx_default_ble_gc2
    					0xBC,				//- tx_default_ble_bias2_1
    					0xA8,				//- tx_default_ble_bias2_2
    					//42,			        //- tx_gfsk_max_gain;
    					18,			        //- tx_gfsk_min_gain;
    					//41,				//- tx_edr_max_gain;
    					0,				//- tx_edr_min_gain;
    					50,                             //- tx_ble_max_gain;
    					18,                             //- tx_ble_min_gain;
    					0x00,				//- tx_2402_gc1_offset;
					                              55,                 //- tx_gfsk_gain_step_threshold
                                        40,                 //- tx_gfsk_gain_step_threshold_lo
                                        0x0F,               //- tx_gfsk_gain_step_1
                                        0x0D,               //- tx_gfsk_gain_step_2
                                        0x0C,               //- tx_gfsk_gain_step_3
                                        0x0A,               //- tx_edr_gain_step
                              #endif
        		#endif

	       #endif
	    },

	    {
	        {0},                                                // reserved
            //{683, 715, 749, 789, 833, 868, 892},  // tx_mod_temp_step
	   #ifdef AB1122
	   /* BDR */
	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	  {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // tx_bdr_fil_gain_comp
	  {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	  /* EDR */
	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_mod_gain_comp
	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_fil_gain_comp
	  {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	  /* BLE */
	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	  {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // tx_ble_fil_gain_comp
	  {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp

	  {0xFF, 0xFE, 0xFE, 0xFE, 0x00, 0x02, 0x04, 0x07}, // BLE (signed) delta of txagc_gc b/w BLE d4 value                   {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	  {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
    	  #else
       	    #ifdef AB1528C_ONLY
	      #ifdef BLUETOOTH_SPEAKER
	      /* BDR */
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	     /* EDR */
	     {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_mod_gain_comp
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	     /* BLE */
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp
	     {0x04, 0x00, 0x01, 0xFE, 0x00, 0x02, 0x04, 0x05}, // BLE (signed) delta of txagc_gc b/w BLE d4 value
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
	     #else
	     /* BDR */
	      /* BDR */
	     {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	     {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02}, // tx_bdr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	     /* EDR */
	     {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02}, // tx_edr_mod_gain_comp
	     {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02}, // tx_edr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	     /* BLE */
	     {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	     {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02}, // tx_ble_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp
	     {0x06, 0x03, 0x00, 0xFD, 0x00, 0x03, 0x06, 0x0A}, // BLE (signed) delta of txagc_gc b/w BLE d4 value
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
            #endif
            #else
            #ifdef BLUETOOTH_SPEAKER
	    /* BDR */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // tx_bdr_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	    /* EDR */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_mod_gain_comp
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	    /* BLE */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // tx_ble_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp

	    {0xFF, 0xFE, 0xFE, 0xFE, 0x00, 0x02, 0x04, 0x07}, // BLE (signed) delta of txagc_gc b/w BLE d4 value                   {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	    {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
            #else
            /* BDR */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02}, // tx_bdr_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	    /* EDR */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02}, // tx_edr_mod_gain_comp
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02}, // tx_edr_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	    /* BLE */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02}, // tx_ble_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp
	    {0x06, 0x03, 0x00, 0xFD, 0x00, 0x03, 0x06, 0x0A}, // BLE (signed) delta of txagc_gc b/w BLE d4 value
	    {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	    {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
                              	         #endif
       				 #endif

			#endif




	    },

        102, //rssi_gap
        102, //le_rssi_gap
        7    //le_tx_power_in_dbm

	},

	//- CRC
    {0x00,0x00}
};
#elif (defined AB1520D)
SECTOR_RF_MISC_CTL_STRU CODE gSector_RfMiscCtl =
{
	{
        {
            65,               //- 4.0 RX GC TABLE
            46,
            34,
        },
	    {
		#ifdef AB1122
	        //- tx_gain_ctl
    		//0x0D,				//- tx_default_gfsk_gc1
    		//0x75,				//- tx_default_gfsk_gc2
    		0xBC,				//- tx_default_gfsk_bias2_1
    		0xA8,				//- tx_default_gfsk_bias2_2
    		//0x8D,				//- tx_default_edr_gc1
    		//0x74,				//- tx_default_edr_gc2
    		0xBC,				//- tx_default_edr_bias2_1
    		0xA8,				//- tx_default_edr_bias2_2
    		0x85,				//- tx_default_ble_gc1
    		0x78,				//- tx_default_ble_gc2
    		0xBC,				//- tx_default_ble_bias2_1
    		0xA8,				//- tx_default_ble_bias2_2
    		//42,			        //- tx_gfsk_max_gain;
    		18,			        //- tx_gfsk_min_gain;
    		//41,				//- tx_edr_max_gain;
    		0,				//- tx_edr_min_gain;
    		49,                             //- tx_ble_max_gain;
    		18,                             //- tx_ble_min_gain;
    		0x00,				//- tx_2402_gc1_offset;
                56,                 //- tx_gfsk_gain_step_threshold
                44,                 //- tx_gfsk_gain_step_threshold_lo
                0x0C,               //- tx_gfsk_gain_step_1
                0x09,               //- tx_gfsk_gain_step_2
                0x09,               //- tx_gfsk_gain_step_3
                0x0A,               //- tx_edr_gain_step
		#else
        		#ifdef AB1528C_ONLY
		      	      #ifdef BLUETOOTH_SPEAKER
					//- tx_gain_ctl
    					//0x0D,				//- tx_default_gfsk_gc1
    					//0x75,				//- tx_default_gfsk_gc2
    					0xBC,				//- tx_default_gfsk_bias2_1
    					0xA8,				//- tx_default_gfsk_bias2_2
    					//0x8D,				//- tx_default_edr_gc1
    					//0x74,				//- tx_default_edr_gc2
    					0xBC,				//- tx_default_edr_bias2_1
    					0xA8,				//- tx_default_edr_bias2_2
    					0x8D,				//- tx_default_ble_gc1
    					0x79,				//- tx_default_ble_gc2
    					0xBC,				//- tx_default_ble_bias2_1
    					0xA8,				//- tx_default_ble_bias2_2
    					//42,			        //- tx_gfsk_max_gain;
    					18,			        //- tx_gfsk_min_gain;
    					//41,				//- tx_edr_max_gain;
    					0,				//- tx_edr_min_gain;
    					51,                             //- tx_ble_max_gain;
    					18,                             //- tx_ble_min_gain;
    					0x00,				//- tx_2402_gc1_offset;
					            56,                 //- tx_gfsk_gain_step_threshold
                			44,                 //- tx_gfsk_gain_step_threshold_lo
                			0x0C,               //- tx_gfsk_gain_step_1
               				0x09,               //- tx_gfsk_gain_step_2
                			0x09,               //- tx_gfsk_gain_step_3
                			0x0A,               //- tx_edr_gain_step
			      #else
					//- tx_gain_ctl
    					//0x0D,				//- tx_default_gfsk_gc1
    					//0x75,				//- tx_default_gfsk_gc2
    					0xBC,				//- tx_default_gfsk_bias2_1
    					0xA8,				//- tx_default_gfsk_bias2_2
    					//0x8D,				//- tx_default_edr_gc1
    					//0x74,				//- tx_default_edr_gc2
    					0x8B,				//- tx_default_edr_bias2_1
    					0xA8,				//- tx_default_edr_bias2_2
    					0x0D,				//- tx_default_ble_gc1
    					0x99,				//- tx_default_ble_gc2
    					0xBC,				//- tx_default_ble_bias2_1
    					0xA8,				//- tx_default_ble_bias2_2
    					//42,			        //- tx_gfsk_max_gain;
    					18,			        //- tx_gfsk_min_gain;
    					//41,				//- tx_edr_max_gain;
    					0,				//- tx_edr_min_gain;
    					50,                             //- tx_ble_max_gain;
    					18,                             //- tx_ble_min_gain;
    					0x00,				//- tx_2402_gc1_offset;
					            57,                 //- tx_gfsk_gain_step_threshold
                			45,                 //- tx_gfsk_gain_step_threshold_lo
                			0x0E,               //- tx_gfsk_gain_step_1
               				0x0A,               //- tx_gfsk_gain_step_2
                			0x08,               //- tx_gfsk_gain_step_3
                			0x0A,               //- tx_edr_gain_step
                              #endif
        		#else
                              #ifdef BLUETOOTH_SPEAKER
					//- tx_gain_ctl
    		                        //0x0D,				//- tx_default_gfsk_gc1
    		                        //0x75,				//- tx_default_gfsk_gc2
    		                        0xBC,				//- tx_default_gfsk_bias2_1
    		                        0xA8,				//- tx_default_gfsk_bias2_2
    		                        //0x8D,				//- tx_default_edr_gc1
    		                        //0x74,				//- tx_default_edr_gc2
    		                        0xBC,				//- tx_default_edr_bias2_1
    		                        0xA8,				//- tx_default_edr_bias2_2
    		                        0x85,				//- tx_default_ble_gc1
    		                        0x78,				//- tx_default_ble_gc2
    		                        0xBC,				//- tx_default_ble_bias2_1
    		                        0xA8,				//- tx_default_ble_bias2_2
    		                        //42,			        //- tx_gfsk_max_gain;
    		                        18,			        //- tx_gfsk_min_gain;
    		                        //41,				//- tx_edr_max_gain;
    		                        0,				//- tx_edr_min_gain;
    		                        49,                             //- tx_ble_max_gain;
    		                        18,                             //- tx_ble_min_gain;
    		                        0x00,				//- tx_2402_gc1_offset;
                                        56,                 //- tx_gfsk_gain_step_threshold
                                        44,                 //- tx_gfsk_gain_step_threshold_lo
                                        0x0C,               //- tx_gfsk_gain_step_1
                                        0x09,               //- tx_gfsk_gain_step_2
                                        0x09,               //- tx_gfsk_gain_step_3
                                        0x0A,               //- tx_edr_gain_step
			      #else
					//- tx_gain_ctl
    					//0x0D,				//- tx_default_gfsk_gc1
    					//0x75,				//- tx_default_gfsk_gc2
    					0xBC,				//- tx_default_gfsk_bias2_1
    					0xA8,				//- tx_default_gfsk_bias2_2
    					//0x8D,				//- tx_default_edr_gc1
    					//0x74,				//- tx_default_edr_gc2
    					0x8B,				//- tx_default_edr_bias2_1
    					0xA8,				//- tx_default_edr_bias2_2
    					0x0D,				//- tx_default_ble_gc1
    					0x99,				//- tx_default_ble_gc2
    					0xBC,				//- tx_default_ble_bias2_1
    					0xA8,				//- tx_default_ble_bias2_2
    					//42,			        //- tx_gfsk_max_gain;
    					18,			        //- tx_gfsk_min_gain;
    					//41,				//- tx_edr_max_gain;
    					0,				//- tx_edr_min_gain;
    					50,                             //- tx_ble_max_gain;
    					18,                             //- tx_ble_min_gain;
    					0x00,				//- tx_2402_gc1_offset;
					            57,                 //- tx_gfsk_gain_step_threshold
                			45,                 //- tx_gfsk_gain_step_threshold_lo
                			0x0E,               //- tx_gfsk_gain_step_1
               				0x0A,               //- tx_gfsk_gain_step_2
                			0x08,               //- tx_gfsk_gain_step_3
                			0x0A,               //- tx_edr_gain_step
                              #endif
        		#endif

	       #endif
	    },

	    {
	        {0},                                                // reserved
            //{683, 715, 749, 789, 833, 868, 892},  // tx_mod_temp_step
	   #ifdef AB1122
	   /* BDR */
	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	  {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // tx_bdr_fil_gain_comp
	  {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	  /* EDR */
	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_mod_gain_comp
	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_fil_gain_comp
	  {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	  /* BLE */
	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	  {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // tx_ble_fil_gain_comp
	  {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp

	  {0xFF, 0xFE, 0xFE, 0xFE, 0x00, 0x02, 0x04, 0x07}, // BLE (signed) delta of txagc_gc b/w BLE d4 value                   {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	  {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
    	  #else
       	    #ifdef AB1528C_ONLY
	      #ifdef BLUETOOTH_SPEAKER
	      /* BDR */
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	     /* EDR */
	     {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_mod_gain_comp
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	     /* BLE */
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	     {0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp
	     {0x04, 0x00, 0x01, 0xFE, 0x00, 0x02, 0x04, 0x05}, // BLE (signed) delta of txagc_gc b/w BLE d4 value
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
	     #else
	     /* BDR */
	     {0x01, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04}, // tx_bdr_mod_gain_comp
	     {0x00, 0x00, 0x01, 0x02, 0x03, 0x02, 0x02, 0x02}, // tx_bdr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	     /* EDR */
	     {0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04}, // tx_edr_mod_gain_comp
	     {0x00, 0x00, 0x01, 0x02, 0x03, 0x02, 0x01, 0x01}, // tx_edr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	     /* BLE */
	     {0x01, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04},  // tx_ble_mod_gain_comp
	     {0x00, 0x00, 0x01, 0x02, 0x03, 0x02, 0x02, 0x02},  // tx_ble_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02},  // tx_ble_mod_bias_comp
	     {0x07, 0x07, 0x05, 0x01, 0x00, 0x05, 0x08, 0x08},   // BLE (signed) delta of txagc_gc b/w BLE d4 value
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // keep all zero
            #endif
            #else
            #ifdef BLUETOOTH_SPEAKER
	    /* BDR */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // tx_bdr_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	    /* EDR */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_mod_gain_comp
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	    /* BLE */
	    {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // tx_ble_fil_gain_comp
	    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp

	    {0xFF, 0xFE, 0xFE, 0xFE, 0x00, 0x02, 0x04, 0x07}, // BLE (signed) delta of txagc_gc b/w BLE d4 value                   {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	    {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
            #else
          /* BDR */
	     {0x01, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04}, // tx_bdr_mod_gain_comp
	     {0x00, 0x00, 0x01, 0x02, 0x03, 0x02, 0x02, 0x02}, // tx_bdr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	     /* EDR */
	     {0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04}, // tx_edr_mod_gain_comp
	     {0x00, 0x00, 0x01, 0x02, 0x03, 0x02, 0x01, 0x01}, // tx_edr_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	     /* BLE */
	     {0x01, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04},  // tx_ble_mod_gain_comp
	     {0x00, 0x00, 0x01, 0x02, 0x03, 0x02, 0x02, 0x02},  // tx_ble_fil_gain_comp
	     {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02},  // tx_ble_mod_bias_comp
	     {0x07, 0x07, 0x05, 0x01, 0x00, 0x05, 0x08, 0x08},  // BLE (signed) delta of txagc_gc b/w BLE d4 value
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	     {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
	     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // keep all zero
                              	         #endif
       				 #endif

			#endif




	    },

        102, //rssi_gap
        102, //le_rssi_gap
        7    //le_tx_power_in_dbm

	},

	//- CRC
    {0x00,0x00}
};

#endif

#elif (defined OPERATION_IN_RF_AB1520S)
SECTOR_RF_MISC_CTL_STRU CODE gSector_RfMiscCtl =
{
	{
        {
            65,               //- 4.0 RX GC TABLE
            46,
            34,
        },
	    {   //- tx_gain_ctl
    		//0x85,				//- tx_default_gfsk_gc1
    		//0x78,				//- tx_default_gfsk_gc2
    		0xBC,				//- tx_default_gfsk_bias2_1
    		0xA8,				//- tx_default_gfsk_bias2_2
    		//0x09,				//- tx_default_edr_gc1
    		//0x73,				//- tx_default_edr_gc2
    		0xBC,				//- tx_default_edr_bias2_1
    		0xA8,				//- tx_default_edr_bias2_2
    		0x85,				//- tx_default_ble_gc1
    		0x78,				//- tx_default_ble_gc2
    		0xBC,				//- tx_default_ble_bias2_1
    		0xA8,				//- tx_default_ble_bias2_2
    		//49,					//- tx_gfsk_max_gain;
    		18,					//- tx_gfsk_min_gain;
    		//38,					//- tx_edr_max_gain;
    		0,					//- tx_edr_min_gain;
    		49,                 //- tx_ble_max_gain;
    		16,                 //- tx_ble_min_gain;
    		0x00,				//-	tx_2402_gc1_offset;
            56,                 //- tx_gfsk_gain_step_threshold
            44,                 //- tx_gfsk_gain_step_threshold_lo
            0x0F,               //- tx_gfsk_gain_step_1
            0x0E,               //- tx_gfsk_gain_step_2
            0x0C,               //- tx_gfsk_gain_step_3
            0x0A,               //- tx_edr_gain_step
	    },

	    {
	        {0},                                                // reserved
            //{683, 715, 749, 789, 833, 868, 892},  // tx_mod_temp_step
           /* BDR */
	   {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_bdr_mod_gain_comp
	   {0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x03}, // tx_bdr_fil_gain_comp
	   {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_bdr_mod_bias_comp
	   /* EDR */
           	{0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_edr_mod_gain_comp
	   {0x00, 0x00, 0x01, 0x01, 0x02, 0x03, 0x03, 0x03}, // tx_edr_fil_gain_comp
	   {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_edr_mod_bias_comp
	   /* BLE */
           	  {0x00, 0x01, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03}, // tx_ble_mod_gain_comp
	   {0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x03}, // tx_ble_fil_gain_comp
	   {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, // tx_ble_mod_bias_comp

	    //{0x07, 0x04, 0x01, 0xFD, 0x00, 0x03, 0x05, 0x07}, // BDR (signed) delta of txagc_gc b/w BDR d4 value
	    //{0x07, 0x04, 0x01, 0xFD, 0x00, 0x03, 0x05, 0x07}, // EDR (signed) delta of txagc_gc b/w EDR d4 value
            //{0xFF, 0xFE, 0xFE, 0xFE, 0x00, 0x02, 0x04, 0x06}, // BLE (signed) delta of txagc_gc b/w BLE d4 value
            {0xFE, 0xFC, 0xFB, 0xFB, 0x00, 0x00, 0x00, 0x04}, // BLE (signed) delta of txagc_gc b/w BLE d4 value
	    {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06}, // keep all zero
	    {0xF8, 0xFA, 0xFC, 0xFE, 0x00, 0x02, 0x04, 0x06},  // keep all zero
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // keep all zero
	    },

        102, //rssi_gap
        102, //le_rssi_gap
        7    //le_tx_power_in_dbm
	},

	//- CRC
    {0x00,0x00}
};

#endif
