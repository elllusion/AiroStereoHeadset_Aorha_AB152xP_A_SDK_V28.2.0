#ifndef _SECTOR_SYS_LOCAL_DEVICE_INFO_H_
#define _SECTOR_SYS_LOCAL_DEVICE_INFO_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sys_local_device_info.h defines the local device informations.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    SUPPORT_FEATURES        LE_feature;
    SUPPORT_FEATURES        LE_state;
    SUPPORT_VERSION         LE_version;

} LE_INFO_STRU;

typedef union union_ext_flash_ctrl
{
    U8 value;
	struct stru_ext_flash_ctrl
	{
		U8 EnableExtFlashOtaUpdate  : 1;
		U8 Rsvd1                    : 1;
		U8 Rsvd2                    : 1;
        U8 Rsvd3                    : 1;
        U8 Rsvd4                    : 1;
        U8 Rsvd5                    : 1;
        U8 Rsvd6                    : 1;
        U8 Rsvd7                    : 1;

	} field;

} EXT_FLASH_CTRL_UNION;

typedef struct
{
    U8                      asic_version;
    U8                      class_of_device[3];

    HCI_VERSION             HciVersion;
    HCI_REVISION            HciRevision;

    #if (BR_EDR_SUPPORTED_CONTROLLER || LE_SUPPORTED_CONTROLLER)
    LMP_VERSION             LmpVersion;
    COMP_ID                 LmpCompId;
    LMP_SUBVERSION          LmpSubVersNr;
    #endif

	#if (DUAL_SUPPORTED_CONTROLLER || BR_EDR_SUPPORTED_CONTROLLER || LE_SUPPORTED_CONTROLLER)
    SUPPORTTED_FEATURES     LmpFeatures[NUM_OF_PAGES_OF_LMP_SUPPORTTED_FEATURES];
	#endif

    SUPPORT_FEATURES        LE_feature;
    #if 1
    SUPPORT_FEATURES        LE_state;
    #endif
    SUPPORT_VERSION         LE_version;

    //#warning "remove currentMaxPage"
    U8      currentMaxPage;

	#if !((defined AB1520SB) && (defined AB1520SC) && (defined AB1520SD)) //ROM code not support
    EXT_FLASH_CTRL_UNION      ExtFlashCtrl;
	#endif

} SYS_LOCAL_DEVICE_INFO_STRU;

typedef struct stru_sector_sys_local_device_info
{
    SYS_LOCAL_DEVICE_INFO_STRU SysLocalDeviceInfo;

    CRC16 CRC;

} SECTOR_SYS_LOCAL_DEVICE_INFO_STRU;


#endif /* _SECTOR_SYS_LOCAL_DEVICE_INFO_H_ */
