#include "reside_flash.inc"
#include "..\SDAP\SDAP_interface.h"
#include "..\MMI\MMI_PSM.h"
#include "..\MMI\MMI_SCN.h"
#include "../HID/HID_interface.h"

#ifdef HID_Profile

#define DESCRIPTOR_TYPE_STANDARD                        (0x00<<5)
#define DESCRIPTOR_TYPE_CLASS                           (0x01<<5)
#define DESCRIPTOR_TYPE_VENDOR                          (0x02<<5)

#define CLASS_DESCRIPTOR_TYPE_REPORT                    (DESCRIPTOR_TYPE_CLASS | 0x02)
#define CLASS_DESCRIPTOR_TYPE_PHYSICAL_DESCRIPTOR       (DESCRIPTOR_TYPE_CLASS | 0x03)

#define HID_SUBCLASS_NOT_KB_NOT_POINTING_DEVICE         (0x00<<6)
#define HID_SUBCLASS_KB                                 (0x01<<6)
#define HID_SUBCLASS_POINTING_DEVICE                    (0x02<<6)
#define HID_SUBCLASS_COMBO_KB_POINTING_DEVICE           (0x03<<6)

#define HID_SUBCLASS_UNCATEGORIZED_DEVICE               (0x00<<2)
#define HID_SUBCLASS_JOYSTICK                           (0x01<<2)
#define HID_SUBCLASS_GAMEPAD                            (0x02<<2)
#define HID_SUBCLASS_REMOTE_CONTROL                     (0x03<<2)
#define HID_SUBCLASS_SENSING_DEVICE                     (0x04<<2)
#define HID_SUBCLASS_DIGITIZER_TABLET                   (0x05<<2)
#define HID_SUBCLASS_CARD_READER                        (0x06<<2)
#define HID_SUBCLASS_DIGITAL_PEN                        (0x07<<2)
#define HID_SUBCLASS_HANDHELD_SCANNER                   (0x08<<2)
#define HID_SUBCLASS_HANDHELD_GESTURAL_INPUT_DEVICE     (0x09<<2)

#define HID_BYTE_SIZE_0                                 (0x00)
#define HID_BYTE_SIZE_1                                 (0x01)
#define HID_BYTE_SIZE_2                                 (0x02)
#define HID_BYTE_SIZE_4                                 (0x03)

#define HID_ITEM_TYPE_MAIN                              (0x00<<2)  
#define HID_ITEM_TYPE_GLOBAL                            (0x01<<2)
#define HID_ITEM_TYPE_LOCAL                             (0x02<<2)

#define HID_MAIN_ITEM_INPUT                             (0x80 | HID_ITEM_TYPE_MAIN)
#define HID_MAIN_ITEM_OUTPUT                            (0x90 | HID_ITEM_TYPE_MAIN)
#define HID_MAIN_ITEM_FEATURE                           (0xB0 | HID_ITEM_TYPE_MAIN)
#define HID_MAIN_ITEM_COLLECTION                        (0xA0 | HID_ITEM_TYPE_MAIN)
#define HID_MAIN_ITEM_END_COLLECTION                    (0xC0 | HID_ITEM_TYPE_MAIN)

#define HID_GLOBAL_ITEM_USAGE_PAGE                      (0x00 | HID_ITEM_TYPE_GLOBAL)  
#define HID_GLOBAL_ITEM_LOGICAL_MINIMUM                 (0x10 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_LOGICAL_MAXIMUM                 (0x20 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_PHYSICAL_MINIMUM                (0x30 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_PHYSICAL_MAXIMUM                (0x40 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_UNIT_EXPONENT                   (0x50 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_UNIT                            (0x60 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_REPORT_SIZE                     (0x70 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_REPORT_ID                       (0x80 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_REPORT_COUNT                    (0x90 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_PUSH                            (0xA0 | HID_ITEM_TYPE_GLOBAL) 
#define HID_GLOBAL_ITEM_POP                             (0xB0 | HID_ITEM_TYPE_GLOBAL)

