#ifndef _LE_XPORT_INTERFACE_H_ 
#define _LE_XPORT_INTERFACE_H_

#ifdef _LE_XPORT_C_
#define LE_XPORT_EXTERN
#else
#define LE_XPORT_EXTERN extern
#endif

#ifdef SUPPORT_LE_XPORT

LE_XPORT_EXTERN OSMQ XDATA OSMQ_SPP_CMD_To_LE_XPORT;
#define OSMQ_SPP_CMD_To_LE_XPORT_ptr &OSMQ_SPP_CMD_To_LE_XPORT

PUBLIC void LE_XPORT(void);
PUBLIC void LE_XPORT_init(void);

#endif
#endif
