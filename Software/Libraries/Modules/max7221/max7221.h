/*
 * max7221.h
 *
 * Created: 22.12.2024 13:00:37
 * Author: Florian Reichart
 */ 


#ifndef MAX7221_H_
#define MAX7221_H_

#include <avr/io.h>

/**  
 * @brief Structure to represent an SPI device.  
 *  
 * This structure contains the necessary information to control a specific  
 * SPI device, including its chip select (CS) pin configuration.  
 */  
typedef struct {  
    volatile uint8_t *cs_ddr;  /**< Pointer to the data direction register for the CS pin */  
    volatile uint8_t *cs_port;  /**< Pointer to the port register for the CS pin */  
    uint8_t cs_pin;             /**< The specific pin number for the CS signal */  
} SpiDevice;


// Register Addresses
#define MAX7221_REG_NO_OP           0x00
#define MAX7221_REG_DIGIT0         0x01
#define MAX7221_REG_DIGIT1         0x02
#define MAX7221_REG_DIGIT2         0x03
#define MAX7221_REG_DIGIT3         0x04
#define MAX7221_REG_DIGIT4         0x05
#define MAX7221_REG_DIGIT5         0x06
#define MAX7221_REG_DIGIT6         0x07
#define MAX7221_REG_DIGIT7         0x08
#define MAX7221_REG_DECODE_MODE    0x09
#define MAX7221_REG_INTENSITY      0x0A
#define MAX7221_REG_SCAN_LIMIT     0x0B
#define MAX7221_REG_SHUTDOWN       0x0C
#define MAX7221_REG_DISPLAY_TEST   0x0F

/**
 * @defgroup Addresses MAX7221 Addreses
 * Addresses for controlling the max7221 chip
*/

/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to control the digits
 * @note This is the offset of the first digit address. To address any digit add the digit to this offset. But it starts at the 5th LED.
 * @ingroup Address
 */
#define MAX7221_REG_DIGIT           0x01

/**
 * @def MAX7221_ADDRESS_DECODE_MODE
 * Address to set the decode mode
 * @ingroup Address
 */
// Decode modes
#define MAX7221_NO_DECODE		0x00		// You can control each segment [a,b,c,d,e,f,g,dp]
#define MAX7221_DECODE_TOP		0x0F		// Only decode top row
#define MAX7221_DECODE_BOTTOM	0xF0		// Only decode bottom row
#define MAX7221_DECODE_ALL		0xFF		// You can send the number or letter directly and it will automatically convert it.

/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to control the intensity of the digits
 * @ingroup Address
 */




/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to control the scan limit
 * @ingroup Address
 */
#define MAX7221_SCAN_LIMIT_0	0x00
#define MAX7221_SCAN_LIMIT_1	0x01
#define MAX7221_SCAN_LIMIT_2	0x02
#define MAX7221_SCAN_LIMIT_3	0x03
#define MAX7221_SCAN_LIMIT_4	0x04
#define MAX7221_SCAN_LIMIT_5	0x05
#define MAX7221_SCAN_LIMIT_6	0x06
#define MAX7221_SCAN_LIMIT_7	0x07


/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to shutdown and start the address
 * @ingroup Address
 */
#define MAX7221_SHUTDOWN_MODE      0x00
#define MAX7221_NORMAL_OPERATION   0x01

/**
 * @def MAX7221_ADDRESS_DIGIT
 * Address to control the test mode of the max7221
 * @ingroup Address
 */
#define MAX7221_DISPLAY_TEST_OFF   0x00
#define MAX7221_DISPLAY_TEST_ON    0x01


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
#define MAX7221_DISPLAY_0                 0x00
#define MAX7221_DISPLAY_1                 0x01
#define MAX7221_DISPLAY_2                 0x02
#define MAX7221_DISPLAY_3                 0x03
#define MAX7221_DISPLAY_4                 0x04
#define MAX7221_DISPLAY_5                 0x05
#define MAX7221_DISPLAY_6                 0x06
#define MAX7221_DISPLAY_7                 0x07
#define MAX7221_DISPLAY_8                 0x08
#define MAX7221_DISPLAY_9                 0x09
#define MAX7221_DISPLAY_HYPHEN            0x0A
#define MAX7221_DISPLAY_E                 0x0B
#define MAX7221_DISPLAY_H                 0x0C
#define MAX7221_DISPLAY_L                 0x0D
#define MAX7221_DISPLAY_P                 0x0E
#define MAX7221_DISPLAY_BLANK             0x0F




