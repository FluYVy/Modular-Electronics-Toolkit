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

/**
 * @def MAX30101_I2C_ADDRESS
 * 
 * I2C address for the MAX30101 sensor.
 * The default value is 0x55, which can be modified to other addresses if needed.
*/
#define MAX30101_I2C_ADDRESS 0x55

/**
* @brief Starts measurements on the MAX30101 sensor.
*
* This function starts the measurement process for the MAX30101 sensor.
*/
void max30101_start_measurements();

/**
* @brief Perform a single measurement
*
* This function reads measurement data from the MAX30101 sensor and
* stores it in the provided array. The length parameter determines
* how many values will be read and stored.
*
* @param[out] values Array to store the measurement values.
*                    Should have at least the length specified.
* @param[in] length The number of values to read (1-3).
* 0 ... no values are returned
* 1 ... measure Heart rate
* 2 ... measure Heart Rate and SpO2
* 3 ... measure Heart Rate, SpO2 and Interbeat interval (IBI)
*
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
