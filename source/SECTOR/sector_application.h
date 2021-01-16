#ifndef _SECTOR_APPLICATION_H_
#define _SECTOR_APPLICATION_H_

#include "cpu.h"
#include "crc16.h"

typedef struct
{
    U16 appType;
    U16 size;
    U32 version;
    
}APPLICATION_STRU;

typedef struct
{
    APPLICATION_STRU appCtl;
    
    CRC16 CRC;
}SECTOR_APPLICATION_STRU;

#endif /* _SECTOR_APPLICATION_H_ */