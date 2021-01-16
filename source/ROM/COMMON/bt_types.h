#ifndef _BT_TYPES_H_
#define _BT_TYPES_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * bt_types.h is the file defines the types which is mentioned in SPEC.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "config.h"
#include "cpu.h"
#include "os_types.h"
#include "ctlr_config.h"
#include "bt_const.h"
#include "types.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
 * Bluetooth Device Address
 */
typedef union
{
    struct
    {
        U8 LAP[3];
        U8 UAP[1];
        U8 NAP[2];
    } group;
    U8 member[6];
} BD_ADDR;

typedef U8 LAP_ARR[3];
typedef U8 UAP;
typedef U8 NAP_ARR[2];

typedef struct stru_bd_addr
{
    LAP_ARR Lap;
    UAP Uap;
    NAP_ARR Nap;

} BD_ADDR_STRU, OS_PTR BD_ADDR_STRU_PTR;

typedef U8 BD_ADDR_ARR[6];

typedef U8 PARITY_ARR[5];

typedef struct
{
    U8 PARITY_B0;
    U8 PARITY_B1;
    U8 PARITY_B2;
    U8 PARITY_B3;
    U8 PARITY_B4;
} PARITY;

#define MAX_EIR_SIZE    240
typedef U8 EIR_ARR[MAX_EIR_SIZE];

typedef S8 RSSI;

typedef U8 FHS_ARR[18];

/*
 * Link Key or Long Term Key
 */
typedef U8 LINK_KEY_ARR[16];

/*
 * Key Information
 */
typedef struct stru_link_key_info
{
    BD_ADDR_STRU BdAddr;
    LINK_KEY_ARR LinkKey;

} LINK_KEY_INFO_STRU;

/*
 * Class Of Device
 */
typedef U8 COD_ARR[3];

/*
 * Bluetooth 3.0 Channel Map
 */
typedef U8 BT_CH_MAP_ARR[10];

/*
 * Bluetooth 4.0 Channel Map
 */
typedef U8 LE_CH_MAP_ARR[5];

typedef U32 BTCLK;
typedef U8 BTCLK_ARR[4];

typedef struct stru_btclk
{
    U8 B3;
    U8 B2;
    U8 B1;
    U8 B0;

} BTCLK_STRU;

typedef union union_btclk
{
    BTCLK value;
    BTCLK_ARR byte;
    BTCLK_STRU field;

} BTCLK_UNION;


typedef U8 LMP_FEATURES_ARR[8];


#define D4_LINK_SUPERVISION_TIMEOUT 0xFA00UL


/* Bluetooth Clock Constant */
#define BTCLK_FULL_ROUND_TIME 0x10000000UL
#define BTCLK_HALF_ROUND_TIME 0x08000000UL
#define BTCLK_27_0_MASK 0x0FFFFFFFUL
#define BTCLK_27_2_MASK 0x0FFFFFFCUL



#if (BR_EDR_SUPPORTED_CONTROLLER)
#define NUM_OF_PAGES_OF_LMP_SUPPORTTED_FEATURES     (2)
#elif (LE_SUPPORTED_CONTROLLER)
#define NUM_OF_PAGES_OF_LMP_SUPPORTTED_FEATURES     (2)
#endif

typedef BYTE_UNION SUPPORTTED_FEATURES[8];
typedef BYTE_UNION SUPPORTTED_STATES[8];

typedef struct stru_lmp_version_info
{
    U8 Version;
    U8 CompIdB0;
    U8 CompIdB1;
    U8 SubversionB0;
    U8 SubversionB1;

} VERSION_INFO;

typedef U8 COMP_ID[2];
typedef U8 LMP_SUBVERSION[2];

typedef U8 HCI_REVISION[2];


typedef U8 ACCESS_ADDR_ARR[4];
typedef U8 LE_CRC_ARR[3];



////////////////////////////////////////////////////////////////////////////////
// Constant Defintions /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define SIZE_OF_ASIC_VERSION    10



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 ASIC_VERSION[SIZE_OF_ASIC_VERSION];



/* For Vender Command */
////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* AFE Init */
typedef struct stru_vcmd_afe_init
{
    U8 B0;
    U8 B1;
    U8 Addr;

} VCMD_AFE_INIT_STRU;

typedef struct stru_vcmd_sfr_init
{
    U8 Data;
    U8 Offset;
    U8 Bank;

} VCMD_SFR_INIT_STRU;

typedef struct stru_vcmd_delay_init
{
    U8 B0;
    U8 B1;
    U8 Tag;

} VCMD_DELAY_INIT_STRU;

typedef struct stru_vcmd_generic_init
{
    U8 B0;
    U8 B1;
    U8 B2;

} VCMD_GENERIC_INIT_STRU;

typedef union u_vcmd_init
{
    VCMD_AFE_INIT_STRU AfeInit;
    VCMD_SFR_INIT_STRU SfrInit;
    VCMD_DELAY_INIT_STRU DelayInit;
    VCMD_GENERIC_INIT_STRU GenericInit;

} VCMD_INIT_UNION;

typedef union
{
    struct
    {
        U8 LmpVersion;
        U8 CompIdB0;
        U8 CompIdB1;
        U8 LmpSubVersNrB0;
        U8 LmpSubVersNrB1;
    } group;
    U8  member[5];
}SUPPORT_VERSION;

typedef struct
{
    U8 b0:1;
    U8 b1:1;
    U8 b2:1;
    U8 b3:1;
    U8 b4:1;
    U8 b5:1;
    U8 b6:1;
    U8 b7:1;
}BIT_GROUP;

typedef union
{
    struct
    {
        U8  B0;
        U8  B1;
        U8  B2;
        U8  B3;
        U8  B4;
        U8  B5;
        U8  B6;
        U8  B7;
    } group;
    U8  member[8];
    BIT_GROUP  bit_group[8];
}SUPPORT_FEATURES;


#endif /* _BT_TYPES_H_ */

