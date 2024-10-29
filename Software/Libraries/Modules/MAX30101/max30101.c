/*
* max30101.c
*
* Created: 27.10.2024 09:37:54
* Author: Florian Reichart
*/

#include "max30101.h"

void max30101_start_measurements()
{
	// Set output mode
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x10); // Set output mode
	i2c_master_sendChar(0x00);
	i2c_master_sendChar(0x02); // Algorithm Data
	i2c_master_stop();
	
	// Enable the AGC
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x52); // Algorithm Mode Enable
	i2c_master_sendChar(0x00);
	i2c_master_sendChar(0x01); // Enable AGC
	i2c_master_stop();
	
	// Enable the MAX30101 sensor
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x44); // Sensor Mode Enable
	i2c_master_sendChar(0x03);
	i2c_master_sendChar(0x01); // Enable MAX30101 sensor
	i2c_master_stop();
	
	// Enable the Algorithm
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x52); // Algorithm	Mode Enable
	i2c_master_sendChar(0x02); 
	i2c_master_sendChar(0x01); // Enable algorithm
	i2c_master_stop();
}

void max30101_single_measurement(uint16_t values[], uint8_t length)
{
	uint8_t received_bytes[9]; // Buffer to store received bytes
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x12); // Read Output FIFO
	i2c_master_sendChar(0x01); // Read data stored in output FIFO
	i2c_master_stop();
	_delay_ms(40); // Wait for measurements to be ready
	
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x01); // Set to read from the data register
	i2c_master_receiveChar(0x01);
	
	// Receive measurement bytes
	for (uint8_t byte = 0; byte < 8; byte++)
	{
		received_bytes[byte] = i2c_master_receiveChar(0x01); // Read data bytes
	}
	received_bytes[8] = i2c_master_receiveChar(0x00); // Read the last byte
	i2c_master_stop();
	
	// Transform Output
	switch(length)
	{
		case 3:
		values[2] = (uint16_t)(received_bytes[7] << 8 | received_bytes[8]); // Store 3rd value
		case 2:
		values[1] = (uint16_t)(received_bytes[3] << 8 | received_bytes[4]); // Store 2nd value
		case 1:
		values[0] = (uint16_t)(received_bytes[0] << 8 | received_bytes[1]); // Store 1st value
		break;
		
		default:
		// Initialize values to zero if length is invalid
		values[0] = 0x0000;
		values[1] = 0x0000;
		values[2] = 0x0000;
		break;
	}
}

void max30101_stop_measurements()
{
	// Disable the sensor
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x44); // Sensor Mode Enable
	i2c_master_sendChar(0x03);
	i2c_master_sendChar(0x00); // Disable MAX30101 sensor
	i2c_master_stop();
	
	// Disable the Algorithm
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x52); // Algorithm	Mode Enable
	i2c_master_sendChar(0x02);
	i2c_master_sendChar(0x00); // Disable the Algorithm
	i2c_master_stop();
}
