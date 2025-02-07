/*
 * max7221.c
 *
 * Created: 22.12.2024 15:02:28
 *  Author: Florian Reichart
 */ 

#define F_CPU 12000000UL
#include "max7221.h"
#include <util/delay.h>
#include <math.h>
#include <stdint.h>

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
	_delay_ms(1000);
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

void max7221_print_uint8_default(uint8_t value) {
	max7221_print_uint8(value, 0);
}

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
			if (i < 3)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
				value = value / 10;
				_delay_ms(10);
				digit--;
			}
			else if (negative == 1)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_HYPHEN));
				negative = 0;
			}
		}
    }
}

void max7221_print_int8_default(int8_t value) {
	max7221_print_int8(value, 0);
}


void max7221_print_uint16(uint16_t value, uint8_t digit)
{
    if (device_set)
	{
		digit = digit + 4;
		for (uint8_t i = 0; i < 5; i++)
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
			_delay_ms(1);
			digit--;
		}
    }
}

void max7221_print_uint16_default(uint16_t value) {
	max7221_print_uint16(value, 0);
}


// working version with 0's
/*void max7221_print_int16_length(int16_t value, uint8_t digit, uint8_t length)
{
    if (device_set)
	{
		uint8_t negative = 0;
		length--;
		
		if (length > 5)
		{
			length = 5;
		}
		else if (length < 1)
		{
			length = 1;
		}
		
		
		digit = digit + length;
		
		
		
		if (value < 0)
		{
			value = -value;
			negative = 1;
		}
		
		
		
		
		for (uint8_t i = 0; i < (length+1); i++)
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
			
			
			if (i < length)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
				value = value / 10;
				digit--;
			}
			else if (negative == 1)
			{
				max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_HYPHEN));
				negative = 0;
			}
			else
			{
				max7221_transfer((uint16_t)((digit_set << 8) | ((value % 10) & 0x0F)));
			}
		}
    }
}*/

