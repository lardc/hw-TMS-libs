// -----------------------------------------
// API for SCI module
// ----------------------------------------

#ifndef __ZW_SCI_H
#define __ZW_SCI_H

// Include
#include "stdinc.h"
//
// IO defs
#include "_ZwSCI_Defs.h"

// Data types & constants
//
enum Zw_SCI_ParityMode { ZW_PAR_NONE = 0, ZW_PAR_EVEN, ZW_PAR_ODD };


// Functions
//
// Init SCI with specified parameters
void ZwSCIa_Init(Int32U BaudRate, Int16U DataBits, enum Zw_SCI_ParityMode Parity, Boolean TwoStopBits, Boolean EnableRXPU);
void ZwSCIb_Init(Int32U BaudRate, Int16U DataBits, enum Zw_SCI_ParityMode Parity, Boolean TwoStopBits, Boolean EnableRXPU);
// Enable FIFO feature
void ZwSCIa_InitFIFO(Int16U RXLevel, Int16U TXLevel);
void ZwSCIb_InitFIFO(Int16U RXLevel, Int16U TXLevel);
// Enable RX & TX interrupt signals
void ZwSCIa_EnableInterrupts(Boolean RX, Boolean TX);
void ZwSCIb_EnableInterrupts(Boolean RX, Boolean TX);
// Enable interrupts on CPU-level
void ZwSCI_EnableInterruptsGlobal(Boolean Enable);
// Send single char
void ZwSCIa_SendChar(Int16U Value);
void ZwSCIb_SendChar(Int16U Value);
// Send Int16 as 2 chars (BIG-ENDIAN)
void ZwSCIa_SendInt16(Int16U Value);
void ZwSCIb_SendInt16(Int16U Value);
// Send array of chars
void ZwSCIa_SendArray16(pInt16U Buffer, Int16U BufferSize);
void ZwSCIb_SendArray16(pInt16U Buffer, Int16U BufferSize);
// Receive single char
Int16U ZwSCIa_ReceiveChar();
Int16U ZwSCIb_ReceiveChar();
// Receive Int16 as 2 chars (BIG-ENDIAN)
void ZwSCIa_ReceiveInt16(pInt16U Value);
void ZwSCIb_ReceiveInt16(pInt16U Value);
// Receive message from FIFO buffer
void ZwSCIa_ReceiveArray16(pInt16U Buffer, Int16U BufferSize);
void ZwSCIb_ReceiveArray16(pInt16U Buffer, Int16U BufferSize);
// Enable/disable TX
void ZwSCIa_SetTXState(Boolean EN);
void ZwSCIb_SetTXState(Boolean EN);
// Enable/disable RX
void ZwSCIa_SetRXState(Boolean EN);
void ZwSCIb_SetRXState(Boolean EN);
// Clear FIFO buffer
void ZwSCIa_ClearInputBuffer();
void ZwSCIb_ClearInputBuffer();

// Inline functions
//
Int16U inline ZwSCIa_GetBytesToReceive()
{
	return SciaRegs.SCIFFRX.bit.RXFFST;
}
//
Int16U inline ZwSCIb_GetBytesToReceive()
{
	return ScibRegs.SCIFFRX.bit.RXFFST;
}
//
void inline ZwSCIa_ProcessRXInterrupt()
{
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
}
//
void inline ZwSCIa_ProcessTXInterrupt()
{
	SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;	
}
//
void inline ZwSCIb_ProcessRXInterrupt()
{
	ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
}
//
void inline ZwSCIb_ProcessTXInterrupt()
{
	ScibRegs.SCIFFTX.bit.TXFFINTCLR = 1;	
}

// ISR postfixes
#define SCI_ISR_DONE 	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9

#endif // end __ZW_SCI_H
