// -----------------------------------------
// PWM operational functions
// ----------------------------------------- 

#ifndef __ZW_PWM_H
#define __ZW_PWM_H

// Include
#include "stdinc.h"
//
// IO defs
#include "_ZwPWM_Defs.h"
#include "ZwPIE.h"
#include "ZwSysCtrl.h"

// Types
//
typedef enum __ZwPWMType
{
	PWMUp = TB_COUNT_UP,
	PWMDown = TB_COUNT_DOWN,
	PWMUpDown = TB_COUNT_UPDOWN
} ZwPWMType;
//
typedef enum __ZwPWMSYNCType
{
	SYNCIn = TB_SYNC_IN,
	SYNCZero = TB_CTR_ZERO,
	SYNCCmpB = TB_CTR_CMPB,
	SYNCDisable = TB_SYNC_DISABLE
} ZwPWMSYNCType;
//
typedef enum __ZwPWMETType
{
	ETNone = 0,
	ETZero = ET_CTR_ZERO,
	ETPeriod = ET_CTR_PRD,
	ETCmpAUp = ET_CTRU_CMPA,
	ETCmpADown = ET_CTRD_CMPA,
	ETCmpBUp = ET_CTRU_CMPB,
	ETCmpBDown = ET_CTRD_CMPB
} ZwPWMETType;
//
typedef enum __ZwPWMETPrescaler
{
	ETPNone = 0,
	ETPEvery = ET_1ST,
	ETPOdd = ET_2ND,
	ETP3d = ET_3RD
} ZwPWMETPrescaler;
//
typedef enum __ZwPWMFunctional
{
	PFEnable,
	PFDisable,
	PFDontcare
} ZwPWMFunctional;

typedef enum __ZwPWMQualification
{
	PQ_Async = 0,
	PQ_SysClock,
	PQ_Sample3,
	PQ_Sample6
} ZwPWMQualification;

// Functions
//
// Init PWMx
void ZwPWM1_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST,
				 Boolean ATZLow, Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB);
void ZwPWM2_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST,
				 Boolean ATZLow, Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB);
void ZwPWM3_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST,
				 Boolean ATZLow, Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB);
void ZwPWM4_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST,
				 Boolean ATZLow, Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB);
void ZwPWM5_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST,
				 Boolean ATZLow, Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB);
void ZwPWM6_Init(ZwPWMType Type, Int32U CPU_Freq, Int32U PWM_Freq, Boolean A, Boolean B, Int16U TZMaskCBC, Int16U TZMaskOST,
				 Boolean ATZLow, Boolean BTZLow, Boolean LoadOnShadow, Boolean InvertB, Boolean ChBCmpB);
// Init TZ pins separately
void ZwPWM_InitTZIndirect(Boolean TZ1, Boolean TZ2, Boolean TZ3, Boolean TZ4, Boolean TZ5, Boolean TZ6, ZwPWMQualification Qualification);
// Set TZ pull'up mode
void ZwPWM_SetTZPullup(ZwPWMFunctional TZ1, ZwPWMFunctional TZ2, ZwPWMFunctional TZ3, ZwPWMFunctional TZ4, ZwPWMFunctional TZ5, ZwPWMFunctional TZ6);
// Init interrupts for TZ OST
void ZwPWM_ConfigTZIntOST(Boolean PWM1, Boolean PWM2, Boolean PWM3, Boolean PWM4, Boolean PWM5, Boolean PWM6);
// Init interrupts for TZ CBC
void ZwPWM_ConfigTZIntCBC(Boolean PWM1, Boolean PWM2, Boolean PWM3, Boolean PWM4, Boolean PWM5, Boolean PWM6);
// Init interrupts for ET
void ZwPWM_ConfigETInt(Boolean ET1, Boolean ET2, Boolean ET3, Boolean ET4, Boolean ET5, Boolean ET6);
// Init ET services
void ZwPWM1_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	     ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod);
void ZwPWM2_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	     ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod);
void ZwPWM3_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	     ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod);
void ZwPWM4_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	     ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod);
void ZwPWM5_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	     ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod);
void ZwPWM6_ConfigET(Boolean EnableSOCA, ZwPWMETType SOCAType, ZwPWMETPrescaler SOCAPeriod, Boolean EnableSOCB, ZwPWMETType SOCBType,
		 	 	     ZwPWMETPrescaler SOCBPeriod, ZwPWMETType IntType, ZwPWMETPrescaler IntPeriod);
