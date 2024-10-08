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
/* Commanddefinitions                                                   */
/************************************************************************/
#define SCD41_COMMAND_START_PRERIODIC_MEASUREMENT	0x21b1
#define SCD41_COMMAND_READ_MEASUREMENT				0xec05
#define SCD41_COMMAND_STOP_PRERIODIC_MEASUREMENT	0x3f86




/**
	gets the current state of the sensor
	0 ... standby / not measuring
	1 ... measuring
*/
uint8_t scd41_get_current_state();


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



#endif /* SCD41_H_ */