// -----------------------------------------
// BCCI-Slave communication interface
// ----------------------------------------

// Header
#include "BCCISlave.h"
//
// Includes
#include "CRC16.h"
#include "SysConfig.h"


// Macro
//
#define CAN_ID_W_16				10
#define CAN_ID_W_16_2			12
#define CAN_ID_W_32				14
#define CAN_ID_R_16				20
#define CAN_ID_R_16_2			22
#define CAN_ID_R_32				24
#define CAN_ID_WB_16			30
#define CAN_ID_RB_16			40
#define CAN_ID_CALL				50
#define CAN_ID_ERR				61
#define CAN_ID_R_BP				70
#define CAN_ID_A_BP				71
//
#define MBOX_W_16				1
#define MBOX_W_16_A				2
#define MBOX_W_16_2				3
#define MBOX_W_16_2_A			4
#define MBOX_W_32				5
#define MBOX_W_32_A				6
#define MBOX_R_16				7
#define MBOX_R_16_A				8
#define MBOX_R_16_2				9
#define MBOX_R_16_2_A			10
#define MBOX_R_32				11
#define MBOX_R_32_A				12
#define MBOX_C					13
#define MBOX_C_A				14
#define MBOX_ERR_A				15
#define MBOX_RB_16				17
#define MBOX_RB_16_A			18
#define MBOX_WB_16  			19
#define MBOX_WB_16_A			20
#define MBOX_BP					23
#define MBOX_BP_A				24

// Forward functions
//
static void BCCI_SendErrorFrame(pBCCI_Interface Interface, Int16U ErrorCode, Int16U Details);
static void BCCI_SendResponseFrame(pBCCI_Interface Interface, Int16U Mailbox, pCANMessage Message);
static void BCCI_SendResponseFrameEx(pBCCI_Interface Interface, Int16U Mailbox, pCANMessage Message, Int16U MessageLength);
//
static void BCCI_HandleRead16(pBCCI_Interface Interface);
static void BCCI_HandleBroadcastPing(pBCCI_Interface Interface);
static void BCCI_HandleRead32(pBCCI_Interface Interface);
static void BCCI_HandleRead16Double(pBCCI_Interface Interface);
static void BCCI_HandleWrite16(pBCCI_Interface Interface);
static void BCCI_HandleWrite32(pBCCI_Interface Interface);
static void BCCI_HandleWrite16Double(pBCCI_Interface Interface);
static void BCCI_HandleWriteBlock16(pBCCI_Interface Interface);
static void BCCI_HandleCall(pBCCI_Interface Interface);
static void BCCI_HandleReadBlock16(pBCCI_Interface Interface);


