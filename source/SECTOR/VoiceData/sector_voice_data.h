#ifndef _SECTOR_VOICE_DATA_H_
#define _SECTOR_VOICE_DATA_H_

#include "flash_descriptor.h"
#include "sectors.h"
#include "..\DRIVER\voice_command_ctl.h"

EXTERN SECTOR_DRIVER_VOICE_COMMAND_STRU CODE gDriverVoiceCommandData;


#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	SECTOR_NAME,
enum
{
	#include "sector_voice_data_member.h"
	NUM_OF_SECTOR_VOICE_DATA
};

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_VOICE_DATA];

    CRC16 CRC;

} SECTOR_VOICE_DATA_HEADER_STRU;

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_VOICE_DATA_HEADER_STRU CODE gSectorVoiceDataHeader;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GET_SECTOR_VOICE_DATA_ADDR(A) ((SECTOR_VOICE_DATA_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrVoiceData)->SectorInfo[A].addr
#define GET_SECTOR_VOICE_DATA_LEN(A)  ((SECTOR_VOICE_DATA_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrVoiceData)->SectorInfo[A].length

#endif /* _SECTOR_VOICE_DATA_H_ */
