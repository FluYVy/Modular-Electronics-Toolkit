/*
* SCD41.c
*
* Driver for the Sensirion SCD41 CO2, temperature and humidity sensor
* Communicates with the sensor via I2C
*
* Created: 30.09.2024 15:44:46
* Author: Florian Reichart
*/

#include "SCD41.h"

#ifndef F_CPU
#define F_CPU 12000000UL  // CPU frequency in Hz
#endif

// Measurement storage
uint16_t last_co2 = 0;         // CO2 concentration (ppm)
uint16_t last_temperature = 0;  // Temperature
uint16_t last_humidity = 0;     // Relative humidity

// Sensor state (0 = standby, 1 = measuring)
uint8_t current_state = 0;

/**
 * @brief Get current sensor operating state
 * @return 0 if in standby, 1 if measuring
 */
uint8_t scd41_get_current_state()
{
	return current_state;
}

/**
 * @brief Get last measured CO2 value
 * @return CO2 concentration in ppm
 */
uint16_t get_co2()
{
	return last_co2;
}

/**
 * @brief Get last measured temperature
 * @return Temperature
 */
uint16_t get_temperature()
{
	return last_temperature;
}

/**
 * @brief Get last measured humidity
 * @return Relative humidity
 */
uint16_t get_humidity()
{
	return last_humidity;
}

/**
 * @brief Start periodic measurement mode
 */
void start_periodic_measurement(){
	scd41_sequence_send_command(SCD41_COMMAND_START_PRERIODIC_MEASUREMENT);
	current_state = 1;	// Set sensor state to running
}

/**
 * @brief Read latest measurement from sensor
 * @note Updates last_co2, last_temperature and last_humidity
 */
void read_measurement()
{
	uint16_t data[3];
	scd41_sequence_read(SCD41_COMMAND_READ_MEASUREMENT, data, 3);
	
	last_co2 = data[0];
	last_temperature = data[1];
	last_humidity = data[2];
}

/**
 * @brief Stop periodic measurement mode
 * @param wait If true, waits 500ms for command completion
 */
void stop_periodic_measurement(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT);
	
	// Wait 500ms for command to complete if requested
	if (wait)
	{
		_delay_ms(500);
	}
	
	current_state = 0;  // Set sensor state to stopped
}

/**
 * @brief Generate CRC8 checksum for 16-bit value
 * @param value Value to generate checksum for
 * @return CRC8 checksum
 * @note Based on: https://stackoverflow.com/a/51773839
 */
uint8_t generate_checksum(uint16_t value)
{
	uint8_t crc = 0xff; // Initialize CRC with all bits set
	
	// Process each byte of the 16-bit value
	for (uint8_t i = 0; i < 2; i++)
	{
		crc ^= (uint8_t)(value>>8*(1-i));  	// XOR the current byte into CRC

		
		// Process each bit using CRC-8 polynomial
		for (uint8_t j = 0; j < 8; j++)
		{
			// If MSB is 1, shift left and XOR with polynomial 0x31
			if ((crc & 0x80) != 0)
			{
				crc = (uint8_t)((crc << 1) ^ 0x31);
			}
			// If MSB is 0, just shift left
			else
			{
				crc <= 1;
			}
		}
	}
	
	return crc;
}

/**
 * @brief Validate checksum against 16-bit value
 * @param value Value to validate
 * @param checksum Expected checksum
 * @return 1 if valid, 0 if invalid
 */
uint8_t check_checksum(uint16_t value, uint8_t checksum)
{
	return (uint8_t)(generate_checksum(value) == checksum);
}

/**
 * @brief Set temperature offset for compensation
 * @param offset Temperature offset
 */
void set_temperature_offset(uint16_t offset)
{
	scd41_sequence_write(SCD41_COMMAND_SET_TEMPERATURE_OFFSET, offset);
}

/**
 * @brief Get current temperature offset
 * @return Temperature offset, 0xFFFF if checksum fails
 */
uint16_t get_temperature_offset()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_TEMPERATURE_OFFSET, data, 1);
	return data[0];
}

/**
 * @brief Set sensor altitude for pressure compensation
 * @param altitude Altitude in meters above sea level
 */
void set_sensor_altitude(uint16_t altitude)
{
	scd41_sequence_write(SCD41_COMMAND_SET_SENSOR_ALTITUDE, altitude);
}

/**
 * @brief Get current sensor altitude setting
 * @return Altitude in meters, 0xFFFF if checksum fails
 */
uint16_t get_sensor_altitude()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_SENSOR_ALTITUDE, data, 1);
	return data[0];
}

/**
 * @brief Set ambient pressure for compensation
 * @param pressure Ambient pressure in Pascal/100
 */
void set_ambient_pressure(uint16_t pressure)
{
	scd41_sequence_write(SCD41_COMMAND_SET_AMBIENT_PRESSURE, pressure);
}

/**
 * @brief Get current ambient pressure setting
 * @return Pressure in Pascal/100, 0xFFFF if checksum fails
 */
uint16_t get_ambient_pressure()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_AMBIENT_PRESSURE, data, 1);
	return data[0];
}

