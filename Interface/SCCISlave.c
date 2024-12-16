// -----------------------------------------
// SCCI-Slave communication interface
// ----------------------------------------

// Header
#include "SCCISlave.h"
//
// Includes
#include "CRC16.h"
#include "SysConfig.h"

// Constants
//
enum DispID
{
	DISP_NONE			=	0,
	DISP_R_16			=	1,
	DISP_R_32			=	2,
	DISP_R_16_2			=	3,
	DISP_W_16			=	4,
	DISP_W_32			=	5,
	DISP_W_16_2			=	6,
	DISP_RB_16			=	7,
	DISP_RRB_16			=	8,
	DISP_WB_16			=	9,
	DISP_C				=	10,
	DISP_RBF_16			=	11,
	DISP_RRBF_16		= 	12,
	DISP_RB_32			=	13,
	DISP_RRB_32			=	14
};


// Forward functions
//
static void SCCI_DispatchHeader(pSCCI_Interface Interface);
static void SCCI_DispatchBody(pSCCI_Interface Interface, Boolean MaskStateChangeOperations);
static void SCCI_SendErrorFrame(pSCCI_Interface Interface, Int16U ErrorCode, Int16U Details);
static void SCCI_SendResponseFrame(pSCCI_Interface Interface, Int16U FrameSize);
//
static void SCCI_HandleRead16(pSCCI_Interface Interface);
static void SCCI_HandleRead32(pSCCI_Interface Interface);
static void SCCI_HandleRead16Double(pSCCI_Interface Interface);
static void SCCI_HandleWrite16(pSCCI_Interface Interface);
static void SCCI_HandleWrite32(pSCCI_Interface Interface);
static void SCCI_HandleWrite16Double(pSCCI_Interface Interface);
static void SCCI_HandleReadBlock16(pSCCI_Interface Interface, Boolean Repeat);
static void SCCI_HandleWriteBlock16(pSCCI_Interface Interface);
#ifdef RS232_X32_EP
static void SCCI_HandleReadBlock32(pSCCI_Interface Interface, Boolean Repeat);
#endif
static void SCCI_HandleReadBlockFast16(pSCCI_Interface Interface, Boolean Repeat);
static void SCCI_HandleCall(pSCCI_Interface Interface);


// Variables
//
static Int16U ZeroBuffer[xCCI_BUFFER_SIZE] = {0};


// Functions
//
#ifdef RS232_X32_EP
void SCCI_Init(pSCCI_Interface Interface, pSCCI_IOConfig IOConfig, pxCCI_ServiceConfig ServiceConfig,
 			   pInt16U DataTable, Int16U DataTableSize, Int32U MessageTimeoutTicks,
 			   void *ArgumentForCallback16, void *ArgumentForCallback32)
#else
void SCCI_Init(pSCCI_Interface Interface, pSCCI_IOConfig IOConfig, pxCCI_ServiceConfig ServiceConfig,
 			   pInt16U DataTable, Int16U DataTableSize, Int32U MessageTimeoutTicks,
 			   void *ArgumentForCallback16)
#endif
{
	Int16U i;
	
	// Reset fields
	Interface->State = SCCI_STATE_WAIT_STARTBYTE;
	Interface->DispID = DISP_NONE;
	Interface->ExpectedBodyLength = 0;
	Interface->LastTimestampTicks = 0;
	Interface->ProtectionAndEndpoints.ProtectedAreasUsed = 0;
	
	for(i = 0; i < xCCI_BUFFER_SIZE; ++i)
		Interface->MessageBuffer[i] = 0;

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
	Interface->TimeoutValueTicks = MessageTimeoutTicks;
	Interface->ArgForEPCallback1 = ArgumentForCallback16;
#ifdef RS232_X32_EP
	Interface->ArgForEPCallback2 = ArgumentForCallback32;
#endif
}
// ----------------------------------------

