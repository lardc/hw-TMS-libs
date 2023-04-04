// -----------------------------------------
// CAN high-level interface functions
// ----------------------------------------

// Header
#include "ZwCAN.h"

// Include
#include "ZwPIE.h"
#include "ZwGPIO.h"

// Types
//
// Store information about mailbox configuration
typedef struct __CANMailboxCfg
{
	// Handling functions
	CANMailboxHandler Handler, TOHandler;
	// Mailbox direction
	Boolean Direction;
	// Mailbox timeout
	Int32U Timeout;
} CANMailboxCfg;

// Forward functions
//
// Internal initialization of CAN-A module
void ZwCAN_InitInternal(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, Int16U BRP,
						 Int16U TSEG1, Int16U TSEG2, Int16U SJW);
// Configuration of mailbox (CAN-A/B)
void ZwCAN_ConfigMailbox(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes,
						 volatile struct LAM_REGS *ECanxLAMRegs, CANMailboxCfg *CANxHandlers, Int16U mBox,
						 Int32U MsgID, Boolean Dir, Int16U DataLen, Int32U Flags, Int16U TransmitPriority, Int32U LAM);
// Enable timeout signal for mailbox (CAN-A/B)
void ZwCAN_EnableTimeoutNotification(volatile struct ECAN_REGS *ECanxRegs, volatile struct MOTO_REGS *ECanxMOTORegs,
									 CANMailboxCfg *CANxHandlers, Int32U CANxBitRate, Int16U mBox);
// Send message (CAN-A/B)
Boolean ZwCAN_SendMessage(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, Int16U mBox,
					   pCANMessage Data, Boolean PollingMode, Boolean AlterMessageID, Boolean AlterMessageLength);
// Cancel sending message via mailbox
void ZwCAN_CancelMessage(volatile struct ECAN_REGS *ECanxRegs, Int16U mBox);
// Cancel all pending requests to send message
void ZwCAN_CancelAllMessages(volatile struct ECAN_REGS *ECanxRegs);
// Write mailbox content (CAN-A/B)
void ZwCAN_PrepareMessage(volatile struct ECAN_MBOXES *ECanxMboxes, Int16U mBox, pCANMessage Data, Boolean AlterMessageLength);
// Write content to RTR mailbox (CAN-A/B)
void ZwCAN_WriteRTRAnswer(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, Int16U mBox, pCANMessage Data);
// Get message content (CAN-A/B)
void ZwCAN_GetMessage(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, Int16U mBox, pCANMessage Data);
// Init interrupt service (CAN-A/B)
void ZwCAN_InitInterrupts(volatile struct ECAN_REGS *ECanxRegs, Boolean SystemOnly);
// Dispatch event (CAN-A/B)
void ZwCAN_DispatchSysEvent(volatile struct ECAN_REGS *ECanxRegs, CANMailboxCfg *CANxHandlers,
							 CANSysEventHandler CANxSysHandler);
// Dispatch mail (CAN-A/B)
void ZwCAN_DispatchMailEvent(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, CANMailboxCfg *CANxHandlers);
// Get number of mailbox caused the interrupt (CAN-A/B)
Int16U ZwCAN_GetTargetMailbox(volatile struct ECAN_REGS *ECanxRegs, pBoolean pMTOF);
// Get interrupt type for system events (CAN-A/B)
ZwCAN_SysFlags ZwCAN_GetIntFlag(volatile struct ECAN_REGS *ECanxRegs);
// Retrieve diagnostic info from registers (CAN-A/B)
CANDiagnosticInfo ZwCAN_GetDiagnosticInfo(volatile struct ECAN_REGS *ECanxRegs);
// Get time-stamp counter
Int32U ZwCAN_GetTimeStamp(volatile struct ECAN_REGS *ECanxRegs);
// Init GPIO for CAN-A
void ZwCANa_Gpio();
// Init GPIO for CAN-B
void ZwCANb_Gpio();

// Variables
//
// CAN-A mailbox configurations and handlers
static CANMailboxCfg CANaHandlers[ZW_CAN_MBOX_COUNT];
static CANSysEventHandler CANaSysHandler = 0;
static Int32U CANaBitRate = 0;
static Boolean CANaPollingMode = FALSE;
//
// CAN-B mailbox configurations and handlers
static CANMailboxCfg CANbHandlers[ZW_CAN_MBOX_COUNT];
static CANSysEventHandler CANbSysHandler = 0;
static Int32U CANbBitRate = 0;
static Boolean CANbPollingMode = FALSE;


// Functions
//
void ZwCANa_Init(Int32U BR, Int16U BRP, Int16U TSEG1, Int16U TSEG2, Int16U SJW)
{
    Int16U i;
    CANMailboxCfg zero = {0, 0, 0, 0};

    // Init GPIO for functions
	ZwCANa_Gpio();
    // Init module
    ZwCAN_InitInternal(&ECanaRegs, &ECanaMboxes, BRP, TSEG1, TSEG2, SJW);
    
    // Save bit rate
    CANaBitRate = BR;
    
	// Disable all handlers
    for(i = 0; i < ZW_CAN_MBOX_COUNT; i++)
    	CANaHandlers[i] = zero;
}
// ----------------------------------------

