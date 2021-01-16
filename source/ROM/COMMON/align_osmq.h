#ifndef _ALIGN_OSMQ_H_
#define _ALIGN_OSMQ_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * align.h defines a common data structure placed from memory start
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"
#ifndef __C251__
#include "os_mq_format.h"
#endif
#include "ctlr_config.h"


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    //- Input Queue of Kerner Task
    OSMQ OSMQ_HCI_ACL_Data_To_Host[NUM_OF_SUPPORTED_ACL_LINK];      /* PDU to Host */
    OSMQ OSMQ_HCI_Event_To_Host;
    OSMQ OSMQ_HCI_Event_To_Xport;

    //- Input Queue of HC Task
    OSMQ OSMQ_HCI_Command_From_Host;

    OSMQ OSMQ_HCI_Command_To_LC[NUM_OF_SUPPORTED_ACL_LINK];
    //- Input Queue of LM/LL Task
    OSMQ OSMQ_HCI_Command_Relay_To_LM[NUM_OF_SUPPORTED_ACL_LINK];
    OSMQ OSMQ_LMP_Data_To_LM[NUM_OF_SUPPORTED_ACL_LINK];
    #if (LE_SUPPORTED_CONTROLLER)
    OSMQ OSMQ_HCI_Command_To_LL;
    #endif

    //- Input Queue of LC
    OSMQ OSMQ_HCI_ACL_Data_From_Host[NUM_OF_SUPPORTED_ACL_LINK];    /* PDU from Host */
    OSMQ OSMQ_LMP_Data_From_LM[NUM_OF_SUPPORTED_ACL_LINK];
    #if 0 // duplicated
    OSMQ OSMQ_LC_Cmd_from_LL;
    #endif

    OSMQ OSMQ_HCI_BC_Data_From_Host;

    //- BC
    OSMQ OSMQ_HC_BC_Data_From_Bus;

    OSMQ OSMQ_code;
    OSMQ OSMQ_I2C_SPI_IssueBuffer;
    //- HC
    OSMQ OSMQ_HC_Event_To_Bus;
    OSMQ OSMQ_HC_Data_To_UART;
    OSMQ OSMQ_HC_Command_From_Bus;
    OSMQ OSMQ_HC_Event_To_UART;
    //- NEW HC
    OSMQ OSMQ_NHC_Event_To_Bus;
    OSMQ OSMQ_NHC_Data_To_Bus;
    OSMQ OSMQ_NHC_Data_From_Bus[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_NHC_Command_From_Bus;
    OSMQ OSMQ_NHC_Command_From_UART;

    OSMQ OSMQ_UART_TX_Data;
    //- LC
    #ifdef INQUIRY_SUPPORT
    OSMQ OSMQ_LC_Inquiry_Command_From_LM;
    OSMQ OSMQ_LC_Inquiry_Event_To_LM;
    #endif
    OSMQ OSMQ_LC_Page_Command_From_LM;
    OSMQ OSMQ_LC_Page_Event_To_LM;
    OSMQ OSMQ_LC_PageScan_Event_To_LM;
    //- LM
    OSMQ OSMQ_HC_CmdRsp;
    OSMQ OSMQ_HC_Rsp_By_Link[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_HC_Cmd_By_Link[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LM_Event_To_HC;
    OSMQ OSMQ_LM_BCData_To_HC;
    OSMQ OSMQ_LM_Normal_Pdu[MAX_NUM_ACL_LINK];
    //- BC
    OSMQ OSMQ_LC_BCData_From_LM_HC;
    OSMQ OSMQ_LC_BCData_Completed_To_LM_HC;
    //- LC
    OSMQ OSMQ_LC_Data_From_LM[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Data_From_HC[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Command_From_LM[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Event_To_LM[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Data_To_LM[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Data_To_HC[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Completed_Data_To_HC[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Recycle_Data_To_HC[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Command_From_HC[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Event_To_HC[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Ack_To_LM[MAX_NUM_ACL_LINK];
    OSMQ OSMQ_LC_Data_From_HC_TEMP_NON_AUTO_FLUSH[MAX_NUM_ACL_LINK];
    // HCI
    OSMQ OSMQ_HCIDRV_Data_From_Bus[MAX_MULTI_POINT_NO];
    // GAP
    OSMQ OSMQ_GAP_Command_From_PROFILE;
    OSMQ OSMQ_GAP_Event_To_MMI;
    #ifdef LE_SUPPORTED_HOST
    OSMQ OSMQ_GAP_Event_To_MMILE;
    #endif

    OSMQ OSMQ_LC_Cmd_from_LL;
    OSMQ OSMQ_LC_Data_from_LL[MAX_NUM_LE_LINK];
    OSMQ OSMQ_LC_Ack_To_LL[MAX_NUM_LE_LINK];
    OSMQ OSMQ_LC_Event_To_LL;
    OSMQ OSMQ_LC_Data_To_LL[MAX_NUM_LE_LINK];

    OSMQ OSMQ_LC_LE_Event_To_HC;
    OSMQ OSMQ_LL_Event_To_HC;
    OSMQ OSMQ_HC_Command_To_LL;
    OSMQ OSMQ_HC_Cmd_By_Link_To_LL[MAX_NUM_LE_LINK];
    OSMQ OSMQ_LC_LE_Data_To_HC[MAX_NUM_LE_LINK];
    OSMQ OSMQ_HC_LE_Data_From_Bus[MAX_NUM_LE_LINK];
    OSMQ OSMQ_HC_FlashData_From_UART;
    OSMQ OSMQ_HC_FlashData_To_UART;
    OSMQ OSMQ_NHC_LE_Data_From_Bus[MAX_NUM_LE_LINK];
    #ifdef HID_SOC
    OSMQ OSMQ_MCU_DATA_IN;
    OSMQ OSMQ_MCU_DATA_OUT;
    #endif

    /* For FEA_NEW_CONNHDL */
    OSMQ OSMQ_LM_Free_Link;
    OSMQ OSMQ_LM_Used_Link;

} ALIGN_OSMQ_STRU;



////////////////////////////////////////////////////////////////////////////////
// External Variables //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN ALIGN_OSMQ_STRU		XDATA gAlign;
EXTERN OSMQ XDATA OSMQ_Garbage_Msg_From_Host;


#define OSMQ_code_ptr                           &gAlign.OSMQ_code
#define OSMQ_I2C_SPI_IssueBuffer_ptr            &gAlign.OSMQ_I2C_SPI_IssueBuffer
//- HC
#define OSMQ_HC_Event_To_Bus_ptr                &gAlign.OSMQ_HC_Event_To_Bus
#define OSMQ_HC_Data_To_UART_ptr                &gAlign.OSMQ_HC_Data_To_UART
#define OSMQ_HC_Command_From_Bus_ptr            &gAlign.OSMQ_HC_Command_From_Bus
#define OSMQ_HC_Event_To_UART_ptr               &gAlign.OSMQ_HC_Event_To_UART
//- NEW HC
#define OSMQ_NHC_Event_To_Bus_ptr               &gAlign.OSMQ_NHC_Event_To_Bus
#define OSMQ_NHC_Data_To_Bus_ptr                &gAlign.OSMQ_NHC_Data_To_Bus
#define OSMQ_NHC_Data_From_Bus_ptr              &gAlign.OSMQ_NHC_Data_From_Bus
#define OSMQ_NHC_Command_From_Bus_ptr           &gAlign.OSMQ_NHC_Command_From_Bus
#define OSMQ_NHC_Command_From_UART_ptr          &gAlign.OSMQ_NHC_Command_From_UART
#define OSMQ_UART_TX_Data_ptr                   &gAlign.OSMQ_UART_TX_Data
//- LC
#ifdef INQUIRY_SUPPORT
#define OSMQ_LC_Inquiry_Command_From_LM_ptr     &gAlign.OSMQ_LC_Inquiry_Command_From_LM
#define OSMQ_LC_Inquiry_Event_To_LM_ptr         &gAlign.OSMQ_LC_Inquiry_Event_To_LM
#endif
#define OSMQ_LC_Page_Command_From_LM_ptr        &gAlign.OSMQ_LC_Page_Command_From_LM
#define OSMQ_LC_Page_Event_To_LM_ptr            &gAlign.OSMQ_LC_Page_Event_To_LM
#define OSMQ_LC_PageScan_Event_To_LM_ptr        &gAlign.OSMQ_LC_PageScan_Event_To_LM
//- LM
#define OSMQ_HC_CmdRsp_ptr                      &gAlign.OSMQ_HC_CmdRsp
#define OSMQ_HC_Rsp_By_Link_ptr                 &gAlign.OSMQ_HC_Rsp_By_Link
#define OSMQ_HC_Cmd_By_Link_ptr                 &gAlign.OSMQ_HC_Cmd_By_Link
#define OSMQ_LM_Event_To_HC_ptr                 &gAlign.OSMQ_LM_Event_To_HC
#define OSMQ_LM_BCData_To_HC_ptr                &gAlign.OSMQ_LM_BCData_To_HC
#define OSMQ_LM_Normal_Pdu_ptr                  &gAlign.OSMQ_LM_Normal_Pdu
//- BC
#define OSMQ_HC_BC_Data_From_Bus_ptr            &gAlign.OSMQ_HC_BC_Data_From_Bus
#define OSMQ_LC_BCData_From_LM_HC_ptr           &gAlign.OSMQ_LC_BCData_From_LM_HC
#define OSMQ_LC_BCData_Completed_To_LM_HC_ptr   &gAlign.OSMQ_LC_BCData_Completed_To_LM_HC
//- LC
#define OSMQ_LC_Data_From_LM_ptr                &gAlign.OSMQ_LC_Data_From_LM
#define OSMQ_LC_Data_From_HC_ptr                &gAlign.OSMQ_LC_Data_From_HC
#define OSMQ_LC_Command_From_LM_ptr             &gAlign.OSMQ_LC_Command_From_LM
#define OSMQ_LC_Event_To_LM_ptr                 &gAlign.OSMQ_LC_Event_To_LM
#define OSMQ_LC_Data_To_LM_ptr                  &gAlign.OSMQ_LC_Data_To_LM
#define OSMQ_LC_Data_To_HC_ptr                  &gAlign.OSMQ_LC_Data_To_HC
#define OSMQ_LC_Completed_Data_To_HC_ptr        &gAlign.OSMQ_LC_Completed_Data_To_HC
#define OSMQ_LC_Recycle_Data_To_HC_ptr          &gAlign.OSMQ_LC_Recycle_Data_To_HC
#define OSMQ_LC_Command_From_HC_ptr             &gAlign.OSMQ_LC_Command_From_HC
#define OSMQ_LC_Event_To_HC_ptr                 &gAlign.OSMQ_LC_Event_To_HC
#define OSMQ_LC_Ack_To_LM_ptr                   &gAlign.OSMQ_LC_Ack_To_LM
#define OSMQ_LC_Data_From_HC_TEMP_NON_AUTO_FLUSH_ptr &gAlign.OSMQ_LC_Data_From_HC_TEMP_NON_AUTO_FLUSH
// HCI
#define OSMQ_HCIDRV_Data_From_Bus_ptr           &gAlign.OSMQ_HCIDRV_Data_From_Bus
// GAP
#define OSMQ_GAP_Command_From_PROFILE_ptr       &gAlign.OSMQ_GAP_Command_From_PROFILE
#define OSMQ_GAP_Event_To_MMI_ptr               &gAlign.OSMQ_GAP_Event_To_MMI
#ifdef LE_SUPPORTED_HOST
#define OSMQ_GAP_Event_To_MMILE_ptr             &gAlign.OSMQ_GAP_Event_To_MMILE
#endif

#define OSMQ_LC_Command_From_LL_ptr             &gAlign.OSMQ_LC_Cmd_from_LL
#define OSMQ_LC_Data_From_LL_ptr                &gAlign.OSMQ_LC_Data_from_LL
#define OSMQ_LC_Ack_To_LL_ptr                   &gAlign.OSMQ_LC_Ack_To_LL
#define OSMQ_LC_Event_To_LL_ptr                 &gAlign.OSMQ_LC_Event_To_LL
#define OSMQ_LC_Data_To_LL_ptr                  &gAlign.OSMQ_LC_Data_To_LL


#define OSMQ_LC_LE_Event_To_HC_ptr              &gAlign.OSMQ_LC_LE_Event_To_HC
#define OSMQ_LL_Event_To_HC_ptr                 &gAlign.OSMQ_LL_Event_To_HC
#define OSMQ_HC_Command_To_LL_ptr               &gAlign.OSMQ_HC_Command_To_LL
#define OSMQ_HC_Cmd_By_Link_To_LL_ptr           &gAlign.OSMQ_HC_Cmd_By_Link_To_LL
#define OSMQ_LC_LE_Data_To_HC_ptr               &gAlign.OSMQ_LC_LE_Data_To_HC
#define OSMQ_HC_LE_Data_From_Bus_ptr            &gAlign.OSMQ_HC_LE_Data_From_Bus
#define OSMQ_HC_FlashData_From_UART_ptr         &gAlign.OSMQ_HC_FlashData_From_UART
#define OSMQ_HC_FlashData_To_UART_ptr           &gAlign.OSMQ_HC_FlashData_To_UART
#define OSMQ_NHC_LE_Data_From_Bus_ptr           &gAlign.OSMQ_NHC_LE_Data_From_Bus
#ifdef HID_SOC
#define OSMQ_MCU_DATA_IN_ptr                    &gAlign.OSMQ_MCU_DATA_IN
#define OSMQ_MCU_DATA_OUT_ptr                   &gAlign.OSMQ_MCU_DATA_OUT
#endif

#define OSMQ_LM_Free_Link_ptr                   &gAlign.OSMQ_LM_Free_Link
#define OSMQ_LM_Used_Link_ptr                   &gAlign.OSMQ_LM_Used_Link

#endif /* _ALIGN_OSMQ_H_ */

