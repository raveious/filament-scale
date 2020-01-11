
set(AVR_MCU_SPEED "8000000UL")
set(AVR_MCU "attiny85")
set(AVR_UPLOAD_SPEED "9600")
set(AVR_PROGRAMMER "jtag2isp")

# AVR Fuses, must be in concordance with your hardware and F_CPU
# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p
set(L_FUSE 0x62)
set(H_FUSE 0xDD)
set(E_FUSE 0xFF)
set(LOCK_BIT 0xFF)

include(${CMAKE_CURRENT_LIST_DIR}/avr-gcc-toolchain.cmake)