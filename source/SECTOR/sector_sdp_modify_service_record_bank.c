#include "config.h"
#include "cpu.h"
#include "bt_config_profile.h"

#include "sector_sdp_modify_service_record_bank.h"

#pragma userclass (HCONST = CONFIG)


#if defined(HID_SOC)

#ifdef HID_IIM_SOC
SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	24,//service_name_len
	{0x09, 0x01, 0x00, 0x25, 0x13, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f, 0x20, 0x52, 0x56, 0x4c, 0x2d,
	                               0x43, 0x4e, 0x54, 0x2d, 0x30, 0x31},//Service Name "Nintendo RVL-CNT-01"
	24,//service_descript_len
	{0x09, 0x01, 0x01, 0x25, 0x13, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f, 0x20, 0x52, 0x56, 0x4c, 0x2d,
	                               0x43, 0x4e, 0x54, 0x2d, 0x30, 0x31},//"Nintendo RVL-CNT-01"
	13,//provider_name_len
	{0x09, 0x01, 0x02, 0x25, 0x08, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f}, //"Nintendo//provider_name
	{0x09, 0x02, 0x02, 0x08, 0x04},//HID Device Subclass
	{0x09, 0x02, 0x03, 0x08, 0x33},//HID Country code
	{0x09, 0x02, 0x04, 0x28, 0x00},//HID Virtual Cable
	228,//report_descriptor_len
	{0x09, 0x02, 0x0e, 0x28, 0x00},//HID Boot Device
	0x01,//reconnect_initial
	0x00,//normal_connectable
	0x0C80,//supervision_timeout
	0x0640,//ssrhostmaxlatency
	0x0320,//ssrhostmintimeout
	{0x09, 0x02, 0x01, 0x09, 0x05, 0x7e},//Vendor ID
	{0x09, 0x02, 0x02, 0x09, 0x03, 0x06},//Product ID
	{0x09, 0x02, 0x03, 0x09, 0x3a, 0x1c},//Product Version
	0,//num_of_records
	{ 25 , 11 , 7 },//total_attribute_num
	0,//record_base_num
	0,//attribute_data_base_num
	0,//attribute_id16
	0, //attribute_len
	{0x09, 0x02, 0x06, 0x35, 0xDF, 0x35, 0xDD, 0x08, 0x22,//Report descriptor wii
	 0x25, 0xD9,
	 0x05, 0x01,         //# Usage Page (Generic Desktop)
     0x09, 0x05,         //# Usage (Gamepad)
     0xa1, 0x01,               //# Collection (Physical)
     0x85, 0x10,         //# Report ID=10
     0x15, 0x00,         //# Logical min=0
     0x26, 0xff, 0x00,    //  # Logical max=ff
     0x75, 0x08,         //# Report size=8
     0x95, 0x01,         //# Report count=1
     0x06, 0x00, 0xff,   //# Usage page ff00?
     0x09, 0x01,         //# Usage (Pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x11,         //# Report ID=11
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x12,         //# Report ID=12
     0x95, 0x02,          //# Report count=2
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x13,         //# Report ID=13
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x14,         //# Report ID=14
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x15,         //# Report ID=15
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x16,         //# Report ID=16
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x17,         //# Report ID=17
     0x95, 0x06,         //# Report count=6
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x18,         //# Report ID=18
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x19,         //# Report ID=19
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x1a,         //# Report ID=1a
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x20,         //# Report ID=20
     0x95, 0x06,         //# Report count=6
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x21,         //# Report ID=21
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x22,         //# Report ID=22
     0x95, 0x04,         //# Report count=4
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x30,         //# Report ID=30
     0x95, 0x02,         //# Report count=2
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x31,         //# Report ID=31
     0x95, 0x05,         //# Report count=5
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x32,         //# Report ID=32
     0x95, 0x0a,         //# Report count=10
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x33,         //# Report ID=33
     0x95, 0x11,         //# Report count=11
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x34,         //# Report ID=34
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x35,         //# Report ID=35
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x36,         //# Report ID=36
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x37,         //# Report ID=37
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x3d,         //# Report ID=3d
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x3e,         //# Report ID=3e
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x3f,         //# Report ID=3f
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0xc0            //# (End collection)
	 },
	 {0}//record_data_bank
};

