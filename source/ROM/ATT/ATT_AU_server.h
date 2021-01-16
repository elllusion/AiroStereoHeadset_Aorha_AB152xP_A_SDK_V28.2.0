#ifdef AIR_UPDATE_ENABLE
//////////////////////////////////////////////////////////
// Copyright@ 2014 Airoha.  All rights reserved.
//////////////////////////////////////////////////////////
#ifndef _ATT_SERVER_H_ 
#define _ATT_SERVER_H_

#include <cpu.h>
#include "ATT_AirUpdate.h"

typedef enum
{
	ATT_H_RESERVED 	  = 0x0000,

	AU_H_PRIM_SERVICE = 0x0001,
	AU_H_AUTH_CHAR,
	AU_H_AUTHENTICATION,
	AU_H_AUTH_CIPHER,
	AU_H_ACTION_SETUP_CHAR,
	AU_H_ACTION_SETUP,
	AU_H_FLASH_DATA_CHAR,
	AU_H_FLASH_DATA,
    AU_H_FLASH_DATA_CCCD,
    AU_H_DATA_END_CHAR,
    AU_H_DATA_END,
    AU_H_ACTION_REPORT_CHAR,
    AU_H_ACTION_REPORT,
    AU_H_ACTION_REPORT_CCCD,
    AU_H_REBOOT_CHAR,
    AU_H_REBOOT,
    AU_H_FLASH_INFO_CHAR,
    AU_H_FLASH_INFO,
    AU_H_SFR_ACCESS_CHAR,
    AU_H_SFR_ACCESS,
    AU_H_SFR_ACCESS_RSP_CHAR,
    AU_H_SFR_ACCESS_RSP,
    AU_H_SFR_ACCESS_RSP_CCCD,

	ATT_H_BATTERY_SERVICE,
	ATT_H_BAS_LEVEL_CHAR,
	ATT_H_BAS_LEVEL,
	ATT_H_BAS_CCCD,
	ATT_H_GAP_SERVICE,
	ATT_H_GAP_INCLUDE,
	ATT_H_GAP_NAME_CHAR,
	ATT_H_GAP_NAME,
	ATT_H_GAP_NAME_DISC,
	ATT_H_GAP_APPEAR_CHAR,
	ATT_H_GAP_APPEAR,
	ATT_H_GATT_SERVICE,
	ATT_H_DIS_SERVICE,
	ATT_H_DIS_MANU_CHAR,
	ATT_H_DIS_MANU,
	ATT_H_DIS_SRV_CHAR,
	ATT_H_DIS_SRV,
	
	ATT_ATTRIBUTE_TOTAL_NUM
}ATT_HANDLE;

#define ATT_AU_SERVER_SIZE         		ATT_ATTRIBUTE_TOTAL_NUM
#define ATT_AU_MAX_ATTRIBUTE_COUNT 		ATT_ATTRIBUTE_TOTAL_NUM

#define ATT_R_PMT(p)            ((p)&0x0F)
#define ATT_W_PMT(p)            ((p)&0xF0)

typedef union {
    U8 CODE_PTR  Cptr;
    struct{
        U8 rsvd[2];
        U8 XDATA_PTR ptr;  
    }Xptr;
}CODE_XDATA_PTR_UNION;

typedef struct {
	U16 handle;
    U16 uuid;
    U16 len;
	U8 permission;
    
	CODE_XDATA_PTR_UNION dataptr; 
}attribute;

typedef struct{
    attribute att[ATT_AU_SERVER_SIZE];
}ATT_SERVER_TYPE_AU;

typedef struct{
    U8 Char_Property;
    U16 Handle;
    U16 UUID;
}ATT_CHAR_DECLARE_TYPE;

typedef struct{
    U8 AU_PrimServ[2];
    U8 AU_AuthChar[5];
    U8 AU_AuthPlain[AU_AUTH_PLAIN_LEN];
    U8 AU_AuthCipher[AU_AUTH_LEN];
    U8 AU_ActionSetupChar[5];
    AU_ActionSetupType AU_ActionSetup;
    U8 AU_FlashDataChar[5];
    AU_FlashDataType AU_FlashData;
    U8 AU_FlashDataCCCD[2];
    U8 AU_DataEndChar[5];
    U8 AU_DataEnd;
    U8 AU_ActionReportChar[5];
    AU_ActionReportType AU_ActionReport;
    U8 AU_ActionReportCCCD[2];
    U8 AU_RebootChar[5];
    U8 AU_Reboot;
    U8 AU_FlashInfoChar[5];
    U8 AU_FlashInfo;
    U8 AU_SFR_AccessChar[5];
    AU_SFR_AccessType AU_SFR_Access;
    U8 AU_SFR_AccessRspChar[5];
    AU_SFR_AccessRspType AU_SFR_AccessRsp;
    U8 AU_SFR_AccessRspCCCD[2];

    U8 BAS_PrimServ [ 2];
    U8 BAS_LevelChar[ 5];
    U8 BAS_Level    [ 1];
    U8 BAS_CCCD     [ 2];

    U8 GAP_PrimServ [ 2];
    U8 Test_include [ 6];
    U8 GAP_NameChar [ 5];
    U8 GAP_Name     [ 9];
    U8 GAP_Descrip  [48]; 
    U8 GAP_AppearChar[5];
    U8 GAP_Appear   [ 2];

    U8 GATT_PrimServ[ 2];

    U8 DIS_PrimServ [ 2];
    U8 DIS_ManuChar [ 5];
    U8 DIS_Manu     [12];
    U8 DIS_SoftChar [ 5];
    U8 DIS_SoftRV   [ 6];    
}ATT_SERVER_VALUE_AU;

extern ATT_SERVER_VALUE_AU CODE gATT_Value_AU;
extern ATT_SERVER_TYPE_AU CODE gATT_Server_AU;

extern ATT_SERVER_VALUE_AU XDATA_PTR pATT_Value_AU;
extern ATT_SERVER_TYPE_AU XDATA_PTR pATT_Server_AU;

#endif
#endif
