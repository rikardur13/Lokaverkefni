/*
 * storage.h
 *
 * Created: 11.9.2017 14:33:54
 *  Author: rikardur
 */ 


#ifndef STORAGE_H_
#define STORAGE_H_

#include "global.h"

/************************************************************************/
/* Functions:                                                           */
/************************************************************************/
// Read the EEPROM and store the according byte in ascending order
void storage_struct_read_eeprom_init(uint8_t *ConfigStructPointer, uint16_t ConfigLen, uint16_t EepromIndex);
// Initialize struct to zero
void storage_init_struct_to_zero(uint8_t *ConfigStructPointer, uint16_t ConfigLen);
// Initialize variables
void storage_init(void);
/************************************************************************/
/* Realtime variables struct                                            */
/************************************************************************/
// Todo: Launch Control button pressed (ON/OFF)
// Todo: Current ignition advance from table
// Todo: Current vehicle speed (km/h)
// Todo: Current vehicle gear (R(-1), N(0), 1, 2, 3, 4, 5, 6)
// Todo: Required ignition spark timing (�)
// Todo: Required ignition coil charging time (�s)
struct engine_realtime_
{
	uint8_t dummy100;
	uint8_t SquirtStatus;
	uint8_t EngineStatus;		// Uses definitions in global.h
	uint8_t Dwell;
	uint16_t Map;				// Current kPa value of Manifold Absolute Pressure sensor
	uint8_t Iat;				// Current temperature value of Intake Air Temperature sensor (Offset by 40�C)
	uint8_t Clt;				// Current temperature value of CooLant Temperature sensor (Offset by 40�C)
	uint8_t TpsAdc;				// Current ADC value of TPS sensor 1 (0 to 2^8-1)
	uint8_t BattVolt;			// Current Voltage value of the Battery voltage (Offset by factor of 10, 12 V = 120)
	uint8_t Afr;				// Current AFR value (Offset by factor of 10, 120 AFR = 12.0 AFR)
	uint8_t EgoCorrection;
	uint8_t AirCorrection;
	uint8_t WarmupEnrich;
	uint16_t Rpm;				// Current engine speed (RPM)
	uint8_t AccelEnrich;
	uint8_t BaroCorrection;
	uint8_t GammaEnrich;
	uint8_t VeTarget;			// Current volumetric efficiency value from table
	uint8_t AfrTarget;			// Current air to fuel ratio from table
	uint8_t PulseWidth;			// Required injector duration (currently tenths of ms) Todo: change to microsecs
	uint8_t TpsDot;				// TPS %/s
	uint8_t DegAdvance;
	uint8_t Tps;				// 0-100% value of TPS sensor 1
	uint8_t BattCorrection;
	uint8_t SparkStatus;
	uint8_t Afr2;
	int16_t RpmDot;
	uint8_t Errors;
	uint8_t TwiFault;
	uint8_t LastTps;			// Last ADC value of TPS sensor 1 (0 to 2^8-1)
	uint8_t Seconds;
};
/************************************************************************/
/* Configuration structs                                                */
/************************************************************************/
struct engine_config2_
{
	uint8_t	AfterStartEnrichPct;					// Afterstart enrichment [%]
	uint8_t	AfterStartEnrichCycles;					// Afterstart enrichment cycles. This is the number of ignition cycles that the afterstart enrichment % lasts for
	uint8_t	WarmUpEnrichPct[10];					// Warmup enrichment array [%]
	uint8_t	WarmUpEnrichTemp[10];					// Warmup Enrichment bins [�C]
	uint8_t	CrankingEnrichPct;						// Cranking enrichment [%]
	uint8_t	CrankingRpm;							// RPM below which the engine is considered to be cranking
	uint8_t	TpsFlood;								// TPS value that triggers flood clear mode (No fuel whilst cranking)
	uint8_t	PinLayout;								// Todo: Pin Mapping version
	uint8_t	TachoPin	: 6;						// Todo: Custom pin setting for tacho output (6 bits)
	uint8_t	unused26	: 2;						// Unused (2 bits)
	uint8_t	TpsDecelEnrichPct;						// Todo: TPS Deceleration enrichment [%]
	uint8_t	TpsColdAccelEnrichPct;					// Todo: Cold Accel Enrichment [%]
	uint8_t	TpsThres;								// TPS value threshold [%/s]
	uint8_t	TpsAccelTime;							// TPS duration of the acceleration enrichment[ms]
	uint8_t display		: 3;						// Todo
	uint8_t display1	: 3;						// Todo
	uint8_t display2	: 2;						// Todo
	uint8_t display3	: 3;						// Todo
	uint8_t display4	: 2;						// Todo
	uint8_t display5	: 3;						// Todo
	uint8_t displayB1	: 4;						// Todo
	uint8_t displayB2	: 4;						// Todo
	uint8_t ReqFuel;								// Todo: DECIDE WHETHER TO USE THIS VARIABLE [ms*10]
	uint8_t unused35;
	uint8_t Alternate	: 1;						// Todo
	uint8_t	unused36	: 7;
	uint8_t injOpen;								// Injector opening time [ms*10]
	// 	uint16_t Inj1Ang;								// Injector squirt closing angle [� 0-360]
	// 	uint16_t Inj2Ang;								// ...
	// 	uint16_t Inj3Ang;								// ...
	// 	uint16_t Inj4Ang;								// ...
	// 	uint16_t Inj5Ang;								// ...
	// 	uint16_t Inj6Ang;								// ...
	// 	uint16_t Inj7Ang;								// ...
	// 	uint16_t Inj8Ang;								// ...
	uint16_t InjAng[8];								// Injector squirt closing angle [� 0-360] NEEDS TO BE TESTED
	// Config 1
	uint8_t	MapSample	: 2;						// Todo: "Instantaneous", "Cycle Average", "Cycle Minimum"
	uint8_t TwoStroke	: 1;						// Todo: "Four-stroke", "Two-stroke"
	uint8_t InjType		: 1;						// Todo: "Port", "Throttle Body"
	uint8_t NrCylinders	: 4;						// "INVALID","1","2","3","4","5","6","INVALID","8","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID"
	// Config 2
	uint8_t Unused55	: 4;
	uint8_t	NrInjectors	: 4;						// "INVALID","1","2","3","4","5","6","INVALID","8","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID"
	// Config 3
	uint8_t EngineType	: 1;						// Todo: "Even fire",     "Odd fire"
	uint8_t Unused56b	: 1;
	uint8_t Algorithm	: 1;						// Todo: "Speed Density", "Alpha-N"
	uint8_t BaroCorr	: 1;						// Todo: "Off",           "On"
	uint8_t InjLayout	: 2;						// Todo: "Paired", "Semi-Sequential", "INVALID", "Sequential"
	uint8_t CanEnable	: 1;						// Todo: "Disable", "Enable"
	uint8_t Unused56h	: 1;
	