#elif defined (APP_GAMEPAD)
SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	24,//service_name_len
	{0x09, 0x01, 0x00, 0x25, 0x13, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f, 0x20, 0x52, 0x56, 0x4c, 0x2d,
	                               0x43, 0x4e, 0x54, 0x2d, 0x30, 0x31},//Service Name "Nintendo RVL-CNT-01"
	24,//service_descript_len
	{0x09, 0x01, 0x01, 0x25, 0x13, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f, 0x20, 0x52, 0x56, 0x4c, 0x2d,
	                               0x43, 0x4e, 0x54, 0x2d, 0x30, 0x31},//"Nintendo RVL-CNT-01"
	13,//provider_name_len
	{0x09, 0x01, 0x02, 0x25, 0x08, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f}, //"Nintendo//provider_name
	{0x09, 0x02, 0x02, 0x08, 0x04},//HID Device Subclass
	{0x09, 0x02, 0x03, 0x08, 0x33},//HID Country code
	{0x09, 0x02, 0x04, 0x28, 0x00},//HID Virtual Cable
	237,//report_descriptor_len
	{0x09, 0x02, 0x0e, 0x28, 0x00},//HID Boot Device
	0x01,//reconnect_initial
	0x00,//normal_connectable
	0x1F40,//supervision_timeout
	0x0640,//ssrhostmaxlatency
	0x0320,//ssrhostmintimeout
	{0x09, 0x02, 0x01, 0x09, 0x05, 0x7e},//Vendor ID
	{0x09, 0x02, 0x02, 0x09, 0x03, 0x06},//Product ID
	{0x09, 0x02, 0x03, 0x09, 0x3a, 0x1c},//Product Version
	0,//num_of_records
	{ 25 , 11 , 7 },//total_attribute_num
	0,//record_base_num
	0,//attribute_data_base_num
	0,//attribute_id16
	0, //attribute_len
	{0x09, 0x02, 0x06, 0x35, 0xE8, 0x35, 0xE6, 0x08, 0x22,//Report descriptor Game Pad
	 0x25, 0xE2,
	 0x05, 0x01,//#	Usage Page (Generic Desktop),### MOUSE
	 0x09, 0x02,//#	Usage (Mouse),
	 0xA1, 0x01,//#	Collection: (Application),
	 0x85, 0x02,//#		REPORT_ID (2)
	 0x09, 0x01,//#		Usage (Pointer),
     0xA1, 0x00,//#	    Collection: (Linked),
	 0x05, 0x09,//#		    Usage Page (Buttons),
	 0x19, 0x01,//#		    Usage Minimum (01),
	 0x29, 0x08,//#		    Usage Maximum (08),
	 0x15, 0x00,//#		    Log Min (0),
	 0x25, 0x01,//#		    Log Max (1),
	 0x75, 0x01,//#		    Report Size (1),
	 0x95, 0x08,//#		    Report Count (8),
	 0x81, 0x02,//#		    Input (Data, Variable, Absolute),
	 //0x75, 0x05,//#		    Report Size (5),
	 //0x95, 0x01,//#		    Report Count (1),
	 //0x81, 0x01,//#		    Input (Constant),
     0x05, 0x01,//#		    Usage Page (Generic Desktop),
	 0x09, 0x30,//#		    Usage (X),
	 0x09, 0x31,//#		    Usage (Y),
	 //0x09, 0x38,//#		    Usage (Wheel),
	 //0x15, 0x81,//#		    Logical min (-127),
	 //0x25, 0x7F,//#		    Logical Max (127),
	 0x16, 0x01, 0x80,//Local MIN
	 0x26, 0xff, 0x7f,//# Logical max
	 0x75, 0x10,//#		    Report Size (16),
	 0x95, 0x02,//#		    Report Count (2) (X,Y,Wheel)
	 0x81, 0x06,//#		    Input (Data, Variable, Relative),

	 0x09, 0x38,//# Usage Wheel
	 0x15, 0x81,//# Local MIN
	 0x25, 0x7f,//# Logical max
	 0x75, 0x08,//Report Size 8
	 0x95, 0x01,//Report Count 1
	 0x81, 0x06,//Input

	 0x05, 0x0C,//#       Usage Page (Consumer)				05 0C
	 0x0A, 0x38, 0x02,//#       Usage (Reserved)				0A 38 02
	 0x95, 0x01,//#       Report Count (1)					95 01
	 0x75, 0x08,//#		    Report Size (8),
	 0x81, 0x06,//#       Input (Data, Variable, Relative)			81 06
	 0xC0,//#		  END_COLLECTION (Logical)
	 0xC0,//#	END_COLLECTION
	 0x05, 0x01,//# USAGE PAGE (Generic Desktop)	### GAMEPAD
     0x09, 0x05,//# USAGE (Gamepad)
     0xA1, 0x01,//# COLLECTION (Application)
     0x85, 0x07,//# REPORT ID (7)
     0x75, 0x08,//# REPORT SIZE (8)
     0x95, 0x04,//# REPORT COUNT (4)
     0x15, 0x00,//# LOGICAL MINIMUM (0)
     0x26, 0xFF, 0x00,//# LOGICAL MAXIMUM (255)
     0x35, 0x00,//# PHYSICAL MINIMUM (0)
     0x46, 0xFF, 0x00,//# PHYSICAL MAXIMUM (255)
     0x09, 0x30,//# USAGE (X)                    ;X-axis on left joystick
     0x09, 0x31,//# USAGE (Y)                    ;Y-axis on left joystick
     0x09, 0x32,//# USAGE (Z)                    ;X-axis on right joystick
     0x09, 0x35,//# USAGE (Rz)                  ;Y-axis on left joystick
     0x81, 0x02,//# INPUT (Data, Var, Abs)
     0x75, 0x04,//# REPORT SIZE (4)
     0x95, 0x01,//# REPORT COUNT (1)
     0x25, 0x07,//# LOGICAL MAXIMUM (7)
     0x46, 0x3B, 0x01,//# PHYSICAL MAXIMUM (315)
     0x66, 0x14, 0x00,//# UNIT (Rotation? Degrees?)
     0x09, 0x39,//# USAGE (Hat switch)
     0x81, 0x42,//# INPUT (Data,Var,Abs,Null)
     0x66, 0x00, 0x00,//# UNIT (Reset to "No unit"?)
     0x75, 0x01,//# REPORT SIZE (1)
     0x95, 0x0C,//# REPORT COUNT (12)
     0x25, 0x01,//# LOGICAL MAXIMUM (1)
     0x45, 0x01,//# PHYSICAL MAXIMUM (1)
     0x05, 0x09,//# USAGE PAGE (Buttons)
     0x19, 0x01,//# USAGE MINIMUM (1)
     0x29, 0x0C,//# USAGE MAXIMUM (12)
     0x81, 0x02,//# INPUT (Data, Var, Abs)        ; 12 buttons
	 0x06, 0x00, 0xFF,//# USAGE PAGE (Vendor Specific)
     0x95, 0x01,//# REPORT COUNT (1)
	 0x75, 0x02,//# REPORT SIZE (2)
	 0x25, 0x01,//# LOGICAL MAXIMUM (1)
	 0x45, 0x01,//# PHYSICAL MAXIMUM (1)
	 0x09, 0x00,//# USAGE (Undefined)
	 0x91, 0x02,//# OUTPUT (Data, Var, Abs)            ; Vibration bits
	 0x95, 0x01,//#   REPORT_COUNT (1)
	 0x75, 0x06,//#   REPORT_SIZE (6)
	 0x91, 0x03,//#   OUTPUT (Cnst,Var,Abs)
	 0xC0,//# END COLLECTION (Logical)
	 0x05,  0x01,//                   # USAGE_PAGE (Generic Desktop)  ### KEYBOARD
     0x09,  0x06,//                   # USAGE (Keyboard)
     0xA1,  0x01,//                   # COLLECTION (Application)
     0x85,  0x01,//                   #   REPORT_ID (1)
     0x75,  0x01,//                   #   REPORT_SIZE (1)
     0x95,  0x08,//                   #   REPORT_COUNT (8)
     0x05,  0x07,//                   #   USAGE_PAGE (Keyboard)
     0x19,  0xE0,//                   #   USAGE_MINIMUM (Keyboard LeftControl)
     0x29,  0xE7,//                   #   USAGE_MAXIMUM (Keyboard Right GUI)
     0x15,  0x00,//                   #   LOGICAL_MINIMUM (0)
     0x25,  0x01,//                   #   LOGICAL_MAXIMUM (1)
     0x81,  0x02,//                   #   INPUT (Data,Var,Abs)
     0x95,  0x01,//                   #   REPORT_COUNT (1)
     0x75,  0x08,//                   #   REPORT_SIZE (8)
     0x81,  0x03,//                   #   INPUT (Cnst,Var,Abs)
     0x95,  0x05,//                   #   REPORT_COUNT (5)
     0x75,  0x01,//                   #   REPORT_SIZE (1)
     0x05,  0x08,//                   #   USAGE_PAGE (LEDs)
     0x19,  0x01,//                   #   USAGE_MINIMUM (Num Lock)
     0x29,  0x05,//                   #   USAGE_MAXIMUM (Kana)
     0x91,  0x02,//                   #   OUTPUT (Data,Var,Abs)
     0x95,  0x01,//                   #   REPORT_COUNT (1)
     0x75,  0x03,//                   #   REPORT_SIZE (3)
     0x91,  0x03,//                   #   OUTPUT (Cnst,Var,Abs)
     0x95,  0x06,//                   #   REPORT_COUNT (6)
     0x75,  0x08,//                   #   REPORT_SIZE (8)
     0x15,  0x00,//                   #   LOGICAL_MINIMUM (0)
     0x26,  0xFF,  0x00,//                #   LOGICAL_MAXIMUM (255)
     0x05,  0x07,//                   #   USAGE_PAGE (Keyboard)
     0x19,  0x00,//                   #   USAGE_MINIMUM (Reserved (no event indicated))
     0x29,  0xFF,//                   #   USAGE_MAXIMUM (Reserved (no event indicated))
     0x81,  0x00,//                   #   INPUT (Data,Ary,Abs)
     0xC0,//                      # END_COLLECTION
	 },
	 {0}//record_data_bank
};
#elif defined (APP_KEYBOARD)


