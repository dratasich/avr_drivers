/*
 * uart1.h
 * ==============================================
 * Denise Ratasich
 * 15.11.2012 (copied from uart0.h)
 */

#ifndef __UART1_H__
#define __UART1_H__

#include <avr/io.h>	// e.g. uint8_t, uint16_t	

void uart1_init();
void uart1_putc(char);
void uart1_print(char*);
void uart1_println(char*);
void uart1_printInt8(int8_t);
void uart1_printUInt8(uint8_t);
void uart1_printUInt16(uint16_t);
void uart1_printInt16(int16_t);
uint8_t uart1_getc(char*);
int8_t uart1_getUInt16(uint16_t*);
float uart1_getFloat2d3(void);
// receive-ISR UART1

#endif
