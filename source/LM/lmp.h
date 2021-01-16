#ifndef _LMP_H_
#define _LMP_H_
/* 
 * AIROHA Bluetooth Device Firmware
 * COPYRIGHT (C) 2014 AIROHA TECHNOLOGY CORP. CO., LTD. ALL RIGHTS RESERVED
 *
 * HCI.h is the file defines the constants and command/event format
 * 
 * Programmer : CharlesSu@airoha.com.tw, Ext.2882
 */
#include "os.h"



////////////////////////////////////////////////////////////////////////////////
// Bluetooth LMP OpCode Definitions ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
 * LMP Op Code
 */
typedef U8 LMP_OPCODE;
#define LMP_OPCODE_NAME_REQ                         (LMP_OPCODE)1
#define LMP_OPCODE_NAME_RES                         (LMP_OPCODE)2
#define LMP_OPCODE_ACCEPTED                         (LMP_OPCODE)3
#define LMP_OPCODE_NOT_ACCEPTED                     (LMP_OPCODE)4
#define LMP_OPCODE_CLKOFFSET_REQ                    (LMP_OPCODE)5
#define LMP_OPCODE_CLKOFFSET_RES                    (LMP_OPCODE)6
#define LMP_OPCODE_DETACH                           (LMP_OPCODE)7
#define LMP_OPCODE_IN_RAND                          (LMP_OPCODE)8
#define LMP_OPCODE_COMB_KEY                         (LMP_OPCODE)9
#define LMP_OPCODE_UNIT_KEY                         (LMP_OPCODE)10
#define LMP_OPCODE_AU_RAND                          (LMP_OPCODE)11
#define LMP_OPCODE_SRES                             (LMP_OPCODE)12
#define LMP_OPCODE_TEMP_RAND                        (LMP_OPCODE)13
#define LMP_OPCODE_TEMP_KEY                         (LMP_OPCODE)14
#define LMP_OPCODE_ENCRYPTION_MODE_REQ              (LMP_OPCODE)15
#define LMP_OPCODE_ENCRYPTION_KEY_SIZE_REQ          (LMP_OPCODE)16
#define LMP_OPCODE_START_ENCRYPTION_REQ             (LMP_OPCODE)17
#define LMP_OPCODE_STOP_ENCRYPTION_REQ              (LMP_OPCODE)18
#define LMP_OPCODE_SWITCH_REQ                       (LMP_OPCODE)19
#define LMP_OPCODE_HOLD                             (LMP_OPCODE)20
#define LMP_OPCODE_HOLD_REQ                         (LMP_OPCODE)21
#define LMP_OPCODE_SNIFF_REQ                        (LMP_OPCODE)23
#define LMP_OPCODE_UNSNIFF_REQ                      (LMP_OPCODE)24
#define LMP_OPCODE_PARK_REQ                         (LMP_OPCODE)25
#define LMP_OPCODE_SET_BROADCAST_SCAN_WINDOW        (LMP_OPCODE)27
#define LMP_OPCODE_MODIFY_BEACON                    (LMP_OPCODE)28
#define LMP_OPCODE_UNPARK_BD_ADDR_REQ               (LMP_OPCODE)29
#define LMP_OPCODE_UNPARK_PM_ADDR_REQ               (LMP_OPCODE)30
#define LMP_OPCODE_INCR_POWER_REQ                   (LMP_OPCODE)31
#define LMP_OPCODE_DECR_POWER_REQ                   (LMP_OPCODE)32
#define LMP_OPCODE_MAX_POWER                        (LMP_OPCODE)33
#define LMP_OPCODE_MIN_POWER                        (LMP_OPCODE)34
#define LMP_OPCODE_AUTO_RATE                        (LMP_OPCODE)35
#define LMP_OPCODE_PREFERRED_RATE                   (LMP_OPCODE)36
#define LMP_OPCODE_VERSION_REQ                      (LMP_OPCODE)37
#define LMP_OPCODE_VERSION_RES                      (LMP_OPCODE)38
#define LMP_OPCODE_FEATURES_REQ                     (LMP_OPCODE)39
#define LMP_OPCODE_FEATURES_RES                     (LMP_OPCODE)40
#define LMP_OPCODE_QUALITY_OF_SERVICE               (LMP_OPCODE)41
#define LMP_OPCODE_QUALITY_OF_SERVICE_REQ           (LMP_OPCODE)42
#define LMP_OPCODE_SCO_LINK_REQ                     (LMP_OPCODE)43
#define LMP_OPCODE_REMOVE_SCO_LINK_REQ              (LMP_OPCODE)44
#define LMP_OPCODE_MAX_SLOT                         (LMP_OPCODE)45
#define LMP_OPCODE_MAX_SLOT_REQ                     (LMP_OPCODE)46
#define LMP_OPCODE_TIMING_ACCURACY_REQ              (LMP_OPCODE)47
#define LMP_OPCODE_TIMING_ACCURACY_RES              (LMP_OPCODE)48
#define LMP_OPCODE_SETUP_COMPLETE                   (LMP_OPCODE)49
#define LMP_OPCODE_USE_SEMI_PERMANENT_KEY           (LMP_OPCODE)50
#define LMP_OPCODE_HOST_CONNECTION_REQ              (LMP_OPCODE)51
#define LMP_OPCODE_SLOT_OFFSET                      (LMP_OPCODE)52
#define LMP_OPCODE_PAGE_MODE_REQ                    (LMP_OPCODE)53
#define LMP_OPCODE_PAGE_SCAN_MODE_REQ               (LMP_OPCODE)54
#define LMP_OPCODE_SUPERVISION_TIMEOUT              (LMP_OPCODE)55
#define LMP_OPCODE_TEST_ACTIVATE                    (LMP_OPCODE)56
#define LMP_OPCODE_TEST_CONTROL                     (LMP_OPCODE)57
#define LMP_OPCODE_ENCRYPTION_KEY_SIZE_MASK_REQ     (LMP_OPCODE)58
#define LMP_OPCODE_ENCRYPTION_KEY_SIZE_MASK_RES     (LMP_OPCODE)59
#define LMP_OPCODE_SET_AFH                          (LMP_OPCODE)60
#define LMP_OPCODE_ENCAPSULATED_HEADER              (LMP_OPCODE)61
#define LMP_OPCODE_ENCAPSULATED_PAYLOAD             (LMP_OPCODE)62
#define LMP_OPCODE_SIMPLE_PAIRING_CONFIRM           (LMP_OPCODE)63
#define LMP_OPCODE_SIMPLE_PAIRING_NUMBER            (LMP_OPCODE)64
#define LMP_OPCODE_DHKEY_CHECK                      (LMP_OPCODE)65
#define LMP_OPCODE_ESCAPE1                          (LMP_OPCODE)124
#define LMP_OPCODE_ESCAPE2                          (LMP_OPCODE)125
#define LMP_OPCODE_ESCAPE3                          (LMP_OPCODE)126
#define LMP_OPCODE_ESCAPE4                          (LMP_OPCODE)127

