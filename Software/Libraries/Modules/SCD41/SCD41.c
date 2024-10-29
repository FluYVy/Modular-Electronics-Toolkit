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

// Variables to store the last measured values from the sensor
uint16_t last_co2 = 0;         // CO2 concentration in ppm
uint16_t last_temperature = 0;  // Temperature in degrees Celsius
uint16_t last_humidity = 0;     // Relative humidity in %

// Variable to keep track of the current state of the sensor
// 0 = standby/stopped, 1 = running/measuring
uint8_t current_state = 0;

// Returns the current state of the sensor (0 = standby, 1 = running)
uint8_t scd41_get_current_state()
{
	return current_state;
}

// Returns the last measured CO2 value in ppm
uint16_t get_co2()
{
	return last_co2;
}

// Returns the last measured temperature in degrees Celsius
uint16_t get_temperature()
{
	return last_temperature;
}

// Returns the last measured relative humidity in %
uint16_t get_humidity()
{
	return last_humidity;
}

// Starts periodic measurement mode on the sensor
void start_periodic_measurement(){
	i2c_master_start();                            // Start I2C communication
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);   // Send SCD41 sensor address with write bit
	i2c_master_sendChar(SCD41_COMMAND_START_PRERIODIC_MEASUREMENT >> 8);    // Send upper byte of start command
	i2c_master_sendChar(SCD41_COMMAND_START_PRERIODIC_MEASUREMENT);         // Send lower byte of start command
	i2c_master_stop();                             // Stop I2C communication
	
	current_state = 1;                             // Set sensor state to running
}

// Reads the latest measurement from the sensor
// Returns error flags if checksum validation is enabled
uint8_t read_measurement()
{
	uint8_t errors = 0x00;
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Define temporary variables for storing measurements and checksums
	uint16_t co2 = 0;
	uint16_t temperature = 0;
	uint16_t humidity = 0;
	
	uint8_t co2_checksum = 0;
	uint8_t temperature_checksum = 0;
	uint8_t humidity_checksum = 0;
	#endif
	
	i2c_master_start();							// Start I2C communication
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00); // Send sensor address with write bit
	i2c_master_sendChar(SCD41_COMMAND_READ_MEASUREMENT>>8);	// Send upper byte of read command
	i2c_master_sendChar(SCD41_COMMAND_READ_MEASUREMENT);		// Send lower byte of read command
	i2c_master_start();											// Repeated start
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);				// Send sensor address with read bit
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Read measurements and checksums when validation is enabled
	co2 = i2c_master_receiveChar(0x01)<<8;
	co2 |= i2c_master_receiveChar(0x01);
	co2_checksum = i2c_master_receiveChar(0x01);
	temperature = i2c_master_receiveChar(0x01)<<8;
	temperature |= i2c_master_receiveChar(0x01);
	temperature_checksum = i2c_master_receiveChar(0x01);
	humidity = i2c_master_receiveChar(0x01)<<8;
	humidity |= i2c_master_receiveChar(0x01);
	humidity_checksum = i2c_master_receiveChar(0x00);
	#else
	// Read measurements directly when validation is disabled
	last_co2 = i2c_master_receiveChar(0x01)<<8;
	last_co2 |= i2c_master_receiveChar(0x01);
	i2c_master_receiveChar(0x01);  // Skip checksum byte
	last_temperature = i2c_master_receiveChar(0x01)<<8;
	last_temperature |= i2c_master_receiveChar(0x01);
	i2c_master_receiveChar(0x01);  // Skip checksum byte
	last_humidity = i2c_master_receiveChar(0x01)<<8;
	last_humidity |= i2c_master_receiveChar(0x01);
	i2c_master_receiveChar(0x00);  // Skip checksum byte
	#endif
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Validate checksums and update measurements if valid
	errors = 0x07;  // Set all error bits initially
	
	last_co2 = 0;
	last_temperature = 0;
	last_humidity = 0;
	
	if(check_checksum(co2, co2_checksum)){
		last_co2 = co2;
		errors &= (0x01);  // Clear CO2 error bit
	}
	
	if(check_checksum(temperature, temperature_checksum)){
		last_temperature = temperature;
		errors &= (0x02);  // Clear temperature error bit
	}
	
	if(check_checksum(humidity, humidity_checksum)){
		last_humidity = humidity;
		errors &= (0x04);  // Clear humidity error bit
	}
	#endif
	return errors; // Return error flags
}

