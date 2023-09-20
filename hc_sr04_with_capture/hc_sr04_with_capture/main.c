/*
 * main.c
 *
 * Created: 9/11/2023 9:51:45 AM
 *  Author: tnc
 */ 

#define  F_CPU 16000000UL

#define _TRIG (1<<1)
#define _ECHO (1<<0)
#define U_S PORTB
#define U_S_ DDRB
#define _0 (1<<0)
#define _1 (1<<1)
#define _2 (1<<2)
#define _3 (1<<3)
#define _4 (1<<4)
#define _5 (1<<5)
#define L_ PORTC
#define L__ DDRC
#define prescallar 0x02 //2   CS10
#define risefall 0x40  //rising ICES1

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>       
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uart_hal.h"


uint8_t print_dist[40] ;
uint32_t buffer_value[20];
static volatile uint16_t distance_mm;
static volatile uint16_t travel_distance ;
static volatile uint32_t travel_time ;
static  uint16_t cycle_tick_times;  //for overflow
static volatile uint16_t start_time ;
static volatile uint16_t finish_time;

static volatile short allow_trig_send;
static volatile short allow_echo_read;


void Ultros_Init();
void Ultros_Trig();
void Led_Init();
void Led_Update();
void Init_time();
void Clear_time();
uint16_t filter( );/////// filtreden geçecek deðerleri // general deðiþken olduðu için almakta sýkýnt çekmeyeceksin coef lerden sokup çýkar 

ISR(TIMER1_CAPT_vect){
	
	if (((TCCR1B & risefall)>>6)==0)//falling edge start timer
	{
		finish_time =TCNT1;
		TCCR1B |= risefall;
		allow_echo_read =0x01;
		allow_trig_send =0x01;
	}
	
	else if (((TCCR1B & risefall)>>6)==1)		//rising edge start timer
	{
		TCNT1=0x00;
		start_time = TCNT1;
		TCCR1B &= ~risefall;
	}
	
	
}

// 		ISR(TIMER1_OVF_vect){would not work at prescallar 1
// 		//	TCNT1=0x00;   
// 			cycle_tick_times ++;   //65536_tick i bir arttýr
// 		}


int main(void)
{
	
	int sample=0;
	uart_init(9600,0);
	Ultros_Init();
	allow_trig_send=0x01;
	Led_Init();
	Init_time();
	Clear_time();
	
	sei();
    while(1)
    {
		
		if (((U_S & 1<<_ECHO)==0 ))
		{
			_delay_ms(50);
			
			
			
			if (allow_echo_read>0x00) 
			{	//Led_Update(distance_mm/10);
			


				if(finish_time>start_time)
				{
					//travel_time =(cycle_tick_times);//travel_time = (travel_time <<16);//travel_time |= (start_time-finish_time);
							
					travel_time = (finish_time-start_time);
				}
						
				else if (finish_time<=start_time)
				{

//					if (cycle_tick_times > 0x00)
//						travel_time =((cycle_tick_times-1));
					//else
					//give logic error set struct with 8 bit
					
//travel_time =travel_time<<16;//travel_time |= (0xFFFF)-(start_time-finish_time);//travel_time |= (0xFFFF)-(start_time-finish_time);
				}
				
				if (sample>=10)
				{
					
					distance_mm =filter();
				
					memset(print_dist,0,sizeof(print_dist));
				
					sprintf((char*)print_dist,"distance: %u  mm  _     \n \r",distance_mm);
					//Led_Flash(cycle_tick_times);
					uart_send_string(print_dist);
					sample=0x00;
				}
				
				else 
				{
				travel_distance = travel_time/12;
				buffer_value[sample]=(int)travel_distance;
				sample++;
				}
				
				
			}
		Led_Update((int16_t)(distance_mm/10));
		_delay_ms(30);
		Clear_time();
			
		Ultros_Trig();
			
		allow_echo_read=0x00;
		allow_trig_send=0x00;
		}
		

	}
}


	
void Ultros_Init()
{
	U_S_ |= _TRIG;
	U_S_ &= ~_ECHO;
	U_S &= ~_ECHO;
	U_S &= ~_ECHO;
}
void Ultros_Trig()
{
	U_S |= _TRIG;
	_delay_us(10);
	U_S &= ~_TRIG;
}
void Led_Init()
{
	L__ = 0x3F;
	L_ = 0x3F;
	_delay_ms(100);
	L_ &= ~(0x3F) ;
}
void Led_Update(uint16_t a)
{
	if (a>40)
		L_ ^=0x3F;
	else {
		if (a>5)
			L_ =0x00;
		if (a>10)
			L_ |=0x01;
		if (a>15)
			L_ |=0x02;
		if (a>20)
			L_ |=0x04;
		if (a>25)
			L_ |=0x08;
		if (a>30)
			L_ |=0x10;
		if (a>35)
			L_ |=0x20;
	}
	
		
	 //a/5;  //for a/N lower than 2^A   PORTX ~(~(0x1<<A)

}

