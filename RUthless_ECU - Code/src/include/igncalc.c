/*
 * igncalc.c
 *
 * Created: 30.9.2017 15:45:08
 *  Author: rikardur
 */ 



#include "igncalc.h"



// Calculate the amount of teeth to skip for the next ignition event
// Returns an integer to skip
// Example: Skip 7,4 teeth ---> the function returns the integer 7
uint16_t igncalc_ign_time_teeth(uint16_t ign_degree)
{
	uint32_t temp = ((CRANK_DEGREE_INTERVAL * 10 - ign_degree) * 1000) / CRANK_DEGREE_INTERVAL;
	uint32_t temp1 = (temp * (CRANK_TEETH/TACH_EVENTS))/10000;
	return	temp1;
}


// Calculate the time between 2 teeth
// Returns the counts to interval from the current tooth
// Example: Skip 7,4 teeth ---> the function uses the decimal number 0,4 to calculate the spark event
uint32_t igncalc_ign_time_interval(uint16_t ign_degree, uint8_t IgnTeethToSkip)
{
	uint32_t temp1 = (((CRANK_DEGREE_INTERVAL * 10 - ign_degree) * 1000) / CRANK_DEGREE_INTERVAL);
	temp1 = (temp1 * (CRANK_TEETH/TACH_EVENTS));
	temp1 = temp1 - (IgnTeethToSkip * 10000);
	temp1 = (temp1 * CrankCurrToothGapCounts)/10000;
	
	return temp1;
}

uint32_t igncalc_counts_until_ign_start(uint8_t IgnDwell, uint8_t IgnTeethToSkip)
{
	uint32_t DwellCounts = (GLOBAL_TIMER_FREQ * IgnDwell) / 10000;
	uint32_t temp = (IgnTeethToSkip * CrankPrevToothGapCounts) + IgnInterval;
	return temp - DwellCounts;
}


uint16_t igncalc_battery_dwell_correction(void)
{
	uint8_t voltage = engine_realtime.BattVolt;
	uint16_t temp = math_interpolation_vector(&engine_config6.BattRefVoltBins, &engine_config4.DwellCorrectionValues, voltage, 100, sizeof(engine_config4.DwellCorrectionValues));
	return temp;
}


uint8_t igncalc_ignition_dwell(void)
{
	uint16_t Correction = igncalc_battery_dwell_correction();
	uint8_t TotalDwell;
	
	if (engine_realtime.Rpm < RPM_CRANKING)
	{
		return IGN_DWELL_CRANKING;
	}
	else
	{
		TotalDwell = (IGN_DWELL_RUNNING * Correction)/100;
		if (TotalDwell > IGN_DWELL_MAX)
		{
			return IGN_DWELL_MAX;
		}
	}
	engine_realtime.Dwell = TotalDwell;
	return TotalDwell;
}