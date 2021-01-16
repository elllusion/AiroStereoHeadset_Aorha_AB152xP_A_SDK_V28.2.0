#ifndef _LC_INITIATING_H_
#define _LC_INITIATING_H_

#define BLE_INITIATING_ENABLE  0x01
#define BLE_INITIATING_DISABLE 0x00

#define ENABLE_INITATING_CHANNEL_37 0x01
#define ENABLE_INITATING_CHANNEL_38 0x02
#define ENABLE_INITATING_CHANNEL_39 0x04

#define INITIATING_TX_ENABLE 0x01
#define INITIATING_TX_DISABLE 0x00



enum initiator_policy
{
    initiator_not_using_white_list,
    initiator_using_white_list
};

#endif



