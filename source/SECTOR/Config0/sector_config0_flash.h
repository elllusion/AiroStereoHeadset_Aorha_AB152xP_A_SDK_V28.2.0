#ifndef _SECTOR_CONFIG0_FLASH_H_
#define _SECTOR_CONFIG0_FLASH_H_

#include "crc16.h"
#include "flash_descriptor.h"
#include "sectors.h"
#include "sector_config0.h"
#include "sector_afe_init.h"
#include "sector_sfr_init.h"
#include "sector_rf_misc_ctl.h"
#include "sector_hw_misc_ctl.h"
#include "sector_uart_parameter.h"


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SECTOR_CFG0_HEADER_STRU CODE gSectorConfig0Header;



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GET_SECTOR_CFG0_ADDR(A) ((SECTOR_CFG0_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrConfig0)->SectorInfo[A].addr
#define GET_SECTOR_CFG0_LEN(A)  ((SECTOR_CFG0_HEADER_STRU CODE_PTR)gFlashDescriptor->pSectorHdrConfig0)->SectorInfo[A].length

#endif /* _SECTOR_CONFIG0_FLASH_H_ */
