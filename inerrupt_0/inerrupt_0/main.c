/*
 * main.c
 *
 * Created: 8/25/2023 4:38:51 PM
 *  Author: tnc
 */ 
#ifndef F_CPU
#define F_CPU 16000000L // 16MHz clock speed
#endif

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
uint8_t toogle ;
ISR(INT0_vect){
	PORTB = toogle<<5;
	toogle = ~toogle; 
	//EIFR &= ~(0x1<<INTF0);
}
ISR(INT1_vect){
	PORTB = toogle<<5;
	toogle= ~toogle;
	//EIFR &=  ~(0x1<<INTF1); 
}

int main(void)
{
	//pd2 in button
	DDRD = 0xFC; //0b1111 0011
	//pb5 out led 
	DDRB  =0x20; //0b0010 0000
	
	EICRA = 0x06; //masks ISC11 and ISC10  ISC01 and ISC00 falling edge interupt 0b (INT1)01[any logical change] (INT0)10[_falling]
	EIMSK = 0x03; // 
	sei();
	
	toogle =0x1;
	while (1)
	{
	}
}