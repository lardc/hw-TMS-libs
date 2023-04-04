#ifndef __ZW_XINT_DEFS_H
#define __ZW_XINT_DEFS_H

#include "ZwBase.h"

//---------------------------------------------------------------------------
// External Interrupt Register Bits:
//
struct XINTCR_BITS {
    Uint16   ENABLE:1;    // 0      enable/disable
    Uint16   rsvd1:1;     // 1      reserved
    Uint16   POLARITY:2;  // 3:2    pos/neg, both triggered
    Uint16   rsvd2:12;    // 15:4   reserved
};

union XINTCR_REG {
   Uint16               all;
   struct XINTCR_BITS   bit;
};  

struct XNMICR_BITS {
    Uint16   ENABLE:1;    // 0      enable/disable
    Uint16   SELECT:1;    // 1      Timer 1 or XNMI connected to INT13
    Uint16   POLARITY:2;  // 3:2    pos/neg, or both triggered
    Uint16   rsvd2:12;    // 15:4   reserved
};

union XNMICR_REG {
   Uint16               all;
   struct XNMICR_BITS   bit;
};

//---------------------------------------------------------------------------
// External Interrupt Register File:
//
struct XINTRUPT_REGS {
   union XINTCR_REG XINT1CR;
   union XINTCR_REG XINT2CR;
   Uint16           rsvd1[5];
   union XNMICR_REG XNMICR;
   Uint16           XINT1CTR;
   Uint16           XINT2CTR;
   Uint16           rsvd[5];
   Uint16           XNMICTR;
};

//---------------------------------------------------------------------------
// External Interrupt References & Function Declarations:
//
extern volatile struct XINTRUPT_REGS XIntruptRegs;

#endif // end __ZW_XINT_DEFS_H
