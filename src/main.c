/*
 * FilamentScale.c
 *
 * Created: 9/13/2019 4:31:23 PM
 * Author : Ian
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ssd1306xled.h"

//#
//include "hx711.h"


#define TESTING_DELAY 500

extern void ssd1306_start_data(void);	// Initiate transmission of data
extern void ssd1306_data_byte(uint8_t);	// Transmission 1 byte of data
extern void ssd1306_stop(void);			// Finish transmission

int main(void)
{
	// HX711_init(128);
	// HX711_set_scale(1.f);
	// HX711_set_gain(128);
	// HX711_tare(10);
	
	// DDRB =  0xFF;
	// // DDRB |= (1 << PB0); // PB0 is an output
	// PORTB = 0xAA;
	
    // while (1) 
    // {
	// 	// PORTB ^= (1 << PB0); // Toggle PB0
	// 	PORTB ^= 0xFF;
	// 	_delay_ms(250);
    // }

	// ---- Initialization ----
	_delay_ms(40);	// Small delay might be necessary if ssd1306_init is the first operation in the application.
	ssd1306_init();

	// ---- Main Loop ----
	for (;;) {
		ssd1306_clear();	// Clear the screen.

		// ---- Fill out screen with random bytes values ----
		uint16_t r = 100;
		ssd1306_setpos(0, 0);
		ssd1306_start_data();	// Initiate transmission of data
		for (uint16_t i = 0; i < 128 * 8; i++) {
			// rand=(rand*109+89)%251; // Ref: https://www.avrfreaks.net/forum/random-number-generation-0
			r = (r * 109 + 89) % 521; // Generate a pseudo random number. Linear congruential generator
			ssd1306_data_byte(r);
		}
		ssd1306_stop();
		_delay_ms(TESTING_DELAY);

		// ---- Fill out screen with sequential bytes values ----
		ssd1306_setpos(0, 0);
		ssd1306_start_data();	// Initiate transmission of data
		for (uint16_t i = 0; i < 128 * 8; i++) {
			ssd1306_data_byte(i);
		}
		ssd1306_stop();
		_delay_ms(TESTING_DELAY);

		// ---- Fill out screen line by line ----
		uint8_t p = 0xff;
		for (uint8_t i = 0; i < 8; i++) {
			p = (p >> 1);
			ssd1306_fill(~p);
		}
		_delay_ms(TESTING_DELAY);

		// ---- Fill out screen with patters ----
		ssd1306_fill(0xAA);	// Horizontal lines
		_delay_ms(TESTING_DELAY);
		ssd1306_fill2(0XFF, 0x00);	// Vertical lines
		_delay_ms(TESTING_DELAY);
		ssd1306_fill2(0x55, 0xAA);	// Dots
		_delay_ms(TESTING_DELAY);
		ssd1306_fill4(0xCC, 0xCC, 0x33, 0x33);	// Small squares
		_delay_ms(TESTING_DELAY);
		ssd1306_fill4(0xC0, 0x30, 0x0C, 0x03);	// Slashes, sloping lines
		_delay_ms(TESTING_DELAY);
		ssd1306_fill4(0x30, 0xC0, 0x03, 0x0C);	// Slashes, sloping lines
		_delay_ms(TESTING_DELAY);
		ssd1306_fill(0XFF);	// Solid
		_delay_ms(TESTING_DELAY);
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

