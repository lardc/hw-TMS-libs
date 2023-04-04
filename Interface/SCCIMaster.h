// -----------------------------------------
// SCCI-Master communication interface
// ----------------------------------------

#ifndef __SCCIM_H
#define __SCCIM_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "xCCICommon.h"
#include "SCCITypes.h"


#ifdef SCCI_MASTER

// Types
//
// SCCI instance state
typedef struct __SCCIM_Interface
{
	pSCCI_IOConfig IOConfig;
	Int16U MessageBuffer[xCCI_BUFFER_SIZE];
	Int32U TimeoutValueTicks;
	volatile Int64U *pTimerCounter;
} SCCIM_Interface, *pSCCIM_Interface;

// Functions
//
// Init interface instance
void SCCIM_Init(pSCCIM_Interface Interface, pSCCI_IOConfig IOConfig, Int32U MessageTimeoutTicks, volatile Int64U *pTimer);
// Write 16-bit value
Int16U SCCIM_Write16(pSCCIM_Interface Interface, Int16U NodeID, Int16U Address, Int16U Value);
// Read 16-bit value
Int16U SCCIM_Read16(pSCCIM_Interface Interface, Int16U NodeID, Int16U Address, pInt16U Value);
// Read array of 16-bit values
Int16U SCCIM_ReadArray16(pSCCIM_Interface Interface, Int16U NodeID, Int16U Endpoint, Int16U MaxCount, pInt16U Out,
		pInt16U OutCounter);
Int16U SCCIM_ReadArray16Callback(pSCCIM_Interface Interface, Int16U NodeID, Int16U Endpoint, Int16U MaxCount,
		pInt16U Out, pInt16U OutCounter, void (*Callback)());
// Call action
Int16U SCCIM_Call(pSCCIM_Interface Interface, Int16U NodeID, Int16U Action);

#endif // SCCI_MASTER


#endif // __SCCIM_H
