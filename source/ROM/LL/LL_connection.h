#ifndef _LL_CONNECTION_H_
#define _LL_CONNECTION_H_
#include "config.h"
#include "ctlr_config.h"
#if (LE_SUPPORTED_CONTROLLER)
#include "LL_interface.h"
#include "bt_types.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8 subevent_code;
    U8 status;
    U8 conn_hdl[2];
    U8 role;
    U8 peer_addr_type;
    BD_ADDR_STRU peer_addr;
    U8 conn_interval[2];
    U8 conn_latency[2];
    U8 SuperviTO[2];
    U8 mca;

} LL_CONN_COMPLETE_META_EVENT_PARAM_STRU, OS_PTR LL_CONN_COMPLETE_META_EVENT_PARAM_STRU_PTR;

typedef struct
{
    U8 subevent_code;
    U8 status;
    U8 conn_hdl[2];
    U8 conn_interval[2];
    U8 conn_latency[2];
    U8 SuperviTo[2];

} LL_CONN_UPDATE_META_EVENT_PARAM_STRU, OS_PTR LL_CONN_UPDATE_META_EVENT_PARAM_STRU_PTR;

typedef struct
{
    U8 status;
    U8 conn_hdl[2];
    U8 err_code;

} LL_DISC_COMPLETE_EVENT_PARAM_STRU, OS_PTR LL_DISC_COMPLETE_EVENT_PARAM_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U32 LL_AccessAddrCal( VOID);
EXTERN U8 LL_SCA_Mapping(U16 sca_in_ppm);
EXTERN U16 LL_ConnIntervalCal(U16 IntervalMax, U16 IntervalMin);
EXTERN VOID LL_SetConnReqPdu( LL_CONN_REQ_PDU_STRU_PTR pConnReqPdu,HCI_CMD_PARA_LE_CREATE_CONN_STRU_PTR pCreateConnPara);

EXTERN HC_EVT_MSG_STRU_PTR LL_ConnComplMetaEvt(LL_LINK_CONTROL_STRU_PTR pLinkCtl);
EXTERN HC_EVT_MSG_STRU_PTR LL_ConnUpdateMetaEvt(LL_LINK_CONTROL_STRU_PTR pLinkCtl);
EXTERN HC_EVT_MSG_STRU_PTR LL_DiscComplEvt(U8 ErrCode);
EXTERN SYS_ERRCODE LL_Detach(LL_LINK_CONTROL_STRU_PTR pLinkCtl,U8 reason);


#endif

#endif /* _LL_CONNECTION_H_ */