void SCCI_Process(pSCCI_Interface Interface, Int64U CurrentTickCount, Boolean MaskStateChangeOperations)
{
	switch(Interface->State)
	{
		case SCCI_STATE_WAIT_STARTBYTE:
			if(Interface->IOConfig->IO_GetBytesToReceive() > 0)
			{
				Int16U startByte = Interface->IOConfig->IO_ReceiveByte();

				if(startByte == START_BYTE)
					Interface->State = SCCI_STATE_WAIT_HEADER;
			}
			break;
		case SCCI_STATE_WAIT_HEADER:
			if(Interface->IOConfig->IO_GetBytesToReceive() >= 3)
			{
				Int16U nextByte = Interface->IOConfig->IO_ReceiveByte();
				Interface->MessageBuffer[0] = nextByte | (START_BYTE << 8);

				Interface->IOConfig->IO_ReceiveArray16(Interface->MessageBuffer + 1, 1);
				SCCI_DispatchHeader(Interface);

				if(Interface->State == SCCI_STATE_WAIT_BODY)
					Interface->LastTimestampTicks = CurrentTickCount + Interface->TimeoutValueTicks;
			}
			break;
		case SCCI_STATE_WAIT_BODY:
			if(Interface->IOConfig->IO_GetBytesToReceive() >= Interface->ExpectedBodyLength)
			{
				Interface->IOConfig->IO_ReceiveArray16(Interface->MessageBuffer + 2, Interface->ExpectedBodyLength);
				SCCI_DispatchBody(Interface, MaskStateChangeOperations);
			}
			else if(Interface->TimeoutValueTicks && (CurrentTickCount > Interface->LastTimestampTicks))
				SCCI_SendErrorFrame(Interface, ERR_TIMEOUT, CurrentTickCount - Interface->LastTimestampTicks);
			break;
	}
}
// ----------------------------------------

static void SCCI_DispatchHeader(pSCCI_Interface Interface)
{
	Int16U fnc = Interface->MessageBuffer[1] >> 8;

	if((Interface->MessageBuffer[0] & 0xFF) == DEVICE_SCCI_ADDRESS)
	{
		switch((fnc & FUNCTION_CODE_MASK) >> 3)
		{
			case FUNCTION_WRITE:
				switch(fnc & FUNCTION_SCODE_MASK)
				{
					case SFUNC_16:
						Interface->ExpectedBodyLength = 3;
						Interface->State = SCCI_STATE_WAIT_BODY;
						Interface->DispID = DISP_W_16;	
						break;
					case SFUNC_32:
						Interface->ExpectedBodyLength = 4;
						Interface->State = SCCI_STATE_WAIT_BODY;	
						Interface->DispID = DISP_W_32;	
						break;
					case SFUNC_16_2:
						Interface->ExpectedBodyLength = 5;
						Interface->State = SCCI_STATE_WAIT_BODY;	
						Interface->DispID = DISP_W_16_2;	
						break;
					default:
						SCCI_SendErrorFrame(Interface, ERR_INVALID_SFUNCTION, fnc & FUNCTION_SCODE_MASK);
						break;
				}
				break;
			case FUNCTION_READ:
				switch(fnc & FUNCTION_SCODE_MASK)
				{
					case SFUNC_16:
						Interface->ExpectedBodyLength = 2;
						Interface->State = SCCI_STATE_WAIT_BODY;	
						Interface->DispID = DISP_R_16;	
						break;
					case SFUNC_32:
						Interface->ExpectedBodyLength = 2;
						Interface->State = SCCI_STATE_WAIT_BODY;	
						Interface->DispID = DISP_R_32;	
						break;
					case SFUNC_16_2:
						Interface->ExpectedBodyLength = 3;
						Interface->State = SCCI_STATE_WAIT_BODY;	
						Interface->DispID = DISP_R_16_2;	
						break;
					default:
						SCCI_SendErrorFrame(Interface, ERR_INVALID_SFUNCTION, fnc & FUNCTION_SCODE_MASK);
						break;
				}
				break;
			case FUNCTION_WRITE_BLOCK:
				switch(fnc & FUNCTION_SCODE_MASK)
				{
					case SFUNC_16:
						Interface->ExpectedBodyLength = 5;
						Interface->State = SCCI_STATE_WAIT_BODY;	
						Interface->DispID = DISP_WB_16;	
						break;
					case SFUNC_32:
						SCCI_SendErrorFrame(Interface, ERR_NOT_SUPPORTED, fnc & FUNCTION_SCODE_MASK);
						break;
					default:
						SCCI_SendErrorFrame(Interface, ERR_INVALID_SFUNCTION, fnc & FUNCTION_SCODE_MASK);
						break;
				}
				break;
			case FUNCTION_READ_BLOCK:
				switch(fnc & FUNCTION_SCODE_MASK)
				{
					case SFUNC_16:
						Interface->ExpectedBodyLength = 2;
						Interface->State = SCCI_STATE_WAIT_BODY;	
						Interface->DispID = DISP_RB_16;	
						break;
					case SFUNC_32:
#ifdef RS232_X32_EP
						Interface->ExpectedBodyLength = 2;
						Interface->State = SCCI_STATE_WAIT_BODY;
						Interface->DispID = DISP_RB_32;
#else
						SCCI_SendErrorFrame(Interface, ERR_NOT_SUPPORTED, fnc & FUNCTION_SCODE_MASK);
#endif
						break;
					case SFUNC_REP_16:
						Interface->ExpectedBodyLength = 2;
						Interface->State = SCCI_STATE_WAIT_BODY;
						Interface->DispID = DISP_RRB_16;
						break;
#ifdef RS232_X32_EP
					case SFUNC_REP_32:
						Interface->ExpectedBodyLength = 2;
						Interface->State = SCCI_STATE_WAIT_BODY;
						Interface->DispID = DISP_RRB_32;
						break;
#endif
					default:
						SCCI_SendErrorFrame(Interface, ERR_INVALID_SFUNCTION, fnc & FUNCTION_SCODE_MASK);
						break;
				}
				break;
			case FUNCTION_CALL:
				if((fnc & FUNCTION_SCODE_MASK) == 0)
				{
					Interface->ExpectedBodyLength = 2;
					Interface->State = SCCI_STATE_WAIT_BODY;
					Interface->DispID = DISP_C;	
				}
				else
					SCCI_SendErrorFrame(Interface, ERR_INVALID_SFUNCTION, fnc & FUNCTION_SCODE_MASK);
				break;
			case FUNCTION_FAST_READ_BLK:
				switch(fnc & FUNCTION_SCODE_MASK)
				{
					case SFUNC_16:
						Interface->ExpectedBodyLength = 2;
						Interface->State = SCCI_STATE_WAIT_BODY;
						Interface->DispID = DISP_RBF_16;
						break;
					case SFUNC_32:
						SCCI_SendErrorFrame(Interface, ERR_NOT_SUPPORTED, fnc & FUNCTION_SCODE_MASK);
						break;
					case SFUNC_REP_16:
						Interface->ExpectedBodyLength = 2;
						Interface->State = SCCI_STATE_WAIT_BODY;
						Interface->DispID = DISP_RRBF_16;
						break;
					default:
						SCCI_SendErrorFrame(Interface, ERR_INVALID_SFUNCTION, fnc & FUNCTION_SCODE_MASK);
						break;
				}
				break;
			default:
				SCCI_SendErrorFrame(Interface, ERR_INVALID_FUNCTION, fnc & FUNCTION_CODE_MASK);
				break;
		}
	}
}
// ----------------------------------------

