/*
 * main.c
 *
 * Created: 9/10/2023 12:49:24 PM
 *  Author: tnc
 */ 



#define F_CPU 16000000UL

#define _TRIG (1<<4)
#define _ECHO (1<<3)
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

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uart_hal.h"

//ýsr da int1 de counta hapset elsete count u al ve
//mainde portlarý tanýmla
//loopta 


uint8_t print_dist[40] ;

static volatile short send_signal;
static volatile uint16_t pulse;//interger  to access all though the program
static volatile uint16_t COUNT ;
static volatile uint16_t a ;
static volatile uint16_t b ;
static volatile uint8_t c ;



ISR(INT0_vect){//falling edge   high to low
	b=TCNT1;
	TCCR1B=0x00;
	
	pulse=b-pulse;
	pulse =pulse>>5;
	
	send_signal=0x01;
	COUNT = (int)(pulse/c);
	L_=0x8;
	EIMSK =0x00;
	EICRA =0x00;
	
}
ISR(INT1_vect){//rising edge low to high
	
	L_ =0x4;
	TCNT1=0;
	TCCR1B=0x2;		//enable counter
	pulse=TCNT1;	//count memory is updated to integer
	
	
	
}

int main(void){
	
	L__ = 0x3F;  //set led port
	L_ = 0x00;
	
	
	U_S_ =0b01100;	  //set hcsr04 port
	U_S =0x00;
	
	//EIMSK |=0x03;   //enable INT0 and INT1
	//int0 counterý çalýþtýrmalarýna izin verir   falling edge detect  10
	//int1 echonun high olduðunu anlar            rising edge detect   11
	//EICRA |=0x0E;   
	//INT1  INT0 ||| ISC11 ISC10  ISC01 ISC00  
	
	send_signal=0x01;
	pulse= 0x00;
	COUNT =0x0;
	a=0x0;
	b=0x0;
	c=58;
	
	
	_delay_ms(1000);
	PORTC=0x3F;
	_delay_ms(500);
	PORTC=0x00;
	
	uart_init(9600,0);	
	
	sei();
	while(1){

		if(send_signal==0x01)
		{
			EIMSK |=0x03;
			EICRA |=0x0E;   
			_delay_ms(20);
			if (COUNT>5)
			a=0x00;
			if (COUNT>20)
			a|=1<<0;
			if (COUNT>50)
			a|=1<<1;
			if (COUNT>100)
			a|=1<<2;
			if (COUNT>200)
			a|=1<<3;
			if (COUNT>300)
			a|=1<<4;
			if (COUNT>400)
			a|=1<<5;
			L_ = a;//show distance on led
			
			memset(print_dist,0,sizeof(print_dist));
			sprintf((char*)print_dist,"distance: %u cm %u , %u   \n \r",b,pulse,c);
			uart_send_string(print_dist);
			
			TCCR1B =0x2;//set prescaler as 8  enabling counter
				U_S   = _TRIG;  //set trig high
				_delay_us(10);   //apply high for 10 mikro second
				U_S &= ~_TRIG;//set trig low
				send_signal=0x00;
		}
		
// 		else if ((b/c) >300)
// 		{
// 			TCCR1B=0x2;		//disabling counter
// 			pulse=0;	//count memory is updated to integer
// 			TCNT1=0;
// 			//PORTC =0x4;
// 			send_signal  = 0x01;
// 			
// 		}
// 		b= TCNT1-pulse;
// 		//PORTC =0x2;
		
		
	}
}


/*
			int main(void)
			{
				DDRC = 0x3F;
				PORTC = 0x00;
				_delay_ms(2000);
				int a =0x0;
				while(1)
				{
					if(a<0x4F){PORTC = (a++); _delay_ms(150);}
					else{a=0x00;}
				}
			}
*/