// Letters
#define MAX7221_STRING_a                 0b01110111		// dp,a,b,c,d,e,f,g
#define MAX7221_STRING_b                 0b00011111
#define MAX7221_STRING_c                 0b00001101
#define MAX7221_STRING_d                 0b00111101
#define MAX7221_STRING_e                 0b01001111
#define MAX7221_STRING_f                 0b01000111
#define MAX7221_STRING_g                 0b01011110
#define MAX7221_STRING_h                 0b00110111
#define MAX7221_STRING_i                 0b00000110
#define MAX7221_STRING_j                 0b00111100
#define MAX7221_STRING_k                 0b01010111
#define MAX7221_STRING_l                 0b00001110
#define MAX7221_STRING_m                 0b01010101
#define MAX7221_STRING_n                 0b00010101
#define MAX7221_STRING_o                 0b00011101
#define MAX7221_STRING_p                 0b01100111
#define MAX7221_STRING_q                 0b01110011
#define MAX7221_STRING_r                 0b00000101
#define MAX7221_STRING_s                 0b01011011
#define MAX7221_STRING_t                 0b00001111
#define MAX7221_STRING_u                 0b00111110
#define MAX7221_STRING_v                 0b00101010
#define MAX7221_STRING_w                 0b00101011
#define MAX7221_STRING_x                 0b00010011
#define MAX7221_STRING_y                 0b00111011
#define MAX7221_STRING_z                 0b01101101

// Numbers 0-9
#define MAX7221_STRING_0                 0b01111110
#define MAX7221_STRING_1                 0b00110000
#define MAX7221_STRING_2                 0b01101101
#define MAX7221_STRING_3                 0b01111001
#define MAX7221_STRING_4                 0b00110011
#define MAX7221_STRING_5                 0b01011011
#define MAX7221_STRING_6                 0b01011111
#define MAX7221_STRING_7                 0b01110000
#define MAX7221_STRING_8                 0b01111111
#define MAX7221_STRING_9                 0b01111011

// Special Characters
#define MAX7221_STRING_SPACE             0b00000000    // All segments off
#define MAX7221_STRING_HYPHEN            0b00000001    // Middle segment (g)
#define MAX7221_STRING_DEGREE            0b01100011    // ° symbol
#define MAX7221_STRING_UNDERSCORE        0b00001000    // Lower segment (d)
#define MAX7221_STRING_EQUALS            0b00001001    // Middle + Lower segments (g + d)
#define MAX7221_STRING_DOT               0b10000000    // Decimal point
#define MAX7221_STRING_SLASH             0b00100101
#define MAX7221_STRING_BACKSLASH		 0b00010011
#define MAX7221_STRING_QUOTE_SINGLE      0b00100000    // Upper-right segment (f)
#define MAX7221_STRING_QUOTE_DOUBLE      0b00100010    // Upper-right segment (f)



/** @} */


/**
 * @brief Initialize the display
 * 
 * @param device The SPI-device of the MAX7221-chip
 */
void max7221_start(SpiDevice *device, uint8_t prescaler);

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
void max7221_set_digit_dp(uint8_t digit, uint8_t value);

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
void max7221_clear_digit(uint8_t digit);

/**
 * @brief Prints a unsigned 8-bit number on the specified display.
 * 
 * @param value The number to print
 * @param display The display to print on
 */
void max7221_print_uint8_length(uint8_t value, uint8_t digit, uint8_t length);
void max7221_print_uint8(uint8_t value, uint8_t digit);
void max7221_print_uint8_default(uint8_t value);

/**
 * @brief Prints a signed 8-bit number on the specified display.
 * 
 * @param value The number to print
 * @param display The display to print on
 */
void max7221_print_int8_length(int8_t value, uint8_t digit, uint8_t length);
void max7221_print_int8(int8_t value, uint8_t digit);
void max7221_print_int8_default(int8_t value);

/**
 * @brief Prints a unsigned 16-bit number on both the displays.
 * 
 * @param value The number to print
 */
void max7221_print_uint16_length(uint16_t value, uint8_t digit, uint8_t length);
void max7221_print_uint16(uint16_t value, uint8_t digit);
void max7221_print_uint16_default(uint16_t value);

/**
 * @brief Prints a signed 16-bit number on both the displays.
 * 
 * @param value The number to print
 */
void max7221_print_int16_length(int16_t value, uint8_t digit, uint8_t length);
void max7221_print_int16(int16_t value, uint8_t digit);
void max7221_print_int16_default(int16_t value);


/**
 * @brief Prints a signed 16-bit number on both the displays.
 * 
 * @param value The number to print
 * @param decimal The number of decimal places to display
 */
void max7221_print_float_length(float value, int8_t decimal, uint8_t digit, uint8_t length);
void max7221_print_float(float value, uint8_t decimal);
void max7221_print_float_default(float value);

/**  
 * @brief Sets the display brightness  
 *   
 * @param brightness Value from 1-16 where:  
 *        1 = minimum brightness (1/16 duty cycle)  
 *        16 = maximum brightness (15/16 duty cycle)  
 * @note Values outside 1-16 will be clamped to this range  
 */  
void max7221_set_brightness(uint8_t brightness);

void max7221_set_decode_none();
void max7221_set_decode_all();

void max7221_print_string(const char *value, uint8_t digit);

uint8_t map_digit(uint8_t digit);

void max7221_transfer(uint16_t data);



#endif /* MAX7221_H_ */