#ifndef _SECTOR_BOUNDARY_H_
#define _SECTOR_BOUNDARY_H_

#include "crc16.h"
#include "flash_descriptor.h"
#include "sectors.h"
#include "sector_mp_parameter.h"
#include "sector_mp_parameter_F.h"
#include "..\DRIVER\voice_command_adaption_data_ctl.h"

#ifdef AB1520B
#include "sector_sys_patch_code_init.h"
#endif

#ifndef AB1520B
#include "sector_dsp_func_para.h"
#include "sector_peq_hpf_param.h"
#endif



#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	SECTOR_NAME,
enum
{
	#include "sector_boundary_member.h"
	NUM_OF_SECTOR_BOUNDARY
};

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_BOUNDARY];

    CRC16 CRC;

} SECTOR_BOUNDARY_HEADER_STRU;

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_BOUNDARY_HEADER_STRU CODE gSectorBoundaryHeader;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GET_SECTOR_BOUNDARY_ADDR(A) ((SECTOR_BOUNDARY_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrBoundary)->SectorInfo[A].addr
#define GET_SECTOR_BOUNDARY_LEN(A)  ((SECTOR_BOUNDARY_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrBoundary)->SectorInfo[A].length

#endif /* _SECTOR_BOUNDARY_H_ */