#define HID_LOCAL_ITEM_USAGE                            (0x00 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_USAGE_MINIMUM                    (0x10 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_USAGE_MAXIMUM                    (0x20 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_DESIGNATOR_INDEX                 (0x30 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_DESIGNATOR_MINIMUM               (0x40 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_DESIGNATOR_MAXIMUM               (0x50 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_STRING_INDEX                     (0x70 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_STRING_MINIMUM                   (0x80 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_STRING_MAXIMUM                   (0x90 | HID_ITEM_TYPE_LOCAL) 
#define HID_LOCAL_ITEM_DELIMITER                        (0xA0 | HID_ITEM_TYPE_LOCAL) 

/// DATA
#define HID_COLLECTION_ITEM_PHYSICAL                    (0x00)
#define HID_COLLECTION_ITEM_APPLICATION                 (0x01)
#define HID_COLLECTION_ITEM_LOGICAL                     (0x02)
#define HID_COLLECTION_ITEM_REPORT                      (0x03)
#define HID_COLLECTION_ITEM_NAMED_ARRAY                 (0x04)
#define HID_COLLECTION_ITEM_USAGE_SWITCH                (0x05)
#define HID_COLLECTION_ITEM_USAGE_MODIFIER              (0x06)

#define HID_USAGE_PAGE_GENERIC_DESKTOP                  (0x01)
#define HID_USAGE_PAGE_SIMULATION_CONTROLS              (0x02)
#define HID_USAGE_PAGE_VR_CONTROLS                      (0x03)
#define HID_USAGE_PAGE_SPORT_CONTROLS                   (0x04)
#define HID_USAGE_PAGE_GAME_CONTROLS                    (0x05)
#define HID_USAGE_PAGE_GENERIC_DEVICE_CONTROLS          (0x06)
#define HID_USAGE_PAGE_KEYBOARD_KEYPAD                  (0x07)
#define HID_USAGE_PAGE_LED                              (0x08)
#define HID_USAGE_PAGE_BUTTON                           (0x09)
#define HID_USAGE_PAGE_ORDINAL                          (0x0A)
#define HID_USAGE_PAGE_TELEPHONY_DEVICE                 (0x0B)
#define HID_USAGE_PAGE_COMSUMER                         (0x0C)
#define HID_USAGE_PAGE_DIGITIZERS                       (0x0D)
#define HID_USAGE_PAGE_UNICODE                          (0x10)
#define HID_USAGE_PAGE_ALPHANUMERIC_DISPLAY             (0x14)
#define HID_USAGE_PAGE_MEDICAL_INSTRUMENT               (0x40)

#define HID_USAGE_POINTER                               (0x01)
#define HID_USAGE_MOUSE                                 (0x02)
#define HID_USAGE_KB                                    (0x06)
#define HID_USAGE_X                                     (0x30)  
#define HID_USAGE_Y                                     (0x31)

#define HID_REPORT_DATA                                 (0x00<<0)
#define HID_REPORT_CONSTANT                             (0x01<<0)
#define HID_REPORT_ARRAY                                (0x00<<1)
#define HID_REPORT_VARIABLE                             (0x01<<1)
#define HID_REPORT_ABSOLUTE                             (0x00<<2)
#define HID_REPORT_RELATIVE                             (0x01<<2)
#define HID_REPORT_NO_WRAP                              (0x00<<3)
#define HID_REPORT_WRAP                                 (0x01<<3)
#define HID_REPORT_LINEAR                               (0x00<<4)
#define HID_REPORT_NON_LINEAR                           (0x01<<4)
#define HID_REPORT_PREFERRED_STATE                      (0x00<<5)
#define HID_REPORT_NO_PREFERRED                         (0x01<<5)
#define HID_REPORT_NO_NULL_POSITION                     (0x00<<6)
#define HID_REPORT_NULL_STATE                           (0x01<<6)
#define HID_REPORT_NON_VOLATILE                         (0x00<<7)
#define HID_REPORT_VOLATILE                             (0x01<<7)
#define HID_REPORT_BIT_FIELD                            (0x00<<8)
#define HID_REPORT_BUFFERED_BYTES                       (0x01<<8)


#define VERSION_HID             0x0100


ATTRDEF_ROM(hid, ServiceRecordHandle) = 
			{ ATTRID(ATTRID_ServiceRecordHandle), ATTRLEN(5), SERVICE_HANDLE32(PROFILE_HID) };
