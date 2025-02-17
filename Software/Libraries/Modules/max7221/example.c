/*
 * Example_1.c
 *
 * Created: 26/01/2025 23:44:35
 * Author : flori
 */ 

#define F_CPU 12000000UL

#include <avr/io.h>
#include <avr/portpins.h>
#include <util/delay.h>
#include "max7221.h"
#include "string.h"

int main(void)
{
	DDRC |= 0x0F;  // Debug LEDs
	
	// Initialize SPI with prescaler 16
	//spi_master_init(16);
	
	// Configure MAX7221 device
	
	SpiDevice max_device = {
		.cs_ddr = &DDRC,
		.cs_port = &PORTC,
		.cs_pin = PC6
	};
	
	// Initialize device
	
	
	
	max7221_start(&max_device, 128);
	
	
	
	uint8_t i_uint8 = 142;
	int8_t i_int8 = -115;
	uint16_t i_uint16 = 735;
	int16_t i_int16 = -817;
	float i_float = 13597345895;
	
	
	
	char alphabet[] = "0123456789 -°_=./\\'\"aAbBcdefghijklmnopqrstuvwxyz";  // Full alphabet + numbers + specialchars
	int alphabet_length = strlen(alphabet);
	int start_index = 0;  // Index to track the starting letter
	
	while(1)
	{
		
		//////////////////////////////////////////////////Demo code
		max7221_set_digit(0, 2);
		max7221_set_digit_dp(1, 4);
		max7221_set_digit(2, 6);
		max7221_set_digit(3, 3);
		_delay_ms(500);
		max7221_clear_digit(2);
		_delay_ms(500);
		max7221_set_brightness(1);
		_delay_ms(500);
		max7221_set_brightness(16);
		_delay_ms(500);
		max7221_set_brightness(6);
		_delay_ms(500);
		max7221_clear_all();
		_delay_ms(500);
		
		max7221_clear_all();
		max7221_print_uint8(i_uint8, 0);
		_delay_ms(500);
		max7221_clear_all();
		max7221_print_uint8(i_uint8, 4);
		_delay_ms(500);
		max7221_clear_all();
		max7221_print_int8(i_int8, 0);
		_delay_ms(500);
		max7221_clear_all();
		max7221_print_uint16(i_uint16, 0);
		_delay_ms(500);
		max7221_clear_all();
		max7221_print_int16_length(i_int16, 0, 4);
		_delay_ms(500);
		for (int i = 0; i < 20; i++)
		{
			max7221_print_float_length(i_float, 7, 0, 20);
			i_float /= 13.47;
			_delay_ms(200);
		}
		_delay_ms(300);
		max7221_print_string("hello",0);
		_delay_ms(500);
		
		for (int i = 0; i < 30; i++)
		{
			// Create a buffer for the current display segment
			char display_buffer[9];  // 8 characters + 1 for null terminator
			for (int i = 0; i < 8; i++)
			{
				display_buffer[i] = alphabet[(start_index + i) % alphabet_length];
			}
			display_buffer[8] = '\0';  // Null-terminate the string

			max7221_print_string(display_buffer, 0);  // Display the current segment

			start_index = (start_index + 1) % alphabet_length;  // Move to the next character

			_delay_ms(200);  // Adjust delay for the shifting speed
		}
		
		max7221_set_decode_all();
		_delay_ms(500);
		max7221_clear_all();
		_delay_ms(5000);
	}
}