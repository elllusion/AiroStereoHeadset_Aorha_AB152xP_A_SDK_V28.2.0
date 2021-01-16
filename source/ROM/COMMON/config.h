#ifndef _CONFIG_H_
#define _CONFIG_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * config.h defines the configuration of this chip.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "boolean.h"
#include "ctlr_config.h"


////////////////////////////////////////////////////////////////////////////////
// CHIP MODEL DEFINITIONS //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * Chip Model is number of AIROHA chip number that is a value not more than 65536.
 *
 * CHIP MODEL LIST
 * 1520, 1602
 */
#define CHIP_MODEL                                                              1520

/**
 * Momery Configurations
 */
#if (CHIP_MODEL == 1520)
#ifdef AB1520D
#define XDATA_MEMORY_BEGIN                                                      0x6000
#define XDATA_MEMORY_SIZE                                                       0xA000
#else
#define XDATA_MEMORY_BEGIN                                                      0x8000
#define XDATA_MEMORY_SIZE                                                       0x8000
#endif
#elif (CHIP_MODEL == 1602)
#define XDATA_MEMORY_BEGIN                                                      0xC000
#define XDATA_MEMORY_SIZE                                                       0x2800
#endif

/**
 * Common Assembly Speedup Options
 */
#define FEA_BT_TIMER_SPEEDUP                                                    (TRUE)

/**
 * OS Assembly Speedup Options
 */
#define FEA_OSMEM_SPEEDUP                                                       (TRUE)
#define FEA_OSMEM_LIST_SPEEDUP                                                  (TRUE)
#define FEA_OSMEM_DLIST_SPEEDUP                                                 (TRUE)
#define FEA_OSTASK_SPEEDUP                                                      (FALSE)
#define FEA_OST_SPEEDUP                                                         (TRUE)

/**
 * RC
 */
#define FEA_SYS_SPEEDUP                                                         (TRUE)
#define FEA_BT_CLOCK_SPEEDUP                                                    (TRUE)
#define FEA_CRC16_SPEEDUP                                                       (FALSE)
#define FEA_SUPP_FLASH_PROGRAMMER                                               (TRUE && (CHIP_MODEL == 1520))
#define FEA_SUPP_HCI_SPI_TRANSPORT                                              (TRUE && (CHIP_MODEL == 1602))
#define FEA_SUPP_LAB_TEST                                                       (TRUE)
#define FEA_SUPP_RF_BIST                                                        (TRUE)
#define FEA_SUPP_SPI_SLAVE                                                      (FALSE)


/**
 * Driver Assembly Speedup Options
 */
#define FEA_3WIRE_SPEEDUP                                                       (TRUE)
#define FEA_DMA_SPEEDUP                                                         (TRUE)
#define FEA_GPIO_SPEEDUP                                                        (FALSE)

/**
 * DSP
 */
#define FEA_SUPP_DSP                                                            (CHIP_MODEL == 1520)
#define FEA_SUPP_ZSP410_EMBEDDED                                                (TRUE && FEA_SUPP_DSP)
#define	FEA_SUPP_I2S_DBG                                                        (TRUE && FEA_SUPP_DSP)

/**
 * LC
 */
#define FEA_LC_SPEEDUP                                                          (TRUE)

/**
 * NEW_OSMEM FEATUREs
 */
#define NUM_OF_SUPPORTED_LE_ACL_LINK                                            (2)
#define NUM_OF_SUPPORTED_BT_ACL_LINK                                            (4)
#define NUM_OF_SUPPORTED_ACL_LINK                                               (NUM_OF_SUPPORTED_LE_ACL_LINK + NUM_OF_SUPPORTED_BT_ACL_LINK)

/**
 * I2C
 */
#define FEA_SUPP_I2C                                                            (TRUE)

/**
 * Flash Programmer
 */
#define FEA_FLASH_WRITE_BY_CONTROLLER                                           (TRUE && (CHIP_MODEL == 1520))

/**
 * Watchdog
 */
#define FEA_SUPP_WDT                                                            (FALSE)

#define LC_BLACK_LIST

#define SUPVIS_TIMEOUT_BY_LC
//#define SUPVIS_TIMEOUT_BY_LM

//#define SAVING_RX_LARGE_MEMORY

/* Modify */
#define NEW_SNIFF_TRANSITION            1
#define NEW_LC_SNIFF_TRANSITION         1

//-----------------------------------------------------//

//#define USED_DEBUG_PIN_P0
//#define USED_DEBUG_PIN_P3

//- For BT Qualification
//#define LC_TEST_CASE_CONFIG
//#define CTL_MODE_TMP
#ifdef CTL_MODE_TMP
    #define HC_CMD_TESTER
#endif

//#define FOR_SERVICE_BEHAVIOR_TEST

