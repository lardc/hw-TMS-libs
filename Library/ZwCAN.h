// -----------------------------------------
// CAN high-level interface functions
// ----------------------------------------

#ifndef __ZW_CAN_H
#define __ZW_CAN_H

// Include
#include "stdinc.h"
//
// IO defs
#include "_ZwCAN_Defs.h"


// Adjust
//
// CAN 2.0b specification - extended IDs
#define ZW_CAN2b				1
// ECAN mode
#define ZW_CAN_MBOX_COUNT		32


// Constants
//
// General
//
// Parameter for ZwCANx_ConfigMailbox
#define ZW_CAN_RECEIVE_MAILBOX		TRUE
#define ZW_CAN_TRANSMIT_MAILBOX		FALSE
#define ZW_CAN_MSG_LENGTH	    	8
#define ZW_CAN_NO_PRIORITY			0
#define ZW_CAN_STRONG_MATCH			0
//
// Parameter for ZwCANx_GetIntFlag
#define ZW_CAN_LINE0	    	    FALSE
#define ZW_CAN_LINE1		        TRUE
//
// CAN system states
typedef enum __ZwCAN_SysFlags
{
	// Bus-off
	BOIM = 0x01,
	// Error passive
	EPIM = 0x02,
	// Warning level
	WLIM = 0x04,
	// Unknown
	UNKNOWN = 0
} ZwCAN_SysFlags;
//
// Mailbox configuration flags
enum
{
	// Mailbox will be triggered by RTRs
	ZW_CAN_RTRAnswer = 0x01,
	// Overwrite protection mode 
	ZW_CAN_MBProtected = 0x02,
	// Flag for extended ID usage
	ZW_CAN_UseExtendedID = 0x04
};


// Types
//
// Callback function for mailbox
typedef void (*CANMailboxHandler)(Int16U MBox);
// Callback function for system event
typedef void (*CANSysEventHandler)(ZwCAN_SysFlags Flag);
//
// CAN message (64bit)
typedef struct __CANMessage
{
	union
	{
		Int32U DWORD_0;
		struct
		{
			Int16U WORD_1;
			Int16U WORD_0;
		} WORD;
	} HIGH;	
	union
	{
		Int32U DWORD_1;
		struct
		{
			Int16U WORD_3;
			Int16U WORD_2;
		} WORD;
	} LOW;	
	union
	{
		Int32U all;
		struct
		{
			Int16U MsgID_L;
			Int16U MsgID_H;
		} bit;
	} MsgID;
	Int16U DLC;
} CANMessage, *pCANMessage;
//
typedef struct __CANDiagnosticInfo
{
	Int32U Status;
	Int16U TEC;
	Int16U REC;
} CANDiagnosticInfo;

// Functions
//
// Initialize CAN
// 		Parameters:	BRP - Baud-rate prescaler,
//					TSEG1 - length of timing segment 1,			
//					TSEG2 - length of timing segment 2			
void ZwCANa_Init(Int32U BR, Int16U BRP, Int16U TSEG1, Int16U TSEG2, Int16U SJW);
// Configure mailbox
// 		Parameters:	mBox - Mailbox number,
//					MsgID - message ID for mailbox,			
//					Dir - direction (TRUE - receive),
//					DataLen - message length,			
//					Flags - configuration flags (RTRAnswer, MBProtection, ...)			
void ZwCANa_ConfigMailbox(Int16U mBox, Int32U MsgID, Boolean Dir, Int16U DataLen, Int32U Flags, Int16U TransmitPriority, Int32U LAM);
// Set timeout for mailbox
// 		Parameters:	mBox - Mailbox number,
//					TimeoutMS - time-out value in ms,			
void ZwCANa_SetMailboxTimeout(Int16U mBox, Int32U TimeoutMS);
// Enable timeout notification from specified mailbox
// 		Parameters:	mBox - Mailbox number,
void ZwCANa_EnableTimeoutNotification(Int16U mBox);
// Send message via mailbox
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],			
//		Return:		TRUE - OK, FALSE - mailbox is busy by uncompleted preceding transmission
Boolean ZwCANa_SendMessage(Int16U mBox, pCANMessage Data);
// Send message via mailbox
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],
//					AlterMessageID - do alternate message ID
//					AlterMessageLength - do alternate message length
//		Return:		TRUE - OK, FALSE - mailbox is busy by uncompleted preceding transmission
Boolean ZwCANa_SendMessageEx(Int16U mBox, pCANMessage Data, Boolean AlterMessageID, Boolean AlterMessageLength);
// Cancel sending message via mailbox
// 		Parameters:	mBox - Mailbox number,
void ZwCANa_CancelMessage(Int16U mBox);
// Cancel all pending requests to send message
void ZwCANa_CancelAllMessages();
// Write content to mailbox
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],			
void ZwCANa_PrepareMessage(Int16U mBox, pCANMessage Data);
// Write content to mailbox
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],
//					AlterMessageLength - do alternate message length
void ZwCANa_PrepareMessageEx(Int16U mBox, pCANMessage Data, Boolean AlterMessageLength);
// Send prepared message via mailbox
// 		Parameters:	mBox - Mailbox number,
void ZwCANa_SendPreparedMessage(Int16U mBox);
// Write data to mailbox for sending on RTR
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],			
void ZwCANa_WriteRTRAnswer(Int16U mBox, pCANMessage Data);
// Check mailbox for new message
// 		Parameters:	mBox - Mailbox number
Boolean ZwCANa_IsMessageReceived(Int16U mBox, pBoolean pMessageLost);
// Get message from mailbox
// 		Parameters:	mBox - Mailbox number,
//					Buffer - pointer to array Int16U[4]			
void ZwCANa_GetMessage(Int16U mBox, pCANMessage Data);
// Initialize interrupt lines
void ZwCANa_InitInterrupts(Boolean SystemOnly);
// Enable interrupts
void ZwCANa_EnableInterrupts(Boolean SystemOnly);
// Handles system event on specified line
void ZwCANa_DispatchSysEvent();
// Invoke function associated with asserted mailbox
void ZwCANa_DispatchMailEvent();
// Associate data handling function with mailbox
// 		Parameters:	Mailbox - Mailbox number,
//					Function - callback for this mailbox on receive event			
void ZwCANa_RegisterMailboxDataHandler(Int16U Mailbox, CANMailboxHandler Function);
// Associate time-out handling function with mailbox
// 		Parameters:	Mailbox - Mailbox number,
//					Function - callback for this mailbox time-out event			
void ZwCANa_RegisterMailboxTimeoutHandler(Int16U Mailbox, CANMailboxHandler Function);
// Register handler for system events
//					Function - callback functions			
void ZwCANa_RegisterSysEventHandler(CANSysEventHandler Function);
// Retrieve diagnostic info from CAN registers
CANDiagnosticInfo ZwCANa_GetDiagnosticInfo();
// Get time-stamp counter
// 		Parameters:	Void
//		Return:		Current state of CANTSC register
Int32U ZwCANa_GetTimeStamp();

