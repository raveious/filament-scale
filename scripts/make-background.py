#!/bin/env python3

from PIL import Image
import sys
import os

# Open background image
with Image.open(sys.argv[1]) as img:
    pix = img.load()

    # Assert that the picture must be the size of the screen
    assert(img.size[0] is 128) 
    assert(img.size[1] is 32)

    # Go through all of the pixels in the picture
    data = []
    for i in range(0, img.size[1], 8):
        for j in range(img.size[0]):
            b = 0x00

            # Find all of the non-white pixels, and make them bytes
            for r in range(8):
                b = b | ((not any(pix[j, i + r])) << r)
            
            # Store the background bytes into an array
            data.append(b)

# Make the output directory
if (not os.path.exists(os.path.dirname(sys.argv[2]))):
    os.makedirs(os.path.dirname(sys.argv[2]))

# Write an output header file
with open(sys.argv[2], "w") as output:
    output.write("""// Auto-generated background

#include <avr/pgmspace.h>

const uint8_t background_data [] PROGMEM = {
""")
    for i in range(0, len(data), 8):
        output.write("    0x{:02X}, 0x{:02X}, 0x{:02X}, 0x{:02X}, 0x{:02X}, 0x{:02X}, 0x{:02X}, 0x{:02X},\n".format(
            data[i], data[i + 1], data[i + 2], data[i + 3], data[i + 4], data[i + 5], data[i + 6], data[i + 7]))
    
    output.write("};\n")

