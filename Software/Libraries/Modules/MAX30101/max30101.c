/*
 * max30101.c
 *
 * Created: 27.10.2024 09:37:54
 *  Author: Florian Reichart
 */ 

#include "max30101.h"

void max30101_start_measurements()
{
	//Set output mode
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x10);
	i2c_master_sendChar(0x00);
	i2c_master_sendChar(0x02);	//Algorithm Data
	i2c_master_stop();
	
	//Enable the AGC
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x52);
	i2c_master_sendChar(0x00);
	i2c_master_sendChar(0x01);
	i2c_master_stop();
	
	//Enable the AFE
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x44);
	i2c_master_sendChar(0x03);
	i2c_master_sendChar(0x01);
	i2c_master_stop();
	
	//Enable the Algorithm
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x52);
	i2c_master_sendChar(0x02);
	i2c_master_sendChar(0x01);
	i2c_master_stop();
}

void max30101_single_measurement(uint16_t values[], uint8_t length)
{
	uint8_t received_bytes[9];
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x12);
	i2c_master_sendChar(0x01);
	i2c_master_stop();
	_delay_ms(20);
	
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x01);
	i2c_master_receiveChar(0x01);
		
	for (uint8_t byte = 0; byte < 8; byte++)
	{
		received_bytes[byte] = i2c_master_receiveChar(0x01);
	}
	received_bytes[8] = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	//Transform Output
	switch(length)
	{
		case 3:
			values[2] = (uint16_t)(received_bytes[7]<<8 | received_bytes[8]);
		case 2:
			values[1] = (uint16_t)(received_bytes[3]<<8 | received_bytes[4]);
		case 1:
			values[0] = (uint16_t)(received_bytes[0]<<8 | received_bytes[1]);
			break;
		
		default:
			values[0] = 0x0000;
			values[1] = 0x0000;
			values[2] = 0x0000;
			break;
	}
}

void max30101_stop_measurements()
{
	//Disable the AFE
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x44);
	i2c_master_sendChar(0x03);
	i2c_master_sendChar(0x00);
	i2c_master_stop();
	
	//Disable the Algorithm
	i2c_master_start();
	i2c_master_sendAddress(MAX30101_I2C_ADDRESS, 0x00);
	i2c_master_sendChar(0x52);
	i2c_master_sendChar(0x02);
	i2c_master_sendChar(0x00);
	i2c_master_stop();
}