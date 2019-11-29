/**
 * SSD1306xLED - Library for the SSD1306 based OLED/PLED 128x64 displays
 *
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/SSD1306xLED project.
 *
 * Copyright (c) 2018 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Retain in your source code the link http://tinusaur.org to the Tinusaur project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 *
 */

// ============================================================================

#include <stdlib.h>
// #include <math.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "num2str.h"

#include "ssd1306xled.h"
#include "ssd1306xledtx.h"

// ----------------------------------------------------------------------------

extern void ssd1306_start_data(void);	// Initiate transmission of data
extern void ssd1306_data_byte(uint8_t);	// Transmission 1 byte of data
extern void ssd1306_stop(void);			// Finish transmission

// ----------------------------------------------------------------------------

const uint8_t *ssd1306tx_font_src;
uint8_t ssd1306tx_font_char_base;

// ----------------------------------------------------------------------------

void ssd1306tx_init(const uint8_t *fron_src, uint8_t char_base) {
	ssd1306tx_font_src = fron_src;
	ssd1306tx_font_char_base = char_base;
}

// ----------------------------------------------------------------------------

void ssd1306tx_char(char ch) {
	uint16_t j = (ch << 2) + (ch << 1) - 192; // Equiv.: j=(ch-32)*6 <== Convert ASCII code to font data index.
	ssd1306_start_data();
	for (uint8_t i = 0; i < 6; i++) {
		ssd1306_data_byte(pgm_read_byte(&ssd1306tx_font_src[j + i]));
	}
	ssd1306_stop();
}

void ssd1306tx_string(char *s) {
	while (*s) {
		ssd1306tx_char(*s++);
	}
}

// ----------------------------------------------------------------------------

char ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS + 1];

void ssd1306tx_numdec(uint32_t num) {
	for (uint8_t i = 0; i <= USINT2DECASCII_MAX_DIGITS; i++)
		ssd1306_numdec_buffer[i] = '\0';

	uint8_t digits = usint2decascii(num, ssd1306_numdec_buffer);
	ssd1306tx_string(ssd1306_numdec_buffer + digits);
}

void ssd1306tx_numhex(uint32_t num) {
	for (uint8_t i = 0; i <= USINT2HEXASCII_MAX_DIGITS; i++)
		ssd1306_numdec_buffer[i] = '\0';

	uint8_t digits = usint2hexascii(num, ssd1306_numdec_buffer);
	ssd1306tx_string(ssd1306_numdec_buffer + digits);
}

void ssd1306tx_double(double num, uint8_t decimal_point)
{
    // Extract integer part 
    int32_t ipart = (int)num;

	// Extract floating part 
	double fpart = num - ipart;
	
	if (ipart >= 0)
	{
		ssd1306tx_char('+');
	}
	else
	{
		ssd1306tx_char('-');
		ipart = -ipart;
	}

	ssd1306tx_numdec((uint32_t)ipart);
  
    // check for display option after point 
    if (decimal_point != 0) {

		ssd1306tx_char('.');

		uint32_t power = 1;
		for (uint8_t i = 0; i < decimal_point; i++)
			power *= 10;		

		fpart *= power;

        ssd1306tx_numdec((uint32_t)fpart);
    }
}

void ssd1306tx_numdecp(uint32_t num) {
	ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS] = '\0';   // Terminate the string.
	for (uint8_t i = 0; i < USINT2DECASCII_MAX_DIGITS; i++)
		ssd1306_numdec_buffer[i] = ' ';

	usint2decascii(num, ssd1306_numdec_buffer);
	ssd1306tx_string(ssd1306_numdec_buffer);
}

void ssd1306tx_numhexp(uint32_t num) {
	ssd1306_numdec_buffer[USINT2HEXASCII_MAX_DIGITS] = '\0';   // Terminate the string.
	for (uint8_t i = 0; i < USINT2HEXASCII_MAX_DIGITS; i++)
		ssd1306_numdec_buffer[i] = '0';

	usint2hexascii(num, ssd1306_numdec_buffer);
	ssd1306tx_string(ssd1306_numdec_buffer);
}

// ============================================================================
