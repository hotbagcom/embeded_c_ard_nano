
#ifndef 
#define F_CPU 16000000UL
#endif


#include <xc.h>
#include <util/delay.h>
#include <avr/io.h>
//#include "avr/iom328p.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adc_hal_v2.h"


//uint8_t printfbuffer[64] = {0};
uint16_t convert = 0;// Store the analog to digital value 
int main(void)
{
	
    
	DDRD =0xF0;
	DDRB =0x3F;
	
	adc_init();
	adc_pin_enable(ADC0_PIN);
	adc_pin_sellect(ADC0_PIN);
	
	sei();
	
	while(1)
    {
		convert = adc_convert(); // Store the analog to digital value 

		
		PORTD = (PORTD & 0x0F) & ((convert & 0x00F)<<4);
		PORTB = (PORTB & 0xC0) & ((convert & 0x3F0)>>4);
		
		_delay_ms(700);
    }
}