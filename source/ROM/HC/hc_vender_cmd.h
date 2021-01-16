#ifndef _HC_VENDER_CMD_H_
#define _HC_VENDER_CMD_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * hc_vender_cmd.h is the file defines the function prototypes of HC Vender
 * command.
 *
 * Programmer : WenChiehLo@airoha.com.tw, Ext.3374
 */
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////// /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * HC_FlashAclVCmdHdlr
 *
 * Transport flash data from UART to HC.
 *
 * @Author : DingaCheng <DingaCheng@airoha.com.tw>
 *
*/
EXTERN VOID HC_FlashAclVCmdHdlr(VOID);

#endif /* _HC_VENDER_CMD_H_ */

