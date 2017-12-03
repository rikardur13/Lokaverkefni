/*
 * fuelcalc.c
 *
 * Created: 9.10.2017 15:51:08
 *  Author: rikardur
 */ 



#include "fuelcalc.h"

uint32_t fuelcalc_pulsewidth(void)
{
	uint16_t Rpm = engine_realtime.Rpm;
	uint16_t Map = engine_realtime.Map;
	
	
	// int16_t Temperature = engine_realtime.Iat - TEMPERATURE_OFFSET;
	uint16_t Temperature = engine_realtime.Iat;
		
	uint16_t Ve = math_interpolation_array(Rpm, Map, &VE, 1);
	engine_realtime.VeTarget = Ve / 10;																	// 70% VE
	
	
	uint16_t Afr = math_interpolation_array(Rpm, Map, &AFR, 1);
	engine_realtime.AfrTarget = Afr / 10;																// 9.8 AFR ratio E85
		
	
	
	// Fuel equation:
	// Mair = (VE *  MAP * CYL_DISP) / (R (287 J/(Kg Kelvin)) * (IAT + 273))
	
// 	#define CYL_DISPLACEMENT			600		// 600cc
// 	#define NR_OF_INJECTORS				4		// How many injectors
// 	#define INJECTOR_FLOW_RATE			197		// cc/min
// 	#define GASOLINE_DENSITY			779		// kg/m^3  -----------  Ethanol = 789 ----- Gasoline = 719.7 ------ E85 = 778.605
// 	#define GAS_CONSTANT				287		// J/(kg K)
// 	#define FUEL_CONST					(CYL_DISPLACEMENT * 60 * 1000)/GASOLINE_DENSITY * 1000 / GAS_CONSTANT / NR_OF_INJECTORS * 1000 / INJECTOR_FLOW_RATE
// 	#define TPS_FLOOD_CLEAR				80		// Means 80%, TODO: MAKE CONFIGURABLE IN THE FUTURE


	// MAP is in kPa, required to change it to Pa (1000) AND AFR is * 10, 13.0 = 130 AND VE is /100 since it is in % 
	// InjectorTime is in 100 nano seconds, InjectorTime = 10 = 1µs
	uint64_t Numerator = (uint64_t)Map * Ve * FUEL_CONST; 
	uint32_t Denominator = (uint32_t)Afr * (Temperature + 273);  
	uint64_t InjectorTime = Numerator / Denominator;
	
	// Calculate enrichments
	uint16_t gammaEnrich = fuelcalc_GammaEnrich();
	
	InjectorTime = ((InjectorTime * gammaEnrich) / 100) + (InjOpeningTime * 100);
		
	// TODO: CALCULATE !!!  uint32_t CorrectedInjectorOpenTime = InjectorOpenTime * 
	uint64_t RevolutionTime = (CrankCurrCycleCounts * 100) / (GLOBAL_TIMER_FREQ / 1000); // SHOULD NOT BE CONSTANT HERE NOT PRETTY
	
	if (InjOpeningCounts > (RevolutionTime * engine_config2.DutyLim)) // Above injector duty cycle limit
	{	
		return 0;
	}
	engine_realtime.PulseWidth = InjectorTime / 1000;
	return (uint32_t)InjectorTime;
}

/************************************************************************/
/* Gamma Enrichment is multiplied together, it consists of:
Warmup Enrichment
Afterstart Enrichment
                                                                     */
/************************************************************************/
uint16_t fuelcalc_GammaEnrich(void)
{
	uint16_t TotalEnrich = 100;
	uint16_t CoolantTemperature = engine_realtime.Clt;
	
	// Calculate warm up enrichment
	TotalEnrich *= math_interpolation_vector(&engine_config2.WarmUpEnrichTemp ,&engine_config2.WarmUpEnrichPct, CoolantTemperature, 100, WARMUP_ENRICH_SIZE);
	TotalEnrich /= 100;	
	// Calculate after start enrichment
	if (CrankCycleCounter < (engine_config2.AfterStartEnrichCycles))
	{
		TotalEnrich *= engine_config2.AfterStartEnrichPct;
		TotalEnrich /= 100;
	}
	// Calculate cranking enrichment
	if (engine_config2.CrankingRpm * RPM_SCALER > engine_realtime.Rpm)
	{
		// TotalEnrich *= (100 + engine_config2.CrankingEnrichPct);
		TotalEnrich *= engine_config2.CrankingEnrichPct;
		TotalEnrich /= 100;
		// Reset CrankCycleCounter if the rpm is below cranking, used in afterstart enrichment
		CrankCycleCounter = 0;
	}
/*	
		engine_config6
		uint8_t EgoKP;									// Todo: Regulation parameter
		uint8_t EgoKI;									// Todo: Regulation parameter
		uint8_t EgoKD;									// Todo: Regulation parameter
		uint8_t EgoTemp;								// Todo: The temperature above which closed loop functions
		uint8_t EgoCount;								// Todo: Todo: The number of ignition cylces per step
		uint8_t EgoDelta;								// Todo: The step size (In %) when using simple algorithm
		uint8_t EgoLimit;								// Todo: Maximum amount the closed loop will vary the fueling
		uint8_t EgoMin;									// Todo: AFR must be above this for closed loop to function
		uint8_t EgoMax;									// Todo: AFR must be below this for closed loop to function
		uint8_t EgoSecDelay;							// Todo: Time in seconds after engine starts that closed loop becomes available
		uint8_t EgoRpm;									// Todo: RPM must be above this for closed loop to function
		uint8_t EgoTpsMax;								// Todo: TPS must be below this for closed loop to function
		uint8_t EgoLoadMax;								// Todo: Load (TPS or MAP) must be below this for closed loop to function
		uint8_t EgoLoadMin;								// Todo: Load (TPS or MAP) must be above this for closed loop to function
		
		engine_realtime.Afr
		engine_realtime.AfrTarget
*/
	
	if (engine_realtime.Afr < (engine_realtime.AfrTarget + AFR_REGULATION_OFFSET) | engine_realtime.AfrTarget > (engine_realtime.Afr - AFR_REGULATION_OFFSET))
	{
		TotalEnrich *= engine_config2.CrankingEnrichPct;
		TotalEnrich /= 100;
	}
	// Prevent overflow
	if (TotalEnrich > 255)
	{
		TotalEnrich = 255;
	}
	engine_realtime.GammaEnrich = TotalEnrich;
	return TotalEnrich;
}

