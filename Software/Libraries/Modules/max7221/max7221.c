/*
 * max7221.c
 *
 * Created: 22.12.2024 15:02:28
 *  Author: Florian Reichart
 */ 

#define F_CPU 12000000UL
#include "max7221.h"
#include <util/delay.h>
#include <math.h>se
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>	// for case sensitivity control

SpiDevice *max7221_device;
uint8_t device_set = 0;
uint8_t digit_set = 0x00;
bool decode_mode[8] = { false };

#define CS_ACTIVE (*(max7221_device->cs_port) &= ~(1 << max7221_device->cs_pin))
#define CS_INACTIVE (*(max7221_device->cs_port) |= (1 << max7221_device->cs_pin))


void max7221_init(uint8_t prescaler)
{
	// Initialize Ports
	DDRB |= (1 << PB4) | (1 << PB7) | (1 << PB5);	// Set PB4 (SS), PB7 (SCK) and PB5 (MOSI) as outputs
	DDRB &= ~(1 << PB6);	// Set PB6 (MISO) as input
	
	PORTB |= (1 << PB4);	// Set PB4 high for SlaveSelect = Master
	*(max7221_device->cs_port) |= (1 << max7221_device->cs_pin);  // Set PC6 high initially
	*(max7221_device->cs_ddr) |= (1 << max7221_device->cs_pin);
	
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
	SPCR |= (1 << SPE) | (1 << MSTR);
}

void max7221_transfer(uint16_t data)
{
	// Begin SPI transaction by activating chip select
	CS_ACTIVE;

	// Transfer data bytes
	SPDR = (uint8_t)((data >> 8) & 0xFF);  // Send high byte first
	while (!(SPSR & (1 << SPIF)));
	SPDR = (uint8_t)(data & 0xFF); // Send low byte
	while (!(SPSR & (1 << SPIF)));
	
	// End SPI transaction by deactivating chip select
	CS_INACTIVE;
}

void max7221_start(SpiDevice *device, uint8_t prescaler)
{
	max7221_device = device;
	device_set = 1;
	max7221_init(prescaler);
	max7221_transfer((uint16_t)((MAX7221_REG_SHUTDOWN << 8) | MAX7221_SHUTDOWN_MODE));			// shutdown for setting parameters
	max7221_transfer((uint16_t)((MAX7221_REG_SCAN_LIMIT << 8) | MAX7221_SCAN_LIMIT_7));			// use all 8 segments 0-7
	max7221_transfer((uint16_t)((MAX7221_REG_DECODE_MODE << 8) | MAX7221_DECODE_ALL));			// set numbers instead of segments
	for (int i = 0; i < 8; i++)
	{
		decode_mode[i] = true;
	}
	max7221_transfer((uint16_t)((MAX7221_REG_DISPLAY_TEST << 8) | MAX7221_DISPLAY_TEST_OFF));	// exit display test mode
	max7221_transfer((uint16_t)((MAX7221_REG_SHUTDOWN << 8) | MAX7221_NORMAL_OPERATION));		// no shutdown
	max7221_set_brightness(6);
	max7221_clear_all();
}





void max7221_stop()
{
	if (device_set)
	{
		max7221_transfer((uint16_t)((MAX7221_REG_SHUTDOWN << 8) | MAX7221_SHUTDOWN_MODE));
		device_set = 0;
	}
}

void max7221_set_digit(uint8_t digit, uint8_t value)
{
	if (device_set)
	{
		digit_set = map_digit(digit);
		max7221_transfer((uint16_t)(((digit_set & 0xFF) << 8) | (value & 0x0F)));
	}
}


void max7221_set_digit_dp(uint8_t digit, uint8_t value)
{
	if (device_set)
	{
		digit_set = map_digit(digit);
		max7221_transfer((uint16_t)(0x80 | ((digit_set & 0xFF) << 8) | (value & 0x0F)));
	}
}


void max7221_clear_digit(uint8_t digit)
{
	if (device_set)
	{
		digit_set = map_digit(digit);
		max7221_transfer((uint16_t)(((digit_set & 0xFF) << 8) | MAX7221_DISPLAY_BLANK)); // Clear each digit
	}
}


