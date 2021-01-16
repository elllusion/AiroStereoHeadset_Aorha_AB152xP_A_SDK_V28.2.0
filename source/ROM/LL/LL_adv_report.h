#ifndef _LL_ADV_REPORT_H_
#define _LL_ADV_REPORT_H_
#include "ctlr_config.h"
#if (LE_SUPPORTED_CONTROLLER)
#include "LL_interface.h"
#include "Bt_types.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8 Subevent_code;
    U8 Num_reports;
    U8 Event_type;
    U8 Addr_type;
    BD_ADDR_STRU Addr;
    U8 Length;
    U8 Data[1];

} LL_ADV_REPORT_META_EVENT_PARAM_STRU, OS_PTR LL_ADV_REPORT_META_EVENT_PARAM_STRU_PTR;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN HC_EVT_MSG_STRU_PTR LL_AdvReportMetaEvt(LC_EVT_STRU_PTR pLcEvt);
EXTERN VOID LL_ScanFilterInit( VOID );
EXTERN U8 LL_ScanFilter(U8 FilterDuplicates, LL_ADV_PDU_STRU_PTR pAdvPdu);
EXTERN S8 LL_CalRssi(U8 agc_cs,U8 agc_rssi);


#endif
#endif /* _LL_ADV_REPORT_H_ */