// Alter TZ mode
void ZwPWM_ConfigTZ1(Boolean Enable, ZwPWMQualification Qualification);
void ZwPWM_ConfigTZ2(Boolean Enable, ZwPWMQualification Qualification);
void ZwPWM_ConfigTZ3(Boolean Enable, ZwPWMQualification Qualification);
void ZwPWM_ConfigTZ4(Boolean Enable, ZwPWMQualification Qualification);
void ZwPWM_ConfigTZ5(Boolean Enable, ZwPWMQualification Qualification);
void ZwPWM_ConfigTZ6(Boolean Enable, ZwPWMQualification Qualification);

// Internal functions
void ZwPWMx_ClearTZ(volatile struct EPWM_REGS *EPwmxRegs);
void ZwPWMx_SetValueA(volatile struct EPWM_REGS *EPwmxRegs, Int16S Duty_A);
void ZwPWMx_SetValueB(volatile struct EPWM_REGS *EPwmxRegs, Int16S Duty_B);
void ZwPWMx_SetFrequency(volatile struct EPWM_REGS *EPwmxRegs, Int32U PWM_Freq, Boolean IsBidirectional);
void ZwPWMx_SetPhase(volatile struct EPWM_REGS *EPwmxRegs, Boolean Enable, Int32U PhaseValue, Boolean UpDir, ZwPWMSYNCType Sync);


