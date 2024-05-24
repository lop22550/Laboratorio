/* Universidad del Valle de Guatemala
 * Autor : Boris Josué López Moreira
 * IE2023 Programación de Microcontroladores
 * Proyecto:Proyecto 2 - Brazo robótico 
 * Hardware: ATMEGA328P
 * Creado: 28/04/2024 12:44:26
 
 */ 

#define  F_CPU 16000000UL

#define CERO 48
#define UNO 49
#define DOS 50

//Librerías
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>



#include "InitPWM1/InitPWM1.h"
#include "InitPWM2/InitPWM2.h"


//Variables globales 
uint8_t valor_ADC1 = 0;		//Las variables valor_ADCn son las variables donde se guarda el valor de la conversión del ADC de cada potenciómetro.
uint8_t valor_ADC2 = 0;
uint8_t valor_ADC3 = 0;
uint8_t valor_ADC4 = 0;
uint8_t Cambio = 0;			//Variable utilizad como bandera para marcar que canal ADC se está realizando la conversión.
uint8_t Estado = 1;			//Variable utilizada como bandera para indicar el cambio de estado. 


uint8_t flagw_pos1 = 0; 
uint8_t flagw_pos2 = 0; 
uint8_t flagw_pos3 = 0;
uint8_t flagw_pos4 = 0;

uint8_t flagr_pos1 = 0;
uint8_t flagr_pos2 = 0;
uint8_t flagr_pos3 = 0;
uint8_t flagr_pos4 = 0;

uint8_t lectura_servo1 = 0; 
uint8_t lectura_servo2 = 0;
uint8_t lectura_servo3 = 0;
uint8_t lectura_servo4 = 0;

//Variables de control para comunicación serial 

volatile char bufferRX;

uint8_t servo1 = 0;
uint8_t servo2 = 0;
uint8_t servo3 = 0;
uint8_t servo4 = 0;
uint8_t pos1 = 0;
uint8_t pos2 = 0;
uint8_t pos3 = 0;
uint8_t pos4 = 0;
uint8_t EstadoSerial = 0;

 char lectura_slider; 
int slider =0;

#define DEBOUNCE_TIME 100

//Prototipos de funciones
void InitADC (void);
void checkADC (void);
void init (void); 
void writeEEPROM (void); 
void readEEPROM (void); 
void initUART9600 (void);				//Función donde se configura la comunicación serial UART
void writeUART(char caracter);
void writeTextUART(char* texto);
void ConvertASCII (uint8_t convert);				//Función convierte un número en valores ASCII
void ReceiveAIO (void); 






int main(void)
{
	cli();
	init();
	InitADC();
	initPWM1();
	initPWM2();
	initUART9600();
	writeUART(bufferRX);

	sei();
    
    while (1) 
    {
		

		switch (Estado) {
			case 1:
				PORTD |= (1 << PORTD4);
				PORTD &= ~((1 << PORTD5)|(1<<PORTD6)|(1 << PORTD7));
				checkADC();
				OCR1A = updateDutyCycle1A(valor_ADC1);
				OCR1B = updateDutyCycle1A(valor_ADC2);
				OCR2A = updateDutyCycle2A(valor_ADC3);
				OCR2B = updateDutyCycle2B(valor_ADC4);
				break;
			
			case 2:
				PORTD |= (1 << PORTD5);
				PORTD &= ~((1 << PORTD4)|(1<<PORTD6)|(1 << PORTD7));
				writeEEPROM();
				break;
			
			case 3:

				PORTD |= (1 << PORTD6);
				PORTD &= ~((1 << PORTD4)|(1<<PORTD5)|(1 << PORTD7));
				readEEPROM(); 
				break;
			
			case 4:
				initUART9600();
				PORTD |= (1 << PORTD7);
				PORTD &= ~((1 << PORTD4)|(1<<PORTD5)|(1 << PORTD6));
				ReceiveAIO();
			
				break;
			
			default:
				PORTD &= ~((1 << PORTD4)|(1 << PORTD5)|(1 << PORTD6));
				break;
		}
		
    }
}

