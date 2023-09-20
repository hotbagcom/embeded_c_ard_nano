/*
 * main.c
 *
 * Created: 9/5/2023 2:24:47 PM
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
#include "uart_hal.h"

volatile uint8_t toogle_B = 0x1 ;
volatile uint8_t toogle_C = 0x1 ;
volatile uint8_t toogle_A = 0x1 ;
volatile uint8_t toogle_D = 0x1 ;
volatile uint8_t counter = 0x0;
//uint8_t print_buffer[64] = {0};

/*
// volatile uint16_t icr1l =0;
// volatile uint16_t icr1h =0;
*/


/*
// triangle(uint8_t height);
// squera(uint8_t height);
*/

ISR(TIMER1_COMPB_vect){//1110  //A ve B nin ilk ayrýlýklarýnda aktik oluyor
	PORTB = (PORTB & 0xFE) | (toogle_A<<0);
	toogle_A = ~toogle_A;
	//counter++;
	
}

ISR(TIMER1_OVF_vect){//1101
	PORTB = (PORTB & 0xFD) | (toogle_B<<1);
	toogle_B = ~toogle_B;
	//counter++;
	
}


ISR(TIMER1_COMPA_vect){//1011
	PORTB = (PORTB & 0xFB) | (toogle_C<<2);
	toogle_C = ~toogle_C;
	counter++;
	
}
ISR(TIMER1_CAPT_vect){//0111
	PORTB = (PORTB & 0xF7) | (toogle_D<<3);
	toogle_D = ~toogle_D;
	//counter++;
	
}

void squera(uint8_t height){
	char c ='O';
	for (char ii=0; ii<height ;ii++ )
	{
		for (char jj=0; jj<= height ;jj++)
		{
			uart_send_byte(c);
		}
	uart_send_byte('\r');
	}
}
 
void triangle(int8_t height){
	char c ='O';
	for (char ii=0; ii<height ;ii++ )
	{
		for (char jj=0; jj<=ii ;jj++)
		{
			uart_send_byte(c);
		}
	uart_send_byte('\r');
	}
}

int main(void)
{
	
	DDRB = 0xF;//(portB 0 1 2 output)
	//PORTB |=0x1;
	//WGM13	WGM12 WGM11	WGM10 	TOP	  Update of OCR1x at	  TOV1 Flag Set on   Timer/Counter Mode of Operation
	//  1     1     1     0    ICR1       BOTTOM                  TOP                       CTC
	//CS12  CS11  CS10  Prescaler
	//  0    1     1      64
	//COM1A1  COM1A0  COM1B1  COM1B0     (For Fast PWM Compare Mode)
	//  1       0       1       0    Set OC1A/OC1B on compare match, clear OC1A/OC1B at	BOTTOM (non-inverting mode)
	TCCR1B |= (3<<WGM12) | (3<<CS10);
	TCCR1A |= (2<<WGM10) | (10<<COM1B0);

	
	//clk =(16E6)^-1 prescaler =64 => Target ferq : 250000    1ms takes 250 cycle 
	OCR1B = (25*17)-1;// (1.7)ms  ;((1<<15)-1)-(125*13)-1;//10ms = 250*10/4E-6 sec
	ICR1 = (25*200)-1;// (20)ms  31ms = 250*31/4E-6  sec
	OCR1A = (25*13)-1;//  (1.3)ms   ((1<<15)-1)-((125*21)-1);//72ms = 250*72/4E-6 sec  top = ocr1a
	
	TIMSK1 |=(1<<ICIE1) |(1<<OCIE1B) | (1<<OCIE1A) |(1<<TOIE1);// Set Enable (overflov)
	
	//uint8_t start[] = "Program Start\n\r";
	
	
	uart_init(9600,0);
	//uart_send_string(start);
	
	sei();//Activate Global Interrupt
	
	

	while (1)
	{
			if(counter%50 == 10)			{
				//OCR1B = (5*50)-1;//250:1.0,2.0   ;  25:10,20  ;5:50,100
				triangle(6);
			} 
			else if(counter%50 == 0)			{
				//OCR1B = (5*100)-1;
				squera(3);
			}
			
			
		
	}

}



