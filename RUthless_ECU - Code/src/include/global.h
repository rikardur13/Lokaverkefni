/*
 * global.h
 *
 * Created: 5.9.2017 15:12:32
 *  Author: rikardur
 */ 



#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <asf.h>

/************************************************************************/
/* Global definitions:                                                  */
/************************************************************************/

#define FALSE						0
#define TRUE						1
#define MILLI_SEC					1000	// Factor of milliseconds

#define MAX_RPM_TABLE_LENGTH		8
#define MAX_LOAD_TABLE_LENGTH		8
#define RPM_CRANKING				500

#define CRANK_TEETH					24
#define CYLINDERS					4
#define TACH_EVENTS					1
#define CRANK_DEGREE_INTERVAL		360


#define FIRST_TACH_TOOTH			7
#define SECOND_TACH_TOOTH			19




#define TEMPERATURE_OFFSET			40		// IAT value from sensor - 40°C
#define AFR_REGULATION_OFFSET		5		// Offset for Fuel corrections regarding real AFR value and Target AFR value 5 = 0.5 afr
#define RPM_SCALER					100		// used for logging purposes
#define RPM_LIMIT					14000	// The Rpm limiter

#define CYL_DISPLACEMENT			600		// 600cc
#define NR_OF_INJECTORS				4		// How many injectors
#define INJECTOR_FLOW_RATE			197		// cc/min
#define GASOLINE_DENSITY			779		// kg/m^3  -----------  Ethanol = 789 ----- Gasoline = 719.7 ------ E85 = 778.605
#define GAS_CONSTANT				287		// J/(kg K)
#define FUEL_CONST					(CYL_DISPLACEMENT * 60 * 1000)/GASOLINE_DENSITY * 1000 / GAS_CONSTANT / NR_OF_INJECTORS * 1000 / INJECTOR_FLOW_RATE
#define TPS_FLOOD_CLEAR				80		// Means 80%, TODO: MAKE CONFIGURABLE IN THE FUTURE





/************************************************************************/
/* Struct definitions:                                                  */
/************************************************************************/


volatile struct engine_config2_ engine_config2;
volatile struct engine_config4_ engine_config4;
volatile struct engine_config6_ engine_config6;
volatile struct engine_config7_ engine_config7;
volatile struct engine_config8_ engine_config8;
volatile struct engine_config9_ engine_config9;
volatile struct engine_realtime_ engine_realtime;


/************************************************************************/
/* Table definitions:                                                   */
/************************************************************************/
volatile struct Table3D VE;
volatile struct Table3D AFR;
volatile struct Table3D IGN;

#define THREE_D_TABLE_SIZE			16	// 3D table size definitions
#define WARMUP_ENRICH_SIZE			10
#define ADC_TABLE_LENGTH			20
#define IGN_DWELL_VECTOR_SIZE		6

#define LUT_SIZE					1024

uint8_t TempSerialValues[ADC_TABLE_LENGTH];
uint8_t TempTemperatureValues[ADC_TABLE_LENGTH];


/************************************************************************/
/* Timer definitions:                                                   */
/************************************************************************/
#define CYLINDER_1_TIMER	0
#define CYLINDER_2_TIMER	1
#define CYLINDER_3_TIMER	2
#define CYLINDER_4_TIMER	3
#define GLOBAL_TIMER		8


uint16_t TimerCorrection;

#define GLOBAL_MCK					84000000
#define GLOBAL_TIMER_FREQ			2625000		// MCK/32 -> 84MHz / 32
#define GLOBAL_MILLI_SEC			1000
#define GLOBAL_MILLIS_SCALER		GLOBAL_TIMER_FREQ/MILLI_SEC
#define GLOBAL_FREQ_ADC_SCALER		50


uint32_t globalCurrMillis;								// Variable to keep track of running time in milliseconds
uint32_t globalPrevMillis;								// Compare value for triggering


/************************************************************************/
/* Nested vector interrupt priority definitions:                        */
/************************************************************************/
// Lower number means higher priority
#define PIOB_PRIORITY	0		// Crank-, camshaft input
#define TC0_PRIORITY	1		// Cylinder 1, Ignition and injection
#define TC1_PRIORITY	2		// Cylinder 2, Ignition and injection
#define TC2_PRIORITY	3		// Cylinder 3, Ignition and injection
#define TC3_PRIORITY	4		// Cylinder 4, Ignition and injection
#define TC4_PRIORITY	5		
#define TC5_PRIORITY	6		
#define TC6_PRIORITY	7		
#define TC7_PRIORITY	8		
#define TC8_PRIORITY	9		// General clock, not necessary, maybe put lower in the queue
#define ADC_PRIORITY	10		// Analog to digital conversion completed
#define PIOA_PRIORITY	11		// Neutral switch
#define UART_PRIORITY	12		// UART communication for tuning environment
#define UOTGHS_PRIORITY 13		// USB communication
#define CAN0_PRIORITY	14		// Can bus communication
#define USART1_PRIORITY 15		// Telemetry communication
#define TWI1_PRIORITY	16		// I2C EEPROM communication

