/**
 * @file filter_average.h
 * @author Denise Ratasich
 * @date 28.09.2013
 *
 * @brief Header for the moving average filter.
 *
 * The window size is specified in this header file (WINDOW_SIZE),
 * but must not exceed 255. This filter uses only integer values, 
 * i.e. the inputs for the filter are represented as int16.
 *
 * Coefficients can be specified during initialization, by default 
 * the coefficients for the inputs evaluate to 1.
 */

#ifndef __FILTER_AVERAGE_H__
#define __FILTER_AVERAGE_H__

#include <avr/io.h>	// e.g. int8_t, int16_t

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
void filter_average_init(void);

/**
 * @brief Initializes the filter with coefficients for the inputs.
 *
 * @param coeffB The coefficients for the inputs, e.g. for applying
 * a hamming window.
 */
void filter_average_init_coeff(const int8_t coeffB[WINDOW_SIZE]);

/**
 * @brief Returns the next estimated value, i.e. the average of the 
 * last WINDOW_SIZE inputs.
 * 
 * @param input A new input value.
 * @return The filtered value.
 */
int16_t filter_average_estimate(enum sensors sensor, int16_t input);

/**
 * @brief Returns the last estimate.
 *
 * @return The last filtered value.
 */
int16_t filter_average_getLastEstimate(enum sensors sensor);

#endif
