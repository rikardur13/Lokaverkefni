/*
 * interrupts.c
 *
 * Created: 24.1.2017 14:50:55
 *  Author: rikardur
 */ 

#include "interrupts.h"

/* Initialize nested vector interrupts */
/* Enable interrupt controller */
/************************************************************************/
	/*
	TC  Chan    NVIC "irq"  IRQ handler    function PMC id
	TC0    0    TC0_IRQn    TC0_Handler    ID_TC0
	TC0    1    TC1_IRQn    TC1_Handler    ID_TC1
	TC0    2    TC2_IRQn    TC2_Handler    ID_TC2
	TC1    0    TC3_IRQn    TC3_Handler    ID_TC3
	TC1    1    TC4_IRQn    TC4_Handler    ID_TC4
	TC1    2    TC5_IRQn    TC5_Handler    ID_TC5
	TC2    0    TC6_IRQn    TC6_Handler    ID_TC6
	TC2    1    TC7_IRQn    TC7_Handler    ID_TC7
	TC2    2    TC8_IRQn    TC8_Handler    ID_TC8                         
	*/
/************************************************************************/

void interrupts_enable_interrupt_vector(uint32_t irqn, uint32_t priority)
{
	
	NVIC_DisableIRQ(irqn);
	NVIC_ClearPendingIRQ(irqn);
	NVIC_SetPriority(irqn, priority);
	NVIC_EnableIRQ(irqn);
}





/*
-----------------------------------------------------------------------
------------------------------Initialize ------------------------------
-----------------------------------------------------------------------
*/

// Crank sensor & Cam sensor
void interrupts_init_regB(void)
{
	/* Enable interrupt registers */
	pmc_enable_periph_clk(ID_PIOB);									// Enable the peripheral clock for port A
	//PMC->PMC_PCER0	=	(1 << ID_PIOB);								
	
	PIOB->PIO_PER	=	CRANK_SIGNAL | CAM_SIGNAL;					// Enable the PIO control to the pins

	PIOB->PIO_IER	=	CRANK_SIGNAL | CAM_SIGNAL;					// Enable the interrupt register on pin 7 in port A
	interrupts_enable_interrupt_vector(PIOB_IRQn, PIOB_PRIORITY);				// Enable nested interrupt vector and set priority 0 in port B
	
	/*	Interrupt on rising edge		*/
	PIOB->PIO_AIMER		=	CRANK_SIGNAL | CAM_SIGNAL;				// Enable additional interrupt modes
	PIOB->PIO_ESR		=	CRANK_SIGNAL | CAM_SIGNAL;				// Select edge detection
	PIOB->PIO_REHLSR	=	CRANK_SIGNAL;							// Select rising edge detection for the crank signal
	PIOB->PIO_FELLSR	=	CAM_SIGNAL;								// Select falling edge detection for the cam signal
}
// TWI_PERIPHERAL->PIO_IDR = TWI_DATA_PIN;		// Interrupt disable register
// TWI_PERIPHERAL->PIO_IDR = TWI_CLOCK_PIN;
// TWI_PERIPHERAL->PIO_PDR = TWI_DATA_PIN;		// Disable input/output which enables peripheral mode
// TWI_PERIPHERAL->PIO_PDR = TWI_CLOCK_PIN;
// TWI_PERIPHERAL->PIO_ABSR &= ~TWI_DATA_PIN;	// Enable Peripheral A (TWI)
// TWI_PERIPHERAL->PIO_ABSR &= ~TWI_CLOCK_PIN;
// TWI_PERIPHERAL->PIO_PUER = TWI_DATA_PIN;
// TWI_PERIPHERAL->PIO_PUER = TWI_CLOCK_PIN;



// Neutral switch
void interrupts_init_regA(void)
{
	/* Enable interrupt registers */
	PMC->PMC_PCER0	=	(1 << ID_PIOA);					// Enable the peripheral clock for port A
	
	PIOA->PIO_PER	=	NEUTRAL_SWITCH;					// Enable the PIO control to the pins
	
	PIOA->PIO_IER	=	NEUTRAL_SWITCH;					// Enable the interrupt register on pin 7 in port A
	interrupts_enable_interrupt_vector(PIOA_IRQn, PIOA_PRIORITY);	// Enable nested interrupt vector and set priority 0 in port A
	
	/*	Interrupt on rising edge		*/
	PIOA->PIO_AIMER		=	NEUTRAL_SWITCH;				// Enable additional interrupt modes
	PIOA->PIO_ESR		=	NEUTRAL_SWITCH;				// Select edge detection
	PIOA->PIO_REHLSR	=	NEUTRAL_SWITCH;				// Select rising edge detection
	
	
	
	/*	Debouncing filter for a mechanical switch on pin D31	*/
	PIOA->PIO_IFER	=	NEUTRAL_SWITCH;					// Enable input glitch filter register
	PIOA->PIO_DIFSR	=	NEUTRAL_SWITCH;					// Select debouncing filter
	PIOA->PIO_SCDR	=	0x01;							// Set debouncing frequency  DIV = (32768Hz*(1/frequency))/2 - 1 
	
}



