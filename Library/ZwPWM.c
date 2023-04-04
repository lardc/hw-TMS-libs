// -----------------------------------------
// PWM operational functions
// ----------------------------------------- 

// Header
#include "ZwPWM.h"

// Include
#include "ZwSysCtrl.h"
#include "ZwPIE.h"
#include "ZwGPIO.h"


// Forward functions
void ZwPWM1_InitGPIO(Boolean A, Boolean B);
void ZwPWM2_InitGPIO(Boolean A, Boolean B);
void ZwPWM3_InitGPIO(Boolean A, Boolean B);
void ZwPWM4_InitGPIO(Boolean A, Boolean B);
void ZwPWM5_InitGPIO(Boolean A, Boolean B);
void ZwPWM6_InitGPIO(Boolean A, Boolean B);
//
void ZwPWMx_InitPWM(volatile struct EPWM_REGS *EPwmxRegs, ZwPWMType Type, Int32U CPUdivPWM, Boolean A, Boolean B,
					Boolean ATZLow, Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB);
void ZwPWMx_InitTZ(volatile struct EPWM_REGS *EPwmxRegs, Int16U TZMaskCBC, Int16U TZMaskOST);
void ZwPWMx_ConfigET(volatile struct EPWM_REGS *EPwmxRegs, Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod,
					 Boolean EnableSOCB, ZwPWMETType SOCBType, ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod);


// Functions
//
void ZwPWM1_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST, Boolean ATZLow,
		         Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB)
{
	// Init modules
	ZwPWMx_InitPWM(&EPwm1Regs, Type, CPU_Freq / PWM_Freq, A, B, ATZLow, BTZLow, LoadOnShadow, InvertB, ChBCmpB);

	// Set TZ functions
   	ZwPWMx_InitTZ(&EPwm1Regs, TZMaskCBC, TZMaskOST);

   	// Init pins for PWM
	ZwPWM1_InitGPIO(A, B);
}
// ----------------------------------------- 

void ZwPWM2_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST, Boolean ATZLow,
		         Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB)
{
	// Init modules
	ZwPWMx_InitPWM(&EPwm2Regs, Type, CPU_Freq / PWM_Freq, A, B, ATZLow, BTZLow, LoadOnShadow, InvertB, ChBCmpB);

	// Set TZ functions
   	ZwPWMx_InitTZ(&EPwm2Regs, TZMaskCBC, TZMaskOST);

   	// Init pins for PWM & TZ functionality
	ZwPWM2_InitGPIO(A, B);
}
// ----------------------------------------- 

void ZwPWM3_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST, Boolean ATZLow,
		         Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB)
{
	// Init modules
	ZwPWMx_InitPWM(&EPwm3Regs, Type, CPU_Freq / PWM_Freq, A, B, ATZLow, BTZLow, LoadOnShadow, InvertB, ChBCmpB);

	// Set TZ functions
   	ZwPWMx_InitTZ(&EPwm3Regs, TZMaskCBC, TZMaskOST);

   	// Init pins for PWM & TZ functionality
	ZwPWM3_InitGPIO(A, B);
}
// ----------------------------------------- 

void ZwPWM4_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST, Boolean ATZLow,
		         Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB)
{
	// Init modules
	ZwPWMx_InitPWM(&EPwm4Regs, Type, CPU_Freq / PWM_Freq, A, B, ATZLow, BTZLow, LoadOnShadow, InvertB, ChBCmpB);

	// Set TZ functions
   	ZwPWMx_InitTZ(&EPwm4Regs, TZMaskCBC, TZMaskOST);

	// Init pins for PWM & TZ functionality
	ZwPWM4_InitGPIO(A, B);
}
// ----------------------------------------- 

