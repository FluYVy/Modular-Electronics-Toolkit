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


uint16_t get_co2()
{
	return last_co2;
}

uint16_t get_temperature()
{
	return last_temperature;
}

uint16_t get_humidity()
{
	return last_humidity;
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




void set_temperature_offset(uint16_t offset)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_SET_TEMPERATURE_OFFSET>>8);
	i2c_master_sendChar(SCD41_COMMAND_SET_TEMPERATURE_OFFSET);
	i2c_master_sendChar(offset>>8);
	i2c_master_sendChar(offset);
	i2c_master_sendChar(generate_checksum(offset));
	i2c_master_stop();
}

uint16_t get_temperature_offset()
{
	uint16_t offset = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_TEMPERATURE_OFFSET>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_TEMPERATURE_OFFSET);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	offset |= i2c_master_receiveChar(0x01)<<8;
	offset |= i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM 
	if (!check_checksum(offset, checksum))
	{
		offset = 0xffff;
	}
	#endif	
	return offset;
}

void set_sensor_altitude(uint16_t altitude)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_SET_SENSOR_ALTITUDE>>8);
	i2c_master_sendChar(SCD41_COMMAND_SET_SENSOR_ALTITUDE);
	i2c_master_sendChar(altitude>>8);
	i2c_master_sendChar(altitude);
	i2c_master_sendChar(generate_checksum(altitude));
	i2c_master_stop();
}

uint16_t get_sensor_altitude()
{
	uint16_t altitude = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_SENSOR_ALTITUDE>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_SENSOR_ALTITUDE);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	altitude |= i2c_master_receiveChar(0x01)<<8;
	altitude |= i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(altitude, checksum))
	{
		altitude = 0xffff;
	}
	#endif
	return altitude;
}

void set_ambient_pressure(uint16_t pressure)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_SET_AMBIENT_PRESSURE>>8);
	i2c_master_sendChar(SCD41_COMMAND_SET_AMBIENT_PRESSURE);
	i2c_master_sendChar(pressure>>8);
	i2c_master_sendChar(pressure);
	i2c_master_sendChar(generate_checksum(pressure));
	i2c_master_stop();
}

uint16_t get_ambient_pressure()
{
	uint16_t pressure = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_AMBIENT_PRESSURE>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_AMBIENT_PRESSURE);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	pressure |= i2c_master_receiveChar(0x01)<<8;
	pressure |= i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(pressure, checksum))
	{
		pressure = 0xffff;
	}
	#endif
	return pressure;
}



int16_t perform_forced_recalibration(uint16_t reference, uint8_t return_correction)
{
	int16_t correction = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_FORCED_RECALIBRATION>>8);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_FORCED_RECALIBRATION);
	i2c_master_sendChar(reference>>8);
	i2c_master_sendChar(reference);
	i2c_master_sendChar(generate_checksum(reference));
	_delay_ms(400);
	
	if(return_correction)
	{
		i2c_master_start();
		i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
		correction |= i2c_master_receiveChar(0x01)<<8;
		correction |= i2c_master_receiveChar(0x01);
		uint8_t checksum = i2c_master_receiveChar(0x00);
		
		#ifdef SCD41_PROCESS_CHECKSUM
		if (!check_checksum(correction, checksum))
		{
			correction = 0x7fff;
		}
		#endif
	}
	
	i2c_master_stop();
	return correction;	
}

void set_automatic_self_calibration_enabled(uint8_t enabled)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED>>8);
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED);
	i2c_master_sendChar(0x00);
	i2c_master_sendChar(enabled);
	i2c_master_sendChar(generate_checksum((uint16_t)enabled));
	i2c_master_stop();
}

uint8_t get_automatic_self_calibration_enabled()
{
	uint8_t enabled = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	i2c_master_receiveChar(0x01);
	enabled = i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
		
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum((uint16_t)enabled, checksum))
	{
		enabled = 0x02;
	}
	#endif
	return enabled;
}


void set_automatic_self_calibration_target(uint16_t target_CO2)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET>>8);
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET);
	i2c_master_sendChar((uint8_t)target_CO2>>8);
	i2c_master_sendChar((uint8_t)target_CO2);
	i2c_master_sendChar(generate_checksum(target_CO2));
	i2c_master_stop();
}

uint16_t get_automatic_self_calibration_target()
{
	uint16_t target = 0;
		
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	target |= i2c_master_receiveChar(0x01)<<8;
	target |= i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
		
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(target, checksum))
	{
		target = 0xffff;
	}
	#endif
	return target;
}



void start_low_power_periodic_measurement()
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT>>8);
	i2c_master_sendChar(SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT);
	i2c_master_stop();
}

uint8_t get_data_ready_status()
{
	uint8_t status = 0;
	uint16_t readout = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	readout |= i2c_master_receiveChar(0x01)<<8;
	readout |= i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	if ((readout & 0x07ff) == 0)
	{
		status = 0;
	}else
	{
		status = 1;
	}
	
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum((uint16_t)readout, checksum))
	{
		status = 0;
	}
	#endif
	
	return status;
}



