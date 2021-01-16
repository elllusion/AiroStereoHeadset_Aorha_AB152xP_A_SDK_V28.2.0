/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sys_local_device_EIR.c defines the local device EIR.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */

#include "sector_sys_local_device_EIR.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_SYS_LOCAL_DEVICE_EIR_STRU CODE gSector_SysLocalDeviceEIR =
{

	{//- SYS_LOCAL_DEVICE_EIR_STRU
	#if defined(HID_SOC)
		0x00,
		0x0C,
		0x00,
		0x00,
		{
			//Data format(Total n octets) : [Length:1 octet][Data: n - 1 octets]
			//Name
			0x07, 0x09,  'A',  'I',  'R',  'O',  'H',  'A',

			//Service Class UUID
			0x03, 0x03, 0x24, 0x11
		}
	#else
		0x01, //- DM1 prefer
		0x08,
		0x00,
		0x00,
		{
			//Service Class UUID
			//- 0x1108: Headset
			//- 0x110B: Audio_Sink
			//- 0x110C: AVRCP Target
			//- 0x110E: AVRCP Control
			//- 0x110F: AVRCP ControlController
			//- 0x111E: HFP

			//Data format(Total n octets) : [Length:1 octet][Data: n - 1 octets]
			//Name
			0x01, 0x09, //this element will be construct from runtime
		}
	#endif
	},

    //- CRC
    {0,0}
};