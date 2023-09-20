/*
 * main.c
 *
 * Created: 9/12/2023 3:35:51 PM
 *  Author: tnc
 */ 

#define  F_CPU 16000000UL
#define _TRIG (1<<4)
#define _ECHO_0 (1<<2)
#define _ECHO_1 (1<<3)
#define U_S PORTD
#define U_S_ DDRD
#define _0 (1<<0)
#define _1 (1<<1)
#define _2 (1<<2)
#define _3 (1<<3)
#define _4 (1<<4)
#define _5 (1<<5)
#define L_ PORTC
#define L__ DDRC
#define prescaller 0x02

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uart_hal.h"


uint8_t print_dist[40] ;
static volatile short allov_trig_send;
static volatile short allov_echo_read;
static volatile uint32_t delta_time ;
static volatile uint16_t travel_time ;
static volatile uint16_t start_time ;
static volatile uint16_t finish_time;
unsigned char secound_tour;


ISR(INT0_vect){///rising edge 
	
	start_time=0x00;
	finish_time=0x00;
	
	TCNT1 =0x00;
	
	
	start_time =TCNT1;
	
	
}
ISR(INT1_vect){//falling edge
	
	

	
	finish_time =TCNT1;
	
	 if (finish_time<=start_time)
	 {
		 travel_time = ((2^16)-1)-(start_time-finish_time);
		 
	 }
	 else if(finish_time>start_time)
	 {
		 travel_time = (finish_time-start_time);
		 
	 }
	
	
	TCCR1B=0x00;
	allov_trig_send=0x01;
	secound_tour =0x01;
	
}


int main(void)
{
	
	///ddr1 1 mean is that pin is output from MCU
	U_S_ =0b10000;//Pd4 output toggle for 10us
	U_S =0x00;
	
	//led on off
	L__ =0b111111;
	L_ = 0x3F;
	_delay_ms(300);
	L_ = 0x00;
	_delay_ms(500);
	
	uart_init(9600,0);
	
	//define external interrupt
	EIMSK = 1<<INT1 | 1<<INT0 ; //activate int1  and int0   ---permanent
	
	
	EICRA = 0b1011; // INT1 falling active ,INT0 rising active ---permanent
	
	//define timer thing set 
	TCCR1A= 0x00;
	TCCR1B= prescaller;///set prescaller as 8
	TCCR1C=0x00;
	
	allov_trig_send =0x01;//give permission to toggle trig
	secound_tour =0;
	sei();
    while(1)
    {

	 
		   if (secound_tour>0x00)
		   {
			  
			   
			   memset(print_dist,0,sizeof(print_dist));
			   sprintf((char*)print_dist,"start : %u and finish : %u --distance: %u  ticks   \n \r",start_time, finish_time,travel_time);
			   uart_send_string(print_dist);
			   
			   
			   
		   }
		   
		   _delay_ms(500);
		   U_S |= _TRIG;
		   _delay_us(15);
		   U_S &=~_TRIG;
		   TCCR1B= prescaller;
		  
		   allov_trig_send=0x00;
	

	   
	   
	   
	   
		
	   
	   
    }
}