/**
 * @brief Perform forced recalibration with reference CO2 value
 * @param reference Reference CO2 concentration in ppm
 * @param return_correction If true, returns correction value
 * @return Correction value or 0x7FFF if checksum fails
 */
int16_t perform_forced_recalibration(uint16_t reference, uint8_t return_correction)
{
	// Variable to store the FRC correction value
	int16_t correction = 0;
	
	// Start I2C communication in write mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send forced recalibration command
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_FORCED_RECALIBRATION>>8);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_FORCED_RECALIBRATION);
	
	// Send reference CO2 concentration value
	i2c_master_sendChar(reference>>8);    // Upper byte
	i2c_master_sendChar(reference);       // Lower byte
	
	// Send checksum for reference value
	i2c_master_sendChar(generate_checksum(reference));
	
	// Wait for forced recalibration to complete (400ms according to datasheet)
	_delay_ms(400);
	
	// If return_correction flag is set, read back the correction value
	if(return_correction)
	{
		// Start new I2C transaction in read mode
		i2c_master_start();
		i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
		
		// Read correction value
		correction |= i2c_master_receiveChar(0x01)<<8;    // Upper byte
		correction |= i2c_master_receiveChar(0x01);       // Lower byte
		
		// Read checksum byte
		uint8_t checksum = i2c_master_receiveChar(0x00);
		
		// Validate checksum if enabled
		#ifdef SCD41_PROCESS_CHECKSUM
		if (!check_checksum(correction, checksum))
		{
			correction = 0xFfff;  // Return error value if checksum invalid
		}
		#endif
	}
	
	// Stop I2C communication
	i2c_master_stop();
	return correction;
}

/**
 * @brief Enable/disable automatic self-calibration (ASC)
 * @param enabled 1 to enable ASC, 0 to disable
 */
void set_automatic_self_calibration_enabled(uint8_t enabled)
{
	scd41_sequence_write(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED, (uint16_t)enabled);
}

/**
 * @brief Get automatic self-calibration state
 * @return 1 if enabled, 0 if disabled, 0x02 if checksum fails
 */
uint8_t get_automatic_self_calibration_enabled()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED, data, 1);
	
	if (data[0] == 1)
	{
		return 0x01;
	}else
	{
		return 0x00;
	}
}

/**
 * @brief Set target CO2 concentration for automatic self-calibration
 * @param target_CO2 Target CO2 concentration in ppm
 */
void set_automatic_self_calibration_target(uint16_t target_CO2)
{
	scd41_sequence_write(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET, target_CO2);
}

/**
 * @brief Get target CO2 concentration for automatic self-calibration
 * @return Target CO2 in ppm, 0xFFFF if checksum fails
 */
uint16_t get_automatic_self_calibration_target()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET, data, 1);
	return data[0];
}

/**
 * @brief Start low power periodic measurement mode
 */
void start_low_power_periodic_measurement()
{
	scd41_sequence_send_command(SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT);
}

/**
 * @brief Check if new measurement data is available
 * @return 1 if data ready, 0 if not ready, 0x02 if checksum fails
 */
uint8_t get_data_ready_status()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED, data, 1);
	if (data[0] == 1)
	{
		return 0x01;
	}else
	{
		return 0x00;
	}
}

/**
 * @brief Save current configuration to EEPROM
 * @param wait If 1, waits 500ms for EEPROM write
 */
void persist_settings(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_PERSIST_SETTINGS);
	// Wait 500ms for EEPROM write to complete if requested
	if (wait)
	{
		_delay_ms(500);
	}
}

/**
 * @brief Get sensor serial number
 * @return 48-bit serial number as 64-bit integer, 0 if checksum fails
 */
uint64_t get_serial_number()
{
	uint16_t data[3];
	scd41_sequence_read(SCD41_COMMAND_GET_SENSOR_ALTITUDE, data, 3);
	return (data[2])<<32 | (data[1])<<16 | (data[0]));
}

/**
 * @brief Perform sensor self-test
 * @param wait If 1, waits 10 seconds for test
 * @return 0 if pass, non-zero if fail, 0xFF if checksum fails
 */
uint8_t perform_self_test()
{
	// Variable to store the sensor's self-test result
	uint16_t readout = 0;
	
	// Start I2C communication and send self-test command
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_SELF_TEST>>8);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_SELF_TEST);
	
	// Wait 10 seconds for self-test to complete
	_delay_ms(10000);
	
	// Read self-test result from sensor
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	readout = i2c_master_receiveChar(0x01);
	readout = i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	// Validate checksum if enabled
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(readout, checksum))
	{
		return 0xff;  // Return 0xFF if checksum validation fails
	}
	#endif
	
	// Combine and return the test result bytes
	return ((uint8_t)readout | (uint8_t)(readout>>8));
}

/**
 * @brief Reset sensor to factory defaults
 * @param wait If 1, waits 1200ms for reset
 */
void perform_factory_reset(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_PERFORM_FACTORY_RESET);
	
	// If wait parameter is set, delay for 1200ms while reset completes
	if (wait)
	{
		_delay_ms(1200);
	}
}

