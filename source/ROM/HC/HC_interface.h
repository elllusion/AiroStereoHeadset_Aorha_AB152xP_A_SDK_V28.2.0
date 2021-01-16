//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////
#ifndef _HC_INTERFACE_H_
#define _HC_INTERFACE_H_

#include "os.h"
#include "hci.h"
#include "hci_airoha.h"
#include "HC_intr.h"
#include "drv_sector.h"
#include "hc.h"

#include "sector_hc_parameter.h"

//#define ENABLE_PAYLOAD_COUNT_FOR_CHANNEL_ASSESS

/* Temp old HCI DATA STRUCTURE */
typedef struct stru_hc_data_msg_old
{
    U8 Offset;
    U8 RSVD1;
    U8 RSVD2;
    U8 RSVD3;
    U8 RSVD4;
    U8 PbFlag;
    U16 Len;
    U8 Payload[1];

} HC_DATA_MSG_OLD_STRU;

#ifndef HC_DATA_MSG_OLD_OFFSET
#define HC_DATA_MSG_OLD_OFFSET (OS_OFFSET_OF(HC_DATA_MSG_OLD_STRU,PbFlag))
#else
#error "HC_DATA_MSG_OLD_OFFSET has been defined"
#endif


//OGF offset:
#define HC_OGF01_OFFSET                     0x00                        //0x00
#define HC_OGF02_OFFSET                     (HC_OGF01_OFFSET + 0x3C)    //0x3C
#define HC_OGF03_OFFSET                     (HC_OGF02_OFFSET + 0x11)    //0x4D
#define HC_OGF04_OFFSET                     (HC_OGF03_OFFSET + 0x6D)    //0xBA
#define HC_OGF05_OFFSET                     (HC_OGF04_OFFSET + 0x0A)    //0xC4
#define HC_OGF06_OFFSET                     (HC_OGF05_OFFSET + 0x0B)    //0xCF
#define HC_OGF08_OFFSET                     (HC_OGF06_OFFSET + 0x09)    //0xD8
#define HC_AIROHA_OGF_OFFSET                (HC_OGF08_OFFSET + 0x1F)    //0xF7


//OGF:1 Link Control Commands------------------------------------------------------
#define HCI_CMD_INQUIRY                                 (0x01)
#define HCI_CMD_INQUIRY_CANCEL                          (0x02)
#define HCI_CMD_PERIODIC_INQUIRY_MODE                   (0x03)
#define HCI_CMD_EXIT_PERIODIC_INQUIRY_MODE              (0x04)
#define HCI_CMD_CREATE_CONNECTION                       (0x05)
#define HCI_CMD_DISCONNECT                              (0x06)
#define HCI_CMD_ADD_SCO_CONNECTION                      (0x07)//Deprecated
#define HCI_CMD_CREATE_CONNECTION_CANCEL                (0x08)
#define HCI_CMD_ACCEPT_CONNECTION_REQUEST               (0x09)
#define HCI_CMD_REJECT_CONNECTION_REQUEST               (0x0A)
#define HCI_CMD_LINK_KEY_REQUEST_REPLY                  (0X0B)
#define HCI_CMD_LINK_KEY_REQUEST_NEGATIVE_REPLY         (0X0C)
#define HCI_CMD_PIN_CODE_REQUEST_REPLY                  (0X0D)
#define HCI_CMD_PIN_CODE_REQUEST_NEGATIVE_REPLY         (0X0E)
#define HCI_CMD_CHANGE_CONNECTION_PACKET_TYPE           (0x0F)
#define HCI_CMD_ACL_U_GO_STOP_CONFIRM                   (0x10)//Airoha defined
#define HCI_CMD_AUTHENTICATION_REQUESTED                (0x11)
#define HCI_CMD_ACL_LINK_INDEX_CHANGED_CONFIRM          (0x12)//Airoha defined
#define HCI_CMD_SET_CONNECTION_ENCRYPTION               (0x13)
#define HCI_CMD_RESEGMENT_CNF                           (0x14)//Airoha defineds
#define HCI_CMD_CHANGE_CONNECTION_LINK_KEY              (0x15)
//Not Defined                                           (0x16)
#define HCI_CMD_MASTER_LINK_KEY                         (0x17)
//Not Defined                                           (0x18)
#define HCI_CMD_REMOTE_NAME_REQUEST                     (0x19)
#define HCI_CMD_REMOTE_NAME_REQUEST_CANCEL              (0x1A)
#define HCI_CMD_READ_REMOTE_SUPPORTED_FEATURES          (0x1B)
#define HCI_CMD_READ_REMOTE_EXTENDED_FEATURES           (0x1C)
#define HCI_CMD_READ_REMOTE_VERSION_INFORMATION         (0x1D)
//Not Defined                                           (0x1E)
#define HCI_CMD_READ_CLOCK_OFFSET                       (0x1F)
#define HCI_CMD_READ_LMP_HANDLE                         (0x20)
//Not Defined                                           (0x21~0x27)
#define HCI_CMD_SETUP_SYNCHRONOUS_CONNECTION            (0x28)
#define HCI_CMD_ACCEPT_SYNCHRONOUS_CONNECTION_REQUEST   (0x29)
#define HCI_CMD_REJECT_SYNCHRONOUS_CONNECTION_REQUEST   (0x2A)
#define HCI_CMD_IO_CAPABILITY_REQUEST_REPLY             (0x2B)
#define HCI_CMD_USR_CONFIRMATION_REQ_REPLY              (0x2C)
#define HCI_CMD_USR_CONFIRMATION_REQ_NEGATIVE_REPLY     (0x2D)
#define HCI_CMD_USR_PASSKEY_REQ_REPLY                   (0x2E)
#define HCI_CMD_USR_PASSKEY_REQ_NEGATIVE_REPLY          (0x2F)
#define HCI_CMD_REMOTE_OOB_DATA_REQ_REPLY               (0x30)
//Not Defined                                           (0x31~0x32)
#define HCI_CMD_REMOTE_OOB_DATA_REQ_NEGATIVE_REPLY      (0x33)
#define HCI_CMD_IO_CAPABILITY_REQ_NEGATIVE_REPLY        (0x34)
#define HCI_CMD_CREATE_PHYSICAL_LINK                    (0x35)
#define HCI_CMD_ACCEPT_PHYSICAL_LINK                    (0x36)
#define HCI_CMD_DISCONNECT_PHYSICAL_LINK                (0x37)
#define HCI_CMD_CREATE_LOGICAL_LINK                     (0x38)
#define HCI_CMD_ACCEPT_LOGICAL_LINK                     (0x39)
#define HCI_CMD_DISCONNECT_LOGICAL_LINK                 (0x3A)
#define HCI_CMD_LOGICAL_LINK_CANCEL                     (0x3B)
#define HCI_CMD_FLOW_SPEC_MODIFY                        (0x3C)
//---------------------------------------------------------------------------------

