// -----------------------------------------
// General system management definitions.
//
// COPYRIGHT (C) Texas Instruments
// _________________________________________
//
// DEPENDENCIES: 
//		ZwBase.h
// _________________________________________
//
// REVISION 	- 0
// REV. DATE 	- 4.01.2008
//
// ----------------------------------------- 

#ifndef __ZW_SYSCTRL_DEFS_H
#define __ZW_SYSCTRL_DEFS_H

#include "ZwBase.h"

// Data Types definitions
//
// XCLKOUT Control
struct XCLK_BITS   {    // bits  description
   Uint16 XCLKOUTDIV:2; // 1:0   XCLKOUT Divide Ratio
   Uint16 XCLKOUTDAT:1; // 2     reserved for TI use only
   Uint16 X1DAT:1;      // 3     reserved for TI use only
   Uint16 XCLKINDAT:1;  // 4     reserved for TI use only
   Uint16 rsvd1:3;      // 7:5   reserved
   Uint16 X1CNT:4;      // 11:8  reserved for TI use only
   Uint16 XCLKINCNT:4;  // 15:12 reserved for TI use only
};

union XCLK_REG {
   Uint16              all;
   struct XCLK_BITS    bit;
};

// PLL Status Register
struct PLLSTS_BITS   {    // bits  description
   Uint16 PLLLOCKS:1;     // 0     PLL lock status
   Uint16 CLKINDIV:1;     // 1     CLKIN div by 2
   Uint16 PLLOFF:1;       // 2     PLL off bit
   Uint16 MCLKSTS:1;      // 3     Missing clock status bit
   Uint16 MCLKCLR:1;      // 4     Missing clock clear bit
   Uint16 OSCOFF:1;       // 5     Oscillator clock off
   Uint16 MCLKOFF:1;      // 6     Missing clock detect
   Uint16 rsvd2:9;        // 15:7  reserved
};

union PLLSTS_REG {
   Uint16              all;
   struct PLLSTS_BITS  bit;
};

// High speed peripheral clock register bit definitions:
struct HISPCP_BITS  {   // bits  description
   Uint16 HSPCLK:3;     // 2:0   Rate relative to SYSCLKOUT
   Uint16 rsvd1:13;     // 15:3  reserved
};

union HISPCP_REG {
   Uint16              all;
   struct HISPCP_BITS  bit;
};

// Low speed peripheral clock register bit definitions:
struct LOSPCP_BITS  {   // bits  description
   Uint16 LSPCLK:3;     // 2:0   Rate relative to SYSCLKOUT
   Uint16 rsvd1:13;     // 15:3  reserved
};

union LOSPCP_REG {
   Uint16              all;
   struct LOSPCP_BITS  bit;
};

// Peripheral clock control register 0 bit definitions:
struct PCLKCR0_BITS  {  // bits  description
   Uint16 rsvd1:2;      // 1:0   reserved
   Uint16 TBCLKSYNC:1;  // 2     EWPM Module TBCLK enable/sync
   Uint16 ADCENCLK:1;   // 3     Enable high speed clk to ADC
   Uint16 I2CAENCLK:1;  // 4     Enable SYSCLKOUT to I2C-A
   Uint16 rsvd2:1;      // 5     reserved
   Uint16 SPICENCLK:1;  // 6     Enable low speed clk to SPI-C
   Uint16 SPIDENCLK:1;  // 7     Enable low speed clk to SPI-D
   Uint16 SPIAENCLK:1;  // 8     Enable low speed clk to SPI-A
   Uint16 SPIBENCLK:1;  // 9     Enable low speed clk to SPI-B
   Uint16 SCIAENCLK:1;  // 10    Enable low speed clk to SCI-A
   Uint16 SCIBENCLK:1;  // 11    Enable low speed clk to SCI-B
   Uint16 rsvd3:2;      // 13:12 reserved
   Uint16 ECANAENCLK:1; // 14    Enable system clk to eCAN-A
   Uint16 ECANBENCLK:1; // 15    Enable system clk to eCAN-B
};

union PCLKCR0_REG {
   Uint16              all;
   struct PCLKCR0_BITS bit;
};

