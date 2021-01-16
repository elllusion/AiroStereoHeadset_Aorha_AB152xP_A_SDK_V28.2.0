#ifdef LE_SUPPORTED_HOST_FLASH

#if defined(SUPPORT_LE_PROFILE_HOGP)
//ATT_H_JUMP_TO(0x5566)
//SERVICE    [num]  [type]	[UUID]
//CHARACT    [R/W][size] [symbol name]	[char property]			[CallBack Function]		[UUID]
//DESCRIPTOR [R/W][size] [symbol name]	[CallBack Function]		[UUID]


//GAP service
SERVICE_016_M(	1,  0x2800,	0x1800)
CHARACT_016_M(0x81, 10,   gAttValueGAP.DeviceName,        (C_READ),          NULL,            0x2A00)
CHARACT_016_M(0x81,  2,   gAttValueGAP.Appearance,        (C_READ),          NULL,            0x2A01)

//GATT service
SERVICE_016_M(	2,  0x2800,	0x1801)

//HID service
SERVICE_016_M(	3,  0x2800,	0x1812)
CHARACT_016_M(0x81,  4, gAttValueHID.HIDInfo,               (C_READ),                   NULL,                   0x2A4A)
CHARACT_016_M(0x81, 82, gAttValueHID.ReportMap,             (C_READ),                   NULL,                   0x2A4B)
CHARACT_016_M(0x81,  3, gAttValueHID.BootMouseIReport,      (C_READ|C_NOTIFY),          NULL,                   0x2A33)
DESCRIPTORS_M(0x11,  2, gAttValueHID.BMIRCCCD,                                          NULL,	                0x2902)
CHARACT_016_M(0x81,  6, gAttValueHID.Report1,               (C_READ|C_NOTIFY),          HOGP_ReportUpdate_CB,   0x2A4D)
DESCRIPTORS_M(0x11,  2, gAttValueHID.Report1_CCCD,                                      NULL,	                0x2902)
DESCRIPTORS_M(0x11,  2, gAttValueHID.Report1_Reference,                                 NULL,	                0x2908)
CHARACT_016_M(0x11,  1, gAttValueHID.Report2,               (C_READ|C_WRITE),           NULL,                   0x2A4D)
DESCRIPTORS_M(0x11,  2, gAttValueHID.Report2_Reference,                                 NULL,	                0x2908)
DESCRIPTORS_M(0x11, 65, gAttValueHID.Report2_UserDiscrip,                               NULL,	                0x2901)
CHARACT_016_M(0x18,  1, gAttValueHID.HIDCtlPoint,           (C_WRITE_NO_RSP),           NULL,                   0x2A4C)
CHARACT_016_M(0x11,  1, gAttValueHID.ProtocolMode,          (C_READ|C_WRITE_NO_RSP),    NULL,                   0x2A4E)

//Battery service
SERVICE_016_M(	4,  0x2800,	0x180F)
CHARACT_016_M(0x81,  1,   gBAS_Ctrl.gBAS_Value,             (C_READ),                   NULL,                   0x2A19)
DESCRIPTORS_M(0x11,  2,   gBAS_CCCD,                                                    NULL,                   0x2902)
CHARACT_128_M(0x11, 6,    gBAS_Ctrl.BD_ADDR,                (C_READ),                   APP_ReadBdAddr_CB,      0x43484152, 0x2DAB0041, 0x69726F68, 0x61424C45)

//Device information service
SERVICE_016_M(	5,  0x2800,	0x180A)
CHARACT_016_M(0x81,  6,   gAttValueDIS.SoftRV,            (C_READ),          NULL,                                0x2A28)
CHARACT_016_M(0x81, 12,   gAttValueDIS.Manufacture,       (C_READ),          NULL,                                0x2A29)

#elif defined(SUPPORT_LE_XPORT)