//OGF:2 Link Policy Commands-------------------------------------------------------
#define HCI_CMD_HOLD_MODE                               (0x01 + HC_OGF02_OFFSET)
//Not Defined                                           (0x02 + HC_OGF02_OFFSET)
#define HCI_CMD_SNIFF_MODE                              (0x03 + HC_OGF02_OFFSET)
#define HCI_CMD_EXIT_SNIFF_MODE                         (0x04 + HC_OGF02_OFFSET)
#define HCI_CMD_PARK_STATE                              (0x05 + HC_OGF02_OFFSET)
#define HCI_CMD_EXIT_PARK_STATE                         (0x06 + HC_OGF02_OFFSET)
#define HCI_CMD_QOS_SETUP                               (0x07 + HC_OGF02_OFFSET)
//Not Defined                                           (0x08 + HC_OGF02_OFFSET)
#define HCI_CMD_ROLE_DISCOVERY                          (0x09 + HC_OGF02_OFFSET)
//Not Defined                                           (0x0A + HC_OGF02_OFFSET)
#define HCI_CMD_SWITCH_ROLE                             (0x0B + HC_OGF02_OFFSET)
#define HCI_CMD_READ_LINK_POLICY_SETTINGS               (0x0C + HC_OGF02_OFFSET)
#define HCI_CMD_WRITE_LINK_POLICY_SETTINGS              (0x0D + HC_OGF02_OFFSET)
#define HCI_CMD_READ_DEFAULT_LINK_POLICY_SETTINGS       (0x0E + HC_OGF02_OFFSET)
#define HCI_CMD_WRITE_DEFAULT_LINK_POLICY_SETTINGS      (0x0F + HC_OGF02_OFFSET)
#define HCI_CMD_FLOW_SPECIFICATION                      (0x10 + HC_OGF02_OFFSET)
#define HCI_CMD_SNIFF_SUBRATING                         (0x11 + HC_OGF02_OFFSET)
//---------------------------------------------------------------------------------

