#ifndef _STRUCT_DRIVER_RESERVED_H_
#define _STRUCT_DRIVER_RESERVED_H_

#include <crc16.h>

typedef struct driver_reserved_data_st
{
	U8 reserved;
}DRIVER_RESERVED_DATA_TYPE;


typedef struct
{
	DRIVER_RESERVED_DATA_TYPE	driver_reserved_data;
	CRC16 CRC;
} SECTOR_DRIVER_RESERVED_DATA_STRU;


#endif