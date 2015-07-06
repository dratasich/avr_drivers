/*
 * soundmodule.c
 * ================================================
 * Denise Ratasich
 * 10.11.2012
 *
 * implementation of the serial protocol of the soundmodule SOMO-14D
 *
 * Timer 3 for clock generation and timings:
 * - 16MHz -> 62,5ns/tick, no prescaler
 * - startbit: 2ms = 32000 ticks (16x timer-period)
 * - clk-period: 250us (5kHz) = 4000 ticks
 * - period when sending: 125us = 2000 ticks
 */

#include <avr/interrupt.h>
#include "soundmodule.h"

// with prescaler /256 about 300ms:
#define SM_REL_RESET		(65536 - 19000)

// with no prescaler:
#define SM_REL_IDLE		0
#define SM_REL_STARTBIT		(65536 - 32000)		// 2ms
#define SM_REL_CLOCK		(65536 - 2000)		// 125us
#define SM_REL_TIMEOUT		(65536 - 32000)		// 2ms

#define SM_RESET	0
#define SM_IDLE		1
#define SM_STARTBIT	2
#define SM_CLOCK	3
#define SM_STOP		4
#define SM_TIMEOUT	5

static volatile int8_t state = SM_RESET;
static volatile uint16_t data;

void soundmodule_init() {
  // init pins
  SM_DDR |= (1<<SM_CLK) | (1<<SM_DATA);
  SM_DDR &= ~(1<<SM_BUSY);
  SM_PORT |= (1<<SM_CLK) | (1<<SM_DATA);

  // init timer for protocol (clk, timing, ..)
  TCNT3 = SM_REL_RESET;		// set start value
  TIMSK3 |= (1<<TOIE3);		// enable overflow interrupt
  TCCR3B |= (1<<CS32);		// start timer (prescaler /256)

  sei();
}

void soundmodule_playSound(uint8_t soundNr)
{
	if(soundNr > SOUND_NR_MAX)
		return;
	
	soundmodule_sendCmd(soundNr);
	
	while( (SM_PIN & (1<<SM_BUSY)) ); //wait until busy goes to "0"
}

uint8_t soundmodule_isReady(void) {
  if (state == SM_IDLE)
    return 1;
  
  return 0;
}

void soundmodule_sendCmd(uint16_t cmd) {
  if (!soundmodule_isReady())
    return;

  data = cmd;
  state = SM_STARTBIT;

  SM_PORT &= ~(1<<SM_CLK);	// set startbit ('0') on clk pin
  if (data & 0x8000)		// put MSB of command on data pin
    SM_PORT |= (1<<SM_DATA);
  else
    SM_PORT &= ~(1<<SM_DATA);

  // set timer reload value for startbit (2ms)
  TCNT3 = SM_REL_STARTBIT;
}

ISR(TIMER3_OVF_vect) {
  static int16_t bitCounter = 15;
  static uint8_t clkHigh = 0;

  switch(state) {
  case SM_RESET:
    TCCR3B = 0;			// timer off
    TCNT3 = SM_REL_IDLE;
    TCCR3B |= (1<<CS30);	// timer on with no prescaler
    state = SM_IDLE;		// wait on send request
    break;
  case SM_IDLE:
    break;
  case SM_STARTBIT:		// startbit written
    TCNT3 = SM_REL_CLOCK;
    bitCounter = 15;
    clkHigh = 0;		// indicate that clock is low
    state = SM_CLOCK;		// put data bits out next
    break;
  case SM_CLOCK:
    TCNT3 = SM_REL_CLOCK;
    SM_PORT ^= (1<<SM_CLK);	// change clock polarity

    // change data bit at falling edge of clk
    if (clkHigh) {
      clkHigh = 0;
      bitCounter--;

      if (bitCounter >= 0) {	// set next data bit
	if (data & (1<<bitCounter))		
	  SM_PORT |= (1<<SM_DATA);
	else
	  SM_PORT &= ~(1<<SM_DATA);
      }

      if (bitCounter == 0)	// last data bit written
	state = SM_STOP;
    } else
      clkHigh = 1;
    break;
  case SM_STOP:
    SM_PORT |= (1<<SM_CLK);
    TCNT3 = SM_REL_TIMEOUT;
    state = SM_TIMEOUT;
    break;
  case SM_TIMEOUT:
    state = SM_IDLE;
    break;
  }
}
