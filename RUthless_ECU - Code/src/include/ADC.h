/*
 * ADC.h
 *
 * Created: 7.9.2017 13:00:56
 *  Author: rikardur
 */ 



#ifndef ADC_H_
#define ADC_H_

#include "include/global.h"
#include "interrupts.h"

#define ADC_CH0						0
#define ADC_CH1						1
#define ADC_CH2						2
#define ADC_CH3						3
#define ADC_CH4						4
#define ADC_CH5						5
#define ADC_CH6						6
#define ADC_CH7						7
#define ADC_CH8						8
#define ADC_CH9						9
#define ADC_CH10					10
#define ADC_CH11					11
#define ADC_CH12					12
#define ADC_CH13					13
#define ADC_CH14					14
#define ADC_CH15					15
// Channel 15 is temperature sensor



/* Define active ADC channels, used for interrupts */
#define NR_OF_ACTIVE_ADC_CHANNELS	6
#define MAX_NR_OF_ADC_CHANNELS		15

// ADC median digital filter configuration (remember to use median function in math.c)
#define ADC_MEDIAN_FILTER_LENGTH	3		// if this is more than 8 bit integer remember to configure the variable below
volatile uint8_t AdcMedianCounter[MAX_NR_OF_ADC_CHANNELS];

// Configure ADC resolution, Attention there is needed to configure registers to change it. This is used in calculations see math.c
#define ADC_RESOLUTION				10 // 2^12 ADC_12_BITS

// /* Array for the ADC channel numbers */
volatile uint8_t AdcChannels[NR_OF_ACTIVE_ADC_CHANNELS];
// volatile uint8_t AdcChannels[NR_OF_ACTIVE_ADC_CHANNELS];

/* Data array for the configured active ADC channels */
volatile uint16_t AdcData[MAX_NR_OF_ADC_CHANNELS][ADC_MEDIAN_FILTER_LENGTH];

/* Global flag to indicate new results from interrupt */
// NOT USED AT THE MOMENT, maybe unnecessary
volatile uint8_t AdcFlag;


void adc_initialize(void);
void adc_turn_on_channels(void);



#endif /* ADC_H_ */