// -----------------------------------------
// Flash functions
// ----------------------------------------

#ifndef __FLASH_H
#define __FLASH_H

// Include
//
#include "stdinc.h"
//
#include "Flash280x_API_Library.h"
#include "Flash280x_API_Config.h"

// Variables
//
extern volatile Uint16  Status;
extern volatile Uint16 *PrgWrFlag;
extern volatile Uint16 *PrgAdrStart;
extern volatile Uint16 *PrgAdrCount;
extern volatile FLASH_ST FlashStatus;


// Functions
//
void FLASH_Init();


#endif // __FLASH_H
