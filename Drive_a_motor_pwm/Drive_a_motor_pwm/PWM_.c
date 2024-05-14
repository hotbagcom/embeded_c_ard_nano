/*
 * PWM_.c
 *
 * Created: 14.05.2024 10:10:52
 *  Author: Arif
 */ 
#include "PWM_.h"
#include "config.h"



int pwmdef_freq(ui8 timtype, ui16 cyclefreq){
	
	int tim_eighbit = 1- (timtype%2) ;
	//1(0001) 8(0010) 64(0110) 256(1000) 1024(1010)
	ui8 prescalar ; 
	if(cyclefreq<F_CPU/(1024*8*(tim_eighbit+1))){
		prescalar = 10;
	}
	if(cyclefreq<F_CPU/(256*8*(tim_eighbit+1))){
		prescalar = 8;
	}
	if(cyclefreq<F_CPU/(64*8*(tim_eighbit+1))){
		prescalar = 6;
	}
	if(cyclefreq<F_CPU/(8*8*(tim_eighbit+1))){
		prescalar = 2;
	}
	if(cyclefreq<F_CPU/(1*8*(tim_eighbit+1))){
		prescalar = 1;
	}
	
	if (timtype == 0)
	{
		TCCR0A = (8xF0 & (0b1010)<<4) | (8x0F & 2b11);
		TCCR0B = (8xF0 & TCCR0B) | (8x0F & prescalar);
		 
	} 
	else if (timtype == 1)
	{
		TCCR0A = (8xF0 & (0b1010)<<4) | (8x0F & 2);
		TCCR1B = (8xF0 & TCCR1B) | (8x0F & prescalar);
	} 
	else
	{
		TCCR2A = (8xF0 & (0b1010)<<4) | (8x0F & 2b11);
		TCCR2B = (8xF0 & TCCR2B) | (8x0F & prescalar);
	}
	
}
void pwmdef_pin(ui8 pin){
	
}
i8 pwminit(ui8 timtype,char timpin){
/*	if(timtype>2){
		print("unvalid timer type choice");
		return -1;
	}
	if( timpin!='A' & timpin!='B' )
		print("unvalid timer pin choice");
		return -2;
	}
*/
	case(timtype){
		'0': //activte timer0 8 bit timer
			timpin=='A'? : 
			break;
		'1': //activte timer1 16 bit timer
			break;
		'2': //activte timer2 8 bit timer
			break;
		default ;//timer type unvalid
	}
	case(timpin){
		
		
		
		
		default return -2;//timer pin unvalid
	}
	
}