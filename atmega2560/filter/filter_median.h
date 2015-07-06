/**
 * @file filter_median.h
 * @author Denise Ratasich
 * @date 28.09.2013
 *
 * @brief Header for the moving median filter.
 *
 * The window size is specified in this header file (WINDOW_SIZE),
 * but must not exceed 255, take an odd number. This filter uses 
 * only integer values, i.e. the inputs for the filter are 
 * represented as int16.
 */

#ifndef __FILTER_MEDIAN_H__
#define __FILTER_MEDIAN_H__	

#include <avr/io.h>	// e.g. int16_t

// ---------------------------------------------------------------
// parameters
// ---------------------------------------------------------------
#define WINDOW_SIZE		5

// ---------------------------------------------------------------
// sensors to filter
// ---------------------------------------------------------------
enum sensors {
  IR_LEFT,
  IR_MID,
  IR_RIGHT,
  SONIC,
  NUM_SENSORS		// a dummy (for modulo-operation)
};

// ---------------------------------------------------------------
// function prototypes
// ---------------------------------------------------------------

/**
 * @brief Initializes the filter, i.e. sets the inputs and output 
 * to 0.
 */
void filter_median_init(void);

/**
 * @brief Returns the next estimated value, i.e. the median of the 
 * last WINDOW_SIZE inputs.
 * 
 * @param input A new input value.
 * @return The filtered value.
 */
int16_t filter_median_estimate(enum sensors sensor, int16_t input);

/**
 * @brief Returns the last estimate.
 *
 * @return The last filtered value.
 */
int16_t filter_median_getLastEstimate(enum sensors sensor);

#endif
