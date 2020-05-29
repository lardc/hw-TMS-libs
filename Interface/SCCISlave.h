// -----------------------------------------
// SCCI-Slave communication interface
// ----------------------------------------

#ifndef __SCCIS_H
#define __SCCIS_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "SCCIParams.h"
#include "xCCICommon.h"
#include "SCCITypes.h"


// Functions
//
// Init interface instance
#ifdef RS232_X32_EP
void SCCI_Init(pSCCI_Interface Interface, pSCCI_IOConfig IOConfig, pxCCI_ServiceConfig ServiceConfig,
 			   pInt16U DataTable, Int16U DataTableSize, Int32U MessageTimeoutTicks,
 			   void *ArgumentForCallback16, void *ArgumentForCallback32);
#else
void SCCI_Init(pSCCI_Interface Interface, pSCCI_IOConfig IOConfig, pxCCI_ServiceConfig ServiceConfig,
 			   pInt16U DataTable, Int16U DataTableSize, Int32U MessageTimeoutTicks,
 			   void *ArgumentForCallback16);
#endif

// Process packets
void SCCI_Process(pSCCI_Interface Interface, Int64U CurrentTickCount, Boolean MaskStateChangeOperations);


// Internal functions
//
// Create protected area
Int16U inline SCCI_AddProtectedArea(pSCCI_Interface Interface, Int16U StartAddress, Int16U EndAddress)
{
	return xCCI_AddProtectedArea(&(Interface->ProtectionAndEndpoints), StartAddress, EndAddress);
}
//
// Remove protected area
Boolean inline SCCI_RemoveProtectedArea(pSCCI_Interface Interface, Int16U AreaIndex)
{
	return xCCI_RemoveProtectedArea(&(Interface->ProtectionAndEndpoints), AreaIndex);
}
//
// Register read endpoint 16-bit source callback
Boolean inline SCCI_RegisterReadEndpoint16(pSCCI_Interface Interface, Int16U Endpoint,
								    xCCI_FUNC_CallbackReadEndpoint16 ReadCallback)
{
	return xCCI_RegisterReadEndpoint16(&(Interface->ProtectionAndEndpoints), Endpoint, ReadCallback);
}
//
// Register read endpoint 32-bit source callback
Boolean inline SCCI_RegisterReadEndpoint32(pSCCI_Interface Interface, Int16U Endpoint,
								    xCCI_FUNC_CallbackReadEndpoint32 ReadCallback)
{
	return xCCI_RegisterReadEndpoint32(&(Interface->ProtectionAndEndpoints), Endpoint, ReadCallback);
}
//
// Register write endpoint 16-bit destination callback
Boolean inline SCCI_RegisterWriteEndpoint16(pSCCI_Interface Interface, Int16U Endpoint,
									 xCCI_FUNC_CallbackWriteEndpoint16 WriteCallback)
{
	return xCCI_RegisterWriteEndpoint16(&(Interface->ProtectionAndEndpoints), Endpoint, WriteCallback);
}
//
// Register write endpoint 32-bit destination callback
Boolean inline SCCI_RegisterWriteEndpoint32(pSCCI_Interface Interface, Int16U Endpoint,
									 xCCI_FUNC_CallbackWriteEndpoint32 WriteCallback)
{
	return xCCI_RegisterWriteEndpoint32(&(Interface->ProtectionAndEndpoints), Endpoint, WriteCallback);
}


#endif // __SCCIS_H
