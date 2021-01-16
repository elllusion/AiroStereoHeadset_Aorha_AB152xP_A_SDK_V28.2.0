#ifndef _SECTOR_RUNTIME_H_
#define _SECTOR_RUNTIME_H_


#include "config.h"
#include "bt_config_profile.h"
#include "flash_descriptor.h"
#include "sectors.h"
#include "sector_app_callername_data.h"

EXTERN SECTOR_APP_CALLERNAME_DATA_STRU CODE gSector_AppCallerNameData;


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	SECTOR_NAME,
enum
{
	#include "sector_runtime_member.h"
	NUM_OF_SECTOR_RUNTIME
};

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_RUNTIME];

    CRC16 CRC;

} SECTOR_RUNTIME_HEADER_STRU;

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_RUNTIME_HEADER_STRU CODE gSectorRuntimeHeader;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GET_SECTOR_RUNTIME_ADDR(A) ((SECTOR_RUNTIME_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrRuntime)->SectorInfo[A].addr
#define GET_SECTOR_RUNTIME_LEN(A)  ((SECTOR_RUNTIME_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrRuntime)->SectorInfo[A].length



#endif /* _SECTOR_RUNTIME_H_ */