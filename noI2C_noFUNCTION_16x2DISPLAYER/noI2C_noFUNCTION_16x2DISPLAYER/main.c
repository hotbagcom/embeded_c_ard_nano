/*
 * main.c
 *
 * Created: 8/27/2023 3:24:19 PM
 *  Author: tnc
 */ 

/*PB0 RS
**PB1 E
**PD4 bit0
**PD5 bit1
**PD6 bit2
**PD7 bit3
*/

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

#include <xc.h>
#include "func_names.h"

int main(void)
{
	
	DDRD = 0xF0; //0b 1111 0000
	DDRB = 0x03; //0b 0000 0011
	
	unsigned char A = 'D';
	unsigned char B = 'E';
	INITIALIZE_LCD_Commands();
	lcdGoToSecondLineBegin();
	while(1){
		_delay_ms(800);
		//chip goes to bed for 800 ms for properly seeing printing of the letters 
		WRITE_LCD_Data(A);
		//send A variable to displayer
		
		_delay_ms(800);
		WRITE_LCD_Data(B);
	        //send B variable to displayer

		//Because of I did not set cursor, letters will print next to next 
	    }
	
	
}



