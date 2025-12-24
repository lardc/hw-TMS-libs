// Header
#include "SaveToFlash.h"

// Includes
#include "Flash.h"
#include "ZwSysCtrl.h"
#include "ZwUtils.h"
#include "SysConfig.h"
#include "StorageDescription.h"
#include "Global.h"

// Forward functions
Int16U STF_StartAddressShift(Int16U Index);
Int16U STF_GetTypeLength(DataType CurrentType);
Int32U STF_ShiftStorageEnd();
Int32U STF_ShiftCounterStorageEnd();
Int32U STF_ReadCounter32(Int32U Address);
Int16U strlen(const char* string);

typedef enum __ReadCountersStateMachine
{
	RCSM_DescriptionType = 0,
	RCSM_DescriptionLength,
	RCSM_Description,
	RCSM_DataType,
	RCSM_DataLength,
	RCSM_Data,
} ReadCountersStateMachine;

typedef enum __SaveCountersStateMachine
{
	SCSM_CheckTimer = 0,
	SCSM_CheckDataChange,
	SCSM_CheckFlash,
	SCSM_ClearSectorE,
	SCSM_ClearSectorF,
	SCSM_ClearSectorG,
	SCSM_ClearSectorH,
	SCSM_SaveData,
} SaveCountersStateMachine;

ReadCountersStateMachine CurrentState = RCSM_DescriptionType;
SaveCountersStateMachine SaveState = SCSM_CheckTimer;
Int64U SaveTimer = 0; // Последняя отметка времени автосохранения
Int16U LineNumber;
Int16U DataPosition;
Int32U FlashPosition;
Int32U ShiftedAddress;

#ifdef FLASH_COUNTER_START_ADDR
static Boolean SubstituteZeroForErased = FALSE;
static Int64U ErasePauseTimeout = 0; // момент времени, когда можно выполнять следующее стирание

// Functions
//
// Функции для работы со счётчиками
void STF_ResetStateMachine()
{
	CurrentState = RCSM_DescriptionType;
	SubstituteZeroForErased = FALSE;
	LineNumber = 0;
	DataPosition = 0;
	FlashPosition = STF_ShiftCounterStorageEnd();

	if(FlashPosition != FLASH_COUNTER_START_ADDR)
		FlashPosition -= CounterStorageSize * 2; // each 32-bit counter occupies 2 words (16-bit)
}
// ----------------------------------------

Int16U STF_ReadCounter()
{
	Int16U RetVal = 0;
	switch(CurrentState)
	{
		case RCSM_DescriptionType:
			RetVal = DT_Char;
			CurrentState = RCSM_DescriptionLength;
			DataPosition++;

			if (LineNumber == CounterStorageSize)
				return 0xFFFF;

			break;

		case RCSM_DescriptionLength:
			RetVal = strlen(CounterStorageDescription[LineNumber].Description);
			CurrentState = RCSM_Description;
			DataPosition = 0;
			break;

		case RCSM_Description:
			RetVal = CounterStorageDescription[LineNumber].Description[DataPosition++];
			if (CounterStorageDescription[LineNumber].Description[DataPosition] == '\0')
				CurrentState = RCSM_DataType;
			break;

		case RCSM_DataType:
			RetVal = DT_Int32U;
			CurrentState = RCSM_DataLength;
			break;

		case RCSM_DataLength:
			RetVal = 1;
			CurrentState = RCSM_Data;
			DataPosition = 0;
			break;

		case RCSM_Data:
			if(DataPosition == 0)
			{
				Int16U low = *(pInt16U)FlashPosition;
				Int16U high = *(pInt16U)(FlashPosition + 1);
				SubstituteZeroForErased = (low == 0xFFFF || high == 0xFFFF);
			}

			RetVal = SubstituteZeroForErased ? 0 : *(pInt16U)FlashPosition;

			FlashPosition++;
			DataPosition++;

			if (DataPosition == 2)
			{
				CurrentState = RCSM_DescriptionType;
				LineNumber++;
				SubstituteZeroForErased = FALSE;
			}
			break;
	}

	return RetVal;
}
// ----------------------------------------

void STF_EraseCounterDataSector()
{
	ZwSystem_DisableDog();
	DINT;
	Flash_Erase(FLASH_COUNTER_SECTOR_MASK, (FLASH_ST *)&FlashStatus);
	EINT;
	ZwSystem_EnableDog(SYS_WD_PRESCALER);
}
// ----------------------------------------

