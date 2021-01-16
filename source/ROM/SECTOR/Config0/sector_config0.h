#ifndef _SECTOR_CONFIG0_H_
#define _SECTOR_CONFIG0_H_

#include "crc16.h"
#include "sectors.h"


enum
{
	SECTOR_AFE_INIT,
	SECTOR_SFR_INIT,
	SECTOR_RF_MISC_CTL,
	SECTOR_HW_MISC_CTL,
	#ifdef AB1520B
	#else
    SECTOR_UART_PARAMETER_ENG,
	#endif
	
	NUM_OF_SECTOR_CFG0
};

typedef struct
{
    U8 NumOfSector;

    SECTOR_INFO_STRU SectorInfo[NUM_OF_SECTOR_CFG0];

    CRC16 CRC;

} SECTOR_CFG0_HEADER_STRU;

#endif /* _SECTOR_CONFIG0_H_ */
