/*
 * main.c
 *
 * Created: 8/31/2023 4:04:41 PM
 *  Author: tnc
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <xc.h>
#include "config.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "adc_hal_v3.h"
#include "uart_hal_v0.h"


 uint8_t print_buffer[64] = {0};


int main(void)
{
	DDRC &= 0x0F;
	const uint8_t start[] = "\n\rProgram Start\n\r";
	DDRD = 0xF0;
	DDRB |= 0x3F;
	uint16_t convert = 0;
		
	uart_init(9600,0);
	
	adc_init_rig();
	
	sei();
	uart_send_string((uint8_t*)start);
	
    while(1)
    {
        adc_average_buff(1);
        memset(print_buffer,0,sizeof(print_buffer));
		convert=getJoy1x();
		sprintf((char*)print_buffer,"ADC0=:0x%04x - ADC0=%d\r",convert,convert );
		PORTD = (PORTD & 0xF )| ((convert & 0xF)<<4);
		PORTB = (PORTB & 0xC0) | ((convert & 0x3F0 )>>4);
        uart_send_string(print_buffer); 
		_delay_ms(50);
		
    }///yazdýrma sýrasý farklý önce sonu sonra 1 i sonra 2 yi þeklinde
}