void max7221_clear_all()
{
	if (device_set)
	{
		for (uint8_t i = 1; i <= 8; i++) {
			max7221_transfer((uint16_t)(((i & 0xFF) << 8) | MAX7221_DISPLAY_BLANK)); // Clear each digit
		}
	}
}

void max7221_print_uint8_length(uint8_t value, uint8_t digit, uint8_t length)
{
	if (device_set)
	{
		uint8_t del_digit = 0;
		
		// maximum length for 8bit = 255 = 3
		if (length > 3)
		{
			length = 3;
		}
		// minimum length = 1
		else if (length < 1)
		{
			length = 1;
		}
		
		while (value < (int)pow(10, length-1) && length >= 2)
		{
			length--;
			del_digit += 1;
		}
		
		// new rightmost digit - 1 (because first digit is 0 and not 1)
		digit = digit + length - 1;
		
		// if rightmost digits need to be deleted
		digit += del_digit;
		while (del_digit >= 1)
		{
			digit_set = map_digit(digit);
			
			// delete digit
			max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_BLANK));
			del_digit -= 1;
			digit -= 1;
		}
		
		for (uint8_t i = 0; i < length; i++)
		{
			digit_set = map_digit(digit);
			
			max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
			value = value / 10;
			if (digit > 0)
			{
				digit--;
			}
		}
	}
}

void max7221_print_uint8(uint8_t value, uint8_t digit)
{
	max7221_print_uint8_length(value, digit, 3);
}

void max7221_print_uint8_default(uint8_t value) {
	max7221_print_uint8_length(value, 0, 3);
}

void max7221_print_int8_length(int8_t value, uint8_t digit, uint8_t length)
{
	if (device_set)
	{
		uint8_t negative = 0;
		uint8_t del_digit = 0;
		
		// maximum length for 8bit = 127 = (3 + hyphen) = 4
		if (length > 4)
		{
			length = 4;
		}
		// minimum length = 1
		else if (length < 1)
		{
			length = 1;
		}
		
		// if negative value, convert to positive value and set negative = 1
		if (value < 0)
		{
			value = -value;
			negative = 1;
			
			// check if the number displayed needs all defined digits and if not, delete rightmost digits (using del_digit += 1)
			while (value < (int)pow(10, (length-2)) && length >= 3)
			{
				length--;
				del_digit += 1;
			}
		}
		// if positive value
		else
		{
			// check if the number displayed needs all defined digits and if not, delete rightmost digits (using del_digit += 1)
			while (value < (int)pow(10, length-1) && length >= 2)
			{
				length--;
				del_digit += 1;
			}
		}
		
		// new rightmost digit - 1 (because first digit is 0 and not 1)
		digit = digit + length - 1;
		
		// if rightmost digits need to be deleted
		digit += del_digit;
		while (del_digit >= 1)
		{
			digit_set = map_digit(digit);
			
			// delete digit
			max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_BLANK));
			del_digit -= 1;
			digit -= 1;
			
		}
		
		for (uint8_t i = 0; i < length; i++)
		{
			digit_set = map_digit(digit);
			
			// if not at leftmost digit
			if (i < length - 1)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
				value = value / 10;
				digit--;
			}
			// for negative number display hyphen
			else if (negative == 1)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_HYPHEN));
				negative = 0;
			}
			// for positive number display number instead of hyphen
			else
			{
				max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
			}
		}
	}
}

void max7221_print_int8(int8_t value, uint8_t digit)
{
	max7221_print_int8_length(value, digit, 4);
}

void max7221_print_int8_default(int8_t value)
{
	max7221_print_int8_length(value, 0, 4);
}

