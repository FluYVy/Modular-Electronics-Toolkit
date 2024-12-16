/*
 * icm20948.h
 *
 * Created: 02.12.2024 13:42:26
 *  Author: Florian Reichart
 */ 


#ifndef ICM20948_H_
#define ICM20948_H_

#include "i2c_master.h"

/**
 * @def ICM20948_I2C_ADDRESS
 *
 * This value is used to communicate with the ICM20948 over an I2C bus.
 */
#define ICM20948_I2C_ADDRESS            0x68

/**
 * @def ICM20948_AK09916_I2C_ADDRESS
 *
 * This value is used to communicate with the AK09916 within the ICM20948-IC over an I2C bus.
 */
#define ICM20948_AK09916_I2C_ADDRESS    0x0C

/**
 * @def ICM20948_REG_ACCEL_XOUT_H
 *
 * This value is used to read the high byte of the accelerometer data for the x-axis.
 */
#define ICM20948_REG_ACCEL_XOUT_H       0x2D

/**
 * @def ICM20948_REG_ACCEL_XOUT_L
 *
 * This value is used to read the low byte of the accelerometer data for the x-axis.
 */
#define ICM20948_REG_ACCEL_XOUT_L       0x2E

/**
 * @def ICM20948_REG_ACCEL_YOUT_H
 *
 * This value is used to read the high byte of the accelerometer data for the y-axis.
 */
#define ICM20948_REG_ACCEL_YOUT_H       0x2F

 /**
  * @def ICM20948_REG_ACCEL_YOUT_L
  *
 * This value is used to read the low byte of the accelerometer data for the y-axis.
 */
#define ICM20948_REG_ACCEL_YOUT_L       0x30

/**
 * @def ICM20948_REG_ACCEL_ZOUT_H
 *
 * This value is used to read the high byte of the accelerometer data for the z-axis.
 */
#define ICM20948_REG_ACCEL_ZOUT_H       0x31

/**
 * @def ICM20948_REG_ACCEL_ZOUT_L
 *
 * This value is used to read the low byte of the accelerometer data for the z-axis.
 */
#define ICM20948_REG_ACCEL_ZOUT_L       0x32


/**
 * @def ICM20948_REG_GYRO_XOUT_H
 *
 * This value is used to read the high byte of the gyro data for the X-axis.
 */
#define ICM20948_REG_GYRO_XOUT_H        0x33

/**
 * @def ICM20948_REG_GYRO_XOUT_L
 *
 * This value is used to read the low byte of the gyro data for the X-axis.
 */
#define ICM20948_REG_GYRO_XOUT_L        0x34

/**
 * @def ICM20948_REG_GYRO_YOUT_H
 *
 * This value is used to read the high byte of the gyro data for the Y-axis.
 */
#define ICM20948_REG_GYRO_YOUT_H        0x35

/**
 * @def ICM20948_REG_GYRO_YOUT_L
 *
 * This value is used to read the low byte of the gyro data for the Y-axis.
 */
#define ICM20948_REG_GYRO_YOUT_L        0x36

/**
 * @def ICM20948_REG_GYRO_ZOUT_H
 *
 * This value is used to read the high byte of the gyro data for the Z-axis.
 */
#define ICM20948_REG_GYRO_ZOUT_H        0x37

/**
 * @def ICM20948_REG_GYRO_ZOUT_L
 *
 * This value is used to read the low byte of the gyro data for the Z-axis.
 */
#define ICM20948_REG_GYRO_ZOUT_L        0x38


/**
 * @def ICM20948_REG_TEMP_OUT_H
 *
 * This value is used to read the high byte bit of the temperature data.
 */
#define ICM20948_REG_TEMP_OUT_H         0x39

/**
 * @def ICM20948_REG_TEMP_OUT_L
 *
 * This value is used to read the low byte of the temperature data.
 */
#define ICM20948_REG_TEMP_OUT_L         0x3A

/**
 * @def ICM20948_AK09916_REG_MAG_XOUT_H
 * 
 * This value is used to read the high byte of the magnetometer data for the X-axis
 */
#define ICM20948_AK09916_REG_MAG_XOUT_H 0x11

/**
 * @def ICM20948_AK09916_REG_MAG_XOUT_L
 * 
 * This value is used to read the low byte of the magnetometer data for the X-axis
 */
#define ICM20948_AK09916_REG_MAG_XOUT_L 0x12

/**
 * @def ICM20948_AK09916_REG_MAG_YOUT_H
 * 
 * This value is used to read the high byte of the magnetometer data for the Y-axis
 */
#define ICM20948_AK09916_REG_MAG_YOUT_H 0x13

/**
 * @def ICM20948_AK09916_REG_MAG_YOUT_L
 * 
 * This value is used to read the low byte of the magnetometer data for the Y-axis
 */
#define ICM20948_AK09916_REG_MAG_YOUT_L 0x14

/**
 * @def ICM20948_AK09916_REG_MAG_ZOUT_H
 * 
 * This value is used to read the high byte of the magnetometer data for the Z-axis
 */
#define ICM20948_AK09916_REG_MAG_ZOUT_H 0x15

/**
 * @def ICM20948_AK09916_REG_MAG_ZOUT_L
 * 
 * This value is used to read the low byte of the magnetometer data for the Z-axis
 */
