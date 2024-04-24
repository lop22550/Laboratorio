/*
 * Prelaboratorio 6.c
 *
 * Created: 23/04/2024 17:20:01
 * Author : bjosu
 */ 

#define F_CPU 16000000UL 

#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/delay.h>

//Variables globales


//Prototipos de funciones 
void initUART9600 (void);		//Función donde se configura la comunicación serial UART
void writeUART(char caracter); 
void writeTextUART(char* texto); 
unsigned char ReadUART(void);  

volatile char bufferRX; 


int main(void)
{
    initUART9600(); 
	sei(); 
	
	//writeUART('H'); 
	writeTextUART("hola mundo XD");
    while (1) 
    {
		/*PORTD = (bufferRX & 0b00000011)>>6;
		PORTB = (bufferRX & 0b11111100)<<2;*/
		PORTB = bufferRX;
    }
}

//Funciones
void initUART9600(){
	DDRD &= ~(1<<DDD0);		//Entrada RX
	DDRD |= (1<<DDD1);		//Salida TX
	
	DDRD |= (1<<DDD6)|(1<<DDD7); //Configurando salidas para LEDS
	PORTD &= ~(1<<PORTD6); 
	PORTD &= ~(1<<PORTD7);
	
	DDRB = 0xFF; 
	PORTB = 0; 
	

	
	UCSR0A = 0; 
	UCSR0B = 0; 
	UCSR0C = 0; 
	
	UCSR0A |= (1<<U2X0);	//Configurar A Modo Fast U2X0 = 1
	
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);	//Configurar B > Habilitar ISR RX, Habilitarmos RX y TX
	
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);		//Configurar C > Frame: 8 bits de datos, no paridad, 1 bit de stop
	
	UBRR0 = 207; //Usando un Baudrate de 9600
}

//Función de transmisión UART
void writeUART(char caracter) { 
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = caracter; 
}

//Función de recepción UART
unsigned char ReadUART (void){
	//Espera recibir un dato
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0; 	//Devuelve el dato
}

void writeTextUART(char* texto) {
	uint8_t i;
	for (i = 0; texto[i]!= '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = texto[i];
	}
	
}

//Funciones de interrupción 
ISR(USART_RX_vect){
	bufferRX = UDR0; 
	
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = bufferRX;
	
}