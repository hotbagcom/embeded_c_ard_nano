/*
 * func_names.h
 *
 * Created: 28.08.2023 11:37:40
 *  Author: tnc
 */ 
#ifndef FUNC_NAMES_H_
#define FUNC_NAMES_H_
//extern unsigned char strLCD[32];  

extern  void WRITE_LCD_Data(unsigned char data);
extern void WRITE_LCD_Commands(unsigned char command);
extern void INITIALIZE_LCD_Commands(); 
// void LCD_String(*unsigned char);
extern void lcdReturnHome();
extern void lcdClear();
extern void lcdFunctionSet(unsigned char mod);
extern void lcdDispCursBli(unsigned char mod);
extern void lcdShift(unsigned char mod);
extern void lcdShiftDecCurs();
extern void lcdShiftIncCurs();
extern void lcdShiftDispRig();
extern void lcdShiftDispLef();
extern void lcdGoToFirstLineBegin();
extern void lcdGoToSecondLineBegin();


#endif /* FUNC_NAMES_H_ */