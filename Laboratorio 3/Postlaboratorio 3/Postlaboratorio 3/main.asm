;************************************************************************
; Universidad del Valle de Guatemala
; Programación de microcontroladores
; Laboratorio2.asm
; Autor : Boris Moreira
; Carné 22550
; Proyecto: Postlaboratorio 3
; Hardware: Atmega328P
; Creado: 16/02/2024
; Última modificación: 20/02/2023 00:47 am
;************************************************************************
;Encabezado
;************************************************************************
.include "M328PDEF.INC"
.cseg
.org 0x0000			; Vector RESET
	JMP MAIN 
.org 0x0008			; Vector de interrupción  PIN CHANGE 
	JMP ISR_PCINT1
.org 0x0020			; Vector: Overflow Timer0
	JMP ISR_T0_OVF 


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

TABLA7SEGMENTOS: .DB 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47 	


;************************************************************************
; CONFIGURACIÓN
;************************************************************************
	LDI R16, 0x00
	STS UCSR0B, R16 ; Deshabilita a RX y TX del puerto D

	LDI ZH, HIGH(TABLA7SEGMENTOS <<1)
	LDI ZL, LOW(TABLA7SEGMENTOS << 1)
	
	; I/O para Contador binario
	SBI PORTB, PB0	;Habilitando pull-up en PB0
	CBI DDRB, PB0	;Habilitando PB0 como entrada
	SBI PORTB, PB1	;Habilitando pull-up en PB1
	CBI DDRB, PB1	;Habilitando PB1 como entrada

	LDI R16, (1<<PC0)|(1<<PC1)|(1<<PC2)|(1<<PC3)
	OUT DDRC, R16	;Habilitando PC0 - PC3 como salidas del contador binario
	LDI R16, (0<<PC0)|(0<<PC1)|(0<<PC2)|(0<<PC3)
	OUT PORTC, R16	;Apagando los pines

	;I/O para Contadores hexadecimales 
	SBI DDRB, PB2
	CBI PORTB, PB2
	SBI DDRB, PB3
	CBI PORTB, PB3	; Habilitando salidas de los dos transistores

	LDI R16, 0xFF
	OUT DDRD, R16
	LDI R16, 0x00
	OUT PORTD, R16	; Habilitando salidas para los displays de 7 segmentos

	;Configuración del Timer0
	LDI R16, 0x00
	OUT TCCR0A, R16			;Coloca el timer en modo de operación normal
	LDI R16, 0b0000_0101	; pone en 1 los bits CS02 y CS00 para colocar un prescaler en 1024
	OUT TCCR0B, R16 
	LDI R16, 98				; valor de comparación
	OUT TCNT0, R16 	

	LDI R16, (1<<TOIE0)		;Habilito interrupción
	STS TIMSK0, R16	; Configuración de Timer0

	;Prescaler
	LDI R16, (1 << CLKPCE) ; Se habilita el clock prescaler
	STS CLKPR, R16 
	LDI R16, 0b0000_0101 ; se define la frecuencia de 1MHz
	STS CLKPR, R16

	
	;Configuración de Pin Change
	LDI R16, (1<<PCIE0)
	STS PCICR, R16							;Se habilita los puertos B como interrupciones
	LDI R16, (1<<PCINT0)|(1<<PCINT1)		
	STS PCMSK0, R16							; Habilitando PCINT en los pines PB0 y PB1.

	.DEF CONTADOR1 = R18
	.DEF CONTADOR2 = R22

	SEI					;Se inician las interrupciones globales
;************************************************************************
;LOOP PRINCIPAL 
;************************************************************************

LOOP:
	; PIN CHANGE
	OUT		PORTC, R20		;Muestra contador binario

	;TIMER0 OVERFLOW
	;SIETE_SEGMENTOS:
	;PB3 ES PARA EL CONTADOR DE SEGUNDOS
	;PB2 ES PARA EL CONTADOR DE DECENAS

	;Segundero
	SBI		PORTB, PB3 
	LDI		ZL, LOW(TABLA7SEGMENTOS << 1)
	LDI		ZH, HIGH(TABLA7SEGMENTOS << 1) 
	ADD		ZL, R17
	LPM		CONTADOR1, Z
	OUT		PORTD, CONTADOR1
	RCALL	Delay
	CBI		PORTB, PB3

	
	;Decenas
	SBI		PORTB, PB2
	LDI		ZL, LOW(TABLA7SEGMENTOS << 1)
	ADD		ZL, R21
	LPM		CONTADOR2, Z
	OUT		PORTD, CONTADOR2
	RCALL	Delay
	CBI		PORTB, PB2
	
	
	;lo demas...
	;INC		R17
	CPI		R17, 0b0000_1010
	BREQ	REINICIAR
	
	CPI		R19, 100
	BRNE	LOOP
	CLR		R19

	RJMP	LOOP
	
	REINICIAR: 
		LDI R17, 0x00
		LDI ZL, LOW(TABLA7SEGMENTOS << 1)
		INC R21
		CPI R21, 0b0000_1010
		BREQ REINICIAR_2
		RJMP LOOP

	REINICIAR_2:
		LDI R21, 0x00
		RJMP LOOP
		
;***********************************************************
;SUBRUTINAS DE INTERRUPCIÓN (ISR) DE PIN CHANGE: 
;***********************************************************
ISR_PCINT1:
	PUSH R16	;Guarda en pila lo que tenga R16
	IN R16, SREG
	PUSH R16	;Guarda en pila el registro SREG

	IN R16, PINB
	
	SBRC R16, PB0
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
;SUBRUTINAS DE INTERRUPCIÓN (ISR) DE TIMER0 OVERFLOW: 
;***********************************************************
ISR_T0_OVF:
	PUSH R16
	IN R16, SREG
	PUSH R16

	LDI R16, 50
	OUT TCNT0, R16
	SBI TIFR0, TOV0
	INC R19

	;SBI PINC, PC0
	INC R17

	
	POP R16
	OUT SREG, R16
	POP R16
	RETI 

	RET 

Delay:
	LDI R16, 100 
	delaydeespera:
		DEC R16
		BRNE delaydeespera
	RET

