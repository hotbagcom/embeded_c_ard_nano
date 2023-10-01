/*
 * adc_hal_v4.c
 *
 * Created: 30.09.2023 08:37:02
 *  Author: tnc
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

uint16_t a_1[6];//result wil be btw
volatile uint32_t averall[6];
short adc_convert_done;
uint8_t max_sample_amount=5;

void set_sample_amount(uint8_t maxsample){
	max_sample_amount =maxsample ;
}

void adc_pin_enable(uint8_t pin){
	DIDR0 |= 1 << pin;
}

void adc_pin_disable(uint8_t pin){
	DIDR0 &= ~(1 << pin);
}

void adc_pin_select(uint8_t souce)
{
	ADMUX &= 0xF0;
	ADMUX = (ADMUX&0xF8) | (0x7&souce);
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

void adc_convrt_a_1 (uint8_t a){
	
	adc_pin_enable(a);
	adc_pin_select(a);
	//for (int8_t iii=0 ; iii<max_sample_amount ; iii++)
		averall[a] = (70*averall[a] + 30*((500000*ADC_Read(a))>>10))/100;
	a_1[a] = averall[a]/1000;
	adc_pin_disable(a);
	if (a==1)//A1 done
	adc_convert_done=1;
}

