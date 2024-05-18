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
#include <avr/eeprom.h>

#include "InitPWM1/InitPWM1.h"
#include "InitPWM2/InitPWM2.h"

//Variables globales 
uint8_t valor_ADC1 = 0;		//Las variables valor_ADCn son las variables donde se guarda el valor de la conversión del ADC de cada potenciómetro.
uint8_t valor_ADC2 = 0;
uint8_t valor_ADC3 = 0;
uint8_t valor_ADC4 = 0;
uint8_t Cambio = 0;			//Variable utilizada como bandera para marcar que canal ADC se está realizando la conversión.
uint8_t Estado = 1;			//Variable utilizada como bandera para indicar el cambio de estado. 

uint8_t flagw_pos1 = 0; 
uint8_t flagw_pos2 = 0; 
uint8_t flagw_pos3 = 0;
uint8_t flagw_pos4 = 0;

uint8_t flagr_pos1 = 0;
uint8_t flagr_pos2 = 0;
uint8_t flagr_pos3 = 0;
uint8_t flagr_pos4 = 0;

#define DEBOUNCE_TIME 50

//Prototipos de funciones
void InitADC (void);
void checkADC (void);
void init (void); 
void updateRGBLed(void);
void writeEEPROM (void); 
void readEEPROM (void); 

int main(void)
{
	cli();
	init();
	InitADC(); 
	initPWM1();
	initPWM2();
	sei();
    
    while (1) 
    {
		//PORTD |= (1<<PORTD5); 
		checkADC();
		OCR1A = updateDutyCycle1A(valor_ADC1); 
		OCR1B = updateDutyCycle1A(valor_ADC2); 
		OCR2A = updateDutyCycle2A(valor_ADC3);
		OCR2B = updateDutyCycle2B(valor_ADC4);
		
		
		updateRGBLed();
		
		
		
    }
}

void InitADC(void){
	ADCSRA = 0;
	ADMUX = 0; 
	ADMUX |= (1<<ADLAR); //AJUSTE A LA IZQUIERDA (SOLO SE USA ADCH)
	ADMUX |= (1<<REFS0); //Selecciona un voltaje interno de referencia de 5V.
	ADMUX &= ~(1<<REFS1);
	
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Enciente ADC | Prescaler de 128 (125kHZ)
	//ADCSRA |= (1<<ADIE) //Interrupción ADC
	//Deshabilita la entrada digital PC5
	//DIDR0 |= (1<<ADC0D)|(1<<ADC1D)|(1<<ADC2D)|(1<<ADC3D);
} 

void checkADC(void){
	
	switch(Cambio){
		case 0:
			while (ADCSRA & (1<<ADEN)){ADCSRA &= ~(1<<ADEN);}
			Cambio = 1; 
			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADCSRA |= (1<<ADEN);
			ADCSRA |= (1<<ADSC);
			_delay_ms(5);
			valor_ADC1 = ADCH;
			
			break;
			
		case 1:
			while (ADCSRA & (1<<ADEN)){ADCSRA &= ~(1<<ADEN);}
			Cambio = 2; 
			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX |= (1<<MUX0);
			ADCSRA |= (1<<ADEN);
			ADCSRA |= (1<<ADSC);
			_delay_ms(5);
			valor_ADC2 = ADCH;
			break;
		
		case 2: 
			while (ADCSRA & (1<<ADEN)){ADCSRA &= ~(1<<ADEN);}
			Cambio = 3; 
			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX |= (1<<MUX1);
			ADMUX |= (1<<MUX0);
			ADCSRA |= (1<<ADEN);
			ADCSRA |= (1<<ADSC);
			_delay_ms(5);
			valor_ADC3 = ADCH;
			break;
			
		case 3: 
			while (ADCSRA & (1<<ADEN)){ADCSRA &= ~(1<<ADEN);}
			Cambio = 0; 
			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX |= (1<<MUX1)|(1<<MUX0);
			ADMUX |= (1<<MUX0);
			ADCSRA |= (1<<ADEN);
			ADCSRA |= (1<<ADSC);
			_delay_ms(5);
			valor_ADC4 = ADCH;
			break; 
			
		
	}//fin del switch
	
	
}

void init (void){
	CLKPR |= (1<<CLKPCE)|(1<< CLKPS0);
	
	DDRD |= (1<<DDD4); 
	PORTD &= ~(1<<PORTD4);
	
	DDRD |= (1<<DDD5);
	PORTD &= ~(1<<PORTD5);
	
	DDRD |= (1<<DDD6);
	PORTD &= ~(1<<PORTD6);
	
	DDRD |= (1<<DDD7);
	PORTD &= ~(1<<PORTD7);
	
	//botones
	DDRB &= ~(1<<DDB0);
	PORTB |= (1<<PORTB0); //modo
	
	DDRB &= ~(1<<DDB4);
	PORTB |= (1<<PORTB4); //pos 1
	
	DDRB &= ~(1<<DDB5);
	PORTB |= (1<<PORTB5); //pos 2
	
	DDRC &= ~(1<<DDC4);
	PORTC |= (1<<PORTC4); //pos 3
	
	DDRD &= ~(1<<DDD2);
	PORTD |= (1<<PORTD2); //pos 4
	
	//CONFIGURACIÓN DE INTERRUPCION PIN CHANGE
	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT0);

}


void updateRGBLed(void) {
	switch (Estado) {
		case 1:
		PORTD |= (1 << PORTD4); 
		PORTD &= ~((1 << PORTD5)|(1<<PORTD6));
		
		break;
		case 2:
		PORTD |= (1 << PORTD5);
		PORTD &= ~((1 << PORTD4)|(1<<PORTD6));
		break;
		case 3:
		PORTD |= (1 << PORTD6);
		PORTD &= ~((1 << PORTD4)|(1<<PORTD5));
		break;
		default:
		PORTD &= ~((1 << PORTD4)|(1 << PORTD5)|(1 << PORTD6)); 
		break;
	}
}


//Interrupciones --------------------------------------------------------
ISR(PCINT0_vect){
	
	if (!(PINB & (1 << PINB))) {
		_delay_ms(DEBOUNCE_TIME); // Anti-rebote
		if (!(PINB & (1 << PINB))){ // Verificar de nuevo el pin después del delay
			// Cambio de estado
			Estado++;
			if (Estado > 3) {
				Estado = 1;
			}
			
		}
}
}



void writeEEPROM (void){
	
	
}

void readEEPROM (void){
	
	
	
}