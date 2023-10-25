/*
 * pwm_sg.h son sürüm c:\users\tnc\Documents\Atmel Studio\7.0\pwm_timer0_ovf\pwm_timer0_ovf\pwm_sg_v1.c
 *
 * Created: 30.09.2023 08:40:16
 *  Author: tnc
 */ 


#ifndef PWM_SG_H_
#define PWM_SG_H_

#include <avr/io.h>

extern void pwm_pin_enable( unsigned char port , short pin);
extern void Init_pwm();
extern void Set_pwm_value(short pin,uint16_t new_value);

#endif /* PWM_SG_H_ */