void STF_EraseCounterDataSingleSector(Uint16 Sector)
{
	ZwSystem_DisableDog();
	DINT;
	Flash_Erase(Sector, (FLASH_ST *)&FlashStatus);
	EINT;
	ZwSystem_EnableDog(SYS_WD_PRESCALER);
}
// ----------------------------------------

void STF_LoadCounters()
{
	Int32U StoragePointer = STF_ShiftCounterStorageEnd();
	Boolean SavedData = StoragePointer != FLASH_COUNTER_START_ADDR;

	if(SavedData)
		StoragePointer -= CounterStorageSize * 2;

	Int16U i;
	for (i = 0; i < CounterStorageSize; ++i)
	{
		Int32U Value = SavedData ? STF_ReadCounter32(StoragePointer) : 0;
		Value = (Value == INT32U_MAX) ? 0 : Value;

		CounterTablePointers[i].Value = *(pInt32U)CounterTablePointers[i].Address = Value;
		StoragePointer += 2;
	}
}
// ----------------------------------------

void STF_AssignCounterPointer(Int16U Index, Int32U Pointer)
{
	if (Index < CounterStorageSize)
		CounterTablePointers[Index] = (CounterData){0, Pointer};
}
// ----------------------------------------

void STF_SaveCounterData()
{
	// Проверка на то, изменились ли данные с момента последней записи
	Int16U i;
	for (i = 0; i < CounterStorageSize; ++i)
	{
		if (CounterTablePointers[i].Value != *(pInt32U)CounterTablePointers[i].Address)
			break;
	}
	if (i == CounterStorageSize)
		return;

	ZwSystem_DisableDog();
	DINT;

	Int32U ShiftedAddress = STF_ShiftCounterStorageEnd();

	// Проверка на свободное место в памяти
	if (ShiftedAddress + CounterStorageSize * 2 > FLASH_COUNTER_END_ADDR)
	{
		STF_EraseCounterDataSector();
		ShiftedAddress = FLASH_COUNTER_START_ADDR;
	}

	for (i = 0; i < CounterStorageSize; ++i)
	{
		Flash_Program((pInt16U)ShiftedAddress, (pInt16U)CounterTablePointers[i].Address, 2,
				(FLASH_ST *)&FlashStatus);
		ShiftedAddress += 2;
	}

	EINT;
	ZwSystem_EnableDog(SYS_WD_PRESCALER);
}
// ----------------------------------------

void STF_SaveCounterDataTimed(volatile Int64U TimeCounter)
{
	Int16U i;
	switch(SaveState)
	{
		case SCSM_CheckTimer:
			if (TimeCounter - SaveTimer >= SAVE_TIMEOUT)
			{
				SaveTimer = TimeCounter;
				SaveState = SCSM_CheckDataChange;
			}
			break;

		case SCSM_CheckDataChange:
			for (i = 0; i < CounterStorageSize; ++i)
			{
				if (CounterTablePointers[i].Value != *(pInt32U) CounterTablePointers[i].Address)
				{
					SaveState = SCSM_CheckFlash;
					break;
				}
			}
			if (i == CounterStorageSize)
				SaveState = SCSM_CheckTimer;
			break;

		case SCSM_CheckFlash:
			ShiftedAddress = STF_ShiftCounterStorageEnd();
			if (ShiftedAddress + CounterStorageSize * 2 > FLASH_COUNTER_END_ADDR)
			{
				SaveState = SCSM_ClearSectorE;
				ShiftedAddress = FLASH_COUNTER_START_ADDR;
				ErasePauseTimeout = TimeCounter + ERASE_PAUSE;
			}
			else
				SaveState = SCSM_SaveData;
			break;

		case SCSM_ClearSectorE:
			if (TimeCounter >= ErasePauseTimeout)
			{
				if (FLASH_COUNTER_SECTOR_MASK & SECTORE)
				{
					STF_EraseCounterDataSingleSector(SECTORE);
					ErasePauseTimeout = TimeCounter + ERASE_PAUSE;
				}
				SaveState = SCSM_ClearSectorF;
			}
			break;

		case SCSM_ClearSectorF:
			if (TimeCounter >= ErasePauseTimeout)
			{
				if (FLASH_COUNTER_SECTOR_MASK & SECTORF)
				{
					STF_EraseCounterDataSingleSector(SECTORF);
					ErasePauseTimeout = TimeCounter + ERASE_PAUSE;
				}
				SaveState = SCSM_ClearSectorG;
			}
			break;

		case SCSM_ClearSectorG:
			if (TimeCounter >= ErasePauseTimeout)
			{
				if (FLASH_COUNTER_SECTOR_MASK & SECTORG)
				{
					STF_EraseCounterDataSingleSector(SECTORG);
					ErasePauseTimeout = TimeCounter + ERASE_PAUSE;
				}
				SaveState = SCSM_ClearSectorH;
			}
			break;

		case SCSM_ClearSectorH:
			if (TimeCounter >= ErasePauseTimeout)
			{
				if (FLASH_COUNTER_SECTOR_MASK & SECTORH)
					STF_EraseCounterDataSingleSector(SECTORH);
				SaveState = SCSM_SaveData;
			}
			break;

		case SCSM_SaveData:
			ZwSystem_DisableDog();
			DINT;
			for (i = 0; i < CounterStorageSize; ++i)
			{
				Flash_Program((pInt16U)ShiftedAddress, (pInt16U)CounterTablePointers[i].Address, 2,
						(FLASH_ST *)&FlashStatus);
				ShiftedAddress += 2;
			}
			EINT;
			ZwSystem_EnableDog(SYS_WD_PRESCALER);
			SaveState = SCSM_CheckTimer;
			break;
	}
}
// ----------------------------------------

