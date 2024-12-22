/*
 * max7221.c
 *
 * Created: 22.12.2024 15:02:28
 *  Author: Florian Reichart
 */ 

#include "sevseg.h"	

SpiDevice max7221_device;
uint8_t device_set = 0;

void max7221_start(SpiDevice device)
{
	max7221_device = device;
	device_set = 1;

	spi_master_device_init(&max7221_device);
	spi_master_transfer(&max7221_device, (uint8_t[]){MAX7221_ADDRESS_SHUTDOWN, 0x01}, 2);
}


void max7221_stop()
{
	if (device_set)
	{
		spi_master_transfer(&max7221_device, (uint8_t[]){MAX7221_ADDRESS_SHUTDOWN, 0x00}, 2);
		device_set = 0;
	}
}


void max7221_set_digit(uint8_t digit, uint8_t value)
{
	if (device_set)
	{
		spi_master_transfer(&max7221_device, (uint8_t[]){MAX7221_ADDRESS_DIGIT + (digit & 0x07), (value & 0x0F)}, 2);
	}
}


void max7221_set_digit_dp(uint8_t digit, uint8_t value, uint8_t decimal_point)
{
	if (device_set)
	{
		spi_master_transfer(&max7221_device, (uint8_t[]){MAX7221_ADDRESS_DIGIT + (digit & 0x07), ((value & 0x0F) | (decimal_point << 7))}, 2);
	}
}


void max7221_clear_all()
{
	if (device_set)
	{
		max7221_clear_display(0);
		max7221_clear_display(1);
	}
}


void max7221_clear_display(uint8_t display)
{
	if (device_set)
	{
		uint8_t offset = (display & 0x01)<<2;
		max7221_set_digit(offset + 0, 0x0F);
		max7221_set_digit(offset + 1, 0x0F);
		max7221_set_digit(offset + 2, 0x0F);
		max7221_set_digit(offset + 3, 0x0F);
	}
}


void max7221_print_uint8(uint8_t value, uint8_t display)
{
    if (device_set)
	{
        uint8_t digits[4] = {0, 0 , 0, 0};
        uint8_t digitCount = 0;
        uint8_t offset = (display & 0x01)<<2;

        while (value > 0)
        {
            digits[digitCount++] = value % 10;
            value /= 10;
        }
        
        for (uint8_t i = 0; i < 4; ++i) {
            max7221_set_digit(offset + 3 - i, digits[i]);
        }
    }
}


void max7221_print_int8(int8_t value, uint8_t display)
{
    if (device_set)
	{
        uint8_t digits[3] = {0 , 0, 0};
        uint8_t digitCount = 0;
        uint8_t negative = 0;
        uint8_t offset = (display & 0x01)<<2;

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
            max7221_set_digit(offset, MAX7221_DIGIT_HYPHEN);
        }
        else
        {
            max7221_set_digit(offset, MAX7221_DIGIT_BLANK);
        }
        
        for (uint8_t i = 0; i < 3; ++i) {
            max7221_set_digit(offset + 3 - i, digits[i]);
        }
    }
}


void max7221_print_uint16(uint16_t value)
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
        
        max7221_set_digit(0, MAX7221_DIGIT_BLANK);
        max7221_set_digit(1, MAX7221_DIGIT_BLANK);
        max7221_set_digit(2, digits[4]);
        max7221_set_digit(3, digits[3]);
        max7221_set_digit(4, digits[2]);
        max7221_set_digit(5, digits[1]);
        max7221_set_digit(6, digits[0]);
        max7221_set_digit(7, MAX7221_DIGIT_BLANK);
    }
}


void max7221_print_int16(int16_t value)
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
            max7221_set_digit(0, MAX7221_DIGIT_HYPHEN);
        }
        else
        {
            max7221_set_digit(0, MAX7221_DIGIT_BLANK);
        }
        
        max7221_set_digit(1, MAX7221_DIGIT_BLANK);
        max7221_set_digit(2, digits[4]);
        max7221_set_digit(3, digits[3]);
        max7221_set_digit(4, digits[2]);
        max7221_set_digit(5, digits[1]);
        max7221_set_digit(6, digits[0]);
        max7221_set_digit(7, MAX7221_DIGIT_BLANK);
    }
}


void max7221_print_float(float value, uint8_t decimal)
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
            firstDigit = MAX7221_DIGIT_HYPHEN;
        }
        else
        {
            firstDigit = MAX7221_DIGIT_BLANK;
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