void InitADC(void){
	ADCSRA = 0;
	ADMUX = 0; 
	ADMUX |= (1<<ADLAR); //AJUSTE A LA IZQUIERDA (SOLO SE USA ADCH)
	ADMUX |= (1<<REFS0); //Selecciona un voltaje interno de referencia de 5V.
	ADMUX &= ~(1<<REFS1);	
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Enciente ADC | Prescaler de 128 (125kHZ)
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
			ADCSRA |= (1<<ADEN);
			ADCSRA |= (1<<ADSC);
			_delay_ms(5);
			valor_ADC3 = ADCH;
			OCR2A = updateDutyCycle2A(valor_ADC3);
			break;
			
		case 3: 
			while (ADCSRA & (1<<ADEN)){ADCSRA &= ~(1<<ADEN);}
			Cambio = 0; 
			//ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX |= (1<<MUX1)|(1<<MUX0);
			ADCSRA |= (1<<ADEN);
			ADCSRA |= (1<<ADSC);
			_delay_ms(5);
			valor_ADC4 = ADCH;
			OCR2B = updateDutyCycle2B(valor_ADC4);
			break; 
		default:
			break;
	}//fin del switch
}

void init (void){
	CLKPR |= (1<<CLKPCE)|(1<< CLKPS0);
	
	DDRD |= (1<<DDD4);  //led rojo - Control manul de servomotores 
	PORTD &= ~(1<<PORTD4);
	
	DDRD |= (1<<DDD5);  //led verde - Escribir en EEPROM
	PORTD &= ~(1<<PORTD5);
	
	DDRD |= (1<<DDD6);  // led azul - Leer en EEPROM
	PORTD &= ~(1<<PORTD6);
	
	DDRD |= (1<<DDD7);  // led amarillo - serial y AIO
	PORTD &= ~(1<<PORTD7); 
	
		DDRB &= ~(1<<DDB0);
		PORTB |= (1<<PORTB0); //modo
		
		DDRB &= ~(1<<DDB4);
		PORTB |= (1<<PORTB4); //pos 1
		
		DDRB &= ~(1<<DDB5);
		PORTB |= (1<<PORTB5); //pos 2
		
		DDRC &= ~(1<<DDC4);
		PORTC |= (1<<PORTC4); //pos 3
		
		DDRC &= ~(1<<DDC5);
		PORTC |= (1<<PORTC5); //pos 4
	
	//CONFIGURACIÓN DE INTERRUPCION PIN CHANGE
	PCICR |= (1<<PCIE0)|(1<<PCIE1);
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT4);
	PCMSK1 |= (1<<PCINT12)|(1<<PCINT13);
}