// Stops periodic measurement mode
// wait: if true, waits 500ms for command to complete
void stop_periodic_measurement(uint8_t wait)
{
	i2c_master_start(); // Initialize I2C communication
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00); // Send device address in write mode
	i2c_master_sendChar(SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT>>8); // Send stop measurement command (high byte)
	i2c_master_sendChar(SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT); // Send stop measurement command (low byte)
	i2c_master_stop(); // End I2C transmission
	
	// Wait 500ms for command to complete if requested
	if (wait)
	{
		_delay_ms(500);
	}
	
	current_state = 0;  // Set sensor state to stopped
}

// Generates CRC8 checksum for a 16-bit value
// Based on: https://stackoverflow.com/a/51773839
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
	
	// Return final CRC-8 checksum
	return crc;
}

// Validates a checksum against a 16-bit value
// Returns 1 if valid, 0 if invalid
uint8_t check_checksum(uint16_t value, uint8_t checksum)
{
	return (uint8_t)(generate_checksum(value) == checksum);
}

// Sets the temperature offset for compensation
// offset: temperature offset in degrees Celsius
void set_temperature_offset(uint16_t offset)
{
	// Start I2C communication
	i2c_master_start();
	
	// Send SCD41 sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command bytes for setting temperature offset
	i2c_master_sendChar(SCD41_COMMAND_SET_TEMPERATURE_OFFSET>>8);    // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_SET_TEMPERATURE_OFFSET);       // Lower byte
	
	// Send temperature offset value
	i2c_master_sendChar(offset>>8);    // Upper byte
	i2c_master_sendChar(offset);       // Lower byte
	
	// Send CRC8 checksum of offset value
	i2c_master_sendChar(generate_checksum(offset));
	
	// Stop I2C communication
	i2c_master_stop();
}

// Gets the current temperature offset
// Returns 0xFFFF if checksum validation fails
uint16_t get_temperature_offset()
{
	// Initialize offset variable
	uint16_t offset = 0;
	
	// Start I2C communication
	i2c_master_start();
	
	// Send device address in write mode
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command bytes for getting temperature offset
	i2c_master_sendChar(SCD41_COMMAND_GET_TEMPERATURE_OFFSET>>8);    // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_GET_TEMPERATURE_OFFSET);       // Lower byte
	
	// Restart I2C for reading response
	i2c_master_start();
	
	// Send device address in read mode
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	// Read temperature offset value
	offset |= i2c_master_receiveChar(0x01)<<8;    // Upper byte
	offset |= i2c_master_receiveChar(0x01);       // Lower byte
	
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	
	// Stop I2C communication
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Validate checksum if enabled
	if (!check_checksum(offset, checksum))
	{
		offset = 0xffff;  // Return error value if checksum invalid
	}
	#endif
	return offset;
}

// Sets the sensor altitude for pressure compensation
// altitude: altitude in meters above sea level
void set_sensor_altitude(uint16_t altitude)
{
	// Start I2C communication
	i2c_master_start();
	
	// Send device address in write mode
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command bytes for setting sensor altitude
	i2c_master_sendChar(SCD41_COMMAND_SET_SENSOR_ALTITUDE>>8);    // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_SET_SENSOR_ALTITUDE);       // Lower byte
	
	// Send altitude value
	i2c_master_sendChar(altitude>>8);    // Upper byte
	i2c_master_sendChar(altitude);       // Lower byte
	
	// Send CRC8 checksum of altitude value
	i2c_master_sendChar(generate_checksum(altitude));
	
	// Stop I2C communication
	i2c_master_stop();
}