#elif defined (APP_MOUSE)
SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	39,//service_name_len
	{0x09, 0x01, 0x00, 0x25, 0x22, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68, 0x20, 0x56, 0x34, 0x37, 0x30,
	                               0x20, 0x43, 0x6f, 0x72, 0x64, 0x6c, 0x65, 0x73, 0x73, 0x20, 0x4c, 0x61, 0x73,
								   0x65, 0x72, 0x20, 0x4d, 0x6f, 0x75, 0x73, 0x65},//Service Name "logitch"
	20,//service_descript_len
	{0x09, 0x01, 0x01, 0x25, 0x0f, 0x42, 0x6c, 0x75, 0x65, 0x74, 0x6f, 0x6f, 0x74, 0x68, 0x20, 0x4d, 0x6f, 0x75,
	                               0x73, 0x65},//"BLuetooth mouse"
	13,//provider_name_len
	{0x09, 0x01, 0x02, 0x25, 0x08, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68}, //"logitch//provider_name
	{0x09, 0x02, 0x02, 0x08, 0x80},//HID Device Subclass
	{0x09, 0x02, 0x03, 0x08, 0x21},//HID Country code
	{0x09, 0x02, 0x04, 0x28, 0x01},//HID Virtual Cable
	190,//report_descriptor_len
	{0x09, 0x02, 0x0e, 0x28, 0x01},//HID Boot Device
	0x01,//reconnect_initial
	0x00,//normal_connectable
	0x1F40,//supervision_timeout
	0x0640,//ssrhostmaxlatency
	0x0320,//ssrhostmintimeout
	{0x09, 0x02, 0x01, 0x09, 0x04, 0x6d},//Vendor ID
	{0x09, 0x02, 0x02, 0x09, 0xb0, 0x09},//Product ID
	{0x09, 0x02, 0x03, 0x09, 0x03, 0x12},//Product Version
	0,//num_of_records
	{ 25 , 11 , 0 },//total_attribute_num
	0,//record_base_num
	0,//attribute_data_base_num
	0,//attribute_id16
	0, //attribute_len
	{0x09, 0x02, 0x06, 0x35, 0xB9, 0x35, 0xB7, 0x08, 0x22,//Report descriptor avago
	 0x25, 0xB3,
	 0x05, 0x01,//# Usage Page (Generic Desktop)
	 0x09, 0x02,//# Usage (Mouse)
	 0xa1, 0x01,//# # Collection (Application)
	 0x85, 0x02,//# Report ID=2
	 0x09, 0x01,//USAGE (pointer)
     0xa1, 0x00,//#Collection (Physical)
	 0x05, 0x09,//USAGE PAGE(Button)
	 0x19, 0x01,//USAGE MIN(Button)
	 0x29, 0x08,//USAGE MAX(Button)
	 0x15, 0x00,//# Logical min=0
	 0x25, 0x01,//# Logical max=1
	 0x75, 0x01,//Report Size 1
	 0x95, 0x08,//Report Count 8
	 0x81, 0x02,//Input //0x95, 0x01, 0x75, 0x05, 0x81, 0x03,
	 0x05, 0x01,//# Usage Page (Generic Desktop)
	 0x09, 0x30,//# Usage (X)
	 0x09, 0x31,//# Usage (Y)
	 0x16, 0x01, 0x80,//Local MIN
	 0x26, 0xff, 0x7f,//# Logical max
	 0x75, 0x10,//Report Size 16
	 0x95, 0x02,//Report Count 2
	 0x81, 0x06,//Input
	 0x09, 0x38,//# Usage Wheel
	 0x15, 0x81,//# Local MIN
	 0x25, 0x7f,//# Logical max
	 0x75, 0x08,//Report Size 8
	 0x95, 0x01,//Report Count 1
	 0x81, 0x06,//Input
	 0x05, 0x0C,//#Usage Page  (Consumer)
     0x0A, 0x38, 0x02,//#Usage (AC Pan)
     0x95, 0x01,//#Report Count (1)
     0x75, 0x08,//#Report Size (8)
     0x15, 0x81,//#Logical min (-127),
     0x25, 0x7F,//#Logical Max (127),
     0x81, 0x06,//#Input (Data, Variable, Relative)
	 0xc0,//# (End collection)
	 0xc0,//# (End collection)
	 0x06, 0x00, 0xff,//# Usage Page (Vendor Specific)
	 0x09, 0x01,//# Usage (01)
	 0xA1, 0x01,//# # Collection (Application)
	 0x85, 0x10,//# Report ID=16
	 0x75, 0x08,//Report Size 8
	 0x95, 0x06,//Report Count 6
	 0x15, 0x00,//# Local MIN
	 0x26, 0xff, 0x00, //# Local MAX
	 0x09, 0x01,//# Usage (01)
	 0x91, 0x00,//output
	 0xC0,//# (End collection)
     0x05,  0x01,//                   # USAGE_PAGE (Generic Desktop)
     0x09,  0x06,//                   # USAGE (Keyboard)
     0xA1,  0x01,//                   # COLLECTION (Application)
     0x85,  0x01,//                   #   REPORT_ID (1)
     0x75,  0x01,//                   #   REPORT_SIZE (1)
     0x95,  0x08,//                   #   REPORT_COUNT (8)
     0x05,  0x07,//                   #   USAGE_PAGE (Keyboard)
     0x19,  0xE0,//                   #   USAGE_MINIMUM (Keyboard LeftControl)
     0x29,  0xE7,//                   #   USAGE_MAXIMUM (Keyboard Right GUI)
     0x15,  0x00,//                   #   LOGICAL_MINIMUM (0)
     0x25,  0x01,//                   #   LOGICAL_MAXIMUM (1)
     0x81,  0x02,//                   #   INPUT (Data,Var,Abs)
     0x95,  0x01,//                   #   REPORT_COUNT (1)
     0x75,  0x08,//                   #   REPORT_SIZE (8)
     0x81,  0x03,//                   #   INPUT (Cnst,Var,Abs)
     0x95,  0x05,//                   #   REPORT_COUNT (5)
     0x75,  0x01,//                   #   REPORT_SIZE (1)
     0x05,  0x08,//                   #   USAGE_PAGE (LEDs)
     0x19,  0x01,//                   #   USAGE_MINIMUM (Num Lock)
     0x29,  0x05,//                   #   USAGE_MAXIMUM (Kana)
     0x91,  0x02,//                   #   OUTPUT (Data,Var,Abs)
     0x95,  0x01,//                   #   REPORT_COUNT (1)
     0x75,  0x03,//                   #   REPORT_SIZE (3)
     0x91,  0x03,//                   #   OUTPUT (Cnst,Var,Abs)
     0x95,  0x06,//                   #   REPORT_COUNT (6)
     0x75,  0x08,//                   #   REPORT_SIZE (8)
     0x15,  0x00,//                   #   LOGICAL_MINIMUM (0)
     0x26,  0xFF,  0x00,//                #   LOGICAL_MAXIMUM (255)
     0x05,  0x07,//                   #   USAGE_PAGE (Keyboard)
     0x19,  0x00,//                   #   USAGE_MINIMUM (Reserved (no event indicated))
     0x29,  0xFF,//                   #   USAGE_MAXIMUM (Reserved (no event indicated))
     0x81,  0x00,//                   #   INPUT (Data,Ary,Abs)
     0xC0,//                      # END_COLLECTION
	 0x05,  0xff,//		# Usage Page (generic desktop)
     0x09,  0x02,//		# Usage (Mouse)
     0xA1,	0x01,//
     0x85,	0x04,//		# REPORT_ID (4)
     0x75,	0x08,//
     0x95,	0x02,//		# Need 2 bytes
     0xB1,	0x03,//
     0xC0,//
	 },
	 {0}//record_data_bank
};

