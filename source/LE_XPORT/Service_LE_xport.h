#ifdef SUPPORT_LE_XPORT
#define LE_XPORT_ATT_MTU_SIZE   23
#define LE_XPORT_DATA_LEN       (LE_XPORT_ATT_MTU_SIZE - 3)  //[opcode:1][handle:2]
#define LE_XPORT_CONNPARA_LEN   9
#define LE_XPORT_PATCH_LEN      23
#define LE_XPORT_PATCH_CMD_LEN  20
#define LE_XPORT_PATCH_NOTI_LEN 20
#define LE_XPORT_MODEL_NUM_LEN  16
#define LE_XPORT_MANU_NAME_LEN  20
#define LE_XPORT_SW_RV_LEN      4
#define LE_XPORT_SYSTEM_ID_LEN  8

#if 0 //LE_XPORT_DIS_SERIAL_NUM_ENABLE
#define LE_XPORT_SERIAL_NUM_LEN 16
#endif
#if 0 //LE_XPORT_DIS_C_VERSION_ENABLE
#define LE_XPORT_VERSION_LEN 3
#endif
#if 0 //LE_XPORT_DIS_SPECIFIC_ENABLE
#define LE_XPORT_S_DIS_LEN 16
#endif

typedef struct 
{
    U8 status;
    U16 minInterval;
    U16 maxInterval;
    U16 slaveLatency;
    U16 timoutMultiplier;
}LE_XPORT_UPDATE_CONN;

typedef struct 
{
    U8 cmdID;
    U8 cmdPara[LE_XPORT_PATCH_CMD_LEN];
}LE_XPORT_AIR_PATCH_WRITE;

typedef struct 
{
    U8 status;
    U8 returnID;
    U8 returnPara[LE_XPORT_PATCH_NOTI_LEN];
}LE_XPORT_AIR_PATCH_NOTIFY;

typedef	union
{
	LE_XPORT_AIR_PATCH_WRITE w;
	LE_XPORT_AIR_PATCH_NOTIFY n;
}LE_XPORT_AIR_PATCH;

typedef struct 
{
    U8 TxIdx;
    U8 TX_CCCD[2];
    U8 RxIdx;
    U8 connPara[LE_XPORT_CONNPARA_LEN];
    U8 airPatch[LE_XPORT_PATCH_LEN];
    U8 airPatch_CCCD[2];
#if 0 //LE_XPORT_SPECIFIC_SERVICE_ENABLE
    U8 specific_TX[LE_XPORT_DATA_LEN];
    U8 specific_TX_CCCD[2];
    U8 specific_RX[LE_XPORT_DATA_LEN];
#endif
    U8 modelNum[LE_XPORT_MODEL_NUM_LEN];
#if 0 //LE_XPORT_DIS_SERIAL_NUM_ENABLE
    U8 serialNum[LE_XPORT_SERIAL_NUM_LEN];
#endif
    U8 manuName[LE_XPORT_MANU_NAME_LEN];
    U8 softRV[LE_XPORT_SW_RV_LEN];
    U8 systemID[LE_XPORT_SYSTEM_ID_LEN];
#if 0 //LE_XPORT_DIS_C_VERSION_ENABLE
    U8 hardRV[LE_XPORT_VERSION_LEN];
    U8 firmRV[LE_XPORT_VERSION_LEN];
#endif
#if 0 //LE_XPORT_DIS_SPECIFIC_ENABLE
    U8 s_DIS_1[LE_XPORT_S_DIS_LEN];
    U8 s_DIS_2[LE_XPORT_S_DIS_LEN];
#endif
}LE_XPORT_T;

extern LE_XPORT_T XDATA gleXport;

U8 LeXport_RxCB(U8 cbType, U8 linkIdx);
U8 LeXport_connUpdateCB(U8 cbType, U8 linkIdx);
U8 LeXport_AirPatchCB(U8 cbType, U8 linkIdx);
#if 0 //LE_XPORT_SPECIFIC_SERVICE_ENABLE
U8 LeXport_S_RxCB(U8 cbType, U8 linkIdx);
#endif
#endif
