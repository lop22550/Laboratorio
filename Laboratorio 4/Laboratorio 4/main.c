
/* 
 * Prelaboratorio 4 
 * Programación de Microcontroladores{ 
 *  
 * Creado:  4/10/2024 4:23:13 PM
 * Autor: Boris Josué López Moreira 
 */  
 
#define F_CPU 16000000 
 
#include <avr/io.h>  
#include <util/delay.h> 
#include <avr/interrupt.h>
 
void init(void); 
void incremento(void); 
void decremento (void); 
 
 
uint8_t contador = 0; 
const uint8_t Lista [16] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113};
	
	
void init(void){ 
 //Configuración del ADC
 ADMUX = 0b0101; //Selectiona el puerto ADC5 
 ADCSRA = 0;
 ADMUX |= (1<<ADLAR); //AJUSTE A LA IZQUIERDA (SOLO SE USA ADCH)
 ADMUX |= (1<<REFS0); //Selecciona un voltaje interno de referencia de 5V. 
 ADMUX &= ~(1<<REFS1);
 
 //SE ENCIENDE ADC
 ADCSRA |= (1<<ADEN);
 
 // SE HABILITA LA INTERRUPCIÓN DEL ADC
 ADCSRA |= (1<<ADIE);
 
 //PRESCALER DE 128 (125kHZ)
 ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
 
 //Deshabilita la entrada digital PC5
 DIDR0 |= (1<<ADC5D);
 
 //Apaga RX y TX
 UCSR0B = 0; 
  
 //inicializando puertos de salida. 
 DDRD = 0xFF; 
 PORTD = 0; 
  
 DDRB = 0xFF; 
 PORTB = 0; 
  
 // Inicializando puertos de entrada para los botones 
 DDRC &= ~(1<<PC1); 
 DDRC &= ~(1<<PC2); 
 PORTC |= (1<<PC1)|(1<<PC2); //Habilita las resistencias pull up internas  
 }; 
 
 
void incremento(void){ 
 if (contador<255){  
  contador++; 
  //PORTD = contador;  
 } 
}; 
 
void decremento(void){ 
 if (contador>0){ 
  contador--; 
 // PORTD = contador; 
 } 
}; 
 
 
int main(void)
{
	cli();
    init();
	sei();
	ADCSRA |= (1<<ADSC);
    PORTB = (1<<PINB2);
	while(1)
    {
	    /*if (!(PINC & (1<<PC1))){
		    _delay_ms(50);
		    incremento();
		    while (!(PINC & (1 << PC1)));
	    }
	     
	    if  (!(PINC & (1<<PC2))){
		    _delay_ms(50);
		    decremento();
		    while (!(PINC & (1 << PC2)));
	    }*/
    }
	return 0;   //TODO:: Please write your application code
}


ISR (ADC_vect){
	/*PORTB &= ~(1<<PB0);
	PORTB &= ~(1<<PB1);
	PORTB |= (1<<PB2);
	*/
	uint8_t valor_adc = ADCH; // 1010 1110 & 0000 1111
	uint8_t valor_disp2 = valor_adc & 0b00001111; // Result = 0000 1110 // 14
	uint8_t	valor_disp1 = (valor_adc & 0b11110000) >> 4; // Result = 0000 1010
	
	PORTD = Lista[valor_disp1];
	
	
	ADCSRA |= (1<<ADIF);
	ADCSRA |= (1<<ADSC);
};
