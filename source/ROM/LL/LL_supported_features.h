#ifndef _LL_SUPPORTED_FEATURES_H_
#define _LL_SUPPORTED_FEATURES_H_

#include "LL_interface.h"

#define LE_ENCRYPTION_SUPPORTED(pLinkCtl)               (((LL_LINK_CONTROL_STRU_PTR)pLinkCtl)->CFeatures[0].field.Bit0)
#define CONN_PARA_REQ_PROC_SUPPORTED(pLinkCtl)          (((LL_LINK_CONTROL_STRU_PTR)pLinkCtl)->CFeatures[0].field.Bit1)
#define EXT_REJECT_IND_SUPPORTED(pLinkCtl)              (((LL_LINK_CONTROL_STRU_PTR)pLinkCtl)->CFeatures[0].field.Bit2)
#define SLAVE_INIT_FEA_EXCHG_SUPPORTED(pLinkCtl)        (((LL_LINK_CONTROL_STRU_PTR)pLinkCtl)->CFeatures[0].field.Bit3)
#define LE_PING_SUPPORTED(pLinkCtl)                     (((LL_LINK_CONTROL_STRU_PTR)pLinkCtl)->CFeatures[0].field.Bit4)
#define LE_DATA_PKT_LEN_EXT_SUPPORTED(pLinkCtl)         (((LL_LINK_CONTROL_STRU_PTR)pLinkCtl)->CFeatures[0].field.Bit5)
#define LL_PRIVACY_SUPPORTED(pLinkCtl)                  (((LL_LINK_CONTROL_STRU_PTR)pLinkCtl)->CFeatures[0].field.Bit6)    
#define EXT_SCAN_FILTER_POLICIES_SUPPORTED(pLinkCtl)    (((LL_LINK_CONTROL_STRU_PTR)pLinkCtl)->CFeatures[0].field.Bit7)    


#endif