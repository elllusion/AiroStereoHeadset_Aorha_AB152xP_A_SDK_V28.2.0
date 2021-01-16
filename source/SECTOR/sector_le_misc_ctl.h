#ifndef SECTOR_LE_MISC_H_
#define SECTOR_LE_MISC_H_

#include <crc16.h>

typedef struct
{
    U8 autoInitiateLenUpdateReq;
    U16 connInitMaxRxOctets;    
    U16 connInitMaxRxTime;        
    U16 connInitMaxTxOctets;
    U16 connInitMaxTxTime;    
    U16 supportedMaxRxOctets;
    U16 supportedMaxRxTime;
    U16 supportedMaxTxOctets;
    U16 supportedMaxTxTime;    
    
}LE_DATA_LENGTH_CTL_STRU;

typedef struct
{
    LE_DATA_LENGTH_CTL_STRU le_data_length_ctl;
}LE_MISC_CONTROL_STRU;

typedef struct stru_sector_le_misc_ctl
{
    LE_MISC_CONTROL_STRU LeMiscCtl;

    CRC16 CRC;

} SECTOR_LE_MISC_CTL_STRU;

#endif
