// -----------------------------------------
// PWM H-bridge operational functions
// -----------------------------------------

#ifndef __ZW_PWMHBRIDGE_H
#define __ZW_PWMHBRIDGE_H

// Include
#include "stdinc.h"
//
// IO defs
#include "ZwPWM.h"

// Functions
//
// Init PWM1 and PWM2 channels to control H-bridge (A outputs only)
void ZwPWMB_InitBridgeA12(Int32U CPU_Freq, Int32U PWM_Freq, Int16U TZMaskCBC, Int16U TZMaskOST, Int16S Saturation);
// Init PWM1 and PWM2 channels to control H-bridge
void ZwPWMB_InitBridge12(Int32U CPU_Freq, Int32U PWM_Freq, Int16U TZMaskCBC, Int16U TZMaskOST, Int16U DBandValue, Int16S Saturation);
// Set value to drive bridge
void ZwPWMB_SetValue12(Int16S Duty);

#endif // __ZW_PWMHBRIDGE_H
