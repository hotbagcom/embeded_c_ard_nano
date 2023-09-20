
#include <18F4520.h>   //Kullanilan mikrodenetleyicinin surucusu derleyici uzerinde kaynak dosyasina tanimlanir.
  

/*FUSES: cihaza bagli cesitli opsiyonlari ayarlamak icin kullanilir. Mevcut ozellikleri en ustte belirtilir. 
       Konfigurasyon bileri vardir, bu komut ile mikrodenetleyicinin konfigurasyon pinleri ayarlan齬.
  Yaygin kullanilan cihaz secenekleri:
      路 LP, XT, HS, RC :LP dusuk guclu kristal osilator, XT klasik kristal osilator, HS yuksek hizli kristal osilator, RC Direnc / Kondansator osilator
      路 WDT, NOWDT  :Programin olasi kilitlenmelerde resetlenmesini sa餷ar. WDT dersek bu mod aktif, NOWDT dersek bu mod pasiftir.     
      路 PROTECT, NOPROTECT :Kod koruma, mikrokontrollerin i鏸ne y黭leyecegin kodlar kopyalananaz demek, NOPROTECT ise kodlar geriye okunup kopyalanabilir demektir. 
      路 PUT, NOPUT (Power Up Timer) : NOPUT, zamanlayicilar kapali demek, yani timer olarak gecen zaman dongulerini kullanmayacagini belirtirsin.
      路 BROWNOUT, NOBROWNOUT:Dusuk gerilimle program reset, bu ozellik gerilim seviyesi belli bir degerin altina dustugunde, mikrokontrollerin reset atmasidir,
          genelde NOBROWNOUT modunda tutulur, eyer BROWNOUT olarak kullanilirsa, ani gerilim dalgalanmalarinda sistem surrekli resetlenir.
      路 INTRC: Internal, dahili osilatoru kullan anlamina gelir.    
      路 NOMCLR: Master Clear functionality, MCLR girisi PIC'i resetlemek icin kullanilir. Bu fuse yoksa #FUSE MCLR ve MCLR pini 10k ile 5V a 
Tum #FUSES ozelliklerini kapatmak icin " #FUSES none" kullanilir.                              
*/                                          
                                         
#FUSES NOWDT, WDT1, NOLVP,NOCPD, NOWRT, NOMCLR, IESO, INTRC 
#use delay (internal=8MHz)                         
//#use rs232(baud=9600, xmit=PIN_C6,rcv=PIN_C7)
                                                 
                                           
                                              
