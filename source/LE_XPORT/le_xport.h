#ifndef _LE_XPORT_H_ 
#define _LE_XPORT_H_

#ifdef SUPPORT_LE_XPORT
#include "le_xport_interface.h"
#include "..\MMI\MMI_Base.h"

#define LE_XPORT_ATT_OP_NOTI            0x1B
#define LE_XPORT_HCI_TRSPX_HEADER_LEN   (OS_OFFSET_OF(HciUARTTrspXDataType, para))
#define LE_XPORT_DATA_OFFSET            (OS_OFFSET_OF(MMIMsgType, msgBodyPtr) + LE_XPORT_HCI_TRSPX_HEADER_LEN)

typedef struct 
{
    MMIMsgType XDATA_PTR rxPktPtr;
}LE_XPORT_CTL_TYPE;

LE_XPORT_EXTERN LE_XPORT_CTL_TYPE XDATA gleXport_ctl;

#endif
#endif

