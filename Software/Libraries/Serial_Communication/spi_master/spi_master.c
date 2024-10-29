/*
 * spi_master.c
 *
 * Created: 29.10.2024 08:12:07
 *  Author: Florian Reichart
 */ 

#include "spi_master.h"

void spi_master_init(uint8_t prescaler)
{
	DDRB |= (1<<PB7)|(1<<PB5);
	DDRB &= ~(1<<PB6);
	
	SPCR &= 0x00;
	SPSR &= 0xFE;
	
	if (prescaler <= 2)
	{
		SPSR |= (1<<SPI2X);
	}
	else if(prescaler <= 4)
	{
		
	}
	else if(prescaler <= 8)
	{
		SPSR |= (1<<SPI2X);
		SPCR |= (1<<SPR0);
	}
	else if(prescaler <= 16)
	{
		SPCR |= (1<<SPR0);
	}
	else if(prescaler <= 32)
	{
		SPSR |= (1<<SPI2X);
		SPCR |= (1<<SPR1);
	}
	else if(prescaler <= 64)
	{
		SPCR |= (1<<SPR1);
	}
	else
	{
		SPCR |= (1<<SPR1)|(1<<SPR0);
	}
	
	SPCR |= (1<<SPE)|((SPI_MASTER_VALUE_DORD & 0x01)<<DORD)|(1<<MSTR)|((SPI_MASTER_VALUE_CPOL & 0x01)<<CPOL)|((SPI_MASTER_VALUE_CPHA & 0x01)<<CPHA);
}

void spi_master_device_init(SpiDevice *device)
{
	*(device->cs_ddr) |= (1 << device->cs_pin);		//Set CS pin as output
	*(device->cs_port) |= (1 << device->cs_pin);		//Deselect device
}

uint8_t spi_master_transfer(SpiDevice *device, uint8_t data)
{
	*(device->cs_port) &= ~(1 << device->cs_pin);		//Select device
	
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));	//wait for transmission to complete
	
	*(device->cs_port) |= (1 << device->cs_pin);		//Deselect device
	
	return SPDR;
}
