#ifndef _AUDIO_MAILBOX_H_
#define _AUDIO_MAILBOX_H_

/******************************************************************************
 * FILE NAME :      bsp_mailbox.h
 *
 * DESIGNER :       Gailong
 *
 * COPYRIGHT :      Copyright (c) Airoha Tek. Cop.
 *
 * DESCRIPTION :    Driver for DSP mailbox control
 *
 * REVISION HISTORY :
 *  MAINTAINER  DATE        DESCRIPTION
 *  Gailong     20130708    Creation
 *
 ******************************************************************************/
#include "os.h"
#include "rc.h"
#include "DRIVER_interface.h"
#include "Audio_MailBox_structure.h"
#include "align_flash.h"
#include "hci_airoha.h"
#define MAX_PARAM_NO	6

#define WORDINBYTE	1
#define SUBSTATE_INIT 0xff
#define VP_TO_ESCO_RELAY 0x56
/******************************************************************************
 * Type definition
 ******************************************************************************/
typedef struct
{
	U8 dataOffset; //to the start of meaningful message
	U8 reserveByte[RESERVE_BYTE_NO];
	U8 msgOpcode;
	U8 eventCode;
	U8 paraLen;     //should sync to

	M2D_CTL_STRU m2d_ctl;
	CMD_RESPONSE_STRU cmd_resp;
  //VFUN callBackFunc;
}DRIVERMSG_WITH_MAILBOXCMD_STRU;

typedef struct
{
	U8 dataOffset; //to the start of meaningful message
	U8 reserveByte[RESERVE_BYTE_NO];
	U8  msgOpcode;
	U8 eventCode;
	U8 paraLen;     //should sync to
	U8 ogf;
	U8 ocf;
    U8 param;
}DRIVERMSG_STRU;
typedef struct
{
	U8 ocf;
	U8 state;
	DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CmdPtr;
}MAILBOX_STATE_STRU;



/******************************************************************************
 * Enumeration
 ******************************************************************************/
enum mb_task_enum
{
    TASK_NULL       = 0x0000,
    TASK_VPRTAT     = 0x0100,
    TASK_APP        = 0x0200,
    TASK_DSP        = 0x0300,
#ifdef SUPPORT_VOICE_COMMAND
    TASK_VC         = 0x0400,
    MAX_TASK_NUM    = 0x0500,
#else
    MAX_TASK_NUM    = 0x0400,
#endif
};

enum mb_code_enum                 //mailbox code enum
{
	OGF_VP = 0,
	OGF_RT,
	OGF_VC,
	OGF_APP,
	OGF_DSP,
	OGF_AT,
	OGF_MAX_NUM,
};

enum gain_target_enum
{
	TARGET_NONE,
	TARGET_APP    = 1<<1,
	TARGET_VPRTAT = 1<<2,
};

enum dsp_ocf_cmd_enum               //DSP OCF command enum
{
    OCF_DSP_AUDIO_ON = 0,
    OCF_DSP_AUDIO_OFF,
    OCF_DSP_SET_VOL,
    OCF_DSP_UPDATE_PARA,
    OCF_DSP_MAX_NUM
};

enum dsp_dbb_para_enum
{
	DSP_DBB_NO_UPDATE			= 0,
	DSP_DBB_OFF					= 1,
	DSP_DBB_ON					= 2,
};


enum dsp_para_update_enum
{
    DSP_NO_UPDATE           = 0,
    DSP_UPDATE_PEQ          = 1,
    DSP_UPDATE_NB_TX_CPD    = 2,
    DSP_UPDATE_WB_TX_CPD    = 3,
    DSP_UPDATE_NB_RX_CPD    = 4,
    DSP_UPDATE_WB_RX_CPD    = 5,
    DSP_UPDATE_VP_CPD       = 6,
    DSP_UPDATE_A2DP_CPD     = 7,
    DSP_UPDATE_LINEIN_CPD   = 8,
    DSP_UPDATE_INS          = 9,
    DSP_UPDATE_DBB          = 10,
    DSP_UPDATE_VC           = 11,
    DSP_UPDATE_WID          = 12,
    DSP_UPDATE_NR_EQ        = 13,
    DSP_UPDATE_EC_NR        = 14,
    DSP_UPDATE_MAX_ENTRY    = 15,
    DSP_FEEDBACK_REF_GAIN   = 16,
};

enum dsp_soundeffect_para_enum 
{
	DSP_SOUNDEFFECT_NO_UPDATE = 0,
	DSP_SOUNDEFFECT_NONE ,
	DSP_SOUNDEFFECT_WIDENING ,
	DSP_SOUNDEFFECT_ROOM ,
	DSP_SOUNDEFFECT_BATHROOM ,
	DSP_SOUNDEFFECT_AUDITORIUM ,
	DSP_SOUNDEFFECT_TOTAL_NO,
};

enum vp_ocf_cmd_enum               //VP task OCF command enum
{
	OCF_VP_START = 0,
	OCF_VP_STOP,
	OCF_VP_INIT,
	OCF_VP_MAX_NUM
};

enum rt_ocf_cmd_enum               //RT task OCF command enum
{
	OCF_RT_START = 0,
	OCF_RT_STOP,
	OCF_RT_MAX_NUM
};
enum vc_ocf_cmd_enum               //VC task OCF command enum
{
	OCF_VC_START = 0,
	OCF_VC_STOP,
	OCF_VC_MAX_NUM
};

