/**
 * @file rtc.h
 * @date 07.01.2014
 *
 * @brief Header for using the real time counter.
 */

#ifndef __RTC_H__
#define __RTC_H__

#include <avr/io.h>	// e.g. uint8_t

void rtc_init(void);
void rtc_set(uint8_t h, uint8_t m, uint8_t s);
void rtc_get(uint8_t* h, uint8_t* m, uint8_t* s);
// Timer 2 interrupt

#endif
