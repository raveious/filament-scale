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
#include "ssd1306xledfx.h"

#include "background.h"

#include "hx711.h"

#define LEFT_CHANNEL  HX711_GAIN_128_CH_A
#define RIGHT_CHANNEL HX711_GAIN_32_CH_B

#define TARE_SAMPLE_COUNT 10

uint32_t EEMEM left_offset = 0;
uint32_t EEMEM right_offset = 0;

inline void tare(void)
{
	HX711_set_channel(LEFT_CHANNEL);
	HX711_tare(TARE_SAMPLE_COUNT); // Tare A (left)
	eeprom_update_dword(&left_offset, HX711_get_offset(LEFT_CHANNEL));

	HX711_set_channel(RIGHT_CHANNEL);
	HX711_tare(TARE_SAMPLE_COUNT); // Tare B (right)
	eeprom_update_dword(&right_offset, HX711_get_offset(RIGHT_CHANNEL));
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
	
	HX711_init(LEFT_CHANNEL);
	HX711_set_scale(LEFT_CHANNEL, 1.663101604278075); // grams
	HX711_set_scale(RIGHT_CHANNEL, 1.70855614973262); // grams

	// Print the values from EEPROM onto the screen

	HX711_set_offset(LEFT_CHANNEL, eeprom_read_dword(&left_offset));
	// ssd1306_setpos(18, 1);
	// ssd1306tx_numdec(HX711_get_offset(LEFT_CHANNEL));

	HX711_set_offset(RIGHT_CHANNEL, eeprom_read_dword(&right_offset));
	// ssd1306_setpos(82, 1);
	// ssd1306tx_numdec(HX711_get_offset(RIGHT_CHANNEL));

	// Put the software version onto the screen
	ssd1306_setpos(24, 2);
	ssd1306tx_string("Ver: ");
	ssd1306tx_numhex(GIT_COMMIT_HASH);

	_delay_ms(500);

	ssd1306_clear();	// Clear the screen.

	ssd1306fx_draw_bmp(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, background_data);

	// NOTE: Screen width - 128, that is 21 symbols per row for the 6x8 font.
	for (;;) {
		if ((PINB & _BV(PB1)) != 0)
		{
			// ssd1306_setpos(55, 3);
			// ssd1306tx_string("TARE");

			tare(); // Tare both channels
			
			// Clear the "TARE" off of the screen
			// ssd1306_setpos(55, 3);
			// ssd1306tx_string("    ");
		}

		HX711_set_channel(LEFT_CHANNEL);
		left_reading = HX711_get_units(3);
		
		HX711_set_channel(RIGHT_CHANNEL);
		right_reading = HX711_get_units(3);

		ssd1306_setpos(18, 2);
		ssd1306tx_string("    ");
		ssd1306_setpos(18, 2);
		ssd1306tx_numdec(left_reading);

		ssd1306_setpos(86, 1);
		ssd1306tx_string("    ");;
		ssd1306_setpos(86, 1);
		ssd1306tx_numdec(right_reading);

		_delay_ms(10);
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

