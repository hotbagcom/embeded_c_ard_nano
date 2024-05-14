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

#ifndef
#define F_CPU 16000000UL
#endif


#include <xc.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PWM_.h"


int main(void)
{
    //use the PWM_ functions
	
	
    while (1) 
    {
    }
}

