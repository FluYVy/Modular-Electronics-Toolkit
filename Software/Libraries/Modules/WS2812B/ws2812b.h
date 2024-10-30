/*
 * ws2812b.h
 *
 * Created: 21.10.2024 16:19:07
 *  Author: Florian Reichart
 */ 


#ifndef WS2812B_H_
#define WS2812B_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/portpins.h>

#define WS2812B_DDR DDRC
#define WS2812B_PORT PORTC
#define WS2812B_PIN PC0

void ws2812b_init();
void ws2812b_send(uint8_t values_rgb[], uint16_t length);


#endif /* WS2812B_H_ */