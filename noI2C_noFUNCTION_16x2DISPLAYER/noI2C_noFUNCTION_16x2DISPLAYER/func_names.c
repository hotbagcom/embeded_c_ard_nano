/*
 * func_names.c
 *
 * Created: 28.08.2023 12:02:03
 *  Author: tnc
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

#include "func_names.h"

//PB
#define LCD_RS 0
#define LCD_E 1

//-Display Cursor Blink	{1}{(1/0)[On/Off]}{(1/0)[On/Off]}{(1/0)[On/Off]}
#define  onDoffCoffB 0x0C  // 0b1100
#define  onDoffConB  0x0D  // 0b1101
#define  onDonCoffB  0x0E  // 0b1110
#define  onDonConB   0x0F  // 0b1111

//-Function Set			{1}{(1/0)[8bit/4bit]} {(1/0)[2line/1line]}{(1/0)[5x10/5x7]}{*}{*}
#define fPinoLinesevPix 0x20 // 0b10 0000
#define fPinoLinetenPix 0x24 // 0b10 0100
#define fPintLinesevPix 0x28 // 0b10 1000
#define fPintLinetenPix 0x2C // 0b10 1100
#define ePinoLinesevPix 0x30 // 0b11 0000
#define ePinoLinetenPix 0x34 // 0b11 0100
#define ePintLinesevPix 0x38 // 0b11 1000
#define ePintLinetenPix 0x3C // 0b11 1100

//Entry Shift Mode Set	{1} {(1/0)[Display/Cursor]}{(1/0)[Right/Left]}{*}{*}
#define sCurs_sLeft 0x10 // 0b1 0000
#define sCurs_sRig  0x14 // 0b1 0100
#define sDisp_sLeft 0x18 // 0b1 1000
#define sDisp_sRig  0x1C // 0b1 1100

void WRITE_LCD_Data(unsigned char data){

	//PORTD LOWER data defined
	
	PORTD = (PORTD & 0x0F) | (data & 0xF0);//data 7to4
	
	PORTB |=(0x1<<LCD_RS);//define RS {(1/0)[Data/command-instruction]}
	PORTB |=(0x1<<LCD_E); //define Enable as high and hold for a time
	_delay_ms(2);//tPw min =450 ns
	PORTB &= ~(0x1<<LCD_E);//set only enable on PORTB
	_delay_ms(2);
	
	//PORTD UPPER data defined
	PORTD = (PORTD & 0x0F) | (data << 4);
	PORTB |=(0x1<<LCD_E); 			 //EN=1
	_delay_ms(2);
	PORTB &= ~(0x1<<LCD_E);		//EN=0
	_delay_ms(2);
}

void WRITE_LCD_Commands(unsigned char command){
	
	
	//PORTD LOWER command defined
	PORTD = (PORTD & 0x0F) | (command && 0XF0);
	
	PORTB &= ~(0x1<<LCD_RS) ;		 //RS =0
	PORTB |=  (0x1<<LCD_E); 		 //EN=1
	_delay_ms(2);
	PORTB &= ~(0x1<<LCD_E);		//EN=0
	//RS is not changed because of and command
	_delay_ms(2);
	
	//PORTD UPPER command defined
	PORTD = (PORTD & 0x0F) | (command << 4);
	PORTB |=(0x1<<LCD_E); 			 //EN=1
	_delay_ms(2);
	PORTB &= ~(0x1<<LCD_E);		//EN=0
	_delay_ms(2);
}

//tamam
void lcdReturnHome(){//-Return Home	
	/**
	{1}{*}
	**/
	WRITE_LCD_Commands(0x02);			
}
//tamam
void lcdClear(){//Clear Display			
	/*
	{1}
	*/
	WRITE_LCD_Commands(0x01);
}

//tamam
void lcdFunctionSet(unsigned char mod){
	/**
	@brief set 8-4 pin , used 1-2 line , pix 5X7- 5X10
	@param mod = fPin-ePin oLine-tLine sevPix-tenPix
	return none
	*/
	/*
	{1}
	{(1/0)[8bit/4bit]} 
	{(1/0)[2line/1line]}
	{(1/0)[5x10/5x7]}
	{*}
	{*}
	*/
	if (mod == fPinoLinesevPix)
		WRITE_LCD_Commands(mod);
	else if (mod == fPinoLinetenPix)
		WRITE_LCD_Commands(mod);
	else if (mod == fPintLinesevPix)
		WRITE_LCD_Commands(mod);
	else if (mod == fPintLinetenPix)
		WRITE_LCD_Commands(mod);
	else if (mod == ePinoLinesevPix)
		WRITE_LCD_Commands(mod);
	else if (mod == ePinoLinetenPix)
		WRITE_LCD_Commands(mod);
	else if (mod == ePintLinesevPix)
		WRITE_LCD_Commands(mod);
	else if (mod == ePintLinetenPix)
		WRITE_LCD_Commands(mod);
}


//tamam
void lcdDispCursBli(unsigned char mod){
	/*
	{1}
	{(1/0)[On/Off]}
	{(1/0)[On/Off]}
	{(1/0)[On/Off]}
	*/
	if (mod == onDoffCoffB)
		WRITE_LCD_Commands(mod);
	else if (mod == onDoffConB)
		WRITE_LCD_Commands(mod);
	else if (mod == onDonCoffB)
		WRITE_LCD_Commands(mod);
	else if (mod == onDonConB)
		WRITE_LCD_Commands(mod);  
	/*else //off display
		WRITE_LCD_Commands(0x08);*/
}


void lcdShift(unsigned char mod){//Entry Shift Mode Set		
	/*
	{1} 
	{(1/0)[Display/Cursor]}
	{(1/0)[Right/Left]}
	{*}
	{*}
	*/	
	if (mod == sCurs_sLeft)
		WRITE_LCD_Commands(mod);
	else if (mod == sCurs_sRig)
		WRITE_LCD_Commands(mod);
	else if (mod == sDisp_sLeft)
		WRITE_LCD_Commands(mod);
	else if (mod == sDisp_sRig)
		WRITE_LCD_Commands(mod);
}

void lcdShiftDecCurs(){
	WRITE_LCD_Commands(0x04);
}

void lcdShiftIncCurs(){
	WRITE_LCD_Commands(0x06);
}
void lcdShiftDispRig(){
	WRITE_LCD_Commands(0x05);
}
void lcdShiftDispLef(){
	WRITE_LCD_Commands(0x07);
}
//tamam
void lcdGoToFirstLineBegin(){
	WRITE_LCD_Commands(0x80);
}
//tamam
void lcdGoToSecondLineBegin(){
	WRITE_LCD_Commands(0xC0);
}

void INITIALIZE_LCD_Commands(){
	lcdReturnHome();
	lcdFunctionSet(fPintLinesevPix);
	lcdDispCursBli(onDonCoffB);
	lcdShift(sDisp_sRig);
	lcdClear();
	
}