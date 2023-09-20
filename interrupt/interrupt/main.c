/*
 * main.c
 *
 * Created: 8/25/2023 2:58:37 PM
 *  Author: tnc
 */ 


#ifndef F_CPU
#define F_CPU 16000000UL // 16MHz clock speed
#endif
#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

 uint8_t dir;



ISR(INT0_vect){
	dir ++;
}

int main()
{

	//PD2 reserved for interrupt
	uint8_t	 PD[10]={  0b10000010,//1
					0b01011011,//2
					0b11001011,//3
					0b11100010,//4
					0b11101001,//5
					0b11111001,//6
					0b10000011,//7
					0b11111011,//8
					0b11101011,//9
					0b10111011//0
					};
					
				
	DDRD = ~0x04;//PD2 (interrupt 0) except all port output 
	
	EICRA = 0x01; //logic change controlled interrupt0
	EIMSK = 0x1; //masking interrupt 0
	sei();
	
	uint8_t seg =1;
	dir = 0x1;
	
					while(1)
				{
					
					
					if((seg<11) & (seg>0))
					{
						PORTD =  PD[seg-1] ;
						PORTD |= (PD[seg-1]>>5 & 0x4) ;
						_delay_ms(800);
					}
					if (dir%2)
					{
						seg++;
					}
					else{
						seg--;
						}
					if(dir%2 && seg==11)
					seg=1;
					if(!dir%2 && seg==0)
					seg=10;
				}
				
			
    
}