//OGF:3 Controller & Baseband Commands---------------------------------------------
#define HCI_CMD_SET_EVENT_MASK                          (0x01 + HC_OGF03_OFFSET)
//Not Defined                                           (0x02 + HC_OGF03_OFFSET)
#define HCI_CMD_RESET                                   (0x03 + HC_OGF03_OFFSET)
//Not Defined                                           (0x04 + HC_OGF03_OFFSET)
#define HCI_CMD_SET_EVENT_FILTER                        (0x05 + HC_OGF03_OFFSET)
//Not Defined                                           (0x06 + HC_OGF03_OFFSET)
//Not Defined                                           (0x07 + HC_OGF03_OFFSET)
#define HCI_CMD_FLUSH                                   (0x08 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_PIN_TYPE                           (0x09 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_PIN_TYPE                          (0x0A + HC_OGF03_OFFSET)
#define HCI_CMD_CREATE_NEW_UNIT_KEY                     (0x0B + HC_OGF03_OFFSET)
//Not Defined                                           (0x0C + HC_OGF03_OFFSET)
#define HCI_CMD_READ_STORED_LINK_KEY                    (0x0D + HC_OGF03_OFFSET)
//Not Defined                                           (0x0E + HC_OGF03_OFFSET)
//Not Defined                                           (0x0F + HC_OGF03_OFFSET)
//Not Defined                                           (0x10 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_STORED_LINK_KEY                   (0x11 + HC_OGF03_OFFSET)
#define HCI_CMD_DELETE_STORED_LINK_KEY                  (0x12 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_LOCAL_NAME                        (0x13 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_LOCAL_NAME                         (0x14 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_CONNECTION_ACCEPT_TIMEOUT          (0x15 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_CONNECTION_ACCEPT_TIMEOUT         (0x16 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_PAGE_TIMEOUT                       (0x17 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_PAGE_TIMEOUT                      (0x18 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_SCAN_ENABLE                        (0x19 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_SCAN_ENABLE                       (0x1A + HC_OGF03_OFFSET)
#define HCI_CMD_READ_PAGE_SCAN_ACTIVITY                 (0x1B + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_PAGE_SCAN_ACTIVITY                (0x1C + HC_OGF03_OFFSET)
#define HCI_CMD_READ_INQUIRY_SCAN_ACTIVITY              (0x1D + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_INQUIRY_SCAN_ACTIVITY             (0x1E + HC_OGF03_OFFSET)
#define HCI_CMD_READ_AUTHENTICATION_ENABLE              (0x1F + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_AUTHENTICATION_ENABLE             (0x20 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_ENCRYPTION_MODE                    (0x21 + HC_OGF03_OFFSET) //Deprecated
#define HCI_CMD_WRITE_ENCRYPTION_MODE                   (0x22 + HC_OGF03_OFFSET) //Deprecated
#define HCI_CMD_READ_CLASS_OF_DEVICE                    (0x23 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_CLASS_OF_DEVICE                   (0x24 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_VOICE_SETTING                      (0x25 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_VOICE_SETTING                     (0x26 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_AUTOMATIC_FLUSH_TIMEOUT            (0x27 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_AUTOMATIC_FLUSH_TIMEOUT           (0x28 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_NUM_BROADCAST_RETRANSMISSIONS      (0x29 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_NUM_BROADCAST_RETRANSMISSIONS     (0x2A + HC_OGF03_OFFSET)
#define HCI_CMD_READ_HOLD_MODE_ACTIVITY                 (0x2B + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_HOLD_MODE_ACTIVITY                (0x2C + HC_OGF03_OFFSET)
#define HCI_CMD_READ_TRANSMIT_POWER_LEVEL               (0x2D + HC_OGF03_OFFSET)
#define HCI_CMD_READ_SYNCHRONOUS_FLOW_CONTROL_ENABLE    (0x2E + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_SYNCHRONOUS_FLOW_CONTROL_ENABLE   (0x2F + HC_OGF03_OFFSET)
//Not Defined                                           (0x30 + HC_OGF03_OFFSET)
#define HCI_CMD_SET_CONTROLLER_TO_HOST_FLOW_CONTROL     (0x31 + HC_OGF03_OFFSET)
//Not Defined                                           (0x32 + HC_OGF03_OFFSET)
#define HCI_CMD_HOST_BUFFER_SIZE                        (0x33 + HC_OGF03_OFFSET)
//Not Defined                                           (0x34 + HC_OGF03_OFFSET)
#define HCI_CMD_HOST_NUMBER_OF_COMPLETED_PACKETS        (0x35 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_LINK_SUPERVISION_TIMEOUT           (0x36 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_LINK_SUPERVISION_TIMEOUT          (0x37 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_NUMBER_OF_SUPPORTED_IAC            (0x38 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_CURRENT_IAC_LAP                    (0x39 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_CURRENT_IAC_LAP                   (0x3A + HC_OGF03_OFFSET)
//Not Defined                                           (0x3B~0x3E + HC_OGF03_OFFSET)
#define HCI_CMD_SET_AFH_HOST_CHANNEL_CLASSIFICATION     (0x3F + HC_OGF03_OFFSET)
//Not Defined                                           (0x40 + HC_OGF03_OFFSET)
//Not Defined                                           (0x41 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_INQUIRY_SCAN_TYPE                  (0x42 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_INQUIRY_SCAN_TYPE                 (0x43 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_INQUIRY_MODE                       (0x44 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_INQUIRY_MODE                      (0x45 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_PAGE_SCAN_TYPE                     (0x46 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_PAGE_SCAN_TYPE                    (0x47 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_AFH_CHANNEL_ASSESSMENT_MODE        (0x48 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_AFH_CHANNEL_ASSESSMENT_MODE       (0x49 + HC_OGF03_OFFSET)
//Not Defined                                           (0x4A~0x50 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_EXTENDED_INQUIRY_RESPONSE          (0x51 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_EXTENDED_INQUIRY_RESPONSE         (0x52 + HC_OGF03_OFFSET)
#define HCI_CMD_REFRESH_ENCRYPTION_KEY                  (0x53 + HC_OGF03_OFFSET)
//Not Defined                                           (0x54 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_SIMPLE_PAIRING_MODE                (0x55 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_SIMPLE_PAIRING_MODE               (0x56 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_LOCAL_OOB_DATA                     (0x57 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_INQUIRY_RESPONSE_TRANSMIT_POWER_LEVEL (0x58 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL      (0x59 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_DEFAULT_ERRONEOUS_DATA_REPORTING   (0x5A + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_DEFAULT_ERRONEOUS_DATA_REPORTING  (0x5B + HC_OGF03_OFFSET)
//Not Defined                                           (0x5C~0x5E + HC_OGF03_OFFSET)
#define HCI_CMD_ENHANCED_FLUSH                          (0x5F + HC_OGF03_OFFSET)
#define HCI_CMD_SEND_KEYPRESS_NOTIFICATION              (0x60 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_LOGICAL_LINK_ACCEPT_TIMEOUT        (0x61 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT       (0x62 + HC_OGF03_OFFSET)
#define HCI_CMD_SET_EVENT_MASK_PAGE_2                   (0x63 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_LOCATION_DATA                      (0x64 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_LOCATION_DATA                     (0x65 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_FLOW_CONTROL_MODE                  (0x66 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_FLOW_CONTROL_MODE                 (0x67 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_ENHANCE_TRANSMIT_POWER_LEVEL       (0x68 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_BEST_EFFORT_FLUSH_TIMEOUT          (0x69 + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_BEST_EFFORT_FLUSH_TIMEOUT         (0x6A + HC_OGF03_OFFSET)
#define HCI_CMD_SHORT_RANGE_MODE                        (0x6B + HC_OGF03_OFFSET)
#define HCI_CMD_READ_LE_HOST_SUPPORT                    (0x6C + HC_OGF03_OFFSET)
#define HCI_CMD_WRITE_LE_HOST_SUPPORT                   (0x6D + HC_OGF03_OFFSET)
//---------------------------------------------------------------------------------

//OGF:4 Informational Parameters---------------------------------------------------
#define HCI_CMD_READ_LOCAL_VERSION_INFORMATION          (0x01 + HC_OGF04_OFFSET)
#define HCI_CMD_READ_LOCAL_SUPPORTED_COMMANDS           (0x02 + HC_OGF04_OFFSET)
#define HCI_CMD_READ_LOCAL_SUPPORTED_FEATURES           (0x03 + HC_OGF04_OFFSET)
#define HCI_CMD_READ_LOCAL_EXTENDED_FEATURES            (0x04 + HC_OGF04_OFFSET)
#define HCI_CMD_READ_BUFFER_SIZE                        (0x05 + HC_OGF04_OFFSET)
//Not Defined                                           (0x06~0x08 + HC_OGF03_OFFSET)
#define HCI_CMD_READ_BD_ADDR                            (0x09 + HC_OGF04_OFFSET)
#define HCI_CMD_READ_DATE_BLOCK_SIZE                    (0x0A + HC_OGF04_OFFSET)
//---------------------------------------------------------------------------------

