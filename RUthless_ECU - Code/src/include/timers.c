/*
 * timers.c
 *
 * Created: 24.1.2017 14:49:34
 *  Author: rikardur
 */ 


#include "timers.h"
#include "uart.h"
#include "interrupts.h"
#include <asf.h>

/*
-----------------------------------------------------------------------
------------------------------	Timers	 ------------------------------
-----------------------------------------------------------------------
*/

// Global timer
void timers_init9(void)
{
	PMC->PMC_PCER1	=	(1 << (ID_TC8 - 32));			// Enable peripheral clock for timer 0
	interrupts_enable_interrupt_vector(TC8_IRQn, TC8_PRIORITY);				// Enable nested vector interrupt for timer 0
	
	TC2->TC_CHANNEL[2].TC_IER	=	TC_IER_COVFS;
	
	/*
	TIMER_CLOCK1	-	MCK/2
	TIMER_CLOCK2	-	MCK/8
	TIMER_CLOCK3	-	MCK/32
	TIMER_CLOCK4	-	MCK/128
	TIMER_CLOCK5	-	SLCK
	
	Where MCK is the master clock (84 MHz for the Arduino Due) and SLCK is the slow clock 
	(which can be clocked at 32.768 kHz by Y2 on the Arduino Due).
	*/	
	/* TC Channel mode register (MCK / 32) */
	TC2->TC_CHANNEL[2].TC_CMR	=	TC_CMR_TCCLKS_TIMER_CLOCK3	|	TC_CMR_WAVE;
		
	/* TC channel control register, enable counter */
	TC2->TC_CHANNEL[2].TC_CCR	=	TC_CCR_SWTRG	|	TC_CCR_CLKEN;
}




/* TimerChannel is 0-8 and priority is nested vector interrupt priority */
void timers_init(uint32_t TimerChannel, uint32_t TimerMode, uint32_t InterruptMode, uint8_t TimerInterruptPriority)
{
	// Check if the programmer is screwed
	if (TimerChannel > 8)
	return;
	
	// Enable peripheral clock
	pmc_enable_periph_clk(ID_TC0 + TimerChannel);
	
	Tc *Timer;
	// Initialize timer
	if (TimerChannel < 3) // TC0
	Timer = TC0;
	else if (TimerChannel < 6) // TC1
	Timer = TC1;
	else // TC2
	Timer = TC2;
	
	tc_init(Timer, (TimerChannel%3), TimerMode);
	
	interrupts_enable_interrupt_vector(TC0_IRQn + TimerChannel, TimerInterruptPriority);
	
	tc_enable_interrupt(Timer, (TimerChannel%3), InterruptMode);
	
	tc_start(Timer, (TimerChannel%3));
	
	// TODO !!!!!!!!!!!!!
	TC8_Overflow = FALSE;
	globalCurrMillis = 0;
	globalPrevMillis = 0;
}



void timers_set_compareRA_value(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value)
{
	if (ul_value < p_tc->TC_CHANNEL[ul_channel].TC_CV)
	{
		p_tc->TC_CHANNEL[ul_channel].TC_RA = p_tc->TC_CHANNEL[ul_channel].TC_CV + 50;
	}
	else
	{
		p_tc->TC_CHANNEL[ul_channel].TC_RA = ul_value;
	}
}

void timers_set_compareRB_value(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value)
{
	if (ul_value < p_tc->TC_CHANNEL[ul_channel].TC_CV)
	{
		p_tc->TC_CHANNEL[ul_channel].TC_RB = p_tc->TC_CHANNEL[ul_channel].TC_CV + 50;
	}
	else
	{
		p_tc->TC_CHANNEL[ul_channel].TC_RB = ul_value;
	}
}



/*
-----------------------------------------------------------------------
------------------------------	Handlers ------------------------------
-----------------------------------------------------------------------
*/

/*
Built in function
tc_read_ra (Tc *p_tc, uint32_t ul_channel)

tc_write_ra (Tc *p_tc, uint32_t ul_channel, uint32_t ul_value)

tc_enable_interrupt(Tc *p_tc, uint32_t ul_channel, uint32_t ul_sources)
*/




