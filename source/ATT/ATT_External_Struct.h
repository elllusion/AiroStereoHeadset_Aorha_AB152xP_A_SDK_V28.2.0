#ifdef LE_SUPPORTED_HOST_FLASH
#if defined(SUPPORT_LE_PROFILE_HOGP)

#include "..\LE_PROFILE\Service_Generic_Access.h"
#include "..\LE_PROFILE\Service_Battery.h"
#include "..\LE_PROFILE\Service_Device_Information.h"
#include "..\LE_PROFILE\Service_Human_Interface_Device.h"

#elif defined(SUPPORT_LE_XPORT)

#include "..\LE_XPORT\Service_LE_xport.h"

#else


typedef struct
{
	U8  test1;
	U16 test2;
	U8  test3[3];
}U_TYPE;

typedef struct{
	U8 gBAS_Value;
    U8 gTestVL1[5];
    U8 gTestVL2[10];
	U_TYPE gBAS_Level;
	U16 gBAS_CCCD;
	U8 BD_ADDR[6];
}BAS_CTRL;

#ifdef _ATT_SERVER_C_

U32 XDATA gBAS_CCCD = 0x0000;

BAS_CTRL XDATA gBAS_Ctrl = {
	0x78,
    {0},
    {0},
	{0x56,0x5566,{0x12,0x34,0xAA}},
	0x0002,
	{0},
};

#else

extern BAS_CTRL gBAS_Ctrl;
extern U32 XDATA gBAS_CCCD;
#endif

#endif
#endif /* LE_SUPPORTED_HOST_FLASH */