//OGF:5 Status Parameters----------------------------------------------------------
#define HCI_CMD_READ_FAILED_CONTACT_COUNTER             (0x01 + HC_OGF05_OFFSET)
#define HCI_CMD_RESET_FAILED_CONTACT_COUNTER            (0x02 + HC_OGF05_OFFSET)
#define HCI_CMD_READ_LINK_QUALITY                       (0x03 + HC_OGF05_OFFSET)
//Not Defined                                           (0x04 + HC_OGF05_OFFSET)
#define HCI_CMD_READ_RSSI                               (0x05 + HC_OGF05_OFFSET)
#define HCI_CMD_READ_AFH_CHANNEL_MAP                    (0x06 + HC_OGF05_OFFSET)
#define HCI_CMD_READ_CLOCK                              (0x07 + HC_OGF05_OFFSET)
#define HCI_CMD_READ_ENCRYPTION_KEY_SIZE                (0x08 + HC_OGF05_OFFSET)
#define HCI_CMD_READ_LOCAL_AMP_INFO                     (0x09 + HC_OGF05_OFFSET)
#define HCI_CMD_READ_LOCAL_AMP_ASSOC                    (0x0A + HC_OGF05_OFFSET)
#define HCI_CMD_WRITE_REMOTE_AMP_ASSOC                  (0x0B + HC_OGF05_OFFSET)
//---------------------------------------------------------------------------------


//OGF:6 Testing Commands-----------------------------------------------------------
#define HCI_CMD_READ_LOOPBACK_MODE                      (0x01 + HC_OGF06_OFFSET)
#define HCI_CMD_WRITE_LOOPBACK_MODE                     (0x02 + HC_OGF06_OFFSET)
#define HCI_CMD_ENABLE_DEVICE_UNDER_TEST_MODE           (0x03 + HC_OGF06_OFFSET)
#define HCI_CMD_WRITE_SIMPLE_PAIRING_DEBUG_MODE         (0x04 + HC_OGF06_OFFSET)
//Not Defined                                           (0x05 + HC_OGF06_OFFSET)
//Not Defined                                           (0x06 + HC_OGF06_OFFSET)
#define HCI_CMD_ENABLE_AMP_RECEIVER_REPORTS             (0x07 + HC_OGF06_OFFSET)
#define HCI_CMD_AMP_TEST_END                            (0x08 + HC_OGF06_OFFSET)
#define HCI_CMD_AMP_TEST                                (0x09 + HC_OGF06_OFFSET)
//---------------------------------------------------------------------------------


//OGF:8 LC Controller Commands-----------------------------------------------------------
#define HCI_CMD_LE_SET_EVENT_MASK                       (0x01 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_READ_BUFFER_SIZE                     (0x02 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_READ_LOCAL_SUPPORTED_FEATURES        (0x03 + HC_OGF08_OFFSET)
//Not Defined                                           (0x04 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_SET_RANDOM_ADDRESS                   (0x05 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_SET_ADVERTISING_PARAMETER            (0x06 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_READ_ADVERTISING_CHANNEL_TX_POWER    (0x07 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_SET_ADVERTISING_DATA                 (0x08 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_SET_SCAN_RESPONSE_DATA               (0x09 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_SET_ADVERTISING_ENABLE               (0x0A + HC_OGF08_OFFSET)
#define HCI_CMD_LE_SET_SCAN_PARAMETERS                  (0x0B + HC_OGF08_OFFSET)
#define HCI_CMD_LE_SET_SCAN_ENABLE                      (0x0C + HC_OGF08_OFFSET)
#define HCI_CMD_LE_CREATE_CONNECTION                    (0x0D + HC_OGF08_OFFSET)
#define HCI_CMD_LE_CREATE_CONNECTION_CANCEL             (0x0E + HC_OGF08_OFFSET)
#define HCI_CMD_LE_READ_WHITE_LIST_SIZE                 (0x0F + HC_OGF08_OFFSET)
#define HCI_CMD_LE_CLEAR_WHITE_LIST                     (0x10 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_ADD_DEVICE_TO_WHITE_LIST             (0x11 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_REMOVE_DEVICE_FROM_WHITE_LIST        (0x12 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_CONNECTION_UPDATE                    (0x13 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_SET_HOST_CHANNEL_CLASSIFICATION      (0x14 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_READ_CHANNEL_MAP                     (0x15 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_READ_REMOTE_USED_FEATURES            (0x16 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_ENCRYPT                              (0x17 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_RAND                                 (0x18 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_START_ENCRYPTION                     (0x19 + HC_OGF08_OFFSET)
#define HCI_CMD_LE_LONG_TERM_KEY_REQUEST_REPLY          (0x1A + HC_OGF08_OFFSET)
#define HCI_CMD_LE_LONG_TERM_KEY_REQUEST_NEGATIVE_REPLY (0x1B + HC_OGF08_OFFSET)
#define HCI_CMD_LE_READ_SUPPORTED_STATES                (0x1C + HC_OGF08_OFFSET)
#define HCI_CMD_LE_RECEIVER_TEST                        (0x1D + HC_OGF08_OFFSET)
#define HCI_CMD_LE_TRANSMITTER_TEST                     (0x1E + HC_OGF08_OFFSET)
#define HCI_CMD_LE_TEST_END                             (0x1F + HC_OGF08_OFFSET)
//---------------------------------------------------------------------------------

//OGF:Airoha----------------------------------------------------------------------------
#define HCI_VCMD_AIROHA                                 (0x01 + HC_AIROHA_OGF_OFFSET)   //- 0xF2
#define HCI_VCMD_AIROHA_TO_UART							(0xFF)
//#define HCI_VCMD_SET_AFH                              (0x08 + HC_AIROHA_OGF_OFFSET)   //- 0xFF //CxR's cmd/evt, useless now


//===================================================================================



#define HCI_CMD_OFFSET 2
typedef struct
{
    U8 offset;
    U8 dummy;
    U8 reduceOpcode;
    U8 stdOgf; //standard ogf filed
    U8 len;
    U8 firstPara;
}HciCmdType;

