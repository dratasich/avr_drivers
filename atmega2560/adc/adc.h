/**
 * @file adc.c
 * @author Thomas Schlaudoschitz
 * @date 09.11.2012
 *
 * @brief Header of ADC module, without interrupt, i.e. AD-conversion
 * is blocking.
 */

#ifndef __ADC_H__
#define __ADC_H__

#include <avr/io.h>	// e.g. uint8_t

void adc_init(void);
uint8_t adc_get(uint8_t);

#endif
