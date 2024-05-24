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

//Se definen los valores m�ximos, m�nimos y de tope calculados. 
#define PWM_TOP1 156 
#define MIN1 3
#define MAX1 45

//Se definen los prototipos de funci�n de la librer�a. 
void initPWM1 (void);
uint8_t updateDutyCycle1A(uint8_t duty1);
uint8_t updateDutyCycle1B(uint8_t duty2);


#endif /* INITPWM1_H_ */