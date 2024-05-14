/*
 * PWM_.h
 *
 * Created: 14.05.2024 10:10:36
 *  Author: Arif
 */ 


#ifndef PWM__H_
#define PWM__H_
#ifndef
#define F_CPU 16000000UL
#endif

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "config.h"


int pwmdef_freq(ui8 timtype, ui16 cyclefreq);
void pwmdef_pin(ui8 pin);
i8 pwminit(ui8 timtype, char timpin);





#endif /* PWM__H_ */