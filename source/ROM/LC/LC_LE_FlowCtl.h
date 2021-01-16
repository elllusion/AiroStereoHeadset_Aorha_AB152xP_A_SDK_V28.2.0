#ifndef _LC_LE_FLOWCTL_H_
#define _LC_LE_FLOWCTL_H_
#include "config.h"
#include "LC_BLEDeviceCtl.h"




////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8 num_conn_handle;
    U8 conn_handle[2];
    U16 num_compl_pkt;

} NUM_COMPL_PKT_EVENT_PARAM_STRU, OS_PTR NUM_COMPL_PKT_EVENT_PARAM_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN VOID LC_LE_FlowCtl_Init(U8 link_idx);
EXTERN LC_TX_DATA_PKT_STRU_PTR LC_LE_GetL2CAPDataPdu(U8 linkIdx);
EXTERN VOID LC_LE_ChkCompletePkt(LC_TX_DATA_PKT_STRU_PTR ptr1, U8 linkIdx);
EXTERN VOID LC_LE_ChangeDataLenReq(
    IN      U8 linkIdx,
    IN      U8 max_tx_octets);
EXTERN VOID LC_LE_L2cap_Pause(
    IN      U8  link_idx);
EXTERN VOID LC_LE_L2cap_Resume(
    IN      U8  link_idx);

#endif /* _LC_LE_FLOWCTL_H_ */

