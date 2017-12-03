/*
 * sensors.h
 *
 * Created: 12.9.2017 10:07:33
 *  Author: rikardur
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

#include "global.h"




// Lookup tables for coolant temp, intake air temp, air/fuel ratio
volatile uint8_t CltAdc_LUT[LUT_SIZE];
volatile uint8_t IatAdc_LUT[LUT_SIZE];
volatile uint8_t AfrAdc_LUT[LUT_SIZE];

// Function to read from the EEPROM
void sensors_init(void);

// Read values from ADC and convert them to the appropriate unit
void sensors_read_adc(void);


uint16_t sensors_read_temp_sensors(uint8_t channel);

#endif /* SENSORS_H_ */