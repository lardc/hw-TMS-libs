// -----------------------------------------
// GPIO management functions.
// ----------------------------------------

// Header
#include "ZwGPIO.h"

// This function initializes the Gpio to a known (default) state.
void ZwGPIO_Init(Int16U GPASamplingCoff0, Int16U GPASamplingCoff1, Int16U GPASamplingCoff2, Int16U GPASamplingCoff3, Int16U GPBSamplingCoff)
{
	EALLOW;

    // By default, all are GPIO 
    GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
    GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
    GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO34

    // By default, GPIO are inputs 
    GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are inputs
    GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO34 are inputs   
   
    // Input synchronized to SYSCLKOUT
    GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT 
    GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
    GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO34 Synch to SYSCLKOUT 

    // Pull-ups can be enabled or disabled. 
    GpioCtrlRegs.GPAPUD.all = 0x00000FFF;  // Pullup's enabled GPIO0-GPIO31
    GpioCtrlRegs.GPBPUD.all = 0x0;         // Pullup's enabled GPIO32-GPIO34

	// Sampling frequency
	GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = GPASamplingCoff0;
	GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = GPASamplingCoff1;
	GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = GPASamplingCoff2;
	GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = GPASamplingCoff3;
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = GPBSamplingCoff;

    EDIS;
}	
//----------------------------------------

// Set pin write mode
void ZwGPIO_PinToOutput(Int16U PinNumber)
{
	EALLOW;
	
	// Mode = TRUE - write
	if(PinNumber < 32)
		GpioCtrlRegs.GPADIR.all |= (0x01L << PinNumber);
	else
		GpioCtrlRegs.GPBDIR.all |= (0x01L << (PinNumber - 32));
	
	if(PinNumber < 16)
		GpioCtrlRegs.GPAMUX1.all &= ~(0x03L << (PinNumber * 2));
	else if(PinNumber < 32)
		GpioCtrlRegs.GPAMUX2.all &= ~(0x03L << ((PinNumber - 16) * 2));
	else
		GpioCtrlRegs.GPBMUX1.all &= ~(0x03L << ((PinNumber - 32) * 2));

	EDIS; 
}
//----------------------------------------

// Set pin to read mode
void ZwGPIO_PinToInput(Int16U PinNumber, Boolean EnablePullup, Int16U SampleNumber)
{
	Int32U mask;

	if(PinNumber < 32)
	{
		EALLOW;

		// Mode = FALSE - read
		GpioCtrlRegs.GPADIR.all &= ~(0x01L << PinNumber);

		// Pull-up
		if(EnablePullup)
			GpioCtrlRegs.GPAPUD.all &= ~(0x01L << PinNumber);
		else
			GpioCtrlRegs.GPAPUD.all |= (0x01L << PinNumber);


		// Bit-mask for QSEL
		switch(SampleNumber)
		{
			case 3:
				mask = 0x01;	// 01b
				break;
			case 6:
				mask = 0x02;	// 10b
				break;
			default:
				mask = 0x00; 	// 00b
				break;
		}

		// Set registers
		if(PinNumber < 16)
		{
			GpioCtrlRegs.GPAQSEL1.all |= (mask << (PinNumber * 2));
		}
		else
		{
			PinNumber -= 16;
			GpioCtrlRegs.GPAQSEL2.all |= (mask << (PinNumber * 2));
		}

		EDIS;
	}
	else
	{
		EALLOW;

		// Mode = FALSE - read
		GpioCtrlRegs.GPBDIR.all &= ~(0x01L << (PinNumber - 32));

		// Pull-up
		if(EnablePullup)
			GpioCtrlRegs.GPBPUD.all &= ~(0x01L << (PinNumber - 32));
		else
			GpioCtrlRegs.GPBPUD.all |= (0x01L << (PinNumber - 32));

		// Bit-mask for QSEL
		switch(SampleNumber)
		{
			case 3:
				mask = 0x01;	// 01b
				break;
			case 6:
				mask = 0x02;	// 10b
				break;
			default:
				mask = 0x00; 	// 00b
				break;
		}

		// Set registers
		GpioCtrlRegs.GPBQSEL1.all |= (mask << ((PinNumber - 32) * 2));


		if(PinNumber < 16)
			GpioCtrlRegs.GPAMUX1.all &= ~(0x03L << (PinNumber * 2));
		else if(PinNumber < 32)
			GpioCtrlRegs.GPAMUX2.all &= ~(0x03L << ((PinNumber - 16) * 2));
		else
			GpioCtrlRegs.GPBMUX1.all &= ~(0x03L << ((PinNumber - 32) * 2));

		EDIS;
	}
}

//----------------------------------------
#pragma DATA_SECTION(GpioCtrlRegs,"GpioCtrlRegsFile");
volatile struct GPIO_CTRL_REGS GpioCtrlRegs;

//----------------------------------------
#pragma DATA_SECTION(GpioDataRegs,"GpioDataRegsFile");
volatile struct GPIO_DATA_REGS GpioDataRegs;

//----------------------------------------
#pragma DATA_SECTION(GpioIntRegs,"GpioIntRegsFile");
volatile struct GPIO_INT_REGS GpioIntRegs; 

// No more.
