


#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//define
/*
 ****    *   *   *    *      *    *       *     *      
*    *    * *    * *  *     * *   **     **    * *      
*    *     *     *  * *    *   *  * *   * *   *   *   
*    *     *     *   **    *****  *  * *  *   *****   
 ****      *     *    *    *   *  *   *   *   *   * 
*/


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

int main(void)
{
	ADC_Init();										/* Initialize ADC */
	DDRB |= 1<< PINB1 | 1<<PINB2 ;					/* Make OC1A OC1B pin as output */
	
	ICR1 = 4999;									/* Set TOP count for timer1 in ICR1 register */
	TCCR1A = (1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/prescaler64 */
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	
	uint16_t a_1;
	uint16_t b_1;
	

		
	while(1)
	{
		adc_pin_enable(0);
		adc_pin_select(0);
		a_1 = 125 + (int)(ADC_Read(0)/2.048);
		adc_pin_disable(0);
		
		adc_pin_enable(1);
		adc_pin_select(1);
		b_1 = 125 + (int)(ADC_Read(1)/2.048);
		adc_pin_disable(1);
		
		OCR1A=a_1;
		OCR1B=b_1;
					/* Read ADC channel 0 and make count in between 125-625 i.e. 0.5-2.5 mSec duty cycle variation */
		
	}
}