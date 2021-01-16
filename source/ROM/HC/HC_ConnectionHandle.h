#ifndef _HC_CONNECTIONHANDLE_H_
#define _HC_CONNECTIONHANDLE_H_

#include "os.h"
#include "hci_conn_hdl.h"

//#define CONNHDL_BASE	0x81

typedef struct stru_connhdl_ctl
{
	U16  ConnHdl;
	OSMQ OSMQ_L2CAP_Data_To_HC;
	OSMQ OSMQ_L2CAP_Data_From_HC;
	OSMQ OSMQ_LC_Data_To_HC;
	OSMQ OSMQ_LC_Data_From_HC;
	OSMQ OSMQ_LM_Cmd_From_HC;

} CONNHDL_CTL_STRU;



/* Fucntion Prototypes */
VOID HC_ConnHdl_Init(VOID);
CONNHDL_CTL_STRU XDATA_PTR HC_NewConnHdl(VOID) REENTRANT;
VOID HC_RlsConnHdl(U16 ConnHdl) REENTRANT;
CONNHDL_CTL_STRU XDATA_PTR HC_GetConnHdl(U16 ConnHdl) REENTRANT;



/* Extern Variables */
extern CONNHDL_CTL_STRU XDATA gConnHdl_ctl[];
extern U8 XDATA gConnHdl_UsedMask;


/* Macro */
#define HC_CONNHDL_MASK_ADD(A)  (gConnHdl_UsedMask |= U8_BIT_MASK(A))
#define HC_CONNHDL_MASK_DEL(A)  (gConnHdl_UsedMask &= (~U8_BIT_MASK(A)))
#define HC_CONNHDL_IS_USED(A)   (gConnHdl_UsedMask & U8_BIT_MASK(A))

#endif
