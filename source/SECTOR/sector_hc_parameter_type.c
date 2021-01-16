//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#include "os.h"
#include "bt_config_profile.h"
#include "sector_hc_parameter.h"

#pragma userclass (HCONST = CONFIG)


SECTOR_HC_PARAMETER_STRU CODE gHC_parameter_init =
{

    0x00,   //  U8  scattrnet_mode      /* Deprecated */
    0x00,   //  U8  scan_enable;        0: inquiry scan 1: page scan
    0x00,   //  U8  sac_scan_enable;    /* Deprecated */

#ifdef HEADSET_SOC
    1,      //  U8  inquiry_scan_type;                  1 : interlace
#else
    1,      //  U8  inquiry_scan_type;                  1 : interlace
#endif
    2,      //  U8  inquiry_mode;// 1 means RSSI inquiry
#ifdef HEADSET_SOC
    1,      //  U8  page_scan_type;                     1 : interlace
#else
    #ifdef HID_MOUSE_IOT
    0,      //  U8  page_scan_type;                     1 : interlace
    #else
    1,      //  U8  page_scan_type;                     1 : interlace
    #endif
#endif

    1,      //  U8  SR,
    0,      //  U8  PIN_type;
    0,      //  U8  authentication_enable;
    0,      //  U8  encryption_mode;
    0,      //  U8  hold_mode_activity;
    0,      //  U8  num_broadcast_retransmissions;
    1,      //  U8  AFH_channel_assessment_mode;
    0,      //  U8  SSP mode
    0,      //  U8  SSP debug mode
    #ifdef  HID_SOC
    0,
    #else
    0,      //  U8  mmi1_interval
    #endif
    1,      //  U8  number_of_support_iac
    0,      //  U8  current_iac_used_idx
    {
        {0x33, 0x8B, 0x9E, 0x00, 0x00, 0x00, 0xE2, 0x3A, 0x1A, 0x33, 0x02}, //  giac;
        {0x00, 0x8B, 0x9E, 0x00, 0x00, 0x00, 0x48, 0xC3, 0x45, 0xEB, 0x01}, //  liac;
    },
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F},   //  U8  AFH_host_channel_classification[10];
#ifdef HID_IIM_SOC
    0x0400, //  U16 inquiry_scan_interval;
    0x00D0, //  U16 inquiry_scan_window;
    //0x2000,   //  U16 page_timeout; //2.56s
    0x1000,
    0x1F40, //  U16 connection_accept_timeout;
    0x1F40, //  U16 read_logical_link_accept_timeout;
    0x0400, //  U16 page_scan_interval;
    0x0200, //  U16 page_scan_window;
#else
    #ifdef HEADSET_SOC
    #ifdef ASIC_RELEASE
    0x00A0, //  U16 inquiry_scan_interval;
    #else
    0x0800, //  U16 inquiry_scan_interval;
    #endif
    0x0012, //  U16 inquiry_scan_window;
    #ifdef PRODUCT_TYPE_K
    0x1800, //  U16 page_timeout;
    #else
    0x3000, //  U16 page_timeout;
    #endif
    0x1F40, //  U16 connection_accept_timeout;
    0x1F40, //  U16 read_logical_link_accept_timeout;
    0x0800, //  U16 page_scan_interval;
    0x0012, //  U16 page_scan_window;
    #else
    0x0800, //  U16 inquiry_scan_interval;
    0x0012, //  U16 inquiry_scan_window;
    0x3000, //  U16 page_timeout;
    0x1F40, //  U16 connection_accept_timeout;
    0x1F40, //  U16 read_logical_link_accept_timeout;
    0x0800, //  U16 page_scan_interval;
    0x0012, //  U16 page_scan_window;
    #endif
#endif

#ifdef  HID_SOC
    0x0000, //- U16 sac_scan_interval_in_2slots;
#else
    0x0400, //- U16 sac_scan_interval_in_2slots;
#endif
    0x006C, //  U16 voice_setting;
    0x0007, //  U16 default_link_policy_setting;
    0x01,   //  U8 current_num_of_stored_link_key;
    {
        0x03,   //max_retry_num;
        0x01,   //waiting_interval; //100ms
    },
    {
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#if MAX_NUM_ACL_LINK > 1
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#endif
#if MAX_NUM_ACL_LINK > 2
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#endif
#if MAX_NUM_ACL_LINK > 3
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#endif
#if MAX_NUM_ACL_LINK > 4
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#endif
#if MAX_NUM_ACL_LINK > 5
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#endif
#if MAX_NUM_ACL_LINK > 6
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#endif
#if MAX_NUM_ACL_LINK > 7
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#endif
#if MAX_NUM_ACL_LINK > 8
        {
            0x0000, //  U16 flush_timeout;
            0x0000, //  U16 link_policy_setting;
            0x0000  //  U16 failed_contact_counter;
        },
#endif
#if MAX_NUM_ACL_LINK > 9
#error MAX_NUM_ACL_LINK > 9
#endif
    },//HC_LINK_INFO_TYPE   link_info[MAX_NUM_ACL_LINK];
    96000, /*30s*/ // timer interval of bb for page scan enabled mode, detach if timer expired
    192000, /*60s*/// timer interval of bb for discoveriable mode, detach if timer expired

    HC_MISC_SUPP_FEA0_AUTO_CONN_ACP             |
//- HC_MISC_SUPP_FEA0_USB_HUB_SUPPORT           |
//- HC_MISC_SUPP_FEA0_RESET_SOFT                |
    HC_MISC_SUPP_FEA0_RESET_HARD                |
    0x00,
    #ifdef REMOTE_EEPROM_UPDATE
    HC_MISC_SUPP_FEA1_REMOTE_EEPROM_UPDATE      |
    #endif
	#ifdef TRSPX_App
    HC_MISC_SUPP_FEA1_DISABLE_LIGHT_DBGPRINT |
	#elif defined AIR_UPDATE_DONGLE
	HC_MISC_SUPP_FEA1_DISABLE_LIGHT_DBGPRINT |
	#elif defined KB_App
	HC_MISC_SUPP_FEA1_DISABLE_LIGHT_DBGPRINT |
	#else
	//HC_MISC_SUPP_FEA1_DISABLE_LIGHT_DBGPRINT |
	//HC_MISC_SUPP_FEA1_SPI_LIGHT_DBGPRINT |
	#endif
    0x00,

    //- EnableDUT,
    0,

	{
		{
			1,
			{
				{0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0},
			}
		},
		{
			0,
			{
				{0, 0x01, 0, 0, 0, 0, 0, 0},
				{0, 0x01, 0, 0, 0, 0, 0, 0},
			}
		}
	},

	4,
	{
		0x00000000,
		0x00000000,
		0x00000000,
	},
    //- CRC
    {0,0}
};
