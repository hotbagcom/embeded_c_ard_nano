/*
 * main.c
 *
 * Created: 9/15/2023 11:36:43 AM
 *  Author: tnc
 */ 
#define  F_CPU 16000000UL
#define prescallar 0x01
#define L__ DDRD
#define L_ PORTD
#define L(x) L_=x<<4



#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uart_hal.h"

void float_(char iii);
void int_(char iii);
void hybrid_(char iii);

static volatile uint16_t start_time ;
static volatile uint16_t finish_time;
static volatile uint16_t a_s ;
static volatile uint16_t a_f;
static volatile uint16_t b_s ;
static volatile uint16_t b_f;
static volatile uint16_t h_s ;
static volatile uint16_t h_f;

float a_1=15.564,a_2=217.956,a_3;
uint16_t b_1=15,b_2=217,b_3;
uint32_t h_1=(15.564)*64,h_2=(217.956)*64,h_3;
char clc[5]={'+' , '-' , '*' , '/' , '%' };
uint8_t print_dist[40] ;
int main(void)
{
	
		L__ =0xF0;
		L_ =0xF0;
		short bufer_mem;
		
		TCCR1A= 0x00;  //normal mode operation
		TCCR1B= prescallar;///set prescallar as 1
		TCCR1C=0x00;
	
		uart_init(9600,0);
		start_time=0x00;
		finish_time=0x00;
		TCNT1 =0x00;
		start_time =TCNT1;
		sei();
		L_ =0x00;;
		for (bufer_mem=0 ; bufer_mem <((sizeof(clc))/(sizeof(clc[0]))) ; bufer_mem++)
		{
			_delay_ms(50);
			
		float_(clc[bufer_mem]);
		int_(clc[bufer_mem]);
		hybrid_(clc[bufer_mem]);
		
		
		memset(print_dist,0,sizeof(print_dist));
		sprintf((char*)print_dist,
		"  operation %c  \n \r"
		,clc[bufer_mem] );
		uart_send_string(print_dist);
		
		memset(print_dist,0,sizeof(print_dist));
		sprintf((char*)print_dist,
		"  %u tick  %f : %f : %f  \n \r"
		,(a_f-a_s),(a_1),(a_2),(a_3) );
		uart_send_string(print_dist);	
		
		memset(print_dist,0,sizeof(print_dist));
		sprintf((char*)print_dist,
		"  %u tick  %d : %d : %d  \n \r"
		,(b_f-b_s),(b_1),(b_2),(b_3) );
		uart_send_string(print_dist);
		
		memset(print_dist,0,sizeof(print_dist));
		sprintf((char*)print_dist,
		"  %u tick  %lu : %lu : %lu  \n \r"
		,(h_f-h_s),(h_1),(h_2),(h_3) );
		uart_send_string(print_dist);
		
		memset(print_dist,0,sizeof(print_dist));
		sprintf((char*)print_dist,
		" --------------------  \n \r");
		uart_send_string(print_dist);
		}
			
			
			
						
		
		 
}

void float_(char iii){
	TCNT1=0x00;
	a_s=TCNT1;
	switch(iii){
		case '+':a_3=a_2+a_1;break;
		case '-': a_3=a_2-a_1;break;
		case '*':a_3=a_2-a_1; break;
		case '/': a_3= a_2/a_1;break;
		case '%':a_3= 121;break;
		default :float_(++iii);
	}
	a_f=TCNT1;
}
void int_(char iii){
	TCNT1=0x00;
	b_s=TCNT1;
	switch(iii){
		case '+':b_3=b_2+b_1;break;
		case '-': b_3=b_2-b_1;break;
		case '*':b_3=b_2-b_1; break;
		case '/': b_3= b_2/b_1;break;
		case '%':b_3=b_2%b_1;break;
		default :int_(++iii);
	}
	b_f=TCNT1;
	
}
void hybrid_(char iii){
	TCNT1=0x00;
	h_s=TCNT1;
	switch(iii){
		case '+':h_3=h_2+h_1;break;
		case '-': h_3=h_2-h_1;break;
		case '*':h_3=h_2-h_1; break;
		case '/': h_3= h_2/h_1;break;
		case '%':h_3=h_2%h_1;break;
		default :hybrid_(++iii);
	}
	h_f=TCNT1;
}