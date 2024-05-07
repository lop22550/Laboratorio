/*
 * InitPWM1.c
 *
 * Created: 7/05/2024 14:00:44
 *  Author: bjosu
 */ 

#include "InitPWM1.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

void initPWM1(void){
	DDRB |= (1<<PORTB1);	//Se está configurando el puerto OC1A como salida
	DDRB |= (1<<PORTB2);	//Se está configurando el puerto OC1B como salida
	
	TCCR1A = 0;
	TCCR1B = 0;
	
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11); // Configurando OC1A y OC1B como No invertido| Configurando Modo Fast PWM con ICR1 como TOP
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS12)|(1<<CS10); //Configurando Modo Fast PWM con ICR1 como TOP | Prescaler de 1024
	
	ICR1H = (PWM_TOP & 0xFF00)>>8;
	ICR1L = (PWM_TOP & 0x00FF);

	OCR1AH =(MIN1 & 0xFF00)>>8;
	OCR1AL = (MIN1 & 0x00FF);
}

uint8_t updateDutyCycle1A(uint8_t duty1){
	return (uint8_t)(((float)duty1/255)*(MAX1 - MIN1) + MIN1);
}

uint8_t updateDutyCycle1B(uint8_t duty2){
	return (uint8_t)(((float)duty2/255)*(MAX1 - MIN1) + MIN1);
}
