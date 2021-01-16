#ifdef LE_SUPPORTED_HOST_FLASH

#define  __SERVICE_DEVICE_INFORMATION_H__


typedef struct
{
    U8 SoftRV[6];
    U8 Manufacture[12];
}ATT_VALUE_DIS_T;

#ifdef __SERVICE_HEADER_INCLUDED_BY_PROFILE__

ATT_VALUE_DIS_T XDATA gAttValueDIS =
{
    {'0' ,'0' ,'0' ,'0' ,'0' ,'0' },
    {'A' ,'I' ,'R' ,'O' ,'H' ,'A' ,' ' ,'C' ,'o' ,'r' ,'p' ,'.'},
};

#else

extern ATT_VALUE_DIS_T XDATA gAttValueDIS;

#endif

#endif /* LE_SUPPORTED_HOST_FLASH */