// Functions
//
void BCCI_Init(pBCCI_Interface Interface, pBCCI_IOConfig IOConfig, pxCCI_ServiceConfig ServiceConfig,
 			   pInt16U DataTable, Int16U DataTableSize, void *ArgumentForCallback)
{
	Int16U i;
	
	// Reset fields
	Interface->ProtectionAndEndpoints.ProtectedAreasUsed = 0;
	
	for(i = 0; i < xCCI_MAX_READ_ENDPOINTS; ++i)
	{
		Interface->ProtectionAndEndpoints.Endpoints[i].ReadEndpoint16 = NULL;
		Interface->ProtectionAndEndpoints.Endpoints[i].Name = 0;
		Interface->ProtectionAndEndpoints.Endpoints[i].Initialized = FALSE;

		Interface->ProtectionAndEndpoints.ReadEndpoints32[i] = NULL;
	} 

	for(i = 0; i < xCCI_MAX_WRITE_ENDPOINTS + 1; ++i)
	{
		Interface->ProtectionAndEndpoints.WriteEndpoints16[i] = NULL;
		Interface->ProtectionAndEndpoints.WriteEndpoints32[i] = NULL;
	} 

	// Save parameters
	Interface->IOConfig = IOConfig;
	Interface->ServiceConfig = ServiceConfig;
	Interface->DataTableAddress = DataTable;
	Interface->DataTableSize = DataTableSize;
	Interface->ArgForEPCallback = ArgumentForCallback;

	// Setup messages
	Interface->IOConfig->IO_ConfigMailbox(MBOX_W_16, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_W_16, TRUE, 4, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_W_16_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_W_16 + 1, FALSE, 2, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_W_16_2, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_W_16_2, TRUE, 8, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_W_16_2_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_W_16_2 + 1, FALSE, 4, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_W_32, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_W_32, TRUE, 6, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_W_32_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_W_32 + 1, FALSE, 2, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_R_16, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_R_16, TRUE, 2, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_R_16_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_R_16 + 1, FALSE, 4, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_R_16_2, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_R_16_2, TRUE, 4, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_R_16_2_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_R_16_2 + 1, FALSE, 8, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_R_32, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_R_32, TRUE, 2, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_R_32_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_R_32 + 1, FALSE, 6, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_C, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_CALL, TRUE, 2, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_C_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_CALL + 1, FALSE, 2, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_ERR_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_ERR, FALSE, 4, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_RB_16, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_RB_16, TRUE, 2, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_RB_16_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_RB_16 + 1, FALSE, 8, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_WB_16, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_WB_16, TRUE, 2, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_WB_16_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_WB_16 + 1, FALSE, 8, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);

	Interface->IOConfig->IO_ConfigMailbox(MBOX_BP, CAN_ID_R_BP, TRUE, 0, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
	Interface->IOConfig->IO_ConfigMailbox(MBOX_BP_A, DEVICE_CAN_ADDRESS * DEV_ADDR_MPY + CAN_ID_A_BP, FALSE, 0, ZW_CAN_MBProtected | ZW_CAN_UseExtendedID, ZW_CAN_NO_PRIORITY, ZW_CAN_STRONG_MATCH);
}
// ----------------------------------------

void BCCI_Process(pBCCI_Interface Interface, Boolean MaskStateChangeOperations)
{
	CANMessage dummy;

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_W_16, NULL))
	{
		if(MaskStateChangeOperations)
		{
			Interface->IOConfig->IO_GetMessage(MBOX_W_16, &dummy);
			BCCI_SendErrorFrame(Interface, ERR_BLOCKED, 0);
		}
		else
			BCCI_HandleWrite16(Interface);

		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_W_16_2, NULL))
	{
		if(MaskStateChangeOperations)
		{
			Interface->IOConfig->IO_GetMessage(MBOX_W_16_2, &dummy);
			BCCI_SendErrorFrame(Interface, ERR_BLOCKED, 0);
		}
		else
			BCCI_HandleWrite16Double(Interface);

		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_W_32, NULL))
	{
		if(MaskStateChangeOperations)
		{
			Interface->IOConfig->IO_GetMessage(MBOX_W_32, &dummy);
			BCCI_SendErrorFrame(Interface, ERR_BLOCKED, 0);
		}
		else
			BCCI_HandleWrite32(Interface);

		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_R_16, NULL))
	{
		BCCI_HandleRead16(Interface);
		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_R_16_2, NULL))
	{
		BCCI_HandleRead16Double(Interface);
		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_R_32, NULL))
	{
		BCCI_HandleRead32(Interface);
		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_C, NULL))
	{
		BCCI_HandleCall(Interface);
		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_RB_16, NULL))
	{
		BCCI_HandleReadBlock16(Interface);
		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_WB_16, NULL))
	{
		if(MaskStateChangeOperations)
		{
			Interface->IOConfig->IO_GetMessage(MBOX_WB_16, &dummy);
			BCCI_SendErrorFrame(Interface, ERR_BLOCKED, 0);
		}
		else
			BCCI_HandleWriteBlock16(Interface);

		return;
	}

	if(Interface->IOConfig->IO_IsMessageReceived(MBOX_BP, NULL))
	{
		BCCI_HandleBroadcastPing(Interface);
		return;
	}
}
// ----------------------------------------