ATTRDEF_ROM(hid, ServiceClassIdList) = 
			{ ATTRID(ATTRID_ServiceClassIdList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_HUMAN_INTERFACE_DEVICE) };
ATTRDEF_ROM(hid, ProtocolDescriptorList) = 
			{ ATTRID(ATTRID_ProtocolDescriptorList), ATTRLEN(15), DEDEF_SEQU8(13),
																	DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP), PSM_U16(PSM_HID_CTL),
																	DEDEF_SEQU8(3), PROTOCOL_UUID16(PROTOCOL_UUID_HID) };
ATTRDEF_ROM(hid, BrowseGroupList) =
            { ATTRID(ATTRID_BrowseGroupList), ATTRLEN(5), DEDEF_SEQU8(3), SERVICE_UUID16(SERVICE_UUID_PUBLIC_BROWSE_ROOT) };
ATTRDEF_ROM(hid, LangBaseAttributeIdList) =
            { ATTRID(ATTRID_LangBaseAttributeIdList), ATTRLEN(11), DEDEF_SEQU8(9), 
                                                                    DEDEF_U16(0x656E), // language base attribute id list "english"
                                                                    DEDEF_U16(0x006A), // character encode 6a
                                                                    DEDEF_U16(0x0100) }; // language base id
ATTRDEF_ROM(hid, ProfileDescriptorList) = 
			{ ATTRID(ATTRID_ProfileDescriptorList), ATTRLEN(10), DEDEF_SEQU8(8), 
                                                                    DEDEF_SEQU8(6), PROFILE_UUID16(PROFILE_UUID_HID), VERSION_U16(VERSION_HID) };
ATTRDEF_ROM(hid, AddProtocolDescriptorLists) = 
			{ ATTRID(ATTRID_AddProtocolDescriptorLists), ATTRLEN(17), DEDEF_SEQU8(15),  
			                                                            DEDEF_SEQU8(13),
			                                                                DEDEF_SEQU8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP), PSM_U16(PSM_HID_INT), 
			                                                                DEDEF_SEQU8(3), PROTOCOL_UUID16(PROTOCOL_UUID_HID) };
ATTRDEF_ROM(hid, VirtualCable) = 
			{ ATTRID(ATTRID_VirtualCable), ATTRLEN(2), DEDEF_BOOL(1) };
ATTRDEF_ROM(hid, NormallyConnectable) = 
			{ ATTRID(ATTRID_NormallyConnectable), ATTRLEN(2), DEDEF_BOOL(0) };
ATTRDEF_ROM(hid, ReconnectInitiate) = 
			{ ATTRID(ATTRID_ReconnectInitiate), ATTRLEN(2), DEDEF_BOOL(1) };			
ATTRDEF_ROM(hid, CountryCode) = 
			{ ATTRID(ATTRID_CountryCode), ATTRLEN(2), DEDEF_U8(0X21) };
ATTRDEF_ROM(hid, DeviceSubclass) = 
			{ ATTRID(ATTRID_DeviceSubclass), ATTRLEN(2), DEDEF_U8(0X40) };			
ATTRDEF_ROM(hid, SupervisionTimeout) = 
			{ ATTRID(ATTRID_SupervisionTimeout), ATTRLEN(3), DEDEF_U16(0X1F40) };
ATTRDEF_ROM(hid, BootDevice) = 
			{ ATTRID(ATTRID_BootDevice), ATTRLEN(2), DEDEF_BOOL(1) };
ATTRDEF_ROM(hid, SDPDisable) = 
			{ ATTRID(ATTRID_SDPDisable), ATTRLEN(2), DEDEF_BOOL(0) };				
ATTRDEF_ROM(hid, ProfileVersion) = 
			{ ATTRID(ATTRID_ProfileVersion), ATTRLEN(3), DEDEF_U16(0x0100) };
ATTRDEF_ROM(hid, DeviceReleaseNumber) = 
			{ ATTRID(ATTRID_DeviceReleaseNumber), ATTRLEN(3), DEDEF_U16(0x0100) };
ATTRDEF_ROM(hid, ParserVersion) = 
			{ ATTRID(ATTRID_ParserVersion), ATTRLEN(3), DEDEF_U16(0x0111) };