#define HCI_CMD_REDUCED_OPCODE(ptr) (((HciCmdType XDATA_PTR)ptr)->reduceOpcode)
#define HCI_CMD_STD_OGF(ptr) (((HciCmdType XDATA_PTR)ptr)->stdOgf)
#define HCI_CMD_PARAMETER_LENGTH(ptr) (((HciCmdType XDATA_PTR)ptr)->len)
#define HCI_CMD_PARAMETER_PTR(ptr) ((U8 XDATA_PTR)&(((HciCmdType XDATA_PTR)ptr)->firstPara))
#define HCI_CMD_CONTENT(ptr,N) (((U8 XDATA_PTR)&(((HciCmdType XDATA_PTR)ptr)->firstPara))[N])
#define HCI_CMD_PARAM_PTR(ptr) HCI_CMD_PARAMETER_PTR(ptr)
#define HCI_CMD_PARAM(ptr,N) HCI_CMD_CONTENT(ptr,N)

typedef struct
{
    U8 offset;
    U8 dummy;
    U8 reduceOpcode;
    U8 stdOgf;
    U8 len;
    U8 vcmdOpcode;
    U8 ogf;
    U8 firstPara;
}HciVCmdType;

#define HCI_VCMD_OPCODE(ptr) (((HciVCmdType XDATA_PTR)ptr)->vcmdOpcode)
#define HCI_VCMD_OGF(ptr) (((HciVCmdType XDATA_PTR)ptr)->ogf)
#define HCI_VCMD_PARAMETER_LENGTH(ptr) (((HciVCmdType XDATA_PTR)ptr)->len)
#define HCI_VCMD_PARAMETER_PTR(ptr) ((U8 XDATA_PTR)&(((HciVCmdType XDATA_PTR)ptr)->firstPara))
#define HCI_VCMD_PARAM(ptr,N) (((U8 XDATA_PTR)&(((HciVCmdType XDATA_PTR)ptr)->firstPara))[N])

#define HCI_L2CAP_CID_FOR_REMOTE_EEPROM_UPDATE      0x0022

#define HCI_DATA_PB(ptr)                    (*(U8 XDATA_PTR)(ptr + (*ptr) + 0) & (0x03 << 4))
#define HCI_DATA_FIRST_BYTE(ptr)            (*(U8 XDATA_PTR)(ptr + (*ptr)))
#define HCI_DATA_TOTAL_LENGTH(ptr)          (*(U8 XDATA_PTR)(ptr + (*ptr) + 1) + ((U16)(*((U8 XDATA_PTR)(ptr + (*ptr) + 2))) << 8))
#define HCI_DATA_L2CAP_LENGTH(ptr)          (*(U8 XDATA_PTR)(ptr + (*ptr) + 3) + ((U16)(*((U8 XDATA_PTR)(ptr + (*ptr) + 4))) << 8))
#define HCI_DATA_L2CAP_CID(ptr)             (*(U8 XDATA_PTR)(ptr + (*ptr) + 5) + ((U16)(*((U8 XDATA_PTR)(ptr + (*ptr) + 6))) << 8))
#define HCI_DATA_DATA_BEGIN_PTR(ptr)        ((U8 XDATA_PTR)(ptr + (*ptr) + 3))

#define HC_EVENT_CODE(ptr)                  *(U8 XDATA_PTR)(ptr + (*ptr))
#define HC_EVENT_PARAM_TOTAL_LENGTH(ptr)    *(U8 XDATA_PTR)(ptr + (*ptr) + 1)
#define HC_EVENT_PARAM_PTR(ptr)             (ptr + (*ptr) + 2)

#define HC_PB_MASK                          (0x03 << 4)
#define HC_PB_L2CAP_NON_FLUSH_START         (0x00 << 4)
#define HC_PB_L2CAP_START                   (0x02 << 4)
#define HC_PB_L2CAP_CONTINUE                (0x01 << 4)
#define HC_BC_MASK                          (0x03 << 6)
#define HC_BC_ASB                           (0x01 << 6)
#define HC_BC_PSB                           (0x02 << 6)
#define HC_FLOW_GO                          (0x01 << 5)
#define HC_PF_MASK                          (0x01 << 3)

#define HC_DATA_OFFSET                      5



//For Flush event
#define HC_FLUSH_TIMEOUT_DISABLE            0x0000
#define HC_LC_EVENT_FLUSH_OCCOR             (0x11)
#define HC_LC_EVENT_CODE(ptr)               *(U8 XDATA_PTR)(ptr+*(ptr))
#define HC_LC_EVENT_FLUSH_OCCOR_PARAM_CONNECTION_HANDLE(ptr)                (*(U8 XDATA_PTR)(ptr+*(ptr)+1))



typedef U8 HCI_AUTO_ACCEPT_FLAG;
#define HCI_AUTO_ACCEPT_FLAG__OFF                                           (HCI_AUTO_ACCEPT_FLAG)0x01
#define HCI_AUTO_ACCEPT_FLAG__ON_WITHOUT_ROLE_SWITCH                        (HCI_AUTO_ACCEPT_FLAG)0x02
#define HCI_AUTO_ACCEPT_FLAG__ON_WITH_ROLE_SWITCH                           (HCI_AUTO_ACCEPT_FLAG)0x03

//Event code: in LM_interface.h

#define HCI_STANDARD_SCAN                                                   0

#ifdef HID_SOC
    #define HC_COMPLETE_ARRAY_SIZE                                              32
#else
    #define HC_COMPLETE_ARRAY_SIZE                                              8
#endif
#define HC_COMPLETE_ARRAY_MASK                                              (HC_COMPLETE_ARRAY_SIZE - 1)

#define HC_IN   0
#define HC_OUT  1

//================  BT4.0 ================//
#define PUBLIC_DEVICE_ADDRESS   0x00
#define RANDOM_DEVICE_ADDRESS   0x01


//-------------Scatter gather------------------------------------
#define     HC_BC_LINK_INDEX    MAX_NUM_ACL_LINK //the last one
#define     HC_LC_BLOCKS        4
#define     HC_LC_DATA_TIME_OUT 1
#define     HC_FLUSH_TRANSPORT_STOP         0
#define     HC_FLUSH_TRANSPORT_START        1