// Gets the current sensor altitude setting
// Returns 0xFFFF if checksum validation fails
uint16_t get_sensor_altitude()
{
	// Initialize altitude variable
	uint16_t altitude = 0;
	
	// Start I2C communication
	i2c_master_start();
	
	// Send device address in write mode
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command bytes for getting sensor altitude
	i2c_master_sendChar(SCD41_COMMAND_GET_SENSOR_ALTITUDE>>8);    // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_GET_SENSOR_ALTITUDE);       // Lower byte
	
	// Restart I2C for reading response
	i2c_master_start();
	
	// Send device address in read mode
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	// Read altitude value
	altitude |= i2c_master_receiveChar(0x01)<<8;    // Upper byte
	altitude |= i2c_master_receiveChar(0x01);       // Lower byte
	
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	
	// Stop I2C communication
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Validate checksum if enabled
	if (!check_checksum(altitude, checksum))
	{
		altitude = 0xffff;  // Return error value if checksum invalid
	}
	#endif
	return altitude;
}

// Sets the ambient pressure for compensation
// pressure: ambient pressure in Pascal
void set_ambient_pressure(uint16_t pressure)
{
	// Start I2C communication
	i2c_master_start();
	
	// Send device address in write mode
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command bytes for setting ambient pressure
	i2c_master_sendChar(SCD41_COMMAND_SET_AMBIENT_PRESSURE>>8);    // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_SET_AMBIENT_PRESSURE);       // Lower byte
	
	// Send pressure value
	i2c_master_sendChar(pressure>>8);    // Upper byte
	i2c_master_sendChar(pressure);       // Lower byte
	
	// Send checksum byte
	i2c_master_sendChar(generate_checksum(pressure));
	
	// Stop I2C communication
	i2c_master_stop();
}

// Gets the current ambient pressure setting
// Returns 0xFFFF if checksum validation fails
uint16_t get_ambient_pressure()
{
	// Variable to store the ambient pressure reading
	uint16_t pressure = 0;
	
	// Start I2C communication
	i2c_master_start();
	
	// Send device address in write mode
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command bytes for getting ambient pressure
	i2c_master_sendChar(SCD41_COMMAND_GET_AMBIENT_PRESSURE>>8);    // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_GET_AMBIENT_PRESSURE);       // Lower byte
	
	// Restart I2C communication in read mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	// Read pressure value
	pressure |= i2c_master_receiveChar(0x01)<<8;    // Upper byte
	pressure |= i2c_master_receiveChar(0x01);       // Lower byte
	
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	
	// Stop I2C communication
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Validate checksum if enabled
	if (!check_checksum(pressure, checksum))
	{
		pressure = 0xffff;  // Return error value if checksum invalid
	}
	#endif
	return pressure;
}

// Performs forced recalibration with a reference CO2 value
// reference: Reference CO2 concentration in ppm
// return_correction: If true, returns the correction value
// Returns correction value or 0x7FFF if checksum fails
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
			correction = 0x7fff;  // Return error value if checksum invalid
		}
		#endif
	}
	
	// Stop I2C communication
	i2c_master_stop();
	return correction;
}

// Enables or disables automatic self-calibration (ASC) feature
// enabled: 1 to enable ASC, 0 to disable ASC
void set_automatic_self_calibration_enabled(uint8_t enabled)
{
	// Start I2C communication in write mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command to set automatic self-calibration state
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED>>8);  // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED);     // Lower byte
	
	// Send enabled state (0x00 = disabled, 0x01 = enabled)
	i2c_master_sendChar(0x00);    // Upper byte is always 0
	i2c_master_sendChar(enabled); // Lower byte contains enabled state
	
	// Send checksum for validation
	i2c_master_sendChar(generate_checksum((uint16_t)enabled));
	
	// End I2C communication
	i2c_master_stop();
}

// Gets the current state of automatic self-calibration
// Returns: 1 if ASC is enabled, 0 if disabled, 0x02 if checksum fails
uint8_t get_automatic_self_calibration_enabled()
{
	// Initialize return value
	uint8_t enabled = 0;
	
	// Start I2C communication in write mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command to get automatic self-calibration state
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED>>8);  // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED);     // Lower byte
	
	// Restart I2C communication in read mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	// Read enabled state (2 bytes)
	i2c_master_receiveChar(0x01);      // Discard upper byte (always 0)
	enabled = i2c_master_receiveChar(0x01);  // Lower byte contains enabled state
	
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	
	// End I2C communication
	i2c_master_stop();
	
	// Validate checksum if enabled
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum((uint16_t)enabled, checksum))
	{
		enabled = 0x02;  // Return error value if checksum invalid
	}
	#endif
	return enabled;
}