void max7221_print_uint16_length(uint16_t value, uint8_t digit, uint8_t length)
{
	if (device_set)
	{
		uint8_t del_digit = 0;
		
		// maximum length for 16bit = 65535 = 5
		if (length > 5)
		{
			length = 5;
		}
		// minimum length = 1
		else if (length < 1)
		{
			length = 1;
		}
		
		while (value < (int)pow(10, length-1) && length >= 2)
		{
			length--;
			del_digit += 1;
		}
		
		// new rightmost digit - 1 (because first digit is 0 and not 1)
		digit = digit + length - 1;
		
		// if rightmost digits need to be deleted
		digit += del_digit;
		while (del_digit >= 1)
		{
			digit_set = map_digit(digit);
			
			// delete digit
			max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_BLANK));
			del_digit -= 1;
			digit -= 1;
		}
		
		for (uint8_t i = 0; i < length; i++)
		{
			digit_set = map_digit(digit);
			
			max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
			value = value / 10;
			if (digit > 0)
			{
				digit--;
			}
		}
	}
}

void max7221_print_uint16(uint16_t value, uint8_t digit)
{
	max7221_print_uint16_length(value, digit, 5);
}

void max7221_print_uint16_default(uint16_t value)
{
	max7221_print_uint16_length(value, 0, 5);
}

void max7221_print_int16_length(int16_t value, uint8_t digit, uint8_t length)
{
	// check if code should be run
    if (device_set)
	{
		uint8_t negative = 0;
		uint8_t del_digit = 0;
		
		
		
		// maximum length for 16bit = 32767 = (5 + hyphen) = 6
		if (length > 6)
		{
			length = 6;
		}
		// minimum length = 1
		else if (length < 1)
		{
			length = 1;
		}
		
		// if negative value, convert to positive value and set negative = 1
		if (value < 0)
		{
			value = -value;
			negative = 1;
			
			// check if the number displayed needs all defined digits and if not, delete rightmost digits (using del_digit += 1)
			while (value < (int)pow(10, (length-2)) && length >= 3)
			{
				length--;
				del_digit += 1;
			}
		}
		// if positive value
		else
		{
			// check if the number displayed needs all defined digits and if not, delete rightmost digits (using del_digit += 1)
			// Checking for 10^5 would exceed the maixmum int_16 length
			if (length == 6)
			{
				length--;
			}
			while (value < (int)pow(10, length-1) && length >= 2)
			{
				length--;
				del_digit += 1;
			}
		}
		
		// new rightmost digit - 1 (because first digit is 0 and not 1)
		digit = digit + length - 1;
		
		
		
		// if rightmost digits need to be deleted
		digit += del_digit;
		while (del_digit >= 1)
		{
			digit_set = map_digit(digit);
			
			// delete digit
			max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_BLANK));
			del_digit -= 1;
			digit -= 1;
		}
		
		
		
		// for every digit the number has to be displayed on
		for (uint8_t i = 0; i < (length); i++)
		{
			
			digit_set = map_digit(digit);
			
			
			// if not at leftmost digit
			if (i < length - 1)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
				value = value / 10;
				digit--;
			}
			// for negative number display hyphen
			else if (negative == 1)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_HYPHEN));
				negative = 0;
			}
			// for positive number display number instead of hyphen
			else
			{
				max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
			}
		}
    }
}



void max7221_print_int16_default(int16_t value) {
	max7221_print_int16_length(value, 0, 5);
}

void max7221_print_int16(int16_t value, uint8_t digit)
{
	max7221_print_int16_length(value, digit, 5);
}


