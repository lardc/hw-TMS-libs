// -----------------------------------------
// API for DSP initializations and control
// ----------------------------------------

#ifndef __ZW_SYSCTRL_H
#define __ZW_SYSCTRL_H

// Include
#include "stdinc.h"
//
// IO defs
#include "_ZwSysCtrl_Defs.h"


// Constants
//
// Peripheral modules
#define	ZW_POWER_PWM1_CLK	 BIT0
#define	ZW_POWER_PWM2_CLK	 BIT1
#define	ZW_POWER_PWM3_CLK 	 BIT2
#define	ZW_POWER_PWM4_CLK 	 BIT3
#define	ZW_POWER_PWM5_CLK 	 BIT4
#define	ZW_POWER_PWM6_CLK 	 BIT5
#define	ZW_POWER_QEP1_CLK 	 BIT6
#define	ZW_POWER_QEP2_CLK 	 BIT7
#define	ZW_POWER_CANA_CLK    BIT8
#define	ZW_POWER_CANB_CLK 	 BIT9
#define	ZW_POWER_SCIA_CLK 	 BIT10
#define	ZW_POWER_SCIB_CLK 	 BIT11
#define	ZW_POWER_SPIA_CLK 	 BIT12	
#define	ZW_POWER_SPIB_CLK 	 BIT13	
#define	ZW_POWER_SPIC_CLK 	 BIT14	
#define	ZW_POWER_SPID_CLK 	 BIT15	
#define	ZW_POWER_ADC_CLK 	 BIT16	
#define	ZW_POWER_I2CA_CLK 	 BIT17
#define	ZW_POWER_CAP1_CLK 	 BIT18
#define	ZW_POWER_CAP2_CLK 	 BIT19


// Functions
//
// General system initialization
Boolean ZwSystem_Init(Int16U PLLValue, Int16U ClkInDiv, Int16U LowPClkPrescaler, Int16U HighPClkPrescaler, Int32U ModulesClock);
// Select watch-dog function: WDINT or WDRST
void ZwSystem_SelectDogFunc(Boolean Interrupt);
// Get current state of WDINT
Boolean ZwSystem_GetDogIntState();
// Enable WD with specified prescaler
void ZwSystem_EnableDog(Int16U Divider);
// Disable dog
void ZwSystem_DisableDog();
// Force system reset using WD
void ZwSystem_ForceDog();
// Clear WD counter
void ZwSystem_ServiceDog();
// Lock WD state
void ZwSystem_LockDog();
// Get alarm flag that indicates that system has been reseted by WD
Boolean ZwSystem_GetDogAlarmFlag();
// Clear WD alarm flag
void ZwSystem_ClearDogAlarmFlag();
// Alter peripheral clocks
void ZwSystem_AlterPeripheralClocks(Int32U ModulesClockToEnable, Int32U ModulesClockToDisable);

#endif // end __ZW_SYSCTRL_H
