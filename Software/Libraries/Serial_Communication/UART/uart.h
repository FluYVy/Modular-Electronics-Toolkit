/*
 * uart.h
 * UART driver header file for AVR microcontrollers
 *
 * Created: 30.10.2024 17:16:20
 *  Author: Florian Reichart
 */ 

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>


// Size of the receive buffer
#define UART_RX_BUFFER_SIZE 64

/**
 * Initializes the UART interface with the specified baud rate
 * @param baudrate The desired baud rate in bits per second
 */
void uart_init(uint32_t baudrate);

/**
 * Sends a single byte via UART
 * @param data The byte to send
 */
void uart_send_byte(uint8_t data);

/**
 * Checks if there is data available in the receive buffer
 * @return 1 if data is available, 0 otherwise
 */
uint8_t uart_data_available();

/**
 * Reads a single byte from the receive buffer
 * @return The next byte from the buffer, or 0 if buffer is empty
 */
uint8_t uart_read_byte();

/**
 * Sends a null-terminated string via UART
 * @param str Pointer to the string to send
 */
void uart_send_string(uint8_t *str);

/**
 * Reads available bytes from the receive buffer into a string
 * @param str Buffer to store received string
 * @param max_length Maximum number of bytes to read
 * @return Number of bytes actually read
 */
uint8_t uart_read_string(uint8_t *str, uint8_t max_length);

/**
 * Clears the receive buffer
 */
void uart_flush_buffer();

#endif /* UART_H_ */