#include "definitions.h"                                                                                                      
#include "main.h"                                  
#include "delayms.h"                                
#include "ucregisters.h"               
#include "init.h"                                      
#include "button.h"  
#include "DS1307.h"
#include "usart_rs232.h"

                           
#include "lcd.h"  
#include "lcd.c"                        
#include "button.c"          
#include "delayms.c"  
#include "DS1307.c"  
#include "usart_rs232.c"    
#include "modbusrtu.c"     
                        
                                            
uint16_t iSayac; 
tmr1_time_t tmr1_time;   
                           
                  
void setupTimerIOSC(void){ 
 //Setup Osilator
   IRCF2 = 1;       
   IRCF1 = 1;
   IRCF0 = 1;                   
                        
   //D ve P portlari cikis olarak ayarlanip sifirlandi.
   TRISB = 0x00;                 
   TRISD = 0x00;
   TRISE = 0x00;      
   PORTB = 0x00;                                                      
   PORTD = 0x00;
   PORTE = 0x00;  
                                                     
   //TMR1 etkinlik bayraklari sifirlandi
   tmr1_time.all = 0;

   // Kesmelere izin verildi
   RCON_IPEN = 1;                                              
   INTCON_GIE_GIEH = 1;
   INTCON_PEIE = 1;          
   IPR1_TMR1IP = 1; // TMR1 Overflow Interrupt Priority bit
   PIE1_TMR1IE = 1; // TMR1 Overflow Interrupt Enable bit
   PIR1_TMR1IF = 0; // TMR1 Overflow Interrupt Flag bit
                                                                                      
   //TIMER1 Konfigurasyon //50us
   T1CON_RD16 = 0; // 16 - bit Read / Write Mode Enable bit
   T1CON_T1RUN = 0; // Timer1 System Clock Statusbit
   T1CON_T1CKPS1 = 0; // Timer1 Input Clock Prescale Select bits | 1:1 Olcekleme
   T1CON_T1CKPS0 = 0; // Timer1 Input Clock Prescale Select bits |
   T1CON_T1OSCEN = 0; // Timer1 Oscillator Enable bit
   T1CON_T1SYNC = 0; // Timer1 External Clock Input Synchronization Select bit
   T1CON_TMR1CS = 0; // Timer1 Clock Source Select bit
   TMR1H = 0xF8; //Tasma saayci|
   TMR1L = 0x30; // |||65436 decimal sayisina esit. 
   T1CON_TMR1ON = 1; // Timer1 On bit                                  
}      
                                                   
                                                                                                                 
void printOn2x16(){
   delayUsFun(40);                                                                                                                                      
   twoLinesBCDMode();   
   LinesActive();  
   lcdCursorOff();
   lcdShiftRight();                     
   lcdClear();                                                   
   memcpy(strLCD, "INFORMELEKTRONIKAR-GE    STAJYER", 32); 
   countLCDChar=0;   
   LCDW1SecFlag=1;                       
   // LCD_Custom_CHAR(7, legrandLCDCharacter);
   //LCD_Custom_CHAR(6, stickManLCDCharacter);                               
}         
                                                 
#INT_TIMER1                                                           
void interrupt(){            
   TMR1H = 0xF8; //Tasma saayci|
   TMR1L = 0x30; // |||15536 decimal sayisina esit.                     
   PIR1_TMR1IF = 0; // Kesme bayragi sifirlaniyor.
   iSayac++; 
   if (iSayac % 1 == 0) //1ms 
      tmr1_time.tmr1_flags._1ms = true;      
   if (iSayac % 2 == 0) //2ms     
      tmr1_time.tmr1_flags._2ms = true;
   if (iSayac % 20 == 0) //20ms  
      tmr1_time.tmr1_flags._20ms = true;             
   if (iSayac % 50 == 0) //50ms 
      tmr1_time.tmr1_flags._50ms = true;
   if (iSayac > 1000){   // 1 Saniye         
      tmr1_time.tmr1_flags._1000ms = true;  
      iSayac = 0;                                                 
   }               
                          
}                                                
                                              
             
#ORG 0x0100,0x0400                                                                    
void main(){                                 
   setupTimerIOSC();
   printOn2x16(); 
   I2C_Init();           
   DS1307_Init();
   USART_Init(); 
   while(TRUE){          
     // WDTreset(); 
     if(tmr1_time.tmr1_flags._1ms){  
         tmr1_time.tmr1_flags._1ms = false; 
         if(RCIF_PIR1==1){  
            T1CON_TMR1ON = 0;
            modbusRTUrx();
            T1CON_TMR1ON = 1;  
         }                                    
      }                                             
      if(tmr1_time.tmr1_flags._1000ms){              
         tmr1_time.tmr1_flags._1000ms = false;         
         LCDW1SecFlag=1;             
         PORTB_7=~PORTB_7;                        
      }            
       
      if(tmr1_time.tmr1_flags._20ms){  
         tmr1_time.tmr1_flags._20ms = false; 
         ds1307_run(27,7,23,2,14,59,10);                             
      }   
      if(tmr1_time.tmr1_flags._2ms){                                 
         tmr1_time.tmr1_flags._2ms = false;  
         if(LCDW1SecFlag){     
            LCD_String();    
         } 
      }   
   }                                                        
}                                       


void WDTreset(void){               
   #asm                
   CLRWDT
   #endasm                                 
}                
                                                                                            
                                                                                                
                                       

       
                   