void max7221_print_float_length(float value, int8_t decimal, uint8_t digit, uint8_t length)
{
    if (device_set)
	{
        uint8_t negative = 0;
		uint8_t numtype = 0;
		uint8_t numlength = 0;

        if (value < 0)
        {
            negative = 1;
            value = -value;
        }
		
		while (value >= 10)
		{
			value /= 10.0f;
			decimal += 1;
		}
		while (value < 1 && value != 0)
		{
			value *= 10.0f;
			decimal -= 1;
		}
		// number between 1-9.99999 (e.g. 3.5256)
		
		// maximum length of 7-segment display = (7 + hyphen) = 8
		if (length > 8)
		{
			length = 8;
		}
		// minimum length = 3
		else if (length < 3)
		{
			length = 3;
		}
		
		// new rightmost digit
		digit = digit + length - 1;
		
		if (decimal >= 10 && (((length >= 5) && negative) || ((length >= 4) && !negative)))
		{
			//length -= 3;
			numtype = 1;
			numlength = 3;
		}
		else if (decimal > 0 && (((length >= 4) && negative) || ((length >= 3) && !negative)))
		{
			//length -= 2;
			numtype = 2;
			numlength = 2;
		}
		else if (decimal <= -10 && (((length >= 6) && negative) || ((length >= 5) && !negative)))
		{
			//length -= 4;
			numtype = 3;
			numlength = 4;
			decimal = -decimal;
		}
		else if (decimal < 0 && (((length >= 5) && negative) || ((length >= 4) && !negative)))
		{
			//length -= 3;
			numtype = 4;
			numlength = 3;
			decimal = -decimal;
		}
		else if (decimal == 0 && (((length >= 4) && negative) || ((length >= 3) && !negative)))
		{
			// for displaying E0
			//length -= 2;
			numtype = 5;
			numlength = 2;
		}
		else
		{
			// display "ERR"
			digit = digit - length + 1 + 2;
			length = 3;
			numtype = 6;
			numlength = 3;
		}
		
		// for every digit the number has to be displayed on
		for (uint8_t i = 0; i < length; i++)
		{
			digit_set = map_digit(digit);
			
			if (i < numlength)
			{
				switch (numtype)
				{
					case 0:
						// display "ERR" for error
						if (i == 0)
						{
							if (decode_mode[digit] == true)
							{
								max7221_set_decode_none();
								// display R
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01000110));
							}
							else
							{
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01000110));
							}
							
						}
						else if (i == 1)
						{
							if (decode_mode[digit] == true)
							{
								max7221_set_decode_none();
								// display R
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01000110));
							}
							else
							{
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01000110));
							}
							
						}
						else
						{
							if (decode_mode[digit] == true)
							{
								max7221_set_decode_none();
								// display E
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01001111));
							}
							else
							{
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01001111));
							}
							_delay_ms(1000);
							max7221_set_decode_all();
						}
						break;
					case 1:
						if (i <= 1)
						{
							max7221_transfer((uint16_t)((digit_set << 8) | ((decimal % 10) & 0x0F)));
							decimal /= 10;
						}
						else
						{
							max7221_transfer((uint16_t)((digit_set << 8) | (MAX7221_DISPLAY_E)));
						}
						break;
					case 2:
						if (i < 1)
						{
							max7221_transfer((uint16_t)((digit_set << 8) | ((decimal % 10) & 0x0F)));
							decimal /= 10;
						}
						else
						{
							max7221_transfer((uint16_t)((digit_set << 8) | (MAX7221_DISPLAY_E)));
						}
						break;
					case 3:
						if (i <= 1)
						{
							max7221_transfer((uint16_t)((digit_set << 8) | ((decimal % 10) & 0x0F)));
							decimal /= 10;
						}
						else if (i == 2)
						{
							max7221_transfer((uint16_t)((digit_set << 8) | (MAX7221_DISPLAY_HYPHEN)));
						}
						else
						{
							max7221_transfer((uint16_t)((digit_set << 8) | (MAX7221_DISPLAY_E)));
						}
						break;
					case 4:
						if (i < 1)
						{
							max7221_transfer((uint16_t)((digit_set << 8) | ((decimal % 10) & 0x0F)));
							decimal /= 10;
						}
						else if (i == 1)
						{
							max7221_transfer((uint16_t)((digit_set << 8) | (MAX7221_DISPLAY_HYPHEN)));
						}
						else
						{
							max7221_transfer((uint16_t)((digit_set << 8) | (MAX7221_DISPLAY_E)));
						}
						break;
					case 5:
						if (i < 1)
						{
							max7221_transfer((uint16_t)((digit_set << 8) | 0x00));
						}
						else
						{
							max7221_transfer((uint16_t)((digit_set << 8) | (MAX7221_DISPLAY_E)));
						}
						break;
					case 6:
						// display "ERR" for error
						if (i == 0)
						{
							if (decode_mode[digit] == true)
							{
								max7221_set_decode_none();
								// display R
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01000110));
							}
							else
							{
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01000110));
							}
							
						}
						else if (i == 1)
						{
							if (decode_mode[digit] == true)
							{
								max7221_set_decode_none();
								// display R
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01000110));
							}
							else
							{
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01000110));
							}
							
						}
						else
						{
							if (decode_mode[digit] == true)
							{
								max7221_set_decode_none();
								// display E
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01001111));
							}
							else
							{
								max7221_transfer((uint16_t)((digit_set << 8) | 0b01001111));
							}
							_delay_ms(1000);
							max7221_set_decode_all();
						}
						
						
						break;
					default:
						break;
				}
			}
			// convert to int8_t so the value doesn't go to 255 if lower than 0
			else if ((int8_t)(i) < (int8_t)(length) - 5)
			{
				if (negative == 1)
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*10000.0f)) % 10) & 0x0F)));
				}
				else
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*100000.0f)) % 10) & 0x0F)));
				}
			}
			else if ((int8_t)(i) < (int8_t)(length) - 4)
			{
				if (negative == 1)
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*1000.0f)) % 10) & 0x0F)));
				}
				else
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*10000.0f)) % 10) & 0x0F)));
				}
			}
			else if ((int8_t)(i) < (int8_t)(length) - 3)
			{
				if (negative == 1)
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*100.0f)) % 10) & 0x0F)));
				}
				else
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*1000.0f)) % 10) & 0x0F)));
				}
			}
			else if ((int8_t)(i) < (int8_t)(length) - 2)
			{
				if (negative == 1)
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*10.0f)) % 10) & 0x0F)));
				}
				else
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*100.0f)) % 10) & 0x0F)));
				}
			}
			else if ((int8_t)(i) < (int8_t)(length) - 1)
			{
				if (negative == 1)
				{
					max7221_set_digit_dp(digit, (((uint32_t)(value) % 10) & 0x0F));
				}
				else
				{
					max7221_transfer((uint16_t)((digit_set << 8) | ((((uint32_t)(value*10.0f)) % 10) & 0x0F)));
				}
			}
			// for negative number display hyphen
			else if (negative == 1)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_HYPHEN));
				negative = 0;
			}
			// for positive number display number instead of hyphen
			else
			{
				max7221_set_digit_dp(digit, (((uint32_t)(value) % 10) & 0x0F));
			}
			if (digit > 0)
			{
				digit--;
			}
			
		}
    }
}

