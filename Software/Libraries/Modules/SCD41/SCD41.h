/*
 * SCD41.h
 *
 * Created: 30.09.2024 15:45:02
 *  Author: Florian Reichart
 */ 


#ifndef SCD41_H_
#define SCD41_H_


#include "i2c_master.h"
#include <util/delay.h>


/**
	Whether the checksum should be checked, when data is received
*/

//#define SCD41_PROCESS_CHECKSUM


/**
	The i2c-address of the SCD41-IC
*/
#define SCD41_ADDRESS 0x62


/************************************************************************/
/* Command definitions                                                   */
/************************************************************************/

// basic commands
#define SCD41_COMMAND_START_PRERIODIC_MEASUREMENT						0x21b1
#define SCD41_COMMAND_READ_MEASUREMENT									0xec05
#define SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT						0x3f86

// On-chip output signal compensation
#define SCD41_COMMAND_SET_TEMPERATURE_OFFSET							0x241d
#define SCD41_COMMAND_GET_TEMPERATURE_OFFSET							0x2318
#define SCD41_COMMAND_SET_SENSOR_ALTITUDE								0x2427
#define SCD41_COMMAND_GET_SENSOR_ALTITUDE								0x2322
#define SCD41_COMMAND_SET_AMBIENT_PRESSURE								0xe000
#define SCD41_COMMAND_GET_AMBIENT_PRESSURE								0xe000

// Field calibration
#define SCD41_COMMAND_PERFORM_FORCED_RECALIBRATION						0x362f
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED			0x2416
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED			0x2313
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_TARGET				0x243a
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_TARGET				0x233f

// Low power
#define SCD41_COMMAND_START_LOW_POWER_PERIODIC_MEASUREMENT				0x21ac
#define SCD41_COMMAND_GET_DATA_READY_STATUS								0xe4b8

// Advanced features
#define SCD41_COMMAND_PERSIST_SETTINGS									0x3615
#define SCD41_COMMAND_GET_SERIAL_NUMBER									0x3682
#define SCD41_COMMAND_PERFORM_SELF_TEST									0x3639
#define SCD41_COMMAND_PERFORM_FACTORY_RESET								0x3632
#define SCD41_COMMAND_REINIT											0x3646
#define SCD41_COMMAND_GET_SENSOR_VARIANT								0x202f

// Single Shot Measurement Mode
#define SCD41_COMMAND_MEASURE_SINGLE_SHOT								0x219d
#define SCD41_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY						0x2196
#define SCD41_COMMAND_POWER_DOWN										0x36e0
#define SCD41_COMMAND_WAKE_UP											0x36f6
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD		0x2445
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD		0x2340
#define SCD41_COMMAND_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD	0x244e
#define SCD41_COMMAND_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD	0x234b



/**
	gets the current state of the sensor
	0 ... standby / not measuring
	1 ... measuring
*/
uint8_t scd41_get_current_state();

uint16_t get_co2();
uint16_t get_temperature();
uint16_t get_humidity();



/**
	start measuring
*/
void start_periodic_measurement();


/**
	read the values from the sensors and returns errors.
	A set bit (1) represents an error of that functionality.
	
	Bits:
	0 ... CO2
	1 ... temperature
	2 ... humidity
*/
uint8_t read_measurement();

/**
	stop measuring
	wait ... whether or not a delay should be added.
			 If not selected: please wait for at least 500ms before communicating with the SCD41-chip again.
*/
void stop_periodic_measurement(uint8_t wait);


void set_temperature_offset(uint16_t offset);
uint16_t get_temperature_offset();

void set_sensor_altitude(uint16_t altitude);
uint16_t get_sensor_altitude();

void set_ambient_pressure(uint16_t pressure);
uint16_t get_ambient_pressure();



int16_t perform_forced_recalibration(uint16_t reference, uint8_t return_correction);

void set_automatic_self_calibration_enabled(uint8_t enabled);
uint8_t get_automatic_self_calibration_enabled();

void set_automatic_self_calibration_target(uint16_t target_CO2);
uint16_t get_automatic_self_calibration_target();



void start_low_power_periodic_measurement();
uint8_t get_data_ready_status();



void persist_settings(uint8_t wait);
uint64_t get_serial_number();
uint8_t perform_self_test(uint8_t wait);
void perform_factory_reset(uint8_t wait);
void reinit(uint8_t wait);
uint8_t get_sensor_variant();



void measure_single_shot(uint8_t wait);
void measure_single_shot_rht_only(uint8_t wait);
void power_down();
void wake_up(uint8_t wait);
void set_automatic_self_calibration_initial_period(uint16_t hours);
uint16_t get_automatic_self_calibration_initial_period();
void set_automatic_self_calibration_standard_period(uint16_t hours);
uint16_t get_automatic_self_calibration_standard_period();





#endif /* SCD41_H_ */