/************[num]  [type]	[UUID:B1,B2,B3,B4] ************/
SERVICE_128_M( 	1,	0x2800,	0x49535343, 0xFE7D4AE5, 0x8FA99FAF, 0xD205E455)
/************[R/W]  [size]                  [MAXSIZE]               [symbol name]           [char property]             [need callback?]        [UUID] ************/
VL_CHAR_128_M(0x88,	LE_XPORT_DATA_LEN,      LE_XPORT_DATA_LEN,      gleXport.TxIdx,         (C_NOTIFY),                 NULL,                   0x49535343, 0x1E4D4BD9, 0xBA6123C6, 0x47249616)
DESCRIPTORS_M(0x11, 2,                                              gleXport.TX_CCCD,                                   NULL,                   0x2902)
VL_CHAR_128_M(0x10,	LE_XPORT_DATA_LEN,      LE_XPORT_DATA_LEN,      gleXport.RxIdx,         (C_WRITE_NO_RSP|C_WRITE),   LeXport_RxCB,           0x49535343, 0x884143F4, 0xA8D4ECBE, 0x34729BB3)

/*** LE_XPORT_UPDATE_CONNECTION ***/
CHARACT_128_M(0x11, LE_XPORT_CONNPARA_LEN,                          gleXport.connPara,      (C_READ|C_WRITE),           LeXport_connUpdateCB,   0x49535343, 0x6DAA4D02, 0xABF61956, 0x9ACA69FE)

/*** LE_XPORT_AIR_PATCH ***/
//CHARACT_128_M(0x10, LE_XPORT_PATCH_LEN,                             gleXport.airPatch,      (C_WRITE|C_NOTIFY),         LeXport_AirPatchCB,     0x49535343, 0xACA3481C, 0x91ECD85E, 0x28A60318)
CHARACT_128_M(0x11, LE_XPORT_PATCH_LEN,                             gleXport.airPatch,      (C_READ|C_WRITE|C_NOTIFY),  LeXport_AirPatchCB,     0x49535343, 0xACA3481C, 0x91ECD85E, 0x28A60318)
DESCRIPTORS_M(0x11, 2,                                              gleXport.airPatch_CCCD,                             NULL,                   0x2902)

/*** LE_XPORT_SPECIFIC_SERVICE ***/
#if 0 //LE_XPORT_SPECIFIC_SERVICE_ENABLE
#if 0 //LE_XPORT_SPECIFIC_SERVICE_128
SERVICE_128_M( 	2,	0x2800,	0x49535343, 0xFE7D4AE5, 0x8FA99FAF, 0xD205E466)
CHARACT_128_M(0x88, LE_XPORT_DATA_LEN,                              gleXport.specific_TX,   (C_NOTIFY),                 NULL,                   0x49535343, 0x1E4D4BD9, 0xBA6123C6, 0x47249622)
DESCRIPTORS_M(0x11, 2,                                              gleXport.specific_TX_CCCD,                          NULL,                   0x2902)
CHARACT_128_M(0x10, LE_XPORT_DATA_LEN,                              gleXport.specific_RX,   (C_WRITE_NO_RSP|C_WRITE),   LeXport_S_RxCB,         0x49535343, 0x884143F4, 0xA8D4ECBE, 0x34729B11)
#else //LE_XPORT_SPECIFIC_SERVICE_16
SERVICE_016_M(	2,  0x2800,	0x8888)
CHARACT_016_M(0x88, LE_XPORT_DATA_LEN,                              gleXport.specific_TX,   (C_NOTIFY),                 NULL,                   0x8889)
DESCRIPTORS_M(0x11, 2,                                              gleXport.specific_TX_CCCD,                          NULL,                   0x2902)
CHARACT_016_M(0x10, LE_XPORT_DATA_LEN,                              gleXport.specific_RX,	(C_WRITE_NO_RSP|C_WRITE),   LeXport_S_RxCB,         0x888A)
#endif
#endif

/*** Device information service ***/
/************[num]  [type]	[UUID] ************/
SERVICE_016_M(	3,  0x2800,	0x180A)
/************[R/W]  [size]                  [symbol name]           [char property]             [need callback?]            [UUID] ************/
CHARACT_016_M(0x81, LE_XPORT_MODEL_NUM_LEN, gleXport.modelNum,      (C_READ),                   NULL,                       0x2A24)
#if 0 //LE_XPORT_DIS_SERIAL_NUM_ENABLE
CHARACT_016_M(0x81, LE_XPORT_SERIAL_NUM_LEN,gleXport.serialNum,     (C_READ),                   NULL,                       0x2A25)
#endif
CHARACT_016_M(0x81, LE_XPORT_MANU_NAME_LEN, gleXport.manuName,      (C_READ),                   NULL,                       0x2A29)
CHARACT_016_M(0x81, LE_XPORT_SW_RV_LEN,     gleXport.softRV,        (C_READ),                   NULL,                       0x2A28)
CHARACT_016_M(0x81, LE_XPORT_SYSTEM_ID_LEN, gleXport.systemID,      (C_READ),                   NULL,                       0x2A23)

