/**
 * @file sonic.c
 * @author Denise Ratasich
 * @date 2012-11-10
 *
 * @brief Reads the distance to an object of an SRF04 (sonic sensor).
 *
 * Timer 5 for triggering and capturing the output signal of the SRF04:
 * - normal mode, prescaling /8 -> 0,5us
 * - trigger: 15us = 30ticks
 * - start capturing at rising edge, stop counting at falling edge
 * - make a small timeout between two measurements
 */

#include <avr/interrupt.h>
#include "sonic.h"
#include "io.h"  // port/pin definitions

// timer reload values (prescaler /8)
#define SONIC_REL_TRIG		(65536 - 24)	// 12us
#define SONIC_REL_TIMEOUT	(65536 - 20000)	// min 10ms

// states
#define SONIC_IDLE	0
#define SONIC_TRIGGER	1
#define SONIC_WAIT_ECHO	2
#define SONIC_ECHO	3
#define SONIC_TIMEOUT	4
#define SONIC_RESULT    5

static volatile int8_t state = SONIC_IDLE;
static volatile uint8_t ovf = 0;
static volatile uint32_t distance = 0;	// 32bits for calculation

void sonic_init(void)
{
    // init pins
    SONIC_DDR |= (1<<SONIC_TRIG);
    SONIC_DDR &= ~(1<<SONIC_CAPT);
    SONIC_PORT &= ~(1<<SONIC_TRIG);

    // basic config timer 5 (rest in sonic start)
    TIMSK5 |= (1<<TOIE5) | (1<<ICIE5);	// enable overflow interrupt

    sei();
}

void sonic_trigger(void)
{
    //if (state != SONIC_IDLE)
    //  return;

    TCCR5B = 0;  // stop timer

    state = SONIC_TRIGGER;
    TCCR5B |= (1<<ICES5);  // capture on rising edge
    TIFR5 &= ~(1<<ICF5);  // clear input capture flag
    TCNT5 = SONIC_REL_TRIG;  // set start value
    SONIC_PORT |= (1<<SONIC_TRIG);  // start triggering sonic sensor

    TCCR5B |= (1<<CS51);  // start timer (prescaler /8)
}

int8_t sonic_hasDistance(void)
{
    if (state == SONIC_RESULT)
        return 1;

    return 0;
}

uint16_t sonic_getDistance(void)
{
    state = SONIC_IDLE;
    if (distance > 300)
        return 300;

    return (uint16_t) distance;
}

ISR(TIMER5_OVF_vect)
{
    ovf++;

    switch(state) {
    case SONIC_TRIGGER:
        SONIC_PORT &= ~(1<<SONIC_TRIG);  // release trigger
        state = SONIC_WAIT_ECHO;  // await echo start
        ovf = 0;
        break;
    case SONIC_WAIT_ECHO:
        // waited 65ms and no echo signal received
        if (ovf >= 2) {
            TCNT5 = SONIC_REL_TIMEOUT;
            state = SONIC_TIMEOUT;
        }
        break;
    case SONIC_ECHO:  // await echo end
        if (ovf >= 2  &&  // echo needs too long?!
            !(SONIC_PORT & (1<<SONIC_TRIG))) {
            TCNT5 = SONIC_REL_TIMEOUT;
            state = SONIC_TIMEOUT;
        }
        break;
    case SONIC_TIMEOUT:
        distance = 300;
        state = SONIC_RESULT;
        break;
    }
}

ISR(TIMER5_CAPT_vect)
{
    static uint16_t stopValue;

    if (state == SONIC_WAIT_ECHO) {     // rising edge detected
        TCNT5 = 0;                      // reset for counting
        ovf = 0;
        TCCR5B &= ~(1<<ICES5);          // capture on falling edge
        TIFR5 &= ~(1<<ICF5);            // clear input capture flag
        state = SONIC_ECHO;
    } else if (state == SONIC_ECHO) {
        stopValue = ICR5;               // save counter value

        // calculate length of echo
        distance = 172 * (((uint32_t)ovf)*32768 + (stopValue/2)) / 10000;

        state = SONIC_RESULT;
    }
}
