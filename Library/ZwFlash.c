// -----------------------------------------
// API for Flash initialization
// ----------------------------------------

// Header
#include "ZwFlash.h"

// Functions
//
#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(InitFlash, "ramfuncs");
#endif
void InitFlash(Int16U FlashWait, Int16U OTPWait)
{
	EALLOW;
    
	//Enable Flash Pipeline mode to improve performance
    //of code executed from Flash.
    FlashRegs.FOPT.bit.ENPIPE = 1;

    //                CAUTION
    // Minimum waitstates required for the flash operating
    // at a given CPU rate must be characterized by TI.
    // Refer to the datasheet for the latest information.

    // Set the Paged waitstate for the Flash
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = FlashWait;

    // Set the Random waitstate for the Flash
    FlashRegs.FBANKWAIT.bit.RANDWAIT = FlashWait;

    // Set the waitstate for the OTP
    FlashRegs.FOTPWAIT.bit.OTPWAIT = OTPWait;

    //                CAUTION
    // ONLY THE DEFAULT VALUE FOR THESE 2 REGISTERS SHOULD BE USED
    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
    
	EDIS;

    // Force a pipeline flush to ensure that the write to
    // the last register configured occurs before returning.
    asm(" RPT #7 || NOP");
}

//----------------------------------------
#pragma DATA_SECTION(FlashRegs,"FlashRegsFile");
volatile struct FLASH_REGS FlashRegs;

// No more.