void Clear_time()
{
	travel_time=0x00;
	cycle_tick_times =0x00;
	start_time=0x00;
	finish_time=0x00;
//	TCNT1 =0x00;
}

void Init_time()
{
	//define timer thing set
	TCCR1A= 0x00;
	TCCR1B= risefall|prescallar;
	TCCR1C=0x00;
	TIMSK1 |= 1<<ICIE1 ; // | 1<<TOIE1  enable overflow interrupt
}
uint16_t filter(){
	uint32_t sum=0;
	
	for (int ii =0; ii<10;ii++)
	{
		sum += buffer_value[ii];	
	}
	
	return (uint16_t)(sum/10);
}

/*

#define F_CPU 16000000UL		//this header file is responsible for the cpu frequency hence we use atmega328p so its freq is 16Mhz
#include <avr/io.h>             // This header file includes the appropriate Input/output definitions for the device
#include <util/delay.h>         // to use delay function we need to include this library
#include <stdlib.h>             // we'll be using itoa() function to convert integer to character array that resides in this library
#include "uart_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define US_PORT PORTB           // we have connected the Ultrasonic sensor on port D. to use the ultrasonic we need two pins of the ultrasonic to be connected on port C
#define	US_PIN	PINB            // we need to initialize the pin resistor when we want to take input.
#define US_DDR 	DDRB            // we need data-direction-resistor (DDR) to set the direction of data flow. input or output. we will define it later, now we're just naming it.

#define US_TRIG_POS	PB1         // the trigger pin of ultrasonic sound sensor is connected to port C pin 0
#define US_ECHO_POS	PB0         // the echo pin of the ultrasonic sound sensor is connected to port C pin 1


#define US_ERROR		-1      // we're defining two more variables two know if the ultrasonic sensor is working or not
#define	US_NO_OBSTACLE	-2

uint8_t distance, previous_distance;



//We need total three functions to use the ultrasonic sound sensor. First we will initialize it, then generate ultrasonic sound wave and the
//third step is measuring the time between generated wave and echo back wave after hitting an object. The complete reference to use the ultrasonic
//sound sensor with math will be available on this link:  https://mendupmindcode.blogspot.com/2021/11/csr04-ultrasonic-sensor-with-atmega328p.html
//Though the tutorial is for arduino but the concept is all the same. Please read the article throughly to have a clear understanding.



void HCSR04Init();
void HCSR04Trigger();



void HCSR04Init()
{
	// we're setting the trigger pin as output as it will generate ultrasonic sound wave
	US_DDR|=(1<<US_TRIG_POS);
}

void HCSR04Trigger()
{   // this function will generate ultrasonic sound wave for 15 microseconds
	//Send a 10uS pulse on trigger line
	
	US_PORT|=(1<<US_TRIG_POS);	//high
	
	_delay_us(15);				//wait 15uS
	
	US_PORT&=~(1<<US_TRIG_POS);	//low
}

uint16_t GetPulseWidth()
{
	// this function will be used to measure the pulse duration. When the ultra sound echo back after hitting an object
	// the microcontroller will read the pulse using the echo pin of the ultrasonic sensor connected to it.
	
	uint32_t i,result;

	// Section - 1: the following lines of code before the section - 2 is checking if the ultrasonic is working or not
	// it check the echo pin for a certain amount of time. If there is no signal it means the sensor is not working or not connect properly
	for(i=0;i<600000;i++)
	{
		if(!(US_PIN & (1<<US_ECHO_POS)))
		continue;	//Line is still low, so wait
		else
		break;		//High edge detected, so break.
	}

	if(i==600000)
	return US_ERROR;	//Indicates time out
	
	//High Edge Found
	
	// Section -2 : This section is all about preparing the timer for counting the time of the pulse. Timers in microcontrllers is used for timimg operation
	//Setup Timer1
	TCCR1A=0X00;
	TCCR1B=(1<<CS11);	// This line sets the resolution of the timer. Maximum of how much value it should count.
	TCNT1=0x00;			// This line start the counter to start counting time

	// Section -3 : This section checks weather the there is any object or not
	for(i=0;i<600000;i++)                // the 600000 value is used randomly to denote a very small amount of time, almost 40 miliseconds
	{
		if(US_PIN & (1<<US_ECHO_POS))
		{
			if(TCNT1 > 60000) break; else continue;   // if the TCNT1 value gets higher than 60000 it means there is not object in the range of the sensor
		}
		else
		break;
	}

	if(i==600000)
	return US_NO_OBSTACLE;	//Indicates time out

	//Falling edge found

	result=TCNT1;          // microcontroller stores the the value of the counted pulse time in the TCNT1 register. So, we're returning this value to the
	// main function for utilizing it later

	//Stop Timer
	TCCR1B=0x00;

	if(result > 60000)
	return US_NO_OBSTACLE;	//No obstacle
	else
	return (result>>1);
}



//Using LCD with avr microcontrollers is a bit tricky. It is hard to explain in code comments. I highly recommend to watch the following video and read the article
//to have a clear understanding of how LCD works with microcontrollers for LCD i will put a detail tutorial but now
//Video: https://www.youtube.com/channel/UCV706NGK4bYrhMHQChhpE-g
//Article: https://mendupmindcode.blogspot.com/2021/11/csr04-ultrasonic-sensor-with-atmega328p.htmly



void set_portc_led(uint8_t a){
	if(a<100)
	PORTC = (1<<((int)(a/15)));
}
uint16_t pulse;
uint8_t print_dist[40] = {0};
	
int main()
{
	       

	HCSR04Init();
	uart_init(9600,0);
	
	sei();
	
	while(1)
	{
			
		//Send a trigger pulse
		HCSR04Trigger();               // calling the ultrasonic sound wave generator function

		//Measure the width of pulse
		pulse=GetPulseWidth();             // getting the duration of the ultrasound took to echo back after hitting the object

		//Handle Errors
		if(pulse==US_ERROR)                // if microcontroller doesn't get any pulse then it will set the US_ERROR variable to -1
		// the following code will check if there is error then it will be displayed on the LCD screen
		{

			_delay_ms(20);	
		}
		else
		{
				
			distance=(pulse*0.034/2.0);	// This will give the distance in centimeters
				
				
			set_portc_led(distance);
				
				
				
			memset(print_dist, 0, 40);    // distance is an integer number, we can not display integer directly on the LCD. this line converts integer into array of character
			sprintf((char*)print_dist,"Distance:%u \n \r",distance);
			uart_send_string(print_dist);
				
				
				
				
				
			_delay_ms(30);
		}
	}
		
}

*/