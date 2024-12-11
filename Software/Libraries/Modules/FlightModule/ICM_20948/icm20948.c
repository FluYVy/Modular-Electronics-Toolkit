/*
 * icm20948.c
 *
 * Created: 02.12.2024 13:42:08
 *  Author: Florian Reichart
 */
/*
 * icm20948.c
 *
 * Created: 02.12.2024 13:42:08
 *  Author: Florian Reichart
 */

#include "icm20948.h"

uint16_t icm20948_get_accelerometer_x_raw()
{
    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_REG_ACCEL_XOUT_H);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}

uint16_t icm20948_get_accelerometer_y_raw()
{
    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_REG_ACCEL_YOUT_H);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}

uint16_t icm20948_get_accelerometer_z_raw()
{
    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_REG_ACCEL_ZOUT_H);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}


uint16_t icm20948_get_gyro_x_raw()
{
    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_REG_GYRO_XOUT_H);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}

uint16_t icm20948_get_gyro_y_raw()
{
    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_REG_GYRO_YOUT_H);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}

uint16_t icm20948_get_gyro_z_raw()
{
    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_REG_GYRO_ZOUT_H);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}


uint16_t icm20948_get_magnetometer_x_raw()
{
    //TEST: If AK09916 can print out multiple values continuously

    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_AK09916_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_AK09916_REG_MAG_XOUT_L);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_AK09916_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}

uint16_t icm20948_get_magnetometer_y_raw()
{
    //TEST: If AK09916 can print out multiple values continuously

    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_AK09916_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_AK09916_REG_MAG_YOUT_L);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_AK09916_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}

uint16_t icm20948_get_magnetometer_z_raw()
{
    //TEST: If AK09916 can print out multiple values continuously

    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_AK09916_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_AK09916_REG_MAG_ZOUT_L);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_AK09916_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}


//TODO: rework datatypes if needed
float icm20948_get_temperature()
{
    //TEST: If correct formula for temperature is used

    return (icm20948_get_temperature_raw())/333.87;
}

//TODO: rework datatypes if needed
uint16_t icm20948_get_temperature_raw()
{
    //TEST: If AK09916 can print out multiple values continuously

    uint16_t data = 0;

    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x00);
    i2c_master_sendChar(ICM20948_REG_TEMP_OUT_H);
    i2c_master_start();
    i2c_master_sendAddress(ICM20948_I2C_ADDRESS, 0x01);
    data = (i2c_master_receiveChar(0x01)<<8);
    data |= i2c_master_receiveChar(0x00);
    i2c_master_stop();
    return data;
}