#define     HC_FLUSH_NON_START          0
#define     HC_FLUSH_FIRST_START        1
#define     HC_FLUSH_CLEAN_COMPLETED_Q  2
#define     HC_FLUSH_CLEAN_L2CAP        3

#define     HC_SEND_DATA_TO_LC_WAIT_TO_TRIGGER      0
#define     HC_SEND_DATA_TO_LC_TIMEOUT              1
#define     HC_SEND_DATA_TO_LC_BC                   2
#define     HC_SEND_DATA_TO_LC_NEXT_IS_BC           3
#define     HC_SEND_DATA_TO_LC_L2CAPEND             4
#define     HC_SEND_DATA_TO_LC_FULLFILL_BLOCK       5
#define     HC_SEND_DATA_TO_LC_BIGGER_THAN_SLOT     6
#define     HC_SEND_DATA_TO_LC_FIRST_L2CAP_PKT      7
#define     HC_SEND_DATA_TO_LC_IMMEDIATELY          8
#define     HC_SEND_DATA_TO_LC_NEXT_PKT_IS_L2CAP_START  9

#define     HC_FLUSH_STATE_NO_FLUSH                     0
#define     HC_FLUSH_STATE_PREPARE_CMD_TO_LC            1
#define     HC_FLUSH_STATE_START_CMD_TO_LC              2
#define     HC_FLUSH_STATE_START_EVENT_FROM_LC          3
#define     HC_FLUSH_STATE_GROW_UP_TO_BIG_PACKET        4
#define     HC_FLUSH_STATE_PB_00_PKT_CA_ZAI_LC          5
#define     HC_FLUSH_STATE_CLEAN_DOWN_QUEUE             6
#define     HC_FLUSH_STATE_ON_GOING                     7
#define     HC_FLUSH_STATE_END_PROCESS                  8
#define     HC_FLUSH_STATE_STOP_CMD_TO_LC               9

#define     HC_LC_DATA_SCR_BIT0_MASK_L2CAPEND           0x80
#define     HC_LC_DATA_SCR_BIT0_MASK_RELEASEALL         0x40
#define     HC_LC_DATA_SCR_BIT0_MASK_NBLOCK             0x0E
#define     HC_LC_DATA_SCR_BIT0_MASK_SCATTERGATTER      0x01

#define     HC_LC_DATA_SCR_BIT1_MASK_PB                 0xC0
#define     HC_LC_DATA_SCR_BIT1_MASK_EDRPKTTYPE         0x1F
#define     HC_LC_DATA_SCR_BIT1_MASK_EDR                0x10
#define     HC_LC_DATA_SCR_BIT1_MASK_PKTTYPE            0x0F

#define     HC_LC_ACL_SHARE_CTRL_MASK_L2CAPFLOW         0x80
#define     HC_LC_ACL_SHARE_CTRL_MASK_ACLUGO            0x20
#define     HC_LC_ACL_SHARE_CTRL_MASK_EDRPKTTYPE        0x1F
#define     HC_LC_ACL_SHARE_CTRL_MASK_EDR               0x10
#define     HC_LC_ACL_SHARE_CTRL_MASK_PKTTYPE           0x0F

#define     HC_DATA_OFFSET_NEW      5

typedef struct
{
    U8  XDATA_PTR       data_ptr;
    U16                 data_len;
    U8  XDATA_PTR       head_ptr;
} HC_LC_BLOCK_INFO;

typedef struct
{
    U8                      offset;
    U8                      dummy[HC_DATA_OFFSET-1];
    U8                      ctrl_B0;
    U8                      ctrl_B1;
    U16                     total_len;
    HC_LC_BLOCK_INFO        block_info[HC_LC_BLOCKS];
} HC_LC_DATA_TRANS_CTRL_BLOCK;

typedef union
{
    struct
    {
        U8              support_packet_type         :4;
        U8              support_edr                 :1;
        U8              aclu_go                     :1;
        U8              b6                          :1;
        U8              L2CAP_flow                  :1;
    } member;
    U8 value;
} HC_LC_ACL_SHARE_CTRL_B0;

typedef struct
{
    OST XDATA_PTR multi_flush_timer_ptr[MQ_SIZE_LC_DATA_FROM_HC];
    U8  pb_array[MQ_SIZE_LC_DATA_FROM_HC];
    U8  in_index;
    U8  out_index;
} HC_FLUSH_PB_TIMER;


typedef struct
{
    U8  job[MQ_SIZE_LC_DATA_FROM_HC];
    U8  in;
    U8  out;
    U8  used;
} HC_FLUSH_JOB_QUEUE;

//--------------end scatter gather--------------------------

typedef struct
{
    U8              flow_stop                   : 1;
    U8              send_to_lc_immediately      : 1;
    U8              waiting_LC_recycle_respn    : 1;
    U8              ACL_U_stop                  : 1;
    U8              acl_packet_resegment_active : 1;
    U8              b5                          : 1;
    U8              send_to_lc                  : 1;
    U8              LC_recycle_done             : 1;
} HC_DATA_LINK_BIT_CTL0;

typedef struct
{

    U8              data_link_state;
    U16             total_l2cap_length;
    U16             remain_l2cap_length;
    U8              num_data_packets;
    U8              num_completed_data_packets;
    OST XDATA_PTR   flush_timer_ptr;
    HC_DATA_LINK_BIT_CTL0   bit_ctl0;
    HC_LC_DATA_TRANS_CTRL_BLOCK XDATA_PTR   last_trans_ctrl_block_ptr;
    OST XDATA_PTR           hc_lc_data_ctrl_timer_ptr;
    U8  XDATA_PTR           remain_hci_pkt_ptr;
    U16                     remain_hci_pkt_len;
    U8  XDATA_PTR           remain_hci_pkt_original_ptr;
    U16                     segment_remain_capacity;
    U8                      index;
    HC_LC_ACL_SHARE_CTRL_B0     hc_lc_acl_share_ctrl_b0;
    HC_FLUSH_PB_TIMER   flush_pb_timer;
    HC_FLUSH_JOB_QUEUE  flush_job_queue;
    U8              new_flush_state;
    U8              flush_freeze;
    U8              at_flush_start;
} HC_DATA_LINK_CTL_TYPE;