#if 0 //LE_XPORT_DIS_C_VERSION_ENABLE
CHARACT_016_M(0x81, LE_XPORT_VERSION_LEN,   gleXport.hardRV,        (C_READ),                   NULL,                       0x2A27)
CHARACT_016_M(0x81, LE_XPORT_VERSION_LEN,   gleXport.firmRV,        (C_READ),                   NULL,                       0x2A26)
#endif

#if 0 //LE_XPORT_DIS_SPECIFIC_ENABLE
CHARACT_016_M(0x81, LE_XPORT_S_DIS_LEN,     gleXport.s_DIS_1,       (C_READ),                   NULL,                       0x2A2A)
CHARACT_016_M(0x81, LE_XPORT_S_DIS_LEN,     gleXport.s_DIS_1,       (C_READ),                   NULL,                       0x2A50)
#endif



#else
//(Service)    [num][type][UUID]
//(Character)  [R/W][size][symbol name][char property][CallBack Function][UUID]
//(Descriptor) [R/W][size][symbol name][CallBack Function][UUID]

SERVICE_016_M(	1,  0x2800,	0x180F)

CHARACT_016_M(0x11,	1,    gBAS_Ctrl.gBAS_Value,	(C_READ|C_NOTIFY),		NULL,	0x2A19)
DESCRIPTORS_M(0x11, 2,    gBAS_CCCD,	   NULL,	0x2902)
CHARACT_128_M(0x11, 6,    gBAS_Ctrl.BD_ADDR,    (C_READ),       APP_ReadBdAddr_CB,            0x43484152, 0x2DAB0041, 0x69726F68, 0x61424C45)

#ifdef MMI_LE_APP_ENABLE
SERVICE_128_M(	2,  0x2800,	0x5052494D, 0x2DAB0141, 0x69726F68, 0x61424C45) 


CHARACT_128_M(0x11, 6,      AppCtl.Status,               (C_READ|C_NOTIFY),      Status_CB,                 0x43484152, 0x2DAB1141, 0x69726F68, 0x61424C45)
DESCRIPTORS_M(0x11, 2,      AppCtl.CCCD_Bat,	         NULL,	0x2902)

CHARACT_128_M(0x11, 6,      AppCtl.BD_ADDR,              (C_READ|C_WRITE),       Read_BDAddr_CB,            0x43484152, 0x2DAB1241, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 2,      AppCtl.FW_Version,           (C_READ),               Read_FW_Version_CB,        0x43484152, 0x2DAB1341, 0x69726F68, 0x61424C45)
VL_CHAR_128_M(0x11,	7, 20,  AppCtl.DeviceName,           (C_READ|C_WRITE),       RW_DeviceName_CB,          0x43484152, 0x2DAB1441, 0x69726F68, 0x61424C45)

