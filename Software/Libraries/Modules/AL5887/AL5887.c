/*
 * AL5887.c
 *
 * Created: 28.10.2024 17:25:39
 *  Author: Florian Reichart
 */ 

#include "AL5887.h"

void AL5887_send_data(uint8_t register_address, uint8_t value)
{
	i2c_master_start();
	i2c_master_sendAddress(AL5887_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(register_address);
	i2c_master_sendChar(value);
	i2c_master_stop();
}

void AL5887_start()
{
	AL5887_send_data(AL5887_REGISTER_DEVICE_CONFIG0, (1<<6));
}

void AL5887_stop()
{
	AL5887_send_data(AL5887_REGISTER_DEVICE_CONFIG0, 0x00);
}

void AL5887_set_global_brightness(uint8_t brightness)
{
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
	if (led < 36)
	{
		AL5887_send_data(AL5887_BASE_REGISTER_VALUE_LEDS + led, brightness);
	}
}

void AL5887_set_led_to_state(uint8_t led, uint8_t state)
{
	if (state)
	{
		AL5887_set_led_brightness(led, 0xFF);
	}else
	{
		AL5887_set_led_brightness(led, 0x00);
	}
}

void AL5887_set_led_to_on(uint8_t led)
{
	AL5887_set_led_brightness(led, 0xFF);
}

void AL5887_set_led_to_off(uint8_t led)
{
	AL5887_set_led_brightness(led, 0x00);
}

void AL5887_flip_led_state(uint8_t led)
{
	i2c_master_start();
	i2c_master_sendAddress(AL5887_I2C_ADDRESS, 0x00);
	i2c_master_receiveChar(AL5887_BASE_REGISTER_VALUE_LEDS + led)
	i2c_master_start();
	i2c_master_sendAddress(AL5887_I2C_ADDRESS, 0x01);
	uint8_t current_state = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	if (current_state)
	{
		AL5887_set_led_to_off(led);
	}else
	{
		AL5887_set_led_to_on(led);
	}
}