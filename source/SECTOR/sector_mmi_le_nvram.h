#ifndef _SECTOR_MMI_LE_NVRAM_H_
#define _SECTOR_MMI_LE_NVRAM_H_

#include <crc16.h>

#define MMI_LE_ADV_DATA_MAX_DATA_LEN    31
#define MMI_LE_SCAN_RSP_MAX_DATA_LEN    31

typedef struct
{
	U8 adv_data_Len;                            //adv data total len
	U8 adv_data[MMI_LE_ADV_DATA_MAX_DATA_LEN];  //adv data
}LE_ADV_DATA_TYPE;

typedef struct
{
	U16 advIntMin;      //Adv_Interval_Min, 0x0020~0x4000, time unit:0.625 ms
	U16 advIntMax;      //Adv_Interval_Max, 0x0020~0x4000, time unit:0.625 ms
	U8 advType;         //adv_Type
    U8 ownAddrType;     //Own_Addr_Type
    U8 peerAddrType;    //Peer_Addr_Type
    U8 peerAddr[6];     //Peer_Addr
    U8 channelMap;      //Adv_Channel_Map
	U8 filterPolicy;    //Adv_Filter_Policy 
}LE_ADV_PARA_TYPE;

typedef struct
{
	U16 intMin;         //Conn_Interval_Min, 0x0008~0x0C80, time unit:1.25 ms
	U16 intMax;         //Conn_interval_Max, 0x0010~0x0C80, time unit:1.25 ms
	U16 slaveLatency;   //Slave_Latency, 0x0000~0x03E8
	U16 timeout;        //Supervision, 0x00A0~0x0C80, time unit:10 ms
}LE_CONN_SETTING_TYPE;

typedef struct
{
	U8 scan_rsp_data_len;                           //Scan rsp data length
	U8 scan_rsp_data[MMI_LE_SCAN_RSP_MAX_DATA_LEN]; //Scan rsp data
}LE_SCAN_RSP_TYPE;

typedef struct
{
    LE_ADV_DATA_TYPE advData;           //Advertising Data
    LE_ADV_PARA_TYPE advPara;           //Advertising Parameter
	LE_CONN_SETTING_TYPE connSetting;   //connection update req para
	LE_SCAN_RSP_TYPE scanRsp;           //Scan response
	U8 advTimout;                       //Adv_Timout 0x00~0xFF (0x00: won't enter reduce power adv)reduce_power_timeout = PairModeTimeout - adv_timeout
	U16 rPowerAdvIntMin;                //Reduce_Power_Adv_Interval_Min, 0x0020~0x4000, time unit:0.625 ms
	U16 rPowerAdvIntMax;                //Reduce_Power_Adv_interval_Max, 0x0020~0x4000, time unit:0.625 ms
}MMI_LE_NVRAM_TYPE;

typedef struct
{
    MMI_LE_NVRAM_TYPE mmi_le_nvram;

    CRC16 CRC;

}SECTOR_MMI_LE_NVRAM_STRU;

#endif //_SECTOR_SM_NVRAM_H_