void persist_settings(uint8_t wait)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_PERSIST_SETTINGS>>8);
	i2c_master_sendChar(SCD41_COMMAND_PERSIST_SETTINGS);
	i2c_master_stop();
	
	if (wait)
	{
		_delay_ms(500);
	}
}

uint64_t get_serial_number()
{
	uint16_t serial_number_word_1 = 0;
	uint16_t serial_number_word_2 = 0;
	uint16_t serial_number_word_3 = 0;
		
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	
	serial_number_word_1 |= i2c_master_receiveChar(0x01)<<8;
	serial_number_word_1 |= i2c_master_receiveChar(0x01);
	uint8_t checksum_word_1 = i2c_master_receiveChar(0x01);
	
	serial_number_word_2 |= i2c_master_receiveChar(0x01)<<8;
	serial_number_word_2 |= i2c_master_receiveChar(0x01);
	uint8_t checksum_word_2 = i2c_master_receiveChar(0x01);
	
	serial_number_word_3 |= i2c_master_receiveChar(0x01)<<8;
	serial_number_word_3 |= i2c_master_receiveChar(0x01);
	uint8_t checksum_word_3 = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!(check_checksum(serial_number_word_1, checksum_word_1) || check_checksum(serial_number_word_2, checksum_word_2) || check_checksum(serial_number_word_3, checksum_word_3)))
	{
		return 0
	}
	#endif
	return (uint64_t)((serial_number_word_1)<<32 | (serial_number_word_2)<<16 | (serial_number_word_3));
}

uint8_t perform_self_test(uint8_t wait)
{
	uint16_t readout = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_SELF_TEST>>8);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_SELF_TEST);
	_delay_ms(10000);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	readout = i2c_master_receiveChar(0x01);
	readout = i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(readout, checksum))
	{
		return 0xff;
	}
	#endif
	return ((uint8_t)readout | (uint8_t)(readout>>8));
}

void perform_factory_reset(uint8_t wait)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_FACTORY_RESET>>8);
	i2c_master_sendChar(SCD41_COMMAND_PERFORM_FACTORY_RESET);
	i2c_master_stop();
	
	if (wait)
	{
		_delay_ms(1200);
	}
}

void reinit(uint8_t wait)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_REINIT>>8);
	i2c_master_sendChar(SCD41_COMMAND_REINIT);
	i2c_master_stop();
	
	if (wait)
	{
		_delay_ms(30);
	}
}

uint8_t get_sensor_variant()
{
	uint16_t readout = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_SENSOR_VARIANT>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_SENSOR_VARIANT);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	readout = i2c_master_receiveChar(0x01);
	readout = i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(readout, checksum))
	{
		return 0xff;
	}
	#endif
	
	return (uint8_t)(readout>>12);
}



void measure_single_shot(uint8_t wait)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT>>8);
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT);
	i2c_master_stop();
	
	if (wait)
	{
		_delay_ms(5000);
	}
}

void measure_single_shot_rht_only(uint8_t wait)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY>>8);
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY);
	i2c_master_stop();
	
	if (wait)
	{
		_delay_ms(50);
	}
}

void power_down()
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT>>8);
	i2c_master_sendChar(SCD41_COMMAND_MEASURE_SINGLE_SHOT);
	i2c_master_stop();
}

void wake_up(uint8_t wait)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_WAKE_UP>>8);
	i2c_master_sendChar(SCD41_COMMAND_WAKE_UP);
	i2c_master_stop();
	
	if (wait)
	{
		_delay_ms(30);
	}
}

void set_automatic_self_calibration_initial_period(uint16_t hours)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD>>8);
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD);
	i2c_master_sendChar(hours>>8);
	i2c_master_sendChar(hours);
	i2c_master_sendChar(generate_checksum(hours));
	i2c_master_stop();
}

uint16_t get_automatic_self_calibration_initial_period()
{
	uint16_t hours = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	hours |= i2c_master_receiveChar(0x01)<<8;
	hours |= i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(hours, checksum))
	{
		hours = 0xffff;
	}
	#endif
	return hours;
}

void set_automatic_self_calibration_standard_period(uint16_t hours)
{
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD>>8);
	i2c_master_sendChar(SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD);
	i2c_master_sendChar(hours>>8);
	i2c_master_sendChar(hours);
	i2c_master_sendChar(generate_checksum(hours));
	i2c_master_stop();
}

uint16_t get_automatic_self_calibration_standard_period()
{
	uint16_t altitude = 0;
	
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x00);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD>>8);
	i2c_master_sendChar(SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD);
	i2c_master_start();
	i2c_master_sendAddress(SCD41_ADDRESS, 0x01);
	altitude |= i2c_master_receiveChar(0x01)<<8;
	altitude |= i2c_master_receiveChar(0x01);
	uint8_t checksum = i2c_master_receiveChar(0x00);
	i2c_master_stop();
	
	#ifdef SCD41_PROCESS_CHECKSUM
	if (!check_checksum(altitude, checksum))
	{
		altitude = 0xffff;
	}
	#endif
	return altitude;
}