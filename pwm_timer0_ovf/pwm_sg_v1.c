/*
 * pwm_sg_v1.c
 *
 * Created: 3.10.2023 06:00:58
 *  Author: tnc
 */ 
#define F_CPU 16000000UL

/*
#include "pwm_sg.h"
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define prescaller 0x4//prescaller 256 0x5//prescaller 1024  
uint8_t timr0_width;
int timr0_ovf;
volatile uint16_t OCR0[2]; 

ISR(TIMER0_COMPB_vect){
	if(timr0_ovf == 4)
	OCR0B =(OCR0[1]);
	else
	OCR0B = 0;
}
ISR(TIMER0_COMPA_vect){
	if(timr0_ovf == 4){
		OCR0A = (OCR0[0]);
		timr0_ovf =0;
	}
	else 
	OCR0A = 0;
}
/*
ISR(TIMER0_OVF_vect){
	
	if(timr0_ovf == 4)
	{
			OCR0A = (OCR0[0]);
			OCR0B =(OCR0[1]);
		timr0_ovf=0;
		PORTB =timr0_ovf;// 4 ms flash
	}
	else 
	{
		timr0_ovf++;
		PORTB =timr0_ovf;//16ms flash
	}
	//0x00 da ocrn are updates 
	//TCNT0=0xFF - timr0_width;  //do not control this time  //2;  //
}
////////////////////////
void pwm_pin_enable( unsigned char port , short pin){
	switch(port){
		case 'B': DDRB|= 1<<pin;PORTB|= 1<<pin;break;
		case 'D': DDRD|= 1<<pin;PORTD|= 1<<pin;break;
		case 'b': DDRB|= 1<<pin;PORTB &= ~( 1<<pin);break;
		case 'd': DDRD|= 1<<pin;PORTD &= ~( 1<<pin);break;
	}
}


void Init_pwm(){
	timr0_ovf = 0;
	timr0_width=250;//overflov 4ms yapýyor 
	
	/* was for phase correct pwm
	//0;//arkadaþ dipte kndini tcnt0 konumuna atýyo
	//ve bu nedenle düþüþtekini ayarlayamýyoruz,
	//bizde periodtan yaþanmamasý gereken aný çýkartýyoruz
	//    |-------|****(tepe)************|
	//phase correct timer
	
	//phase için aþagýdan yukarý sayarken high
	//yukarýdan aþagý inerken karþýlaþýrlarsa low yap
	// Set phase Fast PWM, TOP in 0xFF, Clear OC1A on compare match, clk/prescaler 
//////////////////////////////
	TCCR0A =  (2<< COM0A0)|(2<< COM0B0) | (3<<WGM00); //fast pwm
	TCCR0B = (prescaller<<CS00);
	TIMSK0 |= (1<<OCIE0B) | (1<<OCIE0A)	 ; //|(1<<TOIE0);// 
	
	//boþ sonra silebilirsin
	DDRB =0xF;
}

void Set_pwm_value(short pin , uint16_t new_value){
	
		switch(pin)
		{
			case 5:OCR0[0] = 32+ (new_value);break;
			case 6:OCR0[1] = 32+ (new_value);break;
			//32+ (0-64) = >> 1 cycle 4ms ,servo motor is btw 500us and 1500us == >>  256 tick 4000us  |||  (2^8) /8 -- (2^8)*3/8 
			//2 ye bölünmeden dolayý iþi garantiye alalým dedik
			//o yüzden çift
			
		}
		
	
}
//ovf timer0= 5 sayýsý ovf , fast pwm mode
//  2ms=/253*2=max_pwm=value\   20ms=/0\/1\/2\/3\/4\/5\/6\/7\/8\/9\

*/