/*
-----------------------------------------------------------------------
------------------------------ Handlers -------------------------------
-----------------------------------------------------------------------
*/

// Crank and cam signal handler
void PIOB_Handler(void)
{
	CrankCurrTriggerCounts	=		TC2->TC_CHANNEL[2].TC_CV;				// Store the current counter value	
	uint32_t readpio		=		PIOB->PIO_ISR;							// Read what pin is interrupting
	
	// Check if Crank signal is triggered

	if (readpio & CRANK_SIGNAL)
	{
		
		CrankPrevToothGapCounts = CrankCurrToothGapCounts;
		CrankCurrToothGapCounts = CrankCurrTriggerCounts - CrankPrevTriggerCounts;				// Counts from previous tooth to current tooth
		
		CrankPrevTriggerCounts = CrankCurrTriggerCounts;										// Store the current counter value		
		
		if (CrankCurrToothGapCounts > (3 * (CrankPrevToothGapCounts >> 1)))						// Check if current gap is larger then 1.5 * the previous gap
		{
			CrankCurrCycleCounts = CrankCurrTriggerCounts - CrankPrevCycleCounts;
			CrankPrevCycleCounts = CrankCurrTriggerCounts;
			CrankTooth = 0;
			CrankCycleCounter++;
		}
		else
		{
			CrankTooth++;
		}
		
		if (CrankTooth == IgnToothFirstTach) ////////////////////////////////////////////// IGN TACH 1
		{
			if (CamSignal)
			{
				if (Ign4Status)
				{
					Ign4Wait =TRUE;
				}
				else
				{
					timers_set_compareRA_value(TC1, 0, CrankCurrTriggerCounts + IgnInterval);
					IgnDwellCounts = (CrankCurrTriggerCounts + IgnInterval) - IgnFirstRAValue;
				}
			}
			else
			{
				if (Ign1Status)
				{
					Ign1Wait =TRUE;
				}
				else
				{
					timers_set_compareRA_value(TC0, 0, CrankCurrTriggerCounts + IgnInterval);
					IgnDwellCounts = (CrankCurrTriggerCounts + IgnInterval) - IgnFirstRAValue;
				}
			}
		}
		else if (CrankTooth == IgnToothSecondTach) ////////////////////////////////////////////// IGN TACH 2
		{
			if (CamSignal)
			{
				if (Ign3Status)
				{
					Ign3Wait =TRUE;
				}
				else
				{
					timers_set_compareRA_value(TC0, 2, CrankCurrTriggerCounts + IgnInterval);
					IgnDwellCounts = (CrankCurrTriggerCounts + IgnInterval) - IgnSecondRAValue;
				}
			}
			else
			{
				if (Ign2Status)
				{
					Ign2Wait =TRUE;
				}
				else
				{
					timers_set_compareRA_value(TC0, 1, CrankCurrTriggerCounts + IgnInterval);
					IgnDwellCounts = (CrankCurrTriggerCounts + IgnInterval) - IgnSecondRAValue;
				}
			}
		}
		
		
		
		if (CrankTooth == InjToothFirstTach) ////////////////////////////////////////////// INJ TACH 1
		{
			if (CamSignal)
			{
				timers_set_compareRB_value(TC0, 0, CrankCurrTriggerCounts + InjClosingInterval);
			}
			else
			{
				timers_set_compareRB_value(TC1, 0, CrankCurrTriggerCounts + InjClosingInterval);
				
				
			}
		}
		else if (CrankTooth == InjToothSecondTach) ////////////////////////////////////////////// INJ TACH 2
		{
			if (CamSignal)
			{
				timers_set_compareRB_value(TC0, 1, CrankCurrTriggerCounts + InjClosingInterval);
			}
			else
			{
				timers_set_compareRB_value(TC0, 2, CrankCurrTriggerCounts + InjClosingInterval);
			}
		}
		
		
		
		CrankSignal = TRUE;
	}
	
	if (readpio & CAM_SIGNAL)
	{
		
		global_toggle_output_pin(RPM_OUT); 
		CamSignal = TRUE;
	}
}


// Neutral switch handler
void PIOA_Handler(void)
{
	uint32_t TimerCounterValue	=		TC2->TC_CHANNEL[2].TC_CV;			// Store the current counter value
	uint32_t readpio			=		PIOA->PIO_ISR;						// Read what pin is interrupting
	
	
	DebugFlag = ~DebugFlag;
}