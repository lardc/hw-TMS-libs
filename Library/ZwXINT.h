// -----------------------------------------
// XINT module management functions
// ----------------------------------------

#ifndef __ZW_XINT_H
#define __ZW_XINT_H

// Inlcude
#include "stdinc.h"
//
// IO defs
#include "_ZwXINT_Defs.h"
#include "_ZwGPIO_Defs.h"

// ISR postfix
#define XINT_ISR_DONE    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//
#define TRIG_RISE_EDGE	1
#define TRIG_FALL_EDGE	2
//

// Functions
void ZwXINT1_Init(Int16U PinNumber, Int16U Edge);

#endif // __ZW_XINT_H
