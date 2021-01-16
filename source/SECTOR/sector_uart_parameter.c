/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_uart_para.c defines the options of UART transport interface.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "sector_uart_parameter.h"
#include "drv_uart.h"
#include "bt_config_profile.h"

#pragma userclass (HCONST = CONFIG)


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SECTOR_UART_PARAMETER_STRU CODE gSector_UartParameter =
{
    {
        /**
         * NOTATION
         *
         * 1520 only provides baudrate in following list
         * 115200, 230400, 460800, 614400, 921600, 3000000
         */
        //- Baudrate
		#ifdef TRSPX_App
		115200UL,
		#else
        3000000UL,
		#endif        

        //- RxTrigLv
        0x20,

        //- RxCharTimeout
        0x20,

        {
            //- word_length
            UART_8_BIT_WORD_LENGTH,

            //- num_of_stop_bits
            UART_1_STOP_BIT,

            //- parity_enable
            UART_PARITY_OFF,

            //- parity
            UART_ODD_PARITY,

            //- stick_parity_enable
            UART_STICK_PARAITY_OFF,

            //- reserved_b6
            0,

            //- hw_flow_ctl
            #ifdef TRSPX_App
                UART_HW_FLOW_CTL_ON,
            #elif defined KB_App
                UART_HW_FLOW_CTL_ON,
            #else
                UART_HW_FLOW_CTL_OFF,
            #endif
        },

		//U8 uart_h5_enabled
        0,
        //U32     uart_h5_session_timeout;
        30*3200L,
        //U8      uart_h5_config_setting;
        0x07 |     // window size (1~7)
        0x00 <<3 |  // OOF (0-1)
        0x01 <<4 |  // DIC type (0-1)
        0x00 <<5 |	// version (default:0)
				0,
    },

    //- CRC
    {0,0}
};

