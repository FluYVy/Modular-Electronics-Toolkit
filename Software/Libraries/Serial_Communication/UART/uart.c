/*
 * uart.c
 * UART driver implementation for AVR microcontrollers
 *
 * Created: 30.10.2024 17:16:12
 * Author: Florian Reichart
 */ 

#include "uart.h"

volatile uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
volatile uint8_t rx_index = 0;

// Interrupt handler for UART receive complete
ISR(USART_RXC_vect) {
    if (rx_index < UART_RX_BUFFER_SIZE) {
        rx_buffer[rx_index++] = UDR;
    } else {
        // Buffer full - discard byte by reading UDR
        uint8_t dummy = UDR;
    }
}

/**
 * Initializes the UART interface with the specified baud rate
 * @param baudrate The desired baud rate in bits per second
 */
void uart_init(uint32_t baudrate)
{
    // Calculate UBRR register value based on CPU frequency and desired baud rate
    uint16_t ubrr = (F_CPU / 16 / baudrate) - 1;
    UBRRH = (uint8_t)(ubrr>>8);  // Set high byte of UBRR
    UBRRL = (uint8_t)ubrr;       // Set low byte of UBRR

    // Enable receiver, transmitter and receive interrupt
    UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
    // Configure frame format: 8 data bits, 1 stop bit
    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
    
    // Enable global interrupts
    sei();
}

/**
 * Sends a single byte via UART
 * @param data The byte to send
 */
void uart_send_byte(uint8_t data)
{
    // Wait until transmit buffer is empty
    while(!(UCSRA & (1<<UDRE)));
    // Load data into transmit register
    UDR = data;
}

/**
 * Checks if there is data available in the receive buffer
 * @return 1 if data is available, 0 otherwise
 */
uint8_t uart_data_available()
{
    return rx_index > 0;
}

/**
 * Reads a single byte from the receive buffer
 * @return The next byte from the buffer, or 0 if buffer is empty
 */
uint8_t uart_read_byte()
{
    if (rx_index == 0) {
        return 0;
    }

    return rx_buffer[--rx_index];
}

/**
 * Sends a null-terminated string via UART
 * @param str Pointer to the string to send
 */
void uart_send_string(uint8_t *str)
{
    // Send each character until null terminator is reached
    while(*str)
    {
        uart_send_byte(*str++);
    }
}

/**
 * Reads available bytes from the receive buffer into a string
 * @param str Buffer to store received string
 * @param max_length Maximum number of bytes to read
 * @return Number of bytes actually read
 */
uint8_t uart_read_string(uint8_t *str, uint8_t max_length)
{
    uint8_t bytes_read = 0;
    
    // Read up to max_length bytes or until buffer is empty
    while (bytes_read < max_length && uart_data_available()) {
        str[bytes_read++] = uart_read_byte();
    }
    
    // Add null terminator if there's room
    if (bytes_read < max_length) {
        str[bytes_read] = '\0';
    }
    
    return bytes_read;
}

/**
 * Clears the receive buffer
 */
void uart_flush_buffer()
{
    rx_index = 0;
}