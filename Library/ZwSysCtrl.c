// -----------------------------------------
// API for DSP initializations and control
// ----------------------------------------

// Header
#include "ZwSysCtrl.h"

// Constants
//
#define ZW_WD_CHK 	      (BIT3 | BIT5)			// CHK bits
#define ZW_WD_DIVIDER_MSK (BIT0 | BIT1 |BIT2)	// mask for divider bits
#define ZW_WD_FORCE 	  0						// wrong CHK bits to reset
#define ZW_SC_XCLKPCP 	  0x03    				// XCLKOUT = NONE

// Forward functions
//
static void ZwSystem_InitPeripheralClocks(Int16U LowPClkPrescaler, Int16U HighPClkPrescaler, Int32U ModulesClock);
static Boolean ZwSystem_InitPll(Int16U Value, Int16U ClkInDiv);

// Functions
//
Boolean ZwSystem_Init(Int16U PLLValue, Int16U ClkInDiv, Int16U LowPClkPrescaler, Int16U HighPClkPrescaler, Int32U ModulesClock)
{
   // Initialize the PLL control: PLLCR and CLKINDIV
   if(ZwSystem_InitPll(PLLValue, ClkInDiv))
   {
	   // Initialize the peripheral clocks
	   ZwSystem_InitPeripheralClocks(LowPClkPrescaler, HighPClkPrescaler, ModulesClock);
	   return TRUE;
   }
   
   return FALSE;
}
// ----------------------------------------

void ZwSystem_SelectDogFunc(Boolean Interrupt)
{
	EALLOW;
	if(Interrupt)
		SysCtrlRegs.SCSR |= BIT1;
	else
		SysCtrlRegs.SCSR = 0;
	EDIS;
}
// ----------------------------------------

Boolean ZwSystem_GetDogIntState()
{
	return (SysCtrlRegs.SCSR & BIT2) ? TRUE : FALSE;
}
// ----------------------------------------

void ZwSystem_EnableDog(Int16U Divider)
{
	Int16U ctrlWord = ZW_WD_CHK | (ZW_WD_DIVIDER_MSK & Divider);

	EALLOW;
    SysCtrlRegs.WDCR = ctrlWord;
    EDIS;
}
// ----------------------------------------

void ZwSystem_DisableDog()
{
    EALLOW;
    SysCtrlRegs.WDCR = ZW_WD_CHK | BIT6;
    EDIS;
}
// ----------------------------------------

void ZwSystem_ForceDog()
{
    EALLOW;
    SysCtrlRegs.WDCR = ZW_WD_FORCE;
    EDIS;
}
// ----------------------------------------

void ZwSystem_ServiceDog()
{
	EALLOW;
	SysCtrlRegs.WDKEY = 0x55;
	SysCtrlRegs.WDKEY = 0xAA;
	EDIS;
}
// ----------------------------------------

void ZwSystem_LockDog()
{
	EALLOW;
	SysCtrlRegs.SCSR |= BIT0;
	EDIS;
}
// ----------------------------------------

Boolean ZwSystem_GetDogAlarmFlag()
{
	return (SysCtrlRegs.WDCR & BIT7) ? TRUE : FALSE;
}
// ----------------------------------------

void ZwSystem_ClearDogAlarmFlag()
{
	Int16U ctrlWord = SysCtrlRegs.WDCR;
	ctrlWord |= (ZW_WD_CHK | BIT7);

	EALLOW;
    SysCtrlRegs.WDCR = ctrlWord;
    EDIS;
}
// ----------------------------------------

