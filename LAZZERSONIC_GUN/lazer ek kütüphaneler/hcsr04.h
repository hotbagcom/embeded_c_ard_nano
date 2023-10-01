/*
 * hcsr04.h
 *
 * Created: 25.09.2023 19:22:46
 *  Author: tnc
 */ 


#ifndef HCSR04_H_
#define HCSR04_H_
#define  F_CPU 16000000UL

// extern uint8_t print_dist[40] ;
// extern uint32_t buffer_value[20];
// extern  volatile uint32_t sample;
// extern  volatile uint32_t delta_time ;
// extern  volatile uint16_t travel_time ;
// extern  volatile uint16_t start_time ;
// extern  volatile uint16_t finish_time;
// extern  volatile short allow_trig_send;
// extern  volatile short allow_echo_read;

	extern void Ultros_Init();
// 	extern void Ultros_Trig();
	extern void Init_time();
// 	extern void Clear_time();
// 	extern uint16_t filter();
// 	extern void interr_1_Init();
	extern void Ultros_main()

#endif /* HCSR04_H_ */