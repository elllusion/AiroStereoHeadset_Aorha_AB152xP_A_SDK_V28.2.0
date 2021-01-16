#ifndef _HCI_AIROHA_FLASH_H_
#define _HCI_AIROHA_FLASH_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * HCI_airoha.h is the file defines the constants and command/event format
 * of Airoha proprietory.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 * Programmer : DingaCheng@airoha.com.tw, Ext.2828
 * Programmer : WenChiehLo@airoha.com.tw, Ext.3374
 */
#include "rc.h"
#include "hci.h"
#include "sector_rf_misc_ctl.h"
#include "sector_mp_parameter.h"
#include "rc_flash.h"
#include "hci_airoha.h"


/*
 * 48.F5 AIRAPP_CMD_GET_DEVICE_NAME
 */
typedef struct
{
	BD_NAME local_name;
}HCI_VCMD_PARA_AIRAPP_CMD_GET_DEV_NAME;

#endif /* _HCI_AIROHA_FLASH_H_ */

