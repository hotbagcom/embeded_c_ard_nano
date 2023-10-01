/*
 * main.c
 *
 * Created: 9/30/2023 12:56:53 PM
 *  Author: tnc
 */ 

#define  F_CPU 16000000UL

#include <xc.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pwm_sg.h"
#include "adc_hal_v4.h"


int main(void)
{
	pwm_pin_enable('d',5);
	pwm_pin_enable('d',6);
	Init_pwm();
	
	adc_pin_enable(1);
	adc_pin_enable(0);
	set_sample_amount(20);
	ADC_Init();
	
	sei();
	
	/*16+
	1 =1.02ms
	6=1.41ms
	12=1.79ms;
	*/
	/*8+
	0= no pwm
	1=708us
	
	*/
	/*3+
	0=?250us
	10=775us
	20=1410us
	25=1800us
	30=2090us
	*/
	short adcpwm[2];
	
	
	Set_pwm_value(5,10);
	Set_pwm_value(6,10);
	
	int nn= 0;
	while(1)
	{
		
		
		
		//500(adc) ü 25(pwm) e sýðdýr
		
		if (adc_convert_done)
		{
			Set_pwm_value(5,adcpwm[0]);
			Set_pwm_value(6,adcpwm[1]);
			adc_convert_done=0x0;
			nn=0;
		}
		
			adc_convrt_a_1(nn);
			adcpwm[nn]=(int)(a_1[nn]/25);
			nn++;
			 
		
		 
		 
    }
}