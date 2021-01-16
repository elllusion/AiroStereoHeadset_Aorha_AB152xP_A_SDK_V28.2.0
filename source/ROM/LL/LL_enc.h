#ifndef _LL_ENC_H_
#define _LL_ENC_H_
#include "config.h"
#include "ctlr_config.h"
#if (LE_SUPPORTED_CONTROLLER)

#include "LL_interface.h"



enum
{
    LINK_ENC_OFF =0,
    LINK_ENC_ON_E0_BREDR_AES_LE,
    LINK_ENC_ON_AES_BREDR
};



typedef struct
{
    U8 numhcipkt;
    U8 ocf;
    U8 ogf;
    U8 status;
    U8 enc_data[16];
}LL_ENC_DATA_COMPL_EVENT_STRU;

typedef struct
{
    U8 subevent_code;
    U8 conn_hdl[2];
    U8 Rand[8];
    U8 EDIV[2];

} LL_LTK_REQ_META_EVENT_PARAM_STRU, OS_PTR LL_LTK_REQ_META_EVENT_PARAM_STRU_PTR;

typedef struct
{
    U8 status;
    U8 conn_hdl[2];
    U8 enc_enabled;

} LL_ENC_KEY_CHANGE_EVENT_PARAM_STRU, OS_PTR LL_ENC_KEY_CHANGE_EVENT_PARAM_STRU_PTR;

typedef struct
{
    U8 status;
    U8 conn_hdl[2];

} LL_ENC_KEY_REFRESH_EVENT_PARAM_STRU, OS_PTR LL_ENC_KEY_REFRESH_EVENT_PARAM_STRU_PTR;

EXTERN VOID LL_EncConn_ChkAesReady_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

EXTERN VOID LL_PasEncConn_pause_enc_req_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_PasEncConn_pause_enc_rsp_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_PasEncConn_enc_req_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_PasEncConn_ltk_rply_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_PasEncConn_session_key_ready_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_PasEncConn_wait_Aes_sem_key_ready_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_PasEncConn_start_enc_rsp_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_PasEncConn_start_enc_rsp_acked_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);


EXTERN VOID LL_ActEncConn_start_enc_cmd_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_ActEncConn_pause_enc_rsp_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_ActEncConn_pause_enc_rsp_acked_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_ActEncConn_enc_rsp_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_ActEncConn_session_key_ready_hdlr( LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_ActEncConn_start_enc_req_hdlr( LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_ActEncConn_start_enc_rsp_hdlr( LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);



EXTERN SYS_ERRCODE LL_ENC_StartEncryptData(HC_CMD_MSG_STRU_PTR pCmdMsg);
EXTERN SYS_ERRCODE LL_PasStartEncryptConn(LL_LINK_CONTROL_STRU_PTR pLinkCtl, LC_LMP_STRU_PTR pLmpMsg);
EXTERN SYS_ERRCODE LL_PasReStartEncryptConn(LL_LINK_CONTROL_STRU_PTR pLinkCtl);
EXTERN SYS_ERRCODE LL_ActStartEncryptConn(LL_LINK_CONTROL_STRU_PTR pLinkCtl,HC_CMD_MSG_STRU_PTR pCmdMsg);


#endif
#endif