void initUART9600(void){
	DDRD &= ~(1<<DDD0);		//Entrada RX
	DDRD |= (1<<DDD1);		//Salida TX
	
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

void writeTextUART(char* texto) {
	uint8_t i;
	for (i = 0; texto[i]!= '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = texto[i];
	}
}

void ConvertASCII(uint8_t convert){
	uint8_t unidades, decenas, centenas;
	
	unidades = convert % 10;						//ej. 156 %10 = 6
	convert = (convert - unidades)/10;		//(156-6)/10 = 150/10 = 15
	decenas = convert % 10;						//15%10 = 5
	convert = (convert - decenas)/10;		//(15-5)/10 = 1
	centenas = convert % 10;						//1%10 = 1
	
	writeUART(CERO + centenas);
	writeUART(CERO + decenas);
	writeUART(CERO + unidades);
	
	
	writeUART('\n');
	//writeUART('\r');
}



//Interrupciones ----------------------------------------------------------------------------------------------------------------------
ISR(PCINT0_vect){
	
	if (!(PINB & (1 << PINB0))) {
		_delay_ms(DEBOUNCE_TIME); // Anti-rebote
		if (!(PINB & (1 << PINB0))){ // Verificar de nuevo el pin después del delay
			// Cambio de estado
			Estado++;
			if (Estado > 4) {Estado = 1;}
		}
	}
	
	if (Estado==2){
		if (!(PINB & (1 << PINB4))) {
			_delay_ms(DEBOUNCE_TIME); // Anti-rebote
			if (!(PINB & (1 << PINB4))){ // Verificar de nuevo el pin después del delay
				flagw_pos1 = 1; writeUART(flagw_pos1);}
		}
	
		if (!(PINB & (1 << PINB5))) {
			_delay_ms(DEBOUNCE_TIME); // Anti-rebote
			if (!(PINB & (1 << PINB5))){ // Verificar de nuevo el pin después del delay
			flagw_pos2 = 1; writeUART(flagw_pos2);}
		}
	
	}// fin del swithc de Guardado
	
	
	if (Estado==3){
		if (!(PINB & (1 << PINB4))) {
			_delay_ms(DEBOUNCE_TIME); // Anti-rebote
			if (!(PINB & (1 << PINB4))){ // Verificar de nuevo el pin después del delay
				// Cambio de estado
				flagr_pos1 = 1; writeUART(flagr_pos1);}
		}
		
		if (!(PINB & (1 << PINB5))) {
			_delay_ms(DEBOUNCE_TIME); // Anti-rebote
			if (!(PINB & (1 << PINB5))){ // Verificar de nuevo el pin después del delay
				// Cambio de estado
			flagr_pos2 = 1;  writeUART(flagr_pos2);}
		}
		
		
		
	}// fin del swithc de Lectura 
	
		
		
}

ISR(PCINT1_vect){
	
	if (Estado==2){
		if (!(PINC & (1 << PINC4))) {
			_delay_ms(DEBOUNCE_TIME); // Anti-rebote
			if (!(PINC & (1 << PINC4))){ // Verificar de nuevo el pin después del delay
				// Cambio de estado
			flagw_pos3 = 1;}
		}
		
		if (!(PINC & (1 << PINC5))) {
			_delay_ms(DEBOUNCE_TIME); // Anti-rebote
			if (!(PINC & (1 << PINC5))){ // Verificar de nuevo el pin después del delay
				// Cambio de estado
			flagw_pos4 = 1;}
		}
		
	}// fin del swithc de Guardado
	
	
	if (Estado==3){
		if (!(PINC & (1 << PINC4))) {
			_delay_ms(DEBOUNCE_TIME); // Anti-rebote
			if (!(PINC & (1 << PINC4))){ // Verificar de nuevo el pin después del delay
				// Cambio de estado
			flagr_pos3 = 1;}
		}
		
		if (!(PINC & (1 << PINC5))) {
			_delay_ms(DEBOUNCE_TIME); // Anti-rebote
			if (!(PINC & (1 << PINC5))){ // Verificar de nuevo el pin después del delay
				// Cambio de estado
			flagr_pos4 = 1;}
		}
		
		
		
	}// fin del swithc de Lectura
	
	
	
}

ISR(USART_RX_vect){
	bufferRX = UDR0;
	
	//Guardar y leer
	if (bufferRX == 'G'){
		EstadoSerial = 1; 
	}
	if (bufferRX == 'L'){
		EstadoSerial = 2;
	}
	
	//posiciones 
	if (bufferRX == 'a'){
		pos1 = 1;
	}
	
	if (bufferRX == 'b'){
		pos2 = 1;
	}
	
	if (bufferRX == 'c'){
		pos3 = 1;
	}
	
	if (bufferRX == 'd'){
		pos4 = 1;
	}
	
	//recibir sliders de servos
	if (bufferRX == 'e'){
		servo1 = 1;
	}
	
	if (bufferRX == 'f'){
		servo2 = 1;
	}
	
	if (bufferRX == 'g'){
		servo3 = 1;
	}
	
	if (bufferRX == 'h'){
		servo4 = 1;
	}
	
	
	//lectura de cadenas de caracteres enviados por los sliders
	if (servo1 == 1){
		lectura_slider = bufferRX; 
		//servo1 = 0; 
	}

	if (servo2 == 1){
		lectura_slider = bufferRX;
		//servo2 = 0;
	}
	
	if (servo3 == 1){
		lectura_slider = bufferRX;
		//servo3 = 0;
	}
	
	if (servo4 == 1){
		lectura_slider = bufferRX;
		//servo4 = 0;
	}
	
	

}




void writeEEPROM (void){
	if (flagw_pos1 == 1)
	{
		eeprom_write_byte(( uint8_t*)0, valor_ADC1); 
		eeprom_write_byte(( uint8_t*)1, valor_ADC2);
		eeprom_write_byte(( uint8_t*)2, valor_ADC3);
		eeprom_write_byte((uint8_t*)3, valor_ADC4);
		flagw_pos1 = 0; 
	}
	
	if (flagw_pos2 == 1)
	{
		eeprom_write_byte(( uint8_t*)4, valor_ADC1);
		eeprom_write_byte(( uint8_t*)5, valor_ADC2);
		eeprom_write_byte(( uint8_t*)6, valor_ADC3);
		eeprom_write_byte((uint8_t*)7, valor_ADC4);
		flagw_pos2 = 0;
	}
	
	if (flagw_pos3 == 1)
	{
		eeprom_write_byte(( uint8_t*)8, valor_ADC1);
		eeprom_write_byte(( uint8_t*)9, valor_ADC2);
		eeprom_write_byte(( uint8_t*)10, valor_ADC3);
		eeprom_write_byte((uint8_t*)11, valor_ADC4);
		flagw_pos3 = 0;
	}
	
	if (flagw_pos4 == 1)
	{
		eeprom_write_byte(( uint8_t*)12, valor_ADC1);
		eeprom_write_byte(( uint8_t*)13, valor_ADC2);
		eeprom_write_byte(( uint8_t*)14, valor_ADC3);
		eeprom_write_byte((uint8_t*)15, valor_ADC4);
		flagw_pos4 = 0;
	}
	
	}
void readEEPROM (void){
	if (flagr_pos1 == 1){
		lectura_servo1 = eeprom_read_byte((uint8_t*) 0); 
		lectura_servo2 = eeprom_read_byte((uint8_t*) 1);
		lectura_servo3 = eeprom_read_byte((uint8_t*) 2);
		lectura_servo4 = eeprom_read_byte((uint8_t*) 3);
		OCR1A = updateDutyCycle1A(lectura_servo1);
		OCR1B = updateDutyCycle1B(lectura_servo2);
		OCR2A = updateDutyCycle2A(lectura_servo3);
		OCR2B = updateDutyCycle2B(lectura_servo4);
		flagr_pos1 = 0;
		}
		
	if (flagr_pos2 == 1){
		lectura_servo1 = eeprom_read_byte((uint8_t*) 4);
		lectura_servo2 = eeprom_read_byte((uint8_t*) 5);
		lectura_servo3 = eeprom_read_byte((uint8_t*) 6);
		lectura_servo4 = eeprom_read_byte((uint8_t*) 7);
		OCR1A = updateDutyCycle1A(lectura_servo1);
		OCR1B = updateDutyCycle1B(lectura_servo2);
		OCR2A = updateDutyCycle2A(lectura_servo3);
		OCR2B = updateDutyCycle2B(lectura_servo4);
		flagr_pos2 = 0;
	}
	
	if (flagr_pos3 == 1){
		lectura_servo1 = eeprom_read_byte((uint8_t*) 8);
		lectura_servo2 = eeprom_read_byte((uint8_t*) 9);
		lectura_servo3 = eeprom_read_byte((uint8_t*) 10);
		lectura_servo4 = eeprom_read_byte((uint8_t*) 11);
		OCR1A = updateDutyCycle1A(lectura_servo1);
		OCR1B = updateDutyCycle1B(lectura_servo2);
		OCR2A = updateDutyCycle2A(lectura_servo3);
		OCR2B = updateDutyCycle2B(lectura_servo4);
		flagr_pos3 = 0;
	}
	
	if (flagr_pos4 == 1){
		lectura_servo1 = eeprom_read_byte((uint8_t*) 12);
		lectura_servo2 = eeprom_read_byte((uint8_t*) 13);
		lectura_servo3 = eeprom_read_byte((uint8_t*) 14);
		lectura_servo4 = eeprom_read_byte((uint8_t*) 15);
		OCR1A = updateDutyCycle1A(lectura_servo1);
		OCR1B = updateDutyCycle1B(lectura_servo2);
		OCR2A = updateDutyCycle2A(lectura_servo3);
		OCR2B = updateDutyCycle2B(lectura_servo4);
		flagr_pos4 = 0;
	}
}

void ReceiveAIO (void){
	_delay_ms(10);
	//Guardar
	if (EstadoSerial == 1){
		PORTD |= (1 << PORTD5);
		PORTD &= ~((1 << PORTD4)|(1<<PORTD6));
		if (pos1==1){
			flagw_pos1 = 1;
			pos1 = 0; 
			writeEEPROM(); }
		if (pos2==1){
			flagw_pos2 = 1;
			pos2 = 0;
			writeEEPROM();}
		if (pos3==1){
			flagw_pos3 = 1;
			pos3 = 0;
			writeEEPROM();}
		if (pos4==1){
			flagw_pos4 = 1;
			pos4 = 0;
			writeEEPROM();}
	}
	//Leer
	if (EstadoSerial == 2){
		PORTD |= (1 << PORTD6);
		PORTD &= ~((1 << PORTD4)|(1<<PORTD5));
		if (pos1==1){
			flagr_pos1 = 1;
			pos1 = 0;
			readEEPROM();}
		if (pos2==1){
			flagr_pos2 = 1;
			pos2 = 0;
			readEEPROM();}
		if (pos3==1){
			flagr_pos3 = 1;
			pos3 = 0;
			readEEPROM();}
		if (pos4==1){
			flagr_pos4 = 1;
			pos4 = 0;
			readEEPROM();}
	}
	
	//servos
	if (servo1 == 1){
		//slider = atoi(lectura_slider);
		//printf(slider);
		//OCR1A = updateDutyCycle1A(slider);
	}
	
	
	
	}