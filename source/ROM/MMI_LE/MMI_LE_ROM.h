//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _MMI_LE_ROM_H_
#define _MMI_LE_ROM_H_

#include "MMI_LE_ROM_keypad.h"

#ifdef _MMI_LE_ROM_C_
	#define MMI_LE_ROM_EXTERN
#else
	#define MMI_LE_ROM_EXTERN extern
#endif


#define LE_ADV_DURATION     180  //unit: seconds
#define AUTO_SHUTDOWN_TIME  600  //unit: seconds

//Low battery ADC value
#define ADC_LOW_BAT_LI_MODE       575   //3.1v
#define ADC_LOW_BAT_N_LI_MODE     390   //2.1v


typedef struct
{
	U8 dataOffset;
	U8 reserveByte;
	U8 cmdOpcode;
	U8 eventCode;
	U8 paraLen;
	U8 cmdParaPtr[1];
}MMI_LE_ROM_CMDTYPE;

enum{
    LE_FAIL,
    LE_SUCCESS
};

enum{
    LE_ADV_DISABLE      = 0x00,
    LE_ADV_ENABLE       = 0x01
};

typedef struct
{
    #ifdef LE_SUPPORTED_HOST
    U8 MsgSentFlag;
    U8 MMI_LE_State;
    U8 MMI_LE_SubState;
    U16 connectionHDL;
    OST XDATA_PTR AdvDurationTimerPtr;
    #endif
    OST XDATA_PTR AutoShutdownTimerPtr;
    VOID CODE_PTR pLastLEDPattern;
    VOID CODE_PTR pCurrLEDPattern;
    #ifdef LE_SUPPORTED_HOST
    U16 ADCVinLowBattery;
//  key detection related variable in engineering mode
    U32 KeyLongPressInterval;
    OST XDATA_PTR KeyPressTimerPtr[AU_TOTAL_KEY_NO];
    U16 currKeyBitmap;
    U16 oldKeyBitmap;
    U8	KeyPressTimerType[AU_TOTAL_KEY_NO];
    VOID (*KeyPressAction)(VOID);
    VOID (*KeyLongPressAction)(VOID);
    #endif
} MMI_LE_ROM_CTL;

#ifdef DFU_LOADER_IN_BOOT_CODE
EXTERN MMI_LE_ROM_CTL XDATA gMMI_le_rom_ctl;
#else
MMI_LE_ROM_EXTERN MMI_LE_ROM_CTL XDATA gMMI_le_rom_ctl;
#endif


#endif
