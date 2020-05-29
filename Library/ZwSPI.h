// -----------------------------------------
// API for SPI module
// ----------------------------------------

#ifndef __ZW_SPI_H
#define __ZW_SPI_H

// Include
#include "stdinc.h"
//
// IO defs
#include "_ZwSPI_Defs.h"
//
#include "ZwGPIO.h"
#include "ZwUtils.h"

// Types
//
#define	ZW_SPI_INIT_RX 				BIT0
#define	ZW_SPI_INIT_TX  			BIT1
#define ZW_SPI_INIT_CS  			BIT2
//
typedef enum __SPITransmissionType
{
	STTNormal = 0,
	STTStream = 1,
	STTDelayed = 2
} SPITransmissionType;

// Internal variables
//
extern Int16U DummyBuffer[];


// Functions
//
// Init SPIa module into specified state
void ZwSPIa_Init(Boolean Master, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase, Int16U Mode, Boolean SkipGPIO, Boolean EnablePU);
void ZwSPIb_Init(Boolean Master, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase, Int16U Mode, Boolean SkipGPIO, Boolean EnablePU);
void ZwSPIc_Init(Boolean Master, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase, Int16U Mode, Boolean SkipGPIO, Boolean EnablePU);
void ZwSPId_Init(Boolean Master, Int32U BaudRate, Int16U CharLength, Boolean Polarity, Boolean Phase, Int16U Mode, Boolean SkipGPIO, Boolean EnablePU);
// Enable FIFO feature
void ZwSPIa_InitFIFO(Int16U RXLevel, Int16U TXLevel);
void ZwSPIb_InitFIFO(Int16U RXLevel, Int16U TXLevel);
void ZwSPIc_InitFIFO(Int16U RXLevel, Int16U TXLevel);
void ZwSPId_InitFIFO(Int16U RXLevel, Int16U TXLevel);
// Config interrupts
void ZwSPIa_ConfigInterrupts(Boolean Rx, Boolean Tx);
void ZwSPIb_ConfigInterrupts(Boolean Rx, Boolean Tx);
void ZwSPIc_ConfigInterrupts(Boolean Rx, Boolean Tx);
void ZwSPId_ConfigInterrupts(Boolean Rx, Boolean Tx);
// Enable peripheral interrupts
void ZwSPIa_EnableInterrupts(Boolean Rx, Boolean Tx);
void ZwSPIb_EnableInterrupts(Boolean Rx, Boolean Tx);
void ZwSPIc_EnableInterrupts(Boolean Rx, Boolean Tx);
void ZwSPId_EnableInterrupts(Boolean Rx, Boolean Tx);
// Enable interrupts on CPU-level
void ZwSPI_EnableInterruptsGlobal(Boolean Enable);
//
// Internal functions
void ZwSPIx_Send(volatile struct SPI_REGS *SpixRegs, pInt16U Buffer, Int16U BufferSize, Int16U CharSize, SPITransmissionType TransmissionType);
void ZwSPIx_EndReceive(volatile struct SPI_REGS *SpixRegs, pInt16U Buffer, Int16U BufferSize);
Boolean ZwSPIx_ProcessRXInterrupt(volatile struct SPI_REGS *SpixRegs);
void ZwSPIx_ProcessTXInterrupt(volatile struct SPI_REGS *SpixRegs);
void ZwSPIx_AlterBaudrate(volatile struct SPI_REGS *SpixRegs, Int32U BaudRate);

// Inline functions
//
// Transmit data
void inline ZwSPIa_Send(pInt16U Buffer, Int16U BufferSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
#ifdef HWUSE_SPI_A

	// Wait for end of current transmitting
	if(TransmissionType != STTStream)
		while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG==1); //NOT TESTED!!!

	ZwSPIx_Send(&SpiaRegs, Buffer, BufferSize, CharSize, TransmissionType);

