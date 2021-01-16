#ifndef _CTLR_CONFIG_H_
#define _CTLR_CONFIG_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * ctlr_config.h defines the controller configuration.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "boolean.h"



/**
 * Configuration of controller types
 */
#define BR_EDR_SUPPORTED_CONTROLLER                                             (TRUE)
#ifdef AB1520S
#define LE_SUPPORTED_CONTROLLER                                                 (FALSE)
#else
#define LE_SUPPORTED_CONTROLLER                                                 (TRUE)
#endif
#define DUAL_SUPPORTED_CONTROLLER                                               (BR_EDR_SUPPORTED_CONTROLLER && LE_SUPPORTED_CONTROLLER)

#if (!BR_EDR_SUPPORTED_CONTROLLER && !LE_SUPPORTED_CONTROLLER)
#error "AIROHO Config Error : At least select of one between BT and BLE."
#endif

#endif /* _CTLR_CONFIG_H_ */