/* Escape 4 */
typedef U8 LMP_EXT_OPCODE;
#define LMP_EXT_OPCODE_ACCEPTED_EXT                 (LMP_EXT_OPCODE)1
#define LMP_EXT_OPCODE_NOT_ACCEPTED_EXT             (LMP_EXT_OPCODE)2
#define LMP_EXT_OPCODE_FEATURES_REQ_EXT             (LMP_EXT_OPCODE)3
#define LMP_EXT_OPCODE_FEATURES_RES_EXT             (LMP_EXT_OPCODE)4
#define LMP_EXT_OPCODE_PACKET_TYPE_TABLE_REQ        (LMP_EXT_OPCODE)11
#define LMP_EXT_OPCODE_ESCO_LINK_REQ                (LMP_EXT_OPCODE)12
#define LMP_EXT_OPCODE_REMOVE_ESCO_LINK_REQ         (LMP_EXT_OPCODE)13
#define LMP_EXT_OPCODE_CHANNEL_CLASSIFICATION_REQ   (LMP_EXT_OPCODE)16
#define LMP_EXT_OPCODE_CHANNEL_CLASSIFICATION       (LMP_EXT_OPCODE)17
#define LMP_EXT_OPCODE_SNIFF_SUBRATING_REQ          (LMP_EXT_OPCODE)21
#define LMP_EXT_OPCODE_SNIFF_SUBRATING_RES          (LMP_EXT_OPCODE)22
#define LMP_EXT_OPCODE_PAUSE_ENCRYPTION_REQ         (LMP_EXT_OPCODE)23
#define LMP_EXT_OPCODE_RESUME_ENCRYPTION_REQ        (LMP_EXT_OPCODE)24
#define LMP_EXT_OPCODE_IO_CAPABILITY_REQ            (LMP_EXT_OPCODE)25
#define LMP_EXT_OPCODE_IO_CAPABILITY_RES            (LMP_EXT_OPCODE)26
#define LMP_EXT_OPCODE_NUMERIC_COMPARISON_FAILED    (LMP_EXT_OPCODE)27
#define LMP_EXT_OPCODE_PASSKEY_FAILED               (LMP_EXT_OPCODE)28
#define LMP_EXT_OPCODE_OOB_FAILED                   (LMP_EXT_OPCODE)29
#define LMP_EXT_OPCODE_KEYPRESS_NOTIFICATION        (LMP_EXT_OPCODE)30

