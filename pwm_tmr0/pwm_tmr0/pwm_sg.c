/*
 * pwm_sg.c
 *
 * Created: 30.09.2023 08:40:39
 *  Author: tnc
 */ 

#define prescaller 0x5//prescaller 1024  0x4//prescaller 256

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t timr0_width;
volatile uint8_t timr0_ovf;
static uint16_t OCR0[2]; 

// ISR(TIMER0_COMPA_vect){
// 	OCR0A = 0xFF - (OCR0[0]);
// }
// ISR(TIMER0_COMPB_vect){
// 	OCR0B = 0xFF - (OCR0[1]);
// }

ISR(TIMER0_OVF_vect){
	TCNT0= 0xff - timr0_width;  //0x200 - timr0_width*2 ;
	OCR0B = 0xFF - (OCR0[1]);
	OCR0A = 0xFF - (OCR0[0]);
	
	//0;//arkadaþ dipte kndini tcnt0 konumuna atýyo 
	//ve bu nedenle düþüþtekini ayarlayamýyoruz, 
	//bizde periodtan yaþanmasý gereken aný çýkartýyoruz 
	//    |-------|****(tepe)************|
		
		
		
	/*
	TCNT0=0xFF-timr0_width;
	if (timr0_ovf ==4)
	{
		//TCCR0A |= (2<< COM0A0)|(2<< COM0B0) ;
		timr0_ovf=0;
		OCR0A = OCR0[0];
		OCR0B = OCR0[1];
	}
	
	else
	{
		//TCCR0A &= ~((3<< COM0A0)|(3<< COM0B0));
		OCR0A = 0;
		OCR0B =0;
		timr0_ovf++;
	}
	*/
}

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
	timr0_width=60;//overflov 10ms yapýyor 
	
	//phase için aþagýdan yukarý sayarken high
	//yukarýdan aþagý inerken karþýlaþýrlarsa low yap
	/* Set phase Fast PWM, TOP in0xFF, Clear OC1A on compare match, clk/prescaler1024 */

	TCCR0A =  (3<< COM0A0)|(3<< COM0B0) | (1<<WGM00); //
	TCCR0B = (prescaller<<CS00);
	TIMSK0 |= (1<<OCIE0B) | (1<<OCIE0A) |(1<<TOIE0);
}

void Set_pwm_value(short pin , uint16_t new_value){
	
		switch(pin)
		{//elden geçmesi lazým
			case 5:OCR0[0] = 3+ (new_value);break;
			case 6:OCR0[1] = 3+ (new_value);break;
		}
		
	
}