/* Universidad del Valle de Guatemala
 * Autor : Boris Josué López Moreira
 * IE2023 Programación de Microcontroladores
 * Proyecto:Proyecto 2 - Brazo robótico 
 * Hardware: ATMEGA328P
 * Creado: 28/04/2024 12:44:26
 
 */ 

#define  F_CPU 16000000UL

//Librerías
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "InitPWM1/InitPWM1.h"
#include "InitPWM2/InitPWM2.h"

//Variables globales 
uint8_t valor_ADC1 = 0; 
uint8_t valor_ADC2 = 0;
uint8_t valor_ADC3 = 0;
uint8_t valor_ADC4 = 0;
uint8_t Cambio = 0; 


//Prototipos de funciones
void InitADC (void);
void checkADC (void);


int main(void)
{
	InitADC(); 
	initPWM1();
	initPWM2();
    
    while (1) 
    {
		checkADC();
		OCR1A = updateDutyCycle1A(valor_ADC1); 
		OCR1B = updateDutyCycle1A(valor_ADC2); 
		OCR2A = updateDutyCycle2A(valor_ADC3);
		OCR2B = updateDutyCycle2B(valor_ADC4);
    }
}

void InitADC(void){
	ADCSRA = 0;
	ADMUX = 0; 
	ADMUX |= (1<<ADLAR); //AJUSTE A LA IZQUIERDA (SOLO SE USA ADCH)
	ADMUX |= (1<<REFS0); //Selecciona un voltaje interno de referencia de 5V.
	ADMUX &= ~(1<<REFS1);
	
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Enciente ADC | Prescaler de 128 (125kHZ)
	//ADCSRA |= (1<<ADIE) //Interrupción ADC
	//Deshabilita la entrada digital PC5
	DIDR0 |= (1<<ADC0D)|(1<<ADC1D)|(1<<ADC2D)|(1<<ADC3D);
} 

void checkADC(void){
	
	switch(Cambio){
		case 0:
			while (ADCSRA & (1<<ADSC));
			Cambio = 1; 
			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADCSRA |= (1<<ADSC);
			valor_ADC1 = ADCH;
			break;
			
		case 1:
			while (ADCSRA & (1<<ADSC));
			Cambio = 2; 
			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX |= (1<<MUX0);
			ADCSRA |= (1<<ADSC);
			valor_ADC2 = ADCH;
			break;
		
		case 2: 
			while (ADCSRA & (1<<ADSC));
			Cambio = 3; 
			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX |= (1<<MUX1);
			ADMUX |= (1<<MUX0);
			ADCSRA |= (1<<ADSC);
			valor_ADC3 = ADCH;
			break;
			
		case 3: 
			while (ADCSRA & (1<<ADSC));
			Cambio = 0; 
			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX |= (1<<MUX1)|(1<<MUX0);
			ADMUX |= (1<<MUX0);
			ADCSRA |= (1<<ADSC);
			valor_ADC4 = ADCH;
			break; 
			
		
	}//fin del switch
	
	
}