static void SCCI_DispatchBody(pSCCI_Interface Interface, Boolean MaskStateChangeOperations)
{
	Int16U crc = CRC16_ComputeCRC(Interface->MessageBuffer, Interface->ExpectedBodyLength + 1);
	if(Interface->MessageBuffer[Interface->ExpectedBodyLength + 1] != crc)
	{
		SCCI_SendErrorFrame(Interface, ERR_CRC, crc);
		return;
	}

	if(MaskStateChangeOperations &&
	   (Interface->DispID == DISP_W_16 ||
	    Interface->DispID == DISP_W_32 ||
	    Interface->DispID == DISP_W_16_2 ||
	    Interface->DispID == DISP_WB_16))
	{
		SCCI_SendErrorFrame(Interface, ERR_BLOCKED, 0);
		return;
	}

	switch(Interface->DispID)
	{
		case DISP_R_16:
			SCCI_HandleRead16(Interface);
			break;
		case DISP_R_32:
			SCCI_HandleRead32(Interface);
			break;
		case DISP_R_16_2:
			SCCI_HandleRead16Double(Interface);
			break;
		case DISP_W_16:
			SCCI_HandleWrite16(Interface);
			break;
		case DISP_W_32:
			SCCI_HandleWrite32(Interface);
			break;
		case DISP_W_16_2:
			SCCI_HandleWrite16Double(Interface);
			break;
		case DISP_RB_16:
			SCCI_HandleReadBlock16(Interface, FALSE);
			break;
		case DISP_RRB_16:
			SCCI_HandleReadBlock16(Interface, TRUE);
			break;
		case DISP_WB_16:
			SCCI_HandleWriteBlock16(Interface);
			break;
		case DISP_C:
			SCCI_HandleCall(Interface);
			break;
		case DISP_RBF_16:
			SCCI_HandleReadBlockFast16(Interface, FALSE);
			break;
		case DISP_RRBF_16:
			SCCI_HandleReadBlockFast16(Interface, TRUE);
			break;
#ifdef RS232_X32_EP
		case DISP_RB_32:
			SCCI_HandleReadBlock32(Interface, FALSE);
			break;
		case DISP_RRB_32:
			SCCI_HandleReadBlock32(Interface, TRUE);
			break;
#endif
		default:
			SCCI_SendErrorFrame(Interface, ERR_NOT_SUPPORTED, 0);
			break;
	}
}
// ----------------------------------------

