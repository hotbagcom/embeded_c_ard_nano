/*
 * pwm_sg.c
 *
 * Created: 30.09.2023 08:40:39
 *  Author: tnc
 */ 

#define prescaller 0x4//prescaller 64

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t timr0_width;
volatile uint8_t timr0_ovf;
uint16_t OCR0[2]; 

ISR(TIMER0_OVF_vect){
	TCNT0=0xFF-timr0_width;
	if (timr0_ovf ==5)
	{
		timr0_ovf=0;
		OCR0A = OCR0[0];
		OCR0B = OCR0[1];
	}
	
	else
	{
		OCR0A = 0;
		OCR0B = 0;
		timr0_ovf++;
	}
}

void pwm_pin_enable( unsigned char port , short pin){
	switch(port){
		case 'B': DDRB= 1<<pin;PORTB= 1<<pin;break;
		case 'D': DDRD= 1<<pin;PORTD= 1<<pin;break;
		case 'b': DDRB= 1<<pin;PORTB= 1<<pin;break;
		case 'd': DDRD= 1<<pin;PORTD= 1<<pin;break;
	}
}

void Init_pwm(){////elden geçmesi lazým
	timr0_ovf = 0;
	timr0_width=250;//5 overflov döngüsü 20ms yapýyor
	
	TCCR0A = (2<< COM0A0)|(2<< COM0B0) | (3<<WGM00);	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/prescaler64 */
	TCCR0B = (prescaller<<CS00);
	TIMSK0 = (1<<TOIE0);
}

void Set_pwm_value(short pin,uint16_t new_value){
	if (new_value)
	{
		switch(pin){
			case 5:OCR0[0] = 62+ (int)(new_value/4);break;
			case 6:OCR0[1] = 62+ (int)(new_value/4);break;
		}
		
	}
}