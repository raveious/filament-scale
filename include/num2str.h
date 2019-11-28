/**
 * NUM2STR - Functions to handle the conversion of numeric vales to strings.
 *
 * @author	Neven Boyanov
 *
 * This is part of the Tinusaur/TinyAVRLib project.
 *
 * Copyright (c) 2017 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Retain in your source code the link http://tinusaur.org to the Tinusaur project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/tinyavrlib
 *
 */

// ============================================================================

#ifndef NUM2STR_H
#define NUM2STR_H

// ----------------------------------------------------------------------------

#include <stdint.h>

// ----------------------------------------------------------------------------

#define USINT2DECASCII_MAX_DIGITS 10
#define USINT2HEXASCII_MAX_DIGITS 8
#define USINT2BINASCII_MAX_DIGITS 32

// ----------------------------------------------------------------------------

uint8_t usint2decascii(uint32_t, char *);
uint8_t usint2hexascii(uint32_t, char *);
uint8_t usint2binascii(uint32_t, char *);

// ----------------------------------------------------------------------------

#endif

// ============================================================================