// Peripheral clock control register 1 bit definitions:
struct PCLKCR1_BITS  {    // bits  description
   Uint16 EPWM1ENCLK:1;   // 0     Enable SYSCLKOUT to EPWM1
   Uint16 EPWM2ENCLK:1;   // 1     Enable SYSCLKOUT to EPWM2
   Uint16 EPWM3ENCLK:1;   // 2     Enable SYSCLKOUT to EPWM3
   Uint16 EPWM4ENCLK:1;   // 3     Enable SYSCLKOUT to EPWM4
   Uint16 EPWM5ENCLK:1;   // 4     Enable SYSCLKOUT to EPWM5
   Uint16 EPWM6ENCLK:1;   // 5     Enable SYSCLKOUT to EPWM6
   Uint16 rsvd1:2;        // 7:6   reserved
   Uint16 ECAP1ENCLK:1;   // 8     Enable SYSCLKOUT to ECAP1
   Uint16 ECAP2ENCLK:1;   // 9     Enable SYSCLKOUT to ECAP2
   Uint16 ECAP3ENCLK:1;   // 10    Enable SYSCLKOUT to ECAP3
   Uint16 ECAP4ENCLK:1;   // 11    Enable SYSCLKOUT to ECAP4
   Uint16 rsvd2:2;        // 13:12 reserved
   Uint16 EQEP1ENCLK:1;   // 11    Enable SYSCLKOUT to EQEP1
   Uint16 EQEP2ENCLK:1;   // 11    Enable SYSCLKOUT to EQEP2
};

union PCLKCR1_REG {
   Uint16              all;
   struct PCLKCR1_BITS bit;
};

// PLL control register bit definitions:
struct PLLCR_BITS {      // bits  description
   Uint16 DIV:4;         // 3:0   Set clock ratio for the PLL
   Uint16 rsvd1:12;      // 15:4  reserved
};

union PLLCR_REG {
   Uint16             all;
   struct PLLCR_BITS  bit;
};

// Low Power Mode 0 control register bit definitions:
struct LPMCR0_BITS {     // bits  description
   Uint16 LPM:2;         // 1:0   Set the low power mode
   Uint16 QUALSTDBY:6;   // 7:2   Qualification
   Uint16 rsvd1:7;       // 14:8  reserved
   Uint16 WDINTE:1;      // 15    Enables WD to wake the device from STANDBY
};

union LPMCR0_REG {
   Uint16              all;
   struct LPMCR0_BITS  bit;
};


//---------------------------------------------------------------------------
// System Control Register File:
//
struct SYS_CTRL_REGS {
   union   XCLK_REG    XCLK;      // 0
   union   PLLSTS_REG  PLLSTS;    // 1
   Uint16              rsvd1[8];  // 2-9
   union   HISPCP_REG  HISPCP;    // 10: High-speed peripheral clock pre-scaler
   union   LOSPCP_REG  LOSPCP;    // 11: Low-speed peripheral clock pre-scaler
   union   PCLKCR0_REG PCLKCR0;   // 12: Peripheral clock control register
   union   PCLKCR1_REG PCLKCR1;   // 13: Peripheral clock control register
   union   LPMCR0_REG  LPMCR0;    // 14: Low-power mode control register 0
   Uint16              rsvd2;     // 15: reserved
   Uint16              rsvd3;     // 16: reserved
   union   PLLCR_REG   PLLCR;     // 17: PLL control register
   // No bit definitions are defined for SCSR because
   // a read-modify-write instruction can clear the WDOVERRIDE bit
   Uint16              SCSR;      // 18: System control and status register
   Uint16              WDCNTR;    // 19: WD counter register
   Uint16              rsvd4;     // 20
   Uint16              WDKEY;     // 21: WD reset key register
   Uint16              rsvd5[3];  // 22-24
   // No bit definitions are defined for WDCR because
   // the proper value must be written to the WDCHK field
   // whenever writing to this register.
   Uint16              WDCR;      // 25: WD timer control register
   Uint16              rsvd6[6];  // 26-31
};


//---------------------------------------------------------------------------
// System Control External References & Function Declarations:
//
extern volatile struct SYS_CTRL_REGS SysCtrlRegs; 

#endif // end __ZW_SYSCTRL_DEFS_H

