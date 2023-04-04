// -----------------------------------------
// CPU timer declarations.
//
// COPYRIGHT (C) TI
// _________________________________________
//
// DEPENDENCIES: 
//		ZwBase.h
// _________________________________________
//
// REVISION 	- 0
// REV. DATE 	- 6.01.2008
//
// ----------------------------------------- 

#ifndef __ZW_TIMER_DEFS_H
#define __ZW_TIMER_DEFS_H

#include "ZwBase.h"

//---------------------------------------------------------------------------
// CPU Timer Register Bit Definitions:
//
//
// TCR: Control register bit definitions:
struct  TCR_BITS {          // bits  description
   Uint16    rsvd1:4;       // 3:0   reserved
   Uint16    TSS:1;         // 4     Timer Start/Stop
   Uint16    TRB:1;         // 5     Timer reload
   Uint16    rsvd2:4;       // 9:6   reserved
   Uint16    SOFT:1;        // 10    Emulation modes
   Uint16    FREE:1;        // 11
   Uint16    rsvd3:2;       // 12:13 reserved
   Uint16    TIE:1;         // 14    Output enable
   Uint16    TIF:1;         // 15    Interrupt flag
}; 

union TCR_REG {
   Uint16           all;
   struct TCR_BITS  bit;
};

// TPR: Pre-scale low bit definitions:
struct  TPR_BITS {        // bits  description
   Uint16     TDDR:8;     // 7:0   Divide-down low
   Uint16     PSC:8;      // 15:8  Prescale counter low
};

union TPR_REG {
   Uint16           all;
   struct TPR_BITS  bit;
};

// TPRH: Pre-scale high bit definitions:
struct  TPRH_BITS {       // bits  description
   Uint16     TDDRH:8;      // 7:0   Divide-down high
   Uint16     PSCH:8;       // 15:8  Prescale counter high
};

union TPRH_REG {
   Uint16           all;
   struct TPRH_BITS bit;
};

// TIM, TIMH: Timer register definitions:
struct TIM_REG {
   Uint16  LSW;
   Uint16  MSW;
};

union TIM_GROUP {
   Uint32          all;
   struct TIM_REG  half;
};

// PRD, PRDH: Period register definitions:
struct PRD_REG {
   Uint16  LSW;
   Uint16  MSW;
};

union PRD_GROUP {
   Uint32          all;
   struct PRD_REG  half;
};

// CPU Timer Register File:
//
struct CPUTIMER_REGS {
   union TIM_GROUP TIM;   // Timer counter register
   union PRD_GROUP PRD;   // Period register
   union TCR_REG   TCR;   // Timer control register
   Uint16          rsvd1; // reserved
   union TPR_REG   TPR;   // Timer pre-scale low
   union TPRH_REG  TPRH;  // Timer pre-scale high
};

//---------------------------------------------------------------------------
// IO Variables
//
extern volatile struct CPUTIMER_REGS CpuTimer0Regs;
extern volatile struct CPUTIMER_REGS CpuTimer1Regs;
extern volatile struct CPUTIMER_REGS CpuTimer2Regs;

#endif // end __ZW_TIMER_DEFS_H

