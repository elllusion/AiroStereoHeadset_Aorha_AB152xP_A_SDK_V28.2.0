#ifndef _MMI_HID_H_
	#define _MMI_HID_H_
    
    #ifdef HID_Profile

    #ifdef _MMI_HID_C_
		#define MMI_HID_EXTERN
	#else
		#define MMI_HID_EXTERN extern
	#endif

    #define HID_LENGTH_OFFSET     (8)
    #define HID_HEADER_OFFSET     (12)  
    #define HID_REPORT_ID_OFFSET  (13)
    #define HID_DATA_OFFSET       (14)   

    #define MMI_HID_ACTION_SEND_SELFIE_KEY  (1)

    typedef struct
	{
		//U8 hidState;
		U8 actionMisc;
	}MmiHidCtlType;
	
	typedef struct
	{
		MmiHidCtlType hidCtl[MAX_MULTI_POINT_NO];
	}GMmiHidCtlType;
	
	MMI_HID_EXTERN GMmiHidCtlType XDATA gMMI_HID_ctl;
    
    MMI_HID_EXTERN PUBLIC void MMI_HID_CreateConnection(U8 linkIndex);
    MMI_HID_EXTERN PUBLIC BOOL MMI_HID_ReleaseServiceLink(U8 linkIndex);
    MMI_HID_EXTERN PUBLIC void MMI_HID_EventHandler(U8 linkIndex, U8 eventCode);
    MMI_HID_EXTERN PUBLIC void MMI_HID_ConnectedHandle(U8 linkIndex);
    MMI_HID_EXTERN PUBLIC void MMI_HID_ProcessUartCmd(U8 linkIndex, U16 len, U8 XDATA_PTR dataPtr, U8 XDATA_PTR msgPtr);
    MMI_HID_EXTERN PUBLIC void MMI_HID_ProcessKeyEvent(U8 linkIndex, U8 index);
    MMI_HID_EXTERN PUBLIC void MMI_HID_SetProfileAvailable(BOOL enabled);
    
    #endif
#endif
