
/* 
 * Prelaboratorio 4 
 * Programación de Microcontroladores{ 
 *  
 
 * Creado:  4/10/2024 4:23:13 PM
 * Autor: Boris Josué López Moreira 
 * Última modificación: 4/14/2024 18:37 PM
 */  
 
#define F_CPU 16000000 
 
#include <avr/io.h>  
#include <util/delay.h> 
#include <avr/interrupt.h>
 
void init(void); 
void incremento(void); 
void decremento (void); 
 
 
uint8_t contador = 0; 
uint8_t valor_adc = 0;
uint8_t valor_disp2 = 0;
uint8_t valor_disp1 = 0;
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
	
	ADCSRA |= (1<<ADSC); //Inicia la conversión del ADC
    
	
	
	while(1)
    {	
		
		_delay_ms(5);
		PORTB &= ~(1<<PB0); //Se apagan los LEDS del contador binario
		PORTB |= (1<<PB1);	//Se encienden los displays de 7 segmentos de unidades.
		PORTB &= ~(1<<PB2);
		PORTD = Lista[valor_disp1];
		_delay_ms(5);
		PORTB &= ~(1<<PB0); //Se apagan los LEDS del contador binario
		PORTB &= ~(1<<PB1);
		PORTB |= (1<<PB2);	//Se encienden los displays de 7 segmentos de decenas.
		PORTD = Lista[valor_disp2];
		
		_delay_ms(5);
		PORTB |= (1<<PB0);	//Se enciende los LEDS del contador binario
		PORTB |= (1<<PB1);	//Se apagan los displays de 7 segmentos.
		PORTB &= ~(1<<PB2);
		PORTD = contador;
  
	    if (!(PINC & (1<<PC1))){
		    _delay_ms(10);
		    incremento();
		   while (!(PINC & (1 << PC1)));
	    }
	     
	    if  (!(PINC & (1<<PC2))){
		    _delay_ms(10);
		    decremento();
		   while (!(PINC & (1 << PC2)));
	    }
		
		if (valor_adc == contador){
			PORTB |= (1<<PB3);
		} else {
			PORTB &= ~(1<<PB3);}
		
    }
	
	
	return 0;   //TODO:: Please write your application code
}


ISR (ADC_vect){
	/*PORTB &= ~(1<<PB0); //Se apagan los LEDS del contador binario
	PORTB |= (1<<PB1);	//Se encienden los displays de 7 segmentos de unidades.
	PORTB |= (1<<PB2);*/
	
	//uint8_t 
	valor_adc = ADCH; // 1010 1110 & 0000 1111
	//uint8_t 
	valor_disp2 = valor_adc & 0b00001111; // Result = 0000 1110 // 14
	//uint8_t	
	valor_disp1 = (valor_adc & 0b11110000) >> 4; // Result = 0000 1010
	
	
	
	ADCSRA |= (1<<ADIF);
	ADCSRA |= (1<<ADSC);
};
