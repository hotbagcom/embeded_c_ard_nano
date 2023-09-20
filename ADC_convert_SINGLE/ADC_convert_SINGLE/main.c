/*
 * main.c
 *
 * Created: 8/30/2023 5:29:41 PM
 *  Author: tnc
 */ 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart_hal_v0.h"
#include "adc_hal_v3.h"

uint8_t print_buffer[64] = {0};
	


int main(void)
{
	const uint8_t start[] = "\n\rProgram Start\n\r";

	uint16_t convert = 0;
	
	DDRD = 0xF0;	
	DDRB |= 0x3F;
	uart_init(9600,0);
	
	adc_init();
	adc_pin_enable(ADC0_PIN);
	adc_pin_select(ADC0_PIN);


	sei();
	uart_send_string((uint8_t*)start);

	
	while (1)
	{
		
		convert = adc_convert();
		float volts = ADC_VOLT(convert);
		memset(print_buffer,0,sizeof(print_buffer));
		sprintf((char*)print_buffer,"ADC res:0x%04x V:%0.2f \r",convert,volts);
		uart_send_string(print_buffer);
		
		PORTD = (PORTD & 0xF )| ((convert & 0xF)<<4);
		PORTB = (PORTB & 0xC0) | ((convert & 0x3F0 )>>4);	 
		_delay_ms(100);
	}
}