void TC0_Handler(void)
{
	uint32_t CounterValue	=	TC0->TC_CHANNEL[0].TC_CV;		// Read the counter value of timer/counter TC0 - Channel 0
	uint32_t TimerStatus	=	TC0->TC_CHANNEL[0].TC_SR;		// Read the Timer/Counter TC0, channel 0, status register
	
	
	
	if (TimerStatus & TC_SR_CPAS)								// Compare match for ignition
	{
		
		if (IgnStatus)
		{
			if (!Ign1Status)
			{
				PIOC->PIO_SODR = IGN1_OUT;
			}
			else
			{
				
				PIOC->PIO_CODR = IGN1_OUT;
				if (Ign1Wait)
				{
					timers_set_compareRA_value(TC0, 0, CrankCurrTriggerCounts + IgnInterval);
					Ign1Wait = FALSE;
				}
				
				Ign1Status = FALSE;
			}
		}
		else
		{
			PIOC->PIO_SODR = IGN1_OUT;
		}
		
	}
	if (TimerStatus & TC_SR_CPBS)								// Compare match for injection
	{
		if (engine_realtime.SquirtStatus)
		{
			if (!Inj1Status)
			{
				PIOC->PIO_SODR = INJ1_OUT;
			}
			else
			{
				PIOC->PIO_CODR = INJ1_OUT;
				Inj1Status = FALSE;
			}
		}
		else
		{
			PIOC->PIO_SODR = INJ1_OUT;
		}
		
	}
// 	if (TimerStatus & TC_SR_CPCS) // Compare register C injector 2
// 	{
// 	}
// 	if (TimerStatus & TC_SR_COVFS) // Overflow
// 	{
// 		// TODO: Necessary to handle the Overflow probably best to check when loading RA, RB and RC
// 	}
	
}






void TC1_Handler(void)
{
	uint32_t CounterValue	=	TC0->TC_CHANNEL[1].TC_CV;		// Read the counter value of timer/counter TC0 - Channel 1
	uint32_t TimerStatus	=	TC0->TC_CHANNEL[1].TC_SR;		// Read the Timer/Counter TC0, channel 1, status register

	if (TimerStatus & TC_SR_CPAS)								// Compare match for ignition
	{
		if (IgnStatus)
		{
			if (!Ign2Status)
			{
				PIOC->PIO_SODR = IGN2_OUT;
			}
			else
			{
				PIOC->PIO_CODR = IGN2_OUT;
				if (Ign2Wait)
				{
					timers_set_compareRA_value(TC0, 1, CrankCurrTriggerCounts + IgnInterval);
					Ign2Wait = FALSE;
				}
				Ign2Status = FALSE;
			}
		}
		else
		{
			PIOC->PIO_SODR = IGN2_OUT;
		}
		
	}
	if (TimerStatus & TC_SR_CPBS)								// Compare match for injection
	{
		if (engine_realtime.SquirtStatus)
		{
			if (!Inj2Status)
			{
				PIOC->PIO_SODR = INJ2_OUT;
			}
			else
			{
				PIOC->PIO_CODR = INJ2_OUT;
				Inj2Status = FALSE;
			}
		}
		else
		{
			PIOC->PIO_SODR = INJ2_OUT;
		}
		
	}
	// 	if (TimerStatus & TC_SR_CPCS) // Compare register C injector 2
	// 	{
	// 	}
	// 	if (TimerStatus & TC_SR_COVFS) // Overflow
	// 	{
	// 		// TODO: Necessary to handle the Overflow probably best to check when loading RA, RB and RC
	// 	}
}