// Initialize CAN
// 		Parameters:	BRP - Baud-rate prescaler,
//					TSEG1 - length of timing segment 1,
//					TSEG2 - length of timing segment 2
void ZwCANb_Init(Int32U BR, Int16U BRP, Int16U TSEG1, Int16U TSEG2, Int16U SJW);
// Configure mailbox
// 		Parameters:	mBox - Mailbox number,
//					MsgID - message ID for mailbox,
//					Dir - direction (TRUE - receive),
//					DataLen - message length,
//					Flags - configuration flags (RTRAnswer, MBProtection, ...)
void ZwCANb_ConfigMailbox(Int16U mBox, Int32U MsgID, Boolean Dir, Int16U DataLen, Int32U Flags, Int16U TransmitPriority, Int32U LAM);
// Set timeout for mailbox
// 		Parameters:	mBox - Mailbox number,
//					TimeoutMS - time-out value in ms,
void ZwCANb_SetMailboxTimeout(Int16U mBox, Int32U TimeoutMS);
// Enable timeout notification from specified mailbox
// 		Parameters:	mBox - Mailbox number,
void ZwCANb_EnableTimeoutNotification(Int16U mBox);
// Send message via mailbox
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],
//		Return:		TRUE - OK, FALSE - mailbox is busy by uncompleted preceding transmission
Boolean ZwCANb_SendMessage(Int16U mBox, pCANMessage Data);
// Send message via mailbox
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],
//					AlterMessageID - do alternate message ID
//					AlterMessageLength - do alternate message length
//		Return:		TRUE - OK, FALSE - mailbox is busy by uncompleted preceding transmission
Boolean ZwCANb_SendMessageEx(Int16U mBox, pCANMessage Data, Boolean AlterMessageID, Boolean AlterMessageLength);
// Cancel sending message via mailbox
// 		Parameters:	mBox - Mailbox number,
void ZwCANb_CancelMessage(Int16U mBox);
// Cancel all pending requests to send message
void ZwCANb_CancelAllMessages();
// Write content to mailbox
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],
void ZwCANb_PrepareMessage(Int16U mBox, pCANMessage Data);
// Write content to mailbox
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],
//					AlterMessageLength - do alternate message length
void ZwCANb_PrepareMessageEx(Int16U mBox, pCANMessage Data, Boolean AlterMessageLength);
// Send prepared message via mailbox
// 		Parameters:	mBox - Mailbox number,
void ZwCANb_SendPreparedMessage(Int16U mBox);
// Write data to mailbox for sending on RTR
// 		Parameters:	mBox - Mailbox number,
//					Data - pointer to an array Int16U[4],
void ZwCANb_WriteRTRAnswer(Int16U mBox, pCANMessage Data);
// Check mailbox for new message
// 		Parameters:	mBox - Mailbox number
Boolean ZwCANb_IsMessageReceived(Int16U mBox, pBoolean pMessageLost);
// Get message from mailbox
// 		Parameters:	mBox - Mailbox number,
//					Buffer - pointer to array Int16U[4]
void ZwCANb_GetMessage(Int16U mBox, pCANMessage Data);
// Initialize interrupt lines
void ZwCANb_InitInterrupts(Boolean SystemOnly);
// Enable interrupts
void ZwCANb_EnableInterrupts(Boolean SystemOnly);
// Handles system event on specified line
void ZwCANb_DispatchSysEvent();
// Invoke function associated with asserted mailbox
void ZwCANb_DispatchMailEvent();
// Associate data handling function with mailbox
// 		Parameters:	Mailbox - Mailbox number,
//					Function - callback for this mailbox on receive event
void ZwCANb_RegisterMailboxDataHandler(Int16U Mailbox, CANMailboxHandler Function);
// Associate time-out handling function with mailbox
// 		Parameters:	Mailbox - Mailbox number,
//					Function - callback for this mailbox time-out event
void ZwCANb_RegisterMailboxTimeoutHandler(Int16U Mailbox, CANMailboxHandler Function);
// Register handler for system events
//					Function - callback functions
void ZwCANb_RegisterSysEventHandler(CANSysEventHandler Function);
// Retrieve diagnostic info from CAN registers
CANDiagnosticInfo ZwCANb_GetDiagnosticInfo();
// Get time-stamp counter
// 		Parameters:	Void
//		Return:		Current state of CANTSC register
Int32U ZwCANb_GetTimeStamp();


// ISR postfix
#define CAN_ISR_DONE    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9


#endif // __ZW_CAN_H
