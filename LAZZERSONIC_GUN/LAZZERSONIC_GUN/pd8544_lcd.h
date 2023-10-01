/*
 * pd8544_lcd.h
 *
 * Created: 18.09.2023 12:01:10
 *  Author: tnc
 */ 


#ifndef PD8544_LCD_H_
#define PD8544_LCD_H_
#include <xc.h>


//extern short allow_SPI_transfer;


extern void Port_Init_lcd();
extern void SPI_Init();
extern void SPI_SS_Enable();
extern void Display_Cmnd(int data);
extern void Display_Init_pcd8544();
extern void Display_Clear_pcd8544();
extern void Display_SetXY_pcd8544(int X, int Y);
extern void Display_Dig_pcd8544(int dig  , int X, int Y);
extern void Process_distance_pcd8544(uint16_t mm_dist);

#endif /* PD8544_LCD_H_ */