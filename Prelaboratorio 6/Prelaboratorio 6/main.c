/*
 * Prelaboratorio 6.c
 * Proyecto: Pre laboratorio 6
 * Hardware: ATMEGA328P
 * Autor: Boris Josu� L�pez Moreira
 * Created: 23/04/2024 17:20:01
 */ 

#define F_CPU 16000000UL 
#define CERO 48
#define UNO 49
#define DOS 50

#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/delay.h>

//Variables globales
uint8_t opcion = 0;						//Variable de seleccion para men�
uint8_t lectura_ADC = 0;				//Lee el valor del ADC para la opci�n 1. Leer potenci�metro 
uint8_t bandera_menu = 1;				//Esta bandera ser� para desplegar el men� y no colapsar el ciclo
uint8_t conversionADC = 0;				//Esta variable la usar� para convertir la lectura en ADC en un valor char que pueda mostrarse en la terminal 
uint8_t flag_ASCII = 0;					//Bandera utilizada para permitir el ingreso de un ASCII en la opci�n 2 del men�. 

//Prototipos de funciones 
void initUART9600 (void);				//Funci�n donde se configura la comunicaci�n serial UART
void writeUART(char caracter); 
void writeTextUART(char* texto); 
void initADC(void); 
void ConvertADC (uint8_t lectura_ADC);

volatile char bufferRX; 


int main(void)
{
	cli();
    initUART9600(); 
	initADC();
	sei(); 
	
	//writeUART('H'); 
	//writeTextUART("\n Hola Pedro XD");
    while (1) 
    {
		writeUART(opcion);
		if (flag_ASCII == 1){
			PORTD = (opcion & 0b00000011)<<6;
			PORTB = (opcion & 0b11111100)>>2;
			
		}
		
		if (bandera_menu == 1){
			opcion = 0; 
			writeTextUART("\n 1. Leer potenci�metro.");
			writeTextUART("\n 2. Enviar ASCII.");
			
			while (!((opcion == UNO)||(opcion == DOS))); 
			
			
			switch (opcion){
				case UNO: 
					ADCSRA |= (1<< ADSC); 
					ConvertADC(lectura_ADC); 
					bandera_menu = 0; 
					_delay_ms(10); 
					break; 
				case DOS: 
					bandera_menu = 0;
					writeTextUART("Ingrese un ASCII"); 
					PORTD = (bufferRX & 0b00000011)<<6;
					PORTB = (bufferRX & 0b11111100)>>2;
					while (bandera_menu == 0); 
					
					break; 
			}//fin swithc
			
		}//fin del if 
		
	bandera_menu = 1; 
	writeTextUART("\n Salir"); 
    }
}

//Funciones
void initUART9600(void){
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

//Funci�n de transmisi�n UART
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

//Funci�n de configuraci�n del ADC en el puerto A5 (Puerto C5) 
void initADC(void){
	//Configuraci�n del ADC
	ADMUX = 0b0101; //Selectiona el puerto ADC5
	ADCSRA = 0;
	ADMUX |= (1<<ADLAR); //AJUSTE A LA IZQUIERDA (SOLO SE USA ADCH)
	ADMUX |= (1<<REFS0); //Selecciona un voltaje interno de referencia de 5V.
	ADMUX &= ~(1<<REFS1);
	
	//SE ENCIENDE ADC
	ADCSRA |= (1<<ADEN);
	
	// SE HABILITA LA INTERRUPCI�N DEL ADC
	ADCSRA |= (1<<ADIE);
	
	//PRESCALER DE 128 (125kHZ)
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	//Deshabilita la entrada digital PC5
	DIDR0 |= (1<<ADC5D);

};

void ConvertADC(uint8_t conversionADC){
	uint8_t unidades, decenas, centenas; 
	
	unidades = conversionADC % 10;						//ej. 156 %10 = 6
	conversionADC = (conversionADC - unidades)/10;		//(156-6)/10 = 150/10 = 15
	decenas = conversionADC % 10;						//15%10 = 5
	conversionADC = (conversionADC - decenas)/10;		//(15-5)/10 = 1
	centenas = conversionADC % 10;						//1%10 = 1
	
	writeUART(CERO + centenas); 
	writeUART(CERO + decenas); 
	writeUART(CERO + unidades); 
	
	
	writeUART('\n');
	//writeUART('\r');
}


//Funciones de interrupci�n ------------------------------------------------------------------------------------------
ISR(USART_RX_vect){
	bufferRX = UDR0; 
	opcion = bufferRX;
	
	
	
	if (bandera_menu == 1){
		if (opcion == UNO) {
			flag_ASCII = 0;
			writeTextUART("\n Leyendo el potenci�metro...");

		} else if (opcion == DOS){
			flag_ASCII = 1;
			//writeUART(CERO + flag_ASCII);
			writeTextUART("\n Leyendo ASCII desde la terminal serial...");
			//writeUART(opcion);
		} else {writeTextUART("\n Su digitaci�n no es v�lida, ingrese un valor 1 o 2.");}
		} //fin if

	
	if (bandera_menu == 0){
		PORTD = (bufferRX & 0b00000011)<<6;
		PORTB = (bufferRX & 0b11111100)>>2;
		bandera_menu = 1; 
	}
			
	
	
	//while (!(UCSR0A & (1<<UDRE0)));
	//UDR0 = bufferRX;
	
}

ISR(ADC_vect){
	lectura_ADC = ADCH; 
	PORTD=(lectura_ADC<<6);
	PORTB=(lectura_ADC>>2); 
	ADCSRA |= (1<<ADIF); 
	
}