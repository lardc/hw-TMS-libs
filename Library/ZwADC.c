// -----------------------------------------
// ADC operational functions
// ----------------------------------------- 

// Header
#include "ZwADC.h"

// Include
#include "ZwPIE.h"
#include "ZwUtils.h"


// Constants
//
#define ZW_ADC_PONDELAY		5000L	// 5 ms


// Variables
//
FUNC_ADCResultSubscriber SubscriberFunc1 = NULL, SubscriberFunc2 = NULL;


// Functions
//
// Initialize ADC
void ZwADC_Init(Int16U ClockPrescaler, Boolean CoreDiv2, Int16U SHWindow)
{
    // Power on
    AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x03;  		// enable analog core
    AdcRegs.ADCTRL3.bit.ADCPWDN = 0x01;    		// enable digital core           
	// Delay after power on
	DELAY_US(ZW_ADC_PONDELAY);

    // Set S/H window width
    AdcRegs.ADCTRL1.bit.ACQ_PS = SHWindow;
    // Set core clock div
    AdcRegs.ADCTRL1.bit.CPS = (CoreDiv2) ? 0x01 : 0x00; 
    // Set ADC module clock prescaler
    AdcRegs.ADCTRL3.bit.ADCCLKPS = ClockPrescaler;
    // Ignore emulator
    AdcRegs.ADCTRL1.bit.SUSMOD = 0x00;      
	// Start-stop mode
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0x00;
}
// ----------------------------------------- 

void ZwADC_ConfigureSimultaneousCascaded(Int16U ConversionsCount, const Int16U ChannelsAB[8])
{
	// Cascaded mode
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 0x01;
	// Simultaneous sampling mode
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0x01;
    // 	Number of conversions
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = ConversionsCount - 1;
    // Set conversion inputs
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = ChannelsAB[0]; 
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = ChannelsAB[1]; 
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = ChannelsAB[2]; 
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = ChannelsAB[3]; 
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = ChannelsAB[4]; 
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = ChannelsAB[5]; 
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = ChannelsAB[6]; 
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = ChannelsAB[7]; 
}
// -----------------------------------------

void ZwADC_ConfigureSimultaneousDual(Int16U ConversionsCount1, const Int16U ChannelsAB1[4],
									 Int16U ConversionsCount2, const Int16U ChannelsAB2[4])
{
	// Cascaded mode
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 0x00;
	// Simultaneous sampling mode
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0x01;
    // 	Number of conversions
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = ConversionsCount1 - 1;
    AdcRegs.ADCMAXCONV.bit.MAX_CONV2 = ConversionsCount2 - 1;
    // Set conversion inputs
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = ChannelsAB1[0];
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = ChannelsAB1[1];
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = ChannelsAB1[2];
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = ChannelsAB1[3];
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = ChannelsAB2[0];
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = ChannelsAB2[1];
    AdcRegs.ADCCHSELSEQ3.bit.CONV10 = ChannelsAB2[2];
    AdcRegs.ADCCHSELSEQ3.bit.CONV11 = ChannelsAB2[3];
}
// -----------------------------------------

void ZwADC_ConfigureSequentialCascaded(Int16U ConversionsCount, const Int16U Channels[16])
{
	// Cascaded mode
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 0x01;
	// Sequential sampling mode
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0x00;
    // 	Number of conversions
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = ConversionsCount - 1;
    // Set conversion inputs
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = Channels[0];
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = Channels[1];
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = Channels[2];
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = Channels[3];
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = Channels[4];
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = Channels[5];
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = Channels[6];
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = Channels[7];
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = Channels[8];
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = Channels[9];
    AdcRegs.ADCCHSELSEQ3.bit.CONV10 = Channels[10];
    AdcRegs.ADCCHSELSEQ3.bit.CONV11 = Channels[11];
    AdcRegs.ADCCHSELSEQ4.bit.CONV12 = Channels[12];
    AdcRegs.ADCCHSELSEQ4.bit.CONV13 = Channels[13];
    AdcRegs.ADCCHSELSEQ4.bit.CONV14 = Channels[14];
    AdcRegs.ADCCHSELSEQ4.bit.CONV15 = Channels[15];
}
// -----------------------------------------

