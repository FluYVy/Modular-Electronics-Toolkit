/*
 * AL5887.h
 *
 * Created: 28.10.2024 17:25:49
 *  Author: Florian Reichart
 */ 


#ifndef AL5887_H_
#define AL5887_H_

#include "i2c_master.h"

#if AL5887_I2C_ADDRESS_TYPE == 3
#define AL5887_I2C_ADDRESS					0x33
#elif AL5887_I2C_ADDRESS_TYPE == 2
#define AL5887_I2C_ADDRESS					0x32
#elif AL5887_I2C_ADDRESS_TYPE == 1
#define AL5887_I2C_ADDRESS					0x31
#else
#define AL5887_I2C_ADDRESS					0x30
#endif

//Register Addresses
#define AL5887_REGISTER_DEVICE_CONFIG0		0x00
#define AL5887_REGISTER_DEVICE_CONFIG1		0x01

#define AL5887_REGISTER_BRIGHTNESS_RGB00	0x08
#define AL5887_REGISTER_BRIGHTNESS_RGB01	0x09
#define AL5887_REGISTER_BRIGHTNESS_RGB02	0x0a
#define AL5887_REGISTER_BRIGHTNESS_RGB03	0x0b
#define AL5887_REGISTER_BRIGHTNESS_RGB04	0x0c
#define AL5887_REGISTER_BRIGHTNESS_RGB05	0x0d
#define AL5887_REGISTER_BRIGHTNESS_RGB06	0x0e
#define AL5887_REGISTER_BRIGHTNESS_RGB07	0x0f
#define AL5887_REGISTER_BRIGHTNESS_RGB08	0x10
#define AL5887_REGISTER_BRIGHTNESS_RGB09	0x11
#define AL5887_REGISTER_BRIGHTNESS_RGB10	0x12
#define AL5887_REGISTER_BRIGHTNESS_RGB11	0x13

#define AL5887_BASE_REGISTER_VALUE_LEDS		0x14

void AL5887_start();
void AL5887_stop();
void AL5887_send_data(uint8_t register_address, uint8_t value);
void AL5887_set_global_brightness(uint8_t brightness);
void AL5887_set_led_brightness(uint8_t led, uint8_t brightness);
void AL5887_set_led_to_state(uint8_t led, uint8_t state);
void AL5887_set_led_to_on(uint8_t led);
void AL5887_set_led_to_off(uint8_t led);
void AL5887_flip_led_state(uint8_t led);


#endif /* AL5887_H_ */