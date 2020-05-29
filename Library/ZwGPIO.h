// -----------------------------------------
// GPIO management functions
// ----------------------------------------

#ifndef __ZW_GPIO_H
#define __ZW_GPIO_H

// Include
#include "stdinc.h"
//
// IO defs
#include "_ZwGPIO_Defs.h"

// Functions
//
// Init GPIO service
void ZwGPIO_Init(Int16U GPASamplingCoff0, Int16U GPASamplingCoff1, Int16U GPASamplingCoff2, Int16U GPASamplingCoff3, Int16U GPBSamplingCoff);
// Set pin to write mode
void ZwGPIO_PinToOutput(Int16U PinNumber);
// Set pin to read mode
void ZwGPIO_PinToInput(Int16U PinNumber, Boolean EnablePullup, Int16U SampleNumber);

// Inline functions
//
// Read pin A/B
Boolean inline ZwGPIO_ReadPin(Int16U PinNumber)
{
	return (PinNumber < 32) ? ((GpioDataRegs.GPADAT.all & (0x01L << PinNumber)) ? TRUE : FALSE)
							: ((GpioDataRegs.GPBDAT.all & (0x01L << (PinNumber - 32))) ? TRUE : FALSE);
}
//----------------------------------------

// Toggle pin A/B
void inline ZwGPIO_TogglePin(Int16U PinNumber)
{
	if(PinNumber < 32)
		GpioDataRegs.GPATOGGLE.all = (0x01L << PinNumber);
	else
	    GpioDataRegs.GPBTOGGLE.all = (0x01L << (PinNumber - 32));
}
//----------------------------------------

// Write pin A/B
void inline ZwGPIO_WritePin(Int16U PinNumber, Boolean Value)
{
	if(PinNumber < 32)
	{
		if(Value)
			GpioDataRegs.GPASET.all = (0x01L << PinNumber);
		else
			GpioDataRegs.GPACLEAR.all = (0x01L << PinNumber);
	}
	else
	{
		if(Value)
			GpioDataRegs.GPBSET.all = (0x01L << (PinNumber - 32));
		else
			GpioDataRegs.GPBCLEAR.all = (0x01L << (PinNumber - 32));
	}
}
//----------------------------------------

#endif // end __ZW_GPIO_H
