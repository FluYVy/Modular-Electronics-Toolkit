/*
 * max7221.h
 *
 * Created: 22.12.2024 13:00:37
 * Author: Florian Reichart
 */ 


#ifndef MAX7221_H_
#define MAX7221_H_

#include "spi_master.h"

/**
 * @defgroup Addresses MAX7221 Addreses
 * Addresses for controlling the max7221 chip
*/

/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to control the digits
 * @note This is the offset of the first digit address. To address any digit add the digit to this offset.
 * @ingroup Address
 */
#define MAX7221_ADDRESS_DIGIT           0x01

/**
 * @def MAX7221_ADDRESS_DECODE_MODE
 * Address to set the decode mode
 * @ingroup Address
 */
#define MAX7221_ADDRESS_DECODE_MODE     0x09

/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to control the intensity of the digits
 * @ingroup Address
 */
#define MAX7221_ADDRESS_INTENSITY       0x0A

/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to control the scan limit
 * @ingroup Address
 */
#define MAX7221_ADDRESS_SCAN_LIMIT      0x0B

/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to shutdown and start the address
 * @ingroup Address
 */
#define MAX7221_ADDRESS_SHUTDOWN        0x0C

/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to controll the test mode of the max7221
 * @ingroup Address
 */
#define MAX7221_ADDRESS_DISPLAY_TEST    0x0F


/**
 * @defgroup MAX7221_DIGIT Digits for the display
 * @brief Definitions for different characters to print on the display
 *
 * These definitions represent various posible characters for the MAX7221.
 * The following options are available:
 * - `MAX7221_DIGIT_0`      (0x00): Display the character 0
 * - `MAX7221_DIGIT_1`      (0x01): Display the character 1
 * - `MAX7221_DIGIT_2`      (0x02): Display the character 2
 * - `MAX7221_DIGIT_3`      (0x03): Display the character 3
 * - `MAX7221_DIGIT_4`      (0x04): Display the character 4
 * - `MAX7221_DIGIT_5`      (0x05): Display the character 5
 * - `MAX7221_DIGIT_6`      (0x06): Display the character 6
 * - `MAX7221_DIGIT_7`      (0x07): Display the character 7
 * - `MAX7221_DIGIT_8`      (0x08): Display the character 8
 * - `MAX7221_DIGIT_9`      (0x09): Display the character 9
 * - `MAX7221_DIGIT_HYPHEN` (0x0a): Display the character -
 * - `MAX7221_DIGIT_E`      (0x0b): Display the character E
 * - `MAX7221_DIGIT_H`      (0x0c): Display the character H
 * - `MAX7221_DIGIT_L`      (0x0d): Display the character L
 * - `MAX7221_DIGIT_P`      (0x0e): Display the character P
 * - `MAX7221_DIGIT_BLANK`  (0x0f): Display no character
 *
 * @{
 */
#define MAX7221_DIGIT_0                 0x00
#define MAX7221_DIGIT_1                 0x01
#define MAX7221_DIGIT_2                 0x02
#define MAX7221_DIGIT_3                 0x03
#define MAX7221_DIGIT_4                 0x04
#define MAX7221_DIGIT_5                 0x05
#define MAX7221_DIGIT_6                 0x06
#define MAX7221_DIGIT_7                 0x07
#define MAX7221_DIGIT_8                 0x08
#define MAX7221_DIGIT_9                 0x09
#define MAX7221_DIGIT_HYPHEN            0x0A
#define MAX7221_DIGIT_E                 0x0B
#define MAX7221_DIGIT_H                 0x0C
#define MAX7221_DIGIT_L                 0x0D
#define MAX7221_DIGIT_P                 0x0E
#define MAX7221_DIGIT_BLANK             0x0F

/** @} */


/**
 * @brief Initialize the display
 * 
 * @param device The SPI-device of the MAX7221-chip
 */
void max7221_start(SpiDevice device);

/**
 * @brief Turns the display off
 */
void max7221_stop();

/**
 * @brief Write a digit without a decimal point to the display
 * 
 * Sets a single digit without a decimal point on the display to the specifies value
 * @param digit Position of digit (0-7, left to right and top to bottom)
 * @param value Value to display
 *              0x00-0x09 ... The digit will be shown as a decimal value
 *                   0x0A ... Will display a hyphen (-)
 *                   0x0B ... Will display E
 *                   0x0C ... Will display H
 *                   0x0D ... Will display L
 *                   0x0E ... Will display P
 *                   0x0F ... Will display nothing
 *                
 */
void max7221_set_digit(uint8_t digit, uint8_t value);

/**
 * @brief Write a digit to the display
 * 
 * Sets a single digit on the display to the specifies value.
 * @param digit Position of digit (0-7, left to right and top to bottom)
 * @param value Value to display
 *              0x00-0x09 ... The digit will be shown as a decimal value
 *                   0x0A ... Will display a hyphen (-)
 *                   0x0B ... Will display E
 *                   0x0C ... Will display H
 *                   0x0D ... Will display L
 *                   0x0E ... Will display P
 *                   0x0F ... Will display nothing
 * @param decimal_point Decides if there should be a decimal point or not
 *                
 */
void max7221_set_digit_dp(uint8_t digit, uint8_t value, uint8_t decimal_point);

/**
 * @brief Clear all displays
 * Clears all digits on the display by setting them to blank (0x0F)
 */
void max7221_clear_all();

/**
 * @brief Clear a display
 * Clears a single display depending of the given value
 * @param display The display to clear
 */
void max7221_clear_display(uint8_t display);

/**
 * @brief Prints a unsigned 8-bit number on the specified display.
 * 
 * @param value The number to print
 * @param display The display to print on
 */
void max7221_print_uint8(uint8_t value, uint8_t display);

/**
 * @brief Prints a signed 8-bit number on the specified display.
 * 
 * @param value The number to print
 * @param display The display to print on
 */
void max7221_print_int8(int8_t value, uint8_t display);

/**
 * @brief Prints a unsigned 16-bit number on both the displays.
 * 
 * @param value The number to print
 */
void max7221_print_uint16(uint16_t value);

/**
 * @brief Prints a signed 16-bit number on both the displays.
 * 
 * @param value The number to print
 */
void max7221_print_int16(int16_t value);


/**
 * @brief Prints a signed 16-bit number on both the displays.
 * 
 * @param value The number to print
 * @param decimal The number of decimal places to display
 */
void max7221_print_float(float value, uint8_t decimal);

#endif /* MAX7221_H_ */