static void SCCI_HandleRead16(pSCCI_Interface Interface)
{
	Int16U addr = Interface->MessageBuffer[2];

	if(addr >= Interface->DataTableSize)
	{
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr);
	}
	else
	{
		Interface->MessageBuffer[3] = Interface->DataTableAddress[addr];
		SCCI_SendResponseFrame(Interface, 5);
	}
}
// ----------------------------------------

static void SCCI_HandleRead32(pSCCI_Interface Interface)
{
	Int16U addr = Interface->MessageBuffer[2];

	if((addr + 1) >= Interface->DataTableSize)
	{
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr + 1);
	}
	else
	{
		Int32U data = Interface->ServiceConfig->Read32Service(Interface->DataTableAddress, addr);

		Interface->MessageBuffer[3] = data >> 16;
		Interface->MessageBuffer[4] = data & 0x0000FFFF;

		SCCI_SendResponseFrame(Interface, 6);
	}
}
// ----------------------------------------

static void SCCI_HandleRead16Double(pSCCI_Interface Interface)
{
	Int16U addr1 = Interface->MessageBuffer[2];
	Int16U addr2 = Interface->MessageBuffer[3];

	if(addr1 >= Interface->DataTableSize)
	{
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr1);
	}
	else
	{
		Interface->MessageBuffer[3] = Interface->DataTableAddress[addr1];

		if(addr2 >= Interface->DataTableSize)
		{
			SCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr2);
		}
		else
		{
			Interface->MessageBuffer[4] = addr2;
			Interface->MessageBuffer[5] = Interface->DataTableAddress[addr2];
			SCCI_SendResponseFrame(Interface, 7);
		}
	}
}
// ----------------------------------------

static void SCCI_HandleWrite16(pSCCI_Interface Interface)
{
	Int16U addr = Interface->MessageBuffer[2];
	Int16U data = Interface->MessageBuffer[3];

	if(addr >= Interface->DataTableSize)
	{
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr);
	}
	else if(xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr))
	{
		SCCI_SendErrorFrame(Interface, ERR_PROTECTED, addr);
	}
	else if(Interface->ServiceConfig->ValidateCallback16
			&& !Interface->ServiceConfig->ValidateCallback16(addr, data))
	{
		SCCI_SendErrorFrame(Interface, ERR_VALIDATION, addr);
	}
	else
	{
		Interface->DataTableAddress[addr] = data;
		SCCI_SendResponseFrame(Interface, 4);
	}
}
// ----------------------------------------

static void SCCI_HandleWrite32(pSCCI_Interface Interface)
{
	Int16U addr = Interface->MessageBuffer[2];
	Int32U data = (((Int32U)(Interface->MessageBuffer[3])) << 16) | Interface->MessageBuffer[4];

	if((addr + 1) >= Interface->DataTableSize)
	{
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr + 1);
	}
	else if(xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr)
			|| xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr + 1))
	{
		SCCI_SendErrorFrame(Interface, ERR_PROTECTED, addr);
	}
	else if(Interface->ServiceConfig->ValidateCallback32
			&& !Interface->ServiceConfig->ValidateCallback32(addr, data))
	{
		SCCI_SendErrorFrame(Interface, ERR_VALIDATION, addr);
	}
	else
	{
		Interface->ServiceConfig->Write32Service(Interface->DataTableAddress, addr, data);
		SCCI_SendResponseFrame(Interface, 4);
	}
}
// ----------------------------------------

