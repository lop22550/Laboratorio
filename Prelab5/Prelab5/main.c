/* Universidad del Valle de Guatemala
 * Autor : Boris Josué López Moreira
 * IE2023 Programación de Microcontroladores
 * Proyecto: Prelaboratorio 5 
 * Hardware: ATMEGA328P
 * Created: 16/04/2024 19:53:31
 * 
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <stdint.h>
#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"

void setup (void); 

void initPWM1 ();
void initPWM2();
void initADC1 ();
//uint16_t Lectura_ADC();
//void updateDutyCycleA1(uint16_t valor_ADC);


uint16_t dutyCycle = 0; 
uint8_t valor_ADC = 0;
uint8_t valor_ADC2 = 0;
uint8_t Senal_servo1 = 0;
uint8_t LED_variable = 0; 

void initADC1(void){
	//Configuración del ADC
	ADMUX = 0b0101; //Selectiona el puerto ADC5
	ADCSRA = 0;
	
	ADMUX |= (1<<ADLAR); //AJUSTE A LA IZQUIERDA (SOLO SE USA ADCH)
	ADMUX |= (1<<REFS0); //Selecciona un voltaje interno de referencia de 5V.
	ADMUX &= ~(1<<REFS1);
	
	ADCSRA |= (1<<ADEN);//SE ENCIENDE ADC
	ADCSRA |= (1<<ADIE); // SE HABILITA LA INTERRUPCIÓN DEL ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//PRESCALER DE 128 (125kHZ)
	
	DIDR0 |= (1<<ADC5D); //Deshabilita la entrada digital PC5	
}

//Está función se encargará de iniciar la conversión y leer la conversasión analógico - digital 

/*uint16_t Lectura_ADC (){
	valor_ADC = ADCH;
	ADCSRA |= (1<<ADIF);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADCH;
}*/


int main(void)
{
	DDRB = 0xFF;
	PORTB = 0; 

	cli();
	initADC1();
	initPWM1A();
	initPWM2();
	sei();
	ADCSRA |= (1<<ADSC);

	UCSR0A = 0;
	UCSR0B = 0;
	
	while (1) 
    {	
		//ADMUX &= 0b11110000;
		//ADMUX |= (1<<MUX2)|(1<<MUX0); //Activa el Puerto A5
		updateDutyCycleA1(valor_ADC);	
		
		/*_delay_ms(25);
		ADMUX &= 0b11110000;
		ADMUX |= (1<<MUX2); //Activa el Puerto A4
		updateDutyCycle2(valor_ADC);*/
		
		ADMUX &= 0b11110000;
		ADMUX |= (1<<MUX1)|(1<<MUX0); //Activa el Puerto A3
		LED_variable++;
		if (LED_variable == 0){
			PORTB |= (1<<PORTB0);
		} else if (LED_variable == valor_ADC) {
			PORTB &= ~(1<<PORTB0);
		}
		
		
    }
}

ISR (ADC_vect){
	valor_ADC = ADCH;
	updateDutyCycleA1(valor_ADC);
	updateDutyCycle2(valor_ADC);
	ADCSRA |= (1<<ADIF);
	ADCSRA |= (1<<ADSC);
}