// Inline functions
//
// Enable modules clocking
void inline ZwPWM_Enable(Boolean Enable)
{
    // Enable clocking
    EALLOW;
    	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = Enable ? 1 : 0;
    EDIS;
}
// Set PWMx duty for A channel
void inline ZwPWM1_SetValueA(Int16S Duty_A)
{
	ZwPWMx_SetValueA(&EPwm1Regs, Duty_A);
}
//
void inline ZwPWM2_SetValueA(Int16S Duty_A)
{
	ZwPWMx_SetValueA(&EPwm2Regs, Duty_A);
}
//
void inline ZwPWM3_SetValueA(Int16S Duty_A)
{
	ZwPWMx_SetValueA(&EPwm3Regs, Duty_A);
}
//
void inline ZwPWM4_SetValueA(Int16S Duty_A)
{
	ZwPWMx_SetValueA(&EPwm4Regs, Duty_A);
}
//
void inline ZwPWM5_SetValueA(Int16S Duty_A)
{
	ZwPWMx_SetValueA(&EPwm5Regs, Duty_A);
}
//
void inline ZwPWM6_SetValueA(Int16S Duty_A)
{
	ZwPWMx_SetValueA(&EPwm6Regs, Duty_A);
}
// Set PWMx duty for B channel
void inline ZwPWM1_SetValueB(Int16S Duty_B)
{
	ZwPWMx_SetValueB(&EPwm1Regs, Duty_B);
}
//
// Set PWM2 duty in up- or down-mode
void inline ZwPWM2_SetValueB(Int16S Duty_B)
{
	ZwPWMx_SetValueB(&EPwm2Regs, Duty_B);
}
//
// Set PWM3 duty in up- or down-mode
void inline ZwPWM3_SetValueB(Int16S Duty_B)
{
	ZwPWMx_SetValueB(&EPwm3Regs, Duty_B);
}
//
// Set PWM4 duty in up- or down-mode
void inline ZwPWM4_SetValueB(Int16S Duty_B)
{
	ZwPWMx_SetValueB(&EPwm4Regs, Duty_B);
}
//
// Set PWM5 duty in up- or down-mode
void inline ZwPWM5_SetValueB(Int16S Duty_B)
{
	ZwPWMx_SetValueB(&EPwm5Regs, Duty_B);
}
//
// Set PWM6 duty in up- or down-mode
void inline ZwPWM6_SetValueB(Int16S Duty_B)
{
	ZwPWMx_SetValueB(&EPwm6Regs, Duty_B);
}
//
void inline ZwPWM1_SetFrequency(Int32U PWM_Freq, Boolean IsBidirectional)
{
	ZwPWMx_SetFrequency(&EPwm1Regs, CPU_FRQ / PWM_Freq, IsBidirectional);
}
//
void inline ZwPWM2_SetFrequency(Int32U PWM_Freq, Boolean IsBidirectional)
{
	ZwPWMx_SetFrequency(&EPwm2Regs, CPU_FRQ / PWM_Freq, IsBidirectional);
}
//
void inline ZwPWM3_SetFrequency(Int32U PWM_Freq, Boolean IsBidirectional)
{
	ZwPWMx_SetFrequency(&EPwm3Regs, CPU_FRQ / PWM_Freq, IsBidirectional);
}
//
void inline ZwPWM4_SetFrequency(Int32U PWM_Freq, Boolean IsBidirectional)
{
	ZwPWMx_SetFrequency(&EPwm4Regs, CPU_FRQ / PWM_Freq, IsBidirectional);
}
//
void inline ZwPWM5_SetFrequency(Int32U PWM_Freq, Boolean IsBidirectional)
{
	ZwPWMx_SetFrequency(&EPwm5Regs, CPU_FRQ / PWM_Freq, IsBidirectional);
}
//
void inline ZwPWM6_SetFrequency(Int32U PWM_Freq, Boolean IsBidirectional)
{
	ZwPWMx_SetFrequency(&EPwm6Regs, CPU_FRQ / PWM_Freq, IsBidirectional);
}
//
// Set phase parameters
void inline ZwPWM1_SetPhase(volatile struct EPWM_REGS *EPwmxRegs, Boolean Enable, Int32U PhaseValue, Boolean UpDir, ZwPWMSYNCType Sync)
{
	ZwPWMx_SetPhase(&EPwm1Regs, Enable, PhaseValue, UpDir, Sync);
}
//
void inline ZwPWM2_SetPhase(volatile struct EPWM_REGS *EPwmxRegs, Boolean Enable, Int32U PhaseValue, Boolean UpDir, ZwPWMSYNCType Sync)
{
	ZwPWMx_SetPhase(&EPwm2Regs, Enable, PhaseValue, UpDir, Sync);
}
//
void inline ZwPWM3_SetPhase(volatile struct EPWM_REGS *EPwmxRegs, Boolean Enable, Int32U PhaseValue, Boolean UpDir, ZwPWMSYNCType Sync)
{
	ZwPWMx_SetPhase(&EPwm3Regs, Enable, PhaseValue, UpDir, Sync);
}
//
void inline ZwPWM4_SetPhase(volatile struct EPWM_REGS *EPwmxRegs, Boolean Enable, Int32U PhaseValue, Boolean UpDir, ZwPWMSYNCType Sync)
{
	ZwPWMx_SetPhase(&EPwm4Regs, Enable, PhaseValue, UpDir, Sync);
}
//
void inline ZwPWM5_SetPhase(volatile struct EPWM_REGS *EPwmxRegs, Boolean Enable, Int32U PhaseValue, Boolean UpDir, ZwPWMSYNCType Sync)
{
	ZwPWMx_SetPhase(&EPwm5Regs, Enable, PhaseValue, UpDir, Sync);
}
//
void inline ZwPWM6_SetPhase(volatile struct EPWM_REGS *EPwmxRegs, Boolean Enable, Int32U PhaseValue, Boolean UpDir, ZwPWMSYNCType Sync)
{
	ZwPWMx_SetPhase(&EPwm6Regs, Enable, PhaseValue, UpDir, Sync);
}
//
// Handle PWMx TZ interrupt
void inline ZwPWM1_ProcessTZInterrupt()
{
	EALLOW;
	EPwm1Regs.TZCLR.bit.INT = 0x01;
	EDIS;
}
//
void inline ZwPWM2_ProcessTZInterrupt()
{
	EALLOW;
	EPwm2Regs.TZCLR.bit.INT = 0x01;
	EDIS;
}
//
void inline ZwPWM3_ProcessTZInterrupt()
{
	EALLOW;
	EPwm3Regs.TZCLR.bit.INT = 0x01;
	EDIS;
}
//
void inline ZwPWM4_ProcessTZInterrupt()
{
	EALLOW;
	EPwm4Regs.TZCLR.bit.INT = 0x01;
	EDIS;
}
//
void inline ZwPWM5_ProcessTZInterrupt()
{
	EALLOW;
	EPwm5Regs.TZCLR.bit.INT = 0x01;
	EDIS;
}
//
void inline ZwPWM6_ProcessTZInterrupt()
{
	EALLOW;
	EPwm6Regs.TZCLR.bit.INT = 0x01;
	EDIS;
}
//
// Handle PWMx ET interrupt
void inline ZwPWM1_ProcessETInterrupt()
{
	EPwm1Regs.ETCLR.bit.INT = 0x01;
}
//
void inline ZwPWM2_ProcessETInterrupt()
{
	EPwm2Regs.ETCLR.bit.INT = 0x01;
}
//
void inline ZwPWM3_ProcessETInterrupt()
{
	EPwm3Regs.ETCLR.bit.INT = 0x01;
}
//
void inline ZwPWM4_ProcessETInterrupt()
{
	EPwm4Regs.ETCLR.bit.INT = 0x01;
}
//
void inline ZwPWM5_ProcessETInterrupt()
{
	EPwm5Regs.ETCLR.bit.INT = 0x01;
}
//
void inline ZwPWM6_ProcessETInterrupt()
{
	EPwm6Regs.ETCLR.bit.INT = 0x01;
}
// Clear TZ condition
void inline ZwPWM1_ClearTZ()
{
	ZwPWMx_ClearTZ(&EPwm1Regs);
}
//
void inline ZwPWM2_ClearTZ()
{
	ZwPWMx_ClearTZ(&EPwm2Regs);
}
//
void inline ZwPWM3_ClearTZ()
{
	ZwPWMx_ClearTZ(&EPwm3Regs);
}
//
void inline ZwPWM4_ClearTZ()
{
	ZwPWMx_ClearTZ(&EPwm4Regs);
}
//
void inline ZwPWM5_ClearTZ()
{
	ZwPWMx_ClearTZ(&EPwm5Regs);
}
//
void inline ZwPWM6_ClearTZ()
{
	ZwPWMx_ClearTZ(&EPwm6Regs);
}
//
void inline ZwPWM1_AlterSOCA(ZwPWMETType SOCAType)
{
	EPwm1Regs.ETSEL.bit.SOCASEL = (Int16U)SOCAType;
}
//
void inline ZwPWM1_AlterSOCB(ZwPWMETType SOCBType)
{
	EPwm1Regs.ETSEL.bit.SOCBSEL = (Int16U)SOCBType;
}
//
void inline ZwPWM2_AlterSOCA(ZwPWMETType SOCAType)
{
	EPwm2Regs.ETSEL.bit.SOCASEL = (Int16U)SOCAType;
}
//
void inline ZwPWM2_AlterSOCB(ZwPWMETType SOCBType)
{
	EPwm2Regs.ETSEL.bit.SOCBSEL = (Int16U)SOCBType;
}
//
void inline ZwPWM3_AlterSOCA(ZwPWMETType SOCAType)
{
	EPwm3Regs.ETSEL.bit.SOCASEL = (Int16U)SOCAType;
}
//
void inline ZwPWM3_AlterSOCB(ZwPWMETType SOCBType)
{
	EPwm3Regs.ETSEL.bit.SOCBSEL = (Int16U)SOCBType;
}
//
void inline ZwPWM4_AlterSOCA(ZwPWMETType SOCAType)
{
	EPwm4Regs.ETSEL.bit.SOCASEL = (Int16U)SOCAType;
}
//
void inline ZwPWM4_AlterSOCB(ZwPWMETType SOCBType)
{
	EPwm4Regs.ETSEL.bit.SOCBSEL = (Int16U)SOCBType;
}
//
void inline ZwPWM5_AlterSOCA(ZwPWMETType SOCAType)
{
	EPwm5Regs.ETSEL.bit.SOCASEL = (Int16U)SOCAType;
}
//
void inline ZwPWM5_AlterSOCB(ZwPWMETType SOCBType)
{
	EPwm5Regs.ETSEL.bit.SOCBSEL = (Int16U)SOCBType;
}
//
void inline ZwPWM6_AlterSOCA(ZwPWMETType SOCAType)
{
	EPwm6Regs.ETSEL.bit.SOCASEL = (Int16U)SOCAType;
}
//
void inline ZwPWM6_AlterSOCB(ZwPWMETType SOCBType)
{
	EPwm6Regs.ETSEL.bit.SOCBSEL = (Int16U)SOCBType;
}
//
void inline ZwPWM_EnableTZInterruptsGlobal(Boolean Enable)
{
	// Enable CPU INT2 which is connected to PWMx TZ:
	if(Enable)
		IER |= M_INT2;
	else
		IER &= ~M_INT2;
}
//
Boolean inline ZwPWM1_GetTZLatchState(Boolean CBC)
{
	return CBC ? EPwm1Regs.TZFLG.bit.CBC : EPwm1Regs.TZFLG.bit.OST;
}
//
Boolean inline ZwPWM2_GetTZLatchState(Boolean CBC)
{
	return CBC ? EPwm2Regs.TZFLG.bit.CBC : EPwm1Regs.TZFLG.bit.OST;
}
//
Boolean inline ZwPWM3_GetTZLatchState(Boolean CBC)
{
	return CBC ? EPwm3Regs.TZFLG.bit.CBC : EPwm1Regs.TZFLG.bit.OST;
}
//
Boolean inline ZwPWM4_GetTZLatchState(Boolean CBC)
{
	return CBC ? EPwm4Regs.TZFLG.bit.CBC : EPwm1Regs.TZFLG.bit.OST;
}
//
Boolean inline ZwPWM5_GetTZLatchState(Boolean CBC)
{
	return CBC ? EPwm5Regs.TZFLG.bit.CBC : EPwm1Regs.TZFLG.bit.OST;
}
//
Boolean inline ZwPWM6_GetTZLatchState(Boolean CBC)
{
	return CBC ? EPwm6Regs.TZFLG.bit.CBC : EPwm1Regs.TZFLG.bit.OST;
}
// Enable TZ interrupts generation
void inline ZwPWM_EnableTZInterrupts(Boolean PWM1, Boolean PWM2, Boolean PWM3, Boolean PWM4, Boolean PWM5, Boolean PWM6)
{
    // Enable PWMx TZ interrupts
    PieCtrlRegs.PIEIER2.bit.INTx1 = PWM1 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER2.bit.INTx2 = PWM2 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER2.bit.INTx3 = PWM3 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER2.bit.INTx4 = PWM4 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER2.bit.INTx5 = PWM5 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER2.bit.INTx6 = PWM6 ? 0x01 : 0x00;
}
//
void inline ZwPWM_EnableETInterruptsGlobal(Boolean Enable)
{
	// Enable CPU INT3 which is connected to PWMx ET:
	if(Enable)
		IER |= M_INT3;
	else
		IER &= ~M_INT3;
}
// Enable ET interrupts generation
void inline ZwPWM_EnableETInterrupts(Boolean ET1, Boolean ET2, Boolean ET3, Boolean ET4, Boolean ET5, Boolean ET6)
{
	// Enable PWMx ET interrupts
    PieCtrlRegs.PIEIER3.bit.INTx1 = ET1 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER3.bit.INTx2 = ET2 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER3.bit.INTx3 = ET3 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER3.bit.INTx4 = ET4 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER3.bit.INTx5 = ET5 ? 0x01 : 0x00;
    PieCtrlRegs.PIEIER3.bit.INTx6 = ET6 ? 0x01 : 0x00;
}

// ISR post-fix
#define PWM_TZ_ISR_DONE    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2
#define PWM_INT_ISR_DONE    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3

#endif // __ZW_PWM_H
