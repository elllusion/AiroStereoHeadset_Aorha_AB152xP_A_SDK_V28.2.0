#ifndef _LM_CONNECTIONHANDLE_H_
#define _LM_CONNECTIONHANDLE_H_

#include "os.h"
#include "LM_interface.h"
#include "..\HC\HC_connectionhandle.h"

#if 0
typedef struct stru_nlm_link_info
{
    CONNHDL_CTL_STRU XDATA_PTR ConnHdlPtr;
    U8 LinkIdx;
    LM_STABLE_STATE lm_state;
    LM_TESTMODE_PARAM testmode_ctl;
    LM_AFH_PARAM afh_ctl;
    LM_ACLU_PARAM aclu_ctl;
    U16 Tsniff;
    U32 sniffInstant;
    U8 last_error_code;
    U8 errToLocal;
    U8 errToRemote;
    U8 testmode_type;
    REPEATED_ATTEMPT_INFO_TYPE repeat_attempt_info_type;
    LM_ENCRYPTION_HANDLE_INFO encryption_info;
    U8 version;
    U8 lc_addressed;
    LmPktType pktType;
    LmProcCtrlType procCtrlInfo;
    LmProcCtrlType2 procCtrlInfo2;
    LmRSInfoType rsInfo;
    QosSetupInfoType qosSetupInfo;
    PowerControlInfoType powerCtrlInfo;
    SniffSubratingInfoType subratingInfo;
    //When adding timer pointer, we should check the timer memory is released in LM_ClearLinkInfo.
    //When using OST_PutTimer, we should set the timer pointer to NULL after the timer memory is released.
    OST XDATA_PTR waitTimer;
    U8 wait_state_opcode;

    /* Expired Times */
    BTCLK_UNION AudioCtlExpiredTime;
    BTCLK_UNION TestModeTransactionTimeoutExpiredTime;
    BTCLK_UNION DetachTimeoutExpiredTime;
    BTCLK_UNION AuthWaitingIntervalExpiredTime;
    BTCLK_UNION ConnectionSetupExpiredTime;
    BTCLK_UNION PollChk4SlaveExpiredTime;
    BTCLK_UNION SupervisionExpiredTime;

    /* Timeout Values */
    BTCLK_UNION SupervisionTimeout;

    //poll checking timer
    U8 searchExtLevel;

    //for qualification - hci event & lmp timing issue
    #ifdef LC_TEST_CASE_CONFIG
    OST XDATA_PTR hciEvt2SendTimer;
    #endif

    U8 last_top_state;
    U8 last_sub_state;
    U32 currentQosLatency;

    #ifdef ACT_FUNCTION
    U8 sending_disconnected_command;
    #endif

    #ifdef SUPPORT_SET_WAITED_COMMAND
    //Added by Eric Chou, 09.5.15
    U8 waitedCmds[MAX_WAITED_CMDS];
    #endif

    U8 not_anytimePDU;                          //To guard au_rand any-time or not
    U8 active_llSit;                            //True: runSitPtr = llSit2; False: runSitPtr = llSit
    U8 llSit_act_sniff_proc;                    //True: There is an active sniff process in llSit;

} NLM_CONNHDL_INFO_STRU;

extern NLM_CONNHDL_INFO_STRU XDATA gNLM_link_info[MAX_NUM_ACL_LINK]; //2: MAX_NUM_ACL_LINK
#endif
#endif