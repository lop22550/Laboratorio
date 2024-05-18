/*
 * InitPWM1.h
 *
 * Created: 7/05/2024 14:00:27
 *  Author: bjosu
 */ 


#ifndef INITPWM1_H_
#define INITPWM1_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define PWM_TOP1 156 
#define MIN1 3
#define MAX1 45

void initPWM1 (void);
uint8_t updateDutyCycle1A(uint8_t duty1);
uint8_t updateDutyCycle1B(uint8_t duty2);


#endif /* INITPWM1_H_ */