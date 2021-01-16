#ifndef _SECTOR_UART_PARAMETER_H_
#define _SECTOR_UART_PARAMETER_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_uart_para.h defines the data structure of UART parameters.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//-
typedef union union_uart_lcr
{
    struct
    {
        U8  word_length         : 2;
        U8  num_of_stop_bits    : 1;
        U8  parity_enable       : 1;
        U8  parity              : 1;
        U8  stick_parity_enable : 1;
        U8  reserved_b6         : 1;
        U8  hw_flow_ctl         : 1;

    } field;
    U8 value;

} UART_LINE_CTL_UNION;
//-
typedef struct stru_uart_para
{
    U32     Baudrate;

    //- Dma mode configuration
    U16     RxTrigLv;
    U8      RxCharTimeout;

    //- Line Control
    UART_LINE_CTL_UNION LineCtl;

    //- H5 Configuration
    U8      uart_h5_enabled;
    U32     uart_h5_session_timeout;
    U8      uart_h5_config_setting;

} UART_PARAMETER_STRU;

typedef struct stru_sector_uart_para
{
    UART_PARAMETER_STRU UartParameter;

    CRC16 CRC;

} SECTOR_UART_PARAMETER_STRU;


#endif /* _SECTOR_UART_PARAMETER_H_ */

