/*
 * main.c
 *
 * Created: 8/25/2023 4:38:51 PM
 *  Author: tnc
 */ 
#ifndef F_CPU
#define F_CPU 16000000L // 16MHz clock speed
#endif

#include <stdint.h>
#include <avr/io.h>
//#include <util/delay.h>  //we do not need delay 
#include <avr/interrupt.h>

uint8_t toogle ;
ISR(INT0_vect){//activate falling edge of input signal (when you release push button)
	PORTB = toogle<<5;		//left shift toggle for 5 bit
	toogle = ~toogle; //toogle==0  --->>> toogle= ~toogle  --->>> toogle= ~0 --->>> toogle= 1
	
}
ISR(INT1_vect){//activate any logic chance of input signal
	PORTB = toogle<<5;		//left shift toggle for 5 bit
	toogle= ~toogle;

}

int main(void)
{
	//pd2 input for button
	DDRD = 0xFC; //0b1111 0011
	
	//pb5 output for led 
	DDRB  =0x20; //0b0010 0000
	
	EICRA = 0x06;//0b0000 0110
	//masks ISC11 and ISC10  ISC01 and ISC00 . 
	//ISC1x is logic change interrupt
	//ISC0x is falling edge interrupt 
	//0b (INT1)01[any logical change] (INT0)10[_falling]
	
	EIMSK = 0x03; // enable int1 and int0
	//Enable interrupt mask register
	
	sei();
	
	toogle =0x1;
	
	//We do not need while because all precess done in ISR(interrupt service routine) 
// 	while (1)
// 	{
// 	}
}