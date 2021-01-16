#ifndef _FLASH_DESCRIPTOR_H_
#define _FLASH_DESCRIPTOR_H_
#include "config.h"
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_flash_descriptor
{
    VOID (*fFlashInitEntry)(VOID);
    BOOL (*fSectorsCheckFuncEntry)(VOID);
    OS_GENERIC_PTR pFlashCodeDescriptor;
    OS_GENERIC_PTR pSectorHdrConfig0;
    OS_GENERIC_PTR pSectorHdrConfig1;
    OS_GENERIC_PTR pSectorHdrDspData;
    OS_GENERIC_PTR pSectorHdrBoundary;
    OS_GENERIC_PTR pSectorHdrVoiceData;
    OS_GENERIC_PTR pSectorHdrRuntime;
    OS_GENERIC_PTR pSectorHdrToolMisc;

    U32 McuHcontEnd;
    OS_GENERIC_PTR pSectorHdrRuntime_1;
    OS_GENERIC_PTR pSectorHdrRuntime_2;
    OS_GENERIC_PTR reserved_dw52;
    OS_GENERIC_PTR reserved_dw56;

    U16 OtaUpgrateVersion;
    CRC16 CRC;

} FLASH_DESCRIPTOR_STRU;



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN FLASH_DESCRIPTOR_STRU CODE_PTR gFlashDescriptor;

#endif /* _FLASH_DESCRIPTOR_H_ */

