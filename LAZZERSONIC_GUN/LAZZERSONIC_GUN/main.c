/*
 * main.c
 *
 * Create: 9/18/2023d 11:57:50 AM
 *  Author: tnc
 */ 
//asynchronous coding
#define F_CPU 16000000UL



#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pd8544_lcd.h"
#include "hcsr04.h"
#include "adc_hal_v4.h"
#include "pwm_sg.h"

//ekraný kullanmayý öðren+
//mesafeyi ayarla
//potansiyometre ile servoyu ayrla
//potansþymereden deðeri al sakla 
//deðeri ekrana yazdýr 



int main(void)
{
	Init_time();
	Ultros_Init();
	
	//Clear_time();
	ADC_Init();	
	adc_pin_enable(0);
	adc_pin_enable(1);
	
	Init_pwm();
	pwm_pin_enable('D',5);
	pwm_pin_enable('D',6);
	
	Port_Init_lcd();
	SPI_Init();
	SPI_SS_Enable();
	Display_Init_pcd8544();
	
	sei();
	
	while(1)
	{
		
		if (adc_convert_done)
		{
			Set_pwm_value(5,a_1[0]);
			Set_pwm_value(6,a_1[1]);
			sect_sg=0;
		}
		else
		{
			for (short aaa = 0 ; aaa<2 ; aaa++)
			{
				adc_convrt_a_1(aaa);
			}
			
		}
	/*	
		Ultros_D_C();
		if (allow_echo_read)
		{
			
			Process_distance_pcd8544(distance_mm[sect_mm]);
		
		sect_sg++;
		if(sect_sg>12)
		sect_sg=0;
		
			allow_echo_read=0x0;
		}
		
		Ultros_main();
		
		*/
	}//while
}//main