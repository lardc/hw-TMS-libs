// -----------------------------------------
// PIE module managment functions
// ----------------------------------------

#ifndef __ZW_PIE_H
#define __ZW_PIE_H

// Inlcude
#include "stdinc.h"
//
// IO defs
#include "_ZwPIE_Defs.h"

// Macro
//
// ISR map macro
#define BEGIN_ISR_MAP		EALLOW;
#define END_ISR_MAP			EDIS;
#define ADD_ISR(intr, proc) PieVectTable.intr = &proc
#define ISRCALL 			interrupt void

// Constants
//
// Interrupts masks
#define M_INT1  			0x0001
#define M_INT2  			0x0002
#define M_INT3		  		0x0004
#define M_INT4  			0x0008
#define M_INT5  			0x0010
#define M_INT6 		 		0x0020
#define M_INT7  			0x0040
#define M_INT8  			0x0080
#define M_INT9  			0x0100
#define M_INT10 			0x0200
#define M_INT11 			0x0400
#define M_INT12 			0x0800
#define M_INT13 			0x1000
#define M_INT14 			0x2000
#define M_DLOG  			0x4000
#define M_RTOS  			0x8000

// Functions
//
// Init PIE
void ZwPIE_Init();
// Enable interrupts engine
void ZwPIE_Prepare();

#endif // __ZW_PIE_H