#elif defined (APP_PJ001_MOUSE)

SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	35,//service_name_len
	{0x09, 0x01, 0x00, 0x25, 0x1E, 'T', 'a', 'r', 'g', 'u', 's', ' ', 'O', 'p', 't', 'i', 'c', 'a', 'l',
									' ', 'B', 'T', ' ' , 'L', 'a', 'p', 't', 'o', 'p', ' ',  'M', 'o', 'u', 's', 'e'},//Service Name "logitch"
	20,//service_descript_len
	{0x09, 0x01, 0x01, 0x25, 0x0f, 0x42, 0x6c, 0x75, 0x65, 0x74, 0x6f, 0x6f, 0x74, 0x68, 0x20, 0x4d, 0x6f, 0x75,
	                               0x73, 0x65},//"BLuetooth mouse"
	13,//provider_name_len
	{0x09, 0x01, 0x02, 0x25, 0x08, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68}, //"logitch//provider_name
	{0x09, 0x02, 0x02, 0x08, 0x80},//HID Device Subclass
	{0x09, 0x02, 0x03, 0x08, 0x21},//HID Country code
	{0x09, 0x02, 0x04, 0x28, 0x01},//HID Virtual Cable
	109,//report_descriptor_len
	{0x09, 0x02, 0x0e, 0x28, 0x01},//HID Boot Device
	0x01,//reconnect_initial
	0x00,//normal_connectable
	0x1F40,//supervision_timeout
	0x0640,//ssrhostmaxlatency
	0x0320,//ssrhostmintimeout
	{0x09, 0x02, 0x01, 0x09, 0x04, 0x6d},//Vendor ID
	{0x09, 0x02, 0x02, 0x09, 0xb0, 0x09},//Product ID
	{0x09, 0x02, 0x03, 0x09, 0x03, 0x12},//Product Version
	0,//num_of_records
	{ 25 , 11 , 0 },//total_attribute_num
	0,//record_base_num
	0,//attribute_data_base_num
	0,//attribute_id16
	0, //attribute_len
	{0x09, 0x02, 0x06, 0x35, 0x68, 0x35, 0x66, 0x08, 0x22,//Report descriptor avago
	 0x25, 0x62,
	 0x05, 0x01,//# Usage Page (Generic Desktop)
	 0x09, 0x02,//# Usage (Mouse)
	 0xa1, 0x01,//# # Collection (Application)
	 0x85, 0x02,//# Report ID=2
	 0x09, 0x01,//USAGE (pointer)
     0xa1, 0x00,//#Collection (Physical)
	 0x05, 0x09,//USAGE PAGE(Button)
	 0x19, 0x01,//USAGE MIN(Button)
	 0x29, 0x08,//USAGE MAX(Button)
	 0x15, 0x00,//# Logical min=0
	 0x25, 0x01,//# Logical max=1
	 0x75, 0x01,//Report Size 1
	 0x95, 0x08,//Report Count 8
	 0x81, 0x02,//Input //0x95, 0x01, 0x75, 0x05, 0x81, 0x03,
	 0x05, 0x01,//# Usage Page (Generic Desktop)
	 0x09, 0x30,//# Usage (X)
	 0x09, 0x31,//# Usage (Y)
	 0x16, 0x01, 0x80,//Local MIN
	 0x26, 0xff, 0x7f,//# Logical max
	 0x75, 0x10,//Report Size 16
	 0x95, 0x02,//Report Count 2
	 0x81, 0x06,//Input
	 0x09, 0x38,//# Usage Wheel
	 0x15, 0x81,//# Local MIN
	 0x25, 0x7f,//# Logical max
	 0x75, 0x08,//Report Size 8
	 0x95, 0x01,//Report Count 1
	 0x81, 0x06,//Input
	 0x05, 0x0C,//#Usage Page  (Consumer)
     0x0A, 0x38, 0x02,//#Usage (AC Pan)
     0x95, 0x01,//#Report Count (1)
     0x75, 0x08,//#Report Size (8)
     0x15, 0x81,//#Logical min (-127),
     0x25, 0x7F,//#Logical Max (127),
     0x81, 0x06,//#Input (Data, Variable, Relative)
	 0xc0,//# (End collection)
	 0xc0,//# (End collection)
	 0x06, 0x00, 0xff,//# Usage Page (Vendor Specific)
	 0x09, 0x01,//# Usage (01)
	 0xA1, 0x01,//# # Collection (Application)
	 0x85, 0x10,//# Report ID=16
	 0x75, 0x08,//Report Size 8
	 0x95, 0x06,//Report Count 6
	 0x15, 0x00,//# Local MIN
	 0x26, 0xff, 0x00, //# Local MAX
	 0x09, 0x01,//# Usage (01)
	 0x91, 0x00,//output
	 0xC0,//# (End collection)
	 },
	 {0}//record_data_bank
};