#ifdef SELFIE_App

// for iOS
ATTRDEF_ROM(hid, DescriptorListForIOS) = 
    { ATTRID(ATTRID_DescriptorList), ATTRLEN(0x21), DEDEF_SEQU8(0x1F),
			                                                DEDEF_SEQU8(0x1D),
			                                                    0x08, 0x22,
        DEDEF_TEXT8(0x19),
        
//    	0x05, 0x01,         //   USAGE_PAGE (Generic Desktop)
//        0x09, 0x06,         //   USAGE (Keyboard)
//        0xA1, 0x01,         //   COLLECTION (Application)
//        0x85, 0x01,         //   REPORT_ID (1)
//        0x05, 0x07,         //   USAGE_PAGE (Keyboard)
//        0x75, 0x08,         //   REPORT_SIZE (8)
//        0x95, 0x02,         //   REPORT_COUNT (2)
//        0x81, 0x03,         //   INPUT (Cnst,Var,Abs) ??
//        0x15, 0x00,         //   LOGICAL_MINIMUM (0)
//        0x26, 0xFF, 0x00,   //   LOGICAL_MAXIMUM (255)
//        0x75, 0x08,         //   REPORT_SIZE (8)
//        0x95, 0x06,         //   REPORT_COUNT (6)
//        0x19, 0x00,         //   USAGE_MINIMUM (Reserved (no event indicated))
//        0x29, 0xFF,         //   USAGE_MAXIMUM (Reserved (no event indicated))
//        0x81, 0x00,         //   INPUT (Data,Ary,Abs)
//        0xC0, 
                    
    	0x05, 0x0C,         //   USAGE_PAGE (Consumer Devices)
        0x09, 0x01,         //   USAGE (Consumer Control)
        0xA1, 0x01,         //   COLLECTION (Application)
        0x85, 0x03,         //   REPORT_ID(3)
        0x15, 0x00,         //   LOGICAL_MINIMUM (0)
        0x26, 0x80, 0x03,   //   LOGICAL_MAXIMUM (896)
        0x19, 0x00,         //   USAGE_MINIMUM (Reserved (no event indicated))
        0x2A, 0x80, 0x03,   //   USAGE_MAXIMUM (Reserved (no event indicated))
        0x75, 0x10,         //   REPORT_SIZE (16)
        0x95, 0x01,         //   REPORT_COUNT (1)
        0x81, 0x00,         //   INPUT (Data,Ary,Abs)
        0xC0                //   END COLLECTION
    };

