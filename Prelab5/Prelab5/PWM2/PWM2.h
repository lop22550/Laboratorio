/*
 * PWM2.h
 *
 * Created: 17/04/2024 16:54:36
 *  Author: bjosu
 */ 


#ifndef PWM2_H_
#define PWM2_H_

#include <avr/io.h>
#include <stdint.h>

#define invertido 1
#define no_invertido 0
#define MIN2 9
//#define MAX2 

void initPWM2 (void);
void updateDutyCycle2(uint8_t valor_ADC);

#endif /* PWM2_H_ */