/*
 * spi_master.c
 *
 * Created: 29.10.2024 08:03:56
 * Author : Florian Reichart
 */ 

#include <avr/io.h>
#include "spi_master.h"
#include <util/delay.h>

#define TEST_DELAY 50000
#define F_CPU 12000000UL


int main(void)
{
	/*
	SpiDevice spiDevice1;
	spiDevice1.cs_ddr = &DDRC;
	spiDevice1.cs_port = &PORTC;
	spiDevice1.cs_pin = PC7;
	
	SpiDevice spiDevice2;
	spiDevice2.cs_ddr = &DDRC;
	spiDevice2.cs_port = &PORTC;
	spiDevice2.cs_pin = PC6;
	
	spi_master_init(8);
	
	spi_master_device_init(&spiDevice1);
	spi_master_transfer(&spiDevice1, (uint8_t[]){0x00, 0x01}, 2);
	
	spi_master_device_init(&spiDevice1);
	spi_master_transfer(&spiDevice1, (uint8_t[]){0x00, 0x01, 0x02, 0x03}, 2);
	
	spi_master_device_init(&spiDevice1);
	spi_master_transfer(&spiDevice1, (uint8_t[]){0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}, 2);
	*/
	
	SpiDevice spiDevice1;
	spiDevice1.cs_ddr = &DDRC;
	spiDevice1.cs_port = &PORTC;
	spiDevice1.cs_pin = PC7;
	
	spi_master_init(16);
	spi_master_device_init(&spiDevice1);
	spi_master_transfer(&spiDevice1, (uint8_t[]){0x00, 0x01}, 2);
	
	/*
	SpiDevice spiDevice1;
	spiDevice1.cs_ddr = &DDRC;
	spiDevice1.cs_port = &PORTC;
	spiDevice1.cs_pin = PC0;
	
	SpiDevice spiDevice2;
	spiDevice2.cs_ddr = &DDRC;
	spiDevice2.cs_port = &PORTC;
	spiDevice2.cs_pin = PC1;
	
	spi_master_device_init(&spiDevice1);
	spi_master_device_init(&spiDevice2);
	
	_delay_ms(TEST_DELAY);
	spi_master_init(2);
	spi_master_transfer(&spiDevice1, 0xaa);
	_delay_ms(TEST_DELAY);
	
	spi_master_init(4);
	spi_master_transfer(&spiDevice1, 0xaa);
	_delay_ms(TEST_DELAY);
	
	spi_master_init(8);
	spi_master_transfer(&spiDevice1, 0xaa);
	_delay_ms(TEST_DELAY);
	
	spi_master_init(16);
	spi_master_transfer(&spiDevice1, 0xaa);
	_delay_ms(TEST_DELAY);
	
	spi_master_init(32);
	spi_master_transfer(&spiDevice1, 0xaa);
	_delay_ms(TEST_DELAY);
	
	spi_master_init(64);
	spi_master_transfer(&spiDevice1, 0xaa);
	_delay_ms(TEST_DELAY);
	
	spi_master_init(128);
	spi_master_transfer(&spiDevice1, 0xaa);
	_delay_ms(TEST_DELAY);
	
	
	spi_master_transfer(&spiDevice1, 0xaa);
	uint8_t received = spi_master_transfer(&spiDevice1, 0x00);
	received++;
	spi_master_transfer(&spiDevice2, received);*/
	
    /* Replace with your application code */
	uint8_t data[] = {0xaa, 0x55, 0x00, 0xff};
    while (1) 
    {
	    spi_master_transfer(&spiDevice1, data, 4);
		_delay_ms(50);
    }
}

