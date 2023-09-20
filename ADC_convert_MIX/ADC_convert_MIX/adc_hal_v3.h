/*
 * adc_hal_v3.h
 *
 * Created: 30.08.2023 20:15:53
 *  Author: tnc
 */ 
/*
filtre tipleri 
ayrýk zaman düzlemi
t zaman düzlemi
s domain zaman
c2d dönüþüm
ölü zaman
dijital filtreler
step response
*/



#ifndef ADC_HAL_V3_H_
#define ADC_HAL_V3_H_

#include <stdint.h>
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ADC_VOLT(x) (x * (5.0/1024.0))

extern volatile uint16_t adc_buffer[8];
extern volatile float CoefA;
extern volatile unsigned long int_coefA;
extern volatile unsigned long int_coefB;

enum{
	ADC0_PIN,
	ADC1_PIN,
	ADC2_PIN,
	ADC3_PIN,
	ADC4_PIN,
	ADC5_PIN,
	ADC6_PIN,
	ADC7_PIN,
	ADC8_TEMPERATURE,
	ADC_1V1 = 0b1110,
	ADC_GND,
};


void adc_init_rig(void);
void adc_pin_enable(uint8_t pin);
void adc_pin_disable(uint8_t pin);
void adc_pin_select(uint8_t souce);
uint16_t adc_convert(void);
void adc_average_buff(uint8_t max_chanel);
//void adc_average(void); //not convenient for main.c
//void adc_filtered_convert(void); //not convenient for main.c
uint16_t getJoy1x();
uint16_t getJoy1y();
uint16_t getJoy2x();
uint16_t getJoy2y();
uint16_t getpot1();
void set_adc_sample_amount(int8_t amount);

#endif /* ADC_HAL_V3_H_ */