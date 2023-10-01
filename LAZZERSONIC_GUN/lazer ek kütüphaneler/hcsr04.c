/*
 * hcsr04.c
 *
 * Created: 25.09.2023 19:22:11
 *  Author: tnc
 */ 
#include "uart_hal_v0.h"
#include "hcsr04.h"  
  
#define  F_CPU 16000000UL
#define _TRIG (1<<4)
#define _ECHO (1<<3)
#define U_S PORTD
#define U_S_ DDRD
#define prescallar 0x02 //pres=8  CS10
#define risefall 0x04  //rising=1  falling=0 

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint8_t print_dist[40] ;
uint32_t buffer_value[20];
 volatile uint32_t sample;
 volatile uint32_t delta_time ;
 volatile uint16_t travel_time ;
 volatile uint16_t start_time ;
 volatile uint16_t finish_time;
 volatile short allow_trig_send;
 volatile short allow_echo_read;

// void Ultros_Init();
// void Ultros_Trig();
// void Init_time();
// void Clear_time();
// uint16_t filter();
// void interr_1_Init();


ISR(INT1_vect){
	//EICRA
	if (((TCCR1B & risefall)>>ISC10)==0)//falling edge start timer
	{
		finish_time =TCNT1;
		
		if (start_time>finish_time)
			travel_time = (0xFFFF)-(start_time-finish_time)
		else
			travel_time = (finish_time-start_time);
		
		
		allow_trig_send=0x01;
		EICRA |= 3<<ISC10;//makes rising
		
	}
	
	else if (((TCCR1B & risefall)>>ISC10)==1)		//rising edge start timer
	{
		finish_time=0x00;
		TCNT1 =0x00;
		EICRA &= ~(1<<ISC10);//makes falling
		start_time =TCNT1;
		allow_trig_send=0x01;
	}
	
	
}

void interr_1_Init(){
	EIMSK =0x02;
	EICRA |= 3<<ISC10;//rising 2<<ISC10;//falling edge
}

	
void Ultros_Init()
{
	U_S_ |= _TRIG;
	U_S_ &= ~_ECHO;
	U_S &= ~_TRIG;
	U_S &= ~_ECHO;
	sample = 0;
	interr_1_Init();
	allow_trig_send=0x01;
}


void Ultros_Trig()
{
	U_S |= _TRIG;
	_delay_us(10);
	U_S &= ~_TRIG;
	allow_trig_send=0x00;
}

void Clear_time()
{
	travel_time=0x00;
	start_time=0x00;
	finish_time=0x00;
	TCNT1 =0x00;
}

void Init_time()
{
	//define timer thing set
	TCCR1A= 0x00;
	TCCR1B |= prescallar;
	TCCR1C=0x00;
	//TIMSK1 |= 1<<ICIE1 ; // | 1<<TOIE1  enable overflow interrupt
}
uint16_t filter(){
	uint32_t sum=0;
	
	for (int ii =0; ii<10;ii++)
	{
		sum += buffer_value[ii];
	}
	allow_echo_read =0x01;
	return (uint16_t)(sum/10);
}

void Ultros_D_C(){
	if (sample>=10)
	{
		distance_mm =filter();
		
		memset(print_dist,0,sizeof(print_dist));
		
		sprintf((char*)print_dist,"distance: %u  mm  _  %lu tick  \n \r",distance_mm,travel_time);
		//Led_Flash(cycle_tick_times);
		uart_send_string(print_dist);
		sample=0x00;
		allow_echo_read=0x00;
	}
	else
	{
		travel_distance = travel_time/12;
		buffer_value[sample]=(int)travel_distance;
		sample++;
	}
}


void Ultros_main()
{
	if (allow_trig_send )
	{
		
		Ultros_D_C();
		//Clear_time();
		Ultros_Trig();
		
	}
}