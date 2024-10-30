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
* @brief Get current sensor state
* @return 0 if stopped, 1 if running
*/
uint8_t scd41_get_current_state();

/**
* @brief Get last measured CO2 concentration
* @return CO2 concentration in ppm
*/
uint16_t scd41_get_co2();

/**
* @brief Get last measured temperature
* @return Temperature value
*/
uint16_t scd41_get_temperature();

/**
* @brief Get last measured humidity
* @return Relative humidity value
*/
uint16_t scd41_get_humidity();

/**
* @brief Start periodic measurement mode
*/
void start_periodic_measurement();

/**
* @brief Read latest measurement from sensor
* @note Updates last_co2, last_temperature and last_humidity
*/
void read_measurement();

/**
* @brief Stop periodic measurement mode
* @param wait If true, waits 500ms for command completion
*/
void stop_periodic_measurement(uint8_t wait);

/**
* @brief Set temperature offset
* @param offset Temperature offset value
*/
void set_temperature_offset(uint16_t offset);

/**
* @brief Get temperature offset
* @return Current temperature offset
*/
uint16_t get_temperature_offset();

/**
* @brief Set sensor altitude
* @param altitude Altitude in meters
*/
void set_sensor_altitude(uint16_t altitude);

/**
* @brief Get sensor altitude
* @return Current altitude in meters
*/
uint16_t get_sensor_altitude();

/**
* @brief Set ambient pressure
* @param pressure Ambient pressure value
*/
void set_ambient_pressure(uint16_t pressure);

/**
* @brief Get ambient pressure
* @return Current ambient pressure
*/
uint16_t get_ambient_pressure();

/**
* @brief Perform forced recalibration with reference CO2 value
* @param reference Reference CO2 concentration in ppm
* @param return_correction If true, returns correction value
* @return Correction value or 0xFFFF if checksum fails
*/
int16_t perform_forced_recalibration(uint16_t reference, uint8_t return_correction);

/**
* @brief Enable/disable automatic self-calibration (ASC)
* @param enabled 1 to enable ASC, 0 to disable
*/
void set_automatic_self_calibration_enabled(uint8_t enabled);

/**
* @brief Get automatic self-calibration state
* @return 1 if enabled, 0 if disabled, 0x02 if checksum fails
*/
uint8_t get_automatic_self_calibration_enabled();

/**
* @brief Set target CO2 concentration for automatic self-calibration
* @param target_CO2 Target CO2 concentration in ppm
*/
void set_automatic_self_calibration_target(uint16_t target_CO2);

/**
* @brief Get target CO2 concentration for automatic self-calibration
* @return Target CO2 in ppm, 0xFFFF if checksum fails
*/
uint16_t get_automatic_self_calibration_target();

/**
* @brief Start low power periodic measurement mode
*/
void start_low_power_periodic_measurement();

/**
* @brief Check if new measurement data is available
* @return 1 if data ready, 0 if not ready, 0x02 if checksum fails
*/
uint8_t get_data_ready_status();

/**
* @brief Save current configuration to EEPROM
* @param wait If 1, waits 500ms for EEPROM write
*/
void persist_settings(uint8_t wait);

/**
* @brief Get sensor serial number
* @return 48-bit serial number as 64-bit integer, 0 if checksum fails
*/
uint64_t get_serial_number();

/**
* @brief Perform sensor self-test
* @param wait If 1, waits 10 seconds for test
* @return 0 if pass, non-zero if fail, 0xFF if checksum fails
*/
uint8_t perform_self_test(uint8_t wait);

/**
* @brief Reset sensor to factory defaults
* @param wait If 1, waits for reset completion
*/
void perform_factory_reset(uint8_t wait);

/**
* @brief Reinitialize sensor
* @param wait If 1, waits for reinit completion
*/
void reinit(uint8_t wait);

/**
* @brief Get sensor variant
* @return Sensor variant in bits [15:12]:
*         0000: SCD40
*         0001: SCD41
*/
uint8_t get_sensor_variant();

/**
* @brief Perform single measurement
* @param wait If 1, waits 5000ms for measurement
*/
void measure_single_shot(uint8_t wait);

/**
* @brief Measure temperature and humidity only (no CO2)
* @param wait If 1, waits 50ms for measurement
*/
void measure_single_shot_rht_only(uint8_t wait);

/**
* @brief Enter power-down mode to minimize current consumption
*/
void power_down();

/**
* @brief Wake up sensor from power-down mode
* @param wait If 1, waits 30ms for wake-up
*/
void wake_up(uint8_t wait);

/**
* @brief Set initial period for automatic self-calibration
* @param hours Initial calibration period in hours
*/
void set_automatic_self_calibration_initial_period(uint16_t hours);

/**
* @brief Get initial period for automatic self-calibration
* @return Initial period in hours
*/
uint16_t get_automatic_self_calibration_initial_period();

/**
* @brief Set standard period for automatic self-calibration
* @param hours Standard calibration period in hours
*/
void set_automatic_self_calibration_standard_period(uint16_t hours);

/**
* @brief Get standard period for automatic self-calibration
* @return Standard period in hours
*/
uint16_t get_automatic_self_calibration_standard_period();

#endif /* SCD41_H_ */