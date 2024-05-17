/*
 * Drive_a_motor_pwm.c
 *
 * Created: 14.05.2024 09:48:03
 * Author : Arif
 */ 

// BTS7960B max 25kHz
// define the pins
// enable pins 7 and 8
// pwm pins 5 and 6
// timer count pins 9 , 10
// ineterrupt pins 2 and 3 to count use timer 1
//use tx and rx for 0 and 1


#define F_CPU 16000000UL



#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "PWM_.h"

static ui32 periode = 0;
static ui8 position = 0; //0-256
ISR(INT0_vect){//activate falling edge of input signal (when you release push button)
	periode=getTCNT1();
	
}
ISR(INT1_vect){//activate any logic chance of input signal
	position = periode - getTCNT1();
}

int main(void)
{
	
    //use the PWM_ functions
	pwmdef_freq(0,25000);
	pwmdef_pin(5);
	
	//pd2/pd3 //extrnal interrupt
	DDRD |= 0b11<<2;
	EICRA = 0x06;//0b0000 0110
	//masks ISC11 and ISC10  ISC01 and ISC00 .
	//ISC1x is logic change interrupt
	//ISC0x is falling edge interrupt
	//0b (INT1)01[any logical change] (INT0)10[_falling]
		
	EIMSK = 0x03; // enable int1 and int0
	//Enable interrupt mask register
		
	sei();
		
	
	
	setOCR0B(20);
	
    while (1) 
    {
    }
}

