#include "reside_flash.inc"

#include "rc.h"
#include "..\SDK_Proxy\SDK_Proxy.h"
#include "..\MMI\MMI_Base.h"

#define EXTERNAL_CHARGER	0


#if EXTERNAL_CHARGER

#define HW_CHARGER_STATUS_IDLE		0
#define HW_CHARGER_STATUS_TRICKLE	1
#define HW_CHARGER_STATUS_CC_MODE	2
#define HW_CHARGER_STATUS_CV_INIT	3
#define HW_CHARGER_STATUS_CV_MODE	4
#define HW_CHARGER_STATUS_COMPLETE	5
/*  SDK_CB_Charger_ExternalChgStatus: callback function which is called in NDK when inquirying charger status
 *  <output>
 *		U8		: 	#define HW_CHARGER_STATUS_IDLE		0
 *					#define HW_CHARGER_STATUS_TRICKLE	1
 *					#define HW_CHARGER_STATUS_CC_MODE	2
 *					#define HW_CHARGER_STATUS_CV_INIT	3
 *					#define HW_CHARGER_STATUS_CV_MODE	4
 *					#define HW_CHARGER_STATUS_COMPLETE	5
 */
PRIVATE U8 SDK_CB_Charger_ExternalChgStatus(void)
{

}

/*  SDK_CB_Charger_ExternalChgIn: callback function which is called in NDK when inquirying if charger is plugged-in
 *  <output>
 *		U8		: 	0--> no charger; 1: charger is plugged-in
 */
PRIVATE U8 SDK_CB_Charger_ExternalChgIn(void)
{

}
#endif //EXTERNAL_CHARGER


/*  SDK_CB_ChgStatus: callback function which is called in NDK when charger status changes
 *  <input 1>
 *		U8 status	: 	#define CHG_STATUS_NO_CHARGER	1
 *						#define CHG_STATUS_CHARGER_IN	2
 *						#define CHG_STATUS_CHARGER_RECHARGE	3
 *						#define CHG_STATUS_CHARGER_FULL	4
 */
 /*
PRIVATE void SDK_CB_ChgStatus(U8 status)
{

} */


/*  SDK_Charger_IsPluggedIn: check if charger is plugged-in
 *  <output>
 *		BOOL	: 	0--> no charger; 1: charger is plugged-in
 */
PUBLIC BOOL SDK_Charger_IsPluggedIn(void)
{
	return (MMI_IsChargerIn()) ? TRUE : FALSE;
}

PUBLIC void SDK_Charger_Init(void)
{
	#if EXTERNAL_CHARGER
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_CHG_EXT_CHG_STATUS, 	API_GRP_IOSYS, (VFUN)SDK_CB_Charger_ExternalChgStatus);
	SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_CHG_EXT_CHG_IN, 		API_GRP_IOSYS, (VFUN)SDK_CB_Charger_ExternalChgIn);
	#endif
	//SDK_Proxy_RegisterAPI(API_GRP_IOSYS_CB_CHG_STATUS, 			API_GRP_IOSYS, (VFUN)SDK_CB_ChgStatus);
}