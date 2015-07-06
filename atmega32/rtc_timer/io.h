/**
 * @file io.h 
 * @date 05.01.2013
 *
 * @brief Collects the port and pin numbers of the peripherals.
 *
 **/

#ifndef IO_H
#define IO_H

#include <avr/io.h>

//
// watch crystal
//
// RTC: crystal at PC6, PC7
// Timer 2 overflow interrupt used for 1 second

// ----------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------
#define SET_BIT(PORT, BITNUM)    ((PORT) |=  (1<<(BITNUM)))
#define CLEAR_BIT(PORT, BITNUM)  ((PORT) &= ~(1<<(BITNUM)))
#define TOGGLE_BIT(PORT, BITNUM) ((PORT) ^=  (1<<(BITNUM)))

#define IS_BIT_SET(PORT, BITNUM) ((PORT) & (1<<(BITNUM)))

#endif
