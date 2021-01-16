#ifndef _LL_INFO_EXCHANGE_
#define _LL_INFO_EXCHANGE_
#include "config.h"
#include "ctlr_config.h"
#if (LE_SUPPORTED_CONTROLLER)
#include "LL_interface.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8 subevent_code;
    U8 status;
    U8 conn_hdl[2];
    U8 Le_features[8];

} LL_READ_REMOTE_USED_FEA_META_EVENT_PARAM_STRU, OS_PTR LL_READ_REMOTE_USED_FEA_META_EVENT_PARAM_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN HC_EVT_MSG_STRU_PTR LL_RdRemoteVerInfoCplEvt(LL_LINK_CONTROL_STRU_PTR pLinkCtl);
EXTERN HC_EVT_MSG_STRU_PTR LL_RdRemoteVerInfoTimeOutEvt(LL_LINK_CONTROL_STRU_PTR pLinkCtl);
EXTERN SYS_ERRCODE LL_RdRemoteVerInfo(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_VerExchg_rd_remote_ver_cmd_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_VerExchg_ver_ind_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);

EXTERN SYS_ERRCODE LL_RdRemoteUsedFea(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_FeaExchg_rd_remote_used_fea_cmd_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_FeaExchg_fea_rsp_hdlr(LINK_TRANS_CTL_STRU_PTR pLinkTransCtl);
EXTERN VOID LL_UnKnownSlaveFeaReqHdlr(LL_LINK_CONTROL_STRU_PTR pLinkCtl,LC_LMP_STRU_PTR pLmpMsg);


#endif
#endif /* _LL_INFO_EXCHANGE_ */