// for Android	
ATTRDEF_ROM(hid, DescriptorList) =     
    { ATTRID(ATTRID_DescriptorList), ATTRLEN(142), DEDEF_SEQU8(0x8C),
                                                    DEDEF_SEQU8(0x8A),
                                                        0x08, 0x22,			                                                    
	0x25, 0x86,

    //*******************************************************
    //#Joystick (39(dec)= 27(hex))
        0x05, 0x01,         //   USAGE_PAGE (Generic Desktop)
        0x09, 0x04,         //   USAGE (Joystick)
        0xA1, 0x01,         //   COLLECTION (Application)
        0x85, 0x01,         //   REPORT_ID (1)
        0x05, 0x09,         //   USAGE_PAGE (Button)
        0x19, 0x01,         //   USAGE_MINIMUM (Button 1)
        0x29, 0x10,         //   USAGE_MINIMUM (Button 16)
        0x15, 0x00,         //   LOGICAL_MINIMUM (0)
        0x25, 0x01,         //   LOGICAL_MAXIMUM (1)
        0x95, 0x10,         //   REPORT_COUNT (16)
        0x75, 0x01,         //   REPORT_SIZE (1)
        0x81, 0x02,         //   INPUT (Data,Var,Abs)
        0x05, 0x01,         //   USAGE_PAGE (Generic Desktop)
        0x09, 0x30,         //   USAGE (X)
        0x15, 0x81,         //   LOGICAL_MINIMUM (-127)
        0x25, 0x7F,         //   LOGICAL_MAXIMUM (127)
        0x75, 0x08,         //   REPORT_COUNT (8)
        0x95, 0x01,         //   REPORT_SIZE (1)
        0x81, 0x02,         //   INPUT (Data,Var,Abs)
        0xC0,               //   END_COLLECTION 

    /*
    //*******************************************************
    //#KB Reduced (32(dec)= 20(hex))
	    0x05, 0x01,         //   USAGE_PAGE (Generic Desktop)
        0x09, 0x06,         //   USAGE (Keyboard)
        0xA1, 0x01,         //   COLLECTION (Application)
        0x85, 0x01,         //   REPORT_ID (1)
        0x05, 0x07,         //   USAGE_PAGE (Keyboard)
        0x75, 0x08,         //   REPORT_SIZE (8)
        0x95, 0x02,         //   REPORT_COUNT (2)
        0x81, 0x03,         //   INPUT (Cnst,Var,Abs)
        0x15, 0x00,         //   LOGICAL_MINIMUM (0)
        0x26, 0xFF, 0x00,   //   LOGICAL_MAXIMUM (255)
        0x75, 0x08,         //   REPORT_SIZE (8)
        0x95, 0x06,         //   REPORT_COUNT (6)
        0x19, 0x00,         //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0xFF,         //   USAGE_MAXIMUM (Reserved (no event indicated))
        0x81, 0x00,         //   INPUT (Data,Ary,Abs)
        0xC0, */
	//*******************************************************
	//Mouse (30(dec)= 1E(hex))
	
		0x05, 0x01,         //   USAGE_PAGE (Generic Desktop) 
		0x09, 0x02,         //   USAGE (Mouse)
		0xa1, 0x01,         //   COLLECTION (Application)
		0x85, 0x02,         //   REPORT_ID (2)
		0x09, 0x01,         //   USAGE (Pointer)
		0xa1, 0x00,         //   COLLECTION (Physical)
		0x05, 0x09,         //   USAGE_PAGE (Button)
		0x19, 0x01,         //   USAGE MIN(Button) 
		0x29, 0x08,         //   USAGE MAX(Button) 
		0x15, 0x00,         //   LOGICAL_MINIMUM (0)
		0x25, 0x01,         //   LOGICAL_MAXIMUM (0)
		0x75, 0x01,         //   REPORT_SIZE (1)
		0x95, 0x08,         //   REPORT_COUNT (8)
		0x81, 0x02,         //   INPUT (Data,Var,Abs)		
		0xC0,               //   END_COLLECTION 
		0xC0,               //   END_COLLECTION	
	
	//*************************************************************
	/*
	//#KB Standard (66(dec)= 42(hex))
	
		0x05, 0x01,			//   USAGE_PAGE (Generic Desktop)
		0x09, 0x06,         //   USAGE (Keyboard)
		0xA1, 0x01,         //   COLLECTION (Application)
		0x85, 0x01,         //   REPORT_ID (1)
		0x75, 0x01,         //   REPORT_SIZE (1)
		0x95, 0x08,         //   REPORT_COUNT (8)
		0x05, 0x07,         //   USAGE_PAGE (Keyboard)
		0x19, 0xE0,         //   USAGE_MINIMUM (Keyboard LeftControl)
		0x29, 0xE7,         //   USAGE_MAXIMUM (Keyboard Right GUI)
		0x15, 0x00,         //   LOGICAL_MINIMUM (0)
		0x25, 0x01,         //   LOGICAL_MAXIMUM (1)
		0x81, 0x02,         //   INPUT (Data,Var,Abs)
		0x95, 0x01,         //   REPORT_COUNT (1)
		0x75, 0x08,         //   REPORT_SIZE (8)
		0x81, 0x03,         //   INPUT (Cnst,Var,Abs)
		0x95, 0x05,         //   REPORT_COUNT (5)
		0x75, 0x01,         //   REPORT_SIZE (1)
		0x05, 0x08,         //   USAGE_PAGE (LEDs)
		0x19, 0x01,         //   USAGE_MINIMUM (Num Lock)
		0x29, 0x05,         //   USAGE_MAXIMUM (Kana)
		0x91, 0x02,         //   OUTPUT (Data,Var,Abs)
		0x95, 0x01,         //   REPORT_COUNT (1)
		0x75, 0x03,         //   REPORT_SIZE (3)
		0x91, 0x03,         //   OUTPUT (Cnst,Var,Abs)
		0x95, 0x06,         //   REPORT_COUNT (6)
		0x75, 0x08,         //   REPORT_SIZE (8)
		0x15, 0x00,         //   LOGICAL_MINIMUM (0)
		0x26, 0xFF, 0x00,	//   LOGICAL_MAXIMUM (255)
		0x05, 0x07,         //   USAGE_PAGE (Keyboard)
		0x19, 0x00,         //   USAGE_MINIMUM (Reserved (no event indicated))
		0x29, 0xFF,         //   USAGE_MAXIMUM (Reserved (no event indicated))
		0x81, 0x00,			//   INPUT (Data,Ary,Abs)
		0xC0,            	// 	 END_COLLECTION*/
	                                
	//*************************************************************
	//#KB BitMap ((65 dec)= (41 hex))
	
		0x05, 0x0C,         //   USAGE_PAGE (Consumer Devices)
		0x09, 0x01,         //   USAGE (Consumer Control)
		0xA1, 0x01,         //   COLLECTION (Application)
		0x85, 0x03,         //   REPORT_ID(3)
		0x15, 0x00,         //   LOGICAL_MINIMUM (0)
		0x25, 0x01,         //   LOGICAL_MAXIMUM (1)
		0x75, 0x01,         //   REPORT_SIZE (1)
		0x95, 0x0F,         //   REPORT_COUNT (15)
		0x0A, 0x94, 0x01,   //   USAGE (AL Local Machine Browser)
		0x0A, 0x92, 0x01,   //   USAGE (AL Calculator)
		0x0A, 0x83, 0x01,   //   USAGE (AL Consumer Control Configuration)
		0x0A, 0x23, 0x02,   //   USAGE (AC Home)
		0x0A, 0x8A, 0x01,   //   USAGE (AL Email Reader)
		0x0A, 0x2A, 0x02,   //   USAGE (AC Bookmarks)
		0x0A, 0x21, 0x02,   //   USAGE (AC Search)
		0x0A, 0x24, 0x02,   //   USAGE (AC Back)	
		0x0A, 0x25, 0x02,   //   USAGE (AC Forward)
		0x0A, 0x26, 0x02,   //   USAGE (AC Stop)
		0x09, 0xB8,         //   USAGE (Eject)
		0x09, 0xE9,         //   USAGE (Volume Increment)
		0x09, 0xEA,         //   USAGE (Volume Decrement)
		0x09, 0xB7,         //   USAGE (Stop)
		0x09, 0xCD,         //   USAGE (Play/Pause)	
		0x81, 0x02,         //   INPUT (Data,Var,Abs)
		0x95, 0x01,         //   REPORT_COUNT (1)
		0x75, 0x09,         //   REPORT_SIZE (9)
		0x81, 0x03,         //   INPUT (Cnst,Var,Abs)
		0xC0,               //   END_COLLECTION		
    };
    
