
/*  SDK_HCI_InquiryCmd: HCI Inquiry Command
 *	<input 1>
 *		U8 XDATA_PTR LapPtr	: This is the LAP from which the inquiry access code should be derived when the inquiry procedure is made;
 *  	value range			: 0x9E8B00¡V0X9E8B3F. This should be big-endian. It should be 0x008B9E - 0x3f8B9E
 *	<input 2>
 *		U8 inquiryLen		: Maximum amount of time specified before the Inquiry is halted.
 *  	value range			: 0x01 ¡V 0x30. Time = inquiryLen * 1.28 sec.
 *  <inut 3>
 *		U8 numResps			: Maximum number of responses from the Inquiry before the Inquiry is halted.
 *  	value range			: 0 = Unlimited number of responses. 0x01 ¡V 0xFF = maximum number of responses.
 * 	<output>
 *		U8					: The result of command. 
 *		value range			: HCI_CMD_OK-->0; HCI_CMD_FAIL-->1; HCI_CMD_BUSY-->2
 *	<event generated>
 * A Command Status event is sent from the Controller to the Host when the Controller
 * has started the Inquiry process. An Inquiry Result event will be created
 * for each Bluetooth device which responds to the Inquiry message. In addition,
 * multiple Bluetooth devices which respond to the Inquire message may be combined
 * into the same event. An Inquiry Complete event is generated when the
 * Inquiry process has completed.
 * Note: No Command Complete event will be sent by the Controller to indicate
 * that this command has been completed. Instead, the Inquiry Complete event
 * will indicate that this command has been completed. No Inquiry Complete
 * event will be generated for the canceled Inquiry process.
 */
PUBLIC U8 SDK_HCI_InquiryCmd(U8 XDATA_PTR LapPtr, U8 inquiryLen, U8 numResps);


/*  SDK_HCI_InquiryCancelCmd: HCI Inquiry Cancel Command
 *	<event generated>
 * This command will cause the Bluetooth device to stop the current Inquiry if the
 * Bluetooth device is in Inquiry Mode. This command allows the Host to interrupt
 * the Bluetooth device and request the Bluetooth device to perform a different
 * task. The command should only be issued after the Inquiry command has been
 * issued, a Command Status event has been received for the Inquiry command,
 * and before the Inquiry Complete event occurs.
 */
#define HCI_CMD_OK		0
#define HCI_CMD_FAIL	1
#define HCI_CMD_BUSY	2
PUBLIC void SDK_HCI_InquiryCancelCmd(void);

/*  SDK_HCI_SwitchRoleCmd: HCI Switch Role Command
 *	<input 1>
 *		U8 linkIndex		: linkIndex which should be switch-roled.;
 *  	value range			: 
 *	<input 2>
 *		U8 role				: Change own Role to Master or SLAVE for this link.
 *  	value range			: SDK_ROLE_MASTER or SDK_ROLE_SLAVE.
 *	<event generated>
 * The Switch_Role command is used for a Bluetooth device to switch the current
 * role the device is performing for a particular connection with another specified
 * Bluetooth device. The Role indicates the requested new role that the local device performs.
 * Note: If there is an SCO connection between the local device and the device
 * identified by the BD_ADDR parameter of link, an attempt to perform a role switch
 * shall be rejected by the local device.
 * Note: If the connection between the local device and the device identified by
 * the BD_ADDR parameter of link is placed in Sniff Mode, an attempt to perform a role
 * switch will be rejected by the local device.
 */
#define SDK_ROLE_MASTER	1
#define SDK_ROLE_SLAVE	2
PUBLIC void SDK_HCI_SwitchRoleCmd(U8 linkIndex, U8 role);

PUBLIC void SDK_HCI_Init(void);