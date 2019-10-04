
cmake_minimum_required(VERSION 2.8.12)

file(GLOB SRC_FILES "src/*.c") # Load all files in src folder

include_directories(include)

# Create one target
add_avr_executable(filament-scale ${SRC_FILES})
