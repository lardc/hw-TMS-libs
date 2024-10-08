﻿// -----------------------------------------
// SCCI communication interface types
// ----------------------------------------

#ifndef __SCCI_TYPES_H
#define __SCCI_TYPES_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "SCCIParams.h"
#include "xCCICommon.h"


// Macro
//
#define START_BYTE				0xDBu
#define FUNCTION_CODE_MASK		(BIT3 | BIT4 | BIT5 | BIT6)
#define FUNCTION_SCODE_MASK		(BIT0 | BIT1 | BIT2)
#define RESPONSE_MASK			BIT7


// Constants
//
//
enum SCCI_FunctionCodes
{
	FUNCTION_WRITE			= 1,
	FUNCTION_READ			= 2,
	FUNCTION_WRITE_BLOCK	= 3,
	FUNCTION_READ_BLOCK		= 4,
	FUNCTION_CALL			= 5,
	FUNCTION_ERROR			= 6,
	FUNCTION_FAST_READ_BLK  = 7
};
//
enum SCCI_SubFunctionCodes
{
	SFUNC_NONE				= 0,
	SFUNC_16				= 1,
	SFUNC_32				= 2,
	SFUNC_16_2				= 3,
	SFUNC_REP_16			= 3,
	SFUNC_REP_32			= 4
};


// Types
//
// Pointers for IO functions
typedef void (*SCCI_FUNC_SendArray16)(pInt16U Buffer, Int16U BufferSize);
typedef void (*SCCI_FUNC_ReceiveArray16)(pInt16U Buffer, Int16U BufferSize);
typedef Int16U (*SCCI_FUNC_GetBytesToReceive)();
typedef Int16U (*SCCI_FUNC_ReceiveByte)();
//
// IO configuration
typedef struct __SCCI_IOConfig
{
	SCCI_FUNC_SendArray16 IO_SendArray16;
	SCCI_FUNC_ReceiveArray16 IO_ReceiveArray16;
	SCCI_FUNC_GetBytesToReceive IO_GetBytesToReceive;
	SCCI_FUNC_ReceiveByte IO_ReceiveByte;
} SCCI_IOConfig, *pSCCI_IOConfig;
//
// Finite state-machine states
typedef enum __SCCI_States
{
	SCCI_STATE_WAIT_STARTBYTE = 0,
	SCCI_STATE_WAIT_HEADER,
	SCCI_STATE_WAIT_BODY
} SCCI_States;
//
// SCCI instance state
typedef struct __SCCI_Interface
{
	pSCCI_IOConfig IOConfig;
	pxCCI_ServiceConfig ServiceConfig;
	SCCI_States State;
	Int16U ExpectedBodyLength;
	Int16U DispID;
	Int16U MessageBuffer[xCCI_BUFFER_SIZE];
	pInt16U DataTableAddress;
	Int16U DataTableSize;
	Int32U TimeoutValueTicks;
	Int64U LastTimestampTicks;
	void *ArgForEPCallback1;
	void *ArgForEPCallback2;
	xCCI_ProtectionAndEndpoints ProtectionAndEndpoints;
} SCCI_Interface, *pSCCI_Interface;


#endif // __SCCI_TYPES_H
