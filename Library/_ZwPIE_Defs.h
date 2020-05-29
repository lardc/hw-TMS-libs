// -----------------------------------------
// PIE and PVT declarations.
//
// COPYRIGHT (C) TI
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

#ifndef __ZW_PIE_DEFS_H
#define __ZW_PIE_DEFS_H

#include "ZwBase.h"

//---------------------------------------------------------------------------
// PIE Control Register Bit Definitions:
//
// PIECTRL: Register bit definitions:
struct PIECTRL_BITS {      // bits description
   Uint16  ENPIE:1;        // 0    Enable PIE block
   Uint16  PIEVECT:15;     // 15:1 Fetched vector address
};

union PIECTRL_REG {
   Uint16                 all;
   struct PIECTRL_BITS  bit;
};  

// PIEIER: Register bit definitions:
struct PIEIER_BITS {       // bits description
   Uint16 INTx1:1;         // 0    INTx.1
   Uint16 INTx2:1;         // 1    INTx.2
   Uint16 INTx3:1;         // 2    INTx.3
   Uint16 INTx4:1;         // 3    INTx.4
   Uint16 INTx5:1;         // 4    INTx.5
   Uint16 INTx6:1;         // 5    INTx.6
   Uint16 INTx7:1;         // 6    INTx.7
   Uint16 INTx8:1;         // 7    INTx.8
   Uint16 rsvd:8;          // 15:8 reserved
};

union PIEIER_REG {
   Uint16              all;
   struct PIEIER_BITS  bit;
}; 

// PIEIFR: Register bit definitions:
struct PIEIFR_BITS {       // bits description
   Uint16 INTx1:1;         // 0    INTx.1
   Uint16 INTx2:1;         // 1    INTx.2
   Uint16 INTx3:1;         // 2    INTx.3
   Uint16 INTx4:1;         // 3    INTx.4
   Uint16 INTx5:1;         // 4    INTx.5
   Uint16 INTx6:1;         // 5    INTx.6
   Uint16 INTx7:1;         // 6    INTx.7
   Uint16 INTx8:1;         // 7    INTx.8
   Uint16 rsvd:8;          // 15:8 reserved
};

union PIEIFR_REG {
   Uint16              all;
   struct PIEIFR_BITS  bit;
};

// PIEACK: Register bit definitions:
struct PIEACK_BITS {       // bits description
   Uint16 ACK1:1;          // 0    Acknowledge PIE interrupt group 1
   Uint16 ACK2:1;          // 1    Acknowledge PIE interrupt group 2
   Uint16 ACK3:1;          // 2    Acknowledge PIE interrupt group 3
   Uint16 ACK4:1;          // 3    Acknowledge PIE interrupt group 4
   Uint16 ACK5:1;          // 4    Acknowledge PIE interrupt group 5
   Uint16 ACK6:1;          // 5    Acknowledge PIE interrupt group 6
   Uint16 ACK7:1;          // 6    Acknowledge PIE interrupt group 7
   Uint16 ACK8:1;          // 7    Acknowledge PIE interrupt group 8
   Uint16 ACK9:1;          // 8    Acknowledge PIE interrupt group 9
   Uint16 ACK10:1;         // 9    Acknowledge PIE interrupt group 10
   Uint16 ACK11:1;         // 10   Acknowledge PIE interrupt group 11
   Uint16 ACK12:1;         // 11   Acknowledge PIE interrupt group 12
   Uint16 rsvd:4;          // 15:12 reserved
};

union PIEACK_REG {
   Uint16              all;
   struct PIEACK_BITS  bit;
};