/**
 * @brief Reinitialize sensor
 * @param wait If 1, waits 30ms for reinit
 */
void reinit(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_REINIT);
	
	// If wait parameter is set, delay for 30ms while reinitialization completes
	if (wait)
	{
		_delay_ms(30);
	}
}

/**
 * @brief Get sensor variant/type
 * @return 4-bit sensor variant code, 0xFF if checksum fails
 */
uint8_t get_sensor_variant()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_SENSOR_ALTITUDE, data, 1);
		
	// Extract and return the 4-bit sensor variant code from the readout
	return (uint8_t)(data[0]>>12);
}

/**
 * @brief Perform single measurement of CO2, temperature and humidity
 * @param wait If 1, waits 5000ms for measurement
 */
void measure_single_shot(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_MEASURE_SINGLE_SHOT);
	
	// If wait parameter is set, delay for 5000ms while measurement completes
	if (wait)
	{
		_delay_ms(5000);
	}
}

/**
 * @brief Measure temperature and humidity only (no CO2)
 * @param wait If 1, waits 50ms for measurement
 */
void measure_single_shot_rht_only(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY);
	
	// If wait parameter is set, delay for 50ms while measurement completes
	if (wait)
	{
		_delay_ms(50);
	}
}

/**
 * @brief Enter power-down mode to minimize current consumption
 */
void power_down()
{
	scd41_sequence_send_command(SCD41_COMMAND_MEASURE_SINGLE_SHOT);
}

/**
 * @brief Wake up sensor from power-down mode
 * @param wait If 1, waits 30ms for wake-up
 */
void wake_up(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_WAKE_UP);
	
	// If wait parameter is set, delay for 30ms while sensor wakes up
	if (wait)
	{
		_delay_ms(30);
	}
}

/**
 * @brief Set initial period for automatic self-calibration
 * @param hours Initial calibration period in hours
 */
void set_automatic_self_calibration_initial_period(uint16_t hours)
{
	scd41_sequence_write(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD, hours);
}

/**
 * @brief Get initial period for automatic self-calibration
 * @return Period in hours, 0xFFFF if checksum fails
 */
uint16_t get_automatic_self_calibration_initial_period()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD, data, 1);
	return data[0];
}

/**
 * @brief Set standard period for automatic self-calibration
 * @param hours Standard calibration period in hours
 */
void set_automatic_self_calibration_standard_period(uint16_t hours)
{
	scd41_sequence_write(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD, hours);
}

/**
 * @brief Get standard period for automatic self-calibration
 * @return Period in hours, 0xFFFF if checksum fails
 */
uint16_t get_automatic_self_calibration_standard_period()
{
	uint16_t data[1];
	scd41_sequence_read(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD, data, 1);
	return data[0];
}

/**
 * @brief Write command and data to sensor
 * @param command Command to send
 * @param data Data to write
 */
void scd41_sequence_write(uint16_t command, uint16_t data)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(command<<8);
	i2c_master_sendChar(command);
	i2c_master_sendChar(data<<8);
	i2c_master_sendChar(data);
	i2c_master_sendChar(generate_checksum(data));
	i2c_master_stop();
}

/**
 * @brief Send command to sensor
 * @param command Command to send
 */
void scd41_sequence_send_command(uint16_t command)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(command<<8);
	i2c_master_sendChar(command);
	i2c_master_stop();
}

/**
 * @brief Read data from sensor
 * @param command Command to send
 * @param readData Array to store read data
 * @param length Number of 16-bit words to read
 */
void scd41_sequence_read(uint16_t command, uint16_t readData[], uint8_t length)
{
	// Start I2C communication in write mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command bytes
	i2c_master_sendChar(command<<8);    // Upper byte
	i2c_master_sendChar(command);       // Lower byte
	
	// Restart I2C in read mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	// Read requested number of 16-bit words
	for (uint8_t data_index = 0; data_index < length; data_index++)
	{
		uint16_t received_Data = 0;
		uint8_t received_Data_crc = 0;
		
		// Read 16-bit data value
		received_Data = i2c_master_receiveChar(0x01)<<8;    // Upper byte
		received_Data |= i2c_master_receiveChar(0x01);      // Lower byte
		
		// Read CRC byte - NACK on last byte, ACK otherwise
		if (data_index == length-1)
		{
			received_Data_crc = i2c_master_receiveChar(0x00);  // Last byte - NACK
		}else
		{
			received_Data_crc = i2c_master_receiveChar(0x01);  // Not last byte - ACK
		}
		
		// Process checksum if enabled
		#ifdef SCD41_PROCESS_CHECKSUM
		uint16_t
		if (check_checksum(received_Data, received_Data_crc))
		{
			readData[data_index] = received_Data;          // Store valid data
		}else
		{
			readData[data_index] = 0xFFFF;                // Store error value
		}
		#else
		readData[data_index] = received_Data;             // Store data without validation
		#endif
	}
	
	// End I2C communication
	i2c_master_stop();
}