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
    
    while (1) 
    {
		OCR1A = updateDutyCycle1A(valor_ADC1); 
		OCR1B = updateDutyCycle1A(valor_ADC2); 
		OCR2A = valor_ADC3 / 6;
		OCR2B = valor_ADC4 / 6; 
    }
}

void InitADC(void){
	ADCSRA = 0;
	ADMUX = 0; 
	ADMUX |= (1<<ADLAR); //AJUSTE A LA IZQUIERDA (SOLO SE USA ADCH)
	ADMUX |= (1<<REFS0); //Selecciona un voltaje interno de referencia de 5V.
	ADMUX &= ~(1<<REFS1);
	
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Enciente ADC | Interrupción ADC | Prescaler de 128 (125kHZ)
	
	//Deshabilita la entrada digital PC5
	DIDR0 |= (1<<ADC0D)|(1<<ADC1D)|(1<<ADC2D)|(1<<ADC3D);
} 

void checkADC(void){
	
	switch(Cambio){
		case: 0
				
		
	}//fin del switch
	
	
}