/*
 * main.c
 *
 * Created: 9/7/2023 2:16:37 PM
 *  Author: tnc
 */ 

/*
#define F_CPU 16000000UL

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include "adc_hal_v3.h"
#include <util/delay.h>

volatile uint8_t toogle_A = 0x1 ;
volatile uint8_t toogle_B = 0x1 ;
volatile uint8_t toogle_C = 0x1 ;
volatile uint8_t toogle_D = 0x1 ;
volatile static uint16_t adc_value_a;
volatile static uint16_t adc_value_b ;
volatile static uint16_t  adc_pin0;

static uint16_t ADC_Read(char channel);

ISR(TIMER1_COMPA_vect){//1110

}


ISR(TIMER1_CAPT_vect){//0111

	OCR1A = (250*(adc_value_a/1024)+250)-1;
	
}
ISR(ADC_vect){
	
	
	
	//adc_value_a = ADCL | (ADCH <<8);
	//ADCSRA|=1<<ADSC;
}


int main(void){
	DDRB = 0xF;
	
	
	//WGM13	WGM12 WGM11	WGM10 	TOP	  Update of OCR1x at	  TOV1 Flag Set on   Timer/Counter Mode of Operation
	//  1     0     0     0    ICR1       BOTTOM                  BOTTOM          phase and frequence correct 
	//CS12  CS11  CS10  Prescaler
	//  0    1     1      64
	//COM1A1  COM1A0  COM1B1  COM1B0     (For Fast PWM Compare Mode)
	//  1       1       1       1    Set OC1A/OC1B on compare match when up-counting. Clear OC1A/OC1B on compare match when down counting.
	TCCR1B |= (2<<WGM12) | (3<<CS10);
	TCCR1A |= (0<<WGM10) | (10<<COM1B0);
	
	//clk =(16E6)^-1 prescaler =64 => Target ferq : 250000    1ms takes 250 cycle
	ICR1 = (25*200)-1;// (20)ms  31ms = 250*31/4E-6  sec
	OCR1A = (25*5)-1;//  (0.5)ms   ((1<<15)-1)-((125*21)-1);//72ms = 250*72/4E-6 sec  top = ocr1a
	//OCR1B = (25*18)-1;//  (1.5)ms
	TIMSK1 |=(1<<ICIE1) |(1<<OCIE1B) | (1<<OCIE1A) |(1<<TOIE1);// Set Enable (overflov)
	


	
	ADMUX |= 0b01 << REFS0;
	ADCSRA |= 1 << ADEN | 1 << ADIE | 0b111 << ADPS0;
	DIDR0 |= 1 << 0;
	ADMUX &= 0xF0;
	ADMUX |= 0;
	
	sei();//Activate Global Interrupt
	
//  ADCSRA |=1<<ADSC;
	
    while(1)
	{	
		

	}
}
*/
//aþagýdaki kod çalýþmaktadýr




#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t a_1;
uint16_t b_1;
volatile uint32_t averall = 0;
volatile uint32_t bverall = 0;

void adc_pin_enable(uint8_t pin){
	DIDR0 |= 1 << pin;
}

void adc_pin_disable(uint8_t pin){
	DIDR0 &= ~(1 << pin);
}

void adc_pin_select(uint8_t souce){
	ADMUX &= 0xF0;
	ADMUX |= souce;
}
void ADC_Init()										/* ADC Initialization function */
{
	
	ADCSRA |= 1<<ADEN | 1<<ADPS2 | 1<<ADPS1 ;  // AVCC with external capacitor at AREF pin,  64 prescaler
	ADMUX = (1 << REFS0);
}

uint16_t ADC_Read(char channel)							/* ADC Read function */
{
	ADMUX = (ADMUX & 0xF0) | (channel & 0x07);		/* set input channel to read */
	ADCSRA |= (1<<ADSC);							/* Start ADC conversion */
	while(ADCSRA & (0x1 <<ADSC));				//// Wait until conversion is  completed
	return ADCW;									/* Return ADC word */
	
}




void adc_convrt_a_1(void){
	
	adc_pin_enable(0);
	adc_pin_select(0);
		
		averall = (14*averall*(2^10) +  (((2^10)*(500*ADC_Read(0)))>>10));  ///875:125
		averall = averall>>4;
	a_1 = averall>>10;
	adc_pin_disable(0);
}

void adc_convrt_b_1(void){
	
	adc_pin_enable(1);
	adc_pin_select(1);
		bverall  = (70*bverall + 30*((500000*ADC_Read(1))>>10));
		bverall/=100;
	b_1 =bverall/1000;
	adc_pin_disable(1);

}



int main(void)
{
	ADC_Init();										/* Initialize ADC */
	DDRB |= 1<< PINB1 | 1<<PINB2 ;					/* Make OC1A OC1B pin as output */
	
	ICR1 = 4999;									/* Set TOP count for timer1 in ICR1 register */
	TCCR1A = (1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/prescaler64 */
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	
	


	
	while(1)
	{
		adc_convrt_a_1();
		
		adc_convrt_b_1();
		
		
		OCR1A =a_1 +125;
		OCR1B =b_1 +125;
		
		
		/* Read ADC channel 0 and make count in between 125-625 i.e. 0.5-2.5 mSec duty cycle variation */
		
	}
}

