/*
 * FilamentScale.c
 *
 * Created: 9/13/2019 4:31:23 PM
 * Author : Ian
 */ 

#include <avr/io.h>
#include <util/delay.h>

//#
//include "hx711.h"

int main(void)
{
	// HX711_init(128);
	// HX711_set_scale(1.f);
	// HX711_set_gain(128);
	// HX711_tare(10);
	
	DDRB =  0xFF;
	// DDRB |= (1 << PB0); // PB0 is an output
	PORTB = 0xAA;
	
    while (1) 
    {
		// PORTB ^= (1 << PB0); // Toggle PB0
		PORTB ^= 0xFF;
		_delay_ms(250);
    }
}

