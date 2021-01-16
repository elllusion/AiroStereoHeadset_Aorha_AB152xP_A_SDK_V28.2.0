#include "os.h"
#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef APP_ANCS_C

#ifdef _APP_ANCS_C_
	#define APP_ANCS_EXTERN
#else
	#define APP_ANCS_EXTERN extern
#endif

#ifdef ANCS_CELL_PHONE_APP
    
#define ANCS_MAX_TX_DATA_LEN         270
#define ANCS_MAX_RX_DATA_LEN         270
#define ANCS_MAX_SEND_TX_DATA_LEN    (gATT_ctl.attMtu - ATT_REPORT_HEADER_LEN)
    
#define ANCS_APP_CMD_SET_VIBRATION_PARA_LEN 6


typedef struct
{
    U8 hciXportId;
    U8 hciOcf;
	U8 hciOgf;
    U8 hciLen;
    U8 vocf;
    U8 vogf;
    U8 paraPtr[1];
}APP_CMD_STRU;

typedef struct
{
    U8 srcNotifTxIdx;
    U8 srcNotifTxCCCD[2];
    U8 controlPointRx[ANCS_MAX_RX_DATA_LEN];
    U8 dataNotifTxIdx;
    U8 dataNotifTxCCCD[2]; 
}APP_ANCS_STRU;

APP_ANCS_EXTERN APP_ANCS_STRU XDATA gApp_ANCS_ctl;

PUBLIC U8 APP_ANCS_RxCB(U8 CBtype);
PUBLIC void APP_ANCS_CmdRx(APP_CMD_STRU XDATA_PTR cmd, U16 len);
#endif  //ANCS_CELL_PHONE_APP

PUBLIC void APP_ANCS_Init(void);
#endif  //APP_ANCS_C
#endif  //LE_SUPPORTED_HOST_FLASH