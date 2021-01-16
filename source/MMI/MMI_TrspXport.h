#ifndef _MMI_TRSPXPORT_H_
	#define _MMI_TRSPXPORT_H_


#ifdef _MMI_TRSPX_C_
#define MMI_TRSPX_EXTERN
#else
#define MMI_TRSPX_EXTERN extern
#endif

	#ifdef TRSPX_App

    #define MMI_TRSPX_PASSKEY_INVALID       0xFFFFFFFF
    #define MMI_TRSPX_ETRKEY_INVALID_VALUE  0xFF

    enum
    {
        MMI_TRSPX_CONNECTED_NONE,
        MMI_TRSPX_CONNECTED_BR_EDR,
        MMI_TRSPX_CONNECTED_BLE,        
    };

	typedef struct
	{
        U8 mmiBleIdx;
	}MmiTrspxCtlType;

    MMI_TRSPX_EXTERN MmiTrspxCtlType XDATA gTrspx_ctl;

    PUBLIC void MMI_TrspX_Init(void);
    PUBLIC void MMI_TrspX_BleConnected(U8 XDATA_PTR addrPtr, U16 connHandle);
    PUBLIC void MMI_TrspX_BleDisconnect(void);
    PUBLIC void MMI_TrspX_connectHandle(U8 linkIndex, U8 connMode);
	#endif

#endif

