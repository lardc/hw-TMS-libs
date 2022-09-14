// -----------------------------------------
// ADC operational functions
// ----------------------------------------- 

#ifndef __ZW_ADC_H
#define __ZW_ADC_H

// Include
#include "stdinc.h"
//
// IO defs
//
#include "_ZwADC_Defs.h"

// Types
//
typedef void (*FUNC_ADCResultSubscriber)(Int16U *const restrict Result);


// Variables
//
extern FUNC_ADCResultSubscriber SubscriberFunc1, SubscriberFunc2;


// Functions
//
// Initialize ADC
void ZwADC_Init(Int16U ClockPrescaler, Boolean CoreDiv2, Int16U SHWindow);
// Setup source channels for ADC sequencer
void ZwADC_ConfigureSimultaneousCascaded(Int16U ConversionsCount, const Int16U ChannelsAB[8]);
void ZwADC_ConfigureSimultaneousDual(Int16U ConversionsCount1, const Int16U ChannelsAB1[4],
									 Int16U ConversionsCount2, const Int16U ChannelsAB2[4]);
void ZwADC_ConfigureSequentialCascaded(Int16U ConversionsCount, const Int16U Channels[16]);
void ZwADC_ConfigureSequentialDual(Int16U ConversionsCount1, const Int16U Channels1[8],
									 Int16U ConversionsCount2, const Int16U Channels2[8]);
// Config interrupts
void ZwADC_ConfigInterrupts(Boolean Enable1, Boolean Enable2);
// Enable peripheral interrupts
void ZwADC_EnableInterrupts(Boolean Enable1, Boolean Enable2);
// Enable CPU-level interrupts (IER)
void ZwADC_EnableInterruptsGlobal(Boolean Enable);
// COnfigure SOC signals
void ZwADC_ConfigureSOC(Boolean EnableSOCA, Boolean EnableSOCB, Boolean EnableSOCBCasc, Boolean EnableSOCExt);
// Make subscription to results for Dispatch
FUNC_ADCResultSubscriber ZwADC_SubscribeToResults1(FUNC_ADCResultSubscriber Subscriber);
FUNC_ADCResultSubscriber ZwADC_SubscribeToResults2(FUNC_ADCResultSubscriber Subscriber);
// Perform module calibration
void ZwADC_CalibrateLO(Int16U Channel);

// Inline functions
//
// Process interrupt
void inline ZwADC_ProcessInterruptSEQ1()
{
  	// Reinitialize for next ADC sequence
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
}
//
// Process interrupt
void inline ZwADC_ProcessInterruptSEQ2()
{
  	// Reinitialize for next ADC sequence
	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;         // Reset SEQ1
	AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;       // Clear INT SEQ2 bit
}
//
// Get values
pInt16U inline ZwADC_GetValues1()
{
	return (pInt16U)&AdcMirror;
}
// Get values for SEQ2
pInt16U inline ZwADC_GetValues2()
{
	return (pInt16U)&AdcMirror.ADCRESULT8;
}
//
// Dispatch results to subscriber
void inline ZwADC_Dispatch1()
{
	if(SubscriberFunc1)
		SubscriberFunc1(ZwADC_GetValues1());
}
//
// Dispatch results to subscriber
void inline ZwADC_Dispatch2()
{
	if(SubscriberFunc2)
		SubscriberFunc2(ZwADC_GetValues2());
}
//
// Set ADC sample window length
void inline ZwADC_AlterSH(Int16U SHWindow)
{
    // Set S/H window width
    AdcRegs.ADCTRL1.bit.ACQ_PS = SHWindow;
}
//
// Start sequencer 1
void inline ZwADC_StartSEQ1()
{
	// Start sequencer
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0x01;
}
//
// Start sequencer 2
void inline ZwADC_StartSEQ2()
{
	// Start sequencer
	AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 0x01;
}
//
// Get SEQ1 status
Boolean inline ZwADC_IsSEQ1Busy()
{
	return AdcRegs.ADCST.bit.SEQ1_BSY;
}
//
// Get SEQ2 status
Boolean inline ZwADC_IsSEQ2Busy()
{
	return AdcRegs.ADCST.bit.SEQ2_BSY;
}

// Macro
#define ADC_ISR_DONE 	  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1

#endif // __ZW_ADC_H
