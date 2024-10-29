/*
 * spi_master.h
 *
 * Created: 29.10.2024 08:12:29
 *  Author: Florian Reichart
 */ 


#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_

#include <avr/io.h>
#include <avr/portpins.h>

#define SPI_MASTER_VALUE_DORD	1
#define SPI_MASTER_VALUE_CPOL	0
#define SPI_MASTER_VALUE_CPHA	0


typedef struct {
	volatile uint8_t *cs_ddr;
	volatile uint8_t *cs_port;
	uint8_t cs_pin;
} SpiDevice;

void spi_master_init(uint8_t prescaler);
void spi_master_device_init(SpiDevice *device);
uint8_t spi_master_transfer(SpiDevice *device, uint8_t data);


#endif /* SPI_MASTER_H_ */