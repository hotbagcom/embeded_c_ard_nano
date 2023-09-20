
#include "definitions.h" 
#include "lcd.h"
#include "ucregisters.h" 
#include "delayms.h" 
                
              
/*Hex Code   LCD Ekran Karsiligi 
 Initialize Lcd in 4-bit mode               0x02
1   Function Set: 8-bit, 1 Line, 5×7 Dots   0x30   48                                             
2   Function Set: 8-bit, 2 Line, 5×7 Dots   0x38   56                           
3   Function Set: 4-bit, 1 Line, 5×7 Dots   0x20   32
4   Function Set: 4-bit, 2 Line, 5×7 Dots   0x28   40 
0F   LCD acik, Imlec acik
01   Ekrani Temizle                                
02   Satir Basina Dön           
04   Imleci Sola Kaydir
06   Imleci Saga Kaydir
05   Ekrani Saga Kaydir
07   Ekrani Sola Kaydir
0E   Ekran Acik, Imleç Yanip Sonuyor
80   Imleci Ilk Satirin Basinda Konumlandir
C0   Imleci Ikinci Satirin Basinda Konumlandir        
3C   Ikinci Satiri Etkinlestir                                                    
08   Ekran Kapali, Imleç Kapali             
OC   Ekran Acik, Imlec Kapali*/  
                                           
uchar_t strLCD[32];
uint8_t countLCDChar;
uchar_t LCDW1SecFlag;
                                        
/*LCD (CHIP: HD44780), COMMAND MODE*/ 
void dis_cmd(uchar_t cmdout){ 
   PORTD=cmdout;      //Send command to lcdport=PORTB
   PORTE_RS=0;                  
   PORTE_E=1;   
   delayUsFun(1);
   PORTE_E=0;                              
}                                  



void lcdcmd(uchar_t cmd_value){         
   uchar_t cmd_value1;                             
   cmd_value1 = (cmd_value & 0xF0);      // Mask lower nibble because RB4-RB7 pins are being used   
   dis_cmd(cmd_value1);                   // Send to LCD
   cmd_value1 = ((cmd_value<<4) & 0xF0);  // Shift 4-bit and mask
   dis_cmd(cmd_value1);                   // Send to LCD
}
 // 2 lines are active.             
void twoLinesBCDMode(){ 
   lcdcmd(0x02);
}
                       
// 2 lines are active. BCD MODE
void LinesActive(){ 
   lcdcmd(0x28);               
}                         
// LCD screen clear.                  
void lcdClear(){
   lcdcmd(0x01);                  
}                  
// Go to first line.
void lcdGoToFirstLine(){ 
   lcdcmd(0x80);
}                                 
// Go to second line.                       
void lcdGoToSecondLine(){
   lcdcmd(0xC0);
}                       
// Shift cursor to right.
void lcdShiftRight(){  
   lcdcmd(0x06);                                                    
}                                          
// Display on, Cursor on             
void lcdCursorOn(){                 
   lcdcmd(0x0E);
}                                     
//Display off, Cursor off
void lcdCursorOff(){                                            
   lcdcmd(0x0C);                            
}                             
 
void dis_data(uchar_t dataout) { 
   PORTD=dataout;   //Send data to lcdport=PORTB
   PORTE_RS=1;                  
   PORTE_E=1;                         
   delayUsFun(1);
   PORTE_E=0;  
   //PORTD=0x00;
   //while(test_bit(PORTD,7));          
}  
/*DATA MODE*/  
void lcddata(uchar_t text){ 
   uchar_t data;                                                                
   data=(text & 0xF0);        
   dis_data(data);    
   data=((text<<4)&0xF0);     
   dis_data(data);                 
}                               
void LCD_String(){ 
   if(countLCDChar==0) 
      lcdGoToFirstLine();
   if(countLCDChar==16)  
      lcdGoToSecondLine();   
   if(countLCDChar<=31){ 
      lcddata(strLCD[countLCDChar]);
      countLCDChar++;  
   }else{   
      LCDW1SecFlag=0;
      countLCDChar=0;
   }                 
} 


void LCD_Custom_Char(uchar_t loc, uchar_t *msg){
    uchar_t i;
    if(loc<8)
    {
     lcdcmd(0x40 + (loc*8));    /* Command 0x40 and onwards forces the device to point CGRAM address */
     for(i=0;i<8;i++)          /* Write 8 byte for generation of 1 character */
        lcddata(msg[i]);      
    }                       
}

                                         

     
