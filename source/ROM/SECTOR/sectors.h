#ifndef _SECTORS_H_
#define _SECTORS_H_

#define CRC_CHK         1
#define CRC_NOT_CHK     0

typedef struct
{
    U8 CODE_PTR addr;
    U32 length;
    U8 attr;

} SECTOR_INFO_STRU;

#endif /* _SECTORS_H_ */