CHARACT_128_M(0x11, 1,      AppCtl.Reset_PDL,            (C_WRITE_NO_RSP),       ResetPDL_CB,               0x43484152, 0x2DAB1541, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1,      AppCtl.Vol_Ctl,              (C_WRITE_NO_RSP),       Vol_Ctl_CB,                0x43484152, 0x2DAB1641, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1,      AppCtl.PEQ_Mode_Switch,      (C_WRITE_NO_RSP),       PEQ_Mode_Switch_CB,        0x43484152, 0x2DAB1741, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1,      AppCtl.VoicePromptSwitch,    (C_WRITE_NO_RSP),       VoicePromptSwitch_CB,      0x43484152, 0x2DAB1841, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1,      AppCtl.VP_Language_Switch,   (C_WRITE_NO_RSP),       VP_Language_Switch_CB,     0x43484152, 0x2DAB1941, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1,      AppCtl.FindMyAccesory,       (C_WRITE_NO_RSP),       FindMyAccesory_CB,         0x43484152, 0x2DAB1A41, 0x69726F68, 0x61424C45)
#ifdef LIGHTING_ENABLE
CHARACT_128_M(0x11, 1,      AppCtl.OnOff_MusicYouCanSee, (C_WRITE_NO_RSP),       OnOffMusicYouCanSee_CB,	0x43484152, 0x2DAB1B41, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 3,      AppCtl.RGB, 			     (C_WRITE_NO_RSP),       RGB_CB,      				0x43484152, 0x2DAB1C41, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1,      AppCtl.LightingMode,         (C_WRITE_NO_RSP),       LightingMode_CB,       	0x43484152, 0x2DAB1D41, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1,      AppCtl.Alarm_Clock,       	 (C_WRITE_NO_RSP),       AlarmClock_CB,       		0x43484152, 0x2DAB1E41, 0x69726F68, 0x61424C45)
#endif
SERVICE_128_M(	3,  0x2800,	0x5052494D, 0x2DAB0241, 0x69726F68, 0x61424C45) 
CHARACT_128_M(0x11, 1, AppCtl.VoicePromptAdj,           (C_READ|C_WRITE),       NULL,       0x43484152, 0x2DAB2141, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1, AppCtl.CallerNameAnnouncement,   (C_READ|C_WRITE),       NULL,       0x43484152, 0x2DAB2241, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1, AppCtl.RemoteUpdateFW_Cmd,       (C_READ|C_WRITE),       NULL,       0x43484152, 0x2DAB2341, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 1, AppCtl.RemoteUpdateFW_Data,      (C_READ|C_WRITE),       NULL,       0x43484152, 0x2DAB2441, 0x69726F68, 0x61424C45)
CHARACT_128_M(0x11, 20,	AppCtl.phoneNumber, 			(C_NOTIFY), 			NULL,		0x43484152, 0x2DAB2541, 0x69726F68, 0x61424C45)
DESCRIPTORS_M(0x11, 2,	AppCtl.phnoeNumber_CCCD,		NULL,					0x2902)

#ifdef BLE_APP
/************[num]  [type]	[UUID:B1,B2,B3,B4] ************/
SERVICE_128_M( 	4,	0x2800,	0x5052494D, 0x2DAB0341, 0x69726F68, 0x61424C45)
/************[R/W]  [size]  [MAXSIZE]                   [symbol name]           [char property]             [need callback?]        [UUID] ************/
VL_CHAR_128_M(0x88,	20,      BLE_MAX_TX_DATA_LEN,       AppCtl.TxIdx,           (C_NOTIFY),                 NULL,                   0x43484152, 0x2DAB3141, 0x69726F68, 0x61424C45)
VL_DESCRIPT_M(0x11, 2,       2,                         AppCtl.TX_CCCD,          NULL,                      0x2902)
VL_CHAR_128_M(0x10,	20,      BLE_MAX_RX_DATA_LEN,       AppCtl.Rx,              (C_WRITE_NO_RSP|C_WRITE),   Rx_CB,                  0x43484152, 0x2DAB3241, 0x69726F68, 0x61424C45)
#endif

#ifdef ANCS_CELL_PHONE_APP
/************[num]  [type]	[UUID:B1,B2,B3,B4] ************/
SERVICE_128_M( 	5,	0x2800,	0x5052494D, 0x2DAB0441, 0x69726F68, 0x61424C45)
/************[R/W]  [size]  [MAXSIZE]                           [symbol name]                       [char property]             [need callback?]        [UUID] ************/
VL_CHAR_128_M(0x88,	20,      ANCS_MAX_TX_DATA_LEN,               gApp_ANCS_ctl.srcNotifTxIdx,        (C_NOTIFY),                 NULL,                   0x43484152, 0x2DAB4141, 0x69726F68, 0x61424C45)
DESCRIPTORS_M(0x11, 2,       gApp_ANCS_ctl.srcNotifTxCCCD,      NULL,                               0x2902)
VL_CHAR_128_M(0x10,	20,      ANCS_MAX_RX_DATA_LEN,              gApp_ANCS_ctl.controlPointRx,       (C_WRITE_NO_RSP|C_WRITE),   APP_ANCS_RxCB,           0x43484152, 0x2DAB4241, 0x69726F68, 0x61424C45)
VL_CHAR_128_M(0x88,	20,      ANCS_MAX_TX_DATA_LEN,              gApp_ANCS_ctl.dataNotifTxIdx,       (C_NOTIFY),                 NULL,                    0x43484152, 0x2DAB4341, 0x69726F68, 0x61424C45)
DESCRIPTORS_M(0x11, 2,       gApp_ANCS_ctl.dataNotifTxCCCD,     NULL,                               0x2902)
#endif	//#ifdef ANCS_CELL_PHONE_APP

