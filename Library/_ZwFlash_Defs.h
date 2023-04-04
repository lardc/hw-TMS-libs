// -----------------------------------------
// Flash memory definitions.
// DEPENDENCIES: 
//		ZwBase.h
// _________________________________________
//
// REVISION 	- 0
// REV. DATE 	- 10.01.2008
//
// ----------------------------------------- 

#ifndef __ZW_FLASH_DEFS_H
#define __ZW_FLASH_DEFS_H

#include "ZwBase.h"

//---------------------------------------------------------------------------
/* Flash Registers */

#define FLASH_SLEEP   	0x0000;
#define FLASH_STANDBY 	0x0001;
#define FLASH_ACTIVE  	0x0003;

/* Flash Option Register bit definitions */
struct  FOPT_BITS {       // bit   description
   Uint16     ENPIPE:1;   // 0     Enable Pipeline Mode
   Uint16     rsvd:15;    // 1-15  reserved
};

/* Allow access to the bit fields or entire register */
union FOPT_REG {
   Uint16           all;
   struct FOPT_BITS bit;
};

/* Flash Power Modes Register bit definitions */
struct  FPWR_BITS {       // bit   description
   Uint16     PWR:2;      // 0-1   Power Mode bits
   Uint16     rsvd:14;    // 2-15  reserved
};

/* Allow access to the bit fields or entire register */
union FPWR_REG {
   Uint16           all;
   struct FPWR_BITS bit;
};

/* Flash Status Register bit definitions */
struct  FSTATUS_BITS {       // bit   description
   Uint16     PWRS:2;        // 0-1   Power Mode Status bits
   Uint16     STDBYWAITS:1;  // 2     Bank/Pump Sleep to Standby Wait Counter Status bits
   Uint16     ACTIVEWAITS:1; // 3     Bank/Pump Standby to Active Wait Counter Status bits
   Uint16     rsvd1:4;       // 4-7   reserved
   Uint16     V3STAT:1;      // 8     VDD3V Status Latch bit
   Uint16     rsvd2:7;       // 9-15  reserved
};

/* Allow access to the bit fields or entire register */
union FSTATUS_REG {
   Uint16              all;
   struct FSTATUS_BITS bit;
};

/* Flash Sleep to Standby Wait Counter Register bit definitions */
struct  FSTDBYWAIT_BITS {    // bit   description
   Uint16     STDBYWAIT:9;   // 0-8   Bank/Pump Sleep to Standby Wait Count bits
   Uint16     rsvd:7;        // 9-15  reserved
};

/* Allow access to the bit fields or entire register */
union FSTDBYWAIT_REG {
   Uint16                 all;
   struct FSTDBYWAIT_BITS bit;
};

/* Flash Standby to Active Wait Counter Register bit definitions */
struct  FACTIVEWAIT_BITS {   // bit   description
   Uint16     ACTIVEWAIT:9;  // 0-8   Bank/Pump Standby to Active Wait Count bits
   Uint16     rsvd:7;        // 9-15  reserved
};

/* Allow access to the bit fields or entire register */
union FACTIVEWAIT_REG {
   Uint16                  all;
   struct FACTIVEWAIT_BITS bit;
};

/* Bank Read Access Wait State Register bit definitions */
struct  FBANKWAIT_BITS {     // bit   description
   Uint16     RANDWAIT:4;    // 0-3   Flash Random Read Wait State bits
   Uint16     rsvd1:4;       // 4-7   reserved
   Uint16     PAGEWAIT:4;    // 8-11  Flash Paged Read Wait State bits
   Uint16     rsvd2:4;       // 12-15 reserved
};

/* Allow access to the bit fields or entire register */
union FBANKWAIT_REG {
   Uint16                all;
   struct FBANKWAIT_BITS bit;
};

/* OTP Read Access Wait State Register bit definitions */
struct  FOTPWAIT_BITS {      // bit   description
   Uint16     OTPWAIT:5;     // 0-4   OTP Read Wait State bits
   Uint16     rsvd:11;       // 5-15  reserved
};

/* Allow access to the bit fields or entire register */
union FOTPWAIT_REG {
   Uint16               all;
   struct FOTPWAIT_BITS bit;
};

struct FLASH_REGS {
   union FOPT_REG        FOPT;        // Option Register
   Uint16                rsvd1;       // reserved
   union FPWR_REG        FPWR;        // Power Modes Register
   union FSTATUS_REG     FSTATUS;     // Status Register
   union FSTDBYWAIT_REG  FSTDBYWAIT;  // Pump/Bank Sleep to Standby Wait State Register
   union FACTIVEWAIT_REG FACTIVEWAIT; // Pump/Bank Standby to Active Wait State Register
   union FBANKWAIT_REG   FBANKWAIT;   // Bank Read Access Wait State Register
   union FOTPWAIT_REG    FOTPWAIT;    // OTP Read Access Wait State Register
};

//---------------------------------------------------------------------------
// Flash External References declarations:
//
extern volatile struct FLASH_REGS FlashRegs;

#endif // end of __ZW_FLASH_DEFS_H
