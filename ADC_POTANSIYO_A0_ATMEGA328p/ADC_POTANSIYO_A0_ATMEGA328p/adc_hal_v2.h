/*
 * adc_hal_v2.h
 *
 * Created: 30.08.2023 16:39:55
 *  Author: tnc
 */ 
#ifndef ADC_HAL_V2_H_
#define ADC_HAL_V2_H_



// 
// #include <stdint.h>
// 
// #include <avr/io.h>
// #include <util/delay.h>
//#include <avr/interrupt.h>
// 
// 



#define ADC_VOLT(x) (x)*(5.0/1024.0)

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
	ADC_GND = 0b1111,
};

extern void adc_init(void);
extern void adc_pin_enable(uint8_t pin);
extern void adc_pin_disable(uint8_t pin);
extern void adc_pin_sellect(uint8_t source);
extern uint16_t adc_convert(void);




#endif /*ADC_HAL_V2_H_*/ 