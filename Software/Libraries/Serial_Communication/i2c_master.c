/*
 * i2c_master.c
 *
 * Created: 20.07.2024 15:39:04
 *  Author: FluYVy
 */

#include <avr/io.h>
#include <util/twi.h>
#include "i2c_master.h"

void i2c_master_init(uint32_t i2c_frequency)
{
	//Pins
	DDRC &= 0xFC;
	PORTC &= 0xFC;
	
	TWSR &= 0xFC;
	
	//Calculate Prescaler and Baudrate
	if (i2c_frequency <= 83333)
	{
		TWSR |= 3;
		TWBR = ((F_CPU/I2C_FREQUENCY)-16)/(128);
	}else if(i2c_frequency <= 250000)
	{
		TWSR |= 2;
		TWBR = ((F_CPU/I2C_FREQUENCY)-16)/(32);
	}else if(i2c_frequency <= 500000)
	{
		TWSR |= 1;
		TWBR = ((F_CPU/i2c_frequency)-16)/(8);
	}else
	{
		TWBR = ((F_CPU/i2c_frequency)-16)/(2);
	}
	
	//Enable I2C
	TWCR |= (1<<TWEN);
}


void i2c_master_start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}


void i2c_master_stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while (TWCR & (1<<TWSTO));
}


void i2c_master_sendChar(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT))); 
}


uint8_t i2c_master_receiveChar(uint8_t ack)
{
	if (ack)
	{
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	}
	else
	{
		TWCR = (1<<TWINT)|(1<<TWEN);
	}
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}


void i2c_master_sendAddress(uint8_t address, uint8_t rw)
{
	i2c_master_sendChar((address<<1)|(rw & 0x01));
}