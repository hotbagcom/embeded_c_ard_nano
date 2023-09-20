/*
 * main.c
 *
 * Created: 8/24/2023 2:40:28 PM
 *  Author: tnc
 */ 

//#include <xc.h>
#ifndef F_CPU
#define F_CPU 16000000UL // 16MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>


int main()
{
	
	uint8_t	 PD[10]={  0b0000110,//1
					0b1011011,//2
					0b1001111,//3
					0b1100110,//4
					0b1101101,//5
					0b1111101,//6
					0b0000111,//7
					0b1111111,//8
					0b1101111,//9
					0b0111111//0
					};
					
	DDRD =0b01111111;
	uint8_t seg =0;
				while(1)
				{
					
					for (seg = 0; seg<10 ;seg++)
					{
						PORTD =  PD[seg];
						_delay_ms(2000);
						
					}
						
				}
				
			
    
}