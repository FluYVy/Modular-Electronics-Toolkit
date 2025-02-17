/*
* Libraries.c
*
* Created: 20.07.2024 15:28:45
* Author : Florian Reichart
*/

#include "i2c_master.h" // Include the I2C master library
#include <util/delay.h> // Include delay functions

#define F_CPU 12000000UL // Define the CPU frequency for delay calculations
#define TEST_DELAY 5000 // Define a delay value for testing

// Function to send test data over I2C
void send_test_data()
{
	i2c_master_start(); // Start the I2C communication
	i2c_master_sendAddress(0x55, 0x00); // Send the slave address 0x55 with write request (0x00)
	i2c_master_sendChar(0x00); // Send data byte 0x00
	i2c_master_sendChar(0xaa); // Send data byte 0xaa
	i2c_master_sendChar(0xff); // Send data byte 0xff
	i2c_master_stop(); // Stop the I2C communication
}

int main(void)
{
	_delay_ms(TEST_DELAY); // Initial delay

	// Test different I2C speeds
	i2c_master_init(80000); // Initialize I2C with 80kHz
	send_test_data(); // Send test data
	_delay_ms(TEST_DELAY); // Delay
	i2c_master_init(100000); // Initialize I2C with 100kHz
	send_test_data(); // Send test data
	_delay_ms(TEST_DELAY); // Delay
	i2c_master_init(300000); // Initialize I2C with 300kHz
	send_test_data(); // Send test data
	_delay_ms(TEST_DELAY); // Delay
	i2c_master_init(400000); // Initialize I2C with 400kHz
	send_test_data(); // Send test data
	_delay_ms(TEST_DELAY); // Delay
	
	while (1)  // Main loop
	{
		// Send data to slave device 0x63
		i2c_master_start(); // Start I2C communication
		i2c_master_sendAddress(0x63, 0); // Send slave address 0x63 with write request (0)
		i2c_master_sendChar(0xab); // Send data byte 0xab
		i2c_master_sendChar(0xcd); // Send data byte 0xcd
		i2c_master_stop(); // Stop I2C communication
		_delay_ms(500); // Delay

		// Read data from slave device 0x63
		i2c_master_start(); // Start I2C communication
		i2c_master_sendAddress(0x63, 0); // Send slave address 0x63 with write request (0)
		i2c_master_sendChar(0xaa); // Send data byte 0xaa
		i2c_master_sendChar(0x55); // Send data byte 0x55
		i2c_master_start(); // Repeated start for reading
		i2c_master_sendAddress(0x63, 1); // Send slave address 0x63 with read request (1)
		uint8_t rec_1 = i2c_master_receiveChar(1); // Receive a byte and send ACK (1)
		uint8_t rec_2 = i2c_master_receiveChar(0); // Receive a byte and send NACK (0) - last byte
		i2c_master_stop(); // Stop I2C communication
		_delay_ms(500); // Delay

		// Send received data to slave device 0x64
		i2c_master_start(); // Start I2C communication
		i2c_master_sendAddress(0x64, 0); // Send slave address 0x64 with write request (0)
		i2c_master_sendChar(rec_1); // Send the first received byte
		i2c_master_sendChar(rec_2); // Send the second received byte
		i2c_master_stop(); // Stop I2C communication
		_delay_ms(500); // Delay
	}
}