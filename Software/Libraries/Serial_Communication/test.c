/*
 * Libraries.c
 *
 * Created: 20.07.2024 15:28:45
 * Author : Florian Reichart
 */ 

#include <avr/io.h>
#include "i2c_master.h"
#include <util/delay.h>

#define F_CPU 12000000UL

int main(void)
{
	i2c_master_init(80000);
	_delay_ms(5000);
	i2c_master_init(100000);
	_delay_ms(5000);
	i2c_master_init(300000);
	_delay_ms(5000);
	i2c_master_init(400000);
	_delay_ms(5000);
	
    while (1) 
    {
		i2c_master_start();
		i2c_master_sendAddress(0x63, 1);
		i2c_master_sendChar(0xab);
		i2c_master_sendChar(0xcd);
		i2c_master_sendChar(0xef);
		i2c_master_stop();
		_delay_ms(500);
		
		i2c_master_start();
		i2c_master_sendAddress(0x63, 0);
		i2c_master_sendChar(0xaa);
		i2c_master_start();
		i2c_master_receiveChar(1);
		i2c_master_receiveChar(0);
		i2c_master_stop();
		_delay_ms(500);
    }
}

