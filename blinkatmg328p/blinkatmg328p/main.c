/*
 * main.c
 *
 * Created: 8/23/2023 12:27:25 PM
 *  Author: tnc
 */ 
#ifndef F_CPU
#define F_CPU 16000000L // 16MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0xFF; //Makes PORTC as Output
	PORTC = 0x00; //Makes all PORTC Pins LOW 
	 
    while(1)
    {
		 //Turns ON  LEDs
		 PORTC = 0x21; //12  ( 0b10 0001)
		 _delay_ms(1000); //1 second delay
		PORTC = 0x03;  //0b0011
		_delay_ms(1000); //1 second delay
		PORTC = 0x06; //0b 0110
		_delay_ms(1000); //1 second delay
		PORTC = 0x0C; //12  0b0000 1100
		_delay_ms(1000); //1 second delay
		PORTC = 0x18; //12  0b0001 1000
		_delay_ms(1000); //1 second delay
		PORTC = 0x30; //12  0b0011 0000
		_delay_ms(1000); //1 second delay
		
    }
}