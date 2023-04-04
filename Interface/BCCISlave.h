// -----------------------------------------
// BCCI-Slave communication interface
// ----------------------------------------

#ifndef __BCCIS_H
#define __BCCIS_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "BCCIParams.h"
#include "xCCICommon.h"
#include "BCCITypes.h"

#define BCCI_BLOCK_MAX_VAL_16_W 3

// Functions
//
// Init interface instance
void BCCI_Init(pBCCI_Interface Interface, pBCCI_IOConfig IOConfig, pxCCI_ServiceConfig ServiceConfig,
 			   pInt16U DataTable, Int16U DataTableSize, void *ArgumentForCallback);
// Process packets
void BCCI_Process(pBCCI_Interface Interface, Boolean MaskStateChangeOperations);


// Internal functions
//
// Create protected area
Int16U inline BCCI_AddProtectedArea(pBCCI_Interface Interface, Int16U StartAddress, Int16U EndAddress)
{
	return xCCI_AddProtectedArea(&(Interface->ProtectionAndEndpoints), StartAddress, EndAddress);
}
//
// Remove protected area
Boolean inline BCCI_RemoveProtectedArea(pBCCI_Interface Interface, Int16U AreaIndex)
{
	return xCCI_RemoveProtectedArea(&(Interface->ProtectionAndEndpoints), AreaIndex);
}
//
// Register read endpoint 16-bit source callback
Boolean inline BCCI_RegisterReadEndpoint16(pBCCI_Interface Interface, Int16U Endpoint,
								    xCCI_FUNC_CallbackReadEndpoint16 ReadCallback)
{
	return xCCI_RegisterReadEndpoint16(&(Interface->ProtectionAndEndpoints), Endpoint, ReadCallback);
}
//
// Register read endpoint 32-bit source callback
Boolean inline BCCI_RegisterReadEndpoint32(pBCCI_Interface Interface, Int16U Endpoint,
								    xCCI_FUNC_CallbackReadEndpoint32 ReadCallback)
{
	return xCCI_RegisterReadEndpoint32(&(Interface->ProtectionAndEndpoints), Endpoint, ReadCallback);
}
//
// Register write endpoint 16-bit destination callback
Boolean inline BCCI_RegisterWriteEndpoint16(pBCCI_Interface Interface, Int16U Endpoint,
									 xCCI_FUNC_CallbackWriteEndpoint16 WriteCallback)
{
	return xCCI_RegisterWriteEndpoint16(&(Interface->ProtectionAndEndpoints), Endpoint, WriteCallback);
}
//
// Register write endpoint 32-bit destination callback
Boolean inline BCCI_RegisterWriteEndpoint32(pBCCI_Interface Interface, Int16U Endpoint,
									 xCCI_FUNC_CallbackWriteEndpoint32 WriteCallback)
{
	return xCCI_RegisterWriteEndpoint32(&(Interface->ProtectionAndEndpoints), Endpoint, WriteCallback);
}


#endif // __BCCIS_H
