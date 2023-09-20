/*
 * adc_hal_v2.c
 *
 * Created: 30.08.2023 16:41:05
 *  Author: tnc
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc_hal_v2.h"

volatile static uint8_t  adc_convert_done = 1;

ISR(ADC_vect){
	adc_convert_done = 1;
	
	
}


void adc_init(void){
	ADMUX |= 0X01 << REFS0 ;
	//ADMUX |= 0X01 <<ADLAR ;//left adjust result
	//ADMUX |= 0X01 <<sellected_pin
	
	
	ADCSRA |= (0X1 <<ADEN) | (0X01 <<ADIF) | (0X7 <<ADPS0);
}

void adc_pin_enable(uint8_t pin){
	DIDR0 |= (0x01 << pin);
}

void adc_pin_disable(uint8_t pin){
	DIDR0 &= ~(0x01 << pin);
}

void adc_pin_sellect(uint8_t source){
	ADMUX &= (0xF0);
	ADMUX |= (source);
}


uint16_t adc_convert(void){
	uint16_t adcl = 0;
	uint16_t adch = 0;
	
	adc_convert_done = 0;
	
	ADCSRA |= 0x1 << ADSC;
	while(adc_convert_done == 0);
	
	adcl = ADCL;
	adch = ADCH;
	
	return (adch << 8 | adcl);
}