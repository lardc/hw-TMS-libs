// -----------------------------------------
// API for SCI module
// ----------------------------------------

// Header
#include "ZwSCI.h"

// Include
#include "ZwGPIO.h"
#include "ZwPIE.h"
#include "ZwUtils.h"

// Constants
#define SCI_FIFO_LEN	16

// Forward functions
//
void ZwSCIa_InitGPIO(Boolean EnableRXPU);
void ZwSCIb_InitGPIO(Boolean EnableRXPU);
void ZwSCIx_Init(volatile struct SCI_REGS *ScixRegs, Int32U BaudRate, Int16U DataBits,
						 enum Zw_SCI_ParityMode Parity, Boolean TwoStopBits);
void ZwSCIx_InitFIFO(volatile struct SCI_REGS *ScixRegs, Int16U TXLevel, Int16U RXLevel);
Boolean ZwSCIx_ReceiveChar(volatile struct SCI_REGS *ScixRegs, pInt16U Char);
void ZwSCIx_SetRXState(volatile struct SCI_REGS *ScixRegs, Boolean EN);
void ZwSCIx_SetTXState(volatile struct SCI_REGS *ScixRegs, Boolean EN);

// Functions
//
void ZwSCIa_Init(Int32U BaudRate, Int16U DataBits, enum Zw_SCI_ParityMode Parity, Boolean TwoStopBits, Boolean EnableRXPU)
{
	ZwSCIa_InitGPIO(EnableRXPU);
	ZwSCIx_Init(&SciaRegs, BaudRate, DataBits, Parity, TwoStopBits);
}
// ----------------------------------------

void ZwSCIb_Init(Int32U BaudRate, Int16U DataBits, enum Zw_SCI_ParityMode Parity, Boolean TwoStopBits, Boolean EnableRXPU)
{
	ZwSCIb_InitGPIO(EnableRXPU);
	ZwSCIx_Init(&ScibRegs, BaudRate, DataBits, Parity, TwoStopBits);
}
// ----------------------------------------

void ZwSCIa_InitFIFO(Int16U TXLevel, Int16U RXLevel)
{
	ZwSCIx_InitFIFO(&SciaRegs, TXLevel, RXLevel);
}
// ----------------------------------------

void ZwSCIb_InitFIFO(Int16U TXLevel, Int16U RXLevel)
{
	ZwSCIx_InitFIFO(&ScibRegs, TXLevel, RXLevel);
}
// ----------------------------------------