// Sets the target CO2 concentration for automatic self-calibration
// target_CO2: Target CO2 concentration in ppm
void set_automatic_self_calibration_target(uint16_t target_CO2)
{
	// Start I2C communication in write mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command to set automatic self-calibration target
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET>>8);  // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET);     // Lower byte
	
	// Send target CO2 value (2 bytes)
	i2c_master_sendChar((uint8_t)target_CO2>>8);  // Upper byte
	i2c_master_sendChar((uint8_t)target_CO2);     // Lower byte
	
	// Send checksum for validation
	i2c_master_sendChar(generate_checksum(target_CO2));
	
	// End I2C communication
	i2c_master_stop();
}

// Gets the current target CO2 concentration for automatic self-calibration
// Returns: Target CO2 concentration in ppm, or 0xFFFF if checksum fails
uint16_t get_automatic_self_calibration_target()
{
	// Variable to store the target CO2 concentration
	uint16_t target = 0;
	
	// Start I2C communication in write mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command to get automatic self-calibration target
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET>>8);  // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET);     // Lower byte
	
	// Restart I2C communication in read mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	// Read target CO2 value (2 bytes)
	target |= i2c_master_receiveChar(0x01)<<8;  // Upper byte
	target |= i2c_master_receiveChar(0x01);     // Lower byte
	
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	
	// End I2C communication
	i2c_master_stop();
	
	// Validate checksum if enabled
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(target, checksum))
	{
		target = 0xffff;  // Return error value if checksum invalid
	}
	#endif
	return target;
}

// Starts periodic measurement in low power mode
// Measurements are taken every 30 seconds
void start_low_power_periodic_measurement()
{
	// Start I2C communication
	i2c_master_start();
	
	// Send SCD41 sensor address with write bit (0x00)
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command to start low power periodic measurement
	i2c_master_sendChar(SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT>>8);  // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT);     // Lower byte
	
	// End I2C communication
	i2c_master_stop();
}

// Checks if new measurement data is available
// Returns: 1 if data is ready, 0 if not ready or checksum fails
uint8_t get_data_ready_status()
{
	// Initialize status and readout variables
	uint8_t status = 0;
	uint16_t readout = 0;
	
	// Start I2C communication
	i2c_master_start();
	
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send command to check data ready status
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED>>8);  // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED);     // Lower byte
	
	// Restart I2C in read mode
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	// Read data ready status (2 bytes)
	readout |= i2c_master_receiveChar(0x01)<<8;  // Upper byte
	readout |= i2c_master_receiveChar(0x01);     // Lower byte
	
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	
	// End I2C communication
	i2c_master_stop();
	
	// Check if data is ready by examining lower 11 bits
	// If all bits are 0, no new data is available
	if ((readout & 0x07ff) == 0)
	{
		status = 0;  // No new data available
	}else
	{
		status = 1;  // New data is ready
	}
	
	// Validate checksum if enabled
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum((uint16_t)readout, checksum))
	{
		status = 0;  // Return not ready if checksum fails
	}
	#endif
	
	return status;
}

// Saves current configuration to EEPROM
// wait: If 1, waits 500ms for EEPROM write to complete
void persist_settings(uint8_t wait)
{
	// Start I2C communication
	i2c_master_start();
	
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	
	// Send persist settings command (2 bytes)
	i2c_master_sendChar(SCD41_COMMAND_PERSIST_SETTINGS>>8);  // Upper byte
	i2c_master_sendChar(SCD41_COMMAND_PERSIST_SETTINGS);     // Lower byte
	
	// End I2C communication
	i2c_master_stop();
	
	// Wait 500ms for EEPROM write to complete if requested
	if (wait)
	{
		_delay_ms(500);
	}
}