void ZwCANa_ConfigMailbox(Int16U mBox, Int32U MsgID, Boolean Dir, Int16U DataLen, Int32U Flags, Int16U TransmitPriority, Int32U LAM)
{
	ZwCAN_ConfigMailbox(&ECanaRegs, &ECanaMboxes, &ECanaLAMRegs, CANaHandlers, mBox, MsgID, Dir, DataLen, Flags, TransmitPriority, LAM);
}
// ----------------------------------------

void ZwCANa_SetMailboxTimeout(Int16U mBox, Int32U TimeoutMS)
{
	CANaHandlers[mBox].Timeout = TimeoutMS;
}
// ----------------------------------------

void ZwCANa_EnableTimeoutNotification(Int16U mBox)
{
	ZwCAN_EnableTimeoutNotification(&ECanaRegs, &ECanaMOTORegs, CANaHandlers, CANaBitRate, mBox); 		
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_SendMessage, "ramfuncs");
#endif
Boolean ZwCANa_SendMessage(Int16U mBox, pCANMessage Data)
{
	return ZwCAN_SendMessage(&ECanaRegs, &ECanaMboxes, mBox, Data, CANaPollingMode, FALSE, FALSE);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_SendMessageEx, "ramfuncs");
#endif
Boolean ZwCANa_SendMessageEx(Int16U mBox, pCANMessage Data, Boolean AlterMessageID, Boolean AlterMessageLength)
{
	return ZwCAN_SendMessage(&ECanaRegs, &ECanaMboxes, mBox, Data, CANaPollingMode, AlterMessageID, AlterMessageLength);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_CancelMessage, "ramfuncs");
#endif
void ZwCANa_CancelMessage(Int16U mBox)
{
	ZwCAN_CancelMessage(&ECanaRegs, mBox);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_CancelAllMessages, "ramfuncs");
#endif
void ZwCANa_CancelAllMessages()
{
	ZwCAN_CancelAllMessages(&ECanaRegs);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_PrepareMessage, "ramfuncs");
#endif
void ZwCANa_PrepareMessage(Int16U mBox, pCANMessage Data)
{
	ZwCAN_PrepareMessage(&ECanaMboxes, mBox, Data, 0xFF);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_PrepareMessageEx, "ramfuncs");
#endif
void ZwCANa_PrepareMessageEx(Int16U mBox, pCANMessage Data, Boolean AlterMessageLength)
{
	ZwCAN_PrepareMessage(&ECanaMboxes, mBox, Data, AlterMessageLength);
}
// ----------------------------------------

// Send prepared message via mailbox
#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_SendPreparedMessage, "ramfuncs");
#endif
void ZwCANa_SendPreparedMessage(Int16U mBox)
{
   Int32U RegShadow;

	// operational bitmask: calculate bitmask based on mBox number
   Int32U bitMask = (0x01ul << mBox);
	
   // Set TRS bit
   RegShadow = ECanaRegs.CANTRS.all;
   RegShadow |= bitMask;	// one-flag OR-mask
   ECanaRegs.CANTRS.all = RegShadow;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_WriteRTRAnswer, "ramfuncs");
#endif
void ZwCANa_WriteRTRAnswer(Int16U mBox, pCANMessage Data)
{
	ZwCAN_WriteRTRAnswer(&ECanaRegs, &ECanaMboxes, mBox, Data);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_IsMessageReceived, "ramfuncs");
#endif
Boolean ZwCANa_IsMessageReceived(Int16U mBox, pBoolean pMessageLost)
{
   Int32U RegShadow;

   // operational bitmask: calculate bitmask based on mBox number
   Int32U bitMask = (0x01ul << mBox);

   RegShadow = ECanaRegs.CANRML.all;
   if(pMessageLost)
	   *pMessageLost = (RegShadow & bitMask) ? TRUE : FALSE;

   RegShadow = ECanaRegs.CANRMP.all;
   return (RegShadow & bitMask) ? TRUE : FALSE;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_GetMessage, "ramfuncs");
#endif
void ZwCANa_GetMessage(Int16U mBox, pCANMessage Data)
{
	ZwCAN_GetMessage(&ECanaRegs, &ECanaMboxes, mBox, Data);
}
// ----------------------------------------

void ZwCANa_InitInterrupts(Boolean SystemOnly)
{
	CANaPollingMode = SystemOnly;
	ZwCAN_InitInterrupts(&ECanaRegs, SystemOnly);
}
// ----------------------------------------

void ZwCANa_EnableInterrupts(Boolean SystemOnly)
{
   // Enable CPU INT9 which is connected to CAN-A:
   IER |= M_INT9;

   // Enable Line 0 and 1 in the PIE: Group 9 interrupts 5 and 6
   PieCtrlRegs.PIEIER9.bit.INTx5 = 1;
   if(!SystemOnly)
	   PieCtrlRegs.PIEIER9.bit.INTx6 = 1;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwCANa_DispatchSysEvent, "ramfuncs");
#endif
void ZwCANa_DispatchSysEvent()
{
	ZwCAN_DispatchSysEvent(&ECanaRegs, CANaHandlers, CANaSysHandler);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_DispatchMailEvent, "ramfuncs");
#endif
void ZwCANa_DispatchMailEvent()
{
	ZwCAN_DispatchMailEvent(&ECanaRegs, &ECanaMboxes, CANaHandlers);
}
// ----------------------------------------