void max7221_print_float(float value, uint8_t decimal)
{
	max7221_print_float_length(value, decimal, 0, 8);
}

void max7221_print_float_default(float value)
{
	max7221_print_float_length(value, 0, 0, 8);
}


void max7221_set_brightness(uint8_t brightness)
{
	// Clamp brightness to 1-16 range
	if (brightness < 1) {
		brightness = 1;
	}
	else if (brightness > 16) {
		brightness = 16;
	}
	
	// Convert 1-16 range to 0-15 range
	uint8_t intensity = brightness - 1;
	
	max7221_transfer((uint16_t)((MAX7221_REG_INTENSITY << 8) | (intensity & 0x0F)));
}



void max7221_set_decode_none()
{
	max7221_transfer((uint16_t)((MAX7221_REG_DECODE_MODE << 8) | MAX7221_NO_DECODE));
	for (int i = 0; i < 8; i++) {
		decode_mode[i] = false;
	}

	
	for (int i = 1; i <= 8; i++)
	{
		max7221_transfer((uint16_t)((i << 8) | 0x00));
	}
}

void max7221_set_decode_all()
{
	max7221_transfer((uint16_t)((MAX7221_REG_DECODE_MODE << 8) | MAX7221_DECODE_ALL));
	for (int i = 0; i < 8; i++)
	{
		decode_mode[i] = true;
	}
	
	for (int i = 1; i <= 8; i++)
	{
		max7221_transfer((uint16_t)((i << 8) | MAX7221_DISPLAY_BLANK));
	}
}

