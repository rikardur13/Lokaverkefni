/*
 * decoders.c
 *
 * Created: 27.9.2017 18:59:30
 *  Author: rikardur
 */ 


#include "decoders.h"


void decoders_crank_primary(void)
{
	if (CrankTooth == FIRST_TACH_TOOTH)					// First tach interval for timing calculations of cylinders 1 and 4
	{		
		
		IgnDegree = math_interpolation_array(engine_realtime.Rpm, engine_realtime.Map, &IGN, 1);
		
		IgnTeethToSkipFirstTach = igncalc_ign_time_teeth(IgnDegree);
		if (IgnDegree == 0 | IgnDegree == 150 | IgnDegree == 300 | IgnDegree == 450 )
		{
			IgnInterval = TimerCorrection;
		}
		else
		{
			IgnInterval = igncalc_ign_time_interval(IgnDegree, IgnTeethToSkipFirstTach);
		}
		
		IgnToothFirstTach = FIRST_TACH_TOOTH - (CRANK_TEETH - IgnTeethToSkipFirstTach);
		IgnDwell = igncalc_ignition_dwell();
		
		
		InjClosingInterval = igncalc_ign_time_interval(InjClosingDegree, igncalc_ign_time_teeth(InjClosingDegree));
		InjPulseWidthCounts = math_us_to_counts(fuelcalc_pulsewidth()/10);
		
		
		
		IgnFirstRAValue = CrankCurrTriggerCounts + igncalc_counts_until_ign_start(IgnDwell, IgnTeethToSkipFirstTach);
		InjFirstRBValue = CrankCurrTriggerCounts + (CrankCurrCycleCounts * 2) - InjPulseWidthCounts - InjClosingInterval;
		
		
			
		if (CamSignal)									// First tach event with calculations for cylinder 1
		{
			timers_set_compareRA_value(TC0, 0, IgnFirstRAValue);
			timers_set_compareRB_value(TC0, 0, InjFirstRBValue);
			Inj1Status = TRUE;
			Ign1Status = TRUE;
		}
		else											// Third tach event with calculations for cylinder 4
		{
			timers_set_compareRA_value(TC1, 0, IgnFirstRAValue);
			timers_set_compareRB_value(TC1, 0, InjFirstRBValue);
			
			Ign4Status = TRUE;
			Inj4Status = TRUE;
		}
		
	}
	else if (CrankTooth == SECOND_TACH_TOOTH)			// Second tach interval for timing calculations of cylinders 3 and 2
	{
		IgnDegree = math_interpolation_array(engine_realtime.Rpm, engine_realtime.Map, &IGN, 1);
		
		IgnTeethToSkipSecondTach = igncalc_ign_time_teeth(IgnDegree);
		
		if (IgnDegree == 0 | IgnDegree == 150 | IgnDegree == 300 | IgnDegree == 450 )
		{
			IgnInterval = TimerCorrection;
		}
		else
		{
			IgnInterval = igncalc_ign_time_interval(IgnDegree, IgnTeethToSkipSecondTach);
		}
		
		IgnToothSecondTach = SECOND_TACH_TOOTH - (CRANK_TEETH - IgnTeethToSkipSecondTach);
		IgnDwell = igncalc_ignition_dwell();
		
		InjClosingInterval = igncalc_ign_time_interval(InjClosingDegree, igncalc_ign_time_teeth(InjClosingDegree));
		
		InjPulseWidthCounts = math_us_to_counts(fuelcalc_pulsewidth()/10);
		
		IgnSecondRAValue = CrankCurrTriggerCounts + igncalc_counts_until_ign_start(IgnDwell, IgnTeethToSkipSecondTach);
		InjSecondRBValue = CrankCurrTriggerCounts + (CrankCurrCycleCounts * 2) - InjPulseWidthCounts - InjClosingInterval;
				
		if (CamSignal)									// Second tach event with calculations for cylinder 2
		{
			
			timers_set_compareRA_value(TC0, 1, IgnSecondRAValue);
			timers_set_compareRB_value(TC0, 1, InjSecondRBValue);
			Ign2Status = TRUE;
			Inj2Status = TRUE;
			
			CamSignal = FALSE;							
		}
		else											// Fourth tach event with calculations for cylinder 3
		{
			timers_set_compareRA_value(TC0, 2, IgnSecondRAValue);
			timers_set_compareRB_value(TC0, 2, InjSecondRBValue);
			
			Ign3Status = TRUE;
			Inj3Status = TRUE;
		}
		engine_realtime.DegAdvance = IgnDegree;
		
	}
	
	
	
}






void decoders_rpm_calculation(void)
{
	engine_realtime.Rpm = (((GLOBAL_TIMER_FREQ * 100) / CrankCurrCycleCounts) * 6) / 10;
}