/*
 * PWM_.h
 *
 * Created: 14.05.2024 10:10:36
 *  Author: Arif
 */ 


#ifndef PWM__H_
#define PWM__H_
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"


int pwmdef_freq(ui8 timtype, ui16 cyclefreq);
void pwmdef_pin(ui8 pin);
i8 pwminit(ui8 timtype, char timpin);



void setTCNT0(ui8 tcnt0);
void setOCR0A(ui8 ocr0a);
void setOCR0B(ui8 ocr0b);
ui8 getTCNT0(void);
void setICR1(ui16 icr1);
void setOCR1A(ui16 ocr1a);
void setOCR1B(ui16 ocr1b);
ui16 getTCNT1(void);
void setTCNT2(ui8 tcnt2);
void setOCR2A(ui8 ocr2a);
void setOCR2B(ui8 ocr2b);
ui8 getTCNT2(void);




#endif /* PWM__H_ */