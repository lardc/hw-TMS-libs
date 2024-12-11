// -----------------------------------------
// SCCI communication interface
// ----------------------------------------

// Header
#include "xCCICommon.h"


Int16U xCCI_AddProtectedArea(pxCCI_ProtectionAndEndpoints PAE, Int16U StartAddress, Int16U EndAddress)
{
	if(PAE->ProtectedAreasUsed == xCCI_MAX_PROTECTED_AREAS)
		return 0;

	// Save parameters
	PAE->ProtectedAreas[PAE->ProtectedAreasUsed].StartAddress = StartAddress;
	PAE->ProtectedAreas[PAE->ProtectedAreasUsed].EndAddress = EndAddress;

	return 1 + (PAE->ProtectedAreasUsed++);
}
// ----------------------------------------

Boolean xCCI_RemoveProtectedArea(pxCCI_ProtectionAndEndpoints PAE, Int16U AreaIndex)
{
	Int16U i;

	if(AreaIndex > PAE->ProtectedAreasUsed)
		return FALSE;

	// Update indexes
	--AreaIndex;
	--(PAE->ProtectedAreasUsed);
	// Shift data
	for(i = AreaIndex; i < PAE->ProtectedAreasUsed; ++i)
		PAE->ProtectedAreas[i] = PAE->ProtectedAreas[i + 1];

	return TRUE;
}
// ----------------------------------------

Boolean xCCI_EndpointIndex(pxCCI_ProtectionAndEndpoints PAE, Int16U Name, pInt16U Index)
{
	Int16U i;
	for (i = 0; i < xCCI_MAX_READ_ENDPOINTS; ++i)
	{
		if (PAE->Endpoints[i].Name == Name)
		{
			*Index = i;
			return TRUE;
		}
	}

	return FALSE;
}

Boolean xCCI_RegisterReadEndpoint16(pxCCI_ProtectionAndEndpoints PAE, Int16U Endpoint,
								    xCCI_FUNC_CallbackReadEndpoint16 ReadCallback)
{
	Int16U i;
	for (i = 0; i < xCCI_MAX_READ_ENDPOINTS; ++i)
	{
		if (!PAE->Endpoints[i].Initialized)
		{
			PAE->Endpoints[i].ReadEndpoint16 = ReadCallback;
			PAE->Endpoints[i].Name = Endpoint;
			PAE->Endpoints[i].Initialized = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}
// ----------------------------------------

Boolean xCCI_RegisterReadEndpoint32(pxCCI_ProtectionAndEndpoints PAE, Int16U Endpoint,
								    xCCI_FUNC_CallbackReadEndpoint32 ReadCallback)
{
	if(Endpoint < xCCI_MAX_READ_ENDPOINTS)
	{
		PAE->ReadEndpoints32[Endpoint] = ReadCallback;
		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------

Boolean xCCI_RegisterWriteEndpoint16(pxCCI_ProtectionAndEndpoints PAE, Int16U Endpoint,
									 xCCI_FUNC_CallbackWriteEndpoint16 WriteCallback)
{
	if(Endpoint < xCCI_MAX_WRITE_ENDPOINTS + 1)
	{
		PAE->WriteEndpoints16[Endpoint] = WriteCallback;
		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------

Boolean xCCI_RegisterWriteEndpoint32(pxCCI_ProtectionAndEndpoints PAE, Int16U Endpoint,
									 xCCI_FUNC_CallbackWriteEndpoint32 WriteCallback)
{
	if(Endpoint < xCCI_MAX_WRITE_ENDPOINTS + 1)
	{
		PAE->WriteEndpoints32[Endpoint] = WriteCallback;
		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------

Boolean xCCI_InProtectedZone(pxCCI_ProtectionAndEndpoints PAE, Int16U Address)
{
	Int16U i;
	Boolean result = FALSE;

	for(i = 0; i < PAE->ProtectedAreasUsed; ++i)
		if((Address >= PAE->ProtectedAreas[i].StartAddress) && (Address <= PAE->ProtectedAreas[i].EndAddress))
		{
			result = TRUE;
			break;
		}

	return result;
}

// No more
