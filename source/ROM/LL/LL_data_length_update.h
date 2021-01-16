#ifndef _LL_DATA_LENGTH_UPDATE_H_
#define _LL_DATA_LENGTH_UPDATE_H_
#include "ctlr_config.h"
#if (LE_SUPPORTED_CONTROLLER)
#include "LL_interface.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8 subevent_code;
    U8 conn_hdl[2];
    U8 MaxTxOctetsB0;
    U8 MaxTxOctetsB1;
    U8 MaxTxTimeB0;
    U8 MaxTxTimeB1;
    U8 MaxRxOctetsB0; 
    U8 MaxRxOctetsB1;
    U8 MaxRxTimeB0;
    U8 MaxRxTimeB1;
} LL_DATA_LENGTH_CHANGE_EVENT_PARAM_STRU, OS_PTR LL_DATA_LENGTH_CHANGE_EVENT_PARAM_STRU_PTR;

EXTERN VOID LL_ActDataLenUpdate_send_len_req_hdlr(
    IN      LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

EXTERN VOID LL_ActDataLenUpdate_len_rsp_hdlr(
    IN      LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

EXTERN VOID LL_PasDataLenUpdate_len_req_hdlr(
    IN      LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

EXTERN VOID LL_ActDataLenUpdate_exc_hdlr(
    IN      LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

EXTERN SYS_ERRCODE LL_ActDataLenUpdate(
    IN      LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

EXTERN VOID LL_PasDataLenUpdate_len_req_hdlr(
    IN      LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_PasDataLenUpdate_len_rsp_acked_hdlr(
    IN      LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

#endif
#endif