void ZwPWM5_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST, Boolean ATZLow,
		         Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB)
{
	// Init modules
	ZwPWMx_InitPWM(&EPwm5Regs, Type, CPU_Freq / PWM_Freq, A, B, ATZLow, BTZLow, LoadOnShadow, InvertB, ChBCmpB);

	// Set TZ functions
   	ZwPWMx_InitTZ(&EPwm5Regs, TZMaskCBC, TZMaskOST);

	// Init pins for PWM & TZ functionality
	ZwPWM5_InitGPIO(A, B);
}
// ----------------------------------------- 

void ZwPWM6_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST, Boolean ATZLow,
		         Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB)
{
	// Init modules
	ZwPWMx_InitPWM(&EPwm6Regs, Type, CPU_Freq / PWM_Freq, A, B, ATZLow, BTZLow, LoadOnShadow, InvertB, ChBCmpB);

	// Set TZ functions
   	ZwPWMx_InitTZ(&EPwm6Regs, TZMaskCBC, TZMaskOST);

   	// Init pins for PWM & TZ functionality
	ZwPWM6_InitGPIO(A, B);
}
// ----------------------------------------- 

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWM_ConfigTZ1, "ramfuncs");
#endif
void ZwPWM_ConfigTZ1(Boolean Enable, ZwPWMQualification Qualification)
{
	Int16U mask = 0x03;	// 11b (Async)

	// Bit-mask for QSEL
	switch(Qualification)
	{
		case PQ_SysClock:
			mask = 0x00; 	// 00b
			break;
		case PQ_Sample3:
			mask = 0x01;	// 01b
			break;
		case PQ_Sample6:
			mask = 0x02;	// 10b
			break;
	}

	EALLOW;

	if(Enable)
    {
    	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = mask;  // Input GPIO12 (TZ1)
 		GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;      // Configure GPIO12 as TZ1
    }
	else
 		GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;      // Configure GPIO12 as GPIO input

	EDIS;
}
// -----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWM_ConfigTZ2, "ramfuncs");
#endif
void ZwPWM_ConfigTZ2(Boolean Enable, ZwPWMQualification Qualification)
{
	Int16U mask = 0x03;	// 11b (Async)

	// Bit-mask for QSEL
	switch(Qualification)
	{
		case PQ_SysClock:
			mask = 0x00; 	// 00b
			break;
		case PQ_Sample3:
			mask = 0x01;	// 01b
			break;
		case PQ_Sample6:
			mask = 0x02;	// 10b
			break;
	}

	EALLOW;

	if(Enable)
    {
    	GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = mask;  // Input GPIO13 (TZ2)
 		GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;   // Configure GPIO13 as TZ2
    }
	else
 		GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;   // Configure GPIO13 as GPIO input

	EDIS;
}
// -----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWM_ConfigTZ3, "ramfuncs");
#endif
void ZwPWM_ConfigTZ3(Boolean Enable, ZwPWMQualification Qualification)
{
	Int16U mask = 0x03;	// 11b (Async)

	// Bit-mask for QSEL
	switch(Qualification)
	{
		case PQ_SysClock:
			mask = 0x00; 	// 00b
			break;
		case PQ_Sample3:
			mask = 0x01;	// 01b
			break;
		case PQ_Sample6:
			mask = 0x02;	// 10b
			break;
	}

	EALLOW;

	if(Enable)
    {
    	GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = mask;  // Input GPIO14 (TZ3)
 		GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;   // Configure GPIO14 as TZ3
    }
	else
 		GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;   // Configure GPIO14 as GPIO input

	EDIS;
}
// -----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWM_ConfigTZ4, "ramfuncs");
#endif
void ZwPWM_ConfigTZ4(Boolean Enable, ZwPWMQualification Qualification)
{
	Int16U mask = 0x03;	// 11b (Async)

	// Bit-mask for QSEL
	switch(Qualification)
	{
		case PQ_SysClock:
			mask = 0x00; 	// 00b
			break;
		case PQ_Sample3:
			mask = 0x01;	// 01b
			break;
		case PQ_Sample6:
			mask = 0x02;	// 10b
			break;
	}

	EALLOW;

	if(Enable)
    {
    	GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = mask;  // Input GPIO15 (TZ4)
 		GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;   // Configure GPIO15 as TZ4
    }
	else
 		GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;   // Configure GPIO15 as GPIO input

	EDIS;
}
// -----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWM_ConfigTZ5, "ramfuncs");
#endif
void ZwPWM_ConfigTZ5(Boolean Enable, ZwPWMQualification Qualification)
{
	Int16U mask = 0x03;	// 11b (Async)

	// Bit-mask for QSEL
	switch(Qualification)
	{
		case PQ_SysClock:
			mask = 0x00; 	// 00b
			break;
		case PQ_Sample3:
			mask = 0x01;	// 01b
			break;
		case PQ_Sample6:
			mask = 0x02;	// 10b
			break;
	}

	EALLOW;

	if(Enable)
    {
    	GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = mask;  // Input GPIO16 (TZ5)
 		GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;   // Configure GPIO16 as TZ5
    }
	else
 		GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;   // Configure GPIO16 as GPIO input

	EDIS;
}
// -----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWM_ConfigTZ6, "ramfuncs");
#endif
void ZwPWM_ConfigTZ6(Boolean Enable, ZwPWMQualification Qualification)
{
	Int16U mask = 0x03;	// 11b (Async)

	// Bit-mask for QSEL
	switch(Qualification)
	{
		case PQ_SysClock:
			mask = 0x00; 	// 00b
			break;
		case PQ_Sample3:
			mask = 0x01;	// 01b
			break;
		case PQ_Sample6:
			mask = 0x02;	// 10b
			break;
	}

	EALLOW;

	if(Enable)
    {
    	GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = mask;  // Input GPIO17 (TZ6)
 		GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 3;   // Configure GPIO17 as TZ6
    }
	else
 		GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;   // Configure GPIO17 as GPIO input

	EDIS;
}
// -----------------------------------------