static void BCCI_HandleRead16(pBCCI_Interface Interface)
{
	Int16U addr;
	CANMessage CANInput;

	Interface->IOConfig->IO_GetMessage(MBOX_R_16, &CANInput);
	addr = CANInput.HIGH.WORD.WORD_0;

	if(addr >= Interface->DataTableSize)
	{
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr);
	}
	else
	{
		CANMessage CANOutput;

		CANOutput.HIGH.WORD.WORD_0 = addr;
		CANOutput.HIGH.WORD.WORD_1 = Interface->DataTableAddress[addr];

		BCCI_SendResponseFrame(Interface, MBOX_R_16_A, &CANOutput);
	}
}
// ----------------------------------------

static void BCCI_HandleBroadcastPing(pBCCI_Interface Interface)
{
	CANMessage message;
	Interface->IOConfig->IO_GetMessage(MBOX_BP, &message);
	BCCI_SendResponseFrame(Interface, MBOX_BP_A, &message);
}
// ----------------------------------------

static void BCCI_HandleRead16Double(pBCCI_Interface Interface)
{
	Int16U addr, addr2;
	CANMessage CANInput;

	Interface->IOConfig->IO_GetMessage(MBOX_R_16_2, &CANInput);
	addr = CANInput.HIGH.WORD.WORD_0;
	addr2 = CANInput.HIGH.WORD.WORD_1;

	if(addr >= Interface->DataTableSize)
	{
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr);
	}
	else if(addr2 >= Interface->DataTableSize)
	{
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr2);
	}
	else
	{
		CANMessage CANOutput;

		CANOutput.HIGH.WORD.WORD_0 = addr;
		CANOutput.HIGH.WORD.WORD_1 = Interface->DataTableAddress[addr];
		CANOutput.LOW.WORD.WORD_2 = addr2;
		CANOutput.LOW.WORD.WORD_3 = Interface->DataTableAddress[addr2];

		BCCI_SendResponseFrame(Interface, MBOX_R_16_2_A, &CANOutput);
	}
}
// ----------------------------------------

static void BCCI_HandleRead32(pBCCI_Interface Interface)
{
	Int16U addr;
	CANMessage CANInput;

	Interface->IOConfig->IO_GetMessage(MBOX_R_32, &CANInput);
	addr = CANInput.HIGH.WORD.WORD_0;

	if(addr >= Interface->DataTableSize)
	{
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr);
	}
	else
	{
		CANMessage CANOutput;
		Int32U data = Interface->ServiceConfig->Read32Service(Interface->DataTableAddress, addr);

		CANOutput.HIGH.WORD.WORD_0 = addr;
		CANOutput.HIGH.WORD.WORD_1 = data >> 16;
		CANOutput.LOW.WORD.WORD_2 = data & 0x0000FFFF;

		BCCI_SendResponseFrame(Interface, MBOX_R_32_A, &CANOutput);
	}
}
// ----------------------------------------

static void BCCI_HandleWrite16(pBCCI_Interface Interface)
{
	Int16U addr, data;
	CANMessage CANInput;

	Interface->IOConfig->IO_GetMessage(MBOX_W_16, &CANInput);
	addr = CANInput.HIGH.WORD.WORD_0;
	data = CANInput.HIGH.WORD.WORD_1;

	if(addr >= Interface->DataTableSize)
	{
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr);
	}
	else if(xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr))
	{
		BCCI_SendErrorFrame(Interface, ERR_PROTECTED, addr);
	}
	else if(Interface->ServiceConfig->ValidateCallback16
			&& !Interface->ServiceConfig->ValidateCallback16(addr, data))
	{
		BCCI_SendErrorFrame(Interface, ERR_VALIDATION, addr);
	}
	else
	{
		CANMessage CANOutput;

		Interface->DataTableAddress[addr] = data;
		CANOutput.HIGH.WORD.WORD_0 = addr;

		BCCI_SendResponseFrame(Interface, MBOX_W_16_A, &CANOutput);
	}
}
// ----------------------------------------

