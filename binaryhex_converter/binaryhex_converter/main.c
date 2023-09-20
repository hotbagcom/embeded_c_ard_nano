/*
 * main.c
 *
 * Created: 8/26/2023 6:51:42 PM
 *  Author: tnc
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL //16MHz internal clock
#endif

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "LCD_2X16/lcd.h" // JHD-2X16-I2C
int main(void)
{
	DDRC = 0x30; 
	lcd_init(LCD_DISP_ON_CURSOR);
	lcd_clrscr();
	
	
	
	
    while(1)
    {
		lcd_home();
		lcd_puts("Welcome");
		lcd_gotoxy(1,0);
		lcd_puts("Home");
    }
}