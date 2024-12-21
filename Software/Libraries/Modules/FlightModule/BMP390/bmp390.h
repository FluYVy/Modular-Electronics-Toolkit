/*
 * bmp390.h
 *
 * Created: 16.12.2024 15:53:42
 *  Author: Florian Reichart
 */ 


#ifndef BMP390_H_
#define BMP390_H_

#include "i2c_master.h"

/**
 * @def BMP390_ADDRESS
 *
 * The I2C address of the BMP390 sensor.
 * @note You should not need to change this value.
 */
#define BMP390_ADDRESS 0x76

/**
 * @def BMP390_REG_PRESSURE_DATA
 *
 * The register address of the pressure data register.
 */
#define BMP390_REG_PRESSURE_DATA    0x04

/**
 * @def BMP390_REG_TEMPERATURE_DATA
 *
 * The register address of the temperature data register.
 */
#define BMP390_REG_TEMPERATURE_DATA 0x07

/**
 * @def BMP390_REG_PWR_CTRL
 *
 * The register address to control the power mode of the sensor.
 */
#define BMP390_REG_PWR_CTRL         0x1B

/**
 * @def BMP390_REG_ODR
 *
 * The register address to set the ODR (Output Data Rate) of the sensor.
 */
#define BMP390_REG_ODR              0x1D

/**
 * @def BMP390_PWR_MODE_NORMAL
 * 
 * Normal mode for the sensor, which allows continuous measurement
 */
#define BMP390_PWR_MODE_NORMAL      0x33

/**
 * @def BMP390_PWR_MODE_FORCED
 * 
 * Forced mode for the sensor, which is useful for one-off measurements and calibration.
 */
#define BMP390_PWR_MODE_FORCED      0x13

/**
 * @def BMP390_PWR_MODE_SLEEP
 * 
 * Sleep mode for the sensor, which is useful if sensor is not in use.
 */
#define BMP390_PWR_MODE_SLEEP       0x00

/**
 * @defgroup BMP390_ODR BMP390 Output Data Rate (ODR) Settings
 * @brief Definitions for configuring the output data rate of the BMP390 sensor.
 *
 * These definitions represent various output data rate (ODR) settings
 * for the BMP390 sensor in Hz. The following options are available:
 * - `BMP390_ODR_200`    (0x00): Output data rate of 200 Hz
 * - `BMP390_ODR_100`    (0x01): Output data rate of 100 Hz
 * - `BMP390_ODR_50`     (0x02): Output data rate of 50 Hz
 * - `BMP390_ODR_25`     (0x03): Output data rate of 25 Hz
 * - `BMP390_ODR_12p5`   (0x04): Output data rate of 12.5 Hz
 * - `BMP390_ODR_6p25`   (0x05): Output data rate of 6.25 Hz
 * - `BMP390_ODR_3p1`    (0x06): Output data rate of 3.1 Hz
 * - `BMP390_ODR_1p5`    (0x07): Output data rate of 1.5 Hz
 * - `BMP390_ODR_0p78`   (0x08): Output data rate of 0.78 Hz
 * - `BMP390_ODR_0p39`   (0x09): Output data rate of 0.39 Hz
 * - `BMP390_ODR_0p2`    (0x0a): Output data rate of 0.2 Hz
 * - `BMP390_ODR_0p1`    (0x0b): Output data rate of 0.1 Hz
 * - `BMP390_ODR_0p05`   (0x0c): Output data rate of 0.05 Hz
 * - `BMP390_ODR_0p02`   (0x0d): Output data rate of 0.02 Hz
 * - `BMP390_ODR_0p01`   (0x0e): Output data rate of 0.01 Hz
 * - `BMP390_ODR_0p006`  (0x0f): Output data rate of 0.006 Hz
 * - `BMP390_ODR_0p003`  (0x10): Output data rate of 0.003 Hz
 * - `BMP390_ODR_0p0015` (0x11): Output data rate of 0.0015 Hz
 *
 * @{
 */
#define BMP390_ODR_200      0x00
#define BMP390_ODR_100      0x01
#define BMP390_ODR_50       0x02
#define BMP390_ODR_25       0x03
#define BMP390_ODR_12p5     0x04
#define BMP390_ODR_6p25     0x05
#define BMP390_ODR_3p1      0x06
#define BMP390_ODR_1p5      0x07
#define BMP390_ODR_0p78     0x08
#define BMP390_ODR_0p39     0x09
#define BMP390_ODR_0p2      0x0a
#define BMP390_ODR_0p1      0x0b
#define BMP390_ODR_0p05     0x0c
#define BMP390_ODR_0p02     0x0d
#define BMP390_ODR_0p01     0x0e
#define BMP390_ODR_0p006    0x0f
#define BMP390_ODR_0p003    0x10
#define BMP390_ODR_0p0015   0x11

/** @} */



/**
 * @brief Starts a single measurement on the BMP390 sensor.
 *
 * This function initiates the measurement process on the BMP390 sensor. The sensor will only measure data once.
 *
 * @note This function or its periodical counterpart (bmp390_start_measurement_periodical()) must be called before reading any sensor data.
 */
void bmp390_start_measurement_single();

/**
 * @brief Starts periodical measurements on the BMP390 sensor.
 *
 * This function initiates the measurement process on the BMP390 sensor. The sensor will measure data depending on the selected prescaler.
 *
 * @param prescaler The prescaler setting to determine the measurement period.
 * 
 * @note This function or its single-shot counterpart (bmp390_start_measurement_single()) must be called before reading any sensor data.
 */
void bmp390_start_measurement_periodical(uint8_t prescaler);  


/**
 * @brief Stops any measurements on the BMP390 sensor.
 * 
 * This function stops the current measurement process on the BMP390 sensor. The measurement will be suspended until a new start measurement command is issued.
 *
 */
void bmp390_stop_measurement();


/**
 * @brief Reads the raw temperature data from the BMP390 sensor.
 *
 * This function reads the raw temperature data from the BMP390 sensor. The result is returned as an unsigned 32-bit integer.
 *
 * @return The raw temperature data measured by the BMP390 sensor.
 */
uint32_t bmp390_read_temperature_raw();

/**
 * @brief Reads the raw pressure data from the BMP390 sensor.
 *
 * This function reads the raw pressure data from the BMP390 sensor. The result is returned as an unsigned 32-bit integer.
 *
 * @return The raw pressure data measured by the BMP390 sensor.
 */
uint32_t bmp390_read_pressure_raw();



#endif /* BMP390_H_ */