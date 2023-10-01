/*
 * pd8544_lcd.c
 *
 * Created: 18.09.2023 12:00:28
 *  Author: tnc
 */ 
// Vcc   ---------- +3.3V           |--------------
// SCK   -- 330 -- EXT OSC          |   right side
// SDin  -- 330 -- Data in          |
// D/!C  -- 330 -- Data Command	    |   Front 
//!(S)CE -- 330 -- Enable           |   Side
// GND   --- Gnd                    |
// Vout  ------|1uF(-------|GND	    |   left side
//!RST   -- 330 -- Reset            |--------------

#include "pd8544_lcd.h"

// int1 =d3 echo
// d4 trig

// RST d8
// DC  d9    d_c
// SS  d10  sce ce
// MOSI d11 d_in
// MISO d12  (boþta)
// SCK  d13 clock
//
#ifndef F_CPU
#define F_CPU 16000000UL//telling controller crystal frequency attached
#endif


#include <xc.h>
#include <avr/io.h>         //header to enable data flow control over pins
#include <util/delay.h>     //header to enable delay function in program
#include <avr/interrupt.h>


enum  uint8_t{
	RST,
	DC,
	SS,
	MOSI,
	MISO,
	SCK
};

short allow_SPI_transfer = 0x01;




void Port_Init_lcd(){
	DDRB =0b101111;
	PORTB |=(1<<RST) | (1<<SS);	
}
void SPI_Init(){
	SPCR = (1<< SPE)|(1<<MSTR)|(0<<SPR0);//ENABLE SPI , SET AS MASTER , SET PRESCALLER AS fosc/4 IN SPI CONTROL CENTER
}
void SPI_SS_Enable(){
	PORTB &=~(1<<SS);
}
void SPI_SS_Disable(){
	PORTB |= (1<<SS);
}
void SPI_Tranciver(int data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}
int16_t SPI_Tranciver_pcd8544(unsigned char data , uint16_t iii){
	if (allow_SPI_transfer)
	{
		SPDR = data;
		allow_SPI_transfer =0x00;
		iii++;
	}
	
	return iii;
}

void Display_Reset(){
	PORTB &= ~(1<<RST);
	_delay_ms(100);
	PORTB |= (1<<RST);
}

void Display_Clear_pcd8544(){
	PORTB |= (1<<DC);
	_delay_us(20);
	for (int k=0; k<504 ; k++){SPI_Tranciver(0x00);}
	_delay_us(20);
	PORTB &= ~(1<<DC);
}

void Display_Cmnd(int data){
	PORTB &= ~(1<<DC);
	_delay_us(30);
	SPI_Tranciver(data);
	PORTB |= (1<<DC);
	_delay_us(30);
}

void Display_SetXY_pcd8544(int X,int Y){
	
	Display_Cmnd(0x80|(X&0x7F)); //SET COLUM 0-83  6*(0:13)
	Display_Cmnd(0x40|(Y&0x07)); //SET A RO 0 - 5
}

void Display_Init_pcd8544(){
	
	Display_Reset();//reset the display
	PORTB &= ~(1<<DC);
	_delay_us(20);
	SPI_Tranciver(0x21);
	SPI_Tranciver(0xC0);
	SPI_Tranciver(0x07);
	SPI_Tranciver(0x13);
	SPI_Tranciver(0x20);
	SPI_Tranciver(0x0c);
	PORTB |= (1<<DC);
	_delay_us(20);
/*	
	Display_Cmnd(0x21); //cmnd set in addition mode
	
	Display_Cmnd(0xC0); //set the voltage by sending C0 means VOP = 5V
	//	Display_Cmnd(0x84); //set the voltage by sending C0 means VOP = 3.3V
	
	Display_Cmnd(0x07); //set the temp coefficient to 3 0b 0000 01 TC1 TC0
	
	Display_Cmnd(0x13); //set value of Voltage Bias System
	//	Display_Cmnd(0x15); //set value of Voltage Bias System
	//bias hala benim için muamma
	
	Display_Cmnd(0x20); //command set in basic mode
	//	Display_Cmnd(0x22); //command set in veryical mode
	
	Display_Cmnd(0x0c); //display result in normal mode
	//	Display_Cmnd(0x08); //display result in blank mode
	//	Display_Cmnd(0b00001001); //display result in all disp segment on mode
	//	Display_Cmnd(0b00001101); //display result in inverse video mode
*/
	Display_Clear_pcd8544();
}



const unsigned char DIG[13][8] ={
	{0x3E , 0x51 , 0x49 , 0x45 , 0x3E , 0x00 , 0x00 , 0x00 },//0
	{0x00 , 0x42 , 0x7F , 0x40 , 0x00 , 0x00 , 0x00 , 0x00 },//1
	{0x42 , 0x61 , 0x51 , 0x49 , 0x46 , 0x00 , 0x00 , 0x00 },//2
	{0x21 , 0x41 , 0x45 , 0x4B , 0x31 , 0x00 , 0x00 , 0x00 },//3
	{0x18 , 0x14 , 0x12 , 0x7F , 0x10 , 0x00 , 0x00 , 0x00 },//4
	{0x27 , 0x45 , 0x45 , 0x45 , 0x39 , 0x00 , 0x00 , 0x00 },//5
	{0x3C , 0x4A , 0x49 , 0x49 , 0x30 , 0x00 , 0x00 , 0x00 },//6
	{0x01 , 0x71 , 0x09 , 0x05 , 0x03 , 0x00 , 0x00 , 0x00 },//7
	{0x36 , 0x49 , 0x49 , 0x49 , 0x36 , 0x00 , 0x00 , 0x00 },//8
	{0x06 , 0x49 , 0x49 , 0x29 , 0x1E , 0x00 , 0x00 , 0x00 },//9
	{0x00 , 0x41 , 0x22 , 0x14 , 0x08 , 0x00 , 0x00 , 0x00 },
	{0b00111110,//m
	0b00000100,
	0b00111000,
	0b00000100,
	0b00111000,
	0x00	},
	{
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		
	}
};
void Display_Dig_pcd8544(int dig ,int X, int Y){
	//|((PORTB>>DC)& 0b1)  

	Display_SetXY_pcd8544(X,Y);
	
	for (int index = 0 ; index<6 ; index++){
		SPI_Tranciver(DIG[dig][index]);
	
	}
}

void Process_distance_pcd8544(uint16_t mm_dist){

		Display_Dig_pcd8544(11,83-6,1);
		Display_Dig_pcd8544(11,83-12,1);
	short aa =3;
	Display_Dig_pcd8544(12,(83-6*aa), 1);
	Display_Dig_pcd8544(12,(83-6*(aa+1)), 1);
	Display_Dig_pcd8544(12,(83-6*(aa+2)), 1);
	Display_Dig_pcd8544(12,(83-6*(aa+3)), 1);
	
	_delay_ms(10);
	for (;mm_dist!=0;mm_dist/=10)
	{
		Display_Dig_pcd8544(mm_dist%10,(83-6*aa), 1);
		aa++;
	}
	
	
}