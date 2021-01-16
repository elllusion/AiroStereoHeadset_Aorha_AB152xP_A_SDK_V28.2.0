#ifndef _SECTOR_MMI_LE_VARIATION_NVRAM_H_
#define _SECTOR_MMI_LE_VARIATION_NVRAM_H_

//#include "..\SM\SM_interface.h"
#include "sector_mmi_le_nvram.h"
#include "..\COMMON\le_common_hal.h"

#define VIBRATION_SETTING_LEN   6

typedef struct
{
    /* SM para */
	U8 peerAddr[6];
	U8 peerAddrType;
	U8 encKeySize;

	U32 sSignCounter;
	U32 mSignCounter;

	U8 sLTK[16];
	U8 mLTK[16];

	U8 sCSRK[16];
	U8 mCSRK[16];

    U8 peerIRK[16];

	U8 sMasterInfo[10];
	U8 keyDistr;        //initKeyDistr
    U8 respKeyDistr;    //respKeyDistr
	U8 mMasterInfo[10];

    U8 smpFlag;
}NVRAM_MMI_LINK_RECORD_TYPE;

typedef struct
{
    NVRAM_MMI_LINK_RECORD_TYPE nvram_MMI_link_record[MAX_NUM_LE_LINK];
	U8 nextRecordIdx;
    LE_ADV_DATA_TYPE advData;           //Advertising Data
    LE_ADV_PARA_TYPE advPara;           //Advertising Parameter
	LE_CONN_SETTING_TYPE connSetting;   //connection update req para
	LE_SCAN_RSP_TYPE scanRsp;           //Scan response
	U8 advTimout;                       //Adv_Timout 0x00~0xFF (0x00: won't enter reduce power adv)reduce_power_timeout = PairModeTimeout - adv_timeout
	U16 rPowerAdvIntMin;                //Reduce_Power_Adv_Interval_Min, 0x0020~0x4000, time unit:0.625 ms
	U16 rPowerAdvIntMax;                //Reduce_Power_Adv_interval_Max, 0x0020~0x4000, time unit:0.625 ms
	U8 vibrationSetting[VIBRATION_SETTING_LEN];
	U8 leFeatureCtl;
}NVRAM_MMI_LINK_DATA_TYPE;

typedef struct
{
    NVRAM_MMI_LINK_DATA_TYPE nvram_MMI_link_data;
	CRC16 crc;
}NVRAM_MMI_LINK_RECORD_STRU;

#endif //_SECTOR_MMI_LE_VARIATION_NVRAM_H_