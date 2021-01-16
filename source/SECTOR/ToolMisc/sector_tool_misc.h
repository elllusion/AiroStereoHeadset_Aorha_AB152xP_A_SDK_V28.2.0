#ifndef _SECTOR_TOOL_MISC_H_
#define _SECTOR_TOOL_MISC_H_

#include "crc16.h"
#include "flash_descriptor.h"
#include "sectors.h"
#include "sector_ae_info.h"
#include "sector_tool_info.h"


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	SECTOR_NAME,
enum
{
	#include "sector_tool_misc_member.h"
	NUM_OF_SECTOR_TOOL_MISC
};

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_TOOL_MISC];

    CRC16 CRC;

} SECTOR_TOOL_MISC_HEADER_STRU;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_TOOL_MISC_HEADER_STRU CODE gSectorToolMiscHeader;

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GET_SECTOR_TOOLMISC_ADDR(A) ((SECTOR_TOOL_MISC_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrToolMisc)->SectorInfo[A].addr
#define GET_SECTOR_TOOLMISC_LEN(A)  ((SECTOR_TOOL_MISC_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrToolMisc)->SectorInfo[A].length
#endif /* _SECTOR_TOOL_MISC_H_ */
