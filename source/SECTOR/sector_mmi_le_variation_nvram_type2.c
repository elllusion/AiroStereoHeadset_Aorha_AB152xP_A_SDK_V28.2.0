//////////////////////////////////////////////////////////
// Copyright@ 2011 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////
#include <config.h>
#include <cpu.h>

#include "bt_config_profile.h"
#include "sector_mmi_le_variation_nvram.h"

//#ifdef LE_SUPPORTED_HOST_FLASH

#pragma userclass (HCONST = LE_V_NVRAM2)


PUBLIC NVRAM_MMI_LINK_RECORD_STRU CODE gMMI_LE_v_nvram_init2 =
{
    {
    	{        
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    #ifdef AB1600
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0},
    	    {{0}, 0, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0, 0}
    	    #endif
    	},
    	0,		// U8 nextRecordIdx;

        /* advData */
        {
#if defined(SUPPORT_LE_XPORT)
            0x0D,                                               //adv_data_Len;

            {   //adv_data
                0x02,                                           //flags Len;
                0x01, 0x05,                                     //flags: limited discoverable ; doesn't support BR/EDR

                0x09,                                           //localName Len;
                0x09, 'D', 'u', 'a', 'l', '-', 'B', 'L', 'E',   //localNameData

                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
            },
#elif defined (SUPPORT_LE_PROFILE_HOGP)
            0x16,                                                   //adv_data_Len;

            {   //adv_data
                0x0A,                                               //localName Len;
                0x09, 'A', 'i', 'r', 'o', 'h', 'a', 'B', 'L', 'E',  //localNameData

                0x03,                                               //appearance Len;
                0x19, 0xC2, 0x03,                                   //appearance: 03 C2 = mouse, 03 C0 = Generic HID

                0x02,                                               //flags Len;
                0x01, 0x05,                                         //flags: general discoverable ; support BR/EDR controller/host

                0x03,                                               //sUUID Len;
                0x03, 0x12, 0x18,                                   //sUUID

                0, 0, 0, 0, 0, 0, 0, 0, 0,
            },
#else
            0x1C,                                                   //adv_data_Len;

            {   //adv_data
                0x02,                                               //flags Len;
                0x01, 0x1A,                                         //flags: general discoverable ; support BR/EDR controller/host

                0x0A,                                               //localName Len;
                0x09, 'A', 'i', 'r', 'o', 'h', 'a', 'B', 'L', 'E',  //localNameData

                0x05,                                               //sInterval Len;
                0x12, 0xFF, 0xFF, 0xFF, 0xFF,                       //sInterval: Min(2bytes), Max(2bytes)

                0x03,                                               //sUUID Len;
                0x03, 0x12, 0x18,                                   //sUUID

                0x03,                                               //appearance Len;
                0x19, 0xC0, 0x03,                                   //appearance: 03 C2 = mouse, 03 C0 = Generic HID

                0, 0, 0,
            },
#endif            
        },

        /* advPara */
        {
			#if defined(SUPPORT_LE_XPORT)
        	0x0801,									//U16 advIntMin 165ms
        	0x1001,                                 //U16 advIntMax 170ms
			#elif defined (SUPPORT_LE_PROFILE_HOGP)
        	0x2000,									//U16 advIntMin 20ms
        	0x3000,									//U16 advIntMax 30ms
			#else
        	0x4006,                                 //U16 advIntMin 20ms
        	0x0008,                                 //U16 advIntMax 30ms
			#endif

            #ifdef MMI_LE_ENABLE
            0,                                      //U8 advType 0:MMI_LE_ADV_TYPE_ADV_IND
            #else
        	0x02,                                   //U8 advType 0x02:MMI_LE_ADV_TYPE_ADV_SCAN_IND
            #endif

            #ifdef SUPPORT_STATIC_RANDOM_ADDRESS
        	0x01,                                   //U8 ownAddrType
        	#else
        	0x00,                                   //U8 ownAddrType
        	#endif

        	0x00,                                   //U8 peerAddrType
        	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   //U8 peerAddr[6]	direct_addr, don't care when undirected advertising
        	0x07,                                   //U8 channelMap
        	0x00,                                   //U8 filterPolicy   
            
        },

        /* connSetting */
        #if defined(SUPPORT_LE_XPORT)
    	{
            0x0800,                                 //U16 intMin
            0x9001,                                 //U16 intMax
            0x0200,                                 //U16 slaveLatency
            0x0002,                                 //U16 timeout
    	},
    	#elif defined (SUPPORT_LE_PROFILE_HOGP)
    	{
            0x0800,
            0x0800,
            0x0000,
            0x2C01,
    	},
    	#else
    	{
            0x6800,
            0x7800,
            0x0000,
            0x0002,
    	},
        #endif

        /* scanRsp */
        {
            0x03,                                   //scan_rsp_data_len

            {   //scan_rsp_data
                0x02, 0x0A, 0x00,                   //TX power = 0dbm (temporarily)
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            }
        },

		#ifdef SUPPORT_LE_XPORT
        30,                                         //U8 advTimout
		#else
		0,                                          //U8 advTimout
		#endif

        0x4006,                                     //U16 rPowerAdvIntMin
        0x0008,                                     //U16 rPowerAdvIntMax
        //U8 vibrationSetting[6];
        {
            0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00,
        },
        //U8 leFeatureCtl;
    	{
    		0x00,
    	},
    },

	{0,0} 	// CRC16 crc;
};

//#endif /* LE_SUPPORTED_HOST_FLASH */
