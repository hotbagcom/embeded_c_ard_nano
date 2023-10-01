/*
 * pwm_sg.h
 *
 * Created: 30.09.2023 08:40:16
 *  Author: tnc
 */ 


#ifndef PWM_SG_H_
#define PWM_SG_H_

extern void pwm_pin_enable( unsigned char port , short pin);
extern void Init_pwm();
extern void Set_pwm_value(short pin,uint16_t new_value);

#endif /* PWM_SG_H_ */