// -----------------------------------------
// Controller logic
// ----------------------------------------

// Header
#include "Controller.h"
//
// Includes
#include "SysConfig.h"
#include "ZwDSP.h"
#include "DataTable.h"
#include "SCCISlave.h"
#include "DeviceProfile.h"
#include "Flash280x_API_Library.h"
#include "Flash280x_API_Config.h"

// Definitions
//
#define VALUES_WR_SIZE		300

// Variables
//
static volatile Boolean CycleActive = FALSE;
static Int16U MEMBUF_Values_Write[VALUES_WR_SIZE], MEMBUF_ValuesWrite_Counter = 0;
volatile Int64U CONTROL_TimeCounter = 0, CurrentTimeout = BOOT_LOADER_DELAY;
volatile Boolean WaitForFWUpload = FALSE;
volatile Boolean ResetExecute = FALSE;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_EraseFlashSector(Int16U Sector, pInt16U UserError);

// Functions
//
void CONTROL_Init()
{
	Int16U EPWriteIndexes[EP_WRITE_COUNT] = { EP16_WR };
	Int16U EPWriteSized[EP_WRITE_COUNT] = { VALUES_WR_SIZE };
	pInt16U EPWriteCounters[EP_WRITE_COUNT] = { (pInt16U)&MEMBUF_ValuesWrite_Counter };
	pInt16U EPWriteDatas[EP_WRITE_COUNT] = { MEMBUF_Values_Write };

	// Data-table EPROM service configuration
	EPROMServiceConfig EPROMService = { NULL, NULL };

	// Init data table
	DT_Init(EPROMService, FALSE);

	// Device profile initialization
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPWriteService(EPWriteIndexes, EPWriteSized, EPWriteCounters, EPWriteDatas);
	// Reset control values
	DEVPROFILE_ResetControlSection();
}
// ----------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();

	if (CONTROL_TimeCounter > CurrentTimeout)
	{
		if (!WaitForFWUpload)
		{
#ifdef BOOT_ALT_ADDRESS
			asm(" LB 0x3E8000");
#else
			asm(" LB 0x3EC000");
#endif
		}
		if (ResetExecute) asm(" RESET");
	}
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(CONTROL_NotifyCANFault, "ramfuncs");
#endif
void CONTROL_NotifyCANFault(ZwCAN_SysFlags Flag)
{
	DEVPROFILE_NotifyCANFault(Flag);
}
// ----------------------------------------

void CONTROL_EraseFlashSector(Int16U Sector, pInt16U UserError)
{
	DINT;

	Status = Flash_Erase(Sector, (FLASH_ST *)&FlashStatus);
	*UserError = (Status) ? ERR_FLASH_ERROR : ERR_NO_ERROR;

	EINT;
}
// ----------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_FL_ERASE_C:
			CONTROL_EraseFlashSector(SECTORC, UserError);
			break;
		case ACT_FL_ERASE_D:
			CONTROL_EraseFlashSector(SECTORD, UserError);
			break;
		case ACT_FL_ERASE_E:
			CONTROL_EraseFlashSector(SECTORE, UserError);
			break;
		case ACT_FL_ERASE_F:
			CONTROL_EraseFlashSector(SECTORF, UserError);
			break;
		case ACT_FL_ERASE_G:
			CONTROL_EraseFlashSector(SECTORG, UserError);
			break;
		case ACT_FL_ERASE_H:
			CONTROL_EraseFlashSector(SECTORH, UserError);
			break;
		case ACT_FL_RESET_COUNTERS:
			DataTable[REG_XOR_TMS] = 0;
			DataTable[REG_XOR_PC] = 0;
			DEVPROFILE_ResetWriteScopes(0, EP16_WR_MASK);
			break;
		case ACT_FL_SET_WR_ADDRESS:
			{
				Int32U StartAddress;

				StartAddress = DataTable[REG_ADDRESS_H];
				StartAddress <<= 16;
				StartAddress |= DataTable[REG_ADDRESS_L];
				PrgAdrCount = (Uint16 *)StartAddress;

				DataTable[REG_XOR_TMS] ^= DataTable[REG_ADDRESS_H];
				DataTable[REG_XOR_TMS] ^= DataTable[REG_ADDRESS_L];
			}
			break;
		case ACT_FL_WRITE:
			{
				if (MEMBUF_ValuesWrite_Counter > 0)
				{
					Int16U i;

					DINT;
					Status = Flash_Program((Uint16 *)PrgAdrCount, MEMBUF_Values_Write, MEMBUF_ValuesWrite_Counter, (FLASH_ST *)&FlashStatus);
					EINT;

					PrgAdrCount += MEMBUF_ValuesWrite_Counter;

					for (i = 0; i < MEMBUF_ValuesWrite_Counter; i++)
						DataTable[REG_XOR_TMS] ^= MEMBUF_Values_Write[i];

					if (Status)
					{
						*UserError = ERR_FLASH_ERROR;
						DataTable[REG_XOR_TMS] = 0;
					}

					DEVPROFILE_ResetWriteScopes(0, EP16_WR_MASK);
				}
			}
			break;
		case ACT_FL_VALIDATE:
			{
				if (DataTable[REG_XOR_TMS] == DataTable[REG_XOR_PC] && DataTable[REG_XOR_TMS] != 0)
				{
					CurrentTimeout = CONTROL_TimeCounter + BOOT_LOADER_RESP_DELAY;
					WaitForFWUpload = FALSE;
				}
				else
					*UserError = ERR_CRC;
			}
			break;
		case ACT_FL_SET_REQUEST:
			WaitForFWUpload = TRUE;
			break;
		case ACT_FL_CLR_REQUEST:
			*PrgWrFlag = 0;
			break;
		case ACT_FL_RESET:
			{
				CurrentTimeout = CONTROL_TimeCounter + BOOT_LOADER_RESP_DELAY;
				ResetExecute = TRUE;
			}
			break;
		default:
			return FALSE;
	}

	return TRUE;
}
// ----------------------------------------

// No more.
