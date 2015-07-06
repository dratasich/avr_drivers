/**
 * @file filter_average.c
 * @author Denise Ratasich
 * @date 28.09.2013
 *
 * @brief Implements the moving average filter.
 *
 * The window size is specified in this header file (WINDOW_SIZE),
 * but must not exceed 255. This filter uses only integer values, 
 * i.e. the inputs for the filter are represented as int16.
 *
 * Coefficients can be specified during initialization, by default 
 * the coefficients for the inputs evaluate to 1.
 */

#include "filter_average.h"

static int8_t b[WINDOW_SIZE];		//< Transfer coefficients for inputs.
static int16_t bSum;			//< Sum of all coefficients for inputs.
static int16_t inputs[NUM_SENSORS][WINDOW_SIZE];	//< The inputs to filter.
static int16_t output[NUM_SENSORS];			//< The filtered value.

void filter_average_init(void)
{
        uint8_t i, j;

	// initialize coefficients for inputs
	bSum = WINDOW_SIZE;
	for (i = 0; i < WINDOW_SIZE; i++)
		b[i] = 1;

	for (i = 0; i < NUM_SENSORS; i++)
	{
	  // initialize input values
	  for (j = 0; j < WINDOW_SIZE; j++)
	    inputs[i][j] = 0;

	  // initialize output values
	  output[i] = 0;
	}
}

void filter_average_init_coeff(const int8_t coeffB[WINDOW_SIZE]) 
{
	uint8_t i;

	// default initialization
	filter_average_init();

	// initialize coefficients for inputs
	bSum = 0;
	for (i = 0; i < WINDOW_SIZE; i++) {
		b[i] = coeffB[i];
		bSum += b[i];
	}
}

int16_t filter_average_estimate(enum sensors sensor, int16_t input) 
{
	uint8_t i;
	int32_t sum = 0;
	
	// shift array of inputs (discard oldest, append newest)
	for (i = WINDOW_SIZE-1; i > 0; i--) {
	  // shift old ones
	  inputs[sensor][i] = inputs[sensor][i-1];	
	  // sum up for averaging
	  sum += b[i]*inputs[sensor][i];		
	}
	inputs[sensor][0] = input;	// add new one
	sum += b[0]*input;		// sum up for averaging

	// calculate average
	output[sensor] = sum / bSum;

	return output[sensor];
}

int16_t filter_average_getLastEstimate(enum sensors sensor) 
{
	return output[sensor];
}