#endif


/*//
SERVICE_016_M(	8,  0x2800,	0xA003) 
CHARACT_016_M(0x11, 1, gAttValue.UserDefCharacteristic_8,        (C_READ),          NULL,            0xCC04)

//Read FW version
SERVICE_016_M(	9,  0x2800,	0xA004) 
CHARACT_016_M(0x11, 1, gAttValue.UserDefCharacteristic_9,        (C_READ),          NULL,            0xCC05)

//Read Device ID
SERVICE_016_M(	10,  0x2800,	0xA005) 
CHARACT_016_M(0x11, 6, gAttValue.UserDefCharacteristic_10,        (C_READ),          NULL,            0xCC06)

//Modify Device Name
SERVICE_016_M(	11,  0x2800,	0xA006)
CHARACT_016_M(0x11,  30, gAttValue.DeviceName,    (C_WRITE_NO_RSP|C_READ),                   NULL,                   0xCC07)
//DESCRIPTORS_M(0x11,  2, gAttValue.UserDef_CCCD_4,                                    NULL,	                0x2902)

//Reset PDL
SERVICE_016_M(	12,  0x2800,	0xA007)
CHARACT_016_M(0x11,  1, gAttValue.UserDefCharacteristic_11,    (C_WRITE_NO_RSP),                   &Reset_PDL,                   0xCC08)
//Volume Control
SERVICE_016_M(	13,  0x2800,	0xA008)
CHARACT_016_M(0x11,  1, gAttValue.UserDefCharacteristic_12,    (C_WRITE_NO_RSP|C_READ),            &Volume_Control,                   0xCC09)
//PEQ Mode Switch
SERVICE_016_M(	14,  0x2800,	0xA009)
CHARACT_016_M(0x11,  1, gAttValue.UserDefCharacteristic_13,    (C_WRITE_NO_RSP|C_READ),            &Mode_Switch,                   0xCC0A)
//Voice Prompt Switch
SERVICE_016_M(	15,  0x2800,	0xA00A)
CHARACT_016_M(0x11,  1, gAttValue.UserDefCharacteristic_14,    (C_WRITE_NO_RSP|C_READ),            &Voice_Prompt_Switch,                   0xCC0B)
//VP Language Switch
SERVICE_016_M(	16,  0x2800,	0xA00B)
CHARACT_016_M(0x11,  1, gAttValue.UserDefCharacteristic_15,    (C_WRITE_NO_RSP|C_READ),            &VP_Language_Switch,                   0xCC0C)
//Find My Accessory
//SERVICE_016_M(	17,  0x2800,	0xA00C)
//CHARACT_016_M(0x11,  1, gAttValue.UserDefCharacteristic_16,    (C_WRITE_NO_RSP|C_READ),                   NULL,                   0xCC0D)

*/



/*
VL_CHAR_016_M(0x11,	1,   3,	  gBAS_Ctrl.gTestVL1,	(C_READ|C_WRITE|C_NOTIFY),		NULL,	0x2A19)
VL_DESCRIPT_M(0x11, 2,   5,   gBAS_Ctrl.gTestVL2,	   NULL,	0x2902)
VL_DESCRIPT_M(0x11, 2,   10,  gBAS_CCCD,	  		   NULL,	0x2901)
*/

//SERVICE_128_M( 	2,	0x2801,	0x00112233,0x44556677,0x8899AABB,0xCCDDEEFF)
//INCLUDE_016_M(  1)


#endif
#endif /* LE_SUPPORTED_HOST_FLASH */

