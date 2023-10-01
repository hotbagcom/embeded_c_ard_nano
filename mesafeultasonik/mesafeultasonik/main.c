/*https://atmega32-avr.com/distance-measurement-using-hc-sr04-avr-microcontroller/    xxxx
https://www.youtube.com/watch?v=mD0w-Fd9LSs
 * main.c
 *
 * Created: 9/8/2023 3:08:13 PM
 *  Author: tnc
 */ 
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

enum  uint8_t{
	RST,
	DC,
	SS,
	MOSI,
	MISO,
	SCK
	};

unsigned char dist_value[8] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ;
static const unsigned char LVL_DIST[8][8] ={
	{ 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 },
	{ 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xC0 },
	{ 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 },
	{ 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 },
	{ 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0xF8 },
	{ 0xFC , 0xFC , 0xFC , 0xFC , 0xFC , 0xFC , 0xFC , 0xFC },
	{ 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE },
	{ 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF },
};
static const unsigned char DIG[11][8] ={
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
	{0x00 , 0x41 , 0x22 , 0x14 , 0x08 , 0x00 , 0x00 , 0x00 },//>		
};
static const unsigned char MAX_MIN[25] ={	
	 0x0E , 0x01 , 0x0E , 0x01 ,0x0E , 0x00 , 0x0E , 0x05 ,0x0E , 0x00 , 0x0D , 0x02 , 0x0D ,
	 0xE0 , 0x10 , 0xE0 , 0x10 , 0xE0 , 0x00 , 0xF0 , 0x00 , 0xF0 , 0x20 , 0x40 , 0xF0
};
const unsigned char DIST[15] ={
	0x3E , 0x22 , 0x1C , 0x00 , 0x3A , 0x00 , 0x24 , 0x2A , 0x12 , 0x00 , 0x02 , 0x3E , 0x02 , 0x00 , 0x14
};
const unsigned char CM[9] ={
	0x1C , 0x22 , 0x22 , 0x00 , 0x3C , 0x02 , 0x3C , 0x02 , 0x3C 
};

volatile unsigned short hc_sr04_cnt;

#include <avr/io.h>         //header to enable data flow control over pins
#include <util/delay.h>     //header to enable delay function in program
#include <avr/interrupt.h>

