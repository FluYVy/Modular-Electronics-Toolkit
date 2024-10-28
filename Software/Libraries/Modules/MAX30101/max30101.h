/*
 * max30101.h
 *
 * Created: 27.10.2024 09:38:15
 *  Author: Florian Reichart
 */ 


#ifndef MAX30101_H_
#define MAX30101_H_

#include "i2c_master.h"

#define MAX30101_I2C_ADDRESS 0x55

void max30101_start_measurements();

void max30101_single_measurement(uint16_t values[], uint8_t length);

void max30101_stop_measurements();



#endif /* MAX30101_H_ */