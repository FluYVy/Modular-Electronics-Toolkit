/*
* SCD41.h
*
* Created: 30.09.2024 15:45:02
*  Author: Florian Reichart
*/


#ifndef SCD41_H_
#define SCD41_H_


#include "i2c_master.h"
#include <util/delay.h>


/**
* @def SCD41_PROCESS_CHECKSUM
* @brief Enable checksum processing after measuring the data.
*
* @note: This will only affect incoming data as a checksum is always needed when data is send.
*
* To enable, uncomment the following line:
* @code
* #define SCD41_PROCESS_CHECKSUM
* @endcode
*/
//#define SCD41_PROCESS_CHECKSUM


/**
 * @def SCD41_ADDRESS
 * @brief I2C address of the SCD41 sensor.
 *
 * @note: The default address is 0x62 (SCD41).
 * 
 */
#define SCD41_ADDRESS 0x62


/**
 * @defgroup CommandAddresses Different Addresses for command execution.
 * A set of addresses to be used for executing different commands on the SCD41 sensor.
 */

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_START_PRERIODIC_MEASUREMENT
 * 
 * @brief The command to start a periodic measurement.
 * 
 */
#define SCD41_COMMAND_START_PRERIODIC_MEASUREMENT						0x21b1

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_READ_MEASUREMENT
 * 
 * @brief The command to read the measurement data. 
 * 
 */
#define SCD41_COMMAND_READ_MEASUREMENT									0xec05

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT
 * 
 * @brief The command to stop a periodic measurement.
 * 
 */
#define SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT						0x3f86

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_MEASURE_SINGLE_SHOT
 * 
 * @brief The command to perform a single-shot measurement.
 * 
 */
#define SCD41_COMMAND_MEASURE_SINGLE_SHOT								0x219d



/**
 * @defgroup BasicCommands Basic Commands
 * A set of functions for performing basic sensor operations.
 */

/**
* @brief Get current sensor state
* @return 0 if stopped, 1 if running
* @warning This is not fully implemented yet.
* @ingroup BasicCommands
*/
uint8_t scd41_get_current_state();

/**
 * @brief Get last measured CO2-value
 * Gets the last measured CO2-value in ppm.
 * 
 * @return uint16_t CO2 value in ppm
 * @note This does not get the current measurement from the sensor. Use `read_measurement()` to get the most recent data.
 * @ingroup BasicCommands
*/
uint16_t scd41_get_co2();

/**
 * @brief Get last measured temperature
 * Gets the last measured temperature in degrees C.
 * 
 * @return float temperature value in degrees C
 * @note This does not get the current measurement from the sensor. Use `read_measurement()` to get the most recent data.
 * @ingroup BasicCommands
*/
float scd41_get_temperature();

/**
 * @brief Get last raw measured temperature
 * Gets the last measured temperature as an uint16 integer.
 * 
 * @return uint16_t temperature value
 * @note This does not get the current measurement from the sensor. Use `read_measurement()` to get the most recent data.
 * @ingroup BasicCommands
*/
uint16_t scd41_get_temperature_raw();

/**
 * @brief Get last measured humidity
 * Gets the last measured humidity in percent.
 * 
 * @return float relativ humidity value in percent.
 * @note This does not get the current measurement from the sensor. Use `read_measurement()` to get the most recent data.
 * @ingroup BasicCommands
*/
float scd41_get_humidity();

/**
 * @brief Get last raw measured humidity
 * Gets the last measured humidity as an uint16 integer.
 * 
 * @return uint16_t humidity value
 * @note This does not get the current measurement from the sensor. Use `read_measurement()` to get the most recent data.
 * @ingroup BasicCommands
*/
uint16_t scd41_get_humidity_raw();

/**
 * @brief Start periodic measurement mode
 * 
 * Start periodic measurement mode.
 * @note To stop the measurements use the function `stop_periodic_measurement()`.
 * 
 * @ingroup BasicCommands
*/
void scd41_start_periodic_measurement();

/**
* @brief Read latest measurement from sensor
* Gets the last measured CO2, temperature and humidity from the sensor.
* @note This function will not return the data.
* @ingroup BasicCommands
*/
void scd41_read_measurement();

/**
* @brief Stop periodic measurement mode
*
* Stop periodic measurement mode.
* @note If `wait` is true, waits 500ms for command completion. Otherwise, you will need to handle this yourself.
* @param wait If true, waits 500ms for command completion.
* @ingroup BasicCommands
*/
void scd41_stop_periodic_measurement(uint8_t wait);

/**
* @brief Perform a single measurement
* @param wait If 1, waits 5000ms for measurement
* @ingroup SingleShot
*/
void scd41_measure_single_shot(uint8_t wait);

#endif /* SCD41_H_ */