void ZwADC_ConfigureSequentialDual(Int16U ConversionsCount1, const Int16U Channels1[8],
									 Int16U ConversionsCount2, const Int16U Channels2[8])
{
	// Cascaded mode
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 0x00;
	// Sequential sampling mode
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0x00;
    // 	Number of conversions
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = ConversionsCount1 - 1;
    AdcRegs.ADCMAXCONV.bit.MAX_CONV2 = ConversionsCount2 - 1;
    // Set conversion inputs
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = Channels1[0];
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = Channels1[1];
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = Channels1[2];
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = Channels1[3];
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = Channels1[4];
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = Channels1[5];
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = Channels1[6];
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = Channels1[7];
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = Channels2[0];
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = Channels2[1];
    AdcRegs.ADCCHSELSEQ3.bit.CONV10 = Channels2[2];
    AdcRegs.ADCCHSELSEQ3.bit.CONV11 = Channels2[3];
    AdcRegs.ADCCHSELSEQ4.bit.CONV12 = Channels2[4];
    AdcRegs.ADCCHSELSEQ4.bit.CONV13 = Channels2[5];
    AdcRegs.ADCCHSELSEQ4.bit.CONV14 = Channels2[6];
    AdcRegs.ADCCHSELSEQ4.bit.CONV15 = Channels2[7];
}
// -----------------------------------------

void ZwADC_ConfigureSOC(Boolean EnableSOCA, Boolean EnableSOCB, Boolean EnableSOCBCasc, Boolean EnableSOCExt)
{
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = EnableSOCA ? 1 : 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = EnableSOCB ? 1 : 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ = EnableSOCBCasc ? 1 : 0;
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = EnableSOCExt ? 1 : 0;
}
// -----------------------------------------

void ZwADC_ConfigInterrupts(Boolean Enable1, Boolean Enable2)
{
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = Enable1 ? 0x01 : 0x00;  	// Enable SEQ1 interrupt (every EOS)
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = Enable2 ? 0x01 : 0x00;	// Enable SEQ2 interrupt (every EOS)
}
// -----------------------------------------

void ZwADC_EnableInterrupts(Boolean Enable1, Boolean Enable2)
{
	PieCtrlRegs.PIEIER1.bit.INTx1 = Enable1 ? 0x01 : 0x00;
	PieCtrlRegs.PIEIER1.bit.INTx2 = Enable2 ? 0x01 : 0x00;
}
// -----------------------------------------

void ZwADC_EnableInterruptsGlobal(Boolean Enable)
{
    if(Enable)
    	IER |= M_INT1; // Enable CPU Interrupt 1
    else
    	IER &= ~M_INT1;
}
// -----------------------------------------

FUNC_ADCResultSubscriber ZwADC_SubscribeToResults1(FUNC_ADCResultSubscriber Subscriber)
{
	FUNC_ADCResultSubscriber temp;

	temp = SubscriberFunc1;
	SubscriberFunc1 = Subscriber;

	return temp;
}
// -----------------------------------------

FUNC_ADCResultSubscriber ZwADC_SubscribeToResults2(FUNC_ADCResultSubscriber Subscriber)
{
	FUNC_ADCResultSubscriber temp;

	temp = SubscriberFunc2;
	SubscriberFunc2 = Subscriber;

	return temp;
}
// -----------------------------------------

void ZwADC_CalibrateLO(Int16U Channel)
{
	Int16U ADCChannelLO[16];

	// Prepare ADC for conversions
	MemFill16(ADCChannelLO, 16, Channel);	  
	ZwADC_ConfigureSequentialCascaded(sizeof(ADCChannelLO) / sizeof(ADCChannelLO[0]), ADCChannelLO);
	ZwADC_SubscribeToResults1(NULL);
	
	while(TRUE)
	{
		register Int16U i;
		pInt16U Results;
		Boolean ZeroFound = FALSE;
		Int16S Average = 0;
		
		// Convert 16 values
		ZwADC_StartSEQ1();
		DELAY_US(1000);
		Results = ZwADC_GetValues1();

		for(i = 0; i < 16; ++i)
		{
			if(!Results[i])
			{
				ZeroFound = TRUE;
				break;
			}
			
			Average += Results[i];
		}

		// Convert in addition 4 values (to accumulate totally 20)
		if(!ZeroFound)
		{
			ZwADC_StartSEQ1();
			DELAY_US(1000);
			Results = ZwADC_GetValues1();

			for(i = 0; i < 4; ++i)
			{
				if(!Results[i])
				{
					ZeroFound = TRUE;
					break;
				}

				Average += Results[i];
			}
		}
		
		// If zero was found - repeat with new offset
		if(ZeroFound)
		{
			// 40 is recommended step
			AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM += 40;
		}			
		else
		{
			Average /= 20;
			
			// 2^9 = 0x200
			if(Average < 0x200)
			{
				if(AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM >= Average)
					AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM -= Average;
				else
					AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM = 0x200 - Average;
			}
			
			break;			
		}
	}
}

//----------------------------------------
#pragma DATA_SECTION(AdcRegs,"AdcRegsFile");
volatile struct ADC_REGS AdcRegs;

//----------------------------------------
#pragma DATA_SECTION(AdcMirror,"AdcMirrorFile");
volatile struct ADC_RESULT_MIRROR_REGS AdcMirror;

// No more.
