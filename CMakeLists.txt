
cmake_minimum_required(VERSION 2.8.12)

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Put the git values as definitions
add_definitions("-DGIT_COMMIT_HASH=0x${GIT_COMMIT_HASH}")
add_definitions("-DGIT_BRANCH_STRING=\"${GIT_BRANCH}\"")

file(GLOB SRC_FILES "src/*.c") # Load all files in src folder

# Include some headers
include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/scripts)

# Auto-generate the backgrounds
add_subdirectory(scripts)

# Create one target
add_avr_executable(filament-scale ${SRC_FILES})

add_dependencies(filament-scale.elf filament-scale-background)