static void SCCI_HandleWrite16Double(pSCCI_Interface Interface)
{
	Int16U addr1 = Interface->MessageBuffer[2];
	Int16U addr2 = Interface->MessageBuffer[4];
	Int16U data1 = Interface->MessageBuffer[3];
	Int16U data2 = Interface->MessageBuffer[5];

	if(addr1 >= Interface->DataTableSize)
	{
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr1);
	}
	else if(xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr1))
	{
		SCCI_SendErrorFrame(Interface, ERR_PROTECTED, addr1);
	}
	else if(Interface->ServiceConfig->ValidateCallback16
			&& !Interface->ServiceConfig->ValidateCallback16(addr1, data1))
	{
		SCCI_SendErrorFrame(Interface, ERR_VALIDATION, addr1);
	}
	else
	{
		if(addr2 >= Interface->DataTableSize)
		{
			SCCI_SendErrorFrame(Interface, ERR_INVALID_ADDESS, addr2);
		}
		else if(xCCI_InProtectedZone(&Interface->ProtectionAndEndpoints, addr2))
		{
			SCCI_SendErrorFrame(Interface, ERR_PROTECTED, addr2);
		}
		else if(Interface->ServiceConfig->ValidateCallback16
				&& !Interface->ServiceConfig->ValidateCallback16(addr2, data2))
		{
			SCCI_SendErrorFrame(Interface, ERR_VALIDATION, addr2);
		}
		else
		{
			Interface->DataTableAddress[addr1] = data1;
			Interface->DataTableAddress[addr2] = data2;

			Interface->MessageBuffer[3] = addr2;

			SCCI_SendResponseFrame(Interface, 5);
		}
	}
}
// ----------------------------------------

static void SCCI_HandleReadBlock16(pSCCI_Interface Interface, Boolean Repeat)
{
	pInt16U src;
	Int16U length;
	Int16U epnt = Interface->MessageBuffer[2] >> 8;
	Int16U epnt_index;

	if(xCCI_EndpointIndex(&Interface->ProtectionAndEndpoints, epnt, &epnt_index))
	{
		length = Interface->ProtectionAndEndpoints.Endpoints[epnt_index].ReadEndpoint16(epnt, &src, FALSE, Repeat,
																		 Interface->ArgForEPCallback1, SCCI_BLOCK_MAX_VAL_16_R);
		MemZero16(&Interface->MessageBuffer[3], SCCI_BLOCK_MAX_VAL_16_R);

		if(!length || (length > SCCI_BLOCK_MAX_VAL_16_R))
		{
			Interface->MessageBuffer[2] &= 0xFF00;
		}
		else
		{
			Interface->MessageBuffer[2] = (epnt << 8) | length;
			MemCopy16(src, &Interface->MessageBuffer[3], length);
		}

		SCCI_SendResponseFrame(Interface, SCCI_BLOCK_MAX_VAL_16_R + 4);
	}
	else
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ENDPOINT, epnt);
}
// ----------------------------------------

#ifdef RS232_X32_EP
static void SCCI_HandleReadBlock32(pSCCI_Interface Interface, Boolean Repeat)
{
	pInt32U src;
	Int16U length;
	Int16U epnt = Interface->MessageBuffer[2] >> 8;

	if((epnt < xCCI_MAX_READ_ENDPOINTS) && Interface->ProtectionAndEndpoints.ReadEndpoints32[epnt])
	{
		length = Interface->ProtectionAndEndpoints.ReadEndpoints32[epnt](epnt, &src, FALSE, Repeat,
																		 Interface->ArgForEPCallback2, SCCI_BLOCK_MAX_VAL_32_R);
		MemZero32((pInt32U)(&Interface->MessageBuffer[3]), SCCI_BLOCK_MAX_VAL_32_R);

		if(!length || (length > SCCI_BLOCK_MAX_VAL_32_R))
		{
			Interface->MessageBuffer[2] &= 0xFF00;
		}
		else
		{
			Interface->MessageBuffer[2] = (epnt << 8) | length;
			MemCopy32(src, (pInt32U)(&Interface->MessageBuffer[3]), length);
		}

		SCCI_SendResponseFrame(Interface, SCCI_BLOCK_MAX_VAL_32_R * 2 + 4);
	}
	else
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ENDPOINT, epnt);
}
#endif
// ----------------------------------------

