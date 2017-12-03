/*
 * tables.c
 *
 * Created: 24.1.2017 18:01:05
 *  Author: rikardur
 */ 
#include "tables.h"





void tables_Table3D_alloc_mem(struct Table3D *Current, uint8_t Xsize, uint8_t Ysize)
{
	// LSD, lína svo dálkur (line then column)
	Current->Table = (uint8_t **)malloc(Ysize * sizeof(uint8_t*));
	
	for (uint8_t i = 0; i < Ysize; i++)
		Current->Table[i] = (uint8_t *)malloc(Xsize * sizeof(uint8_t));
	
	Current->Xbin = (uint8_t *)malloc(Xsize * sizeof(uint8_t));
	Current->Ybin = (uint8_t *)malloc(Ysize * sizeof(uint8_t));
	Current->Xsize = Xsize;
	Current->Ysize = Ysize;
	Current->Bytes = (uint16_t) Xsize * Ysize + Xsize + Ysize;
}





void tables_Table3D_read_eeprom(struct Table3D *Current, uint16_t EepromIndex)
{
	for (uint8_t i = 0; i < Current->Ysize; i++)
	{
		storage_struct_read_eeprom_init(Current->Table[i], Current->Xsize, EepromIndex);
		EepromIndex += Current->Xsize;
	}
	storage_struct_read_eeprom_init(Current->Xbin, Current->Xsize, EepromIndex);
	EepromIndex += Current->Xsize;
	storage_struct_read_eeprom_init(Current->Ybin, Current->Ysize, EepromIndex);
}





void tables_Table3D_init(struct Table3D *Current, uint8_t Xsize, uint8_t Ysize, uint16_t EepromIndex)
{
	tables_Table3D_alloc_mem(Current, Xsize, Ysize);
	tables_Table3D_read_eeprom(Current, EepromIndex);
}





// Initialization function for arrays
void tables_init(void)
{
	tables_Table3D_init(&VE, THREE_D_TABLE_SIZE, THREE_D_TABLE_SIZE, EEPROM_VE_INDEX);
	tables_Table3D_init(&AFR, THREE_D_TABLE_SIZE, THREE_D_TABLE_SIZE, EEPROM_AFR_INDEX);
	tables_Table3D_init(&IGN, THREE_D_TABLE_SIZE, THREE_D_TABLE_SIZE, EEPROM_IGN_INDEX);
	
	
	/************************************************************************/
	/* Initialize Coolant sensor vectors */
	/************************************************************************/
	TempSerialValues[0] = 11; TempSerialValues[1] = 14; TempSerialValues[2] = 18; TempSerialValues[3] = 23; TempSerialValues[4] = 29; 
	TempSerialValues[5] = 38; TempSerialValues[6] = 49; TempSerialValues[7] = 63; TempSerialValues[8] = 81; TempSerialValues[9] = 101; 
	TempSerialValues[10] = 125; TempSerialValues[11] = 151; TempSerialValues[12] = 175; TempSerialValues[13] = 198; TempSerialValues[14] = 216; 
	TempSerialValues[15] = 230; TempSerialValues[16] = 240; TempSerialValues[17] = 246; TempSerialValues[18] = 250; TempSerialValues[19] = 253; 
	
	TempTemperatureValues[0] = 190; TempTemperatureValues[1] = 180; TempTemperatureValues[2] = 170; TempTemperatureValues[3] = 160; TempTemperatureValues[4] = 150;
	TempTemperatureValues[5] = 140; TempTemperatureValues[6] = 130; TempTemperatureValues[7] = 120; TempTemperatureValues[8] = 110; TempTemperatureValues[9] = 100;
	TempTemperatureValues[10] = 90; TempTemperatureValues[11] = 80; TempTemperatureValues[12] = 70; TempTemperatureValues[13] = 60; TempTemperatureValues[14] = 50;
	TempTemperatureValues[15] = 40; TempTemperatureValues[16] = 40; TempTemperatureValues[17] = 40; TempTemperatureValues[18] = 40; TempTemperatureValues[19] = 40;

	
	
	
	
	/************************************************************************/
	/* Initialize Dwell Voltage correction vectors */
	/************************************************************************/
	engine_config6.BattRefVoltBins[0] = 70; engine_config6.BattRefVoltBins[1] = 80; engine_config6.BattRefVoltBins[2] = 100; engine_config6.BattRefVoltBins[3] = 120; engine_config6.BattRefVoltBins[4] = 140; engine_config6.BattRefVoltBins[5] = 160;
	engine_config4.DwellCorrectionValues[0] = 150; engine_config4.DwellCorrectionValues[1] = 130; engine_config4.DwellCorrectionValues[2] = 110; engine_config4.DwellCorrectionValues[3] = 100; engine_config4.DwellCorrectionValues[4] = 90; engine_config4.DwellCorrectionValues[5] = 80; 
	
	/************************************************************************/
	/* Warmup Enrichment vector */ 
	/************************************************************************/
	engine_config2.WarmUpEnrichTemp[0] = 0; engine_config2.WarmUpEnrichTemp[1] = 20; engine_config2.WarmUpEnrichTemp[2] = 40; engine_config2.WarmUpEnrichTemp[3] = 60; engine_config2.WarmUpEnrichTemp[4] = 80; engine_config2.WarmUpEnrichTemp[5] = 100; engine_config2.WarmUpEnrichTemp[6] = 130; engine_config2.WarmUpEnrichTemp[7] = 150; engine_config2.WarmUpEnrichTemp[8] = 180; engine_config2.WarmUpEnrichTemp[9] = 210; 
	engine_config2.WarmUpEnrichPct[0] = 150; engine_config2.WarmUpEnrichPct[1] = 142; engine_config2.WarmUpEnrichPct[2] = 134; engine_config2.WarmUpEnrichPct[3] = 122; engine_config2.WarmUpEnrichPct[4] = 111; engine_config2.WarmUpEnrichPct[5] = 105; engine_config2.WarmUpEnrichPct[6] = 103; engine_config2.WarmUpEnrichPct[7] = 102; engine_config2.WarmUpEnrichPct[8] = 101; engine_config2.WarmUpEnrichPct[9] = 100; 

}