// Set TZ pull'up mode
void ZwPWM_SetTZPullup(ZwPWMFunctional TZ1, ZwPWMFunctional TZ2, ZwPWMFunctional TZ3, ZwPWMFunctional TZ4, ZwPWMFunctional TZ5, ZwPWMFunctional TZ6)
{
	EALLOW;

	if(TZ1 != PFDontcare)
		GpioCtrlRegs.GPAPUD.bit.GPIO12 = (TZ1 == PFDisable);
	if(TZ2 != PFDontcare)
		GpioCtrlRegs.GPAPUD.bit.GPIO13 = (TZ2 == PFDisable);
	if(TZ3 != PFDontcare)
		GpioCtrlRegs.GPAPUD.bit.GPIO14 = (TZ3 == PFDisable);
	if(TZ4 != PFDontcare)
		GpioCtrlRegs.GPAPUD.bit.GPIO15 = (TZ4 == PFDisable);
	if(TZ5 != PFDontcare)
		GpioCtrlRegs.GPAPUD.bit.GPIO16 = (TZ5 == PFDisable);
	if(TZ6 != PFDontcare)
		GpioCtrlRegs.GPAPUD.bit.GPIO17 = (TZ6 == PFDisable);

	EDIS;
}
// -----------------------------------------

// Init interrupts for TZ OST
void ZwPWM_ConfigTZIntOST(Boolean PWM1, Boolean PWM2, Boolean PWM3, Boolean PWM4, Boolean PWM5, Boolean PWM6)
{
	// Enable interrupts on TZ events
    EALLOW;
	
	EPwm1Regs.TZEINT.bit.OST = PWM1 ? TZ_ENABLE : TZ_DISABLE;
	EPwm2Regs.TZEINT.bit.OST = PWM2 ? TZ_ENABLE : TZ_DISABLE;
	EPwm3Regs.TZEINT.bit.OST = PWM3 ? TZ_ENABLE : TZ_DISABLE;
	EPwm4Regs.TZEINT.bit.OST = PWM4 ? TZ_ENABLE : TZ_DISABLE;
	EPwm5Regs.TZEINT.bit.OST = PWM5 ? TZ_ENABLE : TZ_DISABLE;
	EPwm6Regs.TZEINT.bit.OST = PWM6 ? TZ_ENABLE : TZ_DISABLE;
	
	EDIS;
}
// ----------------------------------------- 

