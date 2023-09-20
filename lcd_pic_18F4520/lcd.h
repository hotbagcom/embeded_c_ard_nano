#ifndef LCD_H                                                                                               
#define LCD_H 
                                            
extern uchar_t strLCD[32];  
extern uint8_t countLCDChar;
extern uchar_t LCDW1SecFlag;        
//uchar_t legrandLCDCharacter [8]= {0x08, 0x1F, 0x19, 0x11, 0x11, 0x13, 0x1F, 0x02};
//uchar_t stickManLCDCharacter [8] = {0x0E, 0x0E, 0x04, 0x0F, 0x14, 0x0A, 0x1B, 0x00};
void lcdcmd(uchar_t);                                                                               
void lcddata(uchar_t);                                                                     
void LCD_String();                                                                             
//LCD (CHIP: HD44780), COMMANDS
void twoLinesBCDMode();
void LinesActive();
void lcdClear();                              
void lcdGoToFirstLine();     
void lcdGoToSecondLine();
void lcdShiftRight();
void lcdCursorOn(); 
void lcdCursorOff();
void LCD_Custom_Char (uchar_t loc, uchar_t *msg);

#endif //LCD_H    
