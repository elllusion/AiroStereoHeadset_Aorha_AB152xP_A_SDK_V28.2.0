#ifndef _SECTOR_DSP_DATA_H_
#define _SECTOR_DSP_DATA_H_

#include "flash_descriptor.h"
#include "sectors.h"
#ifdef AB1520B
#include "sector_dsp_func_para.h"
#include "sector_peq_hpf_param.h"
#endif
#include "sector_dsp_vp_nb.h"



#undef SECTORS_ENUM_MACRO
#define SECTORS_ENUM_MACRO(SECTOR_NAME, SECTOR_INIT, SECTOR_SIZE, ATTR)	SECTOR_NAME,
enum
{
	#include "sector_dsp_data_member.h"
	NUM_OF_SECTOR_DSP_DATA
};

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_DSP_DATA];

    CRC16 CRC;

} SECTOR_DSP_DATA_HEADER_STRU;

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_DSP_DATA_HEADER_STRU CODE gSectorDspDataHeader;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GET_SECTOR_DSP_DATA_ADDR(A) ((SECTOR_DSP_DATA_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrDspData)->SectorInfo[A].addr
#define GET_SECTOR_DSP_DATA_LEN(A)  ((SECTOR_DSP_DATA_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrDspData)->SectorInfo[A].length

#endif /* _SECTOR_DSP_DATA_H_ */
