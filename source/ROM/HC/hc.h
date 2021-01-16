#ifndef _HC_H_
#define _HC_H_
/*
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * hc.h is the file defines the function prototypes of HC.
 *
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"
#include "hci.h"
#include "hci_airoha.h"
#include "hci_conn_hdl.h"
#include "hc_types.h"
#include "hci_error.h"
#include "ctlr_config.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions - Message Formats //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
 * HC Layer Command Message Data Structure
 */
#if 0
typedef struct stru_hc_cmd_msg
{
    U8 Offset;
    U8 RSVD1;
    HCI_CMD_STRU HciCmd;

} HC_CMD_MSG_STRU, OS_PTR HC_CMD_MSG_STRU_PTR;
#else
typedef U8 XDATA_PTR HC_CMD_MSG_STRU_PTR;
#endif


#ifndef HC_CMD_MSG_OFFSET
#define HC_CMD_MSG_OFFSET  2
#else
#error "HC_CMD_MSG_OFFSET has been defined"
#endif

/*
 * HC Layer Event Message Data Structure
 */
typedef struct stru_hc_evt_msg
{
    U8 Offset;
    U8 RSVD1;
    U8 RSVD2;
    HCI_EVT_STRU HciEvt;

} HC_EVT_MSG_STRU, OS_PTR HC_EVT_MSG_STRU_PTR;

#ifndef HC_EVT_MSG_OFFSET
#define HC_EVT_MSG_OFFSET (OS_OFFSET_OF(HC_EVT_MSG_STRU,HciEvt))
#else
#error "HC_EVT_MSG_OFFSET has been defined"
#endif

/*
 * HC Layer Data Message Data Structure
 */
typedef struct stru_ctlr_data_ctl
{
    U8 B0;
    U8 B1;

} DATA_CTL_STRU;

typedef struct stru_hc_data_msg_hdr
{
    U8 Offset;
    BTCLK_UNION TimeStamp;
    DATA_CTL_STRU Ctl;
    U16 Len;

} HC_DATA_MSG_HDR_STRU;

typedef struct stru_hc_data_msg
{
    HC_DATA_MSG_HDR_STRU Hdr;
    U8 Payload[1];

} HC_DATA_MSG_STRU, OS_PTR HC_DATA_MSG_STRU_PTR;

#ifndef HC_DATA_MSG_OFFSET
#define HC_DATA_MSG_OFFSET (OS_OFFSET_OF(HC_DATA_MSG_STRU,Hdr.Ctl.B1)) /* TODO:New offset "Ctl" */
#else
#error "HC_DATA_MSG_OFFSET has been defined"
#endif

/*
 * HC Layer Data Tx Message Data Structure
 */
typedef struct stru_hc_data_tx_msg
{
    U8 Offset;
    U8 RSVD1;
    U8 RSVD2;
    HCI_DATA_STRU HciTxData;

} HC_DATA_TX_MSG_STRU, OS_PTR HC_DATA_TX_MSG_STRU_PTR;

#ifndef HC_DATA_TX_MSG_OFFSET
#define HC_DATA_TX_MSG_OFFSET (OS_OFFSET_OF(HC_DATA_TX_MSG_STRU,HciTxData))
#else
#error "HC_DATA_TX_MSG_OFFSET has been defined"
#endif



////////////////////////////////////////////////////////////////////////////////
// Type Definitions - Command Handler //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- HCI command handler
typedef SYS_ERRCODE (*HCI_CMD_HDLR)(HC_CMD_MSG_STRU_PTR pMsg);

//- Command table
#define NUM_OF_SUPPORTED_HCI_CMD_OGF 8

