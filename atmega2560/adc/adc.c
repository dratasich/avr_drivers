/**
 * @file adc.c
 * @author Thomas Schlaudoschitz
 * @date 09.11.2012
 *
 * @brief ADC module, without interrupt, i.e. AD-conversion is
 * blocking.
 *
 * possible channels: 0..15 on port F and K
 */

#include "adc.h"

void adc_init(void)
{
	ADCSRA |= (1<<ADEN);	// enable ADC, ADC interrupt is disabled
	ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2)); // prescaler 128
}

/**
 * @brief Returns the adc conversion result of the specified channel.
 * @param channel The number of the channel to read (0..15).
 *
 * @note Attention, this function is blocking!
 */
uint8_t adc_get(uint8_t channel) 
{
	if(channel >= 16) 
		return 0;
		
	if(channel < 8)
	{
		ADMUX = 0x20 + channel;		//set ADLAR and set MUX4:0 (single ended)
		ADCSRB &= ~(1<<MUX5);		// low adc byte
	} else {
		ADMUX = 0x20 + channel - 8;	//set ADLAR and set MUX4:0 (single ended)
		ADCSRB |= (1<<MUX5);		// high adc byte
	}
	
	ADCSRA |= 1<<ADSC;				// start conversion
	while( (ADCSRA & (1<<ADSC)) );  //wait until conversion is ready
									//(bit ADCSRA is then cleared)
	return ADCH;
}
