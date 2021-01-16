#ifdef LE_SUPPORTED_HOST_FLASH
//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _MMI_LE_H_
#define _MMI_LE_H_

#include "..\COMMON\le_common_hal.h"
#ifdef SM_SUPPORTED
#include "..\SM\sm_hal.h"
#endif
#include "MMI_LE_ROM.h"
#include "MMI_LE_ROM_interface.h"
#ifdef AB1520B
#include "sector_mmi_le_nvram.h"
#endif
#include "MMI_LE_interface.h"

#ifdef _MMI_LE_C_
	#define MMI_LE_EXTERN
#else
	#define MMI_LE_EXTERN extern	
#endif

#define MMI_LE_SINGLE_LINK
//#define MMI_LE_TEST_NOTIFICATION
#define MMI_LE_CONNCMP_SEND_CONNUPDATE
#define MMI_LE_NOTI_INTERVAL        HALF_SEC	
#define MMI_LE_SECURITY_REQ_TIME    (ONE_SEC*15)

typedef enum
{
    MMI_LE_SUBSTATE_EXPAND = MMI_LE_ROM_ENTER_SUSPEND,
    /* MMI_LE_READY_SUBSTATE */
    MMI_LE_READY_ENABLE_ADV_WAIT_EVT,
    /* MMI_LE_ADVERTISING_SUBSTATE */
    MMI_LE_ADV_DISABLE_WAIT_EVT
}MMI_LE_SUBSTATE;

typedef struct
{
    OST XDATA_PTR notiDurationTimerPtr;
    OST XDATA_PTR delayWriteFlashTimerPtr[MAX_NUM_OF_LINK];
    U8 crtLinkCnt;
    U8 rPowerAdv:1;
    U8 enableAdvAfterCMD:1;
    U8 powerOff:1;
    U8 discActive:1;
    U8 disableBle:1;
    U8 isInited:1;
    U8 reserved:2;
    U8 nextActionAfterDisc;
    U8 randomBdAddr[6];
} MMI_LE_CTL;

MMI_LE_EXTERN MMI_LE_CTL XDATA gMMI_le_ctl;
MMI_LE_EXTERN MMI_LE_APP_NODE XDATA_PTR gMMI_LE_AppNodeList;

#endif
#endif
