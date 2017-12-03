/*
 * igncalc.h
 *
 * Created: 30.9.2017 15:44:45
 *  Author: nanna
 */ 


#ifndef IGNCALC_H_
#define IGNCALC_H_

#include "global.h"

uint16_t igncalc_ign_time_teeth(uint16_t ign_degree);
uint32_t igncalc_ign_time_interval(uint16_t ign_degree, uint8_t IgnTeethToSkip);
uint32_t igncalc_counts_until_ign_start(uint8_t IgnDwell, uint8_t IgnTeethToSkip);
uint16_t igncalc_battery_dwell_correction(void);
uint8_t igncalc_ignition_dwell(void);


#endif /* IGNCALC_H_ */