	uint8_t PrimePulse;								// Todo: Priming Pulsewidth
	uint8_t DutyLim;								// Injector duty cycle limit
	uint8_t TpsMin;									// Current Min ADC value of TPS sensor 1 (for calibration)
	uint8_t TpsMax;									// Current Max ADC value of TPS sensor 1 (for calibration)
	uint8_t MapMin;									// Current Min KPA value of MAP sensor (for calibration)
	uint16_t MapMax;								// Current Max KPA value of MAP sensor (for calibration)
	uint8_t Divider;								// Not sure what this parameter is but it is required by tunerstudio
	uint8_t FuelPumpPrime;							// Todo: Fuel pump prime duration
	uint8_t FiringOrder[8];							// One byte represents each cylinder index 0 is the first cylinder to fire, typically cyl #1 and so on
};

// Primarily ignition related configuration
struct engine_config4_
{
	int16_t TriggerAngle;							// Angle from TDC to trigger wheel tooth number 1 [�]
	uint8_t FixedAngle;								// Fixed translation angle to fire [�]
	uint8_t CrankingAngle;							// Cranking advance angle (THIS WILL NEED TO BE CHECKED LATER ALSO SEE .ini, because this is advance angle, maybe it needs to be minus as in retard ??) [�]
	uint8_t TriggerAngleMultiplier;					// Multiplier for tooth counts that don't evenly divide into 360

	uint8_t TriggerEdge			: 1;				// Trigger cranksignal polarity "Falling", "Rising"
	uint8_t TriggerSpeed		: 1;				// Todo: "Crank Speed", "Cam Speed"
	uint8_t IgnitionInverse		: 1;				// Polarity of ignition coils "Going Low",        "Going High"
	uint8_t Oddfire				: 1;				// Todo: "No",        "Yes"
	uint8_t TriggerPattern		: 4;				// Todo: "Missing Tooth", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
	
