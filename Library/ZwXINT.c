// -----------------------------------------
// XINT module management functions
// ----------------------------------------

// Header
#include "ZwXINT.h"

//----------------------------------------
#pragma DATA_SECTION(XIntruptRegs,"XIntruptRegsFile");
volatile struct XINTRUPT_REGS XIntruptRegs;

void ZwXINT1_Init(Int16U PinNumber, Int16U Edge)
{
	EALLOW;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = PinNumber;
	EDIS;

	XIntruptRegs.XINT1CR.bit.POLARITY = Edge;
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;   // Enable external interrupt
}
//-------------------------------------------

#pragma DATA_SECTION(XIntruptRegs,"XIntruptRegsFile");
volatile struct XINTRUPT_REGS XIntruptRegs;

// No more.
