/*
 * bmp390.c
 *
 * Created: 16.12.2024 15:53:32
 *  Author: Florian Reichart
 */ 

#include "bmp390.h"

void bmp390_start_measurement_single()
{
    i2c_master_start();
    i2c_master_sendAddress(BMP390_ADDRESS, 0x00);
    i2c_master_sendChar(BMP390_REG_PWR_CTRL);
    i2c_master_sendChar(BMP390_PWR_MODE_FORCED);
    i2c_master_stop();
}


void bmp390_start_measurement_periodical(uint8_t prescaler)
{
    i2c_master_start();
    i2c_master_sendAddress(BMP390_ADDRESS, 0x00);
    i2c_master_sendChar(BMP390_REG_ODR);
    i2c_master_sendChar(prescaler & 0x1F);
    i2c_master_stop();

    i2c_master_start();
    i2c_master_sendAddress(BMP390_ADDRESS, 0x00);
    i2c_master_sendChar(BMP390_REG_PWR_CTRL);
    i2c_master_sendChar(BMP390_PWR_MODE_NORMAL);
    i2c_master_stop();
}


void bmp390_stop_measurement()
{
    i2c_master_start();
    i2c_master_sendAddress(BMP390_ADDRESS, 0x00);
    i2c_master_sendChar(BMP390_REG_PWR_CTRL);
    i2c_master_sendChar(BMP390_PWR_MODE_SLEEP);
    i2c_master_stop();
}


uint32_t bmp390_read_temperature_raw()
{
    i2c_master_start();
    i2c_master_sendAddress(BMP390_ADDRESS, 0x00);
    i2c_master_sendChar(BMP390_REG_TEMPERATURE_DATA);
    i2c_master_start();
    i2c_master_sendAddress(BMP390_ADDRESS, 0x01);
    uint8_t data1 = i2c_master_receiveChar(0x01);
    uint8_t data2 = i2c_master_receiveChar(0x01);
    uint8_t data3 = i2c_master_receiveChar(0x00);
    i2c_master_stop();

    return (uint32_t)((data3 << 16) | (data2 << 8) | data1);
}


uint32_t bmp390_read_pressure_raw()
{
    i2c_master_start();
    i2c_master_sendAddress(BMP390_ADDRESS, 0x00);
    i2c_master_sendChar(BMP390_REG_PRESSURE_DATA);
    i2c_master_start();
    i2c_master_sendAddress(BMP390_ADDRESS, 0x01);
    uint8_t data1 = i2c_master_receiveChar(0x01);
    uint8_t data2 = i2c_master_receiveChar(0x01);
    uint8_t data3 = i2c_master_receiveChar(0x00);
    i2c_master_stop();

    return (uint32_t)((data3 << 16) | (data2 << 8) | data1);
}
