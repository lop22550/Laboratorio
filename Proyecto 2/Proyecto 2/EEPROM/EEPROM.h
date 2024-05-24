/*
 * EEPROM.h
 *
 * Created: 20/05/2024 21:34:35
 *  Author: bjosu
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

void writeEEPROM (void);
void readEEPROM (void);


#endif /* EEPROM_H_ */