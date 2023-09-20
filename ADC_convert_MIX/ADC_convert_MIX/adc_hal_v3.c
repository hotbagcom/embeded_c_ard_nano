/*
 * adc_hal_v3.C
 *
 * Created: 30.08.2023 20:16:43
 *  Author: tnc
 */ 

#include "adc_hal_v3.h"


volatile uint8_t channel;
volatile uint8_t times_sample = 20 ;//semple amount
volatile uint16_t adc_buffer[8]; //analog max value cannot be more than 8

volatile float CoefA;
volatile unsigned long int_coefA;
volatile unsigned long int_coefB;

 volatile static uint8_t adc_convert_done = 1;
 ISR(ADC_vect){
	adc_convert_done = 1;
 }
 
 
void filtreInit(){
	//CoefA = 0.95;//
	int_coefA = 1992294;//CoefA*(1<<16);
	int_coefB = 104858 ;//(1-CoefA)*(1<<18);
}

void adc_init_rig(void){
	ADMUX |= 0b01 << REFS0; //referance sellection  right adjust ADLAR=0
	ADCSRA |= 1 << ADEN | 1 << ADIE | 0b111 << ADPS0;//adcenable interruptenable divisionfactor
	filtreInit();
}


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


uint16_t adc_convert(void){
	
	uint8_t adcl = 0;
	uint8_t adch = 0;
	
	//adc_convert_done = 0;
	ADCSRA &= ~(0x1<<ADIF);
	
	ADCSRA |= 1 << ADSC;
	while(adc_convert_done == 0);
	//while((0x1 & (ADCSRA >>ADIF)) == 0x1);
	adcl = ADCL;
	adch = ADCH;
	
	
	return (adch<<8)|adcl ;
	
}


void adc_average(void){// in stuct LMB 3 bit can be use as channel RMB 5 can be use as sample amount
	uint32_t adc_sample_add =0;
	for (uint8_t ii = 0; ii<times_sample && ADCPWM_20ms[5] ;ii++)// ii<times &0xF ;ii++)
	{
		adc_sample_add += adc_convert();
	}
	adc_buffer[channel] =adc_sample_add/times_sample;
	
}

void adc_filtered_convert(void){
	 
	int16_t Sample_0 =0;
	int16_t Sample_1 =0;
	int32_t non_parasite =0;
	
	for (uint8_t ii = 0; ii<times_sample ;ii++)// ii<times &0xF ;ii++)
	{
		Sample_0 = adc_convert();
		non_parasite=((Sample_0*int_coefB + non_parasite*int_coefA)>>21);
		//Sample_1+=Sample_0;
	}
	adc_buffer[channel] = non_parasite;
}

void adc_average_buff(uint8_t max_chanel){
	ADCSRA &= 0xF0;//clear dvision factor
	ADCSRA |=0b110 << ADPS0;//set divison factor as 64
	
	
	
	for(uint8_t ii=0;ii<max_chanel;ii++){
		channel= ii;
		adc_pin_enable(channel);
		adc_pin_select(channel);
		//adc_average();
		adc_filtered_convert();
		adc_pin_disable(channel);
	}
	
	
}

/*
// uint16_t getJoy1x(void){
// 	return adc_buffer[0];
// }
// uint16_t getJoy1y(void){
// 	return adc_buffer[1];
// }
// uint16_t getJoy2x(void){
// 	return adc_buffer[2];
// }
// uint16_t getJoy2y(void){
// 	return adc_buffer[3];
// }
// uint16_t getpot1(void){
// 	return adc_buffer[4];
// }
*/


void set_adc_sample_amount(int8_t amount){
	times_sample = amount;
}