// Init interrupts for TZ OST
void ZwPWM_ConfigTZIntCBC(Boolean PWM1, Boolean PWM2, Boolean PWM3, Boolean PWM4, Boolean PWM5, Boolean PWM6)
{
	// Enable interrupts on TZ events
    EALLOW;

	EPwm1Regs.TZEINT.bit.CBC = PWM1 ? TZ_ENABLE : TZ_DISABLE;
	EPwm2Regs.TZEINT.bit.CBC = PWM2 ? TZ_ENABLE : TZ_DISABLE;
	EPwm3Regs.TZEINT.bit.CBC = PWM3 ? TZ_ENABLE : TZ_DISABLE;
	EPwm4Regs.TZEINT.bit.CBC = PWM4 ? TZ_ENABLE : TZ_DISABLE;
	EPwm5Regs.TZEINT.bit.CBC = PWM5 ? TZ_ENABLE : TZ_DISABLE;
	EPwm6Regs.TZEINT.bit.CBC = PWM6 ? TZ_ENABLE : TZ_DISABLE;

	EDIS;
}
// -----------------------------------------

void ZwPWM_ConfigETInt(Boolean ET1, Boolean ET2, Boolean ET3, Boolean ET4, Boolean ET5, Boolean ET6)
{
	EPwm1Regs.ETSEL.bit.INTEN = ET1 ? 1 : 0;
	EPwm2Regs.ETSEL.bit.INTEN = ET2 ? 1 : 0;
	EPwm3Regs.ETSEL.bit.INTEN = ET3 ? 1 : 0;
	EPwm4Regs.ETSEL.bit.INTEN = ET4 ? 1 : 0;
	EPwm5Regs.ETSEL.bit.INTEN = ET5 ? 1 : 0;
	EPwm6Regs.ETSEL.bit.INTEN = ET6 ? 1 : 0;
}
// -----------------------------------------

void ZwPWM1_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
					 ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod)
{
	ZwPWMx_ConfigET(&EPwm1Regs, EnableSOCA, SOCAType, SOCAPeriod, EnableSOCB, SOCBType, SOCBPeriod, IntType, IntPeriod);
}
// -----------------------------------------

void ZwPWM2_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	 	 ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod)
{
	ZwPWMx_ConfigET(&EPwm2Regs, EnableSOCA, SOCAType, SOCAPeriod, EnableSOCB, SOCBType, SOCBPeriod, IntType, IntPeriod);
}
// -----------------------------------------

void ZwPWM3_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
					 ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod)
{
	ZwPWMx_ConfigET(&EPwm3Regs, EnableSOCA, SOCAType, SOCAPeriod, EnableSOCB, SOCBType, SOCBPeriod, IntType, IntPeriod);
}
// -----------------------------------------

void ZwPWM4_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	 	 ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod)
{
	ZwPWMx_ConfigET(&EPwm4Regs, EnableSOCA, SOCAType, SOCAPeriod, EnableSOCB, SOCBType, SOCBPeriod, IntType, IntPeriod);
}
// -----------------------------------------

void ZwPWM5_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	 	 ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod)
{
	ZwPWMx_ConfigET(&EPwm5Regs, EnableSOCA, SOCAType, SOCAPeriod, EnableSOCB, SOCBType, SOCBPeriod, IntType, IntPeriod);
}
// -----------------------------------------

void ZwPWM6_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	     ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod)
{
	ZwPWMx_ConfigET(&EPwm6Regs, EnableSOCA, SOCAType, SOCAPeriod, EnableSOCB, SOCBType, SOCBPeriod, IntType, IntPeriod);
}
// ----------------------------------------- 