//---------------------------------------------------------------------------
// PIE Control Register File:
//
struct PIE_CTRL_REGS {
   union PIECTRL_REG PIECTRL;       // PIE control register
   union PIEACK_REG  PIEACK;        // PIE acknowledge
   union PIEIER_REG  PIEIER1;       // PIE INT1 IER register  
   union PIEIFR_REG  PIEIFR1;       // PIE INT1 IFR register
   union PIEIER_REG  PIEIER2;       // PIE INT2 IER register 
   union PIEIFR_REG  PIEIFR2;       // PIE INT2 IFR register
   union PIEIER_REG  PIEIER3;       // PIE INT3 IER register 
   union PIEIFR_REG  PIEIFR3;       // PIE INT3 IFR register
   union PIEIER_REG  PIEIER4;       // PIE INT4 IER register             
   union PIEIFR_REG  PIEIFR4;       // PIE INT4 IFR register
   union PIEIER_REG  PIEIER5;       // PIE INT5 IER register  
   union PIEIFR_REG  PIEIFR5;       // PIE INT5 IFR register
   union PIEIER_REG  PIEIER6;       // PIE INT6 IER register 
   union PIEIFR_REG  PIEIFR6;       // PIE INT6 IFR register
   union PIEIER_REG  PIEIER7;       // PIE INT7 IER register 
   union PIEIFR_REG  PIEIFR7;       // PIE INT7 IFR register
   union PIEIER_REG  PIEIER8;       // PIE INT8 IER register
   union PIEIFR_REG  PIEIFR8;       // PIE INT8 IFR register
   union PIEIER_REG  PIEIER9;       // PIE INT9 IER register  
   union PIEIFR_REG  PIEIFR9;       // PIE INT9 IFR register
   union PIEIER_REG  PIEIER10;      // PIE INT10 IER register 
   union PIEIFR_REG  PIEIFR10;      // PIE INT10 IFR register
   union PIEIER_REG  PIEIER11;      // PIE INT11 IER register 
   union PIEIFR_REG  PIEIFR11;      // PIE INT11 IFR register
   union PIEIER_REG  PIEIER12;      // PIE INT12 IER register
   union PIEIFR_REG  PIEIFR12;      // PIE INT12 IFR register
};     

#define PIEACK_GROUP1   0x0001
#define PIEACK_GROUP2   0x0002
#define PIEACK_GROUP3   0x0004
#define PIEACK_GROUP4   0x0008
#define PIEACK_GROUP5   0x0010
#define PIEACK_GROUP6   0x0020
#define PIEACK_GROUP7   0x0040
#define PIEACK_GROUP8   0x0080
#define PIEACK_GROUP9   0x0100
#define PIEACK_GROUP10  0x0200
#define PIEACK_GROUP11  0x0400
#define PIEACK_GROUP12  0x0800

//---------------------------------------------------------------------------
// PIE Control Registers External References & Function Declarations:
//
extern volatile struct PIE_CTRL_REGS PieCtrlRegs;

// #########################################################################
// #########################################################################

//---------------------------------------------------------------------------
// PIE Interrupt Vector Table Definition:
//
// Create a user type called PINT (pointer to interrupt):

typedef interrupt void(*PINT)(void);

// Define Vector Table:
struct PIE_VECT_TABLE {

// Reset is never fetched from this table.  
// It will always be fetched from 0x3FFFC0 in 
// boot ROM

