// -----------------------------------------
// PWM H-bridge operational functions
// -----------------------------------------

// Header
#include "ZwPWMHBridge.h"

// Include
#include "ZwSysCtrl.h"
#include "ZwPIE.h"
#include "ZwGPIO.h"


// Variables
//
static Int16S PWMSaturation12 = 0;


// Functions
//
void ZwPWMB_InitBridgeA12(Int32U CPU_Freq, Int32U PWM_Freq, Int16U TZMaskCBC, Int16U TZMaskOST, Int16S Saturation)
{
	ZwPWM1_Init(PWMUp, CPU_Freq, PWM_Freq, TRUE, FALSE, TZMaskCBC, TZMaskOST, TRUE, TRUE, TRUE, FALSE, FALSE);
	ZwPWM2_Init(PWMUp, CPU_Freq, PWM_Freq, TRUE, FALSE, TZMaskCBC, TZMaskOST, TRUE, TRUE, TRUE, FALSE, FALSE);

	EPwm1Regs.AQCSFRC.bit.CSFB = AQCSFRC_HIGH;
	EPwm2Regs.AQCSFRC.bit.CSFB = AQCSFRC_HIGH;

	// Save saturation value for function PWM_SetValue
    PWMSaturation12 = Saturation;
}
// -----------------------------------------

void ZwPWMB_InitBridge12(Int32U CPU_Freq, Int32U PWM_Freq, Int16U TZMaskCBC, Int16U TZMaskOST, Int16U DBandValue, Int16S Saturation)
{
	ZwPWM1_Init(PWMUp, CPU_Freq, PWM_Freq, TRUE, TRUE, TZMaskCBC, TZMaskOST, TRUE, FALSE, TRUE, TRUE, FALSE);
	ZwPWM2_Init(PWMUp, CPU_Freq, PWM_Freq, TRUE, TRUE, TZMaskCBC, TZMaskOST, TRUE, FALSE, TRUE, TRUE, FALSE);

	EPwm1Regs.AQCSFRC.bit.CSFB = AQCSFRC_HIGH;
	EPwm2Regs.AQCSFRC.bit.CSFB = AQCSFRC_HIGH;

	// Save saturation value for function PWM_SetValue
    PWMSaturation12 = Saturation;
}
// -----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwPWMB_SetValue12, "ramfuncs");
#endif
void ZwPWMB_SetValue12(Int16S Duty)
{
	// Two-side saturation
	if(Duty >= PWMSaturation12)
		Duty = PWMSaturation12;

	if(Duty <= -PWMSaturation12)
		Duty = -PWMSaturation12;

    // Set PWM using sign
	if (Duty > 0)
    {
     	EPwm2Regs.CMPA.half.CMPA = 0;
     	EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD * ((Int32S)Duty) / ZW_PWM_DUTY_BASE;

     	EPwm1Regs.AQCSFRC.bit.CSFA = AQCSFRC_NO_SW;
     	EPwm2Regs.AQCSFRC.bit.CSFA = AQCSFRC_LOW;        // Forcing a continuous LOW on output A of EPWM2
    }
    else if(Duty < 0)
    {
    	EPwm1Regs.CMPA.half.CMPA = 0;
    	EPwm2Regs.CMPA.half.CMPA = EPwm2Regs.TBPRD * (-(Int32S)Duty) / ZW_PWM_DUTY_BASE;

    	EPwm2Regs.AQCSFRC.bit.CSFA = AQCSFRC_NO_SW;
     	EPwm1Regs.AQCSFRC.bit.CSFA = AQCSFRC_LOW;        // Forcing a continuous LOW on output A of EPWM1
    }
	else
	{
		EPwm1Regs.AQCSFRC.bit.CSFA = AQCSFRC_LOW;        // Forcing a continuous LOW on output A of EPWM1
    	EPwm2Regs.AQCSFRC.bit.CSFA = AQCSFRC_LOW;        // Forcing a continuous LOW on output A of EPWM2
	}
}

// No more.