typedef struct stru_hci_cmdogf_table_info
{
    U8 NumOfSupportedCmd;
    HCI_CMD_HDLR CODE_PTR pOgfTable;

} HCI_CMDOGF_TABLE_INFO;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions - HC Controller ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_hc_ctl
{
    BD_ADDR_STRU            BdAddr;
    PARITY_ARR              Parity;

    HCI_EVT_MASK_ARR        HciEvtMask;

    #if (LE_SUPPORTED_CONTROLLER)
    HCI_EVT_MASK_ARR        HciLeEvtMask;
    #endif

    #if (LE_SUPPORTED_CONTROLLER)
    HC_LE_PARAM             hc_le_param;
    #endif

    #if (FEA_SUPP_LAB_TEST)
    HC_VCMD_LABTEST_CONTROL labtest_control;
    #endif

    #if (FEA_SUPP_FLASH_PROGRAMMER)
    HC_VCMD_FLASH_CONTROL   flash_control;
    #endif

    #if (FEA_SUPP_RF_BIST)
    HC_VCMD_RF_BIST_PARA    rf_bist_para;
    #endif

    BTCLK BdAddrGuardTime;
} HC_CTL_STRU, OS_PTR HC_CTL_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U8 (*fVenderCmdHdlr)(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN HC_CTL_STRU gHC_ctl;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN VOID HC_Init(VOID);
EXTERN VOID HC_TaskEntry(VOID);

EXTERN VOID HC_CmdHdlr(VOID);
EXTERN VOID HC_GarbageMsgHdlr(VOID);
EXTERN BOOL HC_IsMaskedEvent(HC_EVT_MSG_STRU_PTR pEvtMsg);
EXTERN VOID HC_SendEventToHost(HC_EVT_MSG_STRU_PTR pEvtMsg);
EXTERN VOID HC_SendHciRstCmdCplEvtToHost(VOID);
#if LE_SUPPORTED_CONTROLLER
EXTERN SYS_ERRCODE HC_HciCmdRelayToLL_ByCH(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciCmdRelayToLL(HC_CMD_MSG_STRU_PTR pCmdMsg);
#endif
EXTERN SYS_ERRCODE HC_HciRejectCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciUnknownCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciDisconnCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciRdRemoteVerInfoCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciSetEvtMaskCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciRstCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciRdLeHostSuppCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciWrLeHostSuppCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciRdLocalVerInfoCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciRdLocalSuppCmdsCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciRdLocalSuppFeaCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciRdBufSizeCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciRdBdAddrCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeSetEvtMaskCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeRdBufSizeCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeRdLocalSuppFeaCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeSetRandAddrCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeRdAdvChTxPwrCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeRdWhiteListSizeCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeClrWhiteListCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeAddDeviceToWhiteListCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeRmDeviceFromWhiteListCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeRdChMapCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeRandCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeLtkReqReplyCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE HC_HciLeRdSuppStateCmdHdlr(HC_CMD_MSG_STRU_PTR pCmdMsg);

EXTERN HC_DATA_TX_MSG_STRU XDATA_PTR HC_NewHciDataTxMsg(U16 TxPldLen);
EXTERN HC_CMD_MSG_STRU_PTR HC_NewHcCmdMsg(U16 HciCmdSize);
EXTERN HC_DATA_MSG_STRU_PTR HC_NewHcDataMsg(U16 HciDataSize);
EXTERN HC_EVT_MSG_STRU_PTR HC_NewHciCmdStatusEvt(HCI_OPCODE OpCode);
EXTERN HC_EVT_MSG_STRU_PTR HC_NewHciCmdCplEvt(HCI_OPCODE OpCode,U8 RtnParaSize);
EXTERN HC_EVT_MSG_STRU_PTR HC_NewHciCmdCplEvtWithUnknownConnHdl(HCI_OPCODE OpCode,CONN_HDL ConnHdl);




////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * HC_NewHcCmdMsg
 *
 * Allocate a memory space to place HCI Command packet.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @HciCmdSize : Size of HCI Command packet.
 * @Return : pointer of HC_CMD_MSG_STRU.
 */
EXTERN HC_CMD_MSG_STRU_PTR HC_NewHcCmdMsg(
    IN      U16 HciCmdSize);

/**
 * HC_NewHcDataMsg
 *
 * Allocate a memory space to place HCI Data packet.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @HciCmdSize : Size of HCI Data packet.
 * @Return : pointer of HC_DATA_MSG_STRU.
 */
EXTERN HC_DATA_MSG_STRU_PTR HC_NewHcDataMsg(
    IN      U16 HciDataSize);

/**
 * HC_NewHciNumOfCplPktEvt
 *
 * Allocate a memory space to cook a HCI Number Of Complete Packets EVENT with
 * the STATUS is assumed SUCCESS.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @ConnHdl : Connection Handle
 * @Nocp : Number of completed packets
 * @Return : pointer of HC_EVT_MSG_STRU
 */
EXTERN HC_EVT_MSG_STRU_PTR HC_NewHciNumOfCplPktEvt(
    IN      CONN_HDL ConnHdl,
    IN      U16 Nocp);



/**
 * HC_NewHciFlushOccurred
 *
 * Allocate a memory space to cook a HCI Flush Occurred EVENT
 *
 * @Author : Huichuan <HuichuanLin@airoha.com.tw>
 *
 * @ConnHdl : Connection Handle
 * @Return : pointer of HC_EVT_MSG_STRU
 */
EXTERN HC_EVT_MSG_STRU_PTR HC_NewHciFlushOccurredEvt(
    IN      CONN_HDL ConnHdl);


/**
 * HC_NewHciEnhancedFlushCpl
 *
 * Allocate a memory space to cook a HCI Enhance Flush Complete EVENT
 *
 * @Author : Huichuan <HuichuanLin@airoha.com.tw>
 *
 * @ConnHdl : Connection Handle
 * @Return : pointer of HC_EVT_MSG_STRU
 */
EXTERN HC_EVT_MSG_STRU_PTR HC_NewHciEnhanceFlushCplEvt(
    IN      CONN_HDL ConnHdl);

#endif /* _HC_H_ */