#endif
}
//
void inline ZwSPIb_Send(pInt16U Buffer, Int16U BufferSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
#ifdef HWUSE_SPI_B

	// Wait for end of current transmitting
	if(TransmissionType != STTStream)
		while(SpibRegs.SPISTS.bit.BUFFULL_FLAG==1);

	ZwSPIx_Send(&SpibRegs, Buffer, BufferSize, CharSize, TransmissionType);

#endif
}
//
void inline ZwSPIc_Send(pInt16U Buffer, Int16U BufferSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
#ifdef HWUSE_SPI_C

	// Wait for end of current transmitting
	if(TransmissionType != STTStream)
		while(!GpioDataRegs.GPADAT.bit.SPI_C_CS);

	ZwSPIx_Send(&SpicRegs, Buffer, BufferSize, CharSize, TransmissionType);

#endif
}
//
void inline ZwSPId_Send(pInt16U Buffer, Int16U BufferSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
#ifdef HWUSE_SPI_D

	// Wait for end of current transmitting
	if(TransmissionType != STTStream)
		while(!GpioDataRegs.GPADAT.bit.SPI_D_CS);

	ZwSPIx_Send(&SpidRegs, Buffer, BufferSize, CharSize, TransmissionType);

#endif
}
//
// Begin receive data
void inline ZwSPIa_BeginReceive(pInt16U OptionalData, Int16U DataSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
#ifdef HWUSE_SPI_A

	if(TransmissionType != STTStream)
		while(!GpioDataRegs.GPADAT.bit.SPI_A_CS);

	ZwSPIx_Send(&SpiaRegs, OptionalData == NULL ? DummyBuffer : OptionalData, DataSize, CharSize, TransmissionType);

#endif
}
//
void inline ZwSPIb_BeginReceive(pInt16U OptionalData, Int16U DataSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
#ifdef HWUSE_SPI_B

	if(TransmissionType != STTStream)
		while(!GpioDataRegs.GPADAT.bit.SPI_B_CS);

	ZwSPIx_Send(&SpibRegs, OptionalData == NULL ? DummyBuffer : OptionalData, DataSize, CharSize, TransmissionType);

#endif
}
//
void inline ZwSPIc_BeginReceive(pInt16U OptionalData, Int16U DataSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
#ifdef HWUSE_SPI_C

	if(TransmissionType != STTStream)
		while(!GpioDataRegs.GPADAT.bit.SPI_C_CS);

	ZwSPIx_Send(&SpicRegs, OptionalData == NULL ? DummyBuffer : OptionalData, DataSize, CharSize, TransmissionType);

#endif
}
//
void inline ZwSPId_BeginReceive(pInt16U OptionalData, Int16U DataSize, Int16U CharSize, SPITransmissionType TransmissionType)
{
#ifdef HWUSE_SPI_D

	if(TransmissionType != STTStream)
		while(!GpioDataRegs.GPADAT.bit.SPI_D_CS);

	ZwSPIx_Send(&SpidRegs, OptionalData == NULL ? DummyBuffer : OptionalData, DataSize, CharSize, TransmissionType);

#endif
}
//
// End receive data
void inline ZwSPIa_EndReceive(pInt16U Buffer, Int16U BufferSize)
{
	ZwSPIx_EndReceive(&SpiaRegs, Buffer, BufferSize);
}
//
void inline ZwSPIb_EndReceive(pInt16U Buffer, Int16U BufferSize)
{
	ZwSPIx_EndReceive(&SpibRegs, Buffer, BufferSize);
}
//
void inline ZwSPIc_EndReceive(pInt16U Buffer, Int16U BufferSize)
{
	ZwSPIx_EndReceive(&SpicRegs, Buffer, BufferSize);
}
//
void inline ZwSPId_EndReceive(pInt16U Buffer, Int16U BufferSize)
{
	ZwSPIx_EndReceive(&SpidRegs, Buffer, BufferSize);
}
//
Int16U inline ZwSPIa_GetWordsToReceive()
{
	return SpiaRegs.SPIFFRX.bit.RXFFST;
}
//
Int16U inline ZwSPIb_GetWordsToReceive()
{
	return SpibRegs.SPIFFRX.bit.RXFFST;
}
//
Int16U inline ZwSPIc_GetWordsToReceive()
{
	return SpicRegs.SPIFFRX.bit.RXFFST;
}
//
Int16U inline ZwSPId_GetWordsToReceive()
{
	return SpidRegs.SPIFFRX.bit.RXFFST;
}
//
Int16U inline ZwSPIa_GetWordsToSend()
{
	return SpiaRegs.SPIFFTX.bit.TXFFST;
}
//
Int16U inline ZwSPIb_GetWordsToSend()
{
	return SpibRegs.SPIFFTX.bit.TXFFST;
}
//
Int16U inline ZwSPIc_GetWordsToSend()
{
	return SpicRegs.SPIFFTX.bit.TXFFST;
}
//
Int16U inline ZwSPId_GetWordsToSend()
{
	return SpidRegs.SPIFFTX.bit.TXFFST;
}
//
// Handle SPI RX interrupt
Boolean inline ZwSPIa_ProcessRXInterrupt()
{
	return ZwSPIx_ProcessRXInterrupt(&SpiaRegs);
}
//
// Handle SPI RX interrupt
Boolean inline ZwSPIb_ProcessRXInterrupt()
{
	return ZwSPIx_ProcessRXInterrupt(&SpibRegs);
}
//
// Handle SPI RX interrupt
Boolean inline ZwSPIc_ProcessRXInterrupt()
{
	return ZwSPIx_ProcessRXInterrupt(&SpicRegs);
}
//
// Handle SPI RX interrupt
Boolean inline ZwSPId_ProcessRXInterrupt()
{
	return ZwSPIx_ProcessRXInterrupt(&SpidRegs);
}
//
// Handle SPI TX interrupt
void inline ZwSPIa_ProcessTXInterrupt()
{
	ZwSPIx_ProcessTXInterrupt(&SpiaRegs);
}
//
void inline ZwSPIb_ProcessTXInterrupt()
{
	ZwSPIx_ProcessTXInterrupt(&SpibRegs);
}
//
void inline ZwSPIc_ProcessTXInterrupt()
{
	ZwSPIx_ProcessTXInterrupt(&SpicRegs);
}
//
void inline ZwSPId_ProcessTXInterrupt()
{
	ZwSPIx_ProcessTXInterrupt(&SpidRegs);
}
//
// Fine tuning
void inline ZwSPIa_AlterCharLength(Int16U NewLength)
{
	SpiaRegs.SPICCR.bit.SPICHAR	= NewLength - 1;
}
//
void inline ZwSPIb_AlterCharLength(Int16U NewLength)
{
	SpibRegs.SPICCR.bit.SPICHAR	= NewLength - 1;
}
//
void inline ZwSPIc_AlterCharLength(Int16U NewLength)
{
	SpicRegs.SPICCR.bit.SPICHAR	= NewLength - 1;
}
//
void inline ZwSPId_AlterCharLength(Int16U NewLength)
{
	SpidRegs.SPICCR.bit.SPICHAR	= NewLength - 1;
}
//
void inline ZwSPIa_AlterBaudrate(Int32U Baudrate)
{
	ZwSPIx_AlterBaudrate(&SpiaRegs, Baudrate);
}
//
void inline ZwSPIb_AlterBaudrate(Int32U Baudrate)
{
	ZwSPIx_AlterBaudrate(&SpibRegs, Baudrate);
}
//
void inline ZwSPIc_AlterBaudrate(Int32U Baudrate)
{
	ZwSPIx_AlterBaudrate(&SpicRegs, Baudrate);
}
//
void inline ZwSPId_AlterBaudrate(Int32U Baudrate)
{
	ZwSPIx_AlterBaudrate(&SpidRegs, Baudrate);
}
//
void inline ZwSPIa_ResetTXFIFO()
{
	SpiaRegs.SPIFFTX.bit.TXFIFO = 0x00;
	DELAY_US(1);
	SpiaRegs.SPIFFTX.bit.TXFIFO = 0x01;
}
//
void inline ZwSPIb_ResetTXFIFO()
{
	SpibRegs.SPIFFTX.bit.TXFIFO = 0x00;
	DELAY_US(1);
	SpibRegs.SPIFFTX.bit.TXFIFO = 0x01;
}
//
void inline ZwSPIc_ResetTXFIFO()
{
	SpicRegs.SPIFFTX.bit.TXFIFO = 0x00;
	DELAY_US(1);
	SpicRegs.SPIFFTX.bit.TXFIFO = 0x01;
}
//
void inline ZwSPId_ResetTXFIFO()
{
	SpidRegs.SPIFFTX.bit.TXFIFO = 0x00;
	DELAY_US(1);
	SpidRegs.SPIFFTX.bit.TXFIFO = 0x01;
}
//
void inline ZwSPIa_ResetRXFIFO()
{
	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0x00;
	DELAY_US(1);
	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0x01;
}
//
void inline ZwSPIb_ResetRXFIFO()
{
	SpibRegs.SPIFFRX.bit.RXFIFORESET = 0x00;
	DELAY_US(1);
	SpibRegs.SPIFFRX.bit.RXFIFORESET = 0x01;
}
//
void inline ZwSPIc_ResetRXFIFO()
{
	SpicRegs.SPIFFRX.bit.RXFIFORESET = 0x00;
	DELAY_US(1);
	SpicRegs.SPIFFRX.bit.RXFIFORESET = 0x01;
}
//
void inline ZwSPId_ResetRXFIFO()
{
	SpidRegs.SPIFFRX.bit.RXFIFORESET = 0x00;
	DELAY_US(1);
	SpidRegs.SPIFFRX.bit.RXFIFORESET = 0x01;
}


// ISR postfixes
#define SPI_ISR_DONE	PieCtrlRegs.PIEACK.all = PIEACK_GROUP6


#endif // end __ZW_SPI_H
