#ifndef _BT_CONST_H_
#define _BT_CONST_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * bt_const.h is the file defines the constants of Bluetooth CORE specification
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 LMP_VERSION;
#define LMP_VERSION_OF_BT_CORE_SPEC_10B     ((LMP_VERSION)(0))
#define LMP_VERSION_OF_BT_CORE_SPEC_11      ((LMP_VERSION)(1))
#define LMP_VERSION_OF_BT_CORE_SPEC_12      ((LMP_VERSION)(2))
#define LMP_VERSION_OF_BT_CORE_SPEC_20EDR   ((LMP_VERSION)(3))
#define LMP_VERSION_OF_BT_CORE_SPEC_21EDR   ((LMP_VERSION)(4))
#define LMP_VERSION_OF_BT_CORE_SPEC_30HS    ((LMP_VERSION)(5))
#define LMP_VERSION_OF_BT_CORE_SPEC_40      ((LMP_VERSION)(6))
#define LMP_VERSION_OF_BT_CORE_SPEC_41      ((LMP_VERSION)(7))

typedef U8 HCI_VERSION;
#define HCI_VERSION_OF_BT_CORE_SPEC_10B     ((HCI_VERSION)(0))
#define HCI_VERSION_OF_BT_CORE_SPEC_11      ((HCI_VERSION)(1))
#define HCI_VERSION_OF_BT_CORE_SPEC_12      ((HCI_VERSION)(2))
#define HCI_VERSION_OF_BT_CORE_SPEC_20EDR   ((HCI_VERSION)(3))
#define HCI_VERSION_OF_BT_CORE_SPEC_21EDR   ((HCI_VERSION)(4))
#define HCI_VERSION_OF_BT_CORE_SPEC_30HS    ((HCI_VERSION)(5))
#define HCI_VERSION_OF_BT_CORE_SPEC_40      ((HCI_VERSION)(6))
#define HCI_VERSION_OF_BT_CORE_SPEC_41      ((HCI_VERSION)(7))

#endif /* _BT_CONST_H_ */

