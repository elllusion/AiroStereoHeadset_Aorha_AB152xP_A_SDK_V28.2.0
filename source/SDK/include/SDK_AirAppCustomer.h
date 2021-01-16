#include "..\SDK\include\SDK_Config.h"

/* HCI Vendor OGF App Command Group: HCI_VENDOR_OGF_AIRAPP_CUSTOMER_CMD */
enum
{
	/*							SAMPLE CODE START						*/
	SDK_AIRAPP_CST_SAMPLE1,
	/*							SAMPLE CODE END							*/
};

/* HCI_VENDOR_OGF_AIRAPP_CUSTOMER_RSP result*/
enum
{
	RSP_SUCCESS,
	RSP_FAIL,
};

PUBLIC void SDK_AirAppCustomerInit(void);