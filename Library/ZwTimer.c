// -----------------------------------------
// Timer management functions
// ----------------------------------------

// Header
#include "ZwTimer.h"

// Include
#include "ZwPIE.h"

// Forward functions
//
void ZwTimer_SetTx(volatile struct CPUTIMER_REGS *Regs, Int32U CPUFreq_MHz, Int32U Period_uSec);
void ZwTimer_SetPeriodTx_us(volatile struct CPUTIMER_REGS *Regs, Int32U CPUFreq_MHz, Int32U Period_uSec);
void ZwTimer_InitTx(volatile struct CPUTIMER_REGS *Regs);

// Functions
//
void ZwTimer_InitT0(void)
{
	ZwTimer_InitTx(&CpuTimer0Regs);
}	
//----------------------------------------
	
void ZwTimer_SetT0(Int32U Period_uSec)
{
	ZwTimer_SetTx(&CpuTimer0Regs, CPU_FRQ_MHZ, Period_uSec);
}
//----------------------------------------

void ZwTimer_SetPeriodT0_us(Int32U Period_uSec)
{
	ZwTimer_SetPeriodTx_us(&CpuTimer0Regs, CPU_FRQ_MHZ, Period_uSec);
}
//----------------------------------------

void ZwTimer_InitT1(void)
{
	ZwTimer_InitTx(&CpuTimer1Regs);
}	
//----------------------------------------
	
void ZwTimer_SetT1(Int32U Period_uSec)
{
	ZwTimer_SetTx(&CpuTimer1Regs, CPU_FRQ_MHZ, Period_uSec);
}
//----------------------------------------

void ZwTimer_SetT1x10(Int32U Period_uSec_x10)
{
	ZwTimer_SetTx(&CpuTimer1Regs, CPU_FRQ_MHZ / 10, Period_uSec_x10);
}
//----------------------------------------

void ZwTimer_SetPeriodT1_us(Int32U Period_uSec)
{
	ZwTimer_SetPeriodTx_us(&CpuTimer1Regs, CPU_FRQ_MHZ, Period_uSec);
}
//----------------------------------------

void ZwTimer_InitT2()
{
	ZwTimer_InitTx(&CpuTimer2Regs);
}	
//----------------------------------------
	
void ZwTimer_SetT2(Int32U Period_uSec)
{
	ZwTimer_SetTx(&CpuTimer2Regs, CPU_FRQ_MHZ, Period_uSec);
}
//----------------------------------------

void ZwTimer_SetPeriodT2_us(Int32U Period_uSec)
{
	ZwTimer_SetPeriodTx_us(&CpuTimer2Regs, CPU_FRQ_MHZ, Period_uSec);
}
//----------------------------------------

void ZwTimer_EnableInterruptsT0(Boolean Enable)
{
   // Enable CPU INT1 which is connected to CPU-Timer 0:
   if(Enable)
	   IER |= M_INT1;
   else
	   IER &= ~M_INT1;

   // Enable TINT0 in the PIE: Group 1 interrupt 7
   PieCtrlRegs.PIEIER1.bit.INTx7 = Enable ? 0x01 : 0x00;
}
//----------------------------------------

void ZwTimer_EnableInterruptsT1(Boolean Enable)
{
   // Enable CPU INT13 which is connected to CPU-Timer 1:
   if(Enable)
	   IER |= M_INT13;
   else
	   IER &= ~M_INT13;
}
//----------------------------------------

void ZwTimer_EnableInterruptsT2(Boolean Enable)
{
   // Enable CPU INT14 which is connected to CPU-Timer 2:
   if(Enable)
	   IER |= M_INT14;
   else
	   IER &= ~M_INT14;
}
//----------------------------------------

// Internal functions
//
void ZwTimer_SetTx(volatile struct CPUTIMER_REGS *Regs, Int32U CPUFreq_MHz, Int32U Period_uSec)
{
	// Initialize timer period:	
	Regs->PRD.all = CPUFreq_MHz * Period_uSec;

	// Set prescaler counter to divide by 1 (SYSCLKOUT):
	Regs->TPR.all = 0;
	Regs->TPRH.all = 0;
	
	// Initialize timer control register:
	Regs->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer
	Regs->TCR.bit.TRB = 1;      // 1 = reload timer
	Regs->TCR.bit.SOFT = 1;
	Regs->TCR.bit.FREE = 1;     // Timer Free Run
	Regs->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt
}
//----------------------------------------

void ZwTimer_SetPeriodTx_us(volatile struct CPUTIMER_REGS *Regs, Int32U CPUFreq_MHz, Int32U Period_uSec)
{
	Int32U Prescaler = CPUFreq_MHz - 1;

	// Initialize timer period:
	Regs->PRD.all = Period_uSec;

	// Set prescaler
	Regs->TPR.all = Prescaler & 0xFFFF;
	Regs->TPRH.all = (Prescaler >> 16) & 0xFFFF;

	// Initialize timer control register:
	Regs->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer
	Regs->TCR.bit.TRB = 1;      // 1 = reload timer
	Regs->TCR.bit.SOFT = 1;
	Regs->TCR.bit.FREE = 1;     // Timer Free Run
	Regs->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt
}
//----------------------------------------

void ZwTimer_InitTx(volatile struct CPUTIMER_REGS *Regs)
{
	// Initialize timer period to maximum:	
	Regs->PRD.all = 0xFFFFFFFF;
	// Initialize prescaler counter to divide by 1 (SYSCLKOUT):
	Regs->TPR.all = 0;
	Regs->TPRH.all = 0;
	// Make sure timer is stopped:
	Regs->TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	Regs->TCR.bit.TRB = 1;
}
//----------------------------------------

// MEMORY IO
//----------------------------------------

#pragma DATA_SECTION(CpuTimer0Regs,"CpuTimer0RegsFile");
volatile struct CPUTIMER_REGS CpuTimer0Regs;
//----------------------------------------
#pragma DATA_SECTION(CpuTimer1Regs,"CpuTimer1RegsFile");
volatile struct CPUTIMER_REGS CpuTimer1Regs;
//----------------------------------------
#pragma DATA_SECTION(CpuTimer2Regs,"CpuTimer2RegsFile");
volatile struct CPUTIMER_REGS CpuTimer2Regs;

// No more.
