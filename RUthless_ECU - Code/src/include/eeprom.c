/*
 * eeprom.c
 *
 * Created: 17.10.2017 11:22:33
 *  Author: rikardur
 */ 

#include "eeprom.h"

// Initialization function
void eeprom_init(void)
{	
	pmc_enable_periph_clk(ID_TWI1);				// Enable peripheral clock
	#define TWI_DATA_PIN PIO_PB12A_TWD1			// Define TWI data pin
	#define TWI_CLOCK_PIN PIO_PB13A_TWCK1		// Define TWI clock pin
	#define TWI_PERIPHERAL PIOB					// Define TWI peripheral (either A(TWI0) or B(TWI1))
	TWI_PERIPHERAL->PIO_IDR = TWI_DATA_PIN;		// Interrupt disable register
	TWI_PERIPHERAL->PIO_IDR = TWI_CLOCK_PIN;
	TWI_PERIPHERAL->PIO_PDR = TWI_DATA_PIN;		// Disable input/output which enables peripheral mode
	TWI_PERIPHERAL->PIO_PDR = TWI_CLOCK_PIN;
	TWI_PERIPHERAL->PIO_ABSR &= ~TWI_DATA_PIN;	// Enable Peripheral A (TWI)
	TWI_PERIPHERAL->PIO_ABSR &= ~TWI_CLOCK_PIN;
	TWI_PERIPHERAL->PIO_PUER = TWI_DATA_PIN;
	TWI_PERIPHERAL->PIO_PUER = TWI_CLOCK_PIN;
	// Initialize TWI
	twi_options_t opt;
	opt.master_clk = sysclk_get_cpu_hz(); opt.speed = 400000; // 400KHz clock frequency
	twi_master_init(TWI1, &opt);
}

// More advanced function to read from the EEPROM
// It basicly checks for TWI comm. success and retries until it enables a Fault
 uint8_t eeprom_read_byte(uint16_t address)
 {
	uint8_t result = 0;
	// Try to read specific number of times otherwise FAULT
	for (uint8_t i = 0; i < TWI_NUMBER_OF_TRIES; i++)
		if (!(at24cxx_read_byte(address, &result) & AT24C_READ_FAIL))
			return result;
	// Let know that there is a fault on the Two wire interface
	return 1; // 1 is the safest number regarding IGN, VE and AFR map
 }



// Read the congigurable variables stored on the EEPROM
void eeprom_read_config_variables(void)
{
	InjClosingDegree = eeprom_read_byte(EEPROM_INJ_CLOSING_TIME_INDEX);
	InjOpeningTime = eeprom_read_byte(EEPROM_INJ_OPENING_TIME_INDEX);
	InjOpeningCounts = math_ms_to_counts(InjOpeningTime * 10);
	InjToothFirstTach = FIRST_TACH_TOOTH - (CRANK_TEETH - igncalc_ign_time_teeth(InjClosingDegree));
	InjToothSecondTach = SECOND_TACH_TOOTH - (CRANK_TEETH - igncalc_ign_time_teeth(InjClosingDegree));
}



// Temporary function to write to the eeprom
void eeprom_init_variables(void)
{
	/************************************************************************/
	/* Initialize configuration variables */
	/************************************************************************/
	// at24cxx_write_byte returns FALSE if write procedure is successful, otherwise it returns TRUE
	// Possible to use this to see if there is a problem with the EEPROM
	at24cxx_write_byte(EEPROM_INJ_CLOSING_TIME_INDEX, 50);
	at24cxx_write_byte(EEPROM_INJ_OPENING_TIME_INDEX, 130);
	
	
	
	
// 	CltAdc_LUT[i]		= eeprom_read_byte(EEPROM_CLT_ADC_INDEX + i);
// 	IatAdc_LUT[i]		= eeprom_read_byte(EEPROM_IAT_ADC_INDEX + i);
// 	AfrAdc_LUT[i]		= eeprom_read_byte(EEPROM_AFR_ADC_INDEX + i);
	
}