// Enable RX & TX interrupt signals
void ZwSCIa_EnableInterrupts(Boolean Rx, Boolean Tx)
{
    // Enable SCIA RXINT interrupt
    PieCtrlRegs.PIEIER9.bit.INTx1 = Rx ? 0x01 : 0x00;
    // Enable SCIA TXINT interrupt
    PieCtrlRegs.PIEIER9.bit.INTx2 = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable RX & TX interrupt signals
void ZwSCIb_EnableInterrupts(Boolean Rx, Boolean Tx)
{
    // Enable SCIB RXINT interrupt
    PieCtrlRegs.PIEIER9.bit.INTx3 = Rx ? 0x01 : 0x00;
    // Enable SCIB TXINT interrupt
    PieCtrlRegs.PIEIER9.bit.INTx4 = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable RX & TX interrupt signals on CPU-level
void ZwSCI_EnableInterruptsGlobal(Boolean Enable)
{
    // Enable CPU INT9 which is connected to SCI-AB:
    if(Enable)
    	IER |= M_INT9;
    else
    	IER &= ~M_INT9;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_SendChar, "ramfuncs");
#endif
void ZwSCIa_SendChar(Int16U Value)
{
	// Wait for free place in queue
	while(SciaRegs.SCIFFTX.bit.TXFFST == SCI_FIFO_LEN) { }
    // Write byte
    SciaRegs.SCITXBUF = Value;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_SendChar, "ramfuncs");
#endif
void ZwSCIb_SendChar(Int16U Value)
{
	// Wait for free place in queue
	while(ScibRegs.SCIFFTX.bit.TXFFST == SCI_FIFO_LEN) { }
    // Write byte
    ScibRegs.SCITXBUF = Value;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_SendInt16, "ramfuncs");
#endif
void ZwSCIa_SendInt16(Int16U Value)
{
	// Send as 2 chars (BIG-ENDIAN)
    ZwSCIa_SendChar(Value >> 8);
    ZwSCIa_SendChar(Value & 0x00FF);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_SendInt16, "ramfuncs");
#endif
void ZwSCIb_SendInt16(Int16U Value)
{
	// Send as 2 chars (BIG-ENDIAN)
    ZwSCIb_SendChar(Value >> 8);
    ZwSCIb_SendChar(Value & 0x00FF);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_SendArray16, "ramfuncs");
#endif
void ZwSCIa_SendArray16(pInt16U Buffer, Int16U BufferSize)
{
    register Int16U i;

	// Fill FIFO queue
    for(i = 0; i < BufferSize; ++i)
	    ZwSCIa_SendInt16(*(Buffer++));
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_SendArray16, "ramfuncs");
#endif
void ZwSCIb_SendArray16(pInt16U Buffer, Int16U BufferSize)
{
    register Int16U i;

	// Fill FIFO queue
    for(i = 0; i < BufferSize; ++i)
	    ZwSCIb_SendInt16(*(Buffer++));
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_ReceiveChar, "ramfuncs");
#endif
Int16U ZwSCIa_ReceiveChar()
{
	Int16U chr;

	while(!ZwSCIx_ReceiveChar(&SciaRegs, &chr)) ;

	return chr;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_ReceiveChar, "ramfuncs");
#endif
Int16U ZwSCIb_ReceiveChar()
{
	Int16U chr;

	while(!ZwSCIx_ReceiveChar(&ScibRegs, &chr)) ;

	return chr;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_ReceiveInt16, "ramfuncs");
#endif
void ZwSCIa_ReceiveInt16(pInt16U Value)
{
	Int16U charH, charL;
		
	// Read MSB
	charH = ZwSCIa_ReceiveChar();
	charL = ZwSCIa_ReceiveChar();
		
	*Value = charL | (charH << 8);		
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_ReceiveInt16, "ramfuncs");
#endif
void ZwSCIb_ReceiveInt16(pInt16U Value)
{
	Int16U charH, charL;
		
	// Read MSB
	charH = ZwSCIb_ReceiveChar();
	charL = ZwSCIb_ReceiveChar();
		
	*Value = charL | (charH << 8);		
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_ReceiveArray16, "ramfuncs");
#endif
void ZwSCIa_ReceiveArray16(pInt16U Buffer, Int16U BufferSize)
{
	register Int16U i;
	
	// Read requested number of words
	for(i = 0; i < BufferSize; ++i)
		ZwSCIa_ReceiveInt16(Buffer++);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_ReceiveArray16, "ramfuncs");
#endif
void ZwSCIb_ReceiveArray16(pInt16U Buffer, Int16U BufferSize)
{
	register Int16U i;
	
	// Read requested number of words
	for(i = 0; i < BufferSize; ++i)
		ZwSCIb_ReceiveInt16(Buffer++);
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_SetRXState, "ramfuncs");
#endif
void ZwSCIa_SetRXState(Boolean EN)
{
	ZwSCIx_SetRXState(&SciaRegs, EN);	
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_SetRXState, "ramfuncs");
#endif
void ZwSCIb_SetRXState(Boolean EN)
{
	ZwSCIx_SetRXState(&ScibRegs, EN);	
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_SetTXState, "ramfuncs");
#endif
void ZwSCIa_SetTXState(Boolean EN)
{
	ZwSCIx_SetTXState(&SciaRegs, EN);	
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_SetTXState, "ramfuncs");
#endif
void ZwSCIb_SetTXState(Boolean EN)
{
	ZwSCIx_SetTXState(&ScibRegs, EN);	
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_A)
	#pragma CODE_SECTION(ZwSCIa_ClearInputBuffer, "ramfuncs");
#endif
void ZwSCIa_ClearInputBuffer()
{
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;	
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SCI_B)
	#pragma CODE_SECTION(ZwSCIb_ClearInputBuffer, "ramfuncs");
#endif
void ZwSCIb_ClearInputBuffer()
{
    ScibRegs.SCIFFRX.bit.RXFIFORESET = 0;
    ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;	
}
// ----------------------------------------

// Internal fucntions
//
void ZwSCIa_InitGPIO(Boolean EnableRXPU)
{
#ifdef HWUSE_SCI_A

		EALLOW;
	
		/* Enable internal pull-up for the selected pins */
		GpioCtrlRegs.GPAPUD.bit.SCI_A_RX = EnableRXPU ? 0 : 1;			// Select pull-up for (SCIRXDA)
		GpioCtrlRegs.GPAPUD.bit.SCI_A_TX = 0;							// Enable pull-up for (SCITXDA)
	
		/* Set qualification for selected pins to asynch only */
		GpioCtrlRegs.SCI_A_QSEL.bit.SCI_A_RX = 3;  						// Asynch input (SCIRXDA)
	
		/* Configure SCI-A pins using GPIO regs*/
		GpioCtrlRegs.SCI_A_MUX.bit.SCI_A_RX = SCI_A_MUX_SELECTOR;   	// Configure for SCIRXDA operation
		GpioCtrlRegs.SCI_A_MUX.bit.SCI_A_TX = SCI_A_MUX_SELECTOR;   	// Configure for SCITXDA operation
		
	    EDIS;

#endif
}
// ----------------------------------------


void ZwSCIb_InitGPIO(Boolean EnableRXPU)
{
#ifdef HWUSE_SCI_B

		EALLOW;
	
		/* Enable internal pull-up for the selected pins */
		GpioCtrlRegs.GPAPUD.bit.SCI_B_RX = EnableRXPU ? 0 : 1;			// Select pull-up for (SCIRXDB)
		GpioCtrlRegs.GPAPUD.bit.SCI_B_TX = 0;	   						// Enable pull-up for (SCITXDB)
	
		/* Set qualification for selected pins to asynch only */
		GpioCtrlRegs.SCI_B_QSEL.bit.SCI_B_RX = 3;  						// Asynch input (SCIRXDB)
	
		/* Configure SCI-B pins using GPIO regs*/
		GpioCtrlRegs.SCI_B_MUX.bit.SCI_B_RX = SCI_B_MUX_SELECTOR;   	// Configure for SCIRXDB operation
		GpioCtrlRegs.SCI_B_MUX.bit.SCI_B_TX = SCI_B_MUX_SELECTOR;   	// Configure for SCITXDB operation
		
	    EDIS;

#endif
}
	
// ----------------------------------------

void ZwSCIx_Init(volatile struct SCI_REGS *ScixRegs, Int32U BaudRate, Int16U DataBits,
						 enum Zw_SCI_ParityMode Parity, Boolean TwoStopBits)
{
	// Calculate BRR register values
	Int32U BRR = (SYS_LSP_FREQ / (8 * BaudRate)) - 1;

	// Disable state-machine
    ScixRegs->SCICTL1.bit.SWRESET = 0x00;

	ScixRegs->SCICCR.bit.SCICHAR = DataBits - 1; // X data bits
	
	// Set parity mode
	if(ZW_PAR_NONE != Parity)
	{
		ScixRegs->SCICCR.bit.PARITYENA = 1;    // enable parity

		if(ZW_PAR_EVEN == Parity)
			ScixRegs->SCICCR.bit.PARITY = 1;   // EVEN mode for parity
	}

	if(TwoStopBits)
		ScixRegs->SCICCR.bit.STOPBITS = 1;     // Two stop bits

    // Enable TX and RX
	ScixRegs->SCICTL1.bit.TXENA = 1;
	ScixRegs->SCICTL1.bit.RXENA = 1;

    // Emulation mode - free run
	ScixRegs->SCIPRI.bit.FREE = 1;
	ScixRegs->SCIPRI.bit.SOFT = 1;

   	ScixRegs->SCIHBAUD = (Int16U)(BRR >> 8);
   	ScixRegs->SCILBAUD = (Int16U)(BRR & 0x000000FF);

	// Disable FIFO by default
    ScixRegs->SCIFFTX.bit.SCIFFENA = 0;

    // re-enable SCI state machine
    ScixRegs->SCICTL1.bit.SWRESET = 0x01;
}
// ----------------------------------------

void ZwSCIx_InitFIFO(volatile struct SCI_REGS *ScixRegs, Int16U TXLevel, Int16U RXLevel)
{
    // Initialize TX FIFO
    ScixRegs->SCIFFTX.bit.TXFFINTCLR = 1;
    ScixRegs->SCIFFTX.bit.SCIFFENA = 1;
	ScixRegs->SCIFFTX.bit.TXFFIENA = 1;    	
    ScixRegs->SCIFFTX.bit.TXFFIL = TXLevel;
    ScixRegs->SCIFFTX.bit.TXFIFOXRESET = 0;
    ScixRegs->SCIFFTX.bit.TXFIFOXRESET = 1;

    // Initialize RX FIFO
    ScixRegs->SCIFFRX.bit.RXFFOVRCLR = 1;
    ScixRegs->SCIFFRX.bit.RXFFINTCLR = 1;
    ScixRegs->SCIFFRX.bit.RXFFIENA = 1;    	
    ScixRegs->SCIFFRX.bit.RXFFIL = RXLevel;
    ScixRegs->SCIFFRX.bit.RXFIFORESET = 0;
    ScixRegs->SCIFFRX.bit.RXFIFORESET = 1;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && (defined(RAM_CACHE_SCI_A) || defined(RAM_CACHE_SCI_B))
	#pragma CODE_SECTION(ZwSCIx_ReceiveChar, "ramfuncs");
#endif
Boolean ZwSCIx_ReceiveChar(volatile struct SCI_REGS *ScixRegs, pInt16U Char)
{
	if(ScixRegs->SCIRXST.bit.RXERROR)
	{
		// Disable state-machine
	    ScixRegs->SCICTL1.bit.SWRESET = 0x00;
	    // Pause
	    DELAY_US(1);
		// Enable state-machine
	    ScixRegs->SCICTL1.bit.SWRESET = 0x01;
	    
	    return FALSE;
	}
	
	if(ScixRegs->SCIFFRX.bit.RXFFST)
	{
		// Read char
		*Char = ScixRegs->SCIRXBUF.all;

		return TRUE;
	}
	else
		return FALSE;
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && (defined(RAM_CACHE_SCI_A) || defined(RAM_CACHE_SCI_B))
	#pragma CODE_SECTION(ZwSCIx_SetRXState, "ramfuncs");
#endif
void ZwSCIx_SetRXState(volatile struct SCI_REGS *ScixRegs, Boolean EN)
{
	if(EN)
	{
        ScixRegs->SCICTL1.bit.RXENA = 1; // Enable subsystem
	}
	else
	{
	   ScixRegs->SCIFFRX.bit.RXFFOVRCLR = 1;   // Clear Overflow flag
	   ScixRegs->SCIFFRX.bit.RXFFINTCLR = 1;   // Clear Interrupt flag
       ScixRegs->SCICTL1.bit.RXENA = 0; 		  // Disable RX subsystem
	}	
}
// ----------------------------------------

#if defined(BOOT_FROM_FLASH) && (defined(RAM_CACHE_SCI_A) || defined(RAM_CACHE_SCI_B))
	#pragma CODE_SECTION(ZwSCIx_SetTXState, "ramfuncs");
#endif
void ZwSCIx_SetTXState(volatile struct SCI_REGS *ScixRegs, Boolean EN)
{
	if(EN)
	{
        ScixRegs->SCICTL1.bit.TXENA = 1;
	}
	else
	{
		ScixRegs->SCIFFTX.bit.TXFFINTCLR = 1;	// Clear SCI Interrupt flag
        ScixRegs->SCICTL1.bit.TXENA = 0; 		// Disable TX subsystem
	}	
}
// ----------------------------------------

// Memory IO Area
//
//----------------------------------------
#pragma DATA_SECTION(SciaRegs,"SciaRegsFile");
volatile struct SCI_REGS SciaRegs;

//----------------------------------------
#pragma DATA_SECTION(ScibRegs,"ScibRegsFile");
volatile struct SCI_REGS ScibRegs;

// No more.
