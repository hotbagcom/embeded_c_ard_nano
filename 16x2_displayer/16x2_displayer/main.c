/*
 * main.c
 *
 * Created: 8/27/2023 1:49:39 PM
 *  Author: tnc
 */ 

#include <xc.h>
#include "lcd16x2/lcd.h"

int main(void)
{
	DDRD = 0XF0;
	lcd_init(LCD_DISP_ON_CURSOR);
	lcd_clrscr();
	lcd_home();
	lcd_puts("Welcome");
	lcd_gotoxy(0,1);
	lcd_puts("Home");
	
 
}