static void BCCI_HandleWrite16Double(pBCCI_Interface Interface)
{
	Int16U addr1, data1, addr2, data2;
	CANMessage CANInput;

	Interface->IOConfig->IO_GetMessage(MBOX_W_16_2, &CANInput);
	addr1 = CANInput.HIGH.WORD.WORD_0;
	data1 = CANInput.HIGH.WORD.WORD_1;
	addr2 = CANInput.LOW.WORD.WORD_2;
	data2 = CANInput.LOW.WORD.WORD_3;

	if(addr1 >= Interface->DataTableSize)
	{
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr1);
	}
	else if(xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr1))
	{
		BCCI_SendErrorFrame(Interface, ERR_PROTECTED, addr1);
	}
	else if(addr2 >= Interface->DataTableSize)
	{
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr2);
	}
	else if(xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr2))
	{
		BCCI_SendErrorFrame(Interface, ERR_PROTECTED, addr2);
	}
	else if(Interface->ServiceConfig->ValidateCallback16
			&& !Interface->ServiceConfig->ValidateCallback16(addr1, data1))
	{
		BCCI_SendErrorFrame(Interface, ERR_VALIDATION, addr1);
	}
	else if(Interface->ServiceConfig->ValidateCallback16
			&& !Interface->ServiceConfig->ValidateCallback16(addr2, data2))
	{
		BCCI_SendErrorFrame(Interface, ERR_VALIDATION, addr2);
	}
	else
	{
		CANMessage CANOutput;

		Interface->DataTableAddress[addr1] = data1;
		CANOutput.HIGH.WORD.WORD_0 = addr1;
		Interface->DataTableAddress[addr2] = data2;
		CANOutput.HIGH.WORD.WORD_1 = addr2;

		BCCI_SendResponseFrame(Interface, MBOX_W_16_2_A, &CANOutput);
	}
}
// ----------------------------------------

static void BCCI_HandleWrite32(pBCCI_Interface Interface)
{
	Int16U addr;
	Int32U data;
	CANMessage CANInput;

	Interface->IOConfig->IO_GetMessage(MBOX_W_32, &CANInput);
	addr = CANInput.HIGH.WORD.WORD_0;
	data = CANInput.LOW.WORD.WORD_2 | (((Int32U)CANInput.HIGH.WORD.WORD_1) << 16);

	if((addr + 1) >= Interface->DataTableSize)
	{
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr);
	}
	else if(xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr)
			|| xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr + 1))
	{
		BCCI_SendErrorFrame(Interface, ERR_PROTECTED, addr);
	}
	else if(Interface->ServiceConfig->ValidateCallback32
			&& !Interface->ServiceConfig->ValidateCallback32(addr, data))
	{
		BCCI_SendErrorFrame(Interface, ERR_VALIDATION, addr);
	}
	else
	{
		CANMessage CANOutput;

		Interface->ServiceConfig->Write32Service(Interface->DataTableAddress, addr, data);
		CANOutput.HIGH.WORD.WORD_0 = addr;

		BCCI_SendResponseFrame(Interface, MBOX_W_32_A, &CANOutput);
	}
}
// ----------------------------------------

static void BCCI_HandleCall(pBCCI_Interface Interface)
{
	Int16U action;
	CANMessage CANInput;

	Interface->IOConfig->IO_GetMessage(MBOX_C, &CANInput);
	action = CANInput.HIGH.WORD.WORD_0;

	if(Interface->ServiceConfig->UserActionCallback != NULL)
	{
		Int16U userError = 0;

		if(!Interface->ServiceConfig->UserActionCallback(action, &userError))
		{
			BCCI_SendErrorFrame(Interface, ERR_INVALID_ACTION, action);
		}
		else if(userError != 0)
		{
			BCCI_SendErrorFrame(Interface, ERR_USER, userError);
		}
		else
		{
			CANMessage CANOutput;

			CANOutput.HIGH.WORD.WORD_0 = action;
			BCCI_SendResponseFrame(Interface, MBOX_C_A, &CANOutput);
		}
	}
	else
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ACTION, action);
}
// ----------------------------------------

