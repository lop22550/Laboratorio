/*
 * InitPWM2.c
 *
 * Created: 7/05/2024 14:51:58
 *  Author: bjosu
 */ 

#include "InitPWM2.h"

void initPWM2(void){
	DDRB |= (1<<DDB3);	//Se está configurando el puerto OC1A como salida
	DDRD |= (1<<DDD3);
	
	TCCR2A = 0;
	TCCR2B = 0;
	
	TCCR2A |= (1<<COM2A1); // Configurando OC1A como No invertido
	TCCR2A |= (1<<WGM21)|(1<<WGM20); //Configurando Modo Fast PWM OCR2A
	TCCR2B |= (1<<WGM22)|(1<<CS22)|(1<<CS21)|(1<<CS20); //Configurando Modo Fast PWM OCR2A | Prescaler de 1024

	OCR2A = (MIN2 & 0xFF);
	OCR2B = (MIN2 & 0xFF);
}