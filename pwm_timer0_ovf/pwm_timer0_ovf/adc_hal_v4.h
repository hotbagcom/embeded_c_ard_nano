/*
 * adc_hal_v4.h  son sürüm uyumlu c:\users\tnc\Documents\Atmel Studio\7.0\pwm_timer0_ovf\pwm_timer0_ovf\adc_hal_v5.c
 *
 * Created: 30.09.2023 08:37:19
 *  Author: tnc
 */ 


#ifndef ADC_HAL_V4_H_
#define ADC_HAL_V4_H_
#include <xc.h>

extern volatile short sect_mm;
extern uint16_t a_1[6];
extern short adc_convert_done;
extern void ADC_Init();
extern void adc_pin_enable(uint8_t pin);
extern void adc_convrt_a_1 (uint8_t a);
extern void set_sample_amount(uint8_t maxsample);
extern void set_output_coef(uint16_t coef);

#endif /* ADC_HAL_V4_H_ */