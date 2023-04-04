// -----------------------------------------
// CPU macro
// ----------------------------------------

#ifndef __ZW_CPU_MACRO
#define __ZW_CPU_MACRO

// CPU Control Registers
extern cregister volatile unsigned int IFR;
extern cregister volatile unsigned int IER;

// CPU interrupt managment instructions macroses
#define  EINT   	asm(" clrc INTM")
#define  DINT   	asm(" setc INTM")
#define  ERTM   	asm(" clrc DBGM")
#define  DRTM   	asm(" setc DBGM")
#define  EALLOW 	asm(" EALLOW")
#define  EDIS   	asm(" EDIS")
#define  ESTOP0 	asm(" ESTOP0")

#endif // __ZW_CPU_MACRO
