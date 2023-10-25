/*
 * main.c
 *
 * Created: 10/3/2023 5:55:25 AM
 *  Author: tnc
 */ 

#define  F_CPU 16000000UL

#include <xc.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "adc_hal_v4.h"
//#include "pwm_sg.h"

///


#define prescaller 0x4//prescaller 256 0x5//prescaller 1024  
uint8_t timr0_width;
int timr0_ovf;
volatile uint16_t OCR0[2]; 
/*
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
*/
ISR(TIMER0_OVF_vect){
	if(timr0_ovf <1250)
	{
		if(timr0_ovf == OCR0[0])
			OCR0A = (OCR0[0]);
		if(timr0_ovf == OCR0[1])
			OCR0B =(OCR0[1]);
		
		timr0_ovf++;
	}
	
	else 
	{
		timr0_ovf=0;
	}
	//0x00 da ocrn are updates 
	//TCNT0=0xFF - timr0_width;  //do not control this time  //2;  //
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
	*/
	TCCR0A =  (2<< COM0A0)|(2<< COM0B0) | (3<<WGM00); //fast pwm
	TCCR0B = (prescaller<<CS00);
	TIMSK0 |= (1<<OCIE0B) | (1<<OCIE0A)	 ; //|(1<<TOIE0);// 
	
	//boþ sonra silebilirsin
	DDRB =0xF;
}

void Set_pwm_value(short pin , uint16_t new_value){
	
		switch(pin)
		{
			case 5:OCR0A = 32+ (new_value);break;
			case 6:OCR0B = 32+ (new_value);break;
			//32+ (0-64) = >> 1 cycle 4ms ,servo motor is btw 500us and 1500us == >>  256 tick 4000us  |||  (2^8) /8 -- (2^8)*3/8 
			//2 ye bölünmeden dolayý iþi garantiye alalým dedik
			//o yüzden çift
			
		}
	
}

///

int main(void)
{
	ADC_Init();
	adc_pin_enable(1);
	adc_pin_enable(0);
	set_sample_amount(10);
	
	
	pwm_pin_enable('d',5);
	pwm_pin_enable('d',6);
	Init_pwm();
	
	
	//  For F_pwm 256 tick 4000us  |||  (2^8) /8 -- (2^8)*3/8 
	//set_output_coef(64);
	
	sei();
	
	
	short adcpwm[2];
	
	_delay_us(20);	
 	Set_pwm_value(5,0);
	 _delay_us(20);	
 	Set_pwm_value(6,64);
	
	int nn= 0;
	
	
	while(1)
	{
		
		if (adc_convert_done)
		{
			_delay_us(20);
			Set_pwm_value(5,adcpwm[0]);
			_delay_us(20);	
			Set_pwm_value(6,adcpwm[1]);
			adc_convert_done=0x0;
			nn=0;
		}
		
			adc_convrt_a_1(nn);
			adcpwm[nn]=(int)(a_1[nn]);//default result coef
			_delay_ms(100);	
			nn++;
			 
    }
	
	
}
