;************************************************************************
; Universidad del Valle de Guatemala
; Programación de microcontroladores
; Laboratorio2.asm
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
.org 0x00	; Vector RESET
	JMP MAIN 
.org 0x02	; Vector de interrupción INT0
	JMP ISR_INT0
.org 0x0020 ; Vector: Overflow Timer0


;************************************************************************
;Stack pointer
;************************************************************************
; Configuración de los espacios a utilizar de la RAM
LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R16, HIGH(RAMEND)
OUT SPH, R16

;Tabulación de datos del 7 segmentos 


;************************************************************************
; CONFIGURACIÓN
;************************************************************************
Setup: 
	LDI R16, (1 << CLKPCE) ; Se habilita el clock prescaler
	STS CLKPR, R16 
	LDI R16, 0b0000_0101 ; se define la frecuencia de 1MHz
	STS CLKPR, R16

	;Configuración del Timer0
	LDI R16, 0x00
	OUT TCCR0A, R16 ;Coloca el timer en modo de operación normal
	LDI R16, 0b0000_0101 ; pone en 1 los bits CS02 y CS00 para colocar un prescaler en 1024
	OUT TCCR0B, R16 
	LDI R16, 98 ; valor de comparación
	OUT OCR0A, R16 

	;ENTRADAS
	LDI R16, 0b0000_1111
	OUT DDRC, R16
	LDI R16, 0b0011_0000
	OUT PORTC, R16  ; activa los pull up en los pines pc4 y pc5
	
	;SALIDAS
	LDI R16, 0xFF
	OUT DDRD, R16
	LDI R16, 0x00
	OUT PORTD, R16
	LDI R16, 0xFF
	OUT DDRB, R16
	LDI R16, 0x00
	OUT PORTD, R16

	LDI R16, 0x00
	STS UCSR0B, R16

	.DEF CONTADOR=R18
	.DEF ENTRADAS=R23

	;CONFIGURACIÓN DE INTERRUPCIONES
	LDI R16, (1<<ISC10)|(1<<ISC11) 
	STS EICRA, R16 ; HABILITA EN EL FLANCO DE SUBIDA

	SBI EIMSK, INT0 ; HABILITA LA INTERRUPCIÓN 

	


	TABLA7SEGMENTOS: .DB 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47 

	SEI
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

	RJMP	LOOP

;***********************************************************
;SUBRUTINAS IMPORTANTES: 
;***********************************************************

 TOOGLE:
	INC R20
	OUT PORTC, R20
	SBI		TIFR0, OCF0A
	/*INC CONTADOR
	CPI	CONTADOR, 0x10
	BRNE PASO_TM0
	LDI	CONTADOR, 0x00 
PASO_TM0:
	CALL MAIN*/
	RJMP	LOOP


INCREMENTAR: 
	CALL ANTIREBOTE
	INC CONTADOR
	CPI	CONTADOR, 0x10
	BRNE PASO_INC
	LDI	CONTADOR, 0x00 
PASO_INC:
	CALL MAIN
	RET

DECREMENTAR: 
	CALL ANTIREBOTE
	DEC CONTADOR
	CPI	CONTADOR, 0xFF
	BRNE PASO_DEC
	LDI	CONTADOR, 0x0F 
PASO_DEC:
	CALL MAIN
	RET

ANTIREBOTE: 
	LDI R19, 255
	DELAY: 
		DEC R19
		BRNE DELAY
	RET 
		
MAIN: 
	LDI ZH, HIGH(TABLA7SEGMENTOS <<1)
	LDI ZL, LOW(TABLA7SEGMENTOS << 1)
	ADD ZL, CONTADOR
	LPM R16, Z
	OUT PORTD, R16
	/*
	MOV R21, R16
	MOV R22, R16
	
	LSL R21
	LSL R21
	MOV R21, R16

	SWAP R22
	LSR R22
	

	OUT PORTD, R21
	OUT PORTB, R22
	*/
	
	RET

ISR_INT0:
	

	RETI

































































































































































































































































































































































































































































































































































































































































































































































































































