/////////////////////////////////////////////////////////////////////////
// WARNNING!!!!
//  When EEPROM enabled, DONNOT define these two constant in SOC project
//  #define OPERATION_IN_BB_FPGA_VERSION
/////////////////////////////////////////////////////////////////////////
#define OPERATION_IN_BB_ASIC_VERSION
//#define OPERATION_IN_BB_FPGA_VERSION

//#define OPERATION_IN_BB_FPGA_AUDIO_ONOFF_PROC //RF may not work

#if defined(OPERATION_IN_BB_ASIC_VERSION)
#ifdef AB1520S
#define OPERATION_IN_RF_AB1520S
#else
#define OPERATION_IN_RF_AB1520
#endif
#elif defined(OPERATION_IN_BB_FPGA_VERSION)
//#define OPERATION_IN_RF_AB1520
#define OPERATION_IN_RF_AB1500F_FPGA
//#define OPERATION_IN_RF_AB1000D
//#define OPERATION_IN_RF_AB1500B
//#define OPERATION_IN_RF_AB1500G
#endif

#if (defined OPERATION_IN_RF_AB1500F_FPGA || defined OPERATION_IN_RF_AB1500G || defined OPERATION_IN_RF_AB1520 || defined OPERATION_IN_RF_AB1520S) // Change this definition if required
// For common power on procedure, some operatins are integrated into a new set of 3-wire commands
// => 3 wire write, RF calibration, 32K calibration
#define COMMON_3WIRE_RF_XTAL_CALIBRATION   //Ugly

#endif


#ifdef OPERATION_IN_BB_FPGA_VERSION
    #define VIRTEX6
#endif

#define UART_H5
#define HW_SPI_SUPPORTED			(TRUE && !(defined AB1520S))

#define Is_ExtFlash_Exist           (TRUE)

//#define FLASH_BOOT
//#define ASIC_DBG_PORT

//#define EXTERNAL_XO32K
//#define NEW_24KB_SRAM
#define SUPPORT_BROADCAST
#define LC_SEND_INITIAL_3WIRED_WHEN_EEPROM_INVALID
#define LL_SUPPORTED_LIGHTDBGPRINT
// OS Debug related define
#define OS_DEBUG
#define OS_TRAP_DBG
#define OS_ASSERT_DBG
#define OS_PRINT_ASSERT
#define OS_DSP_ALIGN_MEMORY_CHECK			(0)
#define OS_PRINT_OSMEM1_MSG					(1)

/*
 * NEW_OSMEM FEATUREs
 */
#define FEA_OSMEM_CHECKSUM                  0
#define FEA_DOUBLE_LINK_LIST                0


#define FEA_TREAT_DBGPRINT_AS_HCI_EVENT     (1)
#define FEA_NEW_CONNHDL                     (0)
#define FEA_NEW_CHIPON_INIT                 (0)



#define OS_AUTO_ENABLE_POWER_SAVING
#define SUPPORT_FAST_OS_FUNC

/* HW Workaround */
#if defined OPERATION_IN_BB_ASIC_VERSION
#define LO_CALIBRATION_WORKAROUND
#endif


#define SINGLE_RX_BUF_ONLY
#define RESERVED_AREA_FOR_UNUSED_RX_BUF                 (0xFFF0)

#ifdef SINGLE_RX_BUF_ONLY
	#define AB1520B_LC_RX_BUFFER_WORKAROUND             (0)
	#define AB1520C_LC_RX_BUFFER_WORKAROUND             (0)
#else
    #if (defined AB1520B)
    #define AB1520B_LC_RX_BUFFER_WORKAROUND             (1)
    #define AB1520C_LC_RX_BUFFER_WORKAROUND             (0)
    #else
    #define AB1520B_LC_RX_BUFFER_WORKAROUND             (0)
    #define AB1520C_LC_RX_BUFFER_WORKAROUND             (1)
    #endif
#endif



/*
 * OSMEM Configuration
 */




// LC LM related define
#define SUPPORT_HOLD
#define SUPPORT_CHANNEL_ASSESSMENT
//#define SUPPORT_QOS
//#define SUPPORT_MASTER_BC
#if (defined(__C251__) || defined(__A251__))
	#ifndef AB1520S
    #define LE_SUPPORTED_HOST
	#define LE_SUPPORTED_HOST_FLASH
	#endif
    //#define SM_SUPPORTED
    //#define AIR_UPDATE_ENABLE
#else
    #define LE_SUPPORTED_HOST
#endif

#if (LE_SUPPORTED_CONTROLLER)
#define LE_ADV_DELAY_SUPPORTED
#define LE_ADV_ANCHOR_UPDATE_SUPPORTED
#define LE_TX_SPEEDUP_SUPPORTED
#define LE_TX_MIC_WORKAROUND
#endif
// HC related define
#define SUPPORT_TRANSPORT
//#define UART_ERROR_AND_ASSERT
//#define UART_NOISE_FILTER

