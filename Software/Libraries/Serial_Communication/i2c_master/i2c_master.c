/*
* i2c_master.c
*
* Created: 20.07.2024 15:39:04
* Author: Florian Reichart
*/

#include "i2c_master.h"

void i2c_master_init(uint32_t i2c_frequency)
{
	// Configure pins: set port directions and output values
	DDRC &= 0xFC;   // Sets the lower 2 bits of PORTC as inputs (SDA and SCL)
	PORTC &= 0xFC;  // Disables the pull-up resistors for SDA and SCL

	// Reset prescaler registers
	TWSR &= 0xFC;

	// Calculate prescaler and baud rate
	if (i2c_frequency <= 83333)
	{
		TWSR |= 3; // Set prescaler to 64
		TWBR = ((F_CPU/i2c_frequency) - 16) / (128); // Calculate baud rate
	}
	else if (i2c_frequency <= 250000)
	{
		TWSR |= 2; // Set prescaler to 16
		TWBR = ((F_CPU/i2c_frequency) - 16) / (32); // Calculate baud rate
	}
	else if (i2c_frequency <= 500000)
	{
		TWSR |= 1; // Set prescaler to 4
		TWBR = ((F_CPU/i2c_frequency) - 16) / (8); // Calculate baud rate
	}
	else
	{
		TWBR = ((F_CPU/i2c_frequency) - 16) / (2); // Calculate baud rate
	}

	// Enable the I2C bus
	TWCR |= (1<<TWEN);
}

void i2c_master_start()
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // Set control bits for START
	while (!(TWCR & (1<<TWINT))); // Wait until the START signal is sent
}

void i2c_master_stop()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); // Set control bits for STOP
	while (TWCR & (1<<TWSTO)); // Wait until the STOP signal is sent
}

void i2c_master_sendChar(uint8_t data)
{
	TWDR = data; // Set the data byte to send
	TWCR = (1<<TWINT) | (1<<TWEN); // Start the transmission
	while (!(TWCR & (1<<TWINT))); // Wait until the transmission is complete
}

uint8_t i2c_master_receiveChar(uint8_t ack)
{
	if (ack)
	{
		TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN); // Prepare to send ACK
	}
	else
	{
		TWCR = (1<<TWINT) | (1<<TWEN); // Prepare to send NACK
	}
	while (!(TWCR & (1<<TWINT))); // Wait until the data byte is received
	return TWDR; // Return the received data byte
}

void i2c_master_sendAddress(uint8_t address, uint8_t rw)
{
	i2c_master_sendChar((address << 1) | (rw & 0x01)); // Addressing the slave device
}
