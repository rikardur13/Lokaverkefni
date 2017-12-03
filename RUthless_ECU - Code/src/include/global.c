/*
 * global.c
 *
 * Created: 5.9.2017 15:12:47
 *  Author: rikardur
 */ 

#include "global.h"


void global_init(void)
{
	
	/*eeprom_init_variables();*/
		
	CrankTooth = 0;
	globalCurrMillis = 0;
	CrankSignal = FALSE;
	CamSignal = FALSE;
	
	CrankCurrTriggerCounts = 0;
	CrankPrevTriggerCounts = 0;
	CrankCurrToothGapCounts = 0;
	CrankPrevToothGapCounts = 0;
	CrankCurrCycleCounts = 0;
	CrankPrevCycleCounts = 0;
	CrankCycleCounter = 0;
	CrankEngineRunning = 0;
	

	
	
// 	InjClosingDegree = eeprom_read_byte(EEPROM_INJ_CLOSING_TIME_INDEX);												// 50 = 5.0° BTDC
// 	InjOpeningTime = eeprom_read_byte(EEPROM_INJ_OPENING_TIME_INDEX);												// 130 = 1.3ms
	InjClosingDegree = 50;
	InjOpeningTime = 130;
	
		
	InjOpeningCounts = math_ms_to_counts(InjOpeningTime * 10);
	// InjToothFirstTach = FIRST_TACH_TOOTH - (CRANK_TEETH - igncalc_ign_time_teeth(InjClosingDegree));
	// InjToothSecondTach = SECOND_TACH_TOOTH - (CRANK_TEETH - igncalc_ign_time_teeth(InjClosingDegree));
	InjToothFirstTach = 6;
	InjToothSecondTach = 18;
	
	
	IgnDegree = math_interpolation_array(engine_realtime.Rpm, engine_realtime.Map, &IGN, 1);
	
	IgnTeethToSkipFirstTach = 0;
	IgnTeethToSkipSecondTach = 0;
	IgnInterval = 0;
	
	IgnFirstRAValue = 0;
	InjFirstRBValue = 0;
	IgnSecondRAValue = 0;
	InjSecondRBValue = 0;
	
	IgnStatus = TRUE;
	Ign1Status = FALSE;
	Ign2Status = FALSE;
	Ign3Status = FALSE;
	Ign4Status = FALSE;
	
	Inj1Status = FALSE;
	Inj2Status = FALSE;
	Inj3Status = FALSE;
	Inj4Status = FALSE;	
	
	TimerCorrection = 10;
	
	
	engine_realtime.SquirtStatus = TRUE;
	
}



global_init_input(void)
{
	
}


void global_init_output(void)
{
	/* Enable simple pin registers */
	PMC->PMC_PCER0	=	(1 << ID_PIOC);			// Peripheral clock enable register
	// Injector outputs
	PIOC->PIO_PER	=	INJ1_OUT | INJ2_OUT | INJ3_OUT | INJ4_OUT;				// PIO Enable Register			-	Enables the pins
	PIOC->PIO_OER	=	INJ1_OUT | INJ2_OUT | INJ3_OUT | INJ4_OUT;				// Output Enable Register		-	Set pins as output
	PIOC->PIO_PUER	=	INJ1_OUT | INJ2_OUT | INJ3_OUT | INJ4_OUT;				// Pull-up enable register		-	Enables pull-up resistor to 3.3V
	
	// Ignition outputs
	PIOC->PIO_PER	=	IGN1_OUT | IGN2_OUT | IGN3_OUT | IGN4_OUT;				// PIO Enable Register			-	Enables the pins
	PIOC->PIO_OER	=	IGN1_OUT | IGN2_OUT | IGN3_OUT | IGN4_OUT;				// Output Enable Register		-	Set pins as output
	PIOC->PIO_PUER	=	IGN1_OUT | IGN2_OUT | IGN3_OUT | IGN4_OUT;				// Pull-up enable register		-	Enables pull-up resistor to 3.3V
	
	// RPM output
	PIOC->PIO_PER	=	RPM_OUT;												// PIO Enable Register			-	Enables the pins
	PIOC->PIO_OER	=	RPM_OUT;												// Output Enable Register		-	Set pins as output
	PIOC->PIO_PUER	=	RPM_OUT;												// Pull-up enable register		-	Enables pull-up resistor to 3.3V
	
	
	// Set output pins to high
	PIOC->PIO_SODR	=	IGN1_OUT | IGN2_OUT | IGN3_OUT | IGN4_OUT | INJ1_OUT | INJ2_OUT | INJ3_OUT | INJ4_OUT | RPM_OUT;
	
}





/* Time delay function of unspecified amount of time */
/* 10,64 = 1ms */
void global_waste_of_time_delay(uint32_t delay)
{
	for (uint32_t i = 0; i < delay; i++)
	{
		for (uint32_t j = 0; j < 1978; j++)
		{
			__asm__("nop");					// Assembly code that makes the processor do nothing
		}
	}
}





void global_toggle_output_pin(uint32_t pin)
{
	// All output pins are in port C
	uint32_t status =	PIOC->PIO_ODSR;			// Store the status on pins in port B

	//	Toggle output pin
	if (status & pin)					// AND the status on pins in port B and the status of pin
	{
		PIOC->PIO_CODR	=	pin;			// If pin is high,	Clear Output Data Register	-	Sets pin to low
	}
	else
	{
		PIOC->PIO_SODR	=	pin;			// If pin is low,	Set Output Data Register	-	Sets pin to high
	}
}
