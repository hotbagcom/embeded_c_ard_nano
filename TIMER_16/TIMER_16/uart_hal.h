/*
 * uart_hal.h
 *
 * Created: 31.08.2023 10:01:51
 *  Author: tnc
 */ 

#ifndef UART_HAL_H_
#define UART_HAL_H_


#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#define RX_BUFFER_SIZE 128

void uart_init(uint32_t baud,uint8_t high_speed);
void uart_send_byte(uint8_t c);
void uart_send_array(uint8_t *c,uint16_t len);
void uart_send_string(uint8_t *c);
uint16_t uart_read_count(void);
uint8_t uart_read(void);

#endif /* UART_HAL_H_ */