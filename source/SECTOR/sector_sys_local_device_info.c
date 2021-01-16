/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sys_local_device_info.c defines the local device informations.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "rc.h"
#include "bt_config_profile.h"
#include "sector_sys_local_device_info.h"

#pragma userclass (HCONST = CONFIG)


//#define BT_VERSION_2_0

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_SYS_LOCAL_DEVICE_INFO_STRU CODE gSector_SysLocalDeviceInfo =
{
    {
        AB1520_SERIES | 0x01,       //- AB1520

        //- U8 class_of_device[3];
        #if defined(HID_IIM_SOC)
        {0x04 , 0x25, 0x00},
        #elif defined (APP_KEYBOARD)
        {0x40 , 0x25, 0x00},
        #elif defined(HID_SOC)
        {0x80 , 0x25, 0x00},
        #else
			#ifdef A2DP_Profile
            {0x04, 0x04, 0x24}, //A2DP, Little endia
            #else
            {0x04, 0x04, 0x20}, //Mono-Headset
            #endif
        #endif

        //- HciVersion
        0x20,

        //- HciRevision
        0x00, 0x00,

        //- LmpVersion
        #if (LM_LE_VERSION)
        0x09,//5.0      //0x08,//4.2
        #elif (LM_BR_EDR_VERSION)
        0x09,//5.0      //0x08,//4.2
        #endif

        //- LmpCompId
        0x94, 0x00,

        //- LmpSubVersNr
        0x32, 0x32,

        //- LmpFeatures
		#if defined(AIR_UPDATE_DONGLE)
		{
            0xFB, 0xFE, 0x85, 0xFA, 0xD8, 0xBF, 0x73, 0x83,
			0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },
		#elif (LM_DUAL_VERSION)
		{
            #ifdef CELT_MODULE
            0xFF, 0xFE, 0x85, 0xFE, 0xDA, 0xBF, 0x7B, 0x83,
            #else
            0xFF, 0xFE, 0x8D, 0xFE, 0xD8, 0xBF, 0x7B, 0x83,
            #endif
            #ifdef KB_App
            0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            #else
            0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            #endif
        },
		#elif (LM_BR_EDR_VERSION)
		{
            #ifdef CELT_MODULE
            0xFF, 0xFE, 0x85, 0xFE, 0x9A, 0xBF, 0x79, 0x83,
            #else
            0xFF, 0xFE, 0x8D, 0xFE, 0x98, 0xBF, 0x79, 0x83,
            #endif
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },
        #elif (LM_LE_VERSION)
        {
            0xfc, 0x02, 0x04, 0x38, 0x78, 0x1A, 0x19, 0x83,
            0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },
        #endif

        {
            {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   //LE_feature, LE encryption
        },
        #if 1
        {
            {0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   //LE_state
        },
        #endif
        {
            {0x06, 0x94, 0x00, 0x32, 0x22}, //LE_version
        },

        1,   //currentMaxPage

		#ifndef AB1520S //AB1520S do not support OTA update
		#ifdef ASIC_DBG_PORT
		0x00, //bit0:EnableExtFlashOtaUpdate
		#else
        0x00, //bit0:EnableExtFlashOtaUpdate
		#endif
		#endif
    },

    //- CRC
    {0,0}
};