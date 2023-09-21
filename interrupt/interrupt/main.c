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

uint8_t dir;	//global variable because of we use this variable in 'ISR()' and 'main()' functions
void hard(uint8_t input);


ISR(INT0_vect){//Interrupt service routine
	dir ++;
}

int main()
{
		//so that I am using D 4,5,6,7 as A,B,C,D AND D 8,9,10,11 as E,F,G,(DP)
		//				 PortD 4,5,6,7			  PortB 0,1, 2, 3
		
		//DP for Dot Point but I did not use

		uint8_t	 SSdp[10]={
		//0b(DP)GFEDCBA
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
					
	//PD2 reserved for interrupt				
	DDRD = ~0x04;		//Port d2 is not output
	EICRA = 0x01; //logic change controlled interrupt0 
	EIMSK = 0x1; //masking interrupt 0
	
	//same as a+=1  ======    a=a+1
	//or PORTD |= (0b00111110) ====== PORTD = (PORTD|0X3E)  
	//or DDRC & = ~(0b00001100)  ====== DDRC &= 0b11110011   ===== DDRC = (DDRC & 0b11110011)
	
		DDRD |= 0xF0; //I am not using D 0,1,2 and 3 pins , D 4,5,6,7 are out put from my chip
		//I am not touching D 0,1,2,3 by | (or) symbol 
		// Only changing D 4,5,6,7 pins as output
		
		DDRB |= 0x0F; //I'm given D 8,9,10 and 11 pins as output
		//similar things happens here only switching PORTB 0,1,2,3 to output
		
		PORTD = 0x00; //using this port for last four input bit
		PORTB = 0x00; //using this port for first four input bit
	
	
	sei();
	
	uint8_t num =1;					//local variable because only used in in 'main()' function
	dir = 0x0;					//set direction as 0 when we not pushing count will increase
								// if you set dir as 0x01(hex)==[1(decimal)] thencount will be decreaseS
	
	while(1)
	{
		
		
		if((num<11) & (num>0))// to control direction num:1 shows digit '0' , num:10 shows digit '9' 
		{
			hard(SSdp[num-1]);
			_delay_ms(800); //wait for 0.8 second
		}
		
		if ((dir& 0x1)==0 )//if direction false   then increase counter
		{
			num++;
		}
		else if((dir& 0x1)!=0)//if direction true   then decrease counter
		{
			num--;
		}
		
		if((dir& 0x1)==0 && num==11)//if counter increases and showed '9'    set 'num' to show '0'
		num=1;
		if((dir& 0x1)!=0 && num==0)//if counter decreases and showed '0'    set 'num' to show '9'
		num=10;
	}   
	
}


void hard(uint8_t input){
	PORTB = (0x0F & input);		//take only first four bit
	PORTD = (0xF0 & input);		//take only last four bit
}