static void SCCI_HandleWriteBlock16(pSCCI_Interface Interface)
{
	Int16U epnt = Interface->MessageBuffer[2] >> 8;
	Int16U length = Interface->MessageBuffer[2] & 0xFF;

	if(length <= SCCI_BLOCK_MAX_VAL_16_W)
	{
		if((epnt < xCCI_MAX_WRITE_ENDPOINTS + 1) && Interface->ProtectionAndEndpoints.WriteEndpoints16[epnt])
		{
			if(Interface->ProtectionAndEndpoints.WriteEndpoints16[epnt](epnt, &Interface->MessageBuffer[3], FALSE,
																	    length, Interface->ArgForEPCallback1))
			{
				Interface->MessageBuffer[2] &= 0xFF00;
				SCCI_SendResponseFrame(Interface, 4);
			}
			else
				SCCI_SendErrorFrame(Interface, ERR_TOO_LONG, epnt);
		}
		else
			SCCI_SendErrorFrame(Interface, ERR_INVALID_ENDPOINT, epnt);
	}
	else
		SCCI_SendErrorFrame(Interface, ERR_ILLEGAL_SIZE, length);
}
// ----------------------------------------

static void SCCI_HandleReadBlockFast16(pSCCI_Interface Interface, Boolean Repeat)
{
	pInt16U src;
	Int16U length;
	Int16U epnt = Interface->MessageBuffer[2] >> 8;
	Int16U epnt_index;

	if(xCCI_EndpointIndex(&Interface->ProtectionAndEndpoints, epnt, &epnt_index))
	{

		length = Interface->ProtectionAndEndpoints.Endpoints[epnt_index].ReadEndpoint16(epnt, &src, TRUE, Repeat,
																		 Interface->ArgForEPCallback1, 0);
		Interface->MessageBuffer[2] = (epnt << 8) | (SCCI_USE_CRC_IN_STREAM ? 1 : 0);

		if(!length || (length > xCCI_BLOCK_STM_MAX_VAL))
			length = 0;

		Interface->MessageBuffer[3] = length;

		if(SCCI_USE_CRC_IN_STREAM)
			Interface->MessageBuffer[4] = CRC16_ComputeCRC(src, length);

		SCCI_SendResponseFrame(Interface, 6);

		Interface->IOConfig->IO_SendArray16(src, length);
		Interface->IOConfig->IO_SendArray16(ZeroBuffer, (8 - length % 8) % 8);
	}
	else
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ENDPOINT, epnt);
}
// ----------------------------------------

static void SCCI_HandleCall(pSCCI_Interface Interface)
{
	Int16U action = Interface->MessageBuffer[2];

	if(Interface->ServiceConfig->UserActionCallback != NULL)
	{
		Int16U userError = 0;

		if(!Interface->ServiceConfig->UserActionCallback(action, &userError))
		{
			SCCI_SendErrorFrame(Interface, ERR_INVALID_ACTION, action);
		}
		else if(userError != 0)
		{
			SCCI_SendErrorFrame(Interface, ERR_USER, userError);
		}
		else
			SCCI_SendResponseFrame(Interface, 4);
	}
	else
		SCCI_SendErrorFrame(Interface, ERR_INVALID_ACTION, action);
}
// ----------------------------------------

static void SCCI_SendResponseFrame(pSCCI_Interface Interface, Int16U FrameSize)
{
	Interface->State = SCCI_STATE_WAIT_STARTBYTE;

	Interface->MessageBuffer[0] = (START_BYTE << 8) | DEVICE_SCCI_ADDRESS;
	Interface->MessageBuffer[1] |= RESPONSE_MASK << 8;
	Interface->MessageBuffer[FrameSize - 1] = CRC16_ComputeCRC(Interface->MessageBuffer, FrameSize - 1);

	Interface->IOConfig->IO_SendArray16(Interface->MessageBuffer, FrameSize); 
}
// ----------------------------------------

static void SCCI_SendErrorFrame(pSCCI_Interface Interface, Int16U ErrorCode, Int16U Details)
{
	Interface->State = SCCI_STATE_WAIT_STARTBYTE;

	Interface->MessageBuffer[0] = (START_BYTE << 8) | DEVICE_SCCI_ADDRESS;
	Interface->MessageBuffer[1] = (RESPONSE_MASK | (FUNCTION_ERROR << 3)) << 8;
	Interface->MessageBuffer[2] = ErrorCode;
	Interface->MessageBuffer[3] = Details;
	Interface->MessageBuffer[4] = CRC16_ComputeCRC(Interface->MessageBuffer, 4);
	
	Interface->IOConfig->IO_SendArray16(Interface->MessageBuffer, 5);
}
// ----------------------------------------

// No more.
