#ifndef _HC_TYPES_H_
#define _HC_TYPES_H_

#include "os.h"
#include "bt_types.h"


/* The size of HCI event mask is defined as 8 in SPEC */
typedef U8 HCI_EVT_MASK_ARR[8];



typedef struct
{
    U8  valid;
    union
    {
        struct
        {
            U8              addr_type;
            BD_ADDR_STRU    addr;
        } group;
        U8 member[7];
    } cmd_param;

} LE_WHITE_LIST_CONTENT;

//- LE related constant
#define TOTAL_NUM_OF_WHITE_LIST_ENTRIES     2
#define LENGTH_OF_LE_CHANNEL_MAP            5

typedef struct
{
    BD_ADDR_STRU                random_device_addr;
    LE_WHITE_LIST_CONTENT       white_list[TOTAL_NUM_OF_WHITE_LIST_ENTRIES];

} HC_LE_PARAM;

typedef struct
{
    // page read
    FOUR_BYTE_UNION_TYPE    read_flash_addr;
    TWO_BYTE_UNION_TYPE     read_length;
} HC_VCMD_FLASH_CONTROL;

typedef struct
{
    U8 XDATA_PTR dpc_ptr;
    BTCLK dpc_invl;

} HC_VCMD_DPC_CTRL;

typedef struct
{
    HC_VCMD_DPC_CTRL tempcomp_ctrl;
    HC_VCMD_DPC_CTRL reportevt_ctrl;
    HC_VCMD_DPC_CTRL rdadc0_ctrl;

} HC_VCMD_DPC_HANDLE;

typedef struct
{
    TWO_BYTE_UNION_TYPE DAC4V2;
    U8 DACStep;
    U8 ThredCnt;
    U8 ThredCntBackUp;
    TWO_BYTE_UNION_TYPE ThredViChg;
    U32 RdVbatTotal;
    U32 RdVbatCnt;

} HC_VCMD_CAL_4V2_DAC_TEST_PARAM;

typedef struct
{
    HC_VCMD_DPC_HANDLE dpc_handle;
    HC_VCMD_CAL_4V2_DAC_TEST_PARAM cal_4v2_dac_para;

} HC_VCMD_LABTEST_CONTROL;


typedef struct
{
    U8 CalGain;

} HC_VCMD_RF_BIST_PARA;



#endif /* _HC_TYPES_H_ */

