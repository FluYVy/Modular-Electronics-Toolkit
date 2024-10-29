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
Whether the checksum should be checked, when data is received
*/

//#define SCD41_PROCESS_CHECKSUM


/**
The i2c-address of the SCD41-IC
*/
#define SCD41_ADDRESS 0x62


/************************************************************************/
/* Command definitions                                                   */
/************************************************************************/

// basic commands
#define SCD41_COMMAND_START_PRERIODIC_MEASUREMENT						0x21b1
#define SCD41_COMMAND_READ_MEASUREMENT									0xec05
#define SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT						0x3f86

// On-chip output signal compensation
#define SCD41_COMMAND_SET_TEMPERATURE_OFFSET							0x241d
#define SCD41_COMMAND_GET_TEMPERATURE_OFFSET							0x2318
#define SCD41_COMMAND_SET_SENSOR_ALTITUDE								0x2427
#define SCD41_COMMAND_GET_SENSOR_ALTITUDE								0x2322
#define SCD41_COMMAND_SET_AMBIENT_PRESSURE								0xe000
#define SCD41_COMMAND_GET_AMBIENT_PRESSURE								0xe000

// Field calibration
#define SCD41_COMMAND_PERFORM_FORCED_RECALIBRATION						0x362f
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED			0x2416
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED			0x2313
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET				0x243a
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET				0x233f

// Low power
#define SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT				0x21ac
#define SCD41_COMMAND_GET_DATA_READY_STATUS								0xe4b8

// Advanced features
#define SCD41_COMMAND_PERSIST_SETTINGS									0x3615
#define SCD41_COMMAND_GET_SERIAL_NUMBER									0x3682
#define SCD41_COMMAND_PERFORM_SELF_TEST									0x3639
#define SCD41_COMMAND_PERFORM_FACTORY_RESET								0x3632
#define SCD41_COMMAND_REINIT											0x3646
#define SCD41_COMMAND_GET_SENSOR_VARIANT								0x202f

// Single Shot Measurement Mode
#define SCD41_COMMAND_MEASURE_SINGLE_SHOT								0x219d
#define SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY						0x2196
#define SCD41_COMMAND_POWER_DOWN										0x36e0
#define SCD41_COMMAND_WAKE_UP											0x36f6
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD		0x2445
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD		0x2340
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD	0x244e
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD	0x234b



/**
* @brief Retrieves the current state of the SCD41 sensor
*
* This function returns the current state of the SCD41 sensor
*
* @return The current state of the sensor
*	0x00: Standby
*	0x01: Running
*/
uint8_t scd41_get_current_state();

/**
* @brief Retrieves the current CO2 concentration measured by the SCD41 sensor.
*
* This function returns the latest CO2 (Carbon Dioxide) concentration reading from the SCD41 sensor.
*
* @return The measured CO2 concentration.
*/
uint16_t scd41_get_co2();

/**
* @brief Retrieves the current temperature measured by the SCD41 sensor.
*
* This function returns the latest temperature reading from the SCD41 sensor.
*
* @return The measured temperature.
*/
uint16_t scd41_get_temperature();

/**
* @brief Retrieves the current relative humidity measured by the SCD41 sensor.
*
* This function requests and returns the latest relative humidity reading from the SCD41 sensor.
*
* @return The measured relative humidity.
*/
uint16_t scd41_get_humidity();

/**
* @brief Starts a periodic measurement cycle on the SCD41 sensor.
*
* This function initiates a series of measurements at predefined intervals, allowing for continuous monitoring of CO2,
* temperature, and humidity.
*/
void start_periodic_measurement();

/**
* @brief Reads the latest measurement data from the SCD41 sensor.
*
* This function triggers a measurement and retrieves the CO2 concentration, temperature, and humidity values.
*
* @return A status code indicating whether the measurements were successful (1 - error, 0 - success):
*         Bit0: CO2
*         Bit1: Temperature
*         Bit2: Humidity
*/
uint8_t read_measurement();

/**
* @brief Stops the periodic measurement cycle on the SCD41 sensor.
*
* This function halts the automatic measurement sequence.
*
* @param wait - A boolean flag indicating whether to wait the needed processing time in the SCD41 sensor.
*              0: Do not wait
*              1: Wait the processing time
*/
void stop_periodic_measurement(uint8_t wait);