typedef struct stru_lmp_tid_opcode
{
    U8 Tid      : 1;
    U8 OpCode   : 7;

} LMP_TID_OPCODE_STRU;

typedef union union_lmp_tid_opcode
{
    U8 value;
    LMP_TID_OPCODE_STRU field;

} LMP_TID_OPCODE_UNION;

typedef struct stru_lmp
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 Payload[1];

} LMP_STRU;

typedef struct stru_lmp_ext
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 ExtendedOpCode;
    U8 Payload[1];

} LMP_EXT_STRU;

/*
 * 1 LMP_NAME_REQ
 */
typedef struct stru_lmp_name_req
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 NameOffset;

} LMP_NAME_REQ_STRU;

/*
 * 2 LMP_NAME_RES
 */
typedef struct stru_lmp_name_res
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 NameOffset;
    U8 NameLength;
    U8 NameFragment[14];

} LMP_NAME_RES_STRU;

/*
 * 3 LMP_ACCEPTED
 */
typedef struct stru_lmp_accepted
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 OpCode;

} LMP_ACCEPTED_STRU;

/*
 * 4 LMP_NOT_ACCEPTED
 */
typedef struct stru_lmp_not_accepted
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 OpCode;
    U8 ErrCode;

} LMP_NOT_ACCEPTED_STRU;

/*
 * 6 LMP_CLKOFFSET_RES
 */
typedef struct stru_lmp_clkoffset_res
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 ClockOffsetB0;
    U8 ClockOffsetB1;

} LMP_CLKOFFSET_RES_STRU;

/*
 * 31 LMP_INCR_POWER_REQ
 */
typedef struct stru_lmp_incr_power_req
{
    LMP_TID_OPCODE_UNION TidOpcode;

} LMP_INCR_POWER_REQ_STRU;

/*
 * 32 LMP_DECR_POWER_REQ
 */
typedef struct stru_lmp_decr_power_req
{
    LMP_TID_OPCODE_UNION TidOpcode;

} LMP_DECR_POWER_REQ_STRU;

/*
 * 33 LMP_MAX_POWER
 */
typedef struct stru_lmp_max_power
{
    LMP_TID_OPCODE_UNION TidOpcode;

} LMP_MAX_POWER_STRU;

/*
 * 34 LMP_MIN_POWER
 */
typedef struct stru_lmp_min_power
{
    LMP_TID_OPCODE_UNION TidOpcode;

} LMP_MIN_POWER_STRU;

/*
 * 37 LMP_VERSION_REQ
 */