// Gets the unique serial number of the sensor
// Returns: 48-bit serial number, or 0 if checksum fails
// Gets the unique serial number of the sensor by reading three 16-bit words
// Returns a 48-bit serial number combined into a 64-bit integer
// Returns 0 if checksum validation fails (when enabled)
uint64_t get_serial_number()
{
	// Initialize variables to store the three 16-bit words of the serial number
	uint16_t serial_number_word_1 = 0;
	uint16_t serial_number_word_2 = 0;
	uint16_t serial_number_word_3 = 0;
	
	// Start I2C communication and send command to get serial number
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	// Read first word and its checksum
	serial_number_word_1 |= i2c_master_receiveChar(0x01)<<8;
	serial_number_word_1 |= i2c_master_receiveChar(0x01);
	uint8_t checksum_word_1 = i2c_master_receiveChar(0x01);
	
	// Read second word and its checksum
	serial_number_word_2 |= i2c_master_receiveChar(0x01)<<8;
	serial_number_word_2 |= i2c_master_receiveChar(0x01);
	uint8_t checksum_word_2 = i2c_master_receiveChar(0x01);
	
	// Read third word and its checksum
	serial_number_word_3 |= i2c_master_receiveChar(0x01)<<8;
	serial_number_word_3 |= i2c_master_receiveChar(0x01);
	uint8_t checksum_word_3 = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	// Validate checksums if enabled
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!(check_checksum(serial_number_word_1, checksum_word_1) || check_checksum(serial_number_word_2, checksum_word_2) || check_checksum(serial_number_word_3, checksum_word_3)))
	{
		return 0
	}
	#endif
	
	// Combine the three 16-bit words into a 48-bit serial number
	// Word 1 shifted left by 32, word 2 by 16, word 3 unchanged
	return (uint64_t)((serial_number_word_1)<<32 | (serial_number_word_2)<<16 | (serial_number_word_3));
}

// Performs sensor self-test
// wait: If 1, waits 10 seconds for test to complete
// Returns: 0 if test passes, non-zero if test fails, 0xFF if checksum fails
uint8_t perform_self_test(uint8_t wait)
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

// Resets sensor to factory defaults
// wait: If 1, waits 1200ms for reset to complete
void perform_factory_reset(uint8_t wait)
{
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send factory reset command (upper byte)
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_FACTORY_RESET>>8);
	// Send factory reset command (lower byte)
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_FACTORY_RESET);
	// Stop I2C communication
	i2c_master_stop();
	
	// If wait parameter is set, delay for 1200ms while reset completes
	if (wait)
	{
		_delay_ms(1200);
	}
}

// Reinitializes sensor
// wait: If 1, waits 30ms for reinitialization
void reinit(uint8_t wait)
{
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send reinitialization command (upper byte)
	i2c_master_sendChar(SCD41_COMMAND_REINIT>>8);
	// Send reinitialization command (lower byte)
	i2c_master_sendChar(SCD41_COMMAND_REINIT);
	// Stop I2C communication
	i2c_master_stop();
	
	// If wait parameter is set, delay for 30ms while reinitialization completes
	if (wait)
	{
		_delay_ms(30);
	}
}

// Gets the sensor variant/type
// Returns: Sensor variant code, or 0xFF if checksum fails
uint8_t get_sensor_variant()
{
	// Variable to store the sensor variant readout
	uint16_t readout = 0;
	
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send get sensor variant command (upper byte)
	i2c_master_sendChar(SCD41_COMMAND_GET_SENSOR_VARIANT>>8);
	// Send get sensor variant command (lower byte)
	i2c_master_sendChar(SCD41_COMMAND_GET_SENSOR_VARIANT);
	// Restart I2C communication for reading
	i2c_master_start();
	// Send sensor address with read bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	// Read first byte of sensor variant data
	readout = i2c_master_receiveChar(0x01);
	// Read second byte of sensor variant data
	readout = i2c_master_receiveChar(0x01);
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	// Stop I2C communication
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Verify checksum if enabled, return error code if invalid
	if (!check_checksum(readout, checksum))
	{
		return 0xff;
	}
	#endif
	
	// Extract and return the 4-bit sensor variant code from the readout
	return (uint8_t)(readout>>12);
}

// Performs single measurement of CO2, temperature and humidity
// wait: If 1, waits 5000ms for measurement to complete
void measure_single_shot(uint8_t wait)
{
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send single shot measurement command (upper byte)
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT>>8);
	// Send single shot measurement command (lower byte)
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT);
	// Stop I2C communication
	i2c_master_stop();
	
	// If wait parameter is set, delay for 5000ms while measurement completes
	if (wait)
	{
		_delay_ms(5000);
	}
}

