#ifndef _FLASH_DESCRIPTOR_OLD_H_
#define _FLASH_DESCRIPTOR_OLD_H_
#include "config.h"
#include "cpu.h"
#include "crc16.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct stru_flash_descriptor_at_begin
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
    OS_GENERIC_PTR pWorkingArea1;
    OS_GENERIC_PTR pWorkingArea2;
    VOID (*fEngineerInitEntry)(VOID);

	U32 reserved_w52;
	U32 reserved_w56;
    U16 reserved_w60;
    CRC16 CRC;

} FLASH_DESCRIPTOR_AT_BEGIN_STRU;


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define FLASH_DESCRIPTOR_ADDR_AT_BEGIN	0x800000L
#define gFlashDescriptorAtBegin		((FLASH_DESCRIPTOR_AT_BEGIN_STRU CODE_PTR)FLASH_DESCRIPTOR_ADDR_AT_BEGIN)

#endif /* _FLASH_DESCRIPTOR_H_ */