#define ICM20948_AK09916_REG_MAG_ZOUT_L 0x16

/**
 * @def ICM20948_REG_BANK_SEL
 * 
 * This value is used to set the current register bank
 */
#define ICM20948_REG_BANK_SEL           0x7F


/**
 * @def ICM20948_REG_XA_OFFS_H
 * 
 * This value is used to set the high byte of the accelerometer offset for the X-axis
 */
#define ICM20948_REG_XA_OFFS_H          0x20

/**
 * @def ICM20948_REG_XA_OFFS_L
 * 
 * This value is used to set the low byte of the accelerometer offset for the X-axis
 */
#define ICM20948_REG_XA_OFFS_L          0x21

/**
 * @def ICM20948_REG_YA_OFFS_H
 * 
 * This value is used to set the high byte of the accelerometer offset for the Y-axis
 */
#define ICM20948_REG_YA_OFFS_H          0x23

/**
 * @def ICM20948_REG_YA_OFFS_L
 * 
 * This value is used to set the low byte of the accelerometer offset for the Y-axis
 */
#define ICM20948_REG_YA_OFFS_L          0x24

/**
 * @def ICM20948_REG_ZA_OFFS_H
 * 
 * This value is used to set the high byte of the accelerometer offset for the Z-axis
 */
#define ICM20948_REG_ZA_OFFS_H          0x26

/**
 * @def ICM20948_REG_ZA_OFFS_L
 * 
 * This value is used to set the low byte of the accelerometer offset for the Z-axis
 */
#define ICM20948_REG_ZA_OFFS_L          0x27



/**
 * @def ICM20948_REG_XG_OFFS_H
 * 
 * This value is used to set the high byte of the gyro offset for the X-axis
 */
#define ICM20948_REG_XG_OFFS_H          0x03

/**
 * @def ICM20948_REG_XG_OFFS_L
 * 
 * This value is used to set the low byte of the gyro offset for the X-axis
 */
#define ICM20948_REG_XG_OFFS_L          0x04

/**
 * @def ICM20948_REG_YG_OFFS_H
 * 
 * This value is used to set the high byte of the gyro offset for the Y-axis
 */
#define ICM20948_REG_YG_OFFS_H          0x05

/**
 * @def ICM20948_REG_YG_OFFS_L
 * 
 * This value is used to set the low byte of the gyro offset for the Y-axis
 */
#define ICM20948_REG_YG_OFFS_L          0x06

/**
 * @def ICM20948_REG_ZG_OFFS_H
 * 
 * This value is used to set the high byte of the gyro offset for the Z-axis
 */
#define ICM20948_REG_ZG_OFFS_H          0x07

/**
 * @def ICM20948_REG_ZG_OFFS_L
 * 
 * This value is used to set the low byte of the gyro offset for the Z-axis
 */
#define ICM20948_REG_ZG_OFFS_L          0x08


/**
 * @brief Get the raw x-axis acceleration value from the accelerometer sensor.
 * This function returns the raw 16-bit value of the x-axis accelerometer sensor, which can then be converted to a float value using the `icm20948_raw_to_float` function.
 *
 * @return The raw x-axis acceleration value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_accelerometer_x_raw();

/**
 * @brief Get the raw y-axis acceleration value from the accelerometer sensor.
 * This function returns the raw 16-bit value of the y-axis accelerometer sensor, which can then be converted to a float value using the `icm20948_raw_to_float` function.
 *
 * @return The raw y-axis acceleration value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_accelerometer_y_raw();

/**
 * @brief Get the raw z-axis acceleration value from the accelerometer sensor.
 * This function returns the raw 16-bit value of the z-axis accelerometer sensor, which can then be converted to a float value using the `icm20948_raw_to_float` function.
 *
 * @return The raw z-axis acceleration value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_accelerometer_z_raw();


/**
 * @brief Get the raw x-axis gyroscope value from the gyroscope sensor.
 *
 * @return The raw x-axis gyroscope value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_gyro_x_raw();

/**
 * @brief Get the raw y-axis gyroscope value from the gyroscope sensor.
 *
 * @return The raw y-axis gyroscope value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_gyro_y_raw();

/**
 * @brief Get the raw z-axis gyroscope value from the gyroscope sensor.
 *
 * @return The raw z-axis gyroscope value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_gyro_z_raw();


/**
 * @brief Get the raw x-axis magnetometer value from the magnetometer sensor.
 *
 * @return The raw x-axis magnetometer value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_magnetometer_x_raw();

/**
 * @brief Get the raw y-axis magnetometer value from the magnetometer sensor.
 *
 * @return The raw y-axis magnetometer value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_magnetometer_y_raw();

/**
 * @brief Get the raw z-axis magnetometer value from the magnetometer sensor.
 *
 * @return The raw z-axis magnetometer value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_magnetometer_z_raw();


//TODO: rework datatypes if needed
/**
 * @brief Get the temperature value from the temperature sensor.
 * @return The temperature value in Celsius.
 */
float icm20948_get_temperature();

//TODO: rework datatypes if needed
/**
 * @brief Get the raw temperature value from the temperature sensor.
 * @return The raw temperature value as a 16-bit unsigned integer.
 */
uint16_t icm20948_get_temperature_raw();

#endif /* ICM20948_H_ */