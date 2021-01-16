//////////////////////////////////////////////////////////
// Copyright@ 2009 Airoha.  All right reserved.
//////////////////////////////////////////////////////////

#ifndef _GAP_ROM_INTERFACE_H_
#define _GAP_ROM_INTERFACE_H_

#include "os.h"

#ifdef AIR_UPDATE_ENABLE
#ifdef _GAP_ROM_C_
#define GAP_ROM_EXTERN
#else
#define GAP_ROM_EXTERN extern
#endif

PUBLIC void GAP_ROM(void);
PUBLIC void GAP_ROM_Init(void);
#endif

//HCI related profile command  B
#define GAP_CMD_OPCODE_OFFSET								0x80

//BLE
#ifdef LE_SUPPORTED_HOST
#define GAP_LE_CMD_FROM_PROFILE_SET_ADV_PARAMETER           (0x31 + GAP_CMD_OPCODE_OFFSET)    //177
#define GAP_LE_CMD_FROM_PROFILE_SET_ADV_DATA   	            (0x32 + GAP_CMD_OPCODE_OFFSET)    //178
#define GAP_LE_CMD_FROM_PROFILE_SET_SCAN_RESP_DATA   	    (0x33 + GAP_CMD_OPCODE_OFFSET)    //179
#define GAP_LE_CMD_FROM_PROFILE_ADVERTISING_ENABLE   	    (0x34 + GAP_CMD_OPCODE_OFFSET)    //180
#define GAP_LE_CMD_FROM_PROFILE_ADVERTISING_DISABLE   	    (0x35 + GAP_CMD_OPCODE_OFFSET)    //181
#define GAP_LE_CMD_FROM_PROFILE_SET_RANDOM_ADDRESS   	    (0x36 + GAP_CMD_OPCODE_OFFSET)    //182
#endif

//BLE
#ifdef LE_SUPPORTED_HOST
#define GAP_EVENT_TO_PROFILE_SET_ADV_PARAMETER_COMPLETE                     (0x31 + GAP_CMD_OPCODE_OFFSET)
#define GAP_EVENT_TO_PROFILE_SET_ADV_DATA_COMPLETE   	                    (0x32 + GAP_CMD_OPCODE_OFFSET)
#define GAP_EVENT_TO_PROFILE_SET_SCAN_RESP_DATA_COMPLETE   	                (0x33 + GAP_CMD_OPCODE_OFFSET)
#define GAP_EVENT_TO_PROFILE_ADVERTISING_ENABLE_COMPLETE   	                (0x34 + GAP_CMD_OPCODE_OFFSET)
#define GAP_EVENT_TO_PROFILE_ADVERTISING_DISABLE_COMPLETE   	            (0x35 + GAP_CMD_OPCODE_OFFSET)
#define GAP_EVENT_TO_PROFILE_SET_RANDOM_ADDRESS_COMPLETE   	                (0x36 + GAP_CMD_OPCODE_OFFSET)
#endif

#define GAP_CONNECTION_TO							(60 * 3200L)//60 Sec

//GAP Command State B
#define GAP_STATE_IDLE								0
#define GAP_STATE_HCI_COMMAND   					1

//HCI Command Para B
#define GAP_COMMAND_OP_OFFSET						2
#define GAP_COMMAND_PARA_LEN_OFFSET					4
#define GAP_COMMAND_PARA0_OFFSET					5
#define GAP_COMMAND_PARA1_OFFSET					6
#define GAP_COMMAND_PARA2_OFFSET					7
#define GAP_COMMAND_PARA3_OFFSET					8
#define GAP_COMMAND_PARA4_OFFSET					9
#define GAP_COMMAND_PARA5_OFFSET					10
#define GAP_COMMAND_PARA6_OFFSET					11
#define GAP_COMMAND_PARA7_OFFSET					12
#define GAP_COMMAND_PARA8_OFFSET					13
#define GAP_COMMAND_PARA9_OFFSET					14
#define GAP_COMMAND_PARAA_OFFSET					15
#define GAP_COMMAND_PARAB_OFFSET					16
#define GAP_COMMAND_PARAC_OFFSET					17
#define GAP_COMMAND_PARAD_OFFSET					18