void ZwCANa_RegisterMailboxDataHandler(Int16U Mailbox, CANMailboxHandler Function)
{
	// Check valid address
	if(Mailbox < ZW_CAN_MBOX_COUNT)
		CANaHandlers[Mailbox].Handler = Function;
}
// ----------------------------------------

void ZwCANa_RegisterMailboxTimeoutHandler(Int16U Mailbox, CANMailboxHandler Function)
{
	// Check valid address
	if(Mailbox < ZW_CAN_MBOX_COUNT)
		CANaHandlers[Mailbox].TOHandler = Function;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANa_GetTimeStamp, "ramfuncs");
#endif
Int32U ZwCANa_GetTimeStamp()
{
	return ZwCAN_GetTimeStamp(&ECanaRegs);
}
// ----------------------------------------

void ZwCANa_RegisterSysEventHandler(CANSysEventHandler Function)
{
	CANaSysHandler = Function;
}
// ----------------------------------------

CANDiagnosticInfo ZwCANa_GetDiagnosticInfo()
{
	return ZwCAN_GetDiagnosticInfo(&ECanaRegs);
}
// ----------------------------------------

// Internal implementation
// -------------------------------------------------

void ZwCAN_InitInternal(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, Int16U BRP, Int16U TSEG1, Int16U TSEG2, Int16U SJW)
{
	/* Create a shadow register structure for the CAN control registers. This is
	 needed, since only 32-bit access is allowed to these registers. 16-bit access
	 to these registers could potentially corrupt the register contents or return
	 false data. This is especially true while writing to/reading from a bit
	 (or group of bits) among bits 16 - 31 */
	struct ECAN_REGS ECanShadow; 

	EALLOW;		// EALLOW enables access to protected bits

	/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/

    ECanShadow.CANTIOC.all = ECanxRegs->CANTIOC.all;
    ECanShadow.CANTIOC.bit.TXFUNC = 1;
    ECanxRegs->CANTIOC.all = ECanShadow.CANTIOC.all;

    ECanShadow.CANRIOC.all = ECanxRegs->CANRIOC.all;
    ECanShadow.CANRIOC.bit.RXFUNC = 1;
    ECanxRegs->CANRIOC.all = ECanShadow.CANRIOC.all;

	/* Initialize all bits of 'Message Control Register' to zero */
	// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
	// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanxMboxes->MBOX0.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX1.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX2.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX3.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX4.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX5.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX6.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX7.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX8.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX9.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX10.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX11.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX12.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX13.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX14.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX15.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX16.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX17.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX18.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX19.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX20.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX21.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX22.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX23.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX24.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX25.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX26.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX27.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX28.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX29.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX30.MSGCTRL.all = 0x00000000;
    ECanxMboxes->MBOX31.MSGCTRL.all = 0x00000000;

    // TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
    // as a matter of precaution.

	ECanxRegs->CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */

	ECanxRegs->CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */

	ECanxRegs->CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */
	ECanxRegs->CANGIF1.all = 0xFFFFFFFF;

    // Configure bit timing parameters for eCANA
    //
	ECanShadow.CANMC.all = ECanxRegs->CANMC.all;
	ECanShadow.CANMC.bit.CCR = 1;            // Set CCR = 1
    ECanxRegs->CANMC.all = ECanShadow.CANMC.all;

    // Wait until the CPU has been granted permission to change the configuration registers
    do
    {
    	ECanShadow.CANES.all = ECanxRegs->CANES.all;
    } while(ECanShadow.CANES.bit.CCE != 1);  		// Wait for CCE bit to be set..

		ECanShadow.CANMC.all = ECanxRegs->CANMC.all;
	    // Configure eCAN for HECC mode - (required to access mailboxes 16 through 31)
		ECanShadow.CANMC.bit.SCB = 1;
		// Free run during emulator halt
		ECanShadow.CANMC.bit.SUSP = 1;
		// Auto bus-on
		ECanShadow.CANMC.bit.ABO = 1;
		ECanxRegs->CANMC.all = ECanShadow.CANMC.all;
	
	  	// SET BAUDRATE
	   	ECanShadow.CANBTC.all = 0;
		//
		ECanShadow.CANBTC.bit.BRPREG = BRP;
	   	ECanShadow.CANBTC.bit.TSEG1REG = TSEG1;
	  	ECanShadow.CANBTC.bit.TSEG2REG = TSEG2;
	   	ECanShadow.CANBTC.bit.SJWREG = SJW;
		//
	    ECanShadow.CANBTC.bit.SAM = 1;
	    ECanxRegs->CANBTC.all = ECanShadow.CANBTC.all;
	
		ECanShadow.CANMC.all = ECanxRegs->CANMC.all;
		ECanShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
		ECanxRegs->CANMC.all = ECanShadow.CANMC.all;

    // Wait until the CPU no longer has permission to change the configuration registers
    do
    {
      ECanShadow.CANES.all = ECanxRegs->CANES.all;
    } while(ECanShadow.CANES.bit.CCE != 0 ); 		// Wait for CCE bit to be  cleared..

    // Disable all Mailboxes  
 	ECanxRegs->CANME.all = 0;		// Required before writing the MSGIDs

    EDIS; 
}
// ----------------------------------------