Int32U STF_ShiftCounterStorageEnd()
{
	Int32U i;
	for (i = FLASH_COUNTER_START_ADDR; i < FLASH_COUNTER_END_ADDR; i += 2)
	{
		if(STF_ReadCounter32(i) == 0xFFFFFFFF)
			return i;
	}
	return FLASH_COUNTER_END_ADDR;
}
// ----------------------------------------

Int32U STF_ReadCounter32(Int32U Address)
{
	Int16U Low = *(pInt16U)Address;
	Int16U High = *(pInt16U)(Address + 1);
	return ((Int32U)High << 16) | Low;
}
// ----------------------------------------
#endif

// Функции для работы с диагностическими данными
#ifdef FLASH_DIAG_START_ADDR
void STF_SaveDiagData()
{
	ZwSystem_DisableDog();
	DINT;

	Int32U ShiftedAddress = STF_ShiftStorageEnd();
	Int16U MaxDataLength = 0;

	Int16U i;
	for (i = 0; i < StorageSize; ++i)
		MaxDataLength += StorageDescription[i].Length * STF_GetTypeLength(StorageDescription[i].Type) + 4 + strlen(StorageDescription[i].Description);

	if (ShiftedAddress + MaxDataLength >= FLASH_DIAG_END_ADDR)
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

		// Копирование описания в RAM-массив
		Int16U tmpDescription[MAX_DESCRIPTION_LEN] = {0};
		MemCopy16((pInt16U)StorageDescription[i].Description, tmpDescription, DescriptionLength);

		// Запись описания
		Status = Flash_Program((pInt16U)ShiftedAddress, tmpDescription, DescriptionLength, (FLASH_ST *)&FlashStatus);
		ShiftedAddress += DescriptionLength;

		// Запись заголовка данных
		Int16U DataHeader[2] = {StorageDescription[i].Type, StorageDescription[i].Length};
		Status = Flash_Program((pInt16U)ShiftedAddress, (pInt16U)DataHeader, 2, (FLASH_ST *)&FlashStatus);
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

void STF_AssignPointer(Int16U Index, Int32U Pointer)
{
	if(Index < StorageSize)
		TablePointers[Index] = Pointer;
}
// ----------------------------------------

Int16U STF_GetTypeLength(DataType CurrentType)
{
	return (CurrentType == DT_Int32U || CurrentType == DT_Int32S || CurrentType == DT_Float) ? 2 : 1;
}
// ----------------------------------------

Int32U STF_ShiftStorageEnd()
{
	Int32U StoragePointer = FLASH_DIAG_START_ADDR;

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
	Flash_Erase(FLASH_DIAG_SECTOR_MASK, (FLASH_ST *)&FlashStatus);
	EINT;
	ZwSystem_EnableDog(SYS_WD_PRESCALER);
}
// ----------------------------------------
#endif

Int16U strlen(const char* string)
{
	int n = -1;
	const char* s = string;
	do n++; while (*s++);
	return n;
}
// ----------------------------------------
