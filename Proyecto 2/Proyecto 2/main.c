/* Universidad del Valle de Guatemala
 * Autor : Boris Josué López Moreira
 * IE2023 Programación de Microcontroladores
 * Proyecto:Proyecto 2 - Brazo robótico 
 * Hardware: ATMEGA328P
 * Creado: 28/04/2024 12:44:26
 
 */ 

#define  F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>


int main(void)
{
    
    while (1) 
    {
		
		OCR1A = updateDutyCycle1A(valor_ADC1); 
		
    }
}

