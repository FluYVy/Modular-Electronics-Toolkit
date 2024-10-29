/*
* i2c_master.h
*
* Created: 20.07.2024 15:42:05
* Author: Florian Reichart
*/

#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#include <avr/io.h>
#include <avr/portpins.h>
#include <util/twi.h>

#ifndef F_CPU
#define F_CPU 12000000UL  // Define CPU frequency if not already defined
#endif

/**
* @brief Initializes the I2C bus as a master.
*
* This function sets up the I2C pins, configures the prescaler, and calculates
* the baud rate based on the desired I2C frequency. It also enables the I2C bus.
*
* @param[in] i2c_frequency The desired I2C frequency in Hertz.
*/
void i2c_master_init(uint32_t i2c_frequency);

/**
* @brief Sends a START signal on the I2C bus.
*
* This function initiates a new I2C communication cycle by sending
* a START signal to begin communication with a slave device.
*/
void i2c_master_start();

/**
* @brief Sends a STOP signal on the I2C bus.
*
* This function ends the current I2C communication by sending a STOP signal.
*/
void i2c_master_stop();

/**
* @brief Sends a data byte over the I2C bus.
*
* This function writes a data byte to the data register and waits
* for the transmission to complete before returning.
*
* @param[in] data The data byte to be sent.
*/
void i2c_master_sendChar(uint8_t data);

/**
* @brief Receives a data byte from the I2C bus.
*
* This function receives a data byte from the I2C bus and sends either
* an ACK or NACK back depending on the value of ack.
*
* @param[in] ack If 1, an ACK (acknowledgment) is sent; if 0, a NACK is sent.
* @return The received data byte.
*/
uint8_t i2c_master_receiveChar(uint8_t ack);

/**
* @brief Sends an I2C address and the read/write command.
*
* This function sends the address of a slave device along with
* the read or write command (0 for write, 1 for read).
*
* @param[in] address The 7-bit address of the I2C slave device.
* @param[in] rw The read/write mode: 0 for write, 1 for read.
*/
void i2c_master_sendAddress(uint8_t address, uint8_t rw);

#endif /* I2C_MASTER_H_ */
