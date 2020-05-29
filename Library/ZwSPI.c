// -----------------------------------------
// API for SPI module
// ----------------------------------------

// Header
#include "ZwSPI.h"

// Include
#include "ZwGPIO.h"
#include "ZwPIE.h"
#include "ZwUtils.h"

// Constants
//
#define ZW_SPI_FIFO_DEPTH			16

// Variables
//
Int16U DummyBuffer[ZW_SPI_FIFO_DEPTH] = { 0 };

// Forward function
void ZwSPIa_InitGPIO(Int16U Mode, Boolean EnablePU, Boolean Master);
void ZwSPIb_InitGPIO(Int16U Mode, Boolean EnablePU, Boolean Master);
void ZwSPIc_InitGPIO(Int16U Mode, Boolean EnablePU, Boolean Master);
void ZwSPId_InitGPIO(Int16U Mode, Boolean EnablePU, Boolean Master);

void ZwSPIx_Init(Boolean Master, volatile struct SPI_REGS *SpixRegs, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase);
void ZwSPIx_InitFIFO(volatile struct SPI_REGS *SpixRegs, Int16U RXLevel, Int16U TXLevel);

// Functions
//
// Init SPIa module into specified state
void ZwSPIa_Init(Boolean Master, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase, Int16U Mode, Boolean SkipGPIO, Boolean EnablePU)
{
	ZwSPIx_Init(Master, &SpiaRegs, BaudRate, CharLength, Polarity, Phase);
	if(!SkipGPIO)
		ZwSPIa_InitGPIO(Mode, EnablePU, Master);
}
// ----------------------------------------

// Init SPIb module into specified state
void ZwSPIb_Init(Boolean Master, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase, Int16U Mode, Boolean SkipGPIO, Boolean EnablePU)
{
	ZwSPIx_Init(Master, &SpibRegs, BaudRate, CharLength, Polarity, Phase);
	if(!SkipGPIO)
		ZwSPIb_InitGPIO(Mode, EnablePU, Master);
}
// ----------------------------------------

// Init SPIc module into specified state
void ZwSPIc_Init(Boolean Master, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase, Int16U Mode, Boolean SkipGPIO, Boolean EnablePU)
{
	ZwSPIx_Init(Master, &SpicRegs, BaudRate, CharLength, Polarity, Phase);
	if(!SkipGPIO)
		ZwSPIc_InitGPIO(Mode, EnablePU, Master);
}
// ----------------------------------------

// Init SPId module into specified state
void ZwSPId_Init(Boolean Master, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase, Int16U Mode, Boolean SkipGPIO, Boolean EnablePU)
{
	ZwSPIx_Init(Master, &SpidRegs, BaudRate, CharLength, Polarity, Phase);
	if(!SkipGPIO)
		ZwSPId_InitGPIO(Mode, EnablePU, Master);
}
// ----------------------------------------

// Init SPIa FIFO
void ZwSPIa_InitFIFO(Int16U RXLevel, Int16U TXLevel)
{
	ZwSPIx_InitFIFO(&SpiaRegs, RXLevel, TXLevel);
}
// ----------------------------------------

// Init SPIb FIFO
void ZwSPIb_InitFIFO(Int16U RXLevel, Int16U TXLevel)
{
	ZwSPIx_InitFIFO(&SpibRegs, RXLevel, TXLevel);
}
// ----------------------------------------

// Init SPIc FIFO
void ZwSPIc_InitFIFO(Int16U RXLevel, Int16U TXLevel)
{
	ZwSPIx_InitFIFO(&SpicRegs, RXLevel, TXLevel);
}
// ----------------------------------------

// Init SPId FIFO
void ZwSPId_InitFIFO(Int16U RXLevel, Int16U TXLevel)
{
	ZwSPIx_InitFIFO(&SpidRegs, RXLevel, TXLevel);
}
// ----------------------------------------