#elif defined (RELIABILITYTEST)
SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	39,//service_name_len
	{0x09, 0x01, 0x00, 0x25, 0x22, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68, 0x20, 0x56, 0x34, 0x37, 0x30,
	                               0x20, 0x43, 0x6f, 0x72, 0x64, 0x6c, 0x65, 0x73, 0x73, 0x20, 0x4c, 0x61, 0x73,
								   0x65, 0x72, 0x20, 0x4d, 0x6f, 0x75, 0x73, 0x65},//Service Name "logitch"
	20,//service_descript_len
	{0x09, 0x01, 0x01, 0x25, 0x0f, 0x42, 0x6c, 0x75, 0x65, 0x74, 0x6f, 0x6f, 0x74, 0x68, 0x20, 0x4d, 0x6f, 0x75,
	                               0x73, 0x65},//"BLuetooth mouse"
	13,//provider_name_len
	{0x09, 0x01, 0x02, 0x25, 0x08, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68}, //"logitch//provider_name
	{0x09, 0x02, 0x02, 0x08, 0x80},//HID Device Subclass
	{0x09, 0x02, 0x03, 0x08, 0x21},//HID Country code
	{0x09, 0x02, 0x04, 0x28, 0x01},//HID Virtual Cable
	190,//report_descriptor_len
	{0x09, 0x02, 0x0e, 0x28, 0x01},//HID Boot Device
	0x01,//reconnect_initial
	0x00,//normal_connectable
	0x1F40,//supervision_timeout
	0x0640,//ssrhostmaxlatency
	0x0320,//ssrhostmintimeout
	{0x09, 0x02, 0x01, 0x09, 0x04, 0x6d},//Vendor ID
	{0x09, 0x02, 0x02, 0x09, 0xb0, 0x09},//Product ID
	{0x09, 0x02, 0x03, 0x09, 0x03, 0x12},//Product Version
	0,//num_of_records
	{ 25 , 11 , 0 },//total_attribute_num
	0,//record_base_num
	0,//attribute_data_base_num
	0,//attribute_id16
	0, //attribute_len
	{0x09, 0x02, 0x06, 0x35, 0xB9, 0x35, 0xB7, 0x08, 0x22,//Report descriptor avago
	 0x25, 0xB3,
	 0x05, 0x01,//# Usage Page (Generic Desktop)
	 0x09, 0x02,//# Usage (Mouse)
	 0xa1, 0x01,//# # Collection (Application)
	 0x85, 0x02,//# Report ID=2
	 0x09, 0x01,//USAGE (pointer)
     0xa1, 0x00,//#Collection (Physical)
	 0x05, 0x09,//USAGE PAGE(Button)
	 0x19, 0x01,//USAGE MIN(Button)
	 0x29, 0x08,//USAGE MAX(Button)
	 0x15, 0x00,//# Logical min=0
	 0x25, 0x01,//# Logical max=1
	 0x75, 0x01,//Report Size 1
	 0x95, 0x08,//Report Count 8
	 0x81, 0x02,//Input //0x95, 0x01, 0x75, 0x05, 0x81, 0x03,
	 0x05, 0x01,//# Usage Page (Generic Desktop)
	 0x09, 0x30,//# Usage (X)
	 0x09, 0x31,//# Usage (Y)
	 0x16, 0x01, 0x80,//Local MIN
	 0x26, 0xff, 0x7f,//# Logical max
	 0x75, 0x10,//Report Size 16
	 0x95, 0x02,//Report Count 2
	 0x81, 0x06,//Input
	 0x09, 0x38,//# Usage Wheel
	 0x15, 0x81,//# Local MIN
	 0x25, 0x7f,//# Logical max
	 0x75, 0x08,//Report Size 8
	 0x95, 0x01,//Report Count 1
	 0x81, 0x06,//Input
	 0x05, 0x0C,//#Usage Page  (Consumer)
     0x0A, 0x38, 0x02,//#Usage (AC Pan)
     0x95, 0x01,//#Report Count (1)
     0x75, 0x08,//#Report Size (8)
     0x15, 0x81,//#Logical min (-127),
     0x25, 0x7F,//#Logical Max (127),
     0x81, 0x06,//#Input (Data, Variable, Relative)
	 0xc0,//# (End collection)
	 0xc0,//# (End collection)
	 0x06, 0x00, 0xff,//# Usage Page (Vendor Specific)
	 0x09, 0x01,//# Usage (01)
	 0xA1, 0x01,//# # Collection (Application)
	 0x85, 0x10,//# Report ID=16
	 0x75, 0x08,//Report Size 8
	 0x95, 0x06,//Report Count 6
	 0x15, 0x00,//# Local MIN
	 0x26, 0xff, 0x00, //# Local MAX
	 0x09, 0x01,//# Usage (01)
	 0x91, 0x00,//output
	 0xC0,//# (End collection)
     0x05,  0x01,//                   # USAGE_PAGE (Generic Desktop)
     0x09,  0x06,//                   # USAGE (Keyboard)
     0xA1,  0x01,//                   # COLLECTION (Application)
     0x85,  0x01,//                   #   REPORT_ID (1)
     0x75,  0x01,//                   #   REPORT_SIZE (1)
     0x95,  0x08,//                   #   REPORT_COUNT (8)
     0x05,  0x07,//                   #   USAGE_PAGE (Keyboard)
     0x19,  0xE0,//                   #   USAGE_MINIMUM (Keyboard LeftControl)
     0x29,  0xE7,//                   #   USAGE_MAXIMUM (Keyboard Right GUI)
     0x15,  0x00,//                   #   LOGICAL_MINIMUM (0)
     0x25,  0x01,//                   #   LOGICAL_MAXIMUM (1)
     0x81,  0x02,//                   #   INPUT (Data,Var,Abs)
     0x95,  0x01,//                   #   REPORT_COUNT (1)
     0x75,  0x08,//                   #   REPORT_SIZE (8)
     0x81,  0x03,//                   #   INPUT (Cnst,Var,Abs)
     0x95,  0x05,//                   #   REPORT_COUNT (5)
     0x75,  0x01,//                   #   REPORT_SIZE (1)
     0x05,  0x08,//                   #   USAGE_PAGE (LEDs)
     0x19,  0x01,//                   #   USAGE_MINIMUM (Num Lock)
     0x29,  0x05,//                   #   USAGE_MAXIMUM (Kana)
     0x91,  0x02,//                   #   OUTPUT (Data,Var,Abs)
     0x95,  0x01,//                   #   REPORT_COUNT (1)
     0x75,  0x03,//                   #   REPORT_SIZE (3)
     0x91,  0x03,//                   #   OUTPUT (Cnst,Var,Abs)
     0x95,  0x06,//                   #   REPORT_COUNT (6)
     0x75,  0x08,//                   #   REPORT_SIZE (8)
     0x15,  0x00,//                   #   LOGICAL_MINIMUM (0)
     0x26,  0xFF,  0x00,//                #   LOGICAL_MAXIMUM (255)
     0x05,  0x07,//                   #   USAGE_PAGE (Keyboard)
     0x19,  0x00,//                   #   USAGE_MINIMUM (Reserved (no event indicated))
     0x29,  0xFF,//                   #   USAGE_MAXIMUM (Reserved (no event indicated))
     0x81,  0x00,//                   #   INPUT (Data,Ary,Abs)
     0xC0,//                      # END_COLLECTION
	 0x05,  0xff,//		# Usage Page (generic desktop)
     0x09,  0x02,//		# Usage (Mouse)
     0xA1,	0x01,//
     0x85,	0x04,//		# REPORT_ID (4)
     0x75,	0x08,//
     0x95,	0x02,//		# Need 2 bytes
     0xB1,	0x03,//
     0xC0,//
	 },
	 {0}//record_data_bank
};