void ZwCAN_ConfigMailbox(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes,
						 volatile struct LAM_REGS *ECanxLAMRegs, CANMailboxCfg *CANxHandlers, Int16U mBox,
						 Int32U MsgID, Boolean Dir, Int16U DataLen, Int32U Flags, Int16U TransmitPriority, Int32U LAM)
{
	Int32U RegShadow;

	// access pointer
    volatile struct MBOX *pMBOX;
    volatile union CANLAM_REG *pLAM;
    // Operational bitmask: calculate bitmask based on mBox number
    Int32U bitMask = (0x01ul << mBox);

    // Set overwrite protection
    if(Flags & ZW_CAN_MBProtected)
    {
    	RegShadow = ECanxRegs->CANOPC.all;
    	RegShadow |= bitMask;
    	ECanxRegs->CANOPC.all = RegShadow;
    }

    // Disable mailbox for writing MSGID
    RegShadow = ECanxRegs->CANME.all;
    RegShadow &= ~bitMask; 	// one-zero AND-mask
    ECanxRegs->CANME.all = RegShadow;

    // calculate the location of specified MBOX
    pMBOX = &ECanxMboxes->MBOX0;
    pMBOX += mBox;
    // Config using MsgID
    pMBOX->MSGID.all = MsgID;
    // Set IDE bit: CAN 2.0b standard
    pMBOX->MSGID.bit.IDE = (Flags & ZW_CAN_UseExtendedID) ? 1 : 0;
    // Enable filtering
    pMBOX->MSGID.bit.AME = (LAM == ZW_CAN_STRONG_MATCH) ? 0 : 1;
    // If mailbox will be RTR-triggered - set AAM bit (auto-answer)
    pMBOX->MSGID.bit.AAM = (Flags & ZW_CAN_RTRAnswer) ? 1 : 0;

    // set direction bit
    RegShadow = ECanxRegs->CANMD.all;
    if(Dir)
	    RegShadow |= bitMask;  	// one-flag OR-mask
    else
    	RegShadow &= ~bitMask; 	// one-zero AND-mask
    ECanxRegs->CANMD.all = RegShadow;

    // Save configuration for interrupt handling
    CANxHandlers[mBox].Direction = Dir;

    // Set message length
    pMBOX->MSGCTRL.bit.DLC = DataLen;
    // set transmit priority
    if(!Dir)
    	pMBOX->MSGCTRL.bit.TPL = TransmitPriority;

    // Calculate location of specified LAM
    pLAM = &ECanxLAMRegs->LAM0;
    pLAM += mBox;
   
    // Set LAM (LAMI = 1 - standard & extended frames)
    LAM |= BIT31;
    pLAM->all = LAM;

    // Enable mailbox
    RegShadow = ECanxRegs->CANME.all;
    RegShadow |= bitMask;		// one-flag OR-mask
    ECanxRegs->CANME.all = RegShadow;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_GetTimeStamp, "ramfuncs");
#endif
Int32U ZwCAN_GetTimeStamp(volatile struct ECAN_REGS *ECanxRegs)
{
	return ECanxRegs->CANTSC;
}
// ----------------------------------------

void ZwCAN_EnableTimeoutNotification(volatile struct ECAN_REGS *ECanxRegs, volatile struct MOTO_REGS *ECanxMOTORegs, CANMailboxCfg *CANxHandlers, Int32U CANxBitRate, Int16U mBox)
{
	Int32U RegShadow;

	// access pointer
    volatile Int32U *pMOTO;
    // operational bitmask: calculate bitmask based on mBox number
    Int32U bitMask = (0x01ul << mBox);

    // Validate timeout value
    if(CANxHandlers[mBox].Timeout)
    {
    	// calculate the location of specified MOTO
	    pMOTO = &ECanxMOTORegs->MOTO0;
	    pMOTO += mBox;
	    *pMOTO = ECanxRegs->CANTSC + CANxBitRate / 1000 * CANxHandlers[mBox].Timeout;
	
	    // Enable timeout
	    RegShadow = ECanxRegs->CANTOC.all;
	    RegShadow |= bitMask;
	    ECanxRegs->CANTOC.all = RegShadow;
   }
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_SendMessage, "ramfuncs");
#endif
Boolean ZwCAN_SendMessage(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, Int16U mBox,
					   pCANMessage Data, Boolean PollingMode, Boolean AlterMessageID, Boolean AlterMessageLength)
{
	Int32U RegShadow;

	// Access pointer
    volatile struct MBOX *pMBOX;
    // Operational bitmask: calculate bitmask based on mBox number
    Int32U bitMask = (0x01ul << mBox);

    // Check for completion of the last transmission
    RegShadow = ECanxRegs->CANTRS.all;
    if(RegShadow & bitMask)
    	return FALSE;

    // Calculate the location of specified MBOX
    pMBOX = &ECanxMboxes->MBOX0;
    pMBOX += mBox;

    // Process TA and AA flags
    if(PollingMode)
    {
    	// Clear TA flag in the case of successful last transmission
    	RegShadow = ECanxRegs->CANTA.all;
    	while (RegShadow & bitMask)
	    {
        	ECanxRegs->CANTA.all = bitMask;
        	RegShadow = ECanxRegs->CANTA.all;
	    }

	    // Clear AA flag in the case of aborted last transmission
    	RegShadow = ECanxRegs->CANAA.all;
    	while (RegShadow & bitMask)
	    {
        	ECanxRegs->CANAA.all = bitMask;
        	RegShadow = ECanxRegs->CANAA.all;
	    }
    }

    // Writing data
    pMBOX->MDL.word.HI_WORD = Data->HIGH.WORD.WORD_0;
    pMBOX->MDL.word.LOW_WORD = Data->HIGH.WORD.WORD_1;
    pMBOX->MDH.word.HI_WORD = Data->LOW.WORD.WORD_2;
    pMBOX->MDH.word.LOW_WORD = Data->LOW.WORD.WORD_3;

    // Update Message ID
    if(AlterMessageID)
    {
    	RegShadow = ECanxRegs->CANME.all;
    	RegShadow &= ~bitMask; // one-flag AND-mask
    	ECanxRegs->CANME.all = RegShadow;

 	    pMBOX->MSGID.all = Data->MsgID.all | (ZW_CAN2b ? BIT31 : 0);

    	RegShadow = ECanxRegs->CANME.all;
    	RegShadow |= bitMask; // one-flag OR-mask
    	ECanxRegs->CANME.all = RegShadow;
    }
   
    // Write message length if needed
    if(AlterMessageLength)
    	pMBOX->MSGCTRL.bit.DLC = Data->DLC;

    // Set TRS bit
    RegShadow = ECanxRegs->CANTRS.all;
    RegShadow |= bitMask;	// one-flag OR-mask
    ECanxRegs->CANTRS.all = RegShadow;

    return TRUE;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_CancelMessage, "ramfuncs");
