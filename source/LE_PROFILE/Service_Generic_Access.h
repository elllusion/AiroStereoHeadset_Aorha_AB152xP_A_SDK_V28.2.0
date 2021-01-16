#ifdef LE_SUPPORTED_HOST_FLASH
#define  __SERVICE_GENERIC_ACCESS_H__


typedef struct 
{
    U8 DeviceName[10];
    U8 Appearance[2];
}ATT_VALUE_GAP_T;

#ifdef __SERVICE_HEADER_INCLUDED_BY_PROFILE__

ATT_VALUE_GAP_T XDATA gAttValueGAP = 
{
    {'A' ,'i' ,'r' ,'o' ,'h' ,'a' ,'_' ,'H' ,'I' ,'D' },
    {0xC2, 0x03},
};

#else

extern ATT_VALUE_GAP_T XDATA gAttValueGAP;

#endif

#endif /* LE_SUPPORTED_HOST_FLASH */