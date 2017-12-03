/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \main page User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>

#include "include/global.h"
#include "include/ADC.h"
#include "include/interrupts.h"
#include "include/math.h"
#include "include/timers.h"
#include "include/uart.h"
#include "include/storage.h"
#include "include/sensors.h"
#include "include/igncalc.h"
#include "include/tables.h"
#include "include/fuelcalc.h"
#include "include/eeprom.h"

int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).
	sysclk_init();
	board_init();
	
	/************************************************************************/
	/* Initialize UART */
	/************************************************************************/
	uart_init();
	uart_tx_interrupt_init();
	uart_enable_rx_interrupt();
	
	/************************************************************************/
	/* Initialize all used output pins */
	/************************************************************************/
	global_init_input();
	global_init_output();
	
	/************************************************************************/
	/* Initialize and turn on ADC channels */
	/************************************************************************/
	
	/*************************/
	// 	ADC_VOLTAGE				
	// 	ADC_O2_SENSOR			
	// 	ADC_AIR_TEMP			
	// 	ADC_COOLANT_TEMP		
	// 	ADC_TPS				
	// 	ADC_MAP					
	/*************************/
	adc_initialize();
	adc_turn_on_channels();
	adc_start(ADC);
	
	/************************************************************************/
	/* Initialize timers */ 
	/************************************************************************/
	timers_init(GLOBAL_TIMER, TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_EEVT_XC0, TC_IER_COVFS | TC_IER_CPAS | TC_IER_CPBS | TC_IER_CPCS, TC8_PRIORITY);	
	timers_init(CYLINDER_1_TIMER, TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_EEVT_XC0, TC_IER_COVFS | TC_IER_CPAS | TC_IER_CPBS | TC_IER_CPCS, TC0_PRIORITY);
	timers_init(CYLINDER_2_TIMER, TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_EEVT_XC0, TC_IER_COVFS | TC_IER_CPAS | TC_IER_CPBS | TC_IER_CPCS, TC1_PRIORITY);
	timers_init(CYLINDER_3_TIMER, TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_EEVT_XC0, TC_IER_COVFS | TC_IER_CPAS | TC_IER_CPBS | TC_IER_CPCS, TC2_PRIORITY);
	timers_init(CYLINDER_4_TIMER, TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_EEVT_XC0, TC_IER_COVFS | TC_IER_CPAS | TC_IER_CPBS | TC_IER_CPCS, TC3_PRIORITY);
	
	tc_write_ra(TC2, 2, GLOBAL_TIMER_FREQ/GLOBAL_FREQ_ADC_SCALER);
	tc_write_rc(TC2, 2, GLOBAL_TIMER_FREQ/MILLI_SEC);
	
	/************************************************************************/
	/* Initialize Interrupts */
	/************************************************************************/
	interrupts_init_regB();
	interrupts_init_regA();
	
	/************************************************************************/
	/* Initialize Storage variables */
	/************************************************************************/
	storage_init();
	
	/************************************************************************/
	/* Initialize Sensors */
	/************************************************************************/
	sensors_init();
	
	/************************************************************************/
	/* Initialize EEPROM storage */
	/************************************************************************/
	eeprom_init();
 	//eeprom_init_variables();
	
	/************************************************************************/
	/* Initialize Tables */
	/************************************************************************/
	tables_init();
	
	/************************************************************************/
	/* Initialize global variables */
	/************************************************************************/
	global_init();
	


 	DebugFlag = FALSE;
	
	uint16_t Ve = 0;
	
	
	
	/* Simple turn on program */
	
	while (1)
	{
		
		if (CrankSignal)
		{
			decoders_crank_primary();
			if (engine_realtime.Rpm > RPM_LIMIT)										
			{
				IgnStatus = FALSE;
			}
			else if (engine_realtime.Rpm < RPM_LIMIT - 500)
			{
				IgnStatus = TRUE;
			}
			
			if (engine_realtime.Rpm < (engine_config2.CrankingRpm * RPM_SCALER) && engine_realtime.Tps > engine_config2.TpsFlood)
			{
				// Turn off injectors
				engine_realtime.SquirtStatus = FALSE;
			}
			else
			{
				// Turn on injectors
				engine_realtime.SquirtStatus = TRUE;
			}
			CrankEngineRunning = globalCurrMillis;
			engine_realtime.EngineStatus = TRUE;
			CrankSignal = FALSE;
		}
		else
		{
			
			if((globalCurrMillis - CrankEngineRunning) > 15)
			{
				// Set output pins to high
				engine_realtime.EngineStatus = FALSE;
				engine_realtime.Rpm = 0;
				PIOC->PIO_SODR	=	IGN1_OUT | IGN2_OUT | IGN3_OUT | IGN4_OUT | INJ1_OUT | INJ2_OUT | INJ3_OUT | INJ4_OUT | RPM_OUT;
				
			}
		}
		
		
		if((globalCurrMillis - globalPrevMillis) > GLOBAL_MILLI_SEC/2 - 1)
		{	
			
			decoders_rpm_calculation();
			globalPrevMillis = globalCurrMillis;
		}
		
		
		
		// A button activated if statement for debugging
		if (DebugFlag)
		{
					
			DebugFlag = ~DebugFlag;
		}	
		
		
		
		if (AdcFlag)
		{
			AdcFlag = FALSE;
			sensors_read_adc();
			
		}
		if (RxFlag)
		{
			RxFlag = FALSE;
			uart_rx_read_buffer();
		}
		

	}
	return 0;
}













