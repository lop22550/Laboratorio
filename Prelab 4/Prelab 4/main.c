/*
 * Pre laboratorio 4
 * Programación de Microcontroladores{
 * 
 * Creado: 4/8/2024 8:33:12 AM
 * Autor: Boris Josué López Moreira
 */ 

#define F_CPU 16000000

//#include <xc.h>
#include <avr/io.h> 
#include <util/delay.h>

void init(void);
void incremento(void);
void decremento (void);


uint8_t contador = 0;

void init(void){
	UCSR0B = 0;
	
	//inicializando puertos de salida.
	
	DDRD = 0xFF;
	PORTD = 0;
	
	DDRB = 0xFF;
	PORTB = 0;
	
	// Inicializando puertos de entrada para los botones
	DDRC &= ~(1<<PC1);
	DDRC &= ~(1<<PC2);
	PORTC |= (1<<PC1)|(1<<PC2);	//Habilita las resistencias pull up internas	
	};


void incremento(void){
	if (contador<255){ 
		contador++;
		PORTD = contador; 
	}
};

void decremento(void){
	if (contador>0){
		contador--;
		PORTD = contador;
	}
};




int main(void)
{
	
	init();
	
	PORTB = (1<<PB0);
	
    while(1)
    {
		if (!(PINC & (1<<PC1))){
			_delay_ms(50);
			incremento();
			while (!(PINC & (1 << PC1))); 
		};
		
		if  (!(PINC & (1<<PC2))){
			_delay_ms(50);
			decremento();
			while (!(PINC & (1 << PC2)));
		};
		
    }
	return 0; 
}