#endif
void ZwCAN_CancelMessage(volatile struct ECAN_REGS *ECanxRegs, Int16U mBox)
{
	// Operational bitmask: calculate bitmask based on mBox number
    Int32U bitMask = (0x01ul << mBox);

    ECanxRegs->CANTRR.all = bitMask;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_CancelAllMessages, "ramfuncs");
#endif
void ZwCAN_CancelAllMessages(volatile struct ECAN_REGS *ECanxRegs)
{
	Int32U RegShadow;

	RegShadow = ECanxRegs->CANTRS.all;
	ECanxRegs->CANTRR.all = RegShadow;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_PrepareMessage, "ramfuncs");
#endif
void ZwCAN_PrepareMessage(volatile struct ECAN_MBOXES *ECanxMboxes, Int16U mBox, pCANMessage Data, Boolean AlterMessageLength)
{
	// Access pointer
    volatile struct MBOX *pMBOX;

    // Calculate the location of specified MBOX
    pMBOX = &ECanxMboxes->MBOX0;
    pMBOX += mBox;

    // Writing data
    pMBOX->MDL.word.HI_WORD = Data->HIGH.WORD.WORD_0;
    pMBOX->MDL.word.LOW_WORD = Data->HIGH.WORD.WORD_1;
    pMBOX->MDH.word.HI_WORD = Data->LOW.WORD.WORD_2;
    pMBOX->MDH.word.LOW_WORD = Data->LOW.WORD.WORD_3;

    // Write message length if needed
    if(AlterMessageLength)
    	pMBOX->MSGCTRL.bit.DLC = Data->DLC;
}
// ----------------------------------------

// Write data to mailbox for sending on RTR
#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_WriteRTRAnswer, "ramfuncs");
#endif
void ZwCAN_WriteRTRAnswer(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, Int16U mBox, pCANMessage Data)
{
	struct ECAN_REGS ECanShadow; 
    
    // Access pointer
    volatile struct MBOX *pMBOX;

	// Set "Data update request" flag for specified mailbox
	ECanShadow.CANMC.all = ECanxRegs->CANMC.all;
	ECanShadow.CANMC.bit.CDR = 0x01;
	ECanShadow.CANMC.bit.MBNR = mBox;
	ECanxRegs->CANMC.all = ECanShadow.CANMC.all;
	
	// Calculate the location of specified MBOX
    pMBOX = &ECanxMboxes->MBOX0;
    pMBOX += mBox;

    // Writing data
    pMBOX->MDL.word.HI_WORD = Data->HIGH.WORD.WORD_0;
    pMBOX->MDL.word.LOW_WORD = Data->HIGH.WORD.WORD_1;
    pMBOX->MDH.word.HI_WORD = Data->LOW.WORD.WORD_2;
    pMBOX->MDH.word.LOW_WORD = Data->LOW.WORD.WORD_3;

	// Clear "Data update request" flag for specified mailbox
	ECanShadow.CANMC.all = ECanxRegs->CANMC.all;
	ECanShadow.CANMC.bit.CDR = 0x00;
	ECanxRegs->CANMC.all = ECanShadow.CANMC.all;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_GetMessage, "ramfuncs");
