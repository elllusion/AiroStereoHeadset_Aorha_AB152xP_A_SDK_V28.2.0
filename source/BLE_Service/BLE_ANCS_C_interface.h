#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef BLE_ANCS_C
#include "..\GATT\GATT_interface.h"

#define ANCS_NOTIF_UID_SIZE         (0x04)

#define ANCS_EVENT_FLAG_MASK_SILENT             (0x01)
#define ANCS_EVENT_FLAG_MASK_IMPORTANT          (0x02)
#define ANCS_EVENT_FLAG_MASK_PRE_EXISTING       (0x04)
#define ANCS_EVENT_FLAG_MASK_POSITIVE_ACTION    (0x08)
#define ANCS_EVENT_FLAG_MASK_NEGATIVE_ACTION    (0x10)

typedef enum
{
    BLE_ANCS_C_STATUS_SUCCESS,
    BLE_ANCS_C_STATUS_EXIST_CB = GATT_STATUS_NUM,
    BLE_ANCS_C_STATUS_NO_RESOURCE,
    BLE_ANCS_C_STATUS_HANDLE_NOT_EXIST,
    BLE_ANCS_C_STATUS_NO_NOTI_SRC_CCCD_HANDLE,
    BLE_ANCS_C_STATUS_WRITE_CCCD_FAILED,
    BLE_ANCS_C_STATUS_BUSY,
    BLE_ANCS_C_STATUS_NOT_SERVICE,
    
} BLE_ANCS_C_STATUS;

typedef enum
{
    ANCS_C_EVT_DISCOVER_COMPLETE,
    ANCS_C_EVT_DISCOVER_FAILED,
    ANCS_C_EVT_SOURCE_NOTIFICATION,
    ANCS_C_EVT_DATA_NOTIFICATION    
} ANCS_C_EVT_TYPE;

typedef enum
{
    CATEGORY_ID_OTHER,
    CATEGORY_ID_INCOMING_CALL,
    CATEGORY_ID_MISSED_CALL,
    CATEGORY_ID_VOICE_EMALL,
    CATEGORY_ID_SOCIAL,
    CATEGORY_ID_SCHEDULE,
    CATEGORY_ID_EMAIL,
    CATEGORY_ID_NEWS,
    CATEGORY_ID_HEALTH_AND_FITNESS,
    CATEGORY_ID_BUSINESS_AND_FINANCE,
    CATEGORY_ID_LOCATION,
    CATEGORY_ID_ENTERMAINMENT,
} ANCS_CATEGORY_ID;

typedef enum
{
    EVENT_ID_NOTIFICATION_ADDED,
    EVENT_ID_NOTIFICATION_MODIFICATION,
    EVENT_ID_NOTIFICATION_REMOVED,
} ANCS_EVENT_ID;

typedef enum
{
    CMD_ID_GET_NOTIFICATION_ATTRIBUTES,
    CMD_ID_GET_APP_ATTRIBUTES,
    CMD_ID_PERFORM_NOTIFICATION_ACTION,
} ANCS_CMD_ID;

typedef enum
{
    NOTI_ATTR_ID_APP_ID,
    NOTI_ATTR_ID_TITILE,     //needs to be followed by a 2-bytes max length parameter
    NOTI_ATTR_ID_SUBTITLE,   //needs to be followed by a 2-bytes max length parameter
    NOTI_ATTR_ID_MESSAGE,    //needs to be followed by a 2-bytes max length parameter
    NOTI_ATTR_ID_MESSAGE_SIZE,
    NOTI_ATTR_ID_DATE,
    NOTI_ATTR_ID_POSITIVE_ACTION_LABEL,
    NOTI_ATTR_ID_NEGATIVE_ACTION_LABEL,
} ANCS_NOTIFICATION_ATTRIBUTE_ID;

typedef enum
{
    ACTION_ID_POSITIVE,
    ACTION_ID_NEGATIVE,
} ANCS_ACTION_ID;

typedef enum
{
    APP_ATTR_ID_DISPLAY_NAME,
} ANCS_APP_ATTRIBUTE_ID;

typedef struct
{
    U8 evtId;
    U8 evtFlag;
    U8 ctgyId;
    U8 ctgyCount;
    U8 notiUID[4];    
} ANCS_NOTI_SRC_STRU;

typedef struct
{
    U8 cmdId;
    U8 notiUID[4];
    U8 attrId;
    U16 attrLen;
    U8 attrValue[1];
} ANCS_DATA_SRC_STRU;

typedef union
{
    U8 errCode;
    ANCS_NOTI_SRC_STRU notiSrc;
    ANCS_DATA_SRC_STRU dataSrc;
} BLE_ANCS_C_RSP_UNION;

typedef struct
{
    U8 linkIdx;
    U8 evtId;
    U16 len;
    BLE_ANCS_C_RSP_UNION rsp;
} BLE_ANCS_C_EVT_STRU;

typedef struct
{
    U8 attrId;
    U16 len;
} BLE_ANCS_C_ATTR_STRU;

typedef struct
{
    U8 attrId;
    U16 len;
    U8 dataPtr[1];
} BLE_ANCS_C_ATTR_LIST_STRU;

typedef struct
{
    U8 notiUID[4];
    U8 numAttr;
    BLE_ANCS_C_ATTR_STRU attrList[1];
} BLE_ANCS_C_GET_NOTI_ATTR_STRU;

typedef struct
{
    //U8 notiUID[4];
    U8 numAttr;
    U16 appIdLen;
    U8 appId[1];        
} BLE_ANCS_C_GET_APP_ATTR_STRU;

typedef struct
{
    U8 notiUID[4];
    U8 actionId;
} BLE_ANCS_C_PERFORM_NOTI_ACTION_STRU;

typedef void (*BLE_ANCS_C_EVT_CB)(BLE_ANCS_C_EVT_STRU XDATA_PTR evtPtr);

PUBLIC U8 BLE_ANCS_C_Init(U8 linkIdx, BLE_ANCS_C_EVT_CB cb);
PUBLIC U8 BLE_ANCS_C_EnableNotifNotificationSource(U8 linkIdx);
PUBLIC U8 BLE_ANCS_C_DisableNotifNotificationSource(U8 linkIdx);
PUBLIC U8 BLE_ANCS_C_EnableNotifDataSource(U8 linkIdx);
PUBLIC U8 BLE_ANCS_C_DisableNotifDataSource(U8 linkIdx);
PUBLIC U8 BLE_ANCS_C_GetNotifAttributes(U8 linkIdx, BLE_ANCS_C_GET_NOTI_ATTR_STRU XDATA_PTR paraPtr);
PUBLIC U8 BLE_ANCS_C_GetAppAttributes(U8 linkIdx, BLE_ANCS_C_GET_APP_ATTR_STRU XDATA_PTR paraPtr);
PUBLIC U8 BLE_ANCS_C_PerformNotiAction(U8 linkIdx, BLE_ANCS_C_PERFORM_NOTI_ACTION_STRU XDATA_PTR paraPtr);
PUBLIC U8 BLE_ANCS_C_WriteData(U8 linkIdx, U8 XDATA_PTR dataPtr, U16 len);
PUBLIC U8 BLE_ANCS_C_WriteVibrationSetting(U8 XDATA_PTR dataPtr);
PUBLIC U8 CODE_PTR BLE_ANCS_C_GetVibrationSetting(void);

#endif
#endif