// Performs single measurement of temperature and humidity only (no CO2)
// wait: If 1, waits 50ms for measurement to complete
void measure_single_shot_rht_only(uint8_t wait)
{
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send RHT-only measurement command (upper byte)
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY>>8);
	// Send RHT-only measurement command (lower byte)
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY);
	// Stop I2C communication
	i2c_master_stop();
	
	// If wait parameter is set, delay for 50ms while measurement completes
	if (wait)
	{
		_delay_ms(50);
	}
}

// Puts sensor into power-down mode to minimize current consumption
void power_down()
{
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send power down command (upper byte)
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT>>8);
	// Send power down command (lower byte)
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT);
	// Stop I2C communication
	i2c_master_stop();
}

// Wakes up sensor from power-down mode
// wait: If 1, waits 30ms for wake-up
void wake_up(uint8_t wait)
{
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send wake up command (upper byte)
	i2c_master_sendChar(SCD41_COMMAND_WAKE_UP>>8);
	// Send wake up command (lower byte)
	i2c_master_sendChar(SCD41_COMMAND_WAKE_UP);
	// Stop I2C communication
	i2c_master_stop();
	
	// If wait parameter is set, delay for 30ms while sensor wakes up
	if (wait)
	{
		_delay_ms(30);
	}
}

// Sets the initial period for automatic self-calibration
// hours: Initial calibration period in hours
void set_automatic_self_calibration_initial_period(uint16_t hours)
{
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send command upper byte
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD>>8);
	// Send command lower byte
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD);
	// Send hours value upper byte
	i2c_master_sendChar(hours>>8);
	// Send hours value lower byte
	i2c_master_sendChar(hours);
	// Send checksum for hours value
	i2c_master_sendChar(generate_checksum(hours));
	// Stop I2C communication
	i2c_master_stop();
}

// Gets the initial period for automatic self-calibration
// Returns: Initial calibration period in hours, or 0xFFFF if checksum fails
uint16_t get_automatic_self_calibration_initial_period()
{
	// Variable to store the calibration period in hours
	uint16_t hours = 0;
	
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send command upper byte
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD>>8);
	// Send command lower byte
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD);
	// Restart I2C communication for reading
	i2c_master_start();
	// Send sensor address with read bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	// Read upper byte of hours value
	hours |= i2c_master_receiveChar(0x01)<<8;
	// Read lower byte of hours value
	hours |= i2c_master_receiveChar(0x01);
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	// Stop I2C communication
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Verify checksum if enabled, return 0xFFFF if invalid
	if (!check_checksum(hours, checksum))
	{
		hours = 0xffff;
	}
	#endif
	return hours;
}

// Sets the standard period for automatic self-calibration
// hours: Standard calibration period in hours
void set_automatic_self_calibration_standard_period(uint16_t hours)
{
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send command upper byte
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD>>8);
	// Send command lower byte
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD);
	// Send hours value upper byte
	i2c_master_sendChar(hours>>8);
	// Send hours value lower byte
	i2c_master_sendChar(hours);
	// Send checksum for hours value
	i2c_master_sendChar(generate_checksum(hours));
	// Stop I2C communication
	i2c_master_stop();
}

// Gets the standard period for automatic self-calibration
// Returns: Standard calibration period in hours, or 0xFFFF if checksum fails
uint16_t get_automatic_self_calibration_standard_period()
{
	// Variable to store the calibration period
	uint16_t altitude = 0;
	
	// Start I2C communication
	i2c_master_start();
	// Send sensor address with write bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	// Send command upper byte
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD>>8);
	// Send command lower byte
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD);
	// Restart I2C communication for reading
	i2c_master_start();
	// Send sensor address with read bit
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	// Read upper byte of period value
	altitude |= i2c_master_receiveChar(0x01)<<8;
	// Read lower byte of period value
	altitude |= i2c_master_receiveChar(0x01);
	// Read checksum byte
	uint8_t checksum = i2c_master_receiveChar(0x00);
	// Stop I2C communication
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Verify checksum if enabled, return 0xFFFF if invalid
	if (!check_checksum(altitude, checksum))
	{
		altitude = 0xffff;
	}
	#endif
	return altitude;
}