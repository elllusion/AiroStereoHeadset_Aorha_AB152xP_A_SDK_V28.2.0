#ifndef _HCI_ERROR_H_
#define _HCI_ERROR_H_
#include "config.h"
#include "cpu.h"



////////////////////////////////////////////////////////////////////////////////
// Bluetooth HCI Error Code Definitions ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//- Error Code;
typedef U8 HCI_ERR_CODE;
#define STATUS_OK                                                       ((HCI_ERR_CODE)0x00)
#define ERR_UNKNOWN_HCI_COMMAND                                         ((HCI_ERR_CODE)0x01)
#define ERR_UNKNOWN_CONNECTION_IDENTIFIER                               ((HCI_ERR_CODE)0x02)
#define ERR_HARDWARE_FAILURE                                            ((HCI_ERR_CODE)0x03)
#define ERR_PAGE_TIMEOUT                                                ((HCI_ERR_CODE)0x04)
#define ERR_AUTHENTICATION_FAILURE                                      ((HCI_ERR_CODE)0x05)
#define ERR_PIN_OR_KEY_MISSING                                          ((HCI_ERR_CODE)0x06)
#define ERR_MEMORY_CAPACITY_EXCEEDED                                    ((HCI_ERR_CODE)0x07)
#define ERR_CONNECTION_TIMEOUT                                          ((HCI_ERR_CODE)0x08)
#define ERR_CONNECTION_LIMIT_EXCEEDED                                   ((HCI_ERR_CODE)0x09)
#define ERR_SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED           ((HCI_ERR_CODE)0x0A)
#define ERR_ACL_CONNECTION_ALREADY_EXISTS                               ((HCI_ERR_CODE)0x0B)
#define ERR_COMMAND_DISALLOWED                                          ((HCI_ERR_CODE)0x0C)
#define ERR_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES                ((HCI_ERR_CODE)0x0D)
#define ERR_CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS                 ((HCI_ERR_CODE)0x0E)
#define ERR_CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR             ((HCI_ERR_CODE)0x0F)
#define ERR_CONNECTION_ACCEPT_TIMEOUT_EXCEEDED                          ((HCI_ERR_CODE)0x10)
#define ERR_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE                      ((HCI_ERR_CODE)0x11)
#define ERR_INVALID_HCI_COMMAND_PARAMETERS                              ((HCI_ERR_CODE)0x12)
#define ERR_REMOTE_USER_TERMINATED_CONNECTION                           ((HCI_ERR_CODE)0x13)
#define ERR_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES    ((HCI_ERR_CODE)0x14)
#define ERR_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF        ((HCI_ERR_CODE)0x15)
#define ERR_CONNECTION_TERMINATED_BY_LOCAL_HOST                         ((HCI_ERR_CODE)0x16)
#define ERR_REPEATED_ATTEMPTS                                           ((HCI_ERR_CODE)0x17)
#define ERR_PAIRING_NOT_ALLOWED                                         ((HCI_ERR_CODE)0x18)
#define ERR_UNKNOWN_LMP_PDU                                             ((HCI_ERR_CODE)0x19)
#define ERR_UNSUPPORTED_REMOTE_FEATURE_UNSUPPORTED_LMP_FEATURE          ((HCI_ERR_CODE)0x1A)
#define ERR_SCO_OFFSET_REJECTED                                         ((HCI_ERR_CODE)0x1B)
#define ERR_SCO_INTERVAL_REJECTED                                       ((HCI_ERR_CODE)0x1C)
#define ERR_SCO_AIR_MODE_REJECTED                                       ((HCI_ERR_CODE)0x1D)
#define ERR_INVALID_LMP_PARAMETERS                                      ((HCI_ERR_CODE)0x1E)
#define ERR_UNSPECIFIED_ERROR                                           ((HCI_ERR_CODE)0x1F)
#define ERR_UNSUPPORTED_LMP_PARAMETER_VALUE                             ((HCI_ERR_CODE)0x20)
#define ERR_ROLE_CHANGE_NOT_ALLOWED                                     ((HCI_ERR_CODE)0x21)
#define ERR_LMP_RESPONSE_TIMEOUT                                        ((HCI_ERR_CODE)0x22)
#define ERR_LMP_ERROR_TRANSACTION_COLLISION                             ((HCI_ERR_CODE)0x23)
#define ERR_LMP_PDU_NOT_ALLOWED                                         ((HCI_ERR_CODE)0x24)
#define ERR_ENCRYPTION_MODE_NOT_ACCEPTABLE                              ((HCI_ERR_CODE)0x25)
#define ERR_LINK_KEY_CAN_NOT_BE_CHANGED                                 ((HCI_ERR_CODE)0x26)
#define ERR_REQUESTED_QOS_NOT_SUPPORTED                                 ((HCI_ERR_CODE)0x27)
#define ERR_INSTANT_PASSED                                              ((HCI_ERR_CODE)0x28)
#define ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                         ((HCI_ERR_CODE)0x29)
#define ERR_DIFFERENT_TRANSACTION_COLLISION                             ((HCI_ERR_CODE)0x2A)
#define ERR_QOS_UNACCEPTABLE_PARAMETER                                  ((HCI_ERR_CODE)0x2C)
#define ERR_QOS_REJECTED                                                ((HCI_ERR_CODE)0x2D)
#define ERR_CHANNEL_CLASSIFICATION_NOT_SUPPORTED                        ((HCI_ERR_CODE)0x2E)
#define ERR_INSUFFICIENT_SECURITY                                       ((HCI_ERR_CODE)0x2F)
#define ERR_PARAMETER_OUT_OF_MANDATORY_RANGE                            ((HCI_ERR_CODE)0x30)
#define ERR_ROLE_SWITCH_PENDING                                         ((HCI_ERR_CODE)0x32)
#define ERR_RESERVED_SLOT_VIOLATION                                     ((HCI_ERR_CODE)0x34)
#define ERR_ROLE_SWITCH_FAILED                                          ((HCI_ERR_CODE)0x35)
#define ERR_EXTENDED_INQUIRY_RESPONSE_TOOL_LARGE                        ((HCI_ERR_CODE)0x36)
#define ERR_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST                        ((HCI_ERR_CODE)0x37)
#define ERR_HOST_BUSY_PAIRING                                           ((HCI_ERR_CODE)0x38)
#define ERR_DIRECTED_ADVERTISING_TIMEOUT                                ((HCI_ERR_CODE)0x3C)
#define ERR_CONN_TERMINATED_DUE_TO_MIC_FAILURE                          ((HCI_ERR_CODE)0x3D)
#define ERR_CONNECTION_FAILED_TO_BE_ESTABLISHED                         ((HCI_ERR_CODE)0x3E)

#endif /* _HCI_ERROR_H_ */

