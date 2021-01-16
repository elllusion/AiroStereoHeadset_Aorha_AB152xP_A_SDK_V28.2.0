#ifdef LE_SUPPORTED_HOST_FLASH

#define  __SERVICE_HUMAN_INTERFACE_DEVICE_H__


typedef struct
{
    U8 HIDInfo[4];
    U8 ReportMap[82];
    U8 BootMouseIReport[3];
    U8 BMIRCCCD[2];
    U8 Report1[6];
    U8 Report1_CCCD[2];
    U8 Report1_Reference[2];
    U8 Report2[1];
    U8 Report2_Reference[2];
    U8 Report2_UserDiscrip[65];
    U8 HIDCtlPoint[1];
    U8 ProtocolMode[1];
}ATT_VALUE_HID_T;

#ifdef __SERVICE_HEADER_INCLUDED_BY_PROFILE__

ATT_VALUE_HID_T XDATA gAttValueHID =
{
    //HIDInfo
    {   0x13, 0x02, 0x40, 0x01},
    //ReportMap
    {   0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x09, 0x01, 0xA1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x05,
        0x15, 0x00, 0x25, 0x01, 0x95, 0x05, 0x75, 0x01, 0x81, 0x02, 0x95, 0x01, 0x75, 0x03, 0x81, 0x03, 
        0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x16, 0x00, 0xF8, 0x26, 0xFF, 0x07, 0x75, 0x10, 0x95, 0x02, 
        0x81, 0x06, 0x09, 0x38, 0x15, 0x80, 0x25, 0x7F, 0x75, 0x08, 0x95, 0x01, 0x81, 0x06, 0x09, 0x01, 
        0x15, 0x00, 0x25, 0x03, 0x95, 0x01, 0x75, 0x02, 0xB1, 0x02, 0x95, 0x01, 0x75, 0x06, 0xB1, 0x03, 
        0xC0, 0xC0},  
    //BootMouseIReport
    {   0x00, 0x00, 0x00},
    //BMIRCCCD
    {   0x00, 0x00},
    //Report1
    {   0x00, 0x0A, 0x00, 0x00, 0x00, 0x00},
    //Report1_CCCD
    {   0x00, 0x00},
    //Report1_Reference
    {   0x00, 0x01},
    //Report2;
    {   0x03},
    //Report2_Reference;
    {   0x00, 0x03},
    //Report2_UserDiscrip;
    {   0x42, 0x4C, 0x45, 0x20, 0x4D, 0x4F, 0x55, 0x53, 0x45, 0x20, 0x52, 0x45, 0x53, 0x4F, 0x4C, 0x55, 
        0x54, 0x49, 0x4F, 0x4E, 0x20, 0x49, 0x4E, 0x20, 0x43, 0x50, 0x49, 0x0A, 0x20, 0x30, 0x20, 0x3D, 
        0x20, 0x34, 0x30, 0x30, 0x0A, 0x20, 0x31, 0x20, 0x3D, 0x20, 0x38, 0x30, 0x30, 0x0A, 0x20, 0x32, 
        0x20, 0x3D, 0x20, 0x31, 0x32, 0x30, 0x30, 0x0A, 0x20, 0x33, 0x20, 0x3D, 0x20, 0x31, 0x36, 0x30, 
        0x30},
    //HIDCtlPoint;
    {   0x00},
    //ProtocolMode;
    {   0x01},
};

#else

extern ATT_VALUE_HID_T XDATA gAttValueHID;

U8 HOGP_ReportUpdate_CB (U8 CBtype);

#endif


#endif /* LE_SUPPORTED_HOST_FLASH */