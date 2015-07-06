/**
 * @file usi.h
 * @date 15.09.2013
 *
 * @brief Header for using SPI with tiny's USI interface.
 */

#ifndef USI_H
#define USI_H

#include <avr/io.h>

void spi_init(void);
void spi_transmit_b(uint8_t cdata);

#endif
