/*
 * hcsr04.c
 *
 * Created: 25.09.2023 19:22:11
 *  Author: tnc
 */ 
#define  F_CPU 16000000UL
#define _TRIG (1<<4)
#define _ECHO (1<<3)
#define U_S PORTD
#define U_S_ DDRD
#define prescallar 0x02 //pres=8  CS10
#define risefall 0x04  //rising=1  falling=0  

#include "uart_hal_v0.h"
#include "hcsr04.h"  
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//C:\Program Files\Microchip\xc8\v2.36\avr\avr\include\bits\alltypes.h



uint8_t print_dist[40] ;
uint32_t buffer_value[40];
volatile uint8_t sample;
uint8_t sample_max=10;
volatile uint32_t delta_time ;
volatile uint16_t travel_time ;
volatile uint16_t start_time ;
volatile uint16_t finish_time;
volatile short allow_trig_send;
volatile short allow_echo_read;
volatile uint16_t distance_mm[16];
volatile short sect_mm ;
volatile short sect_sg;
volatile uint32_t travel_distance;
uint16_t max_time;


//not use
void Clear_time()
{
	travel_time=0x00;
	start_time=0x00;
	finish_time=0x00;
	TCNT1 =0x00;
}
ISR(INT1_vect){
	//EICRA

		finish_time =TCNT1;
		
		if (start_time>finish_time){
			
			if ((TCCR1B>>ICES1)&0x1)
				travel_time = (max_time)-(start_time-finish_time);
			else
				travel_time = (0xFFFF)-(start_time-finish_time);
			
		}
		else
		travel_time = (finish_time-start_time);
		
		sect_mm=sect_sg;
		allow_trig_send=0x01;
	
}

void interr_1_Init(){//servo ayarýndan sonraya koy
	EIMSK =0x02;
	EICRA |= 2<<ISC10;//falling edge 3<<ISC10;//rising 
	max_time=(ICR1H<<8)|ICR1L ; // 14. wave genaeration modu seçtiðini kabul ediyorum umarým öðledir
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
	allow_echo_read =0x0;
	
	uart_init(9600,0);
}

void Ultros_Trig()
{
	U_S |= _TRIG;
	_delay_us(10);
	U_S &= ~_TRIG;
	TCNT1=0x0;
	start_time =TCNT1;
	allow_trig_send=0x00;
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
	//aritmatik ortalamasýný kullanan filtre pek etkili deðil
	
	for (int ii =0; ii<sample_max;ii++)
	{
		sum = sum*700 + 300*buffer_value[ii];
		sum/=1000;
		
	}
	sample=0x0;
	return (uint16_t)(sum);
}

void Ultros_D_C(){
	if (sample>=sample_max)
	{
		distance_mm[sect_mm] =filter();
		
				memset(print_dist,0,sizeof(print_dist));
				sprintf((char*)print_dist,"distance: %u  mm  _  %lu tick  \n \r",distance_mm[0],travel_distance);
		//Led_Flash(cycle_tick_times);
				uart_send_string(print_dist);
		
		allow_echo_read =0x1;
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
		
		
		Ultros_Trig();
		
	}
}

