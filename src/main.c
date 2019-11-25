/**
 * FilamentScale.c
 *
 * Created: 9/13/2019 4:31:23 PM
 * @author : Ian
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "font6x8.h"
#include "ssd1306xled.h"
#include "ssd1306xledtx.h"

#include "hx711.h"

#define TESTING_DELAY 500

int main(void)
{
	// Initialize all of the control registers
	
	DDRB  |= _BV(PB0) | _BV(PB2); // PB0 and PB2 as outputs
	PORTB |= _BV(PB1);            // Enable internal pull-ups on PB1

	// GIMSK = _BV(PCIE); // Enable global interrupts for PCIE (PB1 in this case)

	HX711_init(128);
	HX711_set_scale(1.f);
	HX711_set_gain(128);
	HX711_tare(10);

	// ---- Initialization ----
	_delay_ms(40);	// Small delay might be necessary if ssd1306_init is the first operation in the application.
	ssd1306_init();
	ssd1306tx_init(ssd1306xled_font6x8data, ' ');

	ssd1306_clear();	// Clear the screen.

	// NOTE: Screen width - 128, that is 21 symbols per row.

	// ---- Print some text on the screen ----
	ssd1306_setpos(2, 0);
	ssd1306tx_string("0123456789ABCDEFGHIJK");
	
	ssd1306_setpos(2, 1);
	ssd1306tx_string("A");
	
	ssd1306_setpos(2, 2);
	ssd1306tx_string("B");
	
	ssd1306_setpos(2, 3);
	ssd1306tx_string("C");

	_delay_ms(TESTING_DELAY);

	for (;;) {
		ssd1306_setpos(30, 2);
		ssd1306tx_numdec((uint16_t)HX711_read());

		_delay_ms(50);
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

