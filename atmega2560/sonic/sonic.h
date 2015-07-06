/*
 * sonic.h
 * ==============================================
 * Denise Ratasich
 * 10.11.2012
 */

#ifndef __SONIC_H__
#define __SONIC_H__	

#include <avr/io.h>	// e.g. uint16_t

void sonic_init(void);
void sonic_start(void);
uint16_t sonic_getDistance(void);
void sonic_showValue_cm(void);
// Timer5 Overflow ISR
// Timer5 Input Capture ISR
void sonic_filter(void);
uint16_t sonic_getFilteredDistance(void);

#endif