/**
* @brief Sets the temperature offset for measurements on the SCD41 sensor.
*
* This function allows adjusting the reported temperature values by a user-defined offset.
*
* @param offset - The temperature offset.
*/
void set_temperature_offset(uint16_t offset);

/**
* @brief Retrieves the currently set temperature offset for measurements on the SCD41 sensor.
*
* This function returns the current temperature offset value.
*
* @return The temperature offset.
*/
uint16_t get_temperature_offset();

/**
* @brief Sets the altitude correction for measurements on the SCD41 sensor.
*
* This function sets the altitude of the SCD41 sensor.
*
* @param altitude - The sensor's altitude in meters (m).
*/
void set_sensor_altitude(uint16_t altitude);

/**
* @brief Retrieves the currently set altitude correction for measurements on the SCD41 sensor.
*
* This function returns the current altitude correction value.
*
* @return uint16_t - The sensor's altitude in meters (m).
*/
uint16_t get_sensor_altitude();

/**
* @brief Sets the ambient pressure for measurements on the SCD41 sensor.
*
* This function adjusts the CO2 measurement results based on the ambient atmospheric pressure.
*
* @param pressure - The ambient pressure.
*/
void set_ambient_pressure(uint16_t pressure);

/**
* @brief Retrieves the currently set ambient pressure for measurements on the SCD41 sensor.
*
* This function returns the current ambient pressure value.
*
* @return The ambient pressure.
*/
uint16_t get_ambient_pressure();


/**
* @brief Performs a forced recalibration of the SCD41 sensor.
*
* This function triggers a forced recalibration procedure using a known reference CO2 concentration value.
* It can help correct measurement drift over time.
*
* @param reference - The known reference CO2 concentration in ppm.
* @param return_correction - A boolean flag indicating whether to return the calculated correction factor:
*                           0: Do not return the correction factor
*                           1: Return the calculated correction factor
*
* @return If `return_correction` is set to 1, returns the calculated correction factor.
*/
int16_t perform_forced_recalibration(uint16_t reference, uint8_t return_correction);

/**
* @brief Enables or disables the automatic self-calibration feature of the SCD41 sensor.
*
* This function controls whether the SCD41 sensor performs its automatic self-calibration (ASC) routine periodically to maintain accuracy.
*
* @param enabled - A boolean flag indicating whether to enable or disable the ASC feature:
*                 0: Disable automatic self-calibration
*                 1: Enable automatic self-calibration
*/
void set_automatic_self_calibration_enabled(uint8_t enabled);

/**
* @brief Retrieves the current status of the automatic self-calibration feature on the SCD41 sensor.
*
* This function checks whether the SCD41 sensor's automatic self-calibration (ASC) feature is currently enabled or disabled.
*
* @return uint8_t - A boolean flag indicating whether the ASC feature is enabled:
*                 0: Automatic self-calibration is disabled
*                 1: Automatic self-calibration is enabled
*/
uint8_t get_automatic_self_calibration_enabled();

/**
* @brief Sets the target CO2 concentration for the automatic self-calibration feature of the SCD41 sensor.
*
* This function specifies the target CO2 concentration in ppm to be used by the automatic self-calibration (ASC) routine.
* The ASC will aim to maintain this target concentration for accurate measurements.
*
* @param target_CO2 - The target CO2 concentration.
*/
void set_automatic_self_calibration_target(uint16_t target_CO2);

/**
* @brief Retrieves the current target CO2 concentration for the automatic self-calibration feature on the SCD41 sensor.
*
* This function returns the currently configured target CO2 concentration used by the automatic self-calibration (ASC) routine.
*
* @return The target CO2 concentration in ppm.
*/
uint16_t get_automatic_self_calibration_target();

/**
* @brief Starts a low-power periodic measurement cycle on the SCD41 sensor.
*
* This function initiates a periodic measurement cycle with reduced power consumption. The measurements are taken at regular intervals,
* and the data ready status can be checked using `get_data_ready_status`.
*/
void start_low_power_periodic_measurement();

/**
* @brief Retrieves the current data ready status of the SCD41 sensor.
*
* This function checks whether new measurement data is available from the sensor. If data is ready, it indicates that a new set
* of CO2, temperature, and humidity values can be read.
*
* @return A boolean flag indicating whether new data is available:
*                 0: No new data available
*                 1: New data is available
*/
uint8_t get_data_ready_status();

