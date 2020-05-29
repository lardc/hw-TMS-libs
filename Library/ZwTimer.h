// -----------------------------------------
// Timer management functions
// ----------------------------------------

#ifndef __ZW_TIMER_H
#define __ZW_TIMER_H

// Include
#include "stdinc.h"
//
// IO defs
#include "_ZwTimer_Defs.h"

// Functions
//
// Initialize CPU timer 0
void ZwTimer_InitT0();
// Set timer 0 period
void ZwTimer_SetT0(Int32U Period_uSec);
// Enable interrupt processing on T0
void ZwTimer_EnableInterruptsT0(Boolean Enable);
// Initialize CPU timer 1
void ZwTimer_InitT1();
// Set timer 1 period
void ZwTimer_SetT1(Int32U Period_uSec);
// Set timer 1 period (in us x10)
void ZwTimer_SetT1x10(Int32U Period_uSec_x10);
// Enable interrupt processing on T1
void ZwTimer_EnableInterruptsT1(Boolean Enable);
// Initialize CPU timer 2
void ZwTimer_InitT2();
// Set timer 2 period
void ZwTimer_SetT2(Int32U Period_uSec);
// Enable interrupt processing on T2
void ZwTimer_EnableInterruptsT2(Boolean Enable);

// Macros
//
// Start Timer 0
void inline ZwTimer_StartT0()
{
	CpuTimer0Regs.TCR.bit.TSS = 0;
}
// Stop Timer 0
void inline ZwTimer_StopT0()
{
	CpuTimer0Regs.TCR.bit.TSS = 1;
}
// Start Timer 1
void inline ZwTimer_StartT1()
{
	CpuTimer1Regs.TCR.bit.TSS = 0;
}
// Stop Timer 1
void inline ZwTimer_StopT1()
{
	CpuTimer1Regs.TCR.bit.TSS = 1;
}
// Start Timer 2
void inline ZwTimer_StartT2()
{
	CpuTimer2Regs.TCR.bit.TSS = 0;
}
// Stop Timer 2
void inline ZwTimer_StopT2()
{
	CpuTimer2Regs.TCR.bit.TSS = 1;
}
//
// Reload timer 0
void inline ZwTimer_ReloadT0()
{
	CpuTimer0Regs.TCR.bit.TRB = 1;
}
// Reload timer 1
void inline ZwTimer_ReloadT1()
{
	CpuTimer1Regs.TCR.bit.TRB = 1;
}
// Reload timer 2
void inline ZwTimer_ReloadT2()
{
	CpuTimer2Regs.TCR.bit.TRB = 1;
}
// Return timer 0 value
Int32U inline ZwTimer_GetValueT0()
{
	return CpuTimer0Regs.TIM.all;
}
// Return timer 1 value
Int32U inline ZwTimer_GetValueT1()
{
	return CpuTimer1Regs.TIM.all;
}
// Return timer 2 value
Int32U inline ZwTimer_GetValueT2()
{
	return CpuTimer2Regs.TIM.all;
}


// ISR postfix
#define TIMER0_ISR_DONE    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1
#define TIMER1_ISR_DONE    
#define TIMER2_ISR_DONE    

#endif // __ZW_TIMER_H
