/*
 * PWM1.c
 *
 * Created: 16/04/2024 21:57:53
 *  Author: bjosu
 */ 

#include "PWM1.h"

void initPWM1A (){
	DDRB |= (1<<PORTB1);	//Se está configurando el puerto OC1A como salida
	
	TCCR1A = 0;
	TCCR1B = 0;
	
	TCCR1A |= (1<<COM1A1); // Configurando OC1A como No invertido	
	TCCR1A |= (1<<WGM11); //Configurando Modo Fast PWM con ICR1 como TOP	
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS12)|(1<<CS10); //Configurando Modo Fast PWM con ICR1 como TOP | Prescaler de 1024		
	 
	ICR1H = (PWM_TOP & 0xFF00)>>8;
	ICR1L = (PWM_TOP & 0x00FF);

	OCR1AH =(MIN1 & 0xFF00)>>8;
	OCR1AL = (MIN1 & 0x00FF);	
}

void updateDutyCycleA1(uint8_t valor_ADC){
	//uint8_t Mappeo = (valor_ADC / 255)*(MAX-MIN)+MIN;
	uint8_t Mappeo1 = (valor_ADC / 6);
	uint8_t Senal_Servo1 = Mappeo1;

	OCR1AH = (Senal_Servo1 & 0xFF00)>>8;
	OCR1AL = (Senal_Servo1 & 0x00FF);	
	}



ISR (TIMER1_COMPA_vect){
	//
}

/*
ISR(TIMER1_OVF_vect){
	updatePWM = 0; //Hace overflow y se reg
}*/