// Handle PWMx TZ interrupts
#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWMx_ClearTZ, "ramfuncs");
#endif
void ZwPWMx_ClearTZ(volatile struct EPWM_REGS *EPwmxRegs)
{
    // TZ registers are EALLOW-protected
    EALLOW;
		EPwmxRegs->TZCLR.bit.CBC = 0x01; 		// Clear interrupt
		EPwmxRegs->TZCLR.bit.OST = 0x01; 		// Clear interrupt
	EDIS; 
}
// -----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWMx_SetValueA, "ramfuncs");
#endif
void ZwPWMx_SetValueA(volatile struct EPWM_REGS *EPwmxRegs, Int16S Duty_A)
{
	// Set PWMxA
	if (Duty_A > 0)
    {
		if(Duty_A < ZW_PWM_DUTY_BASE)
		{
			EPwmxRegs->CMPA.half.CMPA = EPwmxRegs->TBPRD * ((Int32U)Duty_A) / ZW_PWM_DUTY_BASE;
			EPwmxRegs->AQCSFRC.bit.CSFA = AQCSFRC_NO_SW;
		}
		else
			EPwmxRegs->AQCSFRC.bit.CSFA = AQCSFRC_HIGH;         // Forcing a continuous High on output A
    }
	else
		EPwmxRegs->AQCSFRC.bit.CSFA = AQCSFRC_LOW;         		// Forcing a continuous Low on output A
}
// ----------------------------------------- 

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWMx_SetValueB, "ramfuncs");
#endif
void ZwPWMx_SetValueB(volatile struct EPWM_REGS *EPwmxRegs, Int16S Duty_B)
{
	// Set PWMxB
	if(Duty_B > 0)
    {
		if(Duty_B < ZW_PWM_DUTY_BASE)
		{
			EPwmxRegs->CMPB = EPwmxRegs->TBPRD * ((Int32U)Duty_B) / ZW_PWM_DUTY_BASE;
			EPwmxRegs->AQCSFRC.bit.CSFB = AQCSFRC_NO_SW;
		}
		else
			EPwmxRegs->AQCSFRC.bit.CSFB = AQCSFRC_HIGH;         // Forcing a continuous High on output B
    }
	else
    	EPwmxRegs->AQCSFRC.bit.CSFB = AQCSFRC_LOW;         		// Forcing a continuous Low on output B
}
// -----------------------------------------

#if defined(BOOT_FROM_FLASH)
	#pragma CODE_SECTION(ZwPWMx_SetFrequency, "ramfuncs");
#endif
void ZwPWMx_SetFrequency(volatile struct EPWM_REGS *EPwmxRegs, Int32U CPUdivPWM, Boolean IsBidirectional)
{
   Int16U period = IsBidirectional ? (CPUdivPWM / 2) : CPUdivPWM;

	// set clock rate - i.e PWM frequency
   EPwmxRegs->TBPRD = period;		// COUNT_UP/DOWN or COUNT_UPDOWN mode
   EPwmxRegs->TBCTR = 0;   			// Reset counter
}
// -----------------------------------------

void ZwPWMx_SetPhase(volatile struct EPWM_REGS *EPwmxRegs, Boolean Enable, Int32U PhaseValue, Boolean UpDir, ZwPWMSYNCType Sync)
{
	// Disable if requested
	if(!Enable)
		EPwmxRegs->TBCTL.bit.PHSEN = TB_DISABLE;

	// set phase value
	EPwmxRegs->TBPHS.half.TBPHS = PhaseValue;
	// set counter direction after phase sync event
	EPwmxRegs->TBCTL.bit.PHSDIR = UpDir ? TB_UP : TB_DOWN;
	// select phase synchronization method
	EPwmxRegs->TBCTL.bit.SYNCOSEL = (Int16U)Sync;

	// Enable if requested
	if(Enable)
		EPwmxRegs->TBCTL.bit.PHSEN = TB_ENABLE;
}
// -----------------------------------------

