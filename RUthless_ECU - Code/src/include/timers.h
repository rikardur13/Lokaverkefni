/*
 * timers.h
 *
 * Created: 24.1.2017 14:49:45
 *  Author: rikardur
 */ 

#include <asf.h>

#ifndef TIMERS_H_
#define TIMERS_H_

void timers_init1(void);
void timers_init2(void);
void timers_init3(void);
void timers_init4(void);
void timers_init9(void);
void timers_init(uint32_t TimerChannel, uint32_t TimerMode, uint32_t InterruptMode, uint8_t TimerInterruptPriority);

void timers_set_compareRA_value(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value);
void timers_set_compareRB_value(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value);
uint8_t TC8_Overflow;



/************************************************************************/
/* INTERRUPT PRIORITY QUEUE
	0. PIOA_Handler: Crank-, camshaft input
	1. TC0_Handler: Cylinder 1, Ign and Inj
	2. TC1_Handler: Cylinder 2, Ign and Inj
	3. TC2_Handler: Cylinder 3, Ign and Inj
	4. TC3_Handler: Cylinder 4, Ign and Inj
	5. TC4_Handler: Cylinder 5, Ign and Inj
	6. TC5_Handler: Cylinder 6, Ign and Inj
	7. TC6_Handler: Cylinder 7, Ign and Inj
	8. TC7_Handler: Cylinder 8, Ign and Inj
	9. TC8_Handler: Crank signal, cam signal, almenn klukka
	10. ADC_Handler: Analog to digital
	11. PIOD_Handler: Speed sensor input
	12. UOTGHS_Handler: USB comm.
	13. CAN0_Handler: Can bus comm.
	14. USART1_Handler: Telemetry
	15. TWI0_Handler: I2C
                                                                        */
/************************************************************************/


#endif /* TIMERS_H_ */