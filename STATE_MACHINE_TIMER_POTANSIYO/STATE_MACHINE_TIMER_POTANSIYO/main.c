/*
 * main.c
 *
 * Created: 9/6/2023 7:20:58 AM
 *  Author: tnc
 */ 
//STATE MACHINE - ADC - FPWM

//library
#include <xc.h>
#include "config.h"
#include "adc_hal_v3.h"
//variable
volatile uint8_t counter ;
volatile uint8_t th_sample[8];
volatile uint8_t max_pin = 2;//kod 2 kanala göre ayarlý 
volatile uint8_t toogle_A = 0x1 ;
volatile uint8_t toogle_B = 0x1 ;
volatile uint8_t toogle_C = 0x1 ;

//define


//struct


typedef struct ADCPWM_1_20ms{
	uint8_t switch_0 :1;
	uint8_t switch_1 :1;
	uint8_t switch_2 :1;
	uint8_t switch_3 :1;
	
	uint8_t switch_4 :1;
	uint8_t switch_5 :1;
	uint8_t switch_6 :1;
	uint8_t switch_7 :1;
};

void set_counter(uint8_t set){
	counter=set;
}
void inc_counter(void){
	counter++;
}
	
struct ADCPWM_1_20ms ADCPWM_20ms;

//interrupt service routine
ISR(TIMER1_COMPA_vect){	//A TYPE
	PORTB = (PORTB & 0b11111110) | (toogle_A<<0);
	toogle_A = ~toogle_A;
}
ISR(TIMER1_COMPB_vect){	//B TYPE
	
	ADCPWM_20ms.switch_0 = counter%20 ==4;//servo HIGH
	ADCPWM_20ms.switch_1 = counter%20 ==5;//servo control
	ADCPWM_20ms.switch_2 = counter%20 ==6;//servo LOW
	ADCPWM_20ms.switch_3 = counter%20 ==7;//potX <-- adc_buffer
	
	ADCPWM_20ms.switch_4 = counter%20 >8;//adc start
	ADCPWM_20ms.switch_5 = counter%20==2;//adc_iç_control 1
	// 	ADCPWM_20ms.switch_6 = (counter%20==0);
	// 	ADCPWM_20ms.switch_7 = (counter%20==0);
	
	PORTB = (PORTB & 0b11111101) | (toogle_B<<1);
	toogle_B = ~toogle_B;
	inc_counter();
}

ISR(TIMER1_CAPT_vect){	//CAPTLOW ÝÇ COUNTER
	//INTERRUPT_MS.FLAG_1ms = counter%1==0;
	
	PORTB = (PORTB & 0b11111011) | (toogle_C<<2);
	toogle_C = ~toogle_C;
	
	
}



int main(void)
{
	DDRB =0xF;//portB 0,1 pwm reserved
	DDRC =0x0;//portC 0,1 adc reserved
	
	pin=0;
	set_counter(0);
	adc_init_rig();
	set_adc_sample_amount(10);//seted to defoult canot be more than 
	//CS12  CS11  CS10  Prescaler
	//  0    1     1      64
	//COM1A1  COM1A0  COM1B1  COM1B0     (For Fast PWM Compare Mode)
	//  1       1       1       1    Set OC1A/OC1B on compare match, clear OC1A/OC1B at	BOTTOM (NON-inverting mode)
	TCCR1B |= (3<<WGM12) | (3<<CS10);
	TCCR1A |= (2<<WGM10) | (10<<COM1B0);
	
	//clk =(16E6)^-1 prescaler =64 => Target ferq : 250000    1ms takes 250 cycle 
	
	ICR1 = (25*200)-1;// (20.0)ms  
	OCR1A = (25*5)-1;// (0.5)ms  0deg
	OCR1B = (25*10)-1;// (1.0)ms  
	TIMSK1 |=(1<<ICIE1)|(1<<OCIE1B);// Enable   | (1<<OCIE1A) 
	
	
	
	sei();
	
    while(1)
    {
	
		
		if (ADCPWM_20ms.switch_0){
			
			TIMSK1 |= (1<<OCIE1A) /*| (1<<OCIE1B)*/;
		}
		else if (ADCPWM_20ms.switch_1){
		}
			
		else if (ADCPWM_20ms.switch_2){
			//TIMSK1 &= ~(1<<OCIE1B) ;
			TIMSK1 &= ~(1<<OCIE1A);//Disable  
			
		}
		else if (ADCPWM_20ms.switch_3){//potX <-- adc_buffer
			uint16_t a =  ((250*adc_buffer[0][0])/1024)-1;// buffer/1024 milisec
			//uint16_t b = ((250*adc_buffer[1][0])/1024)-1;// 
			OCR1A =a;
			//OCR1B =b;
			
		}
		else if (ADCPWM_20ms.switch_4){//adc start
			adc_pin_enable();
			adc_pin_select();
			(th_sample[pin]<(times_sample-1))?(th_sample[pin])++:th_sample[pin];
			adc_buffer[pin][th_sample[pin]]=adc_convert();
			
		}
		else if (ADCPWM_20ms.switch_5)
		{///bu durumda sadece okunan kadarý 
			for(uint8_t pin_jj =0 ; pin_jj<2 ; pin_jj++)
			{
				for (int ii =1 ; ii<(th_sample[0]-1) ; ii++)
				{
					adc_buffer[pin_jj][0] +=adc_buffer[pin_jj][ii];   //th_sample[0]+ =th_sample[ii];
				}
				adc_buffer[pin_jj][0] =adc_buffer[pin_jj][0] / th_sample[pin_jj] ;
			}
		}
		
		
		
		
		
    }
	
}
