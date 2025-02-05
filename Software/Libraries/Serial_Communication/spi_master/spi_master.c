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
	DDRB |= (1<<PB4);	// Set SS (PB4) as output pin to ensure proper master functionality
	
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
	SPCR |= (1 << SPE) | ((SPI_MASTER_VALUE_DORD & 0x01) << DORD) |	(1 << MSTR) | ((SPI_MASTER_VALUE_CPOL & 0x01) << CPOL) | ((SPI_MASTER_VALUE_CPHA & 0x01) << CPHA);
}

void spi_master_device_init(SpiDevice *device)
{
	// Set the CS pin as output
	*(device->cs_ddr) |= (1 << device->cs_pin);
	
	// Deselect the device by setting the CS pin high
	*(device->cs_port) |= (1 << device->cs_pin);
}

void spi_master_transfer(SpiDevice *device, uint8_t data[], uint8_t length)
{
	// Begin SPI transaction by activating chip select
	*(device->cs_port) &= ~(1 << device->cs_pin);

	// Transfer data bytes bidirectionally
	for (uint8_t cur_byte = 0; cur_byte < length; cur_byte++)
	{
		SPDR = data[cur_byte];			// Write byte to transmit register
		while (!(SPSR & (1 << SPIF)));	// Poll until transmission complete
		data[cur_byte] = SPDR;			// Read received byte
	}
	

	// End SPI transaction by deactivating chip select
	*(device->cs_port) |= (1 << device->cs_pin);
}
