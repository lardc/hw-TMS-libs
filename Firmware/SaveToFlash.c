// Header
#include "SaveToFlash.h"

// Includes
#include "Flash.h"
#include "ZwSysCtrl.h"
#include "SysConfig.h"
#include "StorageDescription.h"
#include "Global.h"

// Forward functions
Int16U STF_StartAddressShift(Int16U Index);
Int16U STF_GetTypeLength(DataType CurrentType);
Int32U STF_ShiftStorageEnd();
Int16U strlen(const char* string);

// Functions
//
void STF_AssignPointer(Int16U Index, Int32U Pointer)
{
	if(Index < StorageSize)
		TablePointers[Index] = Pointer;
}
// ----------------------------------------

void STF_SaveDiagData()
{
	ZwSystem_DisableDog();
	DINT;

	Int32U ShiftedAddress = STF_ShiftStorageEnd();
	Int16U MaxDataLength = 0;

	Int16U i;
	for (i = 0; i < StorageSize; ++i)
		MaxDataLength += StorageDescription[i].Length * STF_GetTypeLength(StorageDescription[i].Type) + 4 + strlen(StorageDescription[i].Description);

	if (ShiftedAddress + MaxDataLength >= FLASH_END_ADDR)
		return;

	for(i = 0; i < StorageSize; i++)
	{
		Int16U DescriptionLength = strlen(StorageDescription[i].Description);
		static Int16U DescriptionHeader[2] = {DT_Char};
		DescriptionHeader[1] = DescriptionLength;

		// Запись заголовка описания
		Status = Flash_Program((pInt16U)ShiftedAddress, (pInt16U)DescriptionHeader, 2,
				(FLASH_ST *)&FlashStatus);
		ShiftedAddress += 2;

		// Запись описания
		Status = Flash_Program((pInt16U)ShiftedAddress, (pInt16U)StorageDescription[i].Description,
				DescriptionLength, (FLASH_ST *)&FlashStatus);
		ShiftedAddress += DescriptionLength;

		// Запись заголовка данных
		Int16U DataHeader[2] = {StorageDescription[i].Type, StorageDescription[i].Length};
		Status = Flash_Program((pInt16U)ShiftedAddress,
				(pInt16U)DataHeader, 2, (FLASH_ST *)&FlashStatus);
		ShiftedAddress += 2;

		// Запись данных при наличии указателя
		if(TablePointers[i])
		{
			Int16U DataWriteLength = StorageDescription[i].Length * STF_GetTypeLength(StorageDescription[i].Type);

			Status = Flash_Program((pInt16U)ShiftedAddress,
					(pInt16U)TablePointers[i], DataWriteLength, (FLASH_ST *)&FlashStatus);
			ShiftedAddress += DataWriteLength;
		}
	}

	EINT;
	ZwSystem_EnableDog(SYS_WD_PRESCALER);
}
// ----------------------------------------

Int16U STF_GetTypeLength(DataType CurrentType)
{
	return (CurrentType == DT_Int32U || CurrentType == DT_Int32S || CurrentType == DT_Float) ? 2 : 1;
}
// ----------------------------------------

Int32U STF_ShiftStorageEnd()
{
	Int32U StoragePointer = FLASH_START_ADDR;

	while (*(pInt16U)StoragePointer != 0xFFFF)
	{
		DataType CurrentType = (DataType)(*(pInt16U)StoragePointer);

		if (CurrentType > DT_Float)
			break;

		Int16U TypeLength = STF_GetTypeLength(CurrentType);
		++StoragePointer;

		Int16U Length = *(pInt16U)StoragePointer * (Int16U)TypeLength;
		StoragePointer += Length + 1;
	}
	return StoragePointer;
}
// ----------------------------------------

void STF_EraseDataSector()
{
	ZwSystem_DisableDog();
	DINT;
	Flash_Erase(FLASH_SECTOR_MASK, (FLASH_ST *)&FlashStatus);
	EINT;
	ZwSystem_EnableDog(SYS_WD_PRESCALER);
}
// ----------------------------------------

Int16U strlen(const char* string)
{
	int n = -1;
	const char* s = string;
	do n++; while (*s++);
	return n;
}
// ----------------------------------------
