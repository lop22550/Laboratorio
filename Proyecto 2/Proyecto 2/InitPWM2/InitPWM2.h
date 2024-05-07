/*
 * InitPWM2.h
 *
 * Created: 7/05/2024 14:50:21
 *  Author: bjosu
 */ 


#ifndef INITPWM2_H_
#define INITPWM2_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define MIN2 9
#define MAX2 30

void initPWM2 (void);
uint8_t updateDutyCycle2A(uint8_t duty3);
uint8_t updateDutyCycle2B(uint8_t duty4);



#endif /* INITPWM2_H_ */