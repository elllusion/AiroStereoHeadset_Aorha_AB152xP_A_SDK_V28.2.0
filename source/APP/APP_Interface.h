


#ifndef _APP_INTERFACE_H_
#define _APP_INTERFACE_H_


#define ADV_FLAGS_LEN_OFFSET		1
#define ADV_LOCALNAME_LEN_OFFSET	2
#define ADV_LOCALNAME_OFFSET		4	//flags_len + falsgs_type + local_name_len + local_name_type
#define ADV_LOCALNAME_TYPE_LEN		1
#define SEPARETE_LONCALNAME_OFFSET	2
#define UART_COMMAND_OCF_OGF		2
#define LENGTH_POSITION				2

#define LOCAL_NAME_SHIFT_OFFSET		0
#define DEVICE_NAME_LEN_POSITION	1
#define LOCALNAME_BACKWARD_POSITION	2	//including type

#define APP_HCI_CMD_PACKET		    0x01
#define APP_HCI_DATA_PACKET		    0x02
#define APP_HCI_EVENT_PACKET	    0x04

#ifdef BLE_APP
#define BLE_MAX_TX_DATA_LEN         270
#define BLE_MAX_RX_DATA_LEN         270
#define BLE_MAX_SEND_TX_DATA_LEN    (gATT_ctl.attMtu - ATT_REPORT_HEADER_LEN)
#endif  //#ifdef BLE_APP

enum
{
    A2DP,
    LINE_IN,
};

typedef struct
{
    //Statue(Battery, Vol, PEQ_Mode, VP_Enable/Disable, VP_Lenguage)
    U8 Battery;
    U8 Vol;
    U8 PEQ_Mode[2];
    U8 VP_OnOff;
    U8 VP_Lenguage;
}APP_STATUS;

//#ifdef MMI_LE_APP_ENABLE
typedef struct
{
//===Basic Features===
APP_STATUS Status; //Statue(Battery, Vol, PEQ_Mode, VP_Enable/Disable, VP_Lenguage)
U8 CCCD_Bat[2];
U8 BD_ADDR[6];
U16 FW_Version;
U8 DeviceName[20];
U8 Reset_PDL;
U8 Vol_Ctl;
U8 PEQ_Mode_Switch;
U8 VoicePromptSwitch;
U8 VP_Language_Switch;
U8 FindMyAccesory;
#ifdef LIGHTING_ENABLE
U8 OnOff_MusicYouCanSee;
U8 RGB[3];
U8 LightingMode;
U8 Alarm_Clock;
#endif  //#ifdef LIGHTING_ENABLE
//=======================

//===Advanced Features===
U8 VoicePromptAdj;
U8 CallerNameAnnouncement;
U8 RemoteUpdateFW_Cmd;
U8 RemoteUpdateFW_Data;
U8 phoneNumber[20];
U8 phnoeNumber_CCCD[2];
//=======================

#ifdef BLE_APP
//===AirAPP Features===
U8 TxIdx;
U8 TxCccdIdx;
U8 TX_CCCD[2];
U8 Rx[BLE_MAX_RX_DATA_LEN];
//=======================
#endif  //#ifdef BLE_APP
}ATT_APP_DEF;

U8 Status_CB (U8 CBtype);
U8 Read_BDAddr_CB (U8 CBtype);
U8 Read_FW_Version_CB (U8 CBtype);
U8 RW_DeviceName_CB (U8 CBtype);
U8 ResetPDL_CB (U8 CBtype);
U8 Vol_Ctl_CB (U8 CBtype);
U8 PEQ_Mode_Switch_CB (U8 CBtype);
U8 VoicePromptSwitch_CB (U8 CBtype);
U8 VP_Language_Switch_CB (U8 CBtype);
U8 FindMyAccesory_CB (U8 CBtype);

U8 APP_ReadBdAddr_CB (U8 CBtype);

#ifdef LIGHTING_ENABLE
U8 OnOffMusicYouCanSee_CB (U8 CBtype);
U8 RGB_CB (U8 CBtype);
U8 LightingMode_CB (U8 CBtype);
U8 AlarmClock_CB (U8 CBtype);
#endif  //#ifdef LIGHTING_ENABLE

#ifdef BLE_APP
//===AirAPP Features===
U8 Rx_CB (U8 CBtype);
//=======================
#endif  //#ifdef BLE_APP

#ifdef MMI_LE_APP_ENABLE
#ifdef __SERVICE_HEADER_INCLUDED_BY_PROFILE__

ATT_APP_DEF XDATA AppCtl =
{
//===Basic Features===
{1,2,3,4,5,6},//U8 Status[6]; //Statue(Battery, Vol, PEQ_Mode, VP_Enable/Disable, VP_Lenguage)
{0,0},//U8 CCCD_Bat[2];
{0x11,0x22,0x33,0x44,0x55,0x66},//U8 BD_ADDR[6];
{0x0000},//U16 FW_Version;
{'G','i','r','o','h','a',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},//U8 DeviceName[20];
0,//U8 Reset_PDL;
0,//U8 Vol_Ctl;
0,//U8 PEQ_Mode_Switch;
0,//U8 VoicePromptSwitch;
0,//U8 VP_Language_Switch;
0,//U8 FindMyAccesory;
//=======================

//===Advanced Features===
0,//U8 VoicePromptAdj;
0,//U8 CallerNameAnnouncement;
0,//U8 RemoteUpdateFW_Cmd;
0,//U8 RemoteUpdateFW_Data;
//=======================

#ifdef BLE_APP
//===AirAPP Features===
0,//U8 ;
{0,0},//U8 TX_CCCD[2];
{0},//U8 Rx[BLE_MAX_RX_DATA_LEN];
//=======================
#endif  //#ifdef BLE_APP
};

#else   //#ifdef __SERVICE_HEADER_INCLUDED_BY_PROFILE__

extern ATT_APP_DEF XDATA AppCtl;
BOOL APP_sendTxData(U8 XDATA_PTR dataPtr, U16 len);

#endif  //#ifdef __SERVICE_HEADER_INCLUDED_BY_PROFILE__
#endif  //#ifdef MMI_LE_APP_ENABLE
#endif  //#ifndef _APP_INTERFACE_H_