	uint8_t SecondTriggerEdge	: 1;				// Trigger camsignal polarity "Falling", "Rising"
	uint8_t SecondTriggerPolar	: 1;				// Trigger camsignal polarity High/low at first cylinder
	uint8_t FuelPumpPin			: 6;				// Todo: "Board Default", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"

	
	uint8_t unused7;
	uint8_t IdleAdvanceRpm;							// Todo [RPM]
	uint8_t IdleAdvanceClt;							// Todo [�C]
	uint8_t IdleDelayTime;							// Todo [sec]
	uint8_t StgCycles;								// Todo: The number of revolutions that will be skipped during cranking before the injectors and coils are fired
	// Dwell control
	uint8_t DwellEnable			: 1;				// Fixed duty dwell control "INVALID",  "Dwell control"
	uint8_t DwellLimitEnable	: 1;				// Whether the dwell limiter is "Off",		"On"
	uint8_t SparkOutputMode		: 1;				// "Wasted Spark", "Single Channel",   "Wasted COP", "Sequential"
	uint8_t DfcoEnable			: 1;				// Todo: Decelleration fuel cut off
	uint8_t TriggerFilter		: 2;				// Todo: "Off", "Weak", "Medium", "Aggressive"
	uint8_t IgnCrankLock		: 1;				// Fix cranking timing with trigger "Off",  "On"
	//13 byrja � 1
	uint8_t DwellTimeCranking;						// Dwell time whilst cranking [ms*10]
	uint8_t DwellTimeRunning;						// Dwell time whilst running [ms*10]
	uint8_t TriggerTeethCount;						// The full count of teeth on the trigger wheel if there were no gaps [teeth]
	uint8_t MissingTeethCount;						// The size of the tooth gap (number of missing teeth)
	uint8_t Unused17;
	uint8_t Unused18;
	uint8_t SoftRevLimit;							// Ignition cut ?, [RPM/100]
	uint8_t SoftLimRetard;							// Todo: retard ignition, degrees
	uint8_t SoftLimMax;								// Todo: Time the soft limit can run
	uint8_t HardRevLimit;							// Fuel cut ?, [RPM/100]
	uint8_t TpsAccelEnrichBins[4];					// TPS based acceleration enrichment bins [%/s]
	uint8_t TpsAccelEnrichValues[4];				// TPS based acceleration enrichment rates (% to add)
	uint8_t DwellLimit;								// Dwell max limit [ms]
	uint8_t DwellCorrectionValues[6];				// Correction table for dwell vs battery voltage (See config 6 BattRefVoltBins)
	// 38
	uint8_t IatRetardBins[6];						// Todo: Inlet Air Temp timing retard curve bins [C]
	uint8_t IatRetardValues[6];						// Todo: Inlet Air Temp timing retard curve values [deg]
	uint8_t DfcoRpm;								// Todo: RPM at which DFCO turns off/on at
	uint8_t	DfcoHyster;								// Todo: Hysteris RPM for DFCO
	uint8_t DfcoTpsThresh;							// Todo: TPS must be below this figure for DFCO to engage
	uint8_t Stoich;									// Soichimetric AFR [0.1-25.5]  55
	uint16_t Oddfire2;								// Todo: "The ATDC angle of channel 2 for oddfire engines. This is relative to the TDC angle of channel 1"
	uint16_t Oddfire3;								// ...
	uint16_t Oddfire4;								// ...
	uint8_t Unused60;
	uint8_t Unused61;
	uint8_t Unused62;
	uint8_t Unused63;
};

// Primarily Air to fuel ratio related configuration
struct engine_config6_
{
	uint8_t EgoAlgorithm	: 2;					// Todo: "Simple", "INVALID", "PID", "No correction"
	uint8_t EgoType			: 2;					// Todo: "Disabled", "Narrow Band", "Wide Band", "INVALID"
	uint8_t BoostEnabled	: 1;					// Todo: "Off", "On"
	uint8_t VvtEnabled		: 1;					// Todo: "Off", "On"
	uint8_t BoostCutType	: 2;					// Todo: "Off", "Spark Only", "Fuel Only","Both"
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
	uint8_t BattRefVoltBins[6];						// Bins for the battery reference voltage
	uint8_t InjBattRates[6];						// Values for injector pulsewidth vs voltage
	uint8_t AirDensityBins[9];						// Todo: Bins for the air density correction curve
	uint8_t AirDenRates[9];							// Todo: Values for the air density correction curve
	// PWM frequencies
	uint8_t BoostFreq;								// Todo: Frequency of the boost PWM valve
	uint8_t VvtFreq;								// Todo: Frequency of the vvt PWM valve
	uint8_t IdleFreq;								// Todo
	// Launch control
	uint8_t LaunchPin		: 6;					// Todo: "Board Default", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
	uint8_t LaunchEnable	: 1;					// Todo: "No",        "Yes"
	uint8_t LaunchHiLo		: 1;					// Todo: "LOW",        "HIGH"
	uint8_t LaunchSoftLimit;						// Todo: [RPM/100]
	uint8_t LaunchRetard;							// Todo: [�]
	uint8_t LaunchHardLimit;						// Todo: [RPM/100]
	uint8_t LaunchFuelAdd;							// Todo: [%]