/**
* @brief Persist settings to non-volatile memory.
*
* This function saves the current settings to non-volatile memory, ensuring that they are not lost on power loss.
*
* @param wait If set to 1, the function will wait for the write operation to complete before returning. If set to 0, the function will return immediately without waiting.
*/
void persist_settings(uint8_t wait);

/**
* @brief Get the serial number of the device.
*
* This function retrieves the unique serial number associated with the device, which can be used for identification purposes.
*
* @return uint64_t The 64-bit unsigned integer representing the serial number of the device.
*/
uint64_t get_serial_number();

/**
* @brief Perform a self-test on the system.
*
* This function runs a comprehensive self-test to check the health and functionality of various components within the system. It may take some time depending on the complexity of the tests.
*
* @param wait If set to 1, the function will wait for the self-test to complete before returning. If set to 0, the function will return immediately without waiting.
* @return uint8_t A status code indicating whether the self-test passed or failed.
*/
uint8_t perform_self_test(uint8_t wait);

/**
* @brief Perform a factory reset on the system.
*
* This function resets all settings and configurations to their default values, effectively reverting the device to its original state as if it had just been manufactured. It may take some time to complete.
*
* @param wait If set to 1, the function will wait for the reset process to complete before returning. If set to 0, the function will return immediately without waiting.
*/
void perform_factory_reset(uint8_t wait);

/**
* @brief Reinitialize the system.
*
* This function performs a complete reinitialization of the system, which may include resetting settings, clearing buffers, and other initialization tasks. It may take some time to complete.
*
* @param wait If set to 1, the function will wait for the reinitialization process to complete before returning. If set to 0, the function will return immediately without waiting.
*/
void reinit(uint8_t wait);

/**
* @brief Get the variant of the sensor.
*
* This function retrieves the specific variant or model of the sensor currently in use. Different variants may have different features and capabilities.
*
* @return An identifier representing the variant of the sensor in Bits[15...12].
*         0000: SCD40
*         0001: SCD41
*/
uint8_t get_sensor_variant();

/**
* @brief Perform a single-shot measurement using the sensor.
*
* This function triggers a single measurement using the connected sensor.
*
* @param wait If set to 1, the function will wait for the measurement to complete before returning. If set to 0, the function will return immediately without waiting.
*/
void measure_single_shot(uint8_t wait);

/**
* @brief Perform a single-shot measurement without measuring the co2 concentration.
*
* This function triggers a single measurement using only the RHT (Relative Humidity and Temperature) sensor.
*
* @param wait If set to 1, the function will wait for the measurement to complete before returning. If set to 0, the function will return immediately without waiting.
*/
void measure_single_shot_rht_only(uint8_t wait);

/**
* @brief Power down the system or sensor.
*
* This function powers down the system or connected sensors to save power or enter a low-power state.
*/
void power_down();

/**
* @brief Wake up the system from a low-power state.
*
* This function wakes the system up from a low-power state, resuming normal operation. It may take some time to complete, depending on the duration of the sleep period.
*
* @param wait If set to 1, the function will wait for the system to fully wake up before returning. If set to 0, the function will return immediately without waiting.
*/
void wake_up(uint8_t wait);

/**
* @brief Set the initial period for automatic self-calibration.
*
* This function sets the duration of the initial calibration period after power-up or factory reset. The system will perform a full self-calibration during this period.
*
* @param hours The number of hours to set as the initial self-calibration period.
*/
void set_automatic_self_calibration_initial_period(uint16_t hours);

/**
* @brief Get the current setting for the initial period of automatic self-calibration.
*
* This function retrieves the currently configured duration of the initial calibration period after power-up or factory reset.
*
* @return uint16_t The number of hours set as the initial self-calibration period.
*/
uint16_t get_automatic_self_calibration_initial_period();

/**
* @brief Set the standard period for automatic self-calibration.
*
* This function sets the duration between subsequent calibration cycles to ensure long-term accuracy. The system will perform periodic self-calibrations during this interval.
*
* @param hours The number of hours to set as the standard self-calibration period.
*/
void set_automatic_self_calibration_standard_period(uint16_t hours);

/**
* @brief Get the current setting for the standard period of automatic self-calibration.
*
* This function retrieves the currently configured duration between subsequent calibration cycles for maintaining long-term accuracy.
*
* @return uint16_t The number of hours set as the standard self-calibration period.
*/
uint16_t get_automatic_self_calibration_standard_period();

#endif /* SCD41_H_ */