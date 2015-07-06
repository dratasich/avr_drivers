/**
 * @file rtc.c
 * @date 07.01.2014
 *
 * @brief Count ticks from clock crystal.
 *
 * Timer 2 is used as a counter. On pins TOSC1 and TOSC2 a 32.768kHz
 * crystal is connected. Timer 2 is configured to overflow every
 * second.
 */

#include "rtc.h"
#include "io.h"
#include <avr/interrupt.h>

static uint8_t hours = 0;
static uint8_t minutes = 0;
static uint8_t seconds = 0;

void rtc_init(void)
{
  // normal counter mode
  SET_BIT(ASSR, AS2);		// T2 clocked from crystal connected to TOSC1/2
  SET_BIT(TIMSK, TOIE2);	// interrupt at overflow
  sei();

  // start immediately
  TCCR2 |= (5 << CS20);		// prescaler = 128 overflow exactly every second (256 ticks)
}

void rtc_set(uint8_t h, uint8_t m, uint8_t s)
{
  hours = h;
  minutes = m;
  seconds = s;
}

void rtc_get(uint8_t* h, uint8_t* m, uint8_t* s)
{
  *h = hours;
  *m = minutes;
  *s = seconds;
}

// 1s elapsed
ISR(TIMER2_OVF_vect)
{
  if (seconds < 59)
    seconds++;
  else
    if (minutes < 59) {
      seconds = 0;
      minutes++;
    } else 
      if (hours < 23) {
	seconds = 0;
	minutes = 0;
	hours++;
      } else {
	seconds = 0;
	minutes = 0;
	hours = 0;
      }
}