#if 0
typedef struct
{
    union
    {
        BD_ADDR bd_addr;
        struct
        {
            U8 class_of_device[3];
            U8 class_of_device_mask[3];
        } COD_condition;
    } part_condition;
    U8 auto_accept_flag;
} HC_CONDITION_TYPE;

typedef struct
{
    U8                  filter_type;
    U8                  filter_condition_type;
    HC_CONDITION_TYPE   condition;
} HC_EVENT_FILTER_TYPE;
#endif

typedef struct
{
    U8 b0     : 1;
    U8 flash_mode_active    : 1;
    U8 rx_flow_stop_ctrl    : 1;
    U8 b3                   : 1;
    U8 b4                   : 1;
    U8 b5                   : 1;
    U8 b6                   : 1;
    U8 b7                   : 1;
} HC_MISC_CTL_B0;


#ifdef LE_SUPPORTED_HOST
typedef struct
{
    U16 remain_hci_pkt_len;
    U8  XDATA_PTR remain_pkt_ptr;
    U8  XDATA_PTR rx_pkt_ptr;
    U8  isStart;
} HC_LE_DATA_TRANSPORT;
#endif


EXTERN HCI_CMD_HDLR XDATA pHciVCmdRelayToMMIHdlr;
EXTERN HCI_CMD_HDLR XDATA pHciRfBistGroupVCmdHdlr;


#define SET_FLASH_MODE(onoff)       gHC_ctl.misc_ctrl_B0.flash_mode_active = (BIT)(onoff)
#define GET_FLASH_MODE()            (gHC_ctl.misc_ctrl_B0.flash_mode_active)

PUBLIC U8 HC_ConnectionHandleToIndex (U16 connection_handle);
//PUBLIC void HC_RemoveConnectionHandleIndex (U8 connection_handle);

EXTERN PUBLIC void HC_RemoveL2capLinkAndConnectionHandle(U8 signalLink);


///////////////////////////////////////
//Bus related
/////////////////////////////////////


#define OGF_1                               ((U8)(0x01 << 2))
#define OGF_2                               ((U8)(0x02 << 2))
#define OGF_3                               ((U8)(0x03 << 2))
#define OGF_4                               ((U8)(0x04 << 2))
#define OGF_5                               ((U8)(0x05 << 2))
#define OGF_6                               ((U8)(0x06 << 2))
#define OGF_8                               ((U8)(0x08 << 2))
#define OGF_AIROHA_HCI_CMD_EVT              ((U8)(0xFF << 2))

#ifdef SUPPORT_TRANSPORT
#define HC_UART_HEADER_SIZE                 4
#define HC_UART_PACKET_TYPE_OFFSET          0
#define HC_UART_REDUCE_OPCODE_OFFSET        1
#define HC_UART_STDOGF_OFFSET               2
#define HC_UART_LENGTH_OFFSET               3
#define HC_UART_DATA_BEGIN_OFFSET           4
#endif


// HC_DataFlowControl
#define HC_LAST_PACKET_CLEAN            0x00
#define HC_LAST_PACKET_FROM_TRANSPORT   0x01
#define HC_L2CAP_END_GOING_TO_LC        0x02
#define HC_LAST_PACKET_FROM_LC          0x03

/////////////////////  End Temp SFR    ////////////////////////////////////////
typedef struct
{
    U8 taskReTryCnt;
    U8 BcCplPoolCnt;
    U8 isBC;
    U16 bcConnHandle;
    U16 remainBcLen;

} HC_TRANSPORT_CTL_TYPE;



EXTERN BIT gHC_UART_bus_error;
EXTERN HC_TRANSPORT_CTL_TYPE XDATA gHC_TRANSPORT_ctl;

void HC(void);
void HC_Init(void);
//VOID HC_EngineeringMode(VOID);
VOID HC_VendorCommandHandler(HciVCmdType XDATA_PTR ptr1);
VOID HC_VendorResponseFromMMIToUART(HciVCmdType XDATA_PTR ptr1);
VOID HC_LC_InterfaceHandler(VOID);
VOID HC_DownTransportWrapper1(VOID);


PUBLIC void HC_Download (void);
PUBLIC void HC_Download_UART1 (void);

PUBLIC void HC_Upload (void);
PUBLIC void HC_Upload_UART1 (void);

PUBLIC void HC_Uart_IntrHdlr (void);
PUBLIC void HC_Uart_BusInit (void);
PUBLIC void HC_UART1_InterruptHandle (void);

PUBLIC void VCMD_ReadROMVersion (void);
#ifdef SUPPORT_TRANSPORT
PUBLIC void VCMD_EraseFlash(U8 XDATA_PTR ptr_cmd_from_bus);
PUBLIC void VCMD_ReadFlashInit(U8 XDATA_PTR ptr_cmd_from_bus);
PUBLIC void VCMD_ReadFlash(void);
PUBLIC void VCMD_WriteFlashInit(U8 XDATA_PTR ptr_cmd_from_bus);
PUBLIC void VCMD_WriteFlash(U8 XDATA_PTR ptr_flash_data);
#endif

PUBLIC void VCMD_FlashReadManufacturerAndMemoryType(void);
PUBLIC void VCMD_FlashSectorErase(U32 addr);
PUBLIC void VCMD_FlashPageProgramData(HC_DATA_MSG_STRU XDATA_PTR ptr);
PUBLIC void VCMD_FlashSetPageReadAddr(U8 XDATA_PTR param_ptr);
PUBLIC void VCMD_FlashPageReadOutData(void);

#ifdef DMA_UART
PUBLIC void TxDmaPreSender(U8 pktType);
PUBLIC void TxDmaPreSender_UART1 (U8 pktType);

#endif

#define ACL_DATA_SIZE 382
#define ACL_INIT_PKTNO 8
#define LE_ACL_DATA_SIZE 40
#define LE_NUM_ACL_DATA_BUFFER 2

#if 0
#ifdef SUPPORT_TRANSPORT
	#define HC_UART_RX_DMA_SIZE 380
	#define HC_UART1_RX_DMA_SIZE 50