	uint8_t IdleKP;									// Todo [%]
	uint8_t IdleKI;									// Todo [%]
	uint8_t IdleKD;									// Todo [%]
	uint8_t BoostLimit;								// Todo [kPa]
	uint8_t BoostKP;								// Todo [%]
	uint8_t BoostKI;								// Todo [%]
	uint8_t BoostKD;								// Todo [%]

	uint8_t LaunchPullResistor	: 2;				// Todo: "Float"  , "Pullup", "INVALID", "INVALID"
	uint8_t FuelTrimEnabled		: 1;				// Todo: "No",        "Yes"
	uint8_t Unused60			: 5;				//
	uint8_t Unused61;
	uint8_t Unused62;
	uint8_t Unused63;
};

// Primarily Air to fuel ratio related configuration
struct engine_config7_
{
	uint8_t IacCLValues[10];								// Todo: [RPM]
	uint8_t IacOLStepVal[10];								// Todo: [Steps]
	uint8_t IacOLPWMVal[10];								// Todo: [Duty %]
	uint8_t IacBins[10];									// Todo: [�C]
	uint8_t IacCrankSteps[4];								// Todo: [Steps]
	uint8_t	IacCrankDuty[4];								// Todo: [Duty %]
	uint8_t IacCrankBins[4];								// Todo: [�C]
	uint8_t IacAlgorithm		: 3;						// Todo: "None", "On/Off", "PWM Open loop", "PWM Closed loop", "Stepper", "INVALID", "INVALID", "INVALID"
	uint8_t IacStepTime			: 3;						// Todo: "1", "2", "3", "4", "5", "6"
	uint8_t IacChannels			: 1;						// Todo: "1", "2"
	uint8_t IacPWMDir			: 1;						// Todo: "Normal", "Reverse"
	uint8_t IacFastTemp;									// Todo: [�C]
	uint8_t IacStepHome;									// Todo: [Steps]
	uint8_t IacStepHyster;									// Todo: [Steps]

	// Fan control variables
	uint8_t FanInverse			: 1;						// Todo: "No",        "Yes"
	uint8_t FanEnable			: 1;						// Todo: "Off",       "On/Off"
	uint8_t FanPin				: 6;						// Todo: "Board Default", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
	uint8_t FanSP;											// Todo: [�C]
	uint8_t FanHyster;										// Todo: [�C]
	uint8_t FanFreq;										// Todo: [Hz]
	uint8_t FanPWMBins[4];									// Todo: [�C]
};

struct engine_config8_
{
	uint8_t BoostTable[8][8];								// Todo
	uint8_t RpmBinsBoost[8];								// Todo
	uint8_t TpsBinsBoost[8];								// Todo
	uint8_t VvtTable[8][8];									// Todo
	uint8_t RpmBinsVvt[8];									// Todo
	uint8_t TpsBinsVvt[8];									// Todo
};

struct engine_config9_
{
	uint8_t FuelTrim1Table[6][6];							// Todo
	uint8_t FuelTrim1rpmBins[6];							// Todo
	uint8_t FuelTrim1loadBins[6];							// Todo

	uint8_t FuelTrim2Table[6][6];							// Todo
	uint8_t FuelTrim2rpmBins[6];							// Todo
	uint8_t FuelTrim2loadBins[6];							// Todo

	uint8_t FuelTrim3Table[6][6];							// Todo
	uint8_t FuelTrim3rpmBins[6];							// Todo
	uint8_t FuelTrim3loadBins[6];							// Todo

	uint8_t FuelTrim4Table[6][6];							// Todo
	uint8_t FuelTrim4rpmBins[6];							// Todo
	uint8_t FuelTrim4loadBins[6];							// Todo
};




#endif /* STORAGE_H_ */