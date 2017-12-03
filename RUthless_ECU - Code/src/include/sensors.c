/*
 * sensors.c
 *
 * Created: 12.9.2017 10:07:21
 *  Author: rikardur
 */ 


#include "sensors.h"


// Function to read from the EEPROM
void sensors_init(void)
{
	for (uint16_t i = 0; i < LUT_SIZE; i++)
	{
		CltAdc_LUT[i]		= eeprom_read_byte(EEPROM_CLT_ADC_INDEX + i);
		IatAdc_LUT[i]		= eeprom_read_byte(EEPROM_IAT_ADC_INDEX + i);
		AfrAdc_LUT[i]		= eeprom_read_byte(EEPROM_AFR_ADC_INDEX + i);
	}
}

// Read values from ADC and convert them to the appropriate unit
void sensors_read_adc(void)
{
 	// TODO: TURN GLOBAL INTERRUPTS OFF	
	engine_realtime.Clt =  sensors_read_temp_sensors(ADC_CLT_CH);
	engine_realtime.Iat = sensors_read_temp_sensors(ADC_IAT_CH);
	
	
	
	engine_realtime.Afr = math_map(50, 120, math_find_median(AdcData[ADC_AFR_CH], ADC_MEDIAN_FILTER_LENGTH), 1529);			// From lambda value of 0.50 to 1.51
	engine_realtime.Map = math_map_adc(0, 120, math_find_median(AdcData[ADC_MAP_CH], ADC_MEDIAN_FILTER_LENGTH));			// Kpa values from 0 to 120 Kpa
	engine_realtime.TpsAdc = math_find_median(AdcData[ADC_TPS_CH], ADC_MEDIAN_FILTER_LENGTH) >> 2; // Change to 8 bit
	engine_realtime.Tps = math_map(0, 100, engine_realtime.TpsAdc, 255);													// From Tps % value of 0% to 100%
	engine_realtime.BattVolt = math_map_adc(0, 156, math_find_median(AdcData[ADC_BATTERY_CH], ADC_MEDIAN_FILTER_LENGTH));	// From battery voltage of 0V to 15.6V	
}


uint16_t sensors_read_temp_sensors(uint8_t channel)
{
	uint16_t serial =  math_find_median(AdcData[channel], ADC_MEDIAN_FILTER_LENGTH) >> 2;
	uint16_t temp = math_interpolation_vector(&TempSerialValues, &TempTemperatureValues, serial, 100, ADC_TABLE_LENGTH);
	return  temp - TEMPERATURE_OFFSET;
}

