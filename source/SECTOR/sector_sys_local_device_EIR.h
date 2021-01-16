#ifndef _SECTOR_SYS_LOCAL_DEVICE_EIR_H_
#define _SECTOR_SYS_LOCAL_DEVICE_EIR_H_

#include "os.h"
#include "bt_config_profile.h"

#if defined(LC_TEST_CASE_CONFIG)
#define EIR_CAPACITY                    248
#elif defined(HEADSET_SOC)
#define EIR_CAPACITY                    48
#else
#define EIR_CAPACITY                    61
#endif


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8 fec_required;
    U8 ext_inquiry_len;
    U8 dummy_byte0;         //- LC USED
    U8 dummy_byte1;         //- LC USED
    U8 ext_inquiry_res[EIR_CAPACITY];
} SYS_LOCAL_DEVICE_EIR_STRU;

typedef struct stru_sector_sys_local_device_eir
{
    SYS_LOCAL_DEVICE_EIR_STRU	SysLocalDeviceInfo;

    CRC16 CRC;

} SECTOR_SYS_LOCAL_DEVICE_EIR_STRU;


#endif /* _SECTOR_SYS_LOCAL_DEVICE_EIR_H_ */
