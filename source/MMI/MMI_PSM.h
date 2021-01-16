#ifndef __MMI_PSM_H__
#define __MMI_PSM_H__

enum
{
	PSM_NULL	= 0x0000, // internal use only
 	PSM_SDP		= 0x0001,
	PSM_RFCOMM	= 0x0003,
	PSM_HID_CTL	= 0x0011,
	PSM_HID_INT	= 0x0013,
	PSM_AVCTP	= 0x0017,
	PSM_AVDTP	= 0x0019,
}; // PSM should be odd number

#endif //__MMI_PSM_H__
