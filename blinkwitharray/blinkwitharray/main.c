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

void hard(uint8_t input);				

int main()
{	//I made a mistake by using D0 and D1 pins 
	//Which used by atmega 328p chip as UART connection
	//be careful using Port D 
	//so that I am using D 4,5,6,7 as A,B,C,D AND D 8,9,10,11 as E,F,G,(DP)
	//				 PortD 4,5,6,7			  PortB 0,1, 2, 3
	
	//DP for Dot Point but I did not use 
	
	uint8_t	 SSdp[10]={  
					0b0111111,//0
					0b0000110,//1
					0b1011011,//2
					0b1001111,//3
					0b1100110,//4
					0b1101101,//5
					0b1111101,//6
					0b0000111,//7
					0b1111111,//8
					0b1101111,//9
					};
					
	
	uint8_t num =0;
				while(1)
				{
					
					for (num = 0; num<10 ;num++)
					{
						hard(SSdp[num]) ; 
						_delay_ms(2000);			//wait for 2 second
						
					}
						
				}    
}

void hard(uint8_t input){
	PORTB = (0x0F & input);		//take only first four bit 
	PORTD = (0xF0 & input);		//take only last four bit
}	