#endif
void ZwCAN_GetMessage(volatile struct ECAN_REGS *ECanxRegs, volatile struct ECAN_MBOXES *ECanxMboxes, Int16U mBox, pCANMessage Data)
{
	Int32U RegShadow;

	// Access pointer
    volatile struct MBOX *pMBOX;
    // Operational bitmask: calculate bitmask based on mBox number
    Int32U bitMask = (0x01ul << mBox);
 
    // calculate the location of specified MBOX
    pMBOX = &ECanxMboxes->MBOX0;
    pMBOX += mBox;
   
    // While we read dirty message - repeat
    do
    {
    	// Clear receive flag
   		ECanxRegs->CANRMP.all = bitMask;

		// Read message content
        Data->HIGH.WORD.WORD_0 = pMBOX->MDL.word.HI_WORD;
	    Data->HIGH.WORD.WORD_1 = pMBOX->MDL.word.LOW_WORD;
	    Data->LOW.WORD.WORD_2 = pMBOX->MDH.word.HI_WORD;
	    Data->LOW.WORD.WORD_3 = pMBOX->MDH.word.LOW_WORD;

	    // Read Message ID
	    Data->MsgID.all = pMBOX->MSGID.all & 0x1FFFFFFF;
	    Data->DLC = pMBOX->MSGCTRL.bit.DLC;
			
	    // If message was overwritten while reading - repeat
	    RegShadow = ECanxRegs->CANRMP.all;
    }
    while(RegShadow & bitMask);
}
// ----------------------------------------

void ZwCAN_InitInterrupts(volatile struct ECAN_REGS *ECanxRegs, Boolean SystemOnly)
{
	struct ECAN_REGS ECanShadow; 

	EALLOW;

	if(!SystemOnly)
	{
		// Enable mailbox interrupts
		ECanxRegs->CANMIL.all = 0xFFFFFFFF; // set line 1 for all boxes
		ECanxRegs->CANMIM.all = 0xFFFFFFFF; // enable interrupts on all boxes
	}

    // Enable system interrupts
	//
    ECanShadow.CANGIM.all = ECanxRegs->CANGIM.all;
    // set line 0 for handling global interrupts
    ECanShadow.CANGIM.bit.GIL = 0;
    // set enable flags
	ECanShadow.CANGIM.bit.BOIM = 1;
	ECanShadow.CANGIM.bit.EPIM = 1;
	ECanShadow.CANGIM.bit.WLIM = 1;
	// enable system line
	ECanShadow.CANGIM.bit.I0EN = 1;
	// enable mailbox line
	if(!SystemOnly)
		ECanShadow.CANGIM.bit.I1EN = 1;
	ECanxRegs->CANGIM.all = ECanShadow.CANGIM.all;

	EDIS;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_GetTargetMailbox, "ramfuncs");
#endif
Int16U ZwCAN_GetTargetMailbox(volatile struct ECAN_REGS *ECanxRegs, pBoolean pMTOF)
{
	struct ECAN_REGS ECanShadow;

	ECanShadow.CANGIF0.all = ECanxRegs->CANGIF0.all;
	*pMTOF = ECanShadow.CANGIF0.bit.MTOF0 ? TRUE : FALSE;

	return ECanShadow.CANGIF0.bit.MIV0;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwCAN_GetIntFlag, "ramfuncs");
#endif
ZwCAN_SysFlags ZwCAN_GetIntFlag(volatile struct ECAN_REGS *ECanxRegs)
{
	struct ECAN_REGS ECanShadow; 
	
	ZwCAN_SysFlags result = UNKNOWN;

	ECanShadow.CANGIF0.all = ECanxRegs->CANGIF0.all;

	if(ECanShadow.CANGIF0.bit.BOIF0)
		result |= BOIM;
	if(ECanShadow.CANGIF0.bit.EPIF0)
		result |= EPIM;
	if(ECanShadow.CANGIF0.bit.WLIF0)
		result |= WLIM;

	return result;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwCAN_DispatchSysEvent, "ramfuncs");
#endif
void ZwCAN_DispatchSysEvent(volatile struct ECAN_REGS *ECanxRegs, CANMailboxCfg *CANxHandlers, CANSysEventHandler CANxSysHandler)
{
	Int32U RegShadow;

	// Get system state
	ZwCAN_SysFlags flag = ZwCAN_GetIntFlag(ECanxRegs);
	
	if(CANxSysHandler)
		CANxSysHandler(flag);

	RegShadow = ECanxRegs->CANGIF0.all;
	ECanxRegs->CANGIF0.all = RegShadow;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_DispatchMailEvent, "ramfuncs");
#endif
void ZwCAN_DispatchMailEvent(volatile struct ECAN_REGS *ECanxRegs,  volatile struct ECAN_MBOXES *ECanxMboxes, CANMailboxCfg *CANxHandlers)
{
	Boolean MTOF;

	Int16S target_box = ZwCAN_GetTargetMailbox(ECanxRegs, &MTOF);

	// Choice between MTOF and GMIF
	if(!MTOF)
	{
		// Check mailbox direction
		if(CANxHandlers[target_box].Direction)
		{
			CANMessage dummy;

			// Call handler if it was set
			if(CANxHandlers[target_box].Handler)
				(CANxHandlers[target_box].Handler)(target_box);
			else
				ZwCAN_GetMessage(ECanxRegs, ECanxMboxes, target_box, &dummy);
		}
		else
		{
			Int32U bitMask;

			// Call handler if it was set
			if(CANxHandlers[target_box].Handler)
				(CANxHandlers[target_box].Handler)(target_box);

			// Operational bitmask: calculate bitmask based on mBox number
			bitMask = (0x01ul << target_box);

			// Clear TA bit for mailbox that has just transmitted message
			ECanxRegs->CANTA.all = bitMask;
		}
	}
	else
	{
		Int32U bitMask;

		// Operational bitmask: calculate bitmask based on mBox number
		bitMask = (0x01ul << target_box);

		ECanxRegs->CANTOS.all = bitMask;
	}
}
// ----------------------------------------

