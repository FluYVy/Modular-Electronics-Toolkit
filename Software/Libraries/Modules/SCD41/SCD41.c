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

/**
 * @brief Get last measured CO2 value
 * @return CO2 concentration in ppm
 */
uint16_t scd41_get_co2()
{
	return last_co2;
}

/**
 * @brief Get last measured temperature
 * @return Temperature
 */
float scd41_get_temperature()
{
	return (float)(-45.0+175.0*((float)last_temperature/65535.0));
}

/**
 * @brief Get last raw measured temperature
 * @return Raw Temperature
 */
uint16_t scd41_get_temperature_raw()
{
	return last_temperature;
}

/**
 * @brief Get last measured humidity
 * @return relative humidity
 */
float scd41_get_humidity()
{
	return (float)(100.0*(float)last_humidity/65535.0);
}

/**
 * @brief Get last raw measured humidity
 * @return Raw relative humidity
 */
uint16_t scd41_get_humidity_raw()
{
	return last_humidity;
}

/**
 * @brief Start periodic measurement mode
 */
void scd41_start_periodic_measurement(){
	scd41_sequence_send_command(SCD41_COMMAND_START_PRERIODIC_MEASUREMENT);
}

/**
 * @brief Read latest measurement from sensor
 * @note Updates last_co2, last_temperature and last_humidity
 */
void scd41_read_measurement()
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
void scd41_stop_periodic_measurement(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT);
	
	// Wait 500ms for command to complete if requested
	if (wait)
	{
		_delay_ms(500);
	}
}

/**
 * @brief Generate CRC8 checksum for 16-bit value
 * @param value Value to generate checksum for
 * @return CRC8 checksum
 * @note Based on: https://stackoverflow.com/a/51773839
 */
uint8_t scd41_generate_checksum(uint16_t value)
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
uint8_t scd41_check_checksum(uint16_t value, uint8_t checksum)
{
	return (uint8_t)(generate_checksum(value) == checksum);
}

/**
 * @brief Perform single measurement of CO2, temperature and humidity
 * @param wait If 1, waits 5000ms for measurement
 */
void scd41_measure_single_shot(uint8_t wait)
{
	scd41_sequence_send_command(SCD41_COMMAND_MEASURE_SINGLE_SHOT);
	
	// If wait parameter is set, delay for 5000ms while measurement completes
	if (wait)
	{
		_delay_ms(5000);
	}
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
	i2c_master_sendChar(command>>8);
	i2c_master_sendChar(command);
	i2c_master_sendChar(data>>8);
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
	i2c_master_sendChar(command>>8);
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
	i2c_master_sendChar(command>>8);    // Upper byte
	i2c_master_sendChar(command);       // Lower byte
	
	_delay_ms(2);	
	
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