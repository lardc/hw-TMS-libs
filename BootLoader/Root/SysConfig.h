// -----------------------------------------
// System parameters
// -----------------------------------------

#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

// Include
#include <ZwBase.h>
#include <BoardConfig.h>

// CPU & System
//--------------------------------------------------------
#define CPU_PLL				10          // OSCCLK * PLL div 2 = CPUCLK: 20 * 10 / 2 = 100
#define CPU_CLKINDIV		0           // "div 2" in the previous equation
#define SYS_HISPCP       	0x01   		// SYSCLKOUT / 2
#define SYS_LOSPCP       	0x01    	// SYSCLKOUT / 2
//--------------------------------------------------------

// Power control
//--------------------------------------------------------
#define SYS_PUMOD			(ZW_POWER_SCIA_CLK | ZW_POWER_SCIB_CLK  | ZW_POWER_CANA_CLK)
#define SYS_WD_PRESCALER	0x07
//--------------------------------------------------------

// GPIO
//--------------------------------------------------------
// Input filters
#define GPIO_TSAMPLE		200			// T[sample_A] = (1/ 100MHz) * (2 * 200) = 4 uS
//--------------------------------------------------------

// Flash
//--------------------------------------------------------
#define FLASH_FWAIT			3
#define FLASH_OTPWAIT		5
//--------------------------------------------------------

// Flash loader options
//--------------------------------------------------------
#define BOOT_LOADER_REQUEST		0xABCD
#define BOOT_LOADER_DELAY		5000		// (in ms)
#define BOOT_LOADER_RESP_DELAY	20			// (in ms)
//--------------------------------------------------------

// TIMERs
//--------------------------------------------------------
#define CS_T2_FREQ			1000		// 1000 Hz
#define CS_BLINK			20			// 20 Hz
#define TIMER2_PERIOD		(1000000L / CS_T2_FREQ)
#define TIMER2_BLINK_PERIOD	(CS_T2_FREQ / CS_BLINK)
//--------------------------------------------------------

// SCI-A
//--------------------------------------------------------
#define SCIA_BR				115200L		// UART baudrate = 115200 bps
#define SCIA_DB				8			// 8 bit
#define SCIA_SB				FALSE		// 1 stop bit
#define SCIA_PARITY			ZW_PAR_NONE	// No parity
//--------------------------------------------------------

// SCI-B
//--------------------------------------------------------
#define SCIB_BR				115200L		// UART baudrate = 115200 bps
#define SCIB_DB				8			// 8 bit
#define SCIB_SB				FALSE		// 1 stop bit
#define SCIB_PARITY			ZW_PAR_NONE	// No parity
//--------------------------------------------------------

// CAN-A
//--------------------------------------------------------
#define CANA_BR				1000000L
#define CANA_BRP			9
#define CANA_TSEG1			6
#define CANA_TSEG2			1
#define CANA_SJW			1
//--------------------------------------------------------

#endif // __SYSCONFIG_H