// Enable interrupts on A
void ZwSPIa_ConfigInterrupts(Boolean Rx, Boolean Tx)
{
	// Enable SPI interrupts
	SpiaRegs.SPICTL.bit.SPIINTENA = (Rx || Tx) ? 0x01 : 0x00;
	// Enable RX interrupt
	SpiaRegs.SPIFFRX.bit.RXFFIENA = Rx ? 0x01 : 0x00;
	// Enable TX interrupt
	SpiaRegs.SPIFFTX.bit.TXFFIENA = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable interrupts on B
void ZwSPIb_ConfigInterrupts(Boolean Rx, Boolean Tx)
{
	// Enable SPI interrupts
	SpibRegs.SPICTL.bit.SPIINTENA = (Rx || Tx) ? 0x01 : 0x00;
	// Enable RX interrupt
	SpibRegs.SPIFFRX.bit.RXFFIENA = Rx ? 0x01 : 0x00;
	// Enable TX interrupt
	SpibRegs.SPIFFTX.bit.TXFFIENA = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable interrupts on C
void ZwSPIc_ConfigInterrupts(Boolean Rx, Boolean Tx)
{
	// Enable SPI interrupts
	SpicRegs.SPICTL.bit.SPIINTENA = (Rx || Tx) ? 0x01 : 0x00;
	// Enable RX interrupt
	SpicRegs.SPIFFRX.bit.RXFFIENA = Rx ? 0x01 : 0x00;
	// Enable TX interrupt
	SpicRegs.SPIFFTX.bit.TXFFIENA = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable interrupts on D
void ZwSPId_ConfigInterrupts(Boolean Rx, Boolean Tx)
{
	// Enable SPI interrupts
	SpidRegs.SPICTL.bit.SPIINTENA = (Rx || Tx) ? 0x01 : 0x00;
	// Enable RX interrupt
	SpidRegs.SPIFFRX.bit.RXFFIENA = Rx ? 0x01 : 0x00;
	// Enable TX interrupt
	SpidRegs.SPIFFTX.bit.TXFFIENA = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable interrupts on A
void ZwSPIa_EnableInterrupts(Boolean Rx, Boolean Tx)
{
    // Enable SPIA RXINT interrupt
    PieCtrlRegs.PIEIER6.bit.INTx1 = Rx ? 0x01 : 0x00;
    // Enable SPIA TXINT interrupt
    PieCtrlRegs.PIEIER6.bit.INTx2 = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable interrupts on B
void ZwSPIb_EnableInterrupts(Boolean Rx, Boolean Tx)
{
    // Enable SPIA RXINT interrupt
    PieCtrlRegs.PIEIER6.bit.INTx3 = Rx ? 0x01 : 0x00;
    // Enable SPIA TXINT interrupt
    PieCtrlRegs.PIEIER6.bit.INTx4 = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable interrupts on C
void ZwSPIc_EnableInterrupts(Boolean Rx, Boolean Tx)
{
    // Enable SPIA RXINT interrupt
    PieCtrlRegs.PIEIER6.bit.INTx5 = Rx ? 0x01 : 0x00;
    // Enable SPIA TXINT interrupt
    PieCtrlRegs.PIEIER6.bit.INTx6 = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable interrupts on D
void ZwSPId_EnableInterrupts(Boolean Rx, Boolean Tx)
{
    // Enable SPIA RXINT interrupt
    PieCtrlRegs.PIEIER6.bit.INTx7 = Rx ? 0x01 : 0x00;
    // Enable SPIA TXINT interrupt
    PieCtrlRegs.PIEIER6.bit.INTx8 = Tx ? 0x01 : 0x00;
}
// ----------------------------------------

// Enable interrupts on ABCD
void ZwSPI_EnableInterruptsGlobal(Boolean Enable)
{
    // Enable CPU INT6 which is connected to SPI:
	if(Enable)
		IER |= M_INT6;
	else
		IER &= ~M_INT6;
}
// ----------------------------------------

// Transmit data
#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SPI_ABCD)
	#pragma CODE_SECTION(ZwSPIx_Send, "ramfuncs");
#endif
void ZwSPIx_Send(volatile struct SPI_REGS *SpixRegs, pInt16U Buffer, Int16U BufferSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
	Int16U sent;

	// Remove possible dummy bytes from previous operation
	if(TransmissionType != STTStream)
	{
		SpixRegs->SPIFFRX.bit.RXFIFORESET = 0;
		SpixRegs->SPIFFRX.bit.RXFIFORESET = 1;
	}

	// Write buffer content to FIFO
	for(sent = 0; sent < BufferSize; ++sent)
		SpixRegs->SPITXBUF = (*(Buffer++) << (16 - CharSize));
} 
// ----------------------------------------

// Receive data
#if defined(BOOT_FROM_FLASH) && defined(RAM_CACHE_SPI_ABCD)
	#pragma CODE_SECTION(ZwSPIx_EndReceive, "ramfuncs");
#endif
void ZwSPIx_EndReceive(volatile struct SPI_REGS *SpixRegs, pInt16U Buffer, Int16U BufferSize)
{
	Int16U received;

	// wait for all data
	for(received = 0; received < BufferSize; ++received)
		*(Buffer++) = SpixRegs->SPIRXBUF;
}
// ----------------------------------------

// Handle SPI-A RX interrupt
#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwSPIx_ProcessRXInterrupt, "ramfuncs");
#endif
Boolean ZwSPIx_ProcessRXInterrupt(volatile struct SPI_REGS *SpixRegs)
{
	Int16U ovf = SpixRegs->SPIFFRX.bit.RXFFOVF;

	// Clear int and overflow flags
	if(ovf)
		SpixRegs->SPIFFRX.bit.RXFFOVFCLR = 1;

	SpixRegs->SPIFFRX.bit.RXFFINTCLR = 1;

	return !ovf;
}
// ----------------------------------------

// Handle SPI-A TX interrupt
#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZwSPIx_ProcessTXInterrupt, "ramfuncs");
#endif
void ZwSPIx_ProcessTXInterrupt(volatile struct SPI_REGS *SpixRegs)
{
	// Clear int flag
	SpixRegs->SPIFFTX.bit.TXFFINTCLR = 1;
}
// ----------------------------------------

// Enable FIFO feature
void ZwSPIx_InitFIFO(volatile struct SPI_REGS *SpixRegs, Int16U RxBufferDepth, Int16U TxBufferDepth)
{
	// Reset FIFO
	SpixRegs->SPIFFTX.bit.SPIRST = 0x00;
	
	// Enable FIFO
	SpixRegs->SPIFFTX.bit.SPIFFENA = 0x01;
	// Reset FIFO TX pointer
	SpixRegs->SPIFFTX.bit.TXFIFO = 0x00;
	// Set interrupt level
	SpixRegs->SPIFFTX.bit.TXFFIL = TxBufferDepth;
	
	// Reset FIFO RX pointer
	SpixRegs->SPIFFRX.bit.RXFIFORESET = 0x00;
	// Set interrupt level
	SpixRegs->SPIFFRX.bit.RXFFIL = RxBufferDepth;
	
	// Re-enable FIFO
	SpixRegs->SPIFFRX.bit.RXFIFORESET = 0x01;
	SpixRegs->SPIFFTX.bit.TXFIFO = 0x01;
	SpixRegs->SPIFFTX.bit.SPIRST = 0x01;
}
// ----------------------------------------

void ZwSPIa_InitGPIO(Int16U Mode, Boolean EnablePU, Boolean Master)
{
#ifdef HWUSE_SPI_A

		EALLOW;
	
	    if(Mode & ZW_SPI_INIT_TX)
		    GpioCtrlRegs.GPAPUD.bit.SPI_A_SIMO = EnablePU ? 0 : 1;  	// Enable pull-up on (SPISIMOA)
	    if(Mode & ZW_SPI_INIT_RX)
		    GpioCtrlRegs.GPAPUD.bit.SPI_A_SOMI = EnablePU ? 0 : 1;   	// Select pull-up on (SPISOMIA)
	    GpioCtrlRegs.GPAPUD.bit.SPI_A_CLK = EnablePU ? 0 : 1;   		// Enable pull-up on (SPICLKA)
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.GPAPUD.bit.SPI_A_CS = EnablePU ? 0 : 1;   		// Enable pull-up on (SPISTEA)
	
	    if(Mode & ZW_SPI_INIT_TX)
		    GpioCtrlRegs.SPI_A_QSEL.bit.SPI_A_SIMO = 3; 	// Asynch input (SPISIMOA)
	    if(Mode & ZW_SPI_INIT_RX)
		    GpioCtrlRegs.SPI_A_QSEL.bit.SPI_A_SOMI = 3; 	// Asynch input (SPISOMIA)
	    GpioCtrlRegs.SPI_A_QSEL.bit.SPI_A_CLK = 3; 			// Asynch input (SPICLKA)
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.SPI_A_QSEL.bit.SPI_A_CS = 3;   	// Asynch input (SPISTEA)

		if(((Mode & ZW_SPI_INIT_TX) && Master) ||\
		   ((Mode & ZW_SPI_INIT_RX) && (!Master)))
			GpioCtrlRegs.SPI_A_MUX.bit.SPI_A_SIMO = 1; 		// Configure as SPISIMOA
		if(((Mode & ZW_SPI_INIT_RX) && Master) ||\
		   ((Mode & ZW_SPI_INIT_TX) && (!Master)))
			GpioCtrlRegs.SPI_A_MUX.bit.SPI_A_SOMI = 1; 		// Configure as SPISOMIA

	    GpioCtrlRegs.SPI_A_MUX.bit.SPI_A_CLK = 1; 			// Configure as SPICLKA
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.SPI_A_MUX.bit.SPI_A_CS = 1;    	// Configure as SPISTEA

	    EDIS;	
	
#endif
}
// ----------------------------------------

void ZwSPIb_InitGPIO(Int16U Mode, Boolean EnablePU, Boolean Master)
{
#ifdef HWUSE_SPI_B

		EALLOW;
	
	    if(Mode & ZW_SPI_INIT_TX)
		    GpioCtrlRegs.GPAPUD.bit.SPI_B_SIMO = EnablePU ? 0 : 1;  	// Enable pull-up on (SPISIMOB)
	    if(Mode & ZW_SPI_INIT_RX)
		    GpioCtrlRegs.GPAPUD.bit.SPI_B_SOMI = EnablePU ? 0 : 1;   	// Select pull-up on (SPISOMIB)
	    GpioCtrlRegs.GPAPUD.bit.SPI_B_CLK = EnablePU ? 0 : 1;   		// Enable pull-up on (SPICLKB)
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.GPAPUD.bit.SPI_B_CS = EnablePU ? 0 : 1;   		// Enable pull-up on (SPISTEB)

	    if(Mode & ZW_SPI_INIT_TX)
		    GpioCtrlRegs.SPI_B_QSEL.bit.SPI_B_SIMO = 3; 	// Asynch input (SPISIMOB)
	    if(Mode & ZW_SPI_INIT_RX)
		    GpioCtrlRegs.SPI_B_QSEL.bit.SPI_B_SOMI = 3; 	// Asynch input (SPISOMIB)
	    GpioCtrlRegs.SPI_B_QSEL.bit.SPI_B_CLK = 3; 			// Asynch input (SPICLKB)
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.SPI_B_QSEL.bit.SPI_B_CS = 3;   	// Asynch input (SPISTEB)
	    
		if(((Mode & ZW_SPI_INIT_TX) && Master) ||\
		   ((Mode & ZW_SPI_INIT_RX) && (!Master)))
			GpioCtrlRegs.SPI_B_MUX.bit.SPI_B_SIMO = 3; 		// Configure as SPISIMOB
		if(((Mode & ZW_SPI_INIT_RX) && Master) ||\
		   ((Mode & ZW_SPI_INIT_TX) && (!Master)))
			GpioCtrlRegs.SPI_B_MUX.bit.SPI_B_SOMI = 3; 		// Configure as SPISOMIB

	    GpioCtrlRegs.SPI_B_MUX.bit.SPI_B_CLK = 3; 			// Configure as SPICLKB
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.SPI_B_MUX.bit.SPI_B_CS = 3;    	// Configure as SPISTEB
	
	    EDIS;	
	
#endif
}
// ----------------------------------------

void ZwSPIc_InitGPIO(Int16U Mode, Boolean EnablePU, Boolean Master)
{
#ifdef HWUSE_SPI_C

		EALLOW;
	
	    if(Mode & ZW_SPI_INIT_TX)
		    GpioCtrlRegs.GPAPUD.bit.SPI_C_SIMO = EnablePU ? 0 : 1;   	// Enable pull-up on (SPISIMOC)
	    if(Mode & ZW_SPI_INIT_RX)
		    GpioCtrlRegs.GPAPUD.bit.SPI_C_SOMI = EnablePU ? 0 : 1;   	// Select pull-up on (SPISOMIC)
	    GpioCtrlRegs.GPAPUD.bit.SPI_C_CLK = EnablePU ? 0 : 1;   		// Enable pull-up on (SPICLKC)
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.GPAPUD.bit.SPI_C_CS = EnablePU ? 0 : 1;     	// Enable pull-up on (SPISTEC)
	
	
	    if(Mode & ZW_SPI_INIT_TX)
		    GpioCtrlRegs.SPI_C_QSEL.bit.SPI_C_SIMO = 3; 	// Asynch input (SPISIMOC)
	    if(Mode & ZW_SPI_INIT_RX)
		    GpioCtrlRegs.SPI_C_QSEL.bit.SPI_C_SOMI = 3; 	// Asynch input (SPISOMIC)
	    GpioCtrlRegs.SPI_C_MUX.bit.SPI_C_CLK = 3; 			// Asynch input (SPICLKC)
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.SPI_C_QSEL.bit.SPI_C_CS = 3;   	// Asynch input (SPISTEC)
	    
		if(((Mode & ZW_SPI_INIT_TX) && Master) ||\
		   ((Mode & ZW_SPI_INIT_RX) && (!Master)))
			GpioCtrlRegs.SPI_C_MUX.bit.SPI_C_SIMO = 2; 		// Configure as SPISIMOC
		if(((Mode & ZW_SPI_INIT_RX) && Master) ||\
		   ((Mode & ZW_SPI_INIT_TX) && (!Master)))
			GpioCtrlRegs.SPI_C_MUX.bit.SPI_C_SOMI = 2; 		// Configure as SPISOMIC

	    GpioCtrlRegs.SPI_C_MUX.bit.SPI_C_CLK = 2; 			// Configure as SPICLKC
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.SPI_C_MUX.bit.SPI_C_CS = 2;    	// Configure as SPISTEC
	
	    EDIS;	

#endif
}
// ----------------------------------------


void ZwSPId_InitGPIO(Int16U Mode, Boolean EnablePU, Boolean Master)
{
#ifdef HWUSE_SPI_D

		EALLOW;
	
	    if(Mode & ZW_SPI_INIT_TX)
		    GpioCtrlRegs.GPAPUD.bit.SPI_D_SIMO = EnablePU ? 0 : 1;   	// Enable pull-up on (SPISIMOD)
	    if(Mode & ZW_SPI_INIT_RX)
		    GpioCtrlRegs.GPAPUD.bit.SPI_D_SOMI = EnablePU ? 0 : 1;   	// Select pull-up on (SPISOMID)
	    GpioCtrlRegs.GPAPUD.bit.SPI_D_CLK = EnablePU ? 0 : 1;   		// Enable pull-up on (SPICLKD)
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.GPAPUD.bit.SPI_D_CS = EnablePU ? 0 : 1;    	// Enable pull-up on (SPISTED)
	
	
	    if(Mode & ZW_SPI_INIT_TX)
		    GpioCtrlRegs.SPI_D_QSEL.bit.SPI_D_SIMO = 3; 	// Asynch input (SPISIMOD)
	    if(Mode & ZW_SPI_INIT_RX)
		    GpioCtrlRegs.SPI_D_QSEL.bit.SPI_D_SOMI = 3; 	// Asynch input (SPISOMID)
	    GpioCtrlRegs.SPI_D_QSEL.bit.SPI_D_CLK = 3; 			// Asynch input (SPICLKD)
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.SPI_D_QSEL.bit.SPI_D_CS = 3;   	// Asynch input (SPISTED)
	    
		if(((Mode & ZW_SPI_INIT_TX) && Master) ||\
		   ((Mode & ZW_SPI_INIT_RX) && (!Master)))
			GpioCtrlRegs.SPI_D_MUX.bit.SPI_D_SIMO = 2; 		// Configure as SPISIMOD
		if(((Mode & ZW_SPI_INIT_RX) && Master) ||\
		   ((Mode & ZW_SPI_INIT_TX) && (!Master)))
			GpioCtrlRegs.SPI_D_MUX.bit.SPI_D_SOMI = 2; 		// Configure as SPISOMID

	    GpioCtrlRegs.SPI_D_MUX.bit.SPI_D_CLK = 2; 			// Configure as SPICLKD
	    if(Mode & ZW_SPI_INIT_CS)
		    GpioCtrlRegs.SPI_D_MUX.bit.SPI_D_CS = 2;    	// Configure as SPISTED
	
	    EDIS;	

#endif
}
// ----------------------------------------

void ZwSPIx_Init(Boolean Master, volatile struct SPI_REGS *SpixRegs, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase)
{
	// Reset module
	SpixRegs->SPICCR.bit.SPISWRESET = 0x00;

	if(Master)
		// Set SPI baudrate: BR = LSPCLK/(BRR+1)
		SpixRegs->SPIBRR = (SYS_LSP_FREQ / BaudRate) - 1;
	else
		// Set maximum divisor
		SpixRegs->SPIBRR = 0x7f;
	
	// Set character length
	SpixRegs->SPICCR.bit.SPICHAR = CharLength - 1;
	// Set polarity of data stream
	SpixRegs->SPICCR.bit.CLKPOLARITY = (Polarity ? 0x01 : 0x00);
	// Set shift phase
	SpixRegs->SPICTL.bit.CLK_PHASE = (Phase ? 0x01 : 0x00);
	// Configure as a master
	SpixRegs->SPICTL.bit.MASTER_SLAVE = Master ? 0x01 : 0x00;

	// Free soft run
	SpixRegs->SPIPRI.bit.FREE = 0x01;
	SpixRegs->SPIPRI.bit.SOFT = 0x01;
	
	// Ready to go!
	SpixRegs->SPICCR.bit.SPISWRESET = 0x01;
	SpixRegs->SPICTL.bit.TALK = 0x01;
}
//----------------------------------------

void ZwSPIx_AlterBaudrate(volatile struct SPI_REGS *SpixRegs, Int32U BaudRate)
{
	// Reset module
	SpixRegs->SPICCR.bit.SPISWRESET = 0x00;
	// Set SPI baudrate: BR = LSPCLK/(BRR+1)
	SpixRegs->SPIBRR = (SYS_LSP_FREQ / BaudRate) - 1;
	// Ready to go!
	SpixRegs->SPICCR.bit.SPISWRESET = 0x01;
}

//----------------------------------------
#pragma DATA_SECTION(SpiaRegs,"SpiaRegsFile");
volatile struct SPI_REGS SpiaRegs;

//----------------------------------------
#pragma DATA_SECTION(SpibRegs,"SpibRegsFile");
volatile struct SPI_REGS SpibRegs;

//----------------------------------------
#pragma DATA_SECTION(SpicRegs,"SpicRegsFile");
volatile struct SPI_REGS SpicRegs;

//----------------------------------------
#pragma DATA_SECTION(SpidRegs,"SpidRegsFile");
volatile struct SPI_REGS SpidRegs;

// No more.