void max7221_print_string(const char *value, uint8_t digit)
{
	int length = strlen(value);
	max7221_set_decode_none();
	
	for (int i = 0; i < length; i++)
	{
		uint8_t segment_value = MAX7221_DISPLAY_BLANK;  // Default to blank if not found
		char c = tolower(value[i]);  // Convert character to lowercase
		switch (c)
		{
			// Letters
			case 'a': segment_value = MAX7221_STRING_a; break;
			case 'b': segment_value = MAX7221_STRING_b; break;
			case 'c': segment_value = MAX7221_STRING_c; break;
			case 'd': segment_value = MAX7221_STRING_d; break;
			case 'e': segment_value = MAX7221_STRING_e; break;
			case 'f': segment_value = MAX7221_STRING_f; break;
			case 'g': segment_value = MAX7221_STRING_g; break;
			case 'h': segment_value = MAX7221_STRING_h; break;
			case 'i': segment_value = MAX7221_STRING_i; break;
			case 'j': segment_value = MAX7221_STRING_j; break;
			case 'k': segment_value = MAX7221_STRING_k; break;
			case 'l': segment_value = MAX7221_STRING_l; break;
			case 'm': segment_value = MAX7221_STRING_m; break;
			case 'n': segment_value = MAX7221_STRING_n; break;
			case 'o': segment_value = MAX7221_STRING_o; break;
			case 'p': segment_value = MAX7221_STRING_p; break;
			case 'q': segment_value = MAX7221_STRING_q; break;
			case 'r': segment_value = MAX7221_STRING_r; break;
			case 's': segment_value = MAX7221_STRING_s; break;
			case 't': segment_value = MAX7221_STRING_t; break;
			case 'u': segment_value = MAX7221_STRING_u; break;
			case 'v': segment_value = MAX7221_STRING_v; break;
			case 'w': segment_value = MAX7221_STRING_w; break;
			case 'x': segment_value = MAX7221_STRING_x; break;
			case 'y': segment_value = MAX7221_STRING_y; break;
			case 'z': segment_value = MAX7221_STRING_z; break;
			// Numbers
			case '0': segment_value = MAX7221_STRING_0; break;
			case '1': segment_value = MAX7221_STRING_1; break;
			case '2': segment_value = MAX7221_STRING_2; break;
			case '3': segment_value = MAX7221_STRING_3; break;
			case '4': segment_value = MAX7221_STRING_4; break;
			case '5': segment_value = MAX7221_STRING_5; break;
			case '6': segment_value = MAX7221_STRING_6; break;
			case '7': segment_value = MAX7221_STRING_7; break;
			case '8': segment_value = MAX7221_STRING_8; break;
			case '9': segment_value = MAX7221_STRING_9; break;
			// Special Characters
			case ' ': segment_value = MAX7221_STRING_SPACE; break;
			case '-': segment_value = MAX7221_STRING_HYPHEN; break;
			case '°': segment_value = MAX7221_STRING_DEGREE; break;
			case '_': segment_value = MAX7221_STRING_UNDERSCORE; break;
			case '=': segment_value = MAX7221_STRING_EQUALS; break;
			case '.': segment_value = MAX7221_STRING_DOT; break;
			case '/': segment_value = MAX7221_STRING_SLASH; break;
			case '\\': segment_value = MAX7221_STRING_BACKSLASH; break;
			case '\'': segment_value = MAX7221_STRING_QUOTE_SINGLE; break;
			case '\"': segment_value = MAX7221_STRING_QUOTE_DOUBLE; break;
			default:  segment_value = 0x00; break;  // Display blank for unknown characters
		}
		digit_set = map_digit(digit);
		max7221_transfer((uint16_t)((digit_set << 8) | segment_value));
		digit++;
	}
}

uint8_t map_digit(uint8_t digit)
{
	// link digit 0-7 to actual address
	if (digit > 7)
	{
		digit_set = 4;
	}
	else if (digit <= 3)
	{
		digit_set = ((digit + 5) & 0x0F);
	}
	else
	{
		digit_set = ((digit - 3) & 0x0F);
	}
	return digit_set;
}