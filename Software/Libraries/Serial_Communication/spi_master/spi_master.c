/*
* spi_master.c
*
* Created: 29.10.2024 08:12:07
* Author: Florian Reichart
*/

#include "spi_master.h"

void spi_master_init(uint8_t prescaler)
{
	// Set MOSI (PB5) and SCK (PB7) as output pins
	// Set MISO (PB6) as input pin
	DDRB |= (1 << PB7) | (1 << PB5);
	DDRB &= ~(1 << PB6);
	
	// Reset the SPI control register and status register
	SPCR &= 0x00;
	SPSR &= 0xFE;
	
	// Set the prescaler based on the provided value
	if (prescaler <= 2)
	{
		SPSR |= (1 << SPI2X); // Set double speed mode
	}
	else if(prescaler <= 4)
	{
		// No action needed, prescaler remains at 4
	}
	else if(prescaler <= 8)
	{
		SPSR |= (1 << SPI2X); // Set double speed mode
		SPCR |= (1 << SPR0);  // Set prescaler to 8
	}
	else if(prescaler <= 16)
	{
		SPCR |= (1 << SPR0);  // Set prescaler to 16
	}
	else if(prescaler <= 32)
	{
		SPSR |= (1 << SPI2X);  // Set double speed mode
		SPCR |= (1 << SPR1);   // Set prescaler to 32
	}
	else if(prescaler <= 64)
	{
		SPCR |= (1 << SPR1);   // Set prescaler to 64
	}
	else
	{
		SPCR |= (1 << SPR1) | (1 << SPR0); // Set prescaler to 128
	}
	
	// Enable SPI, set data order, set master mode, set clock polarity and phase
	SPCR |= (1 << SPE) |
	((SPI_MASTER_VALUE_DORD & 0x01) << DORD) |
	(1 << MSTR) |
	((SPI_MASTER_VALUE_CPOL & 0x01) << CPOL) |
	((SPI_MASTER_VALUE_CPHA & 0x01) << CPHA);
}

void spi_master_device_init(SpiDevice *device)
{
	// Set the CS pin as output
	*(device->cs_ddr) |= (1 << device->cs_pin);
	
	// Deselect the device by setting the CS pin high
	*(device->cs_port) |= (1 << device->cs_pin);
}

uint8_t spi_master_transfer(SpiDevice *device, uint8_t data)
{
	// Select the device by pulling the CS pin low
	*(device->cs_port) &= ~(1 << device->cs_pin);

	// Load the data into the SPI data register to start the transfer
	SPDR = data;
	
	// Wait for the transmission to complete
	while (!(SPSR & (1 << SPIF)));

	// Deselect the device by setting the CS pin high
	*(device->cs_port) |= (1 << device->cs_pin);

	// Return the received data from the SPI data register
	return SPDR;
}
