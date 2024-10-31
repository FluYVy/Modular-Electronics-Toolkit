/*
 * Libraries.c
 *
 * Created: 20.07.2024 15:28:45
 * Author : Florian Reichart
 */ 

#include "i2c_master.h"
#include <util/delay.h>

#define F_CPU 12000000UL

void send_test_data()
{
	i2c_master_start();
	i2c_master_sendAddress(0x55, 0x00);
	i2c_master_sendChar(0x00);
	i2c_master_sendChar(0xaa);
	i2c_master_sendChar(0xff);
	i2c_master_stop();
}

int main(void)
{
	i2c_master_init(80000);
	send_test_data();
	_delay_ms(5000);
	i2c_master_init(100000);
	send_test_data();
	_delay_ms(5000);
	i2c_master_init(300000);
	send_test_data();
	_delay_ms(5000);
	i2c_master_init(400000);
	send_test_data();
	_delay_ms(5000);
	
    while (1) 
    {
		i2c_master_start();
		i2c_master_sendAddress(0x63, 0);
		i2c_master_sendChar(0xab);
		i2c_master_sendChar(0xcd);
		i2c_master_stop();
		_delay_ms(500);
		
		i2c_master_start();
		i2c_master_sendAddress(0x63, 0);
		i2c_master_sendChar(0xaa);
		i2c_master_sendChar(0x55);
		i2c_master_start();
		i2c_master_sendAddress(0x63, 1);
		uint8_t rec_1 = i2c_master_receiveChar(1);
		uint8_t rec_2 = i2c_master_receiveChar(0);
		i2c_master_stop();
		_delay_ms(500);
		
		i2c_master_start();
		i2c_master_sendAddress(0x64, 0);
		i2c_master_sendChar(rec_1);
		i2c_master_sendChar(rec_2);
		i2c_master_stop();
		_delay_ms(500);		
    }
}