#else

SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	39,//service_name_len
	{0x09, 0x01, 0x00, 0x25, 0x22, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68, 0x20, 0x56, 0x34, 0x37, 0x30,
	                               0x20, 0x43, 0x6f, 0x72, 0x64, 0x6c, 0x65, 0x73, 0x73, 0x20, 0x4c, 0x61, 0x73,
								   0x65, 0x72, 0x20, 0x4d, 0x6f, 0x75, 0x73, 0x65},//Service Name "logitch"
	20,//service_descript_len
	{0x09, 0x01, 0x01, 0x25, 0x0f, 0x42, 0x6c, 0x75, 0x65, 0x74, 0x6f, 0x6f, 0x74, 0x68, 0x20, 0x4d, 0x6f, 0x75,
	                               0x73, 0x65},//"BLuetooth mouse"
	13,//provider_name_len
	{0x09, 0x01, 0x02, 0x25, 0x08, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68}, //"logitch//provider_name
	{0x09, 0x02, 0x02, 0x08, 0x80},//HID Device Subclass
	{0x09, 0x02, 0x03, 0x08, 0x21},//HID Country code
	{0x09, 0x02, 0x04, 0x28, 0x01},//HID Virtual Cable
	109,//report_descriptor_len
	{0x09, 0x02, 0x0e, 0x28, 0x01},//HID Boot Device
	0x01,//reconnect_initial
	0x00,//normal_connectable
	0x1F40,//supervision_timeout
	0x0640,//ssrhostmaxlatency
	0x0320,//ssrhostmintimeout
	{0x09, 0x02, 0x01, 0x09, 0x04, 0x6d},//Vendor ID
	{0x09, 0x02, 0x02, 0x09, 0xb0, 0x09},//Product ID
	{0x09, 0x02, 0x03, 0x09, 0x03, 0x12},//Product Version
	0,//num_of_records
	{ 25 , 11 , 0 },//total_attribute_num
	0,//record_base_num
	0,//attribute_data_base_num
	0,//attribute_id16
	0, //attribute_len
	{0x09, 0x02, 0x06, 0x35, 0x68, 0x35, 0x66, 0x08, 0x22,//Report descriptor avago
	 0x25, 0x62,
	 0x05, 0x01,//# Usage Page (Generic Desktop)
	 0x09, 0x02,//# Usage (Mouse)
	 0xa1, 0x01,//# # Collection (Application)
	 0x85, 0x02,//# Report ID=2
	 0x09, 0x01,//USAGE (pointer)
     0xa1, 0x00,//#Collection (Physical)
	 0x05, 0x09,//USAGE PAGE(Button)
	 0x19, 0x01,//USAGE MIN(Button)
	 0x29, 0x08,//USAGE MAX(Button)
	 0x15, 0x00,//# Logical min=0
	 0x25, 0x01,//# Logical max=1
	 0x75, 0x01,//Report Size 1
	 0x95, 0x08,//Report Count 8
	 0x81, 0x02,//Input //0x95, 0x01, 0x75, 0x05, 0x81, 0x03,
	 0x05, 0x01,//# Usage Page (Generic Desktop)
	 0x09, 0x30,//# Usage (X)
	 0x09, 0x31,//# Usage (Y)
	 0x16, 0x01, 0x80,//Local MIN
	 0x26, 0xff, 0x7f,//# Logical max
	 0x75, 0x10,//Report Size 16
	 0x95, 0x02,//Report Count 2
	 0x81, 0x06,//Input
	 0x09, 0x38,//# Usage Wheel
	 0x15, 0x81,//# Local MIN
	 0x25, 0x7f,//# Logical max
	 0x75, 0x08,//Report Size 8
	 0x95, 0x01,//Report Count 1
	 0x81, 0x06,//Input
	 0x05, 0x0C,//#Usage Page  (Consumer)
     0x0A, 0x38, 0x02,//#Usage (AC Pan)
     0x95, 0x01,//#Report Count (1)
     0x75, 0x08,//#Report Size (8)
     0x15, 0x81,//#Logical min (-127),
     0x25, 0x7F,//#Logical Max (127),
     0x81, 0x06,//#Input (Data, Variable, Relative)
	 0xc0,//# (End collection)
	 0xc0,//# (End collection)
	 0x06, 0x00, 0xff,//# Usage Page (Vendor Specific)
	 0x09, 0x01,//# Usage (01)
	 0xA1, 0x01,//# # Collection (Application)
	 0x85, 0x10,//# Report ID=16
	 0x75, 0x08,//Report Size 8
	 0x95, 0x06,//Report Count 6
	 0x15, 0x00,//# Local MIN
	 0x26, 0xff, 0x00, //# Local MAX
	 0x09, 0x01,//# Usage (01)
	 0x91, 0x00,//output
	 0xC0,//# (End collection)
	 },
	 {0}//record_data_bank
};