static void BCCI_HandleReadBlock16(pBCCI_Interface Interface)
{
	pInt16U src;
	Int16U epnt, epnt_index;
	CANMessage CANInput;

	Interface->IOConfig->IO_GetMessage(MBOX_RB_16, &CANInput);
	epnt = CANInput.HIGH.WORD.WORD_0;

	if(xCCI_EndpointIndex(&Interface->ProtectionAndEndpoints, epnt, &epnt_index))
	{
		CANMessage CANOutput;

		Int16U length = Interface->ProtectionAndEndpoints.Endpoints[epnt_index].ReadEndpoint16(epnt, &src, FALSE, FALSE,
																		 	 	Interface->ArgForEPCallback, 4);

		switch(length)
		{
			case 4:
				CANOutput.LOW.WORD.WORD_3 = src[3];
			case 3:
				CANOutput.LOW.WORD.WORD_2 = src[2];
			case 2:
				CANOutput.HIGH.WORD.WORD_1 = src[1];
			case 1:
				CANOutput.HIGH.WORD.WORD_0 = src[0];
				BCCI_SendResponseFrameEx(Interface, MBOX_RB_16_A, &CANOutput, length);
				break;
			default:
				BCCI_SendResponseFrameEx(Interface, MBOX_RB_16_A, &CANOutput, 0);
				break;
		}
	}
	else
		BCCI_SendErrorFrame(Interface, ERR_INVALID_ENDPOINT, epnt);
}
// ----------------------------------------

static void BCCI_HandleWriteBlock16(pBCCI_Interface Interface)
{
	CANMessage CANInput;
	Int16U epnt, length;
	Int16U Data[3];

	Interface->IOConfig->IO_GetMessage(MBOX_WB_16, &CANInput);

	epnt = CANInput.HIGH.WORD.WORD_0 >> 8;
	length = CANInput.HIGH.WORD.WORD_0 & 0xFF;
	Data[0] = CANInput.HIGH.WORD.WORD_1;
	Data[1] = CANInput.LOW.WORD.WORD_2;
	Data[2] = CANInput.LOW.WORD.WORD_3;

	if(length <= BCCI_BLOCK_MAX_VAL_16_W)
	{
		if((epnt < xCCI_MAX_WRITE_ENDPOINTS + 1) && Interface->ProtectionAndEndpoints.WriteEndpoints16[epnt])
		{
			CANMessage CANOutput;
			CANOutput.HIGH.WORD.WORD_0 = epnt << 8;

			Interface->ProtectionAndEndpoints.WriteEndpoints16[epnt](epnt, &Data[0], FALSE,
																		length, Interface->ArgForEPCallback);

			BCCI_SendResponseFrameEx(Interface, MBOX_WB_16_A, &CANOutput, length);
		}
		else
			BCCI_SendErrorFrame(Interface, ERR_INVALID_ENDPOINT, epnt);
	}
	else
		BCCI_SendErrorFrame(Interface, ERR_ILLEGAL_SIZE, length);
}
// ----------------------------------------

static void BCCI_SendResponseFrame(pBCCI_Interface Interface, Int16U Mailbox, pCANMessage Message)
{
	Interface->IOConfig->IO_SendMessage(Mailbox, Message);
}
// ----------------------------------------

static void BCCI_SendResponseFrameEx(pBCCI_Interface Interface, Int16U Mailbox, pCANMessage Message, Int16U MessageLength)
{
	Message->DLC = MessageLength * 2;

	Interface->IOConfig->IO_SendMessageEx(Mailbox, Message, FALSE, TRUE);
}
// ----------------------------------------

static void BCCI_SendErrorFrame(pBCCI_Interface Interface, Int16U ErrorCode, Int16U Details)
{
	CANMessage CANOutput;

	CANOutput.HIGH.WORD.WORD_0 = ErrorCode;
	CANOutput.HIGH.WORD.WORD_1 = Details;

	Interface->IOConfig->IO_SendMessage(MBOX_ERR_A, &CANOutput);
}
// ----------------------------------------

// No more.
