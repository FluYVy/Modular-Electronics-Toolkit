/*
 * SCD41.c
 *
 * Created: 08.10.2024 08:31:48
 * Author : Florian Reichart
 */ 

#include <avr/io.h>
#include <avr/portpins.h>
#include <util/delay.h>

#include "SCD41.h"

#define F_CPU 12000000UL

int main(void)
{
	i2c_master_init(400000);
	DDRC = 0xff;
	scd41_start_periodic_measurement();
	
    while (1) 
    {
		scd41_read_measurement();
		
		PORTC = scd41_get_co2()>>8;
		_delay_ms(1000);
		PORTC = scd41_get_co2();
		_delay_ms(1000);
    }
}

