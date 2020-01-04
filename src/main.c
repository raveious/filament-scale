/**
 * FilamentScale.c
 *
 * Created: 9/13/2019 4:31:23 PM
 * @author : Ian
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "font6x8.h"
#include "ssd1306xled.h"
#include "ssd1306xledtx.h"

#include "num2str.h"

#include "hx711.h"

#define CHANNEL_A HX711_GAIN_128_CH_A
#define CHANNEL_B HX711_GAIN_32_CH_B

#define TARE_SAMPLE_COUNT 10

uint32_t EEMEM left_offset = 0;
uint32_t EEMEM right_offset = 0;

inline void tare(void)
{
	HX711_set_channel(CHANNEL_A);
	HX711_tare(TARE_SAMPLE_COUNT); // Tare A (left)
	eeprom_update_dword(&left_offset, HX711_get_offset(CHANNEL_A));

	HX711_set_channel(CHANNEL_B);
	HX711_tare(TARE_SAMPLE_COUNT); // Tare B (right)
	eeprom_update_dword(&right_offset, HX711_get_offset(CHANNEL_B));
}

int main(void)
{
	uint32_t left_reading = 0;
	uint32_t right_reading = 0;

	// Initialize all of the control registers
	
	DDRB  |= _BV(PB0) | _BV(PB2); // PB0 and PB2 as outputs

	// GIMSK = _BV(PCIE); // Enable global interrupts for PCIE (PB1 in this case)

	eeprom_busy_wait();

	// Couldn't figure out how to get avr-gcc to get me a .eep file to program with avrdude
	if (eeprom_read_dword(&left_offset) == 0xFFFFFFFF)
		eeprom_write_dword(&left_offset, 0x0);
		
	if (eeprom_read_dword(&right_offset) == 0xFFFFFFFF)
		eeprom_write_dword(&right_offset, 0x0);

	_delay_ms(40);	// Small delay might be necessary if ssd1306_init is the first operation in the application.

	// ---- Initialization ----
	ssd1306_init();
	ssd1306tx_init(ssd1306xled_font6x8data, ' ');
	ssd1306_clear();	// Clear the screen.
	
	HX711_init(CHANNEL_A);
	HX711_set_scale(CHANNEL_A, 1.f);
	HX711_set_scale(CHANNEL_B, 1.f);

	// Print the values from EEPROM onto the screen

	// HX711_set_offset(CHANNEL_A, eeprom_read_dword(&left_offset));
	// ssd1306_setpos(18, 1);
	// ssd1306tx_numdec(HX711_get_offset(CHANNEL_A));

	// HX711_set_offset(CHANNEL_B, eeprom_read_dword(&right_offset));
	// ssd1306_setpos(82, 1);
	// ssd1306tx_numdec(HX711_get_offset(CHANNEL_B));

	// Put the software version onto the screen
	ssd1306_setpos(24, 2);
	ssd1306tx_string("Ver: ");
	ssd1306tx_numhex(GIT_COMMIT_HASH);

	_delay_ms(500);

	ssd1306_clear();	// Clear the screen.

	// NOTE: Screen width - 128, that is 21 symbols per row for the 6x8 font.
	for (;;) {
		if ((PINB & _BV(PB1)) != 0)
		{
			ssd1306_setpos(52, 3);
			ssd1306tx_string("TARE");

			tare(); // Tare both channels
			
			// Clear the "TARE" off of the screen
			ssd1306_setpos(52, 3);
			ssd1306tx_string("    ");
		}

		HX711_set_channel(CHANNEL_A);
		left_reading = HX711_get_value(3);
		
		HX711_set_channel(CHANNEL_B);
		right_reading = HX711_get_value(3);

		ssd1306_setpos(0, 1);
		ssd1306tx_string("L:        ");
		ssd1306_setpos(18, 1);
		ssd1306tx_numdec(left_reading);

		ssd1306_setpos(64, 1);
		ssd1306tx_string("R:        ");;
		ssd1306_setpos(82, 1);
		ssd1306tx_numdec(right_reading);

		_delay_ms(10);
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