CANDiagnosticInfo ZwCAN_GetDiagnosticInfo(volatile struct ECAN_REGS *ECanxRegs)
{
	struct ECAN_REGS ECanShadow;

	CANDiagnosticInfo result;

	result.Status = ECanxRegs->CANES.all;
	ECanShadow.CANTEC.all = ECanxRegs->CANTEC.all;
	ECanShadow.CANREC.all = ECanxRegs->CANREC.all;
	result.TEC = ECanShadow.CANTEC.bit.TEC;
	result.REC = ECanShadow.CANREC.bit.REC;

	return result;
}
// ----------------------------------------

void ZwCANa_Gpio(void)
{
	EALLOW;

	GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;	    // Enable pull-up for GPIO30 (CANRXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynchronous qualification for GPIO30 (CANRXA)

	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// Configure GPIO30 for CANTXA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// Configure GPIO31 for CANRXA operation

    EDIS;
}
// ----------------------------------------

void ZwCANb_Init(Int32U BR, Int16U BRP, Int16U TSEG1, Int16U TSEG2, Int16U SJW)
{
    Int16U i;
    CANMailboxCfg zero = {0, 0, 0, 0};

    // Init GPIO for functions
	ZwCANb_Gpio();
    // Init module
    ZwCAN_InitInternal(&ECanbRegs, &ECanbMboxes, BRP, TSEG1, TSEG2, SJW);

    // Save bit rate
    CANbBitRate = BR;

	// Disable all handlers
    for(i = 0; i < ZW_CAN_MBOX_COUNT; i++)
    	CANbHandlers[i] = zero;
}
// ----------------------------------------

void ZwCANb_ConfigMailbox(Int16U mBox, Int32U MsgID, Boolean Dir, Int16U DataLen, Int32U Flags, Int16U TransmitPriority, Int32U LAM)
{
	ZwCAN_ConfigMailbox(&ECanbRegs, &ECanbMboxes, &ECanbLAMRegs, CANbHandlers, mBox, MsgID, Dir, DataLen, Flags, TransmitPriority, LAM);
}
// ----------------------------------------

void ZwCANb_SetMailboxTimeout(Int16U mBox, Int32U TimeoutMS)
{
	CANbHandlers[mBox].Timeout = TimeoutMS;
}
// ----------------------------------------

void ZwCANb_EnableTimeoutNotification(Int16U mBox)
{
	ZwCAN_EnableTimeoutNotification(&ECanbRegs, &ECanbMOTORegs, CANbHandlers, CANbBitRate, mBox);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_SendMessage, "ramfuncs");
#endif
Boolean ZwCANb_SendMessage(Int16U mBox, pCANMessage Data)
{
	return ZwCAN_SendMessage(&ECanbRegs, &ECanbMboxes, mBox, Data, CANbPollingMode, FALSE, FALSE);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_SendMessageEx, "ramfuncs");
#endif
Boolean ZwCANb_SendMessageEx(Int16U mBox, pCANMessage Data, Boolean AlterMessageID, Boolean AlterMessageLength)
{
	return ZwCAN_SendMessage(&ECanbRegs, &ECanbMboxes, mBox, Data, CANbPollingMode, AlterMessageID, AlterMessageLength);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_CancelMessage, "ramfuncs");
#endif
void ZwCANb_CancelMessage(Int16U mBox)
{
	ZwCAN_CancelMessage(&ECanbRegs, mBox);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCAN_CancelAllMessages, "ramfuncs");
#endif
void ZwCANb_CancelAllMessages()
{
	ZwCAN_CancelAllMessages(&ECanbRegs);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_PrepareMessage, "ramfuncs");
#endif
void ZwCANb_PrepareMessage(Int16U mBox, pCANMessage Data)
{
	ZwCAN_PrepareMessage(&ECanbMboxes, mBox, Data, 0xFF);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_PrepareMessageEx, "ramfuncs");
#endif
void ZwCANb_PrepareMessageEx(Int16U mBox, pCANMessage Data, Boolean AlterMessageLength)
{
	ZwCAN_PrepareMessage(&ECanbMboxes, mBox, Data, AlterMessageLength);
}
// ----------------------------------------

// Send prepared message via mailbox
#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_SendPreparedMessage, "ramfuncs");
#endif
void ZwCANb_SendPreparedMessage(Int16U mBox)
{
   Int32U RegShadow;

	// operational bitmask: calculate bitmask based on mBox number
   Int32U bitMask = (0x01ul << mBox);

   // Set TRS bit
   RegShadow = ECanbRegs.CANTRS.all;
   RegShadow |= bitMask;	// one-flag OR-mask
   ECanbRegs.CANTRS.all = RegShadow;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_WriteRTRAnswer, "ramfuncs");
#endif
void ZwCANb_WriteRTRAnswer(Int16U mBox, pCANMessage Data)
{
	ZwCAN_WriteRTRAnswer(&ECanbRegs, &ECanbMboxes, mBox, Data);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_IsMessageReceived, "ramfuncs");
