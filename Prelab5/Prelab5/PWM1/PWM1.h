/*
 * PWM1.h
 *
 * Created: 16/04/2024 21:53:02
 *  Author: bjosu
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define invertido 1
#define no_invertido 0
#define PWM_TOP 311
#define MIN1 9
#define MAX1 30

void initPWM1A (void);
void updateDutyCycleA1(uint8_t valor_ADC1);
void Senal ();



#endif /* PWM1_H_ */