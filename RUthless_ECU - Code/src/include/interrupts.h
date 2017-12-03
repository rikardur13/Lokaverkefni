/*
 * interrupts.h
 *
 * Created: 24.1.2017 14:50:42
 *  Author: rikardur
 */ 

#include <asf.h>
#include "global.h"
#include "math.h"

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

void interrupts_enable_interrupt_vector(uint32_t irqn, uint32_t priority);
void interrupts_init_regB(void);
void interrupts_init_regA(void);



#endif /* INTERRUPTS_H_ */