/************************************************************************/
/* PIN DECLERATION														*/
/************************************************************************/

// Digital input pins
#define CAM_SIGNAL			PIO_PB21
#define	CRANK_SIGNAL		PIO_PB25
#define NEUTRAL_SWITCH		PIO_PA28

// Injector output pins 
#define INJ1_OUT			PIO_PC1
#define INJ2_OUT			PIO_PC3
#define INJ3_OUT			PIO_PC5
#define INJ4_OUT			PIO_PC7

// Ignition output pins
#define IGN1_OUT			PIO_PC19
#define IGN2_OUT			PIO_PC21
#define IGN3_OUT			PIO_PC23
#define IGN4_OUT			PIO_PC25

// Digital output pins
#define RPM_OUT				PIO_PC2

// Analog input pins
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



/************************************************************************/
/* HELPER FUNCTIONS														*/
/************************************************************************/

void global_init(void);
void global_init_input(void);
void global_init_output(void);
void global_waste_of_time_delay(uint32_t delay);	// Time delay
void global_toggle_output_pin(uint32_t pin);



/************************************************************************/
/* Interrupt Definitions												*/
/************************************************************************/

uint32_t	CrankCurrTriggerCounts;					// Counter value of the current interrupt
uint32_t	CrankPrevTriggerCounts;					// Counter value of the previous interrupt
uint32_t	CrankCurrToothGapCounts;				// Counts from previous tooth to current tooth
uint32_t	CrankPrevToothGapCounts;				//
uint32_t	CrankCurrCycleCounts;					//
uint32_t	CrankPrevCycleCounts;					//
uint32_t	CrankCycleCounter;

uint8_t		CrankTooth;								// Variable to keep track of the crank position
uint8_t		CrankSignal;							// Boolean variable to indicate if a crank signal has been triggered

uint8_t		CamSignal;

uint32_t	CrankEngineRunning;

/************************************************************************/
/* Ignition Definitions													*/
/************************************************************************/
#define IGN_DWELL_RUNNING		23
#define IGN_DWELL_CRANKING		35
#define IGN_DWELL_MAX			50

uint16_t	IgnDegree;	
		

uint8_t		IgnDwell;
uint32_t	IgnDwellCounts;


uint8_t		IgnTeethToSkipFirstTach;				// Calculated teeth to skip before ignition is triggered					-	see igncalc.c
uint8_t		IgnTeethToSkipSecondTach;
uint32_t	IgnInterval;							// Calculated ignition counts between IgnTeethToSkip and the next tooth		-	see igncalc.c

uint8_t		IgnToothFirstTach;
uint8_t		IgnToothSecondTach;

uint32_t	IgnFirstRAValue;
uint32_t	InjFirstRBValue;
uint32_t	IgnSecondRAValue;
uint32_t	InjSecondRBValue;

uint8_t		IgnStatus;

uint8_t		Ign1Status;
uint8_t		Ign2Status;
uint8_t		Ign3Status;
uint8_t		Ign4Status;

uint8_t		Ign1Wait;
uint8_t		Ign2Wait;
uint8_t		Ign3Wait;
uint8_t		Ign4Wait;				





/************************************************************************/
/* Injection Definitions												*/
/************************************************************************/

uint8_t InjClosingDegree;												// The degree at which the injector closes ----- eeprom.c

uint8_t InjToothFirstTach;
uint8_t InjToothSecondTach;

uint32_t InjClosingInterval;
uint32_t InjPulseWidthCounts;

uint8_t Inj1Status;
uint8_t Inj2Status;
uint8_t Inj3Status;
uint8_t Inj4Status;

uint8_t	 InjOpeningTime;												// Injector latency time in ms*100 ---> 130 = 1.3ms
uint32_t InjOpeningCounts;												// Injector latency time in counts


/************************************************************************/
/* TEST VARIABLES														*/
/************************************************************************/
		
uint8_t DebugFlag;





#include "math.h"
#include "interrupts.h"
#include "uart.h"
#include "timers.h"
#include "ADC.h"
#include "tables.h"
#include "storage.h"
#include "decoders.h"
#include "eeprom.h"

#endif /* GLOBAL_H_ */