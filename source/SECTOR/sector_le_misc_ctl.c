#include "cpu.h"
#include "sector_le_misc_ctl.h"
#include "ctlr_config.h"

#pragma userclass (HCONST = CONFIG)


#if LE_SUPPORTED_CONTROLLER

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SECTOR_LE_MISC_CTL_STRU CODE gLE_MISC_Ctl_Init =
{

    {
        0,       //- U8 autoInitiateLenUpdateReq;
        246,     //- U16 connInitMaxRxOctets; 
        2080,    //- U16 connInitMaxRxTime;  
        246,     //- U16 connInitMaxTxOctets;
        2080,    //- U16 connInitMaxTxTime;  
        246,     //- U16 supportedMaxRxOctets;
        2080,    //- U16 supportedMaxRxTime;
        251,     //- U16 supportedMaxTxOctets;
        2120     //- U16 supportedMaxTxTime;    
     },
    //- CRC
    {0,0}
};
#endif