// own version not working
void max7221_print_int16_length(int16_t value, uint8_t digit, uint8_t length)
{
	// check if code should be run
    if (device_set)
	{
		uint8_t negative = 0;
		uint8_t del_digit = 0;
		
		// make length work with code (because of hyphen length has to be decreased for actual digit number)
		length--;
		
		// maximum length for 16bit = 65535 = (5 + hyphen) = 6, because of length-- it is 5
		if (length > 5)
		{
			length = 5;
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
			
			// check if the number displayed needs all defined digits and if not, delete rightmost digit (using del_digit = 1)
			while (value < (int)pow(10, (length-1)) && length >= 2)
			{
				length--;
				del_digit = 1;
			}
		}
		// if positive value
		else
		{
			// check if the number displayed needs all defined digits and if not, delete rightmost digit (using del_digit = 1)
			while (value < (int)pow(10, length) && length >= 1)
			{
				length--;
				del_digit = 1;
			}
		}
		
		// new rightmost digit (using shortened length) + 1
		digit = digit + length + 1;
		
		// if rightmost digit needs to be deleted
		if (del_digit == 1)
		{
			// link digit 0-7 to actual address
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
			
			// delete digit
			max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_BLANK));
			del_digit = 0;
			
		}
		
		// calculate rightmost digit
		digit = digit - 1;
		
		// for every digit the number has to be displayed on
		for (uint8_t i = 0; i < (length+1); i++)
		{
			
			// link digit 0-7 to actual address
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
			
			
			// if not at leftmost digit
			if (i < length)
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

// o3 mini high (works)
/*void max7221_print_int16_length(int16_t value, uint8_t start_digit, uint8_t total_positions)  
{  
    if (device_set)  
    {  
		
        // Constrain total_positions to available segments (0..7)  
        if (total_positions > 8)  
            total_positions = 8;  
        if (total_positions == 0)  
            return;  // nothing to do  
  
        uint8_t is_negative = 0;  
        uint16_t absVal;  
        if (value < 0)  
        {  
            is_negative = 1;  
            absVal = (uint16_t)(-value);  
        }  
        else  
        {  
            absVal = (uint16_t)value;  
        }  
  
        // Determine the number of digits needed to represent absVal  
        uint8_t numDigits = 0;  
        uint16_t temp = absVal;  
        if (temp == 0)   
        {  
            numDigits = 1;  
        }  
        else  
        {  
            while (temp > 0)  
            {  
                numDigits++;  
                temp /= 10;  
            }  
        }  
  
        // Total characters we want to show:  
        // if negative, we want a leading hyphen plus the digits.  
        uint8_t total_chars = numDigits + (is_negative ? 1 : 0);  
  
        // If the number does not fill all available positions, we’ll blank the remaining ones.  
        // Also if total_chars > total_positions, we only show the most significant total_positions.  
        if (total_chars > total_positions)  
        {  
            // In a negative number if there isn’t room for the sign then drop it.  
            if (is_negative && numDigits >= total_positions)  
            {  
                is_negative = 0;  
                total_chars = numDigits;  
            }  
            else  
            {  
                total_chars = total_positions;  
            }  
        }  
  

        uint8_t display[8];  // maximum 8 segments available  
        uint8_t pos = 0;  
          
        if (is_negative)  
        {  
            display[pos++] = MAX7221_DISPLAY_HYPHEN;  
        }  
  
        // Prepare to extract the digits starting with the most significant.  
        // Compute appropriate power-of-10 divisor.  
        uint16_t divisor = 1;  
        for (uint8_t i = 1; i < numDigits; i++)  
        {  
            divisor *= 10;  
        }  
        for (uint8_t i = 0; i < numDigits && pos < total_chars; i++)  
        {  
            uint8_t d = (absVal / divisor) % 10;  
            display[pos++] = d;  
            divisor /= 10;  
        }  
        // If there are still positions left, fill them with the blank character.  
        while (pos < total_positions)  
        {  
            display[pos++] = MAX7221_DISPLAY_BLANK;  
        }  
  
        // Now send the full display array to the MAX7221 left-to-right.  
        // (Remember: segment 0 is leftmost and 7 is rightmost)  
        for (uint8_t i = 0; i < total_positions; i++)  
        {  
            uint8_t mapped;  
            uint8_t physical_digit = start_digit + i;  
            // --- Mapping logic (assumed to work) ---  
            if (physical_digit < 0)  
            {  
                mapped = 5;  
            }  
            else if (physical_digit > 7)  
            {  
                mapped = 4;  
            }  
            else if (physical_digit <= 3)  
            {  
                mapped = ((physical_digit + 5) & 0x0F);  
            }  
            else  
            {  
                mapped = ((physical_digit - 3) & 0x0F);  
            }  
            // ------------------------------------------  
            max7221_transfer((uint16_t)((mapped << 8) | (display[i] & 0x0F)));  
        }  
    }  
}*/

void max7221_print_int16_default(int16_t value) {
	max7221_print_int16_length(value, 0, 5);
}

void max7221_print_int16(int16_t value, uint8_t digit)
{
	max7221_print_int16_length(value, digit, 5);
}


void max7221_print_float(float value, int8_t decimal, uint8_t digit, uint8_t length)
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

        /*for(uint8_t i = 0; i < decimal; i++)
        {
            value = value * 10.0f;
        }
		for(uint8_t i = 0; i < -decimal; i++)
		{
			value = value / 10.0f;
		}
		// 493.52 or 0.05245*/
		while (value >= 10)
		{
			value /= 10;
			decimal += 1;
		}
		while (value < 1)
		{
			value *= 10;
			decimal -= 1;
		}
		// number between 1-9.99999 (e.g. 3.5256)
		

        uint32_t new_value = (uint32_t)(value);
		
		
		/////////////////////////////////
		uint8_t del_digit = 0;
		
		// make length work with code (because of hyphen, length has to be decreased for actual digit number)
		length--;
		
		// maximum length of 7-segment display = (7 + hyphen) = 8, because of length-- it is 7
		if (length > 7)
		{
			length = 7;
		}
		// minimum length = 3
		else if (length < 3)
		{
			length = 3;
		}
		
		
		
		
		
		// if negative value, convert to positive value and set negative = 1
		if (value < 0)
		{
			value = -value;
			negative = 1;
				
			// check if the number displayed needs all defined digits and if not, delete rightmost digit (using del_digit = 1)
			while (value < (int)pow(10, (length-1)) && length >= 2)
			{
				length--;
				del_digit = 1;
			}
		}
		// if positive value
		else
		{
			// check if the number displayed needs all defined digits and if not, delete rightmost digit (using del_digit = 1)
			while (value < (int)pow(10, length) && length >= 1)
			{
				length--;
				del_digit = 1;
			}
		}
		
		if (decimal >= 10 && (((length >= 5) && negative) || ((length >= 4) && !negative)))
		{
			length = length - 3;
		}
		else if (decimal > 0)
		{
			
		}
		else if (decimal <= -10)
		{
			
		}
		else if (decimal < 0)
		{
			
		}
		
		
		// new rightmost digit (using shortened length) + 1
		digit = digit + length + 1;
		
		// if rightmost digit needs to be deleted
		if (del_digit == 1)
		{
			// link digit 0-7 to actual address
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
			
			// delete digit
			max7221_transfer((uint16_t)((digit_set << 8) | MAX7221_DISPLAY_BLANK));
			del_digit = 0;
			
		}
		
		// calculate rightmost digit
		digit = digit - 1;
		
		// for every digit the number has to be displayed on
		for (uint8_t i = 0; i < (length+1); i++)
		{
			
			// link digit 0-7 to actual address
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
			
			
			// if not at leftmost digit
			if (i < length)
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
		///////////////////////////////////////
		
		

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

// TODO:
// print_string