#ifndef _GATT_H_ 
#define _GATT_H_

#ifdef LE_SUPPORTED_HOST_FLASH
#ifdef GATT_ENABLE
#include "..\COMMON\le_common_hal.h"
#include "GATT_interface.h"

#define GATT_HDL_INFO_DATA_SIZE         (4)
#define GATT_INFO_DATA_UUID_16_SIZE     (4)
#define GATT_INFO_DATA_UUID_128_SIZE    (18)
#define GATT_CHAR_DECL_UUID_16_SIZE     (7)
#define GATT_CHAR_DECL_UUID_128_SIZE    (21)

typedef enum
{
    GATT_STATE_IDLE,
    GATT_STATE_WAIT_EXCHG_MTU_RSP,
    GATT_STATE_WAIT_FIND_BY_TYPE_VALUE_RSP,
    GATT_STATE_WAIT_READ_BY_TYPE_RSP,
    GATT_STATE_WAIT_FIND_INFO_RSP,
    GATT_STATE_WAIT_WRITE_RSP,    
} GATT_STATE;

typedef struct
{
    U8 state;
    GATT_EVT_STRU XDATA_PTR reportPtr;
    ATT_PARA_STRU XDATA_PTR attParaPtr;
    GATT_EVENT_CB cb;
} GATT_CTL_TYPE;

GATT_CTL_TYPE XDATA gGATT_ctl[MAX_NUM_OF_LINK];
#endif
#endif

#endif