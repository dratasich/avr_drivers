/**
 * @file sonic.h
 * @author Denise Ratasich
 * @date 2012-11-10
 *
 * @brief Header for sonar sensor SRF04 driver.
 */

#ifndef __SONIC_H__
#define __SONIC_H__

#include <avr/io.h>	// e.g. uint16_t

/**
 * @brief Initializes timer 5 to measure time-of-flight and port pins.
 */
void sonic_init(void);

/**
 * @brief Trigger single shot (send out sound wave).
 */
void sonic_trigger(void);

/**
 * @brief Returns !=0 when the distance is captured or a timeout occured.
 */
int8_t sonic_hasDistance(void);

/**
 * @brief Returns distance in cm (max 300).
 */
uint16_t sonic_getDistance(void);

// Timer5 Overflow ISR
// Timer5 Input Capture ISR

#endif