#define GAP_EVENT_CODE_OFFSET						3
#define GAP_EVENT_LEN_OFFSET						4
#define GAP_EVENT_PARA0_OFFSET						5
#define GAP_EVENT_PARA1_OFFSET						6
#define GAP_EVENT_PARA2_OFFSET						7
#define GAP_EVENT_PARA3_OFFSET						8
#define GAP_EVENT_PARA4_OFFSET						9
#define GAP_EVENT_PARA5_OFFSET						10
#define GAP_EVENT_PARA6_OFFSET						11
#define GAP_EVENT_PARA7_OFFSET						12
#define GAP_EVENT_PARA8_OFFSET						13
#define GAP_EVENT_PARA9_OFFSET						14
#define GAP_EVENT_PARAA_OFFSET						15

#define OLD_MQ										0
#define NEW_MQ										1

#define GAP_SUBSTATE01				1
#define GAP_SUBSTATE02				2
#define GAP_SUBSTATE03				3
#define GAP_SUBSTATE04				4
#define GAP_SUBSTATE05				5
#define GAP_SUBSTATE06				6
#define GAP_SUBSTATE07				7
#define GAP_SUBSTATE08				8
#define GAP_SUBSTATE09				9
#define GAP_SUBSTATE10				10
#define GAP_SUBSTATE11				11
#define GAP_SUBSTATE12				12
#define GAP_SUBSTATE13				13
#define GAP_SUBSTATE14				14
#define GAP_SUBSTATE_COMPLETE		15

#define GAP_SUB2STATE01				1
#define GAP_SUB2STATE02				2
#define GAP_SUB2STATE03				3
#define GAP_SUB2STATE04				4
#define GAP_SUB2STATE05				5
#define GAP_SUB2STATE06				6
#define GAP_SUB2STATE07				7
#define GAP_SUB2STATE08				8
#define GAP_SUB2STATE09				9
#define GAP_SUB2STATE10				10
#define GAP_SUB2STATE11				11
#define GAP_SUB2STATE12				12
#define GAP_SUB2STATE13				13
#define GAP_SUB2STATE14				14
#define GAP_SUB2STATE_COMPLETE		15

#define GAP_EVENT_TO_PROFILE  1
#define GAP_EVENT_NOT_PROFILE 0

typedef struct
{
	U8 						top1_state;	//Decide IDLE or HCI
	U8 						top2_state; //Decide Which Command
	U8 						sub_state;  //main State in the Command
	U8						sub2_state;  //sub State in the Command
	U8 						caller_top_state;//Decide Which Command
	U8 						caller_sub_state;
	OST XDATA_PTR 			ost_command_ptr;
	U8						command_err;
	U8						cancel_state;//cancel command issued in which state in main process
	U8						command_op;//save which command op is issued now
	U8						inquiry_total_device;//total inquiry number
	U8						device_number;//the command to which device in inquiry table
//	U8						L2CAP_index;//current index of connection_handle_info table
// Found this is not used	U8						channel_index;//current index of L2CAP channel table
	U8						connection_link_type;//current link type of connection_handle_info table
	U8						hci_flow_control_ready;
	U8						discoverability_mode;
	U8						connectability_mode;
	U8						security_mode;
	U8						pairing_mode;
	U8  XDATA_PTR			command_ptr;
	U8						connection_handle;
	U8						security_mode4_authed;
	U8						encrypt_after_authen_enable;
	U8						simple_pairing_bond_enable;
} GAP_ROM_CONTROL_TYPE;

#ifdef AIR_UPDATE_ENABLE
GAP_ROM_EXTERN	GAP_ROM_CONTROL_TYPE XDATA gGAP_ROM_ctl;
#endif

#endif
