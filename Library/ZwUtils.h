// -----------------------------------------
// System utils
// ----------------------------------------

#ifndef __ZW_UTILS_H
#define __ZW_UTILS_H

// Include
#include "stdinc.h"

// Macro
//
// Common
#define ABS(a)				(((a) < 0) ? -(a) : (a))
#define SQR(a)				((a) * (a))
#define MAX(a, b) 			(((a) > (b)) ? (a) : (b))
#define MIN(a, b) 			(((a) < (b)) ? (a) : (b))
#define SIGN(a)				(((a) >= 0) ? 1 : -1)
#define SAT(val, max, min)	((val > max) ? max : ((val < min) ? min : val))


// Functions
//
// Memory copy functions
void inline MemCopy16p(const Int16U * restrict SourceAddr, const Int16U * restrict SourceEndAddr, Int16U * restrict DestAddr)
{
    while(SourceAddr < SourceEndAddr)
	    *DestAddr++ = *SourceAddr++;
}
//
void inline MemCopy16(const Int16U * restrict SourceAddr, Int16U * restrict DestAddr, Int16U WordsToCopy)
{
	register Int16U i;
	
	for(i = 0; i < WordsToCopy; ++i)
		*DestAddr++ = *SourceAddr++;	
}
//
void inline MemCopy32(const Int32U * restrict SourceAddr, Int32U * restrict  DestAddr, Int16U DWordsToCopy)
{
	register Int16U i;
	
	for(i = 0; i < DWordsToCopy; ++i)
		*DestAddr++ = *SourceAddr++;	
}
//
void inline MemFill16(Int16U * restrict StartAddress, Int16U WordsToFill, Int16U Value)
{
	register Int16U i;
	
	for(i = 0; i < WordsToFill; ++i)
		*StartAddress++ = Value;	
}
//
void inline MemFill32(Int32U * restrict StartAddress, Int16U DWordsToFill, Int32U Value)
{
	register Int16U i;
	
	for(i = 0; i < DWordsToFill; ++i)
		*StartAddress++ = Value;	
}
//
void inline MemZero16(Int16U * restrict StartAddress, Int16U WordsToFill)
{
	MemFill16(StartAddress, WordsToFill, 0);
}
//
void inline MemZero32(Int32U * restrict StartAddress, Int16U DWordsToFill)
{
	MemFill32(StartAddress, DWordsToFill, 0L);
}

// Delay function
//
#define DELAY_US(A) DSP28x_usDelay(((((Int32U)A) * CPU_FRQ_MHZ) - 9) / 5)
//
extern void DSP28x_usDelay(Int32U Count);

// Flash shadow
//
extern Int16U RamfuncsLoadStart;
extern Int16U RamfuncsLoadEnd;
extern Int16U RamfuncsRunStart;
//
extern Int16U RamconstsLoadStart;
extern Int16U RamconstsLoadEnd;
extern Int16U RamconstsRunStart;
//
extern Int16U IQmathLoadStart;
extern Int16U IQmathLoadEnd;
extern Int16U IQmathRunStart;
//
extern Int16U IQmathTablesRamLoadStart;
extern Int16U IQmathTablesRamLoadEnd;
extern Int16U IQmathTablesRamRunStart;

// Boot-time macro
//
#ifdef BOOT_FROM_FLASH
	// Optimize access CPU to flash memory
	#define ZW_FLASH_OPTIMIZE(FWait, OTPWait)	InitFlash(FWait, OTPWait)
#else
	#define ZW_FLASH_OPTIMIZE
#endif

#ifdef BOOT_FROM_FLASH
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the F2806.cmd file. 
	#define ZW_FLASH_CODE_SHADOW	MemCopy16p(&RamfuncsLoadStart,\
	 	&RamfuncsLoadEnd, &RamfuncsRunStart)
	#define ZW_FLASH_MATH_SHADOW	{	\
										MemCopy16p(&IQmathLoadStart,\
	 									&IQmathLoadEnd, &IQmathRunStart);\
									}
	#define ZW_FLASH_MATH_TR_SHADOW	{	\
										MemCopy16p(&IQmathTablesRamLoadStart,\
										&IQmathTablesRamLoadEnd, &IQmathTablesRamRunStart);\
									}
	 									
	#define ZW_FLASH_CONST_SHADOW	MemCopy16p(&RamconstsLoadStart,\
	 	&RamconstsLoadEnd, &RamconstsRunStart)
#else
	#define ZW_FLASH_CODE_SHADOW
	#define ZW_FLASH_MATH_SHADOW
	#define ZW_FLASH_MATH_TR_SHADOW
	#define ZW_FLASH_CONST_SHADOW
#endif

// Macro
//
// Program flow-control macro
#define DSP_STOP		asm(" ESTOP0")
#define DSP_LOOP(proc)	for(;;) { proc(); }
#define DSP_INFLOOP		for(;;) { asm(" NOP"); }

#endif

