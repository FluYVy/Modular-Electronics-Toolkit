/*
* AL5887.c
*
* Created: 28.10.2024 17:25:39
* Author: Florian Reichart
*/

#include "AL5887.h"

void AL5887_send_data(uint8_t register_address, uint8_t value)
{
	i2c_master_start();									// Start I2C communication
	i2c_master_sendAddress(AL5887_I2C_ADDRESS, 0x00);	// Send device I2C address
	i2c_master_sendChar(register_address);				// Send the register address
	i2c_master_sendChar(value);							// Send the value to write
	i2c_master_stop();									// Stop I2C communication
}

void AL5887_start()
{
	AL5887_send_data(AL5887_REGISTER_DEVICE_CONFIG0, (1 << 6));	// Set the start bit in the device config
}

void AL5887_stop()
{
	AL5887_send_data(AL5887_REGISTER_DEVICE_CONFIG0, 0x00);	// Clear the start bit
}

void AL5887_set_global_brightness(uint8_t brightness)
{
	// Write the brightness value to each of the brightness registers for the RGB LEDs
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB00, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB01, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB02, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB03, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB04, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB05, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB06, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB07, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB08, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB09, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB10, brightness);
	AL5887_send_data(AL5887_REGISTER_BRIGHTNESS_RGB11, brightness);
}

void AL5887_set_led_brightness(uint8_t led, uint8_t brightness)
{
	if (led < 36) // Check if the LED index is within the valid range
	{
		AL5887_send_data(AL5887_BASE_REGISTER_VALUE_LEDS + led, brightness); // Set brightness for the LED
	}
}

void AL5887_set_led_to_state(uint8_t led, uint8_t state)
{
	if (state)
	{
		AL5887_set_led_brightness(led, 0xFF);	// Turn the LED on
	}
	else
	{
		AL5887_set_led_brightness(led, 0x00);	// Turn the LED off
	}
}

void AL5887_set_led_to_on(uint8_t led)
{
	AL5887_set_led_brightness(led, 0xFF);	// Set brightness to maximum
}

void AL5887_set_led_to_off(uint8_t led)
{
	AL5887_set_led_brightness(led, 0x00);	// Set brightness to zero
}

void AL5887_flip_led_state(uint8_t led)
{
	i2c_master_start();												// Start I2C communication
	i2c_master_sendAddress(AL5887_I2C_ADDRESS, 0x00);				// Send device address for writing
	i2c_master_sendChar(AL5887_BASE_REGISTER_VALUE_LEDS + led);		// Send address of Leds brightness
	i2c_master_start();												// Start a new I2C communication (Repeated Start)
	i2c_master_sendAddress(AL5887_I2C_ADDRESS, 0x01);				// Send device address for reading
	uint8_t current_state = i2c_master_receiveChar(0x00);			// Read the current state
	i2c_master_stop();												// Stop I2C communication
	
	// Toggle the LED based on its current state
	if (current_state)
	{
		AL5887_set_led_to_off(led);  // If on, turn off the LED
	}
	else
	{
		AL5887_set_led_to_on(led);   // If off, turn on the LED
	}
}
