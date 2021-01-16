/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_mp_parameter.c defines the calibration parameters and Rom code may uses.
 *
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 */
#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"
#include "sector_mp_parameter.h"

#pragma userclass (HCONST = MP_PARAM)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_MP_PARAMETER_STRU CODE gSector_MpParameter =
{
    {
        {
    		#ifdef LC_TEST_CASE_CONFIG
    			{0x21, 0x43, 0x65, 0x00, 0x00, 0x00}, //for BT Qualification
    		#else
    			#if defined(HID_IIM_SOC)
    			{0x18, 0x13 , 0x00 , 0xA1 , 0x12 ,0x00 },
    			#elif defined(HID_SOC)
    			{0x21, 0x22 , 0x22 , 0x22 , 0x22 ,0x22 },
    			#elif defined SOUND_BAR
    			{0x7E, 0x96, 0xC6, 0x6A, 0x00, 0x00},
    			#elif defined SCO_WOOFER
    			{0x70, 0x94, 0xC6, 0x6A, 0x00, 0x00},
    			#elif defined FAST_CONNECT_DONGLE
    			{0x88, 0x99, 0xCC, 0x66, 0x00, 0x00},
    			#else
    		//- {0x7E, 0x96, 0xC6, 0x6B, 0x00, 0x00},
    		//- {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    		//- {0x11, 0x11, 0x11, 0x11, 0x11, 0x11},
    		//- {0x66, 0x66, 0x66, 0x66, 0x66, 0x66},
    		//- {0x77, 0x77, 0x77, 0x77, 0x77, 0x77},
    		//- {0x88, 0x88, 0x88, 0x88, 0x88, 0x88},
    		//- {0x0b, 0xcd, 0xe3, 0x60, 0x10, 0x00},
    		//- {0x33, 0x8B, 0x9E, 0x00, 0x00, 0x00},
    		//- {0x21, 0x43, 0x65, 0x21, 0x43, 0x65},
    		//- {0x55, 0x55, 0x55, 0xee, 0xee, 0xee},
    		//- {0x55, 0x44, 0x33, 0x22, 0x11, 0x00},
    			{0x22, 0x33, 0x22, 0x33, 0x03, 0x00}, //for BT Qualification
    			#endif
    		#endif

    		#ifdef USING_SIMPLE_PAIRING_DEBUG_PRIVATE_KEY
            {0x07, 0x91, 0x5f, 0x86, 0x91, 0x8d, 0xdc, 0x27,    //Private key: Big Endian
            0x00, 0x5d, 0xf1, 0xd6, 0xcf, 0x0c, 0x14, 0x2b,
            0x62, 0x5e, 0xd2, 0xef, 0xf4, 0xa5, 0x18, 0xff},
            #else
            {0xff, 0x91, 0x5f, 0x86, 0x91, 0x8d, 0xdc, 0x27,    //Private key: Big Endian
            0x00, 0x5d, 0xf1, 0xd6, 0xcf, 0x0c, 0x14, 0x2b,
            0x62, 0x5e, 0xd2, 0xef, 0xf4, 0xa5, 0x18, 0xff},
            #endif
        },
		{//MP_AfeInit
			1,
			{
			    {0xAC,0x00,0x01,0xFF,0xFF},  //workaround for issue 5039
			}
		},
		{//MP_SfrInit
			0,
			{
			    {0x00,0x00,0x00},
			}
		},
		{//MP_RfMiscCtl
			{//mp_bt_tx_pwr_cfg
            #ifdef OPERATION_IN_BB_ASIC_VERSION
                #if (defined OPERATION_IN_RF_AB1520S)
                0x85,		//- tx_default_gfsk_gc1
                0x78,		//- tx_default_gfsk_gc2
                0x09,		//- tx_default_edr_gc1
                0x73,		//- tx_default_edr_gc2
                49,             //- tx_gfsk_max_gain
                38,             //- tx_edr_max_gain
                #else
                    #ifdef AB1122
                    0x85,	//- tx_default_gfsk_gc1
                    0x78,	//- tx_default_gfsk_gc2
                    0x0D,	//- tx_default_edr_gc1
                    0x73,	//- tx_default_edr_gc2
                    49,                 //- tx_gfsk_max_gain
                    38,                 //- tx_edr_max_gain
                    #else
                        #if (defined AB1520C)
                            #ifdef AB1528C_ONLY
                                #ifdef BLUETOOTH_SPEAKER
                                0x8D,	//-tx_default_gfsk_gc1
                                0x79,	//- tx_default_gfsk_gc2
                                0x8D,	//- tx_default_edr_gc1
                                0x77,	//- tx_default_edr_gc2
                                51,     //- tx_gfsk_max_gain
                                47,     //- tx_edr_max_gain
                                #else
                                0x0D,	//-tx_default_gfsk_gc1
                                0x79,	//- tx_default_gfsk_gc2
                                0x8D,	//- tx_default_edr_gc1
                                0x77,	//- tx_default_edr_gc2
                                50,     //- tx_gfsk_max_gain
                                47,     //- tx_edr_max_gain
                                #endif
                            #else
                                #ifdef BLUETOOTH_SPEAKER
                                0x85,	//- tx_default_gfsk_gc1
                                0x78,	//- tx_default_gfsk_gc2
                                0x0D,	//- tx_default_edr_gc1
                                0x73,	//- tx_default_edr_gc2
                                49,     //- tx_gfsk_max_gain
                                38,     //- tx_edr_max_gain
                                #else
                                0x0D,	//-tx_default_gfsk_gc1
                                0x79,	//- tx_default_gfsk_gc2
                                0x8D,	//- tx_default_edr_gc1
                                0x77,	//- tx_default_edr_gc2
                                50,     //- tx_gfsk_max_gain
                                47,     //- tx_edr_max_gain
                                #endif
                            #endif
                        #elif (defined AB1520D)
                            #ifdef AB1528C_ONLY
                                #ifdef BLUETOOTH_SPEAKER
                                0x8D,	//-tx_default_gfsk_gc1
                                0x79,	//- tx_default_gfsk_gc2
                                0x8D,	//- tx_default_edr_gc1
                                0x77,	//- tx_default_edr_gc2
                                51,     //- tx_gfsk_max_gain
                                47,     //- tx_edr_max_gain
                                #else
                                0x0D,	//-tx_default_gfsk_gc1
                                0x99,	//- tx_default_gfsk_gc2
                                0x0D,	//- tx_default_edr_gc1
                                0x99,	//- tx_default_edr_gc2
                                50,     //- tx_gfsk_max_gain
                                50,     //- tx_edr_max_gain
                                #endif
                            #else
                                #ifdef BLUETOOTH_SPEAKER
                                0x85,	//- tx_default_gfsk_gc1
                                0x78,	//- tx_default_gfsk_gc2
                                0x0D,	//- tx_default_edr_gc1
                                0x73,	//- tx_default_edr_gc2
                                49,     //- tx_gfsk_max_gain
                                38,     //- tx_edr_max_gain
                                #else
                                0x0D,	//-tx_default_gfsk_gc1
                                0x99,	//- tx_default_gfsk_gc2
                                0x0D,	//- tx_default_edr_gc1
                                0x99,	//- tx_default_edr_gc2
                                50,     //- tx_gfsk_max_gain
                                50,     //- tx_edr_max_gain
                                #endif
                            #endif
                        #endif //AB1520C
                    #endif //AB1122
                #endif //AB1520S
            #else
            0x01,               //- tx_default_gfsk_gc1
            0x7C,               //- tx_default_gfsk_gc2
            0x01,               //- tx_default_edr_gc1
            0x78,               //- tx_default_edr_gc2
            56,                 //- tx_gfsk_max_gain
            48,                 //- tx_edr_max_gain
            #endif
			},
			{//mp_mod_gain_comp_cfg
			#ifdef OPERATION_IN_BB_ASIC_VERSION
				#if (defined OPERATION_IN_RF_AB1520S)
				//{671, 704, 739, 781, 825, 866, 898}, //1.35V
				{671, 699, 728, 772, 816, 852, 877}, //1.25V
				{0xFE, 0xFC, 0xFB, 0xFB, 0x00, 0x00, 0x00, 0x04}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
				{0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06}, // New EDR (signed) delta of txagc_gc b/w EDR d4 value
				#else
					#ifdef AB1122
					{671, 699, 728, 772, 816, 852, 877}, //1.25V
					{0xFF, 0xFE, 0xFE, 0xFE, 0x00, 0x02, 0x04, 0x07}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
					{0x08, 0x05, 0x02, 0xFE, 0x00, 0x03, 0x05, 0x07}, // New EDR (signed) delta of txagc_gc b/w EDR d4 value
					#else
                        #if (defined AB1520C)
    						#ifdef AB1528C_ONLY
    							#ifdef BLUETOOTH_SPEAKER
    							{671, 699, 728, 772, 816, 852, 877}, //1.25V
    							{0x04, 0x00, 0x01, 0xFE, 0x00, 0x02, 0x04, 0x05}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
    							{0x07, 0x01, 0x02, 0xFE, 0x00, 0x02, 0x04, 0x05}, // New EDR (signed) delta of txagc_gc b/w EDR d4 value
    							#else
    							{672, 672, 737, 776, 830, 844, 844}, //1.2V
    							{0x06, 0x03, 0x00, 0xFD, 0x00, 0x03, 0x06, 0x0A}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
    							{0x07, 0x05, 0x02, 0xFE, 0x00, 0x03, 0x05, 0x0A}, // New EDR (signed) delta of txagc_gc b/w EDR d4 value
    							#endif
    						#else
    							#ifdef BLUETOOTH_SPEAKER
    							{671, 699, 728, 772, 816, 852, 877}, //1.25V
    							{0xFF, 0xFE, 0xFE, 0xFE, 0x00, 0x02, 0x04, 0x07}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
    							{0x08, 0x05, 0x02, 0xFE, 0x00, 0x03, 0x05, 0x07}, // New EDR (signed) delta of txagc_gc b/w EDR d4 v
    							#else
    							{672, 672, 737, 776, 830, 844, 844}, //1.2V
                  {0x06, 0x03, 0x00, 0xFD, 0x00, 0x03, 0x06, 0x0A}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
                  {0x07, 0x05, 0x02, 0xFE, 0x00, 0x03, 0x05, 0x0A}, // New EDR (signed) delta of txagc_gc b/w EDR d4 value
    							#endif
    						#endif
                        #elif (defined AB1520D)
    						#ifdef AB1528C_ONLY
    							#ifdef BLUETOOTH_SPEAKER
    							{671, 699, 728, 772, 816, 852, 877}, //1.25V
    							{0x04, 0x00, 0x01, 0xFE, 0x00, 0x02, 0x04, 0x05}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
    							{0x07, 0x01, 0x02, 0xFE, 0x00, 0x02, 0x04, 0x05}, // New EDR (signed) delta of txagc_gc b/w EDR d4 value
    							#else
    							{641, 641, 708, 754, 809, 842, 842}, //1.2V
    							{0x07, 0x07, 0x05, 0x01, 0x00, 0x05, 0x08, 0x08}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
    							{0x07, 0x07, 0x05, 0x01, 0x00, 0x05, 0x0B, 0x0B},  // New EDR (signed) delta of txagc_gc b/w EDR d4 value
    							#endif
    						#else
    							#ifdef BLUETOOTH_SPEAKER
    							{671, 699, 728, 772, 816, 852, 877}, //1.25V
    							{0xFF, 0xFE, 0xFE, 0xFE, 0x00, 0x02, 0x04, 0x07}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
    							{0x08, 0x05, 0x02, 0xFE, 0x00, 0x03, 0x05, 0x07}, // New EDR (signed) delta of txagc_gc b/w EDR d4 v
    							#else
    						  {641, 641, 708, 754, 809, 842, 842}, //1.2V
    							{0x07, 0x07, 0x05, 0x01, 0x00, 0x05, 0x08, 0x08}, // New BDR (signed) delta of txagc_gc b/w BDR d4 value
    							{0x07, 0x07, 0x05, 0x01, 0x00, 0x05, 0x0B, 0x0B},  // New EDR (signed) delta of txagc_gc b/w EDR d4 value
    							#endif
    						#endif
                        #endif //1520C
					#endif //1122
				#endif //1520S
			#else
				{750, 770, 810, 850, 880, 910, 925},  // tx_mod_temp_step
				{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // BDR (signed) delta of txagc_gc b/w BDR d4 value
				{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // EDR (signed) delta of txagc_gc b/w EDR d4 value
			#endif //ASIC_VERSION
			},
		},
		{//MP_BatteryPara : Low Battery Threshold 1(U16), Low Battery Threshold 2(U16)
			540, //initBatVolt
			#ifdef OPERATION_IN_BB_FPGA_VERSION
			717, 654,			//lowBatVolt, shutdownBatVolt
			724, 776, 828,		//Checkpoints
			#else
			580, 500,			//lowBatVolt, shutdownBatVolt
			724, 776, 828,		//Checkpoints
			#endif

			900, //S16 chargingRiskHighBound;
			700, //S16 chargingSecureHighBound;
			600, //S16 chargingRechargeHighBound;
			500, //S16 chargingRechargeLowBound;
			400, //S16 chargingSecureLowBound;
			300, //S16 chargingRiskLowBound;

			3800,//873, //s16 dischargingRiskHighBound;
			-3800, //s16 dischargingRiskLowBound;

			#ifdef OPERATION_IN_BB_FPGA_VERSION
			{677, 700, 723, 746, 769, 792, 815, 838, 884},
			#else
			{523, 546, 569, 592, 615, 638, 661, 684, 707},
			#endif
		},
		{  //MP_ChargerDiscountCtl
 			{0x72F6, 0x430C, 0x330C, },		//normalChargerDacCurrent, normalCCAdcCurrent, normalCCAdcCurrent2
			{0x72F6, 0x430C, 0x330C, },		//abnormalChargerDacCurrent, abnormalCCAdcCurrent, abnormalCCAdcCurrent2
			{0x0000, 0x0000, 0x0000, }, 		//stopChargerDacCurrent, stopCCAdcCurrent, stopCCAdcCurrent2
		},
		{//MP_AdcField
			0,
		},
		{//CalibrationInfo
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		},
		{//ManufacturingInfo
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		},

	},

	//- CRC
	{0,0}
};
