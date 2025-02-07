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
 * @def SCD41_COMMAND_SET_TEMPERATURE_OFFSET
 * 
 * @brief The command to set the temperature offset.
 * 
 */
#define SCD41_COMMAND_SET_TEMPERATURE_OFFSET							0x241d

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_TEMPERATURE_OFFSET
 * 
 * @brief The command to get the temperature offset.
 * 
 */
#define SCD41_COMMAND_GET_TEMPERATURE_OFFSET							0x2318

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_SET_SENSOR_ALTITUDE
 * 
 * @brief The command to set the altitude of the sensor.
 * 
 */
#define SCD41_COMMAND_SET_SENSOR_ALTITUDE								0x2427

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_SENSOR_ALTITUDE
 * 
 * @brief The command to get the altitude of the sensor.
 * 
 */
#define SCD41_COMMAND_GET_SENSOR_ALTITUDE								0x2322

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_SET_AMBIENT_PRESSURE
 * 
 * @brief The command to set the ambient pressure.
 * 
 */
#define SCD41_COMMAND_SET_AMBIENT_PRESSURE								0xe000

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_AMBIENT_PRESSURE
 * 
 * @brief The command to get the ambient pressure.
 * 
 */
#define SCD41_COMMAND_GET_AMBIENT_PRESSURE								0xe000


/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_PERFORM_FORCED_RECALIBRATION
 * 
 * @brief The command to perform a forced calibration.
 * 
 */
#define SCD41_COMMAND_PERFORM_FORCED_RECALIBRATION						0x362f

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED
 * 
 * @brief The command to enable or disable automatic self-calibration.
 * 
 */
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED			0x2416

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED
 * 
 * @brief The command to get the current status of automatic self-calibration.
 * 
 */
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED			0x2313

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET
 * 
 * @brief The command to set the target for automatic self-calibration.
 * 
 */
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET				0x243a

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET
 * 
 * @brief The command to get the current target for automatic self-calibration.
 * 
 */
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET				0x233f

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT
 * 
 * @brief The command to start a low-power periodic measurement.
 * 
 */
#define SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT				0x21ac

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_DATA_READY_STATUS
 * 
 * @brief The command to poll for data ready status.
 * 
 */
#define SCD41_COMMAND_GET_DATA_READY_STATUS								0xe4b8


/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_PERSIST_SETTINGS
 * 
 * @brief The command to save the current settings.
 * 
 */
#define SCD41_COMMAND_PERSIST_SETTINGS									0x3615

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_SERIAL_NUMBER
 * 
 * @brief The command to get the serial number of the sensor.
 * 
 */
#define SCD41_COMMAND_GET_SERIAL_NUMBER									0x3682

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_PERFORM_SELF_TEST
 * 
 * @brief The command to perform a self-test.
 * 
 */
#define SCD41_COMMAND_PERFORM_SELF_TEST									0x3639

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_PERFORM_FACTORY_RESET
 * 
 * @brief The command to reset all parameters and perform a factory reset.
 * 
 */
#define SCD41_COMMAND_PERFORM_FACTORY_RESET								0x3632

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_REINIT
 * 
 * @brief The command to reinitialize the sensor.
 * 
 */
#define SCD41_COMMAND_REINIT											0x3646

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_SENSOR_VARIANT
 * 
 * @brief The command to get the sensor variant.
 * 
 */
#define SCD41_COMMAND_GET_SENSOR_VARIANT								0x202f


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
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY
 * 
 * @brief The command to perform a single-shot measurement where only the relativ humidity and temperature is measured.
 * 
 */
#define SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY						0x2196

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_POWER_DOWN
 * 
 * @brief The command to shut down the sensor.
 * 
 */
#define SCD41_COMMAND_POWER_DOWN										0x36e0

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_WAKE_UP
 * 
 * @brief The command to wake up the sensor.
 * 
 */
#define SCD41_COMMAND_WAKE_UP											0x36f6

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD
 * 
 * @brief The command to set the initial period for automatic self-calibration.
 * 
 */
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD		0x2445

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD
 * 
 * @brief The command to get the initial period for automatic self-calibration.
 * 
 */
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD		0x2340

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD
 * 
 * @brief The command to set the standard period for automatic self-calibration.
 * 
 */
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD	0x244e

/**
 * @ingroup CommandAddresses
 * 
 * @def SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD
 * 
 * @brief The command to get the standard period for automatic self-calibration.
 * 
 */
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD	0x234b




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
 * @defgroup SignalCompensation Signal Compensation
 * A set of functions for controlling the on-chip output signal compensation.
*/

/**
* @brief Set temperature offset
* Sets the temperature offset to a given value.
* @param offset Temperature offset value
* @ingroup SignalCompensation
*/
void scd41_set_temperature_offset(uint16_t offset);

/**
* @brief Get temperature offset
* Gets the temperature offset.
* @return uint16_t Current temperature offset
* @ingroup SignalCompensation
*/
uint16_t scd41_get_temperature_offset();

/**
* @brief Set sensor altitude
* Sets the sensor altitude to a given value in meters.
* @param altitude Altitude in meters
* @ingroup SignalCompensation
*/
void scd41_set_sensor_altitude(uint16_t altitude);

/**
* @brief Get sensor altitude
* Gets the sensor altitude in meters.
* @return Current altitude in meters
* @ingroup SignalCompensation
*/
uint16_t scd41_get_sensor_altitude();

