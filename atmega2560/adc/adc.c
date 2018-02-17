/**
 * @file adc.c
 * @author Thomas Schlaudoschitz
 * @date 2012-11-09
 *
 * @brief ADC module.
 *
 * AREF used (connect to desired reference voltage).
 * Possible channels: 0..15 on port F and K.
 */

#include "adc.h"

void adc_init(void)
{
    ADCSRA |= (1<<ADEN); // enable ADC, ADC interrupt is disabled
    ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2)); // prescaler 128
}

uint8_t adc_get(uint8_t channel)
{
    if(channel >= 16)
        return 0;

    // set ADLAR and set MUX (single ended)
    if(channel < 8) {
        ADMUX = 0x20 + channel;
        ADCSRB &= ~(1<<MUX5); // low ADC byte
    } else {
        ADMUX = 0x20 + channel - 8;
        ADCSRB |= (1<<MUX5); // high ADC byte
    }

    ADCSRA |= 1<<ADSC;             // start conversion
    while( (ADCSRA & (1<<ADSC)) ); // wait until conversion is ready
    //(bit ADCSRA is then cleared)

    return ADCH;
}
