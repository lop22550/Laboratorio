;************************************************************************
; Universidad del Valle de Guatemala
; Programación de microcontroladores
; Postlaboratorio2.asm
; Autor : Boris Moreira
; Carné 22550
; Hardware: Atmega328P
; Creado: 12/02/2024
;************************************************************************

;************************************************************************
;Encabezado
;************************************************************************
.include "M328PDEF.INC"
.cseg
.org 0x00

;************************************************************************
;Stack pointer
;************************************************************************
; Configuración de los espacios a utilizar de la RAM
LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R16, HIGH(RAMEND)
OUT SPH, R16


	TABLA7SEGMENTOS: .DB 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47 

;************************************************************************
; CONFIGURACIÓN
;************************************************************************
Setup: 
	LDI R16, (1 << CLKPCE) ; Se habilita el clock prescaler
	STS CLKPR, R16 
	LDI R16, 0b0000_0101 ; se define la frecuencia de 1MHz
	STS CLKPR, R16

	;Configuración del Timer0
	LDI R16, (1<<WGM01)
	OUT TCCR0A, R16 ;Coloca el timer en modo de operación CTC
	LDI R16, 0b0000_0101 ; pone en 1 los bits CS02 y CS00 para colocar un prescaler en 1024
	OUT TCCR0B, R16 
	LDI R16, 98 ; valor de comparación
	OUT OCR0A, R16 

	;Salidas del contador, los pines del pc0 al pc3 serán salidas y los pines pc4 y pc5 serán entradas de los push
	LDI R16, 0b0000_1111
	OUT DDRC, R16
	LDI R16, 0x00
	OUT PORTC, R16
	LDI R16, 0b0011_0000
	OUT PORTC, R16  ; activa los pull up en los pines pc4 y pc5
	
	LDI R16, 0xFF
	OUT DDRD, R16
	LDI R16, 0b1111_0000
	OUT PORTD, R16


	SBI DDRB, PB5
	CBI PORTB, PB5

	.DEF CONTADOR=R18	  ; es donde se almacenan el contador de los botones. 
	.DEF ENTRADAS=R21	  ; se almacenan los registros
	.DEF CONTADOR7SEG = R20  ; es para el contador binario de 4 bits

	;DESACTIVAR TRX EN LOS PUERTOS D
	LDI R16, 0x00
	STS UCSR0B, R16

;LOOP


LOOP:
	;Lee los pines 
	IN ENTRADAS, PINC
	SBRC ENTRADAS, PC4
	CALL INCREMENTAR

	SBRC ENTRADAS, PC5
	CALL DECREMENTAR
	
	IN		R17, TIFR0 ;registro de flags 
	SBRC	R17, OCF0A
	RJMP	TOOGLE
	
	CP CONTADOR, CONTADOR7SEG
	BRNE DESACTIVARALARMA
	BREQ ACTIVARALARMA

	RJMP	LOOP

;***********************************************************
;SUBRUTINAS IMPORTANTES: 
;***********************************************************

 TOOGLE:
	INC R20
	OUT PORTC, R20
	CP R20, CONTADOR

	SBI		TIFR0, OCF0A
	RJMP	LOOP


INCREMENTAR: 
	CALL ANTIREBOTE
	INC CONTADOR
	CALL MAIN
	

	RET

DECREMENTAR: 
	CALL ANTIREBOTE
	DEC CONTADOR 
	CALL MAIN
	
	RET

ANTIREBOTE: 
	LDI R19, 0
	DELAY: 
		INC R19
		CPI R19, 255
		BRNE DELAY
	RET 
		
MAIN: 
	LDI ZH, HIGH(TABLA7SEGMENTOS <<1)
	LDI ZL, LOW(TABLA7SEGMENTOS << 1)
	ADD ZL, CONTADOR
	LPM R16, Z
	OUT PORTD, R16
	RET


DESACTIVARALARMA:
	CBI PORTB, PB5
	RJMP LOOP 	

ACTIVARALARMA:
	SBI PORTB, PB5
	RJMP LOOP



