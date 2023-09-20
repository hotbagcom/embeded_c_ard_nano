/*
 * main.c
 *
 * Created: 9/9/2023 11:09:40 PM
 *  Author: tnc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRC = 0x0f;           // Configure PORTC as output

	while(1)
	{
		PORTC = 0xf;        // Turn ON the Buzzer conneted to PORTC
		_delay_ms(500);      // Wait for some time
		PORTC = 0x0;        // Turn OFF the Buzzer connected to PORTC
		_delay_ms(500);      // Wait for some time
	}
	return 0;
}