#endif//HID_SOC

#elif defined(HEADSET_SOC)
SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	0
};

#else

SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	0
};

#endif

/*


#ifdef HID_Profile//temp
#ifdef HID_IIM_SOC
SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	24,//service_name_len
	{0x09, 0x01, 0x00, 0x25, 0x13, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f, 0x20, 0x52, 0x56, 0x4c, 0x2d,
	                               0x43, 0x4e, 0x54, 0x2d, 0x30, 0x31},//Service Name "Nintendo RVL-CNT-01"
	24,//service_descript_len
	{0x09, 0x01, 0x01, 0x25, 0x13, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f, 0x20, 0x52, 0x56, 0x4c, 0x2d,
	                               0x43, 0x4e, 0x54, 0x2d, 0x30, 0x31},//"Nintendo RVL-CNT-01"
	13,//provider_name_len
	{0x09, 0x01, 0x02, 0x25, 0x08, 0x4e, 0x69, 0x6e, 0x74, 0x65, 0x6e, 0x64, 0x6f}, //"Nintendo//provider_name
	{0x09, 0x02, 0x02, 0x08, 0x04},//HID Device Subclass
	{0x09, 0x02, 0x03, 0x08, 0x33},//HID Country code
	{0x09, 0x02, 0x04, 0x28, 0x00},//HID Virtual Cable
	228,//report_descriptor_len
	{0x09, 0x02, 0x06, 0x35, 0xDF, 0x35, 0xDD, 0x08, 0x22,//Report descriptor wii
	 0x25, 0xD9,
	 0x05, 0x01,         //# Usage Page (Generic Desktop)
     0x09, 0x05,         //# Usage (Gamepad)
     0xa1, 0x01,               //# Collection (Physical)
     0x85, 0x10,         //# Report ID=10
     0x15, 0x00,         //# Logical min=0
     0x26, 0xff, 0x00,    //  # Logical max=ff
     0x75, 0x08,         //# Report size=8
     0x95, 0x01,         //# Report count=1
     0x06, 0x00, 0xff,   //# Usage page ff00?
     0x09, 0x01,         //# Usage (Pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x11,         //# Report ID=11
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x12,         //# Report ID=12
     0x95, 0x02,          //# Report count=2
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x13,         //# Report ID=13
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x14,         //# Report ID=14
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x15,         //# Report ID=15
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x16,         //# Report ID=16
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x17,         //# Report ID=17
     0x95, 0x06,         //# Report count=6
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x18,         //# Report ID=18
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x19,         //# Report ID=19
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x1a,         //# Report ID=1a
     0x95, 0x01,         //# Report count=1
     0x09, 0x01,         //# Usage (pointer?)
     0x91, 0x00,         //# Output(...)
     0x85, 0x20,         //# Report ID=20
     0x95, 0x06,         //# Report count=6
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x21,         //# Report ID=21
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x22,         //# Report ID=22
     0x95, 0x04,         //# Report count=4
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x30,         //# Report ID=30
     0x95, 0x02,         //# Report count=2
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x31,         //# Report ID=31
     0x95, 0x05,         //# Report count=5
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x32,         //# Report ID=32
     0x95, 0x0a,         //# Report count=10
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x33,         //# Report ID=33
     0x95, 0x11,         //# Report count=11
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x34,         //# Report ID=34
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x35,         //# Report ID=35
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x36,         //# Report ID=36
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x37,         //# Report ID=37
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x3d,         //# Report ID=3d
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x3e,         //# Report ID=3e
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0x85, 0x3f,         //# Report ID=3f
     0x95, 0x15,         //# Report count=15
     0x09, 0x01,         //# Usage (pointer?)
     0x81, 0x00,         //# Input(...)
     0xc0            //# (End collection)
	 },
	{0x09, 0x02, 0x0e, 0x28, 0x00},//HID Boot Device
	{0x09, 0x02, 0x01, 0x09, 0x05, 0x7e},//Vendor ID
	{0x09, 0x02, 0x02, 0x09, 0x03, 0x06},//Product ID
	{0x09, 0x02, 0x03, 0x09, 0x3a, 0x1c},//Product Version
	0,//num_of_records
	{ 25 , 11 , 7 },//total_attribute_num
	0,//record_base_num
	0,//attribute_data_base_num
	0,//attribute_id16
	0 //attribute_len
};
#else
SDP_MODIFY_SERVICE_RECORD_BANK CODE gSDP_m_service_record_bank_init =
{
	39,//service_name_len
	{0x09, 0x01, 0x00, 0x25, 0x22, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68, 0x20, 0x56, 0x34, 0x37, 0x30,
	                               0x20, 0x43, 0x6f, 0x72, 0x64, 0x6c, 0x65, 0x73, 0x73, 0x20, 0x4c, 0x61, 0x73,
								   0x65, 0x72, 0x20, 0x4d, 0x6f, 0x75, 0x73, 0x65},//Service Name "logitch"
	20,//service_descript_len
	{0x09, 0x01, 0x01, 0x25, 0x0f, 0x42, 0x6c, 0x75, 0x65, 0x74, 0x6f, 0x6f, 0x74, 0x68, 0x20, 0x4d, 0x6f, 0x75,
	                               0x73, 0x65},//"BLuetooth mouse"
	13,//provider_name_len
	{0x09, 0x01, 0x02, 0x25, 0x08, 0x4c, 0x6f, 0x67, 0x69, 0x74, 0x65, 0x63, 0x68}, //"logitch//provider_name
	{0x09, 0x02, 0x02, 0x08, 0x80},//HID Device Subclass
	{0x09, 0x02, 0x03, 0x08, 0x21},//HID Country code
	{0x09, 0x02, 0x04, 0x28, 0x01},//HID Virtual Cable
	71,//report_descriptor_len
	{0x09, 0x02, 0x06, 0x35, 0x42, 0x35, 0x40, 0x08, 0x22,//Report descriptor avago
	 0x25, 0x3C, 0x05, 0x01, 0x09, 0x02, 0xa1, 0x01, 0x85, 0x02, 0x09, 0x01,//USAGE
     0xa1, 0x00,
	 0x05, 0x09, 0x19, 0x01, 0x29, 0x10, 0x15, 0x00, 0x25, 0x01,
	 0x75, 0x01, 0x95, 0x10, 0x81, 0x02, //0x95, 0x01, 0x75, 0x05, 0x81, 0x03,
	 0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x16, 0x01, 0x80,//Local MIN
	 0x26, 0xff, 0x7f, 0x75, 0x10, 0x95, 0x02, 0x81, 0x06,
	 0x09, 0x38, 0x15, 0x81, 0x25, 0x7f, 0x75, 0x08, 0x95, 0x01, 0x81, 0x06, 0xc0,
	 0xc0},
	{0x09, 0x02, 0x0e, 0x28, 0x01},//HID Boot Device
	{0x09, 0x02, 0x01, 0x09, 0x04, 0x6d},//Vendor ID
	{0x09, 0x02, 0x02, 0x09, 0xb0, 0x09},//Product ID
	{0x09, 0x02, 0x03, 0x09, 0x03, 0x12},//Product Version
	0,//num_of_records
	{ 25 , 11 , 0 },//total_attribute_num
	0,//record_base_num
	0,//attribute_data_base_num
	0,//attribute_id16
	0 //attribute_len
	{0x00}
};
#endif //HID_IIM_SOC
#endif //HID_Profile//temp
*/