ATTRLISTDEF_ROM(hid) =
{
	(SdpServiceAttributePtr) ATTRNAME(hid, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(hid, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(hid, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hid, BrowseGroupList),
	(SdpServiceAttributePtr) ATTRNAME(hid, LangBaseAttributeIdList),
	(SdpServiceAttributePtr) ATTRNAME(hid, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hid, AddProtocolDescriptorLists),

    (SdpServiceAttributePtr) ATTRNAME(hid, VirtualCable),
    (SdpServiceAttributePtr) ATTRNAME(hid, ReconnectInitiate),
    (SdpServiceAttributePtr) ATTRNAME(hid, DescriptorList),

	(SdpServiceAttributePtr) ATTRNAME(hid, BootDevice),
	(SdpServiceAttributePtr) ATTRNAME(hid, SDPDisable),
	(SdpServiceAttributePtr) ATTRNAME(hid, DeviceReleaseNumber),
	(SdpServiceAttributePtr) ATTRNAME(hid, ParserVersion),
	(SdpServiceAttributePtr) ATTRNAME(hid, DeviceSubclass),
	(SdpServiceAttributePtr) ATTRNAME(hid, CountryCode),
	(SdpServiceAttributePtr) ATTRNAME(hid, ProfileVersion),
	(SdpServiceAttributePtr) ATTRNAME(hid, NormallyConnectable),
};

ATTRLISTDEF_ROM(hidForIOS) =
{
	(SdpServiceAttributePtr) ATTRNAME(hid, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(hid, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(hid, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hid, BrowseGroupList),
	(SdpServiceAttributePtr) ATTRNAME(hid, LangBaseAttributeIdList),
	(SdpServiceAttributePtr) ATTRNAME(hid, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hid, AddProtocolDescriptorLists),

    (SdpServiceAttributePtr) ATTRNAME(hid, VirtualCable),
    (SdpServiceAttributePtr) ATTRNAME(hid, ReconnectInitiate),
    (SdpServiceAttributePtr) ATTRNAME(hid, DescriptorListForIOS),

	(SdpServiceAttributePtr) ATTRNAME(hid, BootDevice),
	(SdpServiceAttributePtr) ATTRNAME(hid, SDPDisable),
	(SdpServiceAttributePtr) ATTRNAME(hid, DeviceReleaseNumber),
	(SdpServiceAttributePtr) ATTRNAME(hid, ParserVersion),
	(SdpServiceAttributePtr) ATTRNAME(hid, DeviceSubclass),
	(SdpServiceAttributePtr) ATTRNAME(hid, CountryCode),
	(SdpServiceAttributePtr) ATTRNAME(hid, ProfileVersion),
	(SdpServiceAttributePtr) ATTRNAME(hid, NormallyConnectable),
};
#else
ATTRDEF_ROM(hid, DescriptorList) =
     
     { ATTRID(ATTRID_DescriptorList), ATTRLEN(163), DEDEF_SEQU8(0xA1),
			                                                DEDEF_SEQU8(0x9F),
			                                                    DEDEF_U8(0x22), //0x08, 0x22,	    // class descriptor type = report		                                                    
     DEDEF_TEXT8(0x9B),   // the length of class descriptor data          	
	
	//*************************************************************
	// 3B = 59
	0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01, 0x75, 0x01, 0x95, 0x08, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01, 0x81, 0x02, 0x81, 0x01, 0x75, 0x01, 0x95, 0x05, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02, 0x95, 0x03, 0x91, 0x01, 0x75, 0x08, 0x95, 0x06, 0x26, 0xFF, 0x00, 0x05, 0x07, 0x19, 0x00, 0x2A, 0xFF, 0x00, 0x81, 0x00, 0xC0 ,
	//#KB Standard (66(dec)= 42(hex))
	/*
	
		0x05, 0x01,			//   USAGE_PAGE (Generic Desktop)
		0x09, 0x06,         //   USAGE (Keyboard)
		0xA1, 0x01,         //   COLLECTION (Application)
		0x85, 0x01,         //   REPORT_ID (1)
		0x75, 0x01,         //   REPORT_SIZE (1)
		0x95, 0x08,         //   REPORT_COUNT (8)
		0x05, 0x07,         //   USAGE_PAGE (Keyboard)
		0x19, 0xE0,         //   USAGE_MINIMUM (Keyboard LeftControl)
		0x29, 0xE7,         //   USAGE_MAXIMUM (Keyboard Right GUI)
		0x15, 0x00,         //   LOGICAL_MINIMUM (0)
		0x25, 0x01,         //   LOGICAL_MAXIMUM (1)
		0x81, 0x02,         //   INPUT (Data,Var,Abs)
		
		0x95, 0x01,         //   REPORT_COUNT (1)
		0x75, 0x08,         //   REPORT_SIZE (8)
		0x81, 0x03,         //   INPUT (Cnst,Var,Abs)
		
		0x95, 0x05,         //   REPORT_COUNT (5)
		0x75, 0x01,         //   REPORT_SIZE (1)
		0x05, 0x08,         //   USAGE_PAGE (LEDs)
		0x19, 0x01,         //   USAGE_MINIMUM (Num Lock)
		0x29, 0x05,         //   USAGE_MAXIMUM (Kana)
		0x91, 0x02,         //   OUTPUT (Data,Var,Abs)
		
		0x95, 0x01,         //   REPORT_COUNT (1)
		0x75, 0x03,         //   REPORT_SIZE (3)
		0x91, 0x03,         //   OUTPUT (Cnst,Var,Abs)
		
		0x95, 0x06,         //   REPORT_COUNT (6)
		0x75, 0x08,         //   REPORT_SIZE (8)
		0x15, 0x00,         //   LOGICAL_MINIMUM (0)
		0x26, 0xFF, 0x00,	//   LOGICAL_MAXIMUM (255)
		0x05, 0x07,         //   USAGE_PAGE (Keyboard)
		0x19, 0x00,         //   USAGE_MINIMUM (Reserved (no event indicated))
		0x29, 0xFF,         //   USAGE_MAXIMUM (Reserved (no event indicated))
		0x81, 0x00,			//   INPUT (Data,Ary,Abs)
		
		0xC0,            	// 	 END_COLLECTION
	  */                              
	//*************************************************************
     // 96 dec = (60 hex )
    0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x10, 0x19, 0x00, 0x2A, 0xFF, 0x03, 0x75, 0x0C, 0x95, 0x01, 0x15, 0x00, 0x26, 0xFF, 0x03, 0x81, 0x00, 0x05, 0x01, 0x09, 0x82, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x01, 0x81, 0x02, 0x75, 0x03, 0x95, 0x01, 0x81, 0x01, 0xC0, 0x06, 0xBC, 0xFF, 0x09, 0x88, 0xA1, 0x01, 0x85, 0x22, 0x06, 0x00, 0xFF, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x1A, 0x0A, 0x0A, 0xFA, 0xB1, 0x02, 0x85, 0x27, 0x06, 0x00, 0xFF, 0x75, 0x08, 0x95, 0x1F, 0x0A, 0x0A, 0xFA, 0x81, 0x02, 0x85, 0x24, 0x06, 0x00, 0xFF, 0x0A, 0x0A, 0xFA, 0xB1, 0x02, 0xC0 
   
		}; 
		
ATTRLISTDEF_ROM(hid) =
{
	(SdpServiceAttributePtr) ATTRNAME(hid, ServiceRecordHandle),
	(SdpServiceAttributePtr) ATTRNAME(hid, ServiceClassIdList),
	(SdpServiceAttributePtr) ATTRNAME(hid, ProtocolDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hid, BrowseGroupList),
	(SdpServiceAttributePtr) ATTRNAME(hid, LangBaseAttributeIdList),
	(SdpServiceAttributePtr) ATTRNAME(hid, ProfileDescriptorList),
	(SdpServiceAttributePtr) ATTRNAME(hid, AddProtocolDescriptorLists),

	(SdpServiceAttributePtr) ATTRNAME(hid, DescriptorList),

	//(SdpServiceAttributePtr) ATTRNAME(hid, ServiceName),
	
	//(SdpServiceAttributePtr) ATTRNAME(hid, DeviceReleaseNumber),
    (SdpServiceAttributePtr) ATTRNAME(hid, ParserVersion),
    (SdpServiceAttributePtr) ATTRNAME(hid, DeviceSubclass),
    (SdpServiceAttributePtr) ATTRNAME(hid, CountryCode),
    (SdpServiceAttributePtr) ATTRNAME(hid, VirtualCable),
    (SdpServiceAttributePtr) ATTRNAME(hid, ReconnectInitiate),

    
    
    (SdpServiceAttributePtr) ATTRNAME(hid, ProfileVersion),
    (SdpServiceAttributePtr) ATTRNAME(hid, SupervisionTimeout),
//    (SdpServiceAttributePtr) ATTRNAME(hid, NormallyConnectable),
    (SdpServiceAttributePtr) ATTRNAME(hid, BootDevice),  
    
};
#endif

SRECORDDEF_ROM(hid) =
{
	PROFILE_HID,
	ATTRCOUNT(hid),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(hid)
};

SdpServiceNode gHID_ServiceNode = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&hid),
	NULL
};

#ifdef SELFIE_App
SRECORDDEF_ROM(hidForIOS) =
{
	PROFILE_HID,
	ATTRCOUNT(hid),
	(SdpServiceAttributeListPtr) ATTRLISTNAME(hidForIOS)
};

SdpServiceNode gHID_ServiceNodeForIOS = 
{
	(SdpServiceRecordPtr) SRECORDNAME(&hidForIOS),
	NULL
};
#endif


#endif
