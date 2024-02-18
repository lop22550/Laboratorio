;************************************************************************
; Universidad del Valle de Guatemala
; Programación de microcontroladores
; Laboratorio2.asm
; Autor : Boris Moreira
; Carné 22550
; Proyecto: Postlaboratorio 3
; Hardware: Atmega328P
; Creado: 16/02/2024
;************************************************************************
;Encabezado
;************************************************************************
.include "M328PDEF.INC"
.cseg
.org 0x0000			; Vector RESET
	JMP MAIN 
.org 0x0008			; Vector de interrupción  PIN CHANGE 1 (Puertos C)
	JMP ISR_PCINT1
.org 0x0020			; Vector: Overflow Timer0
	JMP 

MAIN: 
;************************************************************************
;Stack pointer
;************************************************************************
; Configuración de los espacios a utilizar de la RAM
Setup: 
LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R16, HIGH(RAMEND)
OUT SPH, R16


;************************************************************************
; CONFIGURACIÓN
;************************************************************************
	LDI R16, 0x00
	STS UCSR0B, R16 ; Deshabilita a RX y TX del puerto D
	
	;Contador binario
	SBI PORTC, PC4	;Habilitando pull-up en PC4
	CBI DDRC, PC4	;Habilitando PC4 como entrada
	SBI PORTC, PC5
	CBI DDRC, PC5

	LDI R16, (1<<PC0)|(1<<PC1)|(1<<PC2)|(1<<PC3)
	OUT DDRC, R16	;Habilitando PC0 - PC3 como salidas del contador binario
	LDI R16, (0<<PC0)|(0<<PC1)|(0<<PC2)|(0<<PC3)
	OUT PORTC, R16	;Apagando los pines

	;CALL Init_T0	; Configuración de Timer0

	;Prescaler
	LDI R16, (1 << CLKPCE) ; Se habilita el clock prescaler
	STS CLKPR, R16 
	LDI R16, 0b0000_0101 ; se define la frecuencia de 1MHz
	STS CLKPR, R16

	
	;Configuración de Pin Change
	LDI R16, (1<<PCIE1)
	STS PCICR, R16
	LDI R16, (1<<PCINT12)|(1<<PCINT13)		; Habilitando PCINT en los pines PC4 y PC5.
	STS PCMSK1, R16							; Se configuran los pines PC4 y PC5 para detectar un cambio.

	
	TABLA7SEGMENTOS: .DB 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47 

	SEI					;Se inician las interrupciones globales
;************************************************************************
;LOOP PRINCIPAL 
;************************************************************************

LOOP:
	OUT PORTD, R20		;Muestra contador binario


	;IN		R17, TIFR0 ;registro de flags 
	;SBRC	R17, OCF0A

	RJMP	LOOP


;***********************************************************
;SUBRUTINAS DE INTERRUPCIÓN (ISR): 
;***********************************************************
ISR_PCINT1:
	PUSH R16	;Guarda en pila lo que tenga R16
	IN R16, SREG
	PUSH R16	;Guarda en pila el registro SREG

	IN R16, PINC 
	
	SBRC R16, PC0
	RJMP DECREMENTAR
	INC R20 
	SBRC R20, 4 
	CLR R20
	RJMP SALTAR

	DECREMENTAR: 
	SBRS R16, PC1
	DEC R20
	SBRC R20, 4
	LDI R20, 0x0F
	RJMP SALTAR 

	SALTAR: 
	SBI PCIFR, PCIF0
	POP R16
	OUT SREG, R16
	POP R16
	RETI
;***********************************************************
;SUBRUTINAS IMPORTANTES: 
;***********************************************************

;Inicialización del Timer 0
Init_T0:
	;Configuración del Timer0
	LDI R16, 0x00
	OUT TCCR0A, R16			;Coloca el timer en modo de operación normal
	LDI R16, 0b0000_0101	; pone en 1 los bits CS02 y CS00 para colocar un prescaler en 1024
	OUT TCCR0B, R16 
	LDI R16, 98				; valor de comparación
	OUT OCR0A, R16 	


		
MAIN: 
	LDI ZH, HIGH(TABLA7SEGMENTOS <<1)
	LDI ZL, LOW(TABLA7SEGMENTOS << 1)
	ADD ZL, CONTADOR
	LPM R16, Z
	OUT PORTD, R16
	
	RET

































































































































































































































































































































































































































































































































































































































































































































































































































































