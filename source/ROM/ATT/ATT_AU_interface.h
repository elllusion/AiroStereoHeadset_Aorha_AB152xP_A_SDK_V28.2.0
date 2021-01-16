#ifdef AIR_UPDATE_ENABLE

#ifndef _ATT_INTERFACE_H_
#define _ATT_INTERFACE_H_

#include "os.h"

#ifdef _ATT_C_
#define ATT_EXTERN
#else
#define ATT_EXTERN extern
#endif


PUBLIC void ATT_AU_Init ( void );
PUBLIC void ATT_AU_ResetState(void);
PUBLIC void ATT_AU ( void );
PUBLIC void ATT_AU_ChannelHandler ( void );

PUBLIC BOOL ATT_AU_HandleValueReportMux(U8 Opcode, U16 server_index);

PUBLIC ATTRSP_PKT_TYPE XDATA_PTR ATT_AU_AllocDataPdu( U8 opCode );
PUBLIC U8 ATT_AU_ReserveRspPduMem(U8 XDATA_PTR ptr1);
PUBLIC void ATT_AU_SendPduToL2CAP( U8 XDATA_PTR pdu );
PUBLIC U16 ATT_AU_GetU16 (U8 XDATA_PTR ptr1);
PUBLIC void ATT_AU_SetU16(U8 XDATA_PTR ptr1, U16 value);
PUBLIC void ATT_AU_SetValue(U8 XDATA_PTR ptr1, U8 XDATA_PTR ptr2, U8 len);
PUBLIC BOOL ATT_AU_IsSameValue(U8 XDATA_PTR ptr1, U8 XDATA_PTR ptr2, U16 len);
PUBLIC BOOL ATT_AU_IsBTBaseUUID(U8 XDATA_PTR uuidPtr);

#endif
#endif