void TC2_Handler(void)
{
	uint32_t CounterValue	=	TC0->TC_CHANNEL[2].TC_CV;		// Read the counter value of timer/counter TC0 - Channel 2
	uint32_t TimerStatus	=	TC0->TC_CHANNEL[2].TC_SR;		// Read the Timer/Counter TC0, channel 2, status register

	if (TimerStatus & TC_SR_CPAS)								// Compare match for ignition
	{
		if (IgnStatus)
		{
			if (!Ign3Status)
			{
				PIOC->PIO_SODR = IGN3_OUT;
			}
			else
			{
				PIOC->PIO_CODR = IGN3_OUT;
				Ign3Status = FALSE;
				if (Ign3Wait)
				{
					timers_set_compareRA_value(TC0, 2, CrankCurrTriggerCounts + IgnInterval);
					Ign3Wait = FALSE;
				}
			}
		}
		else
		{
			PIOC->PIO_SODR = IGN3_OUT;
		}
		
	}
	if (TimerStatus & TC_SR_CPBS)								// Compare match for injection
	{
		if (engine_realtime.SquirtStatus)
		{
			if (!Inj3Status)
			{
				PIOC->PIO_SODR = INJ3_OUT;
			}
			else
			{
				PIOC->PIO_CODR = INJ3_OUT;
				Inj3Status = FALSE;
			}
		}
		else
		{
			PIOC->PIO_SODR = INJ3_OUT;
		}
		
	}
	// 	if (TimerStatus & TC_SR_CPCS) // Compare register C injector 2
	// 	{
	// 	}
	// 	if (TimerStatus & TC_SR_COVFS) // Overflow
	// 	{
	// 		// TODO: Necessary to handle the Overflow probably best to check when loading RA, RB and RC
	// 	}
}






void TC3_Handler(void)
{
	uint32_t CounterValue	=	TC1->TC_CHANNEL[0].TC_CV;		// Read the counter value of timer/counter TC1 - Channel 0
	uint32_t TimerStatus	=	TC1->TC_CHANNEL[0].TC_SR;		// Read the Timer/Counter TC1, channel 0, status register

	if (TimerStatus & TC_SR_CPAS)								// Compare match for ignition
	{
		if (IgnStatus)
		{
			if (!Ign4Status)
			{
				PIOC->PIO_SODR = IGN4_OUT;
			}
			else
			{
				PIOC->PIO_CODR = IGN4_OUT;
				Ign4Status = FALSE;
				if (Ign4Wait)
				{
					timers_set_compareRA_value(TC1, 0, CrankCurrTriggerCounts + IgnInterval);
					Ign4Wait = FALSE;
				}
			}
		}
		else
		{
			PIOC->PIO_SODR = IGN4_OUT;
		}
			
	}
	if (TimerStatus & TC_SR_CPBS)								// Compare match for injection
	{
		if (engine_realtime.SquirtStatus)
		{
			if (!Inj4Status)
			{
				PIOC->PIO_SODR = INJ4_OUT;
			}
			else
			{
				PIOC->PIO_CODR = INJ4_OUT;
				Inj4Status = FALSE;
			}
		}
		else
		{
			PIOC->PIO_SODR = INJ4_OUT;
		}
		
	}
	// 	if (TimerStatus & TC_SR_CPCS) // Compare register C injector 2
	// 	{
	// 	}
	// 	if (TimerStatus & TC_SR_COVFS) // Overflow
	// 	{
	// 		// TODO: Necessary to handle the Overflow probably best to check when loading RA, RB and RC
	// 	}
	
}





void TC8_Handler(void)
{
	// Read the current TC8 Counter Value
	uint32_t CounterValue = TC2->TC_CHANNEL[2].TC_CV;
	// Read the current TC8 Status, Compare or overflow
	uint32_t TimerStatus = TC2->TC_CHANNEL[2].TC_SR;

	if (TimerStatus & TC_SR_CPAS)															// RA Compare Status register
	{
		tc_write_ra(TC2, 2, CounterValue + GLOBAL_TIMER_FREQ/GLOBAL_FREQ_ADC_SCALER);		// Set the compare value
		adc_start(ADC);																		// Start analog-to-digital conversion 
	}
	if (TimerStatus & TC_SR_CPCS)
	{
		TC2->TC_CHANNEL[2].TC_RC = CounterValue + GLOBAL_MILLIS_SCALER;
		globalCurrMillis++;
	}
	else if (TimerStatus & TC_SR_COVFS)														// Counter overflow status register, if the counter has overflowed
	{
		TC8_Overflow = TRUE;
		tc_write_ra(TC2, 2, GLOBAL_TIMER_FREQ/GLOBAL_FREQ_ADC_SCALER);
		//tc_write_rb(TC2, 2, GLOBAL_TIMER_FREQ/GlobalTimerFreqUARTScaler);
		tc_write_rc(TC2, 2, GLOBAL_TIMER_FREQ/MILLI_SEC);
	}
	

}