#define SUPPORT_MULTI_POINT
#define SUPPORT_SCO_ESCO

#ifdef SOUND_BAR
    #ifndef CELT_MODULE
        #define SUPPORT_1TO2AG_SOUNDBAR
    #endif
#endif

#define SUPPORT_SIMPLE_PAIRING

#define LABTEST_ENABLED
#define INQUIRY_SUPPORT

//#define HC_FLOW_STOP_BY_LM //for ACL rx flow control during role switch

#define MAX_NUM_LE_LINK 2

#if (FEA_NEW_CONNHDL)
#define MAX_NUM_ACL_LINK 2
#else
#define MAX_NUM_ACL_LINK 4
#endif
#define MAX_NUM_SCO_LINK 2 //total sco no of all acl links
#define MAX_NUM_SCO_LINK_OF_EACH_ACL 2 //the max sco no of each acl
//#define ESCO_SUPPORT_MAX_PKT_LEN  90
#define ESCO_SUPPORT_MAX_PKT_LEN    360//BT_MAX_LENGTH_3EV5
#define LE_MAX_PKT_LEN              39


#ifdef SUPPORT_TRANSPORT
	#define SUPPORT_UART
#endif

#ifdef SUPPORT_MULTI_POINT
	#define SUPPORT_2_SLAVE
	#ifdef SUPPORT_1TO2AG_SOUNDBAR
	    #define MAX_MULTI_POINT_NO 3
	#else
	#define MAX_MULTI_POINT_NO 2
	#endif
#else
	#define MAX_MULTI_POINT_NO 1
#endif

// *************************** IMPORTANT************************************
// NOTICE - Structure definition matches specified flash code version.
//          DO NOT modify MAX_MULTI_POINT_NO_FOR_SECTOR.
// *************************** IMPORTANT************************************
#define MAX_MULTI_POINT_NO_FOR_SECTOR 4

#ifdef SUPPORT_TRANSPORT
    #ifdef SUPPORT_UART
        #define DMA_UART
    #endif
        //#define SUPPORT_DONGLE_HC_CMD_EVT
    #define AIROHA_HCI_CMD_EVT
#endif

//WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!
///////////////////////////////////////////////////////////
//  undefine section between AB1000 and AB1000S
///////////////////////////////////////////////////////////
//WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!


//WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!
///////////////////////////////////////////////////////////
//  For Test Case ONLY
///////////////////////////////////////////////////////////
//WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!
#ifdef  LC_TEST_CASE_CONFIG
    #define SUPPORT_BC_SEGMENT
    #define LM_NO_ACT_MAXSLOT
#endif

#ifdef LC_TEST_CASE_CONFIG
    #define LC_DEFAULT_POWER_LEVEL  0x28
    #define LC_MAX_POWER_LEVEL      0x2B
    #define LC_MIN_POWER_LEVEL      0x25
#else
    #define LC_DEFAULT_POWER_LEVEL  0x28    //- can't modify
#endif

//WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!
///////////////////////////////////////////////////////////
//  BEGIN: UNIT Test Code
///////////////////////////////////////////////////////////
//WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!

#ifdef _UTEST_
//#define _UTEST_OS_C_
//#define _UTEST_OVERLAY_
//#define _UTEST_OST_C_
#endif

//WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!
///////////////////////////////////////////////////////////
//  END: UNIT Test Code
///////////////////////////////////////////////////////////
//WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!WARNNING!!


	//#define DBG_UART_ENABLE_MMI_LE_ROM
	//#define DBG_UART_ENABLE_L2CAP_ROM
	//#define DBG_UART_ENABLE_GAP_ROM
	//#define DBG_UART_ENABLE_DES
	//#define DBG_UART_ENABLE_ATT_AU
	//#define DBG_UART_ENABLE_ATT



	//#define DBG_UART_ENABLE_LM
	//#define DBG_UART_ENABLE_L2CAP
	//#define DBG_UART_ENABLE_GAP



	//#define DBG_UART_ENABLE_SDP
	//#define DBG_UART_ENABLE_RFC
	//#define DBG_UART_ENABLE_OPP
	//#define DBG_UART_ENABLE_MMI
	//#define DBG_UART_ENABLE_MAP
	//#define DBG_UART_ENABLE_DRV
	//#define DBG_UART_ENABLE_AVRCP
	//#define DBG_UART_ENABLE_AVDTP
	//#define DBG_UART_ENABLE_AVCTP
	//#define DBG_UART_ENABLE_HFP
	//#define DBG_UART_ENABLE_A2DP


	//#define DBG_UART_ENABLE_MMI_LE
	//#define DBG_UART_ENABLE_LINK_INDEX_CTL
	//#define DBG_UART_ENABLE_SM


#endif /* _CONFIG_H_ */
