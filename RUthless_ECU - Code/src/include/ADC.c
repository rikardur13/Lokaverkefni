/*
 * ADC.c
 *
 * Created: 7.9.2017 13:01:10
 *  Author: rikardur
 */ 

#include "include/ADC.h"





// Initialize function for the ADC
void adc_initialize(void)
{
	pmc_enable_periph_clk(ID_ADC);
	adc_init(ADC, sysclk_get_main_hz(), ADC_FREQ_MIN, ADC_STARTUP_TIME_4);
	adc_configure_timing(ADC, 1, ADC_SETTLING_TIME_3, 1);
	adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_10);
	adc_configure_trigger(ADC, ADC_TRIG_SW, ADC_MR_FREERUN_OFF);
	adc_enable_tag(ADC);

	// Initialize data arrays
	for (uint8_t i = 0; i < MAX_NR_OF_ADC_CHANNELS; i++)
	{
		if (i < NR_OF_ACTIVE_ADC_CHANNELS)
			AdcChannels[i] = 0;
		for (uint16_t j = 0; j < ADC_MEDIAN_FILTER_LENGTH; j++)
			AdcData[i][j] = 0;
		AdcMedianCounter[i] = 0;
	}
}

#define ADC_VOLTAGE			PIO_PA2
#define ADC_BATTERY_CH		ADC_CH0
#define	ADC_O2_SENSOR		PIO_PA4
#define ADC_AFR_CH			ADC_CH2
#define ADC_AIR_TEMP		PIO_PA6
#define ADC_IAT_CH			ADC_CH3
#define ADC_COOLANT_TEMP	PIO_PA16
#define ADC_CLT_CH			ADC_CH7
#define ADC_TPS				PIO_PA22
#define ADC_TPS_CH			ADC_CH4
#define ADC_MAP				PIO_PA24
#define ADC_MAP_CH			ADC_CH6



void adc_turn_on_channels(void)
{
	uint8_t channel_number[NR_OF_ACTIVE_ADC_CHANNELS] = {ADC_CLT_CH, ADC_IAT_CH, ADC_AFR_CH, ADC_MAP_CH, ADC_TPS_CH, ADC_BATTERY_CH};
	for (uint8_t i = 0; i < NR_OF_ACTIVE_ADC_CHANNELS; i++)
	{
		adc_enable_channel(ADC, channel_number[i]);
	}
	
	adc_enable_interrupt(ADC, ADC_IER_DRDY);
	interrupts_enable_interrupt_vector(ADC_IRQn, ADC_PRIORITY);
}






void ADC_Handler(void)
{
	/************************************************************************/
	/* Register ADC_LCDR 
	15 14 13 12		11 10 9 8
	CHNB			LDATA
	7 6 5 4 3 2 1 0
	     LDATA   
	CHNB: Channel number
	LDATA: Last Data Converted */
	/************************************************************************/
	
	
	uint16_t Result = ADC->ADC_LCDR;																		//Last converted data register
	uint16_t ChannelNumber = ((Result & 0xF000) >> 12);	
	AdcData[ChannelNumber][AdcMedianCounter[ChannelNumber]] = (0x0FFF & Result);							// Discard channel number from results							
	AdcMedianCounter[ChannelNumber] = (AdcMedianCounter[ChannelNumber] + 1) % ADC_MEDIAN_FILTER_LENGTH;
	
	AdcFlag = TRUE;
}