typedef struct stru_lmp_version_req
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 VersNr;
    U8 CompIdB0;
    U8 CompIdB1;
    U8 SubVersNrB0;
    U8 SubVersNrB1;

} LMP_VERSION_REQ_STRU;

/*
 * 38 LMP_VERSION_RES
 */
typedef struct stru_lmp_version_res
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 VersNr;
    U8 CompIdB0;
    U8 CompIdB1;
    U8 SubVersNrB0;
    U8 SubVersNrB1;

} LMP_VERSION_RES_STRU;

/*
 * 39 LMP_FEATURES_REQ
 */
typedef struct stru_lmp_features_req
{
    LMP_TID_OPCODE_UNION TidOpcode;
    LMP_FEATURES_ARR Features;

} LMP_FEATURES_REQ_STRU;

/*
 * 40 LMP_FEATURES_RES
 */
typedef struct stru_lmp_features_res
{
    LMP_TID_OPCODE_UNION TidOpcode;
    LMP_FEATURES_ARR Features;

} LMP_FEATURES_RES_STRU;

/*
 * 45 LMP_MAX_SLOT
 */
typedef struct stru_lmp_max_slot
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 MaxSlot;

} LMP_MAX_SLOT_STRU;

/*
 * 46 LMP_MAX_SLOT_REQ
 */
typedef struct stru_lmp_max_slot_req
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 MaxSlot;

} LMP_MAX_SLOT_REQ_STRU;

/*
 * 47 LMP_TIMING_ACCURACY_REQ
 */
typedef struct stru_lmp_timing_accuracy_req
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 Drift;
    U8 Jitter;

} LMP_TIMING_ACCURACY_REQ_STRU;

/*
 * 48 LMP_TIMING_ACCURACY_RES
 */
typedef struct stru_lmp_timing_accuracy_res
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 Drift;
    U8 Jitter;

} LMP_TIMING_ACCURACY_RES_STRU;

/*
 * 52 LMP_SLOT_OFFSET
 */
typedef struct stru_lmp_slot_offset
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 SlotOffsetB0;
    U8 SlotOffsetB1;
    BD_ADDR_STRU BdAddr;

} LMP_SLOT_OFFSET_STRU;

/*
 * 55 LMP_SUPERVISION_TIMEOUT
 */
typedef struct stru_lmp_supervision_timeout
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 SupervisionTimeoutB0;
    U8 SupervisionTimeoutB1;

} LMP_SUPERVISION_TIMEOUT_STRU;

/*
 * 127/03 LMP_FEATURES_REQ_EXT
 */
typedef struct stru_lmp_features_req_ext
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 ExtOpCode;
    U8 FeaturesPage;
    U8 MaxSupportedPage;
    LMP_FEATURES_ARR ExtendedFeatures;

} LMP_FEATURES_REQ_EXT_STRU;

/*
 * 127/04 LMP_FEATURES_RES_EXT
 */
typedef struct stru_lmp_features_res_ext
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 ExtOpCode;
    U8 FeaturesPage;
    U8 MaxSupportedPage;
    LMP_FEATURES_ARR ExtendedFeatures;

} LMP_FEATURES_RES_EXT_STRU;

/*
 * 127/16 LMP_CHANNEL_CLASSIFICATION_REQ
 */
#define AFH_REPORTING_DISABLED 0
#define AFH_REPORTING_ENABLED  1
typedef struct stru_lmp_channel_classification_req
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 ExtOpCode;
    U8 AfhReportingMode;
    U8 AfhMinIntervalB0;
    U8 AfhMinIntervalB1;
    U8 AfhMaxIntervalB0;
    U8 AfhMaxIntervalB1;

} LMP_CHANNEL_CLASSIFICATION_REQ_STRU;

/*
 * 127/17 LMP_CHANNEL_CLASSIFICATION
 */
typedef struct stru_lmp_channel_classification
{
    LMP_TID_OPCODE_UNION TidOpcode;
    U8 ExtOpCode;
    BT_CH_MAP_ARR AfhChannelClassification;

} LMP_CHANNEL_CLASSIFICATION_STRU;


#endif /* _LMP_H_ */

