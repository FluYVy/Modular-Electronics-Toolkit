/*
 * ws2812b.c
 * Driver for WS2812B RGB LED strips using timer-based precise timing
 *
 * Created: 21.10.2024 16:18:57
 * Author: Florian Reichart
 */ 

#include "ws2812b.h"

#define F_CPU 12000000UL  // CPU frequency in Hz

// Counter for sub-bit timing in the WS2812B protocol
volatile uint8_t cur_bit_sub_count = 0;

// Timer0 Compare Match interrupt handler - triggers at 2.5MHz for precise bit timing
ISR(TIMER0_COMP_vect)
{
	cur_bit_sub_count++;
}

// Initialize Timer0 for WS2812B timing
void ws2812b_init()
{
	WS2812B_DDR |= (1<<WS2812B_PIN);	// Set WS2812B_PIN as output
	WS2812B_PORT &= ~(1<<WS2812B_PIN);	// Set WS2812B_PIN low

	TCCR0 = 0x00;	// Reset timer configuration
	TCCR0 |= (1<<WGM01); // Set CTC (Clear Timer on Compare Match) mode
	TCCR0 &= ~((1<<CS00)|(1<<CS01)|(1<<CS02));	// Stop timer by clearing clock source
	
	OCR0 = 0x04;  // Set compare value for 2.5MHz interrupt frequency
	TIMSK |= (1<<OCIE0);  // Enable Timer0 Compare Match interrupt
}

// Send RGB values to WS2812B LED strip
// values_rgb[]: Array of RGB values (3 bytes per LED)
// length: Number of bytes to send
void ws2812b_send(uint8_t values_rgb[], uint16_t length)
{
	TCCR0 |= (1<<CS00); // Start timer with no prescaling
	for (uint16_t i = 0; i < length; i++)
	{
		// Send each bit with precise timing
		for(uint8_t b = 0; b < 8; b++)
		{
			WS2812B_PORT |= (1<<WS2812B_PIN);  // Set data pin high
			uint8_t cur_bit = (values_rgb[i]>>b)&0x01;  // Get current bit
			if (cur_bit)
			{
				while(cur_bit_sub_count < 2);  // High time for '1' bit (800ns)
			}else
			{
				while(cur_bit_sub_count < 1);  // High time for '0' bit (400ns)
			}
			WS2812B_PORT &= ~(1<<WS2812B_PIN);  // Set data pin low
			while(cur_bit_sub_count < 3);  // Wait for total bit time (1.25µs)
			cur_bit_sub_count = 0;  // Reset sub-bit counter
		}
	}
	
	TCCR0 &= ~(1<<CS00);	// Stop timer
	TCNT0 = 0;  // Reset timer counter
	_delay_us(50);  // Reset time between transmissions (>50µs)
}