/**
* @brief Set ambient pressure
* Sets the ambient pressure to a given value in hPa.
* @param pressure Ambient pressure value in hPa
* @ingroup SignalCompensation
*/
void scd41_set_ambient_pressure(uint16_t pressure);

/**
* @brief Get ambient pressure
* Gets the the ambient pressure setting in hPa.
* @return Current ambient pressure in hPa
* @ingroup SignalCompensation
*/
uint16_t scd41_get_ambient_pressure();


/**
 * @defgroup FieldCalibration Field Calibration
 * A set of functions for controlling the on-chip field calibration.
*/

/**
* @brief Perform forced recalibration with reference CO2 value
* Performs a forced recalibration of the sensor using the specified reference CO2 concentration.
* @param reference Reference CO2 concentration in ppm
* @param return_correction If true, returns the used correction value
* @return Correction value or 0xFFFF if checksum fails
* @ingroup FieldCalibration
*/
int16_t scd41_perform_forced_recalibration(uint16_t reference, uint8_t return_correction);

/**
* @brief Enable or disable automatic self-calibration (ASC)
* @param enabled 1 to enable ASC, 0 to disable
* @ingroup FieldCalibrationv
*/
void scd41_set_automatic_self_calibration_enabled(uint8_t enabled);

/**
* @brief Get automatic self-calibration state
* @return 0 if disabled, 1 if enabled, 2 if checksum fails
* @ingroup FieldCalibration
*/
uint8_t scd41_get_automatic_self_calibration_enabled();

/**
* @brief Set target CO2 concentration for automatic self-calibration
* @param target_CO2 Target CO2 concentration in ppm
* @ingroup FieldCalibration
*/
void scd41_set_automatic_self_calibration_target(uint16_t target_CO2);

/**
* @brief Get target CO2 concentration for automatic self-calibration
* @return Target CO2 in ppm, 0xFFFF if checksum fails
* @ingroup FieldCalibration
*/
uint16_t scd41_get_automatic_self_calibration_target();


/**
 * @defgroup LowPower Low Power
 * A set of functions for controlling the low-power modes of the SCD41.
*/

/**
* @brief Start low power periodic measurement mode
* @note To stop the measurements use the function `stop_periodic_measurement()`.
* @ingroup LowPower
*/
void scd41_start_low_power_periodic_measurement();

/**
* @brief Check if new measurement data is available
* @return 0 if data is not ready, 1 if ready, 2 if checksum fails
* @ingroup LowPower
*/
uint8_t scd41_get_data_ready_status();


/**
 * @defgroup AdvancedFeatures Advanced Features
 * A set of functions for controlling advanced features of the SCD41.
*/

/**
* @brief Save current configuration to EEPROM
* @param wait If 1, waits 500ms for EEPROM write, else you need to handle it yourself.
* @ingroup AdvancedFeatures
*/
void scd41_persist_settings(uint8_t wait);

/**
* @brief Get sensor serial number
* @return 48-bit serial number as 64-bit integer, 0 if checksum fails
* @ingroup AdvancedFeatures
*/
uint64_t scd41_get_serial_number();

/**
* @brief Perform sensor self-test
* @return 0 if pass, non-zero if fail, 0xFF if checksum fails
* @ingroup AdvancedFeatures
*/
uint8_t perform_self_test();

/**
* @brief Reset sensor to factory defaults
* @param wait If 1, waits for reset completion
* @ingroup AdvancedFeatures
*/
void scd41_perform_factory_reset(uint8_t wait);

/**
* @brief Reinitialize sensor
* @param wait If 1, waits for reinit completion
* @ingroup AdvancedFeatures
*/
void scd41_reinit(uint8_t wait);

/**
* @brief Get sensor variant
* @return Sensor variant in bits [15:12]:
*         0000: SCD40
*         0001: SCD41
* @ingroup AdvancedFeatures
*/
uint8_t scd41_get_sensor_variant();


/**
 * @defgroup SingleShot Single-Shot measurement control
 * A set of functions for controlling single-shot measurement modes.
*/

/**
* @brief Perform a single measurement
* @param wait If 1, waits 5000ms for measurement
* @ingroup SingleShot
*/
void scd41_measure_single_shot(uint8_t wait);

/**
* @brief Measure temperature and humidity only (no CO2)
* @param wait If 1, waits 50ms for measurement
* @ingroup SingleShot
*/
void scd41_measure_single_shot_rht_only(uint8_t wait);

/**
* @brief Enter power-down mode
* Enter power-down mode to minimize current consumption
* @ingroup SingleShot
*/
void scd41_power_down();

/**
* @brief Wake up sensor
* Wake up sensor from power-down mode
* @param wait If 1, waits 30ms for wake-up
* @ingroup SingleShot
*/
void scd41_wake_up(uint8_t wait);

/**
* @brief Set initial period for automatic self-calibration
* @param hours Initial calibration period in hours
* @ingroup SingleShot
*/
void scd41_set_automatic_self_calibration_initial_period(uint16_t hours);

/**
* @brief Get initial period for automatic self-calibration
* @return Initial period in hours
* @ingroup SingleShot
*/
uint16_t scd41_get_automatic_self_calibration_initial_period();

/**
* @brief Set standard period for automatic self-calibration
* @param hours Standard calibration period in hours
* @ingroup SingleShot
*/
void scd41_set_automatic_self_calibration_standard_period(uint16_t hours);

/**
* @brief Get standard period for automatic self-calibration
* @return Standard period in hours
* @ingroup SingleShot
*/
uint16_t scd41_get_automatic_self_calibration_standard_period();

#endif /* SCD41_H_ */