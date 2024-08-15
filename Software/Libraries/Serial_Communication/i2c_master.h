/*
 * i2c_master.h
 *
 * Created: 20.07.2024 15:42:05
 *  Author: FluYVy
 */ 


#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

void i2c_master_init(uint32_t i2c_frequency);
void i2c_master_start();
void i2c_master_stop();
void i2c_master_sendChar(uint8_t data);
uint8_t i2c_master_receiveChar(uint8_t ack);
void i2c_master_sendAddress(uint8_t address, uint8_t rw);

#endif /* I2C_MASTER_H_ */