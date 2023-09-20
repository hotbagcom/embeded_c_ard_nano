/*
 * main.c
 *
 * Created: 9/1/2023 4:00:12 PM
 *  Author: tnc
 */ 
#define F_CPU 16000000UL

/*
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <string.h>
*/
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include "uart_hal.h"

volatile uint8_t toogle_B = 0x1 ;
volatile uint8_t toogle_C = 0x1 ;
volatile uint8_t toogle_A = 0x1 ;
volatile uint8_t counter = 0x0;
uint8_t print_buffer[64] = {0};





 
int main(void)
{
	
	DDRB = 0x7;//(portB 0 1 2 output)
	//PORTB |=0x1;
	//WGM13	WGM12 WGM11	WGM10 	TOP	  Update of OCR1x at	  TOV1 Flag Set on   Timer/Counter Mode of Operation
	//  1     1     1     1    OCR1A       BOTTOM                   TOP                     Fast PWM
	//CS12  CS11  CS10  Prescaler
	// 0     1     1      64
	//COM1A1  COM1A0  COM1B1  COM1B0     (For Fast PWM Compare Mode)
	//  1       1       1       1    Set OC1A/OC1B on compare match, clear OC1A/OC1B at	BOTTOM (inverting mode)
	TCCR1B |= (3<<WGM12) | (3<<CS10);
	TCCR1A |= (3<<WGM10) | (15<<COM1B0);

	
	//clk =(16E6)^-1 prescaler = 64 => Target ferq : 4E-6    1ms takes 250 cycle 
	OCR1B = (250*11)-1;//10ms = 250*11/4E-6 sec
	ICR1 = (250*61)-1;//31ms = 250*61/4E-6  sec
	OCR1A = (250*72)-1;//72ms = 250*72/4E-6 sec  top = ocr1a
	TIMSK1 |= (1<<ICIE1) |(1<<OCIE1B) | (1<<OCIE1A);// Set Enable 
	
	//uint8_t start[] = "Program Start\n\r";
	
	
	//uart_init(9600,0);
	//uart_send_string(start);
	
	sei();//Activate Global Interrupt
	
	


	while(1){
	//	PORTB = (PORTB & 0xF0 )| (toogle & 0x1);
	//	toogle =~toogle;
	//	_delay_ms(2000);
	}

}



ISR(TIMER1_COMPB_vect){//1110
	PORTB = (PORTB & 0xFE) | (toogle_B<<PORTB0);
	toogle_B = ~toogle_B;
}



/*
// ISR(TIMER1_CAPT_vect){//1101
// 	PORTB = (PORTB & 0xFD) | (toogle_C<<PORTB1);
// 	toogle_C = ~toogle_C;
// }
*/



ISR(TIMER1_COMPA_vect){//1011
	PORTB = (PORTB & 0xFB) | (toogle_A<<PORTB2);
	toogle_A = ~toogle_A;
	//memset(print_buffer,0,sizeof(print_buffer));
	//sprintf((char*)print_buffer,"%04x \n\r", counter);
	//uart_send_string(print_buffer);

	counter++;
	TIFR1 &= ~(1<<OCF1A);
}


