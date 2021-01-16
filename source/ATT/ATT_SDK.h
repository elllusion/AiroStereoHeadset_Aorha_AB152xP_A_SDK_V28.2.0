#ifdef LE_SUPPORTED_HOST_FLASH

#ifndef _ATT_SDK_H_
#define _ATT_SDK_H_

#define CB_BEFORE_SENDING 			0x01
#define CB_AFTER_WRITING 			0x02

#define CB_STATUS_OK				0x00


#define SDK_STATUS_OK				0x01
#define SDK_STATUS_INVALID_TIMER	0x02
#define SDK_STATUS_INVALID_ADDR		0x03
#define SDK_STATUS_INVALID_CHAR		0x04
#define SDK_STATUS_TABLE_TOO_SMALL	0x05


//PeriodicReport type
#define ATT_REPORT_NOTIFY			0x01
#define ATT_REPORT_INDICATE			0x02


#ifdef MCU_ANDES

	#define UUID_TO_PTR(type,uuid)				(type *)(U32)uuid

#else 
//#elif defined MCU_C251

	#define UUID_TO_PTR(type,uuid)				(type *)(U16)uuid
	#define NULL_PTR  							((U8 XDATA_PTR)0x00)

#endif


//////////////////////////////////////////////////////////////////////////////////////
typedef U8 SDK_STATUS;


//SDK_STATUS
#define SDK_SUCCESS				0x00
#define SDK_FAILURE				0x01
#define SDK_INVALID_VALUE		0x02
#define SDK_INVALID_LENGTH		0x03


//SDK set LE Advertising parameter type
#define ADV_DURATION				0x00
#define ADV_PARA_INTVL_MIN			0x01
#define ADV_PARA_INTVL_MAX			0x02
#define ADV_PARA_ADV_TYPE			0x03
#define ADV_PARA_OWN_ADDR_TYPE		0x04
#define ADV_PARA_DIR_ADDR_TYPE		0x05
#define ADV_PARA_DIR_ADDR			0x06
#define ADV_PARA_CHANNEL_MAP		0x07
#define ADV_PARA_FILTER_POLICY		0x08


//SDK function prototype
PUBLIC SDK_STATUS SDK_SetADVParam(U8 type, void* value);
PUBLIC SDK_STATUS SDK_SetADVData(U8 len, U8* value);
PUBLIC SDK_STATUS SDK_SetScanData(U8 len, U8* value);

//parameters that may be customized
#define DFLT_LE_ADV_DURATION				180					//Unit: second

#define DFLT_LE_ADV_PARA_INTVL_MIN			0x0020				//range: 0x0020 ~ 0x4000
#define DFLT_LE_ADV_PARA_INTVL_MAX			0x0040				//range: 0x0020 ~ 0x4000
#define DFLT_LE_ADV_PARA_ADV_TYPE			0x00
#define DFLT_LE_ADV_PARA_OWN_ADDR_TYPE		0x00
#define DFLT_LE_ADV_PARA_DIR_ADDR_TYPE		0x00
#define DFLT_LE_ADV_PARA_DIR_ADDR			{0x11,0x22,0x33,0x44,0x55,0x66}
#define DFLT_LE_ADV_PARA_CHANNEL_MAP		0x07
#define DFLT_LE_ADV_PARA_FILTER_POLICY		0x00

#define DFLT_LE_ADV_DATA_LENGTH				0x16
#define DFLT_LE_ADV_DATA				  {	0x02, 0x01, 0x05, \
											0x03, 0x03, 0x12, 0x18, \
    										0x03, 0x19, 0xC0, 0x03, \
    										0x0A, 0x09, 'A' , 'i' , 'r' , 'o', 'h', 'a', 'S', 'D', 'K' }

#define DFLT_LE_ADV_SCAN_DATA_LENGTH		0x03
#define DFLT_LE_ADV_SCAN_DATA			  {	0x02, 0x0A, 0x00 }




#endif /* _ATT_SDK_H_ */
#endif /* LE_SUPPORTED_HOST_FLASH */

