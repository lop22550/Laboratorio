;************************************************************************
; Universidad del Valle de Guatemala
; Programación de microcontroladores
; Prelaboratorio2.asm
; Autor : Boris Moreira
; Carné 22550
; Hardware: Atmega328P
; Creado: 6/02/2024
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

	
	LDI R16, 0b0000_1111
	OUT DDRC, R16
	LDI R16, 0x00
	OUT PORTC, R16

	SBI DDRB, PB5
	CBI PORTB, PB5

;LOOP
LOOP:
	IN		R17, TIFR0 ;registro de flags 
	SBRC	R17, OCF0A
	RJMP	TOOGLE
	RJMP	LOOP
 TOOGLE:
	SBI		PINB, PB5
	INC R20
	OUT PORTC, R20
	SBI		TIFR0, OCF0A
	RJMP	LOOP