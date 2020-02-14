#define __USE_C99_MATH

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "hx711.h"
/*
void HX711_init(uint8_t gain);
int HX711_is_ready();
void HX711_set_gain(uint8_t gain);
int32_t HX711_read();
int32_t HX711_read_average(uint8_t times);
double HX711_get_value(uint8_t times);
float HX711_get_units(uint8_t times);
void HX711_tare(uint8_t times);
void HX711_set_scale(float scale);
float HX711_get_scale();
void HX711_set_offset(int32_t offset);
int32_t HX711_get_offset();
void HX711_power_down();
void HX711_power_up();
uint8_t shiftIn(void);
*/

hx711_gain_t CURRENT_CHANNEL;		        // Current selected channel
const uint8_t AMP_FACTORS[] = {4, 1, 2};    // amplification factor. Needs to match Table 3 from the data sheet.
// double OFFSETS[HX711_GAIN_MAX_OPTIONS];	// used for tare weight
int32_t OFFSETS[HX711_GAIN_MAX_OPTIONS];	// used for tare weight
double SCALES[HX711_GAIN_MAX_OPTIONS];	    // used to return weight in grams, kg, ounces, whatever

void HX711_init(hx711_gain_t gain)
{
	PD_SCK_SET_OUTPUT;
	DOUT_SET_INPUT;

	for (uint8_t i = 0; i < HX711_GAIN_MAX_OPTIONS; i++)
	{
		OFFSETS[i] = 0;
		SCALES[i] = 0.0;
	}

	HX711_set_gain(gain);
}

int HX711_is_ready(void)
{
	return (DOUT_INPUT & (1 << DOUT_PIN)) == 0;
}

void HX711_set_gain(hx711_gain_t gain)
{
	CURRENT_CHANNEL = gain;

	PD_SCK_SET_LOW;

	// Do a reading, but we only really need it to clock the bus a few times to get the channel to change
	HX711_read();
}

uint32_t HX711_read(void)
{
	// wait for the chip to become ready
	while (!HX711_is_ready());

	uint32_t count = 0;
	uint8_t i = 0;
	
	DOUT_SET_HIGH;
	_delay_us(1);
	PD_SCK_SET_LOW;
	_delay_us(1);
	
	// Wait for the pin to be ready again
	while(DOUT_READ);

	// Read in the 24-bit value
	for (i = 0; i < 24; i++)
	{
		PD_SCK_SET_HIGH;
		
		_delay_us(1);
		
		PD_SCK_SET_LOW;
		
		_delay_us(1);
		
		count <<= 1;
		if(DOUT_READ)
			count++;
	}
	
	// Set the gain for the next reading, because that settings is set afterwards??
	// Adding 1 to gain as the lowest gain still needs 1 pulse.
	for (i = 0; i < (CURRENT_CHANNEL + 1); i++)
	{
		PD_SCK_SET_HIGH;
		
		_delay_us(1);

		PD_SCK_SET_LOW;
		
		_delay_us(1);
	}
	
	count >>= 6;

	// count ^= 0x800000;

	// Ultra hack becuase, for some reason, channel A likes to have this weird offset, then rolls over?
	// if ((CURRENT_CHANNEL == HX711_GAIN_128_CH_A ||
	//      CURRENT_CHANNEL == HX711_GAIN_64_CH_A) &&
	// 	 count > 0x3FD00)
	// {
	// 	count -= 0x3FD00;
	// }
	
	// return (count >= 0x00FFFFFF ? 0x0 : count);
	return count;
}

uint32_t HX711_read_average(uint8_t times)
{
	if (times <= 1)
	{
		return HX711_read();
	}

	uint32_t sum = 0;
	for (uint8_t i = 0; i < times; i++)
	{
		sum += HX711_read();
		// TODO: See if yield will work | yield();
	}
	return sum / times;
}

uint32_t HX711_get_value(uint8_t times)
{
	uint32_t avg = HX711_read_average(times);

	// Add some level of protection for when the value read in is actually less than the offset
	return (avg > HX711_get_offset(CURRENT_CHANNEL) ? ((avg -  HX711_get_offset(CURRENT_CHANNEL)) / AMP_FACTORS[CURRENT_CHANNEL]) : 0 );
	// return (avg > HX711_get_offset(CURRENT_CHANNEL) ? (avg -  HX711_get_offset(CURRENT_CHANNEL)) : 0 );
	// return (avg -  HX711_get_offset());
}

double HX711_get_units(uint8_t times)
{
	return HX711_get_value(times) / HX711_get_scale();
}

void HX711_tare(uint8_t times)
{
	HX711_set_offset(CURRENT_CHANNEL, HX711_read_average(times));
}

void HX711_set_scale(hx711_gain_t channel, double scale)
{
	SCALES[channel] = scale;
}

double HX711_get_scale(void)
{
	return SCALES[CURRENT_CHANNEL];
}

void HX711_set_offset(hx711_gain_t channel, int32_t offset)
{
	OFFSETS[channel] = offset;
}

int32_t HX711_get_offset(hx711_gain_t channel)
{
	return OFFSETS[channel];
}

void HX711_power_down(void)
{
	PD_SCK_SET_LOW;
	PD_SCK_SET_HIGH;
	_delay_us(70);
}

void HX711_power_up(void)
{
	PD_SCK_SET_LOW;
}

uint8_t shiftIn(void)
{
	uint8_t value = 0;

	for (uint8_t i = 0; i < 8; ++i)
	{
		PD_SCK_SET_HIGH;
		value |= DOUT_READ << (7 - i);
		PD_SCK_SET_LOW;
	}
	return value;
}