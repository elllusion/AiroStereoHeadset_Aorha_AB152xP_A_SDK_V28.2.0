#ifndef _SECTOR_SM_NVRAM_H_
#define _SECTOR_SM_NVRAM_H_

#include "..\SM\SM_interface.h"

typedef struct
{
	U8 leSecurityRequirement;

	U8 oobDataFlag:1;
	U8 bondingFlag:2;
	U8 MITM:1;
	U8 ioCapability:3;
	U8 bdAddrType:1;

	U8 keyDistr;	// bit0~bit2: initKeyDistr, bit3~bit5: respKeyDistr
	U8 encKeySize;
}SM_NVRAM_TYPE;

typedef struct
{
    SM_NVRAM_TYPE sm_nvram;

    CRC16 CRC;

} SECTOR_SM_NVRAM_STRU;

#endif //_SECTOR_SM_NVRAM_H_