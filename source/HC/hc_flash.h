#ifndef _HC_FLASH_H
#define _HC_FLASH_H
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * hc.h is the file defines the function prototypes of HC.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"
#include "hc.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    //HC_EVENT_MASK_TYPE      event_mask;
    //HC_EVENT_FILTER_TYPE    event_filter[HCI_NUM_EVENT_FILTER];
    HC_DATA_LINK_CTL_TYPE   data_link_ctl[MAX_NUM_ACL_LINK+1]; //- include broadcast
    //#warning "remove this"
    U8                      num_available_acl_data_buffer;

    #ifdef SUPPORT_SCO_ESCO
    U8                      erroneous_data_reporting; //2009.03.20 PHLO
    #endif

    U8                      num_inquiry_responses_wanted;
    U8                      num_inquiry_responses;

    HC_MISC_CTL_B0          misc_ctrl_B0;

} HC_FLASH_CTL_TYPE;


#define MAX_NUM_KEYS    2

typedef struct stru_hc_bdaddr_link_key
{
    U8 Used;
    LINK_KEY_INFO_STRU HcLinkKeyInfo;

} HC_BDADDR_LINK_KEY_STRU;


////////////////////////////////////////////////////////////////////////////////
// External Global Variables ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN HC_FLASH_CTL_TYPE XDATA gHC_ctl_FLASH;


////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * HC_FLASH_Init
 *
 * HC_Init
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HC_FLASH_Init(VOID);

/**
 * HC
 *
 * HC_Init
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 */
EXTERN VOID HC(VOID);

VOID HC_AclVCmdHandler(VOID);
#endif /* _HC_FLASH_H */


