#include "os.h"
#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef APP_ANCS_C

#ifdef ANCS_CELL_PHONE_APP

#define ANCS_APP_CMD_SET_VIBRATION  1
#define ANCS_APP_CMD_GET_VIBRATION  2

#define ANCS_APP_VOCF_VOGF_LEN  2
#define ANCS_APP_RSP_BASIC_LEN  3

typedef struct
{
    U8 hciXportId;
    U8 hciEvtCode;
	U8 hciLen;
	U8 vocf;
	U8 vogf;
    U8 result;
    U8 paraPtr[1];
} ANCS_APP_RSP_STRU;

#endif  //#ifdef ANCS_CELL_PHONE_APP

#endif  //#ifdef APP_ANCS_C
#endif  //#ifdef LE_SUPPORTED_HOST_FLASH

