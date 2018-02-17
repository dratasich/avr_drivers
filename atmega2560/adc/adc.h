/**
 * @file adc.h
 * @author Thomas Schlaudoschitz
 * @date 2012-11-09
 *
 * @brief Header of ADC module.
 */

#ifndef __ADC_H__
#define __ADC_H__

#include <avr/io.h>	// e.g. uint8_t


/**
 * @brief Initializes the ADC without interrupts.
 * Prescaler set to 128. ADC interrupt disabled.
 */
void adc_init(void);

/**
 * @brief Performs an AD-conversion and returns the result given a channel.
 * @param channel The channel to read (0..15).
 *
 * @note This function is blocking!
 */
uint8_t adc_get(uint8_t); // blocking

#endif
