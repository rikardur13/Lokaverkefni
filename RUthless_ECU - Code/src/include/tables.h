/*
 * tables.h
 *
 * Created: 24.1.2017 18:01:17
 *  Author: rikardur
 */ 


#ifndef TABLES_H_
#define TABLES_H_

#include "global.h"


/************************************************************************/
/* This file consists of all the tables(arrays) to configure or tune the
ECU                                                                     */
/************************************************************************/

struct Table3D
{
	uint8_t **Table;
	uint8_t *Xbin;
	uint8_t *Ybin;
	uint8_t Xsize;
	uint8_t Ysize;
	uint16_t Bytes; // sizeof(Table + Xbin + Ybin)
};

void tables_Table3D_alloc_mem(struct Table3D *Current, uint8_t Xsize, uint8_t Ysize);
void tables_Table3D_read_eeprom(struct Table3D *Current, uint16_t EepromIndex);
void tables_Table3D_init(struct Table3D *Current, uint8_t Xsize, uint8_t Ysize, uint16_t EepromIndex);

// Initialization function for arrays
// TODO: READ FROM EEPROM
void tables_init(void);


#endif /* TABLES_H_ */