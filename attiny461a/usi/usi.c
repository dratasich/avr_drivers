/**
 * @file usi.c
 * @date 15.09.2013
 *
 * @brief Implementing SPI on USI.  
 */

#include "usi.h"

#define IO_USI_DDR		DDRA
#define IO_USI_PORT		PORTA
#define IO_USI_DI		PA0
#define IO_USI_DO		PA1
#define IO_USI_USCK		PA2

#define SET_BIT(PORT, BITNUM)    ((PORT) |=  (1<<(BITNUM)))
#define CLEAR_BIT(PORT, BITNUM)  ((PORT) &= ~(1<<(BITNUM)))
#define TOGGLE_BIT(PORT, BITNUM) ((PORT) ^=  (1<<(BITNUM)))

void spi_init(void)
{
  SET_BIT(IO_USI_DDR,IO_USI_DO);
  CLEAR_BIT(IO_USI_DDR,IO_USI_DI);
  SET_BIT(IO_USI_DDR,IO_USI_USCK);

  USIPP |= (1<<USIPOS);		// USI output on PORTA (not PORTB)
}

void spi_transmit_b(uint8_t cdata)
{
  uint8_t settingsAndClock = (1<<USIWM0) | (1<<USICS1) | (1<<USICLK) | (1<<USITC);

  USIDR = cdata;		// fill data register
  USISR |= (1<<USIOIF);		// clear interrupt flag

  // transfer
  while ((USISR & (1<<USIOIF)) == 0)
    USICR = settingsAndClock;	// toggle USCK
}
