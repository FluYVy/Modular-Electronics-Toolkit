/*
 * SCD41.c
 *
 * Created: 30.09.2024 15:44:46
 *  Author: Florian Reichart
 */ 

#include "SCD41.h"

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

uint16_t last_co2 = 0;
uint16_t last_temperature = 0;
uint16_t last_humidity = 0;
uint8_t current_state = 0;

/** returns the current state of the sensor
	
	0 ... standby
	1 ... running
*/

uint8_t scd41_get_current_state()
{
	return current_state;
}

void start_periodic_measurement(){
	i2c_master_start();							//start i2c
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00); //send i2c address
	i2c_master_sendChar(SCD41_COMMAND_START_PRERIODIC_MEASUREMENT>>8);	//send upper command
	i2c_master_sendChar(SCD41_COMMAND_START_PRERIODIC_MEASUREMENT);		//send lower command
	i2c_master_stop();
	
	current_state = 1;
}

uint8_t read_measurement()
{
	uint8_t errors = 0x00;
	
	#ifdef SCD41_PROCESS_CHECKSUM
	// Define values for 
	uint16_t co2 = 0;
	uint16_t temperature = 0;
	uint16_t humidity = 0;
	
	uint8_t co2_checksum = 0;
	uint8_t temperature_checksum = 0;
	uint8_t humidity_checksum = 0;
	#endif
	
	i2c_master_start();							//start i2c
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00); //send i2c address and write
	i2c_master_sendChar(SCD41_COMMAND_READ_MEASUREMENT>>8);	//send upper command
	i2c_master_sendChar(SCD41_COMMAND_READ_MEASUREMENT);		//send lower command
	i2c_master_start();											//repeated start
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);				//send i2c address and read
	
	#ifdef SCD41_PROCESS_CHECKSUM
	co2 = i2c_master_receiveChar(0x01)<<8;
	co2 |= i2c_master_receiveChar(0x01);
	co2_checksum = i2c_master_receiveChar(0x01);
	temperature = i2c_master_receiveChar(0x01)<<8;
	temperature |= i2c_master_receiveChar(0x01);
	temperature_checksum = i2c_master_receiveChar(0x01);
	humidity = i2c_master_receiveChar(0x01)<<8;
	humidity |= i2c_master_receiveChar(0x01);
	humidity_checksum = i2c_master_receiveChar(0x00);
	#else
	last_co2 = i2c_master_receiveChar(0x01)<<8;
	last_co2 |= i2c_master_receiveChar(0x01);
	i2c_master_receiveChar(0x01);
	last_temperature = i2c_master_receiveChar(0x01)<<8;
	last_temperature |= i2c_master_receiveChar(0x01);
	i2c_master_receiveChar(0x01);
	last_humidity = i2c_master_receiveChar(0x01)<<8;
	last_humidity |= i2c_master_receiveChar(0x01);
	i2c_master_receiveChar(0x00);
	#endif
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	errors = 0x07;
	
	last_co2 = 0;
	last_temperature = 0;
	last_humidity = 0;
	
	if(check_checksum(co2, co2_checksum)){
		last_co2 = co2;
		errors &= (0x01);
	}
	
	if(check_checksum(temperature, temperature_checksum)){
		last_temperature = temperature;
		errors &= (0x02);
	}
	
	if(check_checksum(humidity, humidity_checksum)){
		last_humidity = humidity;
		errors &= (0x04);
	}
	#endif
	
}

void stop_periodic_measurement(uint8_t wait)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT>>8);
	i2c_master_sendChar(SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT);
	i2c_master_stop();
	
	if (wait)
	{
		_delay_ms(500);
	}
	
	current_state = 0;
}


uint8_t generate_checksum(uint16_t value)
{
	//based on: https://stackoverflow.com/a/51773839
	uint8_t crc = 0xff;
	
	//first round
	crc ^= value>>8; //get High-Byte of value
	for (uint8_t i = 0; i < 2; i++)
	{
		crc ^= (uint8_t)(value>>8*(1-i));
		for (uint8_t j = 0; j < 8; j++)
		{
			if ((crc & 0x80) != 0)
			{
				crc = (uint8_t)((crc << 1) ^ 0x31);
			}
			else
			{
				crc <= 1;
			}
		}
	}
	
	return crc;
}

uint8_t check_checksum(uint16_t value, uint8_t checksum)
{
	return (uint8_t)(generate_checksum(value) == checksum);
}