#endif
#endif

#ifdef LABTEST_ENABLED
//RF Test Tool Function
PUBLIC void VCMD_RfTestSwitch(U8 XDATA_PTR ptr_cmd_from_bus);
#endif



EXTERN U8 HC_Std2RdcOpCode(IN U8 OpCodeB0,IN U8 OpCodeB1);

BOOL HC_ProcessAclVCmdFlashOgf(HC_ACL_CMD_STRU XDATA_PTR pMsg);
/**
 * HC_FlashAclVCmdHdlr
 *
 * Transport flash data from UART to HC.
 *
 * @Author : DingaCheng <DingaCheng@airoha.com.tw>
 *
*/
VOID HC_FlashAclVCmdHdlr(VOID);


SYS_ERRCODE HC_HciAclUnknownCmdHdlr(
    IN      HC_ACL_CMD_STRU XDATA_PTR pCmdMsg);

HC_EVT_MSG_STRU_PTR HC_HciInqCplEvt(VOID);
HC_EVT_MSG_STRU_PTR HC_NewHciInqRstEvt(VOID);
HC_EVT_MSG_STRU_PTR HC_NewHciExtInqRstEvt(IN BOOL EirNeeded);

HCI_CMD_OPCODE_UNION _HC_HciOpCodeXferRdc2Std(IN  U8 ReducedOpCode);


SYS_ERRCODE HC_HciLeSetEvtMaskCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeRdBufSizeCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeRdLocalSuppFeaCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeSetRandAddrCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeSetAdvParaCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeRdAdvChTxPwrCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeSetAdvDataCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeSetScanRspDataCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeSetAdvEnableCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeRdWhiteListSizeCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeClrWhiteListCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeAddDeviceToWhiteListCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeRmDeviceFromWhiteListCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeRdChMapCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeRandCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeLtkReqReplyCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeLtkReqNegReplyCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciLeRdSuppStateCmdHdlr(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciVCmdRelayToMMI_FLASH(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);
SYS_ERRCODE HC_HciRfBistVCmdHdlr_FLASH(IN  HC_CMD_MSG_STRU_PTR pCmdMsg);


VOID HC_SendNoCplPktEvt(U16 connHandle);

#ifdef LE_SUPPORTED_HOST
U8   HC_LE_CommandHandler(U8 XDATA_PTR ptr1);
VOID HC_LE_DataDispatch(VOID);
VOID HC_LE_InterfaceHandler(VOID);
VOID HC_LE_SegmentDataToLC(U8 service_idx);
VOID HC_LE_DataFlowControl(VOID);
#endif



/* TEMPORAL CODE */
VOID HC_UpTransportWrapper(VOID);
VOID HC_DownTransportWrapper2(VOID);
VOID HC_DownTransportWrapper1(VOID);
VOID HC_CmdDataQueueWrapper(VOID);
VOID HC_SendHciRstCmdCplEvtToHost(VOID);


/**
 * HC_NewHciRtnLinkKeysEvt
 *
 * Allocate a memory space to cook a HCI RETURN LINK KEYS EVENT.
 *
 * @Author : Charles <CharlesSu@airoha.com.tw>
 *
 * @NumLinkKeys : Number of Link Keys want to return
 * @Return : pointer of HC_EVT_MSG_STRU
 */
HC_EVT_MSG_STRU_PTR HC_NewHciRtnLinkKeysEvt(
    IN      U8 NumLinkKeys);



#endif

/**
 * HC_NewHciVCmdCplEvt
 *
 * Allocate a memory space to cook a HCI VENDOR COMMAND COMPLETE EVENT with the STATUS
 * is assumed SUCCESS.
 *
 * @Author : WenChiehLo <WenChiehLo@airoha.com.tw>
 *
 * @OpCode : HCI Vedor Command OPCODE with AIROHA Defined
 * @RtnParaSize : the return PARAMETER size of given HCI Command
 * @Return : pointer of HC_EVT_MSG_STRU
 */
HC_EVT_MSG_STRU XDATA_PTR HC_NewHciVCmdCplEvt(
    IN      HCI_VOPCODE VOpCode,
    IN      U8 RtnParaSize);

/**
 * HC_NewHciVdrSpecEvt
 *
 * Allocate a memory space to return a vendor-specific event.
 *
 * @Author : WenChiehLo <WenChiehLo@airoha.com.tw>
 *
 * @OpCode : HCI Event OpCode defined by AIROHA
 * @RtnParaSize : the return PARAMETER size of given HCI Vendor Event
 * @Return : pointer of HC_EVT_MSG_STRU
 */
HC_EVT_MSG_STRU_PTR HC_NewHciVdrSpecEvt(
    IN      HCI_VOPCODE VOpCode,
    IN      U8 RtnParaSize);

/**
 * HC_HciRejectVCmdHdlr
 *
 * This function handles the HCI Vendor Command which we don't support. An HCI VENDOR COMMAND
 * COMPLETE EVENT with COMMAND DISALLOWED ERROR CODE will be generated after an
 * HCI Vendor Command which we can't handle has been recieved.
 *
 * @Author : WenChiehLo <WenChiehLo@airoha.com.tw>
 *
 * @pCmdMsg : pointer of HC_CMD_MSG_STRU
 * @Return : the status of memory failure
 */
U8 HC_HciRejectVCmdHdlr(
    IN      HC_CMD_MSG_STRU_PTR pCmdMsg);

/**
 * HC_HciUnknownVCmdHdlr
 *
 * This function handles the HCI Vendor Command with unknown OPCODE. An HCI VENDOR COMMAND
 * COMPLETE EVENT with UNKNOWN HCI COMMAND ERROR CODE will be generated after a
 * unknown HCI Vendor Command has been recieved.
 *
 * @Author : WenChiehLo <WenChiehLo@airoha.com.tw>
 *
 * @pCmdMsg : pointer of HC_CMD_MSG_STRU
 * @Return : the status of memory failure
 */
U8 HC_HciUnknownVCmdHdlr(
    IN      HC_CMD_MSG_STRU_PTR pCmdMsg);




