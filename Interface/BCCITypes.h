// -----------------------------------------
// BCCI communication interface types
// ----------------------------------------

#ifndef __BCCI_TYPES_H
#define __BCCI_TYPES_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "BCCIParams.h"
#include "xCCICommon.h"

// Types
//
// Pointers for IO functions
typedef Boolean (*BCCI_FUNC_SendMessage)(Int16U mBox, pCANMessage Data);
typedef Boolean (*BCCI_FUNC_SendMessageEx)(Int16U mBox, pCANMessage Data, Boolean AlterMessageID, Boolean AlterMessageLength);
typedef void (*BCCI_FUNC_GetMessage)(Int16U mBox, pCANMessage Data);
typedef Boolean (*BCCI_FUNC_IsMessageReceived)(Int16U mBox, pBoolean pMessageLost);
typedef void (*BCCI_FUNC_ConfigMailbox)(Int16U mBox, Int32U MsgID, Boolean Dir, Int16U DataLen, Int32U Flags, Int16U TransmitPriority, Int32U LAM);
//
// IO configuration
typedef struct __BCCI_IOConfig
{
	BCCI_FUNC_SendMessage IO_SendMessage;
	BCCI_FUNC_SendMessageEx IO_SendMessageEx;
	BCCI_FUNC_GetMessage IO_GetMessage;
	BCCI_FUNC_IsMessageReceived IO_IsMessageReceived;
	BCCI_FUNC_ConfigMailbox IO_ConfigMailbox;
} BCCI_IOConfig, *pBCCI_IOConfig;
//
// BCCI instance state
typedef struct __BCCI_Interface
{
	pBCCI_IOConfig IOConfig;
	pxCCI_ServiceConfig ServiceConfig;
	pInt16U DataTableAddress;
	Int16U DataTableSize;
	void *ArgForEPCallback;
	xCCI_ProtectionAndEndpoints ProtectionAndEndpoints;
} BCCI_Interface, *pBCCI_Interface;


#endif // __BCCI_TYPES_H
