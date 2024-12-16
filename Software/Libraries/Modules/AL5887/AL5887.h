/*
* AL5887.h
*
* Created: 28.10.2024 17:25:49
* Author: Florian Reichart
*/

#ifndef AL5887_H_
#define AL5887_H_

#include "i2c_master.h"

// Define the I2C address based on the selected address type.
#if AL5887_I2C_ADDRESS_TYPE == 3
#define AL5887_I2C_ADDRESS                 0x33 // I2C address for type 3
#elif AL5887_I2C_ADDRESS_TYPE == 2
#define AL5887_I2C_ADDRESS                 0x32 // I2C address for type 2
#elif AL5887_I2C_ADDRESS_TYPE == 1
#define AL5887_I2C_ADDRESS                 0x31 // I2C address for type 1
#else
#define AL5887_I2C_ADDRESS                 0x30 // Default I2C address
#endif

// Register Addresses for the AL5887 device
#define AL5887_REGISTER_DEVICE_CONFIG0     0x00 // Device Configuration Register 0
#define AL5887_REGISTER_DEVICE_CONFIG1     0x01 // Device Configuration Register 1

// Brightness Registers for RGB LEDs
#define AL5887_REGISTER_BRIGHTNESS_RGB00   0x08 // Brightness register for RGB LED 0
#define AL5887_REGISTER_BRIGHTNESS_RGB01   0x09 // Brightness register for RGB LED 1
#define AL5887_REGISTER_BRIGHTNESS_RGB02   0x0A // Brightness register for RGB LED 2
#define AL5887_REGISTER_BRIGHTNESS_RGB03   0x0B // Brightness register for RGB LED 3
#define AL5887_REGISTER_BRIGHTNESS_RGB04   0x0C // Brightness register for RGB LED 4
#define AL5887_REGISTER_BRIGHTNESS_RGB05   0x0D // Brightness register for RGB LED 5
#define AL5887_REGISTER_BRIGHTNESS_RGB06   0x0E // Brightness register for RGB LED 6
#define AL5887_REGISTER_BRIGHTNESS_RGB07   0x0F // Brightness register for RGB LED 7
#define AL5887_REGISTER_BRIGHTNESS_RGB08   0x10 // Brightness register for RGB LED 8
#define AL5887_REGISTER_BRIGHTNESS_RGB09   0x11 // Brightness register for RGB LED 9
#define AL5887_REGISTER_BRIGHTNESS_RGB10   0x12 // Brightness register for RGB LED 10
#define AL5887_REGISTER_BRIGHTNESS_RGB11   0x13 // Brightness register for RGB LED 11

#define AL5887_BASE_REGISTER_VALUE_LEDS    0x14 // Base register for LED values

/**
* @brief Starts the AL5887 device.
*
* This function initializes the AL5887 device and configures it
* to start operating. It sets the necessary register values.
*/
void AL5887_start();

/**
* @brief Stops the AL5887 device.
*
* This function stops the AL5887 device by configuring its registers
* to turn off the device.
*/
void AL5887_stop();

/**
* @brief Sends data to a specified register of the AL5887 device.
*
* This function writes a value to a specific register by first
* starting the I2C communication, sending the register address,
* and the value to be written.
*
* @param[in] register_address The address of the register to write to.
* @param[in] value The value to write to the specified register.
*/
void AL5887_send_data(uint8_t register_address, uint8_t value);

/**
* @brief Sets the global brightness of the AL5887 device.
*
* This function sets the brightness for all available RGB LEDs by
* writing the specified brightness value to each brightness register.
*
* @param[in] brightness The brightness value to set (0-255).
*/
void AL5887_set_global_brightness(uint8_t brightness);

/**
* @brief Sets the brightness of a specific LED.
*
* This function sets the brightness for a specified LED if it is
* within the valid range (0-35).
*
* @param[in] led The index of the LED to configure (0-35).
* @param[in] brightness The brightness value to set (0-255).
*/
void AL5887_set_led_brightness(uint8_t led, uint8_t brightness);

/**
* @brief Sets the state of a specific LED.
*
* This function sets the brightness of the specified LED to either
* maximum (on) or minimum (off) based on the state parameter.
*
* @param[in] led The index of the LED to set (0-35).
* @param[in] state The desired state: 1 to turn on, 0 to turn off.
*/
void AL5887_set_led_to_state(uint8_t led, uint8_t state);

/**
* @brief Turns on a specific LED.
*
* This function sets the brightness of the specified LED to maximum.
*
* @param[in] led The index of the LED to turn on (0-35).
*/
void AL5887_set_led_to_on(uint8_t led);

/**
* @brief Turns off a specific LED.
*
* This function sets the brightness of the specified LED to zero.
*
* @param[in] led The index of the LED to turn off (0-35).
*/
void AL5887_set_led_to_off(uint8_t led);

/**
* @brief Toggles the state of a specific LED.
*
* This function reads the current state of the specified LED and toggles it
* between on and off.
*
* @param[in] led The index of the LED to toggle (0-35).
*/
void AL5887_flip_led_state(uint8_t led);

#endif /* AL5887_H_ */
