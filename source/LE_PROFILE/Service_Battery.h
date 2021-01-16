#ifdef LE_SUPPORTED_HOST_FLASH

#define  __SERVICE_BATTERY_H__


typedef struct 
{
    U8 gBAS_Value;
	U8 BD_ADDR[6];
}BAS_CTRL;

#ifdef __SERVICE_HEADER_INCLUDED_BY_PROFILE__

U32 XDATA gBAS_CCCD = 0x0000;

BAS_CTRL XDATA gBAS_Ctrl =
{
    {0x64},
	{0},
};

#else

extern BAS_CTRL XDATA gBAS_Ctrl;
extern U32 XDATA gBAS_CCCD;
#endif

#endif /* LE_SUPPORTED_HOST_FLASH */
