#ifdef LE_SUPPORTED_HOST_FLASH

#ifndef _LINK_INDEX_CTL_H_
#define _LINK_INDEX_CTL_H_

#include "cpu.h"
#include "..\COMMON\le_common_hal.h"
#include "lic_hal.h"


#define LIC_INVALID_LINK_INDEX          0xFF
#define LIC_MAX_CONNECTION_HANDLE       0xEFF
#define LIC_INVALID_CONNECTION_HANDLE   0xFFFF

#define LIC_HC_CC_CONN_HDL(ptr)         (*((U8 XDATA_PTR)(ptr +(*ptr)+4)) + ((U16)(*((U8 XDATA_PTR)(ptr +(*ptr)+5))) << 8))
#define LIC_HC_DCC_CONN_HDL(ptr)        (*((U8 XDATA_PTR)(ptr +(*ptr)+3)) + ((U16)(*((U8 XDATA_PTR)(ptr +(*ptr)+4))) << 8))
#define LIC_L2C_DATA_CONN_HDL(ptr)      (((LIC_L2CAP_DATA_PKT_TYPE XDATA_PTR)ptr)->hciHdr.field.Handle)


PUBLIC void LIC_LE_AddLinkInfo(HCI_LE_EVT_STRU XDATA_PTR ptr);
PUBLIC U8 LIC_LE_DelLinkInfo(U16 ConnHdl);

PUBLIC void LIC_LE_Init(void);
PUBLIC U16 LIC_LE_GetConnHdl(U8 idx);
PUBLIC U8 LIC_LE_GetIdxByConnHdl(U16 connHdl);
PUBLIC U8 LIC_LE_GetNumOfActiveLink(void);
PUBLIC U8 LIC_LE_GetActiveLinkIdx(U8 i);
PUBLIC U8 XDATA_PTR LIC_LE_GetPeerAddr(U8 idx);
PUBLIC U8 LIC_LE_GetPeerAddrType(U8 idx);

#endif
#endif

