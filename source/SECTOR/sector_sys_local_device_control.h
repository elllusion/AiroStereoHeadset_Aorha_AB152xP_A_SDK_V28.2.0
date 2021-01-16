#ifndef _SECTOR_SYS_LOCAL_DEVICE_CONTROL_H_
#define _SECTOR_SYS_LOCAL_DEVICE_CONTROL_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * sector_sys_local_device_control.h defines the control options of device.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "rc.h"
#include "crc16.h"
#include "bt_config_profile.h"
#include "drv_led.h"



////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- feature 0
#define SYS_MISC_SUPP_FEA0_HID_TESTMODE_SUPPORT_3_5_EDR     BIT_MASK8(2)
#define SYS_MISC_SUPP_FEA0_CUSTOMER_CHARGER                 BIT_MASK8(6)

typedef struct stru_sys_misc_ctl0
{
    U8  _reserved_bit0                      : 1;
    U8  _reserved_bit1                      : 1;
    U8  supp_hid_testmode_support_3_5_edr   : 1;
    U8  _reserved_bit3                      : 1;
    U8  _reserved_bit4                      : 1;
    U8  _reserved_bit5                      : 1;
    U8  supp_customer_charger               : 1;
    U8  _reserved_bit7                      : 1;

} SYS_MISC_CTL0_STRU;

typedef union union_sys_misc_ctl0
{
    U8  value;
    SYS_MISC_CTL0_STRU field;

} SYS_MISC_CTL0_UNION;

//- feature 1
typedef struct stru_sys_misc_ctl1
{
    U8  _reserved_bit0                      : 1;
    U8  _reserved_bit1                      : 1;
    U8  _reserved_bit2                      : 1;
    U8  _reserved_bit3                      : 1;
    U8  _reserved_bit4                      : 1;
    U8  _reserved_bit5                      : 1;
    U8  _reserved_bit6                      : 1;
    U8  _reserved_bit7                      : 1;

} SYS_MISC_CTL1_STRU;

typedef union union_sys_misc_ctl1
{
    U8  value;
    SYS_MISC_CTL1_STRU field;

} SYS_MISC_CTL1_UNION;

//- feature 2
#define SYS_MISC_SUPP_FEA2_FORCE_BT_CONTROLLER_MODE BIT_MASK8(0)
#define SYS_MISC_SUPP_FEA2_NOT_ALLOW_RST_BY_HCI_CMD BIT_MASK8(1)

typedef struct stru_sys_misc_ctl2
{
    U8  force_bt_controller     : 1;
    U8  bypass_hci_reset        : 1;    /* Just for ICE debugging */
    U8  Rsvd2                   : 1;
    U8  Rsvd3                   : 1;
    U8  Rsvd4                   : 1;
    U8  Rsvd5                   : 1;
    U8  Rsvd6                   : 1;
    U8  Rsvd7                   : 1;

} SYS_MISC_CTL2_STRU;

typedef union union_sys_misc_ctl2
{
    U8  value;
    SYS_MISC_CTL2_STRU field;

} SYS_MISC_CTL2_UNION;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct      //- size is same as SYS_AUDIO_CTL
{
    TWO_BYTE_UNION_TYPE adc_of_boost_in_deep_sleep;
    U8                  bak[6];
} SYS_BACKUP_CONTROL_INFO;


//- FB 64~77
#define SYS_HW_GPIO_CTRL_WORD               (14+1)//1 for wakeup

typedef union
{
    U8 gpio_ctl[SYS_HW_GPIO_CTRL_WORD];
    struct
    {
        U8  Gpio_O_B0;
        U8  Gpio_O_B1;
        U8  Gpio_O_B2;
        U8  Gpio_O_B3;
        U8  Gpio_OE_B0;
        U8  Gpio_OE_B1;
        U8  Gpio_OE_B2;
        U8  Gpio_OE_B3;
        U8  Gpio_I_Inv_En_B0;
        U8  Gpio_I_Inv_En_B1;
        U8  Gpio_I_Inv_En_B2;
        U8  Gpio_I_Inv_En_B3;
        U8  Gpio_Mux; // invalid in 1520
        U8  Gpio_Tpout_Ctrl; // invalid in 1520
        U8  Wakeup_inv_en;
    } in_sfr_name;

} SYS_GPIO_CONTROL_INFO;


typedef struct
{
    U8 grp_sel_b0;
    U8 grp_sel_b1;
    U8 grp_sel_b2;
    U8 sig_sel_0304;
    U8 sig_sel_0506;
    U8 sig_sel_0708;
    U8 sig_sel_0910;
    U8 sig_sel_1112;
    U8 sig_sel_1314;
    U8 sig_sel_1516;
    U8 sig_sel_1718;
    U8 sig_sel_1920;
    U8 sig_sel_2122;
    U8 sig_sel_2324;
    U8 sig_sel_25;
    U8 dbg_sel_0to3;
    U8 dbg_sel_4;
    U8 led_scl_en;
    U8 dmic_di0_sel;
    U8 dmic_di1_sel;
    U8 i2s0_di_sel;
    U8 i2s1_di_sel;
    U8 uart_ncts_sel;
}GPIO_MUX_CTL;

#define GPIO_MUX_NUM    23
typedef union
{
    U8 gpio_mux_byte[GPIO_MUX_NUM];
    GPIO_MUX_CTL gpio_mux_ctl;
}GPIO_MUX_SEL_UNION;


typedef struct
{
    U16                         os_power_saving_waiting_timer_in_half_slot;
    SYS_MISC_CTL0_UNION         misc_ctl0;
    SYS_MISC_CTL1_UNION         misc_ctl1;
    SYS_MISC_CTL2_UNION         misc_ctl2;

    SYS_GPIO_CONTROL_INFO       gpio_control_info;

    U8                          reserved2;
    U8                          reserved3;
    U8                          bak[3];


    U8                          reserved;
    U8                          WatchDogTimer;
    U8 powerOnLedEnable;
    LedPatternType powerOnLed;
    U8 powerOnLedDisable;

    U8 tpout_Sel;
    GPIO_MUX_SEL_UNION          gpio_mux_sel;

} SYS_LOCAL_DEVICE_CONTROL_STRU;

typedef struct stru_sector_sys_local_device_control
{
    SYS_LOCAL_DEVICE_CONTROL_STRU SysLocalDeviceControl;

    CRC16 CRC;

} SECTOR_SYS_LOCAL_DEVICE_CONTROL_STRU;


#endif /* _SECTOR_SYS_LOCAL_DEVICE_CONTROL_H_ */
