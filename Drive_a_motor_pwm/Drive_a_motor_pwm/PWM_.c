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
	if(cyclefreq<F_CPU/(1024*8*(2-tim_eighbit))){
		prescalar = 10;
	}
	if(cyclefreq<F_CPU/(256*8*(2-tim_eighbit))){
		prescalar = 8;
	}
	if(cyclefreq<F_CPU/(64*8*(2-tim_eighbit))){
		prescalar = 6;
	}
	if(cyclefreq<F_CPU/(8*8*(2-tim_eighbit))){
		prescalar = 2;
	}
	if(cyclefreq<F_CPU/(1*8*(2-tim_eighbit))){
		prescalar = 1;
	}
	
	if (timtype == 0)
	{
		TCCR0A = ((0b1010)<<4) | (0x0F & 0b11);
		TCCR0B = (0xF0 & TCCR0B) | (0x0F & prescalar);
		 
	} 
	else if (timtype == 1)
	{
		TCCR0A = (0xF0 & (0b1010)<<4) | (0x0F & 2);
		TCCR1B = (0xF0 & TCCR1B) | (0x0F & prescalar);
		
	} 
	else
	{
		TCCR2A = (0xF0 & (0b1010)<<4) | (0x0F & 0b11);
		TCCR2B = (0xF0 & TCCR2B) | (0x0F & prescalar);
	}
return 0;	
}

void pwmdef_pin(ui8 pin){// pin 
//	DDRB //1 çýkýþ //0 giriþ
//	PORTB //1 high //0 low
//OC0A PD6 6
//OC0B PD5 5
//OC1A PB1 9
//OC1B PB2 10
//OC2A PB3 11
//OC2B PD3 3
	switch(pin){
		 case 6:
		 DDRD |= (1<<6); 
		 TIMSK0 |=(1<<OCIE0A);
		 break;
		 
		 case 5:		 DDRD |= (1<<5);
		 TIMSK0 |=(1<<OCIE0B);
		 break;
		 
		 case 9:
		 DDRB |= (1<<1);
		 TIMSK1 |=(1<<OCIE1A);
		 break;
		 
		 case 10:
		 DDRB |= (1<<2);
		 TIMSK1 |=(1<<OCIE1B);
		 break;
		 
		 case 11:
		 DDRB |= (1<<3);
		 TIMSK2 |=(1<<OCIE2A);
		 break;
		 
		 case 3:
		 DDRD |= (1<<3);
		 TIMSK2 |=(1<<OCIE2B);
		 break;
		
	}
}

void setTCNT0(ui8 tcnt0){
	TCNT0 = tcnt0;
}
void setOCR0A(ui8 ocr0a){
	OCR0A = ocr0a-1;
}
void setOCR0B(ui8 ocr0b){
	OCR0B = ocr0b-1;
}
ui8 getTCNT0(void){
	return TCNT0;
}
void setICR1(ui16 icr1){
	ICR1 = icr1-1;
}
void setOCR1A(ui16 ocr1a){
	OCR1A = ocr1a-1;
}
void setOCR1B(ui16 ocr1b){
	OCR1B = ocr1b-1;
}
ui16 getTCNT1(void){
	return (TCNT1L)|(TCNT1H <<8);
}

void setTCNT2(ui8 tcnt2){
	TCNT2 = 0xFF-tcnt2;
}
void setOCR2A(ui8 ocr2a){
	OCR2A = ocr2a-1;
}
void setOCR2B(ui8 ocr2b){
	OCR2B = ocr2b-1;
}
ui8 getTCNT2(void){
	return TCNT2;
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
/*
	case(timtype){
		'0': //activte timer0 8 bit timer
			
			break;
		'1': //activte timer1 16 bit timer
			break;
		'2': //activte timer2 8 bit timer
			break;
		default ;//timer type unvalid
	}
*/
return 0;	
}