void Port_Init(){
	DDRB =0b101111;
	PORTB |=(1<<RST) | (1<<SS);
	
	DDRD =0x10;  //portd3 as input INT1
	PORTD =0x00; //all are low 
	EIMSK |=(1<<INT1) ;
	EICRA |=(1<<ISC10);
	
	TCCR1B |=(3<<CS10);		//set prescaller as 64  
}
ISR(INT1_vect){
	if (PIND & (1<<3)){TCNT1 = 0;}
	else {hc_sr04_cnt = TCNT1;}
}
unsigned char hc_sr04_meas(void){
	hc_sr04_cnt =0;
	PORTD |= 1<< 4;			//set echo
	_delay_us(10);				//for 10ms
 	PORTD &=~(1<<4);		//clear echo
	 
	while(hc_sr04_cnt==0) ;
	if (hc_sr04_cnt<3700){return 0.000004 * hc_sr04_cnt /2 * 34300 ;}else{return 41;}
}
void SPI_Init(){
	SPCR = (1<< SPE)|(1<<MSTR)|(1<<SPR0);//ENABLE SPI , SET AS MASTER , SET PRESCALLER AS fosc/16 IN SPI CONTROL CENTER 
}
void SPI_SS_Enable(){
	PORTB &=~(1<<SS);
}
void SPI_SS_Disable(){
	PORTB |= (1<<SS);
}
void SPI_Tranciver(unsigned char data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}
void Display_Reset(){
	PORTB &= ~(1<<RST);
	_delay_ms(100);
	PORTB |= (1<<RST);
}
void Display_Cmnd(unsigned char data){
		PORTB &= ~(1<<DC);
		SPI_Tranciver(data);
		PORTB |= (1<<DC);
}
void Display_Init(){
	Display_Reset();//reset the display
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
}
void Display_SetXY(unsigned char X, unsigned char Y){
	Display_Cmnd(0X80|X); //SET COLUM 0-83
	Display_Cmnd(0X40|Y); //SET A RO 0 - 5
}
void OFF_Dig(unsigned char X, unsigned char Y){
	Display_SetXY(X,Y);
	for(int index = 0;index<5; index++){SPI_Tranciver(0);}//clear buffer
}
void Display_Dig(unsigned char dig ,unsigned char X, unsigned char Y){
	Display_SetXY(X,Y);
	for (int index = 0 ; index<5 ; index++){
	SPDR = DIG[dig][index];
	while(!(SPSR & (1<<SPIF)));
	_delay_ms(10);
	}
}
void Display_level(){
	for (int i =0; i<8 ;i++)
	{
		unsigned int lvl =dist_value[i] /8 ;
		
		for (int index =0 ; index <5 ; index++)
		{
			Display_SetXY(15+i*8 , index);
			for(int index1 = 0;index1<5; index1++){SPI_Tranciver(0);}			
		}
		for (int index =0 ; index <lvl ; index++)
		{
			Display_SetXY(15+i*8 , 5-index);
			for (int index1 =0 ; index1 <8 ; index1++)
			{
				SPDR = LVL_DIST[7][index1];
				while(!(SPSR & (1<<SPIF)));
			}
		}
		if ((dist_value[i]-lvl * 8)>0)
		{
			Display_SetXY(15+i*8 , 5-lvl);
			for(int index1 = 0;index1<5; index1++){
			
				SPDR = LVL_DIST[dist_value[i]-lvl*8][index1];
				while(!(SPSR & (1<<SPIF)));
			}
		}
	_delay_ms(10);
	}
}
void Display_Dist(unsigned char buffer){
	unsigned char temp_int_0 ,temp_int_1 ;//single digit double digit
	 if (buffer ==41)//distance >40
	 {
		 Display_Dig(10,35,0);
		 Display_Dig(4,42,0);
		 Display_Dig(0,48,0);
	 }
	 else//distance<40
	 {
		 OFF_Dig(35,0);
		 temp_int_0= (buffer%100) /10;
		 temp_int_1= buffer%10;
		 if (temp_int_0 >0)
		 {
			 Display_Dig(temp_int_0,42,0);
			 Display_Dig(temp_int_1,48,0);
		 } 
		 else
		 {
			 if (temp_int_1>0)
			 {
				 OFF_Dig(42,0);
				 Display_Dig(temp_int_1,48,0);
			 }
		 }
	 }
}
void Display_label(){
	//word Dist
	Display_SetXY(20,0);
	for (int index = 0; index <15 ;index++)
	{
		SPDR = DIST[index];
		while(!(SPSR & (1<<SPIF)));
		_delay_ms(10);
	}
	//word CM
	Display_SetXY(55,0);
	for (int index = 0; index <9 ;index++)
	{
		SPDR = CM[index];
		while(!(SPSR & (1<<SPIF)));
		_delay_ms(10);
	}
	//Word min max
	Display_SetXY(0,1);
	for (int index = 0; index <13 ;index++)
	{
		SPDR = MAX_MIN[index];
		while(!(SPSR & (1<<SPIF)));
		_delay_ms(10);
	}
	Display_SetXY(0,1);
	for (int index = 13; index <25 ;index++)
	{
		SPDR = MAX_MIN[index];
		while(!(SPSR & (1<<SPIF)));
		_delay_ms(10);
	}
	//line y
	for (int index = 0; index <6 ;index++)
	{
		Display_SetXY(14,index);
		SPDR = 0xFF;
		while(!(SPSR & (1<<SPIF)));
		_delay_ms(10);
	}
	//line x
	Display_SetXY(14,5);
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	//
	Display_SetXY(15,5);
	for (int index = 0; index <69 ;index++)
	{
		SPDR = 0x80;
		while(!(SPSR & (1<<SPIF)));
		_delay_ms(10);
	}
}
void Display_Clear(){
	PORTB |= (1<<DC);
	for (int k=0; k<503 ; k++){SPI_Tranciver(0x00);}
	PORTB &= ~(1<<DC);
}
void Add_Dist_value(unsigned char value){
	for(int i=7 ; i>0 ; i--)
	dist_value[i] = dist_value[i-1];
	dist_value[0] = value;
	
}


int main(void){
	unsigned char sm; //measure the distance
	Port_Init();
	SPI_Init();
	SPI_SS_Enable();
	Display_Init();
	Display_Clear();
	Display_label();
	
	
	sei();
	while(1){
		sm = hc_sr04_meas();
		if (sm>40){sm=40;	Display_Dist(41);} 
		else{Display_Dist(sm);}
		
		Add_Dist_value(sm);
		Display_level();
		_delay_ms(200);
	}
}

	 
/*

ISR(INT1_vect){
// 	if (i)				//high to low
// 	{
// 		TCCR1B=0;		//disabling counter
// 		pulse=TCNT1;	//count memory is updated to integer
// 		TCNT1=0;		//resetting the counter memory
// 		i=0;			//set next interrupt as low to high
// 		PORTD |= (0x1<<PIND2);
// 	}
// 	else if(i==0){		//low to high
// 		TCCR1B|=(1<<CS10);//enabling counter
// 		i=1;			//set next interrupt as high to low
// 		_delay_ms(10);
// 		PORTD &= ~(0x1<<PIND2);
// 	}
PORTD &= ~(1<<PIND4);
}
ISR(INT0_vect){
	PORTD |=(1<<PIND4);
}

int main(void){
	
	DDRD = 0b10011;					//set echo and trig pin input and output
	EIMSK |=(1<<INT1) | (1<<INT0);	//enabling interrupt1
	EICRA |=(1<<ISC10);				//setting interrupt triggering logic change
	
	//int COUNTA ;//storing digital output
	
	sei();
	while(1){
// 		PORTD |=0x1<<PIND3;			//set echo  
// 		_delay_ms(10);				//for 10ms
// 		PORTD &=~(1<<PIND3);		//clear echo
// 		_delay_us(10);
// 		COUNTA = (int)(pulse/58);			//getting the distance based on formula on introduction
// 		
// 		//turn on buzzer accordin to pulse ýsr()
		

		
	}
}

*/
