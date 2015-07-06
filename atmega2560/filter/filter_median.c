/**
 * @file filter_median.c
 * @author Denise Ratasich
 * @date 28.09.2013
 *
 * @brief Implements the moving median filter.
 *
 * The window size is specified in the header file (WINDOW_SIZE),
 * but must not exceed 255, take an odd number. This filter uses 
 * only integer values, i.e. the inputs for the filter are 
 * represented as int16.
 */

#include "filter_median.h"
#include <stdlib.h>

/** Compare function for quick sort */
int compare_int(const void *a, const void *b);

static int16_t inputs[NUM_SENSORS][WINDOW_SIZE];	//< The inputs to filter.
static int16_t output[NUM_SENSORS];			//< The filtered value.

void filter_median_init(void) 
{
  uint8_t i, j;

  for (i = 0; i < NUM_SENSORS; i++)
  {
    // initialize input values
    for (j = 0; j < WINDOW_SIZE; j++)
      inputs[i][j] = 0;

    // initialize output values
    output[i] = 0;
  }
}

int16_t filter_median_estimate(enum sensors sensor, int16_t input) 
{
	uint8_t i;
	int16_t temp[WINDOW_SIZE];
	
	// shift array of inputs (discard oldest, append newest)
	for (i = WINDOW_SIZE-1; i > 0; i--) {
	  // shift old ones
	  inputs[sensor][i] = inputs[sensor][i-1];
	  // copy array for later use (sorting)
	  temp[i] = inputs[sensor][i];
	}
	inputs[sensor][0] = input;	// add new one
	temp[0] = inputs[sensor][0];	// copy array for later use (sorting)

	// search median (sort temp-array and take the middle)
	qsort(temp, WINDOW_SIZE, sizeof(int16_t), compare_int);
	output[sensor] = temp[WINDOW_SIZE/2];

	return output[sensor];
}

int compare_int(const void *a, const void *b) 
{
	int *x = (int *) a;
	int *y = (int *) b;
	return *x - *y;
}

int16_t filter_median_getLastEstimate(enum sensors sensor)
{
	return output[sensor];
}