#endif
Boolean ZwCANb_IsMessageReceived(Int16U mBox, pBoolean pMessageLost)
{
   Int32U RegShadow;

   // operational bitmask: calculate bitmask based on mBox number
   Int32U bitMask = (0x01ul << mBox);

   RegShadow = ECanbRegs.CANRML.all;
   if(pMessageLost)
	   *pMessageLost = (RegShadow & bitMask) ? TRUE : FALSE;

   RegShadow = ECanbRegs.CANRMP.all;
   return (RegShadow & bitMask) ? TRUE : FALSE;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_GetMessage, "ramfuncs");
#endif
void ZwCANb_GetMessage(Int16U mBox, pCANMessage Data)
{
	ZwCAN_GetMessage(&ECanbRegs, &ECanbMboxes, mBox, Data);
}
// ----------------------------------------

void ZwCANb_InitInterrupts(Boolean SystemOnly)
{
	CANbPollingMode = SystemOnly;
	ZwCAN_InitInterrupts(&ECanbRegs, SystemOnly);
}
// ----------------------------------------

void ZwCANb_EnableInterrupts(Boolean SystemOnly)
{
   // Enable CPU INT9 which is connected to CAN-B:
   IER |= M_INT9;

   // Enable Line 0 and 1 in the PIE: Group 9 interrupts 7 and 8
   PieCtrlRegs.PIEIER9.bit.INTx7 = 1;
   if(!SystemOnly)
	   PieCtrlRegs.PIEIER9.bit.INTx8 = 1;
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwCANb_DispatchSysEvent, "ramfuncs");
#endif
void ZwCANb_DispatchSysEvent()
{
	ZwCAN_DispatchSysEvent(&ECanbRegs, CANbHandlers, CANbSysHandler);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_DispatchMailEvent, "ramfuncs");
#endif
void ZwCANb_DispatchMailEvent()
{
	ZwCAN_DispatchMailEvent(&ECanbRegs, &ECanbMboxes, CANbHandlers);
}
// ----------------------------------------

void ZwCANb_RegisterMailboxDataHandler(Int16U Mailbox, CANMailboxHandler Function)
{
	// Check valid address
	if(Mailbox < ZW_CAN_MBOX_COUNT)
		CANbHandlers[Mailbox].Handler = Function;
}
// ----------------------------------------

void ZwCANb_RegisterMailboxTimeoutHandler(Int16U Mailbox, CANMailboxHandler Function)
{
	// Check valid address
	if(Mailbox < ZW_CAN_MBOX_COUNT)
		CANbHandlers[Mailbox].TOHandler = Function;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_CAN)
	#pragma CODE_SECTION(ZwCANb_GetTimeStamp, "ramfuncs");
#endif
Int32U ZwCANb_GetTimeStamp()
{
	return ZwCAN_GetTimeStamp(&ECanbRegs);
}
// ----------------------------------------

void ZwCANb_RegisterSysEventHandler(CANSysEventHandler Function)
{
	CANbSysHandler = Function;
}
// ----------------------------------------

CANDiagnosticInfo ZwCANb_GetDiagnosticInfo()
{
	return ZwCAN_GetDiagnosticInfo(&ECanbRegs);
}
// ----------------------------------------

void ZwCANb_Gpio(void)
{
	EALLOW;

	GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;	    // Enable pull-up for GPIO21 (CANRXB)
	GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;	    // Enable pull-up for GPIO20 (CANTXB)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;   // Asynchronous qualification for GPIO21 (CANRXB)

	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3;	// Configure GPIO21 for CANRXB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3;	// Configure GPIO20 for CANTXB operation

    EDIS;
}
// ----------------------------------------


// Memory IO Area - CAN-A
//
//----------------------------------------
#pragma DATA_SECTION(ECanaRegs,"ECanaRegsFile");
volatile struct ECAN_REGS ECanaRegs;

//----------------------------------------
#pragma DATA_SECTION(ECanaMboxes,"ECanaMboxesFile");
volatile struct ECAN_MBOXES ECanaMboxes;

//----------------------------------------
#pragma DATA_SECTION(ECanaLAMRegs,"ECanaLAMRegsFile");
volatile struct LAM_REGS ECanaLAMRegs;

//----------------------------------------
#pragma DATA_SECTION(ECanaMOTSRegs,"ECanaMOTSRegsFile");
volatile struct MOTS_REGS ECanaMOTSRegs;

//----------------------------------------
#pragma DATA_SECTION(ECanaMOTORegs,"ECanaMOTORegsFile");
volatile struct MOTO_REGS ECanaMOTORegs;


// Memory IO Area - CAN-B
//
//----------------------------------------
#pragma DATA_SECTION(ECanbRegs,"ECanbRegsFile");
volatile struct ECAN_REGS ECanbRegs;

//----------------------------------------
#pragma DATA_SECTION(ECanbMboxes,"ECanbMboxesFile");
volatile struct ECAN_MBOXES ECanbMboxes;

//----------------------------------------
#pragma DATA_SECTION(ECanbLAMRegs,"ECanbLAMRegsFile");
volatile struct LAM_REGS ECanbLAMRegs;

//----------------------------------------
#pragma DATA_SECTION(ECanbMOTSRegs,"ECanbMOTSRegsFile");
volatile struct MOTS_REGS ECanbMOTSRegs;

//----------------------------------------
#pragma DATA_SECTION(ECanbMOTORegs,"ECanbMOTORegsFile");
volatile struct MOTO_REGS ECanbMOTORegs;

// No more.