void ZwPWMx_ConfigET(volatile struct EPWM_REGS *EPwmxRegs, Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod,
					 Boolean EnableSOCB, ZwPWMETType SOCBType, ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod)
{
	// Set prescalers for events
	EPwmxRegs->ETPS.bit.SOCAPRD = (Int16U)SOCAPeriod;
	EPwmxRegs->ETPS.bit.SOCBPRD = (Int16U)SOCBPeriod;
	EPwmxRegs->ETPS.bit.INTPRD = (Int16U)IntPeriod;

	// Set event types
	EPwmxRegs->ETSEL.bit.SOCASEL = (Int16U)SOCAType;
	EPwmxRegs->ETSEL.bit.SOCBSEL = (Int16U)SOCBType;
	EPwmxRegs->ETSEL.bit.INTSEL = (Int16U)IntType;

	// Enable events
	EPwmxRegs->ETSEL.bit.SOCAEN = EnableSOCA ? 1 : 0;
	EPwmxRegs->ETSEL.bit.SOCBEN = EnableSOCB ? 1 : 0;
}
// -----------------------------------------

void ZwPWMx_InitPWM(volatile struct EPWM_REGS *EPwmxRegs, ZwPWMType Type, Int32U CPUdivPWM, Boolean A, Boolean B,
					Boolean ATZLow, Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB)
{
   ZwPWMx_SetFrequency(EPwmxRegs, CPUdivPWM, Type == PWMUpDown);

   // Setup TBCLK
   EPwmxRegs->TBCTL.bit.CTRMODE = (Int16U)Type;
   EPwmxRegs->TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
   EPwmxRegs->TBCTL.bit.PHSEN = TB_DISABLE;        	// Disable phase loading
   EPwmxRegs->TBCTL.bit.HSPCLKDIV = TB_DIV1;       	// Clock ratio to SYSCLKOUT
   EPwmxRegs->TBCTL.bit.CLKDIV = TB_DIV1;			// Div 1
   EPwmxRegs->TBCTL.bit.FREE_SOFT = 0x11;			// Ignore emulator connection	
   EPwmxRegs->TBCTL.bit.PRDLD = LoadOnShadow ? TB_SHADOW : TB_IMMEDIATE;					// Load on zero

   if(LoadOnShadow)
   {
	   EPwmxRegs->CMPCTL.bit.SHDWAMODE = CC_SHADOW;    	// Set automanagement mode
	   EPwmxRegs->CMPCTL.bit.SHDWBMODE = CC_SHADOW;	  	// 	with shadow
	   EPwmxRegs->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // Load registers every ZERO or PERIOD
	   EPwmxRegs->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // Load registers every ZERO or PERIOD
   }
   else
   {
	   EPwmxRegs->CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;  // Set manual mode
	   EPwmxRegs->CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;	// 	without shadow
   }

   EPwmxRegs->AQSFRC.bit.RLDCSF = LoadOnShadow ? LDCSF_ZERO_PRD : LDCSF_IMMEDIATE;

   // Setup compare 
   EPwmxRegs->CMPA.half.CMPA = 0;					// CMPA to zero
   EPwmxRegs->CMPB = 0;								// CMPB to zero
   
   // Set actions
   if(A)
   {
   		if(Type != PWMUpDown)
   		{
			EPwmxRegs->AQCTLA.bit.ZRO = AQ_SET;			// Set PWMxA on zero
			EPwmxRegs->AQCTLA.bit.CAU = AQ_CLEAR;		// Reset PWMxA on CMPA
   		}
   		else
   		{
			EPwmxRegs->AQCTLA.bit.CAU = AQ_CLEAR;		// Reset PWMxA on CMPA UP
			EPwmxRegs->AQCTLA.bit.CAD = AQ_SET;			// Set PWMxA on CMPA DOWN
   		}
   }
   
   if(B)
   {
  		if(Type != PWMUpDown)
  		{
			EPwmxRegs->AQCTLB.bit.ZRO = (InvertB) ? AQ_CLEAR : AQ_SET;			// Set PWMxB on zero
			if(ChBCmpB)
				EPwmxRegs->AQCTLB.bit.CBU = (InvertB) ? AQ_SET : AQ_CLEAR;		// Reset PWMxB on CMPB
			else
				EPwmxRegs->AQCTLB.bit.CAU = (InvertB) ? AQ_SET : AQ_CLEAR;		// Reset PWMxB on CMPB
		}
   		else
   		{
			if(ChBCmpB)
			{
				EPwmxRegs->AQCTLB.bit.CBU = (InvertB) ? AQ_SET : AQ_CLEAR;		// Reset PWMxB on CMPB UP
				EPwmxRegs->AQCTLB.bit.CBD = (InvertB) ? AQ_CLEAR : AQ_SET;		// Set PWMxB on CMPB DOWN
			}
			else
			{
				EPwmxRegs->AQCTLB.bit.CAU = (InvertB) ? AQ_SET : AQ_CLEAR;		// Reset PWMxB on CMPB UP
				EPwmxRegs->AQCTLB.bit.CAD = (InvertB) ? AQ_CLEAR : AQ_SET;		// Set PWMxB on CMPB DOWN
			}
   		}
  	}
   
   // TZ registers are EALLOW-protected
   EALLOW;

   // Init Trip Zone Control Register
   if(A)
   		EPwmxRegs->TZCTL.bit.TZA = ATZLow ? TZ_FORCE_LO : TZ_FORCE_HI; 	// force LOW or HIGH state for PWMxA
   if(B)
   		EPwmxRegs->TZCTL.bit.TZB = BTZLow ? TZ_FORCE_LO : TZ_FORCE_HI; 	// force LOW or HIGH state for PWMxB
   
   EDIS;
}
// ----------------------------------------- 

