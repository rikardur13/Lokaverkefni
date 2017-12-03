/*
 * storage.c
 *
 * Created: 11.9.2017 14:33:42
 *  Author: rikardur
 */ 

#include "storage.h"

void storage_init(void)
{
	engine_realtime.EgoCorrection = 100;
	engine_realtime.EngineStatus = FALSE;
	engine_config2.DutyLim = 80;										// Max duty cycle of injector pulse width
	engine_config2.CrankingRpm = 8;										// Scaled to 800 with RPM_SCALER
	engine_config2.AfterStartEnrichCycles = 100;						// 500 cycles are about 30 seconds at 1000rpm
	engine_config2.AfterStartEnrichPct = 120;							// Enrich the fuel mixture by % after cranking
	engine_config2.CrankingEnrichPct = 110;								// Enrich the fuel mixture by % when cranking
	engine_config2.TpsFlood = 80;
	
}


// Read the EEPROM and store the according byte in ascending order
void storage_struct_read_eeprom_init(uint8_t *ConfigStructPointer, uint16_t ConfigLen, uint16_t EepromIndex)
{
	at24cxx_read_continuous(EepromIndex, ConfigLen, ConfigStructPointer);
}





void storage_init_struct_to_zero(uint8_t *ConfigStructPointer, uint16_t ConfigLen)
{
	for (uint16_t i = 0; i < ConfigLen; i++)
	*(ConfigStructPointer + i) = 0;
}