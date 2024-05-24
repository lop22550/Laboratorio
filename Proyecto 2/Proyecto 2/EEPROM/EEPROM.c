/*
 * EEPROM.c
 *
 * Created: 20/05/2024 21:34:51
 *  Author: bjosu
 */ 

#include "EEPROM.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

uint8_t valor_ADC1 = 0;		
uint8_t valor_ADC2 = 0;
uint8_t valor_ADC3 = 0;
uint8_t valor_ADC4 = 0;

uint8_t flagw_pos1 = 0;
uint8_t flagw_pos2 = 0;
uint8_t flagw_pos3 = 0;
uint8_t flagw_pos4 = 0;

uint8_t flagr_pos1 = 0;
uint8_t flagr_pos2 = 0;
uint8_t flagr_pos3 = 0;
uint8_t flagr_pos4 = 0;

uint8_t lectura_servo1_pos1 = 0;
uint8_t lectura_servo2_pos1 = 0;
uint8_t lectura_servo3_pos1 = 0;
uint8_t lectura_servo4_pos1 = 0;


void writeEEPROM (void){
	if (flagw_pos1 == 1)
	{
		eeprom_write_byte(( uint8_t*)0, valor_ADC1); 
		//eeprom_write_byte(( uint8_t*)1, valor_ADC2);
		//eeprom_write_byte(( uint8_t*)2, valor_ADC3);
		//eeprom_write_byte((uint8_t*)3, valor_ADC4);
		flagw_pos1 = 0; 
	}
	
/*	if (flagw_pos2 == 1)
	{
		eeprom_write_byte(1, valor_ADC2);
		flagw_pos2 = 0;
	}
	
	if (flagw_pos3 == 1)
	{
		eeprom_write_byte(2, valor_ADC3);
		flagw_pos3 = 0;
	}
	
	if (flagw_pos4 == 1)
	{
		eeprom_write_byte(3, valor_ADC4);
		flagw_pos4 = 0;
	}*/
}

void readEEPROM (void){
	if (flagr_pos1 == 1)
	{
		//writeUART(CERO + flagr_pos1);
		lectura_servo1_pos1 = eeprom_read_byte((uint8_t*) 0); 
		//ConvertASCII(lectura_servo1_pos1);
		//writeUART(CERO + flagw_pos1);
		//_delay_ms(5);
		OCR1A = updateDutyCycle1A(lectura_servo1_pos1);
		//updateDutyCycle1B();
		//updateDutyCycle2A();
		//updateDutyCycle2B();
		flagr_pos1 = 0;
	}
	
	
}