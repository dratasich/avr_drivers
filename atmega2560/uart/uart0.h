/*
 * uart0.h
 * ==============================================
 * Denise Ratasich
 * 09.11.2012
 */

#ifndef __UART0_H__
#define __UART0_H__

#include <avr/io.h>	// e.g. uint8_t, uint16_t

void uart0_init();
void uart0_putc(char);
void uart0_print(char*);
void uart0_println(char*);
void uart0_printUInt8(uint8_t);
void uart0_printUInt16(uint16_t);
uint8_t uart0_getc(char*);
// receive-ISR UART0

#endif
