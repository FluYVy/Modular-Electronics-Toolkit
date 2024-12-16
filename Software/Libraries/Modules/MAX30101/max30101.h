/*
* max30101.h
*
* Created: 27.10.2024 09:38:15
* Author: Florian Reichart
*/

#ifndef MAX30101_H_
#define MAX30101_H_

#include "i2c_master.h"

// I2C address for the MAX30101 sensor
#define MAX30101_I2C_ADDRESS 0x55

/**
* @brief Starts measurements on the MAX30101 sensor.
*
* This function configures the sensor to begin taking measurements.
* It sets the output mode, enables the Automatic Gain Control (AGC),
* the Analog Front End (AFE), and activates the algorithm for processing
* the data.
*/
void max30101_start_measurements();

/**
* @brief Performs a single measurement with the MAX30101 sensor.
*
* This function reads measurement data from the MAX30101 sensor and
* stores it in the provided array. The length parameter determines
* how many values will be read and stored.
*
* @param[out] values Array to store the measurement values.
*                    Should have at least the length specified.
* @param[in] length The number of values to read (1-3).
*/
void max30101_single_measurement(uint16_t values[], uint8_t length);

/**
* @brief Stops measurements on the MAX30101 sensor.
*
* This function disables the AFE and the algorithm processing on the
* MAX30101 sensor to stop the measurements.
*/
void max30101_stop_measurements();

#endif /* MAX30101_H_ */