void ZwPWMx_InitTZ(volatile struct EPWM_REGS *EPwmxRegs, Int16U TZMaskCBC, Int16U TZMaskOST)
{
    EALLOW;

	if(TZMaskCBC & BIT0)
		EPwmxRegs->TZSEL.bit.CBC1 = TZ_ENABLE;                 // use TZ pin as cycle-by-cycle
	if(TZMaskCBC & BIT1)
		EPwmxRegs->TZSEL.bit.CBC2 = TZ_ENABLE;                 // use TZ pin as cycle-by-cycle
	if(TZMaskCBC & BIT2)
		EPwmxRegs->TZSEL.bit.CBC3 = TZ_ENABLE;                 // use TZ pin as cycle-by-cycle
	if(TZMaskCBC & BIT3)
		EPwmxRegs->TZSEL.bit.CBC4 = TZ_ENABLE;                 // use TZ pin as cycle-by-cycle
	if(TZMaskCBC & BIT4)
		EPwmxRegs->TZSEL.bit.CBC5 = TZ_ENABLE;                 // use TZ pin as cycle-by-cycle
	if(TZMaskCBC & BIT5)
		EPwmxRegs->TZSEL.bit.CBC6 = TZ_ENABLE;                 // use TZ pin as cycle-by-cycle

	if(TZMaskOST & BIT0)
		EPwmxRegs->TZSEL.bit.OSHT1 = TZ_ENABLE;                 // use TZ pin as one-shot
	if(TZMaskOST & BIT1)
		EPwmxRegs->TZSEL.bit.OSHT2 = TZ_ENABLE;                 // use TZ pin as one-shot
	if(TZMaskOST & BIT2)
		EPwmxRegs->TZSEL.bit.OSHT3 = TZ_ENABLE;                 // use TZ pin as one-shot
	if(TZMaskOST & BIT3)
		EPwmxRegs->TZSEL.bit.OSHT4 = TZ_ENABLE;                 // use TZ pin as one-shot
	if(TZMaskOST & BIT4)
		EPwmxRegs->TZSEL.bit.OSHT5 = TZ_ENABLE;                 // use TZ pin as one-shot
	if(TZMaskOST & BIT5)
		EPwmxRegs->TZSEL.bit.OSHT6 = TZ_ENABLE;                 // use TZ pin as one-shot

    EDIS;
}
// -----------------------------------------

