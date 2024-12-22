/*
* spi_master.h
*
* Created: 29.10.2024 08:12:29
* Author: Florian Reichart
*/

#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_

#include <avr/io.h>
#include <avr/portpins.h>

// SPI Master configuration values
#define SPI_MASTER_VALUE_DORD  1  /**< Data order: 1 = LSB first, 0 = MSB first */
#define SPI_MASTER_VALUE_CPOL  0  /**< Clock polarity: 0 = idle low, 1 = idle high */
#define SPI_MASTER_VALUE_CPHA  0  /**< Clock phase: 0 = leading edge, 1 = trailing edge */

/**
* @brief Structure to represent an SPI device.
*
* This structure contains the necessary information to control a specific
* SPI device, including its chip select (CS) pin configuration.
*/
typedef struct {
	volatile uint8_t *cs_ddr;  /**< Pointer to the data direction register for the CS pin */
	volatile uint8_t *cs_port;  /**< Pointer to the port register for the CS pin */
	uint8_t cs_pin;             /**< The specific pin number for the CS signal */
} SpiDevice;

/**
* @brief Initializes the SPI master interface.
*
* This function configures the SPI settings, including the clock prescaler,
* and sets up the necessary pins for SPI communication.
*
* @param[in] prescaler The prescaler value used to set the SPI clock speed.
*/
void spi_master_init(uint8_t prescaler);

/**
* @brief Initializes the given SPI device.
*
* This function configures the chip select (CS) pin for the specified SPI device
* and prepares it for communication.
*
* @param[in] device A pointer to the SpiDevice structure that represents the SPI device to initialize.
*/
void spi_master_device_init(SpiDevice *device);

/**
* @brief Transfers data via SPI.
*
* This function sends multiple bytes of data to the specified SPI device and receives
* data in return. It handles the necessary chip select (CS) signaling. The received
* data overwrites the input data array.
*
* @param[in] device A pointer to the SpiDevice structure representing the target SPI device.
* @param[in,out] data Array of bytes to send. Received data will be stored back in this array.
* @param[in] length Number of bytes to transfer.
*/
void spi_master_transfer(SpiDevice *device, uint8_t data[], uint8_t length);

#endif /* SPI_MASTER_H_ */