enum at_ocf_cmd_enum               //AT task OCF command enum
{
	OCF_AT_START = 0,
	OCF_AT_STOP,
	OCF_AT_MAX_NUM
};

enum app_ocf_cmd_enum               //APP task OCF command enum
{
	OCF_APP_IDLE_MODE = 0,
	OCF_APP_MONO_MODE ,
	OCF_APP_LIGHT_MONO_MODE,
	OCF_APP_STEREO_MODE,
	OCF_APP_RESERVED_MODE,
	OCF_APP_LINEIN_MODE,
	OCF_APP_ASYNC_MONO_MODE,
	OCF_APP_MAX_NUM
};

enum evt_enum                  //mailbox event state enum
{
	CMD_REPLY_EVT = 0x04,
	VP_END_EVT,
	RT_END_EVT,
	VC_END_EVT

};
enum cmdreply_sta
{
	CMD_FAIL	= 0,
	CMD_COMPL
};
enum mailbox_id_enum               //mailbox owner bit to switch MCU & DSP ownership
{
	MCU_Owned = 0,
	DSP_Owned = 0x0100,
};
enum vp_pattern_header
{
	VPCONT	  = 0x0000,
	VPINIT	  = 0x0100
};
enum ONE_STEP_STATE_enum
{
	SUBSTATE_COMPLETING = 0
};

enum CMD_RESPONSE_STATUS_enum
{
	CMD_RESP_RESET = 0,
	CMD_RESP_COMPLETING,
	CMD_RESP_END
};

enum MONO_SCO_TYPE_enum
{
	SCO	=0,
	eSCO
};

enum MONO_APPLICATION_MODE_enum
{
	MONO_NORMAL_MODE = 0,
    MONO_WOOFER_MODE,
};

enum STEREO_APPLICATION_MODE_enum
{
	STEREO_NORMAL_MODE = 0,
    STEREO_AIROWOOFER_MODE,
    STEREO_ACLWOOFER_MODE,
    STEREO_SOUNDBAR_MODE,
};

enum LINEIN_APPLICATION_MODE_enum
{
	LINEIN_NORMAL_MODE = 0,
    LINEIN_SOUNDBAR_MODE,
    LINEIN_AIROSTEREO_MODE,
    LINEIN_DUPLEX_MODE,
};

enum SONY_SOUNDBAR_INPUT_FS_enum
{
	SONY_SB_IN_48K = 0,
	SONY_SB_IN_44_1K,
	SONY_SB_IN_32K,
};

typedef struct
{
	U8 end_status;
}MAILBOX_PROMPT_STRU;
typedef void (*MAILBOXFUN)(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR)  compact REENTRANT;


typedef struct
{
	U8 waitingDSPRingToneVPAT:1;
	U8 waitingDSPVoiceMusicAUX:1;
	U8 waitingDSPVolumeSet:1;
	U8 setVolumeAgain:1;
	U8 waitingDSPVC:1; 
	U8 reserved:3;
}MAILBOX_CTL_TYPE;

/******************************************************************************
 * External Symbol
 ******************************************************************************/
extern MAILBOX_STATE_STRU XDATA MailBox_State[];
extern MAILBOX_CTL_TYPE XDATA gMailBox_ctl;

/******************************************************************************
 * External Function Prototypes
 ******************************************************************************/

extern VOID MailBox_EvtHandler(VOID);
//extern void MailBox_Check(void);
extern VOID MailBox_Init(VOID);
extern VOID MailBox_CmdHandler(DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR CMD_PTR);

#define DRIVER_SendCmdToMailBox(A)	MailBox_CmdHandler((DRIVERMSG_WITH_MAILBOXCMD_STRU XDATA_PTR)(A))
#define IS_MAILBOX_TASK_VPRT_BUSY	(gMailBox_ctl.waitingDSPRingToneVPAT || gAlignDSP.Mailbox.m2d_ctl.rw_word == DSP_Owned)
#define IS_DSP_BUSY					(gMailBox_ctl.waitingDSPVoiceMusicAUX || gMailBox_ctl.waitingDSPVolumeSet || gMailBox_ctl.waitingDSPRingToneVPAT || gMailBox_ctl.waitingDSPVC || gAlignDSP.Mailbox.m2d_ctl.rw_word == DSP_Owned)
PUBLIC void Mailbox_TaskOgfVPRTAT(DriverMsg XDATA_PTR msgPtr, U8 ogf, U8 ocf);
PUBLIC void Mailbox_TaskOgfAPP(DriverMsg XDATA_PTR msgPtr, U8 ocf);
PUBLIC void Mailbox_TaskOgfDSP(DriverMsg XDATA_PTR msgPtr, U8 ocf);
#ifdef SUPPORT_VOICE_COMMAND
PUBLIC void Mailbox_TaskOgfVC(DriverMsg XDATA_PTR msgPtr, U8 ogf, U8 ocf);
#endif

PUBLIC void Mailbox_ReleaseTASKVPRTATBusyFlag(void);
PUBLIC void Mailbox_ReleaseTASKAppBusyFlag(void);
PUBLIC void Mailbox_ReleaseTASKDSPBusyFlag(void);
PUBLIC void Mailbox_ReleaseTASKVCBusyFlag(void);
#endif

