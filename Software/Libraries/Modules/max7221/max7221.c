/*
 * max7221.c
 *
 * Created: 22.12.2024 15:02:28
 *  Author: Florian Reichart
 */ 

#define F_CPU 12000000UL
#include "max7221.h"
#include <util/delay.h>

SpiDevice *max7221_device;
uint8_t device_set = 0;
uint8_t digit_set = 0x00;

#define CS_ACTIVE (*(max7221_device->cs_port) &= ~(1 << max7221_device->cs_pin))
#define CS_INACTIVE (*(max7221_device->cs_port) |= (1 << max7221_device->cs_pin))


void max7221_init(uint8_t prescaler)
{
	// Initialize Ports
	DDRB |= (1 << PB4) | (1 << PB7) | (1 << PB5);	// Set PB4 (SS), PB7 (SCK) and PB5 (MOSI) as outputs
	DDRB &= ~(1 << PB6);	// Set PB6 (MISO) as input
	
	PORTB |= (1 << PB4);	// Set PB4 high for SlaveSelect = Master
	*(max7221_device->cs_port) |= (1 << max7221_device->cs_pin);  // Set PC6 high initially
	_delay_ms(10);
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
	_delay_ms(100);
	max7221_transfer((uint16_t)((MAX7221_REG_SHUTDOWN << 8) | MAX7221_SHUTDOWN_MODE));
	max7221_transfer((uint16_t)((MAX7221_REG_SCAN_LIMIT << 8) | MAX7221_SCAN_LIMIT_7));
	max7221_transfer((uint16_t)((MAX7221_REG_DECODE_MODE << 8) | MAX7221_DECODE_ALL));
	max7221_transfer((uint16_t)((MAX7221_REG_DISPLAY_TEST << 8) | MAX7221_DISPLAY_TEST_OFF));
	max7221_transfer((uint16_t)((MAX7221_REG_SHUTDOWN << 8) | MAX7221_NORMAL_OPERATION));
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
		if (digit < 0)
		{
			digit_set = 5;
		}
		else if (digit > 7)
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
		
		max7221_transfer((uint16_t)(((digit_set & 0xFF) << 8) | (value & 0x0F)));
	}
}


void max7221_set_digit_dp(uint8_t digit, uint8_t value)
{
	if (device_set)
	{
		if (digit < 0)
		{
			digit_set = 5;
		}
		else if (digit > 7)
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
		
		max7221_transfer((uint16_t)(0x80 | ((digit_set & 0xFF) << 8) | (value & 0x0F)));
	}
}


void max7221_clear_digit(uint8_t digit)
{
	if (device_set)
	{
		if (digit < 0)
		{
			digit_set = 5;
		}
		else if (digit > 7)
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
		max7221_transfer((uint16_t)(((digit_set & 0xFF) << 8) | MAX7221_DISPLAY_BLANK)); // Clear each digit
		_delay_ms(10);
	}
}


void max7221_clear_all()
{
	if (device_set)
	{
		for (uint8_t i = 0; i <= 8; i++) {
			max7221_transfer((uint16_t)(((i & 0xFF) << 8) | MAX7221_DISPLAY_BLANK)); // Clear each digit
			_delay_ms(10);
		}
	}
}

void max7221_print_uint8(uint8_t value, uint8_t digit)
{
    if (device_set)
	{
		digit = digit + 2;
		for (uint8_t i = 0; i < 3; i++)
		{
			if (digit < 0)
			{
				digit_set = 5;
			}
			else if (digit > 7)
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
			max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
			value = value / 10;
			_delay_ms(10);
			digit--;
		}
    }
}

//////////////////////////////Fehlersuche
void max7221_print_int8(int8_t value, uint8_t digit)
{
    if (device_set)
	{
        uint8_t negative = 0;
		digit = digit + 3;
		
		
		for (uint8_t i = 0; i < 4; i++)
		{
			if (digit < 0)
			{
				digit_set = 5;
			}
			else if (digit > 7)
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
			
			if (value < 0)
			{
				value = -value;
				negative = 1;
			}
			else if (negative == 1 && i == 3)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_HYPHEN));
				negative = 0;
			}
			
			if (i < 3)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
				value = value / 10;
				_delay_ms(10);
				digit--;
			}
		}
    }
}


/*void max7221_print_uint16(uint16_t value)
{
    if (device_set)
	{
        uint8_t digits[5] = {0, 0 , 0, 0, 0};
        uint8_t digitCount = 0;

        while (value > 0)
        {
            digits[digitCount++] = value % 10;
            value /= 10;
        }
        
        max7221_set_digit(0, MAX7221_DISPLAY_BLANK);
        max7221_set_digit(1, MAX7221_DISPLAY_BLANK);
        max7221_set_digit(2, digits[4]);
        max7221_set_digit(3, digits[3]);
        max7221_set_digit(4, digits[2]);
        max7221_set_digit(5, digits[1]);
        max7221_set_digit(6, digits[0]);
        max7221_set_digit(7, MAX7221_DISPLAY_BLANK);
    }
}*/


/*void max7221_print_int16(int16_t value)
{
    if (device_set)
	{
        uint8_t digits[5] = {0, 0 , 0, 0, 0};
        uint8_t digitCount = 0;
        uint8_t negative = 0;

        if (value < 0)
        {
            negative = 1;
            value = -value;
        }

        while (value > 0)
        {
            digits[digitCount++] = value % 10;
            value /= 10;
        }
        
        if (negative)
        {
            max7221_set_digit(0, MAX7221_DISPLAY_HYPHEN);
        }
        else
        {
            max7221_set_digit(0, MAX7221_DISPLAY_BLANK);
        }
        
        max7221_set_digit(1, MAX7221_DISPLAY_BLANK);
        max7221_set_digit(2, digits[4]);
        max7221_set_digit(3, digits[3]);
        max7221_set_digit(4, digits[2]);
        max7221_set_digit(5, digits[1]);
        max7221_set_digit(6, digits[0]);
        max7221_set_digit(7, MAX7221_DISPLAY_BLANK);
    }
}*/


/*void max7221_print_float(float value, uint8_t decimal)
{
    if (device_set)
	{
        uint8_t negative = 0;
        uint8_t digits[7] = {0, 0 , 0, 0, 0, 0, 0};
        uint8_t digitCount = 0;

        if (value < 0)
        {
            negative = 1;
            value = -value;
        }

        for(uint8_t i = 0; i < decimal; i++)
        {
            value = value * 10.0f;
        }

        uint32_t new_value = (uint32_t)(value);

        while (new_value > 0 && digitCount < 6)
        {
            digits[digitCount++] = new_value % 10;
            new_value /= 10;
        }

        uint8_t firstDigit = 0;
        if (negative)
        {
            firstDigit = MAX7221_DISPLAY_HYPHEN;
        }
        else
        {
            firstDigit = MAX7221_DISPLAY_BLANK;
        }

        if (decimal == 7)
        {
            max7221_set_digit_dp(0, firstDigit, 1);
        }
        else
        {
            max7221_set_digit(0, firstDigit);
        }

        for (uint8_t i = 0; i < 7; i++)
        {
            max7221_set_digit_dp(7-i, digits[i], (decimal == i));
        }
    }
}*/


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