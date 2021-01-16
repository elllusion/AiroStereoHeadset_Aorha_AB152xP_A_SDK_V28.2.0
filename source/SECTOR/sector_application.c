
#include "sector_application.h"

#pragma userclass (HCONST = CONFIG)


PUBLIC SECTOR_APPLICATION_STRU CODE gApplication_init =
{
    {
        0x0000,                 //U16 appType
        0x0000,                 //U16 size
        0x00000000,             //U32 version
    },
    //- CRC
    {0,0}
};