static Boolean ZwSystem_InitPll(Int16U Value, Int16U ClkInDiv)
{
    volatile Int16U iVol;

    // Make sure the PLL is not running in limp mode
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
    {
		ESTOP0;
		return FALSE;
    }

    // Change the PLLCR
    if (SysCtrlRegs.PLLCR.bit.DIV != Value)
    {
	// CLKINDIV MUST be 0 before PLLCR can be changed from
	// 0x0000. It is set to 0 by an external reset XRSn
	if (SysCtrlRegs.PLLSTS.bit.CLKINDIV != 0)
	{
	    EALLOW;
	    SysCtrlRegs.PLLSTS.bit.CLKINDIV = 0;
	    EDIS;
	}

        EALLOW;
        // Before setting PLLCR turn off missing clock detect logic
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
        SysCtrlRegs.PLLCR.bit.DIV = Value;
        EDIS;

	ZwSystem_DisableDog();
        while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1);

        EALLOW;
        // turn on missing clock detect logic
		SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
        // set clock divider
		SysCtrlRegs.PLLSTS.bit.CLKINDIV = ClkInDiv;
        EDIS;
    }
    
    return TRUE;
}
// ----------------------------------------

void ZwSystem_InitPeripheralClocks(Int16U LowPClkPrescaler, Int16U HighPClkPrescaler, Int32U ModulesClock)
{
	EALLOW;

    // HISPCP/LOSPCP prescaler register settings
    // normally it will be set to default values
    SysCtrlRegs.HISPCP.all = HighPClkPrescaler; 
    SysCtrlRegs.LOSPCP.all = LowPClkPrescaler; 

    // XCLKOUT to SYSCLKOUT ratio.
    SysCtrlRegs.XCLK.bit.XCLKOUTDIV = ZW_SC_XCLKPCP;

    // PWM clock
	if(ModulesClock & ZW_POWER_PWM1_CLK)
		SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1; 
    if(ModulesClock & ZW_POWER_PWM2_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1; 
    if(ModulesClock & ZW_POWER_PWM3_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1; 
    if(ModulesClock & ZW_POWER_PWM4_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1; 
    if(ModulesClock & ZW_POWER_PWM5_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1; 
    if(ModulesClock & ZW_POWER_PWM6_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1; 

    // Disable TBCLK within the ePWM
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

	// QEP clock
    if(ModulesClock & ZW_POWER_QEP1_CLK)
		SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;  
    if(ModulesClock & ZW_POWER_QEP2_CLK)
		SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1;  

	// CAN clock
	if(ModulesClock & ZW_POWER_CANA_CLK)
       SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;    
	if(ModulesClock & ZW_POWER_CANB_CLK)
       SysCtrlRegs.PCLKCR0.bit.ECANBENCLK=1;    

	// SCI clock
	if(ModulesClock & ZW_POWER_SCIA_CLK)
       SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   
	if(ModulesClock & ZW_POWER_SCIB_CLK)
       SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   

	// SPI clock
	if(ModulesClock & ZW_POWER_SPIA_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;   
	if(ModulesClock & ZW_POWER_SPIB_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 1;   
	if(ModulesClock & ZW_POWER_SPIC_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPICENCLK = 1;   
	if(ModulesClock & ZW_POWER_SPID_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIDENCLK = 1;   

	// ADC clock
	if(ModulesClock & ZW_POWER_ADC_CLK)
       SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    
	   
	// I2C clock
	if(ModulesClock & ZW_POWER_I2CA_CLK)
       SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;   

	// CAP clock
	if(ModulesClock & ZW_POWER_CAP1_CLK)
       SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;  
	if(ModulesClock & ZW_POWER_CAP2_CLK)
       SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1;  

    EDIS;
}
// ----------------------------------------

void ZwSystem_AlterPeripheralClocks(Int32U ModulesClockToEnable, Int32U ModulesClockToDisable)
{
	EALLOW;

	// Enable clocks
    // PWM clock
	if(ModulesClockToEnable & ZW_POWER_PWM1_CLK)
		SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;
    if(ModulesClockToEnable & ZW_POWER_PWM2_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;
    if(ModulesClockToEnable & ZW_POWER_PWM3_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;
    if(ModulesClockToEnable & ZW_POWER_PWM4_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;
    if(ModulesClockToEnable & ZW_POWER_PWM5_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1;
    if(ModulesClockToEnable & ZW_POWER_PWM6_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1;

	// QEP clock
    if(ModulesClockToEnable & ZW_POWER_QEP1_CLK)
		SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;
    if(ModulesClockToEnable & ZW_POWER_QEP2_CLK)
		SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1;

	// CAN clock
	if(ModulesClockToEnable & ZW_POWER_CANA_CLK)
       SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;
	if(ModulesClockToEnable & ZW_POWER_CANB_CLK)
       SysCtrlRegs.PCLKCR0.bit.ECANBENCLK=1;

	// SCI clock
	if(ModulesClockToEnable & ZW_POWER_SCIA_CLK)
       SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;
	if(ModulesClockToEnable & ZW_POWER_SCIB_CLK)
       SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;

	// SPI clock
	if(ModulesClockToEnable & ZW_POWER_SPIA_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;
	if(ModulesClockToEnable & ZW_POWER_SPIB_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 1;
	if(ModulesClockToEnable & ZW_POWER_SPIC_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPICENCLK = 1;
	if(ModulesClockToEnable & ZW_POWER_SPID_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIDENCLK = 1;

	// ADC clock
	if(ModulesClockToEnable & ZW_POWER_ADC_CLK)
       SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;

	// I2C clock
	if(ModulesClockToEnable & ZW_POWER_I2CA_CLK)
       SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;

	// CAP clock
	if(ModulesClockToEnable & ZW_POWER_CAP1_CLK)
       SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;
	if(ModulesClockToEnable & ZW_POWER_CAP2_CLK)
       SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1;


	// Disable clocks
    // PWM clock
	if(ModulesClockToDisable & ZW_POWER_PWM1_CLK)
		SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 0;
    if(ModulesClockToDisable & ZW_POWER_PWM2_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 0;
    if(ModulesClockToDisable & ZW_POWER_PWM3_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 0;
    if(ModulesClockToDisable & ZW_POWER_PWM4_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;
    if(ModulesClockToDisable & ZW_POWER_PWM5_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;
    if(ModulesClockToDisable & ZW_POWER_PWM6_CLK)
        SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;

	// QEP clock
    if(ModulesClockToDisable & ZW_POWER_QEP1_CLK)
		SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;
    if(ModulesClockToDisable & ZW_POWER_QEP2_CLK)
		SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;

	// CAN clock
	if(ModulesClockToDisable & ZW_POWER_CANA_CLK)
       SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0;
	if(ModulesClockToDisable & ZW_POWER_CANB_CLK)
       SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0;

	// SCI clock
	if(ModulesClockToDisable & ZW_POWER_SCIA_CLK)
       SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 0;
	if(ModulesClockToDisable & ZW_POWER_SCIB_CLK)
       SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 0;

	// SPI clock
	if(ModulesClockToDisable & ZW_POWER_SPIA_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 0;
	if(ModulesClockToDisable & ZW_POWER_SPIB_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 0;
	if(ModulesClockToDisable & ZW_POWER_SPIC_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPICENCLK = 0;
	if(ModulesClockToDisable & ZW_POWER_SPID_CLK)
       SysCtrlRegs.PCLKCR0.bit.SPIDENCLK = 0;

	// ADC clock
	if(ModulesClockToDisable & ZW_POWER_ADC_CLK)
       SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0;

	// I2C clock
	if(ModulesClockToDisable & ZW_POWER_I2CA_CLK)
       SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0;

	// CAP clock
	if(ModulesClockToDisable & ZW_POWER_CAP1_CLK)
       SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;
	if(ModulesClockToDisable & ZW_POWER_CAP2_CLK)
       SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 0;

	EDIS;
}
// ----------------------------------------

//----------------------------------------
#pragma DATA_SECTION(SysCtrlRegs,"SysCtrlRegsFile");
volatile struct SYS_CTRL_REGS SysCtrlRegs;

// No more.