      PINT     PIE1_RESERVED;  
      PINT     PIE2_RESERVED;
      PINT     PIE3_RESERVED;
      PINT     PIE4_RESERVED;
      PINT     PIE5_RESERVED;
      PINT     PIE6_RESERVED;
      PINT     PIE7_RESERVED;
      PINT     PIE8_RESERVED;
      PINT     PIE9_RESERVED;
      PINT     PIE10_RESERVED;
      PINT     PIE11_RESERVED;
      PINT     PIE12_RESERVED;
      PINT     PIE13_RESERVED;

// Non-Peripheral Interrupts:
      PINT     TINT1_XINT13;    // XINT13
      PINT     TINT2;     // CPU-Timer2
      PINT     DATALOG;   // Datalogging interrupt
      PINT     RTOSINT;   // RTOS interrupt
      PINT     EMUINT;    // Emulation interrupt
      PINT     XNMI;      // Non-maskable interrupt
      PINT     ILLEGAL;   // Illegal operation TRAP
      PINT     USER1;     // User Defined trap 1
      PINT     USER2;     // User Defined trap 2
      PINT     USER3;     // User Defined trap 3
      PINT     USER4;     // User Defined trap 4
      PINT     USER5;     // User Defined trap 5
      PINT     USER6;     // User Defined trap 6
      PINT     USER7;     // User Defined trap 7
      PINT     USER8;     // User Defined trap 8
      PINT     USER9;     // User Defined trap 9
      PINT     USER10;    // User Defined trap 10
      PINT     USER11;    // User Defined trap 11
      PINT     USER12;    // User Defined trap 12
      
// Group 1 PIE Peripheral Vectors:
      PINT     SEQ1INT;
      PINT     SEQ2INT;
      PINT     rsvd1_3;
      PINT     XINT1;     
      PINT     XINT2;
      PINT     ADCINT;    // ADC
      PINT     TINT0;     // Timer 0
      PINT     WAKEINT;   // WD
           
// Group 2 PIE Peripheral Vectors:
      PINT     EPWM1_TZINT; // EPWM-1
      PINT     EPWM2_TZINT; // EPWM-2
      PINT     EPWM3_TZINT; // EPWM-3
      PINT     EPWM4_TZINT; // EPWM-4
      PINT     EPWM5_TZINT; // EPWM-5
      PINT     EPWM6_TZINT; // EPWM-6
      PINT     rsvd2_7;    
      PINT     rsvd2_8;
      
// Group 3 PIE Peripheral Vectors:
      PINT     EPWM1_INT;  // EPWM-1
      PINT     EPWM2_INT;  // EPWM-2
      PINT     EPWM3_INT;  // EPWM-3
      PINT     EPWM4_INT;  // EPWM-4
      PINT     EPWM5_INT;  // EPWM-5
      PINT     EPWM6_INT;  // EPWM-6
      PINT     rsvd3_7;   
      PINT     rsvd3_8;
      
// Group 4 PIE Peripheral Vectors:
      PINT     ECAP1_INT; // ECAP-1
      PINT     ECAP2_INT; // ECAP-2
      PINT     ECAP3_INT; // ECAP-3
      PINT     ECAP4_INT; // ECAP-4
      PINT     rsvd4_5;    
      PINT     rsvd4_6;   
      PINT     rsvd4_7;   
      PINT     rsvd4_8;      
     
// Group 5 PIE Peripheral Vectors:
      PINT     EQEP1_INT; // EQEP-1
      PINT     EQEP2_INT; // EQEP-2
      PINT     rsvd5_3;   
      PINT     rsvd5_4;   
      PINT     rsvd5_5;   
      PINT     rsvd5_6;   
      PINT     rsvd5_7;   
      PINT     rsvd5_8;      

// Group 6 PIE Peripheral Vectors:
      PINT     SPIRXINTA; // SPI-A
      PINT     SPITXINTA; // SPI-A
      PINT     SPIRXINTB; // SPI-B
      PINT     SPITXINTB; // SPI-B
      PINT     SPIRXINTC; // SPI-C
      PINT     SPITXINTC; // SPI-C         
      PINT     SPIRXINTD; // SPI-D
      PINT     SPITXINTD; // SPI-D
      
// Group 7 PIE Peripheral Vectors:
      PINT     rsvd7_1;
      PINT     rsvd7_2;
      PINT     rsvd7_3;
      PINT     rsvd7_4;
      PINT     rsvd7_5;
      PINT     rsvd7_6;
      PINT     rsvd7_7;
      PINT     rsvd7_8;

// Group 8 PIE Peripheral Vectors:
      PINT     I2CINT1A;  // I2C-A
      PINT     I2CINT2A;  // I2C-A
      PINT     rsvd8_3;
      PINT     rsvd8_4;
      PINT     rsvd8_5;
      PINT     rsvd8_6;
      PINT     rsvd8_7;
      PINT     rsvd8_8; 

// Group 9 PIE Peripheral Vectors:   
      PINT     SCIRXINTA;  // SCI-A
      PINT     SCITXINTA;  // SCI-A
      PINT     SCIRXINTB;  // SCI-B
      PINT     SCITXINTB;  // SCI-B
      PINT     ECAN0INTA;  // eCAN-A
      PINT     ECAN1INTA;  // eCAN-A
      PINT     ECAN0INTB;  // eCAN-B
      PINT     ECAN1INTB;  // eCAN-B

// Group 10 PIE Peripheral Vectors:
      PINT     rsvd10_1;
      PINT     rsvd10_2;
      PINT     rsvd10_3;
      PINT     rsvd10_4;
      PINT     rsvd10_5;
      PINT     rsvd10_6;
      PINT     rsvd10_7;
      PINT     rsvd10_8;
            
// Group 11 PIE Peripheral Vectors:
      PINT     rsvd11_1;
      PINT     rsvd11_2;
      PINT     rsvd11_3;
      PINT     rsvd11_4;
      PINT     rsvd11_5;
      PINT     rsvd11_6;
      PINT     rsvd11_7;
      PINT     rsvd11_8;

// Group 12 PIE Peripheral Vectors:
      PINT     rsvd12_1;
      PINT     rsvd12_2;
      PINT     rsvd12_3;
      PINT     rsvd12_4;
      PINT     rsvd12_5;
      PINT     rsvd12_6;
      PINT     rsvd12_7;
      PINT     rsvd12_8;
};

//---------------------------------------------------------------------------
// PIE Interrupt Vector Table External References & Function Declarations:
//
extern struct PIE_VECT_TABLE PieVectTable;

#endif // end __ZW_PIE_DEFS_H
