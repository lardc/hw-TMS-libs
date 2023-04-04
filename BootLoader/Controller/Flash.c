// -----------------------------------------
// Flash functions
// ----------------------------------------

// Header
//
#include "Flash.h"

//Variables
//
volatile FLASH_ST	FlashStatus;
volatile float32	Version = 0;		// Version of the API in floating point
volatile Uint16		VersionHex = 0;		// Version of the API in decimal encoded hex
volatile Uint16		Status = 0;
volatile Uint16		*PrgWrFlag =   ((Uint16*)0x0003FF);
volatile Uint16		*PrgAdrStart = ((Uint16*)0x3EC000);
volatile Uint16		*PrgAdrCount = 0;
//
extern Uint32 Flash_CPUScaleFactor;

// Forward functions
//
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);

// Functions
//
void FLASH_Init()
{
	// Init
	MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);
	Flash_CPUScaleFactor = SCALE_FACTOR;

	// Read info
	VersionHex = Flash_APIVersionHex();
	Version = Flash_APIVersion();
}
// ----------------------------------------

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
       *DestAddr++ = *SourceAddr++;
}
// -----------------------------------------

// No more.
