// -----------------------------------------
// API for Flash initialization
// ----------------------------------------

#ifndef __ZW_FLASH_H
#define __ZW_FLASH_H

// Include
#include "stdinc.h"
//
// IO defs
#include "_ZwFlash_Defs.h"

// Functions
//
// Init Flash - must be called resided to RAM
void InitFlash(Int16U FlashWait, Int16U OTPWait);

#endif