void ZwPWM1_InitGPIO(Boolean A, Boolean B)
{
	EALLOW;
   
    if (A)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;    // Configure GPIO0 as EPWM1A
    	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // Enable pull-up on GPIO0 (EPWM1A)
    }

    if (B)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;    // Configure GPIO1 as EPWM1B
    	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;    // Enable pull-up on GPIO1 (EPWM1B)
    }
   
    EDIS;
}
// -----------------------------------------

void ZwPWM2_InitGPIO(Boolean A, Boolean B)
{
	EALLOW;
   
    if (A)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;    // Configure GPIO2 as EPWM2A
    	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;    // Enable pull-up on GPIO2 (EPWM2A)
    }

    if (B)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;    // Configure GPIO3 as EPWM2B
    	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;    // Enable pull-up on GPIO3 (EPWM2B)
    }
   
    EDIS;
}
// -----------------------------------------

void ZwPWM3_InitGPIO(Boolean A, Boolean B)
{
	EALLOW;
   
    if (A)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;    // Configure GPIO4 as EPWM3A
    	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    // Enable pull-up on GPIO4 (EPWM3A)
    }

    if (B)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;    // Configure GPIO5 as EPWM3B
    	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (EPWM3B)
    }
   
    EDIS;
}
// -----------------------------------------

void ZwPWM4_InitGPIO(Boolean A, Boolean B)
{
	EALLOW;
   
    if (A)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;    // Configure GPIO6 as EPWM4A
    	GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWM4A)
    }

    if (B)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;    // Configure GPIO7 as EPWM4B
    	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;    // Enable pull-up on GPIO7 (EPWM4B)
    }
   
    EDIS;
}
// -----------------------------------------

void ZwPWM5_InitGPIO(Boolean A, Boolean B)
{
	EALLOW;
   
    if (A)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;    // Configure GPIO8 as EPWM5A
    	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pull-up on GPIO8 (EPWM5A)
    }

    if (B)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;    // Configure GPIO9 as EPWM5B
    	GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;    // Enable pull-up on GPIO9 (EPWM5B)
    }
   
    EDIS;
}
// -----------------------------------------

void ZwPWM6_InitGPIO(Boolean A, Boolean B)
{
	EALLOW;
   
    if (A)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;    // Configure GPIO10 as EPWM6A
    	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;    // Enable pull-up on GPIO10 (EPWM6A)
    }

    if (B)
    {
    	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;    // Configure GPIO11 as EPWM6B
    	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up on GPIO11 (EPWM6B)
    }
   
    EDIS;
}

// MEMORY IO
// -------------------------------------------------------------------

//----------------------------------------
#pragma DATA_SECTION(EPwm1Regs,"EPwm1RegsFile");
volatile struct EPWM_REGS EPwm1Regs;

//----------------------------------------
#pragma DATA_SECTION(EPwm2Regs,"EPwm2RegsFile");
volatile struct EPWM_REGS EPwm2Regs;

//----------------------------------------
#pragma DATA_SECTION(EPwm3Regs,"EPwm3RegsFile");
volatile struct EPWM_REGS EPwm3Regs;

//----------------------------------------
#pragma DATA_SECTION(EPwm4Regs,"EPwm4RegsFile");
volatile struct EPWM_REGS EPwm4Regs;

//----------------------------------------
#pragma DATA_SECTION(EPwm5Regs,"EPwm5RegsFile");
volatile struct EPWM_REGS EPwm5Regs;

//----------------------------------------
#pragma DATA_SECTION(EPwm6Regs,"EPwm6RegsFile");
volatile struct EPWM_REGS EPwm6Regs;
