#include "os.h"
#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef BLE_ANCS_C

#include "..\GATT\GATT_interface.h"
#include "BLE_ANCS_C_interface.h"

#define BLE_ANCS_C_CCCD_NOTI_MASK    (0x0001)
#define BLE_ANCS_C_CCCD_IND_MASK     (0x0002)

#define BLE_ANCS_C_SET_NOTI_SRC_CCCD_MASK    (0x01)
#define BLE_ANCS_C_SET_DATA_SRC_CCCD_MASK    (0x02)

typedef enum
{
    ANCS_STATE_NONE,
    ANCS_STATE_WAIT_FOR_AUTHENTICATION,
    ANCS_STATE_WAIT_FOR_ENCRYPT,
    ANCS_STATE_WAIT_FOR_FIND_INFO_NOTIF_SOURCE,
    ANCS_STATE_FIND_INFO_DATA_SOURCE,
    ANCS_STATE_WAIT_FOR_FIND_INFO_DATA_SOURCE,
    ANCS_STATE_FIND_INFO_COMPLETE,
    ANCS_STATE_WAIT_FOR_WRITE_NOTI_SRC_CCCD_RSP,
    ANCS_STATE_WAIT_FOR_WRITE_DATA_SRC_CCCD_RSP,    
} ANCS_STATE;

typedef struct
{
    U16 startHandle;
    U16 endHandle;
} ANCS_FIND_INFO_STRU;

typedef struct
{
    U8 linkIdx;
    U8 writeOp;
    U8 flag;
    U16 handle;
    U16 offset;
    U16 len;
    U16 CCCD;
} ANCS_WRITE_CCCD_STRU;

typedef struct
{
    U8 cmdId;
    U8 notiUID[4];
    U8 attrList[1];
} ANCS_GET_NOTI_ATTR_CMD_STRU;

typedef struct
{
    U8 cmdId;
    U8 appId[1];   // unknow length  
} ANCS_GET_APP_ATTR_CMD_STRU;

typedef struct
{
    U8 cmdId;
    U8 notiUID[4];
    U8 actionId;
} ANCS_PERFORM_NOTIFICATION_ACTION_STRU;

typedef union
{
    ANCS_GET_NOTI_ATTR_CMD_STRU getNotiAttrCmd;
    ANCS_GET_APP_ATTR_CMD_STRU getAppAttrCmd;
    ANCS_PERFORM_NOTIFICATION_ACTION_STRU performNotiAction;    
} ANCS_CONTROL_POINT_STRU;

typedef struct
{
    U8 state;
    U8 action;
    GATT_CHAR_DECL_STRU notiSourceChar;
    GATT_CHAR_DECL_STRU dataSourceChar;
    GATT_CHAR_DECL_STRU controlPointChar;
    U16 notifSourceCCCDHdl;
    U16 dataSourceCCCDHdl;
    ATT_HANDLE_RANGE_STRU serviceRange;
    ATT_HANDLE_RANGE_STRU findNotiSrcDesc;  // big endian
    ATT_HANDLE_RANGE_STRU findDataSrcDesc;  // big endian
    U16 notiSrcCCCD;
    U16 dataSrcCCCD;
    BLE_ANCS_C_EVT_CB cb;
} BLE_ANCS_C_CTL;

BLE_ANCS_C_CTL XDATA gBLE_ANCS